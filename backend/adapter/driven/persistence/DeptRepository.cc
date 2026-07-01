#include "domain/organization/dept/DeptRepository.h"

#include "domain/models/SysDept.h"
#include "SqlGenerator/src/SqlGenerator.h"
#include "common/util/rangesUtils.hpp"
#include <drogon/orm/CoroMapper.h>
#include <drogon/orm/Criteria.h>
#include <drogon/HttpAppFramework.h>
#include <optional>
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl;
using namespace tl::sql;
using namespace drogon_admin;

Task<std::optional<std::int32_t>> DeptRepository::getMaxSubDeptSortNum(
    const optional<std::int32_t> parentId,
    const DbClientPtr &dbClient) const
{
    ParamList paramList;
    if (parentId)
    {
        paramList = {{"parent_id", static_cast<int>(*parentId)}};
    }

    const auto sql =
        sqlGenerator()->getSql("get_max_sub_dept_sort_num", paramList);

    const auto dbResult = co_await dbClient->execSqlCoro(sql);
    if (dbResult[0][0].isNull())
    {
        co_return std::nullopt;
    }
    co_return dbResult[0][0].as<std::int32_t>();
}

Task<> DeptRepository::save(const Dept &dept, const DbClientPtr &dbClient) const
{
    const auto sysDept = static_cast<SysDept>(dept);

    auto mapper = deptMapper(dbClient);

    switch (dept.changingStatus())
    {
        case ChangingStatus::NEW:
            co_await mapper.insert(sysDept);
            break;
        case ChangingStatus::DELETED:  // 软删除也是更新
        case ChangingStatus::UPDATED:
            co_await mapper.update(sysDept);
            break;
        case ChangingStatus::UNCHANGED:
            break;
    }
}

Task<optional<Dept>> DeptRepository::getById(const std::int32_t deptId) const
{
    Criteria criteria{SysDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysDept::Cols::_dept_id, deptId};

    try
    {
        const auto sysDept = co_await deptMapper().findOne(criteria);
        co_return static_cast<Dept>(sysDept);
    }
    catch (const orm::UnexpectedRows & /*ignore*/)
    {
        co_return nullopt;
    }
}

Task<std::int32_t> DeptRepository::countNameByParentId(
    const string &name,
    const optional<std::int32_t> &parentId) const
{
    Criteria criteria{SysDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysDept::Cols::_name, name};
    if (parentId)
    {
        criteria = criteria && Criteria{SysDept::Cols::_parent_id, parentId};
    }
    else
    {
        criteria = criteria &&
                   Criteria{SysDept::Cols::_parent_id, CompareOperator::IsNull};
    }

    co_return co_await deptMapper().count(criteria);
}

Task<size_t> DeptRepository::countSubDept(const std::int32_t deptId) const
{
    Criteria criteria{SysDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysDept::Cols::_parent_id, deptId};

    co_return co_await deptMapper().count(criteria);
}

Task<vector<Dept>> DeptRepository::getByIds(
    const vector<std::int32_t> &idsVector) const
{
    Criteria criteria{SysDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria =
        criteria &&
        Criteria{SysDept::Cols::_dept_id, CompareOperator::In, idsVector};
    const auto deptInDb = co_await deptMapper().findBy(criteria);
    co_return deptInDb |
        views::transform([](const auto &data) { return Dept{data}; }) |
        ranges_utils::to<vector>();
}

Task<vector<Dept>> DeptRepository::getByParentId(
    const optional<std::int32_t> &parentId) const
{
    Criteria criteria{SysDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria =
        criteria && (parentId ? Criteria{SysDept::Cols::_parent_id, *parentId}
                              : Criteria{SysDept::Cols::_parent_id,
                                         CompareOperator::IsNull});
    const auto deptInDb = co_await deptMapper().findBy(criteria);
    co_return deptInDb |
        views::transform([](const auto &data) { return Dept{data}; }) |
        ranges_utils::to<vector>();
}

Task<> DeptRepository::multiSave(const vector<Dept> &depts,
                                 const drogon::orm::DbClientPtr &dbClient) const
{
    // 暂只考虑批量更新
    vector<SysDept> toUpdate;
    toUpdate.reserve(depts.size());
    for (const auto &dept : depts)
    {
        if (dept.changingStatus() == ChangingStatus::UPDATED)
        {
            toUpdate.push_back(static_cast<SysDept>(dept));
        }
    }
    if (toUpdate.size() == 0)
    {
        LOG_WARN << "multiSave被调用，但没有需要更新的数据，请检查代码逻辑";
        co_return;
    }
    Json::Value dataList;

    for (const auto &dept : toUpdate)
    {
        Json::Value item;
        item = dept.toJson();
        // SqlGenerator 太垃圾了
        for (const auto &key : item.getMemberNames())
        {
            if (item[key].isNull())
            {
                item.removeMember(key);
            }
        }
        dataList.append(item);
    }

    const auto sql = sqlGenerator()->getSql("multi_update_dept",
                                            ParamList{{"data_list", dataList}});

    co_await dbClient->execSqlCoro(sql);
}

inline SqlGenerator *DeptRepository::sqlGenerator()
{
    static const auto sqlGenerator_ = app().getPlugin<SqlGenerator>();
    return sqlGenerator_;
}

inline CoroMapper<SysDept> DeptRepository::deptMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysDept>{dbClient};
}
