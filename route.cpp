#include "route.h"
#include <vector>

Route::Route(){
    this->totalDistance = 0;
}

Route::Route(std::vector<int> route, double totalDistance)
{
    this->route = route;
    this->totalDistance = totalDistance;
}