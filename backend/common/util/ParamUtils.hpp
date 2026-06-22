#pragma once

#include <string>

static std::string trim(const std::string &s)
{
    size_t start = 0;
    while (start < s.size() &&
           std::isspace(static_cast<unsigned char>(s[start])))
    {
        start++;
    }

    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
    {
        end--;
    }

    return s.substr(start, end - start);
}

/**
 * @brief 转义字符串，仅处理字符串已有内容，不会额外添加%%
 */
static std::string escapeSqlLike(const std::string &input)
{
    std::string out;

    for (unsigned char c : input)
    {
        switch (c)
        {
            case '\'':
                out += "''";
                break;

            case '%':
                out += "\\%";
                break;
            case '_':
                out += "\\_";
                break;

            case '\\':
                out += "\\\\";
                break;

            default:
                out += c;
                break;
        }
    }
    return out;
}

static std::size_t utf8Length(const std::string &s)
{
    std::size_t count = 0;
    std::size_t i = 0;
    const std::size_t len = s.size();

    while (i < len)
    {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        if (c < 0x80)
        {
            // 单字节合法字符
            ++count;
            ++i;
        }
        else if ((c & 0xE0) == 0xC0)
        {
            // 双字节字符
            if (i + 1 >= len ||
                (static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80)
            {
                ++count;  // 非法，视为1个无效字符
                ++i;
            }
            else
            {
                ++count;
                i += 2;
            }
        }
        else if ((c & 0xF0) == 0xE0)
        {
            // 三字节字符
            if (i + 2 >= len ||
                (static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80 ||
                (static_cast<unsigned char>(s[i + 2]) & 0xC0) != 0x80)
            {
                ++count;
                ++i;
            }
            else
            {
                ++count;
                i += 3;
            }
        }
        else if ((c & 0xF8) == 0xF0)
        {
            // 四字节字符
            if (i + 3 >= len ||
                (static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80 ||
                (static_cast<unsigned char>(s[i + 2]) & 0xC0) != 0x80 ||
                (static_cast<unsigned char>(s[i + 3]) & 0xC0) != 0x80)
            {
                ++count;
                ++i;
            }
            else
            {
                ++count;
                i += 4;
            }
        }
        else
        {
            // 其他非法字节
            ++count;
            ++i;
        }
    }
    return count;
}
