/**
 * @file    A8Q1.cpp
 * @brief   Q1. 프렌드 함수로 중복한 정수 + Circle 연산 (13주차 실습 1)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle (int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
    friend Circle operator+ (int op1, Circle op2);
};

Circle operator+ (int op1, Circle op2) {
    Circle tmp;
    tmp.radius = op1 + op2.radius;
    return tmp;
}

int main() {
    Circle a(5), b(4);
    b = 1 + a;
    a.show();
    b.show();
}
