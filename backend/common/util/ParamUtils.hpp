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
