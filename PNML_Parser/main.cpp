#include "PetriNet.h"
#include <iostream>

int main() {
    PetriNet net;
    
    // PNML file path
    std::string filename = "pnml/example/examples.pnml"; 
    
    std::cout << "Parsing " << filename << "..." << std::endl;
    
    if (net.parsePNML(filename)) {
        std::cout << "--- Parsing Successful ---" << std::endl;
        
        // Print Internal Representation
        std::cout << "Places (" << net.place_ids.size() << "):" << std::endl;
        for (size_t i = 0; i < net.place_ids.size(); ++i) {
            std::cout << "  [" << i << "] " << net.place_ids[i] 
                      << " (Tokens: " << net.initial_marking[i] << ")" << std::endl;
        }

        std::cout << "\nTransitions (" << net.transition_ids.size() << "):" << std::endl;
        for (size_t t = 0; t < net.transition_ids.size(); ++t) {
            std::cout << "  [" << t << "] " << net.transition_ids[t] << std::endl;
            
            // Show input places for this transition
            std::cout << "    IN (Pre): ";
            for (int p_idx : net.pre_matrix[t]) {
                std::cout << net.place_ids[p_idx] << " ";
            }
            std::cout << std::endl;

            // Show output places for this transition
            std::cout << "    OUT (Post): ";
            for (int p_idx : net.post_matrix[t]) {
                std::cout << net.place_ids[p_idx] << " ";
            }
            std::cout << "\n" << std::endl;
        }
    } else {
        std::cerr << "Parsing failed." << std::endl;
    }

    return 0;
}