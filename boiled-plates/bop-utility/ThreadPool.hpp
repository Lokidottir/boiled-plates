#ifndef BOP_THREADPOOL_HPP
#define BOP_THREADPOOL_HPP
#include <threads>

#ifndef BOP_DEFAULT_TYPES
#define BOP_DEFAULT_TYPES
typedef double prec_type;
typedef uintmax_t uint_type;
#endif

/*
template<uint_type THREAD_COUNT>
class ThreadPool {
}
*/



template<class T>
class RubbishCollector {
    private:
        bool active;
        
        std::thread thread;
    public:
        RubbishCollector() : active(false),  {

        }

        void activate;
}

#endif
