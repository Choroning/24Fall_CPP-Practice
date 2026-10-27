/**
 * @file    A3Q3.cpp
 * @brief   Q3. 동적 정수 배열로 구하는 평균 (6주차 문제 6-4)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

int main()
{
    int ave = 0;
    int* p = new int[5];
    
    cout << "정수 5개 입력 >> ";
    for (int i = 0; i < 5; i++){
        cin >> p[i];
        ave += p[i];
    }
    
    cout << "평균 " << ave / 5.0 << endl;
    
    delete [] p;
    p = nullptr;
}
