#pragma once

#include <string>
#include <cstdint>

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

/**
 * @brief 判断字符串是否是整数
 *
 * @param[in] str 要检查的字符串
 * @return true 字符串为合法整数；false 不是合法整数
 *
 * @details
 * 等价正则（整串完全匹配）：
 * @verbatim
 ^[+-]?(?:[1-9]\d*|0)$
 @endverbatim
 *
 * @note 状态机状态说明：
 * @li 0: 初始状态，未读取任何输入
 * @li 1: 读到了正负号：+或者-
 * @li 2: 读到了非零数字，后续可以读取更多的数字
 * @li 3: 读到前导0，不允许后续再有任何字符
 */
static bool isInteger(const std::string_view &str)
{
    uint8_t status = 0;

    for (char c : str)
    {
        switch (status)
        {
            case 0:
                // [+-]
                if (c == '+' || c == '-')
                {
                    status = 1;
                }
                else
                    [[fallthrough]];
            case 1:
                // [+-]?[1-9]
                if (c >= '1' && c <= '9')
                {
                    status = 2;
                }
                // [+-]?0
                else if (c == '0')
                {
                    status = 3;
                }
                else
                {
                    return false;
                }
                break;
            case 2:
                if (c < '0' || c > '9')
                {
                    return false;
                }
                // [+-]?[1-9]\d*
                break;
            case 3:
                // 上一次读到了前导0，无视这一次的输入
                return false;
        }
    }
    // 2: [+-]?[1-9]\d*
    // 3: [+-]?0
    return status == 2 || status == 3;
}
