# MHH - Mathematical Modeling of Symbolic and Algebraic Reasoning in Petri Nets

A comprehensive implementation of Petri net analysis techniques combining explicit computation, symbolic BDD-based reachability, ILP-based deadlock detection, and optimization algorithms.

## Project Overview

This project implements five key tasks for analyzing 1-safe Petri nets:

1. **PNML Parser** - Read Petri nets from standard PNML files
2. **Explicit Reachability** - Enumerate reachable markings using BFS/DFS
3. **Symbolic BDD Reachability** - Compute reachable markings symbolically using Binary Decision Diagrams
4. **Deadlock Detection** - Detect deadlocks using hybrid ILP-BDD approach
5. **Optimization** - Find optimal reachable markings under linear objectives

## Repository Structure

```
MHH/
├── PNML_Parser/              # Task 1: PNML parsing
│   ├── PetriNet.h/cpp        # Core Petri net data structures
│   ├── tinyxml2.h/cpp        # XML parsing library
│   ├── main.cpp              # Standalone parser
│   └── pnml/                 # Test PNML files
│       ├── example/          # Basic examples
│       ├── simple/           # Simple models (29 files)
│       ├── medium/           # Medium-sized models
│       └── large/            # Large models
│
├── reachable_marking_BFS_DFS/  # Task 2: Explicit reachability
│   ├── BFS.h/cpp             # Breadth-first search
│   ├── DFS.h/cpp             # Depth-first search
│   ├── PetriNet.h/cpp        # Petri net implementation
│   └── main.cpp              # Test cases
│
├── SymbolicComputationUsingBDD/  # Task 3: BDD reachability
│   ├── BDD_Reachability.h/cpp    # Symbolic state space exploration
│   ├── main.cpp                  # Standalone BDD computation
│   └── test.pnml                 # Test file
│
├── Deadlock_Detection/        # Task 4: Deadlock detection
│   ├── Deadlock_ILP.h/cpp     # ILP-BDD hybrid algorithm
│   └── main.cpp               # End-to-end deadlock detector
│
├── Optimization/              # Task 5: Optimization
│   ├── optimization.h/cpp     # Optimization algorithm
│   ├── main.cpp               # Test suite
│   ├── Makefile               # Build configuration
│   └── TEST_REPORT.md         # Detailed test results
│
├── CHECKLIST.md              # Project completeness checklist
├── DEPENDENCIES.md           # Detailed dependency information
├── TEST_PLAN.md              # Comprehensive testing strategy
└── README.md                 # This file
```

## Quick Start

### Prerequisites

1. **C++ Compiler**: MinGW-w64 with C++17 support (GCC 11+)
2. **CUDD Library**: Binary Decision Diagrams (Tasks 3, 4)
3. **GLPK**: GNU Linear Programming Kit (Task 4)
4. **TinyXML2**: Included in project

See [DEPENDENCIES.md](DEPENDENCIES.md) for detailed installation instructions.

### Installation

#### Windows (MSYS2/MinGW64)

```bash
# 1. Install MSYS2 from https://www.msys2.org/

# 2. Open MSYS2 MinGW 64-bit terminal
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-glpk

# 3. Install CUDD (see DEPENDENCIES.md for details)
# Download and build CUDD 3.0.0, place in project root as cudd300/

# 4. Clone this repository
git clone https://github.com/HoangCannotCode/MHH.git
cd MHH
git checkout final
```

## Running Each Task

### Task 1: PNML Parser

```bash
cd PNML_Parser
make
./PNML_Parser.exe pnml/example/example.pnml
```

**Output**: Displays places, transitions, initial marking, and arc matrices.

### Task 2: Explicit Reachability (BFS/DFS)

```bash
cd reachable_marking_BFS_DFS
g++ -std=c++17 -o test.exe main.cpp BFS.cpp DFS.cpp PetriNet.cpp
./test.exe
```

**Output**: Test results showing reachable markings for various Petri nets.

### Task 3: BDD Reachability

```bash
cd SymbolicComputationUsingBDD
g++ -std=c++17 main.cpp BDD_Reachability.cpp \
    ../PNML_Parser/PetriNet.cpp ../PNML_Parser/tinyxml2.cpp \
    -I../../cudd300/cplusplus -I../../cudd300/cudd \
    -L../../cudd300/cudd/.libs -L../../cudd300/cplusplus/.libs \
    -lcudd -lobj -ldddmp -o bdd_reachability.exe

./bdd_reachability.exe test.pnml
```

**Output**: Total number of reachable markings computed symbolically.

### Task 4: Deadlock Detection

```bash
cd Deadlock_Detection

# Compile (adjust paths as needed)
g++ -std=c++17 main.cpp Deadlock_ILP.cpp \
    ../SymbolicComputationUsingBDD/BDD_Reachability.cpp \
    ../PNML_Parser/PetriNet.cpp ../PNML_Parser/tinyxml2.cpp \
    -I../../cudd300/cplusplus -I../../cudd300/cudd \
    -L../../cudd300/cudd/.libs -L../../cudd300/cplusplus/.libs \
    -L/path/to/mingw64/lib \
    -lcudd -lobj -ldddmp -lglpk -lz -o detector.exe

./detector.exe ../PNML_Parser/pnml/example/philo.pnml
```

**Output**: Reports deadlock if found, otherwise "No deadlock found."

### Task 5: Optimization

```bash
cd Optimization
make
./optimization.exe
```

**Output**: Test results showing optimal markings and objective values.

## Testing

Comprehensive testing documentation is available in [TEST_PLAN.md](TEST_PLAN.md).

### Quick Test

```bash
# Test Task 1
cd PNML_Parser && make && ./PNML_Parser.exe pnml/example/example.pnml

# Test Task 2
cd ../reachable_marking_BFS_DFS && make && ./test.exe

# Test Task 5
cd ../Optimization && make && ./optimization.exe
```

## Documentation

- **[CHECKLIST.md](CHECKLIST.md)** - Complete project status and requirements
- **[DEPENDENCIES.md](DEPENDENCIES.md)** - Detailed dependency installation guide
- **[TEST_PLAN.md](TEST_PLAN.md)** - Comprehensive testing strategy
- **[docs.txt](docs.txt)** - Original assignment documentation

### Module-Specific Documentation

- [PNML_Parser/README.md](PNML_Parser/README.md) - Parser usage and API
- [Optimization/README.md](Optimization/README.md) - Optimization algorithm details
- [Optimization/TEST_REPORT.md](Optimization/TEST_REPORT.md) - Detailed test results

## Key Features

### Task 1: PNML Parser
- ✅ Parses standard PNML XML format
- ✅ Handles nested `<page>` elements
- ✅ Builds indexed data structures (O(1) lookup)
- ✅ Constructs pre/post adjacency matrices

### Task 2: Explicit Reachability
- ✅ BFS implementation
- ✅ DFS implementation
- ✅ Complete enumeration of reachable markings
- ✅ Built-in test suite with 6 test cases

### Task 3: BDD Reachability
- ✅ Symbolic state encoding using CUDD
- ✅ Iterative image computation
- ✅ Compact representation of large state spaces
- ✅ Performance comparison ready

### Task 4: Deadlock Detection
- ✅ ILP formulation for dead markings
- ✅ BDD-based reachability checking
- ✅ Iterative refinement with cut constraints
- ✅ Reports actual deadlock markings

### Task 5: Optimization
- ✅ Linear objective function (maximize c^T * M)
- ✅ Brute-force optimization over reachable set
- ✅ Extensive test suite (6 test cases)
- ✅ Performance tracking (time measurement)

## Performance Notes

- **Task 2 vs Task 3**: BDD approach is more memory-efficient for large state spaces
- **Task 4**: ILP-BDD hybrid balances expressiveness and efficiency
- **Task 5**: Brute-force is sufficient for 1-safe Petri nets with moderate state spaces

## Known Issues and Limitations

1. **CUDD Dependency**: Tasks 3 and 4 require CUDD library (not included in repo)
2. **Path Configuration**: Compilation commands use absolute paths (need adjustment)
3. **1-Safe Assumption**: Code assumes input Petri nets are 1-safe (not validated)
4. **Performance**: Task 5 uses brute-force (not optimized for very large models)

See [CHECKLIST.md](CHECKLIST.md) for complete status and action items.

## Course Information

- **Course**: CO2011/CSE251 - Mathematical Modeling
- **Institution**: Ho Chi Minh City University of Technology
- **Assignment**: Symbolic and Algebraic Reasoning in Petri Nets
- **Deadline**: December 12, 2025, 23:00 (GMT+7)

## References

### Libraries Used
- **CUDD 3.0.0**: https://github.com/ivmai/cudd
- **GLPK**: https://www.gnu.org/software/glpk/
- **TinyXML2**: https://github.com/leethomason/tinyxml2

### Theoretical Background
- Petri Nets: Classical reference texts
- Binary Decision Diagrams: Bryant (1986)
- Integer Linear Programming: Applied optimization

## Contributing

This is an academic project. For questions or issues:
- Post on BK-eLearning forum
- Contact course instructor
- Open GitHub issue (for technical problems)

## License

Academic project - see course policies regarding code sharing and collaboration.

## Authors

Course: CO2011/CSE251  
GitHub: https://github.com/HoangCannotCode/MHH  
Branch: `final`

---

*Last Updated: December 12, 2025*
