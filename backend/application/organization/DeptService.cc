#include "DeptService.h"

#include <drogon/HttpAppFramework.h>
#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<vector<DeptResponse>> DeptService::getDeptTree() const
{
    co_return co_await deptCqrsRepo_->getDeptTree();
}

Task<> DeptService::createDept(const DeptCreateRequest &request,
                               const int32_t createdBy) const
{
    auto dept = co_await deptAssembler_->fromCreateRequest(request, createdBy);
    dept.toNew();
    co_await deptRepository_->save(dept);
}

Task<> DeptService::updateDept(const int32_t deptId,
                               const DeptUpdateRequest &request,
                               const int32_t updatedBy) const
{
    try
    {
        auto dept = co_await deptRepository_->getById(deptId);

        co_await deptHandler_->updateDept(dept, request.getName(), updatedBy);

        dept.toUpdate();
        co_await deptRepository_->save(dept);
    }
    catch (const orm::UnexpectedRows &e)
    {
        LOG_ERROR << e.what();
        throw BusinessException{"指定id的部门不存在"};
    }
}

drogon::Task<> DeptService::deleteDept(const std::int32_t deptId,
                                       const std::int32_t deletedBy) const
{
    try
    {
        auto dept = co_await deptRepository_->getById(deptId);

        co_await deptHandler_->deleteDept(dept, deletedBy);

        dept.toDelete();

        // TODO: 事务
        co_await deptRepository_->save(dept);
        co_await roleService_->deleteExcludingDept(deptId, deletedBy);
    }
    catch (const orm::UnexpectedRows &e)
    {
        LOG_ERROR << e.what();
        throw BusinessException{"指定id的部门不存在"};
    }
}
