#ifndef PETRINET_H
#define PETRINET_H

#include <vector>

using Marking = std::vector<int>;

class PetriNet {
public:
    Marking M0; // initial marking
    std::vector<std::vector<int>> I; // input matrix (places x transitions OR transitions x places)
    std::vector<std::vector<int>> O; // output matrix (same orientation as I)

    PetriNet() = default;
    PetriNet(const Marking& m0,
             const std::vector<std::vector<int>>& I_,
             const std::vector<std::vector<int>>& O_);
};

#endif // PETRINET_H
