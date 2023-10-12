#include "factorial.h"

#include <type_traits>
#include <utility>
#include <iostream>

namespace details {

template <typename T>
struct is_pair : std::false_type {};

template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};

template <typename T>
constexpr bool is_pair_v = is_pair<T>::value;

template <typename T, typename U = void>
struct is_map : std::false_type {};

template <typename T>
struct is_map<T, std::enable_if_t<
                        is_pair_v< typename std::iterator_traits< typename T::iterator >::value_type > , void
                    > > : std::true_type {};

template <typename T>
constexpr bool is_map_v = is_map<T>::value;
} //details

template <typename T, size_t EL_COUNTS = 10>
struct PrintContainer {

    template <typename U = T, std::enable_if_t<details::is_map_v<std::remove_reference_t<U>>, bool> = true>
    PrintContainer() {
        for (int i = 0; i < EL_COUNTS; ++i) {
            container[i] = fact(i);
        }
    }

    template <typename U = T, std::enable_if_t<not details::is_map_v<std::remove_reference_t<U>>, bool> = true>
    PrintContainer() {
        for (int i = 0; i < EL_COUNTS; ++i) {
            container.push_back(fact(i));
        }
    }

    ~PrintContainer() {
        container.clear();
    }

    template <typename U = T>
    std::enable_if_t<details::is_map_v<std::remove_reference_t<U>>>
    print() {
        for (const auto& [k, v] : container) {
            std::cout << k << " " << v << std::endl;
        }
    }

    template <typename U = T>
    std::enable_if_t<not details::is_map_v<std::remove_reference_t<U>>>
    print() {
        for (const auto v : container) {
            std::cout << v << std::endl;
        }
    }

private:
    T container;
};
