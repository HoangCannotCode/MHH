# BDD-Based Petri Net Reachability Analysis

This project implements symbolic reachability analysis for Petri nets using Binary Decision Diagrams (BDD) via the CUDD library.

## Prerequisites

Before building and running the program, ensure you have the following installed:

- **GCC/G++ compiler** (C++11 or later)
- **CUDD library** (Decision Diagram package)
- **Make** (optional, for automated builds)
- **Git** (for cloning the repository)

## Installation Guide

### Step 1: Install System Dependencies

**On Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential git
```

**On macOS (with Homebrew):**
```bash
brew install gcc
```

### Step 2: Build and Install CUDD Library

The CUDD library should be pre-built in the `cudd/` directory. If not, build it manually:

```bash
cd cudd
./configure
make
sudo make install
```

Alternatively, if CUDD is already compiled:
```bash
# Verify the CUDD library exists
ls -la cudd/.libs/libcudd.a
```

### Step 3: Verify Dependencies

Check that the required headers are present:
```bash
ls -la cudd/cudd/cudd.h
ls -la cudd/include/cudd.h
```

## Building the Program

Navigate to the `SymbolicComputationUsingBDD` directory and compile:

```bash
cd SymbolicComputationUsingBDD
g++ -std=c++11 -o petri_solver main.cpp BDD_Reachability.cpp ../PNML_Parser/PetriNet.cpp ../PNML_Parser/tinyxml2.cpp \
    -I.. -I../cudd/cudd -I../cudd/include \
    -L../cudd/.libs -lcudd -lm
```

### Verification

After compilation, verify the executable was created:
```bash
ls -la petri_solver
```

## Running the Program

### Basic Usage

Run the program with a PNML file as input:

```bash
./petri_solver <path_to_pnml_file>
```

### Example

```bash
./petri_solver testcase/test_20_places.pnml
```

Expected output:
```
Parsing testcase/test_20_places.pnml...
Consistency Check Passed: 20 Places, 10 Transitions loaded.
Computing symbolic reachability...
--------------------------------
Total reachable markings: 11
--------------------------------
```

## Test Files

Test cases are located in the `testcase/` directory with varying complexity:

| Test File | Places | Transitions | Approx. Runtime |
|-----------|--------|-------------|-----------------|
| `testcase/test_many_places.pnml` | 10 | 4 | ~1ms |
| `testcase/test_20_places.pnml` | 20 | 10 | ~10ms |
| `testcase/test_50_places.pnml` | 50 | 49 | ~26ms |
| `testcase/test_100_places.pnml` | 100 | 99 | ~93ms |
| `testcase/test_500_places.pnml` | 500 | 499 | ~12.6s |
| `testcase/test_1000_places.pnml` | 1,000 | 999 | ~65s |
| `testcase/test_1000_places_worst_case.pnml` | 1,000 | 1,998 | > 5min |
| `testcase/test_10000_places.pnml` | 10,000 | 9,999 | > 5min |

### Running Tests

```bash
# Quick test
./petri_solver testcase/test_20_places.pnml

# Small test
./petri_solver testcase/test_100_places.pnml

# Medium test (may take ~12 seconds)
./petri_solver testcase/test_500_places.pnml

# Large test (may take > 1 minute)
./petri_solver testcase/test_1000_places.pnml
```

## Project Structure

```
SymbolicComputationUsingBDD/
├── main.cpp                  # Entry point
├── BDD_Reachability.h        # BDD reachability class header
├── BDD_Reachability.cpp      # BDD reachability implementation
├── petri_solver              # Compiled executable
├── README.md                 # This file
└── testcase/                 # Test files directory
    ├── test_many_places.pnml
    ├── test_20_places.pnml
    ├── test_50_places.pnml
    ├── test_100_places.pnml
    ├── test_500_places.pnml
    ├── test_1000_places.pnml
    ├── test_1000_places_worst_case.pnml
    └── test_10000_places.pnml
```

## PNML File Format

The program expects PNML (Petri Net Markup Language) files. Basic structure:

```xml
<pnml>
  <net id="net1" type="http://www.pnml.org/version-2009/grammar/ptnet">
    <page id="page0">
      <place id="p1">
        <initialMarking> <text>1</text> </initialMarking>
      </place>
      <place id="p2">
        <initialMarking> <text>0</text> </initialMarking>
      </place>
      <transition id="t1"/>
      <arc id="a1" source="p1" target="t1"/>
      <arc id="a2" source="t1" target="p2"/>
    </page>
  </net>
</pnml>
```

## Performance Characteristics

The BDD-based solver demonstrates excellent scalability:

- **Fast computation** for nets up to 500 places
- **Memory efficient** due to BDD compression
- **Symbolic representation** avoids explicit state enumeration
- **Logarithmic complexity** for many operations

Performance degrades gracefully with larger nets due to BDD node growth.

## Troubleshooting

### Error: Cannot find libcudd
```bash
# Solution: Add CUDD library path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../cudd/.libs
./petri_solver ../test.pnml
```

### Error: cudd.h not found
```bash
# Solution: Verify CUDD is built
cd ../cudd
./configure
make
cd ../SymbolicComputationUsingBDD
# Recompile
```

### Error: Cannot parse PNML file
- Ensure the PNML file is well-formed XML
- Check that all places and transitions have unique IDs
- Verify arcs reference existing places/transitions

## Algorithm Overview

1. **Parser**: Loads PNML file and constructs Petri net structure
2. **BDD Construction**: Creates BDD variables for each place
3. **Initial State Encoding**: Encodes initial marking as BDD
4. **Transition Relation**: Builds global transition relation as BDD
5. **Fixed-Point Iteration**: Computes reachable states using symbolic BFS
6. **Counting**: Counts total reachable markings

## Limitations

- **Large nets** (> 1000 places) may require significant memory and time
- **Complex topologies** (highly connected) increase BDD size
- **Unbounded places** can lead to infinite state spaces

## References

- CUDD Library: https://davidkebo.com/cudd
- Petri Nets: https://en.wikipedia.org/wiki/Petri_net
- Binary Decision Diagrams: https://en.wikipedia.org/wiki/Binary_decision_diagram

## License

See LICENSE file in the parent directory.

## Contact

For issues or questions, please refer to the project repository.
