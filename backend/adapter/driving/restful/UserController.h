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
                  Get,
                  Options);
    ADD_METHOD_TO(UserController::createUser, "/user", Post, Options);
    ADD_METHOD_TO(UserController::updateUser, "/user/{id}", Put, Options);
    ADD_METHOD_TO(UserController::deleteUser, "/user/{id}", Delete, Options);
    METHOD_LIST_END

    /**
     * @brief 带条件分页查询用户列表
     */
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

    /**
     * @brief 新增用户
     */
    Task<HttpResponsePtr> createUser(const HttpRequestPtr req,
                                     const UserCreateRequest request) const;

    /**
     * @brief 更新用户
     */
    Task<HttpResponsePtr> updateUser(const HttpRequestPtr req,
                                     const int32_t userId,
                                     const UserUpdateRequest request) const;

    /**
     * @brief 删除用户
     */
    Task<HttpResponsePtr> deleteUser(const HttpRequestPtr req,
                                     const int32_t userId) const;

  private:
    UserServicePtr userService_{
        drogon::DrClassMap::getSingleInstance<UserService>()};
};
