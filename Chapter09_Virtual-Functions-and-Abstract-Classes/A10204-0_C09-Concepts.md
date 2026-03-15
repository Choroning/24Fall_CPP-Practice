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
  - [2.1 Why Virtual Functions?](#21-why-virtual-functions)
  - [2.2 Problem Without virtual](#22-problem-without-virtual)
  - [2.3 virtual Keyword](#23-virtual-keyword)
  - [2.4 Static vs Dynamic Binding](#24-static-vs-dynamic-binding)
  - [2.5 How It Works — vtable and vptr](#25-how-it-works--vtable-and-vptr)
  - [2.6 Override Keyword](#26-override-keyword)
- [3. Virtual Destructors](#3-virtual-destructors)
  - [3.1 Problem](#31-problem)
  - [3.2 Complete Example — The Memory Leak](#32-complete-example--the-memory-leak)
  - [3.3 Rule](#33-rule)
- [4. Abstract Classes](#4-abstract-classes)
  - [4.1 Pure Virtual Function](#41-pure-virtual-function)
  - [4.2 Abstract Class](#42-abstract-class)
  - [4.3 Concrete Class](#43-concrete-class)
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

### 2.1 Why Virtual Functions?

Imagine you are building a drawing program. You have a `Shape` base class and derived classes like `Circle`, `Rectangle`, and `Triangle`. You store all shapes in a single array of `Shape*` pointers and want to draw them all in one loop:

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

**Without virtual:** Every call to `canvas[i]->draw()` calls `Shape::draw()`, because the compiler only sees the `Shape*` type. It does not matter that the object is actually a `Circle` — the compiler already decided at compile time to call `Shape::draw()`. Every shape draws the same thing. This is useless.

**With virtual:** Every call to `canvas[i]->draw()` calls the **correct** `draw()` for the actual object — `Circle::draw()` for circles, `Rectangle::draw()` for rectangles, and so on. The decision happens at runtime based on the actual object type.

This is **the core of OOP polymorphism**: writing code that operates on base class pointers but automatically does the right thing for each derived type. Without virtual functions, inheritance is just code sharing — with virtual functions, inheritance enables *substitutability*.

### 2.2 Problem Without virtual

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

### 2.3 virtual Keyword

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

### 2.4 Static vs Dynamic Binding

Understanding the difference between static and dynamic binding is essential. They determine **when** the decision of "which function to call" is made.

**Static Binding (Compile-Time Binding)**
- The compiler decides which function to call based on the **pointer/reference type**.
- This is the default for non-virtual functions.
- Fast (no runtime lookup), but ignores the actual object type.

**Dynamic Binding (Runtime Binding)**
- The program decides which function to call at runtime based on the **actual object type**.
- Activated by the `virtual` keyword.
- Slightly slower (vtable lookup), but enables true polymorphism.

The `virtual` keyword is the switch that changes a function from static binding to dynamic binding.

**Side-by-Side Comparison — Same Code, Different Behavior:**

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

> **Key Point:** The only difference is the `virtual` keyword on the base class. Everything else — the inheritance, the pointer types, the function calls — is identical. `virtual` is what makes polymorphism work.

### 2.5 How It Works — vtable and vptr

C++ implements dynamic binding through the **vtable** (virtual function table) mechanism:

1. **vtable** — Each class that has virtual functions gets a table of function pointers. The table maps each virtual function to the correct implementation for that class.
2. **vptr** — Each object of a class with virtual functions contains a hidden pointer (`vptr`) that points to its class's vtable.
3. **Runtime lookup** — When a virtual function is called through a base pointer, the program follows the `vptr` to the vtable, looks up the function, and calls the correct version.

**Simple vtable diagram (one virtual function):**

```
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

**Detailed vtable diagram — Shape hierarchy with multiple virtual functions:**

Consider `class Shape` with `virtual draw()` and `virtual area()`, and derived classes `Circle` and `Rectangle`:

```
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

The key insight: **the vtable is per-class** (shared by all objects of that class), but **the vptr is per-object** (each object carries its own pointer to its class's vtable). When you create a `Circle`, its vptr is set to point to `Circle`'s vtable. When you create a `Rectangle`, its vptr points to `Rectangle`'s vtable. This is why the same `p->draw()` call produces different results depending on the actual object.

> **Note:** The vtable mechanism adds a small overhead — one pointer per object and one indirect call per virtual function invocation. This is the cost of runtime polymorphism.

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

### 3.2 Complete Example — The Memory Leak

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

Notice: in the "BUG" case, `[BadDerived] freeing: leak` never appears — the memory is leaked. In the "CORRECT" case, both destructors run in the proper order.

### 3.3 Rule

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
     ┌─────────┼─────────┐
     ▼         ▼         ▼
  DerivedA  DerivedB  DerivedC
   func()    func()    func()
```

Through `p->func()`, the correct derived version is called based on what `p` actually points to — not the pointer type.

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
  Area = 19.6350
```

The power of this design: if you later add a `Pentagon` class, you only need to write the new class and override `draw()` and `area()`. The `printShapeInfo()` function and the loop in `main()` require **zero changes**. This is the Open-Closed Principle in action: open for extension, closed for modification.

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
10 liters = 2.64172 gallons
100 C = 212 F
42.195 km = 26.2188 miles
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

When a derived object is passed **by value** to a function expecting the base type, the derived part is **sliced off** — only the base part is copied. Virtual functions no longer work as expected because the object is now genuinely a `Base`, not a `Derived`.

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
| **What** | Same name, same parameters, base function is `virtual` | Same name, **different** parameters, same scope | Same name, base function is **not** `virtual` |
| **Where** | Derived class re-implements base virtual function | Same class (or same scope) | Derived class has same-name function as base |
| **Effect** | Dynamic binding — correct version called at runtime | Compiler picks based on argument types at compile time | Base version is hidden; only derived version is visible |
| **virtual required?** | Yes | No | No (that is what makes it hiding, not overriding) |

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

> **Warning:** Hiding is almost always a bug. If you intend to override, use `virtual` in the base and `override` in the derived. If the compiler says `override` does not match anything, you might be hiding instead of overriding.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Upcasting | Derived pointer/reference to base; implicit and safe; only base members visible |
| Downcasting | Base pointer to derived; explicit cast required; use `dynamic_cast` for safety |
| Static binding | Without `virtual`, pointer type determines which function is called (compile time) |
| Dynamic binding | With `virtual`, actual object type determines which function runs (runtime) |
| `virtual` keyword | The switch from static to dynamic binding; enables true polymorphism |
| vtable / vptr | vtable is per-class (function pointer table); vptr is per-object (points to its class's vtable) |
| `override` (C++11) | Compiler verifies base has matching virtual function; catches typos and mismatches |
| Virtual destructor | Required if class has any virtual function; ensures correct cleanup through base pointer |
| Pure virtual function | `virtual void func() = 0;` — no body; derived classes must implement |
| Abstract class | Has at least one pure virtual function; cannot be instantiated; serves as interface |
| Concrete class | Implements all pure virtual functions; can be instantiated |
| Polymorphism | One interface, multiple implementations; base pointer calls correct derived version |
| Object slicing | Passing derived by value to base parameter loses derived data; use pointer/reference instead |
| Overriding vs Hiding | Overriding requires `virtual` and uses dynamic binding; hiding is non-virtual and uses static binding |

---
