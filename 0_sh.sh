#!/bin/bash

set -x

rm -f comb_opt_vec

g++ \
    src/*.cpp \
    -std=c++17 \
    -o comb_opt_vec

./comb_opt_vec
