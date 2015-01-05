#ifndef BOP_MEM_RECYCLER_HPP
#define BOP_MEM_RECYCLER_HPP

#include <map>
#include <type_traits>
#include "PrimativeArrayContainer.hpp"
#include "../bop-topology/Stack.hpp"

namespace bop {
    namespace mem {
        template<typename T>
        class PrimativeArrayRecycler {
            private:
                std::map< uint_type,top::Stack<T*> > arrays;
            public:
                #ifdef BOP_TYPE_TRAIT_CHECKS
                static_assert(std::is_fundamental<T>::value, "The bop::mem::PrimativeArrayRecycler<T> class can only have T as a primitive type.");
                #endif
                PrimativeArrayRecycler() : arrays() {

                }

                T* request(uint_type size) {
                    if (this->arrays[size].size() > 0) {
                        return this->arrays[size].pop();
                    }
                    else {
                        return new T[size];
                    }
                }

                void reserve(uint_type size = 1, uint_type number = 1) {
                    for (uint_type i = 0; i < number; i++) {
                        this->arrays[size].push(new T[size]);
                    }
                }

                uint_type size() {
                    return this->arrays->size();
                }

                void give(T* pointer, uint_type size) {
                    this->arrays[size].push(pointer);
                }

                bool hasReserves(uint_type size) {
                    return (this->arrays[size].size() != 0);
                }
        };
    }
}

#endif
