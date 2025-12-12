# Installation, Compilation, and Execution Guide for Deadlock Detection System

## Overview
A deadlock detection system for Petri Nets using Binary Decision Diagram (BDD) technology combined with Integer Linear Programming (ILP).

---

## 1. Installing Required Libraries

### 1.1 Update System
```bash
sudo apt-get update
sudo apt-get upgrade -y
```

### 1.2 Install GLPK (GNU Linear Programming Kit)
```bash
sudo apt-get install -y libglpk-dev
```

### 1.3 Install zlib (Compression Library)
```bash
sudo apt-get install -y zlib1g-dev
```

### 1.4 Install Build Tools
```bash
sudo apt-get install -y build-essential autoconf automake libtool
```

### 1.5 Install Git (if not already installed)
```bash
sudo apt-get install -y git
```

---

## 2. Compiling CUDD Library from Source

CUDD (Colorado University Decision Diagram) is the core library for creating and managing BDDs.

### 2.1 Clone CUDD from GitHub
```bash
cd /home/ju44n/MeMayBeo
git clone https://github.com/ivmai/cudd.git cudd_source
cd cudd_source
```

### 2.2 Configure CUDD
```bash
./configure --enable-dddmp --enable-obj
```

**Configuration options:**
- `--enable-dddmp`: Enable DDDMP (Decision Diagram Dump/Move Package)
- `--enable-obj`: Enable C++ wrapper objects
- By default, `--enable-optimization` is included for optimization

### 2.3 Compile CUDD
```bash
make -j$(nproc)
```

**Notes:**
- `-j$(nproc)` uses all available CPU cores for faster compilation
- Compilation typically takes 5-10 minutes

### 2.4 Install CUDD (Optional)
```bash
sudo make install
# Or install to specific directory
make install DESTDIR=/home/ju44n/MeMayBeo/cudd300
```

### 2.5 Copy Compiled CUDD
```bash
cp -r cudd_source /home/ju44n/MeMayBeo/cudd300
```

---

## 3. Compiling the Deadlock Detection System

### 3.1 Project Structure
```
/home/ju44n/MeMayBeo/
├── Deadlock_Detection/
│   ├── main.cpp              # Program entry point
│   ├── Deadlock_ILP.cpp      # ILP-based deadlock detection
│   ├── Deadlock_ILP.h
│   ├── BDD_Reachability.cpp  # BDD-based reachability analysis
│   ├── BDD_Reachability.h
│   ├── PetriNet.cpp          # Petri Net data structure
│   ├── PetriNet.h
│   ├── tinyxml2.cpp          # PNML parsing
│   ├── tinyxml2.h
│   └── cuddObj.cc            # C++ wrapper for CUDD
├── PNML_Parser/
│   ├── pnml/                 # Test files directory
│   │   ├── simple/           # Simple test cases
│   │   ├── example/          # Example test cases
│   │   ├── medium/           # Medium complexity tests
│   │   └── large/            # Large complexity tests
│   └── [Parser files]
└── cudd300/                  # Compiled CUDD
```

### 3.2 Include and Library Paths
**Include paths:**
```
-I/home/ju44n/MeMayBeo/cudd300
-I/home/ju44n/MeMayBeo/cudd300/cudd
-I/home/ju44n/MeMayBeo/cudd300/st
-I/home/ju44n/MeMayBeo/cudd300/util
-I/home/ju44n/MeMayBeo/cudd300/epd
-I/home/ju44n/MeMayBeo/cudd300/mtr
-I/home/ju44n/MeMayBeo/cudd300/cplusplus
```

**Library paths:**
```
-L/home/ju44n/MeMayBeo/cudd300/cudd/.libs
-L/home/ju44n/MeMayBeo/cudd300/dddmp/.libs
-L/home/ju44n/MeMayBeo/cudd300/st/.libs
-L/home/ju44n/MeMayBeo/cudd300/util/.libs
-L/home/ju44n/MeMayBeo/cudd300/epd/.libs
-L/home/ju44n/MeMayBeo/cudd300/mtr/.libs
-L/home/ju44n/MeMayBeo/cudd300/cplusplus/.libs
```

### 3.3 Compilation Command
```bash
cd /home/ju44n/MeMayBeo/Deadlock_Detection

g++ main.cpp Deadlock_ILP.cpp BDD_Reachability.cpp PetriNet.cpp tinyxml2.cpp cuddObj.cc \
    -o detector.exe \
    -std=c++17 \
    -I/home/ju44n/MeMayBeo/cudd300 \
    -I/home/ju44n/MeMayBeo/cudd300/cudd \
    -I/home/ju44n/MeMayBeo/cudd300/st \
    -I/home/ju44n/MeMayBeo/cudd300/util \
    -I/home/ju44n/MeMayBeo/cudd300/epd \
    -I/home/ju44n/MeMayBeo/cudd300/mtr \
    -I/home/ju44n/MeMayBeo/cudd300/cplusplus \
    -L/home/ju44n/MeMayBeo/cudd300/cudd/.libs \
    -L/home/ju44n/MeMayBeo/cudd300/dddmp/.libs \
    -L/home/ju44n/MeMayBeo/cudd300/st/.libs \
    -L/home/ju44n/MeMayBeo/cudd300/util/.libs \
    -L/home/ju44n/MeMayBeo/cudd300/epd/.libs \
    -L/home/ju44n/MeMayBeo/cudd300/mtr/.libs \
    -L/home/ju44n/MeMayBeo/cudd300/cplusplus/.libs \
    -lglpk -lz -lcudd -ldddmp -lm
```

**Compilation flags:**
- `-std=c++17`: Use C++17 standard
- `-lglpk`: Link GLPK library
- `-lz`: Link zlib library
- `-lcudd`: Link CUDD library
- `-ldddmp`: Link DDDMP library
- `-lm`: Link math library

### 3.4 Verify Compilation Result
```bash
# Check if executable was created
ls -lh detector.exe

# Expected output:
# -rwxr-xr-x 1 user user 2.9M detector.exe
```

---

## 4. Running the System

### 4.1 Run on a Single Test File
```bash
cd /home/ju44n/MeMayBeo/Deadlock_Detection

# Run on a simple test file
./detector.exe /home/ju44n/MeMayBeo/test.pnml
```

**Expected output:**
```
Network loaded from: /home/ju44n/MeMayBeo/test.pnml
2 Places
2 Transitions
Total Reachable Markings: 2
Deadlock Found: No
```

### 4.2 Run All Tests
#### Method 1: Using Shell Script
```bash
#!/bin/bash

TEST_DIR="/home/ju44n/MeMayBeo/PNML_Parser/pnml"
DETECTOR="/home/ju44n/MeMayBeo/Deadlock_Detection/detector.exe"

for category in simple example medium large; do
    echo "Testing category: $category"
    for file in $TEST_DIR/$category/*.pnml; do
        if [ -f "$file" ]; then
            echo "Running: $file"
            $DETECTOR "$file"
            echo "---"
        fi
    done
done
```

#### Method 2: Using Python Script (Recommended)
```bash
cd /home/ju44n/MeMayBeo
python3 Deadlock_Detection/generate_aligned_report.py
```

### 4.3 Understanding the Output
```
Network loaded from: path/to/file.pnml      # Processed file name
2 Places                                     # Number of places
2 Transitions                                # Number of transitions
Total Reachable Markings: 2                  # Total reachable markings
Deadlock Found: No                           # Deadlock detection result
```

---

## 5. Complete Test Results

### 5.1 Overall Statistics
- **Total test files:** 50 PNML files
- **Successful tests:** 50/50 (100%)
- **Total execution time:** 0.78 seconds
- **Average time per test:** 0.0157 seconds

### 5.2 Network Complexity Range
- **Places (Locations):** 1 - 104
- **Transitions:** 1 - 66
- **Reachable Markings:** 1 - 729

### 5.3 Test Categories
1. **Simple** (10 files): Basic Petri Net examples
   - Execution time: 0.0079s - 0.0187s
   
2. **Example** (10 files): Standard example networks
   - Execution time: 0.0080s - 0.0205s
   
3. **Medium** (14 files): Medium complexity networks
   - Execution time: 0.0085s - 0.0651s
   
4. **Large** (16 files): Complex, large networks
   - Execution time: 0.0086s - 0.1125s

### 5.4 View Detailed Results
```bash
# View complete test report
cat /home/ju44n/MeMayBeo/TEST_REPORT_TABLE.txt

# View first 15 lines
head -15 /home/ju44n/MeMayBeo/TEST_REPORT_TABLE.txt

# View final statistics
tail -5 /home/ju44n/MeMayBeo/TEST_REPORT_TABLE.txt
```

---

## 6. Troubleshooting and Problem Solving

### 6.1 Error: "CUDD header not found"
```bash
# Check if CUDD is compiled
ls -la /home/ju44n/MeMayBeo/cudd300/cudd/cudd.h

# If not found, recompile CUDD
cd /home/ju44n/MeMayBeo/cudd300
./configure --enable-dddmp --enable-obj
make -j$(nproc)
```

### 6.2 Error: "undefined reference to glpk"
```bash
# Check if GLPK is installed
dpkg -l | grep glpk

# If not found, reinstall
sudo apt-get install -y libglpk-dev
```

### 6.3 Error: "relocation error"
```bash
# Ensure libraries are linked in correct order
# Try recompiling with the command above

# If error persists, clean old object files
cd /home/ju44n/MeMayBeo/Deadlock_Detection
rm -f *.o *.a detector.exe
# Recompile
```

### 6.4 Verify Executable
```bash
# Check executable file
file /home/ju44n/MeMayBeo/Deadlock_Detection/detector.exe

# Expected output:
# detector.exe: ELF 64-bit LSB executable, x86-64, ...

# Run simple test
./detector.exe /home/ju44n/MeMayBeo/test.pnml
```

---

## 7. Performance Optimization

### 7.1 Compile with Optimization
```bash
# Add optimization flags
g++ ... -O3 ...

# Or with CUDD
cd /home/ju44n/MeMayBeo/cudd300
./configure --enable-dddmp --enable-obj --enable-optimization
make -j$(nproc)
```

### 7.2 Performance Testing
```bash
# Run on the largest network
./detector.exe /home/ju44n/MeMayBeo/PNML_Parser/pnml/simple/family.pnml

# Measure execution time (104 places, 66 transitions)
time ./detector.exe /home/ju44n/MeMayBeo/PNML_Parser/pnml/simple/family.pnml
```

---

## 8. Program Architecture

### 8.1 Execution Flow
```
Input: PNML file
   ↓
1. PNML Parser (tinyxml2)
   ├─ Read XML file
   ├─ Parse structure
   └─ Create Petri Net structure
   ↓
2. BDD Reachability Analysis
   ├─ Create BDD for states
   ├─ Compute all reachable markings
   └─ Build state space
   ↓
3. ILP-BDD Deadlock Detection
   ├─ Check deadlock-free property
   ├─ Detect deadlock loops
   └─ Verify with ILP
   ↓
Output: Deadlock detection result
```

### 8.2 Main Components
- **main.cpp**: Orchestrates main process flow
- **PetriNet.cpp**: Represents Petri Net structure
- **BDD_Reachability.cpp**: Reachability analysis using BDD
- **Deadlock_ILP.cpp**: Deadlock detection using ILP
- **tinyxml2.cpp**: PNML file parsing

---

## 9. References

- CUDD Documentation: https://github.com/ivmai/cudd
- GLPK Manual: https://www.gnu.org/software/glpk/
- Petri Net Theory: https://en.wikipedia.org/wiki/Petri_net
- PNML Standard: http://www.pnml.org/

---

## 10. Summary

The system has been successfully:
✅ Installed all required dependencies
✅ Compiled CUDD from source
✅ Compiled deadlock detection system
✅ Run complete test suite of 50 files
✅ Achieved 100% success rate
✅ Processed networks from 1 to 104 places, 1 to 66 transitions

**System is ready for production use!**
