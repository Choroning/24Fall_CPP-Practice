/**
 * @file    Example_PointIO.cpp
 * @brief   Overloads the insertion and extraction operators for a Point written as (x, y)
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <sstream>
using namespace std;

class Point {
    int x = 0, y = 0;
public:
    Point() = default;
    Point(int x, int y) : x(x), y(y) {}
    friend ostream& operator<<(ostream& out, const Point& p);
    friend istream& operator>>(istream& in, Point& p);
};

ostream& operator<<(ostream& out, const Point& p) {
    out << '(' << p.x << ", " << p.y << ')';
    return out;                                  // returning the stream allows chaining
}

istream& operator>>(istream& in, Point& p) {
    char open, comma, close;
    int x, y;
    if (in >> open >> x >> comma >> y >> close) {   // >> skips spaces before each part
        if (open == '(' && comma == ',' && close == ')') {
            p = Point(x, y);                     // changes p only after a complete read
            return in;
        }
        in.setstate(ios::failbit);               // the shape is wrong: report failure
    }
    return in;
}

int main() {
    Point a(3, 4);
    cout << "a = " << a << endl;

    istringstream input("(1, 2) ( 10 ,20 ) [5, 6] (7, 8)");
    Point p;
    while (input >> p)                           // stops at "[5, 6]"
        cout << "read " << p << endl;
    cout << "fail=" << input.fail() << ", last good value " << p << endl;
    return 0;
}
