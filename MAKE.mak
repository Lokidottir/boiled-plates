#Boiled Plates makefile, primarily used for creating test programs.
CC=g++
CC_FLAGS=-static -static-libgcc -static-libstdc++ -Wall -std=c++11 -g
3RDPARTY_INCL=-I3rdparty/glm -I3rdparty/SDL2/include -I3rdparty/luaJIT/src -I3rdparty/SOIL
3RDPARTY_LIBS=
LD=-Iboiled-plates

all: maths-test

maths-test: 
	$(CC) $(CC_FLAGS) bop-tests/test_maths.cpp -o maths-test $(LD)

clean: