/**
 * @file    Example_ModernBasics.cpp
 * @brief   nullptr, enum class, brace initialization and initializer_list, constexpr, and deleted functions
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <cstddef>
#include <iostream>
#include <vector>
using namespace std;

void f(int)   { cout << "f(int)" << endl; }
void f(char*) { cout << "f(char*)" << endl; }

enum Color { Red, Green };                       // unscoped: names leak into the enclosing scope
enum class Signal : unsigned char { Red, Yellow, Green };   // scoped, with a chosen underlying type

constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }

struct Meters {
    double value;
    explicit Meters(double v) : value(v) {}
    Meters(int) = delete;                        // forbids Meters(3) so that units stay explicit
};

int main() {
    f(0);                                        // 0 is an int
    f(nullptr);                                  // nullptr converts only to pointer types
    // f(NULL);                                  // NULL may be 0 or 0L: ambiguous or f(int)

    Signal s = Signal::Yellow;
    int c = Green;                               // unscoped enum converts to int silently
    int code = static_cast<int>(s);              // scoped enum needs an explicit cast
    cout << "Green=" << c << ", Signal::Yellow=" << code << ", sizeof(Signal)=" << sizeof(Signal) << endl;

    vector<int> a(3, 7);                         // constructor (count, value): 7 7 7
    vector<int> b{3, 7};                         // initializer_list: 3 7
    cout << "a.size()=" << a.size() << ", b.size()=" << b.size() << endl;

    int x{5};
    double d{2.5};
    // int y{d};                                 // error: narrowing from double to int
    int y = static_cast<int>(d);
    cout << "x=" << x << ", y=" << y << endl;

    constexpr int f5 = factorial(5);             // computed by the compiler
    static_assert(f5 == 120, "factorial(5) must be 120");
    int arr[factorial(4)];                       // a constant expression can size an array
    cout << "factorial(5)=" << f5 << ", array length " << sizeof(arr) / sizeof(arr[0]) << endl;

    int n = 6;
    cout << "runtime factorial(6)=" << factorial(n) << endl;     // also callable at run time

    Meters m(2.5);
    // Meters bad(3);                            // error: the int constructor is deleted
    cout << "meters " << m.value << endl;
    return 0;
}
