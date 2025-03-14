#ifndef AVX_H
#define AVX_H

#include <vector>
#include <iostream>

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
#define __m512i vector<int>
#define __mmask16 int

using namespace std;

inline void
print_vec(__m512i v)
{
    cout << "__m512i:";
    for (auto x : v) cout << " " << x;
    cout << endl;
}

inline void
print_mask(__mmask16 m)
{
    cout << "__mmask16:";
    for (int i = 0; i < N; ++i) cout << ((m & (1 << i)) ? 1 : 0);
    cout << endl;
}

inline __m512i
_mm512_loadu_epi32(void* mem_addr)
{
    __m512i r(N);
    for (int i = 0; i < N; ++i) r[i] = static_cast<int*>(mem_addr)[i];
    return r;
}

inline __mmask16
_mm512_cmp_epi32_mask(__m512i a, __m512i b, _MM_CMPINT_ENUM imm8)
{
    __mmask16 m = 0x0;
    switch (imm8)
    {
        case _MM_CMPINT_LE:
            for (int i = 0; i < N; ++i) if (a[i] <= b[i]) m = m | (1 << i);
            break;
        default:
            cout << "Internal error !!!" << endl;
            exit(0);
            break;
    }
    return m;
}

#endif
