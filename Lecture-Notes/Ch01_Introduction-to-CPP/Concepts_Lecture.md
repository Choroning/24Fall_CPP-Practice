# Chapter 1 — Introduction to C++

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 1

> **Learning Objectives:**
> 1. Explain the importance of software and the characteristics of programming languages at each level
> 2. Explain the history of the C++ language and the evolution of its standard
> 3. Explain the features of C++ and the concepts of object-oriented and generic programming
> 4. Explain the C++ development process from editing through compiling, linking, and execution, and the kinds of errors
> 5. Explain the organization of the C++ standard library and its header styles

---

## Table of Contents

- [1. Computing and Programming](#1-computing-and-programming)
  - [1.1 Importance of Software](#11-importance-of-software)
  - [1.2 Programming Languages](#12-programming-languages)
  - [1.3 Compiler](#13-compiler)
- [2. C++ Language](#2-c-language)
  - [2.1 History](#21-history)
  - [2.2 Features](#22-features)
  - [2.3 Object-Oriented Programming](#23-object-oriented-programming)
  - [2.4 Generic Programming](#24-generic-programming)
  - [2.5 The Cost of C Compatibility](#25-the-cost-of-c-compatibility)
  - [2.6 Standard C++ Importance](#26-standard-c-importance)
- [3. C++ Standard Libraries](#3-c-standard-libraries)
  - [3.1 Organization of the Standard Library](#31-organization-of-the-standard-library)
  - [3.2 The Future of `#include`: Modules](#32-the-future-of-include-modules)
- [4. C++ Development Process](#4-c-development-process)
  - [4.1 Build Pipeline](#41-build-pipeline)
  - [4.2 Error Types](#42-error-types)
  - [4.3 Execution and Debugging](#43-execution-and-debugging)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Computing and Programming

### 1.1 Importance of Software

Modern society runs on **software**. From smartphones and automobiles to banking systems and medical devices, software controls virtually every aspect of daily life. Hardware alone is inert; it is software that gives hardware purpose and functionality.

As the saying "Software is eating the world" suggests, the competitiveness of companies such as eBay, Facebook, Google, Netflix, Apple, and Samsung comes from software. Software is also at the core of the technologies driving the Fourth Industrial Revolution, such as self-driving cars, artificial intelligence (Google's AlphaGo, IBM Watson), and the Internet of Things (IoT).

```text
Computer System
├── Hardware
│   ├── CPU (Central Processing Unit)
│   ├── Memory (RAM)
│   ├── Storage (HDD/SSD)
│   └── I/O Devices (keyboard, monitor, etc.)
└── Software
    ├── System Software (OS, drivers, compilers)
    └── Application Software (browsers, games, office suites)
```

*Figure 1. Hardware and software that make up a computer system*

A **program** is a set of instructions written to perform a specific task. **Programming** (or coding) is the process of designing and writing these instructions using a programming language.

> **Key Point:** Software is the bridge between human intent and machine execution. Without programs, even the most powerful hardware cannot do useful work.

### 1.2 Programming Languages

A **programming language** is a formal notation for writing instructions that a computer can process. Languages are classified by their level of abstraction from the hardware.

| Level | Language | Example | Readability | Portability |
|:------|:---------|:--------|:----------:|:-----------:|
| Low-level | Machine Language | `10110000 01100001` | Very low | None |
| Low-level | Assembly Language | `MOV AL, 61h` | Low | None |
| High-level | C, C++, Java, Python | `cout << "Hello";` | High | High |

*Table 1. Classification of programming languages by abstraction level*

**Machine Language** (1st generation):
- Consists of binary codes (`0`s and `1`s) that the CPU directly executes.
- A CPU can inherently process only machine language.
- Differs for every CPU architecture, so it is completely **machine-dependent**.

**Assembly Language** (2nd generation):
- Represents machine instructions with human-readable **mnemonics** (e.g., `MOV`, `ADD`, `JMP`).
- One-to-one correspondence with machine instructions.
- Requires an **assembler** to translate to machine code.

**High-Level Languages** (3rd generation and above):
- Use natural-language-like syntax (`if`, `while`, `class`) to express complex tasks, data structures, and algorithms in a form people understand easily.
- One high-level statement may translate to many machine instructions.
- Requires a **compiler** or **interpreter** to translate to machine code.
- Generally **machine-independent**, so the same source code can be compiled on different platforms.

The following shows how the same computation, 35 + 56, is expressed at each language level.

| Machine Language | Assembly Language | C++ |
|:-----|:-----|:-----|
| `01010000` | `LOAD AX 35` | `int i;` |
| `00101001` | `LOAD BX 56` | `i = 35 + 56;` |
| `11101011` | `ADD AX BX` | |

*Table 2. Code that computes 35 + 56 in each language*

Converting an assembly program into machine code is called **assembling**, and converting a high-level language program into machine code is called **compiling**. The machine code and assembly in the table are illustrative; actual instructions and binary codes differ from CPU to CPU.

### 1.3 Compiler

A **compiler** translates the entire source code of a high-level language into machine code **before** execution. The resulting binary runs directly on the CPU.

```text
Source Code (C++)
        │
        ▼
   ┌──────────┐
   │ Compiler │  ← translates entire program at once
   └──────────┘
        │
        ▼
Machine Code (Binary)
        │
        ▼
   ┌──────────┐
   │   CPU    │  ← executes directly
   └──────────┘
```

*Figure 2. A compiler translating source code into machine code that the CPU executes*

| Translation Method | When | Languages | Speed |
|:-------------------|:-----|:----------|:------|
| **Compiler** | Before execution (all at once) | C, C++, Go, Rust | Fast execution |
| **Interpreter** | During execution (line by line) | Python, JavaScript, Ruby | Slower execution |

*Table 3. Comparison of compilers and interpreters*

C++ is a **compiled language**. The source code is fully translated into an executable binary before the program can run.

> **Note:** **Java's hybrid approach.** Java uses a hybrid approach in which the compiler produces bytecode, which is then interpreted (or JIT-compiled) by the JVM at runtime.

---

<br>

## 2. C++ Language

### 2.1 History

C++ was created by **Bjarne Stroustrup** at Bell Labs. He began work in **1979**, extending C with object-oriented features inspired by Simula. The language was initially called **C with Classes** and was renamed **C++** in **1983**. The `++` is C's increment operator, meaning a language one step beyond C.

The core idea: **C++ = C + Object-Oriented Programming (OOP)**.

```text
Timeline of C++
──────────────────────────────────────────────────────────────
1967    BCPL (Martin Richards) — simplified CPL for compiler writing
1969    B (Ken Thompson) — stripped-down BCPL for early UNIX
1972    C (Dennis Ritchie) — B with types; rewrote UNIX
1979    C with Classes (Bjarne Stroustrup) — C + OOP features
1983    C++ — renamed; first public release
1998    C++98 — first ISO standard (ISO/IEC 14882:1998)
2003    C++03 — bug fixes for C++98
2011    C++11 — major modernization (auto, lambda, move semantics,
                smart pointers, range-based for, nullptr)
2014    C++14 — minor improvements to C++11
2017    C++17 — structured bindings, std::optional, filesystem
2020    C++20 — concepts, coroutines, ranges, modules, three-way
                comparison (<=>)
2023    C++23 — std::print, deducing this, std::expected,
                std::flat_map, multidimensional subscript operator
2026    C++26 — contracts (pre/post/contract_assert), <debugging>,
                <inplace_vector>, <hive>, <simd>, <linalg>,
                constexpr containers (map, set, deque, ...),
                std::execution (async scheduling)
──────────────────────────────────────────────────────────────
```

*Figure 3. Timeline of the C++ language and its standards*

> **Key Point:** C++11 is widely considered the dividing line between classical C++ and modern C++. Many coding practices changed fundamentally with C++11 features like `auto`, smart pointers, and lambda expressions.

> **Note:** **C++26.** C++26 is the standard targeted for publication in 2026. One of its headline features is **contracts**, a language-level mechanism for specifying function preconditions and postconditions. Conditions are written with `pre`, `post`, and `contract_assert`, which provide a more systematic checking facility than the old `assert()` macro. Compiler support for C++26 varies by feature, so check your compiler's support status before using it.

### 2.2 Features

C++ combines the low-level power of C with high-level abstractions. Its key features are as follows.

| Feature | Description |
|:--------|:------------|
| **C Compatibility** | Compatible with C at the source and link level; existing C programs can be reused as they are, so most C code compiles as C++, and C object files and libraries can be linked |
| **Object-Oriented Programming** | Encapsulation, polymorphism, and inheritance for modeling real-world entities (Section 2.3) |
| **Strict Type Checking** | Stronger type enforcement than C; catches more errors at compile time, reducing the chance of runtime errors and making debugging easier |
| **Inline Functions** | The `inline` keyword asks the compiler to expand a function at the call site, reducing the call overhead of small, frequently called functions (Chapter 3) |
| **Function Overloading** | Declare several functions with the same name but different numbers or types of parameters (Chapter 6) |
| **Default Parameters** | Declare a function so that a preset default value is passed to a parameter when the argument is omitted (Chapter 6) |
| **References** | Alias for an existing variable (`int& ref = x;`); safer alternative to pointers in many cases (Chapter 5) |
| **Call by Reference** | Pass arguments by reference in a function call so that the function works directly on the original variables (Chapter 5) |
| **`new` / `delete`** | Dynamic memory management operators replacing C's `malloc`/`free` (Chapter 4) |
| **Operator Overloading** | Redefine operators (`+`, `-`, `<<`, etc.) for user-defined types (Chapter 7) |
| **Generics / Templates** | Write type-independent code; the compiler generates specialized versions for each type (Section 2.4, Chapter 10) |
| **STL (Standard Template Library)** | Rich collection of containers (`vector`, `map`), algorithms (`sort`, `find`), and iterators (Chapter 10) |

*Table 4. Key features of the C++ language*

Function overloading, default parameters, references, call by reference, `new` and `delete`, operator overloading, and generic functions and classes are features that C++ added to the C language.

### 2.3 Object-Oriented Programming

**The three pillars of object-oriented programming:**

```text
OOP
├── Encapsulation  — bundle data + methods; hide internal details (class, private/public)
├── Inheritance    — derive new classes from existing ones; code reuse (class Dog : public Animal)
└── Polymorphism   — same interface, different behavior (virtual functions, overriding)
```

*Figure 4. The three pillars of object-oriented programming*

**Encapsulation** wraps data in a capsule to protect it from outside access. In C++, the capsule is expressed with the `class` keyword. A **class** is a mold (blueprint) for making objects, and an **object** is an entity created from that mold; an object is also called an **instance**.

```cpp
class Circle {
private:
    int radius;   // radius value
public:
    Circle(int r) { radius = r; }
    double getArea() { return 3.14 * radius * radius; }
};
```

In the `Circle` class, which abstracts a circle, `radius` is `private` and cannot be accessed from outside; only the constructor `Circle(int r)` and `getArea()` are exposed. This class can create many circle objects with different radii. Classes and objects are covered in detail in Chapter 3.

**Polymorphism** is the phenomenon in which one function appears or behaves differently depending on the situation. C++ supports polymorphism through the following three means.

- **Operator overloading:** The same operator is given different meanings depending on the operand types. For example, `2 + 3` is 5, but `string("C") + string("++")` joins the two strings into `"C++"`, and a class representing colors can define a red object + a blue object to be a purple object.
- **Function overloading:** Several functions with the same name but different parameters are defined, such as `void add(int a, int b)`, `void add(int a, int b, int c)`, and `void add(int a, double d)`.
- **Function overriding:** A derived class redefines a virtual function of its base class, so the same function call behaves differently depending on the actual type of the object. `speak()` in the example below is such a case.

Once an **inheritance** relationship is defined, an object of the child class can use the member functions and member variables of the parent class without defining them separately. Inheritance is covered in Chapter 8, and virtual functions and overriding in Chapter 9.

```cpp
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void speak() { cout << "..." << endl; }
};

class Dog : public Animal {
public:
    void speak() override { cout << "Woof!" << endl; }
};

int main() {
    Animal* pet = new Dog();
    pet->speak();   // Output: Woof!  (polymorphism)
    delete pet;
    return 0;
}
```

> **Note:** **Support for multiple paradigms.** C++ supports multiple programming paradigms, including procedural, object-oriented, and generic programming. You are not forced to use OOP for every program.

C++ introduced object orientation for two purposes. The first is **improving software productivity**. As software life cycles became shorter, the need to reuse already written code grew, and C++ addresses this with class inheritance and object reuse. The second is **easy modeling of the real world**. Software in the past centered on mathematical computation and statistical processing, for which procedural languages were suitable, but the real world consists of objects and modern software must describe the interactions among objects, so object-oriented languages centered on objects are suitable.

**Procedural programming** lists a sequence of instructions in the order of the steps to be executed; the programmer designs a flowchart and writes the program according to it. **Object-oriented programming** defines objects and implements the program through the relationships and interactions among those objects.

### 2.4 Generic Programming

**Generic functions** and **generic classes** are functions and classes generalized so that the same program code can be applied to various data types. In C++ they are declared with the `template` keyword, so they are also called template functions and template classes, and other languages such as Java and C# have the same concept.

```cpp
template <typename T>
T bigger(T a, T b) {              // generic function that works for any type T
    return (a > b) ? a : b;
}

int main() {
    int i = bigger(3, 7);         // T = int, i = 7
    double d = bigger(2.5, 1.5);  // T = double, d = 2.5
    return 0;
}
```

The paradigm of writing programs with generic functions and generic classes is called **generic programming**, and its importance keeps growing. Templates are covered in detail in Chapter 10.

### 2.5 The Cost of C Compatibility

C++ pursues compatibility with the C language. This lets existing C program code be reused, but it comes at a cost. As in C, global variables and global functions can be used, and some functions such as `main()` can only be written as global functions, so the principle of encapsulating all data and functions inside classes breaks down. There is also a risk of side effects when several functions modify global variables.

### 2.6 Standard C++ Importance

The **ANSI/ISO C++ standard** ensures that C++ code behaves consistently across different compilers and platforms. Writing **standard-conformant code** is critical for portability.

The C++ standard was first established in 1998 by ISO and the American National Standards Institute (ANSI), and the standard document is ISO/IEC 14882. The standard has evolved through C++98 (1998), C++03 (2003), the technical report on standard library extensions TR1 (2007), C++11, C++14, C++17, C++20, and C++23.

| Category | Description | Example |
|:---------|:------------|:--------|
| **Standard Program** | Uses only features defined by the ISO standard | `std::cout`, `std::vector` |
| **Non-Standard Program** | Uses compiler-specific extensions | `__cdecl`, `__declspec`, `#pragma once` |

*Table 5. Comparison of standard and non-standard programs*

A standard C++ program compiles and runs correctly on any compliant compiler (GCC, Clang, MSVC, etc.). A non-standard program may compile only on a specific compiler or platform.

For example, a program written by the standard rules compiles on every standard compiler, such as Visual C++, GNU C++, and Borland C++, and produces the same result regardless of the operating system or compiler. In contrast, a program that uses a Visual C++-specific keyword, as in `int __cdecl main()`, compiles only with the Visual C++ compiler.

```cpp
// Standard C++ — portable
#include <iostream>
int main() {
    std::cout << "Hello" << std::endl;
    return 0;
}
```

```cpp
// Non-standard — compiler-specific extension (MSVC)
void __cdecl myFunc();   // __cdecl is MSVC-specific calling convention
```

> **Caution:** Avoid compiler-specific extensions unless absolutely necessary. If you must use them, isolate them behind platform-specific `#ifdef` guards so the rest of your code remains portable.

---

<br>

## 3. C++ Standard Libraries

### 3.1 Organization of the Standard Library

C++ provides a rich set of standard libraries. By header style, they are divided into **C-inherited libraries** and **C++-specific libraries**.

| Category | Header Style | Examples | Description |
|:---------|:-------------|:---------|:------------|
| C library (C++ form) | `<cXXX>` (no `.h`) | `<cstring>`, `<cstdio>`, `<cmath>`, `<cstdlib>` | C standard functions wrapped in the `std` namespace |
| C++ library | `<XXX>` (no `.h`) | `<iostream>`, `<string>`, `<vector>`, `<algorithm>` | C++-native features: streams, strings, containers, etc. |

*Table 6. Classification of the C++ standard library by header style*

By function, the library is divided into the following three groups.

- **C library:** Functions that adopt the existing C standard library for use in C++; they are declared in header files whose names start with `c`.
- **C++ I/O library:** The library for console and file input and output (`<iostream>`, `<fstream>`, etc.).
- **C++ STL library:** The template library that supports generic programming (`<vector>`, `<algorithm>`, etc.).

**Key differences from C headers:**

1. **No `.h` extension:** C++ standard headers do not use `.h`. `<iostream.h>` is a non-standard header from pre-standard versions of C++ and is no longer used.
2. **`std` namespace:** All standard library names live in the `std` namespace. You access them with `std::` prefix or a `using` declaration.
3. **C headers as `<cXXX>`:** The C library header `<string.h>` becomes `<cstring>` in C++, placing its functions in `std::`.

```cpp
#include <iostream>    // C++ I/O stream library
#include <string>      // C++ string class
#include <vector>      // C++ dynamic array
#include <cmath>       // C math functions (sqrt, pow, etc.) in std::

int main() {
    std::string name = "C++";
    std::vector<int> nums = {1, 2, 3};
    std::cout << name << std::endl;
    std::cout << std::sqrt(2.0) << std::endl;
    return 0;
}
```

> **Key Point:** Always use the C++ form `<cstring>` instead of the C form `<string.h>` in C++ programs. The `<cXXX>` form guarantees that names are placed in the `std` namespace.

### 3.2 The Future of `#include`: Modules

> **Note:** **C++20 and C++23.** The modules feature requires C++20 or later, and `import std;` in the example below requires C++23 or later.

C++20 introduced **modules**, a modern replacement for the traditional `#include` and header-file model. Modules eliminate many long-standing problems with headers: redundant parsing, macro leakage, and fragile include order dependencies.

```cpp
// Traditional approach (headers)
#include <iostream>

// Modern approach (modules, C++20)
import std;           // import the entire standard library as a module

int main() {
    std::cout << "Hello from modules!" << std::endl;
    return 0;
}
```

`import std;`, which imports the entire standard library as a single module, was added in C++23. In C++20, a standard header can be imported as a header unit, as in `import <iostream>;`.

Modules are not yet universally supported across all compilers and build systems, but they represent the direction C++ is heading. For now, `#include` remains the standard practice in most codebases.

---

<br>

## 4. C++ Development Process

### 4.1 Build Pipeline

The process of creating a C++ program follows the four phases below.

```text
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│   1. Edit    │───▶│ 2. Compile   │───▶│  3. Link     │───▶│ 4. Execute   │
│ (source code)│    │ (.cpp → .obj)│    │ (.obj → .exe)│    │ (run & test) │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
```

*Figure 5. The four phases of C++ program development*

```text
   main.cpp        iostream (header)
      │               │
      ▼               │
┌────────────┐        │
│Preprocessor│◀───────┘  (#include expansion)
└────────────┘
      │
      ▼
┌────────────┐
│  Compiler  │       (C++ code → object code)
└────────────┘
      │
      ▼
  main.obj           (object file — machine code, not yet executable)
      │
      ▼
┌────────────┐
│   Linker   │◀───── library files (standard library, etc.)
└────────────┘
      │
      ▼
  main.exe           (final executable)
```

*Figure 6. Building an executable through preprocessing, compiling, and linking*

| Stage | Input | Output | What It Does |
|:------|:------|:-------|:-------------|
| **Edit** | None | `.cpp` source file | Write the program in a text editor or IDE |
| **Compile** | `.cpp` source | `.obj` / `.o` object file | Translate C++ source to machine code; syntax and type checking |
| **Link** | `.obj` file(s) + libraries | `.exe` executable | Combine object files; resolve external references (e.g., `cout`) |
| **Execute** | `.exe` executable | Program output | Run the program and verify results |

*Table 7. Input, output, and work of each development stage*

- **Edit:** A C++ source program is a text file, so it can be edited with a text editor, and its standard extension is `.cpp`. An **integrated development environment (IDE)** that supports every stage of editing, compiling, linking, running, and debugging is convenient; Visual Studio is a representative example.
- **Compile:** The C++ source program is converted into an object file containing machine code (`hello.cpp` → `hello.obj`). One source line usually becomes several machine instructions; for example, `std::cout << "Hello";` is translated into instructions that push the address of the string onto the stack and call the `<<` operator function.
- **Link:** An object file cannot be executed directly. The linker connects the object file with functions of the C++ standard library to build an executable. For example, it combines `hello.obj` with the `cout` object and the `<<` operator function from the standard library to make `hello.exe`.

The extensions `.obj` and `.exe` are those of Windows (Visual Studio). On Linux and macOS, object files have the extension `.o`, and executables are built without an extension (named `a.out` if no name is given).

> **Key Point:** Compilation and linking are separate stages. The compiler translates one source file at a time into an object file. The linker then combines all object files (and libraries) into a single executable.

### 4.2 Error Types

Errors in C++ programs are classified into three kinds by **when** they are detected.

| Error Type | When Detected | Cause | Example |
|:-----------|:-------------|:------|:--------|
| **Compile Error** | Compilation | Syntax mistakes, type mismatches, undeclared identifiers | Missing `;`, wrong type conversion |
| **Link Error** | Linking | Missing function or variable definitions, unresolved symbols | Calling a function with no implementation |
| **Runtime Error** | Execution | Logic errors, illegal operations | Division by zero, accessing invalid memory |

*Table 8. Classification of errors by detection time*

**Compile error:**
```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "Hello" << endl  // missing semicolon → compile error
    return 0;
}
```

**Link error:**
```cpp
void myFunction();   // declared but never defined

int main() {
    myFunction();    // linker cannot find the definition → link error
    return 0;
}
```

**Runtime error:**
```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10, b = 0;
    cout << a / b << endl;  // division by zero → runtime error
    return 0;
}
```

Dividing an integer by zero is undefined behavior in the C++ standard, and on most systems the program terminates abnormally.

> **Caution:** Compile errors are caught by the compiler and are the easiest to fix. Runtime errors are the hardest, because the program compiles and links successfully but produces incorrect results or crashes during execution.

### 4.3 Execution and Debugging

An executable file can be run directly on its own. An error in which the desired result does not come out or the program terminates abnormally during execution is called a **run-time error**, which corresponds to the runtime error in Section 4.2.

The process of finding errors that occur during execution is called **debugging**, and a program that helps with debugging is called a **debugger**. A debugger is usually supplied together with the compiler by the company that makes it. **Source-level debugging** finds errors by executing the C++ source one line at a time while watching how variable values change, and most IDEs, including Visual Studio, support it.

---

<br>

## Summary

| Concept | Key Summary |
|:--------|:-------------|
| Software Importance | Software gives hardware purpose, and modern life and Fourth Industrial Revolution technologies depend on programs. |
| Programming Languages | The level of abstraction rises from machine language (binary) to assembly (mnemonics) to high-level languages (C, C++, Python). |
| Compiler | A compiler translates the entire source code into machine code before execution, and C++ is a compiled language. |
| C++ History | Bjarne Stroustrup started C with Classes in 1979 and renamed it C++ in 1983, and the standard continues through C++98, C++11, C++20, C++23, and C++26. |
| C++ = C + OOP | C++ is compatible with C and adds encapsulation, inheritance, and polymorphism. |
| Key Features | C++ provides function overloading, default parameters, references, `new`/`delete`, operator overloading, templates, inline functions, and the STL. |
| Purpose of OOP | Object orientation raises software productivity through code reuse and makes it easy to model a real world made of objects. |
| Generic Programming | Generic functions and classes declared with `template` apply the same code to many types. |
| Cost of C Compatibility | Global variables and functions remain usable, which breaks the encapsulation principle and risks side effects. |
| Standard C++ | The ANSI/ISO standard ensures portability, so compiler-specific extensions should be avoided. |
| C++ Libraries | Headers such as `<cstring>` (C-inherited) and `<iostream>` (C++-native) have no `.h`, and their names live in the `std` namespace. |
| Build Pipeline | Development proceeds in the order edit (.cpp), compile (.obj), link (.exe), and execute. |
| Compile Error | Syntax or type mistakes are detected during compilation. |
| Link Error | Missing definitions of functions or variables are detected during linking. |
| Runtime Error | Logic errors or illegal operations appear during execution and are found with a debugger. |

---

<br>

## Self-Check Questions

1. **Compiler and interpreter:** Explain the difference between a compiler and an interpreter in terms of translation time and execution speed, and state which approach C++ and Python each use.

   <details>
   <summary>Answer</summary>

   A compiler translates the entire source code into machine code before execution and produces an executable, whereas an interpreter translates and executes the source one line at a time during execution. Because translation is finished in advance, compiled programs generally run faster. C++ uses compilation, and Python uses interpretation.

   </details>

2. **Classifying errors:** State the kind of error in each case. (a) A function that was declared but never defined is called. (b) The semicolon at the end of a statement is missing. (c) An integer is divided by zero during execution.

   <details>
   <summary>Answer</summary>

   (a) is a link error because the linker cannot find the function's definition. (b) is a compile error because it is a syntax mistake. (c) is a runtime error because compiling and linking succeed but the problem appears during execution.

   </details>

3. **Development process:** List in order the stages from `hello.cpp` until `hello.exe` is built and run, with the output of each stage, and explain why an object file cannot be executed directly.

   <details>
   <summary>Answer</summary>

   `hello.cpp` is written by editing, compiling (including preprocessing) produces the object file `hello.obj`, linking produces the executable `hello.exe`, and then it is executed. The object file does not contain code defined in the standard library, such as the `cout` object or the `<<` operator function, so its external references are unresolved; it can run only after the linker combines it with the library.

   </details>

4. **Three pillars of OOP:** Explain encapsulation, inheritance, and polymorphism in one sentence each, and name the three means by which C++ supports polymorphism.

   <details>
   <summary>Answer</summary>

   Encapsulation bundles data and functions into a class and protects the internals from outside access. Inheritance lets a child class inherit and use the member variables and member functions of a parent class without defining them separately. Polymorphism means that one function behaves differently depending on the situation. C++ supports polymorphism through operator overloading, function overloading, and function overriding.

   </details>

5. **Standard headers:** Give the C++-form header names corresponding to the C headers `<string.h>` and `<math.h>`, and explain why C++ programs use the C++ form.

   <details>
   <summary>Answer</summary>

   They are `<cstring>` and `<cmath>`. The `<cXXX>` form is the C++ standard way, and it guarantees that the declared names are placed in the `std` namespace.

   </details>
