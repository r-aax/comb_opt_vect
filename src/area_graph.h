#ifndef AREA_GRAPH_H
#define AREA_GRAPH_H

#include <vector>

using namespace std;

class AreaGraph
{

public:

    vector<vector<int>> inc;
    vector<vector<int>> es;

    AreaGraph();

    inline int
    nodes_count() const
    {
        return inc.size();
    }

    inline int
    edges_count() const
    {
        return es.size();
    }

    void
    link_two_nodes(int a,
                   int b);

    void
    init_from_rectangular_area(int rows,
                               int cols);

    void
    print();
};

#endif
