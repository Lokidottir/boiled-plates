#include <iostream>
#include <utility>
#include <bop-topology/MarkovChain.hpp>
#include <bop-utility/FileLoading.hpp>
#include <string>

using namespace bop;


int markovTesting() {
	top::MarkovChain<std::string> markov1;
	markov1.feed(util::loadIntoVector("hamlet.txt", ' '));
	std::cout << "Loaded hamlet." << std::endl;
	auto generated = markov1.generate(250);
	std::cout << "Generated list." << std::endl;
	for (top::uint_type i = 0; i < generated.size(); i++) {
		std::cout << generated[i];
	}
	return 0;
}

int main() {
	std::cout << "MarkovChain test returned " << markovTesting() << std::endl;
	return 0;
}
