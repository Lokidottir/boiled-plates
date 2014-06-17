#include <iostream>
#define BOP_MATRIX_MULTIPLY_DISCARD_TINY
#include <bop-maths/maths.hpp>
#include "testlibs/funct_bench/funct_benchmark.hpp"
#define TEST_COUNT 10000000

using namespace bop::maths;

void bop_bench_construct() {
	Matrix<double> mat1(3);
}


void bop_bench_multiply() {
	static Matrix<double> mat1 = identityMatrix<double>(3);
	static Matrix<double> mat2 = {{2,3,4},{6,1,7},{3,4,5}};
	mat2 *= mat1;
}

int mat_tests() {
	std::cout << "Matrix construction: " << gist::benchmark(TEST_COUNT, bop_bench_construct) << std::endl;
	std::cout << "Matrix multiplication: " << gist::benchmark(TEST_COUNT, bop_bench_multiply) << std::endl;
	return 0;
}

int main() {
	mat_tests();
	return 0;
}
