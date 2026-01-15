#include "DeptAssembler.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<Dept> DeptAssembler::fromCreateRequest(DeptCreateRequest request,
                                            uint32_t createdBy)
{
    // 验证
    if (request.getParentId())
    {
        co_await deptVerifier_->verifyDepartmentExists(*request.getParentId());
    }
    co_await deptVerifier_->verifyDeptNameNotDuplicated(request.getName(),
                                                        request.getParentId());

    // 准备必备参数
    const auto maxOrder =
        co_await deptRepository_->getMaxSubDeptOrder(request.getParentId());

    Dept dept{request.getName(), maxOrder + 1, createdBy};

    // 可选参数
    if (request.getParentId())
    {
        dept.setParentId(*request.getParentId());
    }

    co_return dept;
}
