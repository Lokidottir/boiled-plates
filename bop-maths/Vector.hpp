#ifndef BOP_VECTOR_HPP
#define BOP_VECTOR_HPP

#include <initializer_list>
#include <string>

/*
	bop::maths::Vector Class file
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
			Vector(int width, T fill = 0) {
				/*
					Creates a new vector of a given width, setting each element as a given default.
					----pivot index is set as -1.
				*/
				this->pivot_index = -1;
				this->data = new T[width];
				this->width = width;
				for (int i = 0; i < this->width; i++) {
					this->data[i] = fill;
				}
			}
			
			Vector(std::initializer_list<T> list) {
				/*
					Creates a new Vector from an initializer list.
					----pivot_index is determined during construction.
					----width is determined from size of list.
				*/
				this->width = list.size();
				this->pivot_index = -1;
				this->data = new T[this->width];
				int i = 0;
				for (T elem : list) {
					this->data[i] = elem;
					if (this->pivot_index < 0 && elem != 0) {
						this->pivot_index = i;
					}
					i++;
				}
			}
			
			Vector(const Vector<T>& vec) {
				/*
					Creates a new Vector from a Vector, a copy constructor.
					----copies all data from the Vector, incl. deep copy of data array pointer.
				*/
				this->width = vec.width;
				this->pivot_index = vec.pivot_index;
				this->data = new T[this->width];
				for (int i = 0; i < this->width; i++) {
					this->data[i] = vec.data[i];
				}
			}
			
			Vector() {
				/*
					Empty constructor, does nothing.
				*/
			}
			
			/*
				Todo: Vector(Vector&& vec) move constructor
			*/
			
			//Operator Overloads
			
			T& operator[] (const unsigned int index) {
				/*
					Index operator, returns the data at the given index.
					----synonymous to Vector::data[const unsigned int]
				*/
				return this->data[index];
			}
			
			Vector<T>& operator= (const Vector<T>& vec) : Vector(vec) {
				/*
					Copy assignment, calls copy constructor.
					----synonymous to the copy constructor
				*/
				return *this;
			}
			
			Vector<T>& operator= (std::initializer_list<T> list) {
				/*
					Initializer list assignment operator, synonymous
					with the constructor that takes the same argument.
					----copied here so the list is not passed by value
				*/
				this->width = list.size();
				this->pivot_index = -1;
				this->data = new T[this->width];
				int i = 0;
				for (T elem : list) {
					this->data[i] = elem;
					if (this->pivot_index < 0 && elem != 0) {
						this->pivot_index = i;
					}
					i++;
				}
				return *this;
			}
			
			//Arithmetic Operator Overloads
			
			Vector<T>& operator*= (const T scalar) {
				/*
					Member multiplication operator, multiplies all
					elements in a vector by the given scalar.
				*/
				if (scalar == 0) return *this;
				for (int i = 0; i < this->width; i++) {
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
				for (int i = 0; i < this->width; i++) {
					this->data[i] /= scalar;
				}
				return *this;
			}
			
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
	
	template<class T>
	Vector<T> operator* (Vector<T>, const T);
	template<class T>
	Vector<T> operator/ (Vector<T>, const T);
	template<class T>
	Vector<T> operator+ (Vector<T>, Vector<T>&);
	template<class T>
	Vector<T> operator- (Vector<T>, Vector<T>&);

}

#endif