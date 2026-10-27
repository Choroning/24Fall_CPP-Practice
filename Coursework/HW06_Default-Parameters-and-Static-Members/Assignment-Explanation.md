# HW6 Assignment Explanation — Default Parameters and Static Members

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 6

> **Learning Objectives:**
> 1. Replace several overloaded constructors with one constructor that has default parameters, and explain the rule that default values are written only in the declaration
> 2. Define a static member variable outside the class and manage the current number of objects by changing it in the constructors and the destructor
> 3. Write a class made of static member functions that are called without an object as `ClassName::`
> 4. Trace how many times and in what order constructors and destructors are called when a dynamic array of objects is created and released with `delete []`

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. A Person Constructor with Default Parameters](#2-q1-a-person-constructor-with-default-parameters)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. A Person Class That Counts Its Members with a Static Member](#3-q2-a-person-class-that-counts-its-members-with-a-static-member)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [4. Q3. A Random Class Made of Static Members](#4-q3-a-random-class-made-of-static-members)
  - [4.1 Requirements](#41-requirements)
  - [4.2 Design and Solution](#42-design-and-solution)
  - [4.3 Output and Review](#43-output-and-review)
- [5. Q4. A Circle Class That Counts Its Objects with a Static Member](#5-q4-a-circle-class-that-counts-its-objects-with-a-static-member)
  - [5.1 Requirements](#51-requirements)
  - [5.2 Design and Solution](#52-design-and-solution)
  - [5.3 Output and Review](#53-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the default parameters and static members of Chapter 6. Q1-Q4 are [Lab 2-2], [Lab 2-3], [Lab 3], and [Lab 4] of the Week 11 lecture slides. The assignment handout is in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW6 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW06_Default-Parameters-and-Static-Members/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # a Person constructor with default parameters
    ├── Q2.cpp               # a Person class that counts its members with a static member
    ├── Q3.cpp               # a Random class made of static members
    └── Q4.cpp               # a Circle class that counts its objects with a static member
```

*Figure 1. Directory layout of HW6*

Each solution is compiled and run as follows. All four files compile without warnings with `g++ -std=c++11 -Wall` and `g++ -std=c++17 -Wall` under Apple clang 21 on macOS.

```bash
cd Coursework/HW06_Default-Parameters-and-Static-Members/Solution
g++ -std=c++17 -Wall -o q2 Q2.cpp
./q2
```

| File | Compilation |
|:-----|:-----|
| `Q1.cpp` | No warnings |
| `Q2.cpp` | No warnings |
| `Q3.cpp` | No warnings |
| `Q4.cpp` | No warnings |

*Table 1. Compilation results of the HW6 solutions*

When a program runs in Xcode, the last console line, `Program ended with exit code: 0`, is not program output but a message Xcode shows after the program ends. None of the four `main()` functions has a `return` statement, but in C++ reaching the end of `main()` is treated as returning 0, so the exit code is 0.

---

<br>

## 2. Q1. A Person Constructor with Default Parameters

### 2.1 Requirements

Replace the three constructors of [Lab 2-1], `Person()`, `Person(int id, string name)`, and `Person(int id, string name, double weight)`, with one constructor that has default parameters and handles all three object creations of the given `main()`. The values of the three objects fixed by the sample run are as follows.

| Creation in `main()` | Arguments passed | `id` | `weight` | `name` |
|:-----|:-----|-----:|-----:|:-----|
| `grace` | None | 1 | 20.5 | `"Grace"` |
| `ashley(2, "Ashley")` | `id`, `name` | 2 | 20.5 | `"Ashley"` |
| `helen(3, "Helen", 32.5)` | `id`, `name`, `weight` | 3 | 32.5 | `"Helen"` |

*Table 2. The three objects of Q1 and the values of their members*

### 2.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Person` class represents a member and keeps `id`, `weight`, and `name` as `private` members. The constructor takes the parameters `id`, `name`, and `weight` in this order, with the default values 1, `"Grace"`, and 20.5. It assigns the received values to the member variables through the `this` pointer. `show()` prints `id`, `weight`, and `name` in order, separated by spaces.

`main()` creates three `Person` objects with different numbers of arguments. `grace` is created with no arguments and uses all three default values, `ashley` passes only `id` and `name` and uses the default value for `weight`, and `helen` passes all three values. The arguments passed fill the parameters from the left, and the remaining parameters get their default values, so the three calls are the same as `Person(1, "Grace", 20.5)`, `Person(2, "Ashley", 20.5)`, and `Person(3, "Helen", 32.5)` ([Chapter 6, Section 2.1](../../Lecture-Notes/Ch06_Overloading-and-Static-Members/Concepts_Lecture.md#21-syntax)). It then prints the information of each object with `show()`.

The order of the parameters is not the declaration order of the member variables (`id`, `weight`, `name`) but `id`, `name`, `weight`, which fits the calls in `main()`. Arguments fill the parameters from the left, so to omit only the weight, as in `ashley(2, "Ashley")`, `weight` must be the rightmost parameter.

The following two points are properties of this design that follow from the rules of default parameters.

- **Default values are written only in the declaration:** The default values are written in the declaration at line 16 and not in the definition outside the class at line 20. Writing them again in the definition, as in `int id = 1`, makes Apple clang report "redefinition of default argument" ([Chapter 6, Section 2.2](../../Lecture-Notes/Ch06_Overloading-and-Static-Members/Concepts_Lecture.md#22-declaration-vs-definition)).
- **It cannot coexist with the original overloaded constructors:** If the constructor with default parameters is kept together with `Person()` of [Lab 2-1], `Person grace;` matches both constructors and the error "call to constructor of 'Person' is ambiguous" occurs. So when the three constructors are merged into one, all of the original constructors are removed ([Chapter 6, Section 2.4](../../Lecture-Notes/Ch06_Overloading-and-Static-Members/Concepts_Lecture.md#24-simplifying-overloads-with-default-parameters)).

> **Caution:** The code uses `string` without including `<string>`. With libc++ on macOS, `<iostream>` includes `<string>` internally, so the code compiles, but the standard does not guarantee this, so it may fail to compile with another implementation of the standard library. A file that uses `string` should write `#include <string>` itself. The same applies to Q2.

### 2.3 Output and Review

```text
1 20.5 Grace
2 20.5 Ashley
3 32.5 Helen
```

The output is the same as the sample run of the problem. Running with `leaks --atExit` and `-fsanitize=address,undefined` reports no errors or leaks. The weight 20.5 of `ashley` is the default value placed in the slot of the third argument, which was not passed.

---

<br>

## 3. Q2. A Person Class That Counts Its Members with a Static Member

### 3.1 Requirements

Add to the `Person` class a `private` static member variable `numPerson` that stores the current number of members, increase it in the constructors, and decrease it in the destructor. The modified `main()` creates two objects and a dynamic array of two elements, which it then releases, printing the number of members along the way. The members to add or change are as follows.

| Member | What it does |
|:-----|:-----|
| `static int numPerson` | Stores the number of `Person` objects that currently exist |
| Constructors | Initialize the object and increase `numPerson` by 1 |
| `~Person()` | Decreases `numPerson` by 1 |
| `show()` | Prints `numPerson` after `id`, `weight`, and `name` |
| `static void nPerson()` | Prints only `numPerson` |

*Table 3. Members added or changed in Q2*

### 3.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The constructors are overloaded in three forms, as in [Lab 2-1]. The constructor without parameters initializes the object with 1, 20.5, and `"Grace"`, the second constructor takes `id` and `name` and uses 20.5 for `weight`, and the third takes all three values. All three constructors increase `numPerson` by 1 whenever an object is created, and the destructor decreases it by 1 when an object is destroyed.

`show()` prints the three member variables of the object and the current number of objects, and the static member function `nPerson()` prints only the current number of objects. `static int numPerson;` inside the class is only a declaration; `int Person::numPerson = 0;` at line 49 defines the variable outside the class and initializes it to 0. Without this line the code compiles, but the linker reports that it cannot find `Person::numPerson` ([Chapter 6, Section 3.1](../../Lecture-Notes/Ch06_Overloading-and-Static-Members/Concepts_Lecture.md#31-static-member-variables)).

`main()` creates `grace` with the default constructor and `helen` with the third constructor. It then creates a dynamic array of two `Person` objects with `new Person[2] { {43, "Tera"}, {98, "Hera"} }`. `{43, "Tera"}` and `{98, "Hera"}` inside the braces become the constructor arguments of the elements, so the second constructor is called twice. This syntax for initializing the elements of a dynamic array was introduced in C++11, and compiling with `-std=c++98` gives an error at this line ([Chapter 4, Section 3.3](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#33-dynamic-object-arrays)). It prints the information of each element with `show()`, checks the current number of objects with `nPerson()`, releases the array, and checks the number of objects again. When the program ends, the destructors of the remaining objects are called.

The following three points are needed to understand the output.

- **One shared variable:** `numPerson` does not exist separately for each object; there is only one for the class, so printing it through the `show()` of any object gives the same value.
- **Why Tera prints 4:** `new Person[2] {...}` returns the address only after both elements have been constructed, so when `p[0].show()` runs there are already four people.
- **Calling through the class name:** `nPerson()` is a static member function, so it has no `this` and is called without an object as `Person::nPerson()`. A static member function can access only static members, and `nPerson()` accesses only the static member variable `numPerson`, so there is no problem ([Chapter 6, Section 3.3](../../Lecture-Notes/Ch06_Overloading-and-Static-Members/Concepts_Lecture.md#33-static-member-functions)).

Two points can be considered to make the code more robust.

- **Copy constructor:** The default copy constructor generated by the compiler does not increase `numPerson`, but when a copied object is destroyed, the destructor decreases `numPerson`. Indeed, putting `{ Person copy = grace; }` into `main()` and running it makes the `nPerson()` after the block print 3 instead of 4, and the last line becomes 1 instead of 2. A class that counts its objects also needs a copy constructor that increases the count, such as `Person(const Person& p) : id(p.id), weight(p.weight), name(p.name) { numPerson++; }`.
- **Merging into one constructor:** With three constructors, `numPerson++` must be written in three places. Replacing them with the one constructor with default parameters from Q1 makes the same `main()` print the same result, and the code that increases the count needs to be in only one place.

### 3.3 Output and Review

```text
1 20.5 Grace 1
13 32.5 Helen 2
43 20.5 Tera 4
98 20.5 Hera 4
4
2
```

The output is the same as the sample run of the problem. `delete [] p;` calls the destructors of the elements in the reverse order of construction, `p[1]` and then `p[0]`, decreasing `numPerson` from 4 to 2, so the last line is 2. When `main()` then ends, the destructors of `helen` and `grace` are called in turn and `numPerson` becomes 0, but this value is not printed. Running with `leaks --atExit` and `-fsanitize=address,undefined` reports no errors or leaks.

---

<br>

## 4. Q3. A Random Class Made of Static Members

### 4.1 Requirements

Complete a `Random` class that has only static member functions, and complete a `main()` that prints 10 random integers from 1 to 100, 10 random letters, and 10 random real numbers. The real numbers are printed five per line on two lines.

| Member function | What it does |
|:-----|:-----|
| `seed()` | Sets the seed of the random numbers with the current time (the body is given) |
| `nextInt(min, max)` | Returns a random integer from `min` to `max`. The default values are 0 and 32767 |
| `nextAlphabet()` | Returns one random letter, uppercase or lowercase |
| `nextDouble()` | Returns a random real number from 0 up to but not including 1 |

*Table 4. Static member functions of the Random class in Q3*

### 4.2 Design and Solution

The program includes `<iostream>`, `<cstdlib>`, and `<ctime>` for input/output, random number generation, and the time function. The given `seed()` is defined inside the class, and the other three functions are only declared inside the class and defined outside it. The definitions outside the class do not repeat the `static` keyword or the default values.

- **`nextInt(min, max)`:** `rand() % (max - min + 1)` is a value from 0 to `max - min`, so adding `min` gives a value from `min` to `max`. The default values 0 and 32767 are only in the declaration.
- **`nextAlphabet()`:** If `rand() % 2` is 1, it makes a lowercase letter with `'a' + rand() % 26`; if it is 0, an uppercase letter with `'A' + rand() % 26`. The two expressions have type `int`, but they are converted to the return type `char`, so `cout` prints a character rather than a number.
- **`nextDouble()`:** converts `rand()` to `(double)` and divides it by `RAND_MAX` to make a real number between 0 and 1. The conversion happens before the division, so it is floating-point division, not integer division.

`main()` first calls `Random::seed()` once to initialize the random numbers. In three parts it then makes and prints 10 integers from 1 to 100 with `Random::nextInt(1, 100)`, 10 letters with `Random::nextAlphabet()`, and 10 real numbers with `Random::nextDouble()`, printing the real numbers five per line on two lines. All member functions are static, so it calls them through the class name without creating a `Random` object.

Because setting the seed is split into the static member function `seed()`, it needs to be called only once at the start of `main()`. The `Random` class of HW1 Q2, which set the seed in its constructor, runs `srand()` again every time an object is created, whereas this class creates no objects and sets the seed only once. Grouping functions that could be global functions as static members of a class gathers the related functions under `Random::` and reduces the number of global names ([Chapter 6, Section 3.4](../../Lecture-Notes/Ch06_Overloading-and-Static-Members/Concepts_Lecture.md#34-use-cases)).

Two points can be considered to make the code more robust.

- **Range of `nextDouble()`:** `rand()` returns a value from 0 to `RAND_MAX`, so when `rand()` returns `RAND_MAX`, `(double) rand() / RAND_MAX` is exactly 1.0. The comment says the function returns a real number less than 1, but the actual range is from 0 to 1 inclusive. On macOS `RAND_MAX` is 2147483647, so the probability of getting 1.0 in one call is $2^{-31}$, but in Visual C++, where `RAND_MAX` is 32767, it is 1/32768. Computing `rand() / (RAND_MAX + 1.0)` makes the result always less than 1. Writing `RAND_MAX + 1` would overflow the integer on macOS, where `RAND_MAX` is the largest `int`, so `1.0` is added to compute in floating point.
- **Preventing object creation:** All members are static, so there is no reason to create a `Random` object, yet the current class allows creating one, as in `Random r;`. Declaring the constructor `private` prevents object creation, so the class can be used only through its name.

### 4.3 Output and Review

One example of the output on macOS is as follows.

```text
1에서 100까지 랜덤한 정수 10개를 출력합니다
87 54 47 27 17 69 51 41 28 6 
랜덤한 알파벳 10개를 출력합니다
C r j F j s Y s T F 
랜덤한 실수 10개를 출력합니다
0.907427 0.120311 0.0723063 0.252541 0.458961 
0.753511 0.262834 0.444927 0.881017 0.253124 
```

The values change with every run. Each line ends with one space because `' '` is appended after every value. By default `cout` prints a `double` with six significant digits and omits trailing zeros, so values with fewer digits, such as `0.73853`, and values with seven digits after the decimal point, such as `0.0723063`, also appear. Running with `leaks --atExit` and `-fsanitize=address,undefined` reports no errors or leaks.

---

<br>

## 5. Q4. A Circle Class That Counts Its Objects with a Static Member

### 5.1 Requirements

Declare in the `Circle` class a `private` static variable `count` that stores the number of `Circle` objects that currently exist, and initialize it outside the class. The two constructors increase `count` and the destructor decreases it, and `show()` prints the current number of objects together with the radius. The given `main()` creates one object and a dynamic array of two elements and calls `show()` before and after releasing the array.

### 5.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Circle` class represents a circle and keeps as `private` members the radius `radius` and the static variable `count`, which tracks the number of `Circle` objects that currently exist. Under `public` are the two constructors, the destructor, and `show()`.

- **Two constructors:** The constructor without parameters initializes `radius` to 0, and the other initializes it with the received value. Both constructors increase `count` by 1 whenever an object is created.
- **Destructor:** only declared inside the class and defined outside it at lines 28-30. It decreases `count` by 1 when an object is destroyed.
- **`show()`:** prints the radius and the current number of objects in the form `Radius: 5, Count: 1`.
- **Definition of `count`:** `int Circle::count = 0;` at line 31 defines the static variable and initializes it to 0.

`main()` creates `a` with the constructor that takes a parameter and prints it with `show()`. It then creates a dynamic array of two `Circle` objects with `new Circle[2]`. There is no initializer list, so the default constructor is called for each element. It prints each element in a `for` loop and prints `a` again, releases the array with `delete[] p;`, and sets `p` to `nullptr`. Finally it prints `a` once more, and when the program ends the destructor of `a` is called.

`delete[] p;` calls the destructor for each element of the array and then returns the memory, so `count` drops from 3 to 1. Writing `delete p;` without `[]` releases an array with `delete`, which is undefined behavior. The problem recommends adding `p = NULL;` after the release, and the code uses `nullptr` from C++11. Assigning `nullptr` to the released pointer keeps it from remaining as a dangling pointer to freed memory. `NULL` is a macro that may be defined as the integer 0, while `nullptr` is a value only for pointers, so there is no risk of it being interpreted as an integer in function overloading ([Chapter 4, Section 4.2](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#42-dangling-pointer)).

As in Q2, the default copy constructor does not increase `count`, so passing or copying a `Circle` by value makes the count smaller than the actual number. Adding a copy constructor that increases the count removes this problem.

### 5.3 Output and Review

```text
Radius: 5, Count: 1
Radius: 0, Count: 3
Radius: 0, Count: 3
Radius: 5, Count: 3
Radius: 5, Count: 1
```

The output is the same as the sample run of the problem. The count on the second and third lines is 3 because `a` and both elements of the array exist, and the 1 on the last line is the value after `delete[] p;` destroyed the two elements. Running with `leaks --atExit` and `-fsanitize=address,undefined` reports no errors or leaks.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Default parameters | One `Person(int id = 1, string name = "Grace", double weight = 20.5)` handles creation with zero, two, or three arguments. |
| Where default values go | Default values are set from the right end and written only in the declaration; writing them again in the definition is a compile error. |
| Static member variable | There is one for the class, shared by all objects, and it must be defined outside the class, as in `int Person::numPerson = 0;`. |
| Counting objects | Increasing in the constructors and decreasing in the destructor gives the number of existing objects, and the copy constructor must also increase the count to be exact. |
| Static member function | It has no `this`, accesses only static members, and is called without an object, as in `Person::nPerson()` and `Random::nextInt(1, 100)`. |
| Dynamic array of objects | `new Person[2] {...}` calls a constructor for each element, and `delete [] p;` calls the destructor for each element in reverse order. |
| Range of `nextDouble()` | `rand() / RAND_MAX` includes 1.0, so dividing by `RAND_MAX + 1.0` is needed when a value below 1 is required. |

---

<br>

## Self-Check Questions

1. **An argument that cannot be omitted:** What happens if `Person kim(4, 50.0);` is added to the `main()` of Q1?

   <details>
   <summary>Answer</summary>

   A compile error occurs. Arguments fill the parameters from the left, so 50.0 is passed to the second parameter, `string name`, and since a `double` cannot be converted to a `string`, no constructor matches. To set the weight, the name must also be passed, as in `Person kim(4, "Kim", 50.0);`.

   </details>

2. **When the array elements are created:** Why does `p[0].show()` in Q2 print `43 20.5 Tera 4` instead of `43 20.5 Tera 3`?

   <details>
   <summary>Answer</summary>

   `new Person[2] {...}` returns the address of the array only after both elements have been constructed. So when `p[0].show()` runs, the four objects `grace`, `helen`, `p[0]`, and `p[1]` already exist, and the shared `numPerson` is 4.

   </details>

3. **Copies and the count:** If `{ Person copy = grace; }` is put right before `Person::nPerson();` at line 61 of Q2, what do lines 61 and 64 print?

   <details>
   <summary>Answer</summary>

   They print 3 and 1 instead of 4 and 2. The default copy constructor does not increase `numPerson`, so it stays 4 when `copy` is created, and it becomes 3 when the block ends and the destructor of `copy` runs. Line 61 therefore prints 3, and line 64, after the array is released, prints 1.

   </details>

4. **Range of the letters:** How many different characters can `nextAlphabet()` of Q3 return? Do `rand() % 2` and `rand() % 26` use the result of the same call to `rand()`?

   <details>
   <summary>Answer</summary>

   There are 52: 26 lowercase and 26 uppercase letters. The two expressions call `rand()` separately, so they use different random numbers. The first call decides between uppercase and lowercase, and the second picks one of the 26 letters.

   </details>

5. **An array that is not released:** If `delete[] p;` is removed from Q4 and `main()` is run, what does the last line print? What happens to the memory of the two elements?

   <details>
   <summary>Answer</summary>

   The elements of the array are not destroyed, so `count` stays at 3 and the last line is `Radius: 5, Count: 3`. The destructors of the two elements are not called and the memory is not returned; then `p = nullptr;` loses the address of the array, so the memory becomes a leak that the program has no way to release.

   </details>
