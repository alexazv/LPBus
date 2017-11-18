#ifndef ROUTETEST_H
#define ROUTETEST_H

#include <set>
#include <stack>
#include "graph.h"

class routeTest
{
public:
    routeTest();
    void AllPaths(Graph G, int s, int t);
    void enumerate(Graph G, int v, int t);
    int combinations(Graph G, int max);
    int routes;
private:
    std::stack<int> path;   // the current path
    std::set<int> onPath;     // the set of vertices on the path
};

#endif // ROUTETEST_H
