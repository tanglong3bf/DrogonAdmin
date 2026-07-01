#include "DeptAssembler.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<Dept> DeptAssembler::fromCreateRequest(DeptCreateRequest request,
                                            int32_t createdBy)
{
    // 验证
    if (request.parentId())
    {
        co_await deptVerifier_->verifyDepartmentExists(*request.parentId());
    }
    co_await deptVerifier_->verifyDeptNameNotDuplicated(static_cast<string>(
                                                            request.name()),
                                                        request.parentId());

    // 准备必备参数
    const auto maxSortNum =
        co_await deptRepository_->getMaxSubDeptSortNum(request.parentId());

    Dept dept{static_cast<string>(request.name()),
              maxSortNum ? *maxSortNum + 1 : 0,
              createdBy};

    // 可选参数
    dept.setParentId(request.parentId());

    dept.markNew();
    co_return dept;
}
