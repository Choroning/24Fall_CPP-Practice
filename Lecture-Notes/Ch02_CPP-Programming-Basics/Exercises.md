# Chapter 2 Exercises — C++ Programming Basics

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 2

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Textbook Theory Problems](#textbook-theory-problems)

---

<br>

## Textbook Theory Problems

**Textbook:** Even-numbered theory problems of the Chapter 2 exercises

### Theory Problem 2

Which of the following statements about the `main()` function in C++ is wrong?

- ① The return type of `main()` defined by the C++ standard is `int`.
- ② Even if written as `void main()`, it is accepted by most compilers.
- ③ The `main()` function must have a `return` statement.
- ④ The `main()` function does not have to return the integer 0.

**[Solution](Solutions.md#theory-problem-2)**

### Theory Problem 4

The following code causes a compile error when compiled with a C compiler but compiles normally with a C++ compiler.

```cpp
int a;
a = 4;
int square = a * a;
```

- (1) What compile error occurs when it is compiled with a C compiler?
- (2) Which feature of the C++ language makes it compile normally with a C++ compiler?
- (3) What are the advantages and disadvantages of this feature?

**[Solution](Solutions.md#theory-problem-4)**

### Theory Problem 6

Fill in the blank so that the following program has no compile error.

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

**[Solution](Solutions.md#theory-problem-6)**

### Theory Problem 8

Correct the wrong part, if any, in the following C++ code.

- (1) `using std::cin;`
- (2) `int year = 1; // year은 년도를 나타낸다.` (the comment reads "year represents the year")
- (3) `int n=1; cout >> n+200;`
- (4) `int year = 2014; cout << 2014+"년";` (`"년"` is the Korean word for year)

**[Solution](Solutions.md#theory-problem-8)**

### Theory Problem 10

Answer O or X to the following statements about C++ strings.

- (1) A C-string is the way the C language handles strings. ( )
- (2) C-string strings are used in C++. ( )
- (3) In C++, the `string` class is used to handle strings. ( )
- (4) When `char name[]="C++";` is compiled, the size of the array `name[]` is 3. ( )
- (5) Executing `char name[10]; cin >> name;` reads at most 9 characters from the keyboard, including whitespace characters. ( )

**[Solution](Solutions.md#theory-problem-10)**

### Theory Problem 12

Consider the following program.

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

**[Solution](Solutions.md#theory-problem-12)**

### Theory Problem 14

Assuming `char buf[100];` is declared, which code is wrong for reading a string until the `<Enter>` key is pressed, as follows?

```text
I love C++<Enter>
```

- ① `cin >> buf;`
- ② `cin.getline(buf, 11);`
- ③ `cin.getline(buf, 20, '\n');`
- ④ `cin.getline(buf, 11, '.');`

**[Solution](Solutions.md#theory-problem-14)**

### Theory Problem 16

What is the namespace in which the entire C++ standard library is declared?

**[Solution](Solutions.md#theory-problem-16)**

### Theory Problem 18

In standard C++, in which header file are the `cin` and `cout` objects declared?

**[Solution](Solutions.md#theory-problem-18)**

### Theory Problem 20

The goal is to write code that produces the following output. Complete the following C++ program.

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

**[Solution](Solutions.md#theory-problem-20)**
