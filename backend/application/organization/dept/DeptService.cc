#include "DeptService.h"

#include "DeptSortRequest.h"
#include "common/exception/BusinessException.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Exception.h>
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
    auto dept = co_await deptRepository_->getById(deptId);
    if (!dept)
    {
        throw BusinessException{"指定的部门id不存在"};
    }
    co_await deptHandler_->updateDept(*dept, request.getName(), updatedBy);
    co_await deptRepository_->save(*dept);
}

Task<> DeptService::deleteDept(const std::int32_t deptId,
                               const std::int32_t deletedBy) const
{
    optional<Dept> dept = co_await deptRepository_->getById(deptId);
    if (!dept)
    {
        // 没有数据，无需操作
        co_return;
    }

    co_await deptHandler_->deleteDept(*dept, deletedBy);

    const auto trans = co_await app().getDbClient()->newTransactionCoro();
    try
    {
        co_await deptRepository_->save(*dept, trans);
        co_await roleService_->deleteExcludingDept(deptId, trans);
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_ERROR << e.base().what();
        trans->rollback();
        throw e;
    }
}

Task<> DeptService::sortDept(const DeptSortRequest &request,
                             const std::int32_t updatedBy) const
{
    const auto parentId = request.getParentId();
    const auto deptIds = request.getDeptIds();

    auto sortResult =
        co_await deptHandler_->sortDept(parentId, deptIds, updatedBy);

    co_await deptRepository_->multiSave(sortResult);
}
