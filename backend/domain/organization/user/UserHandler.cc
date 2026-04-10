#include "UserHandler.h"

using namespace std;
using namespace drogon;

Task<> UserHandler::deleteUser(User &user, const int32_t deletedBy) const
{
    user.setDeletedBy(deletedBy);
    co_return;
}
