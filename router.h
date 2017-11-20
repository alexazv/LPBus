#ifndef ROUTER_H
#define ROUTER_H
#include "router.h"
#include "graph.h"
#include "route.h"

class Router
{
public:
    Router();
    Router(Graph * graph, double cost, double passengerFee, double maxDistance, int maxCapacity);
    void buildRoute();
    void buildRoute(int size, Route solution, int xChosen);
    int chooseStop(std::vector<std::pair<int, double>> routes);
    void buildAllRoutes();
    Route chooseRoute(std::vector<Route> routes);
    Route findPath(std::vector<int>nodes);
    std::vector<Route> routes;
    string makeTrip(Route route);
    double cost, passengerFee, maxDistance;
    int maxCapacity;
private:
    Graph * graph;
    void buildRules();
    bool contains(std::vector<int> list, int element);
    bool contains(std::vector<bool> list, bool element);
    std::map<int, std::vector<int>> rules;
    std::vector<int> getYs(int x);
    std::vector<int> getXs();



    int minIndex(std::vector<double> list);
};

#endif // ROUTER_H
