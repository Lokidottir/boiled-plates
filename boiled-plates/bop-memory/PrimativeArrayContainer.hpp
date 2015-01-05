#ifndef BOP_PRIMATIVE_ARRAY_CONTAINER
#define BOP_PRIMATIVE_ARRAY_CONTAINER

#include <type_traits>
#include "../bop-defaults/types.hpp"

namespace bop {
    namespace mem {

        template<class T>
        class PrimativeArrayRecycler;

        template<class T>
        class PrimativeArrayContainer {
                friend class PrimativeArrayRecycler<T>;
            private:
                uint_type array_size;
                T* data;

                inline void setData(uint_type size) {
                    if (this->data == nullptr || size > this->size()) {
                        this->data = static_cast<T*>(realloc(this->data, size * sizeof(T)));
                    }
                    this->array_size = size;
                }
            public:
                static PrimativeArrayRecycler<T> recycler;
                #ifdef BOP_TYPE_TRAIT_CHECKS
                static_assert(std::is_trivially_copyable<T>::value, "The bop::mem::PrimativeArrayContainer<T> class can only have T as a primitive type.");
                #endif

                PrimativeArrayContainer() {
                    this->array_size = 0;
                    this->data = nullptr;
                }

                PrimativeArrayContainer(uint_type size) {
                    this->setData(size);
                }

                PrimativeArrayContainer(const PrimativeArrayContainer<T>& copy) {
                    this->setData(copy.size());
                    this->data = memcpy(this->data,copy.data, copy.size() * sizeof(T));
                }

                ~PrimativeArrayContainer() {
                    if (this->data != nullptr) {
                        PrimativeArrayContainer::recycler.give(this->data,this->array_size);
                        this->data = nullptr;
                    }
                    else {
                        delete[] this->data;
                        this->data = nullptr;
                    }
                }

                uint_type size() const {
                    return this->array_size;
                }

                T& operator[] (uint_type index) const {
                    return this->data[index];
                }

        };

        template<typename T>
        PrimativeArrayRecycler<T> PrimativeArrayContainer<T>::recycler = PrimativeArrayRecycler<T>();

    }
}

#endif
