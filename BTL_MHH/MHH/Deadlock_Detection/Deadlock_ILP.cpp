#include "../PNML_Parser/PetriNet.h"
#include <glpk.h>
#include "../../cudd300/cudd/cudd.h" 
#include "../../cudd300/cplusplus/cuddObj.hh" 
#include <cmath>
#include <numeric>
#include <algorithm>
#include "Deadlock_ILP.h"
#include <iostream>
#include <vector>

using namespace std;

// Constructor Definition
DeadlockDetector::DeadlockDetector(const PetriNet& pnet, Cudd& mgr, const BDD& r_bdd)
    : net(pnet), manager(mgr), reachable_bdd(r_bdd) {}

// Định nghĩa lại hàm get_num_places
size_t get_num_places(const PetriNet& net) {
    // Lấy kích thước từ vector place_ids hoặc initial_marking
    return net.place_ids.size();
}

// -----------------------------------------------------------------------------
// create_dead_marking_ilp_model
// -----------------------------------------------------------------------------
glp_prob* DeadlockDetector::create_dead_marking_ilp_model() {
    glp_prob *lp = glp_create_prob();
    glp_set_prob_name(lp, "DeadlockSearch");
    glp_set_obj_dir(lp, GLP_MIN);
    
    // SỬA LỖI: Lấy số lượng Places từ place_ids.size()
    int num_places = get_num_places(net); 

    glp_add_cols(lp, num_places);
    for (int i = 0; i < num_places; ++i) {
        glp_set_col_kind(lp, i + 1, GLP_BV);
        glp_set_col_name(lp, i + 1, ("M" + to_string(i)).c_str());
        glp_set_obj_coef(lp, i + 1, 0.0);
    }
    
    add_dead_marking_constraints(lp);
    return lp;
}

// -----------------------------------------------------------------------------
// add_dead_marking_constraints
// -----------------------------------------------------------------------------
void DeadlockDetector::add_dead_marking_constraints(glp_prob* lp) {
    // SỬA LỖI: Lấy số lượng Transitions từ pre_matrix.size()
    size_t num_transitions = net.pre_matrix.size(); 
    
    for (size_t j = 0; j < num_transitions; ++j) {
        // T.preset không còn tồn tại. Sử dụng net.pre_matrix[j]
        const auto& preset_indices = net.pre_matrix[j];
        if (preset_indices.empty()) continue; 

        int row_index = glp_get_num_rows(lp) + 1;
        glp_add_rows(lp, 1);
        glp_set_row_name(lp, row_index, ("T_dead" + to_string(j)).c_str());

        // Kích thước của preset (số lượng Place đầu vào)
        int num_input_places = preset_indices.size(); 
        glp_set_row_bnds(lp, row_index, GLP_UP, 0.0, num_input_places - 1.0); 

        vector<int> ind(num_input_places + 1);
        vector<double> val(num_input_places + 1);

        for (int k = 0; k < num_input_places; ++k) {
            // Sử dụng các chỉ số từ pre_matrix
            ind[k + 1] = preset_indices[k] + 1; 
            val[k + 1] = 1.0;
        }
        glp_set_mat_row(lp, row_index, num_input_places, ind.data(), val.data());
    }
}

// -----------------------------------------------------------------------------
// add_cut_constraint
// -----------------------------------------------------------------------------
void DeadlockDetector::add_cut_constraint(glp_prob* lp, const vector<int>& marking) {
    // SỬA LỖI: Lấy số lượng Places từ marking.size()
    int num_places = marking.size(); 
    int row_index = glp_get_num_rows(lp) + 1;
    
    // ... (logic cut constraint giữ nguyên)
    glp_add_rows(lp, 1);
    glp_set_row_name(lp, row_index, ("Cut" + to_string(row_index)).c_str());

    vector<int> ind(num_places + 1);
    vector<double> val(num_places + 1);
    int num_positive = 0; 
    int k = 1;

    for (int i = 0; i < num_places; ++i) {
        ind[k] = i + 1; 
        if (marking[i] == 1) {
            val[k] = 1.0;
            num_positive++;
        } else {
            val[k] = -1.0;
        }
        k++;
    }
    
    glp_set_row_bnds(lp, row_index, GLP_UP, 0.0, (double)num_positive - 1.0);
    glp_set_mat_row(lp, row_index, num_places, ind.data(), val.data());
}

// -----------------------------------------------------------------------------
// check_reachability_with_bdd
// -----------------------------------------------------------------------------
bool check_reachability_with_bdd(Cudd& mgr, const BDD& reachableBdd, const vector<int>& marking) {
    // ... (Logic BDD Check giữ nguyên)
    BDD cube = mgr.bddOne(); 
    
    for (size_t i = 0; i < marking.size(); ++i) {
        BDD var = mgr.bddVar(i); 
        
        if (marking[i] == 1) {
            cube &= var;
        } else {
            cube &= !var;
        }
    }
    
    return !(reachableBdd * cube).IsZero();
}


// -----------------------------------------------------------------------------
// find_deadlock_ilp_bdd
// -----------------------------------------------------------------------------
vector<int> DeadlockDetector::find_deadlock_ilp_bdd() {
    glp_prob* lp = create_dead_marking_ilp_model();
    glp_iocp parm; 
    glp_init_iocp(&parm); 
    parm.msg_lev = GLP_MSG_OFF; 

    cout << "Starting ILP-BDD Deadlock Search..." << endl;
    
    while (true) {
        glp_intopt(lp, &parm);
        
        if (glp_get_status(lp) != GLP_OPT && glp_get_status(lp) != GLP_FEAS) {
            glp_delete_prob(lp);
            return {};
        }

        // Lấy số lượng Places
        int num_places = get_num_places(net);
        vector<int> M_star(num_places); 
        
        for (int i = 0; i < num_places; ++i) {
            M_star[i] = (int)round(glp_mip_col_val(lp, i + 1));
        }

        if (check_reachability_with_bdd(manager, reachable_bdd, M_star)) {
            glp_delete_prob(lp);
            return M_star;
        }

        add_cut_constraint(lp, M_star);
    }
}