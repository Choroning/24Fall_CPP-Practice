# HW3 Code Walkthrough — Dynamic Memory

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **Learning Objectives:**
> 1. Read the class declarations, the member function definitions, and `main()` of the four solutions line by line and explain the role of each part
> 2. Trace how the objects on the heap and the pointer variables change when `new` and `delete` run
> 3. Trace element by element how a loop reads and accumulates the elements of a dynamic array

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: Dynamically Created Complex Objects](#2-q1cpp-dynamically-created-complex-objects)
- [3. Q2.cpp: The Member Class](#3-q2cpp-the-member-class)
- [4. Q3.cpp: The Average of a Dynamic Integer Array](#4-q3cpp-the-average-of-a-dynamic-integer-array)
- [5. Q4.cpp: A Dynamic Array of Circle Objects](#5-q4cpp-a-dynamic-array-of-circle-objects)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the four solutions puts, in one file, the header includes, a class definition (none in Q3), and `main()`, in that order. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW3 Assignment Explanation](Assignment-Explanation.md).

| File | Class | Dynamically allocated | Input |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Complex` | Two `Complex` objects (`new Complex`) | None |
| [`Q2.cpp`](Solution/Q2.cpp) | `Member` | Nothing | None |
| [`Q3.cpp`](Solution/Q3.cpp) | None | An array of five `int` values (`new int[5]`) | Five integers |
| [`Q4.cpp`](Solution/Q4.cpp) | `Circle` | An array of `size` `Circle` objects (`new Circle [size]`) | The number of circles and the radii |

*Table 1. Layout of the HW3 solution files*

---

<br>

## 2. Q1.cpp: Dynamically Created Complex Objects

```cpp
/**
 * @file    A3Q1.cpp
 * @brief   Q1. 동적으로 생성한 Complex 객체 (6주차 문제 6-1)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Complex {
    int real, image;
public:
    Complex() {
        real = image = 1;
    }
    Complex(int real, int image) {
        this->real = real;
        this->image = image;
    }
    void setReal(int real) {
        this->real = real;
    }
    void setImage(int image) {
        this->image = image;
    }
    void pCom() {
        cout << real << " + " << image << "i\n";
    }
};

int main()
{
    Complex* a = new Complex;
    Complex* b = new Complex;
    a->pCom();
    b->pCom();
    cout << "---------------\n";
    (*a).setReal(4);
    (*b).setImage(9);
    (*a).pCom();
    (*b).pCom();
    
    delete a;
    a = nullptr;
    delete b;
    b = nullptr;
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 11-12 | `class Complex { int real, image;` | With no access specifier, `real` and `image` are `private` members |
| 14-16 | `Complex()` | The default constructor, which initializes both members to 1 |
| 17-20 | `Complex(int real, int image)` | The parameters hide the members, so it assigns to the members through `this->` |
| 21-26 | `setReal()`, `setImage()` | Change one member to the received value in the same way |
| 27-29 | `pCom()` | Prints in the form `real + imagei` and ends the line with `\n` |
| 34-35 | `new Complex` twice | Creates one object each on the heap, calls the default constructor, and stores the addresses in `a` and `b` |
| 36-38 | `a->pCom();`, `b->pCom();`, separator | Prints the initial state of the two objects with the arrow operator |
| 39-40 | `(*a).setReal(4);`, `(*b).setImage(9);` | Calls a member function with `.` on the object obtained by dereferencing the pointer |
| 41-42 | `(*a).pCom();`, `(*b).pCom();` | Prints the changed values |
| 44-47 | `delete` and assigning `nullptr` | Returns the two objects and makes the pointers null pointers |

*Table 2. Line-by-line explanation of Q1.cpp*

| Statement executed | `*a` | `*b` | Output |
|:-----|:-----|:-----|:-----|
| `new Complex` at lines 34-35 | (1, 1) | (1, 1) | |
| Lines 36-38 | (1, 1) | (1, 1) | `1 + 1i`, `1 + 1i`, separator |
| `(*a).setReal(4);` | (4, 1) | (1, 1) | |
| `(*b).setImage(9);` | (4, 1) | (1, 9) | |
| Lines 41-42 | (4, 1) | (1, 9) | `4 + 1i`, `1 + 9i` |
| Lines 44-47 | Returned | Returned | (`a` and `b` are `nullptr`) |

*Table 3. State of the two dynamic objects as Q1 runs (real part, imaginary part)*

---

<br>

## 3. Q2.cpp: The Member Class

```cpp
/**
 * @file    A3Q2.cpp
 * @brief   Q2. 회원 정보를 관리하는 Member 클래스 (6주차 문제 6-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Member {
    string name;
    string id;
    int age;
public:
    Member() {
        name = "noname";
        id = "none";
        age = 0;
    }
    Member(string name, string id, int age) {
        this->name = name;
        this->id = id;
        this->age = age;
    }
    void setName(string name) {
        this->name = name;
    }
    void setId(string id) {
        this->id = id;
    }
    void setAge(int age) {
        this->age = age;
    }
    
    string getName() {
        return name;
    }
    string getId() {
        return id;
    }
    int getAge() {
        return age;
    }
    void printM() {
        cout << name << ", " << id << ", " << age << endl;
    }
};

int main()
{
    Member Kim;
    Member Lee("Lee Hi", "2016001", 22);
    Kim.printM();
    Lee.printM();
    Kim.setName("Kim Haha");
    Kim.setId("2016020");
    Kim.setAge(29);
    Kim.printM();
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 11-14 | `class Member { string name; string id; int age;` | Keeps the name, the ID, and the age as `private` members |
| 16-20 | `Member()` | The default constructor, which assigns `"noname"`, `"none"`, and 0 in the body |
| 21-25 | `Member(string name, string id, int age)` | The three-parameter constructor, which assigns to the members through `this->` |
| 26-34 | `setName()`, `setId()`, `setAge()` | Change one member each to the received value |
| 36-44 | `getName()`, `getId()`, `getAge()` | Return the value of a member. They are not used in this `main()` |
| 45-47 | `printM()` | Prints one line in the form `name, id, age` |
| 52-53 | `Member Kim;`, `Member Lee(...)` | Creates `Kim` with the default constructor and `Lee` with the three-parameter constructor |
| 54-55 | Two `printM()` calls | Print the initial state of the two objects |
| 56-59 | Three set calls and `printM()` | Change the three members of `Kim` and print it again |

*Table 4. Line-by-line explanation of Q2.cpp*

| Statement executed | `Kim` | `Lee` | Output |
|:-----|:-----|:-----|:-----|
| Lines 52-53 | (`"noname"`, `"none"`, 0) | (`"Lee Hi"`, `"2016001"`, 22) | |
| Lines 54-55 | Same | Same | `noname, none, 0`, `Lee Hi, 2016001, 22` |
| Lines 56-58 | (`"Kim Haha"`, `"2016020"`, 29) | Same | |
| Line 59 | Same | Same | `Kim Haha, 2016020, 29` |

*Table 5. State of the two objects as Q2 runs (`name`, `id`, `age`)*

---

<br>

## 4. Q3.cpp: The Average of a Dynamic Integer Array

```cpp
/**
 * @file    A3Q3.cpp
 * @brief   Q3. 동적 정수 배열로 구하는 평균 (6주차 문제 6-4)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

int main()
{
    int ave = 0;
    int* p = new int[5];
    
    cout << "정수 5개 입력 >> ";
    for (int i = 0; i < 5; i++){
        cin >> p[i];
        ave += p[i];
    }
    
    cout << "평균 " << ave / 5.0 << endl;
    
    delete [] p;
    p = nullptr;
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 13 | `int ave = 0;` | The variable that accumulates the sum of the input values |
| 14 | `int* p = new int[5];` | Allocates an array of five `int` values on the heap and stores the address of the first element in `p` |
| 16 | Printing the prompt | The line is not ended, so the input follows the prompt |
| 17-20 | `for` loop | Stores the input value in the `i`-th element with `cin >> p[i];` and adds it to `ave` |
| 22 | `ave / 5.0` | `ave` is converted to `double`, and the result of floating-point division is printed |
| 24-25 | `delete [] p;`, `p = nullptr;` | Returns the array and makes the pointer a null pointer |

*Table 6. Line-by-line explanation of Q3.cpp*

| `i` | Value entered `p[i]` | `ave` after the iteration |
|-----:|-----:|-----:|
| 0 | 1 | 1 |
| 1 | 2 | 3 |
| 2 | 4 | 7 |
| 3 | 5 | 12 |
| 4 | 10 | 22 |

*Table 7. Loop trace of Q3 for the input `1 2 4 5 10`*

After the loop, line 22 computes `22 / 5.0 = 4.4` and prints `평균 4.4` (average 4.4).

---

<br>

## 5. Q4.cpp: A Dynamic Array of Circle Objects

```cpp
/**
 * @file    A3Q4.cpp
 * @brief   Q4. Circle 객체의 동적 배열 (6주차 문제 6-5)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    void setRadius(int radius);
    double getArea();
};

void Circle::setRadius(int radius) {
    this->radius = radius;
}

double Circle::getArea(){
    return 3.14 * radius * radius;
}

int main(void)
{
    Circle* p;
    int size, count = 0;
    cout << "원의 개수 >> ";
    cin >> size;
    p = new Circle [size];
    
    for (int i = 0; i < size; i++) {
        int temp;
        cout << "원 " << i + 1 << "의 반지름 >> ";
        cin >> temp;
        p[i].setRadius(temp);
        
        if (p[i].getArea() >= 100)
            count++;
    }
    cout << "면적이 100보다 큰 원은 " << count << "개 입니다.\n";
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 11-16 | `class Circle { ... };` | The class given by the problem, with the `private` member `radius` and the prototypes of two member functions |
| 18-20 | `Circle::setRadius(int radius)` | Assigns to the member with `this->radius = radius;` |
| 22-24 | `Circle::getArea()` | Returns `3.14 * radius * radius` |
| 28-29 | `Circle* p; int size, count = 0;` | Declares the pointer to the array, the number of circles, and the number of circles whose area is 100 or more |
| 30-31 | Reading the count | Reads the number of circles into `size` |
| 32 | `p = new Circle [size];` | Creates as many `Circle` objects on the heap as decided at run time, calling the default constructor for each element |
| 34-42 | `for` loop | For each circle, reads the radius into `temp`, sets it, and increments `count` if the area is 100 or more |
| 43 | Printing the result | Prints the sentence containing `count`. No `delete [] p;` follows, so the array is not returned |

*Table 8. Line-by-line explanation of Q4.cpp*

| `i` | Radius | Area `getArea()` | 100 or more | `count` after the iteration |
|-----:|-----:|-----:|:-----|-----:|
| 0 | 3 | 28.26 | No | 0 |
| 1 | 7 | 153.86 | Yes | 1 |
| 2 | 2 | 12.56 | No | 1 |
| 3 | 9 | 254.34 | Yes | 2 |
| 4 | 1 | 3.14 | No | 2 |

*Table 9. Loop trace of Q4 for the input 5, 3, 7, 2, 9, 1*

After the loop, line 43 prints `면적이 100보다 큰 원은 2개 입니다.` (there are 2 circles whose area is greater than 100). The array `p` is not returned until the program ends, and this block is the one leak (32 bytes) that `leaks --atExit` reports.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Layout of the files | Q1, Q2, and Q4 consist of a class definition and `main()`, and Q3 consists of `main()` only. |
| Lifetime of dynamic objects | An object created with `new Complex` remains until `delete` runs, and the pointer is set to `nullptr` after `delete`. |
| Calling members through a pointer | `a->pCom()` and `(*a).pCom()` are the same call. |
| Accessing array elements | `p[i]` is `*(p + i)`, so a dynamic array is read and written with the same syntax as an ordinary array. |
| Accumulation and average | `ave` in Q3 grows to 1, 3, 7, 12, and 22, and the average is computed as `22 / 5.0` when it is printed. |
| An array not returned | `new Circle [size]` in Q4 has no matching `delete []` and remains as a leak. |

---

<br>

## Self-Check Questions

1. **Value of the pointer:** Right after `delete a;` at line 44 of Q1 runs, before line 45 runs, what is the value of `a`?

   <details>
   <summary>Answer</summary>

   `delete` returns only the object the pointer points to and does not change the value of the pointer variable, so `a` still holds the address of the returned object. `a` in this state is a dangling pointer, and line 45 assigns `nullptr` to it.

   </details>

2. **Functions that are not used:** If the three get functions of Q2 are removed, does the output of the program change?

   <details>
   <summary>Answer</summary>

   It does not. Neither `main()` nor `printM()` calls the get functions. However, the problem requires them, so they should stay in the solution.

   </details>

3. **Tracing the loop:** If `10 20 30 40 51` is entered into Q3, what are `ave` after the last iteration and the output?

   <details>
   <summary>Answer</summary>

   `ave` grows to 10, 30, 60, 100, and 151, ending at 151, and the program prints `평균 30.2`, which is `151 / 5.0`.

   </details>

4. **Initial state of the elements:** Right after line 32 of Q4 runs, before the loop at line 34 starts, what value does a call to `p[0].getArea()` produce?

   <details>
   <summary>Answer</summary>

   `Circle` has no constructor, and the default constructor provided by the compiler does not initialize `radius`, so `radius` has an indeterminate value at this point. Reading it is undefined behavior, and the result cannot be predicted. This code sets the value with `setRadius()` inside the loop before calling `getArea()`, so the problem does not arise.

   </details>
