#include "passengercreator.h"
#include <time.h>
#include<iostream>

using namespace std;

PassengerCreator::PassengerCreator()
{

}

void PassengerCreator::createPasengers(Graph * map, int nPassengers)
{
    int i = 0;
    int maxPassengers = 50; //change -> arbitrary
    while(i < nPassengers){

        int start, target;

        do{
            start = rand() % map->n_stops();
            target = rand() % map->n_stops();
        } while(start == map->getFinishNode() ||
                target == map->getStartNode() ||
              map->getPassengers(start, true, false).size() > maxPassengers ||
                map->distance[start][target] + map->distance[target][map->getFinishNode()] > map->getMaxDistance());

        map->addPassenger(start, target);
        cout << "added " << i+1 << " passengers" << endl;
        i++;

    }
}
