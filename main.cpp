#include <QCoreApplication>
#include "graph.h"
#include "PassengerCreator.h"
#include "router.h"
#include <QFileinfo>
#include <iostream>
#include "mainwindow.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{

/*
    Graph graph("..//LPBUS//maps//map-stanford-small.txt"); //relative path to build
    graph.setMaxDistance(500); //arbitrary
    graph.setStartNode(19);
    graph.setFinishNode(35);
    graph.calculateDistances();

    PassengerCreator creator;
    creator.createPasengers(&graph, 50);

    Router router(&graph, 5.0, 3.5, 10.0, 5);
    //Route path = router.findPath(std::vector<int>{graph.getStartNode(), 0, 12, 29, graph.getFinishNode()});
    //router.chooseRoute(std::vector<Route>{path});
    router.buildAllRoutes();

    //routeTest routes;*/


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    return 0;
}
