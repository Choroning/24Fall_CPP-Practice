# Chapter 07 — Friend Functions and Operator Overloads

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpum C++ Programming, Revised Edition* — Chapter 7

---

## Table of Contents

- [1. Friend](#1-friend)
  - [1.1 Friend Function](#11-friend-function)
  - [1.2 Friend Class](#12-friend-class)
  - [1.3 Guidelines](#13-guidelines)
- [2. Operator Overloading Basics](#2-operator-overloading-basics)
  - [2.1 Concept](#21-concept)
  - [2.2 Operators That Can / Cannot Be Overloaded](#22-operators-that-can--cannot-be-overloaded)
  - [2.3 Member Function vs Friend Function](#23-member-function-vs-friend-function)
- [3. Binary Operator Overloading](#3-binary-operator-overloading)
  - [3.1 As Member Function](#31-as-member-function)
  - [3.2 As Friend Function](#32-as-friend-function)
  - [3.3 Compound Assignment](#33-compound-assignment)
  - [3.4 Comparison Operators](#34-comparison-operators)
- [4. Unary Operator Overloading](#4-unary-operator-overloading)
  - [4.1 As Member Function](#41-as-member-function)
  - [4.2 As Friend Function](#42-as-friend-function)
  - [4.3 Increment / Decrement](#43-increment--decrement)
- [5. Stream Operator Overloading](#5-stream-operator-overloading)
  - [5.1 `operator<<`](#51-operator)
  - [5.2 `operator>>`](#52-operator-1)
- [6. Subscript Operator](#6-subscript-operator)
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

### 2.1 Concept

**Operator overloading** lets you redefine the meaning of C++ operators for user-defined types (classes). You use the `operator` keyword followed by the operator symbol.

```cpp
Fraction operator+(const Fraction &rhs);   // overloads + for Fraction
```

**Restrictions:**

| Restriction | Detail |
|:------------|:-------|
| Cannot create new operators | You cannot invent `operator@` or `operator$` |
| Cannot change **precedence** | `*` still binds tighter than `+` |
| Cannot change **associativity** | Left-to-right or right-to-left stays the same |
| Cannot change **arity** | Binary operators stay binary; unary stay unary |
| At least one operand must be a user-defined type | You cannot overload `int + int` |

### 2.2 Operators That Can / Cannot Be Overloaded

| Category | Operators |
|:---------|:----------|
| **Can** overload | `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `<<` `>>` `[]` `()` `~` `!` `&` `\|` `^` `++` `--` `+=` `-=` `*=` `/=` `%=` `=` `->` `new` `delete` ... |
| **Cannot** overload | `::` (scope resolution), `.` (member access), `.*` (pointer-to-member), `?:` (ternary), `sizeof`, `typeid` |

### 2.3 Member Function vs Friend Function

| Aspect | Member Function | Friend Function |
|:-------|:---------------|:----------------|
| Left operand | Implicitly `*this` | Explicitly the first parameter |
| Right operand | The parameter | The second parameter |
| Access to privates | Yes (it is a member) | Yes (declared as friend) |
| When to use | Left operand is **your class** type (e.g., `obj + 3`) | Left operand is **NOT** your class type (e.g., `3 + obj`, `cout << obj`) |

```cpp
// Member: left operand is *this
Fraction Fraction::operator+(const Fraction &rhs);

// Friend: both operands are parameters
friend Fraction operator+(const Fraction &lhs, const Fraction &rhs);
```

> **Key Point:** If the left operand is a built-in type or a class you cannot modify (such as `ostream`), you **must** use a friend (or free) function.

---

<br>

## 3. Binary Operator Overloading

### 3.1 As Member Function

The left operand is `*this`; the right operand is the parameter. Return by **value** for arithmetic operators.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    Fraction operator+(const Fraction &rhs) const {
        return Fraction(num * rhs.denom + rhs.num * denom,
                        denom * rhs.denom);
    }
};

// Usage
Fraction a(1, 2), b(1, 3);
Fraction c = a + b;   // calls a.operator+(b)  →  5/6
```

### 3.2 As Friend Function

Both operands are explicit parameters. Useful when the left operand is not the class type.

```cpp
class Complex {
    int real, imag;
public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}

    // Left operand is int, not Complex → must be friend
    friend Complex operator+(int lhs, const Complex &rhs) {
        return Complex(lhs + rhs.real, rhs.imag);
    }
};

// Usage
Complex b(2, -3);
Complex c = 10 + b;   // calls operator+(10, b)  →  12-3i
```

### 3.3 Compound Assignment

Compound assignment operators (`+=`, `-=`, `*=`) modify the left operand and return `*this` by **reference**.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    Fraction& operator*=(const Fraction &rhs) {
        num *= rhs.num;
        denom *= rhs.denom;
        return *this;   // return by reference
    }
};

// Usage
Fraction f1(3, 4), f2(2, 5);
f1 *= f2;   // f1 becomes 6/20
```

| Return type | Why |
|:------------|:----|
| `ClassName&` (reference) | The object is modified in place; returning `*this` allows chaining (`a += b += c`) |

### 3.4 Comparison Operators

Comparison operators return `bool`. They are typically `const` member functions.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    bool operator==(const Fraction &rhs) const {
        // Cross-multiply to compare: a/b == c/d ↔ a*d == c*b
        return num * rhs.denom == rhs.num * denom;
    }
};

// Usage
Fraction f2(2, 5), f5(6, 15);
if (f2 == f5)   // true: 2*15 == 6*5
    cout << "equal" << endl;
```

---

<br>

## 4. Unary Operator Overloading

### 4.1 As Member Function

Unary operators take **no parameters** when implemented as member functions — `*this` is the operand.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    // ~ returns the reciprocal (does NOT modify the original)
    Fraction operator~() const {
        return Fraction(denom, num);
    }
};

// Usage
Fraction f(2, 5);
Fraction inv = ~f;   // inv = 5/2,  f is unchanged
```

### 4.2 As Friend Function

When implemented as a friend, the operand is an explicit parameter.

```cpp
class Complex {
    int real, imag;
public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}

    // ~ returns the conjugate
    friend Complex operator~(const Complex &obj) {
        return Complex(obj.real, -obj.imag);
    }
};

// Usage
Complex b(2, -3);
Complex c = ~b;   // c = 2+3i,  b is unchanged
```

### 4.3 Increment / Decrement

C++ distinguishes **prefix** and **postfix** forms using a dummy `int` parameter.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    // Prefix --obj: modify, then return the modified object by reference
    Fraction& operator--() {
        num -= denom;   // subtract 1 (denom/denom)
        return *this;
    }

    // Postfix obj++: save original, modify, return the saved copy by value
    Fraction operator++(int) {   // int is a dummy parameter
        Fraction old = *this;
        num += denom;   // add 1 (denom/denom)
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
class Complex {
    int real, imag;
public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}

    // Prefix ++obj
    friend Complex& operator++(Complex &obj) {
        obj.real++;
        return obj;
    }

    // Postfix obj--
    friend Complex operator--(Complex &obj, int) {
        Complex old = obj;
        obj.real--;
        return old;
    }
};
```

---

<br>

## 5. Stream Operator Overloading

### 5.1 `operator<<`

The insertion operator `<<` must be a **friend** (or free function) because the left operand is `ostream`, not your class.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    friend ostream& operator<<(ostream &out, const Fraction &f) {
        out << f.num << "/" << f.denom;
        return out;   // return out for chaining
    }
};

// Usage
Fraction f(3, 4);
cout << "Value: " << f << endl;   // prints "Value: 3/4"
```

| Detail | Explanation |
|:-------|:-----------|
| Left operand is `ostream&` | Cannot be a member of your class |
| Returns `ostream&` | Enables chaining: `cout << a << b << c` |
| Second parameter is `const ClassName&` | Output should not modify the object |

### 5.2 `operator>>`

The extraction operator `>>` follows the same pattern but uses `istream` and a **non-const** reference (because it modifies the object).

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    friend istream& operator>>(istream &in, Fraction &f) {
        char slash;
        in >> f.num >> slash >> f.denom;   // expects input like "3/4"
        return in;
    }
};

// Usage
Fraction f;
cin >> f;   // user types "3/4"
```

---

<br>

## 6. Subscript Operator

The `operator[]` allows objects to be accessed like arrays. It returns a **reference** so the caller can both read and write.

```cpp
class Time {
    int hour, min, sec;
public:
    Time(int h = 0, int m = 0, int s = 0) : hour(h), min(m), sec(s) {}

    int& operator[](int index) {
        switch (index) {
            case 0: return hour;
            case 1: return min;
            case 2: return sec;
            default: return hour;   // fallback
        }
    }
};

// Usage
Time t(10, 30, 45);
t[0] = 3;    // sets hour to 3
t[1] = 9;    // sets min to 9
t[2] = 27;   // sets sec to 27
cout << t[0] << ":" << t[1] << ":" << t[2] << endl;   // 3:9:27
```

| Detail | Explanation |
|:-------|:-----------|
| Return type is `int&` (reference) | Enables both reading (`x = t[0]`) and writing (`t[0] = 3`) |
| Must be a **member function** | `operator[]` cannot be a friend/free function |
| Parameter is typically `int` | Represents the index |

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Friend function | Non-member function with access to private/protected; declared with `friend` inside the class |
| Friend class | `friend class B;` grants all of B's member functions access; not symmetric, not transitive |
| Friend guidelines | Use sparingly; breaks encapsulation; necessary for certain operator overloads |
| Operator overloading concept | Redefine operators for user-defined types; `operator` keyword; cannot create new operators or change precedence/associativity/arity |
| Cannot overload | `::`, `.`, `.*`, `?:`, `sizeof`, `typeid` |
| Member vs friend | Member: `*this` is left operand; friend: both operands are parameters; use friend when left operand is not your class |
| Binary as member | `ReturnType operator+(const ClassName &rhs)`; `*this` is left operand |
| Binary as friend | `friend ReturnType operator+(const ClassName &lhs, const ClassName &rhs)` |
| Compound assignment (`+=`, `*=`) | Modify `*this` and return `*this` by reference |
| Comparison (`==`, `!=`, `<=`) | Return `bool`; typically `const` member functions |
| Unary as member | `ReturnType operator~()`; no parameters; `*this` is the operand |
| Unary as friend | `friend ReturnType operator~(const ClassName &obj)` |
| Prefix `++`/`--` | `operator++()` returns reference; modify first, then return |
| Postfix `++`/`--` | `operator++(int)` returns value; save copy, modify, return copy; `int` is dummy |
| `operator<<` | `friend ostream& operator<<(ostream &out, const ClassName &obj)`; must be friend; return `out` for chaining |
| `operator>>` | `friend istream& operator>>(istream &in, ClassName &obj)`; non-const reference |
| `operator[]` | `int& operator[](int index)`; returns reference for read/write; must be a member function |

---
