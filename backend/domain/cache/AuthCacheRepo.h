#pragma once

#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>
#include <drogon/nosql/RedisClient.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/DrObject.h>

/**
 * @brief 权限缓存基类
 */
class AuthCacheRepoBase : public virtual drogon::DrObjectBase
{
  public:
    /**
     * @brief 保存验证码
     */
    virtual void saveCaptcha(std::string_view key,
                             std::string_view captcha,
                             std::uint32_t expireTime = 120) const = 0;

    /**
     * @brief 获取验证码
     */
    virtual drogon::Task<std::string> getCaptcha(
        std::string_view key) const = 0;

    /**
     * @brief 删除验证码
     */
    virtual void removeCaptcha(std::string_view key) const = 0;

    /**
     * @brief 增加用户登录失败次数，如果达到五次，锁定用户
     */
    virtual void addLoginFailCountOrLock(std::string_view username) const = 0;

    /**
     * @brief 检查用户是否锁定
     */
    virtual drogon::Task<bool> checkUserIsLocked(
        std::string_view username) const = 0;

    /**
     * @brief 删除用户登录失败次数
     */
    virtual drogon::Task<> removeLoginFailCount(
        std::string_view username) const = 0;

  public:
    static std::string classTypeName()
    {
        return "AuthCacheRepoBase";
    }
};

/**
 * @brief 权限缓存
 *
 * 对于验证码、失败次数、锁定状态等缓存
 *
 * @see AuthCacheRepoImpl
 */
template <typename T>
class AuthCacheRepo : public AuthCacheRepoBase, public DrAdminObject<T>
{
  private:
    struct Registrator
    {
        Registrator()
        {
            drogon::DrClassMap::registerClass("AuthCacheRepoBase",
                                              [] { return new T; });
        }
    };

    static Registrator registrator_;

    virtual Registrator *touch()
    {
        return &registrator_;
    }
};

template <typename T>
AuthCacheRepo<T>::Registrator AuthCacheRepo<T>::registrator_;

using AuthCacheRepoPtr = std::shared_ptr<AuthCacheRepoBase>;
