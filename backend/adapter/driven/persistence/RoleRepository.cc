#include "domain/authorization/RoleRepository.h"

#include <drogon/HttpAppFramework.h>
#include "domain/authorization/RoleDeptRelationType.hpp"

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<size_t> RoleRepository::countBelongDept(const int32_t deptId) const
{
    Criteria criteria{SysRoleDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysRoleDept::Cols::_dept_id, deptId};
    criteria = criteria &&
               Criteria{SysRoleDept::Cols::_relation_type,
                        static_cast<int16_t>(RoleDeptRelationType::BELONG)};

    co_return co_await roleDeptMapper().count(criteria);
}

Task<vector<RoleDept>> RoleRepository::getExcludingDeptByDeptId(
    const std::int32_t deptId) const
{
    Criteria criteria{SysRoleDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysRoleDept::Cols::_dept_id, deptId};
    criteria = criteria &&
               Criteria{SysRoleDept::Cols::_relation_type,
                        static_cast<int16_t>(RoleDeptRelationType::EXCLUDE)};
    const auto sysRoleDeptList = co_await roleDeptMapper().findBy(criteria);
    co_return buildRoleDeptList(sysRoleDeptList);
}

Task<> RoleRepository::saveRoleDepts(const vector<RoleDept> &roleDeptList) const
{
    vector<SysRoleDept> newData;
    vector<SysRoleDept> toUpdate;
    vector<SysRoleDept> toDelete;

    for (const auto &roleDept : roleDeptList)
    {
        const auto status = roleDept.getChangingStatus();
        const auto item = static_cast<SysRoleDept>(roleDept);
        switch (status)
        {
            case ChangingStatus::NEW:
                newData.emplace_back(item);
                break;
            case ChangingStatus::UPDATED:
                toUpdate.emplace_back(item);
                break;
            case ChangingStatus::DELETED:
                toDelete.emplace_back(item);
                break;
            case ChangingStatus::UNCHANGED:
                // 未做更改，无需操作数据库
                break;
        }
    }

    if (toDelete.size() > 0)
    {
        Json::Value dataList;
        for (const auto &item : toDelete)
        {
            dataList.append(item.toJson());
        }
        auto sql = sqlGenerator()->getSql("multi_delete_for_role_dept",
                                          {{"data_list", dataList}});
        co_await dbClient()->execSqlCoro(sql);
    }
}

vector<RoleDept> RoleRepository::buildRoleDeptList(
    const vector<SysRoleDept> &sysRoleDeptList) const
{
    vector<RoleDept> result;
    for (const auto &sysRoleDept : sysRoleDeptList)
    {
        result.emplace_back(static_cast<RoleDept>(sysRoleDept));
    }
    return result;
}

inline SqlGenerator *RoleRepository::sqlGenerator()
{
    static const auto sqlGenerator_ = app().getPlugin<SqlGenerator>();
    return sqlGenerator_;
}

inline DbClientPtr RoleRepository::dbClient()
{
    static const DbClientPtr dbClient_ = app().getDbClient();
    return dbClient_;
}

inline CoroMapper<SysRoleDept> RoleRepository::roleDeptMapper()
{
    return CoroMapper<SysRoleDept>{dbClient()};
}
