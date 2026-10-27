/**
 * @file    A8Q2.cpp
 * @brief   Q2. 프렌드 함수로 중복한 전위, 후위 ++ 연산자 (13주차 실습 2)
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
    friend Circle operator++(Circle& op);
    friend Circle operator++(Circle& op, int notUsed);
};

Circle operator++(Circle& op) {
    op.radius++;
    return op;
}
Circle operator++(Circle& op, int notUsed) {
    Circle tmp = op;
    op.radius++;
    return tmp;
}

int main() {
    Circle a(5), b(4);
    ++a;        // 반지름을 1 증가시킴
    b = a++;    // 반지름을 1 증가시킴
    a.show();
    b.show();
}
