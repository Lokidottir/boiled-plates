#Boiled Plates makefile, primarily used for creating test programs.
CC=g++
CC_FLAGS= -static -static-libgcc -static-libstdc++ -Wall -std=c++11 -g
#removed from repo
#3RDPARTY_INCL=-I3rdparty/glm -I3rdparty/SDL2/include -I3rdparty/luaJIT/src -I3rdparty/SOIL
#3RDPARTY_LIBS=
LD=-Iboiled-plates

all: maths-test mat-benchmark top-test

maths-test: 
	$(CC) $(CC_FLAGS) bop-tests/test_maths.cpp -o maths-test $(LD)

mat-benchmark:
	$(CC) $(CC_FLAGS) bop-tests/matrix_benchmark.cpp -o mat-benchmark $(LD)
	$(CC) $(CC_FLAGS) -O3 bop-tests/matrix_benchmark.cpp -o mat-benchmark-opti $(LD)

util-test:
	$(CC) $(CC_FLAGS) bop-tests/test_util.cpp -o util-test $(LD)
	
top-test:
	$(CC) $(CC_FLAGS) bop-tests/test_topology.cpp -o top-test $(LD)
	
profile:
	$(CC_FLAGS) += -pg

clean:
	rm -f maths-test
	rm -f mat-benchmark
	rm -f mat-benchmark-opti
	rm -f top-test
