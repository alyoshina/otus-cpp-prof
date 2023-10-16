#include "factorial.h"

#include <algorithm>
#include <map>

namespace details {
    template <typename T>
    struct is_container: std::false_type {};

    template <typename T, typename Alloc>
    struct is_container<List<T, Alloc>>: std::true_type {};

    template <typename T>
    struct is_container_map: std::false_type {};

    template <typename Key, typename T, typename Less, typename Alloc>
    struct is_container_map<std::map<Key, T, Less, Alloc>>: std::true_type {};

    template <typename T, typename U = void>
    using IfContainer = std::enable_if_t<details::is_container<T>::value, U>;
    template <typename T, typename U = void>
    using IfMap = std::enable_if_t<details::is_container_map<T>::value, U>;

} // details

template <typename Container>
details::IfContainer<Container, std::ostream&> operator<<(std::ostream& os, const Container& container) {
    if (!container.empty()) {
        std::cout << *container.begin();
        std::for_each(std::next(container.begin()), container.end(), [] (auto& value) {
            std::cout << ", " << value;
        });
    }
    return os;
}

template <typename Container>
details::IfMap<Container, std::ostream&> operator<<(std::ostream& os, const Container& container) {
    if (!container.empty()) { 
        auto& pair = *container.begin();
        std::cout << pair.first << " " << pair.second;
        std::for_each(std::next(container.begin()), container.end(), [] (const auto& pair) {
            std::cout << ", " << pair.first << " " << pair.second;
        });
    }
    return os;
}

template <typename Container>
details::IfMap<Container> factorial_fill(Container& container, const int n = 10) {
    for (int i = 0; i < n; ++i) {
        container[i] = fact(i);
    }
}

template <typename Container>
details::IfContainer<Container> factorial_fill(Container& container, const int n = 10) {
    for (int i = 0; i < n; ++i) {
        container.push_back(fact(i));
    }
}