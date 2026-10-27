/**
 * @file    A2Q3.cpp
 * @brief   Q3. 정적 배열과 동적 배열 (5주차 강의자료 p.15 코드 분석)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
using namespace std;

int main()
{
    int size;
    cout << "Input size: ";
    cin >> size;
    
    // int arrStatic[size] = { }; 일반 변수: 가능한가?
    int arrStatic[4] = { 1, 3, 5 };
    
    for (int i = 0; i < 4; i++)
        cout << arrStatic[i] << ' ';
    cout << endl;
    
    // 동적 메모리는 가변 길이 배열 선언 가능
    int* pDynamic = new int[size];
    
    for (int i = 0; i < size; i++)
        pDynamic[i] = i * 3;
    
    for (int i = 0; i < size; i++)
        cout << pDynamic[i] << ' ';
    cout << endl;
    
    delete[] pDynamic;
    // pDynamic = NULL;
    pDynamic = nullptr;
}
