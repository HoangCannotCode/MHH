#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include "optimization.h"
#include "../reachable_marking_BFS_DFS/PetriNet.h"
#include "../reachable_marking_BFS_DFS/BFS.h"

// Helper function to print a marking
void printMarking(const Marking& m) {
    std::cout << "(";
    for (size_t i = 0; i < m.size(); i++) {
        std::cout << m[i];
        if (i + 1 < m.size()) std::cout << ",";
    }
    std::cout << ")";
}

// Test 1: Simple cyclic Petri net
void test1_cyclic_net() {
    std::cout << "\n========== TEST 1: Cyclic Petri Net ==========" << std::endl;
    std::cout << "Initial marking: (1,0,0)" << std::endl;
    std::cout << "Transitions: t1: p1->p2, t2: p2->p3, t3: p3->p1" << std::endl;
    std::cout << "Weight vector c: [3, 2, 1]" << std::endl;
    
    // I matrix (input arcs)
    std::vector<std::vector<int>> I = {
        {1, 0, 0},  // p1: consumes from t1, t2, t3
        {0, 1, 0},  // p2: consumes from t2
        {0, 0, 1}   // p3: consumes from t3
    };
    
    // O matrix (output arcs)
    std::vector<std::vector<int>> O = {
        {0, 0, 1},  // p1: produces for t3
        {1, 0, 0},  // p2: produces for t1
        {0, 1, 0}   // p3: produces for t2
    };
    
    PetriNet pn({1, 0, 0}, I, O);
    MarkingSet reachable = bfs_reachable(pn);
    
    std::cout << "Reachable markings: ";
    for (const auto& m : reachable) {
        printMarking(m);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> weights = {3, 2, 1};
    Optimizer opt(std::vector<Marking>(reachable.begin(), reachable.end()), weights);
    
    Marking optimal = opt.solve();
    std::cout << "Optimal marking: ";
    printMarking(optimal);
    std::cout << std::endl;
    std::cout << "Optimal value (c^T * M): " << opt.getOptimalValue() << std::endl;
    std::cout << "Computation time: " << std::fixed << std::setprecision(4) 
              << opt.getComputationTimeMs() << " ms" << std::endl;
}

// Test 2: Net with multiple places and transitions
void test2_complex_net() {
    std::cout << "\n========== TEST 2: Complex Petri Net ==========" << std::endl;
    std::cout << "Initial marking: (1,1,0,0)" << std::endl;
    std::cout << "Weight vector c: [5, 3, 2, 1]" << std::endl;
    
    // 4 places, 3 transitions
    // t0: p0 -> p2
    // t1: p1 -> p3
    // t2: p2, p3 -> p0, p1
    std::vector<std::vector<int>> I = {
        {1, 0, 0},  // p0: input from t0, t2
        {0, 1, 0},  // p1: input from t1, t2
        {0, 0, 1},  // p2: input from t2
        {0, 0, 1}   // p3: input from t2
    };
    
    std::vector<std::vector<int>> O = {
        {0, 0, 1},  // p0: output from t2
        {0, 0, 1},  // p1: output from t2
        {1, 0, 0},  // p2: output from t0
        {0, 1, 0}   // p3: output from t1
    };
    
    PetriNet pn({1, 1, 0, 0}, I, O);
    MarkingSet reachable = bfs_reachable(pn);
    
    std::cout << "Reachable markings: ";
    for (const auto& m : reachable) {
        printMarking(m);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> weights = {5, 3, 2, 1};
    Optimizer opt(std::vector<Marking>(reachable.begin(), reachable.end()), weights);
    
    Marking optimal = opt.solve();
    std::cout << "Optimal marking: ";
    printMarking(optimal);
    std::cout << std::endl;
    std::cout << "Optimal value (c^T * M): " << opt.getOptimalValue() << std::endl;
    std::cout << "Computation time: " << std::fixed << std::setprecision(4) 
              << opt.getComputationTimeMs() << " ms" << std::endl;
}

// Test 3: Net with empty reachable set
void test3_empty_marking() {
    std::cout << "\n========== TEST 3: Net with No Enabled Transitions ==========" << std::endl;
    std::cout << "Initial marking: (0,0)" << std::endl;
    std::cout << "Weight vector c: [1, 2]" << std::endl;
    
    std::vector<std::vector<int>> I = {
        {1, 0},
        {0, 1}
    };
    
    std::vector<std::vector<int>> O = {
        {0, 1},
        {1, 0}
    };
    
    PetriNet pn({0, 0}, I, O);
    MarkingSet reachable = bfs_reachable(pn);
    
    std::cout << "Reachable markings: ";
    if (reachable.empty()) {
        std::cout << "(empty)";
    } else {
        for (const auto& m : reachable) {
            printMarking(m);
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    
    std::vector<int> weights = {1, 2};
    Optimizer opt(std::vector<Marking>(reachable.begin(), reachable.end()), weights);
    
    Marking optimal = opt.solve();
    if (optimal.empty()) {
        std::cout << "Result: No reachable marking found (reachable set is empty)" << std::endl;
    } else {
        std::cout << "Optimal marking: ";
        printMarking(optimal);
        std::cout << std::endl;
        std::cout << "Optimal value (c^T * M): " << opt.getOptimalValue() << std::endl;
    }
    std::cout << "Computation time: " << std::fixed << std::setprecision(4) 
              << opt.getComputationTimeMs() << " ms" << std::endl;
}

// Test 4: Larger net with more places
void test4_larger_net() {
    std::cout << "\n========== TEST 4: Larger Petri Net (6 places, 4 transitions) ==========" << std::endl;
    std::cout << "Initial marking: (1,0,0,0,0,0)" << std::endl;
    std::cout << "Weight vector c: [10, 8, 6, 4, 2, 1]" << std::endl;
    
    // 6 places, 4 transitions
    // Progressive transition: spreads tokens to different places
    std::vector<std::vector<int>> I = {
        {1, 0, 0, 0},  // p0
        {0, 1, 0, 0},  // p1
        {0, 0, 1, 0},  // p2
        {0, 0, 0, 1},  // p3
        {0, 0, 0, 0},  // p4
        {0, 0, 0, 0}   // p5
    };
    
    std::vector<std::vector<int>> O = {
        {0, 0, 0, 0},  // p0
        {1, 0, 0, 0},  // p1: from t0
        {0, 1, 0, 0},  // p2: from t1
        {0, 0, 1, 0},  // p3: from t2
        {0, 0, 0, 1},  // p4: from t3
        {0, 0, 0, 0}   // p5
    };
    
    PetriNet pn({1, 0, 0, 0, 0, 0}, I, O);
    MarkingSet reachable = bfs_reachable(pn);
    
    std::cout << "Reachable markings: ";
    for (const auto& m : reachable) {
        printMarking(m);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> weights = {10, 8, 6, 4, 2, 1};
    Optimizer opt(std::vector<Marking>(reachable.begin(), reachable.end()), weights);
    
    Marking optimal = opt.solve();
    std::cout << "Optimal marking: ";
    printMarking(optimal);
    std::cout << std::endl;
    std::cout << "Optimal value (c^T * M): " << opt.getOptimalValue() << std::endl;
    std::cout << "Computation time: " << std::fixed << std::setprecision(4) 
              << opt.getComputationTimeMs() << " ms" << std::endl;
}

// Test 5: Net with negative weights
void test5_negative_weights() {
    std::cout << "\n========== TEST 5: Net with Negative Weights ==========" << std::endl;
    std::cout << "Initial marking: (1,0)" << std::endl;
    std::cout << "Weight vector c: [10, -15] (minimize p2, maximize p1)" << std::endl;
    
    std::vector<std::vector<int>> I = {
        {1, 0},
        {0, 1}
    };
    
    std::vector<std::vector<int>> O = {
        {0, 1},
        {1, 0}
    };
    
    PetriNet pn({1, 0}, I, O);
    MarkingSet reachable = bfs_reachable(pn);
    
    std::cout << "Reachable markings: ";
    for (const auto& m : reachable) {
        printMarking(m);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> weights = {10, -15};
    Optimizer opt(std::vector<Marking>(reachable.begin(), reachable.end()), weights);
    
    Marking optimal = opt.solve();
    std::cout << "Optimal marking: ";
    printMarking(optimal);
    std::cout << std::endl;
    std::cout << "Optimal value (c^T * M): " << opt.getOptimalValue() << std::endl;
    std::cout << "Computation time: " << std::fixed << std::setprecision(4) 
              << opt.getComputationTimeMs() << " ms" << std::endl;
}

// Test 6: Stress test with many transitions
void test6_stress_test() {
    std::cout << "\n========== TEST 6: Stress Test (5 places, 8 transitions) ==========" << std::endl;
    std::cout << "Initial marking: (1,0,0,0,0)" << std::endl;
    std::cout << "Weight vector c: [100, 50, 25, 10, 5]" << std::endl;
    
    // 5 places, 8 transitions
    std::vector<std::vector<int>> I = {
        {1, 0, 0, 0, 0, 0, 0, 0},  // p0
        {0, 1, 0, 0, 0, 0, 0, 0},  // p1
        {0, 0, 1, 0, 0, 0, 0, 0},  // p2
        {0, 0, 0, 1, 0, 0, 0, 0},  // p3
        {0, 0, 0, 0, 1, 0, 0, 0}   // p4
    };
    
    std::vector<std::vector<int>> O = {
        {0, 1, 0, 0, 0, 0, 0, 0},  // p0 -> p1
        {0, 0, 1, 0, 0, 0, 0, 0},  // p1 -> p2
        {0, 0, 0, 1, 0, 0, 0, 0},  // p2 -> p3
        {0, 0, 0, 0, 1, 0, 0, 0},  // p3 -> p4
        {0, 0, 0, 0, 0, 1, 0, 0}   // p4 -> (termination)
    };
    
    PetriNet pn({1, 0, 0, 0, 0}, I, O);
    MarkingSet reachable = bfs_reachable(pn);
    
    std::cout << "Reachable markings: ";
    for (const auto& m : reachable) {
        printMarking(m);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> weights = {100, 50, 25, 10, 5};
    Optimizer opt(std::vector<Marking>(reachable.begin(), reachable.end()), weights);
    
    Marking optimal = opt.solve();
    std::cout << "Optimal marking: ";
    printMarking(optimal);
    std::cout << std::endl;
    std::cout << "Optimal value (c^T * M): " << opt.getOptimalValue() << std::endl;
    std::cout << "Computation time: " << std::fixed << std::setprecision(4) 
              << opt.getComputationTimeMs() << " ms" << std::endl;
}

// Test 7: Symmetric net with multiple optimal solutions
void test7_symmetric_net() {
    std::cout << "\n========== TEST 7: Symmetric Net with Equal Weights ==========" << std::endl;
    std::cout << "Initial marking: (1,0,0)" << std::endl;
    std::cout << "Weight vector c: [1, 1, 1] (equal weights)" << std::endl;
    std::cout << "All reachable markings have same total tokens (1-safe net)" << std::endl;
    
    std::vector<std::vector<int>> I = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    
    std::vector<std::vector<int>> O = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 0}
    };
    
    PetriNet pn({1, 0, 0}, I, O);
    MarkingSet reachable = bfs_reachable(pn);
    
    std::cout << "Reachable markings: ";
    for (const auto& m : reachable) {
        printMarking(m);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> weights = {1, 1, 1};
    Optimizer opt(std::vector<Marking>(reachable.begin(), reachable.end()), weights);
    
    Marking optimal = opt.solve();
    std::cout << "Optimal marking (first found): ";
    printMarking(optimal);
    std::cout << std::endl;
    std::cout << "Optimal value (c^T * M): " << opt.getOptimalValue() << std::endl;
    std::cout << "Note: All reachable markings are equally optimal" << std::endl;
    std::cout << "Computation time: " << std::fixed << std::setprecision(4) 
              << opt.getComputationTimeMs() << " ms" << std::endl;
}

int main() {
    std::cout << "=" << std::string(68, '=') << std::endl;
    std::cout << "   OPTIMIZATION OVER REACHABLE MARKINGS" << std::endl;
    std::cout << "   Linear objective: maximize c^T * M, M in Reach(M0)" << std::endl;
    std::cout << "=" << std::string(68, '=') << std::endl;
    
    try {
        test1_cyclic_net();
        test2_complex_net();
        test3_empty_marking();
        test4_larger_net();
        test5_negative_weights();
        test6_stress_test();
        test7_symmetric_net();
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n" << std::string(68, '=') << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << std::string(68, '=') << std::endl;
    
    return 0;
}
