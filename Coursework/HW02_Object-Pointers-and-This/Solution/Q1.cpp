/**
 * @file    A2Q1.cpp
 * @brief   Q1. 포인터로 다루는 Complex 객체 배열 (5주차 문제 5-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
#define MAX 2
using namespace std;

class Complex{
    int real, image;
public:
    Complex(){
        real = image = 1;
    }
    Complex(int ir, int ii){
        real = ir;
        image = ii;
    }
    
    void setReal(int ir){
        real = ir;
    }
    void setImage(int ii){
        image = ii;
    }
    void showC(){
        cout << real << " + " << image << "i" << endl;
    }
};

int main()
{
    Complex data[MAX];
    Complex *p = data;
    
    for (int i = 0; i < MAX; i++){
        data[i].showC();
    }
    cout << "---------------" << endl;
    
    data[0].setReal(3);
    (*(p + 1)).setImage(-8);
    
    for (int i = 0; i < MAX; i++){
        (p + i)->showC();
    }
}
