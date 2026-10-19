# Chapter 8 — Inheritance

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 8

> **Learning Objectives:**
> 1. Understand the concept of inheritance and declare derived classes
> 2. Explain the relationship between inheritance and object pointers through upcasting and downcasting
> 3. Explain `protected` access and the differences among public, protected, and private inheritance
> 4. Trace the execution order of constructors and destructors in an inheritance hierarchy
> 5. Declare multiple inheritance and resolve the diamond problem with virtual inheritance
> 6. Use the C++11 inheritance features `final`, `override`, and inheriting constructors

---

## Table of Contents

- [1. Inheritance Concepts](#1-inheritance-concepts)
  - [1.1 Why Inheritance](#11-why-inheritance)
  - [1.2 Syntax](#12-syntax)
  - [1.3 What Is Inherited](#13-what-is-inherited)
  - [1.4 Inheritance and Object Pointers](#14-inheritance-and-object-pointers)
- [2. Access Control with Inheritance](#2-access-control-with-inheritance)
  - [2.1 `public` Inheritance](#21-public-inheritance)
  - [2.2 `protected` Inheritance](#22-protected-inheritance)
  - [2.3 `private` Inheritance](#23-private-inheritance)
  - [2.4 Access Table](#24-access-table)
- [3. Constructor and Destructor in Inheritance](#3-constructor-and-destructor-in-inheritance)
  - [3.1 Constructor Chain](#31-constructor-chain)
  - [3.2 Destructor Chain](#32-destructor-chain)
  - [3.3 Example with Output Trace](#33-example-with-output-trace)
  - [3.4 Constructors and Destructors in Multilevel Inheritance](#34-constructors-and-destructors-in-multilevel-inheritance)
- [4. Member Function Overriding](#4-member-function-overriding)
  - [4.1 Overriding](#41-overriding)
  - [4.2 Calling the Base Version](#42-calling-the-base-version)
- [5. Multiple Inheritance](#5-multiple-inheritance)
- [6. Modern Inheritance Features (C++11)](#6-modern-inheritance-features-c11)
  - [6.1 The `final` Keyword](#61-the-final-keyword)
  - [6.2 The `override` Keyword](#62-the-override-keyword)
  - [6.3 Inheriting Constructors with `using`](#63-inheriting-constructors-with-using)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Inheritance Concepts

### 1.1 Why Inheritance

**Inheritance** lets a new class (derived class) reuse and extend an existing class (base class).

Inheritance is a relationship defined between classes; there is no inheritance relationship between objects. A derived class inherits the attributes and functions of the base class and adds its own, so the concept of a class becomes more specific from the base class toward the derived class.

| Benefit | Explanation |
|:--------|:-----------|
| Code reuse | Common data and functions are written once in the base class |
| IS-A relationship | A `Car` **is a** `Vehicle`; a `Manager` **is an** `Employee` |
| Extensibility | New functionality is added in the derived class without modifying the base |

*Table 1. Benefits of inheritance*

**Terminology:**

| Term | Synonym |
|:-----|:--------|
| Base class | Parent class, Super class |
| Derived class | Child class, Sub class |

*Table 2. Synonyms for base and derived classes*

```text
   Base (Parent)
       ↑
   Derived (Child)
```

*Figure 1. Inheritance relationship between a base class and a derived class*

### 1.2 Syntax

```cpp
class Derived : public Base {
    // additional members
};
```

- The colon `:` is followed by the access specifier (`public`, `protected`, or `private`) and the base class name.
- **`public` inheritance** is the most common and preserves the IS-A relationship.
- If `class Student : public Person` is inherited again as `class StudentWorker : public Student`, `StudentWorker` inherits the members of `Student` together with the members that `Student` inherited from `Person`.

```cpp
class Vehicle {
    string brand;
    int year;
public:
    Vehicle(string b, int y) : brand(b), year(y) {}
    string getBrand() { return brand; }
    int getYear() { return year; }
protected:
    void updateYear(int y) { year = y; }
};

class Car : public Vehicle {
    int numDoors;
public:
    Car(string b = "Unknown", int y = 2024, int d = 4)
        : Vehicle(b, y), numDoors(d) {}
    void setDoors(int d) { numDoors = d; }
    void setYear(int y) { updateYear(y); }   // calls protected base member
    void show() {
        cout << getBrand() << " (" << getYear() << "), "
             << numDoors << " doors" << endl;
    }
};
```

### 1.3 What Is Inherited

All members of the base class are inherited by the derived class. However, **accessibility** depends on the access specifier.

| Base member access | Accessible from derived? |
|:-------------------|:------------------------|
| `public` | Yes (directly accessible) |
| `protected` | Yes (directly accessible) |
| `private` | **No**: inherited but NOT directly accessible; must use public/protected getters |

*Table 3. Accessibility from the derived class by base member access*

```cpp
class Base {
    int secret;        // private
protected:
    int shared;        // protected
public:
    int open;          // public
    int getSecret() { return secret; }
};

class Derived : public Base {
public:
    void test() {
        // cout << secret;     // ERROR: private member of Base
        cout << shared;        // OK: protected is accessible
        cout << open;          // OK: public is accessible
        cout << getSecret();   // OK: public getter for private member
    }
};
```

A `protected` member can be accessed only from member functions of the declaring class and its derived classes. Other classes and external functions (e.g., `main()`) cannot access a `protected` member, whether through a base class object or through a derived class object. In the example above, if `main()` creates `Derived d;` and writes `d.shared = 1;`, it is a compile error.

> **Key Point:** `private` members are inherited (they exist in memory) but cannot be accessed directly from the derived class. Use getters defined in the base class to access them.

### 1.4 Inheritance and Object Pointers

A derived class object contains a base class part, so a base class pointer can point to a derived class object.

- **Upcasting** assigns a derived class pointer to a base class pointer. Like seeing a person as an animal, it is always safe and needs no explicit cast. Through the base class pointer, however, only members declared in the base class are accessible.
- **Downcasting** assigns a base class pointer to a derived class pointer. It always requires an explicit cast and is safe only when the pointer actually points to a derived class object.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Point {
    int x, y;
public:
    void set(int x, int y) { this->x = x; this->y = y; }
    void showPoint() { cout << "(" << x << "," << y << ")" << endl; }
};

class ColorPoint : public Point {
    string color;
public:
    void setColor(string color) { this->color = color; }
    void showColorPoint() { cout << color << ":"; showPoint(); }
};

int main() {
    ColorPoint cp;
    ColorPoint *pDer = &cp;
    Point *pBase = pDer;            // upcasting: no cast needed

    pDer->set(3, 4);
    pBase->showPoint();             // (3,4)
    pDer->setColor("Red");
    pDer->showColorPoint();         // Red:(3,4)
    // pBase->showColorPoint();     // compile error: Point has no showColorPoint()

    ColorPoint *pDer2 = (ColorPoint *)pBase;   // downcasting: explicit cast required
    pDer2->showColorPoint();        // Red:(3,4)
    return 0;
}
```

The output is as follows.

```text
(3,4)
Red:(3,4)
Red:(3,4)
```

```text
                ColorPoint cp
pDer  ──▶  ┌──────────────────────┐
pBase ──▶  │ x, y                 │  Point part: accessible through pBase
           │ set(), showPoint()   │
           ├──────────────────────┤
           │ color                │  ColorPoint part: accessible only through pDer
           │ setColor(),          │
           │ showColorPoint()     │
           └──────────────────────┘
```

*Figure 2. Layout of a derived class object and the access range by pointer type*

| Kind | Direction of assignment | Cast | Members accessible through the pointer |
|:-----|:-----|:-----|:-----|
| Upcasting | Derived class pointer → base class pointer | Not needed | Members of the base class |
| Downcasting | Base class pointer → derived class pointer | Explicit cast required | Members of the derived class as well |

*Table 4. Upcasting and downcasting compared*

> **Caution:** If a downcast pointer actually points to a base class object rather than a derived class object, accessing a derived class member is undefined behavior. Use downcasting only when you know the actual type of the object the pointer points to.

---

<br>

## 2. Access Control with Inheritance

The **inheritance access specifier** determines how base class members are exposed in the derived class. One of `public`, `protected`, or `private` is specified when inheritance is declared, and it decides how the access attributes of the base class's `protected` and `public` members are carried into the derived class.

### 2.1 `public` Inheritance

The most common form, in which base access levels are preserved as they are in the derived class.

```cpp
class Derived : public Base { };
```

| Base member | Becomes in Derived |
|:------------|:-------------------|
| `public` | `public` |
| `protected` | `protected` |
| `private` | inaccessible |

*Table 5. Member access under `public` inheritance*

### 2.2 `protected` Inheritance

Public members of the base become `protected` in the derived class. Rarely used in practice.

```cpp
class Derived : protected Base { };
```

| Base member | Becomes in Derived |
|:------------|:-------------------|
| `public` | `protected` |
| `protected` | `protected` |
| `private` | inaccessible |

*Table 6. Member access under `protected` inheritance*

### 2.3 `private` Inheritance

All accessible base members become `private` in the derived class. Rarely used.

```cpp
class Derived : private Base { };
```

| Base member | Becomes in Derived |
|:------------|:-------------------|
| `public` | `private` |
| `protected` | `private` |
| `private` | inaccessible |

*Table 7. Member access under `private` inheritance*

### 2.4 Access Table

| Base member | `public` inheritance | `protected` inheritance | `private` inheritance |
|:------------|:--------------------|:----------------------|:---------------------|
| `public` | `public` | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private` | inaccessible | inaccessible | inaccessible |

*Table 8. Base member access by inheritance type*

When inheritance is nested over several levels, the inheritance type of each level applies in turn. For example, with `class Derived : private Base`, the public and protected members of `Base` become private inside `Derived`, so member functions of `Derived` itself can call them, but member functions of a class that inherits from `Derived` cannot.

> **Key Point:** Regardless of the inheritance type, `private` members of the base class are **never** directly accessible from the derived class. Only `public` inheritance preserves the IS-A relationship.

---

<br>

## 3. Constructor and Destructor in Inheritance

### 3.1 Constructor Chain

When a derived object is created, the **base constructor runs first**, then the derived constructor. If no base constructor is explicitly called, the base's **default constructor** is used.

```cpp
class Base {
public:
    Base() { cout << "Base()" << endl; }
    Base(int x) { cout << "Base(" << x << ")" << endl; }
};

class Derived : public Base {
public:
    // Explicitly calls Base(int)
    Derived(int x, int y) : Base(x) {
        cout << "Derived(" << x << ", " << y << ")" << endl;
    }
};

// Usage
Derived d(10, 20);
// Output:
//   Base(10)
//   Derived(10, 20)
```

| Scenario | What happens |
|:---------|:-------------|
| No explicit base constructor call | Base **default constructor** is called automatically |
| Explicit call `Derived(args) : Base(args)` | The specified base constructor is called |
| Base has no default constructor and derived does not call one | **Compile error** |

*Table 9. Base class constructor called by a derived class constructor*

If a derived class constructor does not specify a base class constructor, the compiler implicitly inserts a call to the base class's default constructor (`: A()`) into that constructor. The same applies to derived class constructors with parameters.

```cpp
class A {
public:
    A() { cout << "Constructor A" << endl; }
    A(int x) { cout << "Param constructor A" << x << endl; }
};

class B : public A {
public:
    B() { cout << "Constructor B" << endl; }                     // compiled as B() : A()
    B(int x) { cout << "Param constructor B" << x << endl; }     // compiled as B(int x) : A()
    B(int x, int y) : A(x + 3) { cout << "Param constructor B" << x << endl; }   // explicitly selects A(int)
};

// Usage
B b1(5);      // output: Constructor A, Param constructor B5
B b2(5, 0);   // output: Param constructor A8, Param constructor B5
```

### 3.2 Destructor Chain

Destructors are called in **reverse** order of construction. The derived destructor runs first, then the base destructor.

```cpp
class Base {
public:
    ~Base() { cout << "~Base()" << endl; }
};

class Derived : public Base {
public:
    ~Derived() { cout << "~Derived()" << endl; }
};

// When a Derived object goes out of scope:
//   ~Derived()
//   ~Base()
```

| Order | Construction | Destruction |
|:------|:------------|:-----------|
| First | Base constructor | Derived destructor |
| Second | Derived constructor | Base destructor |

*Table 10. Execution order of constructors and destructors*

### 3.3 Example with Output Trace

```cpp
#include <iostream>
using namespace std;

class Employee {
    string name;
protected:
    double salary;
    string department;
public:
    Employee(string n, double sal, string dept)
        : name(n), salary(sal), department(dept) {
        cout << "Employee constructor" << endl;
    }
    ~Employee() { cout << "Employee destructor" << endl; }
    string getName() { return name; }
    double getSalary() { return salary; }
    string getDepartment() { return department; }
};

class Manager : public Employee {
    int teamSize;
public:
    Manager(string n, double sal, string dept, int ts)
        : Employee(n, sal, dept), teamSize(ts) {
        cout << "Manager constructor" << endl;
    }
    ~Manager() { cout << "Manager destructor" << endl; }
    double getBonus() {
        return salary * 0.15;
    }
    void display() {
        cout << "Name: " << getName() << endl;
        cout << "Department: " << department << endl;
        cout << "Salary: " << salary << endl;
        cout << "Team size: " << teamSize << endl;
        cout << "---------------" << endl;
        cout << "Bonus: " << getBonus() << endl << endl;
    }
};

int main() {
    Manager m("Alice", 75000, "Engineering", 8);
    m.display();
    return 0;
}
```

**Output:**

```text
Employee constructor
Manager constructor
Name: Alice
Department: Engineering
Salary: 75000
Team size: 8
---------------
Bonus: 11250

Manager destructor
Employee destructor
```

> **Key Point:** Construction proceeds from base to derived. Destruction proceeds from derived to base. This ensures that derived members that depend on base members are properly initialized and cleaned up.

### 3.4 Constructors and Destructors in Multilevel Inheritance

With several levels of inheritance, constructors run from the topmost base class and destructors from the bottommost derived class. When `C c;` is declared, the compiler compiles `C()` to call `B()` first and `B()` to call `A()` first, so the calls happen in the order C, B, A, but the constructor bodies run in the order A, B, C.

```cpp
#include <iostream>
using namespace std;

class A {
public:
    A() { cout << "Constructor A" << endl; }
    ~A() { cout << "Destructor A" << endl; }
};

class B : public A {
public:
    B() { cout << "Constructor B" << endl; }
    ~B() { cout << "Destructor B" << endl; }
};

class C : public B {
public:
    C() { cout << "Constructor C" << endl; }
    ~C() { cout << "Destructor C" << endl; }
};

int main() {
    C c;        // c is created
    return 0;   // c is destroyed
}
```

The output is as follows.

```text
Constructor A
Constructor B
Constructor C
Destructor C
Destructor B
Destructor A
```

Constructor arguments are also passed upward through the initializer list of each level. In the following example, the constructor of `SmartTV` passes 32 to `WideTV(size, true)`, and the constructor of `WideTV` passes it on to `TV(size)`.

```cpp
#include <iostream>
#include <string>
using namespace std;

class TV {
    int size;   // screen size
public:
    TV() { size = 20; }
    TV(int size) { this->size = size; }
    int getSize() { return size; }
};

class WideTV : public TV {   // WideTV inherits from TV
    bool videoIn;
public:
    WideTV(int size, bool videoIn) : TV(size) {
        this->videoIn = videoIn;
    }
    bool getVideoIn() { return videoIn; }
};

class SmartTV : public WideTV {   // SmartTV inherits from WideTV
    string ipAddr;   // internet address
public:
    SmartTV(string ipAddr, int size) : WideTV(size, true) {
        this->ipAddr = ipAddr;
    }
    string getIpAddr() { return ipAddr; }
};

int main() {
    // creates a 32-inch smart TV object with internet address "192.0.0.1"
    SmartTV htv("192.0.0.1", 32);
    cout << "size=" << htv.getSize() << endl;
    cout << "videoIn=" << boolalpha << htv.getVideoIn() << endl;
    cout << "IP=" << htv.getIpAddr() << endl;
}
```

The output is as follows.

```text
size=32
videoIn=true
IP=192.0.0.1
```

Inside the `htv` object, `size` in the TV part is 32, `videoIn` in the WideTV part is `true`, and `ipAddr` in the SmartTV part is `"192.0.0.1"`. `boolalpha` is a manipulator that makes `bool` values print as `true` and `false` instead of 1 and 0.

---

<br>

## 4. Member Function Overriding

### 4.1 Overriding

When a derived class defines a function with the **same name and signature** as a base class function, the derived version **hides** the base version.

```cpp
class Base {
public:
    void show() { cout << "Base::show()" << endl; }
};

class Derived : public Base {
public:
    void show() { cout << "Derived::show()" << endl; }   // overrides Base::show()
};

int main() {
    Derived d;
    d.show();   // calls Derived::show()
}
```

| Behavior | Explanation |
|:---------|:-----------|
| Same name and parameters | Derived version hides the base version |
| Called through derived object | The derived version is invoked |
| Base version is still present | It exists but is hidden from direct access |

*Table 11. Behavior when a derived class redefines a function of the same name*

> **Caution:** **Redefinition and overriding.** The textbook calls redefining a non-virtual function with the same form in a derived class **function redefinition**, and uses **overriding** for redefining a virtual function, covered in [Chapter 9](../Ch09_Virtual-Functions-and-Abstract-Classes/Concepts_Lecture.md). With redefinition, the function to call is decided by the declared type of the pointer or object, so if a base class pointer points to a `Derived` object as in Section 1.4 and calls `show()`, `Base::show()` runs. Also, name hiding happens by name alone regardless of parameters, so if the derived class has a function with the same name, base class functions of that name with different parameters cannot be called directly through a derived class object either.

### 4.2 Calling the Base Version

Use the scope resolution operator `Base::functionName()` to explicitly call the base class version from the derived class.

```cpp
class Derived : public Base {
public:
    void show() {
        Base::show();   // explicitly calls the base version
        cout << "Derived::show()" << endl;
    }
};
```

You can also call it from outside the class on a derived object.

```cpp
Derived d;
d.Base::show();   // calls Base::show() directly
```

---

<br>

## 5. Multiple Inheritance

C++ supports **multiple inheritance**, where a class inherits from more than one base class.

```cpp
class A {
public:
    void funcA() { cout << "A" << endl; }
};

class B {
public:
    void funcB() { cout << "B" << endl; }
};

class C : public A, public B {
    // C inherits from both A and B
};

int main() {
    C obj;
    obj.funcA();   // from A
    obj.funcB();   // from B
}
```

Member functions of the derived class can also call members of both base classes directly. For example, `dial()` of `MusicPhone`, which inherits from both `MP3` and `MobilePhone`, can play music with `play()` of `MP3` and make a call with `sendCall()` of `MobilePhone`.

**Diamond problem:** When two base classes share a common ancestor, the derived class may inherit **two copies** of the ancestor's members.

```text
      Base
     /    \
    A      B
     \    /
       C        ← C has TWO copies of Base members
```

*Figure 3. Diamond-shaped multiple inheritance*

Accessing a member of `Base` through a `C` object is then ambiguous between the two copies, so it is a compile error.

**Virtual inheritance** resolves this by ensuring only one shared copy of the common base. If `virtual` is placed before the base class when declaring the derived classes, the members of the virtual base class are created only once when an object of the derived class is created.

```cpp
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B { };   // only ONE copy of Base
```

> **Key Point:** Multiple inheritance adds complexity and can cause ambiguity. It is discouraged in most practical situations, and composition or single inheritance is preferred when possible.

---

<br>

## 6. Modern Inheritance Features (C++11)

### 6.1 The `final` Keyword

C++11 introduced the `final` specifier, which makes a class **no longer inheritable** or a virtual function **no longer overridable**.

**Preventing class inheritance:**

```cpp
class Base final {
    // ...
};

// class Derived : public Base { };  // ERROR: Base is marked final
```

**Preventing function overriding:**

```cpp
class Animal {
public:
    virtual void speak() { }
};

class Dog : public Animal {
public:
    void speak() final { cout << "Woof!" << endl; }  // no further override allowed
};

class Puppy : public Dog {
public:
    // void speak() { }   // ERROR: Dog::speak() is marked final
};
```

| Usage | Effect |
|:------|:-------|
| `class MyClass final { };` | No class can inherit from `MyClass` |
| `void func() final;` | No derived class can override `func()` |

*Table 12. Usage and effect of the `final` specifier*

`final` is useful for:
- **Safety:** It prevents unintended extension of a critical class.
- **Performance:** The compiler can devirtualize calls to `final` methods, avoiding the virtual function overhead.

### 6.2 The `override` Keyword

C++11 also introduced the `override` specifier. It tells the compiler that a function is intended to override a base class virtual function. If the function does not actually override anything (e.g., due to a typo in the function name or parameter mismatch), the compiler will produce an error.

```cpp
class Base {
public:
    virtual void display() const { }
};

class Derived : public Base {
public:
    void display() const override { }   // OK: correctly overrides Base::display()
    // void dispaly() const override { } // ERROR: typo, no such function in Base to override
};
```

> **Note:** **Tip.** It is good practice to always use `override` when overriding virtual functions. It catches subtle bugs at compile time. This keyword is covered in detail in [Chapter 9 (Virtual Functions and Abstract Classes)](../Ch09_Virtual-Functions-and-Abstract-Classes/Concepts_Lecture.md).

### 6.3 Inheriting Constructors with `using`

C++11 allows a derived class to **inherit** the constructors of the base class through a `using` declaration, avoiding the need to manually write forwarding constructors.

```cpp
class Base {
public:
    Base(int x) { cout << "Base(int): " << x << endl; }
    Base(int x, int y) { cout << "Base(int, int): " << x << ", " << y << endl; }
    Base(string s) { cout << "Base(string): " << s << endl; }
};

class Derived : public Base {
public:
    using Base::Base;   // inherit ALL constructors from Base

    // Derived can still add its own constructors
    Derived() : Base(0) { cout << "Derived default" << endl; }
};

int main() {
    Derived d1(42);          // calls Base(int) via inherited constructor
    Derived d2(1, 2);        // calls Base(int, int) via inherited constructor
    Derived d3("hello");     // calls Base(string) via inherited constructor
    Derived d4;              // calls Derived's own default constructor
    return 0;
}
```

The output is as follows. For `d4`, `Derived()` first calls `Base(0)` in its initializer list, so `Base(int): 0` is printed first.

```text
Base(int): 42
Base(int, int): 1, 2
Base(string): hello
Base(int): 0
Derived default
```

**Without `using Base::Base`**, you would need to write a forwarding constructor for each base constructor.

```cpp
// Tedious manual forwarding (pre-C++11 style)
class Derived : public Base {
public:
    Derived(int x) : Base(x) { }
    Derived(int x, int y) : Base(x, y) { }
    Derived(string s) : Base(s) { }
};
```

> **Note:** Inherited constructors only initialize the base class portion. If the derived class has additional member variables that need initialization, you should provide your own constructors or use default member initializers. Also, the copy and move constructors of the base class are not inherited by the `using` declaration.

---

<br>

## Summary

| Concept | Key Summary |
|:--------|:-------------|
| Why inheritance | A derived (child) class inherits the members of a base (parent) class, gaining code reuse, an IS-A relationship, and extensibility. |
| Syntax | It is declared as `class Derived : public Base { };`, and `public` is the most common inheritance type. |
| What is inherited | All members are inherited, but `private` members are not directly accessible, and `protected` members are accessible only from member functions of derived classes. |
| Inheritance and object pointers | Upcasting needs no cast but gives access only to base class members, while downcasting requires an explicit cast. |
| `public` inheritance | Public stays public, protected stays protected, and private is inaccessible. |
| `protected` inheritance | Both public and protected become protected, and it is rarely used. |
| `private` inheritance | All accessible members become private, and it is rarely used. |
| Access table | `private` members of the base are never directly accessible regardless of the inheritance type. |
| Constructor chain | The base constructor runs first and the derived constructor afterward, and `Derived(args) : Base(args)` selects a base constructor explicitly. |
| Implicit default constructor call | If no base constructor is specified, the compiler inserts a call to `: Base()`, so a base class without a default constructor causes a compile error. |
| Destructor chain | The derived destructor runs first and the base destructor afterward, in reverse order of construction. |
| Multilevel inheritance | Constructor bodies run from the topmost base class and destructors from the bottommost derived class, and arguments are passed upward through the initializer lists. |
| Overriding | Redefining a function of the same name in a derived class hides the base version, and the textbook calls this function redefinition when the function is not virtual. |
| Calling base version | It is called as `Base::functionName()` inside the derived class and as `obj.Base::func()` from outside. |
| Multiple inheritance | It is declared as `class C : public A, public B { };`, the diamond problem is resolved with virtual inheritance, and it is discouraged in practice. |
| `final` (C++11) | `class X final` prevents inheritance and `void f() final` prevents overriding, enabling compiler optimizations. |
| `override` (C++11) | It marks an intent to override a base virtual function and catches signature mismatches at compile time. |
| Inheriting constructors (C++11) | `using Base::Base;` inherits the base class constructors, so manual forwarding constructors are unnecessary. |

---

<br>

## Self-Check Questions

1. **Upcasting:** With `Point` and `ColorPoint` from Section 1.4, does `pBase->setColor("Red");` compile after `ColorPoint cp; Point *pBase = &cp;`? Explain why and give a way to set the color of `cp`.

   <details>
   <summary>Answer</summary>

   It does not compile. The type of `pBase` is `Point *`, so only members declared in `Point` are accessible through it, and `setColor()` exists only in `ColorPoint`. Call it directly on the object as `cp.setColor("Red");`, or, since `pBase` actually points to a `ColorPoint` object, downcast first as `((ColorPoint *)pBase)->setColor("Red");`.

   </details>

2. **Passing constructor arguments:** In the following code, after `C c(3, 5, 2);` runs, give the values of `x` in `A`, `y` in `B`, and `m` in `C`, and the order in which the three constructor bodies run.

   ```cpp
   class A {
       int x;
   public:
       A() { x = 0; }
       A(int x) { this->x = x; }
   };
   class B : public A {
       int y;
   public:
       B(int x, int y) : A(x + 5) { this->y = y; }
   };
   class C : public B {
       int m;
   public:
       C(int x, int y, int z) : B(x, y) { m = x * y * z; }
   };
   ```

   <details>
   <summary>Answer</summary>

   `x` is 8, `y` is 5, and `m` is 30. `C(3, 5, 2)` calls `B(3, 5)`, and `B(3, 5)` calls `A(3 + 5)`, that is, `A(8)`, so the calls happen in the order C, B, A. The constructor bodies run with A first, storing `x = 8`, then B storing `y = 5`, and finally C storing `m = 3 * 5 * 2 = 30`.

   </details>

3. **`private` inheritance:** Pick every line in the following code that causes a compile error and explain why.

   ```cpp
   class Base {
       int a;
   protected:
       void setA(int a) { this->a = a; }
   public:
       void showA() { cout << a; }
   };
   class Derived : private Base {
       int b;
   protected:
       void setB(int b) { this->b = b; }
   public:
       void showB() { cout << b; }
   };
   int main() {
       Derived x;
       x.a = 5;        // (1)
       x.setA(10);     // (2)
       x.showA();      // (3)
       x.b = 10;       // (4)
       x.setB(10);     // (5)
       x.showB();      // (6)
   }
   ```

   <details>
   <summary>Answer</summary>

   Lines (1) through (5) are all errors, and only (6) compiles. `a` in (1) is a private member of `Base`, so it is not accessible anywhere. `setA()` and `showA()` in (2) and (3) became private inside `Derived` through private inheritance, so `main()` cannot call them. `b` in (4) is a private member of `Derived`, and `setB()` in (5) is a protected member of `Derived`, so the external function `main()` cannot access them. Only `showB()` in (6) is a public member of `Derived`. Changing the inheritance type to `protected` produces errors on the same lines.

   </details>

4. **Diamond problem:** `class Base { public: int x; };` is inherited by `class A : public Base {};` and `class B : public Base {};`, and `class C : public A, public B {};` uses multiple inheritance. Explain why `C c; c.x = 1;` does not compile and how to fix it.

   <details>
   <summary>Answer</summary>

   A `C` object contains two `Base` subobjects, one through `A` and one through `B`, so it is ambiguous which `Base`'s `x` `c.x` refers to, and compilation fails. Declaring `A` and `B` with virtual inheritance, as `class A : virtual public Base {};` and `class B : virtual public Base {};`, creates `Base` only once in a `C` object, so `c.x` refers to a single member.

   </details>

5. **Redefinition and pointers:** When `Base` and `Derived` each define a non-virtual `show()` as in Section 4.1, what does `Derived d; Base *p = &d; p->show();` print?

   <details>
   <summary>Answer</summary>

   It prints `Base::show()`. For a non-virtual function, the function to call is decided by the declared type of the pointer, so even though `p` points to a `Derived` object, `Base::show()` is called because `p` is a `Base *`. Calling `Derived::show()` according to the actual type of the object requires the virtual functions of Chapter 9.

   </details>
