/**
 * @file    Example_StreamState.cpp
 * @brief   Shows the state bits after failed extractions and recovers with clear() and ignore()
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <limits>
using namespace std;

void showState(const istream& in) {
    cout << "  good=" << in.good() << " eof=" << in.eof()
         << " fail=" << in.fail() << " bad=" << in.bad() << endl;
}

int main() {
    // Expected input: abc 17 x 25
    int n = -1;
    cin >> n;                            // "abc" is not an integer
    cout << "after reading abc: n=" << n << endl;
    showState(cin);

    cin >> n;                            // fails at once because failbit is still set
    cout << "second attempt without clear: n=" << n << endl;

    cin.clear();                         // resets the state to goodbit
    cin.ignore(numeric_limits<streamsize>::max(), ' ');   // drops "abc "
    cin >> n;
    cout << "after clear and ignore: n=" << n << endl;
    showState(cin);

    int sum = 0, value;
    while (true) {                       // reads the rest, skipping bad tokens
        if (cin >> value) {
            sum += value;
        } else if (cin.eof()) {
            break;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), ' ');
        }
    }
    cout << "sum of the remaining integers: " << sum << endl;
    showState(cin);
    return 0;
}
