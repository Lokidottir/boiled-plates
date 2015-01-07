#ifndef BOP_MATHS_EXTRA_HPP
#define BOP_MATHS_EXTRA_HPP

#include <iostream>
#include <cmath>
#include "../bop-defaults/types.hpp"

namespace bop {
    #ifdef BOP_USE_INFERIOR_NAMESPACE
    namespace math {
    #else
    namespace maths {
    #endif

        const double pi = std::atan(1)*4;

        size_t minBits (const size_t num) {
            return static_cast<size_t>(log2(num));
        }

        unsigned int factorial(unsigned int n) {
            unsigned int fact = n;
            for (n = n - 1; n > 1; n--) {
                fact *= n;
            }
            return fact;
        }

        template<class T>
        inline T log_n(T base, T val) {
            return (log(val)/log(base));
        }
    }
}

#endif
