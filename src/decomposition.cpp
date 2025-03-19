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
    inc_off = new int[nodes_count];
    for (int i = 0; i < nodes_count; ++i)
    {
        inc_off[i] = inc[i] - inc[0];
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
    delete [] inc_off;
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
    __m512i vc = _mm512_set_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    __m512i vqoff = _mm512_set_epi32(q[15] - q[0], q[14] - q[0], q[13] - q[0], q[12] - q[0],
                                     q[11] - q[0], q[10] - q[0], q[9] - q[0],  q[8] - q[0],
                                     q[7] - q[0],  q[6] - q[0],  q[5] - q[0],  q[4] - q[0],
                                     q[3] - q[0],  q[2] - q[0],  q[1] - q[0],  0);
    __m512i vf = _mm512_loadu_epi32(front);
    __m512i vb = _mm512_loadu_epi32(back);

    while (true)
    {
        __mmask16 is_q = _mm512_cmp_epi32_mask(vf, vb, _MM_CMPINT_LE);

        if (!is_q)
        {
            break;
        }

        __m512i voff = _mm512_mask_add_epi32(v0, is_q, vqoff, vf);
        __m512i vn = _mm512_mask_i32gather_epi32(v0, is_q, voff, q[0], 1);
        __m512i vd = _mm512_mask_i32gather_epi32(v0, is_q, vn, domains, 1);
        __mmask16 is_no_color = _mm512_cmp_epi32_mask(vd, v0, _MM_CMPINT_LT);

        cout << endl << "iter" << endl;
        print_mask(is_q);
        cout << "vf : ";
        print_vec(vf);
        cout << "voff : ";
        print_vec(voff);
        cout << "vn : ";
        print_vec(vn);
        cout << "vd : ";
        print_vec(vd);
        print_mask(is_no_color);

        __m512i vinc_off = _mm512_mask_i32gather_epi32(v0, is_no_color, vn, inc_off, 1);
        cout << "vinc_off : ";
        print_vec(vinc_off);

        _mm512_mask_i32scatter_epi32(domains, is_no_color, vn, vc, 1);

        __m512i vnghs_count = _mm512_mask_i32gather_epi32(v0, is_no_color, vinc_off, inc[0], 1);
        cout << "vnghs_count : ";
        print_vec(vnghs_count);

        __m512i vj = v1;
        __mmask16 is_ngh = _mm512_mask_cmp_epi32_mask(is_no_color, vj, vnghs_count, _MM_CMPINT_LE);
        cout << "is_ngh : ";
        print_mask(is_ngh);
        int j = 1;

        while (is_ngh)
        {
            __m512i v_loc_off = _mm512_add_epi32(vinc_off, vj);
            __m512i vngh = _mm512_mask_i32gather_epi32(v0, is_ngh, v_loc_off, inc[0], 1);

            vb = _mm512_mask_add_epi32(vb, is_ngh, vb, v1);

            __m512i voff2 = _mm512_mask_add_epi32(v0, is_ngh, vqoff, vb);

            _mm512_mask_i32scatter_epi32(q[0], is_ngh, voff2, vngh, 1);
            //_mm512_mask_i32gather_epi32(v0, is_q, voff, q[0], 1);


            //for (int c = 0; c < colors_count; ++c)
            //{
              //  if (is_ngh & (1 << c))
                //{
                  //  int ngh = vngh[c];//inc[vn[c]][j];

                    //q[c][vb[c]] = ngh;
                //}
            //}

            vj = _mm512_add_epi32(vj, v1);
            ++j;
            is_ngh = _mm512_mask_cmp_epi32_mask(is_no_color, vj, vnghs_count, _MM_CMPINT_LE);
            cout << "is_ngh : ";
            print_mask(is_ngh);
        }

        vf = _mm512_mask_add_epi32(vf, is_q, vf, v1);
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
