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

#if 0

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

#endif

#if 1

    // Optimized.

    __m512i v0 = _mm512_set1_epi32(0);
    __m512i v1 = _mm512_set1_epi32(1);
    __m512i vqoff = _mm512_set_epi32(q[15] - q[0], q[14] - q[0], q[13] - q[0], q[12] - q[0],
                                     q[11] - q[0], q[10] - q[0], q[9] - q[0],  q[8] - q[0],
                                     q[7] - q[0],  q[6] - q[0],  q[5] - q[0],  q[4] - q[0],
                                     q[3] - q[0],  q[2] - q[0],  q[1] - q[0],  0);

    while (true)
    {
        __m512i vf = _mm512_loadu_epi32(front);
        __m512i vb = _mm512_loadu_epi32(back);
        __mmask16 cont = _mm512_cmp_epi32_mask(vf, vb, _MM_CMPINT_LE);

        if (!cont)
        {
            break;
        }

        __m512i voff = _mm512_mask_add_epi32(v0, cont, vqoff, vf);
        __m512i vn = _mm512_mask_i32gather_epi32(v0, cont, voff, q[0], 1);

        cout << endl << "iter" << endl;
        print_mask(cont);
        cout << "vf : ";
        print_vec(vf);
        cout << "voff : ";
        print_vec(voff);
        cout << "vn : ";
        print_vec(vn);

        for (int c = 0; c < colors_count; ++c)
        {
            if (front[c] <= back[c])
            {
                int n = vn[c];

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

        vf = _mm512_mask_add_epi32(v0, cont, vf, v1);
        _mm512_mask_storeu_epi32(front, cont, vf);
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
