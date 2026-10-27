# HW7 Assignment Handout — Operator Overloading

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the seventh assignment of section 3 of Computer Language and Training Ⅱ. It consists of two lab problems from the Week 12 lecture slides and one free-code problem on operator overloading.

- **Deadline:** 2024-11-25 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden automatically.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.

---

<br>

## Problem 1. Week 12 Lecture Slides [Lab 2-2]

The `Book` class used in [Lab 2] has the title, the price, and the number of pages of a book as member variables and is defined as follows.

```cpp
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
        cout << title << ' ' << price << "원 " << pages << " 페이지" << endl;
    }

    string getTitle() { return title; }

    int getPrice(void) { return price; }

    void setPrice(int p) { price = p; }
};
```

Add functionality to the `Book` class so that the following `main()` function runs. Implement the `-=` and `+=` operator functions as member functions of the `Book` class, and take into account that code such as `c = a += 500;` must also work.

```cpp
int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    a += 500; // 책 a의 가격 500원 증가
    b -= 500; // 책 b의 가격 500원 감소

    a.show();
    b.show();
}
```

**Sample Run**

```text
청춘 20500원 300 페이지
미래 29500원 500 페이지
```

This problem continues the preceding [Lab 2-1]. [Lab 2-1] gives the same `Book` class with a `main()` that reduces the prices of the two books with `Sub(a, 500);` and `Sub(b, 1000);` and asks for `Sub()` as an ordinary function. [Lab 2-2] asks for the same functionality implemented with operator overloading instead.

---

<br>

## Problem 2. Week 12 Lecture Slides [Lab 3]

The `Book` class used in [Lab 3] is the class of [Lab 2] without `getPrice()` and `setPrice()`.

```cpp
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
        cout << title << ' ' << price << "원 " << pages << " 페이지" << endl;
    }

    string getTitle() {
        return title;
    }
};
```

Write `==` operator functions for the two situations as member functions of the `Book` class so that the following `main()` function runs.

```cpp
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
```

**Sample Run**

```text
명품 C++ 입니다.
두 책은 다른 책입니다.
```

As a further question, the problem asks how a `!=` operator function would be implemented if it were provided as a member function of the `Book` class.

---

<br>

## Problem 3. Free Code on Operator Overloading

Freely write and analyze code that uses operator overloading. Use an example different from the code in the textbook. There is no given class, `main()` function, or sample run.
