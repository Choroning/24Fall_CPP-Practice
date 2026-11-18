/**
 * @file    Example_ValueCategories.cpp
 * @brief   Shows which reference overload each expression selects: lvalue, const lvalue, or rvalue
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <string>
#include <utility>
using namespace std;

void take(string&)        { cout << "string&       (modifiable lvalue)" << endl; }
void take(const string&)  { cout << "const string& (const lvalue)" << endl; }
void take(string&&)       { cout << "string&&      (rvalue)" << endl; }

string make() { return "temp"; }

int main() {
    string s = "name";
    const string cs = "fixed";
    string&& r = make();                         // r is a named rvalue reference

    cout << "take(s)            -> "; take(s);
    cout << "take(cs)           -> "; take(cs);
    cout << "take(make())       -> "; take(make());
    cout << "take(s + \"!\")      -> "; take(s + "!");
    cout << "take(r)            -> "; take(r);            // a named variable is an lvalue
    cout << "take(std::move(s)) -> "; take(std::move(s)); // move only casts to an rvalue
    cout << "s after the call   = " << s << endl;         // nothing was moved out of s
    return 0;
}
