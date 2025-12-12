#ifndef BFS_H
#define BFS_H

#include "PetriNet.h"
#include <set>

using MarkingSet = std::set<Marking>;

MarkingSet bfs_reachable(const PetriNet& pn);

#endif // BFS_H
