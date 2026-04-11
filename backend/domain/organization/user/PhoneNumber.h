#pragma once

#include <string>

class PhoneNumber
{
  public:
    PhoneNumber(const std::string &value);

    operator std::string() const;
    const std::string &value() const;

    bool operator==(const PhoneNumber &rhs) const;

  protected:
    bool valid() const;

  private:
    std::string value_;
};
