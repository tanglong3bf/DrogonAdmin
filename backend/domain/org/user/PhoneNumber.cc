#include "PhoneNumber.h"

#include <algorithm>

using namespace std;

PhoneNumber::PhoneNumber(const string &value) : value_{value}
{
}

PhoneNumber::operator string() const
{
    return value_;
}

const string &PhoneNumber::value() const
{
    return value_;
}

bool PhoneNumber::valid() const
{
    return value_.size() == 11 && value_.starts_with("1") &&
           all_of(value_.begin() + 1, value_.end(), ::isdigit);
}

bool PhoneNumber::operator==(const PhoneNumber &rhs) const
{
    return value_ == rhs.value_;
}
