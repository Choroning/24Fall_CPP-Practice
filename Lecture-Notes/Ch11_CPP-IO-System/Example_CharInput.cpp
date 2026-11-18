/**
 * @file    Example_CharInput.cpp
 * @brief   Reads characters, words, and lines with get(), getline(), ignore(), peek(), and gcount()
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    // Expected input (three lines):
    //   C++ IO
    //   2024 fall
    //   Kim Minsu
    int ch;
    int count = 0;
    while ((ch = cin.get()) != '\n' && ch != EOF) {   // reads every character, spaces included
        cout.put(ch);
        count++;
    }
    cout << " (" << count << " characters)" << endl;

    int year;
    cin >> year;                         // stops at the space after 2024
    string word;
    cin >> word;                         // skips the space, reads "fall", stops at '\n'
    cout << "year=" << year << ", word=" << word << endl;
    cout << "next code: " << cin.peek() << endl;      // the '\n' (10) is still in the stream

    cin.ignore(100, '\n');               // discards characters up to and including '\n'
    char name[20];
    cin.getline(name, 20);               // reads "Kim Minsu" and discards the '\n'
    cout << "name=" << name << ", gcount=" << cin.gcount() << endl;
    return 0;
}
