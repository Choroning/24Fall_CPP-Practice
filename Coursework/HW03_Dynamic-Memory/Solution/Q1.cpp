/**
 * @file    A3Q1.cpp
 * @brief   Q1. 동적으로 생성한 Complex 객체 (6주차 문제 6-1)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Complex {
    int real, image;
public:
    Complex() {
        real = image = 1;
    }
    Complex(int real, int image) {
        this->real = real;
        this->image = image;
    }
    void setReal(int real) {
        this->real = real;
    }
    void setImage(int image) {
        this->image = image;
    }
    void pCom() {
        cout << real << " + " << image << "i\n";
    }
};

int main()
{
    Complex* a = new Complex;
    Complex* b = new Complex;
    a->pCom();
    b->pCom();
    cout << "---------------\n";
    (*a).setReal(4);
    (*b).setImage(9);
    (*a).pCom();
    (*b).pCom();
    
    delete a;
    a = nullptr;
    delete b;
    b = nullptr;
}
