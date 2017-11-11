#ifndef ROUTER_H
#define ROUTER_H
#include "router.h"
#include "map.h"

class Router
{
public:
    Router(Map map, double cost, double passengerFee, double maxDistance);
    void buildRoute();
    int chooseStop(std::vector<std::pair<int, int>> routes);
private:
    Map map;
    double cost, passengerFee, maxDistance;
};

#endif // ROUTER_H
