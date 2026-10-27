# HW8 Assignment Explanation — Friend Functions and Operator Overloading

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

> **Learning Objectives:**
> 1. Overload an operator whose left operand is not an object of the class as a friend function, and explain why it cannot be implemented as a member function
> 2. Write prefix `++` and postfix `++` as separate friend functions and trace the return value of each function and the change in the object's state
> 3. Implement the same operators as member functions and as friend functions and compare the parameter lists and call forms of the two approaches
> 4. Explain how the return type of an operator function affects chained expressions and the use of the return value

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. A Friend Operator Function That Adds an Integer and an Object](#2-q1-a-friend-operator-function-that-adds-an-integer-and-an-object)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. Prefix and Postfix Increment Operators Overloaded as Friend Functions](#3-q2-prefix-and-postfix-increment-operators-overloaded-as-friend-functions)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [4. Q3. Operators of the Matrix Class That Exchange Elements with an Array](#4-q3-operators-of-the-matrix-class-that-exchange-elements-with-an-array)
  - [4.1 Requirements](#41-requirements)
  - [4.2 Design and Solution](#42-design-and-solution)
  - [4.3 Output and Review](#43-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the friend functions and operator overloading of Chapter 7. The three problems are [Lab 1], [Lab 2], and [Lab 3] of the Week 13 lecture slides. The assignment handout is transcribed in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW8 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW08_Friend-Functions-and-Operator-Overloading/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # a friend + operator that computes 1 + a
    ├── Q2.cpp               # prefix and postfix ++ operators overloaded as friend functions
    └── Q3.cpp               # >> and << operators as member functions, with a friend version in a comment
```

*Figure 1. Directory layout of HW8*

Each solution is compiled and run as follows. All three files compile with `g++ -std=c++11 -Wall` and `g++ -std=c++17 -Wall` under Apple clang 21 on macOS.

```bash
cd Coursework/HW08_Friend-Functions-and-Operator-Overloading/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

| File | Compilation | Output |
|:-----|:-----|:-----|
| `Q1.cpp` | No warnings | Same as the submitted screenshot |
| `Q2.cpp` | No warnings | Same as the submitted screenshot |
| `Q3.cpp` | No warnings | Same as the submitted screenshot |

*Table 1. Compilation and output of the HW8 solutions*

The last line of the submitted screenshots, `Program ended with exit code: 0`, is not program output but a message Xcode shows in the console after the program ends. None of the three `main()` functions has a `return` statement, but in C++ reaching the end of `main()` is treated as returning 0, so the exit code is 0.

---

<br>

## 2. Q1. A Friend Operator Function That Adds an Integer and an Object

### 2.1 Requirements

Add an operator to the `Circle` class so that `b = 1 + a;` changes the radius of `b` to the radius of `a` plus 1. This is a binary operation whose left operand is an `int` and whose right operand is a `Circle`, so only one of the two ways of implementing an operator function can be used.

| Approach | Translation of `1 + a` | Possible? |
|:-----|:-----|:-----|
| Member function | `1.operator+(a)` | No. `int` is not a class, so no member function can be added to it |
| External function | `operator+(1, a)` | Yes. `radius` is `private`, so the function is declared a friend of `Circle` |

*Table 2. Comparison of the two ways to implement `1 + a`*

### 2.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Circle` class represents a circle and keeps the radius `radius` as a `private` member variable. Under `public` are the constructor and `show()`, and the `friend` keyword lets the `+` operator function outside the class access the `private` members.

- **Constructor:** takes the radius `radius` with a default value of 0 and assigns the parameter to the member variable with `this->radius = radius;`.
- **`show()`:** prints the radius, as in `radius = 5 인 원` ("a circle with radius = 5").
- **Friend declaration:** `friend Circle operator+ (int op1, Circle op2);` lets the external function of this name access the `private` members of `Circle`. A friend declaration does not make the function a member of the class.
- **`operator+(int op1, Circle op2)`:** defined outside the class without `Circle::`. It creates a temporary object `tmp` with the default constructor, stores `op1 + op2.radius` in `tmp.radius`, and returns `tmp` by value. As a friend it can access `tmp.radius` and `op2.radius` directly ([Chapter 7, Section 1.1](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#11-friend-function), [Chapter 7, Section 3.2](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#32-as-friend-function)).

`main()` creates `a` with radius 5 and `b` with radius 4 and, with `b = 1 + a;`, assigns to `b` the object of radius 6 returned by `operator+(1, a)`. `a` is copied into the value parameter `op2`, so it does not change.

The following four points are properties of this design.

- **Order of the operands:** This function handles only the case where `int` is on the left and `Circle` on the right. For `a + 1`, a `Circle` cannot be passed to the first parameter, `int`, so the error `invalid operands to binary expression ('Circle' and 'int')` occurs. To support `a + 1` as well, a member function `Circle operator+(int op)` or a separate `friend Circle operator+(Circle op1, int op2)` is added.
- **Role of the friend declaration:** If the `friend` declaration in the class is removed, the external function `operator+` accesses `private` members, giving the error `'radius' is a private member of 'Circle'`. If `Circle` had `public` get/set functions, the operator could also be implemented as an ordinary external function without friendship.
- **How the parameters are passed:** `op2` is passed by value, so `a` is copied on every call. A binary operator that does not change its operands usually takes them as `const Circle&`.
- **Chained addition:** `1 + 2 + a` is `(1 + 2) + a`, so the integer addition comes first and it becomes `operator+(3, a)`, producing a circle of radius 8. In contrast, `1 + a + 2` is `(1 + a) + 2`, that is, `Circle + int`, which does not compile with this code.

### 2.3 Output and Review

```text
radius = 5 인 원
radius = 6 인 원
```

The output is the same as the submitted screenshot and the sample run of the slides. The first line is the unchanged `a`, and the second line is `b`, which received the result of `1 + a`. The radius 4 that `b` had at first is lost by the assignment.

---

<br>

## 3. Q2. Prefix and Postfix Increment Operators Overloaded as Friend Functions

### 3.1 Requirements

Write the prefix `++` and the postfix `++` of the `Circle` class as friend functions. `++a;` increases the radius of `a` by 1, and `b = a++;` assigns `a` before the increment to `b` and then increases the radius of `a` by 1.

| Statement | Friend function call | Return value | `a`, `b` afterwards |
|:-----|:-----|:-----|:-----|
| `++a;` | `operator++(a)` | A copy with radius 6 (not used) | `a` is 6, `b` is 4 |
| `b = a++;` | `operator++(a, 0)` | A copy with radius 6 (assigned to `b`) | `a` is 7, `b` is 6 |

*Table 3. Calls of the prefix and postfix `++` in Q2*

### 3.2 Design and Solution

The member variable, the constructor, and `show()` of the `Circle` class are the same as in Q1. The class contains the friend declarations of the two `operator++` functions, and their bodies are defined outside the class.

- **Prefix `operator++(Circle& op)`:** increases the `radius` of the object received by reference by 1 and returns the object. The return type is `Circle`, so a copy of the object after the increment is returned.
- **Postfix `operator++(Circle& op, int notUsed)`:** keeps a copy of the state before the increment with `Circle tmp = op;`, increases the `radius` of the original object by 1, and returns `tmp`. The value of the second parameter, `int`, is not used; it exists only to distinguish prefix from postfix. The compiler calls `a++` by passing a meaningless integer, as in `operator++(a, 0)` ([Chapter 7, Section 4.3](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#43-increment--decrement)).

Both functions take the operand as `Circle&`. A friend function has no `this`, so it must receive the object to change as a parameter, and receiving it by value changes only the copy inside the function. Indeed, changing both parameters to `Circle op` leaves `a` at radius 5 to the end, and `radius = 5 인 원` is printed twice.

`main()` creates `a` with radius 5 and `b` with radius 4 and makes `a` 6 with `++a;`. In `b = a++;`, the postfix function returns the copy before the increment (radius 6), which is assigned to `b`, while `a` becomes 7.

Returning `Circle` by value from the postfix function is correct. The state before the increment remains only in the local object `tmp`, so returning a reference would refer to an object that disappears when the function ends. The prefix `++`, in contrast, yields the incremented object itself as its result, so it usually returns a reference, as in `Circle& operator++(Circle& op)` ([Chapter 7, Section 4.4](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#44-prefix-vs-postfix-detailed-trace)). When the prefix version returns a copy, as in this solution, its result is a temporary object, which leads to the following differences.

- **`++(++a)`:** The inner `++a` returns a temporary copy, but the outer `operator++` takes `Circle&`, to which a temporary cannot be passed. So it does not compile, with the error `cannot increment value of type 'Circle'`. With the return type changed to `Circle&`, it compiles and `a` is incremented twice to radius 7.
- **Difference from a member function:** If the same prefix `++` is written as a member function `Circle operator++()` that returns a copy, `++(++a)` compiles, because a member function can be called on a temporary object as well. However, the second increment applies to the temporary object, and `a` ends at radius 6, incremented only once.
- **Cost of copying:** The object is copied once on every return, even for `++a;`, whose result is not used.

`notUsed` is a parameter whose value is not used, so compiling with `-Wextra` added gives the warning `unused parameter 'notUsed'`. `-Wall` does not include this warning. Omitting the parameter name in the definition, as in `Circle operator++(Circle& op, int)`, removes the warning.

### 3.3 Output and Review

```text
radius = 7 인 원
radius = 6 인 원
```

The output is the same as the submitted screenshot and the sample run of the slides. Both comments in `main()` say that the radius is increased by 1, and indeed both statements increase the radius of `a` by 1, from 5 to 7. The difference between the two statements lies in the return value: `b` receives 6, the value before the second increment.

---

<br>

## 4. Q3. Operators of the Matrix Class That Exchange Elements with an Array

### 4.1 Requirements

In the `Matrix` class, which stores the four elements of a 2×2 matrix in the array `ar`, make `a >> x` copy the elements of `a` into the array `x` and make `b << y` set the elements of `b` from the elements of the array `y`. Part (1) implements the two operator functions as member functions of `Matrix`, and part (2) as friend functions.

| Expression | Call as a member function | Call as a friend function |
|:-----|:-----|:-----|
| `a >> x` | `a.operator>>(x)` | `operator>>(a, x)` |
| `b << y` | `b.operator<<(y)` | `operator<<(b, y)` |

*Table 4. Operator expressions of Q3 and the call forms of the two approaches*

### 4.2 Design and Solution

The member variable `ar[4]`, the two constructors, and `show()` of the `Matrix` class are the same as the code given in the problem. The default constructor initializes the four elements to 0, the four-argument constructor initializes `ar[0]` through `ar[3]` in order, and `show()` prints the elements in the form `Matrix = { 1 2 3 4 }`. The `<<` and `>>` here have a `Matrix` as their left operand, so they can be implemented as member functions; this differs from the stream output `<<`, whose left operand is an `ostream` ([Chapter 7, Section 5](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#5-stream-operator-overloading)).

In the member function version of (1), the `Matrix` object that is the left operand is `this`, so the only parameter is the array, the right operand.

- **`int* operator >> (int op[])`:** copies the four elements of `ar` into `op` and returns `op`. The parameter `int op[]` is in fact a pointer `int*`, so the values are stored in the array `x` of `main()` itself.
- **`Matrix& operator << (int op[])`:** copies the four elements of `op` into `ar` and returns a reference to `*this`.

The friend function version of (2) is in the block comment inside the class (lines 41-52). A friend function has no `this`, so it receives the `Matrix` that is the left operand as the first parameter `op1` and the array as the second parameter `op2`. Although the bodies are defined inside the class, a function marked `friend` is not a member function, and it accesses `private` members through its parameters, as in `op1.ar` ([Chapter 7, Section 2.4](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#24-member-function-vs-friend-function-decision-guide)).

- **`friend int* operator>>(Matrix& op1, int op2[])`:** copies the elements of `op1.ar` into `op2` and returns `op2`.
- **`friend Matrix& operator<<(Matrix& op1, int op2[])`:** copies the elements of `op2` into `op1.ar` and returns a reference to `op1`. Because `op1` is received by reference, `b` itself changes; receiving it by value would change only a copy.

The two versions differ only in their parameter lists; what they do and what they return are the same. The return types were chosen so that the result of an operation can be used further. `<<` returns a reference to the object, so it can be chained, as in `b << y << x;`, which sets `b` to the values of `y` and then to the values of `x`, producing `Matrix = { 4 3 2 1 }`. `>>` returns the address of the array that received the values, so the result can be used directly as an array, as in `(a >> x)[0]`. However, `a >> x >> y` does not compile, because `(a >> x)` is an `int*` and no `>>` between an `int*` and an array is defined.

`main()` creates `a` as (4, 3, 2, 1) and `b` as a matrix whose elements are all 0, copies 4, 3, 2, 1 into `x` with `a >> x;`, and sets the elements of `b` to 1, 2, 3, 4 with `b << y;`. It then prints the elements of `x` and prints `b` with `b.show()`.

To switch to the friend function version, do the following.

1. Delete the member function version (lines 27-39) or turn each of its lines into a comment with `//`.
2. Change `/*` on line 41 to `//`, leaving only the description as a one-line comment. Removing only `/*` would make `(2) <<, >>를 ...` be read as code and cause a compile error.
3. Delete `*/` at the end of line 52.

The code changed in this way compiles without warnings with `-std=c++11` and `-std=c++17` and prints the same result as the member function version. If only the comment around the friend version is removed while the member function version is kept, `a.operator>>(x)` and `operator>>(a, x)` match `a >> x` equally well, so the error `use of overloaded operator '>>' is ambiguous` occurs, and the same happens for `<<`. That is why only one of the two versions must be active.

To make the code more robust, the size of the array can be considered. `int op[]` is a pointer, so the function cannot know the size of the array it receives and always reads or writes four elements. Passing an array with fewer than four elements accesses memory outside the bounds of the array, so taking a reference to an array of fixed size, `int (&op)[4]`, lets the compiler reject code that passes an array of a different size.

### 4.3 Output and Review

```text
4 3 2 1 
Matrix = { 1 2 3 4 }
```

The output is the same as the submitted screenshot and the sample run of the slides. The space at the end of the first line comes from `cout << x[i] << ' ';`, which appends a space after every element. The first line is the result of copying the elements of `a` into `x`, and the second line is `b`, set from the elements of `y`. Switching to the friend function version prints the same two lines.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Friend function | An external function declared with `friend` is not a member of the class but can access the `private` members of that class. |
| Operations whose left operand is a built-in type | No member function can be added to `int`, so `1 + a` can be implemented only as an external function of the form `operator+(1, a)`. |
| Distinguishing prefix and postfix | The postfix operator function has one more, unused `int` parameter, and the compiler calls `a++` as `operator++(a, 0)`. |
| Parameter of a friend unary operator | A friend function has no `this`, so the object to change must be received by reference, as in `Circle&`, for the original to change. |
| Return type of the prefix operator | The prefix `++` usually returns a reference; if it returns a copy, `++(++a)` does not compile in the friend version and increments only once in the member version. |
| Parameter lists of member and friend versions | The member version takes only the right operand and the friend version takes both operands as parameters, and having both versions makes calls ambiguous. |
| Return types for chaining | `<<` returning `Matrix&` can be chained, as in `b << y << x`, and `>>` returning `int*` lets the result be used directly as an array. |

---

<br>

## Self-Check Questions

1. **Limit of member functions:** Why can `1 + a` in Q1 not be implemented as a member function of `Circle`?

   <details>
   <summary>Answer</summary>

   A member operator function is called as a member of the left operand, so `1 + a` would have to be `1.operator+(a)`. But `int` is not a class, so no member function can be added to it. Therefore it is implemented as an external function `operator+(int, Circle)` that takes both operands as parameters, and it is declared a friend of `Circle` to access the `private` member `radius`.

   </details>

2. **Order of the operands:** What happens if `b = 1 + a;` in the `main()` of Q1 is changed to `b = a + 1;`, and what must be added to support this expression?

   <details>
   <summary>Answer</summary>

   A `Circle` cannot be passed to the first parameter of `operator+(int, Circle)`, so a compile error occurs. Adding `Circle operator+(int op)` as a member function, or declaring and defining `friend Circle operator+(Circle op1, int op2);`, makes `a + 1` compile, and `b` becomes a circle of radius 6.

   </details>

3. **Tracing prefix and postfix:** With the `Circle` class of Q2, if `Circle a(5), b(4), c(0); c = a++; b = ++a;` runs, what are the radii of `a`, `b`, and `c`?

   <details>
   <summary>Answer</summary>

   In `c = a++;`, `c` receives 5, the value before the increment, and `a` becomes 6. In `b = ++a;`, `a` becomes 7 and `b` receives 7, the value after the increment. So `a` is 7, `b` is 7, and `c` is 5.

   </details>

4. **Operands received by value:** What is the output if both `operator++` functions of Q2 take the operand as `Circle op` instead of `Circle& op`?

   <details>
   <summary>Answer</summary>

   Both functions increment only a copy of `a`, so the radius of `a` stays 5. The postfix function returns `tmp` (radius 5), a copy of the copy `op`, so `b` also becomes 5. The output is `radius = 5 인 원` twice.

   </details>

5. **Both versions together:** In Q3, what happens if the member function version is kept and only `/*` on line 41 and `*/` on line 52 are tidied up to activate the friend function version?

   <details>
   <summary>Answer</summary>

   For `a >> x`, the member function `a.operator>>(x)` and the friend function `operator>>(a, x)` match equally well, so the compiler cannot choose one and the error `use of overloaded operator '>>' is ambiguous` occurs. `b << y` gives the same error. One of the two versions must be removed or commented out.

   </details>
