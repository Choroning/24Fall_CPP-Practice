# Chapter 8 Solutions — Inheritance

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 8

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

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

**Problem:** Without modifying the `Point` class, which has private coordinates `x` and `y` and a protected `move()` that changes the position, define a `ColorPoint` class that inherits from it so that the given `main()` runs. `ColorPoint` needs a constructor that initializes the color and position, plus `setPoint()`, `setColor()`, and `show()`. The full problem and sample run are in [Exercise 1](Exercises.md#exercise-1-color-and-position-of-a-point).

### Solution

`ColorPoint` adds a `string color` to store the color, and a single constructor with default arguments handles all three declarations in `main()`. Since `Point` has no default constructor, the constructor of `ColorPoint` must explicitly call `Point(x, y)` in its initializer list. Otherwise the compiler compiles a call to `Point()`, which is a compile error ([Chapter 8, Section 3.1](Concepts_Lecture.md#31-constructor-chain)).

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

The output is as follows.

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.
```

The last line of the sample run, `Program ended with exit code: 0`, and the blank line before it are printed by Xcode after the program ends and are not output of the program.

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

The output is as follows.

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

The last line of the sample run, `Program ended with exit code: 0`, is a message printed by Xcode.

---

<br>

## Exercise 3: The Closest Match to Inheritance

**Textbook Problem:** Chapter 8 theory problem 2

**Problem:** Choose the pair closest to the concept of inheritance in object orientation. The options are in [Exercise 3](Exercises.md#exercise-3-the-closest-match-to-inheritance).

### Solution

**Answer:** ①

**Explanation:** Inheritance is an is-a relationship in which a derived class is made by taking over the characteristics of a base class and adding new ones ([Chapter 8, Section 1.1](Concepts_Lecture.md#11-why-inheritance)). A smart TV has all the functions of a TV and adds functions such as internet access, so it is a kind of TV and closest to an inheritance relationship. A fabric sofa and a leather sofa are only different kinds of the same sofa, and neither inherits from the other; a CPU and a PC, and an engine and a transmission, are in a part-whole or part-part relationship (has-a).

---

<br>

## Exercise 4: Compile Errors Under Inheritance Access Specifiers

**Textbook Problem:** Chapter 8 theory problem 4

**Problem:** In classes that mix `public`, `private`, and `protected` inheritance, choose the access that causes a compile error. The full problem and code are in [Exercise 4](Exercises.md#exercise-4-compile-errors-under-inheritance-access-specifiers).

### Solution

**Answer:** ④

**Explanation:** `D` inherits `B` as `protected`, so `w` (originally a member of `A`) and `x`, which were `public` in `B`, become `protected` in `D`. So `d.w = 10;`, outside the class, cannot access it ([Chapter 8, Section 2.2](Concepts_Lecture.md#22-protected-inheritance)). `a.w` in ① is a `public` member, and `b.w` in ② stays `public` under `public` inheritance. `c.y` in ③ is a `public` member declared by `C` itself, so it can be accessed regardless of `C` inheriting `A` as `private`.

---

<br>

## Exercise 5: Access Under `protected` Inheritance

**Textbook Problem:** Chapter 8 theory problem 6

**Problem:** When `B` inherits `A` as `protected`, choose all the lines among the four in `main()` that cause a compile error. The full problem and code are in [Exercise 5](Exercises.md#exercise-5-access-under-protected-inheritance).

### Solution

**Answer:** ①, ②, ④

- **① `a.x = 3;`:** `x` is a `private` member of `A`, so it cannot be accessed outside the class.
- **② `b.y = 3;`:** `y` is a `private` member of `B`.
- **③ `a.setX(5);`:** `setX()` is a `public` member of `A`, so this is correct.
- **④ `b.setX(5);`:** `B` inherits `A` as `protected`, so `setX()` became `protected` in `B` and cannot be called outside the class. Inside the member function `setXY()` of `B`, `setX(x)` can be called.

---

<br>

## Exercise 6: Upcasting, Members, Access, and Downcasting

**Textbook Problem:** Chapter 8 theory problem 8

**Problem:** For `B` and `C`, which inherit `A`, and `D`, which inherits `B`, answer four questions about upcasting, members, access, and downcasting. The full problem and code are in [Exercise 6](Exercises.md#exercise-6-upcasting-members-access-and-downcasting).

### Solution

**Answer:**

- (1) ③. An upcast assigns a pointer to a derived class to a pointer to a base class. `B` and `C` both inherit `A` but are not related to each other by inheritance, so `bp = cp;` is neither an upcast nor a downcast and is a compile error.
- (2) `w`, `x`, `z`. Besides `z`, which it declares itself, `D` has `x` from `B` and `w`, which `B` inherited from `A`.
- (3) ③. `ap`, upcast with `ap = dp;`, is a pointer of type `A`, so it can access only `w`, the member declared in `A`, and not `x`.
- (4) Convert explicitly, as in `dp = (D*)ap;`.

A downcast, which assigns a base class pointer to a derived class pointer, needs an explicit type conversion ([Chapter 8, Section 1.4](Concepts_Lecture.md#14-inheritance-and-object-pointers)). In (4) the previous line put the address of a `D` object into `ap`, so the converted `dp` actually points to a `D` object and is safe. If `ap` pointed to an object that is not a `D` and was downcast to access a member of `D`, the result would be undefined behavior.

---

<br>

## Exercise 7: Inheriting a Base Class Without a Default Constructor

**Textbook Problem:** Chapter 8 theory problem 10

**Problem:** Find the error in the constructors of `B`, which inherits a base class `A` without a default constructor, and fix both constructors so that they produce the given output. The full problem and code are in [Exercise 7](Exercises.md#exercise-7-inheriting-a-base-class-without-a-default-constructor).

### Solution

**Answer:**

(1) Compile errors occur in both constructors, `B()` and `B(int x)`. If a constructor of a derived class does not specify a constructor of the base class, the compiler tries to call the default constructor `A()` of the base class, but `A` has only `A(int x)` and no default constructor.

(2) and (3) specify the base class constructor to call and its argument with a member initializer list.

```cpp
B() : A(20) { cout << "생성자 B" << endl; }
B(int x) : A(x + 20) { cout << "생성자 B " << x << endl; }
```

When an object of a derived class is created, the constructor of the base class runs first and then the constructor of the derived class ([Chapter 8, Section 3.1](Concepts_Lecture.md#31-constructor-chain)). `B b;` runs `A(20)` and then the body of `B()`, printing `생성자 A 20` and `생성자 B`. `B b(15);` calls the constructor of `A` with 35, the received 15 plus 20, so it prints `생성자 A 35` and `생성자 B 15`. Writing `A(20);` inside the constructor body does not initialize the base class part; it is a statement that creates one more unnamed temporary `A` object, so the error about the missing default constructor does not go away.

---

<br>

## Exercise 8: Statements About Multiple Inheritance

**Textbook Problem:** Chapter 8 theory problem 12

**Problem:** Choose the wrong statement about multiple inheritance. The options are in [Exercise 8](Exercises.md#exercise-8-statements-about-multiple-inheritance).

### Solution

**Answer:** ④

**Explanation:** The ambiguity caused by inheriting a member of the same name along two paths through multiple inheritance is found at compile time, not at run time; for the classes of Exercise 10, accessing `smartTV.screenSize` makes Clang report the error `non-static member 'screenSize' found in multiple base-class subobjects of type 'TV'`. Virtual inheritance is a way of inheriting, marked with the `virtual` keyword, that keeps only one copy of the members of a common base class, and it is a different concept from dynamic binding ([Chapter 8, Section 5](Concepts_Lecture.md#5-multiple-inheritance)). The other statements are correct. Java does not allow multiple inheritance of classes and achieves a similar effect with interfaces (①).

---

<br>

## Exercise 9: Declaring Classes with Multiple Inheritance

**Textbook Problem:** Chapter 8 theory problem 14

**Problem:** Declare `HiPen` and `OmniPen`, which inherit from two and three classes at the same time. The full problem and code are in [Exercise 9](Exercises.md#exercise-9-declaring-classes-with-multiple-inheritance).

### Solution

**Answer:**

```cpp
class HiPen : public Pen, public Eraser { ... };                // (1)
class OmniPen : public Pen, public Eraser, public Lock { ... };  // (2)
```

**Explanation:** Multiple inheritance lists the base classes after the colon, separated by commas, with an inheritance access specifier for each base class. If the access specifier is omitted, inheritance is `private` for a `class`, so `public` must be written for each base class whose `public` members are to be used from outside. The constructors are called in the listed order, that is, `Pen`, `Eraser`, `Lock`.

---

<br>

## Exercise 10: A Potential Problem in an Inheritance Relationship

**Textbook Problem:** Chapter 8 theory problem 16

**Problem:** Point out the potential problem of the inheritance relationship of `ColorTV` and `InternetTV`, which inherit `TV`, and fix it. The full problem and code are in [Exercise 10](Exercises.md#exercise-10-a-potential-problem-in-an-inheritance-relationship).

### Solution

**Answer:** If a class that inherits both `ColorTV` and `InternetTV` (for example `SmartTV`) is made later, the member `screenSize` of `TV` is inherited twice along two paths. To prevent this, change both classes to inherit `TV` virtually.

```cpp
class TV {
public: int screenSize;
};
class ColorTV : virtual public TV {
public: int color;
};
class InternetTV : virtual public TV {
public: string ipAddr;
};
class SmartTV : public ColorTV, public InternetTV { };
```

**Explanation:** Without virtual inheritance, a `SmartTV` object contains a separate `TV` part through `ColorTV` and another through `InternetTV`. Then there are two `screenSize` members, which wastes memory, and `smartTV.screenSize` cannot be resolved to one of them, so it is a compile error. If both intermediate classes inherit with `virtual public TV`, only one `TV` part is created inside `SmartTV`, and there is only one `screenSize` ([Chapter 8, Section 5](Concepts_Lecture.md#5-multiple-inheritance)). The problem arises from an inheritance structure shaped like a diamond, so it is commonly called the diamond problem.
