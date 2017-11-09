#ifndef MAP_H
#define MAP_H

#include "QList"
#include <list>

class Map
{
public:
    Map();
private:
    std::list<std::list<int>> map;
    std::list<std::pair> passengers;
};

#endif // MAP_H
