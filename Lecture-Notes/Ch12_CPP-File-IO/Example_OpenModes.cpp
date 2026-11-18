/**
 * @file    Example_OpenModes.cpp
 * @brief   Compares the file modes out, app, in | out, and ate on the same file
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void show(const char* label) {
    ifstream fin("mode.txt");
    string content, line;
    while (getline(fin, line))
        content += line + "/";
    cout << label << ": " << content << endl;
}

int main() {
    { ofstream f("mode.txt"); f << "one\ntwo\n"; }             // out: truncates, then writes
    show("out        ");
    { ofstream f("mode.txt", ios::app); f << "three\n"; }      // app: every write goes to the end
    show("app        ");
    { fstream f("mode.txt", ios::in | ios::out); f << "ONE"; } // in | out: keeps the contents
    show("in | out   ");
    { ofstream f("mode.txt", ios::out | ios::ate); f << "X\n"; } // out | ate: still truncates
    show("out | ate  ");
    { fstream f("missing.txt", ios::in | ios::out);
      cout << "in | out on a missing file opens: " << f.is_open() << endl; }
    return 0;
}
