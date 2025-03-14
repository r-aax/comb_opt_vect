#include "area_graph.h"
#include "decomposition.h"

int main()
{
    size_t side { 50 };
    AreaGraph area_graph;
    area_graph.init_from_rectangular_area(side, side);

    Decomposition decomposition(area_graph, 10);
    decomposition.print(side);

    return 0;
}
