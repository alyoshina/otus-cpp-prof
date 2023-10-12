#include <stdexcept>
#include <iostream>

template <typename T>
IList<T>::IList() 
    : m_size{0}
    , m_first{nullptr}
    , m_last{nullptr} {

}

template <typename T>
std::size_t IList<T>::size() const {
    return m_size;
}

template <typename T>
void IList<T>::move(IList<T>* rhs) {
    m_first = rhs->m_first;
    rhs->m_first = nullptr;
    m_last = rhs->m_last;
    rhs->m_last = nullptr;
    m_size = rhs->m_size;
    rhs->m_size = 0;
}

template <typename T>
void IList<T>::push_back_node(INode* node) {
    if (!m_first) {
        m_first = node;
    }
    if (m_last) {
        m_last->setNext(node);
    }
    m_last = node;
    m_size++;
}

template <typename T>
T& IList<T>::operator[] (const std::size_t pos) const {
    if (pos >= m_size) {
        throw std::out_of_range("IList: out of range"); 
    }
    INode* node = m_first;
    for (std::size_t i = 0; i < m_size; ++i, node = node->next()) {
        if (pos == i) {
            break;
        }
    }
    return node->data();
}