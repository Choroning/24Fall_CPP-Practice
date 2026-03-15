# Chapter 09 — Virtual Functions and Abstract Classes

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpoom C++ Programming (Revised Edition)* — Chapter 9

---

## Table of Contents

- [1. Upcasting and Downcasting](#1-upcasting-and-downcasting)
  - [1.1 Upcasting](#11-upcasting)
  - [1.2 Downcasting](#12-downcasting)
- [2. Virtual Functions](#2-virtual-functions)
  - [2.1 Problem Without virtual](#21-problem-without-virtual)
  - [2.2 virtual Keyword](#22-virtual-keyword)
  - [2.3 How It Works](#23-how-it-works)
  - [2.4 Override Keyword](#24-override-keyword)
- [3. Virtual Destructors](#3-virtual-destructors)
  - [3.1 Problem](#31-problem)
  - [3.2 Rule](#32-rule)
- [4. Abstract Classes](#4-abstract-classes)
  - [4.1 Pure Virtual Function](#41-pure-virtual-function)
  - [4.2 Abstract Class](#42-abstract-class)
  - [4.3 Concrete Class](#43-concrete-class)
- [5. Polymorphism](#5-polymorphism)
  - [5.1 Concept](#51-concept)
  - [5.2 Array of Base Pointers](#52-array-of-base-pointers)
  - [5.3 Practical Example](#53-practical-example)
- [Summary](#summary)

---

<br>

## 1. Upcasting and Downcasting

### 1.1 Upcasting

**Upcasting** converts a derived class pointer (or reference) to a base class pointer (or reference). It is **implicit** and always safe because every derived object **is-a** base object.

```cpp
class Base { };
class Derived : public Base { };

Derived d;
Base* p = &d;       // upcasting — implicit, always safe
Base& r = d;        // upcasting with reference
```

Through the base pointer `p`, **only base class members are accessible**. Even though the actual object is `Derived`, the compiler sees the `Base*` type and restricts access to `Base`'s interface.

```cpp
class Shape {
public:
    void draw() { cout << "Shape" << endl; }
};

class Circle : public Shape {
public:
    void draw() { cout << "Circle" << endl; }
    double getRadius() { return 5.0; }
};

Circle c;
Shape* p = &c;      // upcasting
p->draw();           // "Shape" — calls Shape::draw() (static binding)
// p->getRadius();   // compile error — Shape has no getRadius()
```

> **Key Point:** After upcasting, the pointer type determines which members are visible. The derived-only members are hidden from the base pointer.

### 1.2 Downcasting

**Downcasting** converts a base class pointer to a derived class pointer. It requires an **explicit cast** and is **dangerous** if the actual object is not the target derived type.

```cpp
Base* p = new Derived;
Derived* dp = (Derived*)p;   // C-style downcast — compiles, but risky
```

Use `dynamic_cast` for **safe downcasting**. It performs a runtime check and returns `nullptr` if the cast is invalid (requires at least one virtual function in the base class).

```cpp
Base* p = new Base;
Derived* dp = dynamic_cast<Derived*>(p);  // runtime check
if (dp == nullptr) {
    cout << "Cast failed — p does not point to Derived" << endl;
}
```

> **Warning:** Downcasting with `static_cast` or C-style casts bypasses runtime checks. If the actual object is not the derived type, accessing derived members causes **undefined behavior**.

---

<br>

## 2. Virtual Functions

### 2.1 Problem Without virtual

Without `virtual`, the function that gets called is determined by the **pointer type**, not the actual object type. This is called **static binding** (or early binding) — the decision is made at compile time.

```cpp
class Base {
public:
    void func() { cout << "Base::func()" << endl; }
};

class Derived : public Base {
public:
    void func() { cout << "Derived::func()" << endl; }
};

Base* p = new Derived;
p->func();    // Output: "Base::func()" — static binding!
delete p;
```

The pointer type is `Base*`, so `Base::func()` is called even though `p` points to a `Derived` object. This defeats the purpose of polymorphism.

### 2.2 virtual Keyword

Adding `virtual` to the base class function enables **dynamic binding** (or late binding). At runtime, the actual object type determines which function is called.

```cpp
class Base {
public:
    virtual void func() { cout << "Base::func()" << endl; }
};

class Derived : public Base {
public:
    void func() { cout << "Derived::func()" << endl; }  // overrides
};

Base* p = new Derived;
p->func();    // Output: "Derived::func()" — dynamic binding!
delete p;
```

Once a function is declared `virtual` in the base class, it remains virtual in all derived classes — even without explicitly writing `virtual` again.

### 2.3 How It Works

C++ implements dynamic binding through the **vtable** (virtual function table) mechanism:

1. **vtable** — Each class that has virtual functions gets a table of function pointers. The table maps each virtual function to the correct implementation for that class.
2. **vptr** — Each object of a class with virtual functions contains a hidden pointer (`vptr`) that points to its class's vtable.
3. **Runtime lookup** — When a virtual function is called through a base pointer, the program follows the `vptr` to the vtable, looks up the function, and calls the correct version.

```
┌────────────────┐         ┌─────────────────────┐
│  Base object   │         │   Base vtable        │
│  ┌──────────┐  │         │   func → Base::func  │
│  │  vptr   ─┼──┼────────▶│                      │
│  └──────────┘  │         └─────────────────────┘
└────────────────┘

┌────────────────┐         ┌──────────────────────────┐
│ Derived object │         │   Derived vtable          │
│  ┌──────────┐  │         │   func → Derived::func    │
│  │  vptr   ─┼──┼────────▶│                           │
│  └──────────┘  │         └──────────────────────────┘
└────────────────┘
```

> **Note:** The vtable mechanism adds a small overhead — one pointer per object and one indirect call per virtual function invocation. This is the cost of runtime polymorphism.

### 2.4 Override Keyword

C++11 introduced the `override` keyword. Placing `override` on a derived function tells the compiler to verify that the base class has a matching virtual function. This catches typos and signature mismatches at compile time.

```cpp
class Base {
public:
    virtual void func(int x) { }
};

class Derived : public Base {
public:
    void func(int x) override { }    // OK — matches Base::func(int)
    // void func(double x) override; // compile error — no matching base function
    // void Fun(int x) override;     // compile error — typo in name
};
```

> **Best Practice:** Always use `override` when overriding virtual functions. It is a compile-time safety net at zero runtime cost.

---

<br>

## 3. Virtual Destructors

### 3.1 Problem

When deleting a derived object through a base pointer, if the destructor is **not virtual**, only the base class destructor is called. The derived class destructor is skipped, potentially **leaking resources** allocated by the derived class.

```cpp
class Base {
public:
    ~Base() { cout << "~Base" << endl; }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; cout << "~Derived" << endl; }
};

Base* p = new Derived;
delete p;    // Output: "~Base" only! ~Derived never called → memory leak
```

### 3.2 Rule

If a class has **any virtual function**, its destructor **must be virtual**. This ensures that `delete` through a base pointer correctly calls the derived destructor first, then the base destructor.

```cpp
class Base {
public:
    virtual ~Base() { cout << "~Base" << endl; }
    virtual void func() { }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; cout << "~Derived" << endl; }
};

Base* p = new Derived;
delete p;    // Output: "~Derived" then "~Base" — correct!
```

> **Rule of Thumb:** Any class intended to be used as a base class with virtual functions should have a `virtual` destructor. This is one of the most important C++ best practices.

---

<br>

## 4. Abstract Classes

### 4.1 Pure Virtual Function

A **pure virtual function** has no implementation in the base class. It is declared by appending `= 0` to the function declaration. Derived classes **must** provide an implementation.

```cpp
class Shape {
public:
    virtual double getArea() = 0;   // pure virtual — no body
    virtual void draw() = 0;        // pure virtual
};
```

### 4.2 Abstract Class

A class that contains **at least one pure virtual function** is an **abstract class**. An abstract class:

- **Cannot be instantiated** — you cannot create objects of it directly.
- **Can have pointers and references** — `Shape* p;` is valid.
- **Can have non-pure member functions** — it may provide partial implementations.
- **Serves as an interface/contract** — it defines what derived classes must implement.

```cpp
// Shape s;           // compile error — Shape is abstract
Shape* p;             // OK — pointer to abstract class
Shape& r = *p;        // OK — reference to abstract class
```

### 4.3 Concrete Class

A **concrete class** is a derived class that implements **all** pure virtual functions inherited from the abstract base. Only concrete classes can be instantiated.

```cpp
class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) { }
    double getArea() override { return 3.14159 * radius * radius; }
    void draw() override { cout << "Drawing Circle" << endl; }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) { }
    double getArea() override { return width * height; }
    void draw() override { cout << "Drawing Rectangle" << endl; }
};

Circle c(5.0);        // OK — Circle is concrete
Rectangle r(3.0, 4.0); // OK — Rectangle is concrete
```

> **Key Point:** If a derived class does not override all pure virtual functions, it is still abstract and cannot be instantiated.

---

<br>

## 5. Polymorphism

### 5.1 Concept

**Polymorphism** means "one interface, multiple implementations." A base class pointer can call different derived class implementations depending on the actual object type at runtime.

```
        Base* p
          │
     ┌────┼────┐
     ▼    ▼    ▼
  DerivedA  DerivedB  DerivedC
  func()    func()    func()
```

Through `p->func()`, the correct derived version is called based on what `p` actually points to — not the pointer type.

### 5.2 Array of Base Pointers

A common polymorphism pattern is creating an **array of base class pointers**, each pointing to a different derived object. A single loop can process all objects uniformly.

```cpp
Shape* arr[3];
arr[0] = new Circle(5.0);
arr[1] = new Rectangle(3.0, 4.0);
arr[2] = new Circle(2.5);

for (int i = 0; i < 3; i++) {
    cout << "Area: " << arr[i]->getArea() << endl;  // calls correct version
    arr[i]->draw();                                   // calls correct version
}

for (int i = 0; i < 3; i++) {
    delete arr[i];   // virtual destructor ensures proper cleanup
}
```

Output:

```text
Area: 78.5398
Drawing Circle
Area: 12
Drawing Rectangle
Area: 19.6350
Drawing Circle
```

### 5.3 Practical Example

A unit conversion system using abstract classes and polymorphism. The abstract base `Converter` defines the interface, and each derived class implements a specific conversion.

```cpp
#include <iostream>
using namespace std;

class Converter {
protected:
    double before, after;
public:
    Converter(double val) : before(val), after(0) { }
    virtual void compute() = 0;    // pure virtual
    virtual void display() = 0;    // pure virtual
    virtual ~Converter() { }
};

class LiterToGallon : public Converter {
public:
    LiterToGallon(double val) : Converter(val) { }
    void compute() override { after = before / 3.7854; }
    void display() override {
        cout << before << " liters = " << after << " gallons" << endl;
    }
};

class CelsiusToFahrenheit : public Converter {
public:
    CelsiusToFahrenheit(double val) : Converter(val) { }
    void compute() override { after = before * 9.0 / 5.0 + 32; }
    void display() override {
        cout << before << " C = " << after << " F" << endl;
    }
};

int main() {
    Converter* conv[2];
    conv[0] = new LiterToGallon(10.0);
    conv[1] = new CelsiusToFahrenheit(100.0);

    for (int i = 0; i < 2; i++) {
        conv[i]->compute();
        conv[i]->display();
    }

    for (int i = 0; i < 2; i++) {
        delete conv[i];
    }

    return 0;
}
```

Output:

```text
10 liters = 2.64172 gallons
100 C = 212 F
```

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Upcasting | Derived pointer/reference to base; implicit and safe; only base members visible |
| Downcasting | Base pointer to derived; explicit cast required; use `dynamic_cast` for safety |
| Static binding | Without `virtual`, pointer type determines which function is called (compile time) |
| `virtual` keyword | Enables dynamic binding; actual object type determines which function runs (runtime) |
| vtable / vptr | Each class has a vtable of function pointers; each object has a vptr to its vtable |
| `override` (C++11) | Compiler verifies base has matching virtual function; catches typos and mismatches |
| Virtual destructor | Required if class has any virtual function; ensures correct cleanup through base pointer |
| Pure virtual function | `virtual void func() = 0;` — no body; derived classes must implement |
| Abstract class | Has at least one pure virtual function; cannot be instantiated; serves as interface |
| Concrete class | Implements all pure virtual functions; can be instantiated |
| Polymorphism | One interface, multiple implementations; base pointer calls correct derived version |
| Array of base pointers | Common pattern: `Base* arr[n];` — process different derived objects in a single loop |

---
