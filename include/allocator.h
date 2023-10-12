#pragma once

#include "pool.h"

namespace Mem {

template <typename T, std::size_t SIZE = 5, template <typename> class P = Pool>
class Allocator {
    
public:
    using value_type = T;

    Allocator() noexcept : pool(SIZE) { }
    template <typename U> Allocator(const Allocator<U, SIZE, P>&) noexcept : pool(SIZE) { }

    T* allocate(const std::size_t n) { 
        return  pool.get(n); 
        }
    void deallocate(T* p, std::size_t n) {
        pool.away(p, n);
    }

    template <typename U>
    bool operator!= (const Allocator<U, SIZE, P>& other) const noexcept {return false;}
    template <typename U>
    bool operator== (const Allocator<U, SIZE, P>& other) const noexcept {return true;}

    template<typename U>
    struct rebind
    {
        using other = Allocator<U, SIZE, P>;
    };

    P<T> pool;
};
} //Mem

#include "allocator.ipp"
