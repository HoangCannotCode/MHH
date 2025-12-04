#include <iostream>
#include "PNML_Parser/PetriNet.h"
#include "BDD_Reachability.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./main <pnml_file_path>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    PetriNet pn;

    // Parse file PNML
    std::cout << "Parsing " << filename << "..." << std::endl;
    if (!pn.parsePNML(filename)) {
        std::cerr << "Failed to parse PNML." << std::endl;
        return 1;
    }

    BDDReachability bdd_solver(pn);
    
    std::pair<BDD, double> result = bdd_solver.compute_reachable_markings();

    std::cout << "Total reachable markings: " << (long long)result.second << std::endl;

    return 0;
}