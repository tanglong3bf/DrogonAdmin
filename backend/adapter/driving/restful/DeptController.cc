#include "DeptController.h"

using namespace drogon;

Task<HttpResponsePtr> DeptController::getDeptTree(
    const HttpRequestPtr req) const
{
    const auto tree = co_await deptService_->getDeptTree();
    co_return toResponse(tree);
}
