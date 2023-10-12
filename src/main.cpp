#include "allocator.h"
#include "sfinae.h"
#include "pool.h"
#include "list.h"

//#include "out_operator_for_container.h"

#include <iostream>
#include <map>


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

        // std::cout << details::is_container_map<std::map<int, int>>::value << std::endl; //out 1
        // std::cout << details::is_container<std::map<int, int>>::value << std::endl; //out 0
        // std::cout << details::is_container_map<List<int>>::value << std::endl; //out 0
        // std::cout << details::is_container<List<int>>::value << std::endl; //out 1
        /*
        {
            std::map<int, int> m;
            for (int i = 0; i < 10; ++i) {
                m[i] = fact(i);
            }
            std::cout << m << std::endl;
        }
        */
        /*
        {
            List<int, ListAlloc> l;
            for (int i = 0; i < 10; ++i) {
                l.push_back(fact(i));
            }
            std::cout << l << std::endl;
        }
        */
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}