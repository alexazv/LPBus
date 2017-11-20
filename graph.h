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
    //direction - >true - going up; false - going down
    std::vector<std::pair<int, int> > getPassengers(int stop, bool direction, bool remove);
    void removePassengers(int start, int target, int number);
    int n_passengers();
    int n_passengers(int start, int target);
    void addPassenger(int start, int finish);
    void trimStop(int stop);
    int getStartNode();
    int getFinishNode();
    void setStartNode(int stop);
    void setFinishNode(int stop);
    void setMaxDistance(double max);
    double getMaxDistance();
    int n_stops();
    //std::vector<std::vector<std::pair<int, double>>> getRoutes();
    std::vector<std::pair<int, double> > getRoutes(int stop);

    void calculateDistances();

    std::pair<int, int> imageSize;
    std::vector<std::vector<double>> distance;
    std::vector<std::pair<int, int>> coord;
    std::vector<std::pair<int, int>> passengerList; //pair<start, finish>
    std::vector<string> split(string line, char delim);
private:
    void addStop();
    void addRoute(int start, int finish, double distance);



    double maxDistance;
    int startNode;
    int finishNode;
    std::vector<std::vector<std::pair<int, double>>> routes; //pair <destination, value>

};

#endif // MAP_H
