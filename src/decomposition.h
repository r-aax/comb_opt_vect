#pragma once

#include <string>

#include "area_graph.h"

class Decomposition
{

public:

    AreaGraph& g;
    size_t nodes_count { 0 };
    size_t colors_count { 0 };
    vector<size_t> genotype;
    vector<int> nodes_colors;

    Decomposition(AreaGraph& g_,
                  size_t colors_count_);

    void
    paint_incremental();

    void
    print(size_t count_in_row);
};
