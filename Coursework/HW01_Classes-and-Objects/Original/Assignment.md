# HW1 Assignment Handout — Classes and Objects

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the first assignment of section 3 of Computer Language and Training Ⅱ. It consists of two programming exercises from Chapter 3 of the lecture textbook and one problem from the Week 4 lecture slides.

- **Deadline:** 2024-09-30 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden automatically.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.
- **Notice:** Important lab problems are also given as lab assignments, independently of the lecture course.

---

<br>

## Problem 1. Lecture Textbook Chapter 3 Programming Exercise 3

A program for a bank needs an `Account` class that represents one bank account. The account information consists of three member variables that represent the owner, the account number, and the balance. Write the `Account` class so that the following `main()` function prints the sample run.

```cpp
int main() {
    Account a("kitae", 1, 5000);   // 계좌 번호 1, 잔액 5000원, 주인이 kitae인 계좌 생성
    a.deposit(50000);              // 50000원 입금
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
    int money = a.withdraw(20000); // 20000원 출금, withdraw()는 출금한 금액을 리턴
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
}
```

**Sample Run**

```text
kitae의 잔액은 55000
kitae의 잔액은 35000
```

---

<br>

## Problem 2. Lecture Textbook Chapter 3 Programming Exercise 5

Write a `Random` class that generates random integers. The `Random` class has a constructor and the member functions `next()` and `nextInRange()`. `next()` returns a random integer from 0 to `RAND_MAX`, and `nextInRange(s, f)` returns a random integer from `s` to `f`. Write it in one cpp file together with the following `main()` function.

```cpp
int main() {
    Random r;
    cout << "-- 0에서 " << RAND_MAX << "까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++) {
        int n = r.next();              // 0에서 RAND_MAX 사이의 랜덤한 정수
        cout << n << ' ';
    }
    cout << endl << endl << "-- 2에서 " << "4까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++) {
        int n = r.nextInRange(2, 4);   // 2에서 4 사이의 랜덤한 정수
        cout << n << ' ';
    }
    cout << endl;
}
```

The textbook gives two hints. Random integers are generated with `rand()` from `<cstdlib>`, and the seed is set with `time()` from `<ctime>`, as in `srand((unsigned)time(0));`, so that each run produces different numbers. `RAND_MAX` is a constant defined in `<cstdlib>`. The output repeats twice a line of guidance text (in Korean, "random integers from 0 to `RAND_MAX`" and "from 2 to 4") followed by a line of 10 random integers separated by spaces, and the values change with every run.

---

<br>

## Problem 3. Week 4 Lecture Slides [Problem 1-3]

Write and run a `Point` class that represents a point so that the program prints the output below. The `Point` class is organized as follows.

- **`private` member variables:** the coordinates `int x, y;`
- **Default constructor:** initializes the member variables `x` and `y` to 0 (declared and defined together).
- **Two-argument constructor:** initializes `x` and `y` with the two integers passed to it (declaration and definition separated).
- **Coordinate output function `showP()`:** declaration and definition separated.
- **Get/set functions for `x`:** declared and defined together.
- **Get/set functions for `y`:** declaration and definition separated.

The skeleton of the `main()` function is as follows; the `// 생략` (omitted) parts read and set the coordinates of `p2`.

```cpp
int main() {
    Point p1, p2;
    int temp;

    cout << "Input p2.x: ";
    // 생략

    cout << "Input p2.y: ";
    // 생략

    cout << "p1. ";
    p1.showP();
    cout << "p2. ";
    p2.showP();
}
```

**Sample Run** (with the inputs `-3` and `4`)

```text
Input p2.x: -3
Input p2.y: 4
p1. x: 0, y: 0
p2. x: -3, y: 4
```

The slides build the same `Point` class step by step in the preceding [Problem 1-1] and [Problem 1-2]. [Problem 1-1] reads the coordinates of two points and prints the distance between them with `sqrt()` from `<cmath>`, and [Problem 1-2] prints the coordinates of `Point p1;` and `Point p2(3, 4);` with a `Point` class that has a default constructor, a two-argument constructor, and `showP()`. When a problem says nothing about access control, the member variables and member functions are `public`. [Problem 1-3] adds `private` access control and get/set functions to this.
