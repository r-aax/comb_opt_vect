#pragma once

#include <string>

#include "area_graph.h"

class Decomposition
{

public:

    AreaGraph& g;
    int nodes_count { 0 };
    int colors_count { 0 };
    vector<int> genotype;
    vector<int> nodes_colors;

    Decomposition(AreaGraph& g_,
                  int colors_count_);

    void
    paint_incremental();

    void
    print(int count_in_row);
};
