#include "domain/DeptRepository.h"

#include <drogon/HttpAppFramework.h>
#include <drogon/orm/CoroMapper.h>
#include "SqlGenerator/src/SqlGenerator.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<uint32_t> DeptRepository::getMaxSubDeptOrder(
    const optional<uint32_t> parentId) const
{
    ParamList paramList;
    if (parentId)
    {
        paramList = {{"parent_id", static_cast<int>(*parentId)}};
    }

    const auto sql =
        sqlGenerator()->getSql("get_max_sub_dept_order", paramList);

    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    co_return dbResult[0][0].as<uint32_t>();
}

Task<> DeptRepository::save(const Dept &dept) const
{
    const auto sysDept = static_cast<SysDept>(dept);

    auto mapper = deptMapper();

    switch (dept.getChangingStatus())
    {
        case ChangingStatus::NEW:
            co_await mapper.insert(sysDept);
            break;
        case ChangingStatus::DELETED:  // 软删除也是更新
        case ChangingStatus::UPDATED:
            co_await mapper.update(sysDept);
            break;
        case ChangingStatus::UNCHANGED:
            LOG_WARN << "无需修改的数据调用了save，请检查代码逻辑";
            break;
    }
}

Task<Dept> DeptRepository::getById(const uint32_t deptId) const
{
    auto mapper = deptMapper();

    Criteria criteria{SysDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysDept::Cols::_dept_id, deptId};

    const auto sysDept = co_await mapper.findOne(criteria);
    co_return static_cast<Dept>(sysDept);
}

Task<uint32_t> DeptRepository::countNameByParentId(
    const string &name,
    const optional<uint32_t> &parentId) const
{
    auto mapper = deptMapper();

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

    co_return co_await mapper.count(criteria);
}

inline SqlGenerator *DeptRepository::sqlGenerator()
{
    static const auto sqlGenerator_ = app().getPlugin<SqlGenerator>();
    return sqlGenerator_;
}

inline DbClientPtr DeptRepository::dbClient()
{
    static const drogon::orm::DbClientPtr dbClient_ = app().getDbClient();
    return dbClient_;
}

inline CoroMapper<SysDept> DeptRepository::deptMapper() const
{
    return CoroMapper<SysDept>{dbClient()};
}
