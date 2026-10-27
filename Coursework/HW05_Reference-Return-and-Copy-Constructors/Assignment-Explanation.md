# HW5 Assignment Explanation — Reference Returns and Copy Constructors

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

> **Learning Objectives:**
> 1. Explain why chained calls all act on the same object when a member function returns `*this` by reference, and compare the result with returning by value
> 2. Explain the member-by-member copy performed by the default copy constructor of a class with a pointer member and the shallow copy problem it causes
> 3. Write a deep copy constructor that allocates new memory and copies the contents
> 4. Distinguish a memory leak, a use of freed memory, and a double free, and find their causes from the reports of `leaks` and AddressSanitizer

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. The Accumulator Class That Returns a Reference to Itself](#2-q1-the-accumulator-class-that-returns-a-reference-to-itself)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. The Book Class with a Deep Copy Constructor](#3-q2-the-book-class-with-a-deep-copy-constructor)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the reference returns and copy constructors of Chapter 5. Q1 and Q2 are [Lab 2] and [Lab 3] of the Week 10 lecture slides. The assignment handout is in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW5 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW05_Reference-Return-and-Copy-Constructors/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # the Accumulator class that returns a reference to itself
    └── Q2.cpp               # the Book class with a deep copy constructor
```

*Figure 1. Directory layout of HW5*

Each solution is compiled and run with C++11 or later as follows.

```bash
cd Coursework/HW05_Reference-Return-and-Copy-Constructors/Solution
g++ -std=c++17 -Wall -o q2 Q2.cpp
./q2
```

When a program runs in Xcode, the last console line, `Program ended with exit code: 0`, is not program output but a message Xcode shows after the program ends. Neither `main()` function has a `return` statement, but in C++ reaching the end of `main()` is treated as returning 0, so the exit code is 0.

---

<br>

## 2. Q1. The Accumulator Class That Returns a Reference to Itself

### 2.1 Requirements

Implement the `Accumulator` class as declared so that the statement that calls `add()` in a chain, `acc.add(5).add(6).add(7);`, changes the `value` of `acc` from 10 to 28. The constructor and `get()` are given with their bodies in the declaration, so the part to write is the definition of `add(int n)`, whose return type is `Accumulator&`.

### 2.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Accumulator` class represents an object that accumulates values; `value`, which stores the accumulated value, is a `private` member so that outside code cannot access it directly. A member declared in a class without an access specifier is `private`. The constructor, `add()`, and `get()` are under `public`.

- **Constructor:** assigns the parameter `value` to the member variable `value`. The two names are the same, so `this->value` refers to the member variable and distinguishes it from the parameter.
- **`add(n)`:** defined outside the class with `Accumulator::`. It adds `n` to `value` and returns `*this`, that is, the object that called `add()`, by reference.
- **`get()`:** returns the current `value`.

`main()` creates the object `acc` with the initial value 10, calls `add()` in a chain to add 5, 6, and 7 in turn, and prints the final value 28 with `get()`. `acc.add(5).add(6).add(7)` is evaluated from left to right. Because `acc.add(5)` returns a reference to `acc` itself, `.add(6)` on that result is again a member function call on `acc`, and so is `.add(7)`. Calling the next function on the object a function returned in this way is called **chaining**. The result of a function that returns a reference refers to the existing object itself, not to a copy ([Chapter 5, Section 3.3](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#33-reference-return)).

The `&` in the return type decides this behavior. If `add()` is declared as `Accumulator add(int n)` and returns by value, the program still compiles, but it prints 15. The first `acc.add(5)` acts on `acc` and makes `value` 15, but what it returns is a temporary object that is a copy of `acc`. `.add(6)` and `.add(7)` therefore change the value of that temporary to 21 and 28, the temporary disappears at the end of the statement, and `acc.get()` returns 15. With the return type `void`, separate calls such as `acc.add(5);` work, but a member function cannot be called on `void`, so the chained call is a compile error.

### 2.3 Output and Review

```text
28
```

The program uses no dynamic memory, and `acc` is destroyed automatically when `main()` ends.

---

<br>

## 3. Q2. The Book Class with a Deep Copy Constructor

### 3.1 Requirements

Answer four questions about a `Book` class that stores the title of a book as a dynamically allocated string and the price as an integer. The questions and where their answers are found are as follows.

| Question | Content | Answer |
|:-----|:-----|:-----|
| 1) | Implementing the constructor, the destructor, and `set()` | Lines 22-26, 32-35, and 36-43 of `Q2.cpp` |
| 2) | The code of the default copy constructor the compiler adds | The answer in Section 3.2 |
| 3) | The problem that occurs when no copy constructor is defined | The answer in Section 3.2 |
| 4) | Defining a copy constructor that causes no error | Lines 27-31 of `Q2.cpp` |

*Table 1. The questions of Q2 and where the answers are*

### 3.2 Design and Solution

The program includes `<iostream>` and `<cstring>` for input/output and string handling and uses the standard namespace by default with `using namespace std;`. The `Book` class represents a book and has the title `title` and the price `price` as `private` members. `title` is a `char` pointer that stores the start address of the title string allocated dynamically. The constructor, the copy constructor, the destructor, `set()`, and `show()` are under `public`.

- **Constructor:** allocates `strlen(title) + 1` bytes, the length of the received title plus one byte for the null character at the end of the string, with `new char []`, copies the title with `strcpy()`, and stores the price. The parameters and the members have the same names, so `this->` refers to the members.
- **Copy constructor:** copies the price of the original object `b`, allocates new memory as long as `b.title`, and copies the contents of the string. The original and the copy each have their own memory, so this is a **deep copy**, and it is the answer to question 4) ([Chapter 5, Section 4.5](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#45-deep-copy)).
- **Destructor:** written to free the memory `title` points to with `delete []`. Its condition has an error, which Section 3.3 covers.
- **`set()`:** if the new title is longer than the existing title, frees the existing memory and allocates new memory that fits the new length. Otherwise it reuses the existing memory. It then copies the title and changes the price.
- **`show()`:** prints the title and the price in the form `명품c++ 10000원`.

`main()` creates `cpp` and creates `java` by copying `cpp` with `Book java = cpp;`. A new object is initialized with an object of the same class, so the copy constructor is called ([Chapter 5, Section 4.1](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#41-concept)). It then changes the title and the price of `java` with `java.set()` and prints both books. When the program ends, the destructors of the two objects are called in the reverse order of construction.

The answer to question 2) is that the default copy constructor the compiler creates performs a member-by-member copy and is equivalent to the following code.

```cpp
Book::Book(Book& b) {
    title = b.title;
    price = b.price;
}
```

The answer to question 3) is as follows. In the default copy constructor above, `title` is a pointer, so only a shallow copy is performed: the address `title` points to is copied, and the dynamically allocated contents are not. If the destructor deletes the `title` of one object and then deletes the `title` of the other object again, the same memory is deleted twice, and the answer calls this a memory leak.

The two answers can be completed as follows.

- **The prototype of the default copy constructor:** The answer that it is a member-by-member copy is correct ([Chapter 5, Section 4.2](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#42-default-copy-constructor)). In standard C++, however, the copy constructor the compiler declares implicitly has the form `Book(const Book&)` when every member can be copied from a `const` object, and the `char*` and `int` members satisfy this condition. The default copy constructor can therefore copy a `const Book` object as well, while in a class that defines only `Book(Book&)`, copying a `const Book` object is a "no matching constructor" error. What they do between two objects, copying `title` and `price` unchanged, is the same.
- **The name of the problem:** Freeing the same memory twice is not a memory leak but a **double free**. A memory leak is memory that is left unfreed, while a double free frees memory that has already been freed; its result is undefined behavior, and usually the program terminates abnormally ([Chapter 5, Section 4.3](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#43-shallow-copy-problem)).
- **The problem that comes first in this `main()`:** In this `main()`, a use of freed memory occurs before the double free. `"명품자바"` is 12 bytes in UTF-8 and `"명품c++"` is 9 bytes, so `java.set()` frees the memory shared with `cpp` and allocates new memory only for `java`. `cpp.title` then becomes a **dangling pointer** that points to freed memory, and `cpp.show()` reads that memory ([Chapter 4, Section 4.2](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#42-dangling-pointer)). This situation can be reproduced as described at the end of Section 3.3.

Three points can be considered to make the code more robust.

- **`const char*` parameters:** The type of a string literal is `const char[N]`, and the conversion of it to `char*` was removed from the standard in C++11. Apple clang accepts it as an extension and issues the `-Wwritable-strings` warning, but the standards conformance mode of Visual Studio 2022 (`/permissive-`) treats this conversion as an error. This conversion is why the assignment handout says that `const` may have to be added in Visual Studio 2022. Changing the parameters of the constructor and `set()` to `const char* title` removes the two warnings at lines 47 and 49.
- **A copy constructor that takes a `const` reference:** The copy constructor does not change the original, so declaring it as `Book(const Book& b)` also allows copying from `const` objects and temporary objects ([Chapter 5, Section 5.3](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#53-copy-constructor-parameter-must-be-a-reference)).
- **The assignment operator:** Even with a deep copy constructor, assigning one existing object to another with `java = cpp;` makes the default assignment operator generated by the compiler copy only the pointer value. Testing it with a `Book` whose destructor frees correctly, AddressSanitizer reports a double free, and the memory `java` originally pointed to leaks. A class with a pointer member must define the assignment operator together with the copy constructor and the destructor ([Chapter 5, Section 2.3](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#23-object-assignment)).

### 3.3 Output and Review

```text
명품c++ 10000원
명품자바 12000원
```

The output gives each title followed by its price in won. The title of `cpp` stays the same after the title of `java` is changed because the copy constructor made a deep copy. The two `-Wall` warnings arise because lines 47 and 49 pass string literals to `char*` parameters, and they disappear when the parameters are changed to `const char*` as in Section 3.2.

Running this code under the memory checking tools reports the following.

- **AddressSanitizer and UndefinedBehaviorSanitizer:** report no errors. The copy constructor makes a deep copy, so no freed memory is read and no memory is freed twice.
- **`leaks --atExit`:** reports a leak of 2 blocks, 64 bytes in total. One is the memory the constructor allocated for the title of `cpp`, and the other is the memory `set()` allocated for the new title of `java`. The memory the copy constructor allocated for `java` was freed by `set()`, so it does not remain.

The cause of the leak is the condition `if (!title)` of the destructor at lines 32-35. The condition is true only when `title` is a null pointer, so in the normal case where a title is allocated, `delete []` is not executed. To check for a null pointer, the condition would have to be `if (title)`, but passing a null pointer to `delete []` does nothing, so writing only `delete [] title;` without a condition is enough ([Chapter 4, Section 4.3](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.md#43-destructor-responsibility)). With this fix the output is the same and `leaks` reports no leaks.

The situation of question 3) can be checked by deleting the copy constructor at lines 27-31 and its declaration at line 17 and running the program with the default copy constructor.

- **AddressSanitizer:** reports a heap-use-after-free, because `cpp.show()` reads the memory freed in `set()`.
- **Running without ASan:** on macOS the first line can be printed as `명품자바 10000원`. The allocator handed the block freed in `set()` back for the new title of `java`, so `cpp.title` pointed to that string; this is undefined behavior, so the result differs between environments.
- **With the destructor also fixed:** when the program ends, the destructor of `cpp` frees the already freed memory again. AddressSanitizer reports this as attempting double-free, and without ASan the program terminates abnormally after the output.
- **With the solution's destructor unchanged:** the destructor frees nothing, so no double free appears, but the use of freed memory still occurs.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Reference return | `add()`, which returns `Accumulator&`, gives back `*this`, the calling object itself, so all three calls of `acc.add(5).add(6).add(7)` act on `acc`. |
| Difference from returning by value | If `add()` returns by value, the calls from the second one on act on a temporary copy, so the result is 15, not 28. |
| Default copy constructor | If no copy constructor is defined, the compiler creates one that copies member by member, and for a pointer member only the address is copied. |
| Shallow copy problem | When two objects point to the same memory, a use of freed memory or a double free occurs; this is undefined behavior, not a memory leak. |
| Deep copy | The copy constructor allocates new memory with `new char [strlen(b.title) + 1]` and copies the contents with `strcpy()`. |
| Condition of the destructor | `if (!title) delete [] title;` frees nothing and causes a leak; `delete []` is safe on a null pointer, so the memory is freed without a condition. |
| String literals and `const` | A parameter that receives a string literal must be declared as `const char*` to conform to the standard since C++11. |

---

<br>

## Self-Check Questions

1. **Returning by value:** If `add()` of Q1 is changed to return by value, as in `Accumulator add(int n)`, what is `acc.get()` after `acc.add(5).add(6).add(7);`?

   <details>
   <summary>Answer</summary>

   It is 15. Only the first `add(5)` acts on `acc`; its return value is a temporary object that is a copy of `acc`, so `add(6)` and `add(7)` change the value of the temporary. The temporary disappears at the end of the statement.

   </details>

2. **The return statement:** What happens if `return *this;` in Q1 is written as `return this;`?

   <details>
   <summary>Answer</summary>

   A compile error occurs. `this` is a pointer of type `Accumulator*`, so it cannot be bound to the return type `Accumulator&`. To return a reference, the function must return `*this`, the object the pointer points to.

   </details>

3. **Shared memory:** If the copy constructor of Q2 is deleted and `java.set("명품자바", 12000);` is changed to `java.set("자바", 12000);`, what is the output?

   <details>
   <summary>Answer</summary>

   The first line is `자바 10000원`, not `명품c++ 10000원`, and the second line is `자바 12000원`. `"자바"` is 6 bytes, shorter than the existing title of 9 bytes, so `set()` does not reallocate and copies into the shared memory. The `title` of both objects points to the same memory, so the title of `cpp` changes as well.

   </details>

4. **Order of destruction:** If the copy constructor of Q2 is deleted, the destructor is fixed to `delete [] title;`, and the call of `set()` is also deleted, what happens when the program ends?

   <details>
   <summary>Answer</summary>

   Objects are destroyed in the reverse order of construction, so the destructor of `java` frees the shared memory first, and then the destructor of `cpp` frees the same memory again. This is a double free and undefined behavior. AddressSanitizer reports an attempting double-free in `~Book()`.

   </details>

5. **Allocation size:** Give the number of bytes the constructor of Q2 allocates for `"명품c++"` and the reason 1 is added to the result of `strlen()`.

   <details>
   <summary>Answer</summary>

   In UTF-8 the two Korean characters take 6 bytes and `c++` takes 3 bytes, so `strlen()` returns 9 and the allocated size is 10 bytes. `strlen()` does not count the null character at the end of the string, but `strcpy()` copies the null character as well, so 1 byte is added for it.

   </details>
