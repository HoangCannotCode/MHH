#include <iostream>
#include <vector>
#include <set>
#include "PetriNet.h"
#include "BFS.h"
#include "DFS.h"
using namespace std;
void printMarking(const Marking& m) {
    cout << "(";
    for (int i = 0; i < m.size(); i++) {
        cout << m[i];
        if (i + 1 < m.size()) cout << ",";
    }
    cout << ")";
}

void assertEqual(const MarkingSet& out, const MarkingSet& expected, const string& testName) {
    if (out == expected) {
        cout << "[PASS] " << testName << "\n";
        return;
    }

    cout << "[FAIL] " << testName << "\n";
    cout << "Expected: ";
    for (auto& m : expected) { printMarking(m); cout << " "; }
    cout << "\nGot:      ";
    for (auto& m : out) { printMarking(m); cout << " "; }
    cout << "\n\n";
}

/* ===============================
        TEST CASES
   =============================== */

// Test BFS 1
void test_BFS_1() {
    PetriNet pn(
        {1,0,0},
        {{1,0,0},{0,1,0},{0,0,1}},
        {{0,1,0},{0,0,1},{1,0,0}}
    );

    MarkingSet expected = {
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };

    assertEqual(bfs_reachable(pn), expected, "BFS Test 1");
}

// Test BFS 2
void test_BFS_2() {
    PetriNet pn(
        {1,0},
        {{1,0},{0,1}},
        {{0,1},{1,0}}
    );

    MarkingSet expected = {
        {1,0},
        {0,1}
    };

    assertEqual(bfs_reachable(pn), expected, "BFS Test 2");
}

// Test BFS 3 — Zero-marking case
void test_BFS_3() {
    PetriNet pn(
        {0,0},
        {{1,0},{0,1}},
        {{0,1},{1,0}}
    );

    MarkingSet expected = {
        {0,0}
    };

    assertEqual(bfs_reachable(pn), expected, "BFS Test 3");
}

// ===============================
// DFS TESTS
// ===============================

void test_DFS_1() {
    PetriNet pn(
        {1,0,0},
        {{1,0,0},{0,1,0},{0,0,1}},
        {{0,1,0},{0,0,1},{1,0,0}}
    );

    MarkingSet expected = {
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };

    assertEqual(dfs_reachable(pn), expected, "DFS Test 1");
}

void test_DFS_2() {
    PetriNet pn(
        {1,0},
        {{1,0},{0,1}},
        {{0,1},{1,0}}
    );

    MarkingSet expected = {
        {1,0},
        {0,1}
    };

    assertEqual(dfs_reachable(pn), expected, "DFS Test 2");
}

void test_DFS_3() {
    PetriNet pn(
        {0,0},
        {{1,0},{0,1}},
        {{0,1},{1,0}}
    );

    MarkingSet expected = {
        {0,0}
    };

    assertEqual(dfs_reachable(pn), expected, "DFS Test 3");
}

/* ===============================
            MAIN
   =============================== */

int main() {
    cout << "====== Running BFS Tests ======\n";
    test_BFS_1();
    test_BFS_2();
    test_BFS_3();

    cout << "\n====== Running DFS Tests ======\n";
    test_DFS_1();
    test_DFS_2();
    test_DFS_3();

    cout << "\nAll tests completed.\n";
    return 0;
}
