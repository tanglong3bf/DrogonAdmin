#include "AuthService.h"

#include "DrogonJwtUtil/src/JwtUtil.h"
#include "common/exception/BusinessException.h"
#include <drogon/utils/Utilities.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <random>
#include <memory>
#include <string>
#include <cstdint>
#include <cmath>

using namespace std;
using namespace drogon;

// 定义在User.cc中
bool matches(string_view raw, string_view mask);

/// @note 校验验证码时忽略大小写
bool captchaMatches(string_view cache, string_view raw)
{
    if (cache.size() != raw.size())
        return false;

    auto toLower = [](char c) noexcept -> char {
        if (c >= 'A' && c <= 'Z')
            return c - 'A' + 'a';
        return c;
    };

    for (size_t i = 0; i < cache.size(); ++i)
    {
        if (toLower(cache[i]) != toLower(raw[i]))
            return false;
    }
    return true;
}

Task<LoginResponse> AuthService::login(const LoginRequest &request) const
{
    // 获取验证码
    const auto captchaInCache =
        co_await authCacheRepo_->getCaptcha(request.captchaId());
    if (!captchaMatches(captchaInCache, request.captcha()))
    {
        throw BusinessException{"验证码错误"};
    }
    // 一次性验证码，删除
    authCacheRepo_->removeCaptcha(request.captchaId());

    // 用户名
    const auto user =
        co_await userRepository_->getByUsername(request.username(), true);
    if (!user)
    {
        // 模拟校验密码，防止恶意攻击者通过响应时间判断出账号是否存在
        matches(request.password(), request.password());
        throw BusinessException{"用户名或密码错误，登录失败"};
    }
    // 密码
    if (!matches(request.password(), user->password()))
    {
        authCacheRepo_->addLoginFailCountOrLock(user->username());
        throw BusinessException{"用户名或密码错误，登录失败"};
    }

    // 禁用状态检查
    if (user->isDisabled())
    {
        throw BusinessException{"用户已被禁用"};
    }

    // 锁定状态检查
    bool isLocked =
        co_await authCacheRepo_->checkUserIsLocked(user->username());
    if (isLocked)
    {
        throw BusinessException{"用户已被锁定，请于15分钟后重试"};
    }
    else
    {
        // 正常登录清除失败次数
        co_await authCacheRepo_->removeLoginFailCount(user->username());
    }
    static auto *jwtUtil = drogon::app().getPlugin<tl::jwt::JwtUtil>();

    // 暂时只存userId，后续会存权限
    Json::Value jwtData;
    jwtData["user_id"] = *user->userId();
    const auto token = jwtUtil->encode(jwtData);
    co_return LoginResponse{token, UserResponse{*user}};
}

static int randomInt(int min, int max)
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

static float randomFloat(float min, float max)
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

static void drawLine(uint8_t *canvas,
                     int width,
                     int height,
                     int x1,
                     int y1,
                     int x2,
                     int y2,
                     uint8_t r,
                     uint8_t g,
                     uint8_t b,
                     uint8_t a = 255)
{
    const int channels = 4;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
        {
            size_t pos = (y1 * width + x1) * channels;
            canvas[pos + 0] = r;
            canvas[pos + 1] = g;
            canvas[pos + 2] = b;
            canvas[pos + 3] = a;
        }
        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

CaptchaResponse AuthService::renderCaptchaPng(int width,
                                              int height,
                                              const string &font_path)
{
    // 验证码文本
    const string text = genCaptchaText(4);
    FT_Library ft{};
    if (FT_Init_FreeType(&ft) != 0)
        return {};

    FT_Face face{};
    if (FT_New_Face(ft, font_path.c_str(), 0, &face) != 0)
    {
        FT_Done_FreeType(ft);
        return {};
    }

    FT_Set_Pixel_Sizes(face, 0, 42);

    const int channels = 4;
    const size_t buf_size = width * height * channels;
    // 画布
    unique_ptr<uint8_t[]> canvas = make_unique<uint8_t[]>(buf_size);

    // 白色背景
    for (size_t i = 0; i < buf_size; i += 4)
    {
        canvas[i + 0] = 255;
        canvas[i + 1] = 255;
        canvas[i + 2] = 255;
        canvas[i + 3] = 255;
    }

    int pen_x = randomInt(5, 18);
    int base_y = height - 12;

    for (char ch : text)
    {
        // 随机角度
        float angle_deg = randomFloat(-15.0f, 15.0f);
        float angle_rad = angle_deg * static_cast<float>(M_PI) / 180.0f;

        FT_Matrix matrix;
        matrix.xx = static_cast<FT_Fixed>(cos(angle_rad) * 0x10000L);
        matrix.xy = static_cast<FT_Fixed>(-sin(angle_rad) * 0x10000L);
        matrix.yx = static_cast<FT_Fixed>(sin(angle_rad) * 0x10000L);
        matrix.yy = static_cast<FT_Fixed>(cos(angle_rad) * 0x10000L);

        FT_Set_Transform(face, &matrix, nullptr);

        if (FT_Load_Char(face, ch, FT_LOAD_RENDER) != 0)
            continue;

        auto &glyph = face->glyph;
        FT_Bitmap &bmp = glyph->bitmap;

        // 颜色
        uint8_t fr = randomInt(10, 160);
        uint8_t fg = randomInt(10, 160);
        uint8_t fb = randomInt(10, 160);

        int pen_y = base_y + randomInt(-6, 6);

        for (int y = 0; y < static_cast<int>(bmp.rows); y++)
        {
            for (int x = 0; x < static_cast<int>(bmp.width); x++)
            {
                uint8_t gray = bmp.buffer[y * bmp.pitch + x];
                if (gray == 0)
                    continue;

                int cx = pen_x + glyph->bitmap_left + x;
                int cy = pen_y - glyph->bitmap_top + y;

                if (cx < 0 || cx >= width || cy < 0 || cy >= height)
                    continue;

                size_t pos = (cy * width + cx) * channels;
                // Alpha混合
                float alpha = gray / 255.0f;
                canvas[pos + 0] =
                    static_cast<uint8_t>(255 * (1 - alpha) + fr * alpha);
                canvas[pos + 1] =
                    static_cast<uint8_t>(255 * (1 - alpha) + fg * alpha);
                canvas[pos + 2] =
                    static_cast<uint8_t>(255 * (1 - alpha) + fb * alpha);
                canvas[pos + 3] = 255;
            }
        }

        int advance = (glyph->advance.x >> 6) + randomInt(-2, 2);
        pen_x += advance;
    }

    // 随机画线
    int line_num = randomInt(3, 5);
    for (int i = 0; i < line_num; i++)
    {
        int x1 = randomInt(0, width);
        int y1 = randomInt(0, height);
        int x2 = randomInt(0, width);
        int y2 = randomInt(0, height);
        uint8_t lr = randomInt(60, 180);
        uint8_t lg = randomInt(60, 180);
        uint8_t lb = randomInt(60, 180);
        drawLine(canvas.get(), width, height, x1, y1, x2, y2, lr, lg, lb);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    int png_len = 0;
    // 生成png
    unsigned char *png_mem = stbi_write_png_to_mem(
        canvas.get(), width * channels, width, height, channels, &png_len);

    if (!png_mem || png_len <= 0)
    {
        return {};
    }

    // 生成key
    const string captchaKey = utils::getUuid();
    authCacheRepo_->saveCaptcha(captchaKey, text);

    return {captchaKey,
            unique_ptr<unsigned char[], StbMemDeleter>(png_mem),
            png_len};
}

string AuthService::genCaptchaText(int len)
{
    static constexpr const char *dict = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    static mt19937 rng{random_device{}()};
    static uniform_int_distribution<> dist(0, std::strlen(dict) - 1);
    string s;
    s.reserve(len);
    for (int i = 0; i < len; ++i)
    {
        s += dict[dist(rng)];
    }
    return s;
}
