#include "router.h"
#include <set>
#include "lp_lib.h"
#include "graph.h"

Router::Router(Graph map, double cost, double passengerFee, double maxDistance)
{
    this->graph = map;
    this->cost = cost;
    this->passengerFee = passengerFee;
    this->maxDistance = maxDistance;
}

//todo: add distance constraint
//todo: add stops in waitlist to chooseStop
void Router::buildRoute(){

    std::vector<std::pair<int,int>> passengersInBus;
    std::set<int> waitList; //must-visit nodes
    std::vector<std::vector<int>> busRoutes; //route for each bus

    bool visited[graph.n_stops()];
    memset(visited, false, sizeof(visited));

    while(graph.getPassengers().size() > 0){

        //add start and finish nodes to route
        std::vector<int> busRoute;
        busRoute.push_back(graph.getStartNode());
        busRoute.push_back(graph.getFinishNode());

        int current = graph.getStartNode();

        while(current != graph.getFinishNode()){

            int nextStop = chooseStop(graph.getRoutes(current));

            //get passengers into bus
            std::vector<std::pair<int,int>> passengers = graph.getPassengers(nextStop, true, true);
            for(int i = 0; i < passengers.size(); i++)
                waitList.insert(passengers[i].second);

            passengersInBus.insert(passengersInBus.end(), passengers.begin(), passengers.end());;
            visited[nextStop] = true;

            current = nextStop;
        }

        busRoutes.push_back(busRoute);
    }
}

//todo: add distance constraint
int Router::chooseStop(std::vector<std::pair<int, int> > routes){

    lprec *lp = make_lp(0, routes.size());
    int nrest = 1;

    set_add_rowmode(lp, TRUE);
    double row[1+routes.size()];

    for(int i = 1; i <= routes.size(); i++){
        row[i] = (routes[i-1].second)*cost - passengerFee*graph.getPassengers(i-1, true, false).size();
        set_int(lp, i, TRUE); //value must be int
    }

    //set obj fn = Max sum(C x d - Fee x p)Xi
    //Xi = 1 if stop is chosen, 0 otherwise
    set_obj_fn(lp, row);
    set_maxim(lp);

    resize_lp(lp, nrest, routes.size());

    //add constraint: sum(Xi) = 1
    memset(row, 1, sizeof(row));
    add_constraint(lp, row, EQ, 1.0);


    //todo: add distance constraint

    //finish adding constraints, solve
    set_add_rowmode(lp, FALSE);
    solve(lp);

    get_variables(lp, row);

    //return chosen stop
    for(int i = 0; i < sizeof(row); i++){
        if(row[i])
            return i;
    }

    return -1;
}
