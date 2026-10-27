# Chapter 2 Exercises — C++ Programming Basics

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 2

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

Exercises 1-10 are the even-numbered theory problems (2-20) of Chapter 2 of the textbook.

---

<br>

## Table of Contents

- [Exercise 1: Statements About the `main()` Function](#exercise-1-statements-about-the-main-function)
- [Exercise 2: Declaring Variables After Statements](#exercise-2-declaring-variables-after-statements)
- [Exercise 3: Using `cout` Without `std::`](#exercise-3-using-cout-without-std)
- [Exercise 4: Correcting C++ Code](#exercise-4-correcting-c-code)
- [Exercise 5: Statements About C++ Strings](#exercise-5-statements-about-c-strings)
- [Exercise 6: Reading a Name with `cin`](#exercise-6-reading-a-name-with-cin)
- [Exercise 7: Reading a String with Spaces](#exercise-7-reading-a-string-with-spaces)
- [Exercise 8: The Namespace of the Standard Library](#exercise-8-the-namespace-of-the-standard-library)
- [Exercise 9: The Header File That Declares `cin` and `cout`](#exercise-9-the-header-file-that-declares-cin-and-cout)
- [Exercise 10: Printing Stars with Nested `for` Statements](#exercise-10-printing-stars-with-nested-for-statements)

---

<br>

## Exercise 1: Statements About the `main()` Function

**Textbook Problem:** Chapter 2 theory problem 2

**Problem:** Which of the following statements about the `main()` function in C++ is wrong?

- ① The return type of `main()` defined by the C++ standard is `int`.
- ② Even if written as `void main()`, it is accepted by most compilers.
- ③ The `main()` function must have a `return` statement.
- ④ The `main()` function does not have to return the integer 0.

**[Solution](Solutions.md#exercise-1-statements-about-the-main-function)**

---

<br>

## Exercise 2: Declaring Variables After Statements

**Textbook Problem:** Chapter 2 theory problem 4

**Problem:** The following code causes a compile error when compiled with a C compiler but compiles normally with a C++ compiler.

```cpp
int a;
a = 4;
int square = a * a;
```

- (1) What compile error occurs when it is compiled with a C compiler?
- (2) Which feature of the C++ language makes it compile normally with a C++ compiler?
- (3) What are the advantages and disadvantages of this feature?

**[Solution](Solutions.md#exercise-2-declaring-variables-after-statements)**

---

<br>

## Exercise 3: Using `cout` Without `std::`

**Textbook Problem:** Chapter 2 theory problem 6

**Problem:** Fill in the blank so that the following program has no compile error.

(1)

```cpp
#include <iostream>
______________________
int main() {
   int count = 0;
   std::cin >> count;
   cout << count + 1;
   return 0;
}
```

(2)

```cpp
#include <iostream>
______________________
int main() {
   cout << "I love C++" << endl;
   cout << "I love programming";
   return 0;
}
```

**[Solution](Solutions.md#exercise-3-using-cout-without-std)**

---

<br>

## Exercise 4: Correcting C++ Code

**Textbook Problem:** Chapter 2 theory problem 8

**Problem:** Correct the wrong part, if any, in the following C++ code.

- (1) `using std::cin;`
- (2) `int year = 1; // year은 년도를 나타낸다.` (the comment reads "year represents the year")
- (3) `int n=1; cout >> n+200;`
- (4) `int year = 2014; cout << 2014+"년";` (`"년"` is the Korean word for year)

**[Solution](Solutions.md#exercise-4-correcting-c-code)**

---

<br>

## Exercise 5: Statements About C++ Strings

**Textbook Problem:** Chapter 2 theory problem 10

**Problem:** Answer O or X to the following statements about C++ strings.

- (1) A C-string is the way the C language handles strings. ( )
- (2) C-string strings are used in C++. ( )
- (3) In C++, the `string` class is used to handle strings. ( )
- (4) When `char name[]="C++";` is compiled, the size of the array `name[]` is 3. ( )
- (5) Executing `char name[10]; cin >> name;` reads at most 9 characters from the keyboard, including whitespace characters. ( )

**[Solution](Solutions.md#exercise-5-statements-about-c-strings)**

---

<br>

## Exercise 6: Reading a Name with `cin`

**Textbook Problem:** Chapter 2 theory problem 12

**Problem:** Consider the following program.

```cpp
#include <iostream>
int main() {
   char name[20];
   std::cout << "이름을 입력하세요?";
   std::cin >> name;
   std::cout << name << "님 환영합니다";
   return 0;
}
```

(1) What is the result when the program is run and Kitae is entered from the keyboard as follows? The prompt reads "Enter your name?" in Korean.

```text
이름을 입력하세요?Kitae
```

(2) What is the result when the program is run and Kitae Hwang is entered from the keyboard as follows?

```text
이름을 입력하세요?Kitae Hwang
```

**[Solution](Solutions.md#exercise-6-reading-a-name-with-cin)**

---

<br>

## Exercise 7: Reading a String with Spaces

**Textbook Problem:** Chapter 2 theory problem 14

**Problem:** Assuming `char buf[100];` is declared, which code is wrong for reading a string until the `<Enter>` key is pressed, as follows?

```text
I love C++<Enter>
```

- ① `cin >> buf;`
- ② `cin.getline(buf, 11);`
- ③ `cin.getline(buf, 20, '\n');`
- ④ `cin.getline(buf, 11, '.');`

**[Solution](Solutions.md#exercise-7-reading-a-string-with-spaces)**

---

<br>

## Exercise 8: The Namespace of the Standard Library

**Textbook Problem:** Chapter 2 theory problem 16

**Problem:** What is the namespace in which the entire C++ standard library is declared?

**[Solution](Solutions.md#exercise-8-the-namespace-of-the-standard-library)**

---

<br>

## Exercise 9: The Header File That Declares `cin` and `cout`

**Textbook Problem:** Chapter 2 theory problem 18

**Problem:** In standard C++, in which header file are the `cin` and `cout` objects declared?

**[Solution](Solutions.md#exercise-9-the-header-file-that-declares-cin-and-cout)**

---

<br>

## Exercise 10: Printing Stars with Nested `for` Statements

**Textbook Problem:** Chapter 2 theory problem 20

**Problem:** The goal is to write code that produces the following output. Complete the following C++ program.

```cpp
#include <iostream>
using namespace std;
int main() {
   for(int n=0; n<4; n++) {
      ______________________
      ______________________
      ______________________
   }
}
```

```text
*
**
***
****
```

**[Solution](Solutions.md#exercise-10-printing-stars-with-nested-for-statements)**
