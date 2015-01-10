#include <bop-utility/Benchmark.hpp>
#include <iostream>
#include <vector>
#include <bop-memory/memory.hpp>

#define TEST_COUNT 1000000
#define ALLOC_SIZE 9
#define DIVERSE_RECYCLER

using namespace bop::util;
using namespace bop::mem;

bop::top::Stack<uint_type> stack_to_add;
std::vector<uint_type> vector_to_add;

void bop_mem_stackadd() {
    stack_to_add.push(0);
}

void bop_mem_stackpop() {
    stack_to_add.pop();
}

void bop_mem_vs_vectoradd() {
    vector_to_add.push_back(0);
}

void bop_mem_vs_vectorpop() {
    vector_to_add.pop_back();
}

void bop_mem_primarrcont() {
    PrimativeArrayContainer<double> pri_array(ALLOC_SIZE);
}

void bop_mem_vs_new() {
    delete[] (new double[ALLOC_SIZE]);
}

int stack_tests() {
    std::cout << "Stack pushing:                      " << benchmark(TEST_COUNT, bop_mem_stackadd) << std::endl;
    std::cout << "Stack popping:                      " << benchmark(TEST_COUNT, bop_mem_stackpop) << std::endl;
    std::cout << "std::vector pushing:                " << benchmark(TEST_COUNT, bop_mem_vs_vectoradd) << std::endl;
    std::cout << "std::vector popping:                " << benchmark(TEST_COUNT, bop_mem_vs_vectorpop) << std::endl;
    std::cout << "Recycled memory array construction: " << benchmark(TEST_COUNT, bop_mem_primarrcont) << std::endl;
    #ifdef DIVERSE_RECYCLER
    for (uint_type iter = 1; iter <= 1000; iter++) {
        PrimativeArrayContainer<double>::recycler.reserve(iter,50);
    }
    std::cout << "Recycled memory with diverse set:   " << benchmark(TEST_COUNT, bop_mem_primarrcont) << " @ set diversity " << PrimativeArrayContainer<double>::recycler.size() << std::endl;
    #endif
    std::cout << "Normal array construction:          " << benchmark(TEST_COUNT, bop_mem_vs_new) << std::endl;
    return 0;
}

int main() {
    stack_tests();
    return 0;
}
