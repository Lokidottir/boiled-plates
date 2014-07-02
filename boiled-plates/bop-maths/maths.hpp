#ifndef BOP_MATHS_HPP
#define BOP_MATHS_HPP


#include <iostream>
#include <cmath>

namespace bop {
	namespace maths {
		constexpr double pi() { return std::atan(1)*4; }
	}
}

#ifndef BOP_MATHS_DEFAULT_TYPE
#define BOP_MATHS_DEFAULT_TYPE double
#endif

#include "Vector.hpp"
#include "Matrix.hpp"
#include "MatrixExt.hpp"

#endif