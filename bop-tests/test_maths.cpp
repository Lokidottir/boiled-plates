#include <iostream>
#include <utility>
#define BOP_MATRIX_MULTIPLY_DISCARD_TINY
#include <bop-maths/maths.hpp>

using namespace bop::maths;

int testVectors() {
	std::cout << "####\n####\nmaths::bop::Vector testing\n####\n####" << std::endl;
	/*
		Testing Operators
	*/
	std::cout << "\n####\nOperator Tests\n####" << std::endl;
	Vector<double> vec3 = {4,7,2};
	std::cout << "vector " << vec3 << " multiplied by 4: " << (vec3 * 4.0) << std::endl;
	std::cout << "vector " << vec3 << " divided by 4: " << (vec3 / 4.0) << std::endl;
	Vector<double> vec3_1 = {7,2,9};
	std::cout << "vector " << vec3 << " add: " << vec3_1 << " equals " << (vec3 + vec3_1) << std::endl;
	std::cout << "vector " << vec3 << " subtract: " << vec3_1 << " equals " << (vec3 - vec3_1) << std::endl;
	/*
		Testing Swap
	*/
	std::cout << "\n####\nSwap Tests\n####" << std::endl;
	std::cout << "I shall perform a magic trick! Watch as I change the positions of these vectors!" << std::endl;
	Vector<double> vec8 = {1,2,3,4,5,6,7,8};
	Vector<double> vec4 = {9,10,11,12};
	std::cout << vec8 << " and " << vec4 << " shall be swapped!" << std::endl;
	std::cout << "Algebradabra!" << std::endl;
	std::swap(vec8,vec4);
	std::cout << "The vector of 8 is now: " << vec8 << " and the vector of 4 is: " << vec4 << std::endl;
	/*
		Testing coords
	*/
	std::cout << "\n####\nCoordinate Tests\n####" << std::endl;
	std::cout << "Coordinate vals of " << vec3 << " are x: " << vec3.x() << " y: " << vec3.y() << " z: " << vec3.z() << " with a w of: " << vec3.w() << std::endl;
	/*
		Pivot test
	*/
	std::cout << "\n####\nPivot Tests\n####" << std::endl;
	Vector<double> vec_pivot = {0,0,5,3};
	std::cout << "The pivot col of vector " << vec_pivot << " is: " << vec_pivot.pivot() << std::endl;
	/*
		Bool test
	*/
	std::cout << "\n####\nBoolean Tests\n####" << std::endl;
	Vector<double> vec_bool_1 = {4,2,1};
	Vector<double> vec_bool_2 = {4,2,1};
	Vector<double> vec_bool_3 = {9,3,1};
	std::cout << "For the following vectors: " << vec_bool_1 << ", " << vec_bool_2 << ", " << vec_bool_3 <<std::endl;
	std::cout << "(vec_bool_1 == vec_bool_2) evaluates to " << (vec_bool_1 == vec_bool_2) << std::endl;
	std::cout << "(vec_bool_1 != vec_bool_2) evaluates to " << (vec_bool_1 != vec_bool_2) << std::endl;
	std::cout << "(vec_bool_2 == vec_bool_3) evaluates to " << (vec_bool_2 == vec_bool_3) << std::endl;
	std::cout << "(vec_bool_2 != vec_bool_2) evaluates to " << (vec_bool_2 != vec_bool_3) << std::endl;
	/*
		Size test
	*/
	std::cout << "\n####\nSize Tests\n####" << std::endl;
	Vector<double> vec_size = {5,6,9,3,8,5,7,11,993,1232,-6,2,1.000005,2};
	std::cout << "Size of vector " << vec_size << " is " << vec_size.size() << std::endl;
	/*
		Other tests
	*/
	std::cout << "\n####\nOther Tests\n####" << std::endl;
	Vector<double> vec_oth = vec_size;
	vec_size = vec_oth;
	vec_oth = vec_size;
	std::cout << "the 3rd element of vector: " << vec_oth << " is " << vec_oth[2] << std::endl;
	Vector<double> vec_norm(5);
	std::cout << "vector produced by size init: " << vec_norm << std::endl;
	Vector<double> vec_toimp = {0,0,0,2,4};
	Vector<double> vec_beimp = {1.0/4.0,1.0/2.0,1};
	std::cout << "imposing " << vec_beimp << " on " << vec_toimp << " for result: ";
	vec_toimp.impose(vec_beimp);
	std::cout << vec_toimp << std::endl;
	Vector<double> vec_invalid;
	std::cout << "The validity of a vector constructed with no values is " << bool(vec_invalid) << std::endl;
	return 0;
}

int testMatrices() {
	std::cout << "####\n####\nmaths::bop::Matrix testing\n####\n####" << std::endl;
	/*
		Operator test
	*/
	std::cout << "\n####\nOperator Tests\n####" << std::endl;
	Matrix<double> mat1 = {{2,6,-1},{-1,9,6},{4,2,9}};
	Matrix<double> mat2 = {{3,7,2},{-1,-1,-1},{5,2,-6}};
	std::cout << "Matrix 1:" << std::endl << mat1.string() << std::endl;
	std::cout << "Matrix 1 multiplied by 4:" << std::endl << (mat1 * 4.0).string() << std::endl;
	std::cout << "Matrix 1 divided by 4:" << std::endl << (mat1 / 4.0).string() << std::endl;
	std::cout << "Matrix 2:" << std::endl << mat2.string() << std::endl;
	std::cout << "Matrix 1 multiplied by Matrix 2:" << std::endl << (mat1 * mat2).string() << std::endl;
	std::cout << "Matrix 1 add Matrix 2:" << std::endl << (mat1 + mat2).string() << std::endl;
	std::cout << "Matrix 1 subtract Matrix 2:" << std::endl << (mat1 - mat2).string() << std::endl;
	Matrix<double> mat_withvec = {{2,0,0},{0,1,0},{0,0,2}};
	Vector<double> vec_mat3 = {4,2,1};
	std::cout << "Multiplying vector " << vec_mat3.string() << " by the matrix: " << std::endl << mat_withvec.string() << "gives the vector " << (mat_withvec * vec_mat3).string() << std::endl;
	
	/*
		Swap tests
	*/
	std::cout << "\n####\nSwap Tests\n####" << std::endl;
	std::cout << "Matrix 1:" << std::endl << mat1.string() << std::endl << "to be swapped with Matrix 2:" << std::endl << mat2.string() << std::endl;
	std::swap(mat1,mat2);
	std::cout << "Matrix 1:" << std::endl << mat1.string() << std::endl << "Matrix 2:" << std::endl << mat2.string() << std::endl;
	std::cout << "swapping rows 1 & 3 (non-zero index) of matrix 1:" << std::endl << mat1.string() <<std::endl;
	mat1.swapRows(0,2);
	std::cout << "Matrix 1 is now:" << std::endl << mat1.string();
	/*
		Bool test
	*/
	std::cout << "\n####\nBoolean Tests\n####" << std::endl;
	Matrix<double> bool_mat1 = {{3,7},{-1,2}};
	Matrix<double> bool_mat2 = {{3,7},{-1,2}};
	Matrix<double> bool_mat3 = {{10,3},{5,-7}};
	std::cout << "Bool matrix 1:" << std::endl << bool_mat1.string() << std::endl;
	std::cout << "Bool matrix 2:" << std::endl << bool_mat2.string() << std::endl;
	std::cout << "Bool matrix 3:" << std::endl << bool_mat3.string() << std::endl;
	std::cout << "(mat1 == mat2) evaluates to: " << (bool_mat1 == bool_mat2) << std::endl;
	std::cout << "(mat1 != mat2) evaluates to: " << (bool_mat1 != bool_mat2) << std::endl;
	std::cout << "(mat2 == mat3) evaluates to: " << (bool_mat2 == bool_mat3) << std::endl;
	std::cout << "(mat2 != mat3) evaluates to: " << (bool_mat2 != bool_mat3) << std::endl;
	std::cout << "mat1.identity() evaluates to: " << bool_mat1.identity() << std::endl;
	/*
		Other tests
	*/
	std::cout << "\n####\nOther Tests\n####" << std::endl;
	Matrix<double> mat3_impose = {{2,0,0},{0,2,0},{0,0,2}};
	Matrix<double> mat4_unit = identityMatrix<double>(4);
	std::cout << "imposing:\n" << mat3_impose.string() << "on:\n" << mat4_unit.string() << "yeilds:\n";
	mat4_unit.impose(mat3_impose);
	std::cout << mat4_unit.string() << std::endl;
	Matrix<double> mat_notvalid;
	std::cout << "The validity of a matrix constructed with no values is " << bool(mat_notvalid) << std::endl;
	/*
		External function tests
	*/
	std::cout << "\n####\nNon-member function Tests\n####" << std::endl;
	Matrix<double> unit_m = identityMatrix<double>(3);
	std::cout << "Identity matrix: " << std::endl << unit_m.string();
	Matrix<double> mat_ex1 = {{4,8},{0,1}};
	Matrix<double> mat_ex_i1 = inverseMatrix(mat_ex1);
	std::cout << "the inverse of matrix: " << std::endl << mat_ex1.string() << "is: " << std::endl << inverseMatrix(mat_ex1).string() << std::endl;
	std::cout << "the product of the matrix and it's inverse is " << std::endl << (mat_ex1 * mat_ex_i1).string() << std::endl;
	Matrix<double> mat_3_toinvert = {{1,2,3},{0,1,4},{7,3,0}};
	std::cout << "the inverse of 3 by 3 matrix:\n" << mat_3_toinvert.string();
	Matrix<double> mat_3_inverted = inverseMatrix(mat_3_toinvert);
	std::cout << "is:\n" << mat_3_inverted.string();
	std::cout << "when the matrix is multiplied by it's inverse:\n" << (mat_3_toinvert * mat_3_inverted).string() << std::endl;
	
	Matrix<double> mat_initbynum(5);
	std::cout << "matrix produced by size init\n" << mat_initbynum.string() << std::endl;
	
	Matrix<double> mat_nonsq = {{2,3,5},{6,1,9}};
	std::cout << "the transpose of " << std::endl << mat_nonsq.string() << "is: " << std::endl << transposeMatrix(mat_nonsq).string() << std::endl;
	
	Matrix<double> matrot_1 = rotationMatrix<double>(90, 2);
	std::cout << "A rotation matrix for 90 degrees:\n" << matrot_1.string() << std::endl;
	Matrix<double> matrot_2 = { {2,5},
								{4,7}};
	Vector<double> vec1 = {4,6};
	matrot_1 *= vec1;
	matrot_2 *= matrot_1;
	std::cout << "when applied to matrix" << std::endl << matrot_2.string() << "yeilds" << std::endl << (matrot_2 * matrot_1).string() << "and applied to vector " << vec1.string() << " yeilds " << (matrot_1 * vec1).string() << std::endl;
	
	Matrix<double> mat_LU = {{3,4,5},{1,7,2},{6,6,13}};
	auto LU_decomp = mat_LU.decompose();
	std::cout << "the matrix:\n" << mat_LU.string() << "has the LU decomposition of\n" << LU_decomp.lower.string() << "and\n" << LU_decomp.upper.string() << std::endl; 
	
	return 0;
}

int main() {
	std::cout << "Vector test returned " << testVectors() << std::endl;
	std::cout << "Matrix test returned " << testMatrices() << std::endl;
	return 0;
}
