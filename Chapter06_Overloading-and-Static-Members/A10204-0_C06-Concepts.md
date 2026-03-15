# Chapter 06 — Overloading and Static Members

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpum C++ Programming (Revised Edition)* by Hwang Kitae — Chapter 6

---

## Table of Contents

- [1. Function Overloading](#1-function-overloading)
  - [1.1 Concept](#11-concept)
  - [1.2 Resolution Rules](#12-resolution-rules)
  - [1.3 Examples](#13-examples)
- [2. Default Parameters](#2-default-parameters)
  - [2.1 Syntax](#21-syntax)
  - [2.2 Declaration vs Definition](#22-declaration-vs-definition)
  - [2.3 Overloading Ambiguity](#23-overloading-ambiguity)
- [3. Static Members](#3-static-members)
  - [3.1 Static Member Variables](#31-static-member-variables)
  - [3.2 Static Member Functions](#32-static-member-functions)
  - [3.3 Use Cases](#33-use-cases)
- [Summary](#summary)

---

<br>

## 1. Function Overloading

### 1.1 Concept

**Function overloading** allows multiple functions with the **same name** but **different parameter lists** to coexist. The compiler selects the correct function based on the arguments at the call site.

```cpp
#include <iostream>
using namespace std;

int sum(int a, int b) {
    return a + b;
}

double sum(double a, double b) {
    return a + b;
}

int sum(int a, int b, int c) {
    return a + b + c;
}

int main() {
    cout << sum(1, 2) << endl;        // calls int sum(int, int) → 3
    cout << sum(1.5, 2.5) << endl;    // calls double sum(double, double) → 4
    cout << sum(1, 2, 3) << endl;     // calls int sum(int, int, int) → 6
    return 0;
}
```

> **Key Point:** The **return type alone** does NOT distinguish overloads. Two functions with the same name and same parameter list but different return types cause a compilation error.

```cpp
int    compute(int x);    // OK
double compute(int x);    // ERROR: same parameter list as above
```

**What makes overloads distinct:**

| Distinguishes overloads | Does NOT distinguish |
|:------------------------|:---------------------|
| Number of parameters | Return type |
| Type of parameters | Parameter names |
| Order of parameter types | |

### 1.2 Resolution Rules

When an overloaded function is called, the compiler follows a priority order to find the best match:

| Priority | Match Type | Description |
|:---------|:-----------|:------------|
| 1 | **Exact match** | Argument type matches parameter type exactly |
| 2 | **Promotion** | `char`/`short` to `int`, `float` to `double` |
| 3 | **Standard conversion** | `int` to `double`, `double` to `int`, etc. |

If multiple functions match at the **same priority level**, the call is **ambiguous** and the compiler reports an error.

```cpp
void print(int x)    { cout << "int: " << x << endl; }
void print(double x) { cout << "double: " << x << endl; }
void print(char x)   { cout << "char: " << x << endl; }

print(10);       // exact match → print(int)
print(3.14);     // exact match → print(double)
print('A');      // exact match → print(char)
print(3.14f);    // promotion: float → double → print(double)
```

**Ambiguity example:**

```cpp
void f(float x)  { cout << "float" << endl; }
void f(double x) { cout << "double" << endl; }

f(10);   // ERROR: int → float and int → double are both standard conversions
         // ambiguous — compiler cannot choose
```

### 1.3 Examples

**Absolute value:**

```cpp
int abs(int x) {
    return (x >= 0) ? x : -x;
}

double abs(double x) {
    return (x >= 0) ? x : -x;
}

int main() {
    cout << abs(-5) << endl;      // 5 (int version)
    cout << abs(-3.14) << endl;   // 3.14 (double version)
    return 0;
}
```

**Printing different types:**

```cpp
#include <iostream>
#include <string>
using namespace std;

void print(int x)          { cout << "int: " << x << endl; }
void print(string s)       { cout << "string: " << s << endl; }
void print(int x, int y)   { cout << "two ints: " << x << ", " << y << endl; }

int main() {
    print(42);             // int: 42
    print("hello");        // string: hello
    print(3, 7);           // two ints: 3, 7
    return 0;
}
```

---

<br>

## 2. Default Parameters

### 2.1 Syntax

A **default parameter** provides a fallback value when the caller omits the argument. Defaults must be assigned from the **rightmost** parameter.

```cpp
#include <iostream>
using namespace std;

void greet(string name, string msg = "Hello", int times = 1) {
    for (int i = 0; i < times; i++)
        cout << msg << ", " << name << "!" << endl;
}

int main() {
    greet("Alice");                  // Hello, Alice!        (msg="Hello", times=1)
    greet("Bob", "Hi");             // Hi, Bob!              (times=1)
    greet("Carol", "Hey", 3);       // Hey, Carol! (x3)
    return 0;
}
```

**Rules:**

| Rule | Valid | Invalid |
|:-----|:------|:--------|
| Defaults must be rightmost | `void f(int a, int b = 1, int c = 2)` | `void f(int a = 1, int b, int c = 2)` |
| Cannot skip middle defaults | `f(10)` or `f(10, 20)` or `f(10, 20, 30)` | `f(10, , 30)` |

```cpp
// ERROR: default not at rightmost position
void bad(int a = 1, int b, int c = 3);  // b has no default, but a does

// OK: defaults are contiguous from the right
void ok(int a, int b = 2, int c = 3);
```

### 2.2 Declaration vs Definition

When the function declaration (prototype) and definition are **separate**, the default values must be specified in the **declaration only**, not in the definition.

```cpp
// In header or before main — declaration with defaults
void display(int x, int y = 0, int z = 0);

int main() {
    display(1);          // y=0, z=0
    display(1, 2);       // z=0
    display(1, 2, 3);
    return 0;
}

// Definition — NO defaults here
void display(int x, int y, int z) {
    cout << x << " " << y << " " << z << endl;
}
```

> **Warning:** Specifying defaults in **both** the declaration and definition is a compilation error (redefinition of default argument).

### 2.3 Overloading Ambiguity

Default parameters can cause **ambiguity** with overloaded functions.

```cpp
void func(int a) {
    cout << "one param: " << a << endl;
}

void func(int a, int b = 10) {
    cout << "two params: " << a << ", " << b << endl;
}

int main() {
    func(5);       // ERROR: ambiguous!
    // Could be func(int) or func(int, int) with b defaulting to 10
    // Compiler cannot decide → compilation error

    func(5, 20);   // OK: only func(int, int) matches
    return 0;
}
```

> **Tip:** Avoid defining overloads that differ only by a trailing default parameter. If `func(int)` exists, do not add `func(int, int = default)`.

---

<br>

## 3. Static Members

### 3.1 Static Member Variables

A **static member variable** is shared across **all objects** of the class. There is only **one copy** in memory, regardless of how many objects are created.

**Declaration and definition:**

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    static int count;   // declaration inside class

    Circle(int r) : radius(r) {
        count++;
    }
    ~Circle() {
        count--;
    }
};

// MUST define and initialize outside the class
int Circle::count = 0;

int main() {
    Circle a(10);
    Circle b(20);
    Circle c(30);
    cout << "Number of circles: " << Circle::count << endl;   // 3
    cout << "Via object: " << a.count << endl;                 // 3 (same value)
    return 0;
}
```

```
Memory layout:

Static member (ONE copy):
┌──────────────────┐
│ Circle::count = 3 │  ← shared by all objects
└──────────────────┘

Objects (separate copies):
┌──────────┐  ┌──────────┐  ┌──────────┐
│ a        │  │ b        │  │ c        │
│ radius=10│  │ radius=20│  │ radius=30│
└──────────┘  └──────────┘  └──────────┘
```

**Key rules:**

| Rule | Detail |
|:-----|:-------|
| Declared with `static` inside the class | `static int count;` |
| Must be defined outside the class | `int ClassName::count = 0;` |
| Shared across all objects | Only one copy exists |
| Accessed via class name or object | `ClassName::count` or `obj.count` |
| Exists even with zero objects | Lifetime is the entire program |

> **Warning:** Forgetting the definition outside the class (`int Circle::count = 0;`) causes a **linker error**, not a compiler error. The declaration inside the class only announces existence; the definition allocates memory.

### 3.2 Static Member Functions

A **static member function** can only access **static members**. It does **not** have a `this` pointer because it is not associated with any particular object.

```cpp
#include <iostream>
using namespace std;

class Math {
public:
    static int abs(int x) {
        return (x >= 0) ? x : -x;
    }

    static int max(int a, int b) {
        return (a > b) ? a : b;
    }
};

int main() {
    // Called via class name — no object needed
    cout << Math::abs(-5) << endl;    // 5
    cout << Math::max(3, 7) << endl;  // 7
    return 0;
}
```

**Restrictions of static member functions:**

```cpp
class Example {
    int data;              // non-static member
    static int shared;     // static member
public:
    static void staticFunc() {
        // shared = 10;    // OK: can access static member
        // data = 20;      // ERROR: cannot access non-static member
        // this->data;     // ERROR: no 'this' pointer
    }

    void normalFunc() {
        shared = 10;       // OK: non-static can access static
        data = 20;         // OK: non-static can access non-static
    }
};

int Example::shared = 0;
```

| Feature | Static Member Function | Non-Static Member Function |
|:--------|:----------------------|:--------------------------|
| `this` pointer | No | Yes |
| Access static members | Yes | Yes |
| Access non-static members | **No** | Yes |
| Call without object | `ClassName::func()` | Not possible |

### 3.3 Use Cases

**Object counting:**

```cpp
class Student {
    string name;
    static int count;
public:
    Student(string n) : name(n) { count++; }
    ~Student() { count--; }
    static int getCount() { return count; }
};

int Student::count = 0;

int main() {
    Student a("Alice");
    Student b("Bob");
    cout << Student::getCount() << endl;   // 2
    {
        Student c("Carol");
        cout << Student::getCount() << endl;   // 3
    }   // c destroyed here
    cout << Student::getCount() << endl;   // 2
    return 0;
}
```

**Bulletin board with static array (based on course problems):**

```cpp
#include <iostream>
#include <string>
#define MAX_NO 5
using namespace std;

class Board {
    static int no;                  // current number of posts
    static string board[MAX_NO];   // static array of posts
    Board() {}                     // private constructor — prevent object creation
public:
    static void add(string text) {
        if (no >= MAX_NO) {
            cout << "Cannot add! Maximum " << MAX_NO << " posts." << endl;
            return;
        }
        board[no++] = text;
    }

    static void print() {
        if (no == 0) {
            cout << "<No posts registered.>" << endl;
            return;
        }
        for (int i = 0; i < no; i++)
            cout << i << ": " << board[i] << endl;
    }

    static void clear() {
        no = 0;
    }
};

int Board::no = 0;
string Board::board[MAX_NO];

int main() {
    Board::add("Assignment due Friday.");
    Board::add("Lab session moved to Room 301.");
    Board::print();
    // 0: Assignment due Friday.
    // 1: Lab session moved to Room 301.
    Board::clear();
    Board::print();
    // <No posts registered.>
    return 0;
}
```

**Key design points in this pattern:**

| Point | Explanation |
|:------|:-----------|
| Private constructor | Prevents creating `Board` objects — all interaction through static functions |
| Static array + static count | Data exists without any object; managed entirely through class name |
| All functions are static | `Board::add(...)`, `Board::print()` — called without objects |

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Function overloading | Same name, different parameter lists; return type alone does NOT distinguish |
| Overload resolution | Exact match > promotion > conversion; ambiguity error if tied |
| Default parameters | `void f(int a, int b = 10)` — defaults must be rightmost; cannot skip middle |
| Defaults in declaration only | When prototype and definition are separate, defaults go in the prototype only |
| Default + overloading ambiguity | `f(int)` vs `f(int, int = 10)` makes `f(5)` ambiguous — avoid this pattern |
| Static member variable | Shared by all objects; one copy; declared `static` inside class; defined outside class |
| Static member variable definition | `int ClassName::count = 0;` outside the class — forgetting causes linker error |
| Static member function | No `this` pointer; can only access static members; called via `ClassName::func()` |
| Object counting | Increment static count in constructor, decrement in destructor |
| Private constructor + static members | Prevents object creation; all data/functions accessed via class name (Board pattern) |

---
