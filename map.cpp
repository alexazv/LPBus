#include "map.h"

Map::Map()
{

}

std::vector<std::pair<int, int> > Map::getPassengers(){
    return passengerList;
}

std::vector<std::pair<int, int>> Map::getPassengers(int stop, bool direction, bool remove){

    std::vector<std::pair<int, int>> passengers;
    for(int i = 0; i < passengerList.size(); i++){
        int index = -1;
        if((passengerList[i].first == stop && direction) || (passengerList[i].second == stop && !direction))
            index = i;

        if(index != -1){
            passengers.push_back(passengerList[i]);
            if(remove)
                passengerList.erase(passengerList.begin() + index);
        }
    }
    return passengers;
}
void Map::addPassenger(int start, int finish){
    std::pair<int, int> passenger(start, finish);
    passengerList.push_back(passenger);
}

int Map::getStartNode(){
    return startNode;
}
int Map::getFinishNode(){
    return finishNode;
}
void Map::setStartNode(int stop){
    startNode = stop;
}
void Map::setFinishNode(int stop){
    finishNode = stop;
}

int Map::n_stops(){
    return routes.size();
}

std::vector<std::pair<int, int>> Map::getRoutes(int stop){
    return routes[stop];
}
