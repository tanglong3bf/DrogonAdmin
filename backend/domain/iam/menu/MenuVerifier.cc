#include "MenuVerifier.h"
#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;

Task<> MenuVerifier::verifyActionNotUsed(
    const vector<std::int32_t> &actionIds) const
{
    const auto count =
        co_await menuRepository_->countByActionIds(actionIds);
    if (count > 0)
    {
        throw BusinessException("功能已被使用");
    }
}
