#include "avx.h"

#include <vector>
#include <iostream>

#include "utils.h"

using namespace std;

// Statistics.
int ath_cnt;
int ath_exe;
int cmp_cnt;
int cmp_exe;
int gth_cnt;
int gth_exe;
int sct_cnt;
int sct_exe;

void
zero_stat()
{
    ath_cnt = 0;
    ath_exe = 0;
    cmp_cnt = 0;
    cmp_exe = 0;
    gth_cnt = 0;
    gth_exe = 0;
    sct_cnt = 0;
    sct_exe = 0;
}

void
print_vec(__m512i v)
{
    std::cout << "__m512i:";
    for (auto x : v) std::cout << " " << x;
    std::cout << endl;
}

void
print_mask(__mmask16 m)
{
    std::cout << "__mmask16:";
    for (int i = 0; i < N; ++i) std::cout << ((m & (1 << i)) ? 1 : 0);
    std::cout << endl;
}

__m512i
_mm512_set_epi32(int e15, int e14, int e13, int e12, int e11, int e10, int e9, int e8,
                 int e7, int e6, int e5, int e4, int e3, int e2, int e1, int e0)
{
    __m512i r { e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15 };
    return r;
}

__m512i
_mm512_set1_epi32(int a)
{
    __m512i r { a, a, a, a, /**/ a, a, a, a, /**/ a, a, a, a, /**/ a, a, a, a };
    return r;
}

__m512i
_mm512_loadu_epi32(void* mem_addr)
{
    __m512i r(N);
    for (int i = 0; i < N; ++i) r[i] = static_cast<int*>(mem_addr)[i];
    return r;
}

void
_mm512_storeu_epi32(void* mem_addr, __m512i a)
{
    for (int i = 0; i < N; ++i) static_cast<int*>(mem_addr)[i] = a[i];
}

void
_mm512_mask_storeu_epi32(void* mem_addr, __mmask16 k, __m512i a)
{
    for (int i = 0; i < N; ++i) if (k & (1 << i)) static_cast<int*>(mem_addr)[i] = a[i];
}

/**
 * NB!
 * There is some problem with name _mm512_mask_i32gather_epi32
 * when using icc compiler.
 */
__m512i
_mm512_mask_i32gather_epi32(__m512i src,
                            __mmask16 k,
                            __m512i vindex,
                            void* base_addr,
                            int scale)
{
    __m512i r(N);

    for (int i = 0; i < N; ++i)
    {
        r[i] =
            (k & (1 << i))
            ? *static_cast<int*>(static_cast<void*>(static_cast<char*>(base_addr) + vindex[i] * scale))
            : src[i];
    }

    gth_cnt += 16;
    gth_exe += popcnt(k);

    return r;
}

/**
 * NB!
 * There is some problem with name _mm512_mask_i32scatter_epi32
 * when using icc compiler.
 */
void
_mm512_mask_i32scatter_epi32(void* base_addr,
                             __mmask16 k,
                             __m512i vindex,
                             __m512i a,
                             int scale)
{
    for (int i = 0; i < N; ++i)
    {
        if (k & (1 << i))
        {
            *static_cast<int*>(static_cast<void*>(static_cast<char*>(base_addr) + vindex[i] * scale)) = a[i];
        }
    }

    sct_cnt += 16;
    sct_exe += popcnt(k);
}

__mmask16
_mm512_mask_cmp_epi32_mask(__mmask16 k1, __m512i a, __m512i b, _MM_CMPINT_ENUM imm8)
{
    __mmask16 m = 0x0;

    switch (imm8)
    {
        case _MM_CMPINT_LT:
            for (int i = 0; i < N; ++i) if (k1 & (1 << i)) if (a[i] < b[i]) m = m | (1 << i);
            break;
        case _MM_CMPINT_LE:
            for (int i = 0; i < N; ++i) if (k1 & (1 << i)) if (a[i] <= b[i]) m = m | (1 << i);
            break;
        default:
            cout << "Internal error !!!" << endl;
            exit(0);
            break;
    }

    cmp_cnt += 16;
    cmp_exe += popcnt(k1);

    return m;
}

__mmask16
_mm512_cmp_epi32_mask(__m512i a, __m512i b, _MM_CMPINT_ENUM imm8)
{
    return _mm512_mask_cmp_epi32_mask(0xFFFF, a, b, imm8);
}

__m512i
_mm512_mask_add_epi32(__m512i src, __mmask16 k, __m512i a, __m512i b)
{
    __m512i r(N);

    for (int i = 0; i < N; ++i) r[i] = (k & (1 << i)) ? (a[i] + b[i]) : src[i];

    ath_cnt += 16;
    ath_exe += popcnt(k);

    return r;
}

__m512i
_mm512_add_epi32(__m512i a, __m512i b)
{
    __m512i z(N);
    return _mm512_mask_add_epi32(z, 0xFFFF, a, b);
}

__m512i
_mm512_mask_sub_epi32(__m512i src, __mmask16 k, __m512i a, __m512i b)
{
    __m512i r(N);

    for (int i = 0; i < N; ++i) r[i] = (k & (1 << i)) ? (a[i] - b[i]) : src[i];

    ath_cnt += 16;
    ath_exe += popcnt(k);

    return r;
}

__m512i
_mm512_sub_epi32(__m512i a, __m512i b)
{
    __m512i z(N);
    return _mm512_mask_sub_epi32(z, 0xFFFF, a, b);
}
