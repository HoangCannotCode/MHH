#ifndef DFS_H
#define DFS_H

#include "PetriNet.h"
#include <set>

using MarkingSet = std::set<Marking>;

MarkingSet dfs_reachable(const PetriNet& pn);

#endif // DFS_H
