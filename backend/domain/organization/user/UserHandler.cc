#include "UserHandler.h"

using namespace std;
using namespace drogon;

Task<> UserHandler::deleteUser(User &user, const int32_t deletedBy) const
{
    user.markDeletedBy(deletedBy);
    user.markDeleted();
    co_return;
}
