#pragma once

#include <string>

#define OPT_SETTER(field, Field)                                 \
    void set##Field(const decltype(field##_)::value_type &field) \
    {                                                            \
        field##_ = field;                                        \
    }                                                            \
    void set##Field##ToNullOpt()                                 \
    {                                                            \
        field##_ = std::nullopt;                                 \
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

#define ENTITY_SET(entity, Field, ...)                    \
    do                                                    \
    {                                                     \
        if (request.get##Field() &&                       \
            entity.get##Field() != *request.get##Field()) \
        {                                                 \
            entity.set##Field(*request.get##Field());     \
            __VA_ARGS__;                                  \
        }                                                 \
    } while (0)

#define INIT(field, Field)        \
    field##_                      \
    {                             \
        model.getValueOf##Field() \
    }

#define ENUM_INIT(type, field, Field)                \
    field##_                                         \
    {                                                \
        static_cast<type>(model.getValueOf##Field()) \
    }

#define OPT_INIT(field, Field)                              \
    field##_                                                \
    {                                                       \
        model.get##Field() != nullptr                       \
            ? std::make_optional(model.getValueOf##Field()) \
            : std::nullopt                                  \
    }

#define AUDITABLE_INIT createdBy, Date::now(), createdBy, Date::now()

#define AUDITABLE_INIT_BY_MODEL                                     \
    model.getValueOfCreatedBy(), model.getValueOfCreatedTime(),     \
        model.getValueOfUpdatedBy(), model.getValueOfUpdatedTime(), \
        model.getDeletedBy() != nullptr                             \
            ? std::make_optional(model.getValueOfDeletedBy())       \
            : std::nullopt,                                         \
        model.getDeletedTime() != nullptr                           \
            ? std::make_optional(model.getValueOfDeletedTime())     \
            : std::nullopt

#define SET_OPT(field, Field)            \
    do                                   \
    {                                    \
        if (field##_)                    \
        {                                \
            model.set##Field(*field##_); \
        }                                \
    } while (0)

#define SET_VAL(field, Field) model.set##Field(field##_)

#define SET_VAL_CAST(type, field, Field) \
    model.set##Field(static_cast<type>(field##_))

template <typename T>
std::string toString(const T &);
