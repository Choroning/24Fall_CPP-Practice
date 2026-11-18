/**
 * @file    Example_UserManipulators.cpp
 * @brief   Defines manipulators without arguments, a manipulator with arguments, and an input manipulator
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

ostream& bar(ostream& out) {                     // a manipulator is a function of this shape
    return out << " | ";
}

ostream& money(ostream& out) {                   // changes several settings at once
    out.setf(ios::fixed, ios::floatfield);
    out.precision(2);
    return out << '$';
}

class Repeat {                                   // a manipulator with arguments needs an object
    char ch;
    int count;
public:
    Repeat(char ch, int count) : ch(ch), count(count) {}
    friend ostream& operator<<(ostream& out, const Repeat& r) {
        for (int i = 0; i < r.count; i++)
            out.put(r.ch);
        return out;
    }
};

istream& skipComment(istream& in) {              // an input manipulator: drops "# ... \n"
    in >> ws;                                    // ws discards leading whitespace
    while (in.peek() == '#') {
        in.ignore(1000, '\n');
        in >> ws;
    }
    return in;
}

int main() {
    cout << "name" << bar << "score" << bar << "rank" << endl;
    cout << Repeat('=', 20) << endl;
    cout << money << 1234.5 << endl;
    cout.unsetf(ios::floatfield);
    cout.precision(6);

    istringstream config("# width setting\n# second comment\n  80\n");
    int width;
    config >> skipComment >> width;
    cout << "width = " << width << endl;
    return 0;
}
