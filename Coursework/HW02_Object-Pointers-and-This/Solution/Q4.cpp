/**
 * @file    A2Q4.cpp
 * @brief   Q4. this 포인터를 리턴하는 메서드 체이닝 자유 코드
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
using namespace std;

class Counter {
    int count;
public:
    Counter() : count(0) {}
    Counter* increment() {
        this->count++;
        return this;
    }
    Counter* decrement() {
        this->count--;
        return this;
    }
    void display() {
        cout << "Count: " << this->count << endl;
    }
};

int main() {
    Counter c;
    
    c.increment()->increment()->display();  // Count: 2
    c.decrement()->display();  // Count: 1
}
