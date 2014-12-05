#ifndef BOP_VECTOR_HPP
#define BOP_VECTOR_HPP

#include <cstring>
#include <initializer_list>
#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include "Matrix.hpp"
/*
    bop::maths::Vector Class file
*/

namespace bop {
    #ifdef BOP_USE_INFERIOR_NAMESPACE
    namespace math {
    #else
    namespace maths {
    #endif
        #ifndef BOP_MATHS_DEFAULT_TYPES
        #define BOP_MATHS_DEFAULT_TYPES
        typedef double prec_type;
        typedef uint64_t uint_type;
        #endif
        template<class T>
        class Vector : public Matrix<T> {
                /*
                    Version of Vector that inherits from Matrix.
                */
            public:

                Vector() : Matrix<T>() {

                }

                Vector(const uint_type size, bool is_rowvector) : Vector() {
                    if (is_row_vector) this->setData(size,1);
                    else this->setData(1,size);
                }

        };
    }
}
#endif
