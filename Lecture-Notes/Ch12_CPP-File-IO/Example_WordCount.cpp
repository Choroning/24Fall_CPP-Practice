/**
 * @file    Example_WordCount.cpp
 * @brief   Counts the lines, words, and characters of a text file, like the wc command, with getline() and istringstream
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    const char* path = (argc > 1) ? argv[1] : "words.txt";
    if (argc == 1) {                             // sample input when no file is given
        ofstream make(path);
        make << "the quick brown fox\n  jumps over\n\nthe lazy dog\n";
    }
    ifstream fin(path);
    if (!fin) {
        cerr << "cannot open " << path << endl;
        return 1;
    }
    long lines = 0, words = 0, chars = 0;
    string line, longest;
    while (getline(fin, line)) {
        lines++;
        chars += line.size() + 1;                // + 1 for the '\n' that getline() discarded
        istringstream in(line);
        string w;
        int inLine = 0;
        while (in >> w) {                        // >> splits the line at whitespace
            inLine++;
            if (w.size() > longest.size())
                longest = w;
        }
        words += inLine;
        cout << "line " << lines << ": " << inLine << " words" << endl;
    }
    if (fin.eof() && !line.empty())              // the last line had no '\n'
        chars--;
    cout << lines << ' ' << words << ' ' << chars << " " << path << endl;
    cout << "longest word: " << longest << endl;
    return 0;
}
