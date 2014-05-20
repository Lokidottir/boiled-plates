#ifndef BOP_VECTOR_HPP
#define BOP_VECTOR_HPP

#include <initializer_list>
#include <string>

/*
	bop::maths::Vector forward declaration file
*/

namespace bop::maths {

	template<class T>
	class Vector {
		protected:
			int width;
			int pivot_index;
		public:
			T* data;
			//Constructors
			Vector(int, T);
			Vector(int, int, T);
			Vector(std::initializer_list<T>);
			Vector();
			//Operator Overloads
			T& operator[] (const unsigned int);
			Vector<T>& operator= (const Vector<T>&);
			Vector<T>& operator= (std::initializer_list<T>);
			//Arithmetic Operator Overloads
			Vector<T>& operator*= (const T);
			Vector<T>& operator/= (const T);
			Vector<T>& operator+= (Vector<T>&);
			Vector<T>& operator-= (Vector<T>&);
			//Vector information functions
			int pivot(int);
			unsigned int size();
			std::string string();
			T& x();
			T& y();
			T& z();
	};

}

#endif