#include "decomposition.h"

#include <iostream>
#include <deque>

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

    nodes_colors = new int[nodes_count];
    q = new int[nodes_count];
}

Decomposition::~Decomposition()
{
    delete [] genotype;
    delete [] nodes_colors;
    delete [] q;
}

void
Decomposition::paint_incremental()
{
    for (int i = 0; i < nodes_count; ++i)
    {
        nodes_colors[i] = -1;
    }

    for (int i = 0; i < colors_count; ++i)
    {
        int ni = genotype[i];

        nodes_colors[ni] = i;
        q[i] = ni;
    }

    q_front = 0;
    q_back = colors_count - 1;

    while (q_front <= q_back)
    {
        int node = q[q_front++];
        int color = nodes_colors[node];

        for (auto ngh : g.inc[node])
        {
            if (nodes_colors[ngh] == -1)
            {
                nodes_colors[ngh] = color;
                q[q_back + 1] = ngh;
                q_back++;
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

        cout << " " << nodes_colors[i];
    }

    cout << endl;
}
