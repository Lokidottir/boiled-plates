#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <random>
#include <utility>
#include <sstream>
#include <typeinfo>

namespace bop {
	namespace top {
		#ifndef BOP_TOPOLOGY_DEFAULT_TYPES
		#define BOP_TOPOLOGY_DEFAULT_TYPES
		typedef double prec_type;
		typedef uint64_t uint_type;
		#endif
		
		prec_type probabilityFunction(prec_type multiplier = 1) {
			return (prec_type(rand())/prec_type(RAND_MAX)) * multiplier;
		}
		
		template<class T>
		class ProbabilityState {
				
			public:
				
				std::map<T, uint_type> probability_map;
				uint_type total_fed;
				
				ProbabilityState () : probability_map(), total_fed(0) {
					
				}
				
				ProbabilityState(const ProbabilityState<T>& state) : probability_map(state.probability_map), total_fed(state.total_fed) {
				
				}
				
				ProbabilityState(ProbabilityState<T>&& state) {
					this->probability_map = std::move(state.probability_map);
					this->total_fed = state.total_fed;
				}
						
				void feed(const T& data) {
					this->probability_map[data]++;
					this->total_fed++;
				}
						
				prec_type probabilityOf(const T& data) const {
					if (total_fed == 0) return 0;
					else {
						return prec_type(probability_map[data])/prec_type(this->total_fed);
					}
				}
						
				const T& getNext() const {
					const static T none_found = T();
					prec_type rand_remaining = 0;
					prec_type roll = probabilityFunction();
					for (auto& i : this->probability_map) {
						if (roll - rand_remaining < prec_type(std::get<1>(i))/prec_type(total_fed)) {
							return std::get<0>(i);
						
						}
						else {rand_remaining += std::get<1>(i); }
					}
					return none_found;
				}
						
		};
		
		template<class T>
		class MarkovChain {

				std::map<T, ProbabilityState<T> > states;
				uint_type avg_data_size;
				uint_type times_called;
			public:
				MarkovChain() : states(), avg_data_size(0), times_called(0) {
					
				}
				
				void feed(const std::vector<T>& data) {
					if (data.size() > 0) for (uint_type i = 0; i < data.size() - 1; i++) {
						this->states[data[i]].feed(data[i + 1]);
					}
				}
				
				T* generate(uint_type data_size, const T& begin_with = T()) {
					if (data_size != 0) {
						T* generated = new T[data_size];
						generated[0] = begin_with;
						for (uint_type iter = 1; iter < data_size; iter++) {
							generated[iter] = this->states[generated[iter - 1]].getNext();
						}
						return generated;
					}
					else return nullptr;
				}
				
				uint_type size() {
					return this->states.size();
				}
				
				std::string genChainDescriptor() {
					std::stringstream descr;
					descr << "Markov chain w/ type id: " << typeid(T).name() << " at address " << uint_type(this) << std::endl;
						for (const auto& elem : this->states) {
							descr << "element " << std::get<0>(*elem) << ":" << std::endl;
							for (int i = 0; i < -1; i++) {
								//std::cout << '\t' << std::get<0>(*connect) << "\n\t\t" << std::get<1>(*connect) << std::endl;
							}
						}
					return descr.str();
				}
		};
	}
}
