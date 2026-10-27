# Chapter 1 Solutions — Introduction to C++

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 1

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Textbook Theory Problems](#textbook-theory-problems)

---

<br>

## Textbook Theory Problems

**Textbook:** Even-numbered theory problems of the Chapter 1 exercises

### Theory Problem 2

**Problem:** Choose the wrong statement among four statements about programming languages and compilers.

**Answer:** ④

Machine language is binary code made of 0s and 1s, so it is hard for people to read and modify, and it differs for each CPU architecture, which makes it machine-dependent. Writing programs in machine language gives both low productivity and low portability, so learning machine language and writing programs in it is not the shortcut to becoming a good developer. Today almost all programs are written in high-level languages and translated into machine language by a compiler ([Chapter 1, Section 1.2](Concepts_Lecture.md#12-programming-languages)).

The other statements are correct.

- ① A high-level language uses syntax close to natural language, such as `if`, `while`, and `class`, so it is the easiest for people to understand.
- ② The only thing a CPU can execute directly is machine language. Programs written in assembly language or a high-level language must be translated into machine language before they run.
- ③ The program that translates a high-level language program into machine language is a compiler ([Chapter 1, Section 1.3](Concepts_Lecture.md#13-compiler)).

### Theory Problem 4

**Problem:** Choose the language that is not object-oriented among Java, C, C++, and C#.

**Answer:** ②

C is a procedural language that lists the steps to execute as a sequence of functions and statements, and it has no object-oriented features such as classes, inheritance, and access specifiers. Java, C++, and C# are object-oriented languages that define objects with classes and support encapsulation, inheritance, and polymorphism. C++ is C with object-oriented features added ([Chapter 1, Section 2.1](Concepts_Lecture.md#21-history)), and it also supports procedural and generic programming.

### Theory Problem 6

**Problem:** Choose the one that is not an object-oriented concept among inheritance, flowchart, encapsulation, and polymorphism.

**Answer:** ②

The three pillars of object-oriented programming are encapsulation, inheritance, and polymorphism ([Chapter 1, Section 2.3](Concepts_Lecture.md#23-object-oriented-programming)). A flowchart is a diagram that shows the execution order of a task with shapes and arrows; it is the design tool of procedural programming, which designs a flowchart and writes statements in that order. Object-oriented programming designs a program around the relationships and interactions among objects rather than around the execution order.

- **Encapsulation:** Bundles data and the functions that handle the data into a class and protects the inside from outside access.
- **Inheritance:** Defines a new class by inheriting the members of an existing class.
- **Polymorphism:** An operator or function of the same name behaves differently depending on its target.

### Theory Problem 8

**Problem:** Choose the problem that arose because C++ inherited the syntactic system of the C language.

**Answer:** ④

C++ pursues compatibility with C so that existing C programs can be used as they are, so, as in C, global variables and global functions can be declared outside classes. `main()` must also be written as a global function that belongs to no class. As a result, the object-oriented principle that all data and functions are encapsulated in classes breaks down, and side effects can arise when several functions modify the same global variable ([Chapter 1, Section 2.5](Concepts_Lecture.md#25-the-cost-of-c-compatibility)).

- ①, ② Difficulty in the development process or in program management is not a problem that comes directly from inheriting C syntax. On the contrary, C++ makes code reuse easier with classes and inheritance and thereby helps develop and manage large software.
- ③ Thanks to compatibility with C, C++ programs can use C libraries as they are. The C standard library is part of the C++ standard library and is used through headers such as `<cstring>` and `<cmath>` ([Chapter 1, Section 3.1](Concepts_Lecture.md#31-organization-of-the-standard-library)).

### Theory Problem 10

**Problem:** Choose the advantage gained by writing a program according to the C++ standard.

**Answer:** ②

The C++ standard (ISO/IEC 14882) defines the syntax of the language and the standard library. A program written only with the features the standard defines compiles on every standard-conforming compiler (GCC, Clang, MSVC, and so on) and produces the same result regardless of the operating system and compiler. This property of moving source code to another platform unchanged is compatibility (portability). In contrast, a program that uses an extension of a particular compiler, such as `__cdecl`, compiles only with that compiler ([Chapter 1, Section 2.6](Concepts_Lecture.md#26-standard-c-importance)).

Extensibility, execution performance, and ease of development depend more on the design of the program, the optimization of the compiler, and the development tools and libraries than on conformance to the standard, so they are hard to call advantages that come specifically from following the standard.

### Theory Problem 12

**Problem:** Explain encapsulation, polymorphism, and inheritance, using a TV object, the `>` operator, and the phrase "A grasshopper is a living creature" as the respective examples.

**Answer:**

**(1) Encapsulation:** A TV object bundles data such as the channel number, the volume, and the power state with functions (member functions) such as changing the channel, adjusting the volume, and turning the power on and off. To change the channel, the user only presses the channel button and does not need to know the internal structure that actually changes the channel. Hiding the complex internal implementation and exposing only a simple interface, so that the object is easy to use and its internal data is protected from wrong outside access, is encapsulation.

**(2) Polymorphism:** The same `>` operator behaves differently depending on the types of its operands. Used on integers as in `5 > 3`, it compares the two numbers by size and yields `true`; used on `string` objects as in `string("Apple") > string("Banana")`, it compares the two strings in dictionary order and yields `false`, because `"Apple"` comes before `"Banana"`. One operator or function working in different ways depending on its target is polymorphism, and the `>` of `string` is defined by operator overloading. Because one name can handle several types, the reusability and extensibility of the code increase.

**(3) Inheritance:** If the creature class defines the characteristics common to all living creatures and the grasshopper class inherits from the creature class, the grasshopper class has the characteristics of a creature without defining them again and can additionally define the characteristics of a grasshopper only. A relationship in which "A is a B" holds, as in "A grasshopper is a living creature," is an inheritance relationship. Inheritance increases code reusability and expresses a hierarchy among classes.

The following program runs the two comparisons of (2).

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << (5 > 3) << endl;                                 // 정수의 크기 비교
    cout << (string("Apple") > string("Banana")) << endl;    // 문자열의 사전 순 비교
    return 0;
}
```

**Output:**

```text
1
0
```

`cout` prints the `bool` values `true` and `false` as 1 and 0 by default.

> **Caution:** Comparing string literals with each other, as in `"Apple" > "Banana"`, compares the addresses of the two character arrays, not their dictionary order. The result is unspecified, and Clang warns with `-Wall`: "result of comparison against a string literal is unspecified". Since C++20, comparing two arrays is deprecated in itself. To compare strings in dictionary order, compare `string` objects, or use `strcmp()` for C-strings.

The relationship of (3) in C++ code is as follows.

```cpp
#include <iostream>
using namespace std;

class Creature {                   // 생물: 모든 생물이 공통으로 가지는 특성
public:
    void breathe() { cout << "숨을 쉰다" << endl; }
};

class Grasshopper : public Creature {   // 메뚜기는 생물이다
public:
    void jump() { cout << "높이 뛴다" << endl; }
};

int main() {
    Grasshopper g;
    g.breathe();                   // Creature에서 상속받은 멤버 함수
    g.jump();                      // Grasshopper에 추가한 멤버 함수
    return 0;
}
```

**Output:**

```text
숨을 쉰다
높이 뛴다
```

`Grasshopper` does not define `breathe()`, but it inherits from `Creature`, so `g.breathe()` can be called; the output reads "breathes" and "jumps high" in Korean. Both programs compile without warnings with `g++ -std=c++17 -Wall`. Inheritance is covered in detail in Chapter 8.

### Theory Problem 14

**Problem:** Choose the one that C++ did not newly add to the C language among the `delete` operator, function overloading, call by reference, and type conversion.

**Answer:** ④

Type conversion already existed in C. C also has both implicit type conversion, which happens automatically when `int` and `double` are mixed in a calculation, and explicit type conversion specified with a cast operator, as in `(double)n`. In contrast, the `new` and `delete` operators (C uses the `malloc()` and `free()` functions), function overloading, and references and call by reference are features absent from C that C++ newly added ([Chapter 1, Section 2.2](Concepts_Lecture.md#22-features)).

> **Note:** C++ introduced new cast operators that reveal the intent of a conversion, such as `static_cast`, `dynamic_cast`, `const_cast`, and `reinterpret_cast`. These add safer and clearer means of writing the type conversions that already existed; type conversion as a feature already existed in C.

### Theory Problem 16

**Problem:** Choose the wrong statement among four statements about C++ source files, object files, and debugging.

**Answer:** ①

A C++ source file is a text file made of characters that people can read and write, so it can be written and modified with a text editor ([Chapter 1, Section 4.1](Concepts_Lecture.md#41-build-pipeline)). The binary files are the object file and the executable file produced by compiling and linking.

The other statements are correct.

- ② `.cpp` is the usual choice, but depending on the compiler, extensions such as `.cc`, `.cxx`, and `.c++` are also recognized as C++ source. These extensions are not defined by the C++ standard document but are conventions of compilers and development tools.
- ③ The extension of an object file is `.obj` on Windows (Visual Studio) and `.o` on Linux and macOS.
- ④ Debugging is the process of finding and fixing errors in which the program does not produce the desired result or terminates abnormally while running, and a program that helps with it is a debugger ([Chapter 1, Section 4.3](Concepts_Lecture.md#43-execution-and-debugging)).

### Theory Problem 18

**Problem:** In the figure that builds `hello.exe` from `hello.obj` and the standard C++ library, explain the meaning of linking.

**Answer:** Linking is the process of combining the compiled object file (`hello.obj`) with the code of the necessary libraries into an executable file (`hello.exe`). In this process, the linker resolves references between modules, fixes addresses, and connects the necessary library functions (in this figure, the `cout` object and the `<<` operator functions of the standard C++ library).

Compiling `hello.cpp` translates into machine language only the code written in `hello.cpp`. `cout` and the `<<` operator functions are only declared in `<iostream>`, and their actual code is in the standard C++ library, so `hello.obj` contains only records that it refers to these names, and their contents are missing. These are the empty puzzle places of `hello.obj` in the figure, and for this reason an object file cannot be run directly.

The linker finds in the libraries the names that the object file refers to but does not define (unresolved symbols) and connects them, and it fixes the address each name refers to while placing the code and data of several object files and libraries into one executable file. Only when all the empty places are filled is `hello.exe` complete; if a name whose definition cannot be found remains, a link error occurs ([Chapter 1, Section 4.2](Concepts_Lecture.md#42-error-types)).

If the following `hello.cpp` is only compiled on macOS and the symbols of the object file are listed with `nm`, `std::cout` appears as a symbol not defined in this file (`U`). In libc++, the actual name of `std::cout` is `std::__1::cout`.

```cpp
#include <iostream>
int main() {
    std::cout << "Hello\n";
    return 0;
}
```

```text
$ g++ -std=c++17 -c hello.cpp -o hello.o
$ nm hello.o | c++filt | grep cout
                 U std::__1::cout
```

> **Note:** Copying the code of the libraries into the executable file is called static linking, and the figure shows this method. Recording only the necessary libraries and names in the executable file and letting the operating system connect the libraries when the program runs is called dynamic linking. Apple clang on macOS links the standard C++ library by default as the dynamic library `libc++.1.dylib`.

### Theory Problem 20

**Problem:** Choose the one that is not included in the C++ standard library.

**Answer:** ①

By function, the C++ standard library consists of the following three groups ([Chapter 1, Section 3.1](Concepts_Lecture.md#31-organization-of-the-standard-library)).

- **C library:** Accommodates the functions of the existing C standard library and is declared in headers whose names begin with `c`, such as `<cstring>` and `<cmath>`.
- **C++ input/output library:** The console and file input/output facilities provided by `<iostream>`, `<fstream>`, and others.
- **C++ STL library:** A template library of containers, algorithms, and iterators that supports generic programming, such as `vector` and `sort()`.

A library for network communication is not included in the standard library up to C++23. Network programs are written with the socket API provided by the operating system or with a separate library.

### Theory Problem 22

**Problem:** Explain what a C++ integrated development environment is.

**Answer:** A C++ integrated development environment (IDE) is software that provides, within one program, the various tools needed to develop C++ programs. It has a C++ source editor, a compiler, a linker, a source-level debugger, and so on, and supports all the steps of editing, compiling, linking, running, and debugging in an integrated way.

Without an IDE, one writes the source with a text editor, runs the compiler and the linker on the command line, and runs the debugger separately as well. An IDE connects these tools on one screen, so one build command performs compiling and linking, one can jump directly to the line with a compile error, and one can set breakpoints in the source code and run it line by line while watching the values of variables change ([Chapter 1, Section 4.3](Concepts_Lecture.md#43-execution-and-debugging)). Visual Studio on Windows and Xcode on macOS are representative C++ integrated development environments.
