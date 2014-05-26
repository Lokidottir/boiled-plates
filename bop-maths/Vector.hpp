#ifndef BOP_VECTOR_HPP
#define BOP_VECTOR_HPP

#include <memory>
#include <initializer_list>
#include <string>
#include <sstream>
#include <new>

/*
	bop::maths::Vector Class file
*/

namespace bop {
	namespace maths {
		template<class T>
		class Vector {
			protected:
				unsigned int width;
				int pivot_index;
			public:
				T* data;
				//Constructors
				Vector(int width, T fill = 0) {
					/*
						Creates a new vector of a given width, setting each
						element as a given default.
						----pivot index is set as -1.
					*/
					this->pivot_index = -1;
					this->data = new T[width];
					this->width = width;
					for (unsigned int i = 0; i < this->width; i++) {
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
					for (unsigned int i = 0; i < this->width; i++) {
						this->data[i] = vec.data[i];
					}
				}
				
				Vector(Vector<T>&& vec) {
					/*
						Move constructor.
					*/
					this->width = vec.width;
					this->pivot_index = vec.pivot_index;
					this->data = vec.data;
				}
				
				Vector() {
					/*
						Empty constructor.
					*/
				}
				
				//Destructor
				
				~Vector() {
					//Manual deletion of data array.
					delete[] this->data;
				}
				
				//Operator Overloads
				
				inline T& operator[] (const unsigned int index) const {
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
					this->width = vec.width;
					this->pivot_index = vec.pivot_index;
					this->data = new T[this->width];
					for (unsigned int i = 0; i < this->width; i++) {
						this->data[i] = vec.data[i];
					}
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
				
				//Boolean Logic overloads
				
				bool operator== (const Vector<T>& vec) {
					if (this->width == vec.width) {
						/*
							Not the most elegant solution, then again for testing
							an array there isn't much choice.
						*/
						bool same = true;
						for (unsigned int i = 0; i < this->width && same; i++) {
							same = (this->data[i] == vec.data[i]);
						}
						return same;
					}
					else return false;
				}
				
				bool operator!= (const Vector<T>& vec) {
					return !(this->operator==(vec));
				}
				
				//Arithmetic Operator Overloads
				
				Vector<T>& operator*= (const T scalar) {
					/*
						Member multiplication operator, multiplies all
						elements in a vector by the given scalar.
					*/
					if (scalar == 0) return *this;
					for (unsigned int i = 0; i < this->width; i++) {
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
					for (unsigned int i = 0; i < this->width; i++) {
						this->data[i] /= scalar;
					}
					return *this;
				}
				
				Vector<T>& operator+= (Vector<T>& vec) {
					/*
						Member addition operator, adds the elements of
						a given vector to the corresponding vector to the
						corresponding elements of a subject vector.
						----sets the pivot as needing to be recalculated.
					*/
					for (unsigned int i = 0; i < this->width; i++) {
						this->data[i] += vec[i];
					}
					this->pivot_index = -1;
					return *this;
				}
				
				Vector<T>& operator-= (Vector<T>& vec) {
					/*
						Member subtraction operator, subtracts the elements
						of a given vector from the corresponding elements of 
						a subject vector.
						----sets the pivot as needing to be recalculated.
					*/
					for (unsigned int i = 0; i < this->width; i++) {
						this->data[i] -= vec[i];
					}
					this->pivot_index = -1;
					return *this;
				}
				
				//Vector information functions
				
				int pivot(bool recalc = false) {
					/*
						Returns the position of the first non-zero element in
						the vector.
					*/
					if (recalc || this->pivot_index < 0) {
						this->pivot_index = -1;
						for (unsigned int i = 0; i < this->width && this->pivot_index < 0; i++) {
							if (this->data[i] != 0) this->pivot_index = i;
						}
					}
					return this->pivot_index;
				}
				
				const unsigned int size() const {
					/*
						Returns the width of the vector.
					*/
					return this->width;
				}
				
				std::string string(bool brackets = true) {
					/*
						Returns a string that represents the data in the Vector
						in a human-readable format.
					*/
					std::ostringstream vec_str;
					if (brackets) vec_str << "(";
					for (unsigned int i = 0; i < this->width; i++) {
						vec_str << this->data[i];
						if (i + 1 < this->width) vec_str << ',';
					}
					if (brackets) vec_str << ")";
					return vec_str.str();
				}
				
				/*
					Coordinate functions, synonymous with Vector::operator[](0/1/2/3).
					Checks that those indices exist before returning.
				*/
				
				inline T& x() {
					if (this->width > 0) return this->data[0];
					else return 0;
				}
				
				inline T& y() {
					if (this->width > 1) return this->data[1];
					else return 0;
				}
				
				inline T& z() {
					if (this->width > 2) return this->data[2];
					else return 0;
				}
				
				inline T& w() {
					if (this->width > 3) return this->data[3];
					else return 0;
				}
				
				/*
					Member swap function, intended for use in algorithms such
					as Gauss-Jordan elimination, or whatever else may be in need
					of the swapping of Vectors.
				*/
				
				void swap(Vector<T>& vec) {
					T* temp_arr = vec.data;
					unsigned int temp_w = vec.width;
					int temp_pivot = vec.pivot_index;
					vec.data = this->data;
					vec.width = this->width;
					vec.pivot_index = this->pivot_index;
					this->data = temp_arr;
					this->width = temp_w;
					this->pivot_index = temp_pivot;
				}
				
		};
		
		/*
			External arithmetic operators, return a vector separate from
			the given parameters.
		*/
		
		template<class T>
		Vector<T> operator* (Vector<T> vec, const T scalar) {
			vec *= scalar;
			return vec;
		}
		
		template<class T>
		Vector<T> operator/ (Vector<T> vec, const T scalar) {
			vec /= scalar;
			return vec;
		}
		
		template<class T>
		Vector<T> operator+ (Vector<T> vec1, Vector<T>& vec2) {
			vec1 += vec2;
			return vec1;
		}
		
		template<class T>
		Vector<T> operator- (Vector<T> vec1, Vector<T>& vec2) {
			vec1 -= vec2;
			return vec1;
		}
	}
}

#endif