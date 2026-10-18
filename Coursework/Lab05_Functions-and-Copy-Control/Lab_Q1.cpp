/**
 * @file    A10204-1_C05Q1.cpp
 * @brief   Q1. 다양한 함수 호출법
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int radius) {
        this->radius = radius;
    }
    void show() {
        cout << "반지름이 " << radius << "인 원\n";
    }
    void setRadius(int radius) {
        this->radius = radius;
    }
    int getRadius() {
        return radius;
    }
};

void mySwap(Circle& a, Circle& b) {
    int temp = a.getRadius();
    a.setRadius(b.getRadius());
    b.setRadius(temp);
}

void mySwap(Circle* a, Circle* b) {
    int temp = a->getRadius();
    a->setRadius(b->getRadius());
    b->setRadius(temp);
}

void increaseBy(Circle& a, Circle& b) {
    a.setRadius(a.getRadius() + b.getRadius());
}

int main() {
    Circle x(10), y(5);  // 각각 반지름이 10, 5인 원 객체 생성
    
    mySwap(x, y);        // 참조 버전의 mySwap() 호출
    cout << "x 객체: "; 
    x.show();
    cout << "y 객체: "; 
    y.show();
    cout << endl;
    
    mySwap(&x, &y);      // 포인터 버전의 mySwap() 호출
    cout << "x 객체: "; 
    x.show();
    cout << "y 객체: "; 
    y.show();
    cout << endl;
    
    increaseBy(x, y);    // x 객체를 y 객체만큼 증가
    cout << "x 객체: "; 
    x.show();
    cout << "y 객체: "; 
    y.show();

    return 0;
}
