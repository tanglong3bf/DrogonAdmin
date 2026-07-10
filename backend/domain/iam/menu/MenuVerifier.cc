#include "MenuVerifier.h"
#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;

Task<> MenuVerifier::verifyFunctionNotUsed(
    const vector<std::int32_t> &functionIds) const
{
    const auto count =
        co_await menuRepository_->countByFunctionIds(functionIds);
    if (count > 0)
    {
        throw BusinessException("功能已被使用");
    }
}
