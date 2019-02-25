#ifndef LINKEDINDEX_H
#define LINKEDINDEX_H

#include "Node.h"
using namespace std;

namespace bridge {
	template<typename T>
	class LinkedIndex {
		public:
			LinkedIndex();
			
			LinkedIndex(int length) : length(length), current(nullptr), last(nullptr) {
				map = new Node<T>*[length];
				mapped = new bool[length]{0};
			}

			~LinkedIndex() {
			    if (last) {
                    // Start pointer.
                    Node<T>* node;
                    // Navigate to the end.
                    while (last->getPrevious(node)) {
						// delete(last->getData());
                        delete(last);
                        last = node;
                    }
                    if (last) {
						// delete(last->getData());
                        delete(last);
					}
			    }
				delete(map);
				delete(mapped);
			}

			bool unset(int id) {
				if (id < length) {
					if (mapped[id]) {
						mapped[id] = false;
						if (map[id] == last)
							if (!last->getPrevious(last))
								last = nullptr;
						// // Delete container.
						// delete(map[id]->getData());
						// Delete node.
						delete(map[id]);
					}
					return true;
				} else {
					return false;
				}
			}

			bool set(int id, const T &data) {
				if (id < length) {
					if (!mapped[id]) {
						mapped[id] = true;
						map[id] = new Node<T>;
						if (last)
							last->setNext(map[id]);
						last = map[id];
					}
					// map[id]->setData(new Container<T>(id, data));
					map[id]->setData(data);
					return true;
				} else {
					return false;
				}
			}

			bool get(int id, T &data) {
				if (id < length && mapped[id]) {
					// data = map[id]->getData()->data;
					data = map[id]->getData();
					return true;
				} else {
					return false;
				}
			}
			
			bool next(T &data) {
				if (current) {
					Node<T>* c = current;
					if (!current->getPrevious(current))
						return false;
				} else {
					current = last;
				}
				if (current) {
					// data = current->getData()->data;
					data = current->getData();
					return true;
				} else {
					return false;
				}
			}
			
			// int id() {
				// if (current) {
					// return current->getData()->id;
				// } else {
					// return -1;
				// }
			// }
			
			bool begin() {
				current = nullptr;
				return last;
			}

		private:
			int length;
			bool* mapped;
			Node<T>* current;
			Node<T>* last;
			Node<T>** map;
	};
}


#endif
