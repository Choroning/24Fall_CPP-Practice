# Chapter 05 — Functions and Copy Control

> **Last Updated:** 2026-03-16

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
- [2. Reference Variables](#2-reference-variables)
  - [2.1 Declaration](#21-declaration)
  - [2.2 Reference as Parameter](#22-reference-as-parameter)
  - [2.3 Reference Return](#23-reference-return)
  - [2.4 Full Reference Return Example](#24-full-reference-return-example)
- [3. Copy Constructor](#3-copy-constructor)
  - [3.1 Concept](#31-concept)
  - [3.2 Default Copy Constructor](#32-default-copy-constructor)
  - [3.3 Shallow Copy Problem](#33-shallow-copy-problem)
  - [3.4 Shallow vs Deep Copy: Memory Visualization](#34-shallow-vs-deep-copy-memory-visualization)
  - [3.5 Deep Copy](#35-deep-copy)
  - [3.6 Full Deep Copy Example: Person Class](#36-full-deep-copy-example-person-class)
- [4. Common Mistakes](#4-common-mistakes)
  - [4.1 Returning Reference to Local Variable](#41-returning-reference-to-local-variable)
  - [4.2 Forgetting Deep Copy with Pointer Members](#42-forgetting-deep-copy-with-pointer-members)
  - [4.3 Copy Constructor Parameter Must Be const Reference](#43-copy-constructor-parameter-must-be-const-reference)
  - [4.4 Reference Declaration vs Address-of Operator](#44-reference-declaration-vs-address-of-operator)
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
│  x = 10 → 11 │  ← local copy modified
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
│  x ≡ a        │  ← x IS a (same memory)
│  x = 10 → 11  │
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

### 1.4 From C to C++: Pointers vs References

In C, to modify the caller's variable, you **had to** use pointers. There was no alternative. C++ introduces **references** as a cleaner way to achieve the same goal.

**The same swap function — C (pointers) vs C++ (references), side by side:**

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

> **Takeaway:** If you are coming from C, think of references as "automatically dereferenced pointers that can never be NULL." They are the C++ way of saying "I want to modify the original."

### 1.5 Why References? Motivation and Guidelines

**Why did C++ add references when pointers already work?**

References are **safer** and **simpler** than pointers:

| Property | Pointer | Reference |
|:---------|:--------|:----------|
| Can be NULL/nullptr? | Yes -- must check before use | **No** -- always refers to valid object |
| Can be reassigned to point elsewhere? | Yes -- accidental rebinding possible | **No** -- binding is permanent |
| Requires `*` to access value? | Yes -- easy to forget, causing bugs | **No** -- used like a normal variable |
| Can do arithmetic? | Yes (`p++`, `p+3`) -- powerful but error-prone | **No** -- prevents accidental memory wandering |
| Uninitialized state? | Yes (`int *p;` is a wild pointer) | **No** -- must be initialized at declaration |

**When to use which:**

| Use case | Recommendation |
|:---------|:---------------|
| Modify caller's variable | **Reference** -- cleaner, safer |
| Pass large objects (read-only) | **const reference** (`const MyClass &obj`) |
| Optional parameter (might be absent) | **Pointer** -- can pass `nullptr` to mean "none" |
| Need to reassign to different object | **Pointer** -- references cannot rebind |
| Array traversal or pointer arithmetic | **Pointer** -- references have no arithmetic |
| Ownership transfer / dynamic memory | **Pointer** (or smart pointer in modern C++) |
| Default choice for most cases | **Reference** |

### 1.6 Full Swap Example: All Three Methods

A complete, runnable program comparing all three parameter-passing methods:

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

```
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

### 2.4 Full Reference Return Example

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

```
Output:
Scores: 90, 85, 78, 92, 88
Scores: 90, 100, 78, 92, 88
ref = 92
Scores: 90, 100, 78, 50, 88
Scores: 95, 100, 78, 50, 88
```

> **Why is this useful?** Reference return lets you build clean APIs like `array[i] = value;` — the `operator[]` in `std::vector` and `std::string` works exactly this way internally.

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

**Problems with shallow copy:**

| Problem | Description |
|:--------|:-----------|
| Shared memory | Both objects modify the same data — unintended side effects |
| Double free | Both destructors try to `delete[]` the same memory — crash |
| Dangling pointer | After one object is destroyed, the other has a pointer to freed memory |

### 3.4 Shallow vs Deep Copy: Memory Visualization

The following ASCII diagrams trace the entire lifecycle of copying an object with a pointer member.

**Step 1 -- Before copy: original object exists**

```
  STACK                            HEAP
┌─────────────────┐
│  original       │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────────>│     "Alice"     │
│ │ age = 25    │ │         │    (6 bytes)    │
│ └─────────────┘ │         └─────────────────┘
└─────────────────┘             addr: 0x1000
```

**Step 2a -- After SHALLOW copy (default copy constructor): DANGER**

```
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

**Step 2b -- After DEEP copy (custom copy constructor): SAFE**

```
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

### 3.5 Deep Copy

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

> **Rule of thumb:** If your class allocates dynamic memory (uses `new`), you **must** implement:
> 1. A **copy constructor** (deep copy)
> 2. A **destructor** (to `delete` the memory)
> 3. An **assignment operator** (for the same reason as the copy constructor)
> This is known as the **Rule of Three**.

### 3.6 Full Deep Copy Example: Person Class

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

```
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

> **Try removing the deep copy constructor** (comment it out) and run again. With the default shallow copy, modifying `bob`'s name would corrupt `alice` too, and the program would crash on destruction due to double free.

---

<br>

## 4. Common Mistakes

### 4.1 Returning Reference to Local Variable

When a function returns a reference to a local variable, the variable is destroyed at the end of the function. The caller receives a **dangling reference** -- accessing it is **undefined behavior**.

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

### 4.2 Forgetting Deep Copy with Pointer Members

If your class has pointer members and you do not implement a copy constructor, the compiler's default shallow copy will lead to crashes on destruction.

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

### 4.3 Copy Constructor Parameter Must Be const Reference

The copy constructor **must** take its parameter by **const reference**. Here is what happens if you try to take it by value:

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

The compiler will reject `Bad(Bad other)` with an error. The parameter must be:

```cpp
Bad(const Bad &other) {   // const reference — no copy needed to call this
    x = other.x;
}
```

**Why `const`?** Two reasons:
1. It promises not to modify the source object (which makes sense -- you are copying *from* it).
2. It allows copying from `const` objects and temporaries.

### 4.4 Reference Declaration vs Address-of Operator

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

| Concept | Key Takeaway |
|:--------|:-------------|
| Call by value | Function gets a copy; original unchanged; same as C |
| Call by address (pointer) | Function gets pointer; modifies original via `*`; caller passes `&`; same as C |
| Call by reference | C++ only; function gets reference `&`; alias for original; cleaner syntax |
| C to C++ transition | In C, pointers were the only way to modify caller's variable; C++ references are the cleaner alternative |
| Why references? | Cannot be NULL, cannot rebind, no `*` syntax; use references by default, pointers only when nullable/reassignable |
| Reference variable | `int &ref = a;` — alias, same address; must initialize; cannot reassign binding |
| Reference parameter | `void f(int &x)` — most common use; modifies caller's variable without pointer syntax |
| Reference return | `char& at(int i)` — return value is lvalue; can assign to it; never return reference to local |
| Copy constructor | `ClassName(const ClassName &other)`; called on init-from-object, pass-by-value, return-by-value |
| Default copy constructor | Compiler-generated; member-by-member shallow copy; fine for primitive members |
| Shallow copy problem | Pointer members share same memory; double free crash; dangling pointer |
| Deep copy | Custom copy constructor; allocates new memory and copies contents; required when class uses `new` |
| Rule of Three | If you need a custom destructor, copy constructor, or assignment operator, you likely need all three |
| Dangling reference | Never return reference to local variable; it is destroyed when function returns |
| Copy ctor must be `const &` | Pass by value would cause infinite recursion; `const` allows copying from const objects |
| `&` dual meaning | In declarations = reference; in expressions = address-of; context determines meaning |

---
