# PNML Parser

A C++ parser for **PNML (Petri Net Markup Language)** files that converts XML-based Petri net definitions into efficient internal data structures for further analysis and implementation.

## Overview

This parser reads PNML files and constructs an in-memory representation of Petri nets using:

- **Indexed vectors** for places and transitions
- **Initial marking vector** for token distribution
- **Adjacency matrices** (pre/post) for transition-place relationships

The parser handles nested `<page>` elements and supports the standard PNML format used in model checking and verification tools.

## Features

- ✅ Parses standard PNML XML format
- ✅ Recursive handling of nested `<page>` elements
- ✅ Extracts places, transitions, and arcs
- ✅ Captures initial marking (token counts)
- ✅ Builds pre-matrix (input arcs) and post-matrix (output arcs)
- ✅ Provides O(1) lookup for place/transition indices
- ✅ Consistency validation

---

## Build Instructions

### Prerequisites

- C++ compiler with C++17 support (g++, clang++, MSVC)
- Make (for Windows: MinGW/Cygwin or use nmake alternative)

### Compilation

```bash
make
# mingw32-make for Windows
```

This produces `PNML_Parser.exe` (or adjust for your platform).

To clean build artifacts:

```bash
make clean
# mingw32-make clean for Windows
```

---

## Usage

### Basic Example

```cpp
#include "PetriNet.h"
#include <iostream>

int main() {
    PetriNet net;
    
    if (net.parsePNML("pnml/example/example.pnml")) {
        std::cout << "Parsing successful!" << std::endl;
        
        // Access parsed data
        std::cout << "Places: " << net.place_ids.size() << std::endl;
        std::cout << "Transitions: " << net.transition_ids.size() << std::endl;
    } else {
        std::cerr << "Parsing failed." << std::endl;
    }
    
    return 0;
}
```

### Running the Parser

```bash
./PNML_Parser.exe
```

By default, the parser processes the file specified in `main.cpp`. Modify the `filename` variable to parse different PNML files.

---

## Data Structures

### `PetriNet` Class

#### Public Members

##### 1. ID Storage

```cpp
std::vector<std::string> place_ids;       // Place identifiers (indexed 0 to N-1)
std::vector<std::string> transition_ids;  // Transition identifiers (indexed 0 to M-1)
```

##### 2. Initial Marking

```cpp
std::vector<int> initial_marking;  // Token count for each place
                                   // initial_marking[i] = tokens in place_ids[i]
```

##### 3. Pre-Matrix (Input Arcs)

```cpp
std::vector<std::vector<int>> pre_matrix;  // Size: [num_transitions][variable]
                                           // pre_matrix[t] = list of input place indices
```

For a transition `t`, `pre_matrix[t]` contains indices of all places that provide tokens to `t`.

**Example:**

```cpp
// Transition 0 takes input from Place 1 and Place 3
pre_matrix[0] = {1, 3};
```

##### 4. Post-Matrix (Output Arcs)

```cpp
std::vector<std::vector<int>> post_matrix;  // Size: [num_transitions][variable]
                                            // post_matrix[t] = list of output place indices
```

For a transition `t`, `post_matrix[t]` contains indices of all places that receive tokens from `t`.

**Example:**

```cpp
// Transition 0 produces tokens to Place 2 and Place 4
post_matrix[0] = {2, 4};
```

#### Public Methods

```cpp
bool parsePNML(const std::string& filename);
```

Parses a PNML file and populates all data structures. Returns `true` on success, `false` on failure.

```cpp
int getPlaceIndex(const std::string& id) const;
```

Returns the index of a place given its ID string. Returns `-1` if not found. O(1) lookup.

```cpp
int getTransitionIndex(const std::string& id) const;
```

Returns the index of a transition given its ID string. Returns `-1` if not found. O(1) lookup.

---

## Implementation Guide

### 1. Accessing Places

```cpp
PetriNet net;
net.parsePNML("myfile.pnml");

// Iterate through all places
for (size_t i = 0; i < net.place_ids.size(); ++i) {
    std::cout << "Place " << i << ": " << net.place_ids[i] 
              << " (Tokens: " << net.initial_marking[i] << ")" << std::endl;
}

// Look up a specific place
int idx = net.getPlaceIndex("p_buffer");
if (idx != -1) {
    std::cout << "Place 'p_buffer' has " << net.initial_marking[idx] << " tokens" << std::endl;
}
```

### 2. Accessing Transitions

```cpp
// Iterate through all transitions
for (size_t t = 0; t < net.transition_ids.size(); ++t) {
    std::cout << "Transition " << t << ": " << net.transition_ids[t] << std::endl;
    
    // Show input places (pre-set)
    std::cout << "  Inputs: ";
    for (int p_idx : net.pre_matrix[t]) {
        std::cout << net.place_ids[p_idx] << " ";
    }
    std::cout << std::endl;
    
    // Show output places (post-set)
    std::cout << "  Outputs: ";
    for (int p_idx : net.post_matrix[t]) {
        std::cout << net.place_ids[p_idx] << " ";
    }
    std::cout << std::endl;
}
```

### 3. Checking if a Transition is Enabled

A transition is enabled if all its input places have at least one token (for 1-safe nets).

```cpp
bool isTransitionEnabled(const PetriNet& net, int t_idx, const std::vector<int>& current_marking) {
    for (int p_idx : net.pre_matrix[t_idx]) {
        if (current_marking[p_idx] < 1) {
            return false;
        }
    }
    return true;
}
```

### 4. Firing a Transition

```cpp
void fireTransition(const PetriNet& net, int t_idx, std::vector<int>& current_marking) {
    // Remove tokens from input places
    for (int p_idx : net.pre_matrix[t_idx]) {
        current_marking[p_idx]--;
    }
    
    // Add tokens to output places
    for (int p_idx : net.post_matrix[t_idx]) {
        current_marking[p_idx]++;
    }
}
```

### 5. Building a Reachability Graph

```cpp
#include <queue>
#include <set>

void exploreReachability(PetriNet& net) {
    std::queue<std::vector<int>> queue;
    std::set<std::vector<int>> visited;
    
    queue.push(net.initial_marking);
    visited.insert(net.initial_marking);
    
    while (!queue.empty()) {
        std::vector<int> current = queue.front();
        queue.pop();
        
        // Try firing each transition
        for (size_t t = 0; t < net.transition_ids.size(); ++t) {
            if (isTransitionEnabled(net, t, current)) {
                std::vector<int> next = current;
                fireTransition(net, t, next);
                
                if (visited.find(next) == visited.end()) {
                    visited.insert(next);
                    queue.push(next);
                }
            }
        }
    }
    
    std::cout << "Total reachable states: " << visited.size() << std::endl;
}
```

### 6. Converting to Incidence Matrix

For some algorithms, you may need the incidence matrix `C = Post - Pre`:

```cpp
std::vector<std::vector<int>> buildIncidenceMatrix(const PetriNet& net) {
    size_t num_places = net.place_ids.size();
    size_t num_transitions = net.transition_ids.size();
    
    // Initialize matrix with zeros
    std::vector<std::vector<int>> C(num_places, std::vector<int>(num_transitions, 0));
    
    for (size_t t = 0; t < num_transitions; ++t) {
        // Subtract input arcs
        for (int p : net.pre_matrix[t]) {
            C[p][t] -= 1;
        }
        
        // Add output arcs
        for (int p : net.post_matrix[t]) {
            C[p][t] += 1;
        }
    }
    
    return C;
}
```

---

## PNML Format Reference

The parser expects standard PNML XML structure:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<pnml xmlns="http://www.pnml.org/version-2009/grammar/pnml">
  <net id="..." type="...">
    <place id="p1">
      <name><text>Place Name</text></name>
      <initialMarking><text>1</text></initialMarking>
    </place>
    
    <transition id="t1">
      <name><text>Transition Name</text></name>
    </transition>
    
    <arc id="a1" source="p1" target="t1"/>
    <arc id="a2" source="t1" target="p2"/>
    
    <!-- Nested pages are supported -->
    <page id="page1">
      <place id="p3">...</place>
    </page>
  </net>
</pnml>
```

---

## Test Files

The repository includes several PNML test files organized by complexity:

- **`pnml/example/`** - Basic examples with clear structures
- **`pnml/simple/`** - Small models (philosophers, token ring, etc.)
- **`pnml/medium/`** - Medium-scale models
- **`pnml/large/`** - Large-scale models for performance testing

## Architecture

```plaintext
main.cpp           - Entry point with example usage
PetriNet.h         - Class definition and public interface
PetriNet.cpp       - Implementation (parsing logic)
tinyxml2.h/cpp     - XML parsing library (external dependency)
Makefile           - Build configuration
```

## Error Handling

The parser performs validation:

- ✅ Checks if file can be loaded
- ✅ Verifies PNML structure (root `<pnml>` and `<net>` elements)
- ✅ Consistency check for empty nets
- ✅ Handles missing attributes gracefully

If parsing fails, `parsePNML()` returns `false` and prints error messages to `stderr`.

## Performance Notes

- **Indexing:** All place/transition lookups use hash maps internally (O(1))
- **Memory:** Pre/post matrices use sparse representation (only store existing arcs)
- **Scalability:** Successfully tested on large models (100+ places/transitions)

## Common Pitfalls

1. **Arc Direction:** Remember that `pre_matrix[t]` contains **input places** to transition `t`, and `post_matrix[t]` contains **output places** from transition `t`.

2. **Index vs ID:** Use indices for matrix access, not string IDs:

   ```cpp
   // ❌ Wrong
   net.pre_matrix["t1"][0];  // Doesn't work!
   
   // ✅ Correct
   int t_idx = net.getTransitionIndex("t1");
   net.pre_matrix[t_idx][0];
   ```

3. **Marking Vector:** Always keep a separate `current_marking` vector when simulating. Don't modify `net.initial_marking` directly.

---

## Credits

- **XML Parsing:** This project uses [TinyXML-2](https://github.com/leethomason/tinyxml2) by Lee Thomason for XML parsing.
- **PNML Models:** Many test PNML files are sourced from [dalzilio/mcc](https://github.com/dalzilio/mcc/tree/master) and the [PNML Organization](https://www.pnml.org/version-2009/version-2009.php).
