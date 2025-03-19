#include "area_graph.h"
#include "decomposition.h"

int main()
{
    int side = 1000;
    AreaGraph area_graph;
    area_graph.init_from_rectangular_area(side, side);

    Decomposition decomposition(area_graph, 16);
    decomposition.paint_incremental();
    //decomposition.print(side);

    return 0;
}
