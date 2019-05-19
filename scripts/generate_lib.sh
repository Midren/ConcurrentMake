#!/usr/bin/env bash
rm -rf obj libs
mkdir obj/ libs/
cd obj
g++ ../*.cpp -O3 -c
rm main.o
ar rcs ../libs/lib1.a *.o 
