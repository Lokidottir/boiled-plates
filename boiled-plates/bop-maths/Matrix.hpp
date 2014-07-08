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
		template<class T>
		class Matrix {
			protected:
				unsigned int _width;
				unsigned int _height;
				T* data;
				
				inline void setData(unsigned int width, unsigned int height, bool delete_ptr = true) {
					if (delete_ptr) delete[] this->data;
					this->_width = width;
					this->_height = height;
					this->data = new T[width * height];
				}
				
				class MatrixIndexHandler {
					/*
						Internal handler class for translating [row][col]
						to (row * width) + col for row-major order.
					*/
					public:
						unsigned int temp_index;
						Matrix<T>& matrix;
						
						MatrixIndexHandler(Matrix<T>& mat) : matrix(mat)  {
							this->temp_index = 0;
						}
						
						unsigned int pivot() {
							unsigned int col;
							for (col = 0; col < this->matrix.width(); col++) {
								if (this->matrix.element(this->temp_index,col) != 0) break;
							}
							return col;
						}
						
						inline T& operator[](unsigned int index) {
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
				
				Matrix(unsigned int width, unsigned int height, T fill = 0) : Matrix() {
					/*
						Non-square matrix constructor, sets all values as 
						the given fill value.
					*/
					this->setData(width,height,false);
					for (unsigned int elem = 0; elem < this->width() * this->height(); elem++) {
						this->data[elem] = fill;
					}
				}
				
				Matrix(unsigned int size, T fill = 0) : Matrix(size, size, fill)  {
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
					unsigned int row = 0;
					for (auto sublist : list) {
						unsigned int col = 0;
						for (auto element : sublist) {
							this->element(row,col) = element;
							col++;
						}
						row++;
					}
				}
				
				template<class A>
				Matrix(Matrix<A>& mat) : Matrix() {
					/*
						Copy constructor.
					*/
					this->setData(mat.width(),mat.height(),false);
					for (unsigned int elem = 0; elem < this->width() * this->height(); elem++) {
						this->data[elem] = mat.data[elem];
					}
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
				inline MatrixIndexHandler& operator[] (const unsigned int index) {
				#else
				inline MatrixIndexHandler operator[] (const unsigned int index) {
				#endif
				this->handler.temp_index = index;
					return this->handler;
				}
				
				Matrix<T>& operator= (const Matrix<T>& mat) {
					this->setData(mat.width(), mat.height());
					for (unsigned int row = 0; row < this->height(); row++) {
						for (unsigned int col = 0; col < this->width(); col++) {
							this->element(row,col) = mat.element(row,col);
						}
					}
					return *this;
				}
				
				//Boolean logic overloads
				bool operator== (const Matrix<T>& mat) const {
					if (this->width() == mat.width() && this->height() == mat.height()) {
						bool same = true;
						for (unsigned int row = 0; row < this->height() && same; row++) {
							for (unsigned int col = 0; col < this->width() && same; col++) {
								same = this->element(row,col) == mat.element(row,col);
							}
						}
						return same;
					}
					else return false;
				}
				
				bool operator!= (const Matrix<T>& mat) const {
					return !((*this) == (mat));
				}
				
				//Arithmetic overloads
				Matrix<T>& operator*= (const T scalar) {
					/*
						Scalar multiplication member operator, multiplies all
						elements by the given scalar.
					*/
					for (unsigned int elem = 0; elem < this->height() * this->width(); elem++) {
						this->data[elem] *= scalar;
					}
					return *this;
				}
				
				Matrix<T>& operator*= (Matrix<T>& mat) {
					/*
						Matrix multiplication member operator.
					*/
					T* temp = new T[this->height() * mat.width()];
					for (unsigned int row = 0; row < this->height(); row++) {
						for (unsigned int col = 0; col < mat.width(); col++) {
							temp[(row * mat.width()) + col] = 0;
							for (unsigned int iter = 0; iter < mat.width(); iter++) {
								temp[(row * mat.width()) + col] += this->element(row,iter) * mat.element(iter,col);
							}
						}
					}
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
						for (unsigned int i = 0; i < this->height(); i++) {
							T product = 0;
							for (unsigned int dot_index = 0; dot_index < vec.size(); dot_index++) {
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
						for (unsigned int elem = 0; elem < this->height() * this->width(); elem++) {
							this->data[elem] += mat.data[elem];
						}
					}
					else {
						for (unsigned int row = 0; row < this->height(); row++) {
							for (unsigned int col = 0; col < this->width(); col++) {
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
						for (unsigned int elem = 0; elem < this->height() * this->width(); elem++) {
							this->data[elem] -= mat.data[elem];
						}
					}
					else {
						for (unsigned int row = 0; row < this->height(); row++) {
							for (unsigned int col = 0; col < this->width(); col++) {
								this->element(row,col) -= mat.element(row,col);
							}
						}
					}
					return *this;
				}
				
				Matrix<T> operator- () {
					Matrix<T> mat(*this);
					for (unsigned int elem = 0; elem < this->width() * this->height(); elem++) {
						mat.data[elem] *= -1;
					}
					return mat;
				}
				
				//Information functions
				
				inline T& element(unsigned int row, unsigned int col) const {
					return this->data[(row * this->width()) + col];
				}
				
				inline unsigned int width() const {
					/*
						Returns the width of the Matrix.
					*/
					return this->_width;
				}
				
				inline unsigned int height() const {
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
						for (unsigned int row = 0; row < this->height() && identity; row++) {
							for (unsigned int col = 0; col < this->width() && identity; col++) {
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
					for (unsigned int row = 0; row < this->height(); row++) {
						mat_str << '[';
						for (unsigned int col = 0; col < this->width(); col++) {
							mat_str << this->element(row,col);
							if (col + 1 < this->width()) mat_str << ',';
						}
						mat_str << ']';
						if (newlines) mat_str << '\n';
					}
					return mat_str.str();
				}
				
				inline bool valid() const {
					//Matrix validity determined by the pointer's value.
					return (this->data != nullptr);
				}
				
				operator bool() const {
					/*
						Cast to bool as the validity of the matrix. Allowing for "if (Matrix) { ... }"
						behaviour.
					*/
					return this->valid();
				}
				
				std::pair< Matrix<T>, Matrix<T> > LU() {
					std::pair< Matrix<T>,  Matrix<T> > LU_pair(Matrix<T>(*this), Matrix<T>(this->width(), this->height()));
					/*
						Setting up the first element of the pair as the Upper matrix by copying this
						matrix into it, and the second element as the Lower matrix by initialising
						it as an identity matrix.
					*/
					for (unsigned int elem = 0; elem < this->width() && elem < this->height(); elem++) {
						LU_pair.second[elem][elem] = 1;	
					}
					/*
						With the matrices initialised, the Upper matrix to-be needs to be reduced, 
						with the scalar of each row reduction placed in the Lower matrix.
					*/
					for (unsigned int ind_col = 0; ind_col < this->width() && ind_col < this->height(); ind_col++) {
						
					}
					return LU_pair;
				}
				
				//Matrix manipulation functions.
				
				inline void swapRows(unsigned int index_a, unsigned int index_b) {
					/*
						Row swap function, for use in such algorithms as Gauss-Jordan
						elimination.
					*/
					if (index_a != index_b) {
						for (unsigned int col = 0; col < this->width(); col++) {
							std::swap(this->element(index_a,col), this->element(index_b,col));
						}
					}
				}
				
				void impose(Matrix<T>& mat, unsigned int row_off = 0, unsigned int col_off = 0) {
					for (unsigned int row = 0; row < mat.height() && row + row_off < this->height(); row++) {
						for (unsigned int col = 0; col < mat.width() && col + col_off < this->width(); col++) {
							this->element(row + row_off, col + col_off) = mat.element(row,col);
						}
					}
				}
				
				void impose(Vector<T>& vec, unsigned int row_off = 0, unsigned int col_off = 0) {
					for (unsigned int col = 0; col < vec.size() && col + col_off < this->width(); col++) {
						this->element(row_off, col + col_off) = vec[col];
					}
				}
		};
		
		typedef Matrix<BOP_MATHS_DEFAULT_TYPE> matrix;
		
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
		
		template <class T>
		std::ostream& operator<< (std::ostream& stream, Matrix<T>& mat) {
			stream << mat.string();
			return stream;
		}
	}
}

#endif
