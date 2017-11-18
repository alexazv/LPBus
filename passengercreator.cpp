#include "passengercreator.h"
#include <time.h>
#include<iostream>
#include <set>

using namespace std;

PassengerCreator::PassengerCreator()
{

}

void PassengerCreator::createPasengers(Graph * map, int nPassengers)
{

    std::vector<std::pair<int, int>> rules;

    int i = 0;
    int maxPassengers = 50; //change -> arbitrary
    while(i < nPassengers){

        int start, target;

        do{
            start = rand() % map->n_stops();
            target = rand() % map->n_stops();
        } while(start == target || start == map->getFinishNode() ||
                target == map->getStartNode() ||
              map->getPassengers(start, true, false).size() > maxPassengers ||
                map->distance[start][target] + map->distance[target][map->getFinishNode()] > map->getMaxDistance());

        int count = ((rand() % (int)maxPassengers*0.1+1) + 1);

        count = (count > maxPassengers-i) ? maxPassengers-i : count;

        for(int j = 0; j < count; j++)
            map->addPassenger(start, target);
        //cout << "added " << i+1 << " passengers" << endl;

        i+=count;

        bool found = false;
        for(int j = 0; j < rules.size(); j++){
            if(rules[i].first == start && rules[i].second == target){
                found = true;
                break;
            }
        }
        if(!found)
            rules.push_back(std::pair<int,int>(start, target));
    }

    i = rules.size();

    /*for(int i = 0; i < map->n_stops(); i++){
        cout << "In stop " << i << endl << '\t';
        for(int j = 0; j < map->getPassengers(i, true, false).size(); j++){
            cout << i << "->" << map->getPassengers(i, true, false)[j].second << ",";
        }

        cout << endl;
    }*/

}

