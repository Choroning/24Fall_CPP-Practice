/**
 * @file    Example_Forwarding.cpp
 * @brief   Forwarding references and std::forward keep the value category of an argument; emplace_back builds in place
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

void sink(const string&) { cout << "copy path" << endl; }
void sink(string&&)      { cout << "move path" << endl; }

template <typename T>
void relayWrong(T&& arg) { sink(arg); }                      // arg has a name: always an lvalue

template <typename T>
void relay(T&& arg) { sink(std::forward<T>(arg)); }          // restores the original category

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) { cout << "  Point(" << x << ", " << y << ")" << endl; }
    Point(const Point& o) : x(o.x), y(o.y) { cout << "  copy" << endl; }
    Point(Point&& o) noexcept : x(o.x), y(o.y) { cout << "  move" << endl; }
};

int main() {
    string s = "data";
    cout << "relayWrong(s):            "; relayWrong(s);
    cout << "relayWrong(string(\"t\")):  "; relayWrong(string("t"));
    cout << "relay(s):                 "; relay(s);
    cout << "relay(string(\"t\")):       "; relay(string("t"));

    vector<Point> v;
    v.reserve(4);
    cout << "push_back(Point(1, 2)):" << endl;
    v.push_back(Point(1, 2));                    // builds a temporary, then moves it
    cout << "emplace_back(3, 4):" << endl;
    v.emplace_back(3, 4);                        // forwards 3 and 4 to the constructor
    return 0;
}
