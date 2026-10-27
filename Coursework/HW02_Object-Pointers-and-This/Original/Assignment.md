# HW2 Assignment Handout — Object Pointers and the this Pointer

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the second assignment of section 3 of Computer Language and Training Ⅱ. It consists of one problem from the Week 5 lecture slides, one free coding problem that handles an object through a pointer, and two items that prepare for the next class (an analysis of code from the slides and free code about the `this` pointer).

- **Deadline:** 2024-10-07 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden automatically.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.

---

<br>

## Problem 1. Week 5 Lecture Slides [Problem 5-2]

Define a complex number class `Complex` with a real part and an imaginary part as follows.

- **`private` member variables:** the integers `real` and `image`
- **Default constructor:** initializes the member variables `real` and `image` to 1 each.
- **Two-parameter constructor:** when two integer arguments are given at object creation, initializes `real` and `image` with those argument values.
- **Set member functions:** each can modify one of the `private` values of the real part and the imaginary part, `real` and `image`.
- **`showC()` member function:** prints the `private` member variables `real` and `image` in the form of a complex number.

Reuse the class code written in the preceding [Problem 5-1] as it is, add the macro constant `MAX` at the top of the code, and complete the `main()` function according to the following skeleton.

```cpp
#define MAX 2 // 매크로 상수 코드 상단에 추가

int main() {
    // Complex 배열 객체 data 생성: 배열 크기 MAX, 초기값 없음
    // 배열 data의 주소를 할당 받는 포인터 p 생성

    // 객체 배열 data 정보 출력: for문 활용
    cout << "--------------" << endl;

    // 첫 번째 배열 객체의 real 값을 3으로 업데이트
    // 두 번째 배열 객체의 image 값 포인터 p를 활용하여, -8로 업데이트

    // 포인터 p를 이용하여, 객체 배열의 정보 출력: for문 활용
}
```

**Sample Run**

```text
1 + 1i
1 + 1i
--------------
3 + 1i
1 + -8i
```

[Problem 5-1] uses the same `Complex` class to create an object `vx` and an object `vy` with arguments, and declares a pointer `p` that receives the address of `vy`. After printing the information of the two objects, it changes the `real` of `vx` to 4 and, through the pointer `p`, the `image` of `vy` to 9, and prints the information of the two objects again. [Problem 5-2] does the same work with an object array and pointer arithmetic.

---

<br>

## Problem 2. Free Code That Refers to an Object Through a Pointer

Freely write code that points to an object with a pointer and accesses the members of the object through that pointer, and analyze it.

---

<br>

## Problem 3. Preview: Analysis of the Code on p.15 of the Week 5 Lecture Slides

The following code from the Week 5 lecture slides shows the allocation and release of dynamic memory. Analyze what this code does.

```cpp
#include <iostream>
using namespace std;

int main() {
    int size;
    cout << "Input size: ";
    cin >> size;

    // int arrStatic[size] = { }; 일반 변수: 가능한가?
    int arrStatic[4] = { 1, 3, 5 };

    for (int i = 0; i < 4; i++)
        cout << arrStatic[i] << ' ';
    cout << endl;

    // 동적 메모리는 가변 길이 배열 선언 가능
    int* pDynamic = new int[size];

    for (int i = 0; i < size; i++)
        pDynamic[i] = i * 3;

    for (int i = 0; i < size; i++)
        cout << pDynamic[i] << ' ';
    cout << endl;

    delete[] pDynamic;
    // pDynamic = NULL;
    pDynamic = nullptr;
}
```

**Sample Run** (with the input `5`)

```text
Input size: 5
1 3 5 0
0 3 6 9 12
```

For line 10, the slides add that an ordinary array must be created with a fixed size, while a dynamic array or a `vector` can create an array of variable length. The comment on line 9 asks whether an array whose size is an ordinary variable can be declared.

---

<br>

## Problem 4. Preview: Free Code About the this Pointer and Its Analysis

Freely write code that uses the `this` pointer, and analyze it.
