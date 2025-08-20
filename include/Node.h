#pragma once

/**
 * @brief Node for constructing Iterable containers.
 */
template <class T>
class Node{
private:
    T data;
    Node *next;
public:
    Node() : data(), next(nullptr) {}
    Node(const T& _data) : data(_data), next(nullptr) {}
    Node(const T& _data, const Node* _next) : data(_data), next(_next) {}
public:
    void set_next_(Node<T>* next){
        this->next = next;
    }
    Node<T>* get_next() {
        return next;
    }
    void set_data_(const T &_data){
        this->data = _data;
    }
    const T& get_data() const {
        return data;
    }
    T& get_data() {
        return data;
    }
};
