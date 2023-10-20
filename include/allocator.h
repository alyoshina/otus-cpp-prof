#pragma once

#include "pool.h"
#include "linear.h"

#include <memory>

namespace Mem {

template <typename T, std::size_t SIZE = 5, template <typename> class P = Pool>
class Allocator {
    
public:
    using value_type = T;

    Allocator() noexcept : pool(std::make_shared<P<T>>(SIZE)) {std::cout << "Allocator()" << std::endl; }

    template<typename U>
    Allocator(const Allocator<U, SIZE, P>& other) noexcept /*: pool(std::make_shared<P<U>>(SIZE)) */ {
        std::cout << "const Allocator<U, SIZE, P>& other" << std::endl;
        //pool = std::make_shared<P<U>>(SIZE);
        //pool = other.pool;
    }

    template <typename U>
    Allocator(Allocator<U, SIZE, P>&&other) noexcept : pool(std::make_shared<P<U>>(SIZE)) { 
        std::cout << "Allocator<U, SIZE, P>&& other" << std::endl;
        if (other.pool) {
            pool = std::move(other.pool);
            other.pool = nullptr;
        }
    }

    T* allocate(const std::size_t n) { 
        return  pool->get(n); 
    }
    void deallocate(T* p, std::size_t n) {
        pool->away(p, n);
    }

    template <typename U>
    bool operator!= (const Allocator<U, SIZE, P>& other) const noexcept { return pool != other.pool; }
    template <typename U>
    bool operator== (const Allocator<U, SIZE, P>& other) const noexcept { return pool == other.pool; }

    // template <typename U>
    // Allocator<U, SIZE, P>& operator= (const Allocator<U, SIZE, P>& other) { 
    //     std::cout << "Allocator operator=(&) other type" << std::endl;
    //     return *this;
    // }
 
    // template <typename U>
    // Allocator<U, SIZE, P>& operator= (Allocator<U, SIZE, P>&& other) { 
    //     std::cout << "Allocator operator=(&&) other type" << std::endl;
    //     pool = std::move(other.pool);
    //     return *this;
    // }

    Allocator& operator= (const Allocator& other) { 
        std::cout << "operator= (const Allocator& other)" << std::endl;
        pool = other.pool;
        return *this;
    }

    Allocator& operator= (Allocator&& other) { 
        std::cout << "operator= (Allocator&& other)" << std::endl;
        pool = std::move(other.pool);
        other.pool = nullptr;
        return *this;
    }

    template<typename U>
    struct rebind
    {
        using other = Allocator<U, SIZE, P>;
    };

    std::shared_ptr<P<T>> pool {nullptr};

    Allocator select_on_container_copy_construction() const {
        return Allocator();
    }

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
};
} //Mem

#include "allocator.ipp"
