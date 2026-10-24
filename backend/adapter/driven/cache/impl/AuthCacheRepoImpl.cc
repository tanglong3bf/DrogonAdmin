#include "AuthCacheRepoImpl.h"

#include "common/exception/BusinessException.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/nosql/RedisException.h>
#include <drogon/nosql/RedisResult.h>

using namespace std;
using namespace drogon;
using namespace drogon::nosql;

void AuthCacheRepoImpl::saveCaptcha(string_view key,
                                    string_view captcha,
                                    const uint32_t expireTime) const
{
    redisWrapper_->set(
        format("captcha:{}", key),
        captcha,
        [](const RedisException &e) {
            LOG_ERROR << "存储验证码失败" << e.what();
        },
        expireTime);
}

Task<string> AuthCacheRepoImpl::getCaptcha(string_view key) const
{
    try
    {
        co_return co_await redisWrapper_->get(format("captcha:{}", key));
    }
    catch (const RedisException &e)
    {
        LOG_ERROR << "获取验证码失败" << e.what();
        throw BusinessException("验证码已失效");
    }
}

void AuthCacheRepoImpl::removeCaptcha(string_view key) const
{
    redisWrapper_->del(format("captcha:{}", key), [](const RedisException &e) {
        LOG_ERROR << "存储验证码失败" << e.what();
    });
}

void AuthCacheRepoImpl::addLoginFailCountOrLock(string_view username) const
{
    const char *luaScript = R"(
local exists = redis.call('EXISTS', KEYS[1])
if (exists == 0) then
    redis.call('SET', KEYS[1], 1, 'EX', ARGV[1])
    return 1
end
local cnt = redis.call('GET', KEYS[1])
redis.call('EXPIRE', KEYS[1], ARGV[1])
if tonumber(cnt) < tonumber(ARGV[3]) then
    cnt = redis.call('INCR', KEYS[1])
end
if tonumber(cnt) == tonumber(ARGV[3]) then
    redis.call('SET', KEYS[2], '1', 'EX', ARGV[2])
end
)";
    const auto redisClient = app().getRedisClient();
    redisClient->execCommandAsync([](const RedisResult & /*ignore*/) {},
                                  [](const RedisException &e) {
                                      LOG_ERROR << "执行lua脚本失败："
                                                << e.what();
                                  },
                                  "EVAL %s 2 %s %s %d %d %d",
                                  luaScript,
                                  format("login:fail:{{{}}}", username).c_str(),
                                  format("login:lock:{{{}}}", username).c_str(),
                                  1800,
                                  900,
                                  5);
}

Task<bool> AuthCacheRepoImpl::checkUserIsLocked(string_view username) const
{
    co_return co_await redisWrapper_->exists(
        format("login:lock:{{{}}}", username));
}

Task<> AuthCacheRepoImpl::removeLoginFailCount(std::string_view username) const
{
    redisWrapper_->del(format("login:fail:{{{}}}", username),
                       [](const RedisException &e) {
                           LOG_ERROR << "删除登录失败次数失败" << e.what();
                       });
    co_return;
}
