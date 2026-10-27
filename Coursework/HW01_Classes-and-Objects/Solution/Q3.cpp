/**
 * @file    A1Q3.cpp
 * @brief   Q3. 접근 제어와 get/set 함수를 갖춘 Point 클래스 (4주차 문제 1-3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
using namespace std;

class Point{
private:
    int x, y;
public:
    Point(){
        x = y = 0;
    }
    Point(int ix, int iy);
    
    void showP();
    
    int getX(){
        return x;
    }
    void setX(int ix){
        x = ix;
    }
    
    int getY();
    void setY(int iy);
    
};

Point::Point(int ix, int iy){
    x = ix;
    y = iy;
}

void Point::showP(){
    cout << "x: " << getX() << ", y: " << getY() << endl;
}

int Point::getY(){
    return y;
}

void Point::setY(int iy){
    y = iy;
}

int main()
{
    Point p1, p2;
    int temp;
    
    cout << "Input p2.x: ";
    cin >> temp;
    p2.setX(temp);
    
    cout << "Input p2.y: ";
    cin >> temp;
    p2.setY(temp);
    
    cout << "p1. ";
    p1.showP();
    cout << "p2. ";
    p2.showP();
}
