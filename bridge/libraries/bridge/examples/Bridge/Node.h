#ifndef NODE_H
#define NODE_H

namespace bridge {
	template<typename T>
	class Node {
		public:
			Node() : prev(nullptr), next(nullptr) {}
			Node(T data) : data(data), prev(nullptr), next(nullptr) {}

			// The next of my prev is my next.
			// The prev of my next is my prev.
			~Node() {
				if (prev)
					prev->next = next;
				if (next)
					next->prev = prev;
			}

			// I continue to be the next of my prev.
			// The prev of my next is the new node.
			// My next is the new node.
			// I am the prev of the new node.
			void setNext(Node<T>* node) {
				if (next)
					next->prev = node;
				next = node;
				node->prev = this;
			}

			// I continue to be the prev of my next.
			// The next of my prev is the new node.
			// My prev is the new node.
			// I am the next of the new node.
			void setPrevious(Node<T>* node) {
				if (prev)
					prev->next = node;
				prev = node;
				node->next = this;
			}
			
			// If I have a next, assign it to node and return true.
			// Otherwise just return false.
			bool getNext(Node<T>* &node) {
				if (next) {
					node = next;
					return true;
				} else {
                    return false;
				}
			}
			
			
			// If I have a prev, assign it to node and return true.
			// Otherwise just return false.
			bool getPrevious(Node<T>* &node) {
				if (prev) {
					node = prev;
					return true;
				} else {
                    return false;
				}
			}
			
			// Keep a copy of data.
			void setData(T data) {
				this->data = data;
			}
			
			// Return saved data.
			T getData() {
				return data;
			}

		private:
			T data;
			Node<T>* prev;
			Node<T>* next;
	};
}

#endif
