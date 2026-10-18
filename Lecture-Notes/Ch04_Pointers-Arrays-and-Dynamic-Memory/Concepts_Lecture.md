# Chapter 04 — Pointers, Arrays, and Dynamic Memory

> **Last Updated:** 2026-03-16

---

## Table of Contents

- [1. Object Pointers](#1-object-pointers)
  - [1.1 Pointer to Object](#11-pointer-to-object)
  - [1.2 this Pointer](#12-this-pointer)
- [2. Dynamic Memory](#2-dynamic-memory)
  - [2.1 new and delete](#21-new-and-delete)
  - [2.2 Dynamic Objects](#22-dynamic-objects)
  - [2.3 Dynamic Arrays](#23-dynamic-arrays)
- [3. Object Arrays](#3-object-arrays)
  - [3.1 Static Object Arrays](#31-static-object-arrays)
  - [3.2 Dynamic Object Arrays](#32-dynamic-object-arrays)
- [4. Memory Management](#4-memory-management)
  - [4.1 Memory Leak](#41-memory-leak)
  - [4.2 Dangling Pointer](#42-dangling-pointer)
  - [4.3 Destructor Responsibility](#43-destructor-responsibility)
- [5. Exception Handling](#5-exception-handling)
- [6. Smart Pointers (Modern C++)](#6-smart-pointers-modern-c)
  - [6.1 RAII Concept](#61-raii-concept)
  - [6.2 `std::unique_ptr` — Exclusive Ownership](#62-stdunique_ptr--exclusive-ownership)
  - [6.3 `std::shared_ptr` — Shared Ownership](#63-stdshared_ptr--shared-ownership)
  - [6.4 `make_unique` and `make_shared`](#64-make_unique-and-make_shared)
  - [6.5 When to Use Which](#65-when-to-use-which)
- [Summary](#summary)

---

<br>

## 1. Object Pointers

### 1.1 Pointer to Object

A pointer can hold the address of an object. Use the **arrow operator** (`->`) to access members through a pointer, or dereference with `*` and use the dot operator.

```cpp
Circle c(10);
Circle *p = &c;

p->getArea();      // arrow operator
(*p).getArea();    // dereference + dot operator (same result)
```

```
Stack
┌──────────┐      ┌──────────────┐
│  p ───────┼─────►│  Circle c    │
│ (pointer) │      │  radius = 10 │
└──────────┘      └──────────────┘
```

> **Key Point:** `p->member` is shorthand for `(*p).member`. The arrow operator is preferred for readability.

### 1.2 this Pointer

Every non-static member function has an implicit pointer called `this` that points to the **current object** (the object on which the function was called).

```cpp
class Circle {
    int radius;
public:
    Circle(int radius) {
        this->radius = radius;   // disambiguate: parameter vs member
    }
};
```

Common uses of `this`:

- **Disambiguate** when a parameter name matches a member variable name.
- **Return the current object** from a member function (`return *this;`).

```cpp
class Counter {
    int count;
public:
    Counter(int count) {
        this->count = count;       // this->count = member, count = parameter
    }
    Counter& increment() {
        count++;
        return *this;              // return current object
    }
};
```

> **Note:** `this` is a pointer, so use `this->member` (not `this.member`). The type of `this` inside a `ClassName` method is `ClassName*`.

---

<br>

## 2. Dynamic Memory

### 2.1 new and delete

C++ uses `new` to allocate memory on the **heap** and `delete` to deallocate it. Unlike stack-allocated variables, heap memory persists until explicitly freed.

```cpp
int *p = new int;        // allocate one int on the heap
*p = 42;
delete p;                // deallocate

int *arr = new int[100]; // allocate array of 100 ints
arr[0] = 1;
delete[] arr;            // deallocate array (note: delete[], not delete)
```

| C (malloc/free) | C++ (new/delete) |
|:----------------|:-----------------|
| `int *p = (int*)malloc(sizeof(int));` | `int *p = new int;` |
| `free(p);` | `delete p;` |
| Returns `void*`, needs cast | Returns correct type |
| Does NOT call constructor | Calls constructor |
| Does NOT call destructor | Calls destructor |

> **Key Point:** In C++, prefer `new`/`delete` over `malloc`/`free`. `new` calls the constructor, `delete` calls the destructor — `malloc` and `free` do neither.

> **Note (Modern C++):** Raw `new`/`delete` is considered **legacy style** in modern C++. Since C++11, **smart pointers** (`std::unique_ptr`, `std::shared_ptr`) are the preferred way to manage dynamic memory. They automatically free memory when it is no longer needed, eliminating entire categories of bugs (memory leaks, dangling pointers, double-free). See [Section 6: Smart Pointers (Modern C++)](#6-smart-pointers-modern-c) below for details.

### 2.2 Dynamic Objects

Objects can be created dynamically on the heap. The constructor is called by `new`, and the destructor is called by `delete`.

```cpp
Circle *p = new Circle(10);   // heap allocation + constructor Circle(10)
cout << p->getArea();          // access via arrow operator
delete p;                      // destructor called + memory freed
```

```
Stack                 Heap
┌───────────┐         ┌──────────────┐
│  p ───────┼────────►│ Circle       │
│ (pointer) │         │ radius = 10  │
└───────────┘         └──────────────┘
                      ↑ new allocates / delete frees
```

### 2.3 Dynamic Arrays

Use `new[]` to allocate arrays on the heap and `delete[]` to free them.

**Primitive type array:**

```cpp
int *arr = new int[5];     // 5 uninitialized ints
arr[0] = 10;
arr[1] = 20;
delete[] arr;              // free the entire array
```

**Object array:**

```cpp
Circle *circles = new Circle[3];   // default constructor called 3 times
circles[0].setRadius(1);
circles[1].setRadius(2);
circles[2].setRadius(3);
delete[] circles;                  // destructor called 3 times, then memory freed
```

> **Warning:** Always match `new` with `delete` and `new[]` with `delete[]`. Using `delete` on a `new[]` allocation causes **undefined behavior**.

---

<br>

## 3. Object Arrays

### 3.1 Static Object Arrays

Static (stack-allocated) object arrays call the **default constructor** for each element.

```cpp
Circle arr[3];    // default constructor called for each of the 3 elements
```

To call parameterized constructors, use an **initializer list**:

```cpp
Circle arr[3] = { Circle(1), Circle(2), Circle(3) };
```

Each element in the array is an independent object:

```cpp
arr[0].setRadius(10);   // only arr[0] is affected
cout << arr[1].getArea();
```

### 3.2 Dynamic Object Arrays

Dynamic object arrays are allocated with `new[]`. Only the **default constructor** can be called — you cannot pass arguments to individual elements during allocation.

```cpp
Circle *p = new Circle[4];    // default constructor called 4 times
p[0].setRadius(5);            // access with subscript operator
p[1].setRadius(10);
delete[] p;                   // destructor called 4 times
```

> **Key Point:** `new ClassName[n]` requires the class to have a **default constructor** (either explicitly defined or compiler-provided). If only a parameterized constructor exists, this will fail to compile.

---

<br>

## 4. Memory Management

### 4.1 Memory Leak

A **memory leak** occurs when dynamically allocated memory is never freed. This typically happens when the pointer to the allocation is lost.

```cpp
void leak() {
    int *p = new int[1000];
    // ... forgot to delete[] p
}   // p goes out of scope — memory is leaked!
```

```cpp
int *p = new int(10);
p = new int(20);        // ❌ original memory (10) is leaked — no pointer to it
delete p;               // only frees the second allocation
```

> **Warning:** Every `new` must have a matching `delete`. Lost pointers mean leaked memory that cannot be recovered until the program exits.

### 4.2 Dangling Pointer

A **dangling pointer** points to memory that has already been freed. Using it causes **undefined behavior**.

```cpp
int *p = new int(42);
delete p;             // memory freed
cout << *p;           // ❌ undefined behavior — dangling pointer!
```

**Prevention:** Set the pointer to `nullptr` after `delete`.

```cpp
int *p = new int(42);
delete p;
p = nullptr;          // safe — clearly indicates "no valid target"

if (p != nullptr) {
    cout << *p;       // this block is skipped
}
```

### 4.3 Destructor Responsibility

If a constructor allocates memory with `new`, the destructor **must** free it with `delete`. This prevents memory leaks when objects are destroyed.

```cpp
class String {
    char *buf;
    int length;
public:
    String(const char *s) {
        length = strlen(s);
        buf = new char[length + 1];    // allocate in constructor
        strcpy(buf, s);
    }
    ~String() {
        delete[] buf;                  // free in destructor
    }
};
```

```
Constructor (new)          Destructor (delete)
       │                           │
       ▼                           ▼
┌─ String obj ─┐    ──►    ┌─ String obj ──┐
│ buf ──► heap │           │ buf ──► freed │
│ length = 5   │           │ length = 5    │
└──────────────┘           └───────────────┘
```

> **Rule of thumb:** If your class uses `new` in any constructor, it needs `delete` in the destructor.

---

<br>

## 5. Exception Handling

C++ provides `try`-`catch` for handling runtime errors. This is useful for input validation and dynamic allocation failures.

```cpp
try {
    // code that might throw
    int *p = new int[1000000000];   // might fail
}
catch (bad_alloc &e) {
    cout << "Memory allocation failed: " << e.what() << endl;
}
```

**Basic syntax:**

```cpp
try {
    // risky code
    throw exception_value;    // explicitly throw an exception
}
catch (type1 e1) {
    // handle type1 exceptions
}
catch (type2 e2) {
    // handle type2 exceptions
}
```

**Example — input validation:**

```cpp
int divide(int a, int b) {
    if (b == 0)
        throw "Division by zero!";
    return a / b;
}

int main() {
    try {
        cout << divide(10, 0) << endl;
    }
    catch (const char *msg) {
        cout << "Error: " << msg << endl;   // "Error: Division by zero!"
    }
    return 0;
}
```

| Keyword | Purpose |
|:--------|:--------|
| `try` | Wrap code that might throw an exception |
| `throw` | Raise an exception (pass a value to the catch block) |
| `catch` | Handle a specific type of exception |

> **Note:** Exception handling is introduced briefly here. More advanced patterns (custom exception classes, exception safety guarantees) are covered in later chapters.

---

<br>

## 6. Smart Pointers (Modern C++)

Since C++11, the standard library provides **smart pointers** in `<memory>` that automate memory management. They follow the **RAII (Resource Acquisition Is Initialization)** principle: the resource (heap memory) is tied to the lifetime of an object on the stack. When the smart pointer goes out of scope, it automatically frees the managed memory.

### 6.1 RAII Concept

**RAII** means that resource cleanup is handled by the destructor of a stack-allocated object. This guarantees cleanup even when exceptions are thrown.

```
                Raw pointer (legacy)              Smart pointer (modern)
                ─────────────────────             ─────────────────────
Allocation:     int *p = new int(42);             auto p = make_unique<int>(42);
Usage:          *p = 100;                         *p = 100;
Deallocation:   delete p;  ← YOU must remember    // automatic when p goes out of scope
Risk:           forget delete → leak              no risk of leak
                use after delete → UB             ownership is clear
```

### 6.2 `std::unique_ptr` — Exclusive Ownership

A `unique_ptr` **owns** the object exclusively. It cannot be copied, only moved. When it goes out of scope, it deletes the managed object.

```cpp
#include <memory>
#include <iostream>
using namespace std;

int main() {
    // C++14: use make_unique (preferred)
    unique_ptr<int> p = make_unique<int>(42);
    cout << *p << endl;   // 42

    // unique_ptr cannot be copied
    // unique_ptr<int> q = p;          // ERROR: copy not allowed
    unique_ptr<int> q = move(p);       // OK: transfer ownership
    // p is now nullptr; q owns the int

    cout << *q << endl;   // 42
    // q goes out of scope here → memory is automatically freed
    return 0;
}
```

**With classes:**

```cpp
unique_ptr<Circle> c = make_unique<Circle>(10);
cout << c->getArea() << endl;    // use -> just like a raw pointer
// delete is called automatically when c goes out of scope
```

### 6.3 `std::shared_ptr` — Shared Ownership

A `shared_ptr` allows **multiple pointers** to share ownership of the same object. An internal **reference count** tracks how many `shared_ptr` instances point to the object. The object is deleted when the last `shared_ptr` is destroyed.

```cpp
#include <memory>
#include <iostream>
using namespace std;

int main() {
    // C++11: use make_shared (preferred)
    shared_ptr<int> a = make_shared<int>(100);
    cout << a.use_count() << endl;   // 1

    {
        shared_ptr<int> b = a;       // shared ownership
        cout << a.use_count() << endl;   // 2
        cout << *b << endl;              // 100
    }   // b goes out of scope → reference count drops to 1

    cout << a.use_count() << endl;   // 1
    // a goes out of scope → reference count drops to 0 → memory freed
    return 0;
}
```

### 6.4 `make_unique` and `make_shared`

| Factory Function | Standard | Purpose |
|:----------------|:---------|:--------|
| `std::make_shared<T>(args...)` | C++11 | Create a `shared_ptr<T>`; single allocation for object + control block |
| `std::make_unique<T>(args...)` | C++14 | Create a `unique_ptr<T>`; exception-safe alternative to `new` |

These factory functions are preferred over using `new` directly because they:
1. Prevent memory leaks in expressions with multiple allocations.
2. Are more concise and readable.
3. `make_shared` is more efficient (single memory allocation).

```cpp
// Legacy style (avoid)
unique_ptr<Circle> p(new Circle(10));

// Modern style (preferred)
auto p = make_unique<Circle>(10);     // C++14
auto q = make_shared<Circle>(20);     // C++11
```

### 6.5 When to Use Which

| Pointer Type | Use When |
|:------------|:---------|
| `unique_ptr` | Single, clear owner; the common default choice |
| `shared_ptr` | Multiple owners need to share the same resource |
| Raw pointer (non-owning) | Observing without ownership (e.g., function parameters that do not store the pointer) |
| Raw `new`/`delete` | Legacy code, or rare low-level scenarios (custom allocators, placement new) |

> **Rule of thumb for modern C++:** If you find yourself writing `new` or `delete`, ask whether a smart pointer would work instead. In most cases, it will.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Object pointer | `ClassName *p = &obj;` — access via `p->member()` or `(*p).member()` |
| `this` pointer | Implicit pointer to current object; `this->member` disambiguates names |
| `new` / `delete` | Heap allocation/deallocation; `new` calls constructor, `delete` calls destructor |
| Dynamic objects | `new ClassName(args)` creates on heap; `delete p` destroys |
| Dynamic arrays | `new Type[n]` + `delete[] arr`; never mix `delete` with `new[]` |
| Static object arrays | `ClassName arr[n];` — default constructor for each; initializer list for parameterized |
| Dynamic object arrays | `new ClassName[n]` — requires default constructor; access with `p[i]` |
| Memory leak | Forgetting `delete`; lost pointer = unreachable heap memory |
| Dangling pointer | Using pointer after `delete`; set to `nullptr` after freeing |
| Destructor responsibility | Constructor uses `new` → destructor must use `delete` |
| Exception handling | `try { } catch(type e) { }`; `throw` to raise; useful for allocation failures |
| RAII | Resource Acquisition Is Initialization; tie resource lifetime to object lifetime on the stack |
| `unique_ptr` | Exclusive ownership; cannot copy, only move; auto-deletes on scope exit; prefer `make_unique` |
| `shared_ptr` | Shared ownership via reference counting; auto-deletes when last owner is destroyed; prefer `make_shared` |
| Smart vs raw pointers | Smart pointers (C++11) replace raw `new`/`delete` in modern C++; raw `new`/`delete` is legacy style |

---
