#ifndef BDD_REACHABILITY_H
#define BDD_REACHABILITY_H

#include "PNML_Parser/PetriNet.h" 

#include <cuddObj.hh> 
#include <vector>

class BDDReachability {
private:
    Cudd manager;           // Trình quản lý BDD
    PetriNet& pn;           // Tham chiếu đến Petri Net đã parse

    // Vector lưu biến BDD: x (trạng thái hiện tại), y (trạng thái tiếp theo)
    std::vector<BDD> x_vars; 
    std::vector<BDD> y_vars;

public:
    // Constructor nhận vào đối tượng PetriNet
    BDDReachability(PetriNet& net);

    // Hàm chính thực hiện Task 3
    // Trả về: Pair <BDD biểu diễn tập trạng thái, Số lượng trạng thái>
    std::pair<BDD, double> compute_reachable_markings();
};

#endif