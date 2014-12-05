#!/bin/bash
make -B -f MAKE.mak BP_CC=clang++ 2>
./mat-benchmark-opti
./mat-benchmark
./maths-test
