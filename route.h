#ifndef ROUTE_H
#define ROUTE_H
#include <vector>
#include <string>

class Route
{
public:
    Route();
    Route(std::vector<int> path, double totalDistance);
    std::vector<int> path;
    double totalDistance;
    std::vector<std::string> plan;
    std::vector<double> plan_values;
    double heuristic;
};

#endif // ROUTE_H
