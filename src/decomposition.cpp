#include "decomposition.h"

#include <iostream>

#include "utils.h"

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

    genotype = new int[colors_count];

    for (int i = 0; i < colors_count; ++i)
    {
        int r = randint(static_cast<int>(nodes_count));

        genotype[i] = r;
    }

    domains = new int[nodes_count];
    q = new int[nodes_count];
}

Decomposition::~Decomposition()
{
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

    delete [] genotype;
    delete [] domains;
    delete [] q;
}

void
Decomposition::paint_incremental()
{
    for (int i = 0; i < nodes_count; ++i)
    {
        domains[i] = -1;
    }

    for (int i = 0; i < colors_count; ++i)
    {
        int ni = genotype[i];

        domains[ni] = i;
        q[i] = ni;
    }

    front = 0;
    back = colors_count - 1;

    while (front <= back)
    {
        int node = q[front];
        front++;

        int color = domains[node];

        for (int i = 0; i < inc[node][0]; ++i)
        {
            int ngh = inc[node][i + 1];

            if (domains[ngh] == -1)
            {
                domains[ngh] = color;
                back++;
                q[back] = ngh;
            }
        }
    }
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

        cout << " " << std::hex << domains[i];
    }

    cout << endl;
}
