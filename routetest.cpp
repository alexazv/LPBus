#include "routetest.h"
#include <iostream>
#include <chrono>
using namespace std;

routeTest::routeTest()
{
    routes = 0;
}

void routeTest::AllPaths(Graph G, int s, int t) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    enumerate(G, s, t);

        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();

        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
}

    // use DFS
void routeTest::enumerate(Graph G, int v, int t) {

        //todo: fix cycles

        // add node v to current path from s
        path.push(v);
        onPath.insert(v);

        // found path from s to t - currently prints in reverse order because of stack
        if (v == t){
            routes++;

            /*std::stack<int> printStack = path;

            for(int i = 0; i <= printStack.size(); i++){
                cout << printStack.top() << ",";
                printStack.pop();
            }*/

            cout << "route n " << routes << " found: " << path.size() << " nodes" << endl;
        }

        // consider all neighbors that would continue path with repeating a node
        else {
            std::vector<std::pair<int, double>> routes = G.getRoutes(v);
            for(int i = 0; i < routes.size(); i++)
                if (onPath.find(routes[i].first) == onPath.end())
                    enumerate(G, routes[i].first, t);
            }


        // done exploring from v, so remove from path
        path.pop();
        onPath.erase(v);
}

int routeTest::combinations(Graph G, int max){
    std::vector<int> critical;

    for(int i = 0; i < G.n_stops(); i++){
        if(G.getPassengers(i, true, false).size() + G.getPassengers(i, false, false).size() != 0)
            critical.push_back(i);
    }

    int paths = 1;
    for(int i = 0; i < max; i++){
        paths *= (critical.size()-2-i);
    }

    return paths;

}
