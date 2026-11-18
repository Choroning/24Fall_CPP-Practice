/**
 * @file    Example_ThrowCatch.cpp
 * @brief   Traces the control flow of try, throw, and catch, including a throw inside a called function
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
using namespace std;

int divide(int a, int b) {
    if (b == 0)
        throw b;                                 // leaves divide() at once; the int 0 is thrown
    return a / b;
}

int main() {
    int pairs[3][2] = {{10, 2}, {7, 0}, {9, 3}};
    for (auto& p : pairs) {
        cout << "try " << p[0] << " / " << p[1] << endl;
        try {
            int q = divide(p[0], p[1]);
            cout << "  quotient " << q << endl;  // skipped when divide() throws
        } catch (int divisor) {                  // receives a copy of the thrown int
            cout << "  caught: divisor " << divisor << endl;
        }
        cout << "  after the try-catch" << endl;
    }
    return 0;
}
