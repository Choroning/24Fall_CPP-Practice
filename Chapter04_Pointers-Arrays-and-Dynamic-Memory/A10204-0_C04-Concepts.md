# Chapter 04 — Pointers, Arrays, and Dynamic Memory

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpoom C++ Programming (Revised Edition)* — Chapter 4

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

### 2.2 Dynamic Objects

Objects can be created dynamically on the heap. The constructor is called by `new`, and the destructor is called by `delete`.

```cpp
Circle *p = new Circle(10);   // heap allocation + constructor Circle(10)
cout << p->getArea();          // access via arrow operator
delete p;                      // destructor called + memory freed
```

```
Stack                 Heap
┌──────────┐         ┌──────────────┐
│  p ───────┼────────►│  Circle      │
│ (pointer) │         │  radius = 10 │
└──────────┘         └──────────────┘
                      ↑ new allocates    delete frees ↑
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
Constructor (new)              Destructor (delete)
    │                               │
    ▼                               ▼
┌─ String obj ─┐    ──►    ┌─ String obj ─┐
│ buf ──► heap │           │ buf ──► freed │
│ length = 5   │           │ length = 5   │
└──────────────┘           └──────────────┘
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

---
