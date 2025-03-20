#ifndef AVX_H
#define AVX_H

#include <vector>
#include <iostream>

#include "utils.h"

#define _MM_CMPINT_ENUM int
#define _MM_CMPINT_EQ    0
#define _MM_CMPINT_LT    1
#define _MM_CMPINT_LE    2
#define _MM_CMPINT_FALSE 3
#define _MM_CMPINT_NE    4
#define _MM_CMPINT_NLT   5
#define _MM_CMPINT_NLE   6
#define _MM_CMPINT_TRUE  7

#define N 16
#define __m512i std::vector<int>
#define __mmask16 uint32_t

using namespace std;

// Statistics.
extern int ath_cnt;
extern int ath_exe;
extern int cmp_cnt;
extern int cmp_exe;
extern int gth_cnt;
extern int gth_exe;
extern int sct_cnt;
extern int sct_exe;

void
zero_stat();

void
print_vec(__m512i v);

void
print_mask(__mmask16 m);

__m512i
_mm512_set_epi32(int e15, int e14, int e13, int e12, int e11, int e10, int e9, int e8,
                 int e7, int e6, int e5, int e4, int e3, int e2, int e1, int e0);

__m512i
_mm512_set1_epi32(int a);

__m512i
_mm512_loadu_epi32(void* mem_addr);

void
_mm512_storeu_epi32(void* mem_addr, __m512i a);

void
_mm512_mask_storeu_epi32(void* mem_addr, __mmask16 k, __m512i a);

__m512i
_mm512_mask_i32gather_epi32(__m512i src,
                            __mmask16 k,
                            __m512i vindex,
                            void* base_addr,
                            int scale);

void
_mm512_mask_i32scatter_epi32(void* base_addr,
                             __mmask16 k,
                             __m512i vindex,
                             __m512i a,
                             int scale);

__mmask16
_mm512_mask_cmp_epi32_mask(__mmask16 k1, __m512i a, __m512i b, _MM_CMPINT_ENUM imm8);

__mmask16
_mm512_cmp_epi32_mask(__m512i a, __m512i b, _MM_CMPINT_ENUM imm8);

__m512i
_mm512_mask_add_epi32(__m512i src, __mmask16 k, __m512i a, __m512i b);

__m512i
_mm512_add_epi32(__m512i a, __m512i b);

__m512i
_mm512_mask_sub_epi32(__m512i src, __mmask16 k, __m512i a, __m512i b);

__m512i
_mm512_sub_epi32(__m512i a, __m512i b);

#endif
