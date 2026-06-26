#pragma once

#include "application/organization/user/UserCenterService.h"
#include "application/organization/user/dto/ChangePasswordRequest.h"
#include "application/organization/user/dto/UserInfoUpdateRequest.h"
#include <drogon/HttpController.h>

class UserCenterController : public drogon::HttpController<UserCenterController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserCenterController::updateBasicInfo,
                  "/user_center/basic_info",
                  drogon::Patch,
                  drogon::Options);
    ADD_METHOD_TO(UserCenterController::changePassword,
                  "/user_center/change_password",
                  drogon::Patch,
                  drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 更新用户基础信息
     */
    drogon::Task<drogon::HttpResponsePtr> updateBasicInfo(
        const drogon::HttpRequestPtr req,
        const UserInfoUpdateRequest request) const;
    drogon::Task<drogon::HttpResponsePtr> changePassword(
        const drogon::HttpRequestPtr req,
        const ChangePasswordRequest request) const;

  private:
    UserCenterServicePtr userCenterService_{
        drogon::DrClassMap::getSingleInstance<UserCenterService>()};
};
