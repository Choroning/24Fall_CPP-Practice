# HW5 Assignment Handout — Reference Returns and Copy Constructors

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the fifth assignment of section 3 of Computer Language and Training Ⅱ. It consists of two lab problems from the Week 10 lecture slides.

- **Deadline:** 2024-11-11 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden automatically.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.
- **Notice:** Visual Studio 2022 may require adding `const` in the function definitions; add it if needed.

---

<br>

## Problem 1. Week 10 Lecture Slides [Lab 2]

The class `Accumulator` keeps accumulating values through the `add()` function and is declared as follows. Implement the `Accumulator` class.

```cpp
class Accumulator {
    int value;
public:
    Accumulator(int value) { this->value = value; }
    Accumulator& add(int n);
    int get() { return value; }
};
```

`Accumulator` is used by the following `main()` function; the comments say that the `value` member of `acc` becomes 28 and that 28 is printed.

```cpp
int main() {
    Accumulator acc(10);
    acc.add(5).add(6).add(7); // acc의 value 멤버가 28이 된다.
    cout << acc.get() << endl; // 28 출력
}
```

The three calls of `add()` are made in the order [1] `acc.add(5)`, [2] `acc.add(6)`, [3] `acc.add(7)`.

**Sample Run**

```text
28
```

---

<br>

## Problem 2. Week 10 Lecture Slides [Lab 3]

Answer the questions about the following `Book` class, which stores the title and the price of a book. The comments in the class mark the title string, the price, and the deep copy constructor to be written, and `show()` prints the price followed by the currency unit 원 (won).

```cpp
class Book {
    char *title; // 제목 문자열
    int price; // 가격
public:
    Book(char* title, int price);
    Book(Book& b); // 깊은 복사 생성자 작성
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};

int main() {
    Book cpp("명품C++", 10000);
    Book java = cpp;
    java.set("명품자바", 12000);
    cpp.show();
    java.show();
}
```

1. Implement the constructor, the destructor, and the `set()` function of the `Book` class.
2. What is the code of the default copy constructor that the compiler adds automatically?
3. If the programmer does not provide a definition of the copy constructor, a problem occurs when this code runs. What is the problem?
4. Define a copy constructor so that no error occurs.

**Sample Run**

```text
명품C++ 10000원
명품자바 12000원
```

In Visual Studio, `#define _CRT_SECURE_NO_WARNINGS` is placed at the beginning of the file to prevent the compile error caused by `strcpy()`.
