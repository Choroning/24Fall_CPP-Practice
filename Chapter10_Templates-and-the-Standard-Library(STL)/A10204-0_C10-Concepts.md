# Chapter 10 — Templates and the Standard Library (STL)

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpoom C++ Programming (Revised Edition)* — Chapter 10

---

## Table of Contents

- [1. Function Templates](#1-function-templates)
  - [1.1 Generic Programming](#11-generic-programming)
  - [1.2 Syntax](#12-syntax)
  - [1.3 Template Instantiation](#13-template-instantiation)
  - [1.4 Multiple Template Parameters](#14-multiple-template-parameters)
  - [1.5 Examples](#15-examples)
- [2. Class Templates](#2-class-templates)
  - [2.1 Syntax](#21-syntax)
  - [2.2 Instantiation](#22-instantiation)
  - [2.3 Operator Overloading in Templates](#23-operator-overloading-in-templates)
  - [2.4 Default Template Parameters](#24-default-template-parameters)
- [3. STL Overview](#3-stl-overview)
  - [3.1 Containers](#31-containers)
  - [3.2 Iterators](#32-iterators)
  - [3.3 Algorithms](#33-algorithms)
  - [3.4 vector Example](#34-vector-example)
- [4. Template Compilation Notes](#4-template-compilation-notes)
  - [4.1 Template Definition Must Be in Header](#41-template-definition-must-be-in-header)
  - [4.2 Common Errors](#42-common-errors)
- [Summary](#summary)

---

<br>

## 1. Function Templates

### 1.1 Generic Programming

**Generic programming** allows you to write a function once and have it work for **any type**. Without templates, you would need to duplicate the same logic for each type (`int`, `double`, `char`, etc.).

```cpp
// Without templates — duplicated code
int maxInt(int a, int b) { return (a > b) ? a : b; }
double maxDouble(double a, double b) { return (a > b) ? a : b; }
char maxChar(char a, char b) { return (a > b) ? a : b; }
```

Templates eliminate this redundancy by letting the compiler generate type-specific versions automatically.

### 1.2 Syntax

A function template is declared with the `template` keyword followed by a **template parameter list** in angle brackets.

```cpp
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}
```

- `template <typename T>` — declares a template with one type parameter `T`.
- `typename` and `class` are interchangeable in this context: `template <class T>` is equivalent.
- `T` is a **placeholder** that will be replaced with the actual type when the function is called.

### 1.3 Template Instantiation

When a template function is called with a specific type, the compiler generates a **concrete function** for that type. This process is called **instantiation**.

**Implicit instantiation** — the compiler deduces `T` from the argument types:

```cpp
int result1 = myMax(3, 7);        // T deduced as int → myMax<int>(3, 7)
double result2 = myMax(3.5, 7.2); // T deduced as double → myMax<double>(3.5, 7.2)
```

**Explicit instantiation** — the programmer specifies `T` directly:

```cpp
double result3 = myMax<double>(3, 7.2);  // T explicitly set to double
```

> **Key Point:** Each distinct type used with a template causes the compiler to generate a separate concrete function. `myMax<int>` and `myMax<double>` are two different functions in the compiled binary.

### 1.4 Multiple Template Parameters

A template can have more than one type parameter:

```cpp
template <typename T1, typename T2>
void printPair(T1 a, T2 b) {
    cout << a << ", " << b << endl;
}

printPair(10, 3.14);       // T1=int, T2=double
printPair("Hello", 42);    // T1=const char*, T2=int
```

### 1.5 Examples

**Generic swap:**

```cpp
template <typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int x = 1, y = 2;
mySwap(x, y);       // x=2, y=1

string s1 = "hi", s2 = "bye";
mySwap(s1, s2);     // s1="bye", s2="hi"
```

**Generic print array:**

```cpp
template <typename T>
void printArray(T arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << "  ";
    }
    cout << endl;
}

int nums[] = {1, 2, 3, 4};
printArray(nums, 4);        // 1  2  3  4

double vals[] = {1.1, 2.2, 3.3};
printArray(vals, 3);        // 1.1  2.2  3.3
```

**Generic reverse array:**

```cpp
template <typename T>
void reverseArray(T arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        T temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

int data[] = {1, 10, 100, 1000};
reverseArray(data, 4);      // {1000, 100, 10, 1}
```

---

<br>

## 2. Class Templates

### 2.1 Syntax

A class template defines a class where one or more types are parameterized. The class definition is prefixed with `template <typename T>`.

```cpp
template <typename T>
class Container {
    T data;
    int size;
public:
    Container(T d, int s) : data(d), size(s) { }
    T getData();
    void setData(T d);
};
```

Member functions defined **outside** the class must repeat the template prefix and use `ClassName<T>::`:

```cpp
template <typename T>
T Container<T>::getData() {
    return data;
}

template <typename T>
void Container<T>::setData(T d) {
    data = d;
}
```

### 2.2 Instantiation

Each time a class template is used with a specific type, the compiler generates a **separate class**.

```cpp
Container<int> c1(42, 1);       // generates Container class for int
Container<string> c2("hello", 5); // generates Container class for string
Container<double> c3(3.14, 1);  // generates Container class for double
```

`Container<int>` and `Container<string>` are entirely different classes — they do not share code at runtime.

### 2.3 Operator Overloading in Templates

Template classes can overload operators just like regular classes. A common need is to overload comparison operators so that template functions like `bigger()` can work with custom types.

```cpp
template <typename T>
class Rectangle {
    T width, height;
public:
    Rectangle(T w = 1, T h = 1) : width(w), height(h) { }
    T getArea() { return width * height; }
    bool operator>(const Rectangle<T>& other) const {
        return this->getArea() > other.getArea();
    }
};

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

Rectangle<double> r1(10.2, 23.5);
Rectangle<double> r2(20.4, 13.7);
Rectangle<double> r3 = bigger(r1, r2);  // uses operator> to compare
cout << r3.getArea() << endl;            // 279.48 (area of r2)
```

> **Key Point:** When using a template function that relies on operators (like `>`, `==`, `<`), the type `T` must have those operators defined. For built-in types this is automatic; for custom classes you must overload them.

### 2.4 Default Template Parameters

Template parameters can have **default values**, just like function parameters:

```cpp
template <typename T = int>
class Stack {
    T data[100];
    int top;
public:
    Stack() : top(-1) { }
    void push(T item) { data[++top] = item; }
    T pop() { return data[top--]; }
};

Stack<> s1;            // T defaults to int
Stack<double> s2;      // T is double
Stack<string> s3;      // T is string
```

> **Note:** When using the default, you still need the angle brackets: `Stack<>`, not just `Stack`.

---

<br>

## 3. STL Overview

The **Standard Template Library (STL)** is a core part of the C++ standard library. It provides generic, reusable components organized into three main categories: **containers**, **iterators**, and **algorithms**.

### 3.1 Containers

Containers are data structures that store collections of objects.

| Container | Header | Description |
|:----------|:-------|:------------|
| `vector` | `<vector>` | Dynamic array; contiguous memory; fast random access; resizable |
| `list` | `<list>` | Doubly-linked list; fast insert/delete anywhere; no random access |
| `map` | `<map>` | Key-value pairs sorted by key; fast lookup by key (`O(log n)`) |
| `set` | `<set>` | Sorted unique elements; fast membership test (`O(log n)`) |

```cpp
#include <vector>
#include <map>

vector<int> nums = {1, 2, 3};
map<string, int> ages = {{"Alice", 20}, {"Bob", 22}};
```

### 3.2 Iterators

An **iterator** is an abstract pointer that provides a uniform way to traverse any container. Each container provides its own iterator type.

```cpp
vector<int> v = {10, 20, 30};

vector<int>::iterator it;
for (it = v.begin(); it != v.end(); it++) {
    cout << *it << " ";    // dereference iterator like a pointer
}
// Output: 10 20 30
```

| Operation | Meaning |
|:----------|:--------|
| `begin()` | Iterator to the first element |
| `end()` | Iterator past the last element |
| `*it` | Dereference — access the element |
| `it++` | Advance to the next element |

### 3.3 Algorithms

The `<algorithm>` header provides generic functions that work with iterators.

| Algorithm | Description | Example |
|:----------|:------------|:--------|
| `sort` | Sort elements in a range | `sort(v.begin(), v.end());` |
| `find` | Find an element in a range | `find(v.begin(), v.end(), 20);` |
| `for_each` | Apply a function to each element | `for_each(v.begin(), v.end(), print);` |

```cpp
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int main() {
    vector<int> v = {30, 10, 20};

    sort(v.begin(), v.end());    // {10, 20, 30}

    auto it = find(v.begin(), v.end(), 20);
    if (it != v.end()) {
        cout << "Found: " << *it << endl;  // Found: 20
    }

    return 0;
}
```

### 3.4 vector Example

`vector` is the most commonly used STL container. It is a dynamic array that grows automatically.

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;

    // Adding elements
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // Size
    cout << "Size: " << v.size() << endl;     // 3

    // Access by index
    cout << "v[0] = " << v[0] << endl;        // 10
    cout << "v[1] = " << v[1] << endl;        // 20

    // Iterator loop
    cout << "Iterator: ";
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;    // 10 20 30

    // Range-based for (C++11)
    cout << "Range-based: ";
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;    // 10 20 30

    return 0;
}
```

| Operation | Description |
|:----------|:------------|
| `push_back(val)` | Add element to the end |
| `size()` | Return number of elements |
| `v[i]` | Access element at index `i` |
| `begin()` / `end()` | Iterators for traversal |
| `pop_back()` | Remove last element |
| `clear()` | Remove all elements |

---

<br>

## 4. Template Compilation Notes

### 4.1 Template Definition Must Be in Header

Unlike regular classes, **template definitions cannot be separated** into `.h` and `.cpp` files in the usual way. The compiler needs to see the full template definition at the point of instantiation to generate the concrete code.

```cpp
// MyTemplate.h — CORRECT: put everything in the header
template <typename T>
class MyTemplate {
    T data;
public:
    MyTemplate(T d) : data(d) { }
    T getData();
};

template <typename T>
T MyTemplate<T>::getData() {
    return data;     // definition must also be in the header
}
```

If you put the template member function definitions in a `.cpp` file, the linker will produce an **"undefined reference"** error because the compiler cannot instantiate the template when it compiles other translation units.

```
// BAD: MyTemplate.cpp — will cause linker error
template <typename T>
T MyTemplate<T>::getData() {
    return data;
}
// Other .cpp files using MyTemplate<int> will fail to link!
```

> **Rule:** Keep both template class declarations and member function definitions in the **header file**.

### 4.2 Common Errors

**Missing operator for custom types:**

```cpp
template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;   // requires operator> for type T
    else return b;
}

class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) { }
};

Point p1(1, 2), p2(3, 4);
// bigger(p1, p2);   // compile error — Point has no operator>
```

To fix this, overload `operator>` in the `Point` class:

```cpp
class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) { }
    bool operator>(const Point& other) const {
        return (x + y) > (other.x + other.y);
    }
};

Point p1(1, 2), p2(3, 4);
Point p3 = bigger(p1, p2);  // now works — uses operator>
```

**Type mismatch:**

```cpp
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}

// myMax(3, 7.5);    // compile error — T cannot be both int and double
myMax<double>(3, 7.5);  // OK — explicitly set T to double
```

> **Tip:** When a template function is called with arguments of different types and the template has a single type parameter, the compiler cannot deduce `T`. Either use explicit instantiation or add a second template parameter.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Generic programming | Write once, works for any type; eliminates code duplication |
| Function template syntax | `template <typename T>` before function; `T` is type placeholder |
| Template instantiation | Compiler generates concrete function per type; implicit or explicit |
| Multiple parameters | `template <typename T1, typename T2>` for different types |
| Class template syntax | `template <typename T> class Name { };` member definitions use `Name<T>::` |
| Class instantiation | `Name<int> obj;` generates a separate class for each type |
| Operator overloading in templates | Custom types must define operators used by template logic (e.g., `operator>`) |
| Default template parameters | `template <typename T = int>` — default type if none specified |
| STL containers | `vector`, `list`, `map`, `set` — generic data structures |
| Iterators | Abstract pointers for uniform container traversal (`begin()`, `end()`, `*it`) |
| Algorithms | `sort`, `find`, `for_each` — generic functions on iterator ranges |
| `vector` | Dynamic array; `push_back`, `size`, `[]`, iterator and range-based for loops |
| Templates in headers | Full template definition must be in the header; `.cpp` separation causes linker errors |
| Common errors | Missing operators for custom types; type mismatch with single template parameter |

---
