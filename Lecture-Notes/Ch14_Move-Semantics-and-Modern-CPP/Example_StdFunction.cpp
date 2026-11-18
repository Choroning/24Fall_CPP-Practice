/**
 * @file    Example_StdFunction.cpp
 * @brief   Stores a function, a function object, and lambdas in std::function and uses them as callbacks
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

int add(int a, int b) { return a + b; }

struct Multiply {                                // a function object: a class with operator()
    int calls = 0;
    int operator()(int a, int b) { calls++; return a * b; }
};

class Button {
    vector<function<void(const string&)>> handlers;
public:
    void onClick(function<void(const string&)> h) { handlers.push_back(std::move(h)); }
    void click(const string& who) const {
        for (const auto& h : handlers)
            h(who);
    }
};

int main() {
    map<string, function<int(int, int)>> ops;    // one type for different callables
    ops["+"] = add;
    ops["*"] = Multiply();
    ops["-"] = [](int a, int b) { return a - b; };
    int base = 10;
    ops["b"] = [base](int a, int b) { return base * a + b; };   // captures a copy of base
    for (const auto& [symbol, f] : ops)
        cout << "7 " << symbol << " 3 = " << f(7, 3) << endl;

    int count = 0;
    auto counter = [count]() mutable { return ++count; };       // changes its own copy
    cout << "counter: " << counter() << ' ' << counter() << ", outer count " << count << endl;

    auto p = make_unique<int>(5);
    auto owner = [q = std::move(p)]() { return *q * 2; };       // init capture moves p in
    cout << "init capture: " << owner() << ", p is " << (p ? "not null" : "null") << endl;

    auto twice = [](auto x) { return x + x; };   // generic lambda (C++14)
    cout << "generic: " << twice(4) << ' ' << twice(string("ab")) << endl;

    Button b;
    int clicks = 0;
    b.onClick([](const string& who) { cout << "  hello, " << who << endl; });
    b.onClick([&clicks](const string&) { clicks++; });
    b.click("Kim");
    b.click("Lee");
    cout << "clicks = " << clicks << endl;

    function<int(int, int)> empty;
    try {
        empty(1, 2);                             // calling an empty std::function throws
    } catch (const bad_function_call& e) {
        cout << "bad_function_call" << endl;
    }
    return 0;
}
