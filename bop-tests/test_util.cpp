#include <iostream>
#include <utility>
#include <bop-utility/utility.hpp>

using namespace bop::util;

int testMultiArr() {
    std::cout << "#######################################" << std::endl;
    std::cout << "### Multi dimensional array testing ###" << std::endl;
    std::cout << "#######################################\n" << std::endl;
    std::cout << "Creating a 5-dimensional array with all constructor params as 2" << std::endl;
    IndexHandler<int, 5> arr(2,2,2,2,2);
    std::cout << "arr5_2.size() returns " << arr.size() << std::endl;
    std::cout << "arr5_2.fullSize() returns " << arr.fullSize() << std::endl;
    std::cout << "setting [0][1][1][0][1] as 4" << std::endl;
    arr[0][1][1][0][1] = 4.0;
    std::cout << "printing value..." << std::endl;
    std::cout << arr[0][1][1][0][1] << std::endl;
    
    return 0;
}

int main() {
    std::cout << "testMultiArr returned: " << testMultiArr() << std::endl;
    return 0;
}
