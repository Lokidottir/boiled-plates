#ifndef BOP_MATRIX_HPP
#define BOP_MATRIX_HPP

#include <memory>
#include <initializer_list>
#include <string>
#include <sstream>

#include "Vector.hpp"


namespace bop {
	namespace maths{
		template<class T>
		class Matrix {
			protected:
				unsigned int width;
				unsigned int height;
			public:
			std::unique_ptr< Vector<T>[] > data;
		
		};
	}
}

#endif