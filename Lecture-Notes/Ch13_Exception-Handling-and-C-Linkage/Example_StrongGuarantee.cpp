/**
 * @file    Example_StrongGuarantee.cpp
 * @brief   Compares an assignment that leaves an object half-changed with a copy-and-swap assignment that gives the strong guarantee
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
using namespace std;

static int budget = 100;                         // simulated memory: allocations fail when it runs out

string* allocate(const string& s) {
    if (static_cast<int>(s.size()) > budget)
        throw bad_alloc();
    budget -= static_cast<int>(s.size());
    return new string(s);
}

class Pair {
    string* first;
    string* second;
public:
    Pair(const string& a, const string& b) : first(allocate(a)), second(nullptr) {
        try { second = allocate(b); }
        catch (...) { delete first; throw; }     // the constructor cleans up what it made
    }
    Pair(const Pair& o) : Pair(*o.first, *o.second) {}
    Pair& operator=(const Pair&) = delete;       // assignment goes through assignUnsafe() or assignStrong()
    ~Pair() { delete first; delete second; }

    void assignUnsafe(const Pair& o) {           // basic guarantee at best
        delete first;
        first = allocate(*o.first);              // succeeds
        delete second;
        second = nullptr;
        second = allocate(*o.second);            // may throw: *this is now half new, half empty
    }
    void assignStrong(const Pair& o) {           // strong guarantee
        Pair temp(o);                            // every step that may throw happens here
        swap(first, temp.first);                 // swapping pointers cannot throw
        swap(second, temp.second);
    }                                            // temp releases the old contents
    void print(const char* label) const {
        cout << label << ": " << *first << ", " << (second ? *second : string("<empty>")) << endl;
    }
};

int main() {
    Pair p("red", "blue");
    Pair big("green", string(60, 'x'));          // uses 65 of the budget
    budget = 10;                                 // only small allocations succeed from now on

    try { p.assignStrong(big); }
    catch (const bad_alloc&) { cout << "assignStrong failed" << endl; }
    p.print("after assignStrong");

    try { p.assignUnsafe(big); }
    catch (const bad_alloc&) { cout << "assignUnsafe failed" << endl; }
    p.print("after assignUnsafe");
    return 0;
}
