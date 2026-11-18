/**
 * @file    Example_ScoreFile.cpp
 * @brief   Writes records to a text file with << and reads them back with >>
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ofstream fout("scores.txt");                 // creates the file, or empties an existing one
    if (!fout) {
        cerr << "cannot create scores.txt" << endl;
        return 1;
    }
    fout << "Kim 90\n" << "Lee 85\n" << "Park 77\n";
    fout.close();                                // flushes the buffer and closes the file

    ifstream fin("scores.txt");
    if (!fin.is_open()) {
        cerr << "cannot open scores.txt" << endl;
        return 1;
    }
    string name;
    int score, total = 0, count = 0;
    while (fin >> name >> score) {               // the loop ends when a read fails
        cout << name << " -> " << score << endl;
        total += score;
        count++;
    }
    cout << "count=" << count << ", total=" << total << endl;
    cout << "eof=" << fin.eof() << ", fail=" << fin.fail() << endl;
    return 0;
}
