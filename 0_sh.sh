#!/bin/bash

set -x

rm -f comb_opt_vec

g++ \
    -DVEC \
    src/*.cpp \
    -std=c++11 \
    -O3 \
    -o comb_opt_vec

./comb_opt_vec
