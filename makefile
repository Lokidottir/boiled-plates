#Boiled Plates makefile, primarily used for creating test programs.
BP_CC?= g++
BP_CC_FLAGS?= -pthread -fdiagnostics-color=auto -Wall -std=c++1y -g
OLD_STATIC_FLAGS= -static -static-libgcc -static-libstdc++
BP_LD?= -Iboiled-plates
BP_TESTEXEC_LOC=testbin/
all: make-folder maths-test mat-benchmark memory-test mem-benchmark util-test

make-folder:
	-mkdir testbin

maths-test:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/test_maths.cpp -o $(BP_TESTEXEC_LOC)maths-test $(BP_LD)

mat-benchmark:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/matrix_benchmark.cpp -o $(BP_TESTEXEC_LOC)mat-benchmark $(BP_LD)
	$(BP_CC) $(BP_CC_FLAGS) -O3 bop-tests/matrix_benchmark.cpp -o $(BP_TESTEXEC_LOC)mat-benchmark-opti $(BP_LD)

mem-benchmark:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/memory_benchmark.cpp -o $(BP_TESTEXEC_LOC)mem-benchmark $(BP_LD)
	$(BP_CC) $(BP_CC_FLAGS) -O3 bop-tests/memory_benchmark.cpp -o $(BP_TESTEXEC_LOC)mem-benchmark-opti $(BP_LD)
	$(BP_CC) $(BP_CC_FLAGS) -DBOP_MATRIX_USE_RECYCLER -O3 bop-tests/matrix_benchmark.cpp -o $(BP_TESTEXEC_LOC)mem-benchmark-matrix-recycled $(BP_LD)

memory-test:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/test_memory.cpp -o $(BP_TESTEXEC_LOC)memory-test $(BP_LD)

util-test:
	$(BP_CC) $(BP_CC_FLAGS) -O3 bop-tests/test_util.cpp -o $(BP_TESTEXEC_LOC)util-test $(BP_LD)

top-test:
	$(BP_CC) $(BP_CC_FLAGS) bop-tests/test_topology.cpp -o $(BP_TESTEXEC_LOC)top-test $(BP_LD)

run:
	./run_all_tests

clean:
	rm -f $(BP_TESTEXEC_LOC)*
