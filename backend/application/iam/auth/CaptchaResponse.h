#pragma once

#include <memory>
#include <string>
#include "common/util/Utilities.hpp"

struct StbMemDeleter
{
    void operator()(unsigned char *p) const noexcept
    {
        if (p)
            ::free(p);
    }
};

class CaptchaResponse
{
  public:
    CaptchaResponse(
        const std::string &captchaId = "",
        std::unique_ptr<std::uint8_t[], StbMemDeleter> imageBlob = nullptr,
        std::int32_t size = 0)
        : captchaId_(captchaId), imageBlob_(std::move(imageBlob)), size_(size)
    {
    }

  public:
    GETTER(captchaId)
    GETTER(imageBlob)
    GETTER(size)

  private:
    std::string captchaId_;
    std::unique_ptr<std::uint8_t[], StbMemDeleter> imageBlob_;
    std::size_t size_;
};
