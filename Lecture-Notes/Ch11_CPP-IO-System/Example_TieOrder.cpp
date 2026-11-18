/**
 * @file    Example_TieOrder.cpp
 * @brief   Shows how the tie between cerr and cout keeps the output order when stdout is a pipe
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
using namespace std;

int main() {
    cout << "1 cout\n";
    cerr << "2 cerr\n";                          // cerr is tied to cout: cout is flushed first
    cout << "3 cout\n";
    clog << "4 clog\n";                          // clog is not tied: cout may still hold "3 cout"
    cout << "5 cout\n";
    return 0;
}
