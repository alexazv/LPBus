#include "router.h"
#include <set>
#include "lp_lib.h"
#include "graph.h"
#include "route.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <sstream>

using namespace std;

long int n = 0;

std::vector<Route> routes;

Router::Router(Graph map, double cost, double passengerFee, double maxDistance, int maxCapacity)
{
    this->graph = map;
    this->cost = cost;
    this->passengerFee = passengerFee;
    this->maxDistance = maxDistance;
    this->maxCapacity = maxCapacity;

    //build rules
    for(int i = 0; i < this->graph.passengerList.size(); i++){
        int x = this->graph.passengerList[i].first;
        int y = this->graph.passengerList[i].second;



        if(rules.find(x) == rules.end())
            rules.insert(std::pair<int, std::vector<int>>(x, std::vector<int>()));
        if(std::find(rules[x].begin(), rules[x].end(), y) == rules[x].end())
            rules[x].push_back(y);
    }

    buildAllRoutes();
}

void Router::buildAllRoutes(){

    for(int i = 2; i <= 10; i++){
        routes.clear();
        routes.shrink_to_fit();
        n = 0;
        cout << "For " << i << " points:" << endl;
        chrono::steady_clock sc;   // create an object of `steady_clock` class
        auto start = sc.now();     // start timer

        buildRoute(i, Route(std::vector<int>(1, graph.getStartNode()), 0), 0);

        cout << n << " combinations" << endl;
        auto end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
        auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
        cout<<"Operation took: "<<time_span.count()<<" seconds !!!" << endl;

        start = sc.now();

        chooseRoute(routes);

        end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
        time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
        cout<<"Operation took: "<<time_span.count()<<" seconds !!!" << endl << "*************" << endl;
    }
}

void Router::chooseRoute(std::vector<Route> allRoutes){

    //build in function above
    std::vector<Route>::iterator it = allRoutes.begin();

    double best = -std::numeric_limits<double>::infinity();

    std::vector<Route>::iterator chosen = it;

    while(it != allRoutes.end()){

        std::vector<string> col_names;
        for(int i = 0; i < it->route.size()-1; i++){
            for(int j = i+1; j < it->route.size(); j++){
                std::ostringstream colname;
                colname << "V" << i << "," << j;
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


        int n_rest = 0; //todo -> calculate number of restrictions

        resize_lp(lp, n_rest, n_var);

        //rest: sum(Vij) < Cmax
        memset(row, 0, sizeof(row));


        for(int i = 0; i < it->route.size()-1; i++){ //fix
            memset(row, 0, sizeof(row));
            for(int j = i+1; j < it->route.size(); j++){
                std::ostringstream colname;
                colname << "V" << i << "," << j;
                char * name = const_cast<char*>(colname.str().c_str());
                int index = get_nameindex(lp, name, FALSE);
                row[index] = 1;
            }
            add_constraint(lp, row, LE, maxCapacity);
        }

        //add restriction: each(vij) <= P(i,j).size()
        for(int i = 0; i < it->route.size(); i++){
            for(int j = i+1; j < it->route.size(); j++){
                memset(row, 0, sizeof(row));
                std::ostringstream colname;
                colname << "V" << i << "," << j;
                int index = get_nameindex(lp, const_cast<char*>(colname.str().c_str()), FALSE);
                row[index] = 1;
                add_constraint(lp, row, LE, graph.n_passengers(it->route.at(i), it->route.at(j)));
            }
        }

        memset(row, 1, sizeof(row));
        add_constraint(lp, row, GE, 0.0);

        set_add_rowmode(lp, FALSE);

        //dont print while solving
        set_verbose(lp, IMPORTANT);

        solve(lp);

        //get economic fn value
        double heuristic =  -it->totalDistance*cost + get_objective(lp);
        get_variables(lp, row);

        //compare to previous, save index
        if(heuristic > best){
            chosen = it;
            best = heuristic;
         }

        //todo: return row somehow, use it to remove passengers

        cout << it - allRoutes.begin()+1 << "/" << allRoutes.size() << " combinations checked" << '\r';

        delete_lp(lp);
        it++;
    }
    cout << '\r' << endl;
    //return best route, subtract passengers, repeat until done.
    cout << "Max profit: " << best << endl;
    cout << "Chosen route: " << endl;
    for(int i = 0; i < chosen->route.size(); i++)
        cout << chosen->route[i] << ",";
    cout << endl;
}

void Router::buildRoute(int size, Route solution, int xChosen){

    if(solution.route.size()-1 == size){ //account for finish node

end:
        /*for(int i = 0; i < solution.size(); i++){
            cout << solution[i] << ",";
        }
        cout << endl;*/
        n++;
        solution.route.push_back(graph.getFinishNode());
        routes.push_back(solution);
        return;
    }

    std::vector<int> availables; //build available nodes

    for(int i = 0; i < solution.route.size(); i++){
        std::vector<int> ys = getYs(solution.route[i]);

        //put the ys of the xs in the list in available
        for(int j = 0; j < ys.size(); j++){
            if(ys[j] != graph.getFinishNode() &&
                    !contains(availables, ys[j]) && !contains(solution.route, ys[j]))
                    availables.push_back(ys[j]);
        }
    }

   std::vector<int> xs = getXs();

   for(int i = 0; i < xs.size(); i++){
       if(!contains(availables, xs[i])
               && !contains(solution.route, xs[i])){
           //x node enters if it has a target in finish or if there is space for at least itself and a target
           if(contains(rules[xs[i]], graph.getFinishNode()) || (size - solution.route.size() >= xChosen + 2))
               availables.push_back(xs[i]);
       }
   }

    if(availables.size() == 0){
       if(solution.route.size()-1 != size)
            return;
        goto end;
    }


    for(int i = 0; i < availables.size(); i++){
        Route newSolution = solution;
        newSolution.route.push_back(availables[i]);

        if(newSolution.route.size() > 1)
            newSolution.totalDistance += graph.distance[newSolution.route[newSolution.route.size()-2]][newSolution.route[newSolution.route.size()-1]];
        else
            newSolution.totalDistance += graph.distance[graph.getStartNode()][newSolution.route.size()-1];

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

std::vector<int> Router::getYs(int x){
    if(rules.find(x) != rules.end())
        return rules[x];
    return std::vector<int>();

}

bool Router::contains(std::vector<int> list, int element){
    return std::find(list.begin(), list.end(), element) != list.end();
}

void Router::nonRepeatAdd(std::vector<int>* list, int element){
    if(!contains(*list, element))
        list->push_back(element);
}
