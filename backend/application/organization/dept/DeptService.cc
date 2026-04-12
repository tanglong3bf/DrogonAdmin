#include "DeptService.h"

#include "DeptSortRequest.h"
#include "common/exception/BusinessException.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/coroutine.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<vector<DeptResponse>> DeptService::getDeptTree() const
{
    co_return co_await deptCqrsRepo_->getDeptTree();
}

Task<> DeptService::createDept(const DeptCreateRequest &request,
                               const std::int32_t createdBy) const
{
    auto dept = co_await deptAssembler_->fromCreateRequest(request, createdBy);
    co_await deptRepository_->save(dept);
}

Task<> DeptService::updateDept(const std::int32_t deptId,
                               const DeptUpdateRequest &request,
                               const std::int32_t updatedBy) const
{
    try
    {
        auto dept = co_await deptRepository_->getById(deptId);

        co_await deptHandler_->updateDept(dept, request.getName(), updatedBy);
        co_await deptRepository_->save(dept);
    }
    catch (const orm::UnexpectedRows &e)
    {
        LOG_ERROR << e.what();
        throw BusinessException{"指定id的部门不存在"};
    }
}

Task<> DeptService::deleteDept(const std::int32_t deptId,
                               const std::int32_t deletedBy) const
{
    try
    {
        auto dept = co_await deptRepository_->getById(deptId);

        co_await deptHandler_->deleteDept(dept, deletedBy);

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

Task<> DeptService::sortDept(const DeptSortRequest &request,
                             const std::int32_t updatedBy) const
{
    const auto parentId = request.getParentId();
    const auto allDepts = co_await deptRepository_->getByParentId(parentId);
    const auto deptIds = request.getDeptIds();

    auto sortResult =
        co_await deptHandler_->sortDept(deptIds, allDepts, updatedBy);

    co_await deptRepository_->multiSave(sortResult);
}
