/**
 * @file    A10204-1_C06Q2.cpp
 * @brief   Q2. 구구단
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
using namespace std;

void print99(int i = 19) {
    
    if (i != 19){
        cout << i << "단" << endl;
        for (int j = 1; j < 10; j++) {
            cout << i << " * " << j << " = " << i * j << endl;
        }
        cout << endl;
    } else {
        for (int k = 1; k < 10; k++) {
            cout << k << "단" << endl;
            for (int j = 1; j < 10; j++) {
                cout << k << " * " << j << " = " << k * j << endl;
            }
            cout << endl;
        }
    }
}

int main() {
    print99(3);     // 3단 출력
    print99();      // 1 - 9단(전체) 출력
    
    return 0;
}
