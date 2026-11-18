/**
 * @file    Example_StandardExceptions.cpp
 * @brief   Triggers several standard exceptions and defines an exception class derived from std::runtime_error
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <new>
#include <typeinfo>
#include <limits>
using namespace std;

class InsufficientFunds : public runtime_error {   // a user-defined exception class
    double shortage;
public:
    InsufficientFunds(double need, double have)
        : runtime_error("insufficient funds"), shortage(need - have) {}
    double amount() const noexcept { return shortage; }
};

class Account {
    double balance = 100.0;
public:
    void withdraw(double amount) {
        if (amount <= 0)
            throw invalid_argument("amount must be positive");
        if (amount > balance)
            throw InsufficientFunds(amount, balance);
        balance -= amount;
    }
    double get() const { return balance; }
};

struct Shape { virtual ~Shape() = default; };
struct Circle : Shape {};
struct Square : Shape {};

int main() {
    vector<int> v(3);
    try { v.at(5) = 1; }
    catch (const out_of_range& e) { cout << "out_of_range" << endl; }

    try {
        int a = stoi("12x");                     // converts the leading "12" and stops at 'x'
        cout << "stoi(\"12x\") = " << a << endl;
        int b = stoi("abc");                     // no digits at all
        cout << b << endl;
    } catch (const invalid_argument& e) { cout << "invalid_argument from stoi(\"abc\")" << endl; }

    try {
        int c = stoi("99999999999");             // does not fit in int
        cout << c << endl;
    } catch (const out_of_range& e) { cout << "out_of_range from stoi(\"99999999999\")" << endl; }

    try {
        Square sq;
        Shape& s = sq;
        Circle& c = dynamic_cast<Circle&>(s);    // a reference cast that fails throws
        (void)c;
    } catch (const bad_cast& e) { cout << "bad_cast" << endl; }

    try {
        size_t huge = numeric_limits<size_t>::max() / 4;
        int* p = new int[huge];                  // the request cannot be satisfied
        delete[] p;
    } catch (const bad_alloc& e) { cout << "bad_alloc" << endl; }

    Account acc;
    double requests[] = {30, -5, 200};
    for (double r : requests) {
        try {
            acc.withdraw(r);
            cout << "withdrew " << r << ", balance " << acc.get() << endl;
        } catch (const InsufficientFunds& e) {   // the most derived class comes first
            cout << e.what() << ": short by " << e.amount() << endl;
        } catch (const logic_error& e) {
            cout << "logic_error: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "other: " << e.what() << endl;
        }
    }
    return 0;
}
