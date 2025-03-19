#!/bin/bash

set -x

rm -f comb_opt_vec_knl

icc \
    -DVEC \
    src/*.cpp \
    -std=c++11 \
    -O3 \
    -o comb_opt_vec_knl

./comb_opt_vec_knl
