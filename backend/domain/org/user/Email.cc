#include "Email.h"

using namespace std;

Email::Email(const string &value) : value_{value}
{
}

Email::operator string() const
{
    return value_;
}

const string &Email::value() const
{
    return value_;
}

bool Email::valid() const
{
    const string &s = value_;

    // 非空且至少含一个 '@'
    if (s.empty() || s.find('@') == string::npos)
    {
        return false;
    }

    // 只能有一个 '@'
    size_t at_pos = s.find('@');
    if (s.find('@', at_pos + 1) != string::npos)
    {
        return false;
    }

    // '@' 不能在开头或结尾
    if (at_pos == 0 || at_pos == s.length() - 1)
    {
        return false;
    }

    // 分割 local-part 和 domain
    string local = s.substr(0, at_pos);
    string domain = s.substr(at_pos + 1);

    // local-part 和 domain 均不能为空
    if (local.empty() || domain.empty())
    {
        return false;
    }

    // local-part 不能以 '.' 或 '-' 开头/结尾，也不能有连续的 '.'
    if (local.front() == '.' || local.back() == '.' || local.front() == '-' ||
        local.back() == '-')
    {
        return false;
    }
    if (local.find("..") != string::npos)
    {
        return false;
    }

    // domain 不能以 '.' 或 '-' 开头/结尾，不能有连续的 '.'
    if (domain.front() == '.' || domain.back() == '.' ||
        domain.front() == '-' || domain.back() == '-')
    {
        return false;
    }
    if (domain.find("..") != string::npos)
    {
        return false;
    }

    // 检查 local-part
    // 字符：允许字母、数字、点、下划线、百分号、加号、减号（常见扩展）
    for (char c : local)
    {
        if (!isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '_' &&
            c != '%' && c != '+' && c != '-')
        {
            return false;
        }
    }

    // 检查 domain 字符：允许字母、数字、点、减号（且减号不能在标签边界）
    size_t dot_pos = 0;
    while ((dot_pos = domain.find('.', dot_pos)) != string::npos)
    {
        size_t prev = dot_pos > 0 ? dot_pos - 1 : string::npos;
        size_t next =
            dot_pos + 1 < domain.length() ? dot_pos + 1 : string::npos;

        // 点前后不能是 '-'，也不能是开头/结尾（已检查过首尾）
        if (prev != string::npos && domain[prev] == '-')
        {
            return false;
        }
        if (next != string::npos && domain[next] == '-')
        {
            return false;
        }
        ++dot_pos;
    }

    // domain 中每个标签（由 '.' 分隔）必须非空且只含合法字符
    size_t start = 0;
    while (start < domain.length())
    {
        size_t end = domain.find('.', start);
        if (end == string::npos)
        {
            end = domain.length();
        }
        string label = domain.substr(start, end - start);
        if (label.empty())
        {
            return false;
        }
        for (char c : label)
        {
            if (!isalnum(static_cast<unsigned char>(c)) && c != '-')
            {
                return false;
            }
        }
        start = end + 1;
    }

    // 至少有一个 '.' 在 domain 中（即必须是形如 example.com，而非纯 hostname）
    if (domain.find('.') == string::npos)
    {
        return false;
    }

    return true;
}

bool Email::operator==(const Email &rhs) const
{
    return value_ == rhs.value_;
}
