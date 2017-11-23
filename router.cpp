#include "router.h"
#include "lp_lib.h"
#include "graph.h"
#include "route.h"
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <sstream>
#include <algorithm>

using namespace std;

long int n = 0;

Router::Router(){
}


Router::Router(Graph * map, double cost, double passengerFee, double maxDistance, int maxCapacity)
{
    this->graph = map;
    this->bestPath = std::vector<std::vector<std::vector<int>>>(graph->n_stops(), std::vector<std::vector<int>>(graph->n_stops(), std::vector<int>()));
    this->cost = cost;
    this->passengerFee = passengerFee;
    this->maxDistance = maxDistance;
    this->maxCapacity = maxCapacity;  
}

void Router::buildRules(){
    //build rules
    for(int i = 0; i < this->graph->passengerList.size(); i++){
        int x = this->graph->passengerList[i].first;
        int y = this->graph->passengerList[i].second;



        if(rules.find(x) == rules.end())
            rules.insert(std::pair<int, std::vector<int>>(x, std::vector<int>()));
        if(std::find(rules[x].begin(), rules[x].end(), y) == rules[x].end())
            rules[x].push_back(y);
    }
}

void Router::buildAllRoutes(){

    vector<Route>().swap(routes);

    std::map<int, std::vector<int>>().swap(rules);

    buildRules();

    for(int i = 0; i <= 7 ; i++){
        n = 0;
        cout << "For " << i << " points:" << endl;

        buildRoute(i, Route(std::vector<int>(1, graph->getStartNode()), 0), 0);

        cout << n << " combinations" << endl;

        cout<< endl << "*************" << endl;
    }

    for(int j = 0; j < routes.size(); j++){
        routes[j] = findPath(&routes[j].path);
    }

    cout << "";
}

Route Router::chooseRoute(std::vector<Route> * allRoutes){

    //build in function above
    std::vector<Route>::iterator it = allRoutes->begin();

    double best = LONG_MIN;

    std::vector<Route>::iterator chosen = it;

    std::vector<string> chosen_col_names;
    std::vector<double> var_values;

    chrono::steady_clock sc;   // create an object of `steady_clock` class
    auto start = sc.now();     // start timer

    while(it != allRoutes->end()){

        std::vector<int> critical;
        for(int i = 0; i < it->path.size(); i++){
            if(isInRules(it->path[i]))
                critical.push_back(it->path[i]);
        }

        if(critical.size()==0){
            cout << it - allRoutes->begin()+1 << "/" << allRoutes->size() << " combinations checked" << '\r';
            it++;
            continue;
        }

        std::vector<string> col_names;
        for(int i = 0; i < critical.size()-1; i++){
            for(int j = i+1; j < critical.size(); j++){

                std::ostringstream colname;
                colname << critical[i] << "," << critical[j];
                if(std::find(col_names.begin(), col_names.end(), colname.str()) == col_names.end())
                    col_names.push_back(colname.str());
            }
        }

        int n_var = col_names.size();

        lprec * lp = make_lp(0, n_var);

        //set column names for Vij
        for(int i = 0; i < col_names.size(); i++){
            set_col_name(lp, i+1, const_cast<char*>(col_names[i].c_str()));
        }

        double row[1+n_var];
        set_add_rowmode(lp, TRUE);

        //************


        for(int i = 1; i < 1+n_var; i++){
            row[i] = passengerFee;
            set_int(lp, i, TRUE); //value must be int
        }

        //set obj fn = Max P*(sum(Vij))
        set_obj_fn(lp, row);
        set_maxim(lp);

        //****************


        //int n_rest = 2*(critical.size() + n_var); //todo -> calculate number of restrictions

        //resize_lp(lp, n_rest, n_var);

        //add restriction: sum(Vij) < Cmax

        for(int i = 0; i < critical.size()-1; i++){
            memset(row, 0, sizeof(row));
            for(int j = i+1; j < critical.size(); j++){

                std::ostringstream colname;
                colname << critical[i] << "," << critical[j];
                char * name = const_cast<char*>(colname.str().c_str());
                int index = get_nameindex(lp, name, FALSE);
                row[index] = 1;
            }
            add_constraint(lp, row, LE, graph->getPassengers(critical[i], true, false).size());
        }


        //add restriction: each(vij) <= P(i,j).size()
        for(int i = 0; i < critical.size(); i++){
            for(int j = i+1; j < critical.size(); j++){
                std::ostringstream colname;
                colname << critical[i] << "," << critical[j];
                int index = get_nameindex(lp, const_cast<char*>(colname.str().c_str()), FALSE);
                set_bounds(lp, index, 0.0, graph->n_passengers(critical[i], critical[j]));
            }
        }

        //sum of all who have entered and not left at this point is <cmax
        for(int i = 0; i < critical.size(); i++){

            if(!isInRules(critical[i]))
                continue;

            memset(row, 0, sizeof(row));

            for(int j = i+1; j < critical.size(); j++){

                if(!isInRules(critical[j]))
                    continue;

                std::ostringstream colname;
                colname << critical[i] << "," << critical[j];
                int index = get_nameindex(lp, const_cast<char*>(colname.str().c_str()), FALSE);
                row[index] = 1;
            }

            for(int k = 0; k < i; k++){
                if(!isInRules(critical[k]))
                    continue;

                for(int j = i+1; j < critical.size(); j++){
                    std::ostringstream colname;
                    colname << critical[k] << "," << critical[j];
                    int index = get_nameindex(lp, const_cast<char*>(colname.str().c_str()), FALSE);
                    row[index] = 1;
                }
            }

                add_constraint(lp, row, LE, maxCapacity);
            }


        set_add_rowmode(lp, FALSE);

        //dont print while solving
        set_verbose(lp, IMPORTANT);
        //set_presolve(lp, PRESOLVE_LINDEP || PRESOLVE_IMPLIEDFREE || PRESOLVE_ROWS || PRESOLVE_COLS,  get_presolveloops(lp));
        solve(lp);

        //get economic fn value
        double heuristic =  get_objective(lp) - it->totalDistance*cost;

        get_variables(lp, row);

        //compare to previous, save index
        if(heuristic > best && get_objective(lp) != 0){
            chosen = it;
            best = heuristic;
            chosen_col_names = col_names;
            var_values.clear();
            var_values.insert(var_values.end(), &row[0], &row[n_var]);
         }

        delete_lp(lp);
        cout << it - allRoutes->begin()+1 << "/" << allRoutes->size() << " combinations checked" << '\r';
        it++;

        auto end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
        auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
        if(time_span.count()/60 >= 3)
            break;

    }
    cout << '\r' << endl;
    //return best route, subtract passengers, repeat until done.
    chosen->plan = chosen_col_names;
    chosen->plan_values = var_values;
    chosen->heuristic = best;

    cout << "Max profit: " << best << endl;
    cout << "Chosen route: " << endl;
    for(int i = 0; i < chosen->path.size(); i++)
        cout << chosen->path[i] << ",";
    cout << endl;

    return * chosen;
}

//size: number of stops to go, minus start and finish
//solution: currently building solution
//x nodes chosen til now for solution
void Router::buildRoute(int size, Route solution, int xChosen){


    if(solution.path.size() - 1 == size){

end:
        if(solution.totalDistance + graph->distance[solution.path[solution.path.size()-1]][graph->getFinishNode()] <= maxDistance){
            n++;
            solution.path.push_back(graph->getFinishNode());
            routes.push_back(solution);
        }
        return;
    }

    std::vector<int> availables; //build available nodes

    for(int i = 0; i < solution.path.size(); i++){
        std::vector<int> ys = getYs(solution.path[i]);

        //put the ys of the xs in the list in available
        for(int j = 0; j < ys.size(); j++){
            if(ys[j] != graph->getFinishNode() &&
                    !contains(availables, ys[j]) && !contains(solution.path, ys[j]))
                    availables.push_back(ys[j]);
        }
    }

   std::vector<int> xs = getXs();

   for(int i = 0; i < xs.size(); i++){
       if(!contains(availables, xs[i])
               && !contains(solution.path, xs[i])){
           //x node enters if it has a target in finish or if there is space for at least itself and a target
           if(contains(rules[xs[i]], graph->getFinishNode()) || (size + 1 - (solution.path.size()) >= xChosen + 2))
               availables.push_back(xs[i]);
       }
   }

    if(availables.size() == 0){
       if(solution.path.size() - 1 != size)
            return;
        goto end;
    }


    for(int i = 0; i < availables.size(); i++){
        Route newSolution = solution;
        newSolution.path.push_back(availables[i]);

        if(newSolution.path.size() > 1)
            newSolution.totalDistance += graph->distance[newSolution.path[newSolution.path.size()-2]][newSolution.path[newSolution.path.size()-1]];
        else
            newSolution.totalDistance += graph->distance[graph->getStartNode()][newSolution.path.size()-1];

        if(newSolution.totalDistance <= maxDistance){

            if(contains(xs, availables[i]))
                buildRoute(size, newSolution, xChosen+1);
            else
                buildRoute(size, newSolution, xChosen);

        }
    }
}

std::vector<int> Router::getXs(){
    std::vector<int> xs;

    for(std::map<int, std::vector<int>>::iterator iter = rules.begin(); iter != rules.end(); ++iter)
    {
        int k =  iter->first;
        if(std::find(xs.begin(), xs.end(), k) == xs.end())
            xs.push_back(k);
    }

    return xs;
}

bool Router::isInRules(int stop){

    /*if(stop == graph->getStartNode() || stop == graph->getFinishNode()){
        return true;
    }*/

    std::vector<int> xs = getXs();
    if(contains(xs, stop))
        return true;

    else
       for(int i = 0; i < xs.size(); i++){
           if(contains(getYs(xs[i]), stop))
               return true;
       }
    return false;
}

std::vector<int> Router::getYs(int x){
    if(rules.find(x) != rules.end())
        return rules[x];
    return std::vector<int>();

}

bool Router::contains(std::vector<int> list, int element){
    return std::find(list.begin(), list.end(), element) != list.end();
}

bool Router::contains(std::vector<bool> list, bool element){
    return std::find(list.begin(), list.end(), element) != list.end();
}

Route Router::findPath(std::vector<int>*nodes){

    Route route;
    Route * temp = new Route();

    for(int k = 0; k < nodes->size()-1; k++){
        delete(temp);
        int start = nodes->at(k);
        int target = nodes->at(k+1);

        if(bestPath[start][target].size() > 0){
            temp = new Route(bestPath[start][target], graph->distance[start][target]);
        }

        else{
            //all nodes unvisited
            std::vector<bool> visited(graph->n_stops(), false);
            //distance is infinite
            std::vector<double> distance(graph->n_stops(), std::numeric_limits<int>::max());
            distance[start] = 0;
            //previous is undefined
            std::vector<int> prev(graph->n_stops(), -1);

            while(contains(visited, false)){
                int u = -1;
                for(int i = 0; i < distance.size(); i++){
                    if(!visited[i] && (u == -1 ||  distance[i] < distance[u]))
                        u = i;
                }


                visited[u] = true;

                if(u == target)
                    break;

                std::vector<pair<int,double>> routes = graph->getRoutes(u);

                for(int i = 0; i < routes.size(); i++){
                    int v = routes[i].first;
                    if(!visited[v]){
                        double alt = distance[u] + routes[i].second;
                        if(alt < distance[v]){
                            distance[v] = alt;
                            prev[v] = u;
                        }
                    }
                }

            }

            //if(route.totalDistance + distance[target] > maxDistance)
            //  return Route();

            int u = target;

            temp = new Route();
            temp->path.insert(temp->path.begin(), u);
            while(prev[u] != -1){
                temp->path.insert(temp->path.begin(), prev[u]);
                u = prev[u];
            }

            bestPath[start][target] = temp->path;
            temp->totalDistance = distance[target];

        }

        if(route.path.empty())
            route.path.insert(route.path.end(), temp->path.begin(), temp->path.end());
        else
            route.path.insert(route.path.end(), temp->path.begin()+1, temp->path.end());

        route.totalDistance += temp->totalDistance;
        cout << "";

    }

    return route;
}

string Router::makeTrip(Route route){
    std::vector<int> coming_in(graph->n_stops(), 0);
    std::vector<int> coming_out(graph->n_stops(), 0);
    int bus = 0;

    for(int i = 0; i < route.plan.size(); i++){
        std::vector<string> plan = graph->split(route.plan[i], ',');
        coming_in[std::stoi(plan[0])]+=route.plan_values[i];
        coming_out[std::stoi(plan[1])]+=route.plan_values[i];
    }

    std::ostringstream log;

    log << "Total distance: " << route.totalDistance << endl;
    log << "Profit: " << route.heuristic << endl;
    log << "Chosen route: " << endl;

    for(int i = 0; i < route.path.size(); i++){
        log << route.path[i];
        if(i != route.path.size()-1){
            log << "->";
        }
    }
    log << endl;

    for(int i = 0; i < route.path.size(); i++){
        int in = 0;
        int out = 0;

        if(!isInRules(route.path[i]))
            continue;

        for(int j = i+1; j < route.path.size(); j++){
            if(route.path[i] == route.path[j])
                break;
            if(!isInRules(route.path[j]))
                continue;

            std::ostringstream var;
            var << route.path[i] << "," << route.path[j];
            int index = std::find(route.plan.begin(), route.plan.end(), var.str()) - route.plan.begin();
            graph->removePassengers(route.path[i], route.path[j], route.plan_values[index]);
            in += route.plan_values[index];
        }

        std::set<int> counted;
        for(int j = 0; j < i; j++){

            if(!isInRules(route.path[j]) || counted.find(route.path[j]) != counted.end())
                continue;

            counted.insert(route.path[j]);

            std::ostringstream var;
            var << route.path[j] << "," << route.path[i];
            int index = std::find(route.plan.begin(), route.plan.end(), var.str()) - route.plan.begin();
            out += route.plan_values[index];
        }


        if(in || out){
            log << "Arriving at: " << route.path[i] << " -> ";
            log << in << " enter, " << out << " leave, ";
            bus += in - out;
            coming_in[route.path[i]] = 0;
            coming_out[route.path[i]] = 0;
            log << "Bus has " << bus << " people" << endl <<  "************" << endl;
        }
    }

    return log.str();
}
