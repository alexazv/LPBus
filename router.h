#ifndef ROUTER_H
#define ROUTER_H
#include "router.h"
#include "graph.h"

class Router
{
public:
    Router(Graph graph, double cost, double passengerFee, double maxDistance);
    void buildRoute();
    int chooseStop(std::vector<std::pair<int, double>> routes);
private:
    Graph graph;
    double cost, passengerFee, maxDistance;
};

#endif // ROUTER_H
