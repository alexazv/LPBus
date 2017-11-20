#include "passengercreator.h"
#include <time.h>
#include<iostream>
#include <set>

using namespace std;

PassengerCreator::PassengerCreator()
{

}

void PassengerCreator::createPasengers(Graph * map, int maxPassengers)
{
    int i = 0;
    while(i < maxPassengers){

        int start, target;

        do{
            start = rand() % map->n_stops();
            target = rand() % map->n_stops();
        } while(start == target || start == map->getFinishNode() ||
                target == map->getStartNode() ||
              map->getPassengers(start, true, false).size() > maxPassengers);
        //|| map->distance[start][target] + map->distance[target][map->getFinishNode()] > map->getMaxDistance());

        int count = ((rand() % (int)maxPassengers*0.1+1) + 1);

        count = (count > maxPassengers-i) ? maxPassengers-i : count;

        for(int j = 0; j < count; j++)
            map->addPassenger(start, target);
        //cout << "added " << i+1 << " passengers" << endl;

        i+=count;

    }
}

