#ifndef GRAPH_H
#define GRAPH_H

#include "QList"
#include <vector>
#include <utility>

typedef std::string string;

class Graph
{
public:
    Graph();
    Graph(const char *filename);
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
    std::vector<std::pair<int, int>> getRoutes(int stop); //change distance to double

    void addStop();
    void addRoute(int start, int finish, int distance);


private:
    int startNode;
    int finishNode;
    std::vector<std::list<int>> distance;
    std::vector<std::vector<std::pair<int, int>>> routes; //pair <destination, value>
    std::vector<std::pair<int, int>> passengerList; //pair<start, finish>
    std::vector<string> split(string line, char delim);
};

#endif // MAP_H
