#include <bop-utility/Benchmark.hpp>
#include <iostream>
#include <bop-memory/memory.hpp>

#define TEST_COUNT 100000

using namespace bop::util;
using namespace bop::mem;

bop::top::Stack<uint_type> stack_to_add;

void bop_mem_stackadd() {
    stack_to_add.push(0);
}

void bop_mem_stackpop() {
    stack_to_add.pop();
}

int stack_tests() {
    std::cout << "Stack pushing: " << benchmark(TEST_COUNT, bop_mem_stackadd);
    std::cout << "Stack popping: " << benchmark(TEST_COUNT, bop_mem_stackpop);
    return 0;
}

int main() {
    stack_to_add = bop::top::Stack<uint_type>();
    stack_tests();
    return 0;
}
