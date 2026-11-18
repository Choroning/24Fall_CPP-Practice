/**
 * @file    Example_StreamExceptions.cpp
 * @brief   Makes a file stream throw ios_base::failure with exceptions() instead of only setting state bits
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream fin;
    fin.exceptions(ios::failbit | ios::badbit);  // these bits now raise an exception
    try {
        fin.open("no_such_file.txt");            // open failure sets failbit, so it throws
        cout << "opened" << endl;
    } catch (const ios_base::failure& e) {
        cout << "caught ios_base::failure" << endl;
        cout << "state: fail=" << fin.fail() << ", is_open=" << fin.is_open() << endl;
    }

    { ofstream make("two.txt"); make << "7 8"; }
    ifstream in("two.txt");
    in.exceptions(ios::badbit);                  // only a lost stream throws
    int a, b, c = -1;
    in >> a >> b >> c;                           // the third read fails but does not throw
    cout << a << ' ' << b << ' ' << c << " fail=" << in.fail() << endl;
    return 0;
}
