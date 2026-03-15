# Chapter 03 — Classes and Objects

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpoom C++ Programming (Revised Edition)* — Chapter 3

---

## Table of Contents

- [1. Object-Oriented Concepts](#1-object-oriented-concepts)
  - [1.1 Objects](#11-objects)
  - [1.2 Class vs Object](#12-class-vs-object)
- [2. Class Definition](#2-class-definition)
  - [2.1 Syntax](#21-syntax)
  - [2.2 Member Variables](#22-member-variables)
  - [2.3 Member Functions](#23-member-functions)
  - [2.4 Access Specifiers](#24-access-specifiers)
- [3. Object Creation and Usage](#3-object-creation-and-usage)
  - [3.1 Declaration](#31-declaration)
  - [3.2 Object Arrays](#32-object-arrays)
- [4. Constructors and Destructors](#4-constructors-and-destructors)
  - [4.1 Constructor](#41-constructor)
  - [4.2 Constructor Overloading](#42-constructor-overloading)
  - [4.3 Default Constructor](#43-default-constructor)
  - [4.4 Destructor](#44-destructor)
  - [4.5 Execution Order](#45-execution-order)
- [5. Inline Functions](#5-inline-functions)
- [6. C++ Struct](#6-c-struct)
- [7. Header and Source File Separation](#7-header-and-source-file-separation)
- [Summary](#summary)

---

<br>

## 1. Object-Oriented Concepts

### 1.1 Objects

In object-oriented programming (OOP), **everything is modeled as an object**. An object bundles together:

- **State** — data stored in **member variables** (fields)
- **Behavior** — operations defined by **member functions** (methods)

**Encapsulation** is the principle of hiding internal implementation details and exposing only a well-defined interface. The outside world interacts with an object through its public methods without knowing how the internal data is structured or modified.

```
┌───────────────────────────┐
│         Object            │
│                           │
│  ┌─────────────────────┐  │
│  │  State (private)     │  │
│  │  - member variables  │  │
│  └─────────────────────┘  │
│                           │
│  ┌─────────────────────┐  │
│  │  Behavior (public)   │  │
│  │  - member functions  │  │
│  └─────────────────────┘  │
└───────────────────────────┘
```

### 1.2 Class vs Object

| | Class | Object |
|:--|:------|:-------|
| What | Blueprint / mold | Instance created from a class |
| Memory | Not allocated in memory | Allocated in memory at creation |
| Analogy | Cookie cutter | Cookie |
| Quantity | One class definition | Multiple objects from one class |

Each object has its own **independent copy** of member variables. Changing one object's data does not affect another.

```cpp
class Circle {
    int radius;       // each Circle object has its own radius
public:
    double getArea();
};

Circle a, b;          // two independent objects from one class
```

---

<br>

## 2. Class Definition

### 2.1 Syntax

```cpp
class ClassName {
private:       // access specifier
    // private members
protected:
    // protected members
public:
    // public members
};             // ← semicolon required!
```

- The `class` keyword introduces a class definition.
- Members are grouped under **access specifiers** (`private`, `protected`, `public`).
- If no access specifier is written, the **default is `private`**.
- The closing brace must be followed by a **semicolon**.

### 2.2 Member Variables

Member variables store the **state** (data) of an object. They are typically declared `private` to enforce encapsulation.

```cpp
class Rectangle {
private:
    int width;
    int height;
};
```

### 2.3 Member Functions

Member functions define the **behavior** of an object. They are typically declared `public` so that external code can interact with the object.

A member function can be defined in two ways:

**Inside the class (implicitly inline):**

```cpp
class Circle {
    int radius;
public:
    double getArea() {
        return 3.14 * radius * radius;
    }
};
```

**Outside the class with the scope resolution operator `::`:**

```cpp
class Circle {
    int radius;
public:
    double getArea();
};

double Circle::getArea() {
    return 3.14 * radius * radius;
}
```

### 2.4 Access Specifiers

| Specifier | Accessible From | Typical Use |
|:----------|:----------------|:------------|
| `private` | Only within the same class | Member variables, internal helpers |
| `protected` | Same class + derived classes | Members shared with subclasses |
| `public` | Anywhere | Interface (member functions, getters) |

```cpp
class Account {
private:
    int balance;          // only Account's own methods can access
protected:
    int accountNumber;    // Account + derived classes can access
public:
    int getBalance() {    // anyone can call
        return balance;
    }
};
```

> **Key Point:** If you access a `private` member from outside the class, the compiler raises an error. This is how C++ enforces encapsulation at compile time.

---

<br>

## 3. Object Creation and Usage

### 3.1 Declaration

Declaring an object allocates memory for all its member variables. Use the **dot operator** (`.`) to access public members.

```cpp
Circle c;               // object created — memory allocated
c.setRadius(5);         // call member function via dot operator
double area = c.getArea();
```

> **Warning:** You cannot access `private` members directly from outside the class:
> ```cpp
> c.radius = 5;  // ❌ compile error — radius is private
> ```
> Use a public setter or pass the value through a constructor.

### 3.2 Object Arrays

An array of objects creates multiple independent instances. Each element is a separate object.

```cpp
Circle circles[3];     // 3 Circle objects, each with its own radius

circles[0].setRadius(1);
circles[1].setRadius(2);
circles[2].setRadius(3);
```

The **default constructor** is called for each element when the array is created.

---

<br>

## 4. Constructors and Destructors

### 4.1 Constructor

A **constructor** is a special member function that initializes an object at creation time.

- Has the **same name** as the class.
- Has **no return type** (not even `void`).
- Called **automatically** when an object is created.
- Can be **overloaded** (multiple constructors with different parameters).

```cpp
class Circle {
    int radius;
public:
    Circle() {              // default constructor (no parameters)
        radius = 1;
    }
    Circle(int r) {         // parameterized constructor
        radius = r;
    }
    double getArea() {
        return 3.14 * radius * radius;
    }
};

Circle a;        // calls Circle() → radius = 1
Circle b(10);    // calls Circle(int) → radius = 10
```

### 4.2 Constructor Overloading

Multiple constructors can coexist as long as their **parameter lists differ** (number or types of parameters).

```cpp
class Point {
    int x, y;
public:
    Point() { x = 0; y = 0; }
    Point(int a) { x = a; y = a; }
    Point(int a, int b) { x = a; y = b; }
};

Point p1;         // (0, 0)
Point p2(5);      // (5, 5)
Point p3(3, 7);   // (3, 7)
```

### 4.3 Default Constructor

If **no constructor is defined** by the programmer, the compiler automatically provides a **default constructor** that does nothing (leaves member variables uninitialized).

However, once **any constructor is defined**, the compiler **does NOT** provide a default constructor. If you still need one, you must define it explicitly.

```cpp
class Box {
    int size;
public:
    Box(int s) { size = s; }   // only parameterized constructor
};

Box a;       // ❌ compile error — no default constructor!
Box b(10);   // ✅ OK
```

> **Key Point:** If you define `Box(int s)` but also need `Box()`, you must write both.

### 4.4 Destructor

A **destructor** is called automatically when an object is destroyed (goes out of scope or is `delete`d).

- Name: `~ClassName()`
- No parameters, no return type.
- **Cannot be overloaded** (only one destructor per class).
- Used for **cleanup** (releasing dynamically allocated memory, closing files, etc.).

```cpp
class MyArray {
    int *data;
public:
    MyArray(int size) {
        data = new int[size];
    }
    ~MyArray() {
        delete[] data;    // cleanup: free heap memory
    }
};
```

### 4.5 Execution Order

- Objects are **constructed** in the order they are declared.
- Objects are **destructed in reverse order** (last created, first destroyed).
- Local objects are destroyed when the enclosing **scope exits**.

```cpp
void func() {
    Circle a(1);     // 1st constructed
    Circle b(2);     // 2nd constructed
}                    // b destroyed first, then a
```

```
Construction order: a → b
Destruction order:  b → a
```

---

<br>

## 5. Inline Functions

An **inline function** is a suggestion to the compiler to replace the function call with the function body at the call site, avoiding function call overhead.

```cpp
inline int square(int n) {
    return n * n;
}
```

- Use `inline` for **small, frequently called** functions.
- The compiler may ignore the `inline` hint for large or complex functions.
- Member functions **defined inside the class body** are implicitly inline.

```cpp
class Circle {
    int radius;
public:
    // implicitly inline — defined inside the class
    double getArea() { return 3.14 * radius * radius; }
};
```

> **Note:** `inline` is a suggestion, not a command. Modern compilers often inline functions automatically when beneficial, regardless of the keyword.

---

<br>

## 6. C++ Struct

In C++, `struct` is almost identical to `class`. The **only difference** is the default access level.

| Feature | `struct` | `class` |
|:--------|:--------:|:-------:|
| Default access | `public` | `private` |
| Can have member functions | Yes | Yes |
| Can have constructors/destructors | Yes | Yes |
| Can use inheritance | Yes | Yes |

```cpp
struct Point {
    int x, y;        // public by default
    void print() {
        cout << x << ", " << y << endl;
    }
};

Point p;
p.x = 3;            // ✅ OK — public by default
```

> **Convention:** Use `struct` for simple data holders with mostly public members. Use `class` when encapsulation and access control are important.

---

<br>

## 7. Header and Source File Separation

For larger projects, class **declarations** go in a header file (`.h`) and member function **definitions** go in a source file (`.cpp`).

**`Circle.h`** — class declaration:

```cpp
#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
    int radius;
public:
    Circle();
    Circle(int r);
    double getArea();
};

#endif
```

**`Circle.cpp`** — member function definitions:

```cpp
#include "Circle.h"

Circle::Circle() {
    radius = 1;
}

Circle::Circle(int r) {
    radius = r;
}

double Circle::getArea() {
    return 3.14 * radius * radius;
}
```

**Include guards** prevent the same header from being included multiple times:

| Method | Syntax |
|:-------|:-------|
| Traditional | `#ifndef CIRCLE_H` / `#define CIRCLE_H` / `#endif` |
| Modern | `#pragma once` (widely supported, simpler) |

> **Tip:** `#pragma once` is not part of the C++ standard but is supported by all major compilers (GCC, Clang, MSVC).

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Object | Bundles state (member variables) and behavior (member functions) |
| Encapsulation | Hide internals (`private`), expose interface (`public`) |
| Class vs Object | Class = blueprint (no memory); Object = instance (allocated in memory) |
| Class syntax | `class Name { ... };` — semicolon required; default access is `private` |
| Member variables | Store object state; typically `private` |
| Member functions | Define object behavior; can be defined inside or outside class (`::`) |
| Access specifiers | `private` (class only), `protected` (class + derived), `public` (anywhere) |
| Object creation | `ClassName obj;` allocates memory; use dot operator for access |
| Object arrays | `ClassName arr[n];` — each element is independent; default constructor called |
| Constructor | Same name as class, no return type, called automatically, can be overloaded |
| Default constructor | Compiler provides one only if NO constructor is defined |
| Destructor | `~ClassName()`, no params, no overload, called when object destroyed |
| Execution order | Constructed in declaration order; destructed in reverse |
| Inline functions | `inline` or defined inside class; compiler replaces call with body |
| C++ `struct` | Same as `class` but default access is `public` |
| File separation | `.h` for declaration, `.cpp` for definition; use include guards |

---
