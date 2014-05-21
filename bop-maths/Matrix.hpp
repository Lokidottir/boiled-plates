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
			Matrix(Matrix<T>&& mat) {}
			Matrix() {}
			
			//Destructor
			~Matrix() {}
			
			//Operator overloads
			Vector<T>& operator[] (const unsigned int index) {}
			Matrix<T> operator= (const Matrix<T> &mat) {}
			
			//Arithmetic overloads
			Matrix<T>& operator*= (const T scalar) {}
			Matrix<T>& operator*= (Matrix<T> &mat) {}
			Matrix<T>& operator/= (const T scalar) {}
			Matrix<T>& operator+= (Matrix<T> &mat) {}
			Matrix<T>& operator-= (Matrix<T> &mat) {}
			
			
			//Information functions
			unsigned int w() {}
			unsigned int h() {}
			unsigned int size() {}
			std::string string() {}
			
			
		};
		
		//Matrix related functions
		template<class T>
		T determinant(Matrix<T>& mat, Vector<bool>& allowed_cols, unsigned int& size) {}
		template<class T>
		T determinant(Matrix<T>& mat) {}
		template<class T>
		Matrix<T> identityMatrix(unsigned int size) {}
		template<class T>
		Matrix<T> inverseMatrix() {}
	}
}

#endif