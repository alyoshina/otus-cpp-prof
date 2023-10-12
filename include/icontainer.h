#pragma once
#include <cstddef>

template <typename T>
class IContainer {

public:
    IContainer() {};
    virtual ~IContainer() {};

    virtual void push_front(const T& value) = 0;
    virtual void push_back(const T& value) = 0;
    virtual void insert(std::size_t pos, const T& value) = 0;
    virtual void erase(std::size_t pos) = 0;
    virtual void clear() = 0;
    virtual std::size_t size() const = 0;
    virtual T& operator [] (std::size_t pos) const = 0;
};