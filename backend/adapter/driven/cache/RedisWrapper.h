#pragma once

#include <drogon/HttpAppFramework.h>
#include <drogon/nosql/RedisException.h>

/**
 * @brief Redis简单封装
 */
class RedisWrapper : public drogon::DrObject<RedisWrapper>
{
  public:
    void set(std::string_view key,
             std::string_view value,
             drogon::nosql::RedisExceptionCallback &&callback,
             std::uint32_t expireTime = 0) const;

    drogon::Task<std::string> get(std::string_view key) const;

    void del(std::string_view key,
             drogon::nosql::RedisExceptionCallback &&callback) const;

    drogon::Task<bool> exists(std::string_view key) const;

  private:
    drogon::nosql::RedisClientPtr redisClient_{drogon::app().getRedisClient()};
};

using RedisWrapperPtr = std::shared_ptr<RedisWrapper>;
