/**
 * @file    A7Q2.cpp
 * @brief   Q2. 두 가지 == 연산자를 갖춘 Book 클래스 (12주차 실습 3)
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
    Book(string title = "", int price = 0, int pages = 0) {
        this->title = title;
        this->price = price;
        this->pages = pages;
    }
    void show() {
        cout << title << ' ' << price << "원 " << pages << "페이지" << endl;
    }
    string getTitle() {
        return title;
    }
    bool operator == (string op) {
        if (this->title == op)
            return true;
        else
            return false;
    }
    bool operator == (Book op) {
        if (this->title == op.title)
            return true;
        else
            return false;
    }
};

int main() {
    Book a("명품 C++", 30000, 500), b("고품 C++", 30000, 500);
    
    // 책 title 비교
    if (a == "명품 C++")
        cout << "명품 C++ 입니다." << endl;
    
    // title, price, pages 모두 비교
    if (a == b)
        cout << "두 책이 같은 책입니다." << endl;
    else
        cout << "두 책은 다른 책입니다." << endl;
    
    return 0;
}
