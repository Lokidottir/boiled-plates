#ifndef BOP_SORTING_HPP
#define BOP_SORTING_HPP

namespace bop {
    namespace util {
        template<class Container, class T>
        void quicksort(Container<T>& container) {
            quicksort(container,0,container.size() - 1);
        }

        template<class Container, class T>
        void quicksort(Container<T>& container, uint_type low, uint_type high) {
            static auto calc_pivot = [low,high]() -> uint_type {
                return ((high - low)/2) + low
            };
            std::swap(container[low],container[()]);
        }
    }
}

#endif
