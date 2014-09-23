#ifndef BOP_LINKED_LIST_HPP
#define BOP_LINKED_LIST_HPP
#include <utility>
#include <initializer_list>
#include "LLNode.hpp"

namespace bop {
	namespace top {
		template<class T>
		class LinkedList {
			private:
				BiDirNode<T>* node;
				BiDirNode<T>* cursor_node;
				uint_type cursor_index;
				uint_type ll_size;
				
				void scanCursor(uint_type index) {
					if (this->cursor_node == nullptr){
						if (this->node == nullptr) return;
						else {
							this->cursor_node = this->node;
							this->cursor_index = 0;
						}
					}
					if (cursor_index == index) return;
					else {
						if (index > cursor_index) while (index > cursor_index && cursor_node->next != nullptr) {
							this->cursor_node = this->cursor_node->next;
							cursor_index++;
						}
						else while (index < cursor_index && cursor_node->last != nullptr) {
							this->cursor_node = this->cursor_node->last;
							cursor_index--;
						}
					}
				}
				
			public:
				
				LinkedList() : node(nullptr), cursor_node(nullptr), cursor_index(0), ll_size(0) {
				}
				
				LinkedList(const LinkedList<T>& copy) : LinkedList() {
					this->node = new BiDirNode(copy.node);
					this->ll_size = copy.ll_size;
					this->scanCursor(copy.cursor_index);
				}
				
				LinkedList(LinkedList<T>&& move) : LinkedList() {
					std::swap(this->node,move.node);
					std::swap(this->cursor_node,move.cursor_node);
					std::swap(this->cursor_index,move.cursor_index);
					std::swap(this->ll_size,move.ll_size);
				}

				LinkedList(const std::initializer_list<T>& init_list) : LinkedList() {
					if (init_list.size() > 0) {
						this->node = new BiDirNode<T>();
						for (auto& elem : init_list) {
							this->
						}
					}
				}
				
				void append(const T& data) {
					if (this->size() == 0) {
						this->node = new BiDirNode(data);
						this->scanCursor(0);
					}
					else {
						this->scanCursor(this->size() - 1);
						this->cursor_node->next = new BiDirNode(data);
						this->cursor_node->next->last = this->cursor_node;
					}
					this->ll_size++;
				}
				
				bool remove(uint_type index, uint_type length = 1) {
					if (length == 0 || index >= this->size() || index + length >= this->size()) return false;
					else {
						if (index == 0) {
							BiDirNode<T>* begin_ptr = this->node;
							BiDirNode<T>* end_ptr;
							this->scanCursor(length);
							end_ptr = this->cursor_node;
							this->scanCursor(length + 1);
							
							
						}
						else {
							this->scanCursor(index - 1);
						}
					}
				}
				
				uint_type size() {
					return this->ll_size;
				}
		};
	};
};

#endif
