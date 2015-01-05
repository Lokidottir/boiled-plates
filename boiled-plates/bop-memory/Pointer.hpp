#ifndef BOP_MEM_POINTER
#define BOP_MEM_POINTER
#include <utility>
#include "../bop-defaults/types.hpp"

namespace bop {
    namespace mem {
        template<class T>
        class Pointer{
                T* ptr;
                uint_type arr_size;
                bool delete_ptr;
                bool is_array;
            public:
                Pointer() : ptr(nullptr), arr_size(0), delete_ptr(true), is_array(false) {
                    /*
                        Empty Constructor
                    */
                }

                /*  Copy constructor forever not allowed.
                Pointer(const Pointer& copy) : Pointer() {
                    this->ptr = copy->ptr;
                    this->arr_size = copy->arr_size;
                }
                */

                Pointer(Pointer&& move) : Pointer() {
                    /*
                        Move constructor
                    */
                    std::swap(this->ptr,move.ptr);
                    std::swap(this->arr_size,move.arr_size);
                    std::swap(this->is_array,move.is_array);
                    std::swap(this->delete_ptr,move.delete_ptr);
                }

                Pointer(T* ptr, uint_type arr_size = 1, bool is_array = true, bool delete_ptr = true) : Pointer() {
                    /*
                        Primary initializer constructor.
                        Given a pointer, it's array size and if the pointer should be
                        deleted on destruction of the object.
                    */
                    this->ptr = ptr;
                    this->arr_size = arr_size;
                    this->is_array = arr_size > 1 || is_array;
                    this->delete_ptr = delete_ptr;
                }

                ~Pointer() {
                    /*
                        Destructor
                    */
                    if (this->delete_ptr) this->clean();
                }

                T* operator->() const {
                    /*
                        Member access operator, returns the dereferenced
                        pointer.
                    */
                    return (*this->ptr);
                }

                T& operator[](uint_type index) const {
                    /*
                        Subscript operator, only works if the pointer is a
                        plain C array.
                    */
                    return this->ptr[index];
                }

                T& operator= (Pointer<T> pointer) {
                    /*
                        
                    */

                    this->ptr =;
                }

                #ifdef BOP_MEM_POINTER_SIZECOMPARE
                /*
                    Comparison operators for size sorting.
                */

                bool operator>(const Pointer<T>& compare) const {
                    return (this->arr_size > compare.arr_size);
                }

                bool operator<(const Pointer<T>& compare) const {
                    return (this->arr_size < compare.arr_size);
                }

                bool operator<=(const Pointer<T>& compare) const {
                    return !((*this) > compare);
                }

                bool operator>=(const Pointer<T>& compare) const {
                    return !((*this) < compare);
                }

                bool operator==(const Pointer<T>& compare) const {
                    return (this->arr_size == compare.arr_size);
                }

                bool operator!=(const Pointer<T>& compare) const {
                    return !((*this) == compare);
                }

                #endif

                uint_type size() const {
                    return this->arr_size;
                }

                void clean() {
                    /*
                        Manual destructor.
                    */
                    if (this->is_array) delete[] this->ptr;
                    else delete this->ptr;
                    this->ptr = nullptr;
                    this->arr_size = 0;
                }

        };
    }
}
#endif
