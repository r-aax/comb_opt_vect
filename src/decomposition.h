#pragma once

#include <string>

#include "area_graph.h"

/// <summary>
/// 
/// </summary>
class Decomposition
{

public:

    /// <summary>
    /// 
    /// </summary>
    AreaGraph& g;

    /// <summary>
    /// 
    /// </summary>
    size_t colors_count { 0 };

    /// <summary>
    /// 
    /// </summary>
    vector<size_t> genotype;

    /// <summary>
    /// 
    /// </summary>
    vector<int> nodes_colors;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="g_"></param>
    /// <param name="colors_count_"></param>
    Decomposition(AreaGraph& g_,
                  size_t colors_count_);

    /// <summary>
    /// 
    /// </summary>
    inline void birth()
    {
        paint_from_genome();
    }

    /// <summary>
    /// 
    /// </summary>
    inline void
    reset_nodes_colors()
    {
        for (auto& x : nodes_colors)
        {
            x = -1;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    void
    paint_incremental();

    /// <summary>
    /// 
    /// </summary>
    void
    paint_from_genome();

    /// <summary>
    /// 
    /// </summary>
    /// <param name="count_in_row"></param>
    void
    print(size_t count_in_row);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <param name="side"></param>
    void
    paint(string filename,
          size_t side);
};
