# HW5 Code Walkthrough — Reference Returns and Copy Constructors

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

> **Learning Objectives:**
> 1. Read the class declaration, the member function definitions, and `main()` of the two solutions line by line and explain the role of each part
> 2. Trace which object each call acts on in the chained calls of `add()` and how `value` changes
> 3. Trace how the memory that `title` of the two `Book` objects points to changes when the copy constructor and `set()` run

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: The Accumulator Class](#2-q1cpp-the-accumulator-class)
  - [2.1 Full Code](#21-full-code)
  - [2.2 Line-by-Line Explanation](#22-line-by-line-explanation)
  - [2.3 Execution Trace](#23-execution-trace)
- [3. Q2.cpp: The Book Class](#3-q2cpp-the-book-class)
  - [3.1 Full Code](#31-full-code)
  - [3.2 Line-by-Line Explanation](#32-line-by-line-explanation)
  - [3.3 Execution Trace](#33-execution-trace)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the two solutions puts, in one file, the header includes, the class declaration, the member function definitions outside the class, and `main()`, in that order. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW5 Assignment Explanation](Assignment-Explanation.md).

| File | Class | Members defined outside the class | Input |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Accumulator` | `add()` | None |
| [`Q2.cpp`](Solution/Q2.cpp) | `Book` | Constructor, copy constructor, destructor, `set()` | None |

*Table 1. Layout of the HW5 solution files*

---

<br>

## 2. Q1.cpp: The Accumulator Class

### 2.1 Full Code

```cpp
/**
 * @file    A5Q1.cpp
 * @brief   Q1. 자기 참조를 리턴하는 Accumulator 클래스 (10주차 실습 2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-07
 */

#include <iostream>
using namespace std;

class Accumulator {
    int value;
public:
    Accumulator(int value) { this->value = value; }
    Accumulator& add(int n);
    int get() { return value; }
};
Accumulator& Accumulator::add(int n) {
    value += n;
    return *this;
}

int main() {
    Accumulator acc(10);
    acc.add(5).add(6).add(7);   // acc의 value 멤버가 28이 된다.
    cout << acc.get() << endl;  // 28 출력
}
```

### 2.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-9 | `#include <iostream>`, `using namespace std;` | Includes `<iostream>` for `cout` and `endl` and omits `std::` |
| 11-12 | `class Accumulator { int value;` | `value`, declared without an access specifier, is `private` and stores the accumulated value |
| 13-14 | `public:`, constructor | The constructor defined inside the class. `this->value` is the member variable, and the `value` on the right is the parameter |
| 15 | `Accumulator& add(int n);` | Declaration of `add()`, whose return type is `Accumulator&`, a reference to an `Accumulator` object |
| 16 | `int get() { return value; }` | Returns the current accumulated value |
| 18 | `Accumulator& Accumulator::add(int n)` | The leading `Accumulator&` is the return type, and `Accumulator::` is the class this function belongs to |
| 19 | `value += n;` | Adds `n` to the `value` of the calling object |
| 20 | `return *this;` | Returns the object `this` points to, that is, the calling object itself, by reference. No copy is made |
| 24 | `Accumulator acc(10);` | Calls the constructor, so `acc.value` becomes 10 |
| 25 | `acc.add(5).add(6).add(7);` | The three calls of `add()` run from left to right, and all of them act on `acc`. The return value of the last call is not used |
| 26 | `cout << acc.get() << endl;` | Prints 28, the value of `acc.value` |

*Table 2. Line-by-line explanation of Q1.cpp*

### 2.3 Execution Trace

| Call executed | Object called on | `acc.value` afterwards | Return value |
|:-----|:-----|-----:|:-----|
| `Accumulator acc(10);` | (construction) | 10 | |
| `acc.add(5)` | `acc` | 15 | Reference to `acc` |
| `.add(6)` | `acc`, returned by the previous call | 21 | Reference to `acc` |
| `.add(7)` | `acc`, returned by the previous call | 28 | Reference to `acc` (not used) |
| `acc.get()` | `acc` | 28 | 28 (output `28`) |

*Table 3. Change of `acc.value` through the chained calls of Q1*

If `add()` returned by value, from the second row on the object called on would be the temporary copy made by the previous call instead of `acc`. `acc.value` would then stay at 15 while only the temporary copy changes to 21 and 28, so the output would be 15 ([HW5 Assignment Explanation, Section 2.2](Assignment-Explanation.md#22-design-and-solution)).

---

<br>

## 3. Q2.cpp: The Book Class

### 3.1 Full Code

```cpp
/**
 * @file    A5Q2.cpp
 * @brief   Q2. 깊은 복사 생성자를 갖춘 Book 클래스 (10주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-07
 */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char* title;
    int price;
public:
    Book(char* title, int price);
    Book(Book& b);
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};
Book::Book(char* title, int price) {
    this->title = new char [strlen(title) + 1];
    strcpy(this->title, title);
    this->price = price;
}
Book::Book(Book& b) {
    this->price = b.price;
    this->title = new char [strlen(b.title) + 1];
    strcpy(this->title, b.title);
}
Book::~Book() {
    if (!title)
        delete [] title;
}
void Book::set(char* title, int price) {
    if (strlen(this->title) < strlen(title)) {
        delete [] this->title;
        this->title = new char [strlen(title) + 1];
    }
    strcpy(this->title, title);
    this->price = price;
}


int main() {
    Book cpp("명품c++", 10000);
    Book java = cpp;
    java.set("명품자바", 12000);
    cpp.show();
    java.show();
}
```

### 3.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-10 | `#include`, `using namespace std;` | `<cstring>` provides `strlen()` and `strcpy()` |
| 12-14 | `class Book { char* title; int price;` | `private` members. `title` stores the start address of the title string allocated on the heap |
| 16 | `Book(char* title, int price);` | Declaration of the constructor. The parameter receives string literals but has no `const` |
| 17 | `Book(Book& b);` | Declaration of the copy constructor. The parameter is a reference to its own class |
| 18 | `~Book();` | Declaration of the destructor |
| 19 | `void set(char* title, int price);` | Declaration of the function that changes the title and the price |
| 20 | `void show() { ... }` | Prints the title, a space, the price, and `"원"` and ends the line |
| 22-26 | `Book::Book(char* title, int price)` | Allocates `strlen(title) + 1` bytes, copies the title, and stores the price. `this->` distinguishes the members from the parameters |
| 27-31 | `Book::Book(Book& b)` | Copies the price, allocates new memory as long as `b.title`, and copies the contents. This is a deep copy |
| 32-35 | `Book::~Book()` | `if (!title)` is true only when `title` is a null pointer, so `delete []` does not run when a title is allocated |
| 37-40 | The `if` statement of `set()` | If the existing title is shorter than the new title, frees the existing memory and allocates new memory that fits the new length |
| 41-42 | `strcpy()`, assignment of the price | Copies the title into the memory `this->title` points to and changes the price |
| 47 | `Book cpp("명품c++", 10000);` | Calls the constructor. A string literal is passed as `char*`, so a warning is issued |
| 48 | `Book java = cpp;` | A new object is initialized with `cpp`, so the copy constructor is called |
| 49 | `java.set("명품자바", 12000);` | The new title is 12 bytes, longer than the existing 9 bytes, so the memory is reallocated. This line also causes a warning |
| 50-51 | `cpp.show(); java.show();` | Prints the title and the price of both books |

*Table 4. Line-by-line explanation of Q2.cpp*

### 3.3 Execution Trace

In the following table, blocks A, B, and C are the three heap memory blocks allocated during the run. The size in parentheses is the number of bytes requested for the allocation.

| Statement executed | `cpp` (`title`, `price`) | `java` (`title`, `price`) | Output |
|:-----|:-----|:-----|:-----|
| `Book cpp("명품c++", 10000);` | Block A `"명품c++"` (10), 10000 | | |
| `Book java = cpp;` | Block A, 10000 | Block B `"명품c++"` (10), 10000 | |
| The `if` statement of `java.set(...)` | Block A, 10000 | Frees block B and allocates block C (13) | |
| The rest of `java.set(...)` | Block A, 10000 | Block C `"명품자바"`, 12000 | |
| `cpp.show();` | Block A, 10000 | Block C, 12000 | `명품c++ 10000원` |
| `java.show();` | Block A, 10000 | Block C, 12000 | `명품자바 12000원` |
| End of `main()` | `~Book()` does not free it | `~Book()` does not free it | |

*Table 5. State of the two objects as the `main()` of Q2 runs*

Block B is freed in `set()`, but blocks A and C are not freed by the destructor and remain until the program ends. They are the two leaks that `leaks --atExit` reports, allocated by the constructor at lines 22-26 and by `set()` at lines 36-43, respectively. Because the copy constructor allocated block B separately, changing the title of `java` does not affect block A of `cpp`.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Layout of a file | Each of the two solutions consists of header includes, the class declaration, member function definitions outside the class, and `main()`, in that order. |
| Returning `*this` by reference | `add()` returns the calling object with `return *this;` after `value += n;`, so `acc.value` changes to 10, 15, 21, and 28. |
| Calling the copy constructor | `Book java = cpp;` initializes a new object with an object of the same class, so it calls the copy constructor. |
| Tracking the blocks | `cpp` uses block A from start to end, while `java` receives block B in the copy constructor and block C in `set()`. |
| Memory left behind | Because of `if (!title)` in the destructor, blocks A and C are not freed, and these are the leaks that `leaks` reports. |

---

<br>

## Self-Check Questions

1. **Separate calls:** If line 25 of Q1 is split into `acc.add(5); acc.add(6); acc.add(7);`, is the result the same? Can the return type of `add()` then be `void`?

   <details>
   <summary>Answer</summary>

   The result is the same, 28. All three statements call `add()` directly on `acc` and do not use the return value, so the return type may be `void`. With the return type `void`, the chained call of line 25 would call a member function on `void`, which is a compile error.

   </details>

2. **Intermediate result:** If line 25 of Q1 is changed to `cout << acc.add(5).get() << endl;`, what is printed, and what does line 26 print after it?

   <details>
   <summary>Answer</summary>

   `acc.add(5)` makes the `value` of `acc` 15 and returns `acc`, so `get()` returns 15 and line 25 prints 15. Line 26 reads the value of the same `acc`, so it also prints 15.

   </details>

3. **A case without reallocation:** If line 49 of Q2 is changed to `java.set("C++", 12000);`, does `set()` reallocate memory? What is the output?

   <details>
   <summary>Answer</summary>

   `"C++"` is 3 bytes, shorter than the existing title of 9 bytes, so `set()` does not reallocate and copies it into block B, which the copy constructor allocated. Only `java` points to block B, so `cpp` does not change, and the output is `명품c++ 10000원` and `C++ 12000원`.

   </details>

4. **Another form of calling the copy constructor:** If line 48 of Q2 is changed to `Book java(cpp);`, which constructor is called?

   <details>
   <summary>Answer</summary>

   As with `Book java = cpp;`, the copy constructor `Book(Book& b)` is called with `cpp` as the argument. Both forms initialize a new object with an object of the same class.

   </details>

5. **Order of freeing after fixing the destructor:** If the destructor of Q2 is fixed to `delete [] title;`, which blocks are freed in which order when the program ends?

   <details>
   <summary>Answer</summary>

   Objects are destroyed in the reverse order of construction, so the destructor of `java` frees block C first, and then the destructor of `cpp` frees block A. The two objects point to different blocks, so no double free occurs, and no memory leaks either.

   </details>
