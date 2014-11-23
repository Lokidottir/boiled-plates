#include <iostream>
#include <utility>
#include <bop-topology/MarkovChain.hpp>
#include <bop-utility/FileLoading.hpp>
#include <string>

using namespace bop;


int markovTesting() {
    top::MarkovChain<std::string> markov1;
    auto file_vec = util::loadIntoVector("pg1342_out.txt", ' ');
    markov1.feed(file_vec);
    std::cout << "Loaded hamlet, chain at size: " << markov1.size() << std::endl;
    auto generated = markov1.generate(3000, file_vec[int(top::probabilityFunction(file_vec.size()))]);
    std::cout << "Generated list." << std::endl;
    for (top::uint_type i = 0; i < file_vec.size(); i++) {
        std::cout << generated[i] << " ";
    }
    std::cout << std::endl;
    //std::cout << "Chain summary:\n" << markov1.genChainDescriptor() << std::endl;
    return 0;
}

int main() {
    std::cout << "MarkovChain test returned " << markovTesting() << std::endl;
    return 0;
}
