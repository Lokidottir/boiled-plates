#ifndef BOP_MATRIX_HPP
#define BOP_MATRIX_HPP

#include <memory>
#include <initializer_list>
#include <string>
#include <sstream>
#include <new>
#include "Vector.hpp"

/*
	bop::maths::Matrix class file
*/

namespace bop {
	namespace maths{
		template<class T>
		class Matrix {
			protected:
				unsigned int width;
				unsigned int height;
			public:
				Vector<T>* data;
				
				//Constructors
				Matrix(unsigned int size, T fill = 0) {
					/*
						Square matrix constructor, sets all values as
						the given fill value.
					*/
					this->data = new Vector<T>[size];
					for (int i = 0; i < size; i++) {
						this->data[i] = Vector<T>(size, fill);
					}
					this->width = size;
					this->height = size;
				}
				
				Matrix(unsigned int width, unsigned int height, T fill = 0) {
					/*
						Non-square matrix constructor, sets all values as 
						the given fill value.
					*/
					this->data = new Vector<T>[height];
					for (int i = 0; i < height; i++) {
						this->data[i] = Vector<T>(width, fill);
					}
					this->height = height;
					this->width = width;
				}
				
				Matrix(std::initializer_list< std::initializer_list<T> > list) {
					/*
						Creates a matrix from a 2-dimensional initializer_list.
					*/
					this->height = list.size();
					this->data = new Vector<T>[this->height];
					unsigned int min_w = list.begin()->size();
					int i = 0;
					for (auto elem : list) {
						if (elem.size() < min_w) min_w = elem.size();
						this->data[i] = elem;
						i++;
					}
					this->width = min_w;
				}
				
				Matrix(Matrix<T>& mat) {
					/*
						Copy constructor.
					*/
					this->width = mat.width;
					this->height = mat.height;
					this->data = new Vector<T>[this->height];
					for (unsigned int i = 0; i < this->width; i++) {
						this->data[i] = mat.data[i];
					}
				}
				
				Matrix(Matrix<T>&& mat) {
					/*
						Move constructor.
					*/
					this->width = mat.width;
					this->height = mat.height;
					this->data = mat.data;
				}
				
				Matrix() {
					/*
						Empty constructor.
					*/
				}
				
				//Destructor
				~Matrix() {
					delete[] this->data;
				}
				
				//Operator overloads
				inline Vector<T>& operator[] (const unsigned int index) {}
				Matrix<T>& operator= (const Matrix<T> &mat) {}
				
				//Arithmetic overloads
				Matrix<T>& operator*= (const T scalar) {
					/*
						Scalar multiplication member operator, multiplies all
						elements by the given scalar.
					*/
					for (int i = 0; i < this->height; i++) {
						this->data[i] *= scalar;
					}
					return *this;
				}
				
				Matrix<T>& operator*= (Matrix<T> &mat) {
					/*
						Matrix multiplication member operator.
					*/
					Vector<T>* temp = new Vector<T>[this->height];
					for (int i = 0; i < this->height; i++) {
						temp[i] = Vector<T>(mat.width);
					}
					for (int r = 0; r < this->height; r++) {
						for (int c = 0; c < mat.width; c++) {
							T product = 0;
							for (int i = 0; i < mat.width; i++) {
								product += (this->data[r][i] * mat.data[i][c]);
							}
							temp[r][c] = product;
						}
					}
					delete[] this->data;
					this->data = temp;
					this->width = mat.width;
					return *this;
				}
				
				Matrix<T>& operator/= (const T scalar) {
					/*
						Scalar division member operator, divides all elements
						by the given scalar.
					*/
					for (int i = 0; i < this->height; i++) {
						this->data[i] /= scalar;
					}
					return *this;
				}
				
				Matrix<T>& operator+= (Matrix<T> &mat) {
					/*
						Addition member operator, adds all the elements of a
						given matrix to the matrix.
					*/	
					for (int i = 0; i < this->height; i++) {
						this->data[i] += mat[i];
					}
					return *this;
				}
				
				Matrix<T>& operator-= (Matrix<T> &mat) {
					/*
						Subtraction member operator, subtracts the value of
						each element in a given matrix from the matrix.
					*/
					for (int i = 0; i < this->height; i++) {
						this->data[i] -= mat[i];
					}
					return *this;
				}
				
				//Information functions
				inline unsigned int w() {
					/*
						Returns the width of the Matrix.
					*/
					return this->width;
				}
				
				inline unsigned int h() {
					/*
						Returns the height of the Matrix.
					*/
					return this->height;
				}
				
				inline bool square() {
					/*
						Determines if the Matrix is a square Matrix.
					*/
					return (this->width == this->height);
				}
				
				std::string string(bool newlines = true) {
					/*
						Similar to the maths::Vector::string function, returns 
						a human-readable representation of the Matrix.
					*/
					std::ostringstream mat_str;
					for (int i = 0; i < this->height; i++) {
						mat_str << '[' << this->data[i].string(false) << ']';
						if (newlines) mat_str << '\n';
					}
					return mat_str.str();
				}
			
		};
		
		//External arithmetic overloads
		
		template<class T>
		Matrix<T> operator* (Matrix<T> mat, const T scalar) {}
		template<class T>
		Matrix<T> operator* (Matrix<T> mat1, Matrix<T>& mat2) {}
		template<class T>
		Matrix<T> operator/ (Matrix<T> mat, const T scalar) {}
		template<class T>
		Matrix<T> operator+ (Matrix<T> mat1, Matrix<T>& mat2) {}
		template<class T>
		Matrix<T> operator- (Matrix<T> mat1, Matrix<T>& mat2) {}
		
		//Matrix related functions
		template<class T>
		T determinant(Matrix<T>& mat, Vector<bool>& allowed_cols, unsigned int& size) {}
		template<class T>
		T determinant(Matrix<T>& mat) {}
		template<class T>
		Matrix<T> identityMatrix(unsigned int size) {}
		template<class T>
		Matrix<T> inverseMatrix(Matrix<T> &mat) {}
		template<class T>
		Matrix<T> transposeMatrix(Matrix<T> &mat) {}
	}
}

#endif