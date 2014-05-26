#Boiled Plates makefile, primarily used for creating test programs.
CC=g++
CC_FLAGS=-static -static-libgcc -static-libstdc++ -Wall -std=c++11 -g
LD=-I../
all: maths-test

maths-test: 
	$(CC) $(CC_FLAGS) bop-maths/test_maths.cpp -o maths-test $(LD)

clean: