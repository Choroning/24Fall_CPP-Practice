# Chapter 9 Exercises — Virtual Functions and Abstract Classes

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 9

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Student Grade Management with Virtual Functions](#exercise-1-student-grade-management-with-virtual-functions)
- [Exercise 2: Unit Conversion with Virtual Functions](#exercise-2-unit-conversion-with-virtual-functions)
- [Textbook Theory Problems](#textbook-theory-problems)

---

<br>

## Exercise 1: Student Grade Management with Virtual Functions

**Problem:** Referring to the example code and the sample run, complete the `main()` function, and complete the `Student` class and its derived classes with the inheritance structure in mind so that the program runs correctly. The `Student` class holds the elements shared by the three derived classes `FirstGrade`, `SecondGrade`, and `ThirdGrade`, and the three derived classes are implemented by inheriting from `Student`.

![Figure 1. Class design of Student and the three grade-level derived classes](../../images/Ch09_Student-and-Grade-Classes.png)

*Figure 1. Class design of Student and the three grade-level derived classes*

In the class design above, elements marked in red use `private` access control, elements marked in green use `protected` access control, and elements marked in yellow are declared or overridden as virtual functions with the `virtual` keyword. Each class has the student's name and subject grades as member variables, and a constructor, a destructor, `getAverage()` for the grade average, and `displayGrade()` for printing the grades as member functions. The design is transcribed in the following table.

| Class | Member Variables | Member Functions |
|:-----|:-----|:-----|
| `Student` | `name` (이름, name, `private`), `kor` (국어, Korean, `protected`), `pro` (정보, Informatics, `protected`) | Constructor `Student()` and virtual functions `getAverage()`, `displayGrade()`, `~Student()` |
| `FirstGrade` | `mat` (수학, Math, `private`) | Constructor `FirstGrade()` and virtual functions `getAverage()`, `displayGrade()`, `~FirstGrade()` |
| `SecondGrade` | `sci` (과학, Science, `private`), `mor` (도덕, Ethics, `private`) | Constructor `SecondGrade()` and virtual functions `getAverage()`, `displayGrade()`, `~SecondGrade()` |
| `ThirdGrade` | `eng` (영어, English, `private`), `his` (한국사, Korean History, `private`) | Constructor `ThirdGrade()` and virtual functions `getAverage()`, `displayGrade()`, `~ThirdGrade()` |

*Table 1. Members and access control of the Student hierarchy*

**Program Requirements:**

1. The given example code must not be changed.
2. Declare the member function `getAverage()` of the `Student` class as a pure virtual function.
3. Write the `displayGrade()` function of the `Student` class so that it prints the name and the Korean and Informatics grades.
4. No member variables or member functions other than the elements shown in the class design may be declared or defined.

**Example Code:**

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    Student* p[4];
    p[0] = new FirstGrade("고길동", 80, 70, 70);
    p[1] = new SecondGrade("둘리", 70, 60, 90, 70);
    p[2] = new ThirdGrade("또치", 90, 100, 90, 100);
    p[3] = new SecondGrade("도우너", 80, 90, 60, 100);

    for (int i = 0; i < 4; i++) {
        // Implement displayGrade() correctly using the base class pointer p.
        [ blank ]
    }

    // Write the code that releases the dynamically allocated memory using the delete operator.
    [ blank ]

    return 0;
}
```

**Sample Run:**

```text
[1학년]
이름: 고길동
국어: 80
정보: 70
수학: 70
---------------
평균: 73.3333

[2학년]
이름: 둘리
국어: 70
정보: 60
과학: 90
도덕: 70
---------------
평균: 72.5

[3학년]
이름: 또치
국어: 90
정보: 100
영어: 90
한국사: 100
---------------
평균: 95

[2학년]
이름: 도우너
국어: 80
정보: 90
과학: 60
도덕: 100
---------------
평균: 82.5

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-student-grade-management-with-virtual-functions)**

---

<br>

## Exercise 2: Unit Conversion with Virtual Functions

**Problem:** Referring to the example code and the sample run, complete the `main()` function, and complete the `Convert` class and its derived classes with the inheritance structure in mind so that the program runs correctly. The `Convert` class holds the elements shared by the three derived classes that perform unit conversion, `LtoG` (liters to gallons), `CtoF` (Celsius to Fahrenheit), and `MtoKm` (miles to kilometers), and the three derived classes are implemented by inheriting from `Convert`.

![Figure 2. Class design of Convert and the three unit conversion derived classes](../../images/Ch09_Convert-Classes.png)

*Figure 2. Class design of Convert and the three unit conversion derived classes*

In the class design above, elements marked in green use `protected` access control, and elements marked in yellow are declared or overridden as virtual functions with the `virtual` keyword. The `Convert` class has the values before and after conversion as member variables, and each class has `compute()` for calculating the conversion and `display()` for printing the result as member functions. The design is transcribed in the following table.

| Class | Member Variables | Member Functions |
|:-----|:-----|:-----|
| `Convert` | `before` (value before conversion, `protected`), `after` (value after conversion, `protected`) | Constructor `Convert()` and virtual functions `compute()`, `display()`, `~Convert()` |
| `LtoG` | None | Constructor `LtoG()` and virtual functions `compute()`, `display()`, `~LtoG()` |
| `CtoF` | None | Constructor `CtoF()` and virtual functions `compute()`, `display()`, `~CtoF()` |
| `MtoKm` | None | Constructor `MtoKm()` and virtual functions `compute()`, `display()`, `~MtoKm()` |

*Table 2. Members and access control of the Convert hierarchy*

**Program Requirements:**

1. The given example code must not be changed.
2. Declare the member functions `compute()` and `display()` of the `Convert` class as pure virtual functions.
3. The conversion formula of each derived class is as follows.
   ```text
   LtoG class:   gallons (G) = liters (L) / 3.7854
   CtoF class:   Fahrenheit (F) = Celsius (C) * 9 / 5 + 32
   MtoKm class:  kilometers (Km) = miles (M) * 1.609344
   ```
4. Write the `compute()` function so that it converts the `before` value and stores it in `after`, and the `display()` function so that it prints the conversion result in the same format as the sample run (in Korean).
5. No member variables or member functions other than the elements shown in the class design may be declared or defined.

**Example Code:**

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    Convert* p[3] = { 
        new MtoKm(23.2), new LtoG(10.3), new CtoF(24.5)
    };

    for (int i = 0; i < 3; i++) {
        // Perform the unit conversion using the base class pointer p.
        [ blank ]

        // Print the conversion result using the base class pointer p.
        [ blank ]
    }

    // Write the code that releases the dynamically allocated memory using the delete operator.
    [ blank ]

    return 0;
}
```

**Sample Run:**

```text
23.2 마일 = 37.3368 킬로미터
10.3 리터 = 2.72098 갤런
섭씨 24.5도 = 화씨 76.1도

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-2-unit-conversion-with-virtual-functions)**

---

<br>

## Textbook Theory Problems

**Textbook:** Even-numbered theory problems of the Chapter 9 exercises

### Theory Problem 2

What is it called when a derived class redefines a function with the same type as a virtual function of the base class?

- ① overloading
- ② overriding
- ③ virtual
- ④ dynamic binding

**[Solution](Solutions.md#theory-problem-2)**

### Theory Problem 4

Which object-oriented feature of the C++ language do overloading, overriding, and operator overloading represent?

**[Solution](Solutions.md#theory-problem-4)**

### Theory Problem 6

Answer the questions about the following code.

```cpp
class A {
public:
    void func() { f(); }
    virtual void f() { cout << "A::f() called" << endl; }
};
class B : public A {
public:
    void f() { cout << "B::f() called" << endl; }
};
class C : public B {
public:
    void f() { cout << "C::f() called" << endl; }
};
```

(1) Choose all the virtual functions among the following. ① `f()` of `A` ② `f()` of `B` ③ `f()` of `C` ④ `func()` of `A`

(2)-(5) What is printed when each marked statement of the following code runs?

```cpp
C c;
c.f();        // (2)
A* pa;
B* pb;
pa = pb = &c;
pb->f();      // (3)
pa->f();      // (4)
pa->func();   // (5)
```

**[Solution](Solutions.md#theory-problem-6)**

### Theory Problem 8

For each item, what code fits in the blank of the function `g()`?

```cpp
void f() {
    cout << "f() called" << endl;
}
class A {
public:
    virtual void f() { cout << "A::f() called" << endl; }
};
class B : public A {
public:
    void g() {
        // 빈칸
    }
    void f() { cout << "B::f() called" << endl; }
};
```

(1) The function `g()` calls the external function `f()`.

(2) The function `g()` calls the member function `f()` of the class `A`.

(3) The function `g()` calls its own member function `f()`.

**[Solution](Solutions.md#theory-problem-8)**

### Theory Problem 10

The following is an example of writing a `Person` class and its derived class `Student`.

```cpp
class Person {
    int id;
public:
    Person(int id = 0) { this->id = id; }
    ~Person() { cout << "id=" << id << endl; }
};
class Student : public Person {
    char* name;
public:
    Student(int id, const char* name) : Person(id) {
        int len = strlen(name);
        this->name = new char[len + 1];
        strcpy(this->name, name);
    }
    ~Student() {
        cout << "name=" << name << endl;
        delete [] name;
    }
};
int main() {
    Person *p = new Student(10, "손연재");
    delete p;
}
```

(1) What is the output of this code? What problem do you see in the output?

(2) Fix the problem by changing the `Person` class or the `Student` class.

**[Solution](Solutions.md#theory-problem-10)**

### Theory Problem 12

Which statement about pure virtual functions is wrong?

- ① A class with a pure virtual function is always an abstract class.
- ② A pure virtual function is not meant to be executed but to tell derived classes which function they must implement.
- ③ An external function can also be declared as a pure virtual function.
- ④ When a pure virtual function is called, dynamic binding occurs.

**[Solution](Solutions.md#theory-problem-12)**

### Theory Problem 14

Answer the questions about the following code.

```cpp
class Shape {
public:
    void paint() { draw(); }
    virtual void draw() = 0;
};
class Circle : public Shape {
    int radius;
public:
    Circle(int radius = 1) { this->radius = radius; }
    double getArea() { return 3.14 * radius * radius; }
};
```

(1) Choose all that cause an error. ① `Shape shape;` ② `Shape *p;` ③ `Circle circle(10);` ④ `Circle *pCircle;`

(2) Change the `Shape` or `Circle` class so that running the following code prints `반지름=10인 원` ("a circle with radius=10").

```cpp
Circle *p = new Circle(10);
p->paint();
```

**[Solution](Solutions.md#theory-problem-14)**
