#!/bin/bash

make clean
make
./slytherin.o -i ./test/test.py -o
gcc -o main ./out/asm.s -no-pie
./main