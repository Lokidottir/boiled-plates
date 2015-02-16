#include <iostream>
#define BOP_MATRIX_MULTIPLY_DISCARD_TINY
#include <bop-maths/maths.hpp>
#include <bop-utility/Benchmark.hpp>
#include <bop-defaults/types.hpp>
#include <iostream>
#include <complex>
#define TEST_COUNT 1000000


using namespace bop::maths;
using namespace bop::util;
using namespace bop;

#define BENCH_TYPE double

void bop_bench_construct_empty() {
    Matrix<BENCH_TYPE> mat;
}

void bop_bench_construct() {
    Matrix<BENCH_TYPE> mat1(3,3,0);
}

void bop_bench_constr_inlist() {
    Matrix<BENCH_TYPE> mat1 = {{1,2,3},{4,5,6},{7,8,9}};
}

void bop_bench_copy() {
    static Matrix<BENCH_TYPE> mat1 = {{1,2,3},{4,5,6},{7,8,9}};
    Matrix<BENCH_TYPE> mat2(mat1);
}

void bop_bench_construct_vec() {
    Vector<BENCH_TYPE> vec(9);
}

void bop_bench_det() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    mat1.det();
}

void bop_bench_det_15x15() {
    static Matrix<BENCH_TYPE> mat = IdentityMatrix<BENCH_TYPE>::make(15);
    mat.det();
}

void bop_bench_multiply() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE> mat2 = {{2,3,4},{6,1,7},{3,4,5}};
    mat2 *= mat1;
}

void bop_bench_add_make() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE> mat2 = IdentityMatrix<BENCH_TYPE>::make(3);
    mat1 + mat2;

}

void bop_bench_assign_copy() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE> mat2 = IdentityMatrix<BENCH_TYPE>::make(3);
    mat2 = mat1;
}

void bop_bench_inverse_2x2() {
    static Matrix<BENCH_TYPE> mat = {{2,4},{1,7}};
    mat.inverted();
}

void bop_bench_inverse_unit() {
    static Matrix<BENCH_TYPE> mat = IdentityMatrix<BENCH_TYPE>::make(3);
    mat.inverted();
}

void bop_bench_inverse() {
    static Matrix<BENCH_TYPE> mat = {{3,2,4},{2,7,2},{-1,2,5}};
    mat.inverted();
}

void bop_bench_invert_self() {
    static Matrix<BENCH_TYPE> mat = {{3,2,4},{2,7,2},{-1,2,5}};
    mat.invert();
}

void bop_bench_add() {
    static Matrix<BENCH_TYPE>mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE>mat2 = IdentityMatrix<BENCH_TYPE>::make(3);
    mat1 += mat2;
}

void bop_bench_vector_add() {
    static Vector<BENCH_TYPE> vec1 = {1,2,3,4,5,6,7,8,9};
    static Vector<BENCH_TYPE> vec2 = {9,8,7,6,5,4,3,2,1};
    vec1 += vec2;
}

void bop_bench_subtract() {
    static Matrix<BENCH_TYPE>mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE>mat2 = IdentityMatrix<BENCH_TYPE>::make(3);
    mat1 -= mat2;
}

void bop_bench_scalar() {
    static Matrix<BENCH_TYPE> mat = IdentityMatrix<BENCH_TYPE>::make(3);
    static BENCH_TYPE scalar = 2;
    mat *= scalar;
}

void bop_bench_scalar_div() {
    static Matrix<BENCH_TYPE> mat = IdentityMatrix<BENCH_TYPE>::make(3);
    static BENCH_TYPE scalar = 2;
    mat /= scalar;
}

void bop_bench_impose() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE> mat2 = {{2,4},{6,8}};
    mat1.impose(mat2);
}

void bop_bench_impose_vec() {
    static Matrix<BENCH_TYPE> mat = {{2,2,2},{3,4,5},{1,1,2}};
    static Vector<BENCH_TYPE> vec = {3,3,-1};
    mat.impose(vec);
}

void bop_bench_compare() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE> mat2 = IdentityMatrix<BENCH_TYPE>::make(3);
    mat1 == mat2;
}

void bop_bench_swap() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE> mat2 = IdentityMatrix<BENCH_TYPE>::make(3);
    static Matrix<BENCH_TYPE> mat3;
    mat3 = std::move(mat1);
    mat1 = std::move(mat2);
    mat2 = std::move(mat3);
}

void bop_bench_largemat() {
    static Matrix<BENCH_TYPE> mat1 = IdentityMatrix<BENCH_TYPE>::make(15);
    static Matrix<BENCH_TYPE> mat2 = IdentityMatrix<BENCH_TYPE>::make(15);
    mat1 *= mat2;
}

void bop_bench_transpose() {
    static Matrix<BENCH_TYPE> mat = {{0,1,2},{3,4,5},{6,7,8}};
    mat.transpose();
}

void bop_bench_transpose_3x4() {
    static Matrix<BENCH_TYPE> mat = {{0,1,2},{3,4,5},{6,7,8},{9,10,11}};
    mat.transpose();
}

void bop_bench_access_offset_matrix() {
    const Matrix<int_type>& mat = OffsetMatrix::make(3,3);
}

uint_type num = 0;

void bop_integer_incrementation() {
    num++;
}

int mat_tests() {
    std::cout << "\nAll matrix operations are performed on a 3 by 3 matrix unless otherwise specified" << std::endl << std::fixed;
    std::cout << "The result is the average operation time in nanoseconds for " << TEST_COUNT << " iterations." << std::endl;
    std::cout << "Matrix construction:              " << benchmark(TEST_COUNT, bop_bench_construct) << std::endl;
    std::cout << "Matrix construction (null):       " << benchmark(TEST_COUNT, bop_bench_construct_empty) << std::endl;
    std::cout << "Matrix construction by init list: " << benchmark(TEST_COUNT, bop_bench_constr_inlist) << std::endl;
    std::cout << "Matrix construction by copy:      " << benchmark(TEST_COUNT, bop_bench_copy) << std::endl;
    std::cout << "Matrix assign-copy:               " << benchmark(TEST_COUNT, bop_bench_assign_copy) << std::endl;
    std::cout << "Vector (9) construction:          " << benchmark(TEST_COUNT, bop_bench_construct_vec) << std::endl;
    std::cout << "Matrix multiplication:            " << benchmark(TEST_COUNT, bop_bench_multiply) << std::endl;
    std::cout << "Matrix multiplication (15x15):    " << benchmark(TEST_COUNT/10, bop_bench_largemat) << std::endl;
    std::cout << "Matrix determinant:               " << benchmark(TEST_COUNT, bop_bench_det) << std::endl;
    std::cout << "Matrix determinant (15x15):       " << benchmark(TEST_COUNT, bop_bench_det_15x15) << std::endl;
    std::cout << "Matrix inverse (2x2):             " << benchmark(TEST_COUNT, bop_bench_inverse_2x2) << std::endl;
    std::cout << "Matrix inverse (unit):            " << benchmark(TEST_COUNT, bop_bench_inverse_unit) << std::endl;
    std::cout << "Matrix inverse:                   " << benchmark(TEST_COUNT, bop_bench_inverse) << std::endl;
    std::cout << "Matrix invert self:               " << benchmark(TEST_COUNT, bop_bench_invert_self) << std::endl;
    std::cout << "Matrix addition:                  " << benchmark(TEST_COUNT, bop_bench_add) << std::endl;
    std::cout << "Matrix add for new:               " << benchmark(TEST_COUNT, bop_bench_add_make) << std::endl;
    std::cout << "Vector (9) addition:              " << benchmark(TEST_COUNT, bop_bench_vector_add) << std::endl;
    std::cout << "Matrix subtraction:               " << benchmark(TEST_COUNT, bop_bench_subtract) << std::endl;
    std::cout << "Matrix scalar multiplication:     " << benchmark(TEST_COUNT, bop_bench_scalar) << std::endl;
    std::cout << "Matrix scalar division:           " << benchmark(TEST_COUNT, bop_bench_scalar_div) << std::endl;
    std::cout << "Matrix transposition:             " << benchmark(TEST_COUNT, bop_bench_transpose) << std::endl;
    //std::cout << "Matrix transposition (3x4):       " << benchmark(TEST_COUNT, bop_bench_transpose_3x4) << std::endl;
    std::cout << "Matrix (2x2) imposition on (3x3): " << benchmark(TEST_COUNT, bop_bench_scalar_div) << std::endl;
    std::cout << "Matrix comparison (2 units):      " << benchmark(TEST_COUNT, bop_bench_compare) << std::endl;
    std::cout << "Matrix on matrix imposition:      " << benchmark(TEST_COUNT, bop_bench_impose) << std::endl;
    std::cout << "Vector on matrix imposition:      " << benchmark(TEST_COUNT, bop_bench_impose_vec) << std::endl;
    std::cout << "Matrix move:                      " << benchmark(TEST_COUNT, bop_bench_swap)/3 << std::endl;
    std::cout << "Get offset matrix:                " << benchmark(TEST_COUNT, bop_bench_access_offset_matrix) << std::endl;
    std::cout << "Increment integer:                " << benchmark<double>(TEST_COUNT * 100, bop_integer_incrementation) << std::endl;
    std::cout << num << std::endl;
    return 0;
}

int main() {
    OffsetMatrix::make(3,3);
    mat_tests();
    return 0;
}
