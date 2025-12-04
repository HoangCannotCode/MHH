#ifndef BDD_REACHABILITY_H
#define BDD_REACHABILITY_H

#include "PNML_Parser/PetriNet.h" 

#include <cuddObj.hh> 
#include <vector>

class BDDReachability {
private:
    Cudd manager;           // BDD manager
    PetriNet& pn;

    // Save BDD: x (Temp), y (Next)
    std::vector<BDD> x_vars; 
    std::vector<BDD> y_vars;

public:
    BDDReachability(PetriNet& net);

    // Return Pair <BDD (show status), double (count)>
    std::pair<BDD, double> compute_reachable_markings();
};

#endif