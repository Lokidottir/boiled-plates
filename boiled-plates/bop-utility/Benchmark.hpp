#ifndef BOP_BENCHMARK_HPP
#define BOP_BENCHMARK_HPP
#include <chrono>

namespace bop {
    namespace util {
        #ifndef BOP_UTIL_DEFAULT_TYPES
        #define BOP_UTIL_DEFAULT_TYPES
        typedef double prec_type;
        typedef uint64_t uint_type;
        #endif
        template<typename ...params>
        void nothing(params&&... args) {
            /*
                Used to gauge function call time.
            */
        }

        template<typename F, typename ...params>
        int benchmark(uint_type test_count, F function, params&&... P) {
            /*
                Returns nanoseconds taken to perform a function given
                the parameters. The time taken to call the function is
                subtracted from the result.
            */
            #ifndef BOP_BENCHMARK_KEEP_CALLTIME
            //Get the time before empty function calling
            auto empty_funct_b = std::chrono::high_resolution_clock::now();
            for (unsigned int i = 0; i < test_count; i++) {
                /*
                    Call the empty function as many times as the function being
                    benchmarked to eliminate function call time as a factor.
                */
                nothing(P...);
            }
            //Get the time after the function calling
            auto empty_funct_a = std::chrono::high_resolution_clock::now();
            #endif
            //Get the time before benchmarked function is called
            auto before = std::chrono::high_resolution_clock::now();
            for (unsigned int i = 0; i < test_count; i++) {
                function(P...);
            }
            //Get the time after
            auto after = std::chrono::high_resolution_clock::now();
            return (std::chrono::duration_cast<std::chrono::nanoseconds>((after - before) - (empty_funct_b - empty_funct_a)).count()/test_count);
        }
    }
}


#endif
