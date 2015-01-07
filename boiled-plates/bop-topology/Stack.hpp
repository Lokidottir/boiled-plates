#ifndef BOP_STACK_HPP
#define BOP_STACK_HPP

#include <vector>
#include <utility>
#include "../bop-defaults/types.hpp"

namespace bop {
    namespace top {

        template<class T>
        class Stack {
        private:
            struct Node {

                Node* node;
                T data;

                Node() : node(nullptr), data(T()) {
                }

                Node(const Node& copy) : Node(){
                    this->data = copy.data;
                    if (copy.node != nullptr) this->node = new Node(*copy.node);
                }

                Node(Node&& move) : Node() {
                    std::swap(this->data,move.data);
                    std::swap(this->node,move.node);
                }

                Node(const T& data) : Node() {
                    this->data = data;
                }

                ~Node() {
                    this->clean();
                }

                void clean() {
                    if (this->node != nullptr) this->node->clean();
                    delete this->node;
                    this->node = nullptr;
                }

            };

            Node* stk;
            uint_type logical_size;

            inline Node* topNode() {
                return this->stk;
            }

        public:

            Stack() {
                this->stk = nullptr;
                this->logical_size = 0;
            }

            Stack(const Stack<T>& copy) : Stack() {
                this->stk = new Node(*copy.stk);
                this->logical_size = copy.logical_size;
            }

            Stack(Stack<T>&& move) : Stack() {
                this->stk = move.stk;
                move.stk = nullptr;
                this->logical_size = move.logical_size;
            }

            Stack(const std::vector<T>& copyvec) : Stack() {
                for (uint_type i = 0; i < copyvec.size(); i++) this->push(copyvec[i]);
            }

            ~Stack() {
                delete this->stk;
            }

            Stack<T>& operator=(const Stack<T>& copy) {
                if (copy->stk != nullptr)this->stk = new Node(*copy.stk);
                this->logical_size = copy.logical_size;
                return *this;
            }


            T& operator[] (uint_type index) const {
                /*
                    std::vector<T> behaviour, return the nth value that was added,
                    costly for items far from the top of the stack.
                */
                uint_type wrk_index = this->size() - 1;
                Node* tmp_ptr = this->stk;
                while (tmp_ptr != nullptr && wrk_index > index) {
                    tmp_ptr = tmp_ptr->node;
                    wrk_index--;
                }
                return tmp_ptr->data;

            }

            T pop() {
                /*
                    Pop the value from the top of the stack, returning it. If no
                    item is present, return T().
                */
                T val;
                Node* top_ptr = this->topNode();
                if (top_ptr == nullptr) {
                    return T();
                }
                else {
                    val = std::move(top_ptr->data);
                    this->stk = top_ptr->node;
                    top_ptr->node = nullptr;
                    this->logical_size--;
                    return val;
                }

            }

            void push(const T& data) {
                /*
                    Add an element to the stack so that the node at this->stk is
                    now the top of the stack;
                */
                if (this->topNode() == nullptr) {
                    this->stk = new Node(data);
                }
                else {
                    Node* tmp_ptr = this->stk;
                    this->stk = new Node(data);
                    this->stk->node = tmp_ptr;
                }
                this->logical_size++;
            }

            bool contains(const T& val) const {
                Node* ptr = this->stk;
                while (ptr != nullptr) {
                    if (ptr->data == val) return true;
                    ptr = ptr->node;
                }
                return false;
            }

            operator std::vector<T> () const {
                /*
                    Cast to std::vector object
                */
                std::vector<T> vec(this->size());
                for (uint_type i = 0; i < this->size(); i++) {
                    vec[i] = (*this)[i];
                }
                return vec;
            }

            uint_type size(bool compute_size = false) const {
                if (!compute_size) return this->logical_size;
                else {
                    uint_type computed_size = 0;
                    Node* node_ptr = this->stk;
                    if (node_ptr != nullptr) while (node_ptr != nullptr) {
                        computed_size++;
                        node_ptr = node_ptr->node;
                    }
                    return computed_size;
                }
            }

            T top() const {
                Node* ptr = this->topNode();
                if (ptr != nullptr) return ptr->data;
                else return T();
            }
        };

    };
};

#endif
