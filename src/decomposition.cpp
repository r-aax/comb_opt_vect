#include "decomposition.h"

#include <iostream>

#include "utils.h"

using namespace std;

Decomposition::Decomposition(AreaGraph& g_,
                             int colors_count_)
    : g { g_ },
      nodes_count { g.nodes_count() },
      colors_count { colors_count_ }
{
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

        for (auto ngh : g.inc[node])
        {
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
