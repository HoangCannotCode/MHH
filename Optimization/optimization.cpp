#include "optimization.h"
#include <iostream>
#include <algorithm>
#include <limits>

Optimizer::Optimizer(const std::vector<Marking>& reachableMarkings,
                     const std::vector<int>& weights)
    : markings(reachableMarkings), c(weights), optimalValue(std::numeric_limits<int>::min()), computationTimeMs(0.0)
{
}

Marking Optimizer::solve() {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Handle empty reachable set
    if (markings.empty()) {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
        computationTimeMs = elapsed.count();
        optimalValue = std::numeric_limits<int>::min();
        return Marking(); // return empty marking
    }
    
    // Number of places must match weight vector size
    int n = c.size();
    if (!markings.empty() && markings[0].size() != n) {
        std::cerr << "Error: Marking dimension (" << markings[0].size() 
                  << ") does not match weight vector size (" << n << ")" << std::endl;
        return Marking();
    }
    
    Marking optimalMarking = markings[0];
    optimalValue = 0;
    
    // Compute c^T * M for each marking and find maximum
    for (const auto& marking : markings) {
        int value = 0;
        for (int i = 0; i < n; ++i) {
            value += c[i] * marking[i];
        }
        
        if (value > optimalValue) {
            optimalValue = value;
            optimalMarking = marking;
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
    computationTimeMs = elapsed.count();
    
    return optimalMarking;
}

int Optimizer::getOptimalValue() const {
    return optimalValue;
}

double Optimizer::getComputationTimeMs() const {
    return computationTimeMs;
}
