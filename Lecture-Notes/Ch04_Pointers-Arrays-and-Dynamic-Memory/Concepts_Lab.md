# Chapter 4 Lab — Object Pointers, Dynamic Memory, this, and Strings

[Repository Guide](../../README.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **Last Updated:** 2026-11-18

> **Learning Objectives:**
> 1. Traverse the elements of an object array with an object pointer and explain the relationship between `p[i]` and `*(p + i)`
> 2. Explain why arrays whose size is determined at run time are not allowed in standard C++ and replace them with `new` and `delete[]`
> 3. Find the logic error that arises when a parameter and a member variable have the same name, and fix it with `this`
> 4. Use the comparison, conversion, substring, insertion, deletion, search, and replacement functions of `string`
> 5. Write the constructor and destructor of a class that has a dynamic object array as a member

---

## Table of Contents

- [1. Overview](#1-overview)
- [2. Object Pointers and Object Arrays](#2-object-pointers-and-object-arrays)
  - [2.1 Handling an Object Array with a Pointer](#21-handling-an-object-array-with-a-pointer)
  - [2.2 Complex Number Class Exercise](#22-complex-number-class-exercise)
- [3. Static and Dynamic Allocation](#3-static-and-dynamic-allocation)
  - [3.1 Arrays Whose Size Is Determined at Run Time](#31-arrays-whose-size-is-determined-at-run-time)
  - [3.2 Replacing Them with new and delete[]](#32-replacing-them-with-new-and-delete)
- [4. The this Pointer](#4-the-this-pointer)
  - [4.1 When a Parameter and a Member Share a Name](#41-when-a-parameter-and-a-member-share-a-name)
  - [4.2 Distinguishing Members with this](#42-distinguishing-members-with-this)
- [5. The string Class](#5-the-string-class)
  - [5.1 Moving the First Character to the End](#51-moving-the-first-character-to-the-end)
  - [5.2 Comparison, Conversion, and Modification Functions](#52-comparison-conversion-and-modification-functions)
- [6. A Class with a Dynamic Object Array](#6-a-class-with-a-dynamic-object-array)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overview

> **Note:** The theory of object pointers, `new` and `delete`, dynamic object arrays, the `this` pointer, and the `string` class is covered in the [Chapter 4 lecture notes](Concepts_Lecture.md). This lab confirms that material in code and writes classes that use dynamic memory.

| Topic | Location |
|:-----|:-----|
| Handling an object array with an object pointer, complex number class exercise | Section 2 |
| Handling an array of `Complex` objects with a pointer | [HW2 Problem 1](../../Coursework/HW02_Object-Pointers-and-This/Original/Assignment.md#problem-1-week-5-lecture-slides-problem-5-2) |
| Arrays whose size is determined at run time, and dynamic allocation | Section 3 |
| When the `this` pointer is needed | Section 4 |
| `string` input and its main member functions | Section 5 |
| Dynamically created `Complex` objects, a `Member` class, the average of five integers, the number of circles with area 100 or more | [HW3 Problems 1-4](../../Coursework/HW03_Dynamic-Memory/Original/Assignment.md) |
| A `Sample` class that dynamically allocates an integer array, `Circle` and `CircleManager` classes | [Chapter 4 Exercises 1 and 2](Exercises.md) |
| A `Family` class that manages family members with a dynamic array | Section 6 |

*Table 1. Topics of the lab and their location*

All code is compiled and run with the C++17 standard (for example, `g++ -std=c++17 -Wall`).

---

<br>

## 2. Object Pointers and Object Arrays

### 2.1 Handling an Object Array with a Pointer

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int r) { radius = r; }
    void setRadius(int r) { radius = r; }
    double getArea() { return 3.14 * radius * radius; }
};

int main() {
    Circle circleArray[3] = { Circle(10), Circle(20), Circle() };  // A constructor for each element

    Circle* p = circleArray;            // The array name is the address of the first element
    for (int i = 0; i < 3; i++) {
        cout << "Circle " << i << ": " << p->getArea() << endl;
        p++;                            // Move to the next element
    }

    p = circleArray;
    p[2].setRadius(30);                 // p[i] is the same as *(p + i)
    cout << "Circle 2: " << (*(p + 2)).getArea() << endl;
}
```

```text
Circle 0: 314
Circle 1: 1256
Circle 2: 3.14
Circle 2: 2826
```

- **Array initialization:** Writing a constructor call for each element inside braces creates `circleArray[0]` with `Circle(10)`, `circleArray[1]` with `Circle(20)`, and `circleArray[2]` with the default constructor `Circle()`. Without an initializer list all three elements are created with the default constructor, so declaring an object array requires a default constructor.
- **Pointer arithmetic:** `p++` advances the address not by one byte but by `sizeof(Circle)`, so it points to the next element. `p->getArea()` is the same as `(*p).getArea()`.
- **Subscripts and pointers:** `p[2]` is the same element as `*(p + 2)`. An element can also be selected by subscript without moving the pointer.

### 2.2 Complex Number Class Exercise

**Problem:** Define a complex number class `Complex` with a real part and an imaginary part as follows, and write a `main()` that behaves in the order below.

1. `private` integer member variables `real` and `image`
2. Default constructor: initializes `real` and `image` to 1 each
3. Two-parameter constructor: initializes `real` and `image` with two integer arguments
4. Set member functions that modify `real` and `image` separately
5. `showC()`, which prints `real` and `image` in complex number form

`main()` creates object `vx` with the default constructor and `vy` with the arguments (-3, 2), and stores the address of `vy` in pointer `p`. After printing both objects, it changes the `real` of `vx` to 4 and, through `p`, the `image` of `vy` to 9, and prints them again.

```cpp
#include <iostream>
using namespace std;

class Complex {
    int real, image;
public:
    Complex() { real = 1; image = 1; }
    Complex(int r, int i) { real = r; image = i; }
    void setReal(int r) { real = r; }
    void setImage(int i) { image = i; }
    void showC() { cout << real << " + " << image << "i" << endl; }
};

int main() {
    Complex vx;                         // Default constructor: 1 + 1i
    Complex vy(-3, 2);
    Complex* p = &vy;                   // Pointer to the address of vy

    vx.showC();
    vy.showC();
    cout << "--------------" << endl;
    vx.setReal(4);
    p->setImage(9);                     // Update the image of vy through the pointer
    vx.showC();
    p->showC();
}
```

```text
1 + 1i
-3 + 2i
--------------
4 + 1i
-3 + 9i
```

`p->setImage(9)` calls the member function of `vy`, the object `p` points to, so the `image` of `vy` changes. A pointer points to the original object, not to a copy of it. The problem that creates an object array of the same class and handles it with a pointer is [HW2 Problem 1](../../Coursework/HW02_Object-Pointers-and-This/Original/Assignment.md#problem-1-week-5-lecture-slides-problem-5-2).

---

<br>

## 3. Static and Dynamic Allocation

### 3.1 Arrays Whose Size Is Determined at Run Time

The following code reads the size of an array from the keyboard and creates the array.

```cpp
#include <iostream>
using namespace std;

int main() {
    int size;
    cout << "input size: ";
    cin >> size;
    int data[size];                     // An array whose size is determined at run time
    for (int i = 0; i < size; i++)
        cin >> data[i];
}
```

```text
warning: ISO C++ forbids variable length array 'data' [-Wvla]
warning: variable length arrays in C++ are a Clang extension [-Wvla-cxx-extension]
```

The first line is a warning from GCC (with `-pedantic`) and the second from Clang. In standard C++ the size of an array must be a constant expression known at compile time, so `int data[size];` is code the standard does not allow. Visual Studio (MSVC) treats this code as a compile error, while GCC and Clang accept C99's variable length array as an extension, compiling it with a warning. Portable code does not rely on this extension.

| Aspect | Static allocation | Dynamic allocation |
|:-----|:-----|:-----|
| Memory region | Stack | Heap |
| When the size is determined | At compile time | At run time |
| Allocation and release | Automatic when a function starts and ends | Allocated with `new` and released with `delete` |
| Examples | Ordinary local variables, arrays of constant size | `new int[size]`, `new Circle(30)` |

*Table 2. Comparison of static and dynamic allocation*

### 3.2 Replacing Them with new and delete[]

An array whose size must be determined at run time is allocated dynamically on the heap.

```cpp
#include <iostream>
using namespace std;

int main() {
    int size;
    cout << "input size: ";
    cin >> size;
    int* p = new int[size];             // Allocate size ints on the heap
    for (int i = 0; i < size; i++) {
        cout << i + 1 << " data input: ";
        cin >> p[i];
    }
    cout << "=== DATA ===" << endl;
    for (int i = 0; i < size; i++)
        cout << p[i] << endl;
    delete[] p;                         // Memory allocated as an array is released with delete[]
    p = nullptr;
}
```

```text
input size: 3
1 data input: 10
2 data input: 20
3 data input: 30
=== DATA ===
10
20
30
```

- `new int[size]` allocates contiguous space for `size` values of type `int` on the heap and returns the address of the first element. The pointer `p` can be used with `p[i]` just like an array name.
- Memory allocated with `new[]` must be released with `delete[]`. Releasing it with `delete p;` is undefined behavior. Setting `p` to `nullptr` after release helps prevent accidentally using the released memory again.
- If dynamically allocated memory is not released, a memory leak occurs: that memory cannot be reused until the program ends (Section 4.1 of the Chapter 4 lecture notes). In modern C++ it is common to use a container that manages its own memory, such as `std::vector<int> data(size);` (Chapter 10).

---

<br>

## 4. The this Pointer

### 4.1 When a Parameter and a Member Share a Name

> **Definition:** The **this pointer** is a pointer inside a member function that points to the object that called the function, inserted by the compiler as a hidden parameter of the member function.

The following `Circle` names the parameters of its constructor and of `setRadius()` `radius`, the same as the member variable.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int radius) { radius = radius; }               // Assigns the parameter to itself
    void setRadius(int radius) { radius = radius; }       // The member does not change
    int getRadius() { return radius; }
};

int main() {
    Circle c1;
    Circle c2(2);
    c1.setRadius(4);
    c2.setRadius(5);
    cout << c1.getRadius() << endl;
    cout << c2.getRadius() << endl;
}
```

```text
warning: explicitly assigning value of variable of type 'int' to itself; did you mean to assign to member 'radius'? [-Wself-assign]
warning: explicitly assigning value of variable of type 'int' to itself; did you mean to assign to member 'radius'? [-Wself-assign]
```

Inside the function, the name `radius` refers to the parameter in the nearest scope, so `radius = radius;` only assigns the parameter to itself and does not change the member variable. It is not a syntax error, so it compiles, and Clang warns as shown above. When run, `c1` prints the 1 set by the default constructor unchanged, and the `radius` of `c2` is never initialized, so reading its value is undefined behavior.

### 4.2 Distinguishing Members with this

There are two solutions. One is to give the parameter a name different from the member, such as `r`; the other is to name the member explicitly as `this->radius`.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { this->radius = 1; }
    Circle(int radius) { this->radius = radius; }         // Member = parameter
    void setRadius(int radius) { this->radius = radius; }
    int getRadius() { return radius; }
};

int main() {
    Circle c1;
    Circle c2(2);
    c1.setRadius(4);
    c2.setRadius(5);
    cout << c1.getRadius() << endl;
    cout << c2.getRadius() << endl;
}
```

```text
4
5
```

- The compiler turns `c1.setRadius(4)` into a call like `setRadius(&c1, 4)`, which passes the object's address as the hidden parameter `this`. So even for the same function, `this` points to `c1` when called on `c1` and to `c2` when called on `c2`.
- `this` can be used only inside member functions. A non-member function has no object, and a `static` member function can be called without an object, so it has no `this` (Chapter 6).
- `this` is also used when a member function returns the object's own address or a reference to it, which is often needed in operator overloading (reference returns in Chapter 5, Chapter 7).

---

<br>

## 5. The string Class

### 5.1 Moving the First Character to the End

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cout << "문자열을 입력하세요(한글 안됨) " << endl;
    getline(cin, s, '\n');              // Read a whole line, spaces included
    int len = s.length();
    string first = s.substr(0, 1);      // The first character
    string sub = s.substr(1, len - 1);  // The remaining characters
    s = sub + first;                    // Move the first character to the end
    cout << s << endl;
}
```

```text
문자열을 입력하세요(한글 안됨) 
You
ouY
```

- `getline(cin, s, '\n')` reads one line, spaces included, up to the newline character. `cin >> s` stops at whitespace, so it reads only one word.
- `substr(pos, n)` returns `n` characters starting at position `pos` as a new `string`, and `+` concatenates two strings.
- **Why Korean text does not work:** A `string` is a sequence of bytes (`char`), not of characters. In UTF-8 a Korean character takes 3 bytes, so `substr(0, 1)` splits off only the first byte of the first character, and moving that byte to the end corrupts the character. `length()` likewise returns the number of bytes, not characters. (The prompt "문자열을 입력하세요(한글 안됨)" means "Enter a string (no Korean).")

### 5.2 Comparison, Conversion, and Modification Functions

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string sFirst = "Hello";
    string sSnd = "Hi";
    string data = "123";

    if (sFirst.compare(sSnd) == 0)
        cout << "Same String" << endl;
    else
        cout << "Different String!" << endl;

    cout << "Length of sFirst: " << sFirst.length() << endl;
    cout << "string - integers method:" << stoi(data) + 100 << endl;

    sSnd = sFirst.substr(0, 2);         // Two characters from position 0: "He"
    cout << "1: " << sSnd << endl;
    sSnd.append("~!");                  // Append at the end: "He~!"
    cout << "2: " << sSnd << endl;
    sSnd.insert(1, "i!");               // Insert at position 1: "Hi!e~!"
    cout << "3: " << sSnd << endl;
    sSnd.erase(3, 2);                   // Delete two characters from position 3: "Hi!!"
    cout << "4: " << sSnd << endl;
    int n = sSnd.find("Hi");            // Position of the first occurrence: 0
    cout << n << endl;
    sSnd.replace(n, 3, sFirst);         // Replace three characters from n with "Hello": "Hello!"
    cout << "5: " << sSnd << endl;
}
```

```text
Different String!
Length of sFirst: 5
string - integers method:223
1: He
2: He~!
3: Hi!e~!
4: Hi!!
0
5: Hello!
```

| Function | What it does |
|:-----|:-----|
| `compare(s)` | Compares in lexicographic order; returns 0 if equal, a negative value if it comes first, a positive value if it comes later |
| `length()` | Returns the number of bytes (same as `size()`) |
| `stoi(s)` | Converts a numeric string to `int` (C++11) |
| `substr(pos, n)` | Returns the substring of `n` characters from position `pos` |
| `append(s)` | Appends `s` at the end |
| `insert(pos, s)` | Inserts `s` before position `pos` |
| `erase(pos, n)` | Deletes `n` characters from position `pos` |
| `find(s)` | Returns the position of the first occurrence of `s`, or `string::npos` if absent |
| `replace(pos, n, s)` | Replaces `n` characters from position `pos` with `s` |

*Table 3. The `string` member functions and conversion function used in the example*

- `"Hello"` and `"Hi"` differ at the second character, `'e'` (101) and `'i'` (105), and `'e'` comes first, so `compare()` returns a negative value.
- `insert(1, "i!")` inserts before position 1 (`'e'`) of `"He~!"` to make `"Hi!e~!"`, and `erase(3, 2)` deletes `'e'` at position 3 and `'~'` at position 4 to make `"Hi!!"`.
- `find()` returns `string::npos` when it finds nothing, and storing this value in an `int` makes it look like -1, so production code stores the result in `string::size_type` (or `size_t`) and compares it with `npos`.

---

<br>

## 6. A Class with a Dynamic Object Array

**Problem:** `Person` abstracts a person and `Family` abstracts a family. Add the missing member variables and member functions to the two classes so that the `main()` below behaves as in the sample run.

```cpp
class Person {
    string name;
public:
    Person() { name = ""; }
    Person(string name) { this->name = name; }
};

class Family {
    Person* p;                          // Pointer to a Person array
    int size;                           // Number of family members
public:
    Family(string name, int size);      // Dynamically creates a Person array of size elements
    void show();                        // Prints all family members
    ~Family();
};
```

```cpp
int main() {
    Family* simpson = new Family("Simpson", 3);
    simpson->setName(0, "Mr. Simpson");
    simpson->setName(1, "Mrs. Simpson");
    simpson->setName(2, "Bart Simpson");
    simpson->show();
    delete simpson;
}
```

`Family` lacks the `setName(int, string)` that `main()` uses, a member variable to store the family name, and functions to change and read a `Person`'s name. The complete code with these added is as follows.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
    string name;
public:
    Person() { name = ""; }
    Person(string name) { this->name = name; }
    void setName(string name) { this->name = name; }     // Added member function
    string getName() { return name; }                     // Added member function
};

class Family {
    string name;                        // Added member variable: family name
    Person* p;                          // Pointer to a Person array
    int size;                           // Number of family members
public:
    Family(string name, int size);
    void setName(int index, string name) { p[index].setName(name); }
    void show();
    ~Family();
};

Family::Family(string name, int size) {
    this->name = name;
    this->size = size;
    p = new Person[size];               // Dynamically create size Persons
}

void Family::show() {
    cout << name << " 가족은 다음과 같이 " << size << "명입니다." << endl;
    for (int i = 0; i < size; i++)
        cout << p[i].getName() << '\t';
    cout << endl;
}

Family::~Family() {
    delete[] p;                         // Release the dynamic array
}

int main() {
    Family* simpson = new Family("Simpson", 3);
    simpson->setName(0, "Mr. Simpson");
    simpson->setName(1, "Mrs. Simpson");
    simpson->setName(2, "Bart Simpson");
    simpson->show();
    delete simpson;
}
```

```text
Simpson 가족은 다음과 같이 3명입니다.
Mr. Simpson	Mrs. Simpson	Bart Simpson	
```

- **Dynamic object arrays and the default constructor:** `new Person[size]` calls the default constructor `Person()` for each element, so `Person` needs a default constructor. The names are set later with `setName()`. (The first output line means "The Simpson family has the following 3 members.")
- **Destruction order:** `delete simpson;` first calls the destructor of `Family`, and `delete[] p;` inside the destructor calls the destructors of the three `Person` objects and then releases the array memory. After that, the memory of the `Family` object itself is released. If the destructor did not execute `delete[] p;`, the `Person` array would remain after the `Family` object disappears, causing a memory leak.
- **Delegation:** `Family::setName()` does not change the name itself but delegates to the `setName()` of the corresponding `Person`. The `name` of `Person` is `private`, so even `Family` must go through the `public` functions of `Person`.

> **Caution:** This `Family` has a pointer member `p`, so copying an object, as in `Family a = *simpson;`, makes both objects point to the same `Person` array, and both destructors release the same array twice. Copying classes with pointer members is handled by the deep copy constructor of Chapter 5.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Initializing object arrays | A constructor can be specified for each element inside braces, and elements not specified are created with the default constructor. |
| Object pointer arithmetic | `p++` moves by `sizeof(Class)` to point to the next element, and `p[i]` is the same as `*(p + i)`. |
| Variable length arrays | `int data[size];`, whose size is determined at run time, is not allowed in standard C++ and is replaced by `new int[size]` or `vector`. |
| `new[]` and `delete[]` | Arrays allocated on the heap are released with `delete[]`, and failing to release them causes a memory leak. |
| `this` pointer | A hidden parameter holding the address of the object that called the member function; when a parameter and a member share a name, `this->member` distinguishes them. |
| Bytes in `string` | `length()` and `substr()` work in bytes, so cutting a multibyte character such as UTF-8 Korean text corrupts it. |
| `string` member functions | `compare`, `substr`, `append`, `insert`, `erase`, `find`, and `replace` compare and modify strings, and `stoi` converts them to integers. |
| Classes with dynamic arrays | The constructor creates the array with `new[]` and the destructor releases it with `delete[]`, and the element class needs a default constructor. |

---

<br>

## Self-Check Questions

1. What happens if `circleArray` in Section 2.1 is declared as `Circle circleArray[3];` and the default constructor `Circle()` is removed from the `Circle` class?

   <details>
   <summary>Answer</summary>

   A compile error results. Declaring an object array without an initializer list calls the default constructor for each element, but because the one-parameter constructor `Circle(int r)` exists, the compiler does not insert a default constructor automatically, and there is no default constructor to call. Specifying constructors for all elements, as in `{ Circle(10), Circle(20), Circle(30) }`, compiles without a default constructor.

   </details>

2. What is wrong if `delete[] p;` in the program of Section 3.2 is changed to `delete p;`?

   <details>
   <summary>Answer</summary>

   Releasing memory allocated with `new[]` by `delete` is undefined behavior. It compiles, and for an `int` array the problem may not show on the surface, but the standard guarantees nothing about the result. If the elements were objects with destructors, only the first element's destructor might be called, or the program might terminate abnormally. Allocation and release must pair `new` with `delete` and `new[]` with `delete[]`.

   </details>

3. If only `setRadius()` in the `Circle` of Section 4.1 is fixed to `void setRadius(int r) { radius = r; }` and the constructor is left as is, what is printed?

   <details>
   <summary>Answer</summary>

   `c1.setRadius(4)` and `c2.setRadius(5)` now change the member correctly, so 4 and 5 are printed. The constructor `Circle(int radius) { radius = radius; }` still does not initialize the member, but the `radius` of `c2` is set by `setRadius(5)` before it is read, so the problem does not show in this run. However, calling `getRadius()` right after creating `c2` would read an uninitialized value, so the constructor must be fixed as well.

   </details>

4. For `string s = "Hello World";`, what are `s.find("o")`, `s.substr(6, 5)`, and `s` after `s.erase(5, 6)`?

   <details>
   <summary>Answer</summary>

   `s.find("o")` returns 4, the position of the first `'o'` (the fifth of H, e, l, l, o). `s.substr(6, 5)` returns `"World"`, the five characters starting at `'W'` at position 6, and `s` does not change. `s.erase(5, 6)` deletes the six characters `" World"` starting at the space at position 5, so `s` becomes `"Hello"`.

   </details>

5. What happens when `delete simpson;` runs if `delete[] p;` is removed from the destructor of `Family` in Section 6?

   <details>
   <summary>Answer</summary>

   The memory of the `Family` object is released, but the `Person` array allocated with `new Person[size]` in the constructor is not. The pointer `p` that pointed to the array disappears with the `Family` object, so the memory can never be accessed or released again, which is a memory leak. The destructors of `Person` are not called either, so the memory held by each `Person`'s `string` also remains.

   </details>
