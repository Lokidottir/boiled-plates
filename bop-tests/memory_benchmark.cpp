#include <bop-utility/Benchmark.hpp>
#include <iostream>
#include <vector>
#include <bop-memory/memory.hpp>

#define TEST_COUNT 10000000
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
    std::cout << "(stack size now at " << stack_to_add.size(true) << ")" << std::endl;
    std::cout << "Stack popping:                      " << benchmark(TEST_COUNT, bop_mem_stackpop) << std::endl;
    std::cout << "Vector pushing:                     " << benchmark(TEST_COUNT, bop_mem_vs_vectoradd) << std::endl;
    std::cout << "(vector size now at " << vector_to_add.size() << ")" << std::endl;
    std::cout << "Vector popping:                     " << benchmark(TEST_COUNT, bop_mem_vs_vectorpop) << std::endl;
    std::cout << "Recycled memory array construction: " << benchmark(TEST_COUNT, bop_mem_primarrcont) << std::endl;
    std::cout << "Normal array construction:          " << benchmark(TEST_COUNT, bop_mem_vs_new) << std::endl;
    return 0;
}

int main() {
    #ifdef DIVERSE_RECYCLER
    for (uint_type iter = 1; iter <= 100000; iter++) {
        PrimativeArrayContainer<double>::recycler.reserve(iter,50);
    }
    #endif
    stack_tests();
    return 0;
}
