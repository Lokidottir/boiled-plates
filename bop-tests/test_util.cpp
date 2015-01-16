#include <iostream>
#include <utility>
#include <bop-utility/utility.hpp>
#include <bop-maths/maths.hpp>

using namespace bop::util;

int test_threadpool() {

    std::cout << "This hardware supports " << std::thread::hardware_concurrency() << " threads." << std::endl;

    ThreadPool threads(3);
    bop::maths::Matrix<double> mat1 = {{0,1,2},{3,4,5},{6,7,8}};
    bop::maths::Matrix<double> mat2 = bop::maths::IdentityMatrix<double>::make(3,3);
    mat2 *= 2;
    threads.addTask([](std::string str)->void{ std::cout << str << std::endl;},"1111111111");
    threads.addTask([](std::string str)->void{ std::cout << str << std::endl;},"2222222222");
    threads.addTask([](std::string str)->void{ std::cout << str << std::endl;},"3333333333");
    threads.addTask([](std::string str)->void{ std::cout << str << std::endl;},"4444444444");
    //std::cout << threads.numberOfTasks() << std::endl;
    //std::cout << mat1 << std::endl;
    while(threads.hasRunning());
    return 0;
}

int main() {
    std::cout << "ThreadPool testing returned " << test_threadpool() << std::endl;
    return 0;
}
