#ifndef BOP_MATRIX_HPP
#define BOP_MATRIX_HPP
#ifndef BOP_MATRIX_DISCARD_BY
#define BOP_MATRIX_DISCARD_BY 0xFFFF
#endif
#include <initializer_list>
#include <string>
#include <sstream>
#include <cmath>
#include "Vector.hpp"

/*
	bop::maths::Matrix class file
*/

namespace bop {
	namespace maths{
		#ifndef BOP_MATHS_DEFAULT_TYPES
		#define BOP_MATHS_DEFAULT_TYPES
		typedef double prec_type;
		typedef uint64_t uint_type;
		#endif
		template<class T>
		class Matrix {
			protected:
				uint_type _width;
				uint_type _height;
				T* data;
				
				inline void setData(uint_type width, uint_type height, bool delete_ptr = true) {
					if (this->data == nullptr || (delete_ptr && (width * height) > (this->width() * this->height()))) {
						delete[] this->data;
						this->data = new T[width * height];
					}
					this->_width = width;
					this->_height = height;
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
				
				Matrix(uint_type size, T fill = 0) : Matrix(size, size, fill)  {
					/*
						Square matrix constructor, calls Matrix(uint, uint, T).
					*/
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
				
				Matrix(Matrix<T>& mat) : Matrix() {
					/*
						Copy constructor.
					*/
					this->setData(mat.width(),mat.height(),false);
					memcpy(this->data, mat.data, mat.width() * mat.height() * sizeof(T));
				}
				
				template<class A>
				Matrix(const Matrix<A> mat_tocast) : Matrix() {
					this->setData(mat_tocast.width(), mat_tocast.height(), false);
					for (uint_type elem = 0; elem < mat_tocast.width() * mat_tocast.height(); elem++) this->element(0,elem) = static_cast<T>(mat_tocast.element(0,elem));
				}
				
				Matrix(Matrix<T>&& mat) : Matrix() {
					/*
						Move constructor.
					*/
					this->_width = mat._width;
					this->_height = mat._height;
					this->data = mat.data;
					mat.data = nullptr;
				}
				
				//Destructor
				~Matrix() {
					delete[] this->data;
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
				
				Matrix<T>& operator= (const Matrix<T>& mat) {
					this->setData(mat.width(), mat.height());
					memcpy(this->data, mat.data, this->width() * this->height() * sizeof(T));
					return *this;
				}
				
				//Boolean logic overloads
				bool operator== (const Matrix<T>& mat) const {
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
					T* temp = new T[this->height() * mat.width()];
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
					delete[] this->data;
					this->data = temp;
					this->_width = mat._width;
					return *this;
				}
				
				Vector<T>& operator*= (Vector<T>& vec) {
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
				
				Matrix<T>& operator+= (Matrix<T> &mat) {
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
				
				Matrix<T>& operator-= (Matrix<T> &mat) {
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
					return this->_width;
				}
				
				inline uint_type height() const {
					/*
						Returns the height of the Matrix.
					*/
					return this->_height;
				}
				
				inline bool square() const {
					/*
						Tests if the Matrix is a square Matrix.
					*/
					return (this->width() == this->height());
				}
				
				bool identity() const {
					/*
						Tests if the matrix is an identity matrix.
					*/
					if (this->width() == this->height()) {
						bool identity = true;
						for (uint_type row = 0; row < this->height() && identity; row++) {
							for (uint_type col = 0; col < this->width() && identity; col++) {
								if (row == col) identity = (this->element(row,col) == 1);
								else identity = (this->element(row,col) == 0);
							}
						}
						return identity;
					}
					else return false;
				}
				
				std::string string(bool newlines = true) const {
					/*
						Similar to the maths::Vector::string function, returns 
						a human-readable representation of the Matrix.
					*/
					std::ostringstream mat_str;
					for (uint_type row = 0; row < this->height(); row++) {
						mat_str << '[';
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
				
				struct LU {
					Matrix<T> lower;
					Matrix<T> upper;
				};
				
				LU decompose() {
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
							else {
								return LU_pair;
							}
						}
					}
					return LU_pair;
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
		
		typedef Matrix<prec_type> matrix;
		
		//External arithmetic overloads
		
		template<class T, class A>
		Matrix<T> operator* (Matrix<T> &mat, const A scalar) {
			Matrix<T> mat_p(mat);
			mat_p *= scalar;
			return mat_p;
		}
		
		template<class T>
		Matrix<T> operator* (Matrix<T> &mat1, Matrix<T>& mat2) {
			Matrix<T> mat_p(mat1);
			mat_p *= mat2;
			return mat_p;
		}
		
		template<class T>
		Vector<T> operator* (Matrix<T>& mat, Vector<T>& vec) {
			Vector<T> vec_p(vec);
			mat *= vec_p;
			return vec_p;
		}
		
		template<class T, class A>
		Matrix<T> operator/ (Matrix<T> &mat, const A scalar) {
			Matrix<T> mat_p(mat);
			mat_p /= scalar;
			return mat_p;
		}
		
		template<class T>
		Matrix<T> operator+ (Matrix<T> &mat1, Matrix<T>& mat2) {
			Matrix<T> mat_sum(mat1);
			mat_sum += mat2;
			return mat_sum;
		}
		
		template<class T>
		Matrix<T> operator- (Matrix<T> &mat1, Matrix<T>& mat2) {
			Matrix<T> mat_sum(mat1);
			mat_sum -= mat2;
			return mat_sum;
		}
		
		template<class T_>
		std::ostream& operator<< (std::ostream& stream, const Matrix<T_>& mat) {
			stream << mat.string();
			return stream;
		}
	}
}

#endif
