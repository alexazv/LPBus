#ifndef ROUTER_H
#define ROUTER_H
#include "router.h"
#include "map.h"

class Router
{
public:
    Router();
    void buildRoute(Map*map);
};

#endif // ROUTER_H
