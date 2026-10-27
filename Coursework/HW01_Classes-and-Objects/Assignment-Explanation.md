# HW1 Assignment Explanation — Classes and Objects

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 3

> **Learning Objectives:**
> 1. Declare the member variables and member functions of a class and define member functions outside the class with `ClassName::`
> 2. Initialize objects with a parameterized constructor and explain how a constructor that is declared but never defined is handled at compile time and at link time
> 3. Produce random integers in a given range with `rand()`, `srand()`, and `time()`, and explain why `RAND_MAX` can differ between platforms
> 4. Handle `private` member variables through get/set functions and compare defining a function at its declaration with separating the declaration from the definition

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. The Bank Account Class Account](#2-q1-the-bank-account-class-account)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. The Random Class That Generates Random Integers](#3-q2-the-random-class-that-generates-random-integers)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [4. Q3. The Point Class with Access Control and Get/Set Functions](#4-q3-the-point-class-with-access-control-and-getset-functions)
  - [4.1 Requirements](#41-requirements)
  - [4.2 Design and Solution](#42-design-and-solution)
  - [4.3 Output and Review](#43-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the class definitions, constructors, and access specifiers of Chapter 3. Q1 and Q2 are Programming Exercises 3 and 5 of Chapter 3 of the textbook, and Q3 is [Problem 1-3] of the Week 4 lecture slides. The assignment handout is in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW1 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW01_Classes-and-Objects/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # the bank account class Account
    ├── Q2.cpp               # the Random class that generates random integers
    └── Q3.cpp               # the Point class with private coordinates and get/set functions
```

*Figure 1. Directory layout of HW1*

Each solution is compiled and run as follows. All three files compile with `g++ -std=c++11 -Wall` and `g++ -std=c++17 -Wall` under Apple clang 21 on macOS.

```bash
cd Coursework/HW01_Classes-and-Objects/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

| File | Compilation |
|:-----|:-----|
| `Q1.cpp` | One warning: `-Wunused-variable`, because `money` in `main()` is never used |
| `Q2.cpp` | No warnings |
| `Q3.cpp` | No warnings |

*Table 1. Compilation results of the HW1 solutions*

When a program runs in Xcode, the last console line, `Program ended with exit code: 0`, is not program output but a message Xcode shows after the program ends. None of the three `main()` functions has a `return` statement, but in C++ reaching the end of `main()` is treated as returning 0, so the exit code is 0.

---

<br>

## 2. Q1. The Bank Account Class Account

### 2.1 Requirements

Write an `Account` class that represents one account with three member variables: the owner, the account number, and the balance. The given `main()` uses the following members.

| Use in `main()` | Required member | What it does |
|:-----|:-----|:-----|
| `Account a("kitae", 1, 5000);` | Constructor with three parameters | Initializes the object with the owner, the account number, and the initial balance |
| `a.deposit(50000);` | `void deposit(int)` | Adds the deposit to the balance |
| `a.getOwner()` | `string getOwner()` | Returns the name of the owner |
| `a.inquiry()` | `int inquiry()` | Returns the current balance |
| `int money = a.withdraw(20000);` | `int withdraw(int)` | Subtracts the withdrawal from the balance and returns the amount withdrawn |

*Table 2. Members required by the `main()` of Q1*

### 2.2 Design and Solution

The program includes `<iostream>` and `<string>` for input/output and string handling and uses the standard namespace by default with `using namespace std;`. The `Account` class represents a bank account and holds, under the `public` access specifier, the owner's name `name`, the account number `id`, and the balance `balance`. The class declaration contains only the prototypes of the default constructor, the three-parameter constructor, `getOwner()`, `deposit()`, `withdraw()`, and `inquiry()`; their bodies are defined outside the class with `Account::`, which separates declaration from definition ([Chapter 3, Section 2.3](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.md#23-member-functions)).

- **Three-parameter constructor:** takes the name, the account number, and the initial balance and assigns them to the member variables.
- **`getOwner()`:** returns the name of the owner.
- **`deposit(m)`:** adds the deposit `m` to the balance.
- **`withdraw(m)`:** subtracts the withdrawal `m` from the balance and returns the amount withdrawn, `m`.
- **`inquiry()`:** returns the current balance.

`main()` creates the object `a` with the name `"kitae"`, account number 1, and an initial balance of 5000, deposits 50000, and prints the owner's name and the balance. It then withdraws 20000, stores the amount in `money`, and prints the updated balance again. The balance changes from 5000 to 55000 and then to 35000.

Three points can be considered to make the code more robust.

- **A default constructor that is only declared:** `Account();` has a declaration but no definition. This `main()` never calls the default constructor, so both compiling and linking succeed. However, as soon as some code uses the default constructor, as in `Account b;`, the program still compiles but the linker reports that it cannot find the definition of `Account::Account()`. A constructor that will not be used should not be declared, and one that will be used must also be defined ([Chapter 3, Section 4.5](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.md#45-default-constructor)).
- **`public` member variables:** The problem does not specify access control, so `public` members do not violate the requirements. But `main()` uses only member functions, so moving the three member variables to `private` would not require any change in `main()`. To keep outside code from changing the balance directly, as in `a.balance = -100;`, making them `private` follows the principle of encapsulation ([Chapter 3, Section 2.4](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.md#24-access-specifiers)).
- **Withdrawals larger than the balance:** `withdraw()` does not check the balance, so a request larger than the balance makes the balance negative. The comment in the textbook says that `withdraw()` returns the amount actually withdrawn, so it could be changed to withdraw nothing and return 0 when the balance is insufficient, or to withdraw only the balance and return that amount.

### 2.3 Output and Review

```text
kitae의 잔액은 55000
kitae의 잔액은 35000
```

The output, which reads "kitae's balance is 55000" and "kitae's balance is 35000" in Korean, matches the sample run of the problem. The `-Wall` warning arises because `main()` stores the return value of `withdraw()` in `money` without using it; this is the code of the `main()` given in the textbook.

---

<br>

## 3. Q2. The Random Class That Generates Random Integers

### 3.1 Requirements

Write a `Random` class with three members: a constructor and the member functions `next()` and `nextInRange()`. `next()` returns a random integer from 0 to `RAND_MAX`, and `nextInRange(s, f)` returns a random integer from `s` to `f`. The given `main()` prints 10 random integers with each function.

### 3.2 Design and Solution

The program includes `<iostream>`, `<cstdlib>`, and `<ctime>` for random number generation and input/output. `<ctime>` is used so that the current time serves as the seed and each run produces different random numbers. The `Random` class represents a random number generator; the prototypes of the constructor and the two member functions are under `public`, and their bodies are defined outside the class.

- **Constructor:** sets the current time as the seed with `srand((unsigned)time(0));`. On most systems `time(0)` is the number of seconds since 1970-01-01 00:00:00 UTC, so every run gets a different seed.
- **`next()`:** calls `rand()` and returns a random number from 0 to `RAND_MAX`.
- **`nextInRange(s, f)`:** `rand() % (f - s + 1)` is a value from 0 to `f - s`, so adding `s` gives a value from `s` to `f`. For `nextInRange(2, 4)` this is `rand() % 3 + 2`, which yields one of 2, 3, and 4.

`main()` creates a `Random` object `r`, prints the first line of guidance text, and repeats 10 times in a `for` loop, printing a random number made by `next()`. It then prints the second line of guidance text and repeats 10 times again, printing a random number from 2 to 4 made by `nextInRange(2, 4)`.

The following two points are properties of this design.

- **There is one seed for the whole program:** `srand()` sets the random sequence of the whole program, not of each object. If two `Random` objects are created within the same second, the second constructor resets the sequence to its start with the same seed, so the two objects can produce the same numbers in turn. To set the seed only once, the seed setup can be split into a static member function called once at the start of `main()`, as in HW6 Q3.
- **Bias of the remainder operation:** If `RAND_MAX + 1` is not a multiple of the interval length `f - s + 1`, the smaller values made by the remainder operation appear very slightly more often. With `RAND_MAX` equal to 32767 and an interval length of 3, 32768 divided by 3 leaves a remainder of 2, so 2 and 3 each have one more possible case than 4. The difference is negligible for this assignment; when an exactly uniform distribution is needed, the C++11 `<random>` library (`std::mt19937` and `std::uniform_int_distribution`) is used.

### 3.3 Output and Review

One example of the output on macOS is as follows.

```text
-- 0에서 2147483647까지의 랜덤 정수 10개 --
324124383 1539976289 908575579 1821026083 48439937 233718946 366735059 438069723 1063892545 893158893 

-- 2에서 4까지의 랜덤 정수 10개 --
2 3 3 2 2 2 3 2 2 2 
```

With Visual C++ on Windows, the first line is `-- 0에서 32767까지의 랜덤 정수 10개 --` and the numbers have at most five digits. The C standard only requires `RAND_MAX` to be at least 32767; the Microsoft Visual C++ runtime defines it as 32767, while the C libraries of macOS and Linux define it as 2147483647 ($2^{31} - 1$). The same code therefore prints a different first line and a different range of numbers depending on the platform. The numbers on the second line are one of 2, 3, and 4 on every platform. Each line ends with one space because `cout << n << ' ';` appends a space after every number.

---

<br>

## 4. Q3. The Point Class with Access Control and Get/Set Functions

### 4.1 Requirements

Write a `Point` class with the `private` coordinates `x` and `y`, set the coordinates of `p2` from values entered by the user, and print the coordinates of the two points. The place where each member function is defined is fixed.

| Member | Where it is defined |
|:-----|:-----|
| Default constructor (`x` and `y` set to 0) | Declared and defined together inside the class |
| Two-argument constructor | Defined outside the class |
| `showP()` | Defined outside the class |
| `getX()`, `setX()` | Declared and defined together inside the class |
| `getY()`, `setY()` | Defined outside the class |

*Table 3. Members of Q3 and where they are defined*

### 4.2 Design and Solution

The program includes `<iostream>` for input/output. The `Point` class represents a point on a two-dimensional plane; the coordinates `x` and `y` are under `private`, and the two constructors and `showP()`, `getX()`, `setX()`, `getY()`, and `setY()` are under `public`.

- **Default constructor:** initializes both coordinates to 0 with `x = y = 0;`. The assignment operator associates from right to left, so `y = 0` runs first and its value 0 is assigned to `x`.
- **Two-argument constructor:** assigns the two coordinates it receives to `x` and `y`. It is not used in this `main()`, but it is defined outside the class as required.
- **`showP()`:** gets the coordinates through `getX()` and `getY()` and prints them in the form `x: 0, y: 0`. A member function can access `x` and `y` directly, but going through the get functions keeps the way of reading the coordinates in one place.
- **Get/set functions:** `getX()` and `setX()` are defined inside the class and `getY()` and `setY()` outside, showing both styles. A member function whose body is defined inside the class is treated as an inline function automatically, while a member function defined outside the class is compiled as an ordinary function unless it is marked `inline` ([Chapter 3, Section 5](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.md#5-inline-functions)). The two styles behave the same; they differ in how easily a reader can grasp the interface from the class declaration alone.

`main()` creates `p1` and `p2` with the default constructor, so both points start at (0, 0). It keeps a temporary variable `temp` for input, reads the x coordinate of `p2` and sets it with `setX()`, then reads the y coordinate and sets it with `setY()`. Finally it prints the coordinates of `p1` and `p2` with `showP()`. Because `x` and `y` are `private`, writing `p2.x = temp;` in `main()` is a compile error, which is why the set functions are needed ([Chapter 3, Section 9.2](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.md#92-accessing-private-members-from-outside-the-class)).

### 4.3 Output and Review

With the inputs `-3` and `4`, the program prints the following, which matches the sample run of the problem.

```text
Input p2.x: -3
Input p2.y: 4
p1. x: 0, y: 0
p2. x: -3, y: 4
```

When the input comes from a file or a pipe, the entered values are not echoed on the screen, so the prompts run together on one line, as in `Input p2.x: Input p2.y: p1. x: 0, y: 0`. The output itself is the same. If a non-integer value (for example `a`) is entered, `cin` enters a failed state, 0 is stored in `temp` (since C++11), and the next input is not read either.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Separating declaration and definition | The class declaration holds only the prototypes of the members, and the bodies are defined outside the class as `ClassName::functionName`. |
| Parameterized constructor | It takes arguments when an object is created, as in `Account a("kitae", 1, 5000);`, and initializes the member variables. |
| Undefined constructor | A constructor that is only declared causes no problem as long as it is not called, but calling it causes a link error. |
| Range of random integers | `rand() % (f - s + 1) + s` produces a value from `s` to `f`, and the seed is set with `srand((unsigned)time(0))`. |
| `RAND_MAX` | The standard only requires at least 32767, so it differs between platforms: 32767 in MSVC and 2147483647 on macOS and Linux. |
| Access control and get/set | `private` members cannot be accessed directly outside the class, so they are read and changed through `public` get/set functions. |
| Definitions inside the class | A member function whose body is defined inside the class is treated as an inline function automatically. |

---

<br>

## Self-Check Questions

1. **Link error:** If `Account b;` is added to the `main()` of Q1, at which stage, compiling or linking, does an error occur, and what is it?

   <details>
   <summary>Answer</summary>

   Compiling succeeds because the declaration `Account();` exists. But the default constructor is defined nowhere, so at link time the linker reports that it cannot find the symbol `Account::Account()`. Either the default constructor must be defined or `Account b;` must not be used.

   </details>

2. **Tracing the balance:** If `a.withdraw(60000);` is called once more in Q1, what are the balance and the return value? How would you change the code so that the balance cannot become negative?

   <details>
   <summary>Answer</summary>

   The balance becomes 35000 - 60000 = -25000, and the return value is 60000. Change the function so that it withdraws nothing and returns 0 when the balance is insufficient, as in `if (m > balance) return 0;`, or so that it withdraws only the balance and returns that amount.

   </details>

3. **Computing a range:** Give the range of values that `nextInRange(-5, 5)` in Q2 can return and the number of possible values.

   <details>
   <summary>Answer</summary>

   `f - s + 1` is 11, so `rand() % 11` is from 0 to 10, and adding -5 gives a value from -5 to 5. There are 11 possible values.

   </details>

4. **The same seed:** If the `main()` of Q2 creates two objects in a row, as in `Random r1, r2;`, and calls `r1.next()` and `r2.next()` once each, are the two values the same?

   <details>
   <summary>Answer</summary>

   They are not. When both constructors run within the same second, the constructor of `r2` resets the sequence to its start with the same seed, but the following `r1.next()` and `r2.next()` take the first and second values in turn from a single sequence. The random sequence belongs to the whole program, not to an object. However, if `r1` is created and `next()` is called before `r2` is created within the same second, `r2.next()` produces the same value as the first value of `r1` again.

   </details>

5. **Access control:** What happens if the `main()` of Q3 contains `cout << p2.x;`, and how should it be written to do the same thing?

   <details>
   <summary>Answer</summary>

   `x` is a `private` member, so it cannot be accessed from `main()`, which is outside the class, and a compile error occurs. It must be read through the `public` get function, as in `cout << p2.getX();`.

   </details>
