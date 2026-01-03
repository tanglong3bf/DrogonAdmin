#include <drogon/drogon.h>

using namespace drogon;

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
    app().loadConfigFile("../config.json");
    app().run();
    return 0;
}
