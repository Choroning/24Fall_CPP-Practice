/**
 * @file    Example_RaiiCleanup.cpp
 * @brief   A raw new leaks when an exception skips the delete; a unique_ptr releases the object during stack unwinding
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <memory>
#include <stdexcept>
using namespace std;

struct Resource {
    int id;
    explicit Resource(int i) : id(i) { cout << "  acquire " << id << endl; }
    ~Resource() { cout << "  release " << id << endl; }
};

void work(bool fail) {
    if (fail)
        throw runtime_error("work failed");
}

void rawVersion(bool fail) {
    Resource* r = new Resource(1);
    work(fail);                                  // if this throws, the delete below is skipped
    delete r;
}

void raiiVersion(bool fail) {
    auto r = make_unique<Resource>(2);           // the destructor of r runs during unwinding
    work(fail);
}

int main() {
    try { rawVersion(true); }
    catch (const exception& e) { cout << "caught: " << e.what() << " (resource 1 leaked)" << endl; }

    try { raiiVersion(true); }
    catch (const exception& e) { cout << "caught: " << e.what() << endl; }
    return 0;
}
