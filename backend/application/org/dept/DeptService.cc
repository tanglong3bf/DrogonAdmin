#include "DeptService.h"

#include "DeptSortRequest.h"
#include "common/exception/BusinessException.h"
#include "common/util/rangesUtils.hpp"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Exception.h>
#include <drogon/utils/coroutine.h>
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_admin;

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
    co_await deptHandler_->updateDept(*dept,
                                      static_cast<string>(request.name()),
                                      request.version(),
                                      updatedBy);
    co_await deptRepository_->save(*dept);
}

Task<> DeptService::deleteDept(const std::int32_t deptId,
                               const std::int32_t version,
                               const std::int32_t deletedBy) const
{
    auto dept = co_await deptRepository_->getById(deptId);
    if (!dept)
    {
        // 没有数据，无需操作
        co_return;
    }

    co_await deptHandler_->deleteDept(*dept, version, deletedBy);

    const auto trans = co_await app().getDbClient()->newTransactionCoro();
    try
    {
        co_await deptRepository_->save(*dept, trans);
        // 跨聚合删除关联数据
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
                             const int32_t updatedBy) const
{
    vector<Dept> sortResult =
        co_await deptHandler_->sortDept(request.parentId(),
                                        request.deptIds(),
                                        updatedBy);

    if (sortResult.size() == 0)
    {
        co_return;
    }

    const vector<int32_t> versions =
        // 排序后结果
        sortResult |
        // 取id
        views::transform([](const auto &dept) { return *dept.deptId(); }) |
        // 从请求中找到version
        views::transform([&request](const auto &deptId) {
            return ranges::find_if(request.depts(), [deptId](const auto &item) {
                return deptId == item.deptId;
            });
        }) |
        views::transform([](const auto &it) { return it->version; }) |
        ranges_utils::to<vector>();

    for (size_t i = 0; i < sortResult.size(); i++)
    {
        if (versions[i] != sortResult[i].version())
        {
            throw BusinessException("更新期间数据发生变化，更新失败");
        }
    }

    co_await deptRepository_->multiSave(sortResult, versions);
}
