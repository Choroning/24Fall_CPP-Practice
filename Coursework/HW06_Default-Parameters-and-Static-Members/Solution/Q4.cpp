/**
 * @file    A6Q4.cpp
 * @brief   Q4. static 멤버로 객체 수를 세는 Circle 클래스 (11주차 실습 4)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
    static int count;
public:
    Circle() {
        radius = 0;
        count++;
    }
    Circle (int r) {
        radius = r;
        count++;
    }
    ~Circle();
    void show() {
        cout << "Radius: " << radius << ", Count: " << count << endl;
    }
};
Circle::~Circle() {
    count--;
}
int Circle::count = 0;

int main() {
    Circle a(5);
    a.show();
    
    Circle *p = new Circle[2];
    
    for (int i = 0; i < 2; i++)
        p[i].show();
    
    a.show();
    
    delete[] p;
    p = nullptr;
    a.show();
}
