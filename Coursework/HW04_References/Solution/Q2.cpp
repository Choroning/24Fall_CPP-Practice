/**
 * @file    A4Q2.cpp
 * @brief   Q2. 참조와 관련한 자유 문제 1
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
using namespace std;

void swapNumbers(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void modifyValue(int& value) {
    value *= 2;
}

void printValue(const int& value) {
    cout << "Value is: " << value << endl;
}

int main() {
    int number = 42;
    int& ref = number;
    
    cout << "Original number: " << number << endl;
    ref = 100;
    cout << "After changing through reference: " << number << endl;
    
    int num1 = 10, num2 = 20;
    cout << "\nBefore swap - num1: " << num1 << ", num2: " << num2 << endl;
    swapNumbers(num1, num2);
    cout << "After swap - num1: " << num1 << ", num2: " << num2 << endl;
    
    int value = 5;
    cout << "\nBefore modification: " << value << endl;
    modifyValue(value);
    cout << "After modification: " << value << endl;
    
    const int& constRef = value;
    printValue(value);
    
    return 0;
}
