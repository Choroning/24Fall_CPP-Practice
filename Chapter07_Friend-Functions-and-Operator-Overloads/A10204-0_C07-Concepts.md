# Chapter 07 — Friend Functions and Operator Overloads

> **Last Updated:** 2026-03-16

---

## Table of Contents

- [1. Friend](#1-friend)
  - [1.1 Friend Function](#11-friend-function)
  - [1.2 Friend Class](#12-friend-class)
  - [1.3 Guidelines](#13-guidelines)
- [2. Operator Overloading Basics](#2-operator-overloading-basics)
  - [2.1 Why Operator Overloading?](#21-why-operator-overloading)
  - [2.2 Concept](#22-concept)
  - [2.3 Operators That Can / Cannot Be Overloaded](#23-operators-that-can--cannot-be-overloaded)
  - [2.4 Member Function vs Friend Function — Decision Guide](#24-member-function-vs-friend-function--decision-guide)
- [3. Binary Operator Overloading](#3-binary-operator-overloading)
  - [3.1 As Member Function](#31-as-member-function)
  - [3.2 As Friend Function](#32-as-friend-function)
  - [3.3 Compound Assignment](#33-compound-assignment)
  - [3.4 Comparison Operators](#34-comparison-operators)
- [4. Unary Operator Overloading](#4-unary-operator-overloading)
  - [4.1 As Member Function](#41-as-member-function)
  - [4.2 As Friend Function](#42-as-friend-function)
  - [4.3 Increment / Decrement](#43-increment--decrement)
  - [4.4 Prefix vs Postfix — Detailed Trace](#44-prefix-vs-postfix--detailed-trace)
- [5. Stream Operator Overloading](#5-stream-operator-overloading)
  - [5.1 `operator<<`](#51-operator)
  - [5.2 `operator>>`](#52-operator-1)
- [6. Subscript Operator](#6-subscript-operator)
- [7. Complete Runnable Examples](#7-complete-runnable-examples)
  - [7.1 Complete Vector2D Class](#71-complete-vector2d-class)
  - [7.2 Prefix vs Postfix Output Trace](#72-prefix-vs-postfix-output-trace)
  - [7.3 Why operator<< Must Be a Friend](#73-why-operator-must-be-a-friend)
- [8. Common Mistakes](#8-common-mistakes)
- [Summary](#summary)

---

<br>

## 1. Friend

### 1.1 Friend Function

A **friend function** is a non-member function that is granted access to the `private` and `protected` members of a class. It is declared inside the class with the `friend` keyword, but it is **not** a member function.

```cpp
class Box {
    int width;
public:
    Box(int w) : width(w) {}
    friend void printWidth(const Box &b);   // friend declaration
};

// Definition — NOT a member function (no Box:: scope)
void printWidth(const Box &b) {
    cout << b.width << endl;   // direct access to private member
}
```

| Property | Detail |
|:---------|:-------|
| Declared with `friend` inside the class | Grants access to private/protected members |
| Defined **outside** the class without scope `ClassName::` | It is NOT a member function |
| No `this` pointer | Must receive the object as a parameter |
| Not affected by `public` / `private` / `protected` sections | The `friend` declaration can appear anywhere in the class |

> **Key Point:** A friend function can access private members, but it is not a member of the class. It cannot be called through an object using the dot operator.

### 1.2 Friend Class

When an **entire class** needs access to another class's private members, you can declare it as a **friend class**.

```cpp
class Engine {
    int horsepower;
public:
    Engine(int hp) : horsepower(hp) {}
    friend class Car;   // Car can access Engine's private members
};

class Car {
public:
    void showHP(const Engine &e) {
        cout << e.horsepower << endl;   // OK: Car is a friend of Engine
    }
};
```

| Rule | Detail |
|:-----|:-------|
| `friend class B;` inside class `A` | All member functions of `B` can access `A`'s private/protected members |
| Friendship is **not symmetric** | `A` friends `B` does NOT mean `B` friends `A` |
| Friendship is **not transitive** | `A` friends `B`, `B` friends `C` does NOT mean `A` friends `C` |

### 1.3 Guidelines

- Use `friend` **sparingly** — it breaks encapsulation by exposing private members to external code.
- It is **necessary** for operator overloading when the left operand is not your class type (e.g., `ostream& operator<<`).
- Prefer member functions when possible; use `friend` only when member functions are insufficient.

---

<br>

## 2. Operator Overloading Basics

### 2.1 Why Operator Overloading?

Without operator overloading, operations on user-defined types require explicit method calls. Compare the two approaches:

```cpp
// WITHOUT operator overloading — verbose, hard to read
Vector2D a(1, 2), b(3, 4), c(5, 6);
Vector2D result = a.add(b).subtract(c);          // What does this compute?
if (result.equals(Vector2D(-1, 0)))
    cout << result.toString() << endl;

// WITH operator overloading — natural, reads like math
Vector2D a(1, 2), b(3, 4), c(5, 6);
Vector2D result = a + b - c;                     // Obviously: (1+3-5, 2+4-6)
if (result == Vector2D(-1, 0))
    cout << result << endl;
```

Operator overloading is what makes C++ **expressive** for user-defined types. It lets objects of your classes behave like built-in types, making code more readable and intuitive.

| Without overloading | With overloading | Benefit |
|:--------------------|:-----------------|:--------|
| `a.add(b)` | `a + b` | Reads like math |
| `a.equals(b)` | `a == b` | Familiar syntax |
| `a.multiply(b)` | `a * b` | Less visual clutter |
| `cout << a.toString()` | `cout << a` | Seamless stream integration |
| `a.addAssign(b)` | `a += b` | Chaining works naturally |

### 2.2 Concept

**Operator overloading** lets you redefine the meaning of C++ operators for user-defined types (classes). You use the `operator` keyword followed by the operator symbol.

```cpp
Vector2D operator+(const Vector2D &rhs);   // overloads + for Vector2D
```

**Restrictions:**

| Restriction | Detail |
|:------------|:-------|
| Cannot create new operators | You cannot invent `operator@` or `operator$` |
| Cannot change **precedence** | `*` still binds tighter than `+` |
| Cannot change **associativity** | Left-to-right or right-to-left stays the same |
| Cannot change **arity** | Binary operators stay binary; unary stay unary |
| At least one operand must be a user-defined type | You cannot overload `int + int` |

### 2.3 Operators That Can / Cannot Be Overloaded

| Category | Operators |
|:---------|:----------|
| **Can** overload | `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `<<` `>>` `[]` `()` `~` `!` `&` `\|` `^` `++` `--` `+=` `-=` `*=` `/=` `%=` `=` `->` `new` `delete` ... |
| **Cannot** overload | `::` (scope resolution), `.` (member access), `.*` (pointer-to-member), `?:` (ternary), `sizeof`, `typeid` |

### 2.4 Member Function vs Friend Function — Decision Guide

| Aspect | Member Function | Friend Function |
|:-------|:---------------|:----------------|
| Left operand | Implicitly `*this` | Explicitly the first parameter |
| Right operand | The parameter | The second parameter |
| Access to privates | Yes (it is a member) | Yes (declared as friend) |
| When to use | Left operand is **your class** type (e.g., `obj + 3`) | Left operand is **NOT** your class type (e.g., `3 + obj`, `cout << obj`) |

```cpp
// Member: left operand is *this
Vector2D Vector2D::operator+(const Vector2D &rhs);

// Friend: both operands are parameters
friend Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
```

> **Key Point:** If the left operand is a built-in type or a class you cannot modify (such as `ostream`), you **must** use a friend (or free) function.

**Decision Flowchart:**

```
Is the LEFT operand your class type?
 |
 +-- YES --> Does the operator modify the left operand? (e.g., +=, ++, =)
 |            |
 |            +-- YES --> Member function (return *this by reference)
 |            |
 |            +-- NO  --> Member function (return new object by value)
 |
 +-- NO  --> (e.g., cout << obj,  3 + obj,  cin >> obj)
              |
              Does it need access to private members?
              |
              +-- YES --> Friend function
              |
              +-- NO  --> Non-member non-friend function
```

**Quick-reference table:**

| Expression | Left operand | Recommendation | Reason |
|:-----------|:-------------|:---------------|:-------|
| `obj + obj2` | Your class | Member function | Left operand is `*this` |
| `obj += obj2` | Your class | Member function | Modifies and returns `*this` |
| `3 + obj` | `int` (not your class) | Friend function | Cannot add member to `int` |
| `cout << obj` | `ostream` (not your class) | Friend function | Cannot add member to `ostream` |
| `cin >> obj` | `istream` (not your class) | Friend function | Cannot add member to `istream` |
| `++obj` / `obj++` | Your class | Member function | Left operand is `*this` |
| `obj[i]` | Your class | Member function (required) | `operator[]` must be a member |
| `obj = obj2` | Your class | Member function (required) | `operator=` must be a member |

---

<br>

## 3. Binary Operator Overloading

### 3.1 As Member Function

The left operand is `*this`; the right operand is the parameter. Return by **value** for arithmetic operators.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D &rhs) const {
        return Vector2D(x + rhs.x, y + rhs.y);
    }
};

// Usage
Vector2D a(1, 2), b(3, 4);
Vector2D c = a + b;   // calls a.operator+(b)  →  (4, 6)
```

### 3.2 As Friend Function

Both operands are explicit parameters. Useful when the left operand is not the class type.

```cpp
class Color {
    int r, g, b;
public:
    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    // Left operand is int, not Color → must be friend
    friend Color operator+(int lhs, const Color &rhs) {
        return Color(lhs + rhs.r, lhs + rhs.g, lhs + rhs.b);
    }
};

// Usage
Color sky(100, 150, 200);
Color c = 50 + sky;   // calls operator+(50, sky)  →  (150, 200, 250)
```

### 3.3 Compound Assignment

Compound assignment operators (`+=`, `-=`, `*=`) modify the left operand and return `*this` by **reference**.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    Vector2D& operator*=(int scalar) {
        x *= scalar;
        y *= scalar;
        return *this;   // return by reference
    }
};

// Usage
Vector2D v(3, 4);
v *= 2;   // v becomes (6, 8)
```

| Return type | Why |
|:------------|:----|
| `ClassName&` (reference) | The object is modified in place; returning `*this` allows chaining (`a += b += c`) |

### 3.4 Comparison Operators

Comparison operators return `bool`. They are typically `const` member functions.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Vector2D &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

// Usage
Vector2D v1(3, 4), v2(3, 4);
if (v1 == v2)   // true: both components match
    cout << "equal" << endl;
```

---

<br>

## 4. Unary Operator Overloading

### 4.1 As Member Function

Unary operators take **no parameters** when implemented as member functions — `*this` is the operand.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    // ~ returns the perpendicular vector (does NOT modify the original)
    Vector2D operator~() const {
        return Vector2D(-y, x);
    }
};

// Usage
Vector2D v(3, 4);
Vector2D perp = ~v;   // perp = (-4, 3),  v is unchanged
```

### 4.2 As Friend Function

When implemented as a friend, the operand is an explicit parameter.

```cpp
class Color {
    int r, g, b;
public:
    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    // ~ returns the inverted color
    friend Color operator~(const Color &obj) {
        return Color(255 - obj.r, 255 - obj.g, 255 - obj.b);
    }
};

// Usage
Color red(255, 0, 0);
Color c = ~red;   // c = (0, 255, 255) (cyan),  red is unchanged
```

### 4.3 Increment / Decrement

C++ distinguishes **prefix** and **postfix** forms using a dummy `int` parameter.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    // Prefix --obj: modify, then return the modified object by reference
    Vector2D& operator--() {
        x--;
        y--;
        return *this;
    }

    // Postfix obj++: save original, modify, return the saved copy by value
    Vector2D operator++(int) {   // int is a dummy parameter
        Vector2D old = *this;
        x++;
        y++;
        return old;
    }
};
```

| Form | Signature (member) | Return | Behavior |
|:-----|:-------------------|:-------|:---------|
| Prefix `++obj` / `--obj` | `ClassName& operator++()` | Reference to modified object | Modify first, then return |
| Postfix `obj++` / `obj--` | `ClassName operator++(int)` | Copy of original (before modification) | Save copy, modify, return copy |

> **Key Point:** The `int` parameter in the postfix version is a **dummy** — it is never used. Its sole purpose is to let the compiler distinguish prefix from postfix.

**As friend functions:**

```cpp
class Color {
    int r, g, b;
public:
    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    // Prefix ++obj
    friend Color& operator++(Color &obj) {
        obj.r++;
        obj.g++;
        obj.b++;
        return obj;
    }

    // Postfix obj--
    friend Color operator--(Color &obj, int) {
        Color old = obj;
        obj.r--;
        obj.g--;
        obj.b--;
        return old;
    }
};
```

### 4.4 Prefix vs Postfix — Detailed Trace

Understanding the difference between prefix and postfix is critical. Here is a step-by-step trace with built-in `int` to build intuition:

```
 Expression         Step-by-step                             Result
--------------     ------------------------------------     -----------
int x = 5;         x is initialized to 5                     x=5
int a = ++x;       x becomes 6, THEN a gets the value        x=6, a=6
int b = x++;       b gets the value (6), THEN x becomes 7    x=7, b=6
int c = --x;       x becomes 6, THEN c gets the value        x=6, c=6
int d = x--;       d gets the value (6), THEN x becomes 5    x=5, d=6
```

**Key rule:**
- **Prefix** (`++x`): increment **first**, then use the **new** value.
- **Postfix** (`x++`): use the **old** value, then increment.

Now the class equivalent — notice how the implementation mirrors the behavior above:

```cpp
class Counter {
    int value;
public:
    Counter(int v = 0) : value(v) {}
    int getValue() const { return value; }

    // Prefix ++obj: modify first, return modified object by REFERENCE
    Counter& operator++() {
        value++;            // Step 1: modify
        return *this;       // Step 2: return the modified object
    }

    // Postfix obj++: save old state, modify, return OLD state by VALUE
    Counter operator++(int) {
        Counter old = *this;  // Step 1: save old state
        value++;              // Step 2: modify
        return old;           // Step 3: return the old state (copy)
    }
};
```

Trace for the class:

```
Code                   value (before)     returned             value (after)
------------------     --------------     --------     -----------------------------
Counter x(5);                —                —                      5
Counter a = ++x;             5                6         6 (modify first, return new)
Counter b = x++;             6                6         7 (return old, then modify)
```

> **Why does prefix return by reference but postfix returns by value?**
> - Prefix returns the **actual modified object** — it still exists, so a reference is safe and efficient.
> - Postfix must return the **old state** — but the old state no longer exists in the object, so it must be a temporary copy (returned by value).

---

<br>

## 5. Stream Operator Overloading

### 5.1 `operator<<`

The insertion operator `<<` must be a **friend** (or free function) because the left operand is `ostream`, not your class.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    friend ostream& operator<<(ostream &out, const Vector2D &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;   // return out for chaining
    }
};

// Usage
Vector2D v(3, 4);
cout << "Value: " << v << endl;   // prints "Value: (3, 4)"
```

| Detail | Explanation |
|:-------|:-----------|
| Left operand is `ostream&` | Cannot be a member of your class |
| Returns `ostream&` | Enables chaining: `cout << a << b << c` |
| Second parameter is `const ClassName&` | Output should not modify the object |

### 5.2 `operator>>`

The extraction operator `>>` follows the same pattern but uses `istream` and a **non-const** reference (because it modifies the object).

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    friend istream& operator>>(istream &in, Vector2D &v) {
        in >> v.x >> v.y;   // expects input like "3 4"
        return in;
    }
};

// Usage
Vector2D v;
cin >> v;   // user types "3 4"
```

---

<br>

## 6. Subscript Operator

The `operator[]` allows objects to be accessed like arrays. It returns a **reference** so the caller can both read and write.

```cpp
class Date {
    int year, month, day;
public:
    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}

    int& operator[](int index) {
        switch (index) {
            case 0: return year;
            case 1: return month;
            case 2: return day;
            default: return year;   // fallback
        }
    }
};

// Usage
Date d(2026, 3, 16);
d[0] = 2025;   // sets year to 2025
d[1] = 12;     // sets month to 12
d[2] = 25;     // sets day to 25
cout << d[0] << "-" << d[1] << "-" << d[2] << endl;   // 2025-12-25
```

| Detail | Explanation |
|:-------|:-----------|
| Return type is `int&` (reference) | Enables both reading (`x = d[0]`) and writing (`d[0] = 2025`) |
| Must be a **member function** | `operator[]` cannot be a friend/free function |
| Parameter is typically `int` | Represents the index |

---

<br>

## 7. Complete Runnable Examples

### 7.1 Complete Vector2D Class

A full program demonstrating `operator+`, `operator==`, and `operator<<` working together naturally:

```cpp
#include <iostream>
using namespace std;

class Vector2D {
    int x, y;

public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    // Binary +  (member: left operand is *this)
    Vector2D operator+(const Vector2D &rhs) const {
        return Vector2D(x + rhs.x, y + rhs.y);
    }

    // Comparison ==  (member: const, returns bool)
    bool operator==(const Vector2D &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    // Stream <<  (friend: left operand is ostream, not Vector2D)
    friend ostream& operator<<(ostream &out, const Vector2D &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;
    }
};

int main() {
    Vector2D a(1, 2), b(3, 4);
    Vector2D c = a + b;                // (1+3, 2+4) = (4, 6)

    cout << a << " + " << b << " = " << c << endl;   // (1, 2) + (3, 4) = (4, 6)

    if (c == Vector2D(4, 6))
        cout << "Equal!" << endl;      // Equal!

    Vector2D d = a + b + Vector2D(5, 4);
    cout << "(1,2) + (3,4) + (5,4) = " << d << endl;  // (1,2) + (3,4) + (5,4) = (9, 10)

    return 0;
}
```

**Expected output:**

```
(1, 2) + (3, 4) = (4, 6)
Equal!
(1,2) + (3,4) + (5,4) = (9, 10)
```

### 7.2 Prefix vs Postfix Output Trace

A program that clearly demonstrates the difference between `++x` and `x++` with output at each step:

```cpp
#include <iostream>
using namespace std;

class Counter {
    int value;
public:
    Counter(int v = 0) : value(v) {}

    // Prefix ++obj
    Counter& operator++() {
        value++;
        return *this;
    }

    // Postfix obj++
    Counter operator++(int) {
        Counter old = *this;
        value++;
        return old;
    }

    friend ostream& operator<<(ostream &out, const Counter &c) {
        out << c.value;
        return out;
    }
};

int main() {
    Counter x(5);
    cout << "Initial:  x = " << x << endl;

    Counter a = ++x;   // x becomes 6, a gets 6
    cout << "After a = ++x:  x = " << x << ", a = " << a << endl;

    Counter b = x++;   // b gets 6 (old x), then x becomes 7
    cout << "After b = x++:  x = " << x << ", b = " << b << endl;

    // Chaining: prefix returns reference, so this works
    cout << "++x = " << ++x << endl;   // x becomes 8, prints 8

    return 0;
}
```

**Expected output:**

```
Initial:  x = 5
After a = ++x:  x = 6, a = 6
After b = x++:  x = 7, b = 6
++x = 8
```

### 7.3 Why operator<< Must Be a Friend

This example shows **why** `operator<<` cannot be a member function, and what happens if you try:

```cpp
#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // WRONG: operator<< as a member function
    // If this were a member, the syntax would be:
    //     point << cout;    // Left operand is *this (Point), right is ostream
    // This is backwards! Nobody writes  point << cout.
    //
    // ostream& operator<<(ostream &out) {    // DO NOT DO THIS
    //     out << "(" << x << ", " << y << ")";
    //     return out;
    // }

    // CORRECT: operator<< as a friend function
    // Left operand is ostream (not Point), so it cannot be a Point member.
    friend ostream& operator<<(ostream &out, const Point &p) {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }
};

int main() {
    Point p(3, 7);

    // Natural syntax — works because operator<< is a friend
    cout << "Point: " << p << endl;       // Point: (3, 7)

    // If it were a member function, you would have to write:
    // p << cout;   // Ugly, unnatural, and breaks chaining!

    // Chaining works because we return ostream&
    Point a(1, 2), b(3, 4);
    cout << a << " and " << b << endl;    // (1, 2) and (3, 4)

    return 0;
}
```

**Expected output:**

```
Point: (3, 7)
(1, 2) and (3, 4)
```

**Why it must be a friend (not a member):**

| If member function... | If friend function... |
|:----------------------|:----------------------|
| Syntax: `point << cout;` (backwards!) | Syntax: `cout << point;` (natural!) |
| Left operand is `Point` (`*this`) | Left operand is `ostream` (first parameter) |
| Cannot chain: `point << cout << endl;` fails | Chains correctly: `cout << point << endl;` |
| Breaks convention that every C++ programmer expects | Follows the standard convention |

---

<br>

## 8. Common Mistakes

### Mistake 1: Forgetting `(int)` dummy parameter for postfix

```cpp
// WRONG: both are prefix!
Counter& operator++();       // This is prefix ++obj
Counter operator++();        // ERROR: cannot overload on return type alone

// CORRECT: use int dummy to distinguish
Counter& operator++();       // Prefix  ++obj
Counter operator++(int);     // Postfix obj++   (int is the dummy parameter)
```

The `int` parameter is never used — its sole purpose is to let the compiler distinguish prefix from postfix.

### Mistake 2: Returning by value when you should return by reference

```cpp
// WRONG: returning by value in compound assignment
Vector2D operator+=(const Vector2D &rhs) {   // Should be Vector2D&
    x += rhs.x;
    y += rhs.y;
    return *this;   // Returns a COPY — chaining (a += b += c) breaks
}

// CORRECT: return by reference
Vector2D& operator+=(const Vector2D &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;   // Returns the actual object — chaining works
}
```

The same applies to `operator<<` and `operator>>` — they must return `ostream&` and `istream&`, not by value.

### Mistake 3: Forgetting to return `*this` in compound assignment operators

```cpp
// WRONG: modifies the object but forgets to return it
void operator+=(const Vector2D &rhs) {   // void return — cannot chain!
    x += rhs.x;
    y += rhs.y;
    // Missing: return *this;
}

// CORRECT:
Vector2D& operator+=(const Vector2D &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;   // Enables  a += b += c
}
```

### Mistake 4: Trying to overload operators that cannot be overloaded

The following operators **cannot** be overloaded:

| Operator | Name |
|:---------|:-----|
| `::` | Scope resolution |
| `.` | Member access |
| `.*` | Pointer-to-member access |
| `?:` | Ternary conditional |
| `sizeof` | Size of object/type |
| `typeid` | Runtime type identification |

Attempting to overload any of these will produce a compiler error.

### Mistake 5: Making operator<< a member function instead of a friend

```cpp
// WRONG: operator<< as member function
class Vector2D {
    int x, y;
public:
    // This compiles, but the calling syntax is backwards:
    //     vec << cout;     instead of     cout << vec;
    ostream& operator<<(ostream &out) {
        out << "(" << x << ", " << y << ")";
        return out;
    }
};
// Usage:  v << cout;  // Compiles but looks wrong!

// CORRECT: operator<< as friend function
class Vector2D {
    int x, y;
public:
    friend ostream& operator<<(ostream &out, const Vector2D &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;
    }
};
// Usage:  cout << v;  // Natural and correct!
```

**Remember:** The left operand of `<<` is `ostream`, not your class. Since you cannot add member functions to `ostream`, `operator<<` **must** be a friend or free function.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Why operator overloading | Transforms `a.add(b)` into `a + b`; makes user-defined types as natural to use as built-in types |
| Friend function | Non-member function with access to private/protected; declared with `friend` inside the class |
| Friend class | `friend class B;` grants all of B's member functions access; not symmetric, not transitive |
| Friend guidelines | Use sparingly; breaks encapsulation; necessary for certain operator overloads |
| Operator overloading concept | Redefine operators for user-defined types; `operator` keyword; cannot create new operators or change precedence/associativity/arity |
| Cannot overload | `::`, `.`, `.*`, `?:`, `sizeof`, `typeid` |
| Member vs friend decision | Left operand is your class? Member. Left operand is NOT your class? Friend. Need privates? Friend. Otherwise, non-member non-friend. |
| Binary as member | `ReturnType operator+(const ClassName &rhs)`; `*this` is left operand |
| Binary as friend | `friend ReturnType operator+(const ClassName &lhs, const ClassName &rhs)` |
| Compound assignment (`+=`, `*=`) | Modify `*this` and return `*this` by reference |
| Comparison (`==`, `!=`, `<=`) | Return `bool`; typically `const` member functions |
| Unary as member | `ReturnType operator~()`; no parameters; `*this` is the operand |
| Unary as friend | `friend ReturnType operator~(const ClassName &obj)` |
| Prefix `++`/`--` | `operator++()` returns reference; modify first, then return |
| Postfix `++`/`--` | `operator++(int)` returns value; save copy, modify, return copy; `int` is dummy |
| Prefix vs postfix trace | `int x=5; int a=++x;` gives x=6,a=6. `int b=x++;` gives x=7,b=6. Prefix returns reference, postfix returns value. |
| `operator<<` | `friend ostream& operator<<(ostream &out, const ClassName &obj)`; must be friend; return `out` for chaining |
| `operator>>` | `friend istream& operator>>(istream &in, ClassName &obj)`; non-const reference |
| `operator[]` | `int& operator[](int index)`; returns reference for read/write; must be a member function |
| Common mistake: postfix dummy | `operator++()`is prefix; `operator++(int)` is postfix — the `int` dummy distinguishes them |
| Common mistake: return type | Compound assignment and stream operators must return by **reference**, not by value |
| Common mistake: `*this` | Compound assignment operators must `return *this;` to enable chaining |
| Common mistake: `<<` as member | `operator<<` as a member reverses the syntax to `obj << cout` — always use friend |

---
