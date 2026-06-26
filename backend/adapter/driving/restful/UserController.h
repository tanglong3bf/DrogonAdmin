#pragma once

#include "application/organization/user/UserService.h"
#include "application/organization/user/UserCreateRequest.h"
#include <drogon/HttpController.h>

class UserController : public drogon::HttpController<UserController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::list,
                  "/user?username={}&nickname={}&sex={}&dept_id={}&phone_"
                  "number={}&email={}&status={}&page={}&page_size={}",
                  drogon::Get,
                  drogon::Options);
    ADD_METHOD_TO(UserController::createUser,
                  "/user",
                  drogon::Post,
                  drogon::Options);
    ADD_METHOD_TO(UserController::updateUser,
                  "/user/{id}",
                  drogon::Patch,
                  drogon::Options);
    ADD_METHOD_TO(UserController::deleteUser,
                  "/user/{id}",
                  drogon::Delete,
                  drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 带条件分页查询用户列表
     */
    drogon::Task<drogon::HttpResponsePtr> list(
        const drogon::HttpRequestPtr req,
        const std::string username,
        const std::string nickname,
        const std::string sex,
        const std::string deptId,
        const std::string phoneNumber,
        const std::string email,
        const std::string status,
        const std::string page,
        const std::string pageSize) const;

    /**
     * @brief 新增用户
     */
    drogon::Task<drogon::HttpResponsePtr> createUser(
        const drogon::HttpRequestPtr req,
        const UserCreateRequest request) const;

    /**
     * @brief 更新用户
     */
    drogon::Task<drogon::HttpResponsePtr> updateUser(
        const drogon::HttpRequestPtr req,
        const std::int32_t userId,
        const UserUpdateRequest request) const;

    /**
     * @brief 删除用户
     */
    drogon::Task<drogon::HttpResponsePtr> deleteUser(
        const drogon::HttpRequestPtr req,
        const std::int32_t userId) const;

  private:
    UserServicePtr userService_{
        drogon::DrClassMap::getSingleInstance<UserService>()};
};
