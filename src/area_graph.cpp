#include "area_graph.h"

#include <iostream>

AreaGraph::AreaGraph()
{
}

void
AreaGraph::link_two_nodes(int a,
                          int b)
{
    inc[a].push_back(b);
    inc[b].push_back(a);
    es.push_back({ a, b });
}

void
AreaGraph::init_from_rectangular_area(int rows,
                                      int cols)
{
    inc.clear();
    inc.resize(rows * cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int ind = i * cols + j;

            if (i + 1 < rows)
            {
                link_two_nodes(ind, ind + cols);
            }

            if (j + 1 < cols)
            {
                link_two_nodes(ind, ind + 1);
            }
        }
    }
}

void
AreaGraph::print()
{
    cout << "G (" << nodes_count() << " nodes, " << edges_count() << " edges):" << endl;

    for (int i = 0; i < inc.size(); ++i)
    {
        cout << i << ":";

        for (auto x : inc[i])
        {
            cout << " " << x;
        }

        cout << endl;
    }

    cout << "ES:";

    for (auto& e : es)
    {
        cout << " [" << e[0] << ", " << e[1] << "]";
    }

    cout << endl;
}
