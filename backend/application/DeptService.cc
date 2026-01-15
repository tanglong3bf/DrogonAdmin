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
                               const uint32_t createdBy)
{
    auto dept = co_await deptAssembler_->fromCreateRequest(request, createdBy);
    dept.toNew();
    co_await deptRepository_->save(dept);
}

Task<> DeptService::updateDept(const uint32_t deptId,
                               const DeptUpdateRequest &request,
                               const uint32_t updatedBy)
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
