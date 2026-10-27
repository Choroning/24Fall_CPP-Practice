# Chapter 8 Exercises — Inheritance

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 8

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

Exercises 1 and 2 are lab problems, and Exercises 3-10 are the even-numbered theory problems (2-16) of Chapter 8 of the textbook.

---

<br>

## Table of Contents

- [Exercise 1: Color and Position of a Point](#exercise-1-color-and-position-of-a-point)
- [Exercise 2: Student Grade Management](#exercise-2-student-grade-management)
- [Exercise 3: The Closest Match to Inheritance](#exercise-3-the-closest-match-to-inheritance)
- [Exercise 4: Compile Errors Under Inheritance Access Specifiers](#exercise-4-compile-errors-under-inheritance-access-specifiers)
- [Exercise 5: Access Under `protected` Inheritance](#exercise-5-access-under-protected-inheritance)
- [Exercise 6: Upcasting, Members, Access, and Downcasting](#exercise-6-upcasting-members-access-and-downcasting)
- [Exercise 7: Inheriting a Base Class Without a Default Constructor](#exercise-7-inheriting-a-base-class-without-a-default-constructor)
- [Exercise 8: Statements About Multiple Inheritance](#exercise-8-statements-about-multiple-inheritance)
- [Exercise 9: Declaring Classes with Multiple Inheritance](#exercise-9-declaring-classes-with-multiple-inheritance)
- [Exercise 10: A Potential Problem in an Inheritance Relationship](#exercise-10-a-potential-problem-in-an-inheritance-relationship)

---

<br>

## Exercise 1: Color and Position of a Point

**Problem:** Referring to the example code and the sample run, define the `ColorPoint` class so that `main()` runs correctly. `ColorPoint` is a derived class of `Point`. The problem is based on programming exercises 3 and 4 of Chapter 8 of the textbook.

**Program Requirements**

- Fill in the [ 빈칸 ] (blank) part; the given example code must not be changed.

**Example Code**

```cpp
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
    // definition of the ColorPoint class
    [ 빈칸 ]
};

int main() {
    ColorPoint cp1(5, 5, "RED");  // color "RED", position (5, 5)
    cp1.show();
    cp1.setPoint(10, 20);         // sets position to (10, 20)
    cp1.setColor("BLUE");         // sets color to "BLUE"
    cp1.show();

    ColorPoint cp2(5, 5);         // color "BLACK", position (5, 5)
    cp2.show();
    cp2.setPoint(3, 7);           // sets position to (3, 7)
    cp2.setColor("GREEN");        // sets color to "GREEN"
    cp2.show();

    ColorPoint zeroPoint;         // color "BLACK", position (0, 0)
    zeroPoint.show();

    return 0;
}
```

**Sample Run**

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-color-and-position-of-a-point)**

---

<br>

## Exercise 2: Student Grade Management

**Problem:** Referring to the example code and the sample run, and considering the inheritance structure, complete the `Student` class and its derived classes so that `main()` runs correctly. The `Student` class holds the elements shared by the three derived classes `FirstGrade`, `SecondGrade`, and `ThirdGrade`, and the three derived classes are implemented by inheriting from `Student`.

![Figure 1. Member variables and member functions of the FirstGrade, SecondGrade, and ThirdGrade classes](../../images/Ch08_Grade-Classes.png)

*Figure 1. Member variables and member functions of the FirstGrade, SecondGrade, and ThirdGrade classes*

All three classes have the name (`name`) and the Korean (`kor`) and informatics (`pro`) scores as member variables; `FirstGrade` also has a math score (`mat`), `SecondGrade` science (`sci`) and ethics (`mor`) scores, and `ThirdGrade` English (`eng`) and Korean history (`his`) scores. The member functions of all three classes are a constructor, `getName()` returning the name, `getAverage()` returning the average score, and `display()` printing the scores.

In Figure 1, the element highlighted in red (the name) is `private`, and the elements highlighted in green (the subject scores) are `protected`.

**Program Requirements**

- The given example code must not be changed.

**Example Code**

```cpp
#include <iostream>
using namespace std;

int main() {
    // initializes name and Korean, informatics, and math scores
    FirstGrade f("홍길동", 90, 81, 88);

    // initializes name and Korean, informatics, science, and ethics scores
    SecondGrade s("장영실", 73, 91, 99, 68);
    
    // initializes name and Korean, informatics, English, and Korean history scores
    ThirdGrade t("정도전", 81, 53, 80, 96);
    
    f.display();
    s.display();
    t.display();
    
    return 0;
}
```

**Sample Run**

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

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-2-student-grade-management)**

---

<br>

## Exercise 3: The Closest Match to Inheritance

**Textbook Problem:** Chapter 8 theory problem 2

**Problem:** Which pair is closest to the concept of inheritance in object orientation?

- ① A TV and a smart TV
- ② A fabric sofa and a leather sofa
- ③ A CPU and a PC
- ④ A car engine and a transmission

**[Solution](Solutions.md#exercise-3-the-closest-match-to-inheritance)**

---

<br>

## Exercise 4: Compile Errors Under Inheritance Access Specifiers

**Textbook Problem:** Chapter 8 theory problem 4

**Problem:** For the following code, which statement causes a compile error?

```cpp
class A {
public: int w;
};
class B : public A {
public: int x;
};
class C : private A {
public: int y;
};
class D : protected B {
public: int z;
};
```

- ① `A a; a.w = 10;`
- ② `B b; b.w = 10;`
- ③ `C c; c.y = 10;`
- ④ `D d; d.w = 10;`

**[Solution](Solutions.md#exercise-4-compile-errors-under-inheritance-access-specifiers)**

---

<br>

## Exercise 5: Access Under `protected` Inheritance

**Textbook Problem:** Chapter 8 theory problem 6

**Problem:** Choose all the places in the following code that cause a compile error.

```cpp
class A {
    int x;
public:
    void setX(int x) { this->x = x; }
};
class B : protected A {
    int y;
public:
    void setXY(int x, int y) { setX(x); this->y = y; }
};
int main() {
    A a;
    B b;
    a.x = 3;      // ①
    b.y = 3;      // ②
    a.setX(5);    // ③
    b.setX(5);    // ④
}
```

**[Solution](Solutions.md#exercise-5-access-under-protected-inheritance)**

---

<br>

## Exercise 6: Upcasting, Members, Access, and Downcasting

**Textbook Problem:** Chapter 8 theory problem 8

**Problem:** Given the following classes and variables, answer the questions.

```cpp
class A {
public: int w;
};
class B : public A {
public: int x;
};
class C : public A {
public: int y;
};
class D : public B {
public: int z;
};
A a; B b; C c; D d;
A *ap = &a;
B *bp = &b;
C *cp = &c;
D *dp = &d;
```

(1) Which is not an upcast? ① `ap = bp;` ② `ap = cp;` ③ `bp = cp;` ④ `bp = dp;`

(2) List all the members of the object `d`.

(3) Which access to a member of the object `d` causes a compile error? ① `d.x = 10;` ② `dp->x = 10;` ③ `ap = dp; ap->x = 10;` ④ `bp = dp; bp->x = 10;`

(4) Writing the following two lines causes a compile error in the second line. Fix it.

```cpp
ap = dp;
dp = ap;
```

**[Solution](Solutions.md#exercise-6-upcasting-members-access-and-downcasting)**

---

<br>

## Exercise 7: Inheriting a Base Class Without a Default Constructor

**Textbook Problem:** Chapter 8 theory problem 10

**Problem:** Answer the questions about the following code.

```cpp
class A {
public:
    A(int x) { cout << "생성자 A " << x << endl; }
};
class B : public A {
public:
    B() { cout << "생성자 B" << endl; }
    B(int x) { cout << "생성자 B " << x << endl; }
};
```

(1) Where in the code above does a compile error occur, and what is it?

(2) Change the constructor `B()` so that running `B b;` prints the two lines `생성자 A 20` and `생성자 B` ("constructor A 20", "constructor B").

(3) Change the constructor `B(int x)` so that running `B b(15);` prints the two lines `생성자 A 35` and `생성자 B 15`.

**[Solution](Solutions.md#exercise-7-inheriting-a-base-class-without-a-default-constructor)**

---

<br>

## Exercise 8: Statements About Multiple Inheritance

**Textbook Problem:** Chapter 8 theory problem 12

**Problem:** Which statement about multiple inheritance is wrong?

- ① Java has no multiple inheritance of classes.
- ② It increases programming productivity by reusing existing classes.
- ③ Multiple inheritance can cause ambiguity.
- ④ Ambiguity caused by multiple inheritance is found at run time, so it is solved by virtual inheritance through dynamic binding.

**[Solution](Solutions.md#exercise-8-statements-about-multiple-inheritance)**

---

<br>

## Exercise 9: Declaring Classes with Multiple Inheritance

**Textbook Problem:** Chapter 8 theory problem 14

**Problem:** Given the following classes, answer the questions.

```cpp
class Pen { ... };
class Eraser { ... };
class Lock { ... };
```

(1) Declare a class `HiPen` that inherits from both `Pen` and `Eraser`.

(2) Declare a class `OmniPen` that inherits from `Pen`, `Eraser`, and `Lock` at the same time.

**[Solution](Solutions.md#exercise-9-declaring-classes-with-multiple-inheritance)**

---

<br>

## Exercise 10: A Potential Problem in an Inheritance Relationship

**Textbook Problem:** Chapter 8 theory problem 16

**Problem:** Point out the potential problem of the inheritance relationship of the following classes and fix it.

```cpp
class TV {
public: int screenSize;
};
class ColorTV : public TV {
public: int color;
};
class InternetTV : public TV {
public: string ipAddr;
};
```

**[Solution](Solutions.md#exercise-10-a-potential-problem-in-an-inheritance-relationship)**
