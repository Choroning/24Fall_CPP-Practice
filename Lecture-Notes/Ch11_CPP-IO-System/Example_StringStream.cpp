/**
 * @file    Example_StringStream.cpp
 * @brief   Parses a line with istringstream and builds text with ostringstream
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

int main() {
    string line = "Kim 90 85 77";
    istringstream in(line);                      // reads from the characters of line
    string name;
    int score, total = 0, count = 0;
    in >> name;
    while (in >> score) {
        total += score;
        count++;
    }
    cout << name << ": " << count << " scores, total " << total << endl;

    ostringstream out;                           // writes into an internal string
    out << name << " average " << fixed << setprecision(2) << double(total) / count;
    string report = out.str();
    cout << report << " (" << report.size() << " characters)" << endl;

    string csv = "apple,3,1.20";
    istringstream fields(csv);
    string item, qty, price;
    getline(fields, item, ',');                  // reads up to each comma
    getline(fields, qty, ',');
    getline(fields, price);
    cout << item << " x" << stoi(qty) << " at " << stod(price) << endl;

    in.clear();                                  // eofbit is set from the earlier loop
    in.str("Lee 100");                           // reuses the stream with new contents
    in >> name >> score;
    cout << name << " " << score << endl;
    return 0;
}
