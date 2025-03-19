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
    int* incoff;
    int** es;

    // painting
    int colors_count { 0 };
    int* genotype;
    int* domains;

    // queue
    int** q;
    int* qoff;
    int* front;
    int* back;

    Decomposition(AreaGraph& g_,
                  int colors_count_);

    ~Decomposition();

    double
    paint_incremental_no_opt();

    double
    paint_incremental_opt();

    void
    print(int count_in_row);
};

#endif
