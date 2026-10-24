#pragma once

#include "adapter/driven/cache/RedisWrapper.h"
#include "domain/cache/AuthCacheRepo.h"
#include <drogon/DrClassMap.h>

class AuthCacheRepoImpl : public AuthCacheRepo<AuthCacheRepoImpl>
{
  public:
    /**
     * @brief 保存验证码
     */
    void saveCaptcha(std::string_view key,
                     std::string_view captcha,
                     std::uint32_t expireTime = 120) const override;

    /**
     * @brief 获取验证码
     */
    virtual drogon::Task<std::string> getCaptcha(
        std::string_view key) const override;

    /**
     * @brief 删除验证码
     */
    virtual void removeCaptcha(std::string_view key) const override;

    /**
     * @brief 增加用户登录失败次数，如果达到五次，锁定用户
     */
    virtual void addLoginFailCountOrLock(
        std::string_view username) const override;

    /**
     * @brief 检查用户是否锁定
     */
    virtual drogon::Task<bool> checkUserIsLocked(
        std::string_view username) const override;

    /**
     * @brief 删除用户登录失败次数
     */
    virtual drogon::Task<> removeLoginFailCount(
        std::string_view username) const override;

  private:
    RedisWrapperPtr redisWrapper_{
        drogon::DrClassMap::getSingleInstance<RedisWrapper>()};
};
