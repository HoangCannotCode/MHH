#include "BDD_Reachability.h"
#include <iostream>
#include <algorithm> // For std::find

// Constructor
BDDReachability::BDDReachability(PetriNet& net) : pn(net) {
    // 1. Initialize CUDD Manager
    // 0,0 are default parameters for memory management
    manager = Cudd(0, 0);

    // 2. Create Variables
    // We need 2 variables per Place: x_i (current) and y_i (next).
    // Mapping: Place index i -> x is (2*i), y is (2*i + 1)
    int num_places = pn.place_ids.size();
    
    for (int i = 0; i < num_places; ++i) {
        x_vars.push_back(manager.bddVar(2 * i));     
        y_vars.push_back(manager.bddVar(2 * i + 1)); 
    }
}

std::pair<BDD, double> BDDReachability::compute_reachable_markings() {
    int num_places = pn.place_ids.size();
    int num_transitions = pn.transition_ids.size();

    // --- STEP 1: Encode Initial Marking (M0) ---
    // Logic: AND all places. If M0[i]=1 then x_i, else !x_i
    BDD M0_bdd = manager.bddOne();
    for (int i = 0; i < num_places; ++i) {
        if (pn.initial_marking[i] == 1) { //  Accessing initial_marking vector
            M0_bdd *= x_vars[i];
        } else {
            M0_bdd *= !x_vars[i];
        }
    }

    // --- STEP 2: Build Global Transition Relation (R) ---
    // R(x, y) = OR ( Relation_of_Transition_t )
    BDD Relation = manager.bddZero();

    for (int t = 0; t < num_transitions; ++t) {
        BDD t_enabled = manager.bddOne(); 
        BDD t_change = manager.bddOne();

        // Helpers to quickly check if a place is input/output of this transition
        std::vector<bool> is_input(num_places, false);
        std::vector<bool> is_output(num_places, false);

        // A. Analyze Input Arcs (Pre-set) -> Accessing pre_matrix 
        // pre_matrix[t] contains indices of places inputting to transition t
        for (int p_idx : pn.pre_matrix[t]) {
            is_input[p_idx] = true;
            // Condition to fire: Input place must have a token
            t_enabled *= x_vars[p_idx]; 
        }

        // B. Analyze Output Arcs (Post-set) -> Accessing post_matrix 
        for (int p_idx : pn.post_matrix[t]) {
            is_output[p_idx] = true;
        }

        // C. Define Logic for Next State (y) for ALL places
        for (int p = 0; p < num_places; ++p) {
            // Case 1: Input but not Output -> Token Consumed -> y = 0
            if (is_input[p] && !is_output[p]) {
                t_change *= !y_vars[p];
            }
            // Case 2: Output but not Input -> Token Produced -> y = 1
            else if (!is_input[p] && is_output[p]) {
                t_change *= y_vars[p];
            }
            // Case 3: Both Input and Output (Self-loop) -> Token stays -> y = 1
            else if (is_input[p] && is_output[p]) {
                t_change *= y_vars[p]; 
            }
            // Case 4: Not involved -> Frame Condition -> y = x (State doesn't change)
            else {
                t_change *= (x_vars[p].Xnor(y_vars[p]));
            }
        }

        // Add this transition's logic to the global relation
        Relation += (t_enabled * t_change);
    }

    // --- STEP 3: Symbolic BFS (Fixed Point Iteration) ---
    BDD Reachable = M0_bdd;
    BDD OldReachable = manager.bddZero();

    // Create a "cube" of all x variables for existential abstraction
    BDD cube_x = manager.bddOne();
    for (const auto& x : x_vars) cube_x *= x;

    int iterations = 0;
    while (Reachable != OldReachable) {
        OldReachable = Reachable;
        iterations++;

        // 1. Image Computation: Find Next States (in terms of y)
        // Next(y) = Exist x . (Reachable(x) AND Relation(x,y))
        BDD Next_y = Reachable.AndAbstract(Relation, cube_x);

        // 2. Swap Variables: Convert Next(y) -> Next(x) for the next loop
        // We prepare vectors for swap
        std::vector<BDD> from_y = y_vars;
        std::vector<BDD> to_x = x_vars;
        BDD Next_x = Next_y.SwapVariables(from_y, to_x);

        // 3. Union: Add new states to Reachable set
        Reachable = Reachable + Next_x;
        
        // Debug info (optional)
        // std::cout << "Iteration " << iterations << "..." << std::endl;
    }

    // --- STEP 4: Count States ---
    double num_states = Reachable.CountMinterm(num_places);
    
    return {Reachable, num_states};
}