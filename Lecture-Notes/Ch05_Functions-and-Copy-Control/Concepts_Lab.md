# Chapter 5 Lab — References, Reference Returns, and Copy Constructors

[Repository Guide](../../README.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **Last Updated:** 2026-11-18

> **Learning Objectives:**
> 1. Explain with the copy constructor why, when an object is passed by value, the constructor message is printed once but the destructor message twice
> 2. Confirm the difference between pointers and references by comparing addresses, and explain what a reference initialized from a reference refers to
> 3. Explain why a call to a function that returns a reference can appear on the left side of an assignment
> 4. Write a function that returns its result through a reference parameter and its success through the return value
> 5. Write a copy constructor and confirm that the original and the copy are independent

---

## Table of Contents

- [1. Overview](#1-overview)
- [2. Passing Objects and the Copy Constructor](#2-passing-objects-and-the-copy-constructor)
  - [2.1 Call by Value](#21-call-by-value)
  - [2.2 Call by Address](#22-call-by-address)
- [3. Reference Variables and Reference Returns](#3-reference-variables-and-reference-returns)
  - [3.1 Comparing Pointers and References](#31-comparing-pointers-and-references)
  - [3.2 Reference Returns](#32-reference-returns)
  - [3.3 Returning a Reference to an Array Element](#33-returning-a-reference-to-an-array-element)
- [4. Returning Results through Reference Parameters](#4-returning-results-through-reference-parameters)
- [5. Writing a Copy Constructor](#5-writing-a-copy-constructor)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overview

> **Note:** The theory of call by value, address, and reference, reference variables and reference returns, and copy constructors with shallow and deep copies is covered in the [Chapter 5 lecture notes](Concepts_Lecture.md). This lab runs each concept as short code and confirms it through the output.

| Topic | Location |
|:-----|:-----|
| Execution of constructors and destructors when objects are passed by value and by address | Section 2 |
| Reference variables, reference returns, analysis of returning a reference to an array element | Section 3 |
| A function that returns an average through a reference parameter | Section 4 |
| Writing and running a copy constructor | Section 5 |
| Shallow and deep copies of a `Person` class with a pointer member | [Sections 4.3 and 4.6 of the Chapter 5 lecture notes](Concepts_Lecture.md#43-shallow-copy-problem) |
| Fixing the `increaseBy()` function that adds circle radii | [Chapter 5 Exercise 1](Exercises.md#exercise-1-various-ways-to-call-a-function) |
| An integer stack `MyIntStack` that pops values through a reference parameter | [HW4 Problem 1](../../Coursework/HW04_References/Original/Assignment.md#problem-1-week-9-lecture-slides-lab-problem-2-implementing-and-analysing-myintstack) |
| An `Accumulator` that accumulates values, a `Book` with a deep copy constructor | [HW5 Problems 1 and 2](../../Coursework/HW05_Reference-Return-and-Copy-Constructors/Original/Assignment.md) |

*Table 1. Topics of the lab and their location*

All code is compiled and run with the C++17 standard (for example, `g++ -std=c++17 -Wall`).

---

<br>

## 2. Passing Objects and the Copy Constructor

### 2.1 Call by Value

The following `Circle` prints a message every time its constructor or destructor runs. `increase()` tries to increase the radius of the circle it receives by 1.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle();
    Circle(int r);
    ~Circle();
    double getArea() { return 3.14 * radius * radius; }
    int getRadius() { return radius; }
    void setRadius(int radius) { this->radius = radius; }
};

Circle::Circle() {
    radius = 1;
    cout << "생성자 실행 radius = " << radius << endl;
}

Circle::Circle(int radius) {
    this->radius = radius;
    cout << "생성자 실행 radius = " << radius << endl;
}

Circle::~Circle() {
    cout << "소멸자 실행 radius = " << radius << endl;
}

void increase(Circle c) {               // Call by value: c is a copy of waffle
    int r = c.getRadius();
    c.setRadius(r + 1);
}

int main() {
    Circle waffle(30);
    increase(waffle);
    cout << waffle.getRadius() << endl;
}
```

```text
생성자 실행 radius = 30
소멸자 실행 radius = 31
30
소멸자 실행 radius = 30
```

- Calling `increase(waffle)` initializes the parameter `c` with the contents of `waffle`, as in `Circle c = waffle;`. `c` is an object separate from `waffle`, so changing the radius of `c` to 31 leaves `waffle` at 30. (The messages read "constructor runs" and "destructor runs.")
- The constructor message is printed only once, when `waffle` is created, but the destructor message is printed twice, for `c` (31) and `waffle` (30). A constructor always runs when an object is created; what ran when `c` was created was not one of the two message-printing constructors but the **copy constructor** that the compiler inserted automatically. The copy constructor only copies the members one by one and prints no message.
- `c` is a local object of `increase()`, so it is destroyed first, when the function ends, and `waffle` is destroyed when `main()` ends.

### 2.2 Call by Address

If `increase()` is changed to receive the object's address, no copy is made. The class, constructors, and destructor are the same as in Section 2.1.

```cpp
void increase(Circle* p) {              // Call by address: p points to waffle
    int r = p->getRadius();
    p->setRadius(r + 1);
}

int main() {
    Circle waffle(30);
    increase(&waffle);
    cout << waffle.getRadius() << endl;
}
```

```text
생성자 실행 radius = 30
31
소멸자 실행 radius = 31
```

The parameter `p` is only a pointer variable initialized with the address of `waffle`, as in `Circle* p = &waffle;`, and not a new object, so no extra constructor or destructor runs. `p->setRadius()` changes `waffle` itself, so 31 is printed. A reference parameter `void increase(Circle& c)` gives the same effect without pointer syntax (Section 2.2 of the Chapter 5 lecture notes).

---

<br>

## 3. Reference Variables and Reference Returns

### 3.1 Comparing Pointers and References

> **Definition:** A **reference** is another name (an alias) given to an existing variable or object; it allocates no new memory and shares the target it was initialized with.

Just as a student can be called by student ID instead of name, a reference lets the same target be called by a different name.

```cpp
#include <iostream>
using namespace std;

int main() {
    int test = 3;
    int* p = &test;                     // Pointer: a separate variable storing the address of test
    int& r = test;                      // Reference: another name for test

    cout << test << ' ' << *p << ' ' << r << endl;
    cout << boolalpha;
    cout << (&test == p) << ' ' << (&r == &test) << ' ' << ((void*)&p == (void*)&test) << endl;
    r = r + 2;                          // test changes
    cout << test << ' ' << r << endl;

    int data = 3;
    int& ref = data;
    int& alias = ref;                   // A reference initialized from a reference also refers to data
    alias = 5;
    cout << data << ' ' << ref << ' ' << alias << endl;
    cout << (&alias == &data) << endl;
}
```

```text
3 3 3
true true false
5 5
5 5 5
true
```

- `p` holds the address of `test`, so `&test == p`. The address of the reference `r`, `&r`, is also the address of `test`, because a reference has no space of its own and shares the space of `test`.
- The pointer `p`, on the other hand, is itself a variable that holds an address, so its own address `&p` differs from the address of `test`.
- `int& alias = ref;` does not copy the value 3 of `ref`; it creates one more reference to what `ref` refers to, namely `data`. So `alias = 5;` changes `data` to 5.

### 3.2 Reference Returns

```cpp
#include <iostream>
using namespace std;

int& refReturn(int& ref) {
    return ref;                         // Return a reference to the variable the parameter refers to
}

int main() {
    int data = 3;
    cout << "1. data: " << data << endl;
    refReturn(data) = 5;                // Assign 5 to the returned reference, that is, to data
    cout << "2. data: " << data << endl;
    int tmp = refReturn(data);          // Copy the value the returned reference refers to
    cout << "3. tmp: " << tmp << endl;
}
```

```text
1. data: 3
2. data: 5
3. tmp: 5
```

The call `refReturn(data)` to a function returning a reference denotes the variable `data` itself, not a value, so it can appear on the left side of an assignment. In `int tmp = refReturn(data);`, on the other hand, `tmp` is an ordinary variable, not a reference, so the value 5 that the returned reference refers to is copied into `tmp`.

### 3.3 Returning a Reference to an Array Element

**Analysis:** Explain why `refReturn(a, 2) = 5;` in the following code compiles and changes the array.

```cpp
#include <iostream>
#define SIZE 3
using namespace std;

int& refReturn(int* p, int index);

int main() {
    int i, a[SIZE] = { 1, 2, 3 };
    for (i = 0; i < SIZE; i++)
        cout << a[i] << ", ";
    cout << endl;

    refReturn(a, 2) = 5;                // Assign 5 to a[2]
    for (i = 0; i < SIZE; i++)
        cout << a[i] << ", ";
    cout << endl;
}

int& refReturn(int* p, int index) {
    int& r = p[index];                  // r is an alias for a[index]
    return r;
}
```

```text
1, 2, 3, 
1, 2, 5, 
```

- In `refReturn(a, 2)`, `p` points to the first element of `a`, and `p[2]` is `a[2]`. With `int& r = p[2];`, `r` becomes an alias for `a[2]`.
- Because `r` is returned as a reference, the call expression `refReturn(a, 2)` denotes `a[2]` itself, and `= 5` assigns 5 to `a[2]`.
- The reference `r` is a local name of the function, but its target `a[2]` belongs to the array of `main()`, so it is still alive after the function ends. Returning a local variable inside a function by reference is not allowed, because the variable disappears when the function ends and leaves a dangling reference (Section 5.1 of the Chapter 5 lecture notes).

---

<br>

## 4. Returning Results through Reference Parameters

**Problem:** Write `average()`, which returns the average of an integer array through the reference parameter `avg` and returns whether the computation succeeded as its return value. An array size of 0 or less is a failure.

```cpp
#include <iostream>
using namespace std;

bool average(int a[], int size, int& avg) {
    if (size <= 0)
        return false;                   // Failure: avg is left unchanged
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += a[i];
    avg = sum / size;                   // Deliver the average through the reference parameter
    return true;                        // Deliver success through the return value
}

int main() {
    int x[] = { 0, 1, 2, 3, 4, 5 };
    int avg;
    if (average(x, 6, avg))
        cout << "평균은 " << avg << endl;
    else
        cout << "매개 변수 오류" << endl;

    if (average(x, -2, avg))
        cout << "평균은 " << avg << endl;
    else
        cout << "매개 변수 오류" << endl;
}
```

```text
평균은 2
매개 변수 오류
```

- A function can have only one return value, so to report both a result and success, one goes through the return value and the other through a reference parameter. (The messages read "the average is" and "parameter error.")
- The average 15 / 6 is integer division and gives 2. The second call returns `false`, so the value of `avg` is meaningless, and the caller must check the return value first.

---

<br>

## 5. Writing a Copy Constructor

> **Definition:** A **copy constructor** is the constructor called when a new object is initialized from an object of the same class; it takes a reference to its own class as its parameter.

```cpp
#include <iostream>
using namespace std;

class Circle {
public:
    int radius;
    Circle(Circle& c);                  // Copy constructor declaration
    Circle(int radius) {
        this->radius = radius;
        cout << "인자 1개 생성자: " << radius << endl;
    }
    ~Circle() { cout << "소멸자: " << radius << endl; }
    double getArea() { return 3.14 * radius * radius; }
};

Circle::Circle(Circle& c) {             // Copy constructor definition
    this->radius = c.radius;
    cout << "복사 생성자 실행 radius = " << radius << endl;
}

int main() {
    Circle src(30);
    Circle dest(src);                   // Copy construction, same as Circle dest = src;
    cout << "원본의 면적 = " << src.getArea() << endl;
    cout << "사본의 면적 = " << dest.getArea() << endl;
    dest.radius = 10;
    cout << "사본의 면적 = " << dest.getArea() << endl;
}
```

```text
인자 1개 생성자: 30
복사 생성자 실행 radius = 30
원본의 면적 = 2826
사본의 면적 = 2826
사본의 면적 = 314
소멸자: 10
소멸자: 30
```

- `Circle dest(src);` calls the copy constructor with `src` as the argument. Apart from its output statement, this copy constructor does the same work as the default copy constructor the compiler inserts automatically, namely copying the members one by one. So even if this copy constructor is deleted, the program behaves the same except for one line of message. (The messages read "one-argument constructor," "copy constructor runs," "area of the original," "area of the copy," and "destructor.")
- `dest.radius = 10;` changes only the copy and leaves the original `src` unchanged. When a class has only members that hold values directly, like `radius`, member-by-member copying fully separates the original from the copy.
- The parameter must be a reference. Receiving it by value, as in `Circle(Circle c)`, would require calling the copy constructor again to copy the argument into the parameter, an endless recursion, so the compiler rejects this declaration (Section 5.3 of the Chapter 5 lecture notes). Since the original is not modified, it is usually declared as `Circle(const Circle& c)`.

> **Caution:** If a member is a pointer to dynamically allocated memory, the member-by-member copy of the default copy constructor makes the original and the copy share the same memory, so a deep copy constructor that allocates new memory and copies the contents must be written (Sections 4.3-4.6 of the Chapter 5 lecture notes).

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Passing objects by value | The parameter object is created by the copy constructor, so the user constructor's message is not printed, while the destructor runs when the function ends. |
| Passing objects by address | The parameter is only a pointer, not a new object, so no extra constructor or destructor runs, and the original object changes. |
| Pointers and references | A pointer is a separate variable holding an address and has its own address, whereas a reference is another name for its target and has the same address as the target. |
| References initialized from references | `int& alias = ref;` creates one more reference to the original variable that `ref` refers to. |
| Reference returns | A call to a function returning a reference denotes the variable itself and can appear on the left of an assignment, but a local variable must not be returned by reference. |
| Reference parameters | Returning the result through a reference parameter and success through the return value lets a function deliver two pieces of information together. |
| Copy constructor | It takes a reference to its own class as its parameter, and if none is written, the compiler inserts a default copy constructor that copies member by member. |

---

<br>

## Self-Check Questions

1. What is printed if `increase()` in Section 2.1 is changed to `void increase(Circle& c)`?

   <details>
   <summary>Answer</summary>

   `생성자 실행 radius = 30`, `31`, and `소멸자 실행 radius = 31` are printed. The reference parameter `c` is only another name for `waffle`, not a new object, so neither the copy constructor nor a destructor runs, and `c.setRadius(31)` changes `waffle` itself. The result is the same as the call by address of Section 2.2, but `.` is used inside the function instead of `->`.

   </details>

2. What happens if, in Section 3.1, `int& r;` is declared instead of `int& r = test;` and `r = test;` is written on the next line?

   <details>
   <summary>Answer</summary>

   `int& r;` is a compile error. A reference must be initialized with its target when declared, and the target cannot be changed afterward. Writing `r = test;` to an initialized reference does not change its target; it assigns the value of `test` to the variable the reference already refers to.

   </details>

3. What is wrong if the following function replaces the `refReturn` of Section 3.3?

   ```cpp
   int& refReturn(int* p, int index) {
       int value = p[index];
       return value;
   }
   ```

   <details>
   <summary>Answer</summary>

   `value` is a local variable that copies the value of `p[index]`, so returning a reference to it leaves a dangling reference that remains after the function ends and `value` disappears. `refReturn(a, 2) = 5;` is undefined behavior that writes to space that no longer exists, and even if it runs, `a[2]` does not change. Compilers usually warn that a reference to a local variable is returned.

   </details>

4. If `Circle copy2 = dest;` is added as the last line of `main()` in Section 5, how does the end of the output change?

   <details>
   <summary>Answer</summary>

   `Circle copy2 = dest;` also initializes a new object from an object of the same class, so the copy constructor is called and `복사 생성자 실행 radius = 10` is printed. When `main()` ends, `copy2`, `dest`, and `src` are destroyed in the reverse order of creation, so `소멸자: 10`, `소멸자: 10`, and `소멸자: 30` are printed.

   </details>
