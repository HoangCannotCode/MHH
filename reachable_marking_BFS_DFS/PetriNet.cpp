#include "PetriNet.h"

PetriNet::PetriNet(const Marking& m0,
                   const std::vector<std::vector<int>>& I_,
                   const std::vector<std::vector<int>>& O_)
    : M0(m0), I(I_), O(O_)
{}
