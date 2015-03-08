#ifndef BOP_MATRIX_HPP
#define BOP_MATRIX_HPP
#ifndef BOP_MATRIX_DISCARD_BY
#define BOP_MATRIX_DISCARD_BY 0xFFFF
#endif
#include <initializer_list>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <map>
#include <type_traits>
#include <vector>
#include "MathsExtra.hpp"
#include "Vector.hpp"
#include "../bop-defaults/types.hpp"
#ifdef BOP_MATRIX_USE_RECYCLER
#include "../bop-memory/Recycler.hpp"
#include "../bop-memory/PrimativeArrayContainer.hpp"
#endif

/*
    bop::maths::Matrix class file
*/

namespace bop {
    #ifdef BOP_USE_INFERIOR_NAMESPACE
    namespace math {
    #else
    namespace maths {
    #endif
        template<class T>
        class Matrix {
            protected:
                #ifdef BOP_TYPE_TRAIT_CHECKS
                static_assert(std::is_trivially_copyable<T>::value, "The class bop::maths::Matrix<T> can only have T be a trivially copyable type.");
                static_assert(std::is_trivially_destructable<T>::value, "The class bop::maths::Matrix<T> can only have T be trivially destructable.");
                #endif
                #ifdef BOP_MATRIX_USE_RECYCLER
                static mem::PrimativeArrayRecycler<T> recycler;
                #endif
                uint_type matrix_width;
                uint_type matrix_height;
                T* data;

                inline void setData(uint_type width, uint_type height, bool delete_ptr = true) {
                    if (this->data == nullptr || (delete_ptr && (width * height) > this->width() * this->height())) {
                        this->data = static_cast<T*>(realloc(this->data, width * height * sizeof(T)));
                    }
                    this->matrix_width = width;
                    this->matrix_height = height;
                }

                class MatrixIndexHandler {
                    /*
                        Internal handler class for translating [row][col]
                        to (row * width) + col for row-major order.
                    */
                    public:
                        uint_type temp_index;
                        Matrix<T>& matrix;

                        MatrixIndexHandler(Matrix<T>& mat) : matrix(mat)  {
                            this->temp_index = 0;
                        }

                        uint_type pivot() {
                            uint_type col;
                            for (col = 0; col < this->matrix.width(); col++) {
                                if (this->matrix.element(this->temp_index,col) != 0) break;
                            }
                            return col;
                        }

                        inline T& operator[](uint_type index) {
                            return this->matrix.element(this->temp_index, index);
                        }
                };

                MatrixIndexHandler handler;
            public:

                //Constructors
                Matrix() : handler(*this) {
                    /*
                        Empty constructor.
                    */
                    this->data = nullptr;
                    this->matrix_width = 0;
                    this->matrix_height = 0;
                }

                Matrix(uint_type width, uint_type height, T fill = 0) : Matrix() {
                    /*
                        Non-square matrix constructor, sets all values as
                        the given fill value.
                    */
                    this->setData(width,height,false);
                    for (uint_type elem = 0; elem < this->width() * this->height(); elem++) {
                        this->element(elem) = fill;
                    }
                }

                template<class A>
                Matrix(std::initializer_list< std::initializer_list<A> > list) : Matrix() {
                    /*
                        Creates a matrix from a 2-dimensional initializer_list.
                    */
                    this->setData(list.size(), list.begin()->size(), false);
                    uint_type row = 0;
                    for (auto sublist : list) {
                        uint_type col = 0;
                        for (auto element : sublist) {
                            this->element(row,col) = element;
                            col++;
                        }
                        row++;
                    }
                }

                Matrix(const Matrix<T>& mat) : Matrix() {
                    /*
                        Copy constructor.
                    */
                    this->setData(mat.width(),mat.height(),false);
                    memcpy(this->data, mat.data, mat.width() * mat.height() * sizeof(T));
                }

                template<class A>
                Matrix(const Matrix<A>& mat_tocast) : Matrix() {
                    this->setData(mat_tocast.width(), mat_tocast.height(), false);
                    for (uint_type elem = 0; elem < mat_tocast.width() * mat_tocast.height(); elem++) this->element(0,elem) = static_cast<T>(mat_tocast.element(0,elem));
                }

                #ifndef BOP_MATRIX_DEFAULT_MOVE
                Matrix(Matrix<T>&& mat) : Matrix() {
                    /*
                        Move constructor.
                    */
                    #ifdef BOP_MATRIX_SWAPMOVE
                    std::swap(this->matrix_width, mat.matrix_width);
                    std::swap(this->matrix_height, mat.matrix_height);
                    std::swap(this->data, mat.data);
                    #else
                    this->matrix_width = mat.matrix_width;
                    this->matrix_height = mat.matrix_height;
                    this->data = mat.data;
                    mat.data = nullptr;
                    #endif
                }
                #endif

                //Destructor
                ~Matrix() {
                    #ifdef BOP_MATRIX_USE_RECYCLER
                    if (this->data != nullptr) Matrix<T>::recycler.give(this->data, this->width() * this->height());
                    #else
                    delete[] this->data;
                    #endif
                    this->data = nullptr;
                }

                //Operator overloads
                #ifndef BOP_MATHS_MATRIX_MT_COMPAT_INDEX
                inline MatrixIndexHandler& operator[] (const uint_type index) {
                #else
                inline MatrixIndexHandler operator[] (const uint_type index) {
                #endif
                this->handler.temp_index = index;
                    return this->handler;
                }

                inline Matrix<T>& operator= (const Matrix<T>& mat) {
                    this->setData(mat.width(), mat.height());
                    memcpy(this->data, mat.data, this->width() * this->height() * sizeof(T));
                    return *this;
                }

                inline Matrix<T>& operator= (Matrix<T>&& mat) {
                    #ifdef BOP_MATRIX_SWAPMOVE
                    std::swap(this->matrix_width, mat.matrix_width);
                    std::swap(this->matrix_height, mat.matrix_height);
                    std::swap(this->data, mat.data);
                    #else
                    this->matrix_width = mat.matrix_width;
                    this->matrix_height = mat.matrix_height;
                    this->data = mat.data;
                    mat.data = nullptr;
                    #endif
                    return *this;
                }

                //Boolean logic overloads
                inline bool operator== (const Matrix<T>& mat) const {
                    if (this->width() == mat.width() && this->height() == mat.height()) {
                        bool same = true;
                        for (uint_type row = 0; row < this->height() && same; row++) {
                            for (uint_type col = 0; col < this->width() && same; col++) {
                                same = this->element(row,col) == mat.element(row,col);
                            }
                        }
                        return same;
                    }
                    else return false;
                }

                bool operator!= (const Matrix<T>& mat) const {
                    return !((*this) == mat);
                }

                //Arithmetic overloads
                Matrix<T>& operator*= (const T scalar) {
                    /*
                        Scalar multiplication member operator, multiplies all
                        elements by the given scalar.
                    */
                    for (uint_type elem = 0; elem < this->height() * this->width(); elem++) {
                        this->element(elem) *= scalar;
                    }
                    return *this;
                }

                Matrix<T>& operator*= (const Matrix<T>& mat) {
                    /*
                        Naive matrix multiplication implementation
                    */
                    #ifdef BOP_MATRIX_USE_RECYCLER
                    T* temp = Matrix<T>::recycler.request(this->height() * mat.width());
                    #else
                    T* temp = new T[this->height() * mat.width()];
                    #endif
                    for (uint_type row = 0; row < this->height(); row++) {
                        for (uint_type col = 0; col < mat.width(); col++) {
                            temp[(row * mat.width()) + col] = 0;
                            for (uint_type iter = 0; iter < mat.width(); iter++) {
                                temp[(row * mat.width()) + col] += this->element(row,iter) * mat.element(iter,col);
                            }
                        }
                    }
                    /*
                    //This method is actually slower, surprisingly.
                    for (uint_type elem = 0; elem < mat.width() * this->height(); elem++) {
                        //equation for col = elem % mat.width()
                        //equation for row = (elem - col) / mat.width()
                        temp[elem] = 0;
                        for (uint_type iter = 0; iter < mat.width(); iter++) {
                            temp[elem] += this->element((elem - (elem % mat.width())) / mat.width(), iter) * mat.element(iter, elem % mat.width());
                        }
                    }
                    */
                    #ifdef BOP_MATRIX_USE_RECYCLER
                    Matrix<T>::recycler.give(this->data, this->width() * this->height());
                    #else
                    delete[] this->data;
                    #endif
                    this->data = temp;
                    this->matrix_width = mat.matrix_width;
                    return *this;
                }

                Vector<T>& operator*= (Vector<T>& vec) const {
                    /*
                        Seemingly counter-intuitive multiplication
                        operation, although *= is being used with the
                        matrix as the left operand what is being
                        multiplied is the vector. The matrix is unaffected.
                    */
                    if (vec.width == this->width()) {
                        T* temp_data = new T[this->height()];
                        for (uint_type i = 0; i < this->height(); i++) {
                            T product = 0;
                            for (uint_type dot_index = 0; dot_index < vec.size(); dot_index++) {
                                product += vec[dot_index] * this->element(i,dot_index);
                            }
                            temp_data[i] = product;
                            #ifdef BOP_MATRIX_MULTIPLY_DISCARD_TINY
                            temp_data[i] += BOP_MATRIX_DISCARD_BY;
                            temp_data[i] -= BOP_MATRIX_DISCARD_BY;
                            #endif
                        }
                        delete[] vec.data;
                        vec.data = temp_data;
                        vec.width = this->height();
                    }
                    return vec;
                }

                Matrix<T>& operator/= (const T scalar) {
                    /*
                        Scalar division member operator, divides all elements
                        by the given scalar.
                    */
                    if (scalar != 0) {
                        (*this) *= (1/scalar);
                    }
                    return *this;
                }

                Matrix<T>& operator+= (const Matrix<T> &mat) {
                    /*
                        Addition member operator, adds all the elements of a
                        given matrix to the matrix.
                    */
                    if (this->width() == mat.width() && this->height() == mat.height()) {
                        for (uint_type elem = 0; elem < this->height() * this->width(); elem++) {
                            this->element(elem) += mat.element(elem);
                        }
                    }
                    else {
                        for (uint_type row = 0; row < this->height(); row++) {
                            for (uint_type col = 0; col < this->width(); col++) {
                                this->element(row,col) += mat.element(row,col);
                            }
                        }
                    }
                    return *this;
                }

                Matrix<T>& operator-= (const Matrix<T> &mat) {
                    /*
                        Subtraction member operator, subtracts the value of
                        each element in a given matrix from the matrix.
                    */
                    if (this->width() == mat.width() && this->height() == mat.height()) {
                        for (uint_type elem = 0; elem < this->height() * this->width(); elem++) {
                            this->element(elem) -= mat.element(elem);
                        }
                    }
                    else {
                        for (uint_type row = 0; row < this->height(); row++) {
                            for (uint_type col = 0; col < this->width(); col++) {
                                this->element(row,col) -= mat.element(row,col);
                            }
                        }
                    }
                    return *this;
                }

                Matrix<T> operator- () {
                    Matrix<T> mat(*this);
                    for (uint_type elem = 0; elem < this->width() * this->height(); elem++) {
                        mat.element(elem) *= -1;
                    }
                    return mat;
                }

                //Information functions

                inline T& element(uint_type row, uint_type col) const {
                    if (row == 0) return this->data[col];
                    else return this->data[(row * this->width()) + col];
                }

                inline T& element(uint_type elem) const {
                    return this->data[elem];
                }

                inline uint_type width() const {
                    /*
                        Returns the width of the Matrix.
                    */
                    return this->matrix_width;
                }

                inline uint_type height() const {
                    /*
                        Returns the height of the Matrix.
                    */
                    return this->matrix_height;
                }

                inline bool square() const {
                    /*
                        Tests if the Matrix is a square Matrix.
                    */
                    return (this->width() == this->height());
                }

                inline bool square(uint_type size) const {
                    return (this->matrix_width == size && this->matrix_width == this->matrix_height);
                }

                std::string string(bool newlines = true) const {
                    /*
                        Similar to the maths::Vector::string function, returns
                        a human-readable representation of the Matrix.
                    */
                    std::ostringstream mat_str;
                    if (newlines) mat_str << '\n';
                    for (uint_type row = 0; row < this->height(); row++) {
                        mat_str << '[' << std::showpos;
                        for (uint_type col = 0; col < this->width(); col++) {
                            mat_str << this->element(row,col);
                            if (col + 1 < this->width()) mat_str << ',';
                        }
                        mat_str << ']';
                        if (newlines) mat_str << '\n';
                    }
                    return mat_str.str();
                }

                template<class T_>
                friend std::ostream& operator<< (std::ostream& stream, const Matrix<T_>& mat);

                inline bool valid() const {
                    //Matrix validity determined by the pointer's value.
                    return (this->data != nullptr);
                }

                explicit operator bool() const {
                    /*
                        Cast to bool as the validity of the matrix. Allowing for "if (Matrix) { ... }"
                        behaviour.
                    */
                    return this->valid();
                }

                explicit operator std::string() const {
                    /*
                        Addditional way of getting a string from a Matrix object by casting.
                    */
                    return this->string();
                }

                struct LU {
                    Matrix<T> lower;
                    Matrix<T> upper;
                };

                LU decompose() const {
                    LU LU_pair;

                    LU_pair.lower = Matrix<T>(this->width(), this->height());
                    LU_pair.upper = Matrix<T>(*this);
                    /*
                        Setting up the first element of the pair as the Upper matrix by copying this
                        matrix into it, and the second element as the Lower matrix by initialising
                        it as an identity matrix.
                    */
                    for (uint_type elem = 0; elem < this->width() && elem < this->height(); elem++) {
                        LU_pair.lower[elem][elem] = 1;
                    }
                    /*
                        With the matrices initialised, the Upper matrix to-be needs to be reduced,
                        with the scalar of each row reduction placed in the Lower matrix.
                    */
                    for (uint_type ind_col = 0; ind_col < this->width() && ind_col < this->height(); ind_col++) {
                        for (uint_type row_red = ind_col + 1; row_red < this->height() && row_red < this->width(); row_red++) {
                            if (LU_pair.upper.element(row_red,ind_col) != 0) {
                                LU_pair.lower.element(row_red,ind_col) = LU_pair.upper.element(row_red,ind_col) / LU_pair.upper.element(ind_col,ind_col);
                                for (uint_type elem = ind_col; elem < this->width(); elem++) {
                                    LU_pair.upper.element(row_red,elem) -= LU_pair.lower.element(row_red,ind_col) * LU_pair.upper.element(ind_col, elem);
                                }
                            }
                        }
                    }
                    return LU_pair;
                }

                T det(bool allow_recursive = true) const {
                    if (!this->square()) return 0;
                    else if (this->width() == 2) {
                        return (this->element(0,0) * this->element(1,1)) - (this->element(0,1) * this->element(1,0));
                    }
                    else if (this->width() < 5 && allow_recursive) {
                        /*
                            start and launch recursive determinant.
                        */
                        Vector<bool> allowed_cols(this->width(), true);
                        T deter = 0;
                        T multi = 1;
                        uint_type size = this->height() - 1;
                        for (uint_type i = 0; i < this->width(); i++) {
                            if (this->element(0,i) != 0) {
                                allowed_cols[i] = false;
                                deter += (multi * (this->element(0,i) * this->detCascade(allowed_cols, size)));
                                multi *= -1;
                                allowed_cols[i] = true;
                            }
                        }
                        return deter;
                    }
                    else {
                        /*
                            As we are simply finding the product of the pivots
                            on an upper triangular matrix, we can generate just
                            the upper matrix as the determinant of the lower matrix
                            is always one, and the product of the lower and upper
                            matrix determinants is equal to the determinant of the
                            original matrix.
                        */
                        Matrix<T> upper(*this);
                        for (uint_type ind_col = 0; ind_col < this->width() && ind_col < this->height(); ind_col++) {
                            for (uint_type row_red = ind_col + 1; row_red < this->height() && row_red < this->width(); row_red++) {
                                if (upper.element(row_red,ind_col) == 0) continue;
                                T temp = upper.element(row_red,ind_col) / upper.element(ind_col,ind_col);
                                for (uint_type elem = ind_col; elem < this->width(); elem++) {
                                    upper.element(row_red,elem) -= temp * upper.element(ind_col,elem);
                                }
                            }
                        }
                        T deter = 1;
                        for (uint_type elem = 0; elem < this->height(); elem++) deter *= this->element(elem,elem);
                        return deter;
                    }
                }

            private:

                T detCascade(Vector<bool>& allowed_cols, uint_type& size) const {
                    if (size == 2) {
                        //the base case, finding the 2 by 2 matrix determinant
                        int c1 = -1, c2 = -1;
                        for (uint_type i = 0; i < this->width(); i++) {
                            if (allowed_cols[i]) {
                                if (c1 < 0) {
                                    c1 = i;
                                }
                                else if (c2 < 0) {
                                    c2 = i;
                                    break;
                                }
                            }
                        }
                        return (this->element(this->height() - 2,c1) * this->element(this->height() - 1,c2)) - (this->element(this->height() - 2,c2) * this->element(this->height() - 1,c1));
                    }
                    else {
                        size--;
                        T deter = 0;
                        T multi = 1;
                        for (uint_type i = 0; i < this->width(); i++) {
                            if (!allowed_cols[i]) continue;
                            else {
                                allowed_cols[i] = false;
                                deter += (multi * (this->element(this->height() - size,i) * this->detCascade(allowed_cols, size)));
                                multi *= -1;
                                allowed_cols[i] = true;

                            }
                        }
                        size++;
                        return deter;
                    }
                }

            public:

                Matrix<T>& invert() {
                    T deter = this->det();
                    if (deter != 0) {
                        if (this->height() == 2) {
                            std::swap(this->element(0,0), this->element(1,1));
                            this->element(0,1) *= -1;
                            this->element(1,0) *= -1;
                            (*this) /= deter;
                        }
                        else {
                            Matrix<T> inverse(this->width(),this->height());
                            for (uint_type elem = 0; elem < this->height(); elem++) inverse.element(elem,elem) = 1;
                            /*
                                non-2x2 matrix inverse solution
                            */
                            for (uint_type col = 0; col < this->width(); col++) {
                                uint_type row_index;
                                /*
                                    Find the first row whose pivot index is the same as the
                                    identity column (col) that needs representation. This is
                                    the Pivot row.
                                */
                                for (row_index = col; row_index < this->height() && (*this)[row_index].pivot() != col; row_index++);
                                /*
                                    Simultaneously swap the rows in the given matrix and to-be
                                    inverse matrix so that the Pivot row is at the index where
                                    it's pivot index matches the row index.
                                */
                                this->swapRows(col, row_index);
                                inverse.swapRows(col, row_index);
                                /*
                                    Divide the row at the Pivot row's index in the to-be inverse
                                    by the value at the Pivot row's pivot value. Then, divide the
                                    Pivot row by it's pivot value, making the pivot value in the
                                    Pivot row 1.
                                */
                                T div_temp = this->element(col,col);
                                for (uint_type div_col = 0; div_col < this->width(); div_col++) {
                                    inverse.element(col,div_col) /= div_temp;
                                    this->element(col,div_col) /= div_temp;
                                }

                                for (uint_type i = col + 1; i < this->height(); i++) {
                                    if (i == col || (*this)[i].pivot() != col) continue;
                                    else {
                                        /*
                                            For each row in the matrix with a pivot index identical to
                                            the Pivot row's index, subtract the Pivot row multiplied by
                                            their pivot's value from the row, leaving their pivot value
                                            as 0. Mirror these actions on the to-be inverse Matrix.
                                        */
                                        T temp = this->element(i,col);
                                        for (uint_type elem = 0; elem < this->width(); elem++) {
                                            this->element(i,elem) -= this->element(col,elem) * temp;
                                            inverse.element(i,elem) -= inverse.element(col,elem) * temp;
                                        }
                                    }
                                }
                            }
                            for (uint_type col = this->height() - 1; col > 0; col--) {
                                for (uint_type sub_row = 0; sub_row < col; sub_row++) {
                                    T temp = this->element(sub_row,col);
                                    /*
                                        Now the matrix is in row echelon form, reduce it to an
                                        identity matrix.
                                    */
                                    for (uint_type elem = 0; elem < this->width(); elem++) {
                                        this->element(sub_row,elem) -= this->element(col,elem) * temp;
                                        inverse.element(sub_row,elem) -= inverse.element(col,elem) * temp;
                                    }
                                }
                            }
                            std::swap(this->data, inverse.data);
                        }
                    }
                    return *this;
                }

                inline Matrix<T> inverted() const {
                    return Matrix<T>(*this).invert();
                }

                #ifdef BOP_MATRIX_ALLOW_INVERSE_METHOD
                //macro-enabled method name alternative.
                inline Matrix<T> inverse() const {return Matrix<T>(*this).invert();}
                #endif

                inline Matrix<T>& transpose() {
                    if (this->width() == this->height()) {
                        for (uint_type row = 0; row < this->height(); row++) {
                            for (uint_type col = row + 1; col < this->width(); col++) {
                                std::swap(this->element(row,col), this->element(col,row));
                            }
                        }
                    }
                    else {
                        Matrix<T> mat_transpose(this->height(),this->width());
                        for (uint_type row = 0; row < this->height(); row++) {
                            for (uint_type col = 0; col < this->width(); col++) {
                                mat_transpose.element(col,row) = this->element(row,col);
                            }
                        }
                        std::swap(this->data,mat_transpose.data);
                        std::swap(this->matrix_width, this->matrix_height);
                    }
                    return *this;
                }

                const static uint_type rowvec = 1;
                const static uint_type colvec = 2;

                inline Matrix<T> transposed() const {
                    return Matrix<T>(*this).transpose();
                }

                inline bool isVector() const {
                    /*
                        Returns true if either of the matrix's dimentions square
                        1. Indicates if the matrix represents a Vector.
                    */
                    return (this->width() == 1 || this->height() == 1);
                }

                inline bool isVector(uint_type type) const {
                    if (type == Matrix<T>::rowvec) return (this->height() == 1);
                    else if (type == Matrix<T>::rowvec) return (this->width() == 1);
                    else return false;
                }

                //Matrix manipulation functions.

                inline void swapRows(uint_type index_a, uint_type index_b) {
                    /*
                        Row swap function, for use in such algorithms as Gauss-Jordan
                        elimination.
                    */
                    if (index_a != index_b) {
                        for (uint_type col = 0; col < this->width(); col++) {
                            std::swap(this->element(index_a,col), this->element(index_b,col));
                        }
                    }
                }

                void impose(Matrix<T>& mat, uint_type row_off = 0, uint_type col_off = 0) {
                    for (uint_type row = 0; row < mat.height() && row + row_off < this->height(); row++) {
                        for (uint_type col = 0; col < mat.width() && col + col_off < this->width(); col++) {
                            this->element(row + row_off, col + col_off) = mat.element(row,col);
                        }
                    }
                }

                void impose(Vector<T>& vec, uint_type row_off = 0, uint_type col_off = 0) {
                    for (uint_type col = 0; col < vec.size() && col + col_off < this->width(); col++) {
                        this->element(row_off, col + col_off) = vec[col];
                    }
                }
        };

        #ifdef BOP_MATRIX_USE_RECYCLER
        template<class T>
        mem::PrimativeArrayRecycler<T> Matrix<T>::recycler = mem::PrimativeArrayRecycler<T>();
        #endif
        typedef Matrix<prec_type> matrix;

        //External arithmetic overloads

        template<class T>
        Matrix<T> operator* (Matrix<T>& mat, const T scalar) {
            Matrix<T> mat_p(mat);
            mat_p *= scalar;
            return mat_p;
        }

        template<class T>
        Matrix<T> operator* (const Matrix<T>& mat1, const Matrix<T>& mat2) {
            Matrix<T> mat_p(mat1);
            mat_p *= mat2;
            return mat_p;
        }

        template<class T>
        Vector<T> operator* (const Matrix<T>& mat, Vector<T>& vec) {
            Vector<T> vec_p(vec);
            mat *= vec_p;
            return vec_p;
        }

        template<class T>
        Matrix<T> operator/ (const Matrix<T>& mat, const T scalar) {
            Matrix<T> mat_p(mat);
            mat_p /= scalar;
            return mat_p;
        }

        template<class T>
        Matrix<T> operator+ (const Matrix<T>& mat1, const Matrix<T>& mat2) {
            return Matrix<T>(mat1) += mat2;
        }

        template<class T>
        Matrix<T> operator- (const Matrix<T>& mat1, const Matrix<T>& mat2) {
            return Matrix<T>(mat1) -= mat2;
        }

        template<class T>
        std::ostream& operator<< (std::ostream& stream, const Matrix<T>& mat) {
            //c++ i/o overload, allowing "std::cout << Matrix<T> << std::endl;" behaviour.
            stream << mat.string();
            return stream;
        }

        template<class T>
        struct IdentityMatrix {
            /*
                Identity matrix factory
            */
            static Matrix<T> make(uint_type size) {
                return make(size,size);
            }

            static uint_type encodeMatrixDimentions(uint_type width, uint_type height) {
                /*
                    If the matrix is of a reasonable size, then the
                    width and height are stored in two halfs of a
                    uint_type that is used to index for the unordered
                    map.

                    eg. square matrix of size 6 where uint_type is 16 bit
                    |--width---|--height--|
                    |-00000110-|-00000110-| -> 1542 in decimal

                    this value usually encapsulates a matrix in
                    an application.
                */
                uint_type size_pair = 0;
                if (width < (~size_pair)/2 && height < (~size_pair)/2) {
                    size_pair |= (width << minBits(~uint_type(0))/2);
                    size_pair |= height;
                }
                return size_pair;
            }

            static Matrix<T> make(uint_type width, uint_type height) {
                static std::unordered_map<uint_type,Matrix<T> > map;
                uint_type size_pair = encodeMatrixDimentions(width,height);
                if (map[size_pair]) return map[size_pair];
                else {
                    map[size_pair] = Matrix<T>(width,height);
                    for (uint_type elem = 0; elem < width * height; elem += width + 1) {
                        map[size_pair].element(elem) = 1;
                    }
                    return map[size_pair];
                }

            }
        };

        template<class T>
        struct RotationMatrix {
            /*
                Rotation matrix factory.
            */
            static Matrix<T> make(T angle, bool clockwise = false, bool rads = false) {
                static std::unordered_map<T,Matrix<T> > map;
                if (clockwise) angle -= 180;
                if (!rads) angle = ((pi/180.0) * angle); //Convert the degrees to radians manually
                if (map[angle]) return map[angle];
                else {
                    map[angle] = {{static_cast<T>(cos(angle)), static_cast<T>(-sin(angle))},
                                  {static_cast<T>(sin(angle)), static_cast<T>(cos(angle))}};
                    return map[angle];
                }
            }
        };
        struct OffsetMatrix {
            /*
                Offset matrix factory. returns a constant matrix reference that
                represents the index offset in the one-dimentional array representation
                of the matrix during transposition.
            */
            static inline int_type offset(uint_type width, uint_type height, uint_type row, uint_type col) {
                /*
                    Returns the index offset caused by transposition
                */
                return (col * (height - 1)) - (row * (width - 1));
            }

            static inline uint_type rowOf(uint_type width, uint_type index) {
                return (index - colOf(width,index)) / width;
            }

            static inline uint_type colOf(uint_type width, uint_type index) {
                return index % width;
            }

            template<class T>
            static const Matrix<int_type>& make(const Matrix<T>& matrix) {
                return OffsetMatrix::make(matrix.width(),matrix.height());
            }

            static const Matrix<int_type>& make(uint_type width, uint_type height) {
                static std::map<uint_type,Matrix<int_type> > map;
                uint_type encoded = IdentityMatrix<uint_type>::encodeMatrixDimentions(width,height);
                if (map[encoded]) return map[encoded];
                else {
                    Matrix<int_type> mat = Matrix<int_type>(width,height);
                    for (uint_type row = 0; row < mat.height(); row++) {
                        for (uint_type col = 0; col < mat.width(); col++) {
                            mat[row][col] = OffsetMatrix::offset(width,height,row,col);
                        }
                    }
                    map[encoded] = mat;
                    return map[encoded];
                }
            }
        };

        template<class T>
        struct PrimeMatrix {
            /*
                Factory that creates a matrix comprised completely of
                primes numbers that do no appear anywhere else in the
                matrix.
            */
            /*
            static Matrix<T> make(uint_type width, uint_type height) {
                static std::vector<uint_type> prime_numbers = ([]() -> std::vector<uint_type> {
                    std::vector<uint_type> vec(10000);
                    for (uint_type iter = 0; iter < vec.size(); iter++) {
                        vec[iter] = nth_prime(iter);
                    }
                    return vec;
                })();

            }
            */
        };
    }
}

#endif
