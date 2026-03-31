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

#define INIT(field, Field)  \
    field##_                      \
    {                             \
        model.getValueOf##Field() \
    }

#define ENUM_INIT(type, field, Field)          \
    field##_                                         \
    {                                                \
        static_cast<type>(model.getValueOf##Field()) \
    }

#define OPT_INIT(field, Field)                   \
    field##_                                           \
    {                                                  \
        model.get##Field() != nullptr                  \
            ? make_optional(model.getValueOf##Field()) \
            : nullopt                                  \
    }

#define AUDITABLE_INIT                                              \
    model.getValueOfCreatedBy(), model.getValueOfCreatedTime(),     \
        model.getValueOfUpdatedBy(), model.getValueOfUpdatedTime(), \
        model.getDeletedBy() != nullptr                             \
            ? std::make_optional(model.getValueOfDeletedBy())       \
            : std::nullopt,                                         \
        model.getDeletedTime() != nullptr                           \
            ? std::make_optional(model.getValueOfDeletedTime())     \
            : std::nullopt

template <typename T>
std::string toString(const T &);
