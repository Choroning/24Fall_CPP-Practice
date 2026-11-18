/**
 * @file    Example_LineNumbers.cpp
 * @brief   Copies a text file character by character with get() and put(), then numbers its lines with getline()
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    const char* src = (argc > 1) ? argv[1] : "poem.txt";
    if (argc == 1) {                             // sample input when no file is given, no final '\n'
        ofstream make(src);
        make << "roses are red\n\nviolets are blue";
    }

    ifstream fin(src);
    ofstream fout("copy.txt");
    if (!fin || !fout) {
        cerr << "cannot open the files" << endl;
        return 1;
    }
    int ch, chars = 0;
    while ((ch = fin.get()) != EOF) {            // get() returns int so that EOF stays distinct
        fout.put(static_cast<char>(ch));
        chars++;
    }
    fin.close();
    fout.close();
    cout << "copied " << chars << " characters" << endl;

    ifstream again("copy.txt");
    string line;
    int number = 0;
    size_t longest = 0;
    while (getline(again, line)) {               // the last line counts even without '\n'
        number++;
        if (line.size() > longest)
            longest = line.size();
        cout << number << ": " << line << endl;
    }
    cout << number << " lines, longest " << longest << " characters" << endl;
    return 0;
}
