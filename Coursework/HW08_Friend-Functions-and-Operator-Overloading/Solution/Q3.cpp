/**
 * @file    A8Q3.cpp
 * @brief   Q3. Matrix 클래스의 <<, >> 연산자 (13주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Matrix {
    int ar[4];
public:
    Matrix() { for (int i = 0; i < 4; i++) ar[i] = 0; }
    Matrix(int a1, int a2, int b1, int b2) {
        ar[0] = a1;
        ar[1] = a2;
        ar[2] = b1;
        ar[3] = b2;
    }
    void show() {
        cout << "Matrix = { ";
        for (int i = 0; i < 4; i++) cout << ar[i] << ' ';
        cout << "}" << endl;
    }
    
    // (1) <<, >>를 Matrix의 멤버 함수로 구현
    int* operator >> (int op[]) {
        for (int i = 0; i < 4; i++) {
            op[i] = ar[i];
        }
        return op;
    }
    Matrix& operator << (int op[]) {
        for (int i = 0; i < 4; i++) {
            ar[i] = op[i];
        }
        return *this;
    }
    
    /* (2) <<, >>를 Matrix의 프렌드 함수로 구현
    friend int* operator>>(Matrix& op1, int op2[]) {
        for (int i = 0; i < 4; i++)
            op2[i] = op1.ar[i];
        return op2;
    }
        
    friend Matrix& operator<<(Matrix& op1, int op2[]) {
        for (int i = 0; i < 4; i++)
            op1.ar[i] = op2[i];
        return op1;
    }*/
};

int main() {
    Matrix a(4, 3, 2, 1), b;
    int x[4], y[4] = { 1, 2, 3, 4 };    // 2차원 행렬의 4개의 원소 값
    a >> x;     // a의 각 원소를 배열 x에 복사. x[]는 { 4, 3, 2, 1 }
    b << y;     // 배열 y의 원소 값을 b의 각 원소에 설정
    
    for (int i = 0; i < 4; i++)
        cout << x[i] << ' ';    // x[] 출력
    cout << endl;
    b.show();
}
