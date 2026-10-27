# HW3 Assignment Handout — Dynamic Memory

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the third assignment of section 3 of Computer Language and Training Ⅱ. It consists of four lab problems from the Week 6 lecture slides ([Problem 6-1], [Problem 6-2], [Problem 6-4], and [Problem 6-5]).

- **Deadline:** 2024-10-14 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden automatically.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.

---

<br>

## Problem 1. Week 6 Lecture Slides [Problem 6-1]

Define a complex number class `Complex` with a real part and an imaginary part under the following conditions.

- **Default constructor:** initializes the member variables `real` and `image` to 1 each.
- **Two-parameter constructor:** when two integer arguments are given at creation, initializes `real` and `image` with those argument values.
- **Set member functions:** add set member functions that can each modify one of the `private` values of the real part and the imaginary part.
- **`pCom()` member function:** prints the `private` member variables on the screen.
- **Dynamic creation:** create two complex number objects, each dynamically.
- **Naming rule:** use the same names, `real` and `image`, for the member variables and for the parameters of the constructor and the member functions.

Write the `main()` function according to the following skeleton.

```cpp
int main() {
    // Complex 동적 첫 번째 객체 생성 (초기값 정보 없음). 포인터 a로 관리
    // Complex 동적 두 번째 객체 생성 (초기값 정보 없음). 포인터 b로 관리
    // 포인터 a, b로 관리되는 Complex 동적 객체의 정보 출력.
    // 첫 번째 객체의 real 값을 4로 업데이트
    // 두 번째 객체의 image 값을 9로 업데이트
    // 2개의 동적 객체 배열의 정보 출력
    // 동적 메모리 해제
}
```

---

<br>

## Problem 2. Week 6 Lecture Slides [Problem 6-2]

To manage member information, implement a `Member` class with the following information and complete the `main()` code.

- **`private` member variables:** `name` (`string`), `id` (`string`), `age` (`int`)
- **Default constructor:** initializes the member variables to `"noname"`, `"none"`, and 0.
- **Constructor with three arguments:** initializes `name`, `id`, and `age` with the three arguments.
- **Set functions:** each can modify the value of one `private` member variable.
- **Get functions:** each makes the value of one `private` member variable available.
- **`printM()` function:** prints `name`, `id`, and `age` on the screen.

The `main()` code works in the following order.

1. Create the object `Kim`.
2. Create the object `Lee` with the initial values {Lee Hi, 2016001, 22}.
3. Print the information of the objects `Kim` and `Lee` on the screen.
4. Change the information of the object `Kim` to {Kim Haha, 2016020, 29}.
5. Print the information of the object `Kim` on the screen.

---

<br>

## Problem 3. Week 6 Lecture Slides [Problem 6-4]

Write a `main()` function that dynamically allocates space for 5 integers as an array, reads 5 integers, computes and prints their average, and then destroys the array.

**Sample Run** (with the input `1 2 4 5 10`; in Korean, the prompt asks for five integers and the second line gives the average)

```text
정수 5개 입력>> 1 2 4 5 10
평균 4.4
```

---

<br>

## Problem 4. Week 6 Lecture Slides [Problem 6-5]

The following `Circle` class abstracts a circle. Read the number of circles from the user and create an array of that many `Circle` objects in dynamic memory. Then read the radius of each circle and print the number of circles whose area is 100 or more.

```cpp
class Circle {
    int radius; // 원의 반지름 값
public:
    void setRadius(int radius); // 반지름을 설정한다.
    double getArea(); // 면적을 리턴한다.
};

void Circle::setRadius(int radius) {
    this->radius = radius;
}

double Circle::getArea() {
    return 3.14*radius*radius;
}
```

Write the `main()` function according to the following skeleton.

```cpp
int main() {
    // Circle 동적 배열을 관리할 수 있는 포인터 p 선언
    // Circle 동적 배열 크기 정보 화면에서 입력 받음
    // 입력 받은 크기의 동적 배열을 생성하고, p로 관리
    // 입력 받은 크기의 개별 객체의 반지름 정보 입력 받음
    // 면적이 100 이상인 원 카운팅.
    // 해당 정보 화면 출력
    // 동적 메모리 해제
}
```

**Sample Run** (with the radii 3, 7, 2, 9, and 1 entered for five circles; in Korean, the prompts ask for the number of circles and each radius, and the last line reports that 2 circles have an area greater than 100)

```text
원의 개수 >> 5
원 1의 반지름 >> 3
원 2의 반지름 >> 7
원 3의 반지름 >> 2
원 4의 반지름 >> 9
원 5의 반지름 >> 1
면적이 100보다 큰 원은 2개 입니다
```
