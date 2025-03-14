#ifndef UTILS_H
#define UTILS_H

#include <random>

using namespace std;

/// <summary>
/// 
/// </summary>
/// <returns></returns>
inline int randint()
{
    return abs(rand() ^ (rand() << 16));
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
inline int randint(int a,
                   int b)
{
    return a + randint() % (b - a + 1);
}

/// <summary>
/// 
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
inline int randint(int n)
{
    return randint(0, n - 1);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
inline bool randbool()
{
    return randint(0, 1) == 0;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
inline double rand01()
{
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

/// <summary>
///
/// </summary>
/// <param name="prob"></param>
/// <returns></returns>
inline bool randbool(double prob)
{
    return rand01() < prob;
}

#endif
