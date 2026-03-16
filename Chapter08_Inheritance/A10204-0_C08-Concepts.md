# Chapter 08 — Inheritance

> **Last Updated:** 2026-03-16

---

## Table of Contents

- [1. Inheritance Concepts](#1-inheritance-concepts)
  - [1.1 Why Inheritance](#11-why-inheritance)
  - [1.2 Syntax](#12-syntax)
  - [1.3 What Is Inherited](#13-what-is-inherited)
- [2. Access Control with Inheritance](#2-access-control-with-inheritance)
  - [2.1 `public` Inheritance](#21-public-inheritance)
  - [2.2 `protected` Inheritance](#22-protected-inheritance)
  - [2.3 `private` Inheritance](#23-private-inheritance)
  - [2.4 Access Table](#24-access-table)
- [3. Constructor and Destructor in Inheritance](#3-constructor-and-destructor-in-inheritance)
  - [3.1 Constructor Chain](#31-constructor-chain)
  - [3.2 Destructor Chain](#32-destructor-chain)
  - [3.3 Example with Output Trace](#33-example-with-output-trace)
- [4. Member Function Overriding](#4-member-function-overriding)
  - [4.1 Overriding](#41-overriding)
  - [4.2 Calling the Base Version](#42-calling-the-base-version)
- [5. Multiple Inheritance](#5-multiple-inheritance)
- [6. Modern Inheritance Features (C++11)](#6-modern-inheritance-features-c11)
  - [6.1 The `final` Keyword](#61-the-final-keyword)
  - [6.2 The `override` Keyword](#62-the-override-keyword)
  - [6.3 Inheriting Constructors with `using`](#63-inheriting-constructors-with-using)
- [Summary](#summary)

---

<br>

## 1. Inheritance Concepts

### 1.1 Why Inheritance

**Inheritance** lets a new class (derived class) reuse and extend an existing class (base class).

| Benefit | Explanation |
|:--------|:-----------|
| Code reuse | Common data and functions are written once in the base class |
| IS-A relationship | A `Car` **is a** `Vehicle`; a `Manager` **is an** `Employee` |
| Extensibility | New functionality is added in the derived class without modifying the base |

**Terminology:**

| Term | Synonym |
|:-----|:--------|
| Base class | Parent class, Super class |
| Derived class | Child class, Sub class |

```
   Base (Parent)
       ↑
   Derived (Child)
```

### 1.2 Syntax

```cpp
class Derived : public Base {
    // additional members
};
```

- The colon `:` followed by the access specifier (`public`, `protected`, or `private`) and the base class name.
- **`public` inheritance** is the most common and preserves the IS-A relationship.

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
| `public` | Yes — directly accessible |
| `protected` | Yes — directly accessible |
| `private` | **No** — inherited but NOT directly accessible; must use public/protected getters |

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

> **Key Point:** `private` members are inherited (they exist in memory) but cannot be accessed directly from the derived class. Use getters defined in the base class to access them.

---

<br>

## 2. Access Control with Inheritance

The **inheritance access specifier** determines how base class members are exposed in the derived class.

### 2.1 `public` Inheritance

The most common form. Base access levels are preserved in the derived class.

```cpp
class Derived : public Base { };
```

| Base member | Becomes in Derived |
|:------------|:-------------------|
| `public` | `public` |
| `protected` | `protected` |
| `private` | inaccessible |

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

### 2.4 Access Table

| Base member | `public` inheritance | `protected` inheritance | `private` inheritance |
|:------------|:--------------------|:----------------------|:---------------------|
| `public` | `public` | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private` | inaccessible | inaccessible | inaccessible |

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

### 3.2 Destructor Chain

Destructors are called in **reverse** order of construction: the derived destructor runs first, then the base destructor.

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

---

<br>

## 4. Member Function Overriding

### 4.1 Overriding

When a derived class defines a function with the **same name and signature** as a base class function, the derived version **hides** (overrides) the base version.

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

You can also call it from outside the class on a derived object:

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

**Diamond problem:** When two base classes share a common ancestor, the derived class may inherit **two copies** of the ancestor's members.

```
      Base
     /    \
    A      B
     \    /
       C        ← C has TWO copies of Base members
```

**Virtual inheritance** resolves this by ensuring only one shared copy of the common base:

```cpp
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B { };   // only ONE copy of Base
```

> **Key Point:** Multiple inheritance adds complexity and can cause ambiguity. It is discouraged in most practical situations. Prefer composition or single inheritance when possible.

---

<br>

## 6. Modern Inheritance Features (C++11)

### 6.1 The `final` Keyword

C++11 introduced the `final` specifier to **prevent further inheritance** of a class or **prevent further overriding** of a virtual function.

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

`final` is useful for:
- **Security:** Preventing unintended extension of a critical class.
- **Performance:** The compiler can devirtualize calls to `final` methods, avoiding the virtual function overhead.

### 6.2 The `override` Keyword

C++11 also introduced the `override` specifier, which tells the compiler that a function is **intended to override** a base class virtual function. If the function does not actually override anything (e.g., due to a typo in the function name or parameter mismatch), the compiler will produce an error.

```cpp
class Base {
public:
    virtual void display() const { }
};

class Derived : public Base {
public:
    void display() const override { }   // OK: correctly overrides Base::display()
    // void dispaly() const override { } // ERROR: typo — no such function in Base to override
};
```

> **Tip:** Always use `override` when overriding virtual functions. It catches subtle bugs at compile time. This keyword is covered in detail in Chapter 09 (Virtual Functions and Abstract Classes).

### 6.3 Inheriting Constructors with `using`

C++11 allows a derived class to **inherit all constructors** from the base class using a `using` declaration, avoiding the need to manually write forwarding constructors.

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

**Without `using Base::Base`**, you would need to write a forwarding constructor for each base constructor:

```cpp
// Tedious manual forwarding (pre-C++11 style)
class Derived : public Base {
public:
    Derived(int x) : Base(x) { }
    Derived(int x, int y) : Base(x, y) { }
    Derived(string s) : Base(s) { }
};
```

> **Note:** Inherited constructors only initialize the base class portion. If the derived class has additional member variables that need initialization, you should provide your own constructors or use default member initializers.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Why inheritance | Code reuse, IS-A relationship, extensibility; base (parent) class to derived (child) class |
| Syntax | `class Derived : public Base { };` — `public` is the most common inheritance type |
| What is inherited | All members inherited; `private` not directly accessible; `protected` accessible from derived |
| `public` inheritance | public remains public, protected remains protected, private is inaccessible |
| `protected` inheritance | public becomes protected, protected stays protected; rarely used |
| `private` inheritance | all accessible become private; rarely used |
| Access table | `private` members of the base are never directly accessible regardless of inheritance type |
| Constructor chain | Base constructor called first, then derived; use `Derived(args) : Base(args)` for explicit call |
| Destructor chain | Derived destructor first, then base; reverse order of construction |
| Overriding | Derived redefines base function with same signature; hides base version |
| Calling base version | `Base::functionName()` from inside derived; `obj.Base::func()` from outside |
| Multiple inheritance | `class C : public A, public B { };` — diamond problem; virtual inheritance; discouraged in practice |
| `final` (C++11) | `class X final` prevents inheritance; `void f() final` prevents overriding; enables compiler optimizations |
| `override` (C++11) | Marks a function as intentionally overriding a base virtual function; catches signature mismatches at compile time |
| Inheriting constructors (C++11) | `using Base::Base;` inherits all base constructors; avoids manual forwarding constructors |

---
