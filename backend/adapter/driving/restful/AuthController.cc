#include "AuthController.h"

using namespace drogon;

Task<HttpResponsePtr> AuthController::login(const HttpRequestPtr,
                                            const LoginRequest request) const
{
    const auto response = co_await authService_->login(request);
    co_return toResponse(response);
}
