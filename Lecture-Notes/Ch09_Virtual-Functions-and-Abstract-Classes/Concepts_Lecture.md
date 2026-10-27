# Chapter 9 — Virtual Functions and Abstract Classes

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 9

> **Learning Objectives:**
> 1. Distinguish function redefinition (hiding) from overriding in an inheritance hierarchy
> 2. Explain how virtual functions produce dynamic binding through the vtable and vptr
> 3. Explain why virtual destructors are needed and declare them correctly
> 4. Define an abstract class with pure virtual functions and implement concrete classes that inherit from it
> 5. Write polymorphic programs with arrays of base class pointers and virtual functions

---

## Table of Contents

- [1. Upcasting and Downcasting](#1-upcasting-and-downcasting)
  - [1.1 Upcasting](#11-upcasting)
  - [1.2 Downcasting](#12-downcasting)
- [2. Virtual Functions](#2-virtual-functions)
  - [2.1 Why Virtual Functions?](#21-why-virtual-functions)
  - [2.2 Problem Without virtual](#22-problem-without-virtual)
  - [2.3 virtual Keyword](#23-virtual-keyword)
  - [2.4 Static vs Dynamic Binding](#24-static-vs-dynamic-binding)
  - [2.5 How It Works: vtable and vptr](#25-how-it-works-vtable-and-vptr)
  - [2.6 Override Keyword](#26-override-keyword)
  - [2.7 Conditions for Successful Overriding](#27-conditions-for-successful-overriding)
  - [2.8 Inheritance of Virtual Functions](#28-inheritance-of-virtual-functions)
  - [2.9 The final Specifier](#29-the-final-specifier)
  - [2.10 Calling a Base Class Virtual Function with the Scope Resolution Operator](#210-calling-a-base-class-virtual-function-with-the-scope-resolution-operator)
- [3. Virtual Destructors](#3-virtual-destructors)
  - [3.1 Problem](#31-problem)
  - [3.2 Complete Example: The Memory Leak](#32-complete-example-the-memory-leak)
  - [3.3 Rule](#33-rule)
- [4. Abstract Classes](#4-abstract-classes)
  - [4.1 Pure Virtual Function](#41-pure-virtual-function)
  - [4.2 Abstract Class](#42-abstract-class)
  - [4.3 Concrete Class](#43-concrete-class)
  - [4.4 Abstract Class Implementation Examples](#44-abstract-class-implementation-examples)
- [5. Polymorphism](#5-polymorphism)
  - [5.1 Concept](#51-concept)
  - [5.2 Complete Shape Hierarchy Example](#52-complete-shape-hierarchy-example)
  - [5.3 Complete Converter Example](#53-complete-converter-example)
- [6. Common Mistakes](#6-common-mistakes)
  - [6.1 Forgetting Virtual Destructor](#61-forgetting-virtual-destructor)
  - [6.2 Trying to Instantiate an Abstract Class](#62-trying-to-instantiate-an-abstract-class)
  - [6.3 Forgetting to Override All Pure Virtual Functions](#63-forgetting-to-override-all-pure-virtual-functions)
  - [6.4 Object Slicing](#64-object-slicing)
  - [6.5 Overriding vs Overloading vs Hiding](#65-overriding-vs-overloading-vs-hiding)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Upcasting and Downcasting

### 1.1 Upcasting

**Upcasting** converts a derived class pointer (or reference) to a base class pointer (or reference). It is **implicit** and always **safe** because every derived object **is-a** base object.

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

Use `dynamic_cast` for **safe downcasting**. It performs a runtime check and returns `nullptr` if a pointer cast is invalid (requires at least one virtual function in the base class). A failed cast to a reference throws a `std::bad_cast` exception.

```cpp
Base* p = new Base;
Derived* dp = dynamic_cast<Derived*>(p);  // runtime check
if (dp == nullptr) {
    cout << "Cast failed — p does not point to Derived" << endl;
}
```

> **Caution:** Downcasting with `static_cast` or C-style casts bypasses runtime checks. If the actual object is not the derived type, accessing derived members causes **undefined behavior**.

---

<br>

## 2. Virtual Functions

### 2.1 Why Virtual Functions?

Consider a drawing program as an example. There is a `Shape` base class and derived classes like `Circle`, `Rectangle`, and `Triangle`. All shapes are stored in a single array of `Shape*` pointers, and the goal is to draw them all in one loop:

```cpp
Shape* canvas[100];
canvas[0] = new Circle(10, 20, 5);
canvas[1] = new Rectangle(0, 0, 30, 40);
canvas[2] = new Triangle(5, 5, 10, 20, 15, 5);
// ...

for (int i = 0; i < count; i++) {
    canvas[i]->draw();   // Which draw() gets called?
}
```

**Without virtual:** Every call to `canvas[i]->draw()` calls `Shape::draw()`, because the compiler only sees the `Shape*` type. It does not matter that the object is actually a `Circle`; the compiler already decided at compile time to call `Shape::draw()`. Every shape draws the same thing, so the program cannot achieve its purpose of drawing each shape differently.

**With virtual:** Every call to `canvas[i]->draw()` calls the **correct** `draw()` for the actual object: `Circle::draw()` for circles, `Rectangle::draw()` for rectangles, and so on. The decision happens at runtime based on the actual object type.

This is **the core of OOP polymorphism**: writing code that operates on base class pointers but automatically does the right thing for each derived type. Without virtual functions, inheritance is just code sharing; with virtual functions, inheritance enables **substitutability**.

### 2.2 Problem Without virtual

Without `virtual`, the function that gets called is determined by the **pointer type**, not the actual object type. This is called **static binding** (or **early binding**), and the decision is made at compile time.

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

The textbook calls this rewriting of a non-virtual base class function with the same prototype in a derived class **function redefinition (redefine)**. The derived object then contains both `func()` functions: calling through a `Derived*` runs `Derived::func()`, and calling through a `Base*` runs `Base::func()`. Section 6.5 treats this as hiding.

### 2.3 virtual Keyword

Adding `virtual` to the base class function enables **dynamic binding** (or **late binding**). At runtime, the actual object type determines which function is called.

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

Once a function is declared `virtual` in the base class, it remains virtual in all derived classes, even without explicitly writing `virtual` again.

Defining a function in a derived class with the same prototype as a base class virtual function in this way is called **function overriding**, and it is one form of polymorphism. After overriding, `Base::func()` still exists inside the derived object, but calling through a base class pointer always runs `Derived::func()`, so the base class function loses its presence. To call the base class version on a derived object, use the scope resolution operator of Section 2.10.

### 2.4 Static vs Dynamic Binding

Understanding the difference between static and dynamic binding is essential. The two differ in **when** the function to call is decided.

**Static Binding (Compile-Time Binding)**
- The compiler decides which function to call based on the **pointer/reference type**.
- This is the default for non-virtual functions.
- Fast (no runtime lookup), but ignores the actual object type.

**Dynamic Binding (Runtime Binding)**
- The program decides which function to call at runtime based on the **actual object type**.
- Activated by the `virtual` keyword.
- Slightly slower (vtable lookup), but enables true polymorphism.

The `virtual` keyword is the switch that changes a function from static binding to dynamic binding.

> **[Programming Languages]** In programming languages, binding is the association of a name with the entity it denotes, and bindings are classified by the time at which they are made (binding time) into static bindings, fixed at compile time, and dynamic bindings, made during execution. In a C++ member function call, this distinction applies to the time at which the called name is associated with the function body to run: a non-virtual member function is bound at compile time by the declared type of the pointer, and a virtual function is bound at run time by the actual type of the object.

**Side-by-Side Comparison (Same Code, Different Behavior):**

```cpp
#include <iostream>
using namespace std;

// ============ WITHOUT virtual (static binding) ============
class AnimalStatic {
public:
    void speak() { cout << "..." << endl; }
};
class DogStatic : public AnimalStatic {
public:
    void speak() { cout << "Woof!" << endl; }
};
class CatStatic : public AnimalStatic {
public:
    void speak() { cout << "Meow!" << endl; }
};

// ============ WITH virtual (dynamic binding) ============
class AnimalDynamic {
public:
    virtual void speak() { cout << "..." << endl; }
    virtual ~AnimalDynamic() { }
};
class DogDynamic : public AnimalDynamic {
public:
    void speak() override { cout << "Woof!" << endl; }
};
class CatDynamic : public AnimalDynamic {
public:
    void speak() override { cout << "Meow!" << endl; }
};

int main() {
    // --- Static binding ---
    AnimalStatic* s1 = new DogStatic;
    AnimalStatic* s2 = new CatStatic;
    s1->speak();   // "..."  (wrong! calls AnimalStatic::speak)
    s2->speak();   // "..."  (wrong! calls AnimalStatic::speak)
    delete s1; delete s2;

    // --- Dynamic binding ---
    AnimalDynamic* d1 = new DogDynamic;
    AnimalDynamic* d2 = new CatDynamic;
    d1->speak();   // "Woof!"  (correct! calls DogDynamic::speak)
    d2->speak();   // "Meow!"  (correct! calls CatDynamic::speak)
    delete d1; delete d2;

    return 0;
}
```

Output:

```text
...
...
Woof!
Meow!
```

> **Key Point:** The only difference is the `virtual` keyword on the base class. Everything else, including the inheritance, the pointer types, and the function calls, is identical. `virtual` is what makes polymorphism work.

### 2.5 How It Works: vtable and vptr

C++ compilers typically implement dynamic binding through the **vtable** (virtual function table) mechanism. The C++ standard does not prescribe this technique, but all major compilers use it:

1. **vtable:** Each class that has virtual functions gets a table of function pointers. The table maps each virtual function to the correct implementation for that class.
2. **vptr:** Each object of a class with virtual functions contains a hidden pointer (`vptr`) that points to its class's vtable.
3. **Runtime lookup:** When a virtual function is called through a base pointer, the program follows the `vptr` to the vtable, looks up the function, and calls the correct version.

**Simple vtable diagram (one virtual function):**

```text
┌────────────────┐         ┌──────────────────────┐
│  Base object   │         │   Base vtable        │
│  ┌──────────┐  │         │   func → Base::func  │
│  │  vptr   ─┼──┼────────▶│                      │
│  └──────────┘  │         └──────────────────────┘
└────────────────┘

┌────────────────┐         ┌───────────────────────────┐
│ Derived object │         │   Derived vtable          │
│  ┌──────────┐  │         │   func → Derived::func    │
│  │  vptr   ─┼──┼────────▶│                           │
│  └──────────┘  │         └───────────────────────────┘
└────────────────┘
```

*Figure 1. vptr and vtable of a Base object and a Derived object with one virtual function*

**Detailed vtable diagram (Shape hierarchy with multiple virtual functions):**

Consider `class Shape` with `virtual draw()` and `virtual area()`, and derived classes `Circle` and `Rectangle`:

```text
 Shape* p = new Circle(5.0);
 p->draw();   // How does C++ find Circle::draw()?

 Step 1: Follow p to the Circle object
 Step 2: Read the hidden vptr from the object
 Step 3: Follow vptr to Circle's vtable
 Step 4: Look up draw() in the vtable → Circle::draw
 Step 5: Call Circle::draw()

 Circle object in memory:         Circle's vtable:
 ┌──────────────────┐             ┌──────────────────────────┐
 │  vptr ───────────┼────────────▶│ draw → Circle::draw      │
 │  radius = 5.0    │             │ area → Circle::area      │
 └──────────────────┘             │ ~dtor → Circle::~Circle  │
                                  └──────────────────────────┘

 Rectangle object in memory:      Rectangle's vtable:
 ┌──────────────────┐             ┌───────────────────────────────┐
 │  vptr ───────────┼────────────▶│ draw → Rectangle::draw        │
 │  width = 3.0     │             │ area → Rectangle::area        │
 │  height = 4.0    │             │ ~dtor → Rectangle::~Rectangle │
 └──────────────────┘             └───────────────────────────────┘

 Shape object in memory:          Shape's vtable:
 ┌──────────────────┐             ┌──────────────────────────┐
 │  vptr ───────────┼────────────▶│ draw → Shape::draw       │
 └──────────────────┘             │ area → Shape::area       │
                                  │ ~dtor → Shape::~Shape    │
                                  └──────────────────────────┘
```

*Figure 2. Steps for resolving a virtual function call and the per-class vtables in the Shape hierarchy*

> **Key Point:** **The vtable is per-class** (shared by all objects of that class), but **the vptr is per-object** (each object carries its own pointer to its class's vtable). When you create a `Circle`, its vptr is set to point to `Circle`'s vtable. When you create a `Rectangle`, its vptr points to `Rectangle`'s vtable. This is why the same `p->draw()` call produces different results depending on the actual object.

> **Note:** The vtable mechanism adds a small overhead: one pointer per object and one indirect call per virtual function invocation. This is the cost of runtime polymorphism.

### 2.6 Override Keyword

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

> **Note:** **Best Practice.** Always use `override` when overriding virtual functions. It is a compile-time safety net at zero runtime cost.

For example, suppose `Rect` is meant to override `virtual void draw()` of `Shape` but the function is mistyped as `void drow()`. Without `override`, this merely adds a new function and no error occurs, so `draw()` called through a `Shape*` runs `Shape::draw()`. Writing `void drow() override;` exposes the mistake as a compile error because no matching virtual function exists.

### 2.7 Conditions for Successful Overriding

Overriding succeeds only if the **name, parameter types and count, and return type** of the derived class function all match the base class virtual function.

```cpp
class Base {
public:
    virtual void fail();
    virtual void success();
    virtual void g(int);
};

class Derived : public Base {
public:
    virtual int fail();           // overriding fails: different return type, compile error
    virtual void success();       // overriding succeeds
    virtual void g(int, double);  // different parameters, a new function: compiles normally
};
```

- `fail()`, which differs only in return type, is a compile error. However, when the base class virtual function returns a pointer or reference to the base class, changing the derived class version to return a pointer or reference to the derived class is allowed; this is a **covariant return type**.
- The `virtual` keyword of a virtual function is inherited, so it may be omitted in the derived class. Writing `virtual void success();` in `Derived` as `void success();` is the same overriding.
- A virtual function may have any access specifier: `private`, `protected`, or `public`. The second `Calculator` example in Section 4.4 makes the pure virtual function `calc()` `protected`.

> **Note:** The textbook presents `g(int, double)` as a case of overloading. However, the base class and the derived class have different scopes, so declaring a function with the same name in the derived class hides the base class `g(int)` in the scope of `Derived` (hiding, Section 6.5). Therefore calling `d.g(1)` on a `Derived` object `d` is a compile error, and `Base::g(int)` runs only when `g(1)` is called through a `Base*`. To use both functions together like overloads, declare `using Base::g;` inside `Derived`.

### 2.8 Inheritance of Virtual Functions

A virtual function remains virtual no matter how many levels the inheritance hierarchy has. In the following example (textbook Example 9-3), `Base`, `Derived`, and `GrandDerived` inherit from one another in turn, and all three pointers point to the same `GrandDerived` object.

```cpp
#include <iostream>
using namespace std;

class Base {
public:
    virtual void f() { cout << "Base::f() called" << endl; }
};

class Derived : public Base {
public:
    void f() { cout << "Derived::f() called" << endl; }
};

class GrandDerived : public Derived {
public:
    void f() { cout << "GrandDerived::f() called" << endl; }
};

int main() {
    GrandDerived g;
    Base* bp;
    Derived* dp;
    GrandDerived* gp;

    bp = dp = gp = &g;

    bp->f();   // dynamic binding
    dp->f();   // dynamic binding
    gp->f();
}
```

Output:

```text
GrandDerived::f() called
GrandDerived::f() called
GrandDerived::f() called
```

The `f()` functions of `Derived` and `GrandDerived` have no `virtual`, but both are virtual functions because `Base::f()` is virtual. Therefore, whether the pointer type is `Base*` or `Derived*`, dynamic binding runs the `f()` of the actual object, `GrandDerived`.

If a derived class does not override a virtual function, the function defined by the nearest base class in the inheritance hierarchy is used.

```cpp
#include <iostream>
using namespace std;

class B {
public:
    virtual void who() { cout << "Base\n"; }
};

class D1 : public B {
public:
    void who() { cout << "First derivation\n"; }
};

class D2 : public B {
    // who() is not overridden.
};

int main() {
    B base_obj;
    B* p;
    D1 D1_obj;
    D2 D2_obj;

    p = &base_obj;
    p->who();   // calls B's who()
    p = &D1_obj;
    p->who();   // calls D1's who()
    p = &D2_obj;
    p->who();   // D2 has no who(), so B's who() is called
    return 0;
}
```

Output:

```text
Base
First derivation
Base
```

Dynamic binding also occurs when a virtual function is called inside a member function of the base class. In the following example, `paint()` is a non-virtual function of `Shape`, but the call to `draw()` inside it is the same as `this->draw()`, so the `draw()` of the actual object that `this` points to runs.

```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    void paint() {
        draw();   // this->draw(): dynamic binding
    }
    virtual void draw() {
        cout << "Shape::draw() called" << endl;
    }
};

class Circle : public Shape {
public:
    virtual void draw() {
        cout << "Circle::draw() called" << endl;
    }
};

int main() {
    Circle circle;
    Shape* pShape = &circle;   // upcasting
    pShape->paint();
}
```

Output:

```text
Circle::draw() called
```

Calling `paint()` on a `Shape` object prints `Shape::draw() called`. In this way, base class code can call a function that derived classes implement, and the abstract class examples of Section 4.4 rely on this property.

### 2.9 The final Specifier

The `final` specifier, introduced in C++11, can be used in two places. Strictly speaking, `override` and `final` are not reserved words but identifiers with a special meaning only in specific positions, and the textbook calls both of them specifiers.

1. **Right after a virtual function prototype:** derived classes cannot override that virtual function.
2. **After a class name:** no other class can inherit from that class (inheritance is blocked).

```cpp
class Shape {
public:
    virtual void draw() final;   // can no longer be overridden
};

class Rect : public Shape {
public:
    void draw();                 // compile error: attempts to override a final function
};
```

```cpp
class Shape final {
    // ...
};

class Rect : public Shape {      // compile error: Shape cannot be inherited
    // ...
};
```

```cpp
class Shape {
    // ...
};

class Rect final : public Shape {   // Rect can inherit from Shape
    // ...
};

class RoundRect : public Rect {     // compile error: Rect cannot be inherited
    // ...
};
```

### 2.10 Calling a Base Class Virtual Function with the Scope Resolution Operator

Calling a function in the form `BaseClass::virtualFunction()` with the scope resolution operator `::` suppresses dynamic binding, and the base class function runs through static binding. This is mainly used when an overriding function in a derived class reuses the base class behavior and adds to it.

```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw() {
        cout << "--Shape--";
    }
};

class Circle : public Shape {
public:
    virtual void draw() {
        Shape::draw();   // calls the base class draw() (static binding)
        cout << "Circle" << endl;
    }
};

int main() {
    Circle circle;
    Shape* pShape = &circle;

    pShape->draw();          // dynamic binding: runs Circle::draw()
    pShape->Shape::draw();   // static binding: runs only Shape::draw()
}
```

Output:

```text
--Shape--Circle
--Shape--
```

`pShape->draw()` runs `Circle::draw()` through dynamic binding, and the `Shape::draw()` inside it prints `--Shape--` first through static binding. `pShape->Shape::draw()` is statically bound from the start, so it prints only `--Shape--`.

---

<br>

## 3. Virtual Destructors

### 3.1 Problem

When deleting a derived object through a base pointer, if the destructor is **not virtual**, typical compilers call only the base class destructor (the C++ standard defines this case as undefined behavior). The derived class destructor is skipped, potentially **leaking resources** allocated by the derived class.

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

### 3.2 Complete Example: The Memory Leak

This complete, runnable program demonstrates the problem and the fix side by side:

```cpp
#include <iostream>
#include <cstring>
using namespace std;

// ============ BUG: Non-virtual destructor ============
class BadBase {
public:
    ~BadBase() { cout << "[BadBase] destructor" << endl; }
};

class BadDerived : public BadBase {
    char* name;
public:
    BadDerived(const char* n) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        cout << "[BadDerived] allocated: " << name << endl;
    }
    ~BadDerived() {
        cout << "[BadDerived] freeing: " << name << endl;
        delete[] name;
    }
};

// ============ FIX: Virtual destructor ============
class GoodBase {
public:
    virtual ~GoodBase() { cout << "[GoodBase] destructor" << endl; }
};

class GoodDerived : public GoodBase {
    char* name;
public:
    GoodDerived(const char* n) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        cout << "[GoodDerived] allocated: " << name << endl;
    }
    ~GoodDerived() override {
        cout << "[GoodDerived] freeing: " << name << endl;
        delete[] name;
    }
};

int main() {
    cout << "--- Without virtual destructor (BUG) ---" << endl;
    BadBase* bp = new BadDerived("leak");
    delete bp;   // Only ~BadBase runs! "leak" is never freed.
    cout << endl;

    cout << "--- With virtual destructor (CORRECT) ---" << endl;
    GoodBase* gp = new GoodDerived("safe");
    delete gp;   // ~GoodDerived runs first, then ~GoodBase.
    cout << endl;

    return 0;
}
```

Output:

```text
--- Without virtual destructor (BUG) ---
[BadDerived] allocated: leak
[BadBase] destructor

--- With virtual destructor (CORRECT) ---
[GoodDerived] allocated: safe
[GoodDerived] freeing: safe
[GoodBase] destructor
```

Without a virtual destructor, `[BadDerived] freeing: leak` never appears. The destructor of `BadDerived` does not run, so the memory that `name` points to is leaked. With a virtual destructor, both destructors run in the proper order: derived class first, then base class.

### 3.3 Rule

If a class has **any virtual function**, it is likely to be handled through base class pointers, so its destructor should also be **virtual**. This ensures that `delete` through a base pointer calls the derived destructor first through dynamic binding, and the base destructor runs when the derived destructor finishes.

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

> **Note:** **Rule of Thumb.** Any class intended to be used as a base class with virtual functions should have a `virtual` destructor. This is one of the most important C++ best practices.

---

<br>

## 4. Abstract Classes

### 4.1 Pure Virtual Function

A **pure virtual function** is a virtual function that has only a declaration and no implementation in the base class. It is declared by appending `= 0` to the function prototype, and a derived class **must implement** it to become a concrete class (Section 4.3) from which objects can be created. A base class virtual function is declared pure when it only shows the form of the function that derived classes redefine and needs no code to run.

```cpp
class Shape {
public:
    virtual double getArea() = 0;   // pure virtual — no body
    virtual void draw() = 0;        // pure virtual
};
```

### 4.2 Abstract Class

A class that contains **at least one pure virtual function** is an **abstract class**. An abstract class:

- **Cannot be instantiated:** it is not a complete class, so objects of it cannot be created directly.
- **Can have pointers and references:** `Shape* p;` is valid.
- **Can have non-pure member functions:** it may provide partial implementations, and not every member function has to be pure virtual. A non-pure member function may even call a pure virtual function (in the same way as `paint()` in Section 2.8).
- **Serves as an interface/contract:** it shows the prototypes of the functions that derived classes must implement. An abstract class exists not to create objects but to serve as a base class in inheritance.

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

### 4.4 Abstract Class Implementation Examples

Textbook Example 9-6 implements a `GoodCalc` class by inheriting from an abstract class `Calculator` that consists only of pure virtual functions. `GoodCalc` implements all three pure virtual functions, so it is a concrete class.

```cpp
#include <iostream>
using namespace std;

class Calculator {
public:
    virtual int add(int a, int b) = 0;              // returns the sum of two integers
    virtual int subtract(int a, int b) = 0;         // returns the difference of two integers
    virtual double average(int a[], int size) = 0;  // returns the average of array a; size is the array size
};

class GoodCalc : public Calculator {
public:
    int add(int a, int b) { return a + b; }
    int subtract(int a, int b) { return a - b; }
    double average(int a[], int size) {
        double sum = 0;
        for (int i = 0; i < size; i++)
            sum += a[i];
        return sum / size;
    }
};

int main() {
    int a[] = {1, 2, 3, 4, 5};
    Calculator* p = new GoodCalc();
    cout << p->add(2, 3) << endl;
    cout << p->subtract(2, 3) << endl;
    cout << p->average(a, 5) << endl;
    delete p;
}
```

Output:

```text
5
-1
3
```

> **Note:** `Calculator` has no virtual destructor, so the last line, which deletes a `GoodCalc` object through a `Calculator*`, is the undefined behavior described in Section 3.1. `GoodCalc` has no resources to release, so the problem does not show in this example, but the correct fix is to add `virtual ~Calculator() { }` to `Calculator`.

In textbook Example 9-7, `Calculator` implements `run()`, which handles user input and result output, and leaves only the actual calculation as the pure virtual function `calc()`. The derived classes `Adder` and `Subtractor` only need to implement `calc()`. The call `calc(a, b)` inside `run()` is dynamically bound to the `calc()` of the actual object, like `paint()` in Section 2.8.

```cpp
#include <iostream>
using namespace std;

class Calculator {
    void input() {
        cout << "Enter two integers>> ";
        cin >> a >> b;
    }
protected:
    int a, b;
    virtual int calc(int a, int b) = 0;   // returns the result of calculating two integers
public:
    void run() {
        input();
        cout << "Calculated value: " << calc(a, b) << endl;
    }
};

class Adder : public Calculator {
protected:
    int calc(int a, int b) {   // implements the pure virtual function
        return a + b;
    }
};

class Subtractor : public Calculator {
protected:
    int calc(int a, int b) {   // implements the pure virtual function
        return a - b;
    }
};

int main() {
    Adder adder;
    Subtractor subtractor;
    adder.run();
    subtractor.run();
}
```

Sample run (entering `5 3` both times):

```text
Enter two integers>> 5 3
Calculated value: 8
Enter two integers>> 5 3
Calculated value: 2
```

---

<br>

## 5. Polymorphism

### 5.1 Concept

**Polymorphism** means "one interface, multiple implementations." A base class pointer can call different derived class implementations depending on the actual object type at runtime.

```text
            Base* p
               │
     ┌─────────┼─────────┐
     ▼         ▼         ▼
  DerivedA  DerivedB  DerivedC
   func()    func()    func()
```

*Figure 3. Structure in which one base class pointer calls func() of several derived classes*

Through `p->func()`, the correct derived version is called based on what `p` actually points to, not on the pointer type.

### 5.2 Complete Shape Hierarchy Example

A complete, runnable program demonstrating a `Shape` hierarchy with virtual `draw()` and `area()`, polymorphic behavior through an array of base pointers, and proper virtual destructor usage.

```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw() = 0;            // pure virtual
    virtual double area() = 0;          // pure virtual
    virtual ~Shape() { }                // virtual destructor
};

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) { }
    void draw() override {
        cout << "Drawing Circle (radius=" << radius << ")" << endl;
    }
    double area() override {
        return 3.14159265 * radius * radius;
    }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) { }
    void draw() override {
        cout << "Drawing Rectangle (" << width << "x" << height << ")" << endl;
    }
    double area() override {
        return width * height;
    }
};

class Triangle : public Shape {
    double base, height;
public:
    Triangle(double b, double h) : base(b), height(h) { }
    void draw() override {
        cout << "Drawing Triangle (base=" << base << ", height=" << height << ")" << endl;
    }
    double area() override {
        return 0.5 * base * height;
    }
};

// This function works with ANY Shape — past, present, or future.
// It does not need to know about Circle, Rectangle, or Triangle.
void printShapeInfo(Shape* s) {
    s->draw();
    cout << "  Area = " << s->area() << endl;
}

int main() {
    const int N = 4;
    Shape* canvas[N];
    canvas[0] = new Circle(5.0);
    canvas[1] = new Rectangle(3.0, 4.0);
    canvas[2] = new Triangle(6.0, 3.0);
    canvas[3] = new Circle(2.5);

    // One loop handles ALL shape types uniformly
    for (int i = 0; i < N; i++) {
        printShapeInfo(canvas[i]);
    }

    // Clean up — virtual destructor ensures proper deletion
    for (int i = 0; i < N; i++) {
        delete canvas[i];
    }

    return 0;
}
```

Output:

```text
Drawing Circle (radius=5)
  Area = 78.5398
Drawing Rectangle (3x4)
  Area = 12
Drawing Triangle (base=6, height=3)
  Area = 9
Drawing Circle (radius=2.5)
  Area = 19.635
```

The power of this design: if you later add a `Pentagon` class, you only need to write the new class and override `draw()` and `area()`. The `printShapeInfo()` function and the loop in `main()` require **zero changes**. This is the Open-Closed Principle in action: open for extension, closed for modification.

The textbook demonstrates the same principle with a linked list. `Shape` has a `next` pointer to the next shape, `add()`, `getNext()`, and a `paint()` that calls `draw()`, and `Circle`, `Rect`, and `Line` objects are linked one after another with `add()`. Following the list from the start pointer with a `Shape*` pointer and calling `p->paint()` runs the `draw()` that each object overrides, printing `Circle`, `Rectangle`, `Circle`, `Line`, and `Rectangle` in order. When the shapes are deleted, the virtual destructor of `Shape` also ensures that each derived object is destroyed correctly.

### 5.3 Complete Converter Example

A complete, runnable unit conversion system using abstract classes and polymorphism. The abstract base `Converter` defines the interface, and each concrete derived class implements a specific conversion. An array of `Converter*` pointers processes all converters uniformly.

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

class KmToMiles : public Converter {
public:
    KmToMiles(double val) : Converter(val) { }
    void compute() override { after = before * 0.621371; }
    void display() override {
        cout << before << " km = " << after << " miles" << endl;
    }
};

class KgToPounds : public Converter {
public:
    KgToPounds(double val) : Converter(val) { }
    void compute() override { after = before * 2.20462; }
    void display() override {
        cout << before << " kg = " << after << " lbs" << endl;
    }
};

int main() {
    const int N = 4;
    Converter* conv[N];
    conv[0] = new LiterToGallon(10.0);
    conv[1] = new CelsiusToFahrenheit(100.0);
    conv[2] = new KmToMiles(42.195);
    conv[3] = new KgToPounds(70.0);

    // One loop processes ALL converter types
    for (int i = 0; i < N; i++) {
        conv[i]->compute();
        conv[i]->display();
    }

    for (int i = 0; i < N; i++) {
        delete conv[i];
    }

    return 0;
}
```

Output:

```text
10 liters = 2.64173 gallons
100 C = 212 F
42.195 km = 26.2187 miles
70 kg = 154.323 lbs
```

Adding a new unit conversion (e.g., `InchToCm`) requires only writing a new derived class. The `main()` loop logic needs no modification.

---

<br>

## 6. Common Mistakes

### 6.1 Forgetting Virtual Destructor

When using polymorphism (deleting derived objects through base pointers), forgetting to make the base destructor `virtual` causes the derived destructor to be skipped. This is probably the single most common virtual function bug in C++.

```cpp
class Base {
public:
    virtual void doWork() = 0;
    ~Base() { }                  // BUG: not virtual!
};
class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[1000]; }
    ~Derived() { delete[] data; }
    void doWork() override { }
};

Base* p = new Derived;
delete p;   // ~Derived is NEVER called → 4000 bytes leaked every time
```

The 4000 bytes in the comment is the size that `new int[1000]` allocates in an environment where `int` is 4 bytes.

**Fix:** Always declare `virtual ~Base() { }` (or `virtual ~Base() = default;`) when a class has any virtual function.

### 6.2 Trying to Instantiate an Abstract Class

Abstract classes cannot be instantiated. The compiler will reject it.

```cpp
class Shape {
public:
    virtual void draw() = 0;
};

// Shape s;          // compile error: cannot declare variable 's' to be of abstract type 'Shape'
// new Shape();      // compile error: same reason

Shape* p;            // OK — pointer to abstract class is fine
```

### 6.3 Forgetting to Override All Pure Virtual Functions

If a derived class does not override **every** pure virtual function from its base, it is **still abstract** and cannot be instantiated.

```cpp
class Shape {
public:
    virtual void draw() = 0;
    virtual double area() = 0;
    virtual ~Shape() { }
};

class IncompleteCircle : public Shape {
    double radius;
public:
    IncompleteCircle(double r) : radius(r) { }
    void draw() override { /* ... */ }
    // Forgot to override area()!
};

// IncompleteCircle c(5.0);  // compile error! IncompleteCircle is STILL abstract
                              // because area() is not overridden
```

**Fix:** Override every pure virtual function, or intentionally keep the class abstract (e.g., as an intermediate base class).

### 6.4 Object Slicing

When a derived object is passed **by value** to a function expecting the base type, the derived part is **sliced off** and only the base part is copied. Virtual functions no longer work as expected because the parameter object is now genuinely an object of the base class type, not of the derived class.

```cpp
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void speak() { cout << "..." << endl; }
    virtual ~Animal() { }
};

class Dog : public Animal {
public:
    void speak() override { cout << "Woof!" << endl; }
};

// WRONG: takes Animal by VALUE — slicing occurs!
void makeItSpeak_bad(Animal a) {
    a.speak();      // always calls Animal::speak(), never Dog::speak()
}

// CORRECT: takes Animal by REFERENCE — no slicing
void makeItSpeak_good(Animal& a) {
    a.speak();      // calls the correct override
}

// ALSO CORRECT: takes Animal by POINTER
void makeItSpeak_ptr(Animal* a) {
    a->speak();     // calls the correct override
}

int main() {
    Dog d;
    makeItSpeak_bad(d);    // "..."   — SLICED! Dog part is lost
    makeItSpeak_good(d);   // "Woof!" — correct
    makeItSpeak_ptr(&d);   // "Woof!" — correct
    return 0;
}
```

**Rule:** When working with polymorphism, always pass objects by **pointer** or **reference**, never by value.

### 6.5 Overriding vs Overloading vs Hiding

These three concepts are frequently confused. They have very different meanings:

| | Overriding | Overloading | Hiding |
|:--|:-----------|:------------|:-------|
| **What** | Same name, same parameters, base function is `virtual` | Same name, **different** parameters, same scope | Same name but not overriding (base function is **not** `virtual`, **or** the parameters differ) |
| **Where** | Derived class re-implements base virtual function | Same class (or same scope) | Derived class has same-name function as base |
| **Effect** | Dynamic binding calls the correct version at runtime | Compiler picks based on argument types at compile time | Base version is hidden; only derived version is visible |
| **virtual required?** | Yes | No | No (redefining a non-virtual function with the same prototype is hiding, not overriding) |

*Table 1. Comparison of overriding, overloading, and hiding*

```cpp
class Base {
public:
    virtual void func(int x) { cout << "Base::func(int)" << endl; }
    void nonVirtual(int x)   { cout << "Base::nonVirtual(int)" << endl; }
};

class Derived : public Base {
public:
    // OVERRIDING: same name, same params, base is virtual
    void func(int x) override { cout << "Derived::func(int)" << endl; }

    // HIDING: same name, same params, but base is NOT virtual
    void nonVirtual(int x) { cout << "Derived::nonVirtual(int)" << endl; }

    // OVERLOADING: same name, DIFFERENT params (within Derived's scope)
    void func(double x) { cout << "Derived::func(double)" << endl; }
};

int main() {
    Derived d;
    Base* bp = &d;

    bp->func(1);          // "Derived::func(int)"   — OVERRIDING (dynamic binding)
    bp->nonVirtual(1);    // "Base::nonVirtual(int)" — HIDING (static binding; base version called)

    d.func(3.14);         // "Derived::func(double)" — OVERLOADING (resolved at compile time)
    d.func(1);            // "Derived::func(int)"    — OVERRIDING
    d.nonVirtual(1);      // "Derived::nonVirtual(int)" — HIDING (derived version called on Derived object)
}
```

> **Caution:** Hiding is almost always a bug. If you intend to override, use `virtual` in the base and `override` in the derived. If the compiler says `override` does not match anything, you might be hiding instead of overriding.

---

<br>

## Summary

| Concept | Key Summary |
|:--------|:-------------|
| Upcasting | Upcasting converts a derived class pointer or reference to the base class type; it is implicit and safe, but only base class members are accessible. |
| Downcasting | Downcasting converts a base class pointer to a derived class pointer; it requires an explicit cast, and `dynamic_cast` checks it safely at runtime. |
| Static binding | Without `virtual`, the pointer type determines the function to call at compile time. |
| Dynamic binding | With `virtual`, the actual object type determines the function to call at runtime. |
| `virtual` keyword | The `virtual` keyword is the switch that changes a call from static binding to dynamic binding and enables true polymorphism. |
| vtable / vptr | The vtable is a function pointer table, one per class, and the vptr is one per object and points to its class's vtable. |
| `override` (C++11) | The compiler verifies that the base class has a matching virtual function, catching name typos and prototype mismatches. |
| Conditions for overriding | Overriding succeeds only when the name, parameter types and count, and return type all match, and the inherited `virtual` keyword may be omitted in the derived class. |
| Inheritance of virtual functions | A virtual function stays virtual at every level of the hierarchy, and a derived class that does not override it uses the function of the nearest base class. |
| `final` (C++11) | Placed after a virtual function prototype, `final` forbids overriding that function, and placed after a class name, it forbids inheriting from that class. |
| Scope resolution call | Calling in the form `BaseClass::virtualFunction()` runs the base class function through static binding. |
| Virtual destructor | A class with virtual functions needs a virtual destructor so that `delete` through a base class pointer also runs the derived class destructor correctly. |
| Pure virtual function | A pure virtual function such as `virtual void func() = 0;` has only a declaration, and a derived class must implement it to become concrete. |
| Abstract class | An abstract class has at least one pure virtual function, cannot be instantiated, and serves as an interface that shows the prototypes derived classes implement. |
| Concrete class | A concrete class implements all inherited pure virtual functions and can be instantiated. |
| Polymorphism | Polymorphism handles multiple implementations through one interface, with a base class pointer calling the derived class function that matches the actual object. |
| Object slicing | Passing a derived object by value to a base class parameter slices off the derived part, so objects should be passed by pointer or reference. |
| Overriding vs Hiding | Overriding redefines a base class virtual function with the same prototype and is dynamically bound, while hiding is a same-name function that is not an override, hides the base class function, and is statically bound. |

---

<br>

## Self-Check Questions

1. **Binding:** In the following code, what do `p->f()` and `p->g()` each print?

   ```cpp
   class A {
   public:
       void f() { cout << "A::f()" << endl; }
       virtual void g() { cout << "A::g()" << endl; }
   };
   class B : public A {
   public:
       void f() { cout << "B::f()" << endl; }
       void g() override { cout << "B::g()" << endl; }
   };

   A* p = new B;
   p->f();
   p->g();
   ```

   <details>
   <summary>Answer</summary>

   `p->f()` prints `A::f()` and `p->g()` prints `B::g()`. `f()` is non-virtual, so it is statically bound to `A::f()` at compile time according to the pointer type `A*`. `g()` is virtual, so at runtime it is dynamically bound to the `g()` of the `B` object that `p` actually points to.

   </details>

2. **Virtual destructor:** Suppose `A` in Question 1 has `~A() { cout << "~A()" << endl; }` and `B` has `~B() { cout << "~B()" << endl; }`. When `delete p;` runs, which destructors run and in what order? Answer for the case where `~A()` is non-virtual and the case of `virtual ~A()`.

   <details>
   <summary>Answer</summary>

   If `~A()` is non-virtual, the destructor call is also statically bound by the pointer type, so typical compilers run only `~A()` and not `~B()` (undefined behavior under the standard). With `virtual ~A()`, dynamic binding runs `~B()` first, and `~A()` runs after `~B()` finishes.

   </details>

3. **Identifying abstract classes:** Choose every declaration below that causes a compile error and explain why.

   ```cpp
   class Shape {
   public:
       virtual void draw() = 0;
       virtual double area() = 0;
   };
   class Circle : public Shape {
   public:
       void draw() override { }
       double area() override { return 0; }
   };
   class Half : public Shape {
   public:
       void draw() override { }
   };

   Shape s;          // (a)
   Circle c;         // (b)
   Half h;           // (c)
   Shape* p = &c;    // (d)
   ```

   <details>
   <summary>Answer</summary>

   (a) and (c) are compile errors. `Shape` is an abstract class with pure virtual functions, so no object of it can be created. `Half` does not override `area()`, so it is still abstract. `Circle` is a concrete class that implements both pure virtual functions, so (b) is fine, and a pointer to an abstract class can be declared, so (d) is also fine.

   </details>

4. **Overriding conditions:** When `Base` has `virtual void f(int);`, is each of the following declarations in `Derived` an override, a compile error, or a new function (hiding)? (a) `void f(int);` (b) `int f(int);` (c) `void f(double);`

   <details>
   <summary>Answer</summary>

   (a) is an override. The name, parameters, and return type all match, and the `virtual` keyword is inherited, so the function is virtual even without it. (b) has the same parameters and differs only in return type, so it is a compile error. (c) has different parameters, so it is a new function rather than an override, and it hides `Base::f(int)` in the scope of `Derived`.

   </details>

5. **Scope resolution operator:** In the example of Section 2.10, what happens when `pShape->draw()` runs if `Shape::draw();` inside `Circle::draw()` is changed to `draw();`?

   <details>
   <summary>Answer</summary>

   `draw()` inside `Circle::draw()` is the same as `this->draw()`, and `draw()` is virtual, so dynamic binding calls `Circle::draw()` itself again. This becomes infinite recursion with no termination condition, and the program eventually terminates abnormally with a stack overflow. To run the base class version, static binding must be requested with the scope resolution operator, as in `Shape::draw()`.

   </details>

---
