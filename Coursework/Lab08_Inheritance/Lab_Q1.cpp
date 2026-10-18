/**
 * @file    A10204-1_C08Q1.cpp
 * @brief   Q1. 점의 색상과 위치 정보
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-20
 */

#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    Point(int x, int y) { this->x = x; this->y = y; }
    int getX() { return x; }
    int getY() { return y; }
protected:
    void move(int x, int y) { this->x = x; this->y = y; }
};

class ColorPoint : public Point {
    string color;
public:
    ColorPoint(int x = 0, int y = 0, string color = "BLACK") : Point(x, y) {
        this->color = color;
    }
    void setPoint(int x, int y) {
        move(x, y);
    }
    void setColor(string color) {
        this->color = color;
    }
    void show() {
        cout << color << "색으로 (" << getX() << ", " << getY() << ")에 위치한 점입니다." << endl;
    }
};

int main() {
   ColorPoint cp1(5, 5, "RED");  // 색상 "RED", 위치 (5, 5)로 초기화
   cp1.show();
   cp1.setPoint(10, 20);         // 위치 (10, 20)으로 설정
   cp1.setColor("BLUE");         // 색상 "BLUE"로 설정
   cp1.show();

   ColorPoint cp2(5, 5);         // 색상 "BLACK", 위치 (5, 5)로 초기화
   cp2.show();
   cp2.setPoint(3, 7);           // 위치 (3, 7)로 설정
   cp2.setColor("GREEN");        // 색상 "GREEN"으로 설정
   cp2.show();

   ColorPoint zeroPoint;         // 색상 "BLACK", 위치 (0, 0)로 초기화
   zeroPoint.show();

   return 0;
}
