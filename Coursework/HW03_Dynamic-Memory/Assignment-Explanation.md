# HW3 Assignment Explanation — Dynamic Memory

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **Learning Objectives:**
> 1. Create an object dynamically with `new`, return it with `delete`, and call its member functions through a pointer
> 2. Explain why a member variable must be referred to with `this->` when a parameter has the same name
> 3. Handle a dynamic array of a built-in type with `new int[n]` and `delete []` and explain the difference between integer division and floating-point division
> 4. Create an array of objects dynamically with a size read at run time, and confirm with a tool that memory not returned remains as a leak

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. Dynamically Created Complex Objects](#2-q1-dynamically-created-complex-objects)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. The Member Class That Manages Member Information](#3-q2-the-member-class-that-manages-member-information)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [4. Q3. An Average Computed with a Dynamic Integer Array](#4-q3-an-average-computed-with-a-dynamic-integer-array)
  - [4.1 Requirements](#41-requirements)
  - [4.2 Design and Solution](#42-design-and-solution)
  - [4.3 Output and Review](#43-output-and-review)
- [5. Q4. A Dynamic Array of Circle Objects](#5-q4-a-dynamic-array-of-circle-objects)
  - [5.1 Requirements](#51-requirements)
  - [5.2 Design and Solution](#52-design-and-solution)
  - [5.3 Output and Review](#53-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the dynamic objects, dynamic arrays, dynamic arrays of objects, and the `this` pointer of Chapter 4. All four questions are lab problems from the Week 6 lecture slides: Q1 is [Problem 6-1], Q2 is [Problem 6-2], Q3 is [Problem 6-4], and Q4 is [Problem 6-5]. The assignment handout is in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW3 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW03_Dynamic-Memory/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # dynamically created Complex objects
    ├── Q2.cpp               # the Member class that manages member information
    ├── Q3.cpp               # an average computed with a dynamic integer array
    └── Q4.cpp               # a dynamic array of Circle objects
```

*Figure 1. Directory layout of HW3*

Each solution is compiled and run with C++11 or later as follows. Q3 and Q4 read integers after they start.

```bash
cd Coursework/HW03_Dynamic-Memory/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

Leaks are checked on macOS by running a program as in `leaks --atExit -- ./q4`, which finds the heap memory left unreturned when the program ends. Invalid memory accesses are found by building with AddressSanitizer (`-fsanitize=address,undefined`). AddressSanitizer on macOS does not support leak detection, so leaks are checked with `leaks`. Q1 and Q3 return all the memory they allocate and Q2 uses no dynamic memory, but Q4 does not free its array and leaks memory (Section 5.2).

When a program runs in Xcode, the last console line, `Program ended with exit code: 0`, is not program output but a message Xcode shows after the program ends. None of the four `main()` functions has a `return` statement, but in C++ reaching the end of `main()` is treated as returning 0, so the exit code is 0.

---

<br>

## 2. Q1. Dynamically Created Complex Objects

### 2.1 Requirements

Define a complex number class `Complex` with a default constructor, a two-parameter constructor, set functions, and the output function `pCom()`, and create two objects dynamically, handled through the pointers `a` and `b`. The member variables and the parameters of the constructor and member functions must all use the same names, `real` and `image`. The code that corresponds to each comment of the `main()` skeleton is as follows.

| Comment in the `main()` skeleton | Corresponding code |
|:-----|:-----|
| Create the first dynamic object without initial values and manage it with pointer `a` | `Complex* a = new Complex;` |
| Create the second dynamic object without initial values and manage it with pointer `b` | `Complex* b = new Complex;` |
| Print the dynamic objects managed by pointers `a` and `b` | `a->pCom();`, `b->pCom();` |
| Update `real` of the first object to 4 | `(*a).setReal(4);` |
| Update `image` of the second object to 9 | `(*b).setImage(9);` |
| Print the two dynamic objects | `(*a).pCom();`, `(*b).pCom();` |
| Free the dynamic memory | `delete a;`, `delete b;` (each followed by an assignment of `nullptr`) |

*Table 1. The `main()` skeleton of Q1 and the corresponding code*

### 2.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Complex` class represents a complex number; the real part `real` and the imaginary part `image`, declared without an access specifier, are `private` members and cannot be accessed directly outside the class. Under `public`, the two constructors and `setReal()`, `setImage()`, and `pCom()` are declared and defined together inside the class.

- **Default constructor:** initializes both members to 1 with `real = image = 1;`.
- **Two-parameter constructor:** assigns the two received values to the members with `this->real = real;` and `this->image = image;`.
- **`setReal()`, `setImage()`:** change a member to the received value in the same way, with `this->`.
- **`pCom()`:** prints the complex number in the form `a + bi` with `real << " + " << image << "i\n"`.

The condition that the parameters have the same names as the members is what makes `this` necessary. Inside the member function the parameter `real` hides the member of the same name, so writing only `real` means the parameter. `this` is a pointer holding the address of the object on which the member function was called, so `this->real` refers to the member `real` of that object ([Chapter 4, Section 1.2](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#12-this-pointer)). Writing `real = real;` without `this->` assigns the parameter to itself, and the member does not change. Clang warns about this statement with `explicitly assigning value of variable of type 'int' to itself` (`-Wself-assign`), and calling `setReal(4)` then still prints `1 + 1i`.

`main()` creates a `Complex` object on the heap with `new Complex` and stores its address in the pointer `a`. `new Complex` written without parentheses and arguments calls the default constructor, so both objects start at (1, 1) ([Chapter 4, Section 2.2](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#22-dynamic-objects)). It first prints the initial state by calling `a->pCom()` and `b->pCom()` with the arrow operator, prints a separator line, and then changes the values by dereferencing the pointers with `(*a).setReal(4)` and `(*b).setImage(9)`. `(*a).setReal(4)` is the same call as `a->setReal(4)`. It prints the changed values again with `(*a).pCom()` and `(*b).pCom()`, returns the two objects with `delete a;` and `delete b;`, and assigns `nullptr` to each pointer.

The following two points are properties of this design.

- **A constructor that is not used:** The two-parameter constructor is not called in this `main()`. Creating a dynamic object with arguments, as in `new Complex(2, 3)`, calls this constructor and produces an object equal to `2 + 3i`.
- **`nullptr` after `delete`:** `delete` only returns the object the pointer points to and does not change the value of the pointer variable, so after the return `a` and `b` are dangling pointers. This program does not use the pointers afterwards, so the output is not affected, but with `nullptr` assigned, executing `delete a;` again by mistake does nothing, because `delete` on a null pointer has no effect, and no double free occurs ([Chapter 4, Section 4.2](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#42-dangling-pointer)).

### 2.3 Output and Review

```text
1 + 1i
1 + 1i
---------------
4 + 1i
1 + 9i
```

The first two lines are the initial state made by the default constructor, and the last two lines show the changed real part of the first object and the changed imaginary part of the second. Both objects are returned by `delete a;` and `delete b;`.

---

<br>

## 3. Q2. The Member Class That Manages Member Information

### 3.1 Requirements

Implement a `Member` class whose `private` members are the name `name` (`string`), the ID `id` (`string`), and the age `age` (`int`), create two member objects, print their information, and change it. The class has the following members.

| Member | What it does |
|:-----|:-----|
| `Member()` | Initializes the members to `"noname"`, `"none"`, and 0 |
| `Member(string name, string id, int age)` | Initializes `name`, `id`, and `age` with the three received values |
| `setName()`, `setId()`, `setAge()` | Change each member to the received value |
| `getName()`, `getId()`, `getAge()` | Return the value of each member |
| `printM()` | Prints the three members on one line, separated by commas |

*Table 2. Members of the Member class in Q2*

### 3.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Member` class represents the information of one member, and `name`, `id`, and `age`, declared before any access specifier, are `private` members. The ID is a number that is not used in calculations, like a student number, so it is stored as a `string` as the problem requires.

- **Default constructor:** assigns `"noname"` to `name`, `"none"` to `id`, and 0 to `age` in the body.
- **Three-parameter constructor:** the parameter names are the same as the members, so it assigns them with `this->`, as in `this->name = name;`.
- **Set functions:** one for each of the three members, again referring to the member with `this->`.
- **Get functions:** return the values of the three members. They are not called in this `main()`, but the problem requires them.
- **`printM()`:** prints in the form `name, id, age` and ends the line.

`main()` creates `Kim` with the default constructor and `Lee` with the three-parameter constructor and the values `{"Lee Hi", "2016001", 22}`, and calls `printM()` on both objects to print their initial state. It then changes the information of `Kim` with `Kim.setName("Kim Haha")`, `Kim.setId("2016020")`, and `Kim.setAge(29)` and prints the changed information again with `printM()`.

Three points can be considered to make the code more robust.

- **Including `<string>`:** The code uses `string` without including `<string>`. It compiles under Apple clang (libc++) because the `<iostream>` of that library includes the contents of `<string>` indirectly, and the C++ standard does not guarantee this. For the code to compile with other standard library implementations as well, it should include the header of every name it uses directly.
- **Member initializer list:** The default constructor first creates the members and then assigns values to them in the body. With a member initializer list, as in `Member() : name("noname"), id("none"), age(0) {}`, the members are initialized at the moment they are created ([Chapter 3, Section 4.4](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.md#44-ways-to-initialize-member-variables)).
- **Copying string parameters:** The constructor and the set functions take `string` by value, so a `string` object given as an argument is copied once into the parameter. Taking it as a `const string&` parameter, as in Chapter 5, reads the original without this copy ([Chapter 5, Section 3.2](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#32-reference-as-parameter)).

### 3.3 Output and Review

```text
noname, none, 0
Lee Hi, 2016001, 22
Kim Haha, 2016020, 29
```

The first line shows the initial values of the default constructor, the second line those of the three-parameter constructor, and the third line the information of `Kim` changed by the set functions.

---

<br>

## 4. Q3. An Average Computed with a Dynamic Integer Array

### 4.1 Requirements

Write a `main()` that allocates an array for five integers dynamically, reads five integers, computes and prints their average, and returns the array.

### 4.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. `main()` initializes the integer variable `ave`, which accumulates the input values, to 0 and allocates space for five `int` values on the heap with `int* p = new int[5];` ([Chapter 4, Section 2.3](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#23-dynamic-arrays)). After printing the prompt, it repeats five times in a `for` loop, storing each input value in the array with `cin >> p[i];` and adding the same value to `ave`. `p[i]` means the same as `*(p + i)` and accesses the `i`-th element of the dynamic array through pointer arithmetic.

When the input is done, it computes and prints the average with `ave / 5.0`. The divisor `5.0` is a `double`, so `ave`, an `int`, is converted to `double` and floating-point division is performed. Writing `ave / 5` makes both operands integers, so integer division keeps only the quotient, and for the input `1 2 4 5 10` the program prints 4 instead of 4.4. Finally it returns the array with `delete [] p;` and makes the pointer a null pointer with `p = nullptr;`.

The following three points are worth noting when reading the code.

- **Variable name:** `ave` stores not the average but the sum of the input values, and the average is computed when it is printed. Naming it `sum`, after the value it stores, makes the code easier to read.
- **Role of the array:** To compute only the average, the values could be added as soon as they are read, without an array, but the problem has five spaces allocated in order to practice dynamic arrays. Because the input values remain in the array, the program can be extended to processing that uses the input again, such as counting the numbers greater than the average.
- **Output format:** By default `cout` prints a floating-point number with up to six significant digits and omits trailing zeros. So when the average is an integer, it is printed without a decimal point, as the average of `1 2 3 4 5` is printed as `3`.

### 4.3 Output and Review

With the input `1 2 4 5 10`, the program prints the following.

```text
정수 5개 입력 >> 1 2 4 5 10
평균 4.4
```

The sum of the inputs, 22, divided by 5.0 gives 4.4, which is printed after the Korean word for "average". The prompt asks for five integers in Korean. When the input comes from a pipe, the entered values are not echoed on the screen, so the prompt and the result run together on one line, as in `정수 5개 입력 >> 평균 4.4`.

---

<br>

## 5. Q4. A Dynamic Array of Circle Objects

### 5.1 Requirements

Using the given `Circle` class as it is, read the number of circles and create a `Circle` object array of that size dynamically. Read the radius of each circle, print the number of circles whose area is 100 or more, and finally free the dynamic memory.

### 5.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Circle` class represents a circle; only the `private` member `radius` and the prototypes of the `public` member functions `setRadius()` and `getArea()` are inside the class, and the bodies are defined outside it. `setRadius()` assigns to the member with `this->radius = radius;` because the parameter has the same name as the member, and `getArea()` returns `3.14 * radius * radius`.

`main()` declares the `Circle` pointer `p`, the number of circles `size`, and `count` (initialized to 0), which counts the circles whose area is 100 or more. After reading the number of circles, it creates an array of `size` `Circle` objects on the heap with `p = new Circle [size];`. `Circle` declares no constructor at all, so the default constructor provided by the compiler is called for each element, and this constructor does not initialize `radius` ([Chapter 4, Section 3.3](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#33-dynamic-object-arrays)). For each circle, the `for` loop reads the radius into the temporary variable `temp`, sets it with `p[i].setRadius(temp)`, and immediately increments `count` by 1 if `p[i].getArea()` is 100 or more. When the loop ends, it prints the result sentence containing `count`.

> **Caution:** The last comment of the skeleton asks for the dynamic memory to be freed, but the code has no `delete [] p;`. Running it with `leaks --atExit` on macOS reports 1 leak of 32 bytes. `sizeof(Circle)` is 4, so five objects request 20 bytes, and the allocator reserves a 32-byte block for them. When the program ends the operating system reclaims the memory, so the result of this program does not change, but if the same code were inside a function called repeatedly, the leaks would accumulate and reduce the usable memory ([Chapter 4, Section 4.1](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#41-memory-leak)). With `delete [] p;` and `p = nullptr;` after the output statement, no leak is reported.

Two more points can be noted.

- **Condition and output wording:** The condition `p[i].getArea() >= 100` follows the problem's "100 or more", but the output sentence says "greater than 100", as in the sample run of the problem. With integer radii the area is 78.5 for radius 5 and 113.04 for radius 6, so it is never exactly 100 and the two conditions give the same result. Still, wording the sentence as "100 or more" to match the condition is more accurate.
- **Invalid count input:** Entering 0 runs the loop zero times and prints a count of 0. Entering a negative number makes `new Circle [size]` fail to allocate memory and throw an exception (`std::bad_alloc` in this environment), and because it is not handled, the program terminates abnormally.

### 5.3 Output and Review

With 5 as the number of circles and 3, 7, 2, 9, and 1 as the radii, the program prints the following.

```text
원의 개수 >> 5
원 1의 반지름 >> 3
원 2의 반지름 >> 7
원 3의 반지름 >> 2
원 4의 반지름 >> 9
원 5의 반지름 >> 1
면적이 100보다 큰 원은 2개 입니다.
```

The areas of the five circles are 28.26, 153.86, 12.56, 254.34, and 3.14, so the circles whose area is 100 or more are the two with radii 7 and 9, and the last line reads "There are 2 circles whose area is greater than 100" in Korean. When the input comes from a pipe, the entered values are not echoed, so the prompts run together on one line.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Dynamic objects | `new Complex` creates an object on the heap and calls the default constructor, and `delete` returns it. |
| Distinguishing names with `this` | A parameter hides the member of the same name, so only `this->real = real;` assigns to the member. |
| Dynamic arrays | An array obtained with `new int[5]` is accessed as `p[i]` and returned with `delete [] p;`. |
| Floating-point division | If one operand is a `double`, floating-point division is performed, so `ave / 5.0` is 4.4 and `ave / 5` is 4. |
| Dynamic arrays of objects | `new Circle [size]` creates as many objects as decided at run time, calling the default constructor for each element. |
| Memory leaks | Memory obtained with `new` and not returned is a leak, which `leaks --atExit` on macOS can confirm. |

---

<br>

## Self-Check Questions

1. **Omitting `this`:** If `setReal()` of Q1 is changed to `void setReal(int real) { real = real; }` and the program is run, what is printed on the fourth line?

   <details>
   <summary>Answer</summary>

   `real = real;` assigns the parameter to itself, so the member stays 1, and the fourth line prints `1 + 1i` instead of `4 + 1i`. Clang warns about the statement with `-Wself-assign`.

   </details>

2. **Dynamic creation with arguments:** If `Complex* c = new Complex(2, 3);` is executed in the `main()` of Q1 and `c->pCom();` is called, what is printed, and which constructor is called?

   <details>
   <summary>Answer</summary>

   The two-parameter constructor is called, making `this->real` 2 and `this->image` 3, so `2 + 3i` is printed. When it is no longer needed, it must be returned with `delete c;`.

   </details>

3. **Kinds of division:** What does Q3 print for the input `1 2 3 4 6`? What happens if `5.0` in the output statement is changed to `5`?

   <details>
   <summary>Answer</summary>

   The sum is 16, so `16 / 5.0` gives `평균 3.2` (average 3.2). With `5`, the integer division `16 / 5` gives the quotient, and `평균 3` is printed.

   </details>

4. **`delete` and `delete []`:** Is it correct to add `delete p;` to Q4 to prevent the leak?

   <details>
   <summary>Answer</summary>

   It is not. Memory allocated with `new[]`, as in `new Circle [size]`, must be returned with `delete []`, and returning it with `delete` is undefined behavior. `delete [] p;` must be used.

   </details>

5. **Boundary values:** If 3 is entered as the number of circles in Q4 and 5, 6, and 10 as the radii, how many circles are counted?

   <details>
   <summary>Answer</summary>

   The areas are 78.5, 113.04, and 314, so the circles whose area is 100 or more are the two with radii 6 and 10, and the program prints `면적이 100보다 큰 원은 2개 입니다.`.

   </details>
