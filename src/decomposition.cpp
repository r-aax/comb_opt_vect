#include "decomposition.h"

#include <iostream>
#include <algorithm>
#include <deque>

#include "utils.h"

using namespace std;

Decomposition::Decomposition(AreaGraph& g_,
                             size_t colors_count_)
    : g { g_ },
      nodes_count { g.nodes_count() },
      colors_count { colors_count_ },
      nodes_colors(nodes_count)
{
    genotype.clear();

    for (size_t i = 0; i < colors_count; ++i)
    {
        int r = randint(static_cast<int>(nodes_count));

        genotype.push_back(r);
    }
}

void
Decomposition::paint_incremental()
{
    for (size_t i = 0; i < nodes_count; ++i)
    {
        nodes_colors[i] = -1;
    }

    deque<size_t> q;

    for (size_t i = 0; i < colors_count; ++i)
    {
        size_t ni = genotype[i];

        nodes_colors[ni] = i;
        q.push_back(ni);
    }

    while (!q.empty())
    {
        size_t node = q.front();
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
Decomposition::print(size_t count_in_row)
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
