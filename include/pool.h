#pragma once

#include <stdexcept>
#include <algorithm>
#include <list>

namespace Mem {

template <typename T>
struct Block {
    Block(std::size_t s);
    ~Block();
    T* get(std::size_t n);
    bool away(T* p, std::size_t n);
    bool is_empty();
private:
    T* get_address(std::size_t pos) { return data + pos; }
    std::size_t get_index(T* p);
    T* data{nullptr};
    std::size_t size{};
    std::vector<bool>* bs;
};

template <typename T>
class Pool {
public:
    Pool(std::size_t s) : preferred_block_size(s)  { }
    ~Pool();
    T* get(const std::size_t n);
    void away(T* p, std::size_t n);
private:
    const std::size_t preferred_block_size{};
    const std::size_t k = 2;
    std::list<Block<T>*> blocks;
    T* get_in_blocks(const std::size_t n);
};
} //Mem

#include "pool.ipp"