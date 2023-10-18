#pragma once

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>

/**
* @brief namespace contains template functions print ip address
*
*/
namespace ip_utils {

/**
* @brief namespace to determine what type
*/
namespace details {

/**
* @brief is_iterable is struct that identifies whether T is iterable type
* It inherits from std::false_type or std::true_type, depending on whether T is iterable type
*/
template<typename, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()),
                                    decltype(std::declval<T>().end())>> : std::true_type {};

/**
* @brief
* Checks whether T is a iterable type. Provides the member constant value that is equal to true, if T is the iterable type.
* Otherwise, value is equal to false. 
*/
template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

/**
* @brief is_tuple is struct that identifies whether T is tuple type
* It inherits from std::false_type or std::true_type, depending on whether T is tuple type
*/
template<typename... T>
struct is_tuple : std::false_type {};

template<typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type {};

/**
* @brief
* Checks whether T is a tuple type. Provides the member constant value that is equal to true, if T is the type tuple. Otherwise, value is equal to false. 
*/
template <typename... T>
constexpr bool is_tuple_v = is_tuple<T...>::value;

} //details

/**
* @brief bit shift value right
*
* @param[in] parent value for right shift
* @param[in] parent number of shift positions
* @param[out] parent shift value
* function is instantiated for integral types. T - a type to check
*/
template <typename T>
auto shift_r(T v, std::size_t n) -> std::enable_if_t<std::is_integral_v<T>, int16_t> {
    T temp = v >> n;
    temp &= 255;
    return int16_t((uint8_t)temp);
}

/**
* @brief The function of printing a IP address
*
* @param[in] parent integral IP address value
* @param[in] parent std::ostream
* The function is instantiated for integral types. T - a type to check
*/
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

/**
* @brief The function of printing a IP address
*
* @param[in] parent std::string, char * IP address value
* @param[in] parent std::ostream
* The function is instantiated for string types. T - a type to check
*/
template <typename T>
auto print_ip(T ip, std::ostream& os = std::cout) -> std::enable_if_t<std::is_same_v<std::string, std::remove_reference_t<T>> 
                                        || std::is_same_v<char *, std::remove_reference_t<T>> 
                                        || std::is_same_v<const char *, std::remove_reference_t<T>>> {
    os << ip << std::endl;
}

/**
* @brief The function of printing a IP address
*
* @param[in] parent iterable container IP address value
* @param[in] parent std::ostream
* The function is instantiated for iterable container (not string) types. T - a type to check
*/
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

/**
* @brief The function of end tuple
*
* @param[in] parent tuple with IP address value
* @param[in] parent std::ostream
* The function is instantiated for tuple types. T - a type to check. N - item number in tuple
*/
template<std::size_t N = 0, typename... T>
auto print_tuple(std::tuple<T...>& v, std::ostream& os = std::cout) -> std::enable_if_t<N == sizeof...(T)> {
    os << std::endl;
}

/**
* @brief The function of printing a tuple N-element
*
* @param[in] parent tuple with IP address value
* @param[in] parent std::ostream
* The function is instantiated for tuple types. T - a type to check. N - item number in tuple.
* The check contains a tuple of different types. The compiler is to issue an error if tuple contains items of different types
*/
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

/**
* @brief The function of printing a IP address
*
* @param[in] parent IP address value, tuple that contains the same types
* @param[in] parent std::ostream
* The function is instantiated for tuple types. T - a type to check
*/
template <typename T>
auto print_ip(T ip, std::ostream& os = std::cout) -> std::enable_if_t<details::is_tuple_v<T>> {
    print_tuple(ip, os);
}

} //ip_utils