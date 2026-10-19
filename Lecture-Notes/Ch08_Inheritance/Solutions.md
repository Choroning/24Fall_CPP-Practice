# Chapter 8 Solutions — Inheritance

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 8

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Color and Position of a Point](#exercise-1-color-and-position-of-a-point)
- [Exercise 2: Student Grade Management](#exercise-2-student-grade-management)

---

<br>

## Exercise 1: Color and Position of a Point

**Problem:** Without modifying the `Point` class, which has private coordinates `x` and `y` and a protected `move()` that changes the position, define a `ColorPoint` class that inherits from it so that the given `main()` runs. `ColorPoint` needs a constructor that initializes the color and position, plus `setPoint()`, `setColor()`, and `show()`. The full problem and sample run are in [Exercise 1](Exercises.md#exercise-1-color-and-position-of-a-point).

### Solution

`ColorPoint` adds a `string color` to store the color, and a single constructor with default arguments handles all three declarations in `main()`. Since `Point` has no default constructor, the constructor of `ColorPoint` must explicitly call `Point(x, y)` in its initializer list. Otherwise the compiler compiles a call to `Point()`, which is a compile error (Section 3.1 of the [Chapter 8 notes](Concepts_Lecture.md)).

| Declaration | Arguments received by the constructor | Result |
|:-----|:-----|:-----|
| `ColorPoint cp1(5, 5, "RED")` | x=5, y=5, color="RED" | RED, (5, 5) |
| `ColorPoint cp2(5, 5)` | x=5, y=5, color="BLACK" (default) | BLACK, (5, 5) |
| `ColorPoint zeroPoint` | x=0, y=0, color="BLACK" (all defaults) | BLACK, (0, 0) |

*Table 1. `ColorPoint` declarations and the constructor's default arguments*

`x` and `y` are private members of `Point`, so `ColorPoint` cannot change them directly. `setPoint()` changes the position through `move()`, a protected member function of `Point` that derived classes can call, and `show()` reads the coordinates through the public member functions `getX()` and `getY()`.

> **Note:** **The `<string>` header.** This code uses `string` without `#include <string>`. It compiles with the compiler used because `<iostream>` happens to include `<string>`, but the standard does not guarantee this, so adding `#include <string>` is safer for portability.

The code is as follows ([`Lab_Q1.cpp`](../../Coursework/Lab08_Inheritance/Lab_Q1.cpp)).

```cpp
/**
 * @file    A10204-1_C08Q1.cpp
 * @brief   Q1. 점의 색상과 위치 정보
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-20
 */

#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    Point(int x, int y) { this->x = x; this->y = y; }
    int getX() { return x; }
    int getY() { return y; }
protected:
    void move(int x, int y) { this->x = x; this->y = y; }
};

class ColorPoint : public Point {
    string color;
public:
    ColorPoint(int x = 0, int y = 0, string color = "BLACK") : Point(x, y) {
        this->color = color;
    }
    void setPoint(int x, int y) {
        move(x, y);
    }
    void setColor(string color) {
        this->color = color;
    }
    void show() {
        cout << color << "색으로 (" << getX() << ", " << getY() << ")에 위치한 점입니다." << endl;
    }
};

int main() {
   ColorPoint cp1(5, 5, "RED");  // 색상 "RED", 위치 (5, 5)로 초기화
   cp1.show();
   cp1.setPoint(10, 20);         // 위치 (10, 20)으로 설정
   cp1.setColor("BLUE");         // 색상 "BLUE"로 설정
   cp1.show();

   ColorPoint cp2(5, 5);         // 색상 "BLACK", 위치 (5, 5)로 초기화
   cp2.show();
   cp2.setPoint(3, 7);           // 위치 (3, 7)로 설정
   cp2.setColor("GREEN");        // 색상 "GREEN"으로 설정
   cp2.show();

   ColorPoint zeroPoint;         // 색상 "BLACK", 위치 (0, 0)로 초기화
   zeroPoint.show();

   return 0;
}
```

It compiles without warnings with `g++ -std=c++17 -Wall`, and the output is as follows.

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.
```

The output matches the sample run. The last line of the sample run, `Program ended with exit code: 0`, and the blank line before it are printed by Xcode after the program ends and are not output of the program.

---

<br>

## Exercise 2: Student Grade Management

**Problem:** For the classes `FirstGrade`, `SecondGrade`, and `ThirdGrade`, whose subjects differ by grade, gather the common elements (name, Korean score, informatics score) into a `Student` class and design the three classes to inherit from `Student`, so that the given `main()` runs. The name is private and the subject scores are protected. The full problem, class diagram, and sample run are in [Exercise 2](Exercises.md#exercise-2-student-grade-management).

### Solution

`Student` has the name `name` as private and the Korean `kor` and informatics `pro` scores as protected. `getName()`, which returns the name, and the constructor, which initializes the three values, are protected. Because the constructor is protected, a `Student` object cannot be created on its own, only as part of a derived class object, and `getName()` is called only from `display()` of the derived classes.

Each derived class adds the scores of its own grade as private members and calls `Student(s, kors, pros)` in its constructor's initializer list to initialize the common part. This call is required because `Student` has no default constructor. `display()` reads the private `name` through `getName()` and reads the protected `kor` and `pro` directly. Since the subjects differ by grade, `getAverage()` and `display()` are defined separately in each derived class.

`getAverage()` divides the sum by `3.0` or `4.0` to perform floating-point division. Dividing by `3` would be integer division and drop the fraction, as in 259 / 3 = 86. `cout` prints up to 6 significant digits by default, so 86.3333... is printed as `86.3333`.

| Class (student) | Scores | Sum | Average |
|:-----|:-----|:-----|:-----|
| `FirstGrade` (홍길동) | 90, 81, 88 | 259 | 259 / 3.0 = 86.3333 |
| `SecondGrade` (장영실) | 73, 91, 99, 68 | 331 | 331 / 4.0 = 82.75 |
| `ThirdGrade` (정도전) | 81, 53, 80, 96 | 310 | 310 / 4.0 = 77.5 |

*Table 2. Average calculation by grade*

The code is as follows ([`Lab_Q2.cpp`](../../Coursework/Lab08_Inheritance/Lab_Q2.cpp)).

```cpp
/**
 * @file    A10204-1_C08Q2.cpp
 * @brief   Q2. 학생 성적 관리
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-20
 */

#include <iostream>
using namespace std;

class Student {
    string name;
protected:
    int kor;
    int pro;
    string getName() {
        return name;
    }
    Student(string s, int kors, int pros) {
        name = s;
        kor = kors;
        pro = pros;
    }
};

class FirstGrade : public Student {
    int mat;
public:
    FirstGrade(string s, int kors, int pros, int mats) : Student(s, kors, pros) {
        mat = mats;
    }
    double getAverage() {
        return (kor + pro + mat) / 3.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "수학: " << mat << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

class SecondGrade : public Student {
    int sci;
    int mor;
public:
    SecondGrade(string s, int kors, int pros, int scis, int mors) : Student(s, kors, pros) {
        sci = scis;
        mor = mors;
    }
    double getAverage() {
        return (kor + pro + sci + mor) / 4.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "과학: " << sci << endl;
        cout << "도덕: " << mor << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

class ThirdGrade : public Student {
    int eng;
    int his;
public:
    ThirdGrade(string s, int kors, int pros, int engs, int hiss) : Student(s, kors, pros) {
        eng = engs;
        his = hiss;
    }
    double getAverage() {
        return (kor + pro + eng + his) / 4.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "영어: " << eng << endl;
        cout << "한국사: " << his << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

int main() {
    // 이름, 국어, 정보, 수학 점수 초기화
    FirstGrade f("홍길동", 90, 81, 88);

    // 이름, 국어, 정보, 과학, 도덕 점수 초기화
    SecondGrade s("장영실", 73, 91, 99, 68);

    // 이름, 국어, 정보, 영어, 한국사 점수 초기화
    ThirdGrade t("정도전", 81, 53, 80, 96);
    
    f.display();
    s.display();
    t.display();
    
    return 0;
}
```

It compiles without warnings with `g++ -std=c++17 -Wall`, and the output is as follows.

```text
이름: 홍길동
국어: 90
정보: 81
수학: 88
---------------
평균: 86.3333

이름: 장영실
국어: 73
정보: 91
과학: 99
도덕: 68
---------------
평균: 82.75

이름: 정도전
국어: 81
정보: 53
영어: 80
한국사: 96
---------------
평균: 77.5
```

The output matches the sample run. The last line of the sample run, `Program ended with exit code: 0`, is a message printed by Xcode.
