#include "area_graph.h"

#include <iostream>

/// <summary>
/// 
/// </summary>
AreaGraph::AreaGraph()
{
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
void
AreaGraph::link_two_nodes(size_t a,
                          size_t b)
{
    inc[a].push_back(b);
    inc[b].push_back(a);
    es.push_back({ a, b });
}

/// <summary>
/// 
/// </summary>
/// <param name="rows"></param>
/// <param name="cols"></param>
void
AreaGraph::init_from_rectangular_area(size_t rows,
                                      size_t cols)
{
    inc.clear();
    inc.resize(rows * cols);

    for (auto i{ 0 }; i < rows; ++i)
    {
        for (auto j{ 0 }; j < cols; ++j)
        {
            size_t ind { i * cols + j };

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

/// <summary>
/// 
/// </summary>
void
AreaGraph::print()
{
    cout << "G (" << nodes_count() << " nodes, " << edges_count() << " edges):" << endl;

    for (int i { 0 }; i < inc.size(); ++i)
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
