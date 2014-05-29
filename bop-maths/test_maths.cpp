#include <iostream>
#include "maths.hpp"

using namespace bop::maths;

int testVectors() {
	std::cout << "####\n####\nmaths::bop::Vector testing\n####\n####" << std::endl;
	/*
		Testing Operators
	*/
	std::cout << "\n####\nOperator Tests\n####" << std::endl;
	Vector<double> vec3 = {4,7,2};
	std::cout << "vector " << vec3.string() << " multiplied by 4: " << (vec3 * 4.0).string() << std::endl;
	std::cout << "vector " << vec3.string() << " divided by 4: " << (vec3 / 4.0).string() << std::endl;
	Vector<double> vec3_1 = {7,2,9};
	std::cout << "vector " << vec3.string() << " add: " << vec3_1.string() << " equals " << (vec3 + vec3_1).string() << std::endl;
	std::cout << "vector " << vec3.string() << " subtract: " << vec3_1.string() << " equals " << (vec3 - vec3_1).string() << std::endl;
	/*
		Testing Swap
	*/
	std::cout << "\n####\nSwap Tests\n####" << std::endl;
	std::cout << "I shall perform a magic trick! Watch as I change the positions of these vectors!" << std::endl;
	Vector<double> vec8 = {1,2,3,4,5,6,7,8};
	Vector<double> vec4 = {9,10,11,12};
	std::cout << vec8.string() << " and " << vec4.string() << " shall be swapped!" << std::endl;
	std::cout << "Algebradabra!" << std::endl;
	vec8.swap(vec4);
	std::cout << "The vector of 8 is now: " << vec8.string() << " and the vector of 4 is: " << vec4.string() << std::endl;
	/*
		Testing coords
	*/
	std::cout << "\n####\nCoordinate Tests\n####" << std::endl;
	std::cout << "Coordinate vals of " << vec3.string() << " are x: " << vec3.x() << " y: " << vec3.y() << " z: " << vec3.z() << " with a w of: " << vec3.w() << std::endl;
	/*
		Pivot test
	*/
	std::cout << "\n####\nPivot Tests\n####" << std::endl;
	Vector<double> vec_pivot = {0,0,5,3};
	std::cout << "The pivot col of vector " << vec_pivot.string() << " is: " << vec_pivot.pivot() << std::endl;
	/*
		Bool test
	*/
	std::cout << "\n####\nBoolean Tests\n####" << std::endl;
	Vector<double> vec_bool_1 = {4,2,1};
	Vector<double> vec_bool_2 = {4,2,1};
	Vector<double> vec_bool_3 = {9,3,1};
	std::cout << "For the following vectors: " << vec_bool_1.string() << ", " << vec_bool_2.string() << ", " << vec_bool_3.string() <<std::endl;
	std::cout << "(vec_bool_1 == vec_bool_2) evaluates to " << (vec_bool_1 == vec_bool_2) << std::endl;
	std::cout << "(vec_bool_1 != vec_bool_2) evaluates to " << (vec_bool_1 != vec_bool_2) << std::endl;
	std::cout << "(vec_bool_2 == vec_bool_3) evaluates to " << (vec_bool_2 == vec_bool_3) << std::endl;
	std::cout << "(vec_bool_2 != vec_bool_2) evaluates to " << (vec_bool_2 != vec_bool_3) << std::endl;
	/*
		Size test
	*/
	std::cout << "\n####\nSize Tests\n####" << std::endl;
	Vector<double> vec_size = {5,6,9,3,8,5,7,11,993,1232,-6,2,1.000005,2};
	std::cout << "Size of vector " << vec_size.string() << " is " << vec_size.size() << std::endl;
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
	/*
		Swap tests
	*/
	std::cout << "\n####\nSwap Tests\n####" << std::endl;
	std::cout << "Matrix 1:" << std::endl << mat1.string() << std::endl << "to be swapped with Matrix 2:" << std::endl << mat2.string() << std::endl;
	mat1.swap(mat2);
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
	/*
		Inverse test
	*/
	return 0;
}

int main() {
	std::cout << "Vector test returned " << testVectors() << std::endl;
	std::cout << "Matrix test returned " << testMatrices() << std::endl;
	return 0;
}
