#ifndef BOP_MULTIDIMENTIONAL_ARRAY_HPP
#define BOP_MULTIDIMENTIONAL_ARRAY_HPP

namespace bop {
	namespace util {
		
		template<class T, size_t L>
		struct IndexHandler {
			size_t dimention;
			size_t passed_down_index;
			IndexHandler<T,L - 1> handler;
			
			IndexHandler() {
				this->dimention = 0;
				this->passed_down_index = 0;
			}
			
			IndexHandler<T, L - 1>& operator[] (uint_type index) {
				this->handler.passed_down_index = this->passed_down_index + (index * this->dimention);
				return this->handler;
			}
		};
		
		template<class T>
		struct IndexHandler<T,1> {
			size_t passed_down_index;
			T* data;
			T& operator[] (uint_type last_index) {
				return data[passed_down_index + last_index];
			}
		};
		
		template<class T>
		struct IndexHandler<T,0>{
			//empty struct
		};
	};
};

#endif