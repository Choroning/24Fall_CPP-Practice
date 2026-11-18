/**
 * @file    Example_FormatFlags.cpp
 * @brief   Controls output format with setf(), unsetf(), width(), fill(), and precision()
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
using namespace std;

int main() {
    cout << 255 << ' ';
    cout.setf(ios::hex, ios::basefield);         // replaces dec with hex inside basefield
    cout << 255 << ' ';
    cout.setf(ios::showbase | ios::uppercase);   // adds two flags, keeps the others
    cout << 255 << endl;
    cout.unsetf(ios::showbase | ios::uppercase);
    cout.setf(ios::dec, ios::basefield);

    cout.width(8);                               // applies to the next output only
    cout << 42 << '|' << 42 << '|' << endl;
    cout.fill('.');
    cout.setf(ios::left, ios::adjustfield);
    cout.width(8);
    cout << "Kim" << '|' << endl;
    cout.fill(' ');
    cout.setf(ios::right, ios::adjustfield);

    double pi = 3.14159265358979;
    cout << pi << ' ' << 1234567.0 << endl;      // general notation, 6 significant digits
    cout.precision(3);
    cout << pi << ' ' << 1234567.0 << endl;      // 3 significant digits
    cout.setf(ios::fixed, ios::floatfield);
    cout << pi << ' ' << 1234567.0 << endl;      // 3 digits after the decimal point
    cout.setf(ios::scientific, ios::floatfield);
    cout << pi << ' ' << 1234567.0 << endl;      // 3 digits after the point, with an exponent
    cout.unsetf(ios::floatfield);                // back to general notation

    ios::fmtflags saved = cout.flags();          // saves every format flag
    streamsize savedPrecision = cout.precision();
    cout.setf(ios::showpos | ios::showpoint);
    cout.precision(5);
    cout << 2.5 << ' ' << 10 << endl;
    cout.flags(saved);                           // restores the flags
    cout.precision(savedPrecision);
    cout << 2.5 << ' ' << 10 << endl;
    return 0;
}
