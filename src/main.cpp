#include "area_graph.h"
#include "decomposition.h"

#include <iostream>

#include "avx.h"

using namespace std;

void
calculate_speedup(int side)
{
    AreaGraph area_graph;
    area_graph.init_from_rectangular_area(side, side);

    Decomposition decomposition(area_graph, 16);

    double no_opt = decomposition.paint_incremental_no_opt();
    int cnt = 1;

    while (no_opt < 0.1)
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

void
collect_statistics(int side)
{
    AreaGraph area_graph;
    area_graph.init_from_rectangular_area(side, side);

    Decomposition decomposition(area_graph, 16);

    zero_stat();
    decomposition.paint_incremental_opt();

    cout << "side = " << side
         << ", ath = " << (static_cast<double>(ath_exe) / ath_cnt)
         << ", cmp = " << (static_cast<double>(cmp_exe) / cmp_cnt)
         << ", gth = " << (static_cast<double>(gth_exe) / gth_cnt)
         << ", sct = " << (static_cast<double>(sct_exe) / sct_cnt)
         << endl;
}

int
main(int argc, char** argv)
{
    int start = (argc == 1) ? 20 : atoi(argv[1]);
    int stop { 2000 };

    srand(static_cast<unsigned int>(time({})));

    cout << "start = " << start << endl;

#if 0

    // Run on real machine to measure speedup.

    for (int side = start; side <= stop; side += 20)
    {
        calculate_speedup(side);
    }

#endif

#if 1

    // Run in simulation mode for collec masks density statistics.

    for (int side = start; side <= stop; side += 20)
    {
        collect_statistics(side);
    }

#endif

    return 0;
}
