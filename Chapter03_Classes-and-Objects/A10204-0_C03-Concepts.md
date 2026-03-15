# Chapter 03 — Classes and Objects

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpoom C++ Programming (Revised Edition)* — Chapter 3

---

## Table of Contents

- [1. Object-Oriented Concepts](#1-object-oriented-concepts)
  - [1.1 Objects](#11-objects)
  - [1.2 Class vs Object](#12-class-vs-object)
  - [1.3 From C to C++ — struct vs class](#13-from-c-to-c--struct-vs-class)
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
- [8. Complete Runnable Examples](#8-complete-runnable-examples)
  - [8.1 Circle Class with Constructor, Getter, and Area](#81-circle-class-with-constructor-getter-and-area)
  - [8.2 Constructor and Destructor Execution Order](#82-constructor-and-destructor-execution-order)
  - [8.3 Member Functions — Inside vs Outside the Class](#83-member-functions--inside-vs-outside-the-class)
- [9. Common Mistakes](#9-common-mistakes)
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
│          Object           │
│                           │
│  ┌─────────────────────┐  │
│  │  State (private)    │  │
│  │  - member variables │  │
│  └─────────────────────┘  │
│                           │
│  ┌─────────────────────┐  │
│  │  Behavior (public)  │  │
│  │  - member functions │  │
│  └─────────────────────┘  │
└───────────────────────────┘
```

#### Why Does Encapsulation Matter?

Without encapsulation, anyone can set data to **invalid values**, and there is nothing to prevent bugs:

```cpp
// BAD: radius is public — no protection
class Circle {
public:
    int radius;
    double getArea() { return 3.14 * radius * radius; }
};

Circle c;
c.radius = -5;             // no one stops this!
cout << c.getArea();        // prints 78.5 — a "negative" radius gives a positive
                            // but meaningless area. The object is in an invalid state.
```

With encapsulation, the class **validates data** through a setter, preventing invalid states:

```cpp
// GOOD: radius is private — setter enforces rules
class Circle {
    int radius;
public:
    void setRadius(int r) {
        if (r > 0)
            radius = r;     // only accept positive values
        else
            radius = 1;     // fallback to a safe default
    }
    double getArea() { return 3.14 * radius * radius; }
};

Circle c;
c.setRadius(-5);            // setter rejects -5, sets radius to 1
cout << c.getArea();         // prints 3.14 — always a valid result
```

#### Practical Analogy — Bank Account

Think of a **bank account**. The balance is private data. You cannot walk into the bank vault and change the number on a ledger directly. Instead, you must use **deposit** and **withdraw** operations, which check whether the transaction is valid (e.g., you cannot withdraw more than your balance).

```cpp
class BankAccount {
    int balance;               // private — cannot be touched directly
public:
    BankAccount() { balance = 0; }
    void deposit(int amount) {
        if (amount > 0)        // validation: only positive deposits
            balance += amount;
    }
    bool withdraw(int amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;       // success
        }
        return false;          // failed: invalid amount or insufficient funds
    }
    int getBalance() { return balance; }
};
```

This is encapsulation in action: the **data is protected**, and all modifications go through **controlled, validated methods**.

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

### 1.3 From C to C++ — struct vs class

If you learned C first, you are used to `struct` as a plain data container with no member functions and no access control. In C++, `class` takes the struct concept much further.

| Feature | C `struct` | C++ `class` |
|:--------|:-----------|:------------|
| Data members | Yes | Yes |
| Member functions | No (use separate functions) | Yes (bundled inside) |
| Access control (`private`/`public`) | No (everything public) | Yes (default `private`) |
| Constructors / Destructors | No | Yes |
| Encapsulation | Manual discipline only | Enforced by the compiler |

**Side-by-side comparison — the same Circle problem in C vs C++:**

**C approach (struct + separate functions):**

```c
#include <stdio.h>

struct Circle {
    int radius;          /* all data is public — no way to restrict */
};

double getArea(struct Circle c) {    /* function is separate from struct */
    return 3.14 * c.radius * c.radius;
}

int main() {
    struct Circle c;
    c.radius = 5;        /* direct access — no validation possible */
    printf("Area: %.2f\n", getArea(c));

    c.radius = -3;       /* nothing prevents this! */
    printf("Area: %.2f\n", getArea(c));
    return 0;
}
```

**C++ approach (class with encapsulation):**

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;                      // private by default
public:
    Circle(int r) {                  // constructor initializes the object
        radius = (r > 0) ? r : 1;
    }
    double getArea() {               // member function — bundled with data
        return 3.14 * radius * radius;
    }
};

int main() {
    Circle c(5);                     // initialized via constructor
    cout << "Area: " << c.getArea() << endl;

    // c.radius = -3;               // compile error! radius is private
    return 0;
}
```

> **Key Insight:** In C, keeping data valid depends entirely on programmer discipline. In C++, the compiler enforces it through access control and constructors, catching mistakes at compile time instead of runtime.

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

#### Why Are Constructors Needed?

Without a constructor, an object's member variables contain **garbage values** (whatever was previously in that memory). Using an uninitialized object leads to unpredictable behavior:

```cpp
class Circle {
    int radius;    // no constructor defined
public:
    double getArea() { return 3.14 * radius * radius; }
};

Circle c;
cout << c.getArea();   // radius is garbage (e.g., -858993460) → nonsensical result!
                       // The program compiles and runs, but the output is wrong.
```

A constructor **guarantees** that every object starts in a valid state. The programmer never has to remember to "call an init function" — initialization is automatic.

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

## 8. Complete Runnable Examples

The programs below are self-contained. Copy any of them into a `.cpp` file, compile with `g++ -o program filename.cpp`, and run with `./program`.

### 8.1 Circle Class with Constructor, Getter, and Area

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() {
        radius = 1;
    }
    Circle(int r) {
        radius = (r > 0) ? r : 1;    // validate: reject non-positive values
    }
    int getRadius() {
        return radius;
    }
    void setRadius(int r) {
        if (r > 0)
            radius = r;
    }
    double getArea() {
        return 3.14 * radius * radius;
    }
};

int main() {
    Circle c1;          // default constructor → radius = 1
    Circle c2(5);       // parameterized constructor → radius = 5
    Circle c3(-3);      // invalid value → constructor sets radius = 1

    cout << "c1: radius=" << c1.getRadius() << ", area=" << c1.getArea() << endl;
    cout << "c2: radius=" << c2.getRadius() << ", area=" << c2.getArea() << endl;
    cout << "c3: radius=" << c3.getRadius() << ", area=" << c3.getArea() << endl;

    c2.setRadius(10);
    cout << "c2 after setRadius(10): area=" << c2.getArea() << endl;

    return 0;
}
```

**Expected output:**

```
c1: radius=1, area=3.14
c2: radius=5, area=78.5
c3: radius=1, area=3.14
c2 after setRadius(10): area=314
```

### 8.2 Constructor and Destructor Execution Order

```cpp
#include <iostream>
using namespace std;

class Tracer {
    string name;
public:
    Tracer(string n) : name(n) {
        cout << "  Constructor: " << name << " created" << endl;
    }
    ~Tracer() {
        cout << "  Destructor:  " << name << " destroyed" << endl;
    }
};

void innerFunction() {
    cout << "[Entering innerFunction]" << endl;
    Tracer t3("C");
    Tracer t4("D");
    cout << "[Leaving innerFunction]" << endl;
}

int main() {
    cout << "[Entering main]" << endl;
    Tracer t1("A");
    Tracer t2("B");

    innerFunction();

    cout << "[Leaving main]" << endl;
    return 0;
}
```

**Expected output:**

```
[Entering main]
  Constructor: A created
  Constructor: B created
[Entering innerFunction]
  Constructor: C created
  Constructor: D created
[Leaving innerFunction]
  Destructor:  D destroyed
  Destructor:  C destroyed
[Leaving main]
  Destructor:  B destroyed
  Destructor:  A destroyed
```

Notice: within each scope, destruction happens in **reverse order** of construction.

### 8.3 Member Functions — Inside vs Outside the Class

```cpp
#include <iostream>
using namespace std;

class Rectangle {
    int width, height;
public:
    Rectangle(int w, int h) {
        width = w;
        height = h;
    }

    // Defined INSIDE the class (implicitly inline)
    int getWidth() { return width; }
    int getHeight() { return height; }

    // Only DECLARED here — defined outside below
    int getArea();
    void print();
};

// Defined OUTSIDE the class using scope resolution operator ::
int Rectangle::getArea() {
    return width * height;
}

void Rectangle::print() {
    cout << width << " x " << height << " = " << getArea() << endl;
}

int main() {
    Rectangle r1(3, 4);
    Rectangle r2(10, 5);

    r1.print();     // 3 x 4 = 12
    r2.print();     // 10 x 5 = 50

    return 0;
}
```

**Expected output:**

```
3 x 4 = 12
10 x 5 = 50
```

> **When to use which?** Short one-liner functions (getters/setters) are commonly defined inside the class. Longer functions are defined outside to keep the class declaration readable.

---

<br>

## 9. Common Mistakes

This section lists the most frequent errors beginners make with classes. Knowing these in advance will save you debugging time.

### 9.1 Forgetting the Semicolon After the Class Definition

```cpp
class Circle {
    int radius;
public:
    double getArea() { return 3.14 * radius * radius; }
}           // WRONG: missing semicolon

int main() { ... }
```

The compiler will produce confusing errors that point to lines **after** the class definition. Always end with `};`.

```cpp
class Circle {
    int radius;
public:
    double getArea() { return 3.14 * radius * radius; }
};          // CORRECT
```

### 9.2 Accessing Private Members from Outside the Class

```cpp
Circle c;
c.radius = 5;   // WRONG: compile error — radius is private
```

**Fix:** Use a public setter or pass the value through a constructor.

```cpp
Circle c(5);           // pass via constructor
// or
c.setRadius(5);        // pass via public setter
```

### 9.3 No Default Constructor After Defining Another Constructor

Once you define **any** constructor, the compiler-generated default constructor disappears.

```cpp
class Box {
    int size;
public:
    Box(int s) { size = s; }
};

Box b;       // WRONG: compile error — no default constructor
```

**Fix:** Explicitly define the default constructor if you need it.

```cpp
class Box {
    int size;
public:
    Box() { size = 0; }           // now this works
    Box(int s) { size = s; }
};

Box b;       // OK — calls Box()
```

### 9.4 Calling the Constructor Like a Regular Function

Constructors are called **only at object creation**. You cannot call them again on an existing object.

```cpp
Circle c;
c.Circle(5);    // WRONG: you cannot call a constructor like this
```

**Fix:** Initialize at creation time, or use a setter to change the value later.

```cpp
Circle c(5);         // CORRECT: constructor called at creation
// or later:
c.setRadius(10);     // CORRECT: use a setter to modify
```

### 9.5 Confusing Class Definition with Object Creation

A class definition does **not** create any object. No memory is allocated until you declare a variable of that class type.

```cpp
class Circle {
    int radius;
public:
    Circle(int r) { radius = r; }
};
// No Circle object exists yet!

Circle c(5);    // NOW an object is created and memory is allocated
```

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
