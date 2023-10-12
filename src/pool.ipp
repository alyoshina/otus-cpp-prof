namespace Mem {

template <typename T>
Block<T>::Block(std::size_t s) {
    T* mem = static_cast<T*>(::operator new(s * sizeof(T)));
    if (!mem) {
        throw std::bad_alloc();
    }
    data = mem;
    bs = new std::vector<bool>(s, 0);
}

template <typename T>
Block<T>::~Block() {
    delete bs;
    ::operator delete(data);
}

template <typename T>
bool Block<T>::is_empty() {
    return size == 0; //std::find(bs->begin(), bs->end(), 1) == bs->end();
}

template <typename T>
T* Block<T>::get(std::size_t n) {
    std::vector<bool> v(n, 0);
    auto it = std::search(bs->begin(), bs->end(), v.begin(), v.end());
    if (it == bs->end()) {
        return nullptr;
    }
    std::fill(it, it + n, 1);
    size += n;
    return get_address(std::distance(bs->begin(), it));
}

template <typename T>
bool Block<T>::away(T* p, std::size_t n) {
    bool is_delete = false;
    try {
        auto it = bs->begin() + get_index(p);
        std::fill(it, it + n, 0);
        size -= n;
        is_delete = true;
    } catch (const std::exception &e) {
        ;
    }
    return is_delete;
}

template <typename T>
std::size_t Block<T>::get_index(T* p) {
    if (p < data || p >= data + bs->size()) {
        throw std::out_of_range("Block: address out of range");
    }
    if ((p - data) > bs->size()) {
        throw std::out_of_range("Block: address is invalid");
    }
    return p - data;
}

template <typename T>
Pool<T>::~Pool() {
    for (auto it = blocks.begin(); it != blocks.end();) {
        delete *it;
        it = blocks.erase(it);
    }
}

template <typename T>
T* Pool<T>::get(const std::size_t n) {
    T* p = get_in_blocks(n);
    if (!p) {
        std::size_t max_s = preferred_block_size;
        while (n > max_s) {
            max_s *= k;
        }
        Block<T>* b = new Block<T>(max_s);
        blocks.push_back(b);
        p = b->get(n);
    }
    return p;
}

template <typename T>
void Pool<T>::away(T* p, const std::size_t n) {
    for (auto it = blocks.begin(); it != blocks.end();) {
        if ((*it)->away(p, n)) {
            break;
        }
        if (blocks.size() > 1 && (*it)->is_empty()) {
            delete *it;
            it = blocks.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename T>
T* Pool<T>::get_in_blocks(const std::size_t n) {
    T* p = nullptr;
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        p = (*it)->get(n);
        if (p) {
            break;
        }
    }
    return p;
}   
} //Mem