#ifndef BOP_MATHS_EXTRA_HPP
#define BOP_MATHS_EXTRA_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include "../bop-defaults/types.hpp"

namespace bop {
    #ifdef BOP_USE_INFERIOR_NAMESPACE
    namespace math {
    #else
    namespace maths {
    #endif

        const double pi = std::atan(1)*4;

        size_t minBits(const size_t num) {
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

        /*
        const std::vector<uint_type>& primes_until(uint_type prime_limit) {
            std::map<uint_type,std::vector<>>
        }
        */

        std::vector<uint_type> prime_sieve(uint_type prime_limit) {
            std::vector<bool> sieve_list(prime_limit);
            std::vector<uint_type> prime_list;
            for (uint_type iter = 0; iter < sieve_list.size(); iter++) sieve_list[iter] = (2 < iter);
            for (uint_type iter_sieve = 0; iter_sieve < sieve_list.size(); iter_sieve++) {
                if (sieve_list[iter_sieve]) {
                    for (uint_type iter_remove = iter_sieve * 2; iter_remove < sieve_list.size(); iter_remove += iter_sieve) sieve_list[iter_remove] = false;
                    prime_list.push_back(iter_sieve);
                }
            }
            return prime_list;
        }

        uint_type nth_prime(uint_type n) {
            auto list = prime_sieve(n * n);
            return list[n - 1];
        }
    }
}

#endif
