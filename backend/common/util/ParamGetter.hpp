#pragma once

#include "common/exception/BusinessException.h"
#include "common/util/ParamUtils.hpp"
#include <trantor/utils/Logger.h>
#include <json/value.h>
#include <algorithm>
#include <limits>
#include <optional>
#include <type_traits>
#include <variant>

namespace drogon_admin::util
{

/// 辅助类型 trait，判断是否是整数类型的vector
/// @{
template <typename T>
struct is_vector_of_integral : std::false_type
{
};

template <typename IntegralType>
struct is_vector_of_integral<std::vector<IntegralType>>
    : std::conjunction<std::is_integral<IntegralType>,
                       std::negation<std::is_same<IntegralType, bool>>>
{
};

template <typename T>
inline constexpr bool is_vector_of_integral_v = is_vector_of_integral<T>::value;

/// @}

/**
 * @brief 可空值类型，用于区分三种状态：不存在、存在但为null、存在且有值
 * @tparam T 值的类型
 */
template <typename T>
class NullableValue
{
    static_assert(!std::is_reference_v<T>,
                  "NullableValue cannot hold references");
    static_assert(!std::is_void_v<T>, "Use NullableValue<void> for void type");

  public:
    NullableValue() = default;

    [[nodiscard]] static constexpr NullableValue absent() noexcept
    {
        return NullableValue(std::monostate{});
    }

    [[nodiscard]] static constexpr NullableValue null() noexcept
    {
        return NullableValue(NullTag{});
    }

    [[nodiscard]] static constexpr NullableValue value(T &&val) noexcept(
        std::is_nothrow_move_constructible_v<T>)
    {
        return NullableValue(std::forward<T>(val));
    }

    [[nodiscard]] static constexpr NullableValue value(const T &val) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
    {
        return NullableValue(val);
    }

    template <
        typename U,
        typename = std::enable_if_t<!std::is_same_v<U, T> &&
                                    std::is_constructible_v<T, const U &>>>
    constexpr explicit(!std::is_convertible_v<const U &, T>)
        NullableValue(const NullableValue<U> &other) noexcept(
            std::is_nothrow_constructible_v<T, const U &>)
        : data_()
    {
        if (other.isAbsent())
        {
            data_.template emplace<std::monostate>();
        }
        else if (other.isNull())
        {
            data_.template emplace<NullTag>();
        }
        else
        {
            data_.template emplace<T>(other.value());
        }
    }

    template <typename U,
              typename = std::enable_if_t<!std::is_same_v<U, T> &&
                                          std::is_constructible_v<T, U &&>>>
    constexpr explicit(!std::is_convertible_v<U &&, T>)
        NullableValue(NullableValue<U> &&other) noexcept(
            std::is_nothrow_constructible_v<T, U &&>)
        : data_()
    {
        if (other.isAbsent())
        {
            data_.template emplace<std::monostate>();
        }
        else if (other.isNull())
        {
            data_.template emplace<NullTag>();
        }
        else
        {
            data_.template emplace<T>(std::move(other).value());
        }
    }

    [[nodiscard]] constexpr bool isAbsent() const noexcept
    {
        return std::holds_alternative<std::monostate>(data_);
    }

    [[nodiscard]] constexpr bool isNull() const noexcept
    {
        return std::holds_alternative<NullTag>(data_);
    }

    [[nodiscard]] constexpr bool hasValue() const noexcept
    {
        return std::holds_alternative<T>(data_);
    }

    template <typename D>
    [[nodiscard]] constexpr NullableValue<D> to() const
        noexcept(std::is_nothrow_constructible_v<D, const T &>)
    {
        return NullableValue<D>(*this);
    }

    [[nodiscard]] constexpr const T &value() const &
    {
        if (!hasValue())
        {
            throw std::bad_variant_access{};
        }
        return std::get<T>(data_);
    }

    [[nodiscard]] constexpr T &value() &
    {
        if (!hasValue())
        {
            throw std::bad_variant_access{};
        }
        return std::get<T>(data_);
    }

    [[nodiscard]] constexpr T &&value() &&
    {
        if (!hasValue())
        {
            throw std::bad_variant_access{};
        }
        return std::move(std::get<T>(data_));
    }

    [[nodiscard]] constexpr const T &operator*() const & noexcept
    {
        return *std::get_if<T>(&data_);
    }

    [[nodiscard]] constexpr T &operator*() & noexcept
    {
        return *std::get_if<T>(&data_);
    }

    [[nodiscard]] constexpr T &&operator*() && noexcept
    {
        return std::move(*std::get_if<T>(&data_));
    }

    [[nodiscard]] constexpr const T *operator->() const noexcept
    {
        return std::get_if<T>(&data_);
    }

    [[nodiscard]] constexpr T *operator->() noexcept
    {
        return std::get_if<T>(&data_);
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept
    {
        return hasValue();
    }

    template <typename U>
    [[nodiscard]] constexpr T valueOr(U &&defaultValue) const &
    {
        return hasValue() ? value()
                          : static_cast<T>(std::forward<U>(defaultValue));
    }

    template <typename U>
    [[nodiscard]] constexpr T valueOr(U &&defaultValue) &&
    {
        return hasValue() ? std::move(value())
                          : static_cast<T>(std::forward<U>(defaultValue));
    }

  private:
    struct NullTag
    {
    };

    std::variant<std::monostate, NullTag, T> data_{std::monostate{}};

    constexpr explicit NullableValue(std::monostate) noexcept
        : data_(std::monostate{})
    {
    }

    constexpr explicit NullableValue(NullTag) noexcept : data_(NullTag{})
    {
    }

    constexpr explicit NullableValue(const T &val) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
        : data_(val)
    {
    }

    constexpr explicit NullableValue(T &&val) noexcept(
        std::is_nothrow_move_constructible_v<T>)
        : data_(std::move(val))
    {
    }
};

/**
 * @brief 默认参数验证器
 */
struct DefaultParamValidator
{
    template <typename T>
    constexpr std::string operator()(const T &) const noexcept
    {
        return "";
    }
};

/**
 * @brief 参数提取仿函数对象
 * @tparam T 期望的参数类型
 * @tparam is_necessary 是否为必填参数，默认为false
 * @tparam is_nullable 是否允许参数值为null，默认为false
 */
template <typename T, bool is_necessary, bool is_nullable>
class ParamGetter
{
  public:
    template <typename Validator = DefaultParamValidator>
    auto operator()(const Json::Value &json,
                    const std::string &key,
                    const std::pair<int32_t, int32_t> &range = {-1, -1},
                    Validator validator = DefaultParamValidator{})
    {
        static_assert(noexcept(validator(std::declval<const T &>())),
                      "Validator must be a non-throwing callable! Mark it with "
                      "`noexcept`.");
        assert(range.first < 0 || range.second < 0 ||
               range.first <= range.second);

        // 缺少指定参数
        if (!json.isMember(key))
        {
            if constexpr (is_necessary)
            {
                throw BusinessException{"缺少必备参数：" + key};
            }
            else
            {
                if constexpr (is_nullable)
                {
                    return NullableValue<T>::absent();
                }
                else
                {
                    return std::optional<T>(std::nullopt);
                }
            }
        }

        // 处理 null 值情况
        if (json[key].isNull())
        {
            if constexpr (is_nullable)
            {
                // 允许 null，返回 null 状态
                return NullableValue<T>::null();
            }
            else
            {
                // 不允许 null，视为类型错误
                if constexpr (is_necessary)
                {
                    throw BusinessException{"参数" + key + "不允许为null"};
                }
                else
                {
                    LOG_WARN << "参数" + key + "为null，已忽略";
                    return std::optional<T>(std::nullopt);
                }
            }
        }

        // 获取参数，内部会检查字符串长度、数值范围
        auto result = getParam<T>(json, key, range);

        if (result)
        {
            // 调用验证器
            const auto err = validator(*result);
            if (err.length() > 0)
            {
                throw BusinessException{err};
            }
        }

        // 根据 is_nullable 决定返回值类型
        if constexpr (is_nullable)
        {
            if constexpr (is_necessary)
            {
                // 必填参数检查是否有值
                if (!result)
                {
                    throw BusinessException{"必备参数" + key +
                                            "存在，但是类型错误或值非法"};
                }
                return NullableValue<T>::value(std::move(*result));
            }
            else
            {
                if (result)
                {
                    return NullableValue<T>::value(std::move(*result));
                }
                else
                {
                    // 类型错误或值非法，返回 absent
                    return NullableValue<T>::absent();
                }
            }
        }
        else
        {
            // 非必填返回 optional，必填返回值
            if constexpr (!is_necessary)
            {
                return result;
            }
            else
            {
                if (!result)
                {
                    throw BusinessException{"必备参数" + key +
                                            "存在，但是类型错误或值非法"};
                }
                return *result;
            }
        }
    }

  private:
    // 字符串类型参数
    template <typename D>
    std::enable_if_t<std::is_same_v<D, std::string>, std::optional<D>> getParam(
        const Json::Value &json,
        const std::string &key,
        const std::pair<int32_t, int32_t> &length_range)
    {
        // 类型检查
        if (!json[key].isString())
        {
            if constexpr (is_necessary)
            {
                throw BusinessException{key + "必须是一个字符串"};
            }
            else
            {
                LOG_WARN << key + "类型错误，已忽略";
                return std::nullopt;
            }
        }

        const std::string value = json[key].asString();

        // 长度检查
        const bool isTooShort =
            length_range.first >= 0 &&
            static_cast<int32_t>(utf8Length(value)) < length_range.first;
        const bool isTooLong =
            length_range.second >= 0 &&
            static_cast<int32_t>(utf8Length(value)) > length_range.second;
        if (isTooShort)
        {
            throw BusinessException{key + "参数长度过短"};
        }
        else if (isTooLong)
        {
            throw BusinessException{key + "参数长度过长"};
        }
        return value;
    }

    // bool类型参数
    template <typename D>
    std::enable_if_t<std::is_same_v<D, bool>, std::optional<D>> getParam(
        const Json::Value &json,
        const std::string &key,
        const std::pair<int32_t, int32_t> & /*ignore*/)
    {
        // 类型检查
        if (!json[key].isBool())
        {
            if constexpr (is_necessary)
            {
                throw BusinessException{key + "必须是一个布尔值"};
            }
            else
            {
                LOG_WARN << key + "类型错误，已忽略";
                return std::nullopt;
            }
        }
        return json[key].asBool();
    }

    /**
     * 整数类型参数
     * value_range：考虑到整数类型参数一般只会是关联数据id，限制关联数据数量等
     * 情况，暂只考虑非负数
     */
    template <typename D>
    std::enable_if_t<std::is_integral_v<D> && !std::is_same_v<D, bool>,
                     std::optional<D>>
    getParam(const Json::Value &json,
             const std::string &key,
             const std::pair<int32_t, int32_t> &value_range)
    {
        // 类型检查
        if (!json[key].isInt64())
        {
            if constexpr (is_necessary)
            {
                throw BusinessException{key + "必须是一个整数"};
            }
            else
            {
                LOG_WARN << key + "类型错误，已忽略";
                return std::nullopt;
            }
        }

        const int64_t jsonValue = json[key].asInt64();

        // 符号检查
        if constexpr (std::is_unsigned_v<D>)
        {
            if (jsonValue < 0)
            {
                throw BusinessException{key + "必须是一个非负整数"};
            }
        }

        // 大小检查
        // 超出了指定类型的表达范围
        const bool isOutOfRange =
            jsonValue < std::numeric_limits<D>::lowest() ||
            jsonValue > std::numeric_limits<D>::max();
        // 比期望的最小值更低
        const bool isTooLow =
            value_range.first >= 0 && jsonValue < value_range.first;
        // 比期望的最大值更高
        const bool isTooHigh =
            value_range.second >= 0 && jsonValue > value_range.second;
        if (isOutOfRange)
        {
            throw BusinessException{key + "的值超出了指定类型的表示范围"};
        }
        else if (isTooLow || isTooHigh)
        {
            throw BusinessException{key + "的值超出期望范围"};
        }
        return static_cast<D>(jsonValue);
    }

    template <typename D>
    std::enable_if_t<is_vector_of_integral_v<D>, std::optional<D>> getParam(
        const Json::Value &json,
        const std::string &key,
        const std::pair<int32_t, int32_t> &value_range)
    {
        // 类型检查
        if (!json[key].isArray())
        {
            if constexpr (is_necessary)
            {
                throw BusinessException{key + "必须是一个数组"};
            }
            else
            {
                LOG_WARN << key + "类型错误，已忽略";
                return std::nullopt;
            }
        }

        const Json::Value array = json[key];

        if (array.empty())
        {
            // 允许返回空数组
            return std::make_optional(D{});
        }

        for (const auto &element : array)
        {
            if (!element.isInt64())
            {
                throw BusinessException{"数组" + key + "的元素必须是整数"};
            }
        }

        // 符号检查
        if constexpr (std::is_unsigned_v<typename D::value_type>)
        {
            bool hasNegative = false;
            for (const auto &element : array)
            {
                if (element.asInt64() < 0)
                {
                    hasNegative = true;
                    break;
                }
            }
            if (hasNegative)
            {
                throw BusinessException{key + "必须是一个非负整数数组"};
            }
        }

        std::vector<int64_t> temp;
        temp.reserve(array.size());
        for (const auto &element : array)
        {
            temp.push_back(element.asInt64());
        }

        // 大小检查
        // 部分值超出了指定类型的表达范围
        const bool hasOutOfRange =
            std::ranges::any_of(temp, [](int64_t jsonValue) {
                const auto tooLow =
                    jsonValue <
                    std::numeric_limits<typename D::value_type>::lowest();
                const auto tooHigh =
                    jsonValue >
                    std::numeric_limits<typename D::value_type>::max();
                return tooLow || tooHigh;
            });
        // 部分值比期望的最小值更低
        const bool hasTooLow =
            std::ranges::any_of(temp, [&](int32_t jsonValue) {
                return value_range.first >= 0 && jsonValue < value_range.first;
            });
        // 部分值比期望的最大值更高
        const bool hasTooHigh =
            std::ranges::any_of(temp, [&](int32_t jsonValue) {
                return value_range.second >= 0 &&
                       jsonValue > value_range.second;
            });
        if constexpr (is_necessary)
        {
            if (hasOutOfRange)
            {
                throw BusinessException{key +
                                        "的部分值超出了指定类型的表示范围"};
            }
            else if (hasTooLow || hasTooHigh)
            {
                throw BusinessException{key + "的部分值超出期望范围"};
            }
        }
        else if (hasOutOfRange)
        {
            LOG_WARN << key + "的部分值超出了指定类型的表示范围，已忽略";
            return std::nullopt;
        }
        else if (hasTooLow || hasTooHigh)
        {
            LOG_WARN << key + "的部分值超出期望范围，已忽略";
            return std::nullopt;
        }

        // 转换为目标类型
        D result;
        result.reserve(temp.size());
        for (const auto &element : temp)
        {
            result.push_back(static_cast<typename D::value_type>(element));
        }
        return result;
    }
};

/**
 * @brief 伪装成函数的参数提取仿函数对象
 */
template <typename T, bool is_necessary = false, bool is_nullable = false>
ParamGetter<T, is_necessary, is_nullable> getParam;
};  // namespace drogon_admin::util
