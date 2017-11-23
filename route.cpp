#include "route.h"
#include <vector>
#include <string>

Route::Route(){
    this->totalDistance = 0;
    this->heuristic = INT_MIN;
    this->plan = std::vector<std::string>();
    this->plan_values = std::vector<double>();
}

Route::Route(std::vector<int> route, double totalDistance)
{
    this->path = route;
    this->totalDistance = totalDistance;
    this->plan = std::vector<std::string>();
    this->plan_values = std::vector<double>();
    this->heuristic = INT_MIN;
}
