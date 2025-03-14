#include "decomposition.h"

#include <iostream>
#include <algorithm>
#include <deque>

#include "utils.h"

using namespace std;

/// <summary>
/// 
/// </summary>
/// <param name="g_"></param>
/// <param name="colors_count_"></param>
Decomposition::Decomposition(AreaGraph& g_,
                             size_t colors_count_)
    : g{ g_ },
      colors_count{ colors_count_ },
      nodes_colors(g.nodes_count())
{
    genotype.clear();

    for (int i{ 0 }; i < colors_count; ++i)
    {
        int r { randint(static_cast<int>(g.nodes_count())) };

        genotype.push_back(r);
    }

    birth();
}

/// <summary>
/// 
/// </summary>
void
Decomposition::paint_incremental()
{
    deque<size_t> q;

    // put into deque data from genome
    for (int i{ 0 }; i < genotype.size(); ++i)
    {
        size_t ni{ genotype[i] };

        nodes_colors[ni] = i;
        q.push_back(ni);
    }

    // try to growth
    while (!q.empty())
    {
        size_t node{ q.front() };
        int color{ nodes_colors[node] };

        q.pop_front();

        // check all neighbours
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

/// <summary>
/// 
/// </summary>
void
Decomposition::paint_from_genome()
{
    reset_nodes_colors();
    paint_incremental();
}

/// <summary>
/// 
/// </summary>
/// <param name="count_in_row"></param>
void
Decomposition::print(size_t count_in_row)
{
    for (int i { 0 }; i < nodes_colors.size(); ++i)
    {
        if ((i % count_in_row) == 0)
        {
            cout << endl;
        }

        cout << " " << nodes_colors[i];
    }

    cout << endl;
}
