# HW6 Code Walkthrough — Default Parameters and Static Members

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 6

> **Learning Objectives:**
> 1. Read the class declarations, the definitions outside the classes, and `main()` of the four solutions line by line and explain the role of each part
> 2. Write the actual arguments into which a call of a constructor with default parameters is turned
> 3. Trace statement by statement how the value of a static member variable changes whenever a constructor or destructor is called

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: A Person Constructor with Default Parameters](#2-q1cpp-a-person-constructor-with-default-parameters)
- [3. Q2.cpp: A Person Class That Counts Its Members with numPerson](#3-q2cpp-a-person-class-that-counts-its-members-with-numperson)
- [4. Q3.cpp: A Random Class Made of Static Member Functions](#4-q3cpp-a-random-class-made-of-static-member-functions)
- [5. Q4.cpp: A Circle Class That Counts Its Objects with count](#5-q4cpp-a-circle-class-that-counts-its-objects-with-count)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the four solutions puts, in one file, the header includes, the class declaration, the definitions outside the class, and `main()`, in that order. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW6 Assignment Explanation](Assignment-Explanation.md).

| File | Class | Key members | Definitions outside the class |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Person` | A constructor with default parameters | The constructor |
| [`Q2.cpp`](Solution/Q2.cpp) | `Person` | `static int numPerson`, `static void nPerson()` | Three constructors, the destructor, `nPerson()`, `numPerson` |
| [`Q3.cpp`](Solution/Q3.cpp) | `Random` | Four static member functions | `nextInt()`, `nextAlphabet()`, `nextDouble()` |
| [`Q4.cpp`](Solution/Q4.cpp) | `Circle` | `static int count` | The destructor, `count` |

*Table 1. Layout of the HW6 solution files*

---

<br>

## 2. Q1.cpp: A Person Constructor with Default Parameters

```cpp
/**
 * @file    A6Q1.cpp
 * @brief   Q1. 디폴트 매개변수를 갖는 Person 생성자 (11주차 실습 2-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
public:
    Person(int id = 1, string name = "Grace", double weight = 20.5);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

Person::Person(int id, string name, double weight) {
    this->id = id;
    this->name = name;
    this->weight = weight;
}

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 11-14 | `int id; double weight; string name;` | With no access specifier, the three members are `private` |
| 16 | `Person(int id = 1, string name = "Grace", double weight = 20.5);` | The default values are written only in this declaration |
| 17 | `show()` | Prints `id weight name` in this order, separated by spaces |
| 20-24 | `Person::Person(int id, string name, double weight)` | Defined without default values; assigns to the members through `this->` |
| 27 | `Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);` | Creates three objects with different numbers of arguments in one declaration statement |
| 28-30 | Three `show()` calls | Print the information of the three objects one line each |

*Table 2. Line-by-line explanation of Q1.cpp*

| Creation as written | Actual call | Output |
|:-----|:-----|:-----|
| `grace` | `Person(1, "Grace", 20.5)` | `1 20.5 Grace` |
| `ashley(2, "Ashley")` | `Person(2, "Ashley", 20.5)` | `2 20.5 Ashley` |
| `helen(3, "Helen", 32.5)` | `Person(3, "Helen", 32.5)` | `3 32.5 Helen` |

*Table 3. Constructor calls of Q1 with the arguments filled in by default values*

---

<br>

## 3. Q2.cpp: A Person Class That Counts Its Members with numPerson

```cpp
/**
 * @file    A6Q2.cpp
 * @brief   Q2. static 멤버로 회원 수를 세는 Person 클래스 (11주차 실습 2-3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
    static int numPerson;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    ~Person();
    void show() { cout << id << ' ' << weight << ' ' << name << ' ' << numPerson << endl; }
    static void nPerson();
};
Person::Person() {
    id = 1;
    weight = 20.5;
    name = "Grace";
    numPerson++;
}
Person::Person(int id, string name) {
    this->id = id;
    weight = 20.5;
    this->name = name;
    numPerson++;
}
Person::Person(int id, string name, double weight) {
    this->id = id;
    this->weight = weight;
    this->name = name;
    numPerson++;
}
Person::~Person() {
    numPerson--;
}
void Person::nPerson() {
    cout << numPerson << endl;
}

int Person::numPerson = 0;

int main() {
    Person grace;
    grace.show();
    
    Person helen(13, "Helen", 32.5);
    helen.show();
    
    Person *p = new Person[2] { {43, "Tera"}, {98, "Hera"} };
    p[0].show();
    p[1].show();
    Person::nPerson();
    
    delete [] p;
    Person::nPerson();
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 15 | `static int numPerson;` | Declaration of the member count, of which the class has only one |
| 17-19 | Declarations of the three constructors | Each handles creation with zero, two, or three arguments |
| 20 | `~Person();` | Declaration of the destructor |
| 21 | `show()` | Prints `numPerson` after the three members |
| 22 | `static void nPerson();` | Declaration of the static member function that prints only the member count |
| 24-41 | Definitions of the three constructors | Each initializes the members and then runs `numPerson++` |
| 42-44 | `Person::~Person()` | Runs `numPerson--` |
| 45-47 | `Person::nPerson()` | Prints `numPerson`. The definition does not repeat `static` |
| 49 | `int Person::numPerson = 0;` | Definition and initialization of the static member variable |
| 52-56 | `grace`, `helen` | Created with the default constructor and the third constructor and printed |
| 58 | `new Person[2] { {43, "Tera"}, {98, "Hera"} }` | Creates two elements with the second constructor |
| 59-61 | Two `show()` calls and `nPerson()` | Print the elements of the array and the member count |
| 63-64 | `delete [] p;` and `nPerson()` | Destroy the two elements and print the member count |

*Table 4. Line-by-line explanation of Q2.cpp*

| Statement executed | Functions called | `numPerson` afterwards | Output |
|:-----|:-----|-----:|:-----|
| Definition at line 49 | None | 0 | |
| `Person grace;` | `Person()` | 1 | |
| `grace.show();` | | 1 | `1 20.5 Grace 1` |
| `Person helen(13, "Helen", 32.5);` | `Person(int, string, double)` | 2 | |
| `helen.show();` | | 2 | `13 32.5 Helen 2` |
| `new Person[2] {...}` | `Person(int, string)` twice | 4 | |
| `p[0].show();`, `p[1].show();` | | 4 | `43 20.5 Tera 4`, `98 20.5 Hera 4` |
| `Person::nPerson();` | | 4 | `4` |
| `delete [] p;` | `~Person()` twice (`p[1]`, `p[0]`) | 2 | |
| `Person::nPerson();` | | 2 | `2` |
| End of `main()` | `~Person()` twice (`helen`, `grace`) | 0 | |

*Table 5. Value of numPerson as Q2 runs*

---

<br>

## 4. Q3.cpp: A Random Class Made of Static Member Functions

```cpp
/**
 * @file    A6Q3.cpp
 * @brief   Q3. static 멤버로 이루어진 Random 클래스 (11주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Random {
public:
    // 항상 다른 랜덤수를 발생시키기
    // 위한 seed를 설정하는 함수
    static void seed() {
        srand((unsigned)time(0));
    }
    
    //min과 max 사이의 랜덤 정수 리턴
    static int nextInt(int min = 0, int max = 32767);
    static char nextAlphabet();

    // 0보다 크거나 같고 1보다 적은 실수 리턴
    static double nextDouble();
};

int Random::nextInt(int min, int max) {
    return min + rand() % (max - min + 1);
}
char Random::nextAlphabet() {
    if (rand() % 2)
        return 'a' + rand() % 26;
    else
        return 'A' + rand() % 26;
}
double Random::nextDouble() {
    return (double) rand() / RAND_MAX;
}

int main() {
    Random::seed();
    cout << "1에서 100까지 랜덤한 정수 10개를 출력합니다" << endl;
    for (int i = 0; i < 10; i++)
        cout << Random::nextInt(1, 100) << ' ';
    cout << endl;
    
    cout << "랜덤한 알파벳 10개를 출력합니다" << endl;
    for (int i = 0; i < 10; i++)
        cout << Random::nextAlphabet() << ' ';
    cout << endl;
    
    cout << "랜덤한 실수 10개를 출력합니다" << endl;
    for (int i = 0; i < 5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
    for (int i = 0; i < 5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-10 | `<iostream>`, `<cstdlib>`, `<ctime>` | Included to use `rand()`, `srand()`, `RAND_MAX`, and `time()` |
| 17-19 | `static void seed()` | The function given by the problem, which sets the current time as the seed |
| 22 | `static int nextInt(int min = 0, int max = 32767);` | A declaration with default values |
| 23, 26 | Declarations of `nextAlphabet()` and `nextDouble()` | Their definitions are outside the class |
| 29-31 | `Random::nextInt()` | Returns `min + rand() % (max - min + 1)` |
| 32-37 | `Random::nextAlphabet()` | Chooses lowercase or uppercase with `rand() % 2` and then chooses a letter with another `rand() % 26` |
| 38-40 | `Random::nextDouble()` | Returns `(double) rand() / RAND_MAX` |
| 43 | `Random::seed();` | Sets the seed once without an object |
| 44-47 | 10 integers | Calls `Random::nextInt(1, 100)` 10 times and prints the results |
| 49-52 | 10 letters | Prints the `char` return value of `Random::nextAlphabet()` as a character |
| 54-60 | 10 real numbers | Prints `Random::nextDouble()` five per line on two lines |

*Table 6. Line-by-line explanation of Q3.cpp*

In `nextAlphabet()`, if the first `rand()` returns 7 and the second returns 30, `7 % 2` is 1, so the lowercase branch is chosen and `'a' + 30 % 26 = 'a' + 4 = 'e'` is returned. If the first `rand()` is even, the uppercase branch is chosen and the same second value gives `'E'`.

---

<br>

## 5. Q4.cpp: A Circle Class That Counts Its Objects with count

```cpp
/**
 * @file    A6Q4.cpp
 * @brief   Q4. static 멤버로 객체 수를 세는 Circle 클래스 (11주차 실습 4)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
    static int count;
public:
    Circle() {
        radius = 0;
        count++;
    }
    Circle (int r) {
        radius = r;
        count++;
    }
    ~Circle();
    void show() {
        cout << "Radius: " << radius << ", Count: " << count << endl;
    }
};
Circle::~Circle() {
    count--;
}
int Circle::count = 0;

int main() {
    Circle a(5);
    a.show();
    
    Circle *p = new Circle[2];
    
    for (int i = 0; i < 2; i++)
        p[i].show();
    
    a.show();
    
    delete[] p;
    p = nullptr;
    a.show();
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 12-13 | `int radius; static int count;` | The radius and the object count, of which the class has only one |
| 15-22 | Two constructors | Set the radius to 0 or to the received value and run `count++` |
| 23 | `~Circle();` | Declaration of the destructor; its definition is at lines 28-30 |
| 24-26 | `show()` | Prints in the form `Radius: ..., Count: ...` |
| 28-30 | `Circle::~Circle()` | Runs `count--` |
| 31 | `int Circle::count = 0;` | Definition and initialization of the static member variable |
| 34-35 | `Circle a(5);` and output | Creates one object and prints it |
| 37 | `Circle *p = new Circle[2];` | Creates two elements with the default constructor |
| 39-42 | Printing the elements and `a.show()` | Prints the state in which three objects exist |
| 44-46 | `delete[] p;`, `p = nullptr;`, `a.show()` | Destroys the two elements, clears the pointer, and prints |

*Table 7. Line-by-line explanation of Q4.cpp*

| Statement executed | `count` afterwards | Output |
|:-----|-----:|:-----|
| `Circle a(5);` | 1 | |
| `a.show();` | 1 | `Radius: 5, Count: 1` |
| `new Circle[2]` | 3 | |
| `p[0].show();`, `p[1].show();` | 3 | Two lines of `Radius: 0, Count: 3` |
| `a.show();` | 3 | `Radius: 5, Count: 3` |
| `delete[] p;` | 1 | |
| `a.show();` | 1 | `Radius: 5, Count: 1` |
| End of `main()` | 0 | |

*Table 8. Value of count as Q4 runs*

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| A call with default values filled in | `ashley(2, "Ashley")` in Q1 is called as `Person(2, "Ashley", 20.5)`. |
| Declaration and definition of a static member | `static int numPerson;` inside the class is a declaration, and `int Person::numPerson = 0;` at line 49 is the definition. |
| Tracing the counts | `numPerson` in Q2 goes through 1, 2, 4, and 2 and becomes 0 when `main()` ends, and `count` in Q4 goes through 1, 3, and 1 and becomes 0. |
| Calling static member functions | `Random::seed()`, `Random::nextInt(1, 100)`, and `Person::nPerson()` are all called through the class name without an object. |
| Order of destruction | `delete []` destroys the array elements in the reverse order of construction, and the end of `main()` destroys the local objects in the reverse order of declaration. |

---

<br>

## Self-Check Questions

1. **Actual arguments:** If `Person bob(7);` is added to the `main()` of Q1, with what arguments is the constructor called, and what does `bob.show()` print?

   <details>
   <summary>Answer</summary>

   It is called as `Person(7, "Grace", 20.5)` and prints `7 20.5 Grace`. With only one argument, only `id` is filled, and `name` and `weight` get the default values.

   </details>

2. **`static` in the definition:** What happens if line 45 of Q2 is changed to `static void Person::nPerson() {`?

   <details>
   <summary>Answer</summary>

   A compile error occurs. `static` is written only in the declaration inside the class and cannot be repeated in the definition of a member function outside the class.

   </details>

3. **Tracing:** If line 37 in the `main()` of Q4 is changed to `Circle *p = new Circle[3];` and the loop at line 39 to `i < 3`, what is the `Count` on each line?

   <details>
   <summary>Answer</summary>

   The first line is `Count: 1`, the three elements of the array and the following `a.show()` are `Count: 4`, and the last line after `delete[] p;` is `Count: 1`.

   </details>

4. **Computing a character:** In `nextAlphabet()` of Q3, if the first `rand()` returns 4 and the second returns 27, which character is returned?

   <details>
   <summary>Answer</summary>

   `4 % 2` is 0, so the uppercase branch is chosen, and `27 % 26` is 1, so `'A' + 1`, which is `'B'`, is returned.

   </details>
