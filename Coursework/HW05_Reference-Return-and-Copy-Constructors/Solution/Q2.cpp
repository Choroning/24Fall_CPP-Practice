/**
 * @file    A5Q2.cpp
 * @brief   Q2. 깊은 복사 생성자를 갖춘 Book 클래스 (10주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-07
 */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char* title;
    int price;
public:
    Book(char* title, int price);
    Book(Book& b);
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};
Book::Book(char* title, int price) {
    this->title = new char [strlen(title) + 1];
    strcpy(this->title, title);
    this->price = price;
}
Book::Book(Book& b) {
    this->price = b.price;
    this->title = new char [strlen(b.title) + 1];
    strcpy(this->title, b.title);
}
Book::~Book() {
    if (!title)
        delete [] title;
}
void Book::set(char* title, int price) {
    if (strlen(this->title) < strlen(title)) {
        delete [] this->title;
        this->title = new char [strlen(title) + 1];
    }
    strcpy(this->title, title);
    this->price = price;
}


int main() {
    Book cpp("명품c++", 10000);
    Book java = cpp;
    java.set("명품자바", 12000);
    cpp.show();
    java.show();
}
