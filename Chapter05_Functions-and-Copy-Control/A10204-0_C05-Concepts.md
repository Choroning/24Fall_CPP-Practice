# Chapter 05 — Functions and Copy Control

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpum C++ Programming (Revised Edition)* by Hwang Kitae — Chapter 5

---

## Table of Contents

- [1. Call by Value, Reference, and Pointer](#1-call-by-value-reference-and-pointer)
  - [1.1 Call by Value](#11-call-by-value)
  - [1.2 Call by Address (Pointer)](#12-call-by-address-pointer)
  - [1.3 Call by Reference](#13-call-by-reference)
  - [1.4 Comparison Table](#14-comparison-table)
- [2. Reference Variables](#2-reference-variables)
  - [2.1 Declaration](#21-declaration)
  - [2.2 Reference as Parameter](#22-reference-as-parameter)
  - [2.3 Reference Return](#23-reference-return)
- [3. Copy Constructor](#3-copy-constructor)
  - [3.1 Concept](#31-concept)
  - [3.2 Default Copy Constructor](#32-default-copy-constructor)
  - [3.3 Shallow Copy Problem](#33-shallow-copy-problem)
  - [3.4 Deep Copy](#34-deep-copy)
- [Summary](#summary)

---

<br>

## 1. Call by Value, Reference, and Pointer

### 1.1 Call by Value

The function receives a **copy** of the argument. Modifying the parameter inside the function does **not** affect the original variable. This mechanism is the same as in C.

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

```
Output:
Inside: x = 11
Outside: a = 10
```

```
Call by Value
┌──────────────┐
│ main         │
│  a = 10      │  ← original unchanged
└──────┬───────┘
       │ copy
       ▼
┌──────────────┐
│ addOne       │
│  x = 10 → 11│  ← local copy modified
└──────────────┘
```

### 1.2 Call by Address (Pointer)

The function receives a **pointer** to the original variable. It can modify the original by dereferencing with `*`. The caller must pass an address with `&`. This mechanism is the same as in C.

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

```
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

### 1.3 Call by Reference

**C++ only.** The function receives a **reference** `&` to the original variable. The reference is an alias — it refers to the exact same memory location. No `*` or `&` is needed at the call site, making the syntax cleaner than pointers.

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

```
Call by Reference
┌───────────────┐
│ main          │
│  a = 10 → 11  │  ← original modified
└───────┬───────┘
        │ reference (alias)
        ▼
┌───────────────┐
│ addOne        │
│  x ≡ a       │  ← x IS a (same memory)
│  x = 10 → 11 │
└───────────────┘
```

**Swap example — reference vs pointer:**

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

### 1.4 Comparison Table

| Feature | Call by Value | Call by Address (Pointer) | Call by Reference |
|:--------|:-------------|:--------------------------|:------------------|
| **Parameter declaration** | `void f(int x)` | `void f(int *p)` | `void f(int &x)` |
| **Call syntax** | `f(a)` | `f(&a)` | `f(a)` |
| **Can modify original?** | No | Yes (via `*p`) | Yes (directly) |
| **Can be NULL?** | N/A | Yes (`nullptr`) | No (must refer to valid object) |
| **Access syntax** | `x` | `*p` | `x` |
| **Language** | C and C++ | C and C++ | **C++ only** |
| **Best use case** | Read-only, small types | Optional parameter (nullable), arrays | Modify original, large objects |

---

<br>

## 2. Reference Variables

### 2.1 Declaration

A reference variable is an **alias** for an existing variable. It shares the same memory address as the original.

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

> **Warning:** A reference cannot be reassigned. The `=` operator on a reference **copies the value**, not the binding.

```cpp
int a = 10, b = 50;
int &ref = a;   // ref is bound to a
ref = b;        // this does NOT rebind ref to b; it copies b's value into a
// a is now 50, ref still refers to a
```

### 2.2 Reference as Parameter

The most common use of references. The function parameter becomes an alias for the caller's argument.

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

> **Key Point:** Reference parameters allow a function to modify the caller's variables without pointer syntax. This is the preferred C++ idiom for "output" parameters.

### 2.3 Reference Return

A function can return a **reference** to a variable. This means the return value is an **lvalue** — it can appear on the left side of an assignment.

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

> **Warning:** Never return a reference to a **local variable**. The local variable is destroyed when the function returns, leaving a **dangling reference**.

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

---

<br>

## 3. Copy Constructor

### 3.1 Concept

A **copy constructor** creates a new object as a copy of an existing object. It is called in three situations:

| Situation | Example |
|:----------|:--------|
| Initialization with another object | `MyClass b = a;` or `MyClass b(a);` |
| Passing an object by value | `void f(MyClass obj);` called as `f(a);` |
| Returning an object by value | `MyClass f() { MyClass obj; return obj; }` |

**Prototype:**

```cpp
ClassName(const ClassName &other);
```

- The parameter must be a **const reference**. If it were passed by value, the copy constructor would call itself to copy the argument, causing **infinite recursion**.

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

### 3.2 Default Copy Constructor

If you do not define a copy constructor, the compiler generates one automatically. The **default copy constructor** performs a **member-by-member copy** (shallow copy).

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

> **Key Point:** For classes with only primitive-type members, the default copy constructor works correctly. The problem arises when a class has **pointer members**.

### 3.3 Shallow Copy Problem

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

```
Shallow Copy (default copy constructor):

Book cpp("C++", 10000);     Book java = cpp;

  cpp                         java
┌──────────┐               ┌──────────┐
│ title ───┼──────┐        │ title ───┼──────┐
│ price=10000│     │        │ price=10000│     │
└──────────┘     │        └──────────┘     │
                  ▼                          ▼
              ┌──────────┐  ← SAME memory!
              │ "C++"    │
              └──────────┘

When java is destroyed → delete[] title → memory freed
When cpp  is destroyed → delete[] title → DOUBLE FREE! (crash)
```

**Problems with shallow copy:**

| Problem | Description |
|:--------|:-----------|
| Shared memory | Both objects modify the same data — unintended side effects |
| Double free | Both destructors try to `delete[]` the same memory — crash |
| Dangling pointer | After one object is destroyed, the other has a pointer to freed memory |

### 3.4 Deep Copy

To fix the shallow copy problem, write a **custom copy constructor** that allocates **new memory** and copies the **contents**.

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

```
Deep Copy (custom copy constructor):

Book cpp("C++", 10000);     Book java = cpp;

  cpp                         java
┌──────────┐               ┌──────────┐
│ title ───┼──┐            │ title ───┼──┐
│ price=10000│ │            │ price=10000│ │
└──────────┘ │            └──────────┘ │
              ▼                          ▼
          ┌──────────┐             ┌──────────┐
          │ "C++"    │             │ "C++"    │  ← SEPARATE memory
          └──────────┘             └──────────┘

Each destructor frees its own memory — no conflict.
```

> **Rule of thumb:** If your class allocates dynamic memory (uses `new`), you **must** implement:
> 1. A **copy constructor** (deep copy)
> 2. A **destructor** (to `delete` the memory)
> 3. An **assignment operator** (for the same reason as the copy constructor)
>
> This is known as the **Rule of Three**.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Call by value | Function gets a copy; original unchanged; same as C |
| Call by address (pointer) | Function gets pointer; modifies original via `*`; caller passes `&`; same as C |
| Call by reference | C++ only; function gets reference `&`; alias for original; cleaner syntax |
| Reference variable | `int &ref = a;` — alias, same address; must initialize; cannot reassign binding |
| Reference parameter | `void f(int &x)` — most common use; modifies caller's variable without pointer syntax |
| Reference return | `char& at(int i)` — return value is lvalue; can assign to it; never return reference to local |
| Copy constructor | `ClassName(const ClassName &other)`; called on init-from-object, pass-by-value, return-by-value |
| Default copy constructor | Compiler-generated; member-by-member shallow copy; fine for primitive members |
| Shallow copy problem | Pointer members share same memory; double free crash; dangling pointer |
| Deep copy | Custom copy constructor; allocates new memory and copies contents; required when class uses `new` |
| Rule of Three | If you need a custom destructor, copy constructor, or assignment operator, you likely need all three |

---
