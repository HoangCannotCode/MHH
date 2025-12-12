#ifndef DEADLOCK_ILP_H
#define DEADLOCK_ILP_H

// BẮT BUỘC: Include file đã có (chứa struct PetriNet và các struct/class khác)
#include "../PNML_Parser/PetriNet.h"   
// CUDD (đường dẫn tương đối)
#include "../../cudd300/cplusplus/cuddObj.hh"   
#include <vector>
#include <string>

using namespace std;

// Forward declaration cho GLPK struct (để tránh include glpk.h trong header)
struct glp_prob;

// Hàm BDD Check (Được định nghĩa trong Deadlock_ILP.cpp)
// Chức năng: Kiểm tra xem Marking có nằm trong BDD Reachable Set không.
bool check_reachability_with_bdd(Cudd& mgr, const BDD& reachableBdd, const vector<int>& marking);

// Lớp DeadlockDetector
class DeadlockDetector {
private:
    // 1. Tham chiếu đến Petri Net (sử dụng pre_matrix/post_matrix)
    const PetriNet& net;
    
    // 2. Manager và BDD (từ Task 3)
    Cudd& manager;         
    const BDD& reachable_bdd;  

    // Khai báo các hàm ILP phụ trợ
    glp_prob* create_dead_marking_ilp_model();
    void add_dead_marking_constraints(glp_prob* lp);
    void add_cut_constraint(glp_prob* lp, const vector<int>& marking);

public:
    // Constructor
    DeadlockDetector(const PetriNet& pnet, Cudd& mgr, const BDD& r_bdd);

    // Hàm chính: Vòng lặp ILP-BDD
    vector<int> find_deadlock_ilp_bdd();
};

#endif