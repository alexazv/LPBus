#include <QCoreApplication>
#include "map.h"
#include "PassengerCreator.h"

int main()
{
    int nPassengers; //define
    Map map;
    PassengerCreator creator;

    creator.createPasengers(&map, nPassengers);


}
