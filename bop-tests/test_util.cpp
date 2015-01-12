#include <iostream>
#include <utility>
#include <bop-utility/utility.hpp>
#include <bop-maths/maths.hpp>

using namespace bop::util;

int test_threadpool() {

    std::cout << "This hardware supports " << std::thread::hardware_concurrency() << " threads." << std::endl;

    ThreadPool threads(std::thread::hardware_concurrency() - 1);
    bop::maths::Matrix<double> mat1 = {{0,1,2},{3,4,5},{6,7,8}};
    bop::maths::Matrix<double> mat2 = bop::maths::IdentityMatrix<double>::make(3,3);
    for (uint_type iter = 0; iter < 1000000; iter++) threads.addTask([&, mat1, mat2]() -> void {
        mat1 * mat2;
    });
    std::cout << threads.size() << std::endl;
    while(threads.size() > 0);
    return 0;
}

int main() {
    std::cout << "ThreadPool testing returned " << test_threadpool() << std::endl;
    return 0;
}
