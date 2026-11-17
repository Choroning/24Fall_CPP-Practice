# Chapter 9 Lab — Using Virtual Functions and Abstract Classes

[Repository Guide](../../README.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **Last Updated:** 2026-11-18

> **Learning Objectives:**
> 1. Write a derived class that overrides pure virtual functions to fit the execution flow defined by an abstract class
> 2. Handle objects of different derived classes through an array of base class pointers and explain the results of dynamic binding
> 3. Turn a base class into an abstract class by changing a virtual function into a pure virtual function
> 4. Write a dynamic-array-based stack that implements an interface defined by an abstract class

---

## Table of Contents

- [1. Overview](#1-overview)
- [2. A Unit Converter](#2-a-unit-converter)
- [3. Shapes Inscribed in a Rectangle](#3-shapes-inscribed-in-a-rectangle)
  - [3.1 Computing Areas with a Virtual Function](#31-computing-areas-with-a-virtual-function)
  - [3.2 Turning Shape into an Abstract Class](#32-turning-shape-into-an-abstract-class)
- [4. An Abstract Stack and an Integer Stack](#4-an-abstract-stack-and-an-integer-stack)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overview

> **Note:** The theory of upcasting, virtual functions and dynamic binding, overriding, virtual destructors, and pure virtual functions and abstract classes is covered in the [Chapter 9 lecture notes](Concepts_Lecture.md). This lab solves three problems that inherit a given base class or abstract class and override its virtual functions.

| Topic | Location |
|:-----|:-----|
| `WonToDollar`, which inherits the abstract class `Converter` and converts won to dollars | Section 2 |
| `Oval`, `Rect`, and `Triangular`, which inherit `Shape`, and `Shape` turned into an abstract class | Section 3 |
| `IntStack`, which implements the abstract class `AbstractStack` | Section 4 |
| A `Student` hierarchy that computes averages by year with virtual functions, unit conversion that inherits the abstract class `Convert` | [Chapter 9 Exercises 1 and 2](Exercises.md) |

*Table 1. Topics of the lab and their location*

All code is compiled and run with the C++17 standard (for example, `g++ -std=c++17 -Wall`).

---

<br>

## 2. A Unit Converter

**Problem:** The abstract class `Converter` that converts units is as follows. Inherit it and write a `WonToDollar` class that converts Korean won into dollars. The constructor's argument is the number of won equal to one dollar.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Converter {
protected:
    double ratio;
    virtual double convert(double src) = 0;     // Convert src to the other unit
    virtual string getSourceString() = 0;       // Name of the source unit
    virtual string getDestString() = 0;         // Name of the destination unit
public:
    Converter(double ratio) { this->ratio = ratio; }
    void run() {
        double src;
        cout << getSourceString() << "을 " << getDestString() << "로 바꿉니다. ";
        cout << getSourceString() << "을 입력하세요>> ";
        cin >> src;
        cout << "변환 결과 : " << convert(src) << getDestString() << endl;
    }
};

class WonToDollar : public Converter {
public:
    WonToDollar(double ratio) : Converter(ratio) { }
protected:
    double convert(double src) override { return src / ratio; }   // won / (won per dollar)
    string getSourceString() override { return "원"; }
    string getDestString() override { return "달러"; }
};

int main() {
    WonToDollar wd(1100);               // 1100 won per dollar
    wd.run();
}
```

```text
원을 달러로 바꿉니다. 원을 입력하세요>> 100000
변환 결과 : 90.9091달러
```

- **A base class that sets the framework:** `run()` fixes the flow of reading input, converting, and printing, and leaves only the parts that differ by unit (the conversion formula and the unit names) as pure virtual functions. A derived class needs to override only those three functions. (The output reads "Converting won to dollars. Enter won>> 100000" and "Result: 90.9091 dollars.")
- **Dynamic binding:** `run()` is a function of `Converter`, but the call `convert(src)` inside `wd.run()` is to a virtual function, so it is bound to the `convert()` of the actual object, `WonToDollar`. 100000 / 1100 ≈ 90.9091.
- **Access specifiers:** The three virtual functions are `protected`, so they cannot be called directly from outside and are used only through `run()`. The overriding functions are placed under `protected` for the same reason. `override` lets the compiler confirm that the function overrides a virtual function of the base class (Section 2.6 of the Chapter 9 lecture notes).

---

<br>

## 3. Shapes Inscribed in a Rectangle

The following `Shape` represents a shape inscribed in a rectangle of width `width` and height `height`.

```cpp
class Shape {
protected:
    string name;                        // Name of the shape
    int width, height;                  // Width and height of the rectangle the shape is inscribed in
public:
    Shape(string n = "", int w = 0, int h = 0) {
        name = n; width = w; height = h;
    }
    virtual double getArea() { return 0; }      // Returns a meaningless value
    string getName() { return name; }
};
```

### 3.1 Computing Areas with a Virtual Function

**Problem:** Inherit `Shape` and write an ellipse class `Oval`, a rectangle class `Rect`, and a triangle class `Triangular` so that the following `main()` prints as in the sample run.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Shape {
protected:
    string name;                        // Name of the shape
    int width, height;                  // Width and height of the rectangle the shape is inscribed in
public:
    Shape(string n = "", int w = 0, int h = 0) {
        name = n; width = w; height = h;
    }
    virtual double getArea() { return 0; }      // Returns a meaningless value
    string getName() { return name; }
    virtual ~Shape() { }
};

class Oval : public Shape {
public:
    Oval(string n, int w, int h) : Shape(n, w, h) { }
    double getArea() override { return 3.14 * width * height; }
};

class Rect : public Shape {
public:
    Rect(string n, int w, int h) : Shape(n, w, h) { }
    double getArea() override { return width * height; }
};

class Triangular : public Shape {
public:
    Triangular(string n, int w, int h) : Shape(n, w, h) { }
    double getArea() override { return width * height / 2.0; }
};

int main() {
    Shape* p[3];
    p[0] = new Oval("빈대떡", 10, 20);
    p[1] = new Rect("찰떡", 30, 40);
    p[2] = new Triangular("토스트", 30, 40);
    for (int i = 0; i < 3; i++)
        cout << p[i]->getName() << " 넓이는 " << p[i]->getArea() << endl;
    for (int i = 0; i < 3; i++)
        delete p[i];
}
```

```text
빈대떡 넓이는 628
찰떡 넓이는 1200
토스트 넓이는 600
```

- **An array of base class pointers:** All elements of `p` are `Shape*`, but the objects they point to are an `Oval`, a `Rect`, and a `Triangular`. Since `getArea()` is virtual, `p[i]->getArea()` is bound at run time to the function of the actual object and gives a different area for each shape. `getName()` is not virtual, but none of the three classes overrides it, so `Shape`'s function is used as is. (The shape names are 빈대떡, a Korean pancake, 찰떡, a rice cake, and 토스트, toast, and "넓이는" means "the area is.")
- **Adding a virtual destructor:** The given `Shape` has no destructor, but the code above adds `virtual ~Shape() { }`. `delete p[i]` deletes a derived class object through a `Shape*`, and if the base class destructor is not virtual, this is undefined behavior. A class whose derived objects are deleted through base class pointers should have a virtual destructor (Section 3 of the Chapter 9 lecture notes).

> **Note:** The ellipse area 628 is 3.14 × 10 × 20, a value computed by treating `width` and `height` as the two radii of the ellipse. The two radii of the ellipse inscribed in a rectangle of width 10 and height 20 are 5 and 10, so computed by the literal meaning of the names, the area is 3.14 × 5 × 10 = 157. The triangle (600 from base 30 and height 40) and the rectangle (1200) agree with the areas of the inscribed shapes.

### 3.2 Turning Shape into an Abstract Class

**Problem:** Make `Shape` an abstract class and have the `main()` of Section 3.1 run unchanged.

`Shape::getArea()` returned a "meaningless value." Instead of forcing a 0 on a general shape whose area cannot be determined, changing it into a pure virtual function makes `Shape` an abstract class. Only one line of the code in Section 3.1 changes.

```cpp
    virtual double getArea() = 0;               // Pure virtual function: Shape is an abstract class
```

```text
빈대떡 넓이는 628
찰떡 넓이는 1200
토스트 넓이는 600
```

- `main()` creates no `Shape` object and uses only `Shape*`, so it runs unchanged, and the output is the same as in Section 3.1.
- No object can be created from an abstract class, so `Shape s("x", 1, 1);` gives the error `cannot declare variable 's' to be of abstract type 'Shape'`. If a derived class does not override `getArea()`, that derived class also remains abstract and cannot create objects. This has the effect of forcing derived classes to implement the area.

---

<br>

## 4. An Abstract Stack and an Integer Stack

**Problem:** Write an `IntStack` that inherits the following abstract class `AbstractStack`. `IntStack` has as `private` members the stack size `capacity`, the number of stored integers `tos` (top of stack), and the dynamically allocated stack memory `int* p`, and it has a constructor that takes one integer argument, a destructor, and definitions of the inherited pure virtual functions.

```cpp
#include <iostream>
using namespace std;

class AbstractStack {
public:
    virtual bool push(int n) = 0;       // Push n; return false if the stack is full
    virtual bool pop(int& n) = 0;       // Store the popped integer in n; return false if empty
    virtual int size() = 0;             // Return the number of integers currently stored
    virtual ~AbstractStack() { }
};

class IntStack : public AbstractStack {
    int capacity;                       // Size of the stack
    int tos;                            // Number of stored integers (top of stack)
    int* p;                             // Dynamically allocated stack memory
public:
    IntStack(int capacity) {
        this->capacity = capacity;
        tos = 0;
        p = new int[capacity];
    }
    ~IntStack() { delete[] p; }
    bool push(int n) override {
        if (tos == capacity)
            return false;
        p[tos++] = n;                   // Store at position tos and increment tos
        return true;
    }
    bool pop(int& n) override {
        if (tos == 0)
            return false;
        n = p[--tos];                   // Decrement tos and take the value at that position
        return true;
    }
    int size() override { return tos; }
};

int main() {
    IntStack a(2);
    for (int i = 0; i < 4; i++) {       // The first 2 pushes succeed, the next 2 fail
        if (a.push(i))
            cout << "push 성공" << endl;
        else
            cout << "스택 full" << endl;
    }
    cout << "===============" << endl;
    int n;
    for (int i = 0; i < 3; i++) {       // The first 2 pops succeed, the next 1 fails
        if (a.pop(n))
            cout << "pop 성공 " << n << endl;
        else
            cout << "스택 empty" << endl;
    }
}
```

```text
push 성공
push 성공
스택 full
스택 full
===============
pop 성공 1
pop 성공 0
스택 empty
```

- **Separating interface from implementation:** `AbstractStack` defines only what a stack can do (push, pop, size) and not how it stores data. `IntStack` implements it with a dynamic array. If code that uses a stack deals only with `AbstractStack&` or `AbstractStack*`, that code needs no change even when the stack is replaced by another one implemented with a linked list. (성공 means "success" and 스택 means "stack.")
- **The meaning of tos:** `tos` is both the number of stored integers and the index of the next position to fill. `push()` stores into `p[tos]` and increments `tos`, while `pop()` first decrements `tos` and then takes `p[tos]`. The 1 pushed last comes out first, giving last-in first-out (LIFO) order.
- **Delivering results through a reference parameter:** `pop(int& n)` returns the popped value through a reference parameter and success through the return value (Section 4 of the Chapter 5 lab).
- **Virtual destructor:** Because `AbstractStack` has a virtual destructor, deleting an `IntStack` object through an `AbstractStack*` calls `IntStack`'s destructor and releases `p`.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| An abstract class that sets the framework | When the base class fixes the execution flow and leaves only the varying parts as pure virtual functions, derived classes override only those functions. |
| Dynamic binding | Calling a virtual function through a base class pointer selects the function of the actual object at run time. |
| Turning a class abstract | Changing a virtual function that returned a meaningless value into a pure virtual function with `= 0` prevents object creation and forces derived classes to implement it. |
| Virtual destructor | To `delete` a derived object through a base class pointer, the base class destructor must be virtual. |
| Implementing an interface | An abstract class defines only what is to be done, and a derived class implements it with a concrete storage method such as a dynamic array. |

---

<br>

## Self-Check Questions

1. When writing a `CelsiusToFahrenheit` that inherits the `Converter` of Section 2, which functions must be overridden, and what is the formula of `convert()`? How is `ratio` used in this conversion?

   <details>
   <summary>Answer</summary>

   The three pure virtual functions `convert()`, `getSourceString()`, and `getDestString()` must be overridden. The formula converting Celsius c to Fahrenheit is c × 9 / 5 + 32, written as `return src * 9 / 5 + 32;`. This conversion cannot be expressed by a single ratio, so `ratio` is not used, and any value may be passed in the constructor, as in `Converter(0)`. The unit names are returned as, for example, `"섭씨"` (Celsius) and `"화씨"` (Fahrenheit).

   </details>

2. What is printed if `virtual` is removed from `Shape::getArea()` in Section 3.1?

   <details>
   <summary>Answer</summary>

   All three lines print an area of 0. If `getArea()` is not virtual, `p[i]->getArea()` becomes static binding, fixed at compile time to the function of the pointer's type `Shape`, so `Shape::getArea()` returns 0 regardless of the actual object. In that case the `override` on the derived classes' `getArea()` has no virtual function to override and becomes a compile error, so `override` must be removed for the code to compile.

   </details>

3. If the `pop()` loop in the `main()` of Section 4 is changed from `for (int i = 0; i < 3; i++)` to `while (a.size() > 0)`, how does the bottom of the output change?

   <details>
   <summary>Answer</summary>

   The stack holds 1 and 0, so `pop 성공 1` and `pop 성공 0` are printed, and the loop ends when the size reaches 0, so `스택 empty` is not printed. Checking the remaining count with `size()` first avoids calling `pop()` when nothing can be popped.

   </details>
