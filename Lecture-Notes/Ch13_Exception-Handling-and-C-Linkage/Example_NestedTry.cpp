/**
 * @file    Example_NestedTry.cpp
 * @brief   Traces the handler search through nested try blocks, including a new exception thrown from a handler
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <stdexcept>
using namespace std;

void inner() {
    try {
        throw out_of_range("index 9");
    } catch (const invalid_argument&) {          // does not match out_of_range
        cout << "inner handler" << endl;
    }
    cout << "inner end" << endl;                 // skipped: the exception left inner()
}

void middle() {
    try {
        inner();
    } catch (const out_of_range& e) {
        cout << "middle caught: " << e.what() << endl;
        throw runtime_error("middle gave up");   // a new exception from inside a handler
    } catch (const runtime_error& e) {           // a sibling handler never sees it
        cout << "middle sibling: " << e.what() << endl;
    }
}

int main() {
    try {
        middle();
    } catch (const out_of_range&) {
        cout << "main: out_of_range" << endl;
    } catch (const runtime_error& e) {
        cout << "main caught: " << e.what() << endl;
    }
    return 0;
}
