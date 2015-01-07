#ifndef BOP_VECTOR_HPP
#define BOP_VECTOR_HPP

#include <cstring>
#include <initializer_list>
#include <string>
#include <sstream>
#include <iostream>
#include <utility>
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
        class Matrix;
        template<class T>
        class Vector {
            protected:
                friend class Matrix<T>;
                T zero;

                inline T& checkZero() {
                    /*
                        Awful hack to get a reference to a value 0 for now.
                        Alternate route is not allowing alterations to values
                        retrieved by coordinate related functions which, while
                        seeming elegant, is infuriating.
                    */
                    this->zero = 0;
                    return this->zero;
                }
                T* data;
                uint_type width;
                int pivot_index;

                void setData(uint_type size, bool delete_ptr = true) {
                    if (this->data == nullptr || (this->width < size && delete_ptr)) {
                        this->data = static_cast<T*>(realloc(this->data, size * sizeof(T)));
                    }
                    this->width = size;
                }

            public:

                //Constructors
                Vector(uint_type size, T fill = 0) : Vector() {
                    /*
                        Creates a new vector of a given width, setting each
                        element as a given default.
                        ----pivot index is set as -1.
                    */
                    this->pivot_index = -1;
                    this->setData(size);
                    for (uint_type i = 0; i < this->width; i++) {
                        this->data[i] = fill;
                    }
                }


                Vector(std::initializer_list<T> list) : Vector() {
                    /*
                        Creates a new Vector from an initializer list.
                        ----pivot_index is determined during construction.
                        ----width is determined from size of list.
                    */
                    this->setData(list.size());
                    this->pivot_index = -1;
                    int i = 0;
                    for (auto elem : list) {
                        this->data[i] = static_cast<T>(elem);
                        if (this->pivot_index < 0 && elem != 0) {
                            this->pivot_index = i;
                        }
                        i++;
                    }
                }

                Vector(const Vector<T>& vec) : Vector() {
                    /*
                        Creates a new Vector from a Vector, a copy constructor.
                        ----copies all data from the Vector, incl. deep copy of data array pointer.
                    */
                    this->setData(vec.size());
                    this->pivot_index = vec.pivot_index;
                    memcpy(this->data, vec.data, vec.size() * sizeof(T));
                }

                Vector(Vector<T>&& vec) : Vector() {
                    /*
                        Move constructor.
                    */
                    this->width = vec.width;
                    this->pivot_index = vec.pivot_index;
                    std::swap(this->data, vec.data);
                }

                Vector() {
                    /*
                        Empty constructor.
                    */
                    this->data = nullptr;
                    this->pivot_index = 0;
                    this->zero = 0;
                    this->width = 0;
                }

                //Destructor

                ~Vector() {
                    //Manual deletion of data array.
                    if (this->data != nullptr) delete[] this->data;
                }

                //Operator Overloads

                inline T& operator[] (const uint_type index) const {
                    /*
                        Index operator, returns the data at the given index.
                        ----synonymous to Vector::data[const unsigned int]
                    */
                    return this->data[index];
                }

                Vector<T>& operator= (const Vector<T>& vec) {
                    /*
                        Copy assignment, synonymous to the copy constructor.
                    */

                    this->setData(vec.size());
                    memcpy(this->data, vec.data, vec.size() * sizeof(T));
                    this->pivot_index = vec.pivot_index;
                    return *this;
                }

                template<class A>
                Vector<T>& operator= (std::initializer_list<A> list) {
                    /*
                        Initializer list assignment operator, synonymous
                        with the constructor that takes the same argument.
                        ----copied here so the list is not passed by value
                    */
                    this->setData(list.size());
                    this->pivot_index = -1;
                    int i = 0;
                    for (T elem : list) {
                        this->data[i] = static_cast<T>(elem);
                        if (this->pivot_index < 0 && elem != 0) {
                            this->pivot_index = i;
                        }
                        i++;
                    }
                    return *this;
                }

                //Boolean Logic overloads

                bool operator== (const Vector<T>& vec) const {
                    if (this->width == vec.width) {
                        /*
                            Not the most elegant solution, then again for testing
                            an array there isn't much choice.
                        */
                        bool same = true;
                        for (uint_type i = 0; i < this->width && same; i++) {
                            same = (this->data[i] == vec.data[i]);
                        }
                        return same;
                    }
                    else return false;
                }

                bool operator!= (const Vector<T>& vec) const {
                    return !(this->operator==(vec));
                }

                //Arithmetic Operator Overloads

                Vector<T>& operator*= (const T scalar) {
                    /*
                        Member multiplication operator, multiplies all
                        elements in a vector by the given scalar.
                    */
                    if (scalar == 0) return *this;
                    for (uint_type i = 0; i < this->width; i++) {
                        this->data[i] *= scalar;
                    }
                    return *this;
                }

                Vector<T>& operator/= (const T scalar) {
                    /*
                        Member division operator, divides all
                        elements in a vector by the given scalar.
                        ----synonymous with "vec *= 1/scalar"
                    */
                    if (scalar == 0) return *this;
                    for (uint_type i = 0; i < this->width; i++) {
                        this->data[i] /= scalar;
                    }
                    return *this;
                }

                Vector<T>& operator+= (const Vector<T>& vec) {
                    /*
                        Member addition operator, adds the elements of
                        a given vector to the corresponding vector to the
                        corresponding elements of a subject vector.
                        ----sets the pivot as needing to be recalculated.
                    */
                    for (uint_type i = 0; i < this->width; i++) {
                        this->data[i] += vec[i];
                    }
                    this->pivot_index = -1;
                    return *this;
                }

                Vector<T>& operator-= (const Vector<T>& vec) {
                    /*
                        Member subtraction operator, subtracts the elements
                        of a given vector from the corresponding elements of
                        a subject vector.
                        ----sets the pivot as needing to be recalculated.
                    */
                    for (uint_type i = 0; i < this->width; i++) {
                        this->data[i] -= vec[i];
                    }
                    this->pivot_index = -1;
                    return *this;
                }

                Vector<T> operator- () const {
                    /*
                        Member negation operator, returns a vector identical to
                        the vector but with all elements multiplied by -1.
                    */
                    Vector<T> vec(*this);
                    for (uint_type elem = 0; elem < vec.size(); elem++) {
                        vec[elem] *= -1;
                    }
                    return vec;
                }

                //Vector information functions

                int pivot(bool recalc = true) {
                    /*
                        Returns the position of the first non-zero element in
                        the vector.
                    */
                    if (recalc || this->pivot_index < 0) {
                        this->pivot_index = -1;
                        for (uint_type i = 0; i < this->width && this->pivot_index < 0; i++) {
                            if (this->data[i] != 0) this->pivot_index = i;
                        }
                    }
                    return this->pivot_index;
                }

                uint_type size() const {
                    /*
                        Returns the width of the vector.
                    */
                    return this->width;
                }

                inline bool valid() const {
                    //Vector's validity determined by the pointer's value.
                    return (this->data != nullptr);
                }

                explicit operator bool() const {
                    /*
                        Cast to bool as the validity of the vector. Allowing for "if (Vector) { ... }"
                        behaviour.
                    */
                    return this->valid();
                }

                std::string string(bool brackets = true) const {
                    /*
                        Returns a string that represents the data in the Vector
                        in a human-readable format.
                    */
                    std::ostringstream vec_str;
                    if (this->valid()) {
                        if (brackets) vec_str << "(";
                        for (uint_type i = 0; i < this->width; i++) {
                            vec_str << this->data[i];
                            if (i + 1 < this->width) vec_str << ',';
                        }
                        if (brackets) vec_str << ")";
                    }
                    return vec_str.str();
                }

                template <class T_>
                friend std::ostream& operator<< (std::ostream& stream, const Vector<T_>& vec);

                T mag() const {
                    T magnitude = 0;
                    for (uint_type elem = 0; elem < this->size(); elem++) {
                        magnitude += (*this)[elem] * (*this)[elem];
                    }
                    return static_cast<T>(sqrt(magnitude));
                }

                /*
                    Coordinate functions, synonymous with Vector::operator[](0/1/2/3).
                    Checks that those indices exist before returning.
                */

                inline T& x() {
                    if (this->width > 0) return this->data[0];
                    else {
                        if (this->zero != 0) this->zero = 0;
                        return this->checkZero();
                    }
                }

                inline T& y() {
                    if (this->width > 1) return this->data[1];
                    else return this->checkZero();
                }

                inline T& z() {
                    if (this->width > 2) return this->data[2];
                    else return this->checkZero();
                }

                inline T& w() {
                    if (this->width > 3) return this->data[3];
                    else return this->checkZero();
                }

                /*
                    Member swap function, intended for use in algorithms such
                    as Gauss-Jordan elimination, or whatever else may be in need
                    of the swapping of Vectors.
                */
                void impose(const Vector<T>& vec, uint_type offset = 0) {
                    for (uint_type i = 0; i < vec.width && i + offset < this->width; i++) {
                        this->data[i + offset] = vec[i];
                    }
                }
        };


        typedef Vector<double> vector;

        /*
            External arithmetic operators, return a vector separate from
            the given parameters.
        */

        template<class T>
        Vector<T> operator* (const Vector<T>& vec, const T scalar) {
            Vector<T> vec_p(vec);
            vec_p *= scalar;
            return vec_p;
        }

        template<class T>
        Vector<T> operator/ (const Vector<T>& vec, const T scalar) {
            Vector<T> vec_p(vec);
            vec_p /= scalar;
            return vec_p;
        }

        template<class T>
        Vector<T> operator+ (const Vector<T>& vec1, const Vector<T>& vec2) {
            Vector<T> vec_p(vec1);
            vec_p += vec2;
            return vec_p;
        }

        template<class T>
        Vector<T> operator- (const Vector<T>& vec1, const Vector<T>& vec2) {
            Vector<T> vec_p(vec1);
            vec_p -= vec2;
            return vec_p;
        }

        template <class T>
        std::ostream& operator<< (std::ostream& stream, const Vector<T>& vec) {
            stream << vec.string();
            return stream;
        }

    }
}

#endif
