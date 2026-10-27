/**
 * @file    A3Q4.cpp
 * @brief   Q4. Circle 객체의 동적 배열 (6주차 문제 6-5)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    void setRadius(int radius);
    double getArea();
};

void Circle::setRadius(int radius) {
    this->radius = radius;
}

double Circle::getArea(){
    return 3.14 * radius * radius;
}

int main(void)
{
    Circle* p;
    int size, count = 0;
    cout << "원의 개수 >> ";
    cin >> size;
    p = new Circle [size];
    
    for (int i = 0; i < size; i++) {
        int temp;
        cout << "원 " << i + 1 << "의 반지름 >> ";
        cin >> temp;
        p[i].setRadius(temp);
        
        if (p[i].getArea() >= 100)
            count++;
    }
    cout << "면적이 100보다 큰 원은 " << count << "개 입니다.\n";
}
