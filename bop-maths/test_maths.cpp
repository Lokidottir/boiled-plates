#include <iostream>
#include "maths.hpp"

using namespace bop::maths;

int testVectors() {
	Vector<double> vec3 = {4,7,2};
	std::cout << "vector " << vec3.string() << " multiplied by 4: " << (vec3 * 4.0).string() << " is quite good." << std::endl;
	return 0;
}

int main() {
	std::cout << testVectors();
	return 0;
}
