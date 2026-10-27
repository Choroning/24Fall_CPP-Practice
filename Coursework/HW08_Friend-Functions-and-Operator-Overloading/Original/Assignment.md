# HW8 Assignment Handout — Friend Functions and Operator Overloading

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the eighth assignment of section 3 of Computer Language and Training Ⅱ. It consists of three lab problems from the Week 13 lecture slides.

- **Deadline:** 2024-12-02 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden automatically.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.

---

<br>

## Problem 1. Week 13 Lecture Slides [Lab 1]

Implement an operator so that the operation `b = 1 + a;` works in the following code. `b = 1 + a;` changes the radius of `b` to the radius of `a` plus 1.

```cpp
class Circle {
    int radius;
public:
    Circle(int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
};

int main() {
    Circle a(5), b(4);
    b = 1 + a; // b의 반지름을 a의 반지름에 1을 더한 것으로 변경
    a.show();
    b.show();
}
```

**Sample Run**

```text
radius = 5 인 원
radius = 6 인 원
```

---

<br>

## Problem 2. Week 13 Lecture Slides [Lab 2]

For the same `Circle` class as in Problem 1, write the `++` operator functionality used in the following `main()` function as friend functions.

```cpp
class Circle {
    int radius;
public:
    Circle(int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
};
int main() {
    Circle a(5), b(4);
    ++a; // 반지름을 1 증가시킨다.
    b = a++; // 반지름을 1 증가시킨다.
    a.show();
    b.show();
}
```

**Sample Run**

```text
radius = 7 인 원
radius = 6 인 원
```

---

<br>

## Problem 3. Week 13 Lecture Slides [Lab 3]

The `Matrix` class, which abstracts a two-dimensional matrix, stores the four elements of a 2×2 matrix in the array `ar` and is defined as follows.

```cpp
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
};
```

The following code uses this class. `a >> x` copies each element of `a` into the array `x`, and `b << y` sets each element of `b` to the element values of the array `y`.

```cpp
int main() {
    Matrix a(4, 3, 2, 1), b;
    int x[4], y[4] = { 1, 2, 3, 4 }; // 2차원 행렬의 4 개의 원소 값
    a >> x; // a의 각 원소를 배열 x에 복사. x[]는 {4, 3, 2, 1}
    b << y; // 배열 y의 원소 값을 b의 각 원소에 설정

    for (int i = 0; i<4; i++)
        cout << x[i] << ' '; // x[] 출력
    cout << endl;
    b.show();
}
```

1. Implement the `<<` and `>>` operator functions as member functions of `Matrix`.
2. Implement the `<<` and `>>` operator functions as friend functions of `Matrix`.

**Sample Run**

```text
4 3 2 1
Matrix = { 1 2 3 4 }
```
