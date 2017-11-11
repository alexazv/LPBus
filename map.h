#ifndef MAP_H
#define MAP_H

#include "QList"
#include <vector>
#include <utility>

class Map
{
public:
    Map();
    std::vector<std::pair<int, int> > getPassengers();
    std::vector<std::pair<int, int> > getPassengers(int stop, bool direction, bool remove);
    void addPassenger(int start, int finish);
    void trimStop(int stop);
    int getStartNode();
    int getFinishNode();
    void setStartNode(int stop);
    void setFinishNode(int stop);
    int n_stops();
    //std::vector<std::vector<std::pair<int, int>>> getRoutes();
    std::vector<std::pair<int, int>> getRoutes(int stop);


private:
    int startNode;
    int finishNode;
    std::vector<std::list<int>> distance;
    std::vector<std::vector<std::pair<int, int>>> routes; //pair <destination, value>
    std::vector<std::pair<int, int>> passengerList; //pair<start, finish>
};

#endif // MAP_H
