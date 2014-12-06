#Boiled Plates makefile, primarily used for creating test programs.
BP_CC?= g++
BP_CC_FLAGS?= -fdiagnostics-color=auto -static -static-libgcc -static-libstdc++ -Wall -std=c++1y -g
BP_LD?= -Iboiled-plates

all: maths-test mat-benchmark top-test

maths-test:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/test_maths.cpp -o maths-test $(BP_LD)

mat-benchmark:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/matrix_benchmark.cpp -o mat-benchmark $(BP_LD)
	$(BP_CC) $(BP_CC_FLAGS) -O3 bop-tests/matrix_benchmark.cpp -o mat-benchmark-opti $(BP_LD)

memory-test:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/test_memory.cpp -o memory-test $(BP_LD)

util-test:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/test_util.cpp -o util-test $(BP_LD)

top-test:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/test_topology.cpp -o top-test $(BP_LD)

profile:
	$(BP_CC_FLAGS) += -pg

clean:
	rm -f maths-test
	rm -f mat-benchmark
	rm -f mat-benchmark-opti
	rm -f top-test
	rm -f memory-test
