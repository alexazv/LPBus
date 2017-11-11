#include "lp_lib.h"
#include <vector>
#include <utility>
#include <map.h>

Map map;
double cost, passengerFee;
std::vector<std::pair<int, int> > routes;

int go(){
    int nvar, nrest;
    lprec *lp = make_lp(0, routes.size());

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
