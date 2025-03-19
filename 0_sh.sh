#!/bin/bash

set -x

rm -f comb_opt_vec

g++ \
    src/*.cpp \
    -std=c++11 \
    -o comb_opt_vec

./comb_opt_vec
