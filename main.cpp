#include <QCoreApplication>
#include "graph.h"
#include "PassengerCreator.h"
#include "router.h"
#include <QFileinfo>

int main()
{
    QFileInfo fi("map-stanford-small.txt");
    //Graph graph("C:\\Users\\Alexandre\\Documents\\LPBus\\map-stanford-small.txt");
    Graph graph("..//LPBUS//maps//map-stanford-small.txt"); //relative path to build

    /*PassengerCreator creator;
    creator.createPasengers(&map, 0);
    Router router(map, 0, 0, 0);
    router.buildRoute();*/

}
