# HW4 Assignment Explanation — References

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

> **Learning Objectives:**
> 1. Implement an integer stack with an array and a top index, and explain how push and pop return both the success status and the value taken out
> 2. Change and swap values through references, using the fact that a reference variable is an alias of the original variable
> 3. Explain why a `const` reference parameter reads a value without copying it while preventing changes
> 4. Explain the effect of a getter that returns a member by reference and the rule that selects between getters overloaded on `const` member functions

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. The MyIntStack Class That Returns Values through a Reference Parameter](#2-q1-the-myintstack-class-that-returns-values-through-a-reference-parameter)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. Free Problem 1 on References](#3-q2-free-problem-1-on-references)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [4. Q3. Free Problem 2 Combining References with Classes](#4-q3-free-problem-2-combining-references-with-classes)
  - [4.1 Requirements](#41-requirements)
  - [4.2 Design and Solution](#42-design-and-solution)
  - [4.3 Output and Review](#43-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the reference variables, reference parameters, and reference returns of Chapter 5. Q1 is [Lab Problem 2] of the Week 9 lecture slides, and Q2 and Q3 are code written freely on the subject of references. The assignment handout is transcribed in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW4 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW04_References/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # the MyIntStack class that returns values through a reference parameter
    ├── Q2.cpp               # free code with reference variables and reference parameters
    └── Q3.cpp               # free code with a Person class whose getters return references
```

*Figure 1. Directory layout of HW4*

Each solution is compiled and run as follows. All three files compile with `g++ -std=c++11 -Wall` and `g++ -std=c++17 -Wall` under Apple clang 21 on macOS and read no input.

```bash
cd Coursework/HW04_References/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

| File | Compilation | Output |
|:-----|:-----|:-----|
| `Q1.cpp` | No warnings | Same as the submitted screenshot |
| `Q2.cpp` | One warning: `-Wunused-variable`, because `constRef` is never used | Same as the submitted screenshot |
| `Q3.cpp` | No warnings | Same as the submitted screenshot |

*Table 1. Compilation and output of the HW4 solutions*

The three programs use no dynamic memory, and running them with AddressSanitizer and `leaks --atExit` reports no errors or leaks. The last line of the submitted screenshots, `Program ended with exit code: 0`, is not program output but a message Xcode shows in the console after the program ends.

---

<br>

## 2. Q1. The MyIntStack Class That Returns Values through a Reference Parameter

### 2.1 Requirements

Implement the integer stack `MyIntStack` with an array `p` and a top index `tos`. `push(n)` stores `n` and returns whether it succeeded, and `pop(n)` stores the value taken out in the reference parameter `n` and returns whether it succeeded. Following the one-page version of the slides, the size of the stack is the macro constant `MAX` (2), and `main()` tries to push and to pop `MAX + 1` times each to check the behavior on a full stack and on an empty stack.

### 2.2 Design and Solution

The program includes `<iostream>` for input/output and defines the maximum size of the stack with `#define MAX 2`. The `MyIntStack` class keeps the integer array `p[MAX]`, which is the storage, and the top position `tos` as `private` members; the prototypes of the constructor, `push()`, and `pop()` are under `public`, and their bodies are defined outside the class.

- **Constructor:** sets `tos` to 0 to make an empty stack. `tos` is both the index of the slot where the next value goes and the number of values stacked so far.
- **`push(int n)`:** If `tos < MAX`, there is still space, so it stores `n` in `p[tos]`, increments `tos` by 1, and returns `true`. If the stack is full, it changes nothing and returns `false`.
- **`pop(int& n)`:** If `tos > 0`, there is a value to take out, so it decrements `tos` by 1, stores `p[tos]` in `n`, and returns `true`. If the stack is empty, it returns `false`.

`push()` writes at `tos` and then increments it, and `pop()` decrements `tos` and then reads. Because of this order, the value put in last comes out first, which is **last in, first out (LIFO)**. `main()` pushes 0 and 1 in the first loop and prints a failure message on the third attempt, prints a separator line, then pops 1 and 0 in the second loop and prints a failure message on the third attempt.

`pop()` uses a reference parameter because the function must return two results. It returns the success status (`bool`) as the return value and writes the integer taken out directly into the caller's variable `n`. The parameter `int& n` is an alias of `n` in `main()`, so running `n = p[tos];` inside `pop()` changes `n` in `main()` ([Chapter 5, Section 3.2](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#32-reference-as-parameter)). If the parameter were taken as `int n`, the call would be a call by value and only the copy would change, so `n` in `main()` would keep its uninitialized value. Doing the same with a pointer requires declaring `bool pop(int* n)`, writing `*n = p[tos];`, and calling `a.pop(&n)`; with a reference the caller does not need to write `&`.

The following two points are properties of this design.

- **Size of the array:** The original size in the problem is 10, but the code follows the one-page version of the slides and sets `MAX` to 2. With `#define MAX 10`, the values 0 through 9 are pushed and then `11번째 시도 stack full` is printed, and the values 9 through 0 are popped and then `11번째 시도 stack empty` is printed. Because the size is set by a single macro constant, the array declaration, the condition in `push()`, and the loop counts in `main()` change together.
- **`n` on failure:** When `pop()` returns `false`, it writes nothing to `n`. The caller must therefore check the return value before using `n`, and `main()` prints `n` only when `if (a.pop(n))` holds.

### 2.3 Output and Review

```text
0 1 
3번째 시도 stack full
------------------------
1 0 
3번째 시도 stack empty
```

The output is the same as the submitted screenshot ("stack full/empty at the 3rd attempt"). It differs from the sample run of the slides only by the space in `3 번째` and the length of the separator line. The 0 and 1 on the first line echo the values pushed successfully, and the third push fails because `tos` is already `MAX` (2). The pops take out 1, the value put in last, first and print `1 0`, and the third pop fails because `tos` is 0.

---

<br>

## 3. Q2. Free Problem 1 on References

### 3.1 Requirements

Write any code related to references and analyse it. The submitted code shows three uses: a reference variable, reference parameters, and a `const` reference parameter.

### 3.2 Design and Solution

The program includes `<iostream>` for input/output and declares three functions that use references.

| Function | Parameters | What it does |
|:-----|:-----|:-----|
| `swapNumbers(int& a, int& b)` | Two reference parameters | Swaps the values of the two arguments through the temporary variable `temp` |
| `modifyValue(int& value)` | Reference parameter | Doubles the value of the argument |
| `printValue(const int& value)` | `const` reference parameter | Prints the value of the argument |

*Table 2. The three functions of Q2*

`main()` shows the uses of references in turn. It first makes `ref`, an alias of `number`, with `int& ref = number;`, runs `ref = 100;`, and prints `number` to show that a change through the reference reaches the original. A reference variable must be initialized when it is declared, and the object it refers to cannot be changed to another variable afterwards ([Chapter 5, Section 3.1](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#31-declaration)). It then swaps `num1` and `num2` with `swapNumbers()` and doubles `value` with `modifyValue()`. The parameters of both functions are references, so the changes inside the functions reach the variables of `main()` unchanged. Finally it prints the value with `printValue(value)`.

The parameter `const int&` of `printValue()` has two meanings. As a reference, it reads the original without copying the argument; as `const`, any attempt to change the original inside the function, as in `value = 0;`, is a compile error. A `const` reference can also take values that are not variables (literals or the results of expressions), as in `printValue(7)`. For a small type such as `int` there is almost no copying cost in taking it by value, so the benefit of a `const` reference grows when a `string` or a large class object is passed.

> **Note:** `const int& constRef = value;` at line 43 creates a `const` reference to `value`, but it is never used afterwards, and `printValue()` on the very next line takes `value`, not `constRef`, as its argument. That is why `-Wall` warns `unused variable 'constRef'`. Calling `printValue(constRef);` gives the same result without the warning. Either way, the parameter of `printValue()` is `const int&`, so the function cannot change the original.

### 3.3 Output and Review

```text
Original number: 42
After changing through reference: 100

Before swap - num1: 10, num2: 20
After swap - num1: 20, num2: 10

Before modification: 5
After modification: 10
Value is: 10
```

The output is the same as the submitted screenshot. Changing `ref` turned `number` from 42 into 100, the swap exchanged `num1` and `num2`, and after `modifyValue()` `value` went from 5 to 10. All three results come from the references referring to the originals.

---

<br>

## 4. Q3. Free Problem 2 Combining References with Classes

### 4.1 Requirements

Write any code that uses references together with a class and analyse it. The submitted code shows `const` reference parameters, reference returns, and `const` member functions with a `Person` class that has a name and an age.

### 4.2 Design and Solution

The program includes `<iostream>` and `<string>` for input/output and string handling. The `Person` class keeps the `private` members `name` and `age` and has the following member functions.

- **Constructor:** `Person(const string& n = "", int a = 0) : name(n), age(a) {}`. The parameters have default values, so an object can be created with no arguments or with one or two, and the member initializer list initializes the two members.
- **Two pairs of getters:** `string& getName()` and `int& getAge()` return references to the members, and `const string& getName() const` and `const int& getAge() const` return `const` references. Even with the same name and parameters, the trailing `const` differs, so the overloading is valid.
- **Setters:** `setName(const string& n)` and `setAge(const int& a)` assign the values received as `const` references to the members.
- **`isOlderThan(const Person& other) const`:** returns whether its own age is greater than the age of `other`. As a member function of the same class, it can access the `private` member `other.age` of another object.
- **`printInfo() const`:** prints the name and the age.

`celebrateBirthday(Person& person)`, outside the class, increases the age by 1 with `person.getAge()++;`. `getAge()` returns `int&`, that is, `age` inside the object itself, so `++` changes the member directly. If `getAge()` returned an `int` by value, `getAge()++` would be an increment of a temporary value and a compile error ([Chapter 5, Section 3.3](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#33-reference-return)).

`main()` creates `person1` ("Alice", 25) and `person2` ("Bob", 30), changes the name through the reference with `person1.getName() = "Alice Smith";`, and changes it again with `setName("Alice Johnson")` and `setAge(26)`. It prints the information of both people, compares their ages with `isOlderThan()`, and prints `person1` again after `celebrateBirthday(person1)`.

Which of the overloaded getters is called depends on whether the object is `const`. `person1` and `person2` are not `const`, so every call in `main()` selects the non-`const` version. With a `const` object, as in `const Person p3("Carol", 40);`, `p3.getName()` selects the `const` version, and `p3.getAge()++;` tries to change the value through a `const int&`, giving the error `cannot assign to return value because function 'getAge' returns a const value`. In this way the `const` version of a getter lets a `const` object read the value while preventing changes.

> **Caution:** A getter that returns a non-`const` reference lets outside code change a `private` member at will. `person1.getName() = "Alice Smith";` and `getAge()++` change the members without going through the setters, so even if a setter enforced a rule, such as rejecting a negative age, the rule could be bypassed. This code uses the approach to show the effect of reference returns; to keep encapsulation, getters should return a value or a `const` reference, and changes should go through setters or member functions such as `celebrateBirthday()`.

### 4.3 Output and Review

```text
Person 1: Name: Alice Johnson, Age: 26
Person 2: Name: Bob, Age: 30
Bob is older or same age

After birthday celebration:
Name: Alice Johnson, Age: 27
```

The output is the same as the submitted screenshot. The name `"Alice Smith"` set through the reference was overwritten right away by `setName()`, so it does not appear in the output. 26 is not greater than 30, so `isOlderThan()` returns `false` and the sentence of the `else` branch is printed, and `celebrateBirthday()` increased the age to 27 through the reference.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Array stack | `tos` is both the index of the next slot and the number of stacked values; push writes then increments and pop decrements then reads, which gives LIFO. |
| Passing results through a reference parameter | `pop(int& n)` returns the success status as the return value and passes the value taken out directly into the caller's variable. |
| Reference variable | An alias of the original, initialized at its declaration; changing the reference changes the original. |
| `const` reference parameter | Reads the original without a copy and prevents changes, and it can also take values such as literals as arguments. |
| Reference return | The return value of `int& getAge()` is the member itself, so it can be the target of assignment and increment, as in `getAge()++`. |
| Overloading on `const` member functions | A `const` object selects the `const` version of the getter, and an ordinary object selects the ordinary version. |

---

<br>

## Self-Check Questions

1. **Handling failure:** What is the output of Q1 if `MAX` is changed to 3?

   <details>
   <summary>Answer</summary>

   The pushes of 0, 1, and 2 succeed and the fourth attempt fails, so `0 1 2 ` and `4번째 시도 stack full` are printed. After the separator, the pops take out 2, 1, and 0 and the fourth attempt fails, so `2 1 0 ` and `4번째 시도 stack empty` are printed.

   </details>

2. **A parameter taken by value:** If `pop()` of Q1 is changed to `bool pop(int n)`, what is the output of the second loop?

   <details>
   <summary>Answer</summary>

   `n` inside `pop()` is a copy, so `n` in `main()` does not change. `n` in `main()` is an uninitialized local variable, so the two successful pops print an indeterminate value, which is undefined behavior. The value taken out cannot be returned to the caller, so a reference or pointer parameter is needed.

   </details>

3. **Reseating a reference:** If `int other = 7; ref = other;` runs after `ref = 100;` in Q2, does `ref` come to refer to `other`?

   <details>
   <summary>Answer</summary>

   It does not. A reference cannot change the object fixed at its initialization, so `ref = other;` is a statement that assigns 7 to `number`. After it runs, `number` is 7 and `other` is also 7.

   </details>

4. **A `const` parameter:** What happens if `value *= 2;` is put inside `printValue()` of Q2?

   <details>
   <summary>Answer</summary>

   `value` is a `const int&`, so the original cannot be changed through it, and a compile error occurs.

   </details>

5. **Selecting a getter:** Does Q3 compile if the parameter of `celebrateBirthday()` is changed to `const Person& person`?

   <details>
   <summary>Answer</summary>

   It does not. `person` is `const`, so `person.getAge()` selects the `const` version, which returns `const int&`, and trying to change the `const` value with `++` is an error.

   </details>
