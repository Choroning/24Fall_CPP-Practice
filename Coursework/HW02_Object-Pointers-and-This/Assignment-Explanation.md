# HW2 Assignment Explanation — Object Pointers and the this Pointer

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **Learning Objectives:**
> 1. Assign the name of an object array to an object pointer and call member functions of the array elements with pointer arithmetic
> 2. Explain that `p->member` and `(*p).member`, which access a member through an object pointer, mean the same thing, and why the latter form needs parentheses
> 3. Compare an array of fixed size with a dynamic array created by `new`, and explain that variable-length arrays are not part of standard C++
> 4. Explain, in connection with dangling pointers, why `nullptr` is assigned to a pointer after its dynamic array is released with `delete[]`
> 5. Compare chaining calls by returning `this` from member functions with returning `*this` by reference

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. An Array of Complex Objects Handled Through a Pointer](#2-q1-an-array-of-complex-objects-handled-through-a-pointer)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. The Person Class Referred to Through a Pointer](#3-q2-the-person-class-referred-to-through-a-pointer)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [4. Q3. Static Arrays and Dynamic Arrays](#4-q3-static-arrays-and-dynamic-arrays)
  - [4.1 Requirements](#41-requirements)
  - [4.2 Design and Solution](#42-design-and-solution)
  - [4.3 Output and Review](#43-output-and-review)
- [5. Q4. The Counter Class That Returns the this Pointer](#5-q4-the-counter-class-that-returns-the-this-pointer)
  - [5.1 Requirements](#51-requirements)
  - [5.2 Design and Solution](#52-design-and-solution)
  - [5.3 Output and Review](#53-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the object pointers, dynamic arrays, and the `this` pointer of Chapter 4. Q1 is [Problem 5-2] of the Week 5 lecture slides, Q2 and Q4 are free code on a given topic, and Q3 is an analysis of the dynamic memory code in the Week 5 lecture slides. The assignment handout is transcribed in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW2 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW02_Object-Pointers-and-This/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # an array of Complex objects handled through a pointer
    ├── Q2.cpp               # the Person class referred to through a pointer
    ├── Q3.cpp               # the static and dynamic array code of the slides, to be analyzed
    └── Q4.cpp               # the Counter class that returns the this pointer
```

*Figure 1. Directory layout of HW2*

Each solution is compiled and run as follows. All four files compile with `g++ -std=c++11 -Wall` and `g++ -std=c++17 -Wall` under Apple clang 21 on macOS. Q3 reads a size, so one integer is entered after it starts.

```bash
cd Coursework/HW02_Object-Pointers-and-This/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

| File | Compilation | Output |
|:-----|:-----|:-----|
| `Q1.cpp` | No warnings | Same as the submitted screenshot |
| `Q2.cpp` | No warnings | Same as the submitted screenshot |
| `Q3.cpp` | No warnings | There is no submitted screenshot because this is an analysis item; for the input `5` it matches the sample run in the slides |
| `Q4.cpp` | No warnings | Same as the submitted screenshot |

*Table 1. Compilation and output of the HW2 solutions*

The last line of the submitted screenshots, `Program ended with exit code: 0`, is not program output but a message Xcode shows in the console after the program ends. Except for Q2, the `main()` functions have no `return` statement, but in C++ reaching the end of `main()` is treated as returning 0, so the exit code is 0.

---

<br>

## 2. Q1. An Array of Complex Objects Handled Through a Pointer

### 2.1 Requirements

Define a complex number class `Complex` with the `private` integer members `real` and `image`. The default constructor initializes both members to 1 and the two-parameter constructor initializes them with its arguments; the class also has a set function for each member and `showC()`, which prints the number in complex form. `main()` does the following with an object array of size `MAX` (2) and a pointer `p` that receives its address.

| Comment in the `main()` skeleton | Corresponding code |
|:-----|:-----|
| Create the `Complex` object array `data` of size `MAX` without initial values | `Complex data[MAX];` |
| Create a pointer `p` that receives the address of the array `data` | `Complex *p = data;` |
| Print the information of the object array `data` with a `for` loop | `data[i].showC();` |
| Update the `real` of the first array object to 3 | `data[0].setReal(3);` |
| Update the `image` of the second array object to -8 through the pointer `p` | `(*(p + 1)).setImage(-8);` |
| Print the information of the object array through the pointer `p` with a `for` loop | `(p + i)->showC();` |

*Table 2. The `main()` skeleton of Q1 and the corresponding code*

### 2.2 Design and Solution

The program includes `<iostream>` for input/output, defines the macro constant `MAX` as 2, and uses the standard namespace by default with `using namespace std;`. The `Complex` class represents a complex number. `real` and `image`, declared without an access specifier, are `private` members because the default access of a `class` is `private`, and the two constructors and `setReal()`, `setImage()`, and `showC()` are declared and defined together inside the class under `public`.

- **Default constructor:** initializes both members to 1 with `real = image = 1;`.
- **Two-parameter constructor:** assigns the received real and imaginary parts to the members. It is not used in this `main()`.
- **`setReal()`, `setImage()`:** change the real part and the imaginary part, respectively, to the received value.
- **`showC()`:** prints the complex number in the form `a + bi` with `real << " + " << image << "i"`.

`main()` creates the object array with `Complex data[MAX];`. Because there are no initial values, both elements are created by the default constructor and start at (1, 1) ([Chapter 4, Section 3.1](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#31-static-object-arrays)). In `Complex *p = data;` the array name `data` is converted to the address of its first element, so `p` points to `data[0]`. The first `for` loop prints the two elements with `data[i].showC()`, and after the separator line `data[0].setReal(3);` changes the real part of the first element to 3. `(*(p + 1)).setImage(-8);` dereferences with `*` the second element that `p + 1` points to and calls `setImage()` with the dot operator, changing the imaginary part to -8. Adding 1 to a pointer increases the address not by 1 byte but by the size of one element, `sizeof(Complex)` (8 bytes in this environment), so `p + 1` equals `&data[1]`. The last `for` loop prints the two elements again through the pointer with `(p + i)->showC()` ([Chapter 4, Section 1.1](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#11-pointer-to-object)).

The following two points concern the pointer notation and the output format.

- **Why the dereference needs parentheses:** The dot operator `.` has higher precedence than the dereference operator `*`. Writing `*(p + 1).setImage(-8);` without the outer parentheses is therefore parsed as `*((p + 1).setImage(-8))`, which applies the dot operator to a pointer and is a compile error. In this case Clang reports `member reference type 'Complex *' is a pointer; did you mean to use '->'?`. `(*(p + 1)).setImage(-8)`, `(p + 1)->setImage(-8)`, and `p[1].setImage(-8)` are all the same call.
- **Printing a negative imaginary part:** `showC()` does not handle the sign separately, so an imaginary part of -8 is printed as `1 + -8i`. The sample run in the slides has the same form, so this meets the requirements. To print `1 - 8i` instead, `showC()` must check whether `image` is negative and, if so, print `" - "` and `-image`.

### 2.3 Output and Review

```text
1 + 1i
1 + 1i
---------------
3 + 1i
1 + -8i
```

The output matches the submitted screenshot. It differs from the sample run in the slides only in the number of `-` characters in the separator line (15 in the code, 14 in the skeleton of the slides). The first two lines are the (1, 1) made by the default constructor, and the last two lines show the result of changing the real part of `data[0]` and the imaginary part of `data[1]`.

---

<br>

## 3. Q2. The Person Class Referred to Through a Pointer

### 3.1 Requirements

Freely write code that points to an object with a pointer and accesses the members of the object through that pointer. The solution defines a `Person` class with a name and an age and, through a pointer to the second object, calls the self-introduction function and the function that changes the age.

### 3.2 Design and Solution

The program includes `<iostream>` and `<string>` for input/output and string handling and uses the standard namespace by default with `using namespace std;`. The `Person` class represents a person; it holds the name `name` and the age `age` as `private` members and has a constructor, `introduce()`, and `setAge()` under `public`.

- **Constructor:** `Person(string n, int a) : name(n), age(a) {}` uses a **member initializer list** to initialize `name` with `n` and `age` with `a` at the moment the members are created. The result is the same as assigning in the constructor body ([Chapter 3, Section 4.4](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.md#44-ways-to-initialize-member-variables)).
- **`introduce()`:** prints a self-introduction sentence that contains the name and the age.
- **`setAge()`:** changes the age to the received value.

`main()` creates two objects with different names and ages, `person1` (age 20) and `person2` (age 25), and stores the address of `person2` in a pointer with `Person* personPtr = &person2;`. `person1.introduce();` calls the member function directly on the object, and `personPtr->introduce();` calls the member function of `person2` through the pointer. Next, `(*personPtr).setAge(31);` dereferences the pointer and calls `setAge()` with the dot operator, changing the age of `person2` to 31, and `personPtr->introduce();` again prints the self-introduction with the changed age. `p->f()` and `(*p).f()` mean the same thing, and the arrow operator is more concise ([Chapter 4, Section 1.1](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#11-pointer-to-object)).

The following two points are properties worth knowing when object pointers are used.

- **A pointer does not copy the object:** `personPtr` holds only the address of `person2`, so the age changed through the pointer is the age of `person2` itself. By contrast, if the object is copied, as in `Person copy = person2;`, then `copy.setAge(31);` changes only the copy, and the age of `person2` stays 25.
- **An uninitialized pointer:** Calling `personPtr->introduce()` through a pointer that has not been given an address, as in `Person* personPtr;`, is undefined behavior. It is safe to initialize a pointer at its declaration with the address of a valid object or with `nullptr`.

### 3.3 Output and Review

```text
안녕하세요, 제 이름은 김철수이고 20살입니다.
안녕하세요, 제 이름은 엘리자베스이고 25살입니다.
안녕하세요, 제 이름은 엘리자베스이고 31살입니다.
```

The output, in which each line is a Korean self-introduction that gives a name and an age, matches the submitted screenshot. The second and third lines both come from calling `introduce()` of `person2` through the pointer, and the `setAge(31)` call between them changed only the age from 25 to 31.

---

<br>

## 4. Q3. Static Arrays and Dynamic Arrays

### 4.1 Requirements

Analyze the code in the Week 5 lecture slides that allocates and releases dynamic memory. This item is not about writing code, so `Solution/Q3.cpp` is the code of the slides as it is. The code reads the size of an array, then fills and prints an array of fixed size and a dynamic array of the entered size.

### 4.2 Design and Solution

The submitted answer analyzes this code as a program that shows the difference between static arrays and dynamic arrays. The flow of the code is as follows.

1. It reads the size of the array into `size`.
2. It initializes the static array `arrStatic`, whose size is fixed at 4, with `{ 1, 3, 5 }` and prints its four elements. The size of the array must be fixed at compile time, so the constant 4 is used instead of the entered `size`.
3. `new int[size]` allocates `size` elements of `int` on the heap and stores their address in the `int*` pointer `pDynamic`. Unlike a static array, the size of the array can be set at run time with an entered value ([Chapter 4, Section 2.3](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#23-dynamic-arrays)).
4. The first `for` loop stores `i * 3` in each element, and the second `for` loop prints them.
5. `delete[] pDynamic;` releases the memory allocated in array form. Memory allocated with `new` leaks if it is not released, and memory allocated with `new[]` must be released with `delete[]`, not `delete` ([Chapter 4, Section 2.1](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#21-new-and-delete)).
6. After the release, `nullptr` is assigned to `pDynamic`. A pointer to memory that has already been released is called a **dangling pointer**, and using one by mistake causes undefined behavior, so the pointer is made a null pointer ([Chapter 4, Section 4.2](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#42-dangling-pointer)).

Three parts of the code can be explained more precisely as follows.

- **The array declaration in the comment:** In standard C++ the size of an array must be a constant expression whose value is known at compile time, so `int arrStatic[size];` cannot be declared with the `size` read at run time. Such an array is called a **variable-length array (VLA)**; it was introduced in C99 but is not in the C++ standard. GCC and Clang accept it as an extension (Clang issues the `-Wvla-cxx-extension` warning), while MSVC treats it as a compile error. If it is also initialized, exactly as in the comment `int arrStatic[size] = { };`, Clang 21 reports the error `variable-sized object may not be initialized`. As the slides add, a dynamic array or a `vector` is used to set the size at run time.
- **Zero initialization of the remaining elements:** When there are fewer initial values than elements, as in `int arrStatic[4] = { 1, 3, 5 };`, the remaining element is initialized to 0, so the first line is `1 3 5 0`. In contrast, the elements allocated with `new int[size]` are not initialized, so they must be read only after values are stored, as this code does. Allocating with `new int[size]()` or the C++11 form `new int[size]{}` initializes all elements to 0.
- **`nullptr` and `NULL`:** The commented-out `pDynamic = NULL;` and the actual `pDynamic = nullptr;` both make the pointer a null pointer. However, `NULL` is a macro that may be defined as the integer 0, so when `f(int)` and `f(char*)` are both overloaded, `f(NULL)` is an ambiguous call and a compile error in both Clang 21 and GCC 16. The C++11 keyword `nullptr` converts only to pointers, so `f(nullptr)` calls the pointer version. Assigning `nullptr` also protects only that pointer; any other pointer to the same memory remains a dangling pointer.

### 4.3 Output and Review

With the input `5`, the program prints the following, which matches the sample run in the slides.

```text
Input size: 5
1 3 5 0 
0 3 6 9 12 
```

Each line ends with one space because `' '` is printed after every number. Running the program with `leaks --atExit` on macOS reports no leaks, which confirms that the allocated array is fully released. If 0 is entered, an array with no elements is allocated and the second line is empty; if a negative number is entered, `new int[size]` cannot allocate the memory and throws an exception (`std::bad_alloc` in this environment), and since it is not handled, the program terminates abnormally.

---

<br>

## 5. Q4. The Counter Class That Returns the this Pointer

### 5.1 Requirements

Freely write code that uses the `this` pointer. The solution defines a `Counter` class that increases and decreases a value by 1 and makes the member functions return `this`, so that several member functions are called in a row in one statement.

### 5.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Counter` class represents a counter; it stores the current value in the `private` member `count` and has a constructor, `increment()`, `decrement()`, and `display()` under `public`.

- **Constructor:** initializes `count` to 0 with the member initializer list `: count(0)`.
- **`increment()`, `decrement()`:** increase or decrease `this->count` by 1 and then return `this`, that is, the address of the object on which the member function was called, as a `Counter*`.
- **`display()`:** prints the current value after `Count: `.

`this` is a pointer to the object on which a member function is called ([Chapter 4, Section 1.2](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#12-this-pointer)). `c.increment()` sets the `count` of `c` to 1 and returns `&c`, so `c.increment()->increment()` calls `increment()` of the same object `c` again. This style, in which member functions return the object itself so that calls can be written in a row in one statement, is called **method chaining**. The first statement of `main()` changes `count` from 0 to 1 and 2 and prints `Count: 2` with `display()`, and the second statement decreases it to 1 and prints `Count: 1`. Because no parameter has the same name as the member, `this->count` in this code could be written as just `count` and would refer to the same member.

The following three points are properties that depend on the return type.

- **Comparison with returning a reference:** If the object itself is returned by reference, as in `Counter& increment() { count++; return *this; }`, the calls can be chained with the dot operator, as in `c.increment().increment().display();`, with the same result. The operator overloading of Chapter 7 and the `Accumulator` of HW5 Q1 use this form.
- **The pitfall of returning by value:** If the return type is `Counter` and `*this` is returned, every call returns a copy of the object. Then `c.increment().increment().display();` prints `Count: 2`, but the second `increment()` applies to the copy, so the `count` of `c` stays at 1. The following `c.decrement().display();` therefore prints `Count: 0`.
- **The end of the chain:** `display()` returns `void`, so it can only come last in a chain. `c.increment()->display()->increment();` applies `->` to `void` and is a compile error.

### 5.3 Output and Review

```text
Count: 2
Count: 1
```

The output matches the submitted screenshot and the values written in the two comments of the code.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Object arrays and pointers | The array name is converted to the address of its first element, so in `Complex *p = data;` the expression `p + i` points to element `i`. |
| Arrow operator | `p->f()` is the same as `(*p).f()`, and the latter form needs parentheses because `.` has higher precedence than `*`. |
| Variable-length arrays | In standard C++ an array size must be a constant expression, so a dynamic array made with `new` or a `vector` is used to set the size at run time. |
| Partial initialization | When an array has fewer initial values than elements, the remaining elements are initialized to 0. |
| `delete[]` and `nullptr` | Memory obtained with `new[]` is released with `delete[]`, and assigning `nullptr` to the pointer afterwards prevents the use of a dangling pointer. |
| Returning `this` | When a member function returns `this`, calls on the same object can be chained with `->`; when it returns `*this` by reference, they can be chained with `.`. |

---

<br>

## Self-Check Questions

1. **Pointer arithmetic:** In Q1, when `p` points to `data`, which value of which element changes if `(p + 1)->setReal(7);` is executed? How is the same call written with a subscript?

   <details>
   <summary>Answer</summary>

   `p + 1` points to the second element `data[1]`, so the `real` of `data[1]` becomes 7. `p[1]` means the same as `*(p + 1)`, so `p[1].setReal(7);` or `data[1].setReal(7);` is the same call.

   </details>

2. **Without a default constructor:** What happens to `Complex data[MAX];` if the default constructor is removed from the `Complex` class of Q1?

   <details>
   <summary>Answer</summary>

   Because the two-parameter constructor is defined, the compiler does not provide a default constructor. An object array without initial values must call the default constructor for every element, so a compile error occurs. It compiles if a constructor is specified for each element, as in `Complex data[MAX] = { Complex(1, 1), Complex(1, 1) };`.

   </details>

3. **Partial initialization:** In Q3, what is printed on the first line if `int arrStatic[4] = { 1, 3, 5 };` is changed to `int arrStatic[4] = { };`?

   <details>
   <summary>Answer</summary>

   There are no initial values at all, so all four elements are initialized to 0 and `0 0 0 0 ` is printed. By contrast, a local array declared as `int arrStatic[4];` without `= { }` is not initialized, so the printed values cannot be predicted.

   </details>

4. **The reach of assigning `nullptr`:** In Q3, if `int* q = pDynamic;` is added before `delete[] pDynamic;`, what is the state of `q` after `pDynamic = nullptr;` runs?

   <details>
   <summary>Answer</summary>

   `q` still holds the address of the released memory, so it is a dangling pointer. Assigning `nullptr` changes only the pointer it is assigned to, `pDynamic`, and reading elements through `q` or applying `delete[]` to it again is undefined behavior.

   </details>

5. **Returning by value:** If `increment()` and `decrement()` of Q4 are changed to return `Counter` by value with `return *this;`, and `main()` is written as `c.increment().increment().display();` and `c.decrement().display();`, what is the output?

   <details>
   <summary>Answer</summary>

   It is `Count: 2` and `Count: 0`. Only the first `increment()` applies to `c`, so the `count` of `c` becomes 1, while the second `increment()` and `display()` apply to the copy and print 2. Then `c.decrement()` decreases the `count` of `c` to 0 and returns a copy, so `Count: 0` is printed.

   </details>
