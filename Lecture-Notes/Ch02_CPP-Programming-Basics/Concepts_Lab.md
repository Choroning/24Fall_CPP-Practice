# Chapter 2 Lab — C++ Basics Review

[Repository Guide](../../README.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **Last Updated:** 2026-11-18

> **Learning Objectives:**
> 1. Distinguish literals from variables and the left side from the right side of the assignment operator, and explain the input and output of `bool` values
> 2. Compute the results of arithmetic, relational, logical, compound assignment, increment and decrement, and conditional operators
> 3. Control flow with `if`, `else if`, and `switch` and with `for`, `while`, `do-while`, `break`, and `continue`
> 4. Keep array indices within range and access variables and array elements through pointers
> 5. Define functions with parameters and return values and pass arrays and pointers as arguments

---

## Table of Contents

- [1. Overview](#1-overview)
- [2. Data](#2-data)
  - [2.1 Literals, Variables, and Assignment](#21-literals-variables-and-assignment)
  - [2.2 Data Types and bool](#22-data-types-and-bool)
- [3. Operators](#3-operators)
  - [3.1 Arithmetic, Relational, and Logical Operators](#31-arithmetic-relational-and-logical-operators)
  - [3.2 Compound Assignment Operators](#32-compound-assignment-operators)
  - [3.3 Increment and Decrement Operators](#33-increment-and-decrement-operators)
  - [3.4 The Conditional Operator](#34-the-conditional-operator)
- [4. Conditional Statements](#4-conditional-statements)
- [5. Loops](#5-loops)
  - [5.1 for, while, and do-while](#51-for-while-and-do-while)
  - [5.2 break and continue](#52-break-and-continue)
- [6. Arrays](#6-arrays)
- [7. Pointers](#7-pointers)
- [8. Functions](#8-functions)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overview

> **Note:** The basic elements of a C++ program, console input and output with `cout` and `cin`, and namespaces and `std::` are covered in the [Chapter 2 lecture notes](Concepts_Lecture.md). The textbook assumes that the basic syntax of C (data, operators, control statements, arrays, pointers, functions) is already known, so this lab quickly reviews that material in C++ code.

| Topic | Location |
|:-----|:-----|
| Comments, `main()`, `#include <iostream>`, `cout` and `<<`, namespaces, `cin` and `>>` | [Sections 1-4 of the Chapter 2 lecture notes](Concepts_Lecture.md) |
| Literals, variables, assignment, data types | Section 2 |
| Operators | Section 3 |
| Conditional statements and loops | Sections 4 and 5 |
| Arrays, pointers, functions | Sections 6-8 |

*Table 1. Topics of the lab and their location*

All code is compiled and run with the C++17 standard (for example, `g++ -std=c++17 -Wall`). Readers who do not fully understand this basic syntax should supplement it with a C textbook in order to follow the later chapters.

---

<br>

## 2. Data

### 2.1 Literals, Variables, and Assignment

> **Definition:** A **literal** is a value written directly in code, such as `2`, `'a'`, or `"Hello"`, that is assigned to a variable or used as an operand of an operation or an argument of a function.

> **Definition:** A **variable** is the combination of a data type and a name (identifier) that represents one or more memory locations holding a program's data.

In `int val_1 = 2;`, `val_1` is a variable and `2` is a literal. The left side of `=` denotes the memory location where a value is stored, so it is called the **L-value** (left-hand side), and the right side denotes a value, so it is called the **R-value** (right-hand side).

- In mathematics, `=` is the equal sign, meaning that the left and right sides are equal.
- In programming languages, `=` is the **assignment operator**, which stores the value on the right in the memory location on the left. The right side is always evaluated first and the result stored on the left, so `sum = sum + 1;` means "add 1 to the current value of sum and store it back in sum."

```cpp
int val_1 = 2;      // Assign a value at declaration: initialization
int val_2 = 3;
int sum;            // Declaration only: memory is reserved but not initialized
sum = val_1 + val_2;
```

```text
         memory
       ┌─────────┐
val_1  │    2    │
       ├─────────┤
val_2  │    3    │
       ├─────────┤
sum    │ ??? → 5 │   garbage right after declaration
       └─────────┘
```

*Figure 1. Memory state of variables with and without initialization*

The uninitialized local variable `sum` holds a garbage value left over in that memory, and reading it before assignment is undefined behavior.

### 2.2 Data Types and bool

A **data type** determines the storage requirements, handling, and behavior of variables and function parameters. The basic data types of C++ include `void`, `char`, `short`, `int`, `long`, `long long`, `float`, `double`, and `bool`. C++ provides the `bool` type and the constants `true` and `false` as keywords.

```cpp
#include <iostream>
using namespace std;

int main() {
    bool test = true;
    cout << "1. bool: " << test << ", " << true << endl;
    test = false;
    cout << "2. bool: " << test << ", " << false << endl;
    if (true)
        cout << "TRUE" << endl;
    if (false)
        cout << "FALSE" << endl;
    if (-1)                             // Any nonzero value is true
        cout << "TRUE::-1" << endl;
}
```

```text
1. bool: 1, 1
2. bool: 0, 0
TRUE
TRUE::-1
```

- By default `cout` prints `bool` values as `1` (true) and `0` (false). Sending `cout << boolalpha` first prints them as `true` and `false`.
- In conditions, 0 is false and every nonzero value is true, so `if (-1)` is also true.

---

<br>

## 3. Operators

An **operator** is a symbol that represents a specific mathematical or logical action. For example, `*` is an arithmetic operator that represents multiplication, and `&&` is a logical operator that represents logical AND.

### 3.1 Arithmetic, Relational, and Logical Operators

| Kind | Operator | Meaning |
|:-----|:-----|:-----|
| Arithmetic | `+`, `-`, `*`, `/` | Addition, subtraction, multiplication, division (`/` between integers gives the quotient) |
| Arithmetic | `%` | Remainder of dividing the left by the right |
| Relational | `==`, `!=` | True if the two values are equal, true if they differ |
| Relational | `>`, `<`, `>=`, `<=` | True if the size comparison holds |
| Logical | `&&` | True only if both operands are true |
| Logical | `\|\|` | True if either operand is true |
| Logical | `!` | Reverses the truth of its operand |

*Table 2. Arithmetic, relational, and logical operators*

```cpp
#include <iostream>
using namespace std;

int main() {
    int sum, apple = 7, mango = 3;

    sum = apple + mango;
    cout << "sum: " << sum << endl;
    cout << "+ : " << apple + mango << endl;
    cout << "/ : " << apple / mango << endl;            // Integer division: quotient
    cout << "% : " << apple % mango << endl;            // Remainder
    cout << "--------------" << endl;

    cout << "== : " << (apple == mango) << endl;
    cout << "!= : " << (apple != mango) << endl;
    cout << "<= : " << (apple <= mango) << endl;
    cout << "&& : " << ((apple != mango) && (apple <= mango)) << endl;
    cout << "|| : " << ((apple == mango) || (apple >= mango)) << endl;
    cout << " ! : " << !(true) << endl;
}
```

```text
sum: 10
+ : 10
/ : 2
% : 1
--------------
== : 0
!= : 1
<= : 0
&& : 0
|| : 1
 ! : 0
```

- 7 / 3 is integer division, giving the quotient 2, and 7 % 3 gives the remainder 1.
- Relational and logical expressions are printed inside parentheses. Writing `cout << apple == mango` would be parsed differently from the intent, because `<<` has higher precedence than `==`.
- `(apple != mango) && (apple <= mango)` is false because, although the first part is true (1), the second is false (0).

### 3.2 Compound Assignment Operators

| Operator | Example | Meaning |
|:-----|:-----|:-----|
| `=` | `a = b` | Assign the value of `b` to `a` |
| `+=` | `a += b` | `a = a + b` |
| `-=` | `a -= b` | `a = a - b` |
| `*=` | `a *= b` | `a = a * b` |
| `/=` | `a /= b` | `a = a / b` |
| `%=` | `a %= b` | `a = a % b` |

*Table 3. The assignment operator and compound assignment operators*

### 3.3 Increment and Decrement Operators

`++` and `--` increase or decrease their operand by 1. The prefix form (`++a`) changes the value first and then uses it, while the postfix form (`a++`) uses the current value and then changes it.

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 2, b = 3;

    a++;
    ++b;
    cout << "[1] a: " << a << ", b: " << b << endl;

    b = ++a;                            // Increment a first, then assign
    cout << "[2] a: " << a << ", b: " << b << endl;

    b = a++;                            // Assign the current value of a, then increment
    cout << "[3] a: " << a << ", b: " << b << endl;

    b = ++a + 2;
    cout << "[4] a: " << a << ", b: " << b << endl;

    b = a++ + 2;
    cout << "[5] a: " << a << ", b: " << b << endl;
}
```

```text
[1] a: 3, b: 4
[2] a: 4, b: 4
[3] a: 5, b: 4
[4] a: 6, b: 8
[5] a: 7, b: 8
```

- [1] Used alone, `a++` and `++b` make no difference; both increase by 1.
- [2] `b = ++a` makes `a` 4 and then assigns 4 to `b`.
- [3] `b = a++` assigns the current value 4 of `a` to `b` and then makes `a` 5.
- [4] `++a + 2` gives 6 + 2 = 8; [5] `a++ + 2` assigns 6 + 2 = 8 and then makes `a` 7.

### 3.4 The Conditional Operator

The conditional operator `condition ? expr1 : expr2` takes the value of expr1 if the condition is true and of expr2 if it is false. It is the only operator with three operands, so it is also called the ternary operator.

```cpp
int number = -4;
string result;
result = (number > 0) ? "Positive Number!" : "Negative Number!";   // "Negative Number!"
```

> **Note:** This expression also chooses `"Negative Number!"` when `number` is 0. Zero is neither positive nor negative, so to distinguish the three cases, nest conditional operators, as in `(number > 0) ? "Positive" : (number < 0) ? "Negative" : "Zero"`, or use an `if` statement.

---

<br>

## 4. Conditional Statements

C++ provides conditional statements that use the relational conditions of mathematics (`a < b`, `a == b`, and so on) to act differently in different situations.

- `if` specifies a block to execute when a condition is true.
- `else` specifies a block to execute when the same condition is false.
- `else if` specifies a new condition to test when the previous condition is false.
- `switch` chooses one of several blocks according to a single value.

The following code reads a score and prints a grade, written with `switch`. The same can be written with `if (score >= 90) ... else if (score >= 80) ... else ...`.

```cpp
#include <iostream>
using namespace std;

int main() {
    int score;
    cout << "Enter your test score: ";
    cin >> score;

    if (score >= 0 && score <= 100) {
        switch (score / 10) {           // Branch on the tens digit
        case 10:
        case 9:                         // 10 and 9 execute the same statements
            cout << "Grade: A \n";
            break;
        case 8:
            cout << "Grade: B \n";
            break;
        default:
            cout << "Grade: F \n";
            break;
        }
    }
    else
        cout << "[Input error] Exit the program!" << endl;
}
```

| Input | Output |
|:-----|:-----|
| 100 | `Grade: A` |
| 85 | `Grade: B` |
| 42 | `Grade: F` |
| 120 | `[Input error] Exit the program!` |

*Table 4. Grade output for the entered score*

- `score / 10` is integer division, so 85 becomes 8 and 100 becomes 10. `switch` jumps to the `case` equal to this value.
- After `case 10:` there is neither a statement nor a `break`, so execution continues into `case 9:` below. This property, that without `break` execution falls through to the next `case`, is used to execute the same statements for several values.
- Omitting `break` at the end of a `case` executes the statements of the next `case` too, so a `break` must always be placed unless that is intended.

---

<br>

## 5. Loops

### 5.1 for, while, and do-while

A loop repeats statements zero or more times until its termination condition becomes false (0) or it is exited with `break`. Printing and summing 1 through 4 without a loop requires listing the same code, as in `sum = sum + 1;` and `sum = sum + 2;`, which becomes unmanageable when the range grows to 1 through 10,000. With a changing variable `i` and a loop, only three things need to be decided.

1. The starting value of `i` (initialization): `i = 1`
2. How long `i` keeps changing (condition): continue while `i < 5`, stop when it reaches 5
3. The amount by which `i` changes (update): `i++`

| Loop | Where the condition is tested | Initialization | Update |
|:-----|:-----|:-----|:-----|
| `while` | At the top of the loop | None | None |
| `do-while` | At the bottom of the loop | None | None |
| `for` | At the top of the loop | Yes | Yes |
| Range-based `for` | At the top of the loop | Yes | Yes |

*Table 5. The four loops of C++*

`for (①; ②; ③) { body; }` is the same as `①; while (②) { body; ③; }`. `do-while` tests its condition at the end, so its body runs at least once. The following code computes the sum of all integers between two integers with both `for` and `do-while`.

```cpp
#include <iostream>
using namespace std;

int main() {
    int i, a, b, large, small, sum = 0;
    cout << "Please enter two integers >> ";
    cin >> a >> b;

    if (a > b) { large = a; small = b; }
    else { large = b; small = a; }

    for (i = small; i <= large; i++)    // Sum from small to large
        sum += i;
    cout << small << "에서 " << large << "까지 합은 " << sum << endl;

    sum = 0;
    i = small;
    do {                                // Run the body once first, then test the condition
        sum += i;
        i++;
    } while (i <= large);
    cout << "do-while: " << sum << endl;
}
```

```text
Please enter two integers >> 7 3
3에서 7까지 합은 25
do-while: 25
```

### 5.2 break and continue

- **break:** Immediately exits the nearest loop (or `switch`). An infinite loop whose condition is always true, such as `while (true)`, ends only through `break`.
- **continue:** Skips the rest of the loop body and moves on to the next iteration. In `for` it goes to the update expression, in `while` to the condition, and in `do-while` to the condition at the end.

```cpp
#include <iostream>
using namespace std;

int main() {
    int i = 1;
    while (true) {
        i++;
        if (i % 2 == 1)
            continue;                   // If odd, skip the rest and go to the condition
        cout << i << endl;
        if (i == 6)
            break;                      // End the loop
    }
}
```

```text
2
4
6
```

> **Caution:** In a `while` loop, if `continue` comes before the update statement, the update is skipped and the loop can become infinite. The code above avoids this by placing `i++` at the very beginning of the loop. In the same situation, `for` still executes its update expression after `continue`.

---

<br>

## 6. Arrays

An **array** is a sequence of objects of the same type placed in a contiguous area of memory. C-style arrays are used in C++ as they are, but they are the source of many bugs, so modern C++ recommends `std::vector` or `std::array` (Chapter 10).

```cpp
int n[10];                              // An array of 10 integers
double d[] = { 0.1, 0.2, 0.5, 3.9 };    // Size is the number of initial values, 4
n[10] = 20;                             // Error: indices 0-9 only
d[-1] = 9.9;                            // Error: negative indices cannot be used
int m[2][5];                            // A two-dimensional array of 2 rows and 5 columns
m[2][0] = 5;                            // Error: row indices 0-1 only
```

The indices of an array of size N run from 0 to N - 1. An index out of range is not a compile error but undefined behavior that reads and writes other memory at run time, which calls for extra care.

```cpp
#include <iostream>
using namespace std;

int main() {
    int i, x[3];
    double y[] = { 1.1, 2.2, -3.4, 5.7 };   // Size is the number of initial values, 4

    for (i = 0; i < 3; i++) {
        cout << "[" << i + 1 << "] data input: ";
        cin >> x[i];
    }
    for (i = 0; i < 3; i++)
        cout << x[i] << endl;
    cout << "========" << endl;

    y[0] = y[0] + 8;
    for (i = 0; i < 4; i++)
        cout << y[i] << endl;
}
```

```text
[1] data input: 3
[2] data input: -6
[3] data input: 2
3
-6
2
========
9.1
2.2
-3.4
5.7
```

---

<br>

## 7. Pointers

> **Definition:** A **pointer** is a variable that stores the memory address of an object.

Pointers are used to allocate new objects on the heap, to pass functions to other functions, and to traverse the elements of data structures such as arrays. Raw pointers, however, are a source of serious errors, so modern C++ provides smart pointers for allocating objects, iterators for traversing data structures, and lambda expressions for passing functions (Chapters 4 and 10).

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 3, b;
    int* p = &a;                        // Declare and initialize a pointer

    cout << "[a] " << a << ", [*p] " << *p << endl;
    cout << boolalpha << "p == &a: " << (p == &a) << endl;

    *p = *p + 2;                        // Change a, which p points to
    b = *p;
    cout << "[a] " << a << ", [b] " << b << endl;

    int arr[3] = { 1, -3 };             // The remaining element is 0
    int* q = arr;                       // The array name is the address of the first element
    cout << "arr == &arr[0]: " << (arr == &arr[0]) << endl;
    for (int i = 0; i < 3; i++)
        cout << q[i] << ' ';            // Access by subscript
    cout << endl;
    for (int i = 0; i < 3; i++) {
        cout << *q << ' ';              // Access by moving the pointer
        q++;
    }
    cout << endl;
}
```

```text
[a] 3, [*p] 3
p == &a: true
[a] 5, [b] 5
arr == &arr[0]: true
1 -3 0 
1 -3 0 
```

- `&a` is the address of `a`, and `*p` is the value at the location `p` points to. `*p = *p + 2;` changes `a` to 5.
- When a pointer is declared with no target yet, it is initialized with `int* p = nullptr;` so that it does not hold an arbitrary address.
- If there are fewer initial values than elements, the remaining elements are initialized to 0, so `arr[2]` is 0.

---

<br>

## 8. Functions

A **function** is a block of code that performs some task. It can define parameters so that the caller can pass arguments, and it can return a result as its return value. The parameter types in the function definition and the types of the arguments passed in a call must be compatible. To turn the mathematical function y = x + 3 into C++, decide the function's name (`getPlus`), the information it receives (`int x`), what it does (`y = x + 3`), and the value it returns (`return y;`) along with its type (`int`).

```cpp
#include <iostream>
using namespace std;

int getQuad(int a, int x);                      // Function prototypes
double getSum(double* ob, int size);
bool equalArray(int* p, int* q, int size);

int main() {
    int a = 1, x = 3, result;
    result = getQuad(a, x);                     // Two arguments; the return value goes to result
    cout << "1. result: " << result << '\n';
    cout << "2. result: " << getQuad(2, 4) << endl;

    double d[4] = { 1.1, -3.2, 5.6 };
    cout << getSum(d, 4) << endl;

    int s[] = { 1, 2, 3, 4, 5 };
    int t[] = { 1, 2, 3, 4, 5 };
    if (equalArray(s, t, 5))
        cout << "arrays equal" << '\n';
    else
        cout << "arrays not equal" << '\n';
}

int getQuad(int a, int x) {                     // y = a^2 + x - 3
    int y;
    y = a * a + x - 3;
    return y;
}

double getSum(double* ob, int size) {           // Sum of the array elements
    double sum = 0;
    for (int i = 0; i < size; i++)
        sum = sum + ob[i];
    return sum;
}

bool equalArray(int* p, int* q, int size) {     // Compare the elements of two arrays in turn
    for (int i = 0; i < size; i++) {
        if (*p != *q)
            return false;
        p++;                                    // Point to the next element
        q++;
    }
    return true;
}
```

```text
1. result: 1
2. result: 5
3.5
arrays equal
```

- **Function prototype:** To call from `main()` a function defined after `main()`, a prototype (declaration) is placed before it to tell the compiler the name, parameters, and return type.
- **getQuad:** With a = 1 and x = 3 the result is 1 + 3 - 3 = 1, and with a = 2 and x = 4 it is 4 + 4 - 3 = 5.
- **Array arguments:** Passing an array passes the address of its first element, so the parameter is written as `double* ob` or `double ob[]`, and since the function cannot know the array's size, it receives `size` separately. The fourth element of `d` is 0, so the sum is 1.1 - 3.2 + 5.6 = 3.5.
- **bool return:** `equalArray()` returns `false` as soon as it meets a differing element and returns `true` if all elements match. Its return value is true or false, so it can be used directly as the condition of an `if`.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Literals and variables | A literal is a value written directly in code, a variable denotes a memory location through a data type and a name, and an uninitialized local variable holds a garbage value. |
| Assignment | `=` is the operator that evaluates the right side (R-value) first and stores it in the memory of the left side (L-value). |
| `bool` | `true` and `false` are printed by `cout` as 1 and 0, and in conditions every nonzero value is true. |
| Operators | `/` between integers gives the quotient and `%` the remainder; prefix increment changes then uses, postfix uses then changes. |
| Conditional statements | `if`, `else if`, `else`, and `switch` branch, and a `case` of `switch` falls through to the next `case` without `break`. |
| Loops | `while` and `for` test their condition at the top and `do-while` at the bottom; `break` ends a loop and `continue` moves to the next iteration. |
| Arrays | The indices of an array of size N are 0 to N - 1, and out-of-range access is undefined behavior. |
| Pointers | A variable that stores an address; `&` gets an address, `*` accesses the value pointed to, and an array name is the address of its first element. |
| Functions | They are declared in advance with prototypes, and an array is passed as the address of its first element together with its size. |

---

<br>

## Self-Check Questions

1. After `int a = 5, b;`, if `b = a++ * 2;` and `b = ++a * 2;` are executed in turn, what are `a` and `b` after each statement?

   <details>
   <summary>Answer</summary>

   `b = a++ * 2;` assigns 10, the current value 5 of `a` times 2, to `b` and then makes `a` 6. Next, `b = ++a * 2;` makes `a` 7 and then assigns 7 × 2 = 14 to `b`. So after the first statement a = 6 and b = 10, and after the second a = 7 and b = 14.

   </details>

2. If the `break;` of `case 8:` in the `switch` of Section 4 is removed and 85 is entered, what is printed?

   <details>
   <summary>Answer</summary>

   `Grade: B` and `Grade: F` are printed in turn. `score / 10` is 8, so the statements of `case 8:` run, and since there is no `break`, execution continues into the statements of the next label, `default:`, and leaves the `switch` at the `break` there.

   </details>

3. If the `do-while` of Section 5.1 is changed to `while (i <= large) { sum += i; i++; }`, is there any input for which the result differs?

   <details>
   <summary>Answer</summary>

   Not in this program. `small` is always at most `large`, so the condition `i <= large` is true from the start, and both loops execute their bodies the same number of times. The two loops differ when the condition is false at the start: then `while` never executes its body, while `do-while` executes it once.

   </details>

4. Given the array `int x[3] = { 4, 5, 6 };` and `int* p = x;`, what are the values of `*(p + 2)`, `p[1]`, and `*p + 2`?

   <details>
   <summary>Answer</summary>

   `*(p + 2)` is the value 6 of `x[2]`, two elements after `p`. `p[1]` is the same as `*(p + 1)`, so it is the value 5 of `x[1]`. In `*p + 2`, `*` applies before `+`, so it is the value 4 of `x[0]` plus 2, that is 6.

   </details>
