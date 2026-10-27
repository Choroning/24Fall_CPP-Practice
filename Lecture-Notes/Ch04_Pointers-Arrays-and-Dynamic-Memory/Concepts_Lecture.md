# Chapter 4 — Pointers, Arrays, and Dynamic Memory

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **Learning Objectives:**
> 1. Declare object pointers and access members with the arrow operator
> 2. Declare and initialize object arrays and explain the order in which constructors and destructors run
> 3. Allocate and free memory, arrays, objects, and object arrays dynamically with `new` and `delete`
> 4. Explain the concept of the `this` pointer, when it is needed, and its restrictions
> 5. Avoid memory leaks and dangling pointers and manage resources with smart pointers
> 6. Create, read, search, and modify strings with the `string` class

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
  - [3.2 Two-Dimensional Object Arrays](#32-two-dimensional-object-arrays)
  - [3.3 Dynamic Object Arrays](#33-dynamic-object-arrays)
- [4. Memory Management](#4-memory-management)
  - [4.1 Memory Leak](#41-memory-leak)
  - [4.2 Dangling Pointer](#42-dangling-pointer)
  - [4.3 Destructor Responsibility](#43-destructor-responsibility)
- [5. Exception Handling](#5-exception-handling)
- [6. Smart Pointers (Modern C++)](#6-smart-pointers-modern-c)
  - [6.1 RAII Concept](#61-raii-concept)
  - [6.2 `std::unique_ptr`: Exclusive Ownership](#62-stdunique_ptr-exclusive-ownership)
  - [6.3 `std::shared_ptr`: Shared Ownership](#63-stdshared_ptr-shared-ownership)
  - [6.4 `make_unique` and `make_shared`](#64-make_unique-and-make_shared)
  - [6.5 When to Use Which](#65-when-to-use-which)
- [7. The string Class](#7-the-string-class)
  - [7.1 Creating string Objects and Input/Output](#71-creating-string-objects-and-inputoutput)
  - [7.2 Main Member Functions and Operators](#72-main-member-functions-and-operators)
  - [7.3 Dynamic string Objects and String Processing](#73-dynamic-string-objects-and-string-processing)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

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

```text
Stack
┌──────────┐      ┌──────────────┐
│  p ───────┼─────►│  Circle c    │
│ (pointer) │      │  radius = 10 │
└──────────┘      └──────────────┘
```

*Figure 1. Pointer p pointing to object c*

An object pointer, like a pointer in C, is a variable whose value is an address, so a pointer that has been declared but not assigned an address must not be used to access members. If the same pointer is assigned the address of another object (`p = &pizza;`), later calls such as `p->getArea()` call that object's member function.

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

- **Disambiguation:** When a parameter name matches a member variable name, `this` tells them apart. Without `this`, `radius = radius;` refers to the parameter on both sides, so the member variable does not change.
- **Returning the current object:** A member function returns the object itself with `return *this;` or its address with `return this;`. This pattern is common in operator overloading in Chapter 7.

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

> **Note:** `this` is a pointer, so use `this->member` (not `this.member`). The type of `this` inside a `ClassName` method is `ClassName*` (`const ClassName*` inside a const member function).

`this` is not a variable the programmer declares; it is a parameter the compiler implicitly inserts into each member function. Its value therefore differs for each object. Calling `c1.setRadius(4)` makes `this` inside `setRadius()` point to `c1`, and calling `c2.setRadius(5)` makes it point to `c2`. Conceptually, the compiler transforms a member function and its call as follows.

```cpp
// Code written by the programmer
class Sample {
    int a;
public:
    void setA(int x) { this->a = x; }
};
Sample ob;
ob.setA(5);

// Form after the compiler transforms it (conceptual)
void setA(Sample* this, int x) { this->a = x; }
setA(&ob, 5);    // the address of ob is passed as the this parameter
```

`this` has the following restrictions.

- It cannot be used in a function that is not a member function (such as a global function), because such a function is not tied to any object.
- It cannot be used in a static member function, because a static member function can be called without an object, so there may be no object to point to (static members are covered in Chapter 6).

---

<br>

## 2. Dynamic Memory

### 2.1 new and delete

**Static allocation** obtains memory of a predetermined size by declaring variables or arrays. When the required amount cannot be predicted while writing the program, memory must be obtained in the needed amount during execution; this is called **dynamic allocation**. Dynamically allocated memory comes from the heap area of the process. C uses the library functions `malloc()`/`free()`, and C++ uses the built-in operators `new` and `delete`.

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
| Returns `void*`, needs a cast in C++ | Returns correct type |
| Does NOT call constructor | Calls constructor |
| Does NOT call destructor | Calls destructor |

*Table 1. C malloc/free compared with C++ new/delete*

> **Key Point:** In C++, prefer `new`/`delete` over `malloc`/`free`. `new` calls the constructor and `delete` calls the destructor, while `malloc` and `free` do neither.

> **Note:** **Modern C++.** Raw `new`/`delete` is considered **legacy style** in modern C++. Since C++11, **smart pointers** (`std::unique_ptr`, `std::shared_ptr`) are the preferred way to manage dynamic memory. They automatically free memory when it is no longer needed, eliminating entire categories of bugs (memory leaks, dangling pointers, double-free). See [Section 6](#6-smart-pointers-modern-c) for details.

Memory can be initialized with a value in parentheses at the time of allocation.

```cpp
int *pInt = new int(20);        // allocate an int initialized to 20
char *pChar = new char('a');    // allocate a char initialized to 'a'
```

Only an address obtained from `new` may be passed to `delete`, and only once. The following two cases are undefined behavior and usually show up as run-time errors.

```cpp
int n;
int *p = &n;
delete p;      // error: the memory p points to was not dynamically allocated

int *q = new int;
delete q;      // normal deallocation
delete q;      // error: memory that was already freed cannot be freed again
```

> **[Systems Programming]** In systems programming, the address space of a process is divided into code, global data, heap, and stack regions, and the blocks of the heap are managed by a dynamic memory allocator. `new` usually obtains a block from this allocator (the `malloc()` implementation of the C runtime) and `delete` returns the block; the allocator keeps bookkeeping information such as the size and in-use state of each block and hands out returned blocks again, so passing an address that was never allocated or one that was already returned, as in the code above, can corrupt that bookkeeping information.

### 2.2 Dynamic Objects

Objects can be created dynamically on the heap. The constructor is called by `new`, and the destructor is called by `delete`.

```cpp
Circle *p = new Circle(10);   // heap allocation + constructor Circle(10)
cout << p->getArea();          // access via arrow operator
delete p;                      // destructor called + memory freed
```

```text
Stack                 Heap
┌───────────┐         ┌──────────────┐
│  p ───────┼────────►│ Circle       │
│ (pointer) │         │ radius = 10  │
└───────────┘         └──────────────┘
                      ↑ new allocates / delete frees
```

*Figure 2. Pointer p on the stack and a Circle object on the heap*

`new Circle` without arguments calls the default constructor, and `new Circle(30)` with an argument calls the parameterized constructor. When several objects are created dynamically, they can be deleted in any order, regardless of the order of creation. In the following example, which creates an object inside a loop, uses it, and deletes it right away, omitting `delete p;` leaks memory on every iteration (Section 4.1).

```cpp
int radius;
while (true) {
    cout << "정수 반지름 입력(음수이면 종료)>> ";
    cin >> radius;
    if (radius < 0) break;              // stop when a negative number is entered
    Circle *p = new Circle(radius);     // create a dynamic object
    cout << "원의 면적은 " << p->getArea() << endl;
    delete p;                           // free the object
}
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

The array size can be decided at run time. For example, after reading a count with `cin >> n;`, `int *p = new int[n];` allocates an array of exactly that size.

**Object array:**

```cpp
Circle *circles = new Circle[3];   // default constructor called 3 times
circles[0].setRadius(1);
circles[1].setRadius(2);
circles[2].setRadius(3);
delete[] circles;                  // destructor called 3 times, then memory freed
```

> **Caution:** Always match `new` with `delete` and `new[]` with `delete[]`. Using `delete` on a `new[]` allocation causes **undefined behavior**.

There is no syntax such as `new int[10](20)` that initializes every element of a new array to one value. This expression and `new int(20)[10]` are compile errors through C++17 (from C++20, `new int[10](20)` is allowed, but only the first element becomes 20 and the rest become 0). Since C++11, initial values for each element can be given in braces.

```cpp
int *a = new int[3]{1, 2, 3};   // C++11: initialized to 1, 2, 3
int *b = new int[5]{};          // all elements initialized to 0
delete[] a;
delete[] b;
```

---

<br>

## 3. Object Arrays

### 3.1 Static Object Arrays

Static (stack-allocated) object arrays call the **default constructor** for each element.

```cpp
Circle arr[3];    // default constructor called for each of the 3 elements
```

Declaring an object array allocates space for the whole array, then runs the constructor of each element in the order `arr[0]`, `arr[1]`, `arr[2]`. When the array is destroyed, the destructors run in the reverse order of creation: `arr[2]`, `arr[1]`, `arr[0]`. A declaration that gives every element the same argument, such as `Circle circleArray[3](5);`, is a compile error through C++17.

To call parameterized constructors, use a **brace-enclosed initializer list** that names a constructor for each element:

```cpp
Circle arr[3] = { Circle(1), Circle(2), Circle(3) };
```

Writing `Circle()` for some elements, as in `Circle circleArray[3] = { Circle(10), Circle(20), Circle() };`, creates those elements with the default constructor.

Each element in the array is an independent object:

```cpp
arr[0].setRadius(10);   // only arr[0] is affected
cout << arr[1].getArea();
```

Because an array name is the address of its first element, the array can be assigned to an object pointer with `Circle *p = arr;` and its elements visited in turn by repeating `p->getArea()` and `p++`.

### 3.2 Two-Dimensional Object Arrays

Object arrays can also be two-dimensional. `Circle circles[2][3];` creates six `Circle` objects in 2 rows and 3 columns and calls the default constructor for each element.

```cpp
Circle circles[2][3];          // default constructor called 6 times
circles[0][0].setRadius(1);
circles[0][1].setRadius(2);
circles[0][2].setRadius(3);
circles[1][0].setRadius(4);
circles[1][1].setRadius(5);
circles[1][2].setRadius(6);

for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++)
        cout << "Circle [" << i << "," << j << "]의 면적은 " << circles[i][j].getArea() << endl;
```

```text
           Column 0         Column 1         Column 2
Row 0  circles[0][0]    circles[0][1]    circles[0][2]
       radius = 1       radius = 2       radius = 3
Row 1  circles[1][0]    circles[1][1]    circles[1][2]
       radius = 4       radius = 5       radius = 6
```

*Figure 3. Elements and radii of a 2-by-3 Circle array*

The output is the areas 3.14, 12.56, 28.26, 50.24, 78.5, and 113.04 for radii 1-6. The array can also be initialized in its declaration by grouping each row in braces.

```cpp
Circle circles[2][3] = { { Circle(1), Circle(2), Circle(3) },
                         { Circle(4), Circle(5), Circle(6) } };
```

### 3.3 Dynamic Object Arrays

Dynamic object arrays are allocated with `new[]`. Allocating with `new Circle[n]` and no initializer list calls the **default constructor** for each element, and this form cannot pass arguments to individual elements. Since C++11, a brace-enclosed initializer list such as `new Circle[3]{ Circle(1), Circle(2), Circle(3) }` can name a constructor for each element.

```cpp
Circle *p = new Circle[4];    // default constructor called 4 times
p[0].setRadius(5);            // access with subscript operator
p[1].setRadius(10);
delete[] p;                   // destructor called 4 times
```

> **Key Point:** `new ClassName[n]` without an initializer list requires the class to have a **default constructor** (either explicitly defined or compiler-provided). If only a parameterized constructor exists, this will fail to compile.

The elements of a dynamic array can also be accessed with pointer arithmetic instead of subscripts (`p[i]`). `p->setRadius(10)` is a call on `p[0]`, and `(p+1)->setRadius(20)` is a call on `p[1]`. Executing `delete[] p;` runs the destructor of each element in the reverse order of creation (`p[3]`, `p[2]`, `p[1]`, `p[0]`), as with static arrays, and then frees the memory.

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

> **Caution:** Every `new` must have a matching `delete`. Lost pointers mean leaked memory that cannot be recovered until the program exits.

When a program exits, the operating system reclaims all memory of that process, so leaked memory is also reclaimed at that point. In a long-running program such as a server, however, leaks accumulate and reduce the available memory.

### 4.2 Dangling Pointer

A **dangling pointer** points to memory that has already been freed. Using it causes **undefined behavior**.

```cpp
int *p = new int(42);
delete p;             // memory freed
cout << *p;           // ❌ undefined behavior — dangling pointer!
```

**Prevention:** Set the pointer to `nullptr` after `delete`. Other pointers that pointed to the same memory still dangle, however, so this protects only the pointer that was set to `nullptr`.

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

```text
Constructor (new)          Destructor (delete)
       │                           │
       ▼                           ▼
┌─ String obj ─┐    ──►    ┌─ String obj ──┐
│ buf ──► heap │           │ buf ──► freed │
│ length = 5   │           │ length = 5    │
└──────────────┘           └───────────────┘
```

*Figure 4. String object buffer allocated in the constructor and freed in the destructor*

> **Note:** **Rule of thumb.** If your class uses `new` in any constructor, it needs `delete` in the destructor.

---

<br>

## 5. Exception Handling

C++ provides `try`-`catch` for handling exceptional situations that arise at run time. This is useful for input validation and dynamic allocation failures.

```cpp
try {
    // code that might throw
    int *p = new int[1000000000];   // might fail
}
catch (bad_alloc &e) {
    cout << "Memory allocation failed: " << e.what() << endl;
}
```

> **Caution:** In standard C++, `new` does not return `nullptr` when allocation fails; it throws a `bad_alloc` exception. Code that checks the return value, such as `int *p = new int[n]; if (!p) { ... }`, therefore does not catch the failure. To detect failure through the return value, use `new (nothrow) int[n]` from the `<new>` header; only this form returns `nullptr` on failure.

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

**Input validation example:**

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

*Table 2. Exception handling keywords and their roles*

> **Note:** Exception handling is introduced briefly here. More advanced patterns (custom exception classes, exception safety guarantees) are beyond the scope of this chapter; the textbook covers exception handling in detail in Chapter 13.

---

<br>

## 6. Smart Pointers (Modern C++)

Since C++11, the standard library provides **smart pointers** in `<memory>` that automate memory management. They follow the **RAII (Resource Acquisition Is Initialization)** principle: the resource (heap memory) is tied to the lifetime of an object on the stack. When the smart pointer goes out of scope, it automatically frees the managed memory.

### 6.1 RAII Concept

**RAII** is the technique of acquiring a resource in an object's constructor and releasing it in the destructor, which ties the resource's lifetime to the object's lifetime. When that object is a local object allocated on the stack, its destructor runs automatically when it goes out of scope, so cleanup is guaranteed even when exceptions are thrown.

```text
                Raw pointer (legacy)              Smart pointer (modern)
                ─────────────────────             ─────────────────────
Allocation:     int *p = new int(42);             auto p = make_unique<int>(42);
Usage:          *p = 100;                         *p = 100;
Deallocation:   delete p;  ← YOU must remember    // automatic when p goes out of scope
Risk:           forget delete → leak              no risk of leak
                use after delete → UB             ownership is clear
```

*Figure 5. Resource management with raw pointers and smart pointers*

### 6.2 `std::unique_ptr`: Exclusive Ownership

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

### 6.3 `std::shared_ptr`: Shared Ownership

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

*Table 3. Factory functions that create smart pointers*

These factory functions are preferred over using `new` directly because they:
1. Prevent memory leaks in expressions with multiple allocations (this mattered most before C++17 changed the evaluation-order rules for function arguments).
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

*Table 4. When to use each kind of pointer*

> **Note:** **Rule of thumb for modern C++.** Before writing `new` or `delete`, check whether a smart pointer can be used instead. In most cases, it can.

---

<br>

## 7. The string Class

C++ handles strings in two ways. One is the **C-string**, a `char` array terminated by `'\0'` as in C; the other is an object of the **`string` class** from the C++ standard library. The `string` class is declared in the `<string>` header; it stores a variable-size string that grows as needed and provides operations such as copying, comparison, and length calculation through member functions and operators.

```cpp
#include <string>
using namespace std;

string str = "I love ";   // 7 characters: 'I', ' ', 'l', 'o', 'v', 'e', ' '
str.append("C++.");       // str becomes "I love C++.", 11 characters
```

### 7.1 Creating string Objects and Input/Output

| Constructor | Description |
|:------------|:------------|
| `string()` | Creates an object holding an empty string |
| `string(const string& str)` | Creates a new object that copies the object `str` |
| `string(const char* s)` | Creates an object by copying the C-string `s` |
| `string(const char* s, int n)` | Creates an object by copying the first `n` characters of the character array `s` |

*Table 5. Main constructors of the string class*

```cpp
string str;                            // empty string
string address("서울시 성북구 삼선동 389");  // initialized from a string literal
string copyAddress(address);           // copy of address
char text[] = {'L', 'o', 'v', 'e', ' ', 'C', '+', '+', '\0'};  // C-string
string title(text);                    // "Love C++"

cout << str << endl;                   // empty string, so only an empty line
cout << address << endl;               // 서울시 성북구 삼선동 389
cout << title << endl;                 // Love C++
```

A `string` object is printed with `cout <<` and read with `cin >>`. Because `cin >>` stops reading at a whitespace character, entering `Han Hong Jin` stores only `Han`. To read a whole line including spaces, use `getline(cin, name, '\n')` from the `<string>` header. The third argument is the character that ends the input; if omitted, it is `'\n'`.

```cpp
string names[5];
for (int i = 0; i < 5; i++) {
    cout << "이름 >> ";
    getline(cin, names[i], '\n');      // read a whole line including spaces
}
string latter = names[0];
for (int i = 1; i < 5; i++)
    if (latter < names[i])             // compare in dictionary order
        latter = names[i];
cout << "사전에서 가장 뒤에 나오는 문자열은 " << latter << endl;
```

### 7.2 Main Member Functions and Operators

| Member Function | Example | Result |
|:----------------|:--------|:-------|
| `append(str)`, `append(str, pos, n)` | `string s("I"); s.append(" love "); s.append("really you", 7, 3);` | `"I love you"` |
| `insert(pos, str)` | `string s("I love C++"); s.insert(2, "really ");` | `"I really love C++"` |
| `replace(pos, n, str)` | `string s("I really love C++"); s.replace(2, 11, "study");` | `"I study C++"` |
| `size()`, `length()`, `capacity()` | `string s("I study C++"); s.length();` | 11 (`capacity()` depends on the implementation) |
| `erase(pos, n)`, `clear()` | `string s("I study C++"); s.erase(0, 7);` then `s.clear();` | `" C++"`, then `""` |
| `substr(pos, n)`, `substr(pos)` | `string s = "I love C++"; s.substr(2, 4); s.substr(2);` | `"love"`, `"love C++"` |
| `find(str)`, `find(str, pos)` | `string s = "I love love C++"; s.find("love"); s.find("love", 3); s.find("C#");` | 2, 7, `string::npos` |
| `rfind(str)` | `string s = "I love love C++"; s.rfind("love");` | 7 |
| `at(pos)` | `string s("I love C++"); s.at(7);` | `'C'` |

*Table 6. Main member functions of the string class*

`find()` returns the index where the searched string first appears and returns `string::npos` if it is not found. Because the return type of `find()` is an unsigned integer (`size_t`), the correct failure test is `s.find(x) == string::npos`. Storing the result in an `int` converts `npos` to -1, so code that tests `index == -1` also commonly works.

| Operator | Example | Operator | Example |
|:---------|:--------|:---------|:--------|
| `=` | `s1 = s2;` | `==` | `s1 == s2` |
| `[]` | `char c = s[1];` | `!=` | `s1 != s2` |
| `+` | `s = s1 + s2;` | `<` | `s1 < s2` |
| `+=` | `s1 += s2;` | `>` | `s1 > s2` |
| `<<` | `cout << s1;` | `<=` | `s1 <= s2` |
| `>>` | `cin >> s1;` | `>=` | `s1 >= s2` |

*Table 7. Operators of the string class*

The comparison operators compare strings in dictionary order. `[]` does not check the range, but `at()` throws an `out_of_range` exception for an index outside the string.

### 7.3 Dynamic string Objects and String Processing

A `string` object can also be created and freed dynamically with `new` and `delete`.

```cpp
string *p = new string("C++");   // create a string object dynamically
cout << *p;                      // prints "C++"
p->append(" Great!!");           // the string p points to becomes "C++ Great!!"
cout << *p;                      // prints "C++ Great!!"
delete p;                        // free the string object
```

The next example reads a string that may contain spaces and prints it while rotating it left by one character at a time. It splits off the first character with `substr(0, 1)` and the rest with `substr(1, len-1)`, then joins them with `+`.

```cpp
string s;
getline(cin, s, '\n');                    // e.g., I love you
int len = s.length();
for (int i = 0; i < len; i++) {
    string first = s.substr(0, 1);        // first character
    string sub = s.substr(1, len - 1);    // remaining characters
    s = sub + first;                      // join the two strings
    cout << s << endl;                    // " love youI", "love youI ", ...
}
```

After `len` rotations the string returns to its original form, so the last line is `I love you`, the same as the input. In the same way, combining `find()` and `substr()` can cut an addition expression such as `66+2+8+55+100` at each `+`, convert each part to an integer with `stoi()`, and compute the sum (231), and repeating `find()` and `replace()` can replace every occurrence of a word in a string with another word.

---

<br>

## Summary

| Concept | Key Summary |
|:--------|:------------|
| Object pointer | An object pointer is declared as `ClassName *p = &obj;` and accesses members via `p->member()` or `(*p).member()`. |
| `this` pointer | `this` is an implicit pointer to the current object, and `this->member` resolves name clashes between parameters and members. |
| Restrictions on `this` | `this` exists only in non-static member functions and cannot be used in global functions or static member functions. |
| `new` / `delete` | They allocate and free heap memory, and `new` calls the constructor while `delete` calls the destructor. |
| Dynamic objects | `new ClassName(args)` creates an object on the heap, and `delete p` destroys it. |
| Dynamic arrays | An array allocated with `new Type[n]` is freed with `delete[] arr`, and `delete` must never be mixed with `new[]`. |
| Static object arrays | `ClassName arr[n];` calls the default constructor for each element, and a brace-enclosed initializer list can name a constructor for each element. |
| Order of creation and destruction | Elements of an object array are created in index order and destroyed in the reverse order. |
| Two-dimensional object arrays | `ClassName arr[r][c];` declares the array, `arr[i][j]` accesses an element, and each row can be initialized in its own braces. |
| Dynamic object arrays | `new ClassName[n]` without an initializer list requires a default constructor, and elements are accessed with `p[i]`. |
| Memory leak | Forgetting `delete` or losing the pointer leaves heap memory that cannot be reclaimed before the program exits. |
| Dangling pointer | Using a pointer to freed memory is undefined behavior, so the pointer should be set to `nullptr` after freeing. |
| Destructor responsibility | If a constructor uses `new`, the destructor must free the memory with `delete`. |
| Exception handling | `throw` raises an exception and `try { } catch (type e) { }` handles it, and standard `new` throws `bad_alloc` on failure. |
| RAII | Resource Acquisition Is Initialization ties a resource's lifetime to an object's lifetime so that the destructor releases it automatically. |
| `unique_ptr` | It expresses exclusive ownership, cannot be copied but can be moved, deletes the object automatically on scope exit, and is best created with `make_unique`. |
| `shared_ptr` | It shares ownership through reference counting, deletes the object when the last owner is destroyed, and is best created with `make_shared`. |
| Smart vs raw pointers | In modern C++, smart pointers (C++11) replace raw `new`/`delete`, which is considered legacy style. |
| `string` class | The variable-size string class in `<string>` reads whole lines with `getline()` and provides member functions such as `find()`, `substr()`, and `replace()` and operators such as `+`, `==`, and `<`. |

---

<br>

## Self-Check Questions

1. **Freeing an array:** What goes wrong if an array allocated with `Circle *p = new Circle[3];` is freed with `delete p;`?

   <details>
   <summary>Answer</summary>

   Freeing memory allocated with `new[]` by `delete` is undefined behavior. There is no guarantee that the destructors of all three elements run, and the memory may not be returned correctly. It must be freed with `delete[] p;`, which runs the destructors in the order `p[2]`, `p[1]`, `p[0]` and then returns the memory.

   </details>

2. **Default constructor:** If the `Circle` class declares only the constructor `Circle(int r)`, do `Circle arr[3];` and `new Circle[3]` compile? If not, give two ways to fix it.

   <details>
   <summary>Answer</summary>

   Both are compile errors. Once any constructor is declared, the compiler no longer generates a default constructor, so there is no default constructor to create each element. The fixes are (1) to add a default constructor such as `Circle() { radius = 1; }`, or (2) to name a constructor for each element, as in `Circle arr[3] = { Circle(1), Circle(2), Circle(3) };` or, in C++11, `new Circle[3]{ Circle(1), Circle(2), Circle(3) }`.

   </details>

3. **Order of creation and destruction:** With a `Circle` class whose constructor and destructor print the radius, in what order do the constructors and destructors run when the block `{ Circle c[3] = { Circle(7), Circle(8), Circle(9) }; }` executes?

   <details>
   <summary>Answer</summary>

   The constructors run in the order `c[0]`, `c[1]`, `c[2]` for radii 7, 8, 9, and when the block ends the destructors run in the reverse order, for radii 9, 8, 7.

   </details>

4. **Restriction on this:** Why can `this` not be used inside a static member function?

   <details>
   <summary>Answer</summary>

   `this` is an implicit parameter that receives the address of the object on which the member function was called. A static member function can be called without an object, as in `ClassName::f()`, so there is no object address to pass, and therefore `this` does not exist.

   </details>

5. **string member functions:** For `string s = "I love love C++";`, give the results of `s.find("love")`, `s.find("love", 3)`, `s.rfind("love")`, and `s.substr(2, 4)`.

   <details>
   <summary>Answer</summary>

   `s.find("love")` is 2, the first position; `s.find("love", 3)` searches from index 3 and returns 7, the position of the second `love`; `s.rfind("love")` is 7, the last position; and `s.substr(2, 4)` is `"love"`, the four characters starting at index 2.

   </details>
