#!/bin/bash
make -B -f MAKE.mak BP_CC=clang++
./mat-benchmark-opti
./maths-test
