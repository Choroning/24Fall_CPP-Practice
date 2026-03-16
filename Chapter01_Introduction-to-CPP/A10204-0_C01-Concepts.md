# Chapter 01 — Introduction to C++

> **Last Updated:** 2026-03-16

---

## Table of Contents

- [1. Computing and Programming](#1-computing-and-programming)
  - [1.1 Importance of Software](#11-importance-of-software)
  - [1.2 Programming Languages](#12-programming-languages)
  - [1.3 Compiler](#13-compiler)
- [2. C++ Language](#2-c-language)
  - [2.1 History](#21-history)
  - [2.2 Features](#22-features)
  - [2.3 Standard C++ Importance](#23-standard-c-importance)
- [3. C++ Standard Libraries](#3-c-standard-libraries)
  - [The Future of `#include`: Modules](#the-future-of-include-modules)
- [4. C++ Development Process](#4-c-development-process)
  - [4.1 Build Pipeline](#41-build-pipeline)
  - [4.2 Error Types](#42-error-types)
- [Summary](#summary)

---

<br>

## 1. Computing and Programming

### 1.1 Importance of Software

Modern society runs on **software**. From smartphones and automobiles to banking systems and medical devices, software controls virtually every aspect of daily life. Hardware alone is inert — it is software that gives hardware purpose and functionality.

```
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

A **program** is a set of instructions written to perform a specific task. **Programming** (or coding) is the process of designing and writing these instructions using a programming language.

> **Key Point:** Software is the bridge between human intent and machine execution. Without programs, even the most powerful hardware cannot do useful work.

### 1.2 Programming Languages

A **programming language** is a formal notation for writing instructions that a computer can process. Languages are classified by their level of abstraction from the hardware.

| Level | Language | Example | Readability | Portability |
|:------|:---------|:--------|:----------:|:-----------:|
| Low-level | Machine Language | `10110000 01100001` | Very low | None |
| Low-level | Assembly Language | `MOV AL, 61h` | Low | None |
| High-level | C, C++, Java, Python | `cout << "Hello";` | High | High |

**Machine Language** (1st generation):
- Binary codes (`0`s and `1`s) that the CPU directly executes.
- Different for every CPU architecture — completely **machine-dependent**.

**Assembly Language** (2nd generation):
- Uses human-readable **mnemonics** (e.g., `MOV`, `ADD`, `JMP`).
- One-to-one correspondence with machine instructions.
- Requires an **assembler** to translate to machine code.

**High-Level Languages** (3rd generation and above):
- Use natural-language-like syntax (`if`, `while`, `class`).
- One high-level statement may translate to many machine instructions.
- Requires a **compiler** or **interpreter** to translate to machine code.
- Generally **machine-independent** — the same source code can be compiled on different platforms.

### 1.3 Compiler

A **compiler** translates the entire source code of a high-level language into machine code **before** execution. The resulting binary runs directly on the CPU.

```
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

| Translation Method | When | Languages | Speed |
|:-------------------|:-----|:----------|:------|
| **Compiler** | Before execution (all at once) | C, C++, Go, Rust | Fast execution |
| **Interpreter** | During execution (line by line) | Python, JavaScript, Ruby | Slower execution |

C++ is a **compiled language**. The source code is fully translated into an executable binary before the program can run.

> **Tip:** Java uses a hybrid approach — the compiler produces bytecode, which is then interpreted (or JIT-compiled) by the JVM at runtime.

---

<br>

## 2. C++ Language

### 2.1 History

C++ was created by **Bjarne Stroustrup** at Bell Labs. He began work in **1979**, extending C with object-oriented features inspired by Simula. The language was initially called **"C with Classes"** and was renamed **C++** in **1983** (the `++` being C's increment operator — "one step beyond C").

The core idea: **C++ = C + Object-Oriented Programming (OOP)**.

```
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

> **Key Point:** C++11 is widely considered the dividing line between "classical C++" and "modern C++." Many coding practices changed fundamentally with C++11 features like `auto`, smart pointers, and lambda expressions.

> **[C++26]** C++26 is expected to be ratified in 2026. Its headline feature is **Contracts** — a language-level mechanism for specifying function preconditions and postconditions, replacing the old `assert()` macro with `pre()`, `post()`, and `contract_assert()`. Most compilers do not yet support C++26 features.

### 2.2 Features

C++ combines the low-level power of C with high-level abstractions. Its key features include:

| Feature | Description |
|:--------|:------------|
| **C Compatibility** | Compatible with C at the source and link level; most C code compiles as C++ |
| **Object-Oriented Programming** | Encapsulation, polymorphism, and inheritance for modeling real-world entities |
| **Strict Type Checking** | Stronger type enforcement than C; catches more errors at compile time |
| **Inline Functions** | `inline` keyword allows function expansion at the call site, avoiding function call overhead |
| **Generics / Templates** | Write type-independent code; the compiler generates specialized versions for each type |
| **Operator Overloading** | Redefine operators (`+`, `-`, `<<`, etc.) for user-defined types |
| **References** | Alias for an existing variable (`int& ref = x;`); safer alternative to pointers in many cases |
| **`new` / `delete`** | Dynamic memory management operators replacing C's `malloc`/`free` |
| **STL (Standard Template Library)** | Rich collection of containers (`vector`, `map`), algorithms (`sort`, `find`), and iterators |

**Object-Oriented Programming — Three Pillars:**

```
OOP
├── Encapsulation  — bundle data + methods; hide internal details (class, private/public)
├── Inheritance    — derive new classes from existing ones; code reuse (class Dog : public Animal)
└── Polymorphism   — same interface, different behavior (virtual functions, overriding)
```

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

> **Tip:** C++ supports multiple programming paradigms — procedural, object-oriented, and generic programming. You are not forced to use OOP for every program.

### 2.3 Standard C++ Importance

The **ANSI/ISO C++ standard** ensures that C++ code behaves consistently across different compilers and platforms. Writing **standard-conformant code** is critical for portability.

| Category | Description | Example |
|:---------|:------------|:--------|
| **Standard Program** | Uses only features defined by the ISO standard | `std::cout`, `std::vector` |
| **Non-Standard Program** | Uses compiler-specific extensions | `__cdecl`, `__declspec`, `#pragma once` |

A standard C++ program compiles and runs correctly on any compliant compiler (GCC, Clang, MSVC, etc.). A non-standard program may compile only on a specific compiler or platform.

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

> **Warning:** Avoid compiler-specific extensions unless absolutely necessary. If you must use them, isolate them behind platform-specific `#ifdef` guards so the rest of your code remains portable.

---

<br>

## 3. C++ Standard Libraries

C++ provides a rich set of standard libraries. These are divided into two categories: **C-inherited libraries** and **C++-specific libraries**.

| Category | Header Style | Examples | Description |
|:---------|:-------------|:---------|:------------|
| C library (C++ form) | `<cXXX>` (no `.h`) | `<cstring>`, `<cstdio>`, `<cmath>`, `<cstdlib>` | C standard functions wrapped in the `std` namespace |
| C++ library | `<XXX>` (no `.h`) | `<iostream>`, `<string>`, `<vector>`, `<algorithm>` | C++-native features: streams, strings, containers, etc. |

**Key differences from C headers:**

1. **No `.h` extension** — C++ standard headers do not use `.h`. Writing `<iostream.h>` is non-standard and deprecated.
2. **`std` namespace** — All standard library names live in the `std` namespace. You access them with `std::` prefix or a `using` declaration.
3. **C headers as `<cXXX>`** — The C library header `<string.h>` becomes `<cstring>` in C++, placing its functions in `std::`.

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

### The Future of `#include`: Modules

> **[C++20]** This feature requires C++20 or later.

C++20 introduced **modules**, a modern replacement for the traditional `#include` / header-file model. Modules eliminate many long-standing problems with headers: redundant parsing, macro leakage, and fragile include order dependencies.

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

Modules are not yet universally supported across all compilers and build systems, but they represent the direction C++ is heading. For now, `#include` remains the standard practice in most codebases.

---

<br>

## 4. C++ Development Process

### 4.1 Build Pipeline

The process of creating a C++ program follows four main phases:

```
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│   1. Edit    │───▶│ 2. Compile   │───▶│  3. Link     │───▶│ 4. Execute   │
│ (source code)│    │ (.cpp → .obj)│    │ (.obj → .exe)│    │ (run & test) │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
```

```
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

| Stage | Input | Output | What It Does |
|:------|:------|:-------|:-------------|
| **Edit** | — | `.cpp` source file | Write the program in a text editor or IDE |
| **Compile** | `.cpp` source | `.obj` / `.o` object file | Translate C++ source to machine code; syntax and type checking |
| **Link** | `.obj` file(s) + libraries | `.exe` executable | Combine object files; resolve external references (e.g., `cout`) |
| **Execute** | `.exe` executable | Program output | Run the program and verify results |

> **Key Point:** Compilation and linking are separate stages. The compiler translates one source file at a time into an object file. The linker then combines all object files (and libraries) into a single executable.

### 4.2 Error Types

Errors in C++ programs are classified by **when** they are detected:

| Error Type | When Detected | Cause | Example |
|:-----------|:-------------|:------|:--------|
| **Compile Error** | Compilation | Syntax mistakes, type mismatches, undeclared identifiers | Missing `;`, wrong type conversion |
| **Link Error** | Linking | Missing function/variable definitions, unresolved symbols | Calling a function with no implementation |
| **Runtime Error** | Execution | Logic errors, illegal operations | Division by zero, accessing invalid memory |

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

> **Warning:** Compile errors are caught by the compiler and are the easiest to fix. Runtime errors are the hardest — the program compiles and links successfully but produces incorrect results or crashes during execution.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Software Importance | Software gives hardware purpose; modern life depends on programs |
| Programming Languages | Machine language (binary) → Assembly (mnemonics) → High-level (C, C++, Python) |
| Compiler | Translates entire source code to machine code before execution |
| C++ History | Bjarne Stroustrup, 1979/1983; C with Classes → C++; C++98 → C++11 → C++20 → C++23 → C++26 |
| C++ = C + OOP | Compatible with C; adds encapsulation, inheritance, polymorphism |
| Key Features | Templates, operator overloading, references, `new`/`delete`, inline functions, STL |
| Standard C++ | ANSI/ISO standard ensures portability; avoid compiler-specific extensions |
| C++ Libraries | `<cstring>` (C-inherited) vs `<iostream>` (C++-native); no `.h`; `std` namespace |
| Build Pipeline | Edit (.cpp) → Compile (.obj) → Link (.exe) → Execute |
| Compile Error | Syntax/type mistakes caught during compilation |
| Link Error | Missing definitions caught during linking |
| Runtime Error | Logic errors or illegal operations detected during execution |

---
