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

                Node(T* move) : Node() {
                    (*move);
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

            Node* topNode() {
                Node* ptr = this->stk;
                if (ptr != nullptr) while(ptr->node != nullptr) {
                    ptr = ptr->node;
                }
                return ptr;
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
                this->stk = new Node(*copy.stk);
                this->logical_size = copy.logical_size;
                return *this;
            }

            T& operator[] (uint_type index) const {
                uint_type wrk_index = 0;
                Node* ptr = this->stk;
                if (ptr != nullptr) while (wrk_index < index && ptr->node != nullptr) {
                    ptr = ptr->node;
                }
                return ptr->data;
            }

            T pop() {
                T val;
                Node* ptr = this->stk;
                if (ptr == nullptr) {
                    return T();
                }
                else if (ptr->node == nullptr) {
                    std::swap(val,ptr->data);
                }
                else {
                    while (ptr->node->node != nullptr) {
                        ptr = ptr->node;
                    }
                    std::swap(val,ptr->node->data);
                    delete ptr->node;
                    ptr->node = nullptr;
                }
                if (this->logical_size > 0) this->logical_size--;
                return val;
            }

            void push(const T& data) {
                if (this->stk != nullptr) this->topNode()->node = new Node(data);
                else this->stk = new Node(data);
                this->logical_size++;
            }

            void givePush(T* move) {
                /*
                    Moves and pushes an element.
                */
                if (this->stk != nullptr) this->topNode()->node = new Node(move);
                else this->stk = new Node(move);
                this->logical_size++;
            }

            bool contains(const T& val) const {
                Node* ptr = this->stk;
                if (ptr != nullptr) while (ptr->node != nullptr) {
                    if (ptr->data == val) return true;
                    ptr = ptr->node;
                }
                return false;
            }

            operator std::vector<T> () const {
                std::vector<T> vec(this->size());
                for (uint_type i = 0; i < this->size(); i++) {
                    vec[i] = (*this)[i];
                }
                return vec;
            }

            uint_type size() const {
                return this->logical_size;
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
