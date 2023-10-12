#include <algorithm>

namespace details {
    template <typename T>
    struct is_container: std::false_type {};

    template <typename T, typename Alloc>
    struct is_container<List<T, Alloc>>: std::true_type {};

    template <typename T>
    struct is_container_map: std::false_type {};

    template <typename Key, typename T, typename Less, typename Alloc>
    struct is_container_map<std::map<Key, T, Less, Alloc>>: std::true_type {};

} // details

template <class Container, class = std::enable_if_t<details::is_container<Container>::value>>
std::ostream& operator<<(std::ostream& os, const Container& container) {
    if (!container.empty()) {
        std::cout << *container.begin() << std::endl;
        std::for_each(std::next(container.begin()), container.end(), [] (auto& value) {
            std::cout << " " << value << std::endl;
        });
    }
    return os;
}

template <class Container, class = std::enable_if_t<details::is_container_map<Container>::value>>
std::ostream& operator<<(std::ostream& os, const Container& container) {
    if (!container.empty()) { 
        auto& pair = *container.begin();
        std::cout << pair.first << " " << pair.second << std::endl;
        std::for_each(std::next(container.begin()), container.end(), [] (const auto& pair) {
            std::cout << pair.first << " " << pair.second << std::endl;
        });
    }
    return os;
}
