#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "area_graph.h"

class Decomposition
{

public:

    AreaGraph& g;
    int nodes_count { 0 };
    int colors_count { 0 };
    int* genotype;
    int* nodes_colors;

    Decomposition(AreaGraph& g_,
                  int colors_count_);

    ~Decomposition();

    void
    paint_incremental();

    void
    print(int count_in_row);
};

#endif
