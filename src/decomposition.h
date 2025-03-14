#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "area_graph.h"

class Decomposition
{

public:

    // graph
    int nodes_count { 0 };
    int edges_count { 0 };
    int** inc;
    int** es;

    // painting
    int colors_count { 0 };
    int* genotype;
    int* domains;

    // queue
    int* q;
    int front { -1 };
    int back { -1 };

    Decomposition(AreaGraph& g_,
                  int colors_count_);

    ~Decomposition();

    void
    paint_incremental();

    void
    print(int count_in_row);
};

#endif
