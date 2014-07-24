#ifndef BOP_MULTIDIMENSIONAL_ARRAY_HPP
#define BOP_MULTIDIMENSIONAL_ARRAY_HPP

/*
	Class defining a multidimensional array of a template type 
	represented by a one-dimensional array with the index calculations
	passed down recursively to the base-case class that holds the data 
	in an array	describing the multidimensional array in terms of 
	row-major order for n dimensions.
*/

namespace bop {
	namespace util {
		#ifndef BOP_UTIL_DEFAULT_TYPES
		#define BOP_UTIL_DEFAULT_TYPES
		typedef double prec_type;
		typedef uint64_t uint_type;
		#endif
		template<class T, size_t L>
		struct IndexHandler {
			size_t dimension; // holds the size of this instance of the array
			size_t passed_down_val; // holds the values passed down by higher IndexHandlers
			IndexHandler<T,L - 1> handler; // the next-smallest dimension handler.
			
			IndexHandler() {
				/*
					Empty constructor, initializes unsigned values as 0
				*/
				this->dimension = 0;
				this->passed_down_val = 0;
			}
			
			template<typename param1, typename ...params>
			IndexHandler(param1 P1, params&&... PN) : IndexHandler() {
				/*
					Constructor takes a variable number of unsigned values and
					passes it to the resize method.
				*/
				this->resize(P1, PN...);
			}
			
			IndexHandler<T, L - 1>& operator[] (uint_type index) {
				this->handler.passed_down_val = this->passed_down_val + (index * this->dimension);
				return this->handler;
			}
			
			size_t size() {
				/*
					returns the size of this level of the multidimensional array.
				*/
				return this->dimension;
			}
			
			size_t fullSize() {
				/*
					returns the size of the entire array, regardless of the level.
				*/
				return this->handler.fullSize();
			}
			
			template<typename param1, typename ...params>
			void resize(param1 P1, params&&... PN) {
				/*
					takes a variable number of arguments interpreted as unsigned and
					passes each down to the corresponding class layer finally creating
					an array at the base class the size of the product of the given 
					parameters.
				*/
				this->dimension = static_cast<size_t>(P1);
				this->handler.passed_down_val = static_cast<size_t>(P1);
				this->handler.resize(true, PN...);
			}
			
			T* ptr() {
				/*
					returns the data pointer held at the base class.
				*/
				return this->handler.getPtr();
			}
			
			private:
			
			template<typename param1, typename ...params>
			void resize(bool _not_base_, param1 P1, params&&... PN) {
				/*
					Secondary resize function, first argument is simply to overload
					to this function indicating this isn't the function called initially.
				*/
				this->dimension = static_cast<size_t>(P1);
				this->handler.passed_down_val = this->passed_down_val * static_cast<size_t>(P1);
				this->handler.resize(_not_base_, PN...);
			}
			
		};
		
		template<class T>
		struct IndexHandler<T,1> {
			size_t passed_down_val;
			size_t dimension;
			size_t total_size;
			T* data;
			IndexHandler() {
				this->passed_down_val = 1;
				this->dimension = 0;
				this->total_size = 0;
				this->data = nullptr;
			}
			
			template<typename param1, typename ...params>
			IndexHandler(param1 P1, params&&... PN) : IndexHandler() {
				this->total_size = static_cast<size_t>(P1) * this->passed_down_val;
				this->data = new T[this->total_size];
				this->dimension = static_cast<size_t>(P1);
			}
			
			~IndexHandler() {
				delete[] this->data;
			}
			
			T& operator[] (uint_type last_index) {
				return data[passed_down_val + last_index];
			}
			
			size_t size() {
				return this->dimension;
			}
			
			size_t fullSize() {
				return this->total_size;
			}
			
			T* ptr() {
				return this->data;
			}
			
			template<typename param1, typename ...params>
			void resize(bool _not_base_, param1 P1, params&&... PN) {
				if (this->total_size < (this->dimension * static_cast<size_t>(P1))) {
					delete[] this->data;
					this->data = new T[this->dimension * static_cast<size_t>(P1)];
				}
				this->dimension = static_cast<size_t>(P1);
				this->total_size = this->dimension * this->passed_down_val;
			}
			
			template<typename param1, typename ...params>
			void resize(param1 P1, params&&... PN) {
				if (this->total_size < (static_cast<size_t>(P1))) {
					delete[] this->data;
					this->data = new T[static_cast<size_t>(P1)];
				}
				this->dimension = static_cast<size_t>(P1);
				this->total_size = this->dimension * this->passed_down_val;
			}
		};
		
		template<class T>
		struct IndexHandler<T,0>{
			//empty struct
		};
	};
};

#endif