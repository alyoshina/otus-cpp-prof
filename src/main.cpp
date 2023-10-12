#include "allocator.h"
#include "sfinae.h"
#include "pool.h"
#include "list.h"

#include <iostream>
#include <map>

/*
#include <list>
#include <algorithm>
namespace details {
    template <typename T>
    struct is_container: std::false_type {};

    // partial specializations for vector
    template <typename T, typename Alloc>
    struct is_container<std::vector<T, Alloc>>: std::true_type {};

} // details

template <class Container, class = std::enable_if_t<details::is_container<Container>::value>>
std::ostream& operator<<(std::ostream& os, const Container& container) {

    if (!container.empty()) {
        std::cout << *container.begin();
        std::for_each(std::next(container.begin()), container.end(), [] (auto& value) {
            std::cout << ", " << value;
        });
    }
    return os;
}
*/

const int BLOK_SIZE = 10;
using MapAlloc = Mem::Allocator<std::pair<const int, int>, BLOK_SIZE>;
using ListAlloc = Mem::Allocator<int, BLOK_SIZE>;

int main(int argc, char const *argv[])
{
    try {
        PrintContainer <std::map<int, int>> m_pc;
        PrintContainer <std::map<int, int, std::less<int>, MapAlloc>> m_alloc_pc;

        PrintContainer <List<int>> l_pc;
        PrintContainer <List<int, ListAlloc>> l_alloc_pc;

        m_pc.print();
        m_alloc_pc.print();

        l_pc.print();
        l_alloc_pc.print();
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}