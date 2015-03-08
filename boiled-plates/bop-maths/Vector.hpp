#ifndef BOP_VECTOR_HPP
#define BOP_VECTOR_HPP

#include <cstring>
#include <initializer_list>
#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include "Matrix.hpp"
#include "../bop-defaults/types.hpp"
/*
    bop::maths::Vector Class file
*/

namespace bop {
    #ifdef BOP_USE_INFERIOR_NAMESPACE
    namespace math {
    #else
    namespace maths {
    #endif
        template<class T>
        class Vector : public Matrix<T> {
                /*
                    Version of Vector that inherits from Matrix.
                */
            public:

                Vector() : Matrix<T>() {

                }
                
                Vector(const uint_type size, bool is_row_vector = false) : Vector() {
                    if (is_row_vector) this->setData(size,1);
                    else this->setData(1,size);
                }

                uint_type size() const {
                    return this->width() * this->height();
                }

                /*
                    Coordinate related functions
                */

                T& x() const {
                    return this->element(0);
                }

                T& y() const {
                    return this->element(1);
                }

                T& z() const {
                    return this->element(2);
                }

                T& w() const {
                    return this->element(this->size() - 1)
                }
        };
    }
}
#endif
