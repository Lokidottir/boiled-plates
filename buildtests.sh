#!/bin/bash
make -B -f MAKE.mak
./mat-benchmark-opti
./maths-test
rm maths-test
rm mat-benchmark-opti
rm mat-benchmark
