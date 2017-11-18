#include "graph.h"
#include <map>
#include<iostream>
#include<fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "lp_lib.h"
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

        getline(infile, line);
        while(line != "EDGES" && !infile.eof()){          
            //VERTICES
            tokens = split(line, ';');

            if(line != ""){
                map.insert(std::pair<string,int>(tokens[0], i));
                addStop();
                i++;
                coord.push_back(std::pair<int, int>(std::stoi(tokens[1]), std::stoi(tokens[2])));
            }

            getline(infile, line);
        }


        //todo: error treatment
        getline(infile, line);
        while(!infile.eof()){
            //EDGES
            tokens = split(line, ';');
            addRoute(map.at(tokens[0]), map.at(tokens[1]), std::stod(tokens[2]));
            addRoute(map.at(tokens[1]), map.at(tokens[0]), std::stod(tokens[2]));
            getline(infile, line);
        }

    }

    infile.close();
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

std::vector<std::pair<int, int> > Graph::getPassengers(int stop, bool direction, bool remove){

    std::vector<std::pair<int, int>> passengers;
    std::vector<int> toErase;
    for(int i = 0; i < passengerList.size(); i++){
        int index = -1;
        if((passengerList[i].first == stop && direction) || (passengerList[i].second == stop && !direction))
            index = i;

        if(index != -1){
            passengers.push_back(passengerList[i]);
            if(remove)
                toErase.push_back(index);
        }
    }

    for(int i = 0; i < toErase.size(); i++)
        passengerList.erase(passengerList.begin() + toErase.at(i));

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

std::vector<std::pair<int, double> > Graph::getRoutes(int stop){
    return routes[stop];
}

void Graph::addStop(){
    vector<std::pair<int, double>> stop;
    routes.push_back(stop);
}
void Graph::addRoute(int start, int finish, double distance){
    std::pair<int, double> route(finish, distance);
    routes[start].push_back(route);
}

void Graph::setMaxDistance(double max){
    maxDistance = max;
}

double Graph::getMaxDistance(){
    return maxDistance;
}

void Graph::calculateDistances(){

    int N = n_stops();

    distance = std::vector<std::vector<double>>(N, std::vector<double>(N, maxDistance));


    for(int i = 0; i< N; i++){
        distance[i][i] = 0;
        std::vector<std::pair<int, double>> routes = getRoutes(i);
        for(int j = 0; j < routes.size(); j++)
                distance[i][routes[j].first] = routes[j].second;

    }


    //Floyd-Warshall algorythm (use more efficient one?)
    for(int k = 0 ; k != N; k++)
        for(int i = 0 ; i != N; i++)
           for(int j = 0 ; j != N; j++)
                distance[i][j] = (distance[i][j] < (distance[i][k] + distance[k][j])) ?
                    distance[i][j] : (distance[i][k] + distance[k][j]);
}

int Graph::n_passengers(){
    return passengerList.size();
}
int Graph::n_passengers(int start, int target){
    int number = 0;

    for(int i = 0; i < passengerList.size(); i++)
        if(passengerList.at(i).first == start && passengerList.at(i).second)
            number++;

    return number;
}
