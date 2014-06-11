#ifndef BOP_MATHS_HPP
#define BOP_MATHS_HPP

#include <iostream>
#include <cmath>

namespace bop {
	namespace maths {
		constexpr double pi() { return std::atan(1)*4; } 
	}
}

#include "Vector.hpp"
#include "Matrix.hpp"

#endif