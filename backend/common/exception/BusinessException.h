#pragma once

#include <string>
#include <exception>
#include <drogon/HttpTypes.h>

/**
 * @brief 业务异常
 *
 * @see globalExceptionHandler
 */
class BusinessException : public std::exception
{
  public:
    BusinessException(
        const std::string &message,
        const drogon::HttpStatusCode code = drogon::k400BadRequest);

    const char *what() const noexcept override;
    drogon::HttpStatusCode getCode() const;

  private:
    std::string message_;          ///< 异常信息
    drogon::HttpStatusCode code_;  ///< 响应状态码
};
