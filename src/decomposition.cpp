#include "decomposition.h"

#include <iostream>

#include "utils.h"
#include "avx.h"

using namespace std;

Decomposition::Decomposition(AreaGraph& g,
                             int colors_count_)
    : nodes_count { g.nodes_count() },
      colors_count { colors_count_ }
{
    // init graph
    inc = new int*[nodes_count];
    for (int i = 0; i < nodes_count; ++i)
    {
        inc[i] = new int[g.inc[i].size() + 1];
        inc[i][0] = g.inc[i].size();
        for (int j = 0; j < g.inc[i].size(); ++j)
        {
            inc[i][j + 1] = g.inc[i][j];
        }
    }
    incoff = new int[nodes_count];
    for (int i = 0; i < nodes_count; ++i)
    {
        incoff[i] = inc[i] - inc[0];
    }
    es = new int*[edges_count];
    for (int i = 0; i < edges_count; ++i)
    {
        es[i] = new int[2];
        es[i][0] = g.es[i][0];
        es[i][1] = g.es[i][1];
    }

    // painting
    genotype = new int[colors_count];
    for (int i = 0; i < colors_count; ++i)
    {
        int r = randint(static_cast<int>(nodes_count));
        genotype[i] = r;
    }
    domains = new int[nodes_count];

    // queue
    q = new int*[colors_count];
    for (int i = 0; i < colors_count; ++i)
    {
        q[i] = new int[nodes_count];
    }
    qoff = new int[colors_count];
    for (int i = 0; i < colors_count; ++i)
    {
        qoff[i] = q[i] - q[0];
    }
    front = new int[colors_count];
    back = new int[colors_count];
}

Decomposition::~Decomposition()
{
    // graph
    for (int i = 0; i < nodes_count; ++i)
    {
        delete [] inc[i];
    }
    delete [] inc;
    delete [] incoff;
    for (int i = 0; i < edges_count; ++i)
    {
        delete [] es[i];
    }
    delete [] es;

    // painting
    delete [] genotype;
    delete [] domains;

    // queue
    for (int i = 0; i < colors_count; ++i)
    {
        delete [] q[i];
    }
    delete [] q;
    delete [] qoff;
    delete [] front;
    delete [] back;
}

void
Decomposition::paint_incremental()
{
    for (int n = 0; n < nodes_count; ++n)
    {
        domains[n] = -1;
    }

    for (int c = 0; c < colors_count; ++c)
    {
        int n = genotype[c];
        q[c][0] = n;
        front[c] = 0;
        back[c] = 0;
    }

#if 1

    // Not optimized.

    bool cont = true;

    while (cont)
    {
        cont = false;

        for (int c = 0; c < colors_count; ++c)
        {
            if (front[c] <= back[c])
            {
                cont = true;

                int n = q[c][front[c]];

                front[c]++;

                if (domains[n] == -1)
                {
                    int nghs_count = inc[n][0];

                    domains[n] = c;

                    for (int i = 0; i < nghs_count; ++i)
                    {
                        int ngh = inc[n][i + 1];

                        back[c]++;
                        q[c][back[c]] = ngh;
                    }
                }
            }
        }
    }

#else

#define SET1(I) _mm512_set1_epi32(I)
#define SET(I15, I14, I13, I12, I11, I10, I9, I8, I7, I6, I5, I4, I3, I2, I1, I0) _mm512_set_epi32(I15, I14, I13, I12, I11, I10, I9, I8, I7, I6, I5, I4, I3, I2, I1, I0)
#define LD(A) _mm512_loadu_epi32(A)
#define CMPLE(M, A, B) _mm512_mask_cmp_epi32_mask(M, A, B, _MM_CMPINT_LE)
#define CMPLT(M, A, B) _mm512_mask_cmp_epi32_mask(M, A, B, _MM_CMPINT_LT)
#define ADD(S, M, A, B) _mm512_mask_add_epi32(S, M, A, B)
#define GTH(S, M, OFF, A) _mm512_mask_i32gather_epi32(S, M, OFF, A, 1)
#define GTH2(S, M, OFF1, OFF2, A) GTH(S, M, ADD(S, M, OFF1, OFF2), A)
#define SCT(A, M, OFF, V) _mm512_mask_i32scatter_epi32(A, M, OFF, V, 1)
#define SCT2(A, M, OFF1, OFF2, V) SCT(A, M, ADD(v0, M, OFF1, OFF2), V)

    // Optimized.

    // locals
    __m512i vn = SET1(0);
    __m512i vd = SET1(0);
    __m512i vj = SET1(0);
    __m512i vcnt = SET1(0);
    __m512i vngh = SET1(0);
    __m512i vincoff = SET1(0);
    __mmask16 is_no_color;
    __mmask16 is_ngh;

    __m512i v0 = SET1(0);
    __m512i v1 = SET1(1);
    __m512i vc = SET(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    __m512i vf = LD(front);
    __m512i vb = LD(back);
    __m512i vqoff = LD(qoff);
    __mmask16 is_q = CMPLE(0xFFFF, vf, vb);

    while (is_q)
    {
        vn = GTH2(v0, is_q, vqoff, vf, q[0]);
        vd = GTH(v0, is_q, vn, domains);
        is_no_color = CMPLT(is_q, vd, v0);

        if (is_no_color)
        {
            SCT(domains, is_no_color, vn, vc);
            vincoff = GTH(v0, is_no_color, vn, incoff);
            vcnt = GTH(v0, is_no_color, vincoff, inc[0]);
            vj = v1;
            is_ngh = CMPLE(is_no_color, vj, vcnt);

            while (is_ngh)
            {
                vngh = GTH2(v0, is_ngh, vincoff, vj, inc[0]);
                SCT2(q[0], is_ngh, vqoff, vb, vngh);
                vb = ADD(vb, is_ngh, vb, v1);
                vj = ADD(v0, is_ngh, vj, v1);
                is_ngh = CMPLE(is_ngh, vj, vcnt);
            }
        }

        vf = ADD(v0, is_q, vf, v1);
        is_q = CMPLE(is_q, vf, vb);
    }

#endif

}

void
Decomposition::print(int count_in_row)
{
    for (int i = 0; i < nodes_count; ++i)
    {
        if ((i % count_in_row) == 0)
        {
            cout << endl;
        }

        if (domains[i] < 0)
        {
            cout << " " << '.';
        }
        else
        {
            cout << " " << std::hex << domains[i];
        }
    }

    cout << endl;
}
