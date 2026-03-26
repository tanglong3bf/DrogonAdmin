#pragma once

#include <string>

#define OPT_SETTER(field, Field)                                 \
    void set##Field(const decltype(field##_)::value_type &field) \
    {                                                            \
        field##_ = field;                                        \
    }

#define OPT_GETTER(field, Field)   \
    const auto &get##Field() const \
    {                              \
        return field##_;           \
    }

#define SETTER(field, Field)                         \
    void set##Field(const decltype(field##_) &field) \
    {                                                \
        field##_ = field;                            \
    }

#define GETTER(field, Field)       \
    const auto &get##Field() const \
    {                              \
        return field##_;           \
    }

template <typename T>
std::string toString(const T &);
