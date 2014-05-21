#ifndef BOP_MATRIX_HPP
#define BOP_MATRIX_HPP

#include <memory>
#include <initializer_list>
#include <string>
#include <sstream>
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
				Matrix(unsigned int size, T fill = 0) {}
				Matrix(unsigned int width, unsigned int height, T fill = 0) {}
				Matrix(std::initializer_list< std::initializer_list<T> > list) {}
				Matrix(Matrix<T>& mat) {}
				Matrix(Matrix<T>&& mat) {}
				Matrix() {}
				
				//Destructor
				~Matrix() {}
				
				//Operator overloads
				inline Vector<T>& operator[] (const unsigned int index) {}
				Matrix<T>& operator= (const Matrix<T> &mat) {}
				
				//Arithmetic overloads
				Matrix<T>& operator*= (const T scalar) {}
				Matrix<T>& operator*= (Matrix<T> &mat) {}
				Matrix<T>& operator/= (const T scalar) {}
				Matrix<T>& operator+= (Matrix<T> &mat) {}
				Matrix<T>& operator-= (Matrix<T> &mat) {}
				
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