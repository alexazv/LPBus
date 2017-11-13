#include "graph.h"
#include <map>
#include<iostream>
#include<fstream>
#include <string>
#include <sstream>

using namespace std;

Graph::Graph()
{

}

Graph::Graph(const char * filename){

    string line;

    ifstream infile;
    infile.open(filename);

    if(!infile){
        cout << "failure" << endl;
        return;
    }

    while(line != "VERTICES" && !infile.eof()){
        getline(infile, line);
    }

    if(!infile.eof()){

        int i = 0;
        std::vector<string> tokens;// = split(line);
        std::map<string, int> map;

        while(line != "EDGES" && !infile.eof()){

            getline(infile, line);
            //VERTICES
            tokens = split(line, ';');

            map.insert(std::pair<string,int>(tokens[0], i));
            addStop();
            i++;
        }

        getline(infile, line);
        while(!infile.eof()){
            //EDGES
            tokens = split(line, ';');
            string a3 = tokens[2];
            addRoute(map.at(tokens[0]), map.at(tokens[1]), std::stod(tokens[2]));
            getline(infile, line);
        }

    }

    infile.close();
    system("pause");

}

std::vector<string> Graph::split(string line, char delim){

    std::stringstream stream(line);
    std::vector<string> list;

    string segment;
    while(std::getline(stream, segment, delim))
    {
       list.push_back(segment);
    }
    list.push_back(segment);
    return list;
}

std::vector<std::pair<int, int> > Graph::getPassengers(){
    return passengerList;
}

std::vector<std::pair<int, int>> Graph::getPassengers(int stop, bool direction, bool remove){

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
void Graph::addPassenger(int start, int finish){
    std::pair<int, int> passenger(start, finish);
    passengerList.push_back(passenger);
}

int Graph::getStartNode(){
    return startNode;
}
int Graph::getFinishNode(){
    return finishNode;
}
void Graph::setStartNode(int stop){
    startNode = stop;
}
void Graph::setFinishNode(int stop){
    finishNode = stop;
}

int Graph::n_stops(){
    return routes.size();
}

std::vector<std::pair<int, int>> Graph::getRoutes(int stop){
    return routes[stop];
}

void Graph::addStop(){
    vector<std::pair<int, int>> stop;
    routes.push_back(stop);
}
void Graph::addRoute(int start, int finish, int distance){
    std::pair<int, int> route(finish, distance);
    routes[start].push_back(route);
}
