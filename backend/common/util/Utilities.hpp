#pragma once

#define GETTER(field)                  \
    const auto &field() const noexcept \
    {                                  \
        return field##_;               \
    }

#define ENTITY_SET(entity, field, ...)                           \
    do                                                           \
    {                                                            \
        if (request.field() && entity.field != *request.field()) \
        {                                                        \
            entity.field = *request.field();                     \
            __VA_ARGS__;                                         \
        }                                                        \
    } while (0)

#define INIT(field, Field)        \
    field                         \
    {                             \
        model.getValueOf##Field() \
    }

#define ENUM_INIT(type, field, Field)                \
    field                                            \
    {                                                \
        static_cast<type>(model.getValueOf##Field()) \
    }

#define OPT_INIT(field, Field)                              \
    field                                                   \
    {                                                       \
        model.get##Field() != nullptr                       \
            ? std::make_optional(model.getValueOf##Field()) \
            : std::nullopt                                  \
    }

// OV: Object Value
#define OPT_OV_INIT(type, field, Field)                           \
    field                                                         \
    {                                                             \
        model.get##Field() != nullptr                             \
            ? std::make_optional(type{model.getValueOf##Field()}) \
            : std::nullopt                                        \
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

#define SET_OPT(field, Field)         \
    do                                \
    {                                 \
        if (field)                    \
        {                             \
            model.set##Field(*field); \
        }                             \
    } while (0)

#define SET_VAL(field, Field) model.set##Field(field)

#define SET_OPT_OV(field, Field)              \
    do                                        \
    {                                         \
        if (field)                            \
        {                                     \
            model.set##Field(field->value()); \
        }                                     \
    } while (0)

#define SET_VAL_CAST(type, field, Field) \
    model.set##Field(static_cast<type>(field))
