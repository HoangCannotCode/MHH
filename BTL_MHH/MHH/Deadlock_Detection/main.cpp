#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Khai báo các lớp/structs cần thiết
#include "../PNML_Parser/PetriNet.h" // Chứa PetriNet Class
#include "../SymbolicComputationUsingBDD/BDD_Reachability.h" // Chứa BDDReachability Class
#include "../../cudd300/cplusplus/cuddObj.hh" // Cho kiểu BDD và Cudd
#include "Deadlock_ILP.h" // Chứa DeadlockDetector Class
// class DeadlockDetector; // Không cần nếu Deadlock_ILP.h đã được include

using namespace std;

// Hàm Main (Điểm khởi chạy)
int main(int argc, char* argv[]) {
    
    // --- 1. KIỂM TRA ĐẦU VÀO ---
    if (argc < 2) {
        cout << "Usage: ./detector <pnml_file_path>" << endl;
        return 1;
    }
    
    // Lấy tên file từ đối số dòng lệnh
    std::string filename = argv[1];
    PetriNet net;

    // --- 2. GỌI PNML PARSING (Task 1) ---
    cout << "Parsing file: " << filename << "..." << endl;
    if (!net.parsePNML(filename)) {
        cerr << "ERROR: Failed to parse PNML file or data is incomplete." << endl;
        return 1;
    }
    cout << "Parsing completed successfully. Network details:" << endl;
    cout << "  - Places: " << net.place_ids.size() << endl;
    cout << "  - Transitions: " << net.transition_ids.size() << endl;
    
    // --- 3. CHẠY BDD REACHABILITY (Task 3) ---
    cout << "\n--- Starting Task 3: BDD Reachability ---\n";
    BDDReachability bdd_solver(net); 
    
    std::pair<BDD, double> result = bdd_solver.compute_reachable_markings();
    
    // Lấy các dữ liệu cần thiết cho Task 4
    BDD reachable_bdd = result.first;
    Cudd& mgr = bdd_solver.getManager(); 
    
    cout << "Total Reachable Markings: " << (long long)result.second << endl;

    // --- 4. CHẠY DEADLOCK DETECTION (Task 4) ---
    cout << "\n--- Starting Task 4: ILP-BDD Deadlock Detection ---\n";
    
    // Khởi tạo Detector (sử dụng Cudd Manager và BDD từ Task 3)
    DeadlockDetector detector(net, mgr, reachable_bdd); 
    
    // Lưu ý: Đảm bảo bạn đã sửa DeadlockDetector.cpp để dùng net.place_ids.size()
    vector<int> deadlock_marking = detector.find_deadlock_ilp_bdd(); 
    
    // --- 5. HIỂN THỊ KẾT QUẢ ---
    if (!deadlock_marking.empty()) {
        cout << "\n========================================" << endl;
        cout << "Deadlock FOUND (ILP/BDD) at Marking: (";
        for (size_t i = 0; i < deadlock_marking.size(); ++i) {
            cout << deadlock_marking[i] << (i == deadlock_marking.size() - 1 ? "" : ", ");
        }
        cout << ")" << endl;
        cout << "========================================" << endl;
    } else {
        cout << "\nNo Deadlock found in the Reachable Set." << endl;
    }
    
    return 0;
}