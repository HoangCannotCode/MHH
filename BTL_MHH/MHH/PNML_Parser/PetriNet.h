#ifndef PETRINET_H
#define PETRINET_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

// Represents a connection from a node to another
struct Arc {
    std::string source_id;
    std::string target_id;
    std::string id;
};

class PetriNet {
public:
    // --- DATA STRUCTURES (Public for teammates to access) ---

    // 1. Storage for IDs (indexed 0 to N-1)
    std::vector<std::string> place_ids;
    std::vector<std::string> transition_ids;

    // 2. Initial State Vector (size = num_places)
    // 1-safe nets usually have 0 or 1, but we store as int.
    std::vector<int> initial_marking;

    // 3. Adjacency / Flow Relations
    // Stores indices! [Transition Index] -> List of [Place Indices]
    // Example: pre_matrix[0] = {1, 3} means Transition 0 takes input from Place 1 and Place 3.
    std::vector<std::vector<int>> pre_matrix;  // Input Arcs (Place -> Transition)
    std::vector<std::vector<int>> post_matrix; // Output Arcs (Transition -> Place)

    // --- METHODS ---
    
    // Main parsing function
    bool parsePNML(const std::string& filename);

    // Helpers to get index from string ID (O(1) lookup)
    int getPlaceIndex(const std::string& id) const;
    int getTransitionIndex(const std::string& id) const;

private:
    // Fast lookups used internally during parsing
    std::unordered_map<std::string, int> place_map;
    std::unordered_map<std::string, int> transition_map;

    // Verify consistency: Check for missing nodes or disconnected arcs
    bool verifyConsistency(); 
};

#endif