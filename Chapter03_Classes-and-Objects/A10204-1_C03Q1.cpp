/**
 * @file    A10204-1_C03Q1.cpp
 * @brief   Q1. 클래스 연습
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-26
 */

#include <iostream>
using namespace std;

class Tower{
private:
    int height;
public:
    Tower();
    Tower(int h);
    int getHeight();
};

Tower::Tower():Tower(1){}

Tower::Tower(int h){
    height = h;
}

int Tower::getHeight(){
    return height;
}


int main()
{
    Tower myTower;          // 1미터
    Tower seoulTower(100);  // 100미터
    cout << "높이는 " << myTower.getHeight() << "미터" << endl;
    cout << "높이는 " << seoulTower.getHeight() << "미터" << endl;
    
    return 0;
}
