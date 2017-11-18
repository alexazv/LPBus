#ifndef ROUTER_H
#define ROUTER_H
#include "router.h"
#include "graph.h"
#include "route.h"

class Router
{
public:
    Router(Graph graph, double cost, double passengerFee, double maxDistance, int maxCapacity);
    void buildRoute();
    int chooseStop(std::vector<std::pair<int, double>> routes);
    void buildAllRoutes();
    void chooseRoute(std::vector<Route> routes);
private:
    Graph graph;
    double cost, passengerFee, maxDistance;
    int maxCapacity;
    bool contains(std::vector<int> list, int element);
    std::map<int, std::vector<int>> rules;
    void buildRoute(int size, Route solution, int xChosen);
    void nonRepeatAdd(std::vector<int>* list, int element);
    std::vector<int> getYs(int x);
    std::vector<int> getXs();
};

#endif // ROUTER_H
