#Boiled Plates makefile, primarily used for creating test programs.
CC=g++
CC_FLAGS=-static -static-libgcc -static-libstdc++ -Wall -std=c++11
LD=-I../
all: maths-test

maths-test: 
	$(CC) $(CC_FLAGS) test_env/test_maths.cpp -o maths-test

clean: