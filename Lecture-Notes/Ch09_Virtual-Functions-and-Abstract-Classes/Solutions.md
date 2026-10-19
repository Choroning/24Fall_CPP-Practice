# Chapter 9 Solutions — Virtual Functions and Abstract Classes

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-19

---

<br>

## Table of Contents

- [Exercise 1: Student Grade Management with Virtual Functions](#exercise-1-student-grade-management-with-virtual-functions)
- [Exercise 2: Unit Conversion with Virtual Functions](#exercise-2-unit-conversion-with-virtual-functions)

---

<br>

## Exercise 1: Student Grade Management with Virtual Functions

**Problem:** Complete the `Student` class, which holds the elements shared by the three grade levels, and the derived classes `FirstGrade`, `SecondGrade`, and `ThirdGrade` according to the class design, and fill in the blanks of `main()` so that it prints the grades of four students through an array of base class pointers and then releases the dynamically allocated objects. `getAverage()` is declared as a pure virtual function, and `Student::displayGrade()` prints the name and the Korean and Informatics grades. The class design and the sample run are in [Exercise 1](Exercises.md#exercise-1-student-grade-management-with-virtual-functions).

### Solution

**Class design:**

- `Student` holds what the three grade levels share: the name `name` (`private`) and the Korean and Informatics grades `kor` and `pro` (`protected`). `kor` and `pro` are `protected` because `getAverage()` in the derived classes uses them directly to compute the average, and `name` is `private` because only `Student::displayGrade()` prints it.
- The number of subjects differs by grade level, so `Student` has nothing to implement for `getAverage()`, and it is declared as `virtual double getAverage() = 0;`. This makes `Student` an abstract class from which no object can be created directly, and each of the three derived classes becomes a concrete class by implementing its own `getAverage()`.
- `displayGrade()` is defined in `Student` as a virtual function that prints the shared part (name, Korean, Informatics), and the derived classes override it. A derived `displayGrade()` prints the grade label, calls `Student::displayGrade()` with the scope resolution operator to print the shared part (static binding, [Chapter 9, Section 2.10](Concepts_Lecture.md#210-calling-a-base-class-virtual-function-with-the-scope-resolution-operator)), and then prints the subjects of that grade level and the average.
- The average is computed by dividing by a floating-point literal, as in `(kor + pro + mat) / 3.0`, to avoid integer division. Dividing by `/ 3` would print 73 instead of 73.3333 as the average of 고길동.
- The destructor `~Student()` is declared `virtual` so that `delete` through a `Student*` correctly runs the derived class destructor first ([Chapter 9, Section 3](Concepts_Lecture.md#3-virtual-destructors)). The derived destructors and overriding functions carry `override` so that the compiler checks that the prototypes match.

**Blanks in `main()`:** Calling `p[i]->displayGrade();` inside the loop runs, through dynamic binding, the function of the grade-level object that `p[i]` actually points to, because `displayGrade()` is virtual. The objects are released by running `delete p[i];` four times in a loop.

**Code:** [`Lab_Q1.cpp`](../../Coursework/Lab09_Virtual-Functions-and-Abstract-Classes/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C09Q1.cpp
 * @brief   Q1. 가상함수를 사용한 학생 성적 관리
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-27
 */

#include <iostream>
#include <string>
using namespace std;

class Student {
    string name;
protected:
    int kor, pro;
public:
    Student(string name, int kor, int pro) {
        this->name = name;
        this->kor = kor;
        this->pro = pro;
    }
    virtual double getAverage()  = 0;
    virtual void displayGrade() {
        cout << "이름: " << name << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
    }
    virtual ~Student() {}
};

class FirstGrade : public Student {
    int mat;
public:
    FirstGrade(string name, int kor, int pro, int mat) : Student(name, kor, pro) {
        this->mat = mat;
    }
    double getAverage() override {
        return (kor + pro + mat) / 3.0;
    }
    void displayGrade() override {
        cout << "[1학년]" << endl;
        Student::displayGrade();
        cout << "수학: " << mat << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~FirstGrade() override {}
};

class SecondGrade : public Student {
    int sci, mor;
public:
    SecondGrade(string name, int kor, int pro, int sci, int mor) : Student(name, kor, pro) {
        this->sci = sci;
        this->mor = mor;
    }
    double getAverage() override {
        return (kor + pro + sci + mor) / 4.0;
    }
    void displayGrade() override {
        cout << "[2학년]" << endl;
        Student::displayGrade();
        cout << "과학: " << sci << endl;
        cout << "도덕: " << mor << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~SecondGrade() override {}
};

class ThirdGrade : public Student {
    int eng, his;
public:
    ThirdGrade(string name, int kor, int pro, int eng, int his) : Student(name, kor, pro) {
        this->eng = eng;
        this->his = his;
    }
    double getAverage() override {
        return (kor + pro + eng + his) / 4.0;
    }
    void displayGrade() override {
        cout << "[3학년]" << endl;
        Student::displayGrade();
        cout << "영어: " << eng << endl;
        cout << "한국사: " << his << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~ThirdGrade() override {}
};

int main() {
    Student* p[4];
    p[0] = new FirstGrade("고길동", 80, 70, 70);
    p[1] = new SecondGrade("둘리", 70, 60, 90, 70);
    p[2] = new ThirdGrade("또치", 90, 100, 90, 100);
    p[3] = new SecondGrade("도우너", 80, 90, 60, 100);
    
    for (int i = 0; i < 4; i++) {
        // 기본 클래스 포인터 p를 사용하여 displayGrade()를 올바르게 구현하시오.
        p[i]->displayGrade();
    }

    // delete 연산자를 사용해 동적할당 영역에 대한 반환 코드를 작성하시오.
    for (int i = 0; i < 4; i++) {
        delete p[i];
    }
    
    return 0;
}
```

**Result:** The code compiles without warnings with `g++ -std=c++17 -Wall`, and running it prints the following, which matches the sample run. The last line of the sample run, `Program ended with exit code: 0`, is not printed by the program; it is the message the development environment (Xcode) displays after the program exits.

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
```

---

<br>

## Exercise 2: Unit Conversion with Virtual Functions

**Problem:** Complete the `Convert` class, which holds the elements shared by the unit conversion classes, and the derived classes `LtoG`, `CtoF`, and `MtoKm` according to the class design, and fill in the blanks of `main()` so that it converts and prints three values through an array of base class pointers and then releases the objects. `compute()` and `display()` are declared as pure virtual functions. The class design, conversion formulas, and sample run are in [Exercise 2](Exercises.md#exercise-2-unit-conversion-with-virtual-functions).

### Solution

**Class design:**

- `Convert` holds the values before and after conversion, `before` and `after`, as `protected` members so that `compute()` and `display()` in the derived classes can read and write them directly. The constructor takes the value before conversion and initializes `before`.
- The conversion method and output format differ by unit, so `compute()` and `display()` are declared as pure virtual functions with `= 0`. `Convert` therefore becomes an abstract class, and each of the three derived classes becomes a concrete class by implementing both functions.
- The derived class constructors pass the value to the base class constructor with the member initializer list `: Convert(val)`. Following the class design, the derived classes have no member variables of their own.
- `CtoF::compute()` evaluates the formula $F = C \times 9 / 5 + 32$ as `(9.0/5.0) * before + 32`. Writing `9/5` would give 1 by integer division, so floating-point literals are used.
- The destructor `~Convert()` is declared `virtual` so that `delete` through a `Convert*` runs the derived class destructor first.

**Blanks in `main()`:** Inside the loop, `p[i]->compute();` performs the conversion and `p[i]->display();` prints the result. Both functions are virtual, so the functions of the actual object that `p[i]` points to (`MtoKm`, `LtoG`, or `CtoF`) run through dynamic binding. The objects are released by running `delete p[i];` three times in a loop.

**Check:** $23.2 \times 1.609344 = 37.3367808$, $10.3 / 3.7854 = 2.72098\ldots$, and $24.5 \times 9 / 5 + 32 = 76.1$. By default, `cout` prints up to 6 significant digits, so the first two values appear as 37.3368 and 2.72098.

**Code:** [`Lab_Q2.cpp`](../../Coursework/Lab09_Virtual-Functions-and-Abstract-Classes/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C09Q2.cpp
 * @brief   Q2. 가상함수를 사용한 단위 변환
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-27
 */

#include <iostream>
using namespace std;

class Convert {
protected:
    double before, after;
public:
    Convert(double before) {
        this->before = before;
    }
    virtual void compute() = 0;
    virtual void display() = 0;
    virtual ~Convert() {}
};

class LtoG : public Convert {
public:
    LtoG(double val) : Convert(val) {}
    void compute() override {
        after = before / 3.7854;
    }
    void display() override {
        cout << before << " 리터 = " << after << " 갤런" << endl;
    }
    ~LtoG() override {}
};

class CtoF : public Convert {
public:
    CtoF(double val) : Convert(val) {}
    void compute() override {
        after = (9.0/5.0) * before + 32;
    }
    void display() override {
        cout << "섭씨 " << before << "도 = 화씨 " << after << "도" << endl;
    }
    ~CtoF() override {}
};

class MtoKm : public Convert {
public:
    MtoKm(double val) : Convert(val) {}
    void compute() override {
        after = before * 1.609344;
    }
    void display() override {
        cout << before << " 마일 = " << after << " 킬로미터" << endl;
    }
    ~MtoKm() override {}
};

int main() {
    Convert* p[3] = { 
        new MtoKm(23.2), new LtoG(10.3), new CtoF(24.5)
    };
    
    for (int i = 0; i < 3; i++) {
        // 기본 클래스 포인터 p를 사용하여 단위 변환을 수행하시오.
        p[i]->compute();

        // 기본 클래스 포인터 p를 사용하여 변환 결과를 출력하시오.
        p[i]->display();
    }

    // delete 연산자를 사용해 동적할당 영역에 대한 반환 코드를 작성하시오.
    for (int i = 0; i < 3; i++) {
        delete p[i];
    }
    
    return 0;
}
```

**Result:** The code compiles without warnings with `g++ -std=c++17 -Wall`, and running it prints the following, which matches the sample run. As in Exercise 1, the last line of the sample run, `Program ended with exit code: 0`, is the message displayed by the development environment (Xcode).

```text
23.2 마일 = 37.3368 킬로미터
10.3 리터 = 2.72098 갤런
섭씨 24.5도 = 화씨 76.1도
```
