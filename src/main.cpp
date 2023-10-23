#include "allocator.h"
#include "sfinae.h"
#include "pool.h"
#include "list.h"
#include "container_utils.h"

#include <iostream>
#include <map>

const int BLOK_SIZE = 10;
using MapAlloc = Mem::Allocator<std::pair<const int, int>, BLOK_SIZE>;
using ListAlloc = Mem::Allocator<int, BLOK_SIZE>;

#include <list>
int main(int argc, char const *argv[])
{
    try {
#if 1
        // using MAP = std::map<int, int, std::less<int>, MapAlloc>;
        // MAP a;
        // MAP b;
        // a[1] = 1;
        // b = std::move(a);
        // b[2] = 2;
        // std::cout << "b: "<< b << std::endl;
        using List_ = List<int, ListAlloc>;
        List_ a;
        a.push_back(1);
        List_ b;
        b = std::move(a);
        b.push_back(2);
        std::cout << "b: "<< b << std::endl;

        // std::list<int, ListAlloc> l1;
        // l1.push_back(1);
        // std::list<int, ListAlloc> l2 = l1;
        // l2.push_back(2);
        // std::list<int, ListAlloc> l3;
        // l3 = l2;
        // l3.push_back(4);
        // std::cout << "l1: "<< l1 << std::endl;
        // std::cout << "l2: "<< l2 << std::endl;
        // std::cout << "l3: "<< l3 << std::endl;

        // PrintContainer <std::map<int, int>> m_pc;
        // PrintContainer <std::map<int, int, std::less<int>, MapAlloc>> m_alloc_pc;

        // PrintContainer <List<int>> l_pc;
        // PrintContainer <List<int, ListAlloc>> l_alloc_pc;

        // m_pc.print();
        // m_alloc_pc.print();

        // l_pc.print();
        // l_alloc_pc.print();
#else
        {
            std::map<int, int> m;
            std::map<int, int, std::less<int>, MapAlloc> alloc_m;
            factorial_fill(m);
            factorial_fill(alloc_m);
            std::cout << m << std::endl;
            std::cout << alloc_m << std::endl;
        }
        
        {
            List<int> l;
            List<int, ListAlloc> alloc_l;
            factorial_fill(l);
            factorial_fill(alloc_l);
            std::cout << l << std::endl;
            std::cout << alloc_l << std::endl;
        }
#endif
        
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}