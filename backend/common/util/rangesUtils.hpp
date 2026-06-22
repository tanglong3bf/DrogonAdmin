#include <ranges>
#include <concepts>
#include <iterator>
#include <utility>

namespace tl::ranges_utils
{
template <typename C, typename R>
concept constructible_from_range =
    std::ranges::common_range<R> &&
    std::constructible_from<C,
                            std::ranges::iterator_t<R>,
                            std::ranges::sentinel_t<R>>;

template <typename C, typename R>
concept insertable_range =
    std::ranges::input_range<R> && std::default_initializable<C> &&
    requires(C &c, R &&r) {
        std::copy(std::forward<R>(r), std::inserter(c, c.end()));
    };

// 连续内存转 string_view 特化
template <typename C, std::ranges::contiguous_range R>
    requires std::same_as<C,
                          std::basic_string_view<typename C::value_type,
                                                 typename C::traits_type>> &&
             std::same_as<std::ranges::range_value_t<R>, typename C::value_type>
C to_impl(R &&rng)
{
    return C(std::ranges::data(rng), std::ranges::size(rng));
}

// 迭代器对构造
template <typename C, std::ranges::input_range R>
    requires constructible_from_range<C, R>
C to_impl(R &&rng)
{
    return C(std::ranges::begin(rng), std::ranges::end(rng));
}

// 默认构造 + insert 填充，支持 reserve 优化
template <typename C, std::ranges::input_range R>
    requires(!constructible_from_range<C, R>) && insertable_range<C, R>
C to_impl(R &&rng)
{
    C c;
    if constexpr (std::ranges::sized_range<R>)
    {
        if constexpr (requires(C &c, std::ranges::range_size_t<R> n) {
                          c.reserve(n);
                      })
        {
            c.reserve(std::ranges::size(rng));
        }
    }
    std::copy(std::forward<R>(rng), std::inserter(c, c.end()));
    return c;
}

// ==================== 管道适配器核心实现 ====================
// 闭包1：接收完整容器类型（如 std::vector<int>）
template <typename Container>
struct ToTypedClosure
{
    template <std::ranges::input_range R>
    constexpr auto operator()(R &&range) const
    {
        return to_impl<Container>(std::forward<R>(range));
    }
};

// 闭包2：接收容器模板（如 std::vector），自动推导元素类型
template <template <typename...> class ContainerTpl>
struct ToTemplateClosure
{
    template <std::ranges::input_range R>
    constexpr auto operator()(R &&range) const
    {
        using value_type = std::ranges::range_value_t<R>;
        return to_impl<ContainerTpl<value_type>>(std::forward<R>(range));
    }
};

// 管道运算符：range | 闭包
template <std::ranges::input_range R, typename Closure>
constexpr auto operator|(R &&r, Closure &&closure)
    -> decltype(std::forward<Closure>(closure)(std::forward<R>(r)))
{
    return std::forward<Closure>(closure)(std::forward<R>(r));
}

// 对外入口：两个重载的函数模板，返回对应闭包
// 重载1：传入完整容器类型 to<std::vector<int>>()
template <typename C>
constexpr ToTypedClosure<C> to()
{
    return {};
}

// 重载2：传入容器模板 to<std::vector>()
template <template <typename...> class Tpl>
constexpr ToTemplateClosure<Tpl> to()
{
    return {};
}

}  // namespace tl::ranges_utils
