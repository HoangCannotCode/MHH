#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>
#include <set>
#include <chrono>

using Marking = std::vector<int>;
using MarkingSet = std::set<Marking>;

class Optimizer {
public:
    // Constructor: takes reachable markings and weight vector
    Optimizer(const std::vector<Marking>& reachableMarkings,
              const std::vector<int>& weights);

    // Run optimization: maximize c^T * M over all reachable markings
    // Returns the optimal marking, or empty vector if reachable set is empty
    Marking solve();
    
    // Get the optimal value (c^T * M for the optimal marking)
    int getOptimalValue() const;
    
    // Get the computation time of the last solve
    double getComputationTimeMs() const;

private:
    std::vector<Marking> markings;  // all reachable markings
    std::vector<int> c;              // weight vector
    int optimalValue;                // c^T * M for optimal marking
    double computationTimeMs;        // computation time in milliseconds
};

#endif
