#ifndef ROUTE_H
#define ROUTE_H
#include <vector>

class Route
{
public:
    Route();
    Route(std::vector<int> route, double totalDistance);
    std::vector<int> route;
    double totalDistance;
};

#endif // ROUTE_H
