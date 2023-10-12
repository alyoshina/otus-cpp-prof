namespace Mem {

template <typename T>
T* Linear<T>::get(const std::size_t n) {
    alloc();
    if (size + n > max_size) {
        throw std::bad_alloc();
    }
    size += n;
    return mem + size - n;            
}

template <typename T>
void Linear<T>::alloc() {
    if (!mem) {
        mem = static_cast<T*>(::operator new(max_size * sizeof(T)));
    }
}

template <typename T>
void Linear<T>::dealloc() {
    ::operator delete(mem);
    mem = nullptr;
    size = 0;
}

template <typename T>
T* ExtensionLinear<T>::get(const std::size_t n) {
    if (size + n > block_size) {
        alloc(size + n);
    }
    size += n;
    return blocks.back() + size - n;            
}

template <typename T>
void ExtensionLinear<T>::alloc(std::size_t s) {
    std::size_t max_s = preferred_block_size;
    while (s > max_s) {
        max_s *= k;
    }
    T* mem = static_cast<T*>(::operator new(max_s * sizeof(T)));
    if (!mem) {
        throw std::bad_alloc();
    }
    block_size = max_s;
    blocks.push_back(mem);
    size = 0;
}

template <typename T>
void ExtensionLinear<T>::dealloc() {
    while(blocks.size()) {
        T* mem = blocks.back();
        blocks.pop_back();
        ::operator delete(mem);
    }
    size = 0;
    block_size = 0;
}
} //Mem