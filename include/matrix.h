#pragma once

#include <cstddef>
#include <memory>
#include <map>
#include <numeric>
#include <tuple>
#include <iostream>
#include <algorithm>

namespace matrix {

/**
* @brief An infinite proxy matrix filled with values ​​according to default.
* When reading an element from a free cell, return the default value.
* @tparam T Type of array objects.
* @tparam V Default value.
* @tparam MAX_D Total array depth.
* @tparam D Current array depth.
*/
template<typename T, T V, std::size_t MAX_D, std::size_t D>
class Proxy {
    using container_value_type = Proxy<T, V, MAX_D, D-1>;
    using container_type = typename std::map<std::size_t, std::shared_ptr<container_value_type>>;
    public:
        struct Iterator   {
            container_value_type::Iterator child_it;
            container_type::iterator c_it;
            std::weak_ptr<container_type> container;
            Iterator() {}
            Iterator(std::shared_ptr<container_type> s_ptr) : container(s_ptr) {}
            Iterator(container_type::iterator it, std::shared_ptr<container_type> s_ptr)
                : c_it(it)
                , container(s_ptr) {}
            Iterator(const Iterator &other) {
                c_it = other.c_it;
                container = other.container.lock();
                child_it = other.child_it;
            }
            Iterator &operator=(const Iterator& rhs) {
                if (this != &rhs) {
                    child_it = rhs.child_it;
                    c_it = rhs.c_it;
                    container = rhs.container;
                }
                return *this;
            }
            Iterator &next(std::array<const std::size_t*, MAX_D> &indexes, T **p) {
                child_it = child_it.next(indexes, p);
                if (*p) {
                    indexes[MAX_D - D] = &(c_it->first);
                } else {
                    if (auto c = container.lock()) {
                        c_it++;
                        while (c_it != (*c).end()) {
                            child_it = c_it->second->begin(indexes, p);
                            if (*p) {
                                indexes[MAX_D - D] = &(c_it->first);
                                break;
                            } else {
                                c_it++;
                            }
                        }
                    } 
                }
                return *this;
            }
        };

        Proxy() : elements(std::make_shared<container_type>()) {}
        container_value_type &operator[] (std::size_t pos) {
            if (auto it = elements->find(pos); it != elements->end()) {
                return *(it->second.get());
            }
            return *(elements->insert({pos
                        , std::make_shared<container_value_type>()}).first->second.get());
        }
        std::size_t size() const {
            return std::accumulate(elements->begin(), elements->end(), 0
              , [] (std::size_t value, const typename container_type::value_type& p) {
                        return value + p.second->size();
                    });
        }
        void deleteDefaultValue() {
            std::for_each(elements->begin(), elements->end()
                                , [] (auto &item) { item.second->deleteDefaultValue(); });
        }
        Iterator begin(std::array<const std::size_t*, MAX_D> &indexes, T **p) {
            Iterator it(elements->begin(), elements);
            while (it.c_it != elements->end()) {
                it.child_it = it.c_it->second->begin(indexes, p);
                if (*p) {
                    indexes[MAX_D - D] = &(it.c_it->first);
                    break;
                } else {
                    it.c_it++;
                }
            }
            return it;
        }
    private:
        std::shared_ptr<container_type> elements;
};

/**
* @brief An infinite proxy matrix filled with values ​​according to default.
* When reading an element from a free cell, return the default value.
* Current array depth is 1.
* @tparam T Type of array objects.
* @tparam V Default value.
* @tparam MAX_D Total array depth.
*/
template<typename T, T V, std::size_t MAX_D>
class Proxy<T, V, MAX_D, 1> {
    using container_type = typename std::map<std::size_t, T>;
    public:
        struct Iterator {
            container_type::iterator c_it;
            std::weak_ptr<container_type> container;
            Iterator() {}
            Iterator(std::shared_ptr<container_type> s_ptr) : container(s_ptr) {}
            Iterator(container_type::iterator it, std::shared_ptr<container_type> s_ptr)
                : c_it(it)
                , container(s_ptr) {}
            Iterator(const Iterator &other)
                : c_it(other.c_it)
                , container(other.container) {}
            Iterator &operator=(const Iterator& rhs) {
                if (this != &rhs) {
                    c_it = rhs.c_it;
                    container = rhs.container;
                }               
                return *this;
            }
            Iterator& next(std::array<const std::size_t*, MAX_D> &indexes, T **p) {
                if (auto c = container.lock()) {
                    c_it++;
                    if (c_it != (*c).end()) {
                        indexes[MAX_D - 1] = &(c_it->first);
                        *p = &((*c)[c_it->first]);
                    }
                }
                return *this;
            }
        };

        Proxy() : elements(std::make_shared<container_type>()) {}
        T &operator[] (std::size_t pos) {
            if (auto it = elements->find(pos); it != elements->end()) {
                return it->second;
            }
            return elements->insert({pos, V}).first->second;
        }
        std::size_t size() const {
            return std::count_if(elements->begin(), elements->end(), 
                                        [](const auto &p) { return p.second != V; });
        }
        void deleteDefaultValue() {
            std::erase_if(*elements, [](const auto &item) { return item.second == V; });
        }
        Iterator begin(std::array<const std::size_t*, MAX_D> &indexes, T **p) {
            Iterator it(elements->begin(), elements);
            if (it.c_it != elements->end()) {
                indexes[MAX_D - 1] = &(it.c_it->first);
                *p = &((*elements)[it.c_it->first]);
            } else {
                *p = nullptr;
            }
            return it;
        }
    private:
        std::shared_ptr<container_type> elements;
};

/**
* @brief Build tuple from array.
*
* @tparam Array Array.
* @tparam AN Size of array.
* @tparam I Index element in array.
* @return Tuple from array.
*/
template <typename Array, std::size_t... I>
auto a2t_impl(Array &a, std::index_sequence<I...>) {
    return std::tie(*(a[I])...);
}
/**
* @brief Build tuple from array.
*
* @tparam AT Array element type.
* @tparam AN Size of array.
* @tparam Indices Index sequence.
* @return Tuple from array.
*/
template <typename AT, std::size_t AN, typename Indices = std::make_index_sequence<AN>>
auto a2t(std::array<AT, AN> &a) {
    return a2t_impl(a, Indices{});
}

/**
* @brief An infinite matrix filled with values ​​according to default.
* The matrix must store only occupied elements whose value is at least once were involved.
* Assigning default values ​​to a cell frees the cell.
* When reading an element from a free cell, return the default value.
* @tparam T Type of array objects.
* @tparam V Default value.
* @tparam DEPTH Array depth.
*/
template<typename T, T V, std::size_t DEPTH = 2>
class Matrix {
public:
    using container_type = Proxy<T, V, DEPTH, DEPTH>;

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        std::array<const std::size_t *, DEPTH> indexes;
        T* data {nullptr};
        std::weak_ptr<container_type> container;
        container_type::Iterator p_it;

        using type_tuple = decltype(std::tuple_cat(a2t(indexes), std::tie(*data)));
        using value_type = type_tuple;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator() { indexes.fill(0); }
        Iterator(std::shared_ptr<container_type> s_ptr) : container(s_ptr) { indexes.fill(0); }
        Iterator(const Iterator &other) {
            data = other.data;
            indexes = other.indexes;
            p_it = other.p_it;
        }
        ~Iterator() = default;
        Iterator& operator=(const Iterator& rhs) {
            data = rhs.data;
            indexes = rhs.indexes;
            p_it = rhs.p_it;
            return *this;
        }
        Iterator& operator++() {
            data = nullptr;
            indexes.fill(0);
            p_it.next(indexes, &data);
            return *this;
        }
        Iterator operator++(int) {
            Iterator iter = *this;
            ++(*this);
            return iter;
        }
        bool operator!=(const Iterator& rhs) const {
            return (data != rhs.data) || (indexes != rhs.indexes);
        }
        bool operator==(const Iterator& rhs) const {
            return (data == rhs.data) && (indexes == rhs.indexes);
        }
        struct Tuple {
            Iterator it;
            Tuple(Iterator _it) : it(_it) {}
            Tuple(const Tuple &other) { it = other.it; }
            template <typename... Ts>
            operator std::tuple<Ts...> () {
                return std::tuple_cat(a2t(it.indexes), std::tie(*(it.data)));                
            }
        };
        value_type operator*() const {
            return Tuple(*this);
        }
    };

    /**
    * @brief Creates a array with no elements.
    */
    Matrix() : main(std::make_shared<container_type>()) {}
    /**
    * @brief Access to array data.
    * @param pos The position for which data should be retrieved.
    * @return A reference to the proxy array for call next operator[].
    */
    Proxy<T, V, DEPTH, DEPTH-1> &operator[] (std::size_t pos) {
        main->deleteDefaultValue();
        return (*main)[pos];
    }
    /**
    * @brief How many cells are actually occupied.
    */
    std::size_t size() {
        return main->size();
    }
    /**
    * @brief There are actually occupied cells.
    */
    bool empty() {
        return main->size() == 0;
    }
    /**
    * @brief Returns a iterator that points to the first value in the array.
    */
    Iterator begin() {
        main->deleteDefaultValue();
        Iterator it(main);
        it.p_it = main->begin(it.indexes, &it.data);
        return it;
    }
    /**
    * @brief Returns a iterator that points one past the last value in the array.
    */
    Iterator end() {
        return Iterator();
    }
private:
    std::shared_ptr<container_type> main;
};

} //matrix