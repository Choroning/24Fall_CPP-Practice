/**
 * @file    A99Upcasting_prac.cpp
 * @brief   업캐스팅 연습: 기본 클래스 포인터로 파생 클래스 객체를 가리키기
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-20
 */

#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    void set(int x, int y) { this->x = x; this->y = y; }
    void showPoint() {
        cout << "(" << x << "," << y << ")" << endl;
    }
};

class ColorPoint : public Point {
    string color;
public:
    void setColor(string color) { this->color = color; }
    void showColorPoint();
};

void ColorPoint::showColorPoint() {
    cout << color << ":";
    showPoint();
}

int main(void) {
    ColorPoint cp;
    ColorPoint *pDer = &cp; // 자기가 자기꺼 받음
    Point* pBase = pDer;    // 업캐스팅
    // pBase가 ColorPoint의 cp의 주소 가짐(업캐스팅)
    
    pDer->set(3,4);         // 이 과정에서 형변환?
    pBase->showPoint();
    pDer->setColor("RED");
    pDer->showColorPoint();
    //pBase->showColorPoint();    // 컴파일 오류
}
