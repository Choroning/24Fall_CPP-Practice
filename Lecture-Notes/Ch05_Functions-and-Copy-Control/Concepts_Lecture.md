# Chapter 5 — Functions and Copy Control

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 5

> **Learning Objectives:**
> 1. Explain the differences among call by value, call by address, and call by reference
> 2. Explain the copying that happens when an object is passed to a function, assigned, or returned
> 3. Declare reference variables and use reference parameters and reference returns
> 4. Distinguish shallow copy from deep copy and write a deep copy constructor
> 5. Explain when the copy constructor is called implicitly

---

## Table of Contents

- [1. Call by Value, Reference, and Pointer](#1-call-by-value-reference-and-pointer)
  - [1.1 Call by Value](#11-call-by-value)
  - [1.2 Call by Address (Pointer)](#12-call-by-address-pointer)
  - [1.3 Call by Reference](#13-call-by-reference)
  - [1.4 From C to C++: Pointers vs References](#14-from-c-to-c-pointers-vs-references)
  - [1.5 Why References? Motivation and Guidelines](#15-why-references-motivation-and-guidelines)
  - [1.6 Full Swap Example: All Three Methods](#16-full-swap-example-all-three-methods)
  - [1.7 Comparison Table](#17-comparison-table)
- [2. Passing, Assigning, and Returning Objects](#2-passing-assigning-and-returning-objects)
  - [2.1 Passing an Object by Value](#21-passing-an-object-by-value)
  - [2.2 Passing an Object by Address and by Reference](#22-passing-an-object-by-address-and-by-reference)
  - [2.3 Object Assignment](#23-object-assignment)
  - [2.4 Returning an Object](#24-returning-an-object)
- [3. Reference Variables](#3-reference-variables)
  - [3.1 Declaration](#31-declaration)
  - [3.2 Reference as Parameter](#32-reference-as-parameter)
  - [3.3 Reference Return](#33-reference-return)
  - [3.4 Full Reference Return Example](#34-full-reference-return-example)
- [4. Copy Constructor](#4-copy-constructor)
  - [4.1 Concept](#41-concept)
  - [4.2 Default Copy Constructor](#42-default-copy-constructor)
  - [4.3 Shallow Copy Problem](#43-shallow-copy-problem)
  - [4.4 Shallow vs Deep Copy: Memory Visualization](#44-shallow-vs-deep-copy-memory-visualization)
  - [4.5 Deep Copy](#45-deep-copy)
  - [4.6 Full Deep Copy Example: Person Class](#46-full-deep-copy-example-person-class)
  - [4.7 When the Copy Constructor Is Called Implicitly](#47-when-the-copy-constructor-is-called-implicitly)
- [5. Common Mistakes](#5-common-mistakes)
  - [5.1 Returning Reference to Local Variable](#51-returning-reference-to-local-variable)
  - [5.2 Forgetting Deep Copy with Pointer Members](#52-forgetting-deep-copy-with-pointer-members)
  - [5.3 Copy Constructor Parameter Must Be a Reference](#53-copy-constructor-parameter-must-be-a-reference)
  - [5.4 Reference Declaration vs Address-of Operator](#54-reference-declaration-vs-address-of-operator)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Call by Value, Reference, and Pointer

### 1.1 Call by Value

The function receives a **copy** of the argument. When the function is called, the parameter is created on the stack and the value passed by the caller is copied into it. Modifying the parameter inside the function does **not** affect the original variable. This mechanism is the same as in C.

```cpp
#include <iostream>
using namespace std;

void addOne(int x) {
    x = x + 1;   // modifies local copy only
    cout << "Inside: x = " << x << endl;
}

int main() {
    int a = 10;
    addOne(a);
    cout << "Outside: a = " << a << endl;   // a is still 10
    return 0;
}
```

```text
Output:
Inside: x = 11
Outside: a = 10
```

```text
Call by Value
┌──────────────┐
│ main         │
│  a = 10      │  ← original unchanged
└──────┬───────┘
       │ copy
       ▼
┌──────────────┐
│ addOne       │
│  x = 10 → 11 │  ← local copy modified
└──────────────┘
```

*Figure 1. Call by value modifying only the copy*

### 1.2 Call by Address (Pointer)

The function receives a **pointer** to the original variable. The parameter has a pointer type, the caller must pass an address explicitly with `&` (for an array, the array name is already the address), and the function can modify the original by dereferencing with `*`. This mechanism is the same as in C.

```cpp
#include <iostream>
using namespace std;

void addOne(int *p) {
    *p = *p + 1;   // modifies original via pointer
}

int main() {
    int a = 10;
    addOne(&a);     // pass address of a
    cout << "a = " << a << endl;   // a is now 11
    return 0;
}
```

```text
Call by Address (Pointer)
┌───────────────┐
│ main          │
│  a = 10 → 11  │  ← original modified
└───────┬───────┘
        │ &a (address)
        ▼
┌───────────────┐
│ addOne        │
│  p = &a       │
│  *p = 10 → 11 │
└───────────────┘
```

*Figure 2. Call by address modifying the original through a pointer*

### 1.3 Call by Reference

**C++ only.** The function receives a **reference** `&` to the original variable. The reference is an alias that refers to the exact same memory location. No `*` or `&` is needed at the call site, making the syntax cleaner than pointers.

```cpp
#include <iostream>
using namespace std;

void addOne(int &x) {   // x is a reference to the original
    x = x + 1;          // modifies original directly
}

int main() {
    int a = 10;
    addOne(a);           // no & needed at call site
    cout << "a = " << a << endl;   // a is now 11
    return 0;
}
```

```text
Call by Reference
┌───────────────┐
│ main          │
│  a = 10 → 11  │  ← original modified
└───────┬───────┘
        │ reference (alias)
        ▼
┌───────────────┐
│ addOne        │
│  x ≡ a        │  ← x IS a (same memory)
│  x = 10 → 11  │
└───────────────┘
```

*Figure 3. Call by reference modifying the original through an alias*

**Swap written with a reference and with a pointer:**

```cpp
// Reference version (C++ style) — cleaner
void swapRef(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Pointer version (C style) — works but verbose
void swapPtr(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 10, y = 20;
    swapRef(x, y);     // clean call
    swapPtr(&x, &y);   // must pass addresses
    return 0;
}
```

### 1.4 From C to C++: Pointers vs References

In C, to modify the caller's variable, you **had to** use pointers. There was no alternative. C++ introduces **references** as a cleaner way to achieve the same goal.

**The same swap function in C (pointers) and C++ (references), side by side:**

```cpp
/* ===== C version (pointers) ===== */       /* ===== C++ version (references) ===== */

#include <stdio.h>                            #include <iostream>
                                              using namespace std;

void swap(int *a, int *b) {                   void swap(int &a, int &b) {
    int temp = *a;    // dereference           int temp = a;     // direct access
    *a = *b;          // dereference           a = b;            // direct access
    *b = temp;        // dereference           b = temp;         // direct access
}                                             }

int main() {                                  int main() {
    int x = 10, y = 20;                          int x = 10, y = 20;
    swap(&x, &y);    // pass addresses            swap(x, y);      // just pass variables
    printf("%d %d\n", x, y);                      cout << x << " " << y << endl;
    return 0;                                     return 0;
}                                             }
```

**Key differences:**

| Aspect | C (Pointer) | C++ (Reference) |
|:-------|:-----------|:----------------|
| Parameter declaration | `int *a` | `int &a` |
| Call syntax | `swap(&x, &y)` | `swap(x, y)` |
| Access inside function | `*a`, `*b` (must dereference) | `a`, `b` (direct, like normal variables) |
| Risk of NULL | Yes (`swap(NULL, &y)` compiles) | No (reference must bind to valid object) |
| Readability | `*` symbols cluttered everywhere | Clean, looks like call-by-value |

*Table 1. The C pointer version and the C++ reference version of the same swap function*

> **Key Point:** If you are coming from C, think of references as automatically dereferenced pointers that can never be NULL. They are the C++ way of expressing the intent to modify the original.

### 1.5 Why References? Motivation and Guidelines

**Why did C++ add references when pointers already work?**

References are **safer** and **simpler** than pointers.

| Property | Pointer | Reference |
|:---------|:--------|:----------|
| Can be NULL/nullptr? | Yes, must check before use | **No**, must be initialized with a valid object (it can still dangle if the object is destroyed) |
| Can be reassigned to point elsewhere? | Yes, accidental rebinding possible | **No**, binding is permanent |
| Requires `*` to access value? | Yes, easy to forget, causing bugs | **No**, used like a normal variable |
| Can do arithmetic? | Yes (`p++`, `p+3`), powerful but error-prone | **No**, prevents accidental memory wandering |
| Uninitialized state? | Yes (`int *p;` is a wild pointer) | **No**, must be initialized at declaration |

*Table 2. Properties of pointers and references*

**When to use which:**

| Use case | Recommendation |
|:---------|:---------------|
| Modify caller's variable | **Reference** (cleaner, safer) |
| Pass large objects (read-only) | **const reference** (`const MyClass &obj`) |
| Optional parameter (might be absent) | **Pointer** (can pass `nullptr` to mean none) |
| Need to reassign to different object | **Pointer** (references cannot rebind) |
| Array traversal or pointer arithmetic | **Pointer** (references have no arithmetic) |
| Ownership transfer / dynamic memory | **Pointer** (or smart pointer in modern C++) |
| Default choice for most cases | **Reference** |

*Table 3. Choosing between pointers and references by situation*

### 1.6 Full Swap Example: All Three Methods

The following is a complete, runnable program comparing all three parameter-passing methods.

```cpp
#include <iostream>
using namespace std;

// Method 1: Call by Value — does NOT swap the originals
void swapByValue(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    cout << "  Inside swapByValue: a=" << a << ", b=" << b << endl;
}

// Method 2: Call by Pointer — swaps via dereferencing
void swapByPointer(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    cout << "  Inside swapByPointer: *a=" << *a << ", *b=" << *b << endl;
}

// Method 3: Call by Reference — swaps directly
void swapByReference(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
    cout << "  Inside swapByReference: a=" << a << ", b=" << b << endl;
}

int main() {
    int x, y;

    // Test 1: Call by Value
    x = 10; y = 20;
    cout << "Before swapByValue:     x=" << x << ", y=" << y << endl;
    swapByValue(x, y);
    cout << "After swapByValue:      x=" << x << ", y=" << y << endl;
    cout << endl;

    // Test 2: Call by Pointer
    x = 10; y = 20;
    cout << "Before swapByPointer:   x=" << x << ", y=" << y << endl;
    swapByPointer(&x, &y);
    cout << "After swapByPointer:    x=" << x << ", y=" << y << endl;
    cout << endl;

    // Test 3: Call by Reference
    x = 10; y = 20;
    cout << "Before swapByReference: x=" << x << ", y=" << y << endl;
    swapByReference(x, y);
    cout << "After swapByReference:  x=" << x << ", y=" << y << endl;

    return 0;
}
```

```text
Output:
Before swapByValue:     x=10, y=20
  Inside swapByValue: a=20, b=10
After swapByValue:      x=10, y=20        ← NOT swapped! (copy was modified)

Before swapByPointer:   x=10, y=20
  Inside swapByPointer: *a=20, *b=10
After swapByPointer:    x=20, y=10        ← Swapped via pointer

Before swapByReference: x=10, y=20
  Inside swapByReference: a=20, b=10
After swapByReference:  x=20, y=10        ← Swapped via reference (cleanest)
```

### 1.7 Comparison Table

| Feature | Call by Value | Call by Address (Pointer) | Call by Reference |
|:--------|:-------------|:--------------------------|:------------------|
| **Parameter declaration** | `void f(int x)` | `void f(int *p)` | `void f(int &x)` |
| **Call syntax** | `f(a)` | `f(&a)` | `f(a)` |
| **Can modify original?** | No | Yes (via `*p`) | Yes (directly) |
| **Can be NULL?** | N/A | Yes (`nullptr`) | No (must refer to valid object) |
| **Access syntax** | `x` | `*p` | `x` |
| **Language** | C and C++ | C and C++ | **C++ only** |
| **Best use case** | Read-only, small types | Optional parameter (nullable), arrays | Modify original, large objects |

*Table 4. Comparison of the three argument-passing methods*

---

<br>

## 2. Passing, Assigning, and Returning Objects

### 2.1 Passing an Object by Value

When the parameter is an object passed by value, the caller writes only the object name. When the function is called, space for the parameter object is allocated on the stack and the contents of the argument object are copied into it as they are.

The ordinary constructors of the parameter object (`Circle()`, `Circle(int)`) are not executed at this point, because the state of the argument object at the moment of the call must be passed to the parameter object unchanged. When the function ends, however, the parameter object is destroyed and its destructor is executed. The constructor and destructor of the parameter object thus run asymmetrically.

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

void increase(Circle c) {      // 값에 의한 호출: c는 waffle의 복사본
    int r = c.getRadius();
    c.setRadius(r + 1);        // 복사본만 31이 된다
}

int main() {
    Circle waffle(30);
    increase(waffle);
    cout << waffle.getRadius() << endl;
}
```

```text
Output:
생성자 실행 radius = 30
소멸자 실행 radius = 31
30
소멸자 실행 radius = 30
```

The first line ("생성자 실행" means "constructor executed") is the construction of `waffle`, the second ("소멸자 실행" means "destructor executed") is the destruction of the parameter `c` when `increase()` ends, and the last is the destruction of `waffle` when `main()` ends. Only the copy `c` had its radius changed to 31, so the radius of `waffle` stays 30.

```text
 main() stack                increase() stack
┌──────────────┐            ┌──────────────────┐
│ waffle       │    copy    │ c                │
│ radius = 30  ├───────────►│ radius = 30 → 31 │
└──────────────┘            └──────────────────┘
 waffle: constructor Circle(30) runs, destructor runs at the end of main()
 c: no ordinary constructor runs, destructor runs at the end of increase()
```

*Figure 4. Construction and destruction of the parameter object when an object is passed by value*

> **Note:** The parameter object is actually created by the **copy constructor** (Section 4.1). This `Circle` has no copy constructor, so the default copy constructor generated by the compiler copies the members without printing anything. That is why no constructor message appears in the output.

### 2.2 Passing an Object by Address and by Reference

With call by address, only the address of the object is passed, and the parameter is declared as a pointer to the object. No parameter object is created, so neither the constructor nor the destructor runs, and changes made through the pointer inside the function are reflected in the original object. With call by reference, the parameter is only an alias of the original object and no new object is created either, so it has the same properties.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r) { radius = r; cout << "생성자 실행 radius = " << radius << endl; }
    ~Circle() { cout << "소멸자 실행 radius = " << radius << endl; }
    int getRadius() { return radius; }
    void setRadius(int radius) { this->radius = radius; }
};

void increaseByPointer(Circle *p) {   // 주소에 의한 호출
    p->setRadius(p->getRadius() + 1);
}

void increaseCircle(Circle &c) {      // 참조에 의한 호출
    c.setRadius(c.getRadius() + 1);
}

int main() {
    Circle waffle(30);
    increaseByPointer(&waffle);
    increaseCircle(waffle);
    cout << waffle.getRadius() << endl;
}
```

```text
Output:
생성자 실행 radius = 30
32
소멸자 실행 radius = 32
```

Both functions modify `waffle` itself, so the radius goes from 30 to 32, and the constructor and destructor messages appear only once each, for `waffle`.

| Passing method | Parameter | New object created | Constructor runs | Destructor runs | Original modified |
|:----|:----|:----|:----|:----|:----|
| Call by value | `Circle c` | Yes (a copy of the argument) | Ordinary constructor does not run (copy constructor runs) | Runs when the function ends | No |
| Call by address | `Circle *p` | No | Does not run | Does not run | Yes (access with `p->`) |
| Call by reference | `Circle &c` | No | Does not run | Does not run | Yes (access with `c.`) |

*Table 5. Construction and destruction of the parameter object by passing method*

### 2.3 Object Assignment

Objects of the same class type can be assigned to each other with `=`; this is called **object assignment**. Assignment copies all data members of the right-hand object into the left-hand object. The textbook describes this as a bitwise copy; precisely, the default assignment operator generated by the compiler copies member by member, and when all members are of primitive types the result is the same as a bitwise copy.

The two objects only have the same contents at that moment and keep their own independent storage. Changing one after the assignment therefore does not change the other.

```cpp
Circle c1(5);
Circle c2(30);
c1 = c2;            // c2의 멤버가 c1에 복사된다. c1의 반지름은 30이 된다
c2.setRadius(10);   // c2만 바뀌고 c1은 그대로 30이다
cout << c1.getArea() << " " << c2.getArea() << endl;   // 2826 314
```

> **Caution:** When an object with a pointer member is assigned, the default assignment operator also copies only the pointer value, so the shallow copy problem of Section 4.3 occurs in the same way. In addition, the memory the left-hand object pointed to before is never freed and leaks. A class that needs deep copy therefore defines its own assignment operator along with the copy constructor (the Rule of Three in Section 4.5).

### 2.4 Returning an Object

A function can return an object by value. A copy of the object in the return statement is passed to the caller, and in `c = getCircle();` that copy is assigned to `c`.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int radius) { this->radius = radius; }
    void setRadius(int radius) { this->radius = radius; }
    double getArea() { return 3.14 * radius * radius; }
};

Circle getCircle() {
    Circle tmp(30);
    return tmp;            // tmp 객체를 리턴한다
}

int main() {
    Circle c;              // radius = 1로 초기화된다
    cout << c.getArea() << endl;
    c = getCircle();       // 리턴된 객체가 c에 치환된다. c의 반지름은 30이 된다
    cout << c.getArea() << endl;
}
```

```text
Output:
3.14
2826
```

Running the same `main()` with the `Circle` class of Section 2.1, whose constructors and destructor print messages (Apple clang, C++17), gives the following output.

```text
Output:
생성자 실행 radius = 1
3.14
생성자 실행 radius = 30
소멸자 실행 radius = 30
2826
소멸자 실행 radius = 30
```

The third line is the construction of `tmp` inside `getCircle()`, the fourth is the destruction of the returned object after it has been assigned to `c`, and the last is the destruction of `c` when `main()` ends. The compiler may construct `tmp` directly in the place of the return value and omit the copy (Section 4.7); with copy elision turned off, `tmp` and its copy are destroyed separately and one more destructor message appears.

---

<br>

## 3. Reference Variables

### 3.1 Declaration

A reference variable is an **alias** for an existing variable. Declaring a reference creates only a name; no new memory is allocated, and the reference shares the same memory address as the original it is initialized with.

**Syntax:**

```cpp
int original = 10;
int &ref = original;   // ref is an alias for original
```

**Rules:**

| Rule | Explanation |
|:-----|:-----------|
| Must be initialized at declaration | `int &ref;` alone is an error |
| Cannot be reassigned | Once bound, `ref` always refers to `original` |
| Same memory address | `&ref == &original` is `true` |
| Any change through `ref` changes `original` | They are the same variable |

*Table 6. Rules for reference variables*

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int &ref = a;

    cout << "a = " << a << ", ref = " << ref << endl;       // 10, 10
    cout << "&a = " << &a << ", &ref = " << &ref << endl;   // same address

    ref = 20;
    cout << "a = " << a << endl;   // 20 (a changed through ref)

    a = 30;
    cout << "ref = " << ref << endl;   // 30 (ref reflects a's change)

    return 0;
}
```

> **Caution:** A reference cannot be reassigned. The `=` operator on a reference **copies the value**, not the binding.

```cpp
int a = 10, b = 50;
int &ref = a;   // ref is bound to a
ref = b;        // this does NOT rebind ref to b; it copies b's value into a
// a is now 50, ref still refers to a
```

**Pointer to a reference:** Applying `&` to a reference variable yields the address of the original variable. The following example shows that incrementing through the reference, assigning a value to it, and modifying through a pointer are all reflected in the original `n`.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "i" << '\t' << "n" << '\t' << "refn" << endl;
    int i = 1;
    int n = 2;
    int &refn = n;     // refn은 n의 별명 (새 공간이 생기지 않음)
    n = 4;
    refn++;            // refn = 5, n = 5
    cout << i << '\t' << n << '\t' << refn << endl;

    refn = i;          // refn = 1, n = 1 (재바인딩이 아니라 값 대입)
    refn++;            // refn = 2, n = 2
    cout << i << '\t' << n << '\t' << refn << endl;

    int *p = &refn;    // p는 n의 주소를 가진다
    *p = 20;           // refn = 20, n = 20
    cout << i << '\t' << n << '\t' << refn << endl;
}
```

```text
Output:
i	n	refn
1	5	5
1	2	2
1	20	20
```

**Reference to an object:** A reference variable can also be declared for an object. A reference is not a pointer, so its members are accessed with the dot (`.`) operator; writing `refc->setRadius(30);` is an error.

```cpp
Circle circle;
Circle &refc = circle;     // refc는 circle 객체의 별명
refc.setRadius(10);        // 점(.)으로 멤버에 접근한다
cout << refc.getArea() << " " << circle.getArea() << endl;   // 314 314
```

### 3.2 Reference as Parameter

The most common use of references. A parameter declared with a reference type is a **reference parameter**; only a name is created, and it becomes an alias that shares the storage of the caller's argument. Any operation on a reference parameter is therefore an operation on the argument variable.

```cpp
#include <iostream>
using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 10, y = 20;
    swap(x, y);
    cout << "x = " << x << ", y = " << y << endl;   // x = 20, y = 10
    return 0;
}
```

> **Key Point:** Reference parameters allow a function to modify the caller's variables without pointer syntax. This is the preferred C++ idiom for **output parameters**.

**Returning a result through a reference parameter:** The following `average()` returns 0 on an error and the average otherwise.

```cpp
int average(int a[], int size) {
    if (size <= 0) return 0;     // 오류
    int sum = 0;
    for (int i = 0; i < size; i++) sum += a[i];
    return sum / size;
}
```

If this function returns 0, the caller cannot tell whether the average is 0 or an error occurred. Returning the average through a reference parameter and using the return value only to report success solves the problem.

```cpp
#include <iostream>
using namespace std;

bool average(int a[], int size, int &avg) {
    if (size <= 0)
        return false;          // 실패: avg는 바꾸지 않는다
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += a[i];
    avg = sum / size;          // 참조 매개변수로 평균을 돌려준다
    return true;               // 성공
}

int main() {
    int x[] = {0, 1, 2, 3, 4, 5};
    int avg;
    if (average(x, 6, avg)) cout << "평균은 " << avg << endl;
    else cout << "매개 변수 오류" << endl;

    if (average(x, -2, avg)) cout << "평균은 " << avg << endl;
    else cout << "매개 변수 오류" << endl;
}
```

```text
Output:
평균은 2
매개 변수 오류
```

The integer division of the sum 15 by 6 gives 2, so the first call returns the average 2 ("평균은 2" means "the average is 2"); the second call has a negative size and returns `false` ("매개 변수 오류" means "parameter error").

**Taking an object as a reference parameter:** `readRadius()`, which sets the radius read from the keyboard into a `Circle` object, must take the object by reference so that the caller's object changes.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int radius) { this->radius = radius; }
    void setRadius(int radius) { this->radius = radius; }
    double getArea() { return 3.14 * radius * radius; }
};

void readRadius(Circle &c) {
    int r;
    cout << "정수 값으로 반지름을 입력하세요>>";
    cin >> r;                  // 반지름 값 입력
    c.setRadius(r);            // 객체 c(= donut)에 반지름 설정
}

int main() {
    Circle donut;
    readRadius(donut);
    cout << "donut의 면적 = " << donut.getArea() << endl;
}
```

```text
Output (input 3):
정수 값으로 반지름을 입력하세요>>3
donut의 면적 = 28.26
```

### 3.3 Reference Return

A C function can return only a value of a primitive type or a pointer value. A C++ function can also return a **reference** to a variable, which returns a reference to existing storage rather than the variable's value. The return value is an **lvalue**, so it can appear on the left side of an assignment.

```cpp
#include <iostream>
using namespace std;

char c = 'a';

char get() {              // 문자 값을 리턴
    return c;
}

char& find() {            // c에 대한 참조를 리턴
    return c;
}

int main() {
    char a = get();       // a = 'a'
    // get() = 'b';       // 컴파일 오류: 리턴된 값에는 대입할 수 없다
    char b = find();      // b = 'a' (참조가 가리키는 값이 복사된다)
    char &ref = find();   // ref는 c에 대한 참조
    ref = 'M';            // c = 'M'
    find() = 'b';         // find()가 리턴한 공간에 'b' 저장, c = 'b'
    cout << a << ' ' << b << ' ' << c << endl;   // a a b
}
```

The result of `get()`, which returns a value, is a temporary value, so `get() = 'b';` is a compile error. `find()`, on the other hand, returns the storage `c` itself, so assigning to its result changes `c`. The next example returns a reference to an array element.

```cpp
#include <iostream>
using namespace std;

char& charAt(char str[], int index) {
    return str[index];   // returns reference to the element
}

int main() {
    char name[] = "Mike";
    cout << name << endl;          // Mike

    charAt(name, 0) = 'L';        // assign to return value!
    cout << name << endl;          // Like

    char &ref = charAt(name, 2);
    ref = 'n';
    cout << name << endl;          // Line

    return 0;
}
```

> **Caution:** Never return a reference to a **local variable**. The local variable is destroyed when the function returns, leaving a **dangling reference**.

```cpp
int& bad() {
    int local = 10;
    return local;      // DANGER: local is destroyed after return
}

int main() {
    int &ref = bad();  // ref is a dangling reference — undefined behavior
    return 0;
}
```

### 3.4 Full Reference Return Example

A complete, runnable program demonstrating reference return. The function returns a reference to an array element, and the caller modifies the original array through the returned reference.

```cpp
#include <iostream>
using namespace std;

int scores[] = {90, 85, 78, 92, 88};  // global array of 5 scores

// Returns a reference to the element at the given index
int& getScore(int index) {
    return scores[index];
}

void printScores() {
    cout << "Scores: ";
    for (int i = 0; i < 5; i++) {
        cout << scores[i];
        if (i < 4) cout << ", ";
    }
    cout << endl;
}

int main() {
    printScores();                 // Scores: 90, 85, 78, 92, 88

    // Use return value as lvalue — modify array through returned reference
    getScore(1) = 100;            // scores[1] changes from 85 to 100
    printScores();                 // Scores: 90, 100, 78, 92, 88

    // Store the returned reference in a reference variable
    int &ref = getScore(3);       // ref is an alias for scores[3]
    cout << "ref = " << ref << endl;   // ref = 92

    ref = 50;                     // modifying ref modifies scores[3]
    printScores();                 // Scores: 90, 100, 78, 50, 88

    // Use with increment operator
    getScore(0) += 5;             // scores[0] changes from 90 to 95
    printScores();                 // Scores: 95, 100, 78, 50, 88

    return 0;
}
```

```text
Output:
Scores: 90, 85, 78, 92, 88
Scores: 90, 100, 78, 92, 88
ref = 92
Scores: 90, 100, 78, 50, 88
Scores: 95, 100, 78, 50, 88
```

> **Note:** **Why is this useful?** Reference return lets you build clean APIs like `array[i] = value;`. The `operator[]` of `std::vector` and `std::string` works the same way, returning a reference to the element.

---

<br>

## 4. Copy Constructor

### 4.1 Concept

A **copy constructor** is a special constructor that creates a new object as a copy of an existing object. It has one reference parameter of its own class type and can be overloaded alongside ordinary constructors. It is called in three situations.

| Situation | Example |
|:----------|:--------|
| Initialization with another object | `MyClass b = a;` or `MyClass b(a);` |
| Passing an object by value | `void f(MyClass obj);` called as `f(a);` |
| Returning an object by value | `MyClass f() { MyClass obj; return obj; }` |

*Table 7. The three situations in which the copy constructor is called*

**Prototype:**

```cpp
ClassName(const ClassName &other);
```

- The parameter must be a **reference** to its own class, and it is usually a `const` reference because the source is not modified. Writing it without `const`, as the textbook does with `Circle(Circle &c)`, is also a copy constructor. A by-value form `ClassName(ClassName other)` would have to call itself to copy the argument, causing infinite recursion, so C++ rejects such a declaration with a compile error (Section 5.3).

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r) : radius(r) {}
    Circle(const Circle &other) : radius(other.radius) {
        cout << "Copy constructor called" << endl;
    }
    void show() { cout << "Radius: " << radius << endl; }
};

int main() {
    Circle a(10);
    Circle b(a);       // Copy constructor called
    Circle c = a;      // Copy constructor called (same as Circle c(a))
    b.show();          // Radius: 10
    c.show();          // Radius: 10
    return 0;
}
```

> **Note:** The textbook states that a class can declare only one copy constructor. Standard C++ allows copy constructors that differ in the `const` qualification of the parameter, such as `X(X&)` and `X(const X&)`, to coexist, but a class usually has only the `const` reference version.

### 4.2 Default Copy Constructor

If you do not define a copy constructor, the compiler generates one automatically. The **default copy constructor** performs a **member-by-member copy**, that is, a **shallow copy**.

```cpp
class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) {}
    // No copy constructor defined — compiler provides one
    void show() { cout << "(" << x << ", " << y << ")" << endl; }
};

int main() {
    Point a(3, 4);
    Point b = a;   // default copy constructor: b.x = a.x, b.y = a.y
    b.show();      // (3, 4)
    return 0;
}
```

The default copy constructor inserted by the compiler does the same work as the following code, copying each member of the source object into the copy (`this`) as is. If there is a pointer member, the pointer value (the address) is copied as is too.

```cpp
Point(const Point &p) {   // 컴파일러가 삽입하는 디폴트 복사 생성자와 같은 동작
    this->x = p.x;
    this->y = p.y;
}
```

> **Key Point:** For classes with only primitive-type members, the default copy constructor works correctly. The problem arises when a class has **pointer members**.

### 4.3 Shallow Copy Problem

When a class contains a **pointer member**, the default copy constructor copies the **pointer value** (the address), not the data it points to. Both objects end up pointing to the **same memory**.

```cpp
class Book {
    char *title;
    int price;
public:
    Book(const char *t, int p) {
        title = new char[strlen(t) + 1];
        strcpy(title, t);
        price = p;
    }
    ~Book() {
        delete[] title;   // free dynamic memory
    }
};
```

```text
Shallow Copy (default copy constructor):

Book cpp("C++", 10000);     Book java = cpp;

  cpp                         java
┌─────────────┐               ┌─────────────┐
│ title ──────┼──────┐        │ title ──────┼──────┐
│ price=10000 │      │        │ price=10000 │      │
└─────────────┘      │        └─────────────┘      │
                     ▼                             ▼
                ┌──────────┐  <────────────── SAME memory!
                │  "C++"   │
                └──────────┘

When java is destroyed → delete[] title → memory freed
When cpp  is destroyed → delete[] title → DOUBLE FREE! (crash)
```

*Figure 5. Two Book objects pointing to the same string memory after a shallow copy*

**Problems with shallow copy:**

| Problem | Description |
|:--------|:-----------|
| Shared memory | Both objects modify the same data, causing unintended side effects |
| Double free | Both destructors try to `delete[]` the same memory, causing a crash |
| Dangling pointer | After one object is destroyed, the other has a pointer to freed memory |

*Table 8. Problems with shallow copy*

The textbook's `Person` example shows the same problem. If a `Person` class with a `char *name` member has no copy constructor and is copied with `Person daughter(father);`, then `daughter.changeName("Grace")` changes the name in the shared memory, so the name of `father` also becomes Grace. When the program ends, the objects are destroyed in reverse order of construction, `daughter` then `father`, and the destructor of `father` calls `delete[]` again on memory that has already been freed, so the program terminates abnormally.

### 4.4 Shallow vs Deep Copy: Memory Visualization

The following ASCII diagrams trace the entire lifecycle of copying an object with a pointer member.

**Step 1: before copy, the original object exists**

```text
  STACK                            HEAP
┌─────────────────┐
│  original       │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────────>│     "Alice"     │
│ │ age = 25    │ │         │    (6 bytes)    │
│ └─────────────┘ │         └─────────────────┘
└─────────────────┘             addr: 0x1000
```

*Figure 6. Stack and heap layout of the original object before copying*

**Step 2a: after a SHALLOW copy (default copy constructor), DANGER**

```text
  STACK                            HEAP
┌─────────────────┐
│  original       │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────┐    │     "Alice"     │
│ │ age = 25    │ │    ├───>│    (6 bytes)    │  ← SHARED! Both point here!
│ └─────────────┘ │    │    └─────────────────┘
└─────────────────┘    │        addr: 0x1000
                       │
┌─────────────────┐    │
│  copy           │    │
│ ┌─────────────┐ │    │
│ │ name ───────┼─┼────┘
│ │ age = 25    │ │
│ └─────────────┘ │
└─────────────────┘

Problem 1: copy.name[0] = 'B';  →  original also becomes "Blice"!
Problem 2: ~copy() does delete[] 0x1000
           ~original() does delete[] 0x1000 again  →  CRASH (double free)
```

*Figure 7. Two objects sharing heap memory after a shallow copy*

**Step 2b: after a DEEP copy (custom copy constructor), SAFE**

```text
  STACK                            HEAP
┌─────────────────┐
│  original       │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────────>│     "Alice"     │
│ │ age = 25    │ │         │    (6 bytes)    │
│ └─────────────┘ │         └─────────────────┘
└─────────────────┘             addr: 0x1000

┌─────────────────┐
│  copy           │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────────>│     "Alice"     │  ← SEPARATE allocation
│ │ age = 25    │ │         │    (6 bytes)    │
│ └─────────────┘ │         └─────────────────┘
└─────────────────┘             addr: 0x2000

copy.name[0] = 'B';  →  copy is "Blice", original is still "Alice"
~copy() does delete[] 0x2000     ← safe
~original() does delete[] 0x1000 ← safe, different memory
```

*Figure 8. Two objects with their own heap memory after a deep copy*

### 4.5 Deep Copy

To fix the shallow copy problem, write a **custom copy constructor** that **allocates new memory** of the same size as the source's and **copies the contents** of the source. The result is a complete copy in which the copy and the original share no memory, called a **deep copy**.

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char *title;
    int price;
public:
    Book(const char *t, int p) {
        title = new char[strlen(t) + 1];
        strcpy(title, t);
        price = p;
    }

    // Deep copy constructor
    Book(const Book &other) {
        title = new char[strlen(other.title) + 1];   // allocate NEW memory
        strcpy(title, other.title);                   // copy CONTENTS
        price = other.price;
    }

    ~Book() {
        delete[] title;
    }

    void show() {
        cout << title << " " << price << "won" << endl;
    }
};

int main() {
    Book cpp("C++ Guide", 10000);
    Book java = cpp;   // deep copy constructor called
    cpp.show();        // C++ Guide 10000won
    java.show();       // C++ Guide 10000won — independent copy
    return 0;          // both destructors run safely
}
```

```text
Deep Copy (custom copy constructor):

Book cpp("C++", 10000);     Book java = cpp;

  cpp                         java
┌─────────────┐               ┌─────────────┐
│ title ──────┼──┐            │ title ──────┼──┐
│ price=10000 │  │            │ price=10000 │  │
└─────────────┘  │            └─────────────┘  │
                 ▼                             ▼
            ┌──────────┐                  ┌──────────┐
            │  "C++"   │                  │  "C++"   │  ← SEPARATE memory
            └──────────┘                  └──────────┘

Each destructor frees its own memory — no conflict.
```

*Figure 9. Two Book objects with their own string memory after a deep copy*

> **Note:** **Rule of thumb.** If your class allocates dynamic memory (uses `new`), you **must** implement:
> 1. A **copy constructor** (deep copy)
> 2. A **destructor** (to `delete` the memory)
> 3. An **assignment operator** (for the same reason as the copy constructor, Section 2.3)
>
> This is known as the **Rule of Three**.

### 4.6 Full Deep Copy Example: Person Class

A complete, runnable program with a `Person` class that has a dynamically allocated `name` (`char*`). It demonstrates what goes wrong with shallow copy and how deep copy fixes it, with output traces.

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Person {
    char *name;
    int age;
public:
    // Constructor
    Person(const char *n, int a) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        age = a;
        cout << "  [Constructor] Created: " << name << " (age " << age << ")" << endl;
        cout << "    name allocated at: " << (void*)name << endl;
    }

    // Deep copy constructor
    Person(const Person &other) {
        name = new char[strlen(other.name) + 1];  // allocate NEW memory
        strcpy(name, other.name);                   // copy CONTENTS
        age = other.age;
        cout << "  [Copy Constructor] Copied: " << name << " (age " << age << ")" << endl;
        cout << "    original name at: " << (void*)other.name << endl;
        cout << "    new copy name at: " << (void*)name << endl;
    }

    // Destructor
    ~Person() {
        cout << "  [Destructor] Deleting: " << name
             << " at " << (void*)name << endl;
        delete[] name;
    }

    // Modify name to verify independence
    void setName(const char *n) {
        delete[] name;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    void show() const {
        cout << "  " << name << " (age " << age << ")" << endl;
    }
};

int main() {
    cout << "=== Creating alice ===" << endl;
    Person alice("Alice", 25);

    cout << endl << "=== Creating bob as copy of alice ===" << endl;
    Person bob = alice;   // Deep copy constructor called

    cout << endl << "=== After copy ===" << endl;
    alice.show();         // Alice (age 25)
    bob.show();           // Alice (age 25)

    cout << endl << "=== Modifying bob's name to Bob ===" << endl;
    bob.setName("Bob");
    alice.show();         // Alice (age 25) — unchanged! (deep copy worked)
    bob.show();           // Bob (age 25)

    cout << endl << "=== Destructors will run ===" << endl;
    return 0;
}
```

```text
Output:
=== Creating alice ===
  [Constructor] Created: Alice (age 25)
    name allocated at: 0x1000          (address will vary)

=== Creating bob as copy of alice ===
  [Copy Constructor] Copied: Alice (age 25)
    original name at: 0x1000
    new copy name at: 0x2000           ← DIFFERENT address (deep copy!)

=== After copy ===
  Alice (age 25)
  Alice (age 25)

=== Modifying bob's name to Bob ===
  Alice (age 25)                       ← alice is NOT affected
  Bob (age 25)

=== Destructors will run ===
  [Destructor] Deleting: Bob at 0x2080     ← bob's own memory
  [Destructor] Deleting: Alice at 0x1000   ← alice's own memory (no crash!)
```

Destructors run in reverse order of construction, so `bob` is destroyed first and `alice` last. The address of `bob`'s name changed from 0x2000 to 0x2080 because `setName()` allocated the name anew.

> **Note:** **Shallow copy experiment.** If you remove (comment out) the deep copy constructor and run again, the default shallow copy is used. Modifying `bob`'s name then frees the shared memory and corrupts `alice` too, and the program crashes on destruction due to double free.

### 4.7 When the Copy Constructor Is Called Implicitly

Even without being called explicitly in the code, the copy constructor is called automatically by the compiler in the three cases of Table 7. The following example contains all three cases.

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Person {
    char *name;
    int id;
public:
    Person(int id, const char *name) {
        this->id = id;
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    }
    Person(const Person &person) {          // 깊은 복사 생성자
        id = person.id;
        name = new char[strlen(person.name) + 1];
        strcpy(name, person.name);
        cout << "복사 생성자 실행. 원본 객체의 이름 " << name << endl;
    }
    ~Person() { delete[] name; }
    void changeName(const char *name) {
        if (strlen(name) > strlen(this->name)) return;
        strcpy(this->name, name);
    }
};

void f(Person person) {        // (2) 값에 의한 호출: person이 복사 생성된다
    person.changeName("dummy");
}

Person g() {
    Person mother(2, "Jane");
    return mother;             // (3) 리턴: mother의 복사본이 생성될 수 있다
}

int main() {
    Person father(1, "Kitae");
    Person son = father;       // (1) 객체로 초기화: son이 복사 생성된다
    f(father);
    g();
}
```

```text
Output (Apple clang, C++17):
복사 생성자 실행. 원본 객체의 이름 Kitae
복사 생성자 실행. 원본 객체의 이름 Kitae
```

The first line ("copy constructor executed; name of the source object Kitae") is the initialization of `son`, and the second is the construction of the parameter `person` in `f(father)`. In `return mother;` of `g()` a return object is conceptually copy-constructed as well, but that line is missing from the output above.

> **Note:** **Copy elision.** The compiler may construct the local object being returned directly in the caller's return-value slot and omit the copy constructor call. Elision when returning a named local object (NRVO) is permitted but optional, so it depends on the compiler and options; since C++17, elision is guaranteed when returning a temporary, as in `return Person(2, "Jane");`. Compiling the example above with the `-fno-elide-constructors` option adds the line "복사 생성자 실행. 원본 객체의 이름 Jane" to the output. A copy constructor should therefore have no side effects other than copying the object.

---

<br>

## 5. Common Mistakes

### 5.1 Returning Reference to Local Variable

When a function returns a reference to a local variable, the variable is destroyed at the end of the function. The caller receives a **dangling reference**, and accessing it is **undefined behavior**.

```cpp
// BUG: dangling reference
int& createValue() {
    int x = 42;
    return x;       // x is destroyed here!
}

int main() {
    int &ref = createValue();
    cout << ref << endl;   // Undefined behavior! May print 42, garbage, or crash.
    return 0;
}
```

**Fix:** Return by value, or return a reference to something that outlives the function (class member, global, static, heap-allocated data).

```cpp
// OK: return reference to static variable
int& createValue() {
    static int x = 42;   // static — lives for the entire program
    return x;
}

// OK: return by value (for simple types, this is fine)
int createValueByVal() {
    int x = 42;
    return x;   // returns a COPY — safe
}
```

### 5.2 Forgetting Deep Copy with Pointer Members

If your class has pointer members and you do not implement a copy constructor, the compiler's default shallow copy leads to a double free on destruction. This is undefined behavior and usually shows up as a crash.

```cpp
class Buffer {
    int *data;
    int size;
public:
    Buffer(int s) : size(s) {
        data = new int[size];
    }
    ~Buffer() {
        delete[] data;   // both objects will try to delete the same memory!
    }
    // BUG: no copy constructor defined → default shallow copy
};

int main() {
    Buffer a(10);
    Buffer b = a;    // shallow copy: b.data == a.data (same pointer!)
    return 0;        // ~b() deletes data, then ~a() deletes SAME data → CRASH
}
```

**Rule:** If your class uses `new` in the constructor and `delete` in the destructor, you **must** write a copy constructor (and assignment operator). This is the Rule of Three.

### 5.3 Copy Constructor Parameter Must Be a Reference

The copy constructor must take its parameter by **reference**, usually a **const reference**. Taking it by value causes the following problem.

```cpp
class Bad {
    int x;
public:
    Bad(int v) : x(v) {}

    // WRONG: parameter by value
    Bad(Bad other) {     // ← To call this, compiler needs to copy 'other'...
        x = other.x;     //    which calls this copy constructor again...
    }                     //    which needs to copy again... → INFINITE RECURSION
};
```

Allowing such a declaration would make every call copy endlessly, so the compiler rejects `Bad(Bad other)` with an error (Apple clang reports "copy constructor must pass its first argument by reference"). Take the parameter by reference instead:

```cpp
Bad(const Bad &other) {   // const reference — no copy needed to call this
    x = other.x;
}
```

**Why add `const`?** There are two reasons.
1. It promises not to modify the source object (which makes sense, since you are copying *from* it).
2. It allows copying from `const` objects and temporaries.

`Bad(Bad &other)` without `const` is also a valid copy constructor, but then it cannot copy from `const` objects or temporaries.

### 5.4 Reference Declaration vs Address-of Operator

The `&` symbol has **two different meanings** depending on context. This confuses many beginners.

```cpp
int a = 10;

// Context 1: DECLARATION — & means "reference to"
int &ref = a;      // ref is a reference (alias) for a

// Context 2: EXPRESSION — & means "address of"
int *ptr = &a;     // ptr stores the address of a
```

**How to tell them apart:**

| Context | Syntax | Meaning | Example |
|:--------|:-------|:--------|:--------|
| In a type declaration | `Type &name = ...;` | Reference (alias) | `int &ref = a;` |
| In a parameter list | `void f(Type &param)` | Reference parameter | `void swap(int &a, int &b)` |
| In an expression | `&variable` | Address-of operator | `int *p = &a;` |
| In an expression | `*pointer` | Dereference operator | `cout << *p;` |

*Table 9. Meanings of the `&` and `*` symbols by context*

**Quick rule:** If `&` appears **next to a type name** (in a declaration), it means "reference." If it appears **before a variable name** (in an expression), it means "address-of."

```cpp
int a = 10;
int &ref = a;       // reference declaration — ref IS a
int *ptr = &a;      // address-of — ptr HOLDS a's address

// They look similar but are fundamentally different:
ref = 20;           // changes a to 20 (ref is an alias)
*ptr = 30;          // changes a to 30 (ptr is dereferenced)
cout << &ref;       // prints a's address (address-of the alias = address of a)
cout << ptr;        // prints a's address (ptr holds the address)
```

---

<br>

## Summary

| Concept | Key Summary |
|:--------|:-------------|
| Call by value | The function receives a copy of the argument, so the original does not change; this is the same as in C. |
| Call by address (pointer) | The caller passes an address with `&`, and the function receives a pointer and modifies the original via `*`; this is the same as in C. |
| Call by reference | Available only in C++, the function receives a reference `&` and the parameter becomes an alias for the original, giving cleaner syntax than pointers. |
| C to C++ transition | In C, pointers were the only way to modify the caller's variable, and C++ references are the cleaner alternative. |
| Why references? | References cannot be NULL, cannot rebind, and need no `*` syntax, so use references by default and pointers only when a nullptr or reassignment is needed. |
| Passing an object by value | The parameter object is created by copying the argument, so no ordinary constructor runs, but the destructor runs when the function ends. |
| Passing an object by address or reference | No new object is created, so neither the constructor nor the destructor runs, and changes inside the function are reflected in the original object. |
| Object assignment | `c1 = c2;` copies the members between objects of the same class, and the two objects keep independent storage after the assignment. |
| Returning an object | A copy of the object returned by value is passed to the caller, and the compiler may omit this copy. |
| Reference variable | `int &ref = a;` creates an alias that shares the same address without new storage; it must be initialized and cannot rebind. |
| Reference parameter | `void f(int &x)` is the most common use; it modifies the caller's variable without pointer syntax or returns results besides the return value. |
| Reference return | Returning a reference, as in `char& at(int i)`, makes the return value an lvalue that can be assigned to; never return a reference to a local variable. |
| Copy constructor | It has the form `ClassName(const ClassName &other)` and is called on initialization from an object, pass-by-value, and return-by-value. |
| Default copy constructor | The compiler-generated copy constructor does a member-by-member shallow copy and works correctly when all members are primitive. |
| Shallow copy problem | Pointer members end up sharing the same memory, causing double-free crashes and dangling pointers. |
| Deep copy | A custom copy constructor allocates new memory and copies the contents, and it is required when a class uses `new`. |
| Rule of Three | If you need a custom destructor, copy constructor, or assignment operator, you likely need all three. |
| Implicit copy construction | The copy constructor is called automatically on initialization from an object, pass-by-value, and return-by-value, and copy elision may occur on return. |
| Dangling reference | A local variable is destroyed when the function returns, so never return a reference to it. |
| Copy constructor parameter | The parameter must be a reference, usually `const &`; a by-value declaration would need infinite recursion and is a compile error, and `const` allows copying from const objects. |
| `&` dual meaning | In declarations it means reference, in expressions it means address-of, and the context determines the meaning. |

---

<br>

## Self-Check Questions

1. **Passing an object by value:** With the `Circle` of Section 2.1 (constructors and destructor print messages, no copy constructor), running `Circle waffle(30); increase(waffle);` prints how many constructor messages and how many destructor messages by the end of `main()`? Why?

   <details>
   <summary>Answer</summary>

   One constructor message (`waffle`) and two destructor messages (`c` with radius 31 when `increase()` ends, and `waffle` with radius 30 when `main()` ends). The parameter `c` is created not by an ordinary constructor but by the default copy constructor generated by the compiler, which prints nothing.

   </details>

2. **References and assignment:** After the following code runs, what are the values of `a`, `b`, and `ref`?

   ```cpp
   int a = 10, b = 50;
   int &ref = a;
   ref = b;
   b = 70;
   ```

   <details>
   <summary>Answer</summary>

   `a` is 50, `b` is 70, and `ref` is 50. `ref = b;` does not rebind `ref` to `b`; it assigns the value 50 of `b` to `a`. Changing `b` afterwards has no effect because `ref` is still an alias for `a`.

   </details>

3. **Which statements call the copy constructor:** `Person` defines a copy constructor, and `void f(Person p);` and `void g(Person &p);` exist. Choose all statements that call the copy constructor. (a) `Person b = a;` (b) `c = a;` for an already constructed `Person c` (c) `f(a);` (d) `g(a);`

   <details>
   <summary>Answer</summary>

   (a) and (c). (a) initializes an object from another object, and (c) creates the parameter object through call by value. (b) assigns to an object that already exists, so the assignment operator is called, and (d) uses a reference parameter, so no new object is created.

   </details>

4. **Copy constructor parameter:** Why does a copy constructor declared as `Book(Book other);` not compile? Write two correct declarations.

   <details>
   <summary>Answer</summary>

   Creating the by-value parameter `other` requires calling the copy constructor of `Book`, and that call requires copying the parameter again, which is infinite recursion, so C++ does not allow the declaration. The correct declarations are `Book(const Book &other);` (the usual form) and `Book(Book &other);` (the textbook's form, which cannot copy `const` objects or temporaries).

   </details>

5. **Shallow copy:** A `Book` class allocates `title = new char[...]` in its constructor and calls `delete[] title;` in its destructor but has no copy constructor. What happens at the end of `main()` after `Book a("C++", 10000); Book b = a;`? How do you fix it?

   <details>
   <summary>Answer</summary>

   The default copy constructor copies only the `title` pointer value, so `a.title` and `b.title` point to the same memory. When `main()` ends, `b` and then `a` are destroyed and the same memory is passed to `delete[]` twice, which is undefined behavior (usually a crash). Fix it by writing a deep copy constructor that allocates new memory and copies the string, and, following the Rule of Three, an assignment operator as well.

   </details>
