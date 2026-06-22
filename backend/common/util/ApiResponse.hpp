#pragma once

#include <drogon/HttpResponse.h>
#include <json/value.h>
#include <string>

#define SET_FIELD(param)                      \
    if (!param##_.empty()) [[unlikely]]       \
    {                                         \
        if (param##_.size() == 1) [[likely]]  \
        {                                     \
            json[#param] = param##_[0];       \
        }                                     \
        else                                  \
        {                                     \
            Json::Value data;                 \
            for (const auto &item : param##_) \
            {                                 \
                data.append(item);            \
            }                                 \
            json[#param] = data;              \
        }                                     \
    }

template <typename T>
class ApiResponse
{
  public:
    explicit ApiResponse(const T &data, const drogon::AttributesPtr &attr)
        : data_{data}
    {
        message_ = attr->get<std::vector<std::string>>("message");
        warn_ = attr->get<std::vector<std::string>>("warn");
        error_ = attr->get<std::vector<std::string>>("error");
    }

    Json::Value toJson() const
    {
        Json::Value json;
        SET_FIELD(message);
        SET_FIELD(warn);
        SET_FIELD(error);
        json["data"] = data_.toJson();
        return json;
    }

  private:
    std::vector<std::string> message_;
    std::vector<std::string> warn_;
    std::vector<std::string> error_;
    T data_;
};

#undef SET_FIELD

namespace drogon
{
template <typename T>
HttpResponsePtr toResponse(const ApiResponse<T> &resp)
{
    return HttpResponse::newHttpJsonResponse(resp.toJson());
}
};  // namespace drogon
