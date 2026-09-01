#include "common/exception/BusinessException.h"
#include "DrogonJwtUtil/src/JwtUtil.h"
#include <drogon/drogon.h>

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
"\nVersion: " VER_STR
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
        resp->addHeader("access-control-allow-methods",
                        "GET,POST,PUT,DELETE,PATCH");
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
                json["code"] = -1;
                json["error"] = "未知错误";
                auto resp = HttpResponse::newHttpJsonResponse(json);
                resp->setStatusCode(k500InternalServerError);
                callback(resp);
                return;
            }
            Json::Value json;
            json["code"] = -1;
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

    // 头像文件处理
    app().registerPreRoutingAdvice([](const HttpRequestPtr &req,
                                      AdviceCallback &&ac,
                                      AdviceChainCallback &&acc) {
        // 前端请求路径：/${custom_config.img_prefix}/${MD5}.${ext}
        auto &config = app().getCustomConfig();
        string imgPrefix = config.get("img_prefix", "/uploads").asString();
        regex re{"^" + imgPrefix + "/([\\dA-F]{32})\\.(jpe?g|png)$"};
        smatch matchResult;
        if (!regex_match(req->getPath(), matchResult, re))
        {
            acc();
            return;
        }

        // MD5
        string fileNameNoExt = matchResult[1];
        // ext
        string extension = matchResult[2];

        // 实际文件地址：${app.upload_path}/${MD5}.${ext}
        HttpResponsePtr resp = HttpResponse::newFileResponse(format(
            "{}/{}.{}", app().getUploadPath(), fileNameNoExt, extension));
        ac(resp);
    });

    // 登录检查
    app().registerPreHandlingAdvice([](const HttpRequestPtr &req,
                                       AdviceCallback &&ac,
                                       AdviceChainCallback &&acc) {
        static const auto jwtUtil_ = app().getPlugin<tl::jwt::JwtUtil>();
        static const auto whiteList =
            app().getCustomConfig()["login_check_white_list"];

        // 放行白名单接口
        for (const auto &item : whiteList)
        {
            if (req->getPath() == item.asString())
            {
                acc();
                return;
            }
        }

        const auto authorization = req->getHeader("Authorization");
        // 检查格式
        if (authorization.starts_with("Bearer "))
        {
            const auto token = authorization.substr(7);
            // 校验token
            const auto result = jwtUtil_->decode(token);
            if (result.first == tl::jwt::Result::Ok)
            {
                const auto &userInfo = *result.second;
                req->getAttributes()->insert("userId",
                                             userInfo["user_id"].as<int32_t>());
                acc();
                return;
            }
        }
        // 格式错误或token无效
        Json::Value json;
        json["code"] = -1;
        json["error"] = "未登录";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k401Unauthorized);
        ac(resp);
    });

    app().loadConfigFile("./config.json");
    app().run();
    return 0;
}
