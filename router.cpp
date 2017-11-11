#include "router.h"
#include <set>
#include "lp_lib.h"

Router::Router(Map map, double cost, double passengerFee, double maxDistance)
{
    this->map = map;
    this->cost = cost;
    this->passengerFee = passengerFee;
    this->maxDistance = maxDistance;
}

void Router::buildRoute(){

    std::vector<std::pair<int,int>> passengersInBus;
    std::set<int> waitList;
    std::vector<std::vector<int>> busRoutes;

    bool visited[map.n_stops()];
    memset(visited, false, sizeof(visited));

    while(map.getPassengers().size() > 0){

        std::vector<int> busRoute;
        busRoute.push_back(map.getStartNode());
        busRoute.push_back(map.getFinishNode());

        int current = map.getStartNode();

        while(current != map.getFinishNode()){

            int nextStop = chooseStop(map.getRoutes(current));

            //get passengers into bus
            std::vector<std::pair<int,int>> passengers = map.getPassengers(nextStop, true, true);
            for(int i = 0; i < passengers.size(); i++)
                waitList.insert(passengers[i].second);

            passengersInBus.insert(passengersInBus.end(), passengers.begin(), passengers.end());;
            visited[nextStop] = true;

            current = nextStop;
        }

        busRoutes.push_back(busRoute);
    }
}

int Router::chooseStop(std::vector<std::pair<int, int> > routes){

    lprec *lp = make_lp(0, routes.size());
    int nrest = 1;

    set_add_rowmode(lp, TRUE);
    double row[1+routes.size()];

    for(int i = 1; i <= routes.size(); i++){
        int sum = 0;

        row[i] = (routes[i-1].second)*cost - passengerFee*map.getPassengers(i-1, true, false).size();
        set_int(lp, i, TRUE);
    }

    set_obj_fn(lp, row);

    resize_lp(lp, nrest, routes.size());

    set_row_name(lp, 1, "name");


    memset(row, 1, sizeof(row));
    add_constraint(lp, row, EQ, 1.0);

    set_maxim(lp);

    set_add_rowmode(lp, FALSE);

    solve(lp);

    get_variables(lp, row);

    return 0;
}
