
#include <iostream>
#include <string>
#include <bop-memory/memory.hpp>

using namespace bop;
using namespace mem;

struct TestObj{
    int number;
    std::string str;
    TestObj() : number(0), str("") {
    }

    TestObj(int number, std::string string) : TestObj() {
        this->number = number;
        this->str = string;
    }
};

int test_pointers() {
    std::cout << "---Pointer testing---" << std::endl;
    Pointer<int> ptr_1 = std::move(Pointer<int>(new int(6)));
    return 0;
}

int test_recycler() {
    std::cout << "---Recycler testing---" << std::endl;
    Recycler<TestObj> recycler;
    recycler.reserve(9,512);

    return 0;
}

int main() {
    std::cout << "pointer testing returned " << test_pointers() << std::endl;
    std::cout << "recycler testing returned " << test_recycler() << std::endl;
    return 0;
}
