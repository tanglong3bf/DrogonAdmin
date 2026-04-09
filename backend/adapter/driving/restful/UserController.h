#pragma once

#include <drogon/HttpController.h>
#include "application/organization/user/UserService.h"
#include "application/organization/user/UserCreateRequest.h"

using namespace std;
using namespace drogon;

class UserController : public drogon::HttpController<UserController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::list,
                  "/user?username={}&nickname={}&sex={}&dept_id={}&phone_"
                  "number={}&email={}&status={}&page={}&page_size={}",
                  Get);
    ADD_METHOD_TO(UserController::createUser, "/user", Post);
    METHOD_LIST_END

    Task<HttpResponsePtr> list(const HttpRequestPtr req,
                               const string username,
                               const string nickname,
                               const string sex,
                               const string deptId,
                               const string phoneNumber,
                               const string email,
                               const string status,
                               const string page,
                               const string pageSize) const;

    Task<HttpResponsePtr> createUser(const HttpRequestPtr req,
                                     const UserCreateRequest request) const;

  private:
    UserServicePtr userService_{
        drogon::DrClassMap::getSingleInstance<UserService>()};
};
