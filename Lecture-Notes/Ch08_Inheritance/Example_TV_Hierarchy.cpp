/**
 * @file    A99TA7Q14.cpp
 * @brief   8장 연습문제 이론 문제 16번의 TV, ColorTV, InternetTV 클래스 계층
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-23
 */

#include <iostream>
using namespace std;

class TV {
    public: int screenSize;
};

class ColorTV : public TV {
    public: int color;
};

class InternetTV: public TV {
    public: string ipAddr;
};

int main() {
    
}
