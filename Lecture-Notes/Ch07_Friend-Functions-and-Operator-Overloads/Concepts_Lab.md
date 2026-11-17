# Chapter 7 Lab — Friend Functions and Operator Overloading

[Repository Guide](../../README.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **Last Updated:** 2026-11-18

> **Learning Objectives:**
> 1. Explain why an ordinary function that modifies an object must take its parameter by reference
> 2. Compare accessing a class's `private` members through `public` functions with accessing them through a friend declaration
> 3. Explain how a feature implemented as an ordinary function turns into operator overloading

---

## Table of Contents

- [1. Overview](#1-overview)
- [2. An Ordinary Function That Modifies an Object](#2-an-ordinary-function-that-modifies-an-object)
  - [2.1 A Reference Parameter and public Functions](#21-a-reference-parameter-and-public-functions)
  - [2.2 The Problem with Taking It by Value](#22-the-problem-with-taking-it-by-value)
  - [2.3 Turning It into a Friend Function](#23-turning-it-into-a-friend-function)
- [3. From an Ordinary Function to an Operator Function](#3-from-an-ordinary-function-to-an-operator-function)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overview

> **Note:** Friend functions and friend classes, the three forms of friend declarations and forward declarations, and the overloading of binary, unary, and stream operators are covered in the [Chapter 7 lecture notes](Concepts_Lecture.md). This lab starts from modifying an object with an ordinary function and follows how the same feature leads to a friend function and an operator function.

| Topic | Location |
|:-----|:-----|
| An ordinary function `Sub()` that lowers a book's price, `Sub()` turned into a friend function | Section 2 |
| A `Book` class that implements the same feature with the `+=` and `-=` operators | [HW7 Problem 1](../../Coursework/HW07_Operator-Overloading/Original/Assignment.md#problem-1-week-12-lecture-slides-lab-2-2) |
| `==` that compares with a string, `==` that compares with another book | [HW7 Problem 2](../../Coursework/HW07_Operator-Overloading/Original/Assignment.md#problem-2-week-12-lecture-slides-lab-3) |
| A `Circle` that counts current objects with a `static` member | [HW6 Problem 4](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.md#problem-4-week-11-lecture-slides-lab-4) |
| `+` that enables `1 + a`, prefix and postfix `++` written as friend functions, `<<` and `>>` of `Matrix` | [HW8 Problems 1-3](../../Coursework/HW08_Friend-Functions-and-Operator-Overloading/Original/Assignment.md) |
| Operator overloading for fraction, complex number, and time classes | [Chapter 7 Exercises 1-3](Exercises.md) |

*Table 1. Topics of the lab and their location*

All code is compiled and run with the C++17 standard (for example, `g++ -std=c++17 -Wall`).

---

<br>

## 2. An Ordinary Function That Modifies an Object

**Problem:** Write an ordinary function `Sub()` that lowers a book's price by a given amount, so that the following `Book` class and `main()` run.

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

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    Sub(a, 500);                        // Lower the price of book a by 500 won
    Sub(b, 1000);                       // Lower the price of book b by 1000 won
    a.show();
    b.show();
}
```

### 2.1 A Reference Parameter and public Functions

```cpp
void Sub(Book& b, int amount) {         // Take it by reference to change the original object
    b.setPrice(b.getPrice() - amount);  // Access price through public functions
}
```

```text
청춘 19500원 300 페이지
미래 29000원 500 페이지
```

- `Sub()` is an ordinary function, not a member of `Book`, so it cannot access the `private` member `price` directly. It therefore reads the current price with the `public` function `getPrice()` and writes the new price with `setPrice()`. (The output shows the title, the price in won (원), and the number of pages (페이지); 청춘 and 미래 are titles.)
- The parameter `Book& b` is an alias for the caller's objects `a` and `b`, so `setPrice()` changes the price of the original object.

### 2.2 The Problem with Taking It by Value

If the parameter is changed to `Book b`, the result is as follows.

```cpp
void Sub(Book b, int amount) { b.setPrice(b.getPrice() - amount); }
```

```text
청춘 20000원 300 페이지
```

In a call by value, `b` is a copy of the argument `a`, so only the copy's price goes down, and the copy disappears when the function ends. The price of `a` stays at 20000 won (Section 2.1 of the Chapter 5 lab). A function that modifies an object must take the object by reference or by pointer.

### 2.3 Turning It into a Friend Function

If `Book` provides no get/set functions, or if the function must handle several `private` members together, the function can be declared a friend of the class.

```cpp
#include <iostream>
#include <string>
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
        cout << title << ' ' << price << "원 " << pages << " 페이지" << endl;
    }
    friend void Sub(Book& b, int amount);   // Allow Sub() to access private members
};

void Sub(Book& b, int amount) {
    b.price -= amount;                  // As a friend, it accesses price directly
}

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    Sub(a, 500);
    Sub(b, 1000);
    a.show();
    b.show();
}
```

```text
청춘 19500원 300 페이지
미래 29000원 500 페이지
```

- `friend void Sub(Book& b, int amount);` appears inside the class, but it does not make `Sub()` a member; it is a declaration that grants the external function `Sub()` permission to access `private` members. So `Book::` is not placed before the definition of `Sub()`.
- A friend declaration makes an exception to encapsulation, so when `public` functions suffice, using the published interface as in Section 2.1 makes it easier to change the class's internal implementation.

---

<br>

## 3. From an Ordinary Function to an Operator Function

`Sub(a, 500)` has the form of a function call, so the meaning "lower the price of a by 500" is carried only by its name. C++ allows attaching an operator symbol to such a function, which is called **operator overloading**. The name of an operator function is `operator` followed by the operator symbol, and it is implemented in the following two ways (Section 2.4 of the Chapter 7 lecture notes).

| Approach | Declaration | Interpretation of `a -= 500` |
|:-----|:-----|:-----|
| Member function | `X& operator-=(int n);` | `a.operator-=(500)` |
| Friend function | `friend X& operator-=(X& x, int n);` | `operator-=(a, 500)` |

*Table 2. Two ways to implement an operator function*

`X` in the table is an arbitrary class. An ordinary function that modifies an object, like the `Sub()` of Section 2.3, can be turned into such an operator function, and how to implement compound assignment operators as member and friend functions is covered in Section 3.3 of the Chapter 7 lecture notes. Implementing `+=` and `-=` of `Book` as member functions is [HW7 Problem 1](../../Coursework/HW07_Operator-Overloading/Original/Assignment.md#problem-1-week-12-lecture-slides-lab-2-2).

> **Note:** Operator overloading only adds a new meaning to an existing operator; the number of operands, the precedence, and the associativity of the operator cannot be changed. Also, at least one operand must be of a user-defined type, so the meaning of `+` between `int`s cannot be changed.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Functions that modify objects | Taking an object by value changes only a copy, so to change the original object it must be taken by reference or pointer. |
| public interface | An ordinary function cannot access `private` members directly, so it goes through the get/set functions the class provides. |
| Friend function | A `friend` declaration inside a class only grants an external function access to `private` members and does not make that function a member. |
| Operator function | A function named `operator` followed by the operator symbol, implemented as a member or friend function; the operator's precedence and number of operands cannot be changed. |

---

<br>

## Self-Check Questions

1. What error arises if the `friend` declaration is removed from the `Book` of Section 2.3? How can it be fixed to behave the same without errors?

   <details>
   <summary>Answer</summary>

   A compile error says that `price` is `private` on the line where `Sub()` accesses `b.price`. To behave the same without a friend declaration, give `Book` the `public` functions `getPrice()` and `setPrice()` and have `Sub()` use them as in Section 2.1, or make `Sub()` a member function of `Book` and call it as `a.Sub(500)`.

   </details>

2. Does it make any difference whether the friend function `Sub()` is declared in the `public:` or the `private:` section of the `Book` class?

   <details>
   <summary>Answer</summary>

   It makes no difference. A friend declaration is not a member, so access specifiers do not affect it, and wherever it is placed in the class, it grants the function the same access. For readability, friend declarations are usually gathered at the beginning of the class or in the `public:` section.

   </details>

3. How many parameters does `a -= 500` take when implemented as a member function and when implemented as a friend function? Why?

   <details>
   <summary>Answer</summary>

   The member function takes one parameter (`int amount`), and the friend function takes two (`Book& b`, `int amount`). For a member function, the left operand `a` is the object that called the function, the object `this` points to, so only the right operand is received as a parameter. A friend function is not a member and has no `this`, so it receives both operands as parameters.

   </details>
