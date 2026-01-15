#include <drogon/drogon.h>
#include "common/exception/BusinessException.h"
#include "DrogonJwtUtil/src/JwtUtil.h"

using namespace std;
using namespace drogon;
using namespace drogon::utils;

int main()
{
    app().registerBeginningAdvice([] {
        // clang-format off
        LOG_INFO <<
"\n▄▄▄▄▄▄                                         ▄▄▄▄      ▄▄                    "
"\n███▀▀██▄                                     ▄██▀▀██▄    ██          ▀▀        "
"\n███  ███ ████▄ ▄███▄ ▄████ ▄███▄ ████▄       ███  ███ ▄████ ███▄███▄ ██  ████▄ "
"\n███  ███ ██ ▀▀ ██ ██ ██ ██ ██ ██ ██ ██ ▀▀▀▀▀ ███▀▀███ ██ ██ ██ ██ ██ ██  ██ ██ "
"\n██████▀  ██    ▀███▀ ▀████ ▀███▀ ██ ██       ███  ███ ▀████ ██ ██ ██ ██▄ ██ ██ "
"\n                        ██                                                     "
"\n                      ▀▀▀                                                      "
"\n";
        // clang-format on
    });

    // CORS
    app().registerSyncAdvice([](const HttpRequestPtr &req) -> HttpResponsePtr {
        auto config = app().getCustomConfig();
        const bool isString =
            config.isMember("allowOrigin") && config["allowOrigin"].isString();
        static const string allowOrigin =
            isString ? config["allowOrigin"].asString() : "*";

        if (req->method() != Options)
        {
            return nullptr;
        }
        LOG_TRACE << "为" << req->getPath() << "处理OPTIONS请求";

        auto resp = HttpResponse::newHttpResponse();
        resp->addHeader("access-control-allow-methods", "Get,Post,Put,Delete");
        resp->addHeader("access-control-allow-headers",
                        "Authorization,content-type");
        resp->addHeader("access-control-allow-origin", allowOrigin);
        return resp;
    });
    app().registerPreSendingAdvice(
        [](const HttpRequestPtr &req, const HttpResponsePtr &resp) {
            if (resp->getHeader("access-control-allow-origin") == "")
            {
                LOG_TRACE << req->getPath() << "(" << req->getMethod()
                          << ")响应时补充响应头";
                auto origin = req->getHeader("origin");
                resp->addHeader("access-control-allow-origin",
                                origin != "" ? origin : "*");
            }
        });

    /// @defgroup globalExceptionHandler 统一异常处理
    /// @{
    app().setExceptionHandler(
        [](const std::exception &e,
           const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            stringstream ss;
            ss << "处理" << req->getPath();
            if (req->getQuery().size() > 0)
            {
                ss << "?" << req->getQuery();
            }
            ss << "(" << req->getMethodString() << ") 时抛出了一个异常： "
               << e.what();
            LOG_ERROR << ss.str();

            const auto *businessException =
                dynamic_cast<const BusinessException *>(&e);
            if (businessException == nullptr)
            {
                Json::Value json;
                json["error"] = "未知错误";
                auto resp = HttpResponse::newHttpJsonResponse(json);
                resp->setStatusCode(k500InternalServerError);
                callback(resp);
                return;
            }
            Json::Value json;
            json["error"] = businessException->what();
            auto resp = HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(businessException->getCode());
            callback(resp);
        });
    /// @}

    // 为jwt设置私钥
    app().registerBeginningAdvice([] {
        tl::jwt::JwtUtil *jwtUtil = app().getPlugin<tl::jwt::JwtUtil>();
        if (jwtUtil == nullptr)
        {
            LOG_ERROR << "获取插件失败，请检查配置文件";
            return;
        }
        jwtUtil->setSecret(
            base64Decode("RHJvZ29uLWlzLXRoZS1iZXN0LXdlYi1mcmFtZXdvcmstaW4tdGhlL"
                         "XdvcmxkIQ=="));
    });

    // TODO: 等完成用户管理时，修改此处逻辑，临时用
    app().registerPreHandlingAdvice([](const HttpRequestPtr &req,
                                       AdviceCallback && /*ignore*/,
                                       AdviceChainCallback &&acc) {
        req->setParameter("userId", "1");
        acc();
    });

    app().loadConfigFile("./config.json");
    app().run();
    return 0;
}
