#ifndef BOP_MEM_RECYCLER_HPP
#define BOP_MEM_RECYCLER_HPP

#include <map>
#define BOP_MEM_POINTER_SIZECOMPARE
#include "Pointer.hpp"
#include "../bop-topology/Stack.hpp"

namespace bop {
    namespace mem {
        template<typename T>
        class Recycler {
            private:
                std::map< uint_type,top::Stack<Pointer<T> > > pointers;
            public:
                Recycler() : pointers() {

                }

                Pointer<T> request(uint_type req_size = 1, bool is_array = false) {
                    if (req_size > 1) is_array = true;

                }

                void givePointer(Pointer<T>& pointer) {
                    this->pointers[pointer.size()].push(Pointer<T>(std::move(pointer)));
                }

                void reserve(uint_type size = 1, uint_type number = 1) {
                    for (uint_type i = 0; i < number; i++) {
                        Pointer<T>* temp_ptr(new T[size],size,true,true);
                        this->pointers[size].givePush(std::move(temp_ptr));
                    }
                }

                uint_type size() {
                    return this->pointers->size();
                }
        };
    }
}

#endif
