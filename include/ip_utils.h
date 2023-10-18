#pragma once

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>

namespace ip_utils {

namespace details {

template<typename, typename = void>
struct is_iterable : std::false_type {};
 
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()),
                                    decltype(std::declval<T>().end())>> : std::true_type {};

template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

template<typename... T>
struct is_tuple : std::false_type {};

template<typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type {};

template <typename... T>
constexpr bool is_tuple_v = is_tuple<T...>::value;

} //details

template <typename T>
auto shift_r(T v, std::size_t n) -> std::enable_if_t<std::is_integral_v<T>, int16_t> {
    T temp = v >> n;
    temp &= 255;
    return int16_t((uint8_t)temp);
}

template <typename T>
auto print_ip(T ip, std::ostream& os = std::cout) -> std::enable_if_t<std::is_integral_v<T>> {
    std::size_t n = sizeof(T);
    if (n > 0) {
        os << shift_r(ip, (n - 1) * 8);
        for (int i = 1; i < n; ++i) {
            os << "." << shift_r(ip, (n - 1 - i) * 8);
        }
    }
    os << std::endl;
}

template <typename T>
auto print_ip(T ip, std::ostream& os = std::cout) -> std::enable_if_t<std::is_same_v<std::string, std::remove_reference_t<T>> 
                                        || std::is_same_v<char *, std::remove_reference_t<T>> 
                                        || std::is_same_v<const char *, std::remove_reference_t<T>>> {
    os << ip << std::endl;
}

template <typename T>
auto print_ip(T ip, std::ostream& os = std::cout) -> std::enable_if_t<not std::is_same_v<std::string, std::remove_reference_t<T>>
                                            && details::is_iterable_v<T>> {
    if (!ip.empty()) {
        os << *ip.begin();
        std::for_each(std::next(ip.begin()), ip.end(), [&os] (auto& value) {
            os << "." << value;
        });
        os << std::endl;
    }
}


template<std::size_t N = 0, typename... T>
auto print_tuple(std::tuple<T...>& v, std::ostream& os = std::cout) -> std::enable_if_t<N == sizeof...(T)> {
    os << std::endl;
}

template<std::size_t N = 0, typename... T>
auto print_tuple(std::tuple<T...>& v, std::ostream& os = std::cout) -> std::enable_if_t<N < sizeof...(T)> {
    if constexpr (N == 0) {
        os << std::get<N>(v);
    } else {
        static_assert(std::is_same_v<decltype(std::get<N - 1>(v)), decltype(std::get<N>(v))>
                                                        , "std::tuple contains different types");
        os << "." << std::get<N>(v);
    }
    print_tuple<N + 1, T...>(v, os);
}

template <typename T>
auto print_ip(T ip, std::ostream& os = std::cout) -> std::enable_if_t<details::is_tuple_v<T>> {
    print_tuple(ip, os);
}

} //ip_utils