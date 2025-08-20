#pragma once

#include "Node.h"

template <class T>
class Queue {
    private:
        int length = 0;
        Node<T>* front_node;
        Node<T>* back_node;
    public:
        Queue(){
            back_node = new Node<T>();
            this->front_node = back_node;
            length = 0;
        }
        Queue(const Queue& q) {
            back_node = new Node<T>();
            front_node = back_node;
            length = 0;
            for (Node<T>* node = q.front_node->get_next(); node != nullptr; node = node->get_next()) {
                push_(node->get_data());
            }
        }
        Queue& operator=(const Queue& q) {
            if (this != &q) {
                while(front_node != nullptr){
                    Node<T>* temp_ptr = front_node->get_next();
                    delete front_node;
                    front_node = temp_ptr;
                }
                back_node = new Node<T>();
                front_node = back_node;
                length = 0;
                for (Node<T>* node = q.front_node->get_next(); node != nullptr; node = node->get_next()) {
                    push_(node->get_data());
                }
            }
            return *this;
        }
        ~Queue(){
            while(front_node != nullptr){
                Node<T>* temp_ptr = front_node->get_next();
                delete front_node;
                front_node = temp_ptr;
            }
        }
    public:
        void push_(const T& _data){
            Node<T>* temp = new Node(_data);
            back_node->set_next_(temp);
            back_node = temp;
            length++;
        }
        void push_(T&& _data){
            Node<T>* temp = new Node(std::move(_data));
            back_node->set_next_(temp);
            back_node = temp;
            length++;
        }
        T pop_() {
            if (front_node->get_next() == nullptr) throw std::runtime_error("Queue is empty");
            Node<T>* temp = front_node->get_next();
            T result = temp->get_data();
            front_node->set_next_(temp->get_next());
            if (temp == back_node) back_node = front_node;
            delete temp;
            length--;
            return result;
        }
    public:
        class iterator {
            Node<T>* node;
        public:
            iterator(Node<T>* n) : node(n) {}
            T& operator*() { return node->get_data(); }
            const T& operator*() const { return node->get_data(); }
            iterator& operator++() { node = node->get_next(); return *this; }
            bool operator!=(const iterator& other) const { return node != other.node; }
        };
        iterator begin() { return iterator(front_node->get_next()); }
        iterator end() { return iterator(nullptr); }

        iterator begin() const { return iterator(front_node->get_next()); }
        iterator end() const { return iterator(nullptr); }
};