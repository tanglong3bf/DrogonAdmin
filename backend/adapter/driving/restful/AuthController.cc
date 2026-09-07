#include "AuthController.h"

#include <drogon/utils/Utilities.h>

using namespace std;
using namespace drogon;

Task<HttpResponsePtr> AuthController::login(const HttpRequestPtr,
                                            const LoginRequest request) const
{
    const auto response = co_await authService_->login(request);
    co_return toResponse(response);
}

Task<HttpResponsePtr> AuthController::getCaptcha(
    const HttpRequestPtr,
    const string_view oldCaptchaId) const
{
    // 删除旧的
    if (!oldCaptchaId.empty())
    {
        authCacheRepo_->removeCaptcha(oldCaptchaId);
    }
    const auto customConfig = app().getCustomConfig();
    const auto fontPath =
        customConfig
            .get("captcha_font_path", "resources/fonts/DejaVuSansMono.ttf")
            .asString();
    const auto captchaResponse =
        authService_->renderCaptchaPng(132, 50, fontPath);

    if (!captchaResponse.imageBlob() || captchaResponse.size() == 0)
    {
        throw BusinessException("生成验证码失败");
    }

    auto resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(CT_IMAGE_PNG);
    resp->setBody(std::string(reinterpret_cast<const char *>(
                                  captchaResponse.imageBlob().get()),
                              captchaResponse.size()));
    resp->addHeader("access-control-expose-headers", "x-captcha-id");
    resp->addHeader("x-captcha-id", captchaResponse.captchaId());

    co_return resp;
}
