/**
 * @file    Example_Noexcept.cpp
 * @brief   Uses the noexcept operator and shows that an exception leaving a noexcept function calls std::terminate
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <stdexcept>
#include <type_traits>
using namespace std;

int twice(int x) noexcept { return 2 * x; }
int parse(int x) { if (x < 0) throw invalid_argument("negative"); return x; }

void careless(int x) noexcept {
    cout << "careless(" << x << ")" << endl;
    parse(x);                                    // may throw inside a noexcept function
}

struct Quiet { ~Quiet() {} };                    // destructors are noexcept by default

int main(int argc, char*[]) {
    cout << boolalpha;
    cout << "noexcept(twice(1)) = " << noexcept(twice(1)) << endl;
    cout << "noexcept(parse(1)) = " << noexcept(parse(1)) << endl;
    cout << "is_nothrow_destructible<Quiet> = " << is_nothrow_destructible<Quiet>::value << endl;

    try {
        careless(argc > 1 ? -1 : 1);             // run with an argument to make it throw
        cout << "no exception" << endl;
    } catch (...) {
        cout << "never reached: the catch cannot stop terminate" << endl;
    }
    return 0;
}
