#ifndef BOP_MEM_RECYCLER_HPP
#define BOP_MEM_RECYCLER_HPP

#include <map>
#include <type_traits>
#include <vector>
#include "PrimativeArrayContainer.hpp"
#include "../bop-topology/Stack.hpp"

namespace bop {
    namespace mem {
        template<typename T>
        class PrimativeArrayRecycler {
            private:
                std::map< uint_type,std::vector<T*> > arrays;
            public:
                #ifdef BOP_TYPE_TRAIT_CHECKS
                static_assert(std::is_trivially_destructable<T>::value, "The bop::mem::PrimativeArrayRecycler<T> class can only have T be trivially destructable.");
                #endif
                PrimativeArrayRecycler() : arrays() {

                }

                ~PrimativeArrayRecycler() {
                    this->dispose();
                }

                T* request(uint_type size) {
                    std::vector<T*>& selected_vector = this->arrays[size];
                    if (selected_vector.size() > 0) {
                        T* pointer = selected_vector[selected_vector.size() - 1];
                        selected_vector.pop_back();
                        return pointer;
                    }
                    else {
                        return new T[size];
                    }
                }

                void reserve(uint_type size = 1, uint_type number = 1) {
                    for (uint_type i = 0; i < number; i++) {
                        this->arrays[size].push_back(new T[size]);
                    }
                }

                uint_type size() {
                    return this->arrays.size();
                }

                void give(T* pointer, uint_type size) {
                    this->arrays[size].push_back(pointer);
                }

                void dispose() {
                    for (auto& kv_pair : this->arrays) {
                        for (uint_type iter = 0; iter < kv_pair.second.size(); iter++) {
                            delete[] kv_pair.second[iter];
                        }
                    }
                }

                bool hasReserves(uint_type size) {
                    return (this->arrays[size].size() != 0);
                }
        };
    }
}

#endif
