#include <stdexcept>
#include <iostream>

template <typename T, typename Allocator>
List<T, Allocator>::List() : IList<T>() {
}

template <typename T, typename Allocator>
List<T, Allocator>::List(std::initializer_list <T> l) : IList<T>() {
    for (T value : l) {
        push_back(value);
    }
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const List& other) : IList<T>()
    , node_alloc(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())) {
    *this = other;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List&& other) : IList<T>() {
    *this = std::move(other);
}

template <typename T, typename Allocator>
List<T, Allocator>::~List() {
    clear();
}

template <typename T, typename Allocator>
void List<T, Allocator>::clear() {
    typename IList<T>::INode* node = this->m_first;
    while (node) {
        typename IList<T>::INode* next = node->next();
        //delete node
        NodeAllocTraits::destroy(node_alloc, static_cast<Node*>(node));
        NodeAllocTraits::deallocate(node_alloc, static_cast<Node*>(node), 1);
        node = next;
    }
    this->m_first = nullptr;
    this->m_last = nullptr;
    this->m_size = 0;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List<T, Allocator>& rhs) {
    if (&rhs != this) {
        clear();
        if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value) {
            this->node_alloc = rhs.node_alloc;
        }
        typename IList<T>::INode* rhs_node = rhs.m_first;
        for (std::size_t i = 0; i < rhs.m_size; ++i, rhs_node = rhs_node->next()) {
            this->push_back(rhs_node->data());
        }
    }
    return *this;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List<T, Allocator>&& rhs) {
    if (&rhs != this) {
        clear();
        this->move(&rhs);
        if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
            if (this->get_allocator() != rhs.get_allocator()) {
                this->node_alloc = std::move(rhs.node_alloc);
            }
        }          
    }
    return *this;
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_front(const T& value) {
    //new Node
    Node* new_node = NodeAllocTraits::allocate(node_alloc, 1);
    NodeAllocTraits::construct(node_alloc, new_node);
    new_node->setPrev(nullptr);
    new_node->setNext(this->m_first);
    new_node->setData(value);
    if (!this->m_last) {
        this->m_last = new_node;
    }
    if (this->m_first) {
        this->m_first->setPrev(new_node);
    }
    this->m_first = new_node;
    this->m_size++;
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_back(const T& value) {
    //new Node
    Node* new_node = NodeAllocTraits::allocate(node_alloc, 1);
    NodeAllocTraits::construct(node_alloc, new_node);
    new_node->setPrev(this->m_last);
    new_node->setNext(nullptr);
    new_node->setData(value);
    this->push_back_node(new_node);
}

template <typename T, typename Allocator>
void List<T, Allocator>::insert(const std::size_t pos, const T& value) {
    if (pos >= this->m_size) {
        throw std::out_of_range("List: out of range");
    }
    typename IList<T>::INode* node = this->m_first;
    for (std::size_t i = 0; i < this->m_size; ++i, node = node->next()) {
        if (pos == i) {
            //new Node
            Node* new_node = NodeAllocTraits::allocate(node_alloc, 1);
            NodeAllocTraits::construct(node_alloc, new_node);
            new_node->setPrev(node->prev());
            new_node->setNext(node);
            new_node->setData(value);
            if (!node->prev()) {
                this->m_first = new_node;
            } else {
                node->prev()->setNext(new_node);
            }
            node->setPrev(new_node);
            this->m_size++;
            break;
        }   
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::erase(const std::size_t pos) {
    if (pos >= this->m_size) {
        throw std::out_of_range("List: out of range");
    }
    typename IList<T>::INode* node = this->m_first;
    for (std::size_t i = 0; i < this->m_size; ++i, node = node->next()) {
        if (pos == i) {
            if (!node->prev()) {
                this->m_first = node->next();
            } else {
                node->prev()->setNext(node->next());
            }       
            if (!node->next()) {
                this->m_last = node->prev();
            } else {
                node->next()->setPrev(node->prev());
            }
            this->m_size--;
            //delete node
            NodeAllocTraits::destroy(node_alloc, static_cast<Node*>(node));
            NodeAllocTraits::deallocate(node_alloc, static_cast<Node*>(node), 1);
            break;
        }
    }
}