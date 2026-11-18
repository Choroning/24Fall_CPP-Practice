/**
 * @file    Example_CharOutput.cpp
 * @brief   Prints characters and byte blocks with the ostream member functions put(), write(), and flush()
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <cstring>
using namespace std;

int main() {
    cout.put('C').put('+').put('+').put('\n');   // put() returns ostream&, so calls can be chained

    const char* msg = "Hello, stream world";
    cout.write(msg, 5);                          // writes only the first 5 bytes, "Hello"
    cout.put('\n');
    cout.write(msg, strlen(msg)).put('\n');      // writes the whole string

    int code = 65;
    cout << code << ' ';                         // operator<< converts the int 65 to the text "65"
    cout.put(code);                              // put() writes the character whose code is 65, 'A'
    cout.put('\n');

    cout << "before flush";
    cout.flush();                                // sends the buffered characters to the device now
    cout << ", after flush" << endl;
    return 0;
}
