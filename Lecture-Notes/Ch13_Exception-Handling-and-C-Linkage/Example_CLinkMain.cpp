/**
 * @file    Example_CLinkMain.cpp
 * @brief   Calls functions compiled by a C compiler from C++ through a header with extern "C"
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include "Example_CMath.h"
using namespace std;

int square(int x) { return x * x; }              // C++ linkage: the name is mangled
double square(double x) { return x * x; }        // an overload gets a different mangled name

int main() {
    int scores[] = {90, 85, 77};
    cout << "add(3, 5) = " << add(3, 5) << endl;
    cout << "average = " << average(scores, 3) << endl;
    cout << "square(4) = " << square(4) << ", square(1.5) = " << square(1.5) << endl;
    cout << "__cplusplus = " << __cplusplus << endl;
    return 0;
}
