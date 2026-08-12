#pragma once

#include <json/value.h>
#include <string>

struct ChangePasswordRequest
{
    ChangePasswordRequest() = default;

    ChangePasswordRequest(const Json::Value &json);

    std::string_view oldPassword() const
    {
        return oldPassword_;
    }

    std::string_view newPassword() const
    {
        return newPassword_;
    }

  private:
    std::string oldPassword_;
    std::string newPassword_;
};
