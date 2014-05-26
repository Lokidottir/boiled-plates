#include <iostream>
#include "maths.hpp"

using namespace bop::maths;

int testVectors() {
	/*
		Testing Operators
	*/
	Vector<double> vec3 = {4,7,2};
	std::cout << "vector " << vec3.string() << " multiplied by 4: " << (vec3 * 4.0).string() << std::endl;
	std::cout << "vector " << vec3.string() << " divided by 4: " << (vec3 / 4.0).string() << std::endl;
	Vector<double> vec3_1 = {7,2,9};
	std::cout << "vector " << vec3.string() << " add: " << vec3_1.string() << " equals " << (vec3 + vec3_1).string() << std::endl;
	std::cout << "vector " << vec3.string() << " subtract: " << vec3_1.string() << " equals " << (vec3 - vec3_1).string() << std::endl;
	/*
		Testing Swap
	*/
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
	std::cout << "coordinate vals of " << vec3.string() << " are x: " << vec3.x() << " y: " << vec3.y() << " z: " << vec3.z() << " with a w of: " << vec3.w() << std::endl;
	/*
		
	*/
	return 0;
}

int main() {
	std::cout << testVectors();
	return 0;
}
