#include "BFS.h"
#include <queue>
#include <stdexcept>
using namespace std;
// transpose helper
static vector<vector<int>> transpose(const vector<vector<int>>& mat) {
    if(mat.empty()) return {};
    size_t rows = mat.size();
    size_t cols = mat[0].size();
    vector<vector<int>> res(cols, vector<int>(rows));
    for(size_t i=0;i<rows;++i)
        for(size_t j=0;j<cols;++j)
            res[j][i] = mat[i][j];
    return res;
}

static pair<vector<vector<int>>, vector<vector<int>>>
_infer_IO(const vector<vector<int>>& I,
          const vector<vector<int>>& O,
          int n_places)
{
    int I_rows = (int)I.size();
    int I_cols = I_rows>0 ? (int)I[0].size() : 0;
    int O_rows = (int)O.size();
    int O_cols = O_rows>0 ? (int)O[0].size() : 0;

    if(I_rows == n_places && O_rows == n_places){
        if(I_cols != O_cols) throw invalid_argument("I and O have incompatible trans dimension");
        return {I,O};
    }
    if(I_cols == n_places && O_cols == n_places){
        return {transpose(I), transpose(O)};
    }
    throw invalid_argument("Cannot infer I/O matrix orientation");
}

MarkingSet bfs_reachable(const PetriNet& pn) {
    int n_places = (int)pn.M0.size();
    auto IO = _infer_IO(pn.I, pn.O, n_places);
    const auto& I = IO.first;
    const auto& O = IO.second;
    int n_trans = (int)I[0].size();

    Marking M0 = pn.M0;

    auto enabled = [&](const Marking& M, int j)->bool{
        for(int i=0;i<n_places;++i){
            if(M[i] < I[i][j]) return false;
        }
        for(int i=0;i<n_places;++i){
            int Mnew_i = M[i] - I[i][j] + O[i][j];
            if(Mnew_i > 1) return false; // 1-safe constraint
        }
        return true;
    };

    auto fire = [&](const Marking& M, int j)->Marking{
        Marking Mnew(n_places);
        for(int i=0;i<n_places;++i) Mnew[i] = M[i] - I[i][j] + O[i][j];
        return Mnew;
    };

    MarkingSet visited;
    queue<Marking> q;
    visited.insert(M0);
    q.push(M0);

    while(!q.empty()){
        Marking M = q.front(); q.pop();
        for(int j=0;j<n_trans;++j){
            if(enabled(M,j)){
                Marking Mnew = fire(M,j);
                if(visited.find(Mnew) == visited.end()){
                    visited.insert(Mnew);
                    q.push(Mnew);
                }
            }
        }
    }
    return visited;
}
