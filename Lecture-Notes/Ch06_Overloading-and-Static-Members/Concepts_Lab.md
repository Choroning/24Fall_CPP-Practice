# Chapter 6 Lab — Function Overloading and Default Parameters

[Repository Guide](../../README.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **Last Updated:** 2026-11-19

> **Learning Objectives:**
> 1. Explain the conditions under which function overloading holds and why functions that differ only in return type cannot be overloaded
> 2. Implement the same behavior in two ways, by function overloading and by default parameters, and compare them
> 3. Write a class that accepts initial values in several forms through constructor overloading

---

## Table of Contents

- [1. Overview](#1-overview)
- [2. Function Overloading and Default Parameters](#2-function-overloading-and-default-parameters)
  - [2.1 Conditions for Function Overloading](#21-conditions-for-function-overloading)
  - [2.2 The Larger Value Capped by a Limit](#22-the-larger-value-capped-by-a-limit)
  - [2.3 Choosing Between Function Overloading and Default Parameters](#23-choosing-between-function-overloading-and-default-parameters)
- [3. Constructor Overloading](#3-constructor-overloading)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overview

> **Note:** The theory and examples of function overloading and its resolution rules, default parameters, and `static` members (a `Person` sharing common money, a `Math` class with only `static` functions, a `Circle` that counts living circles) are covered in the [Chapter 6 lecture notes](Concepts_Lecture.md). This lab solves the same problem with function overloading and with default parameters to compare the two approaches.

| Topic | Location |
|:-----|:-----|
| Conditions for function overloading, `big()` implemented with function overloading and with default parameters | Section 2 |
| A `Person` class completed with constructor overloading | Section 3 |
| A `Person` class completed with a single constructor with default parameters | [HW6 Problem 1](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.md#problem-1-week-11-lecture-slides-lab-2-2) |
| A `Person` that counts members with a `static` member, a `Random` made of `static` functions, a `Circle` that counts current objects | [HW6 Problems 2-4](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.md) |
| A `Board` that manages posts with `static` members, `print99()` that prints multiplication tables with default parameters | [Chapter 6 Exercises 1 and 2](Exercises.md) |

*Table 1. Topics of the lab and their location*

All code is compiled and run with the C++17 standard (for example, `g++ -std=c++17 -Wall`).

---

<br>

## 2. Function Overloading and Default Parameters

### 2.1 Conditions for Function Overloading

> **Definition:** **Function overloading** is defining, in the same scope, functions that share a name but differ in the types or number of their parameters.

Function overloading is possible among ordinary functions, among the member functions of a class, and among the member functions of a base class and a derived class related by inheritance; the C language does not allow it. The compiler decides which function to call from the types and number of the arguments used in the call, so two functions that differ only in return type cannot be overloaded.

```cpp
int sum(int a, int b) { return a + b; }
double sum(int a, int b) { return (double)(a + b); }   // Differs only in return type
int main() { return sum(2, 5); }
```

```text
error: ambiguating new declaration of 'double sum(int, int)'
error: functions that differ only in their return type cannot be overloaded
```

The first line is GCC's error and the second Clang's. Looking at `sum(2, 5)` alone cannot decide which `sum()` to call, so the second declaration itself is an error.

### 2.2 The Larger Value Capped by a Limit

**Problem:** Write `big()`, which returns the larger of two integers but returns a limit if the larger value exceeds that limit. If no limit is given, 100 is used. Make the following `main()` run by writing (1) two `big()` functions with function overloading and (2) one `big()` with a default parameter.

```cpp
int main() {
    int x = big(3, 5);          // The larger value 5 is below the limit 100, so 5
    int y = big(300, 60);       // The larger value 300 exceeds the limit 100, so 100
    int z = big(30, 60, 50);    // The larger value 60 exceeds the limit 50, so 50
    cout << x << ' ' << y << ' ' << z << endl;
    return 0;
}
```

**(1) Function overloading**

```cpp
#include <iostream>
using namespace std;

int big(int a, int b, int max) {        // Return max if the larger value exceeds max
    int m = (a > b) ? a : b;
    return (m > max) ? max : m;
}

int big(int a, int b) {                 // Overload that uses 100 as the limit
    return big(a, b, 100);
}

int main() {
    int x = big(3, 5);
    int y = big(300, 60);
    int z = big(30, 60, 50);
    cout << x << ' ' << y << ' ' << z << endl;
    return 0;
}
```

```text
5 100 50
```

**(2) Default parameter**

```cpp
#include <iostream>
using namespace std;

int big(int a, int b, int max = 100) {  // max is 100 when omitted
    int m = (a > b) ? a : b;
    return (m > max) ? max : m;
}

int main() {
    int x = big(3, 5);
    int y = big(300, 60);
    int z = big(30, 60, 50);
    cout << x << ' ' << y << ' ' << z << endl;
    return 0;
}
```

```text
5 100 50
```

- **Function overloading:** A call with two arguments is bound to `big(int, int)` and one with three to `big(int, int, int)`. Having the two-argument version call the three-argument version keeps the comparison logic in one place.
- **Default parameter:** `max = 100` means that 100 is placed in `max` when no argument is passed. The compiler turns `big(3, 5)` into the call `big(3, 5, 100)`. With only one function definition, the code is shorter.
- **Ambiguity when combined:** If both approaches are placed in one program, that is, if `int big(int, int)` and `int big(int, int, int = 100)` coexist, `big(3, 5)` matches both functions and becomes an ambiguous call error (Section 2.3 of the Chapter 6 lecture notes).
- **Position of default parameters:** Default values can be given only to consecutive parameters starting from the rightmost one. A middle parameter cannot be skipped, as in `int big(int a = 0, int b, int max)`.

### 2.3 Choosing Between Function Overloading and Default Parameters

The two methods of Section 2.2 give the same result when only the number of arguments differs. When the **types** of the parameters differ, default parameters cannot take the place of overloading, and function overloading is needed. The following `big()` works for integers and for floating-point numbers separately.

```cpp
#include <iostream>
using namespace std;

int big(int a, int b, int max = 100) {      // integer version
    int m = (a > b) ? a : b;
    return (m > max) ? max : m;
}

double big(double a, double b, double max = 100.0) {   // floating-point version: overloading is needed because the types differ
    double m = (a > b) ? a : b;
    return (m > max) ? max : m;
}

int main() {
    cout << big(3, 5) << ' ' << big(2.5, 1.5) << ' ' << big(300.0, 7.0) << endl;
    return 0;
}
```

```text
5 2.5 100
```

`big(3, 5)` has two `int` arguments, so it matches the integer version exactly, while `big(2.5, 1.5)` and `big(300.0, 7.0)` match the floating-point version exactly. Both versions give the third parameter a default value, so function overloading and default parameters are used together. Even if functions with the same name have default values, no ambiguity arises as long as the argument types determine a single function to call. Arguments of mixed types, as in `big(3, 2.5)`, need standard conversions for both versions and cause an ambiguity error (Section 1.5 of the Chapter 6 lecture notes).

| Situation | Suitable method | Reason |
|:-----|:-----|:-----|
| Only trailing arguments can be omitted from the same processing | Default parameters | There is one function definition, so the default value is managed in one place |
| The parameter types differ | Function overloading | The processing code must differ by type |
| The processing itself differs by the number of arguments | Function overloading | It cannot be expressed with a single default value |

*Table 2. Criteria for choosing between function overloading and default parameters*

---

<br>

## 3. Constructor Overloading

**Problem:** The definition of the `Person` class and `main()` are as follows. Write the three overloaded constructors so that, as in the sample run, values not passed as arguments take the defaults (ID 1, weight 20.5, name `"Grace"`).

```cpp
class Person {
    int id;
    double weight;
    string name;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
```

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

Person::Person() {                      // No arguments: all values default
    id = 1;
    weight = 20.5;
    name = "Grace";
}

Person::Person(int id, string name) {   // Only the weight defaults
    this->id = id;
    this->name = name;
    weight = 20.5;
}

Person::Person(int id, string name, double weight) {
    this->id = id;
    this->name = name;
    this->weight = weight;
}

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
```

```text
1 20.5 Grace
2 20.5 Ashley
3 32.5 Helen
```

- `grace` has no arguments, so `Person()` is called; `ashley` has two, so `Person(int, string)`; and `helen` has three, so `Person(int, string, double)`. The `"Ashley"` of `ashley(2, "Ashley")` is a string literal, but it converts to `string` and matches the second constructor.
- The default value 20.5 is scattered across the three constructors, so changing it requires editing several places. Reducing the same behavior to a single constructor with default parameters is [HW6 Problem 1](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.md#problem-1-week-11-lecture-slides-lab-2-2).

The same three constructors can also be written with the member initializer lists of Chapter 3. Instead of assigning in the constructor body, they give the initial values at the moment the members are created.

```cpp
Person() : id(1), weight(20.5), name("Grace") { }
Person(int id, string name) : id(id), weight(20.5), name(name) { }
Person(int id, string name, double weight) : id(id), weight(weight), name(name) { }
```

```text
1 20.5 Grace
2 20.5 Ashley
3 32.5 Helen
```

In `id(id)`, the `id` outside the parentheses is the member variable and the `id` inside them is the parameter. Only member names can appear outside the parentheses of a member initializer list, so the two are distinguished without `this->`. A member of class type such as `name` is first created as an empty string and then assigned again when assigned in the body, but in an initializer list it is created with the given value from the start. The problem of the default value 20.5 being scattered over several constructors still remains.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Conditions for overloading | Functions must share a name and differ in the types or number of parameters, and functions that differ only in return type cannot be overloaded. |
| Overloading and default parameters | Overloads that differ only in the number of arguments can be reduced to one function with default parameters, but having both approaches together can make calls ambiguous. |
| Position of default parameters | Default values are specified consecutively from the rightmost parameter, and the compiler fills omitted arguments with the default values. |
| Constructor overloading | Several constructors with different parameters initialize objects in several forms, and the constructor to call is determined by the number and types of the arguments. |
| Choosing between overloading and default parameters | Omitting trailing arguments is handled with default parameters, and different types or processing with function overloading, and the two can be used together. |
| Member initializer lists of constructors | Initial values are given when the members are created, as in `Person(int id, string name) : id(id), name(name)`, with the member outside the parentheses and the parameter inside them. |

---

<br>

## Self-Check Questions

1. What happens if `int big(int a, int b) { return (a > b) ? a : b; }` is added to the default parameter version of Section 2.2?

   <details>
   <summary>Answer</summary>

   `int big(int, int)` and `int big(int, int, int = 100)` coexist, so the definitions themselves are allowed, but a call with two arguments such as `big(3, 5)` matches both functions and the compiler cannot choose one, so an ambiguous call error results. The three-argument call `big(30, 60, 50)` matches only the second function, so it causes no problem.

   </details>

2. Which of the declarations `void show(int a = 1, int b);` and `void show(int a, int b = 1);` compiles? Why?

   <details>
   <summary>Answer</summary>

   Only `void show(int a, int b = 1);` compiles. Default values must be specified consecutively from the rightmost parameter, but the first declaration gives a default only to `a` on the left and none to `b` on its right. Omitted arguments are filled from the right, so in `show(5)` the 5 would go to `a` and `b` would be left empty, which makes no sense.

   </details>

3. What happens if `Person tom(7);` is added to the `main()` of Section 3? Which constructor should be added to make it compile?

   <details>
   <summary>Answer</summary>

   There is no constructor that takes a single `int`, so no matching constructor is found and a compile error results. Adding a constructor `Person(int id)` that takes a single `int`, sets only the ID from the argument, and sets the weight and name to their defaults makes `Person tom(7);` compile.

   </details>

4. Given the two `big()` functions of Section 2.3, which function do `big(3, 5, 4)` and `big(3.0, 5.0, 4)` each call, and what do they return?

   <details>
   <summary>Answer</summary>

   `big(3, 5, 4)` has three `int` arguments, so it calls the integer version, which returns 4 because the larger value 5 exceeds the limit 4. For `big(3.0, 5.0, 4)`, matching the integer version requires converting the first two arguments from `double` to `int`, and matching the floating-point version requires converting the third argument from `int` to `double`. The floating-point version fits the first two arguments better and the integer version fits the third argument better, so no function is better or equal for every argument, and the call is an ambiguity error.

   </details>
