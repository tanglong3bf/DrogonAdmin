#pragma once

#include <string>

class Email
{
  public:
    Email(const std::string &value);

    operator std::string() const;
    const std::string &value() const;

    bool operator==(const Email &rhs) const;

  protected:
    bool valid() const;

  private:
    std::string value_;
};
