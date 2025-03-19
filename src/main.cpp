#include "area_graph.h"
#include "decomposition.h"

#include <iostream>

using namespace std;

void
check_side(int side)
{
    AreaGraph area_graph;
    area_graph.init_from_rectangular_area(side, side);

    Decomposition decomposition(area_graph, 16);

    double no_opt = decomposition.paint_incremental_no_opt();
    int cnt = 1;

    while (no_opt < 0.01)
    {
        no_opt += decomposition.paint_incremental_no_opt();
        ++cnt;
    }

    double opt = 0.0;

    for (int i = 0; i < cnt; ++i)
    {
        opt += decomposition.paint_incremental_opt();
    }

    cout << "side = " << side << ", cnt = " << cnt
         << ", no_opt = " << no_opt << ", opt = " << opt
         << ", speedup = " << (no_opt / opt) << endl;
}

int
main()
{
    srand(static_cast<unsigned int>(time({})));

    for (int side = 20; side <= 1000; side += 20)
    {
        check_side(side);
    }

    return 0;
}
