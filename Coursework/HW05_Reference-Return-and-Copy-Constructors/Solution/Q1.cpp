/**
 * @file    A5Q1.cpp
 * @brief   Q1. 자기 참조를 리턴하는 Accumulator 클래스 (10주차 실습 2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-07
 */

#include <iostream>
using namespace std;

class Accumulator {
    int value;
public:
    Accumulator(int value) { this->value = value; }
    Accumulator& add(int n);
    int get() { return value; }
};
Accumulator& Accumulator::add(int n) {
    value += n;
    return *this;
}

int main() {
    Accumulator acc(10);
    acc.add(5).add(6).add(7);   // acc의 value 멤버가 28이 된다.
    cout << acc.get() << endl;  // 28 출력
}
