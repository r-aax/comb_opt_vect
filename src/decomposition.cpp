#include "decomposition.h"

#include <iostream>
#include <algorithm>
#include <deque>

#include "utils.h"

using namespace std;

Decomposition::Decomposition(AreaGraph& g_,
                             int colors_count_)
    : g { g_ },
      nodes_count { g.nodes_count() },
      colors_count { colors_count_ },
      genotype(colors_count),
      nodes_colors(nodes_count)
{
    for (int i = 0; i < colors_count; ++i)
    {
        int r = randint(static_cast<int>(nodes_count));

        genotype[i] = r;
    }
}

void
Decomposition::paint_incremental()
{
    for (int i = 0; i < nodes_count; ++i)
    {
        nodes_colors[i] = -1;
    }

    deque<int> q;

    for (int i = 0; i < colors_count; ++i)
    {
        int ni = genotype[i];

        nodes_colors[ni] = i;
        q.push_back(ni);
    }

    while (!q.empty())
    {
        int node = q.front();
        int color = nodes_colors[node];

        q.pop_front();

        for (auto ngh : g.inc[node])
        {
            if (nodes_colors[ngh] == -1)
            {
                nodes_colors[ngh] = color;
                q.push_back(ngh);
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
