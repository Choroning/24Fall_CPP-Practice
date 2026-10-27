# HW8 Code Walkthrough — Friend Functions and Operator Overloading

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

> **Learning Objectives:**
> 1. Read the class declarations, the friend declarations, the definitions of the operator functions, and `main()` of the three solutions line by line and explain the role of each part
> 2. Write the function call into which an operator expression is turned and trace the state of the objects with each call
> 3. Compare line by line how the friend function version kept in a comment corresponds to the member function version

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: Adding an Integer and a Circle](#2-q1cpp-adding-an-integer-and-a-circle)
- [3. Q2.cpp: Prefix and Postfix ++](#3-q2cpp-prefix-and-postfix-)
- [4. Q3.cpp: The >> and << Operators of Matrix](#4-q3cpp-the--and--operators-of-matrix)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the three solutions puts, in one file, the header includes, the class declaration, the definitions of the operator functions, and `main()`, in that order. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW8 Assignment Explanation](Assignment-Explanation.md).

| File | Class | Operators overloaded | Approach |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Circle` | `int + Circle` | A friend function defined outside the class |
| [`Q2.cpp`](Solution/Q2.cpp) | `Circle` | Prefix `++`, postfix `++` | Friend functions defined outside the class |
| [`Q3.cpp`](Solution/Q3.cpp) | `Matrix` | `>>`, `<<` | Member functions (the friend function version is in a comment) |

*Table 1. Layout of the HW8 solution files*

---

<br>

## 2. Q1.cpp: Adding an Integer and a Circle

```cpp
/**
 * @file    A8Q1.cpp
 * @brief   Q1. 프렌드 함수로 중복한 정수 + Circle 연산 (13주차 실습 1)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle (int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
    friend Circle operator+ (int op1, Circle op2);
};

Circle operator+ (int op1, Circle op2) {
    Circle tmp;
    tmp.radius = op1 + op2.radius;
    return tmp;
}

int main() {
    Circle a(5), b(4);
    b = 1 + a;
    a.show();
    b.show();
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 12 | `int radius;` | The radius, a `private` member |
| 14-16 | `Circle (int radius = 0)` | A constructor with a default parameter, so an object can also be created without arguments, as in `Circle tmp;` |
| 17-19 | `show()` | Prints in the form `radius = 5 인 원` |
| 20 | `friend Circle operator+ (int op1, Circle op2);` | Allows the external function `operator+` to access the `private` members |
| 23-27 | `Circle operator+ (int op1, Circle op2)` | Stores `op1 + op2.radius` in `tmp.radius` and returns `tmp` by value |
| 30 | `Circle a(5), b(4);` | Creates circles with radii 5 and 4 |
| 31 | `b = 1 + a;` | Calls `operator+(1, a)` and assigns the returned temporary object to `b` |
| 32-33 | Two `show()` calls | Print `a` and `b` |

*Table 2. Line-by-line explanation of Q1.cpp*

When `b = 1 + a;` runs, `a` is copied into `op2` (radius 5), and `tmp` starts at radius 0 through the default constructor and becomes `1 + 5 = 6`. `tmp` is returned and assigned to `b`, so the radius of `b` changes from 4 to 6, and `a` stays 5.

---

<br>

## 3. Q2.cpp: Prefix and Postfix ++

```cpp
/**
 * @file    A8Q2.cpp
 * @brief   Q2. 프렌드 함수로 중복한 전위, 후위 ++ 연산자 (13주차 실습 2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle (int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
    friend Circle operator++(Circle& op);
    friend Circle operator++(Circle& op, int notUsed);
};

Circle operator++(Circle& op) {
    op.radius++;
    return op;
}
Circle operator++(Circle& op, int notUsed) {
    Circle tmp = op;
    op.radius++;
    return tmp;
}

int main() {
    Circle a(5), b(4);
    ++a;        // 반지름을 1 증가시킴
    b = a++;    // 반지름을 1 증가시킴
    a.show();
    b.show();
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 20 | `friend Circle operator++(Circle& op);` | The friend declaration of the prefix `++` |
| 21 | `friend Circle operator++(Circle& op, int notUsed);` | The friend declaration of the postfix `++`; the `int` parameter marks it as different from the prefix version |
| 24-27 | Prefix `operator++` | Increments `op.radius` and returns a copy of `op` after the increment |
| 28-32 | Postfix `operator++` | Keeps a copy of `op` before the increment in `tmp`, increments `op.radius`, and returns `tmp` |
| 35 | `Circle a(5), b(4);` | Creates circles with radii 5 and 4 |
| 36 | `++a;` | Calls `operator++(a)`, which makes `a` 6; the return value is not used |
| 37 | `b = a++;` | Calls `operator++(a, 0)`, which makes `a` 7, and the returned 6 is assigned to `b` |
| 38-39 | Two `show()` calls | Print `a` and `b` |

*Table 3. Line-by-line explanation of Q2.cpp*

| Statement executed | Call | Radius of the return value | `a` afterwards | `b` afterwards |
|:-----|:-----|-----:|-----:|-----:|
| `Circle a(5), b(4);` | Constructor twice | | 5 | 4 |
| `++a;` | `operator++(a)` | 6 | 6 | 4 |
| `b = a++;` | `operator++(a, 0)` | 6 | 7 | 6 |

*Table 4. Radii of the two objects as Q2 runs*

---

<br>

## 4. Q3.cpp: The >> and << Operators of Matrix

```cpp
/**
 * @file    A8Q3.cpp
 * @brief   Q3. Matrix 클래스의 <<, >> 연산자 (13주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Matrix {
    int ar[4];
public:
    Matrix() { for (int i = 0; i < 4; i++) ar[i] = 0; }
    Matrix(int a1, int a2, int b1, int b2) {
        ar[0] = a1;
        ar[1] = a2;
        ar[2] = b1;
        ar[3] = b2;
    }
    void show() {
        cout << "Matrix = { ";
        for (int i = 0; i < 4; i++) cout << ar[i] << ' ';
        cout << "}" << endl;
    }
    
    // (1) <<, >>를 Matrix의 멤버 함수로 구현
    int* operator >> (int op[]) {
        for (int i = 0; i < 4; i++) {
            op[i] = ar[i];
        }
        return op;
    }
    Matrix& operator << (int op[]) {
        for (int i = 0; i < 4; i++) {
            ar[i] = op[i];
        }
        return *this;
    }
    
    /* (2) <<, >>를 Matrix의 프렌드 함수로 구현
    friend int* operator>>(Matrix& op1, int op2[]) {
        for (int i = 0; i < 4; i++)
            op2[i] = op1.ar[i];
        return op2;
    }
        
    friend Matrix& operator<<(Matrix& op1, int op2[]) {
        for (int i = 0; i < 4; i++)
            op1.ar[i] = op2[i];
        return op1;
    }*/
};

int main() {
    Matrix a(4, 3, 2, 1), b;
    int x[4], y[4] = { 1, 2, 3, 4 };    // 2차원 행렬의 4개의 원소 값
    a >> x;     // a의 각 원소를 배열 x에 복사. x[]는 { 4, 3, 2, 1 }
    b << y;     // 배열 y의 원소 값을 b의 각 원소에 설정
    
    for (int i = 0; i < 4; i++)
        cout << x[i] << ' ';    // x[] 출력
    cout << endl;
    b.show();
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 12 | `int ar[4];` | The `private` array that stores the four elements of a 2×2 matrix |
| 14-20 | Two constructors | Initialize the four elements to 0 or to the four received values |
| 21-25 | `show()` | Prints the elements in the form `Matrix = { ... }` |
| 27-33 | Member `operator >>` | Copies the elements of `ar` into `op` and returns `op` |
| 34-39 | Member `operator <<` | Copies the elements of `op` into `ar` and returns a reference to `*this` |
| 41-52 | The friend version inside the block comment | Does the same work, taking `Matrix& op1` as the first parameter |
| 56-57 | Declarations of the objects and arrays | `a` is (4, 3, 2, 1), `b` is the zero matrix, `y` is {1, 2, 3, 4}, and `x` has no values yet |
| 58 | `a >> x;` | `a.operator>>(x)` fills `x` with {4, 3, 2, 1} |
| 59 | `b << y;` | `b.operator<<(y)` changes the elements of `b` to {1, 2, 3, 4} |
| 61-64 | Output | Prints the elements of `x` and `b` |

*Table 5. Line-by-line explanation of Q3.cpp*

| Member function version | Friend function version | Difference |
|:-----|:-----|:-----|
| `int* operator >> (int op[])` | `friend int* operator>>(Matrix& op1, int op2[])` | The left operand is the first parameter instead of `this` |
| `op[i] = ar[i];` | `op2[i] = op1.ar[i];` | The member is reached through a parameter |
| `Matrix& operator << (int op[])` | `friend Matrix& operator<<(Matrix& op1, int op2[])` | One more parameter for the same reason |
| `return *this;` | `return op1;` | The object returned is the same; only the way of naming it differs |

*Table 6. Correspondence between the member function version and the friend function version of Q3*

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Expressions and calls | `1 + a` becomes `operator+(1, a)`, `++a` becomes `operator++(a)`, `a++` becomes `operator++(a, 0)`, and `a >> x` becomes `a.operator>>(x)`. |
| Trace of Q1 | After `b = 1 + a;`, `a` is 5 and `b` is 6. |
| Trace of Q2 | `a` changes to 5, 6, and 7, and `b` receives 6, the value before the increment, returned by the postfix `++`. |
| Trace of Q3 | `x` receives the elements of `a`, {4, 3, 2, 1}, and `b` changes to the elements of `y`, {1, 2, 3, 4}. |
| Correspondence of the two versions | The friend function version receives the left operand as the first parameter instead of `this`, and what it does and returns are the same. |

---

<br>

## Self-Check Questions

1. **Initial value of the temporary object:** Why does `Circle tmp;` at line 24 of Q1 compile?

   <details>
   <summary>Answer</summary>

   The constructor at line 14 has the default parameter `int radius = 0`, so it can be called without arguments and serves as the default constructor. Without the default value there would be no constructor that takes no arguments, and `Circle tmp;` would be a compile error.

   </details>

2. **Using the return value:** What is the output if line 37 of Q2 is changed to `b = ++a;`?

   <details>
   <summary>Answer</summary>

   After line 36, `a` is 6; in `b = ++a;`, `a` becomes 7 and the incremented value 7 is assigned to `b`. The output is `radius = 7 인 원` twice.

   </details>

3. **Chaining:** If line 59 of Q3 is changed to `b << y << x;`, what does line 64 print?

   <details>
   <summary>Answer</summary>

   `b << y` returns a reference to `b`, so `b << x` runs next and changes the elements of `b` to the values of `x`, {4, 3, 2, 1}. Line 64 prints `Matrix = { 4 3 2 1 }`.

   </details>

4. **Using the return value as an array:** What is printed if `cout << (a >> x)[2];` runs in Q3?

   <details>
   <summary>Answer</summary>

   `a >> x` copies {4, 3, 2, 1} into `x` and returns the address of `x`, so `(a >> x)[2]` is `x[2]`, and 2 is printed.

   </details>
