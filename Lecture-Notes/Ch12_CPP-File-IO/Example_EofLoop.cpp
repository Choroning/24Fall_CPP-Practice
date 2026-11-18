/**
 * @file    Example_EofLoop.cpp
 * @brief   Shows why while (!fin.eof()) processes the last value twice and gives the correct loop
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <fstream>
using namespace std;

int main() {
    { ofstream f("numbers.txt"); f << "10\n20\n30\n"; }

    ifstream bad("numbers.txt");
    int value = 0, sum = 0;
    while (!bad.eof()) {                         // wrong: eof() is false until a read hits the end
        bad >> value;
        sum += value;
        cout << "read " << value << " (eof=" << bad.eof() << ", fail=" << bad.fail() << ")" << endl;
    }
    cout << "wrong loop sum = " << sum << endl;

    ifstream good("numbers.txt");
    sum = 0;
    while (good >> value)                        // right: test the result of the read itself
        sum += value;
    cout << "correct loop sum = " << sum << endl;
    if (good.bad())
        cout << "stopped by an I/O error" << endl;
    else if (good.eof())
        cout << "stopped at the end of the file" << endl;
    else
        cout << "stopped at data that is not an integer" << endl;
    return 0;
}
