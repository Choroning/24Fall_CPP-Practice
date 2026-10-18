/**
 * @file    A10204-1_C10Q1.cpp
 * @brief   Q1. 제네릭 함수
 * @author  박철원 (Cheolwon Park)
 * @date    2024-12-04
 */

#include <iostream>
using namespace std;

// reverseArray() 정의
template <typename T>
void reverseArray(T arr [], int size) {
    T tmp;
    for (int i = 0; i < size/2; i++) {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

void reverseArray(char arr []) {
    int size = 0;
    while(arr[size])
        size++;
    
    char tmp;
    for (int i = 0; i < size/2; i++) {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

// display() 정의
template <typename T>
void display(const T arr [], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << "  ";
    }
    cout << endl;
}

int main() {
   int x[] = { 1, 10, 100, 1000 };

   reverseArray(x, 4);
   cout << "x = ";
   display(x, 4);

   double y[] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6 };
   reverseArray(y, 6);
   cout << "y = ";
   display(y, 6);

   char str[20] = "C++ is powerful";
   // str을 뒤집는 reverseArray() 호출문
   reverseArray(str);
   cout << "str = " << str << endl;

   return 0;
}
