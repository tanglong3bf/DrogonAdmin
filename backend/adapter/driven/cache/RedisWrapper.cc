#include "RedisWrapper.h"

using namespace std;
using namespace drogon;
using namespace drogon::nosql;

void RedisWrapper::set(string_view key,
                       string_view value,
                       RedisExceptionCallback &&callback,
                       uint32_t expireTime) const
{
    string command;
    if (expireTime != 0)
    {
        command = format("SET {} {} EX {}", key, value, expireTime);
    }
    else
    {
        command = format("SET {} {}", key, value);
    }
    redisClient_->execCommandAsync([](const RedisResult & /*ignore*/) {},
                                   std::move(callback),
                                   command);
}

Task<string> RedisWrapper::get(string_view key) const
{
    auto result = co_await redisClient_->execCommandCoro("GET %s", key.data());
    co_return result.asString();
}

void RedisWrapper::del(string_view key, RedisExceptionCallback &&callback) const
{
    redisClient_->execCommandAsync([](const RedisResult & /*ignore*/) {},
                                   std::move(callback),
                                   "DEL %s",
                                   key.data());
}

Task<bool> RedisWrapper::exists(string_view key) const
{
    auto result =
        co_await redisClient_->execCommandCoro("EXISTS %s", key.data());
    co_return result.asInteger() == 1;
}
