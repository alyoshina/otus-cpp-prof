#pragma once

#include <typeinfo>
#include <vector>
#include <iostream>

namespace Mem {

template <typename T>
class Linear {
public:
    Linear(std::size_t s) : max_size(s) { std::cout << "Linear()" << std::endl; }
    ~Linear() { dealloc(); }
    T* get(const std::size_t n);
    void away(T* p, std::size_t n) { }
private:
    T* mem{nullptr};
    const std::size_t max_size{};
    std::size_t size{};
    
    void alloc();
    void dealloc();
};

template <typename T>
class ExtensionLinear {
public:
    ExtensionLinear(std::size_t s) : preferred_block_size(s)  { }
    ~ExtensionLinear() { dealloc(); }
    T* get(const std::size_t n);
    void away(T* p, std::size_t n) { }
private:
    const std::size_t preferred_block_size{};
    std::vector<T*> blocks;
    std::size_t block_size{};
    std::size_t size{};
    const std::size_t k = 2;
    
    void alloc(std::size_t s);
    void dealloc();
};
} //Mem

#include "linear.ipp"