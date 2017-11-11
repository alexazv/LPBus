#include <QCoreApplication>
#include "map.h"
#include "PassengerCreator.h"
#include "router.h"

int main()
{

    Map map;
    PassengerCreator creator;
    creator.createPasengers(&map, 0);
    Router router(map, 0, 0, 0);
    router.buildRoute();

}
