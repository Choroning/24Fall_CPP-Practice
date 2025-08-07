/**
 * @file    A10204-1_C05Q3.cpp
 * @brief   Q3. 복사 생성자
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char* title;    // 도서 제목 문자열
    int price;      // 도서 가격
public:
    Book(const char* title, int price);
    Book(Book& src);
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};

Book::Book(const char* title, int price) {
    this->price = price;
    this->title = new char [strlen(title) + 1];
    strcpy(this->title, title);
}
Book::Book(Book& src) {
    price = src.price;
    title = new char [strlen(src.title) + 1];
    strcpy(title, src.title);
}
Book::~Book() {
    delete[] title;
    title = nullptr;
}
void Book::set(char* title, int price) {
    this->price = price;
    if (strlen(this->title) < strlen(title)) {
        delete[] this->title;
        this->title = new char[strlen(title) + 1];
    }
    strcpy(this->title, title);
}

int main()
{
    Book cpp("C++가이드", 10000);
    Book java = cpp;  // 복사 생성자 호출
    java.set("Java완전정복", 12000);
    cpp.show();
    java.show();
}
