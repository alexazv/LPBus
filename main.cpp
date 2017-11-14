#include <QCoreApplication>
#include "graph.h"
#include "PassengerCreator.h"
#include "router.h"
#include <QFileinfo>

int main()
{
    Graph graph("..//LPBUS//maps//map-stanford-small.txt"); //relative path to build
    graph.setMaxDistance(500); //arbitrary
    graph.setStartNode(0);
    graph.setFinishNode(graph.n_stops()-1);
    graph.calculateDistances();
    graph.calculateDistances();

    PassengerCreator creator;
    creator.createPasengers(&graph, 50);
    /*Router router(map, 0, 0, 0);
    router.buildRoute();*/

}
