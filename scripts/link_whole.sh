#!/usr/bin/env bash
g++ main.cpp -O3 -c
#g++ main.o -l1 -L./libs -o prog
declare -a libs
for i in libs/*.a
do
    libs+="-l${i:8:-2} "
done
lib_includes="${libs[@]}"
g++ main.o ${lib_includes}-L./libs -o prog
rm main.o
