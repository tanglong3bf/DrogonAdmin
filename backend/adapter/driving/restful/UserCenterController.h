#pragma once

#include "application/organization/user/UserCenterService.h"
#include "application/organization/user/UserInfoUpdateRequest.h"
#include <drogon/HttpController.h>

class UserCenterController : public drogon::HttpController<UserCenterController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserCenterController::updateBasicInfo,
                  "/user_center/basic_info",
                  drogon::Patch,
                  drogon::Options);
    METHOD_LIST_END
    /**
     * @brief 更新用户基础信息
     */
    drogon::Task<drogon::HttpResponsePtr> updateBasicInfo(
        const drogon::HttpRequestPtr req,
        const UserInfoUpdateRequest request) const;

  private:
    UserCenterServicePtr userCenterService_{
        drogon::DrClassMap::getSingleInstance<UserCenterService>()};
};
