#pragma once

#include <drogon/HttpResponse.h>
#include <jsoncpp/json/value.h>
#include <cstdint>
#include <vector>

template <typename T>
class PaginatedResponse
{
    std::int32_t page_;
    std::int32_t pageSize_;
    std::int32_t total_;
    std::vector<T> list_;

  public:
    PaginatedResponse(const std::int32_t page,
                      const std::int32_t pageSize,
                      const std::int32_t total,
                      const std::vector<T> &list)
        : page_{page}, pageSize_(pageSize), total_(total), list_(list)
    {
    }

    Json::Value toJson() const
    {
        Json::Value json;
        json["page"] = page_;
        json["page_size"] = pageSize_;
        json["total"] = total_;
        Json::Value list(Json::arrayValue);
        for (const T &item : list_)
        {
            list.append(item.toJson());
        }
        json["list"] = list;
        return json;
    }
};

namespace drogon
{
template <typename T>
HttpResponsePtr toResponse(const PaginatedResponse<T> &response)
{
    Json::Value json;
    json["data"] = response.toJson();
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon
