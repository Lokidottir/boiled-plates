#ifndef BOP_MATHS_HPP
#define BOP_MATHS_HPP

#include <iostream>
#include <cmath>

namespace bop {
	namespace maths {
		
		constexpr double pi() {
			return std::atan(1)*4;
		}
		
		unsigned int factorial(unsigned int n) {
			unsigned int fact = n;
			for (n = n - 1; n > 1; n--) {
				fact *= n;
			}
			return fact;
		}
		#ifndef BOP_MATHS_DEFAULT_TYPES
		#define BOP_MATHS_DEFAULT_TYPES
		typedef double prec_type;
		typedef uint64_t uint_type;
		#endif
	}
}

#include "Vector.hpp"
#include "Matrix.hpp"
#include "MatrixExt.hpp"

#endif
