#include <iostream>
#define BOP_MATRIX_MULTIPLY_DISCARD_TINY
#include <bop-maths/maths.hpp>
#include "testlibs/funct_bench/funct_benchmark.hpp"
#define TEST_COUNT 1000000

using namespace bop::maths;

void bop_bench_construct() {
	Matrix<double> mat1(3);
}

void bop_bench_det() {
	static Matrix<double> mat1 = identityMatrix<double>(3);
	det(mat1);
}

void bop_bench_multiply() {
	static Matrix<double> mat1 = identityMatrix<double>(3);
	static Matrix<double> mat2 = {{2,3,4},{6,1,7},{3,4,5}};
	mat2 *= mat1;
}

void bop_bench_inverse_2x2() {
	static Matrix<double> mat = {{2,4},{1,7}};
	inverseMatrix(mat);
}

void bop_bench_inverse_unit() {
	static Matrix<double> mat = identityMatrix<double>(3);
	inverseMatrix(mat);
}

void bop_bench_inverse() {
	static Matrix<double> mat = {{3,2,4},{2,7,2},{-1,2,5}};
	inverseMatrix(mat);
}

void bop_bench_add() {
	static Matrix<double>mat1 = identityMatrix<double>(3);
	static Matrix<double>mat2 = identityMatrix<double>(3);
	mat1 += mat2;
}

void bop_bench_subtract() {
	static Matrix<double>mat1 = identityMatrix<double>(3);
	static Matrix<double>mat2 = identityMatrix<double>(3);
	mat1 -= mat2;
}

void bop_bench_scalar() {
	static Matrix<double> mat = identityMatrix<double>(3);
	static double scalar = 2;
	mat *= scalar;
}

void bop_bench_scalar_div() {
	static Matrix<double> mat = identityMatrix<double>(3);
	static double scalar = 2;
	mat /= scalar;
}

void bop_bench_impose() {
	static Matrix<double> mat1 = identityMatrix<double>(3);
	static Matrix<double> mat2 = {{},{}};
}

int mat_tests() {
	std::cout << "All matrix operations are performed on a 3 by 3 matrix unless otherwise specified" << std::endl;
	std::cout << "Matrix construction: " << gist::benchmark(TEST_COUNT, bop_bench_construct) << std::endl;
	std::cout << "Matrix multiplication: " << gist::benchmark(TEST_COUNT, bop_bench_multiply) << std::endl;
	std::cout << "Matrix determinant: " << gist::benchmark(TEST_COUNT, bop_bench_det) << std::endl;
	std::cout << "Matrix inverse (2x2): " << gist::benchmark(TEST_COUNT, bop_bench_inverse_2x2) << std::endl;
	std::cout << "Matrix inverse (unit): " << gist::benchmark(TEST_COUNT, bop_bench_inverse_unit) << std::endl;
	std::cout << "Matrix inverse: " << gist::benchmark(TEST_COUNT, bop_bench_inverse) << std::endl;
	std::cout << "Matrix addition: " << gist::benchmark(TEST_COUNT, bop_bench_add) << std::endl;
	std::cout << "Matrix subtraction: " << gist::benchmark(TEST_COUNT, bop_bench_subtract) << std::endl;
	std::cout << "Matrix scalar multiplication: " << gist::benchmark(TEST_COUNT, bop_bench_scalar) << std::endl;
	std::cout << "Matrix scalar division: " << gist::benchmark(TEST_COUNT, bop_bench_scalar_div) << std::endl;
	return 0;
}

int main() {
	mat_tests();
	return 0;
}
