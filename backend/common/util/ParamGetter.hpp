#pragma once

#include <optional>
#include <json/value.h>
#include <trantor/utils/Logger.h>
#include "common/exception/BusinessException.h"

namespace drogon_admin::util
{

template <typename T, bool is_necessary>
class ParamGetter
{
  public:
    auto operator()(const Json::Value &json,
                    const std::string &key,
                    const std::pair<int32_t, int32_t> &range = {-1, -1})
    {
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
                return std::optional<T>(std::nullopt);
            }
        }

        // 获取参数，内部会检查字符串长度、数值范围
        auto result = getParam<T>(json, key, range);

        // 非必填参数，返回optional
        if constexpr (!is_necessary)
        {
            return result;
        }
        else
        {
            // 必填参数检查是否有值
            if (!result)
            {
                throw BusinessException{"必备参数" + key +
                                        "存在，但是类型错误"};
            }
            return *result;
        }
    }

  private:
    // length_range 表示期望的字符串长度范围，采用闭区间
    // 如果传入负值，表示不限制
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
            static_cast<int32_t>(value.length()) < length_range.first;
        const bool isTooLong =
            length_range.second >= 0 &&
            static_cast<int32_t>(value.length()) > length_range.second;
        if constexpr (is_necessary)
        {
            if (isTooShort)
            {
                throw BusinessException{key + "参数长度过短"};
            }
            else if (isTooLong)
            {
                throw BusinessException{key + "参数长度过长"};
            }
        }
        else
        {
            if (isTooShort)
            {
                LOG_WARN << key + "参数长度过短，已忽略";
                return std::nullopt;
            }
            else if (isTooLong)
            {
                LOG_WARN << key + "参数长度过长，已忽略";
                return std::nullopt;
            }
        }
        return value;
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
        if (!json[key].isInt())
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

        int jsonValue = json[key].asInt();

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
        if constexpr (is_necessary)
        {
            if (isOutOfRange)
            {
                throw BusinessException{key + "的值超出了指定类型的表示范围"};
            }
            else if (isTooLow || isTooHigh)
            {
                throw BusinessException{key + "的值超出期望范围"};
            }
        }
        else if (isOutOfRange)
        {
            LOG_WARN << key + "的值超出了指定类型的表示范围，已忽略";
            return std::nullopt;
        }
        else if (isTooLow || isTooHigh)
        {
            LOG_WARN << key + "的值超出期望范围，已忽略";
            return std::nullopt;
        }
        return static_cast<D>(jsonValue);
    }
};

/**
 * @brief 伪装成函数的参数提取仿函数对象
 */
template <typename T, bool is_necessary = false>
ParamGetter<T, is_necessary> getParam;
};  // namespace drogon_admin::util
