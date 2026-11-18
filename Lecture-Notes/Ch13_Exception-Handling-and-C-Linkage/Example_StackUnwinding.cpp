/**
 * @file    Example_StackUnwinding.cpp
 * @brief   Prints the constructors and destructors that run while an exception leaves nested calls
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Tracer {
    string label;
public:
    explicit Tracer(string s) : label(s) { cout << "  construct " << label << endl; }
    ~Tracer() { cout << "  destroy   " << label << endl; }
};

void level3(int n) {
    Tracer t("t3");
    if (n < 0)
        throw invalid_argument("negative input");
    cout << "  level3 finished" << endl;          // skipped when n < 0
}

void level2(int n) {
    Tracer t("t2");
    level3(n);
    cout << "  level2 finished" << endl;
}

int main() {
    Tracer t0("t0");
    try {
        Tracer t1("t1");
        level2(-5);
        cout << "  main continues inside try" << endl;
    } catch (const invalid_argument& e) {
        cout << "  caught: " << e.what() << endl;
    }
    cout << "  after catch" << endl;
    return 0;
}
