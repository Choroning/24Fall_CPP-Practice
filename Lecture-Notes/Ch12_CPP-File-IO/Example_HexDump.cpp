/**
 * @file    Example_HexDump.cpp
 * @brief   Writes the same text in text mode and binary mode, then prints the bytes of both files
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

void dump(const char* path) {
    ifstream fin(path, ios::in | ios::binary);   // binary mode shows the bytes as stored
    if (!fin) {
        cerr << "cannot open " << path << endl;
        return;
    }
    cout << path << ':';
    int count = 0;
    char c;
    while (fin.get(c)) {
        cout << ' ' << hex << setw(2) << setfill('0')
             << static_cast<int>(static_cast<unsigned char>(c));
        count++;
    }
    cout << dec << setfill(' ') << "  (" << count << " bytes)" << endl;
}

int main() {
    ofstream text("text.txt");                   // text mode is the default
    text << "Hi\nC++\n";
    text.close();

    ofstream bin("binary.txt", ios::out | ios::binary);
    bin << "Hi\nC++\n";
    bin.close();

    dump("text.txt");
    dump("binary.txt");
    return 0;
}
