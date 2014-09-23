#ifndef BOP_LLNODE_HPP
#define BOP_LLNODE_HPP

/*
	Template Node classes, for use in Linked Lists, Stacks, other defined
	types that might make use of liked-list like structres.
*/

namespace bop {
	namespace top {
		template<class T>
		struct Node {
			public:
				Node<T>* node_ptr;
				T data;
				Node() : node_ptr(nullptr), data(T()) {
				}
				
				Node(const Node<T>& copy) : Node() {
					this->copy(copy);
				}
				
				Node(Node<T>&& move) : Node() {
					std::swap(this->data,move.data);
					std::swap(this->node_ptr,move.node_ptr);
				}
				
				Node(const T& data) : Node() {
					this->data = data;
				}
				
				~Node() {
					this->clear();
				}
				
				void clear() {
					if (this->node_ptr != nullptr) {
						this->node_ptr->clear();
						delete this->node_ptr;
						this->node_ptr = nullptr;
					}
				}
				
				void copy(const Node<T>& copy) {
					this->data = data.copy;
					if (copy.node_ptr != nullptr) this->node_ptr = new Node<t>(*copy.node_ptr);
				}
		
		};
		
		/*
			Bi-directional node class.
		*/
		
		template<class T>
		struct BiDirNode {
				BinaryNode<T>* next;
				BinaryNode<T>* last;
				T data;
			private:
				
				void copyRecurseLast(const BiDirNode<T>* copy) {
					if (copy->last != nullptr) { 
						this->last = new BiDirNode();
						this->last->copyRecurseLast(copy->last);
						this->last->next = this;
						this->last->data = T(copy->last->data)
					}
				}
				
				void copyRecurseNext(const BiDirNode<T>* copy) {
					if (copy->next != nullptr) {
						this->next = new BiDirNode();
						this->next->copyRecurseNext(copy->next);
						this->next->last = this;
						this->next->data = T(copy->next->data);
					}
				}
				
				void clearRecurseLast() {
					if (this->last != nullptr) {
						this->last->clearRecurseLast();
						delete this->last;
						this->last = nullptr;
					}
					this->next = nullptr;
				}
				
				void clearRecurseNext() {
					if (this->next != nullptr) {
						this->next->clearRecurseNext();
						delete this->next;
						this->next = nullptr;
					}
					this->last = nullptr;
				}
				
			public:
				
				BiDirNode() : next(nullptr), last(nullptr), data(T()) {
				}
				
				BiDirNode(const BiDirNode<T>& copy) : BiDirNode() {
					this->copy(&copy);
				}
				
				BiDirNode(BiDirNode<T>&& move) : BiDirNode() {
					std::swap(this->last,move.last);
					std::swap(this->next,move.next);
					std::swap(this->data,move.data);
				}
				
				~BiDirNode() {
					this->clear();
				}
				
				BiDirNode<T>& operator= (const BiDirNode<T>& copy) {
					this->copy(&copy);
				}
				
				void clear() {
					if (this->last != nullptr) {
						this->last->clearRecurseLast();
						delete this->last;
						this->last = nullptr;
					}
					if (this->next != nullptr) {
						this->next->clearRecurseNext();
						delete this->next;
						this->next = nullptr;
					}
				}
				
				void copy(const BiDirNode<T>* copy) {
					this->copyRecurseLast(copy);
					this->copyRecurseNext(copy);
					this->data = T(copy->data);
				}
			
		};
	}
};

#endif
