/**
 * @file    A7Q1.cpp
 * @brief   Q1. 멤버 함수로 중복한 +=, -= 연산자 (12주차 실습 2-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
using namespace std;

class Book {
    string title;
    int price;
    int pages;
public:
    Book (string title = "", int price = 0, int pages = 0) {
        this->title = title;
        this->price = price;
        this->pages = pages;
    }
    void show() {
        cout << title << ' ' << price << "원 " << pages << "페이지" << endl;
    }
    
    string getTitle() { return title; }
    int getPrice() { return price; }
    void setPrice(int p) { price = p; }
    Book operator += (int op) {
        this->price += op;
        return *this;
    }
    Book operator -= (int op) {
        this->price -= op;
        return *this;
    }
};

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    a += 500;
    b -= 500;
    
    a.show();
    b.show();
}
