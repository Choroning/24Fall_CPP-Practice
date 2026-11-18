# Chapter 6 — Overloading and Static Members

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-11-19
>
> 명품 C++ Programming, 황기태, Ch 6

> **Learning Objectives:**
> 1. Explain the conditions under which function overloading holds and write overloaded functions
> 2. Declare default parameters and apply the rule by which omitted arguments are filled in
> 3. Identify overloading ambiguities caused by type conversion, reference parameters, and default parameters
> 4. Explain the difference between static and non-static members and declare and access static members
> 5. Understand the restrictions on static member functions and encapsulate global functions and shared variables in a class

---

## Table of Contents

- [1. Function Overloading](#1-function-overloading)
  - [1.1 Concept](#11-concept)
  - [1.2 Resolution Rules](#12-resolution-rules)
  - [1.3 Examples](#13-examples)
  - [1.4 Constructor Overloading and Destructors](#14-constructor-overloading-and-destructors)
  - [1.5 Tracing Overload Resolution Step by Step](#15-tracing-overload-resolution-step-by-step)
  - [1.6 How Overloaded Functions Are Told Apart: Name Mangling](#16-how-overloaded-functions-are-told-apart-name-mangling)
  - [1.7 Declarations That Function Overloading Does Not Distinguish](#17-declarations-that-function-overloading-does-not-distinguish)
- [2. Default Parameters](#2-default-parameters)
  - [2.1 Syntax](#21-syntax)
  - [2.2 Declaration vs Definition](#22-declaration-vs-definition)
  - [2.3 Overloading Ambiguity](#23-overloading-ambiguity)
  - [2.4 Simplifying Overloads with Default Parameters](#24-simplifying-overloads-with-default-parameters)
  - [2.5 When Default Arguments Are Evaluated](#25-when-default-arguments-are-evaluated)
  - [2.6 Rules for Specifying Default Arguments](#26-rules-for-specifying-default-arguments)
- [3. Static Members](#3-static-members)
  - [3.1 Static Member Variables](#31-static-member-variables)
  - [3.2 Accessing Static Members](#32-accessing-static-members)
  - [3.3 Static Member Functions](#33-static-member-functions)
  - [3.4 Use Cases](#34-use-cases)
  - [3.5 Three Uses of static](#35-three-uses-of-static)
  - [3.6 Initialization and Lifetime of static Members](#36-initialization-and-lifetime-of-static-members)
  - [3.7 A Pitfall in Counting Objects with a static Member](#37-a-pitfall-in-counting-objects-with-a-static-member)
- [4. Common Mistakes](#4-common-mistakes)
  - [4.1 Initializing a static Member Variable in a Constructor's Initializer List](#41-initializing-a-static-member-variable-in-a-constructors-initializer-list)
  - [4.2 Declaring a static Member Function const](#42-declaring-a-static-member-function-const)
  - [4.3 Defining a static Member Variable in a Header File](#43-defining-a-static-member-variable-in-a-header-file)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Function Overloading

### 1.1 Concept

**Function overloading** allows multiple functions with the **same name** but **different parameter lists** to coexist. The compiler selects the correct function based on the arguments at the call site.

The textbook calls this **function duplication** (함수 중복). Because one name behaves in several forms depending on the arguments, it is a form of polymorphism; in C, two functions with the same name cannot be defined. Overloading is possible among ordinary functions and among the member functions of one class.

> **Note:** **Inheritance and name hiding.** The textbook states that function duplication is also possible between member functions of a base class and a derived class. However, declaring even one member function with the same name in the derived class hides all base class functions of that name (name hiding), so when called through a derived class object, the functions of the two classes do not become overload candidates together. To make them candidates together, bring the base class name in with `using Base::f;` inside the derived class. Inheritance is covered in Chapter 8.

```cpp
#include <iostream>
using namespace std;

int sum(int a, int b) {
    return a + b;
}

double sum(double a, double b) {
    return a + b;
}

int sum(int a, int b, int c) {
    return a + b + c;
}

int main() {
    cout << sum(1, 2) << endl;        // calls int sum(int, int) → 3
    cout << sum(1.5, 2.5) << endl;    // calls double sum(double, double) → 4
    cout << sum(1, 2, 3) << endl;     // calls int sum(int, int, int) → 6
    return 0;
}
```

With overloading, functions that do similar work need not be given and remembered as separate names such as `msg1()`, `msg2(name)`, `msg3(id, name)`; they can all be called with one name as `msg()`, `msg(name)`, `msg(id, name)`, which reduces calling mistakes.

> **Key Point:** The **return type alone** does NOT distinguish overloads. Two functions with the same name and same parameter list but different return types cause a compilation error.

```cpp
int    compute(int x);    // OK
double compute(int x);    // ERROR: same parameter list as above
```

**What makes overloads distinct:**

| Distinguishes overloads | Does NOT distinguish |
|:------------------------|:---------------------|
| Number of parameters | Return type |
| Type of parameters | Parameter names |
| Order of parameter types | |

*Table 1. What does and does not distinguish overloads*

### 1.2 Resolution Rules

When an overloaded function is called, the compiler follows a priority order to find the best match:

| Priority | Match Type | Description |
|:---------|:-----------|:------------|
| 1 | **Exact match** | Argument type matches parameter type exactly |
| 2 | **Promotion** | `char`/`short` to `int`, `float` to `double` |
| 3 | **Standard conversion** | `int` to `double`, `double` to `int`, etc. |

*Table 2. Priority of match types in overload resolution*

Below these rank user-defined conversions (through constructors or conversion operators) and ellipsis (`...`) matches. If multiple functions match at the **same priority level**, the call is **ambiguous** and the compiler reports an error.

```cpp
void print(int x)    { cout << "int: " << x << endl; }
void print(double x) { cout << "double: " << x << endl; }
void print(char x)   { cout << "char: " << x << endl; }

print(10);       // exact match → print(int)
print(3.14);     // exact match → print(double)
print('A');      // exact match → print(char)
print(3.14f);    // promotion: float → double → print(double)
```

**Ambiguity example:**

```cpp
void f(float x)  { cout << "float" << endl; }
void f(double x) { cout << "double" << endl; }

f(10);   // ERROR: int → float and int → double are both standard conversions
         // ambiguous: compiler cannot choose
```

On the other hand, passing a `double` value as in `f(10.0)` is not ambiguous, because `f(double)` is an exact match.

**Ambiguity caused by reference parameters:**

```cpp
int add(int a, int b)  { return a + b; }
int add(int a, int &b) { b = b + a; return b; }

int s = 10, t = 20;
add(s, t);     // ERROR: copying by value and binding by reference are both exact matches
add(10, 20);   // OK: the literal 20 cannot bind to int&, so only add(int, int) is a candidate
```

When the variable `t` is passed, copying it into `int b` and binding it to `int &b` are both exact matches, so the compiler cannot choose. Therefore two functions whose parameters differ only in being a value or a reference practically cannot be overloaded together. Overloading ambiguity thus arises from type conversion, reference parameters, and default parameters (Section 2.3).

### 1.3 Examples

**Absolute value:**

```cpp
int abs(int x) {
    return (x >= 0) ? x : -x;
}

double abs(double x) {
    return (x >= 0) ? x : -x;
}

int main() {
    cout << abs(-5) << endl;      // 5 (int version)
    cout << abs(-3.14) << endl;   // 3.14 (double version)
    return 0;
}
```

> **Caution:** The standard library also has `abs()`, so writing this example as is at global scope together with `<iostream>` and `using namespace std;` can, depending on the implementation, conflict with the already declared `abs()` and fail to compile. For example, with libc++ in Apple clang, the definition of `double abs(double)` produces a conflict error. In real code, use another name such as `myAbs()` or wrap the functions as static members like the `Math` class in Section 3.3.

**Largest number in an array:**

```cpp
#include <iostream>
using namespace std;

int big(int a, int b) {             // return the larger of a and b
    if (a > b) return a;
    else return b;
}

int big(int a[], int size) {        // return the largest number in array a
    int res = a[0];
    for (int i = 1; i < size; i++)
        if (res < a[i]) res = a[i];
    return res;
}

int main() {
    int array[5] = {1, 9, -2, 8, 6};
    cout << big(2, 3) << endl;        // 3
    cout << big(array, 5) << endl;    // 9
    return 0;
}
```

The two `big()` functions differ in parameter types and meaning, so overloading holds. `big(2, 3)` calls the version that takes two integers, and `big(array, 5)` calls the version that takes an array pointer and a size.

**Printing different types:**

```cpp
#include <iostream>
#include <string>
using namespace std;

void print(int x)          { cout << "int: " << x << endl; }
void print(string s)       { cout << "string: " << s << endl; }
void print(int x, int y)   { cout << "two ints: " << x << ", " << y << endl; }

int main() {
    print(42);             // int: 42
    print("hello");        // string: hello
    print(3, 7);           // two ints: 3, 7
    return 0;
}
```

### 1.4 Constructor Overloading and Destructors

Constructors can also be overloaded. The purpose is to let various forms of initial values be passed through parameters when an object is created.

```cpp
class Circle {
    int radius;
public:
    Circle() { radius = 1; }          // constructor without parameters
    Circle(int r) { radius = r; }     // constructor that takes a radius
};

Circle donut;        // calls Circle()
Circle pizza(30);    // calls Circle(int r)
```

The standard library `string` class also overloads its constructors as `string()`, `string(const string&)`, `string(const char*)`, supporting all of the forms `string str;`, `string copyAddress(address);`, and `string address("서울시 성북구 삼선동 389");`.

Destructors, on the other hand, cannot be overloaded. A destructor cannot have parameters, so a class has exactly one.

### 1.5 Tracing Overload Resolution Step by Step

Applying the rules of Section 1.2 to an actual call takes three steps. First, all functions with the same name as the called name are gathered (candidate functions). Next, only the functions whose number of parameters matches and to whose parameter types each argument can be converted are kept (viable functions). Finally, among the remaining functions, the one whose conversions for each argument have the best rank (Table 2) is chosen, and if no single best function exists, the call is an ambiguity error.

```cpp
#include <iostream>
using namespace std;

void f(int n) { cout << "f(int) " << n << endl; }
void f(double d) { cout << "f(double) " << d << endl; }
void f(const char *s) { cout << "f(const char*) " << s << endl; }

int main() {
    f('A');        // char -> int: promotion
    f(3.5f);       // float -> double: promotion
    f(7);          // int: exact match
    f("hi");       // const char[3] -> const char*: exact match (array-to-pointer conversion)
    // f(10L);     // long -> int and long -> double are both standard conversions: ambiguous
    return 0;
}
```

```text
f(int) 65
f(double) 3.5
f(int) 7
f(const char*) hi
```

| Call | Argument type | `f(int)` | `f(double)` | `f(const char*)` | Result |
|:----|:----|:----|:----|:----|:----|
| `f('A')` | `char` | Promotion | Standard conversion | Not viable | `f(int)`, prints 65, the code of `'A'` |
| `f(3.5f)` | `float` | Standard conversion | Promotion | Not viable | `f(double)` |
| `f(7)` | `int` | Exact match | Standard conversion | Not viable | `f(int)` |
| `f("hi")` | `const char[3]` | Not viable | Not viable | Exact match | `f(const char*)` |
| `f(10L)` | `long` | Standard conversion | Standard conversion | Not viable | Ambiguity error |

*Table 3. Conversion ranks of each call for the three f() functions*

- In `f('A')`, converting `char` to `int` is a promotion and converting it to `double` is a standard conversion, so `f(int)`, with the better rank, is chosen. `char` is an integer type that holds a character code, so 65 is printed.
- The argument of `f("hi")` is an array of `const char`, and the conversion of an array name to a pointer to its first element counts as an exact match. A string cannot be converted to `int` or `double`, so those two functions are dropped from the viable functions from the start.
- The `long` of `f(10L)` converts to `int` and to `double` only through standard conversions. The two conversions have the same rank, so uncommenting the line makes Apple clang report "call to 'f' is ambiguous" and point to the two candidates `f(int)` and `f(double)`. `f(const char*)` is not viable, so it does not appear in the candidate list.

> **Note:** **Null pointers and function overloading.** When `f(int)` and `f(char*)` are overloaded together, `f(0)` calls the integer version because `0` is an `int` literal, which is not what passing a pointer intends. The macro `NULL` is defined by the implementation as `0`, `0L`, or a value internal to the compiler (`__null` in Apple clang), so the same `f(NULL)` may call the integer version or, as in this environment, fail with "call to 'f' is ambiguous". C++11's `nullptr`, which converts only to pointers, is covered in Section 9.1 of Chapter 14.

### 1.6 How Overloaded Functions Are Told Apart: Name Mangling

In C, the function name is the very symbol name the linker looks for, so two functions with the same name cannot be defined. A C++ compiler creates symbol names that add information about the parameter types to the function name, and so distinguishes overloaded functions as different symbols. This is called **name mangling**.

Putting the three `f()` functions of Section 1.5, with their bodies emptied, in one file, compiling it with Apple clang, and listing the symbols of the object file with the `nm` command gives the following lines for the three functions (the addresses depend on the environment). The second output passes through `c++filt`, which turns mangled names back into the original C++ declarations.

```text
$ nm mangle.o
0000000000000020 T __Z1fPKc
0000000000000010 T __Z1fd
0000000000000000 T __Z1fi

$ nm mangle.o | c++filt
0000000000000020 T f(char const*)
0000000000000010 T f(double)
0000000000000000 T f(int)
```

In `__Z1fi`, `1f` stands for the name `f` of length 1, and the final `i` stands for the parameter `int`. `d` is `double`, and `PKc` is a pointer to `const char`. The three functions have the same name but different symbol names, so the linker can tell them apart. A mangled name usually does not include the return type of the function, so two functions that differ only in their return type would become the same symbol and could not be distinguished. The rule that functions differing only in return type cannot be overloaded (Section 1.1) fits this implementation as well. The specific mangling scheme differs by compiler and platform. The problem that name mangling causes when C functions and C++ functions are linked together, and `extern "C"`, are covered in Section 6 of Chapter 13.

### 1.7 Declarations That Function Overloading Does Not Distinguish

Some parameter lists look different but are treated by the compiler as the same types. Two such functions are not overloads but two definitions of the same function, which is a compile error.

```cpp
typedef int Int;
void f(int n) { }
void f(Int n) { }               // (1) a typedef name is not a new type
void g(int n) { }
void g(const int n) { }         // (2) const on a value parameter is not part of the signature
void h(int a[]) { }
void h(int *a) { }              // (3) an array parameter is a pointer parameter
int main() { }
```

```text
same.cpp:3:6: error: redefinition of 'f'
same.cpp:5:6: error: redefinition of 'g'
same.cpp:7:6: error: redefinition of 'h'
```

- **(1)** `typedef` (or `using Int = int;`) only gives another name to an existing type and does not create a new type. `f(Int)` is the same as `f(int)`.
- **(2)** A parameter taken by value is a local variable that copies the caller's argument. Whether that copy can be changed inside the function (`const`) makes no difference to the caller, so the compiler ignores the `const` and treats the two functions as the same. On the other hand, the `const` of the object a reference or pointer refers to, as in `void g(int &r)` and `void g(const int &r)`, matters to the caller, so those are distinguished (the `const` subscript operator of Chapter 7 relies on this).
- **(3)** As in C, an array written as a function parameter becomes a pointer to its first element, so `int a[]` and `int *a` are the same parameter.

As with functions that differ only in return type (Section 1.1), these three cases also produce the same symbol name under name mangling (Section 1.6). Function overloading holds only when there is a difference the caller can distinguish, that is, when the number of parameters or their actual types differ.

---

<br>

## 2. Default Parameters

### 2.1 Syntax

A **default parameter** provides a fallback value when the caller omits the argument. Defaults must be assigned from the **rightmost** parameter.

```cpp
#include <iostream>
using namespace std;

void greet(string name, string msg = "Hello", int times = 1) {
    for (int i = 0; i < times; i++)
        cout << msg << ", " << name << "!" << endl;
}

int main() {
    greet("Alice");                  // Hello, Alice!        (msg="Hello", times=1)
    greet("Bob", "Hi");             // Hi, Bob!              (times=1)
    greet("Carol", "Hey", 3);       // Hey, Carol! (x3)
    return 0;
}
```

**Rules:**

| Rule | Valid | Invalid |
|:-----|:------|:--------|
| Defaults must be rightmost | `void f(int a, int b = 1, int c = 2)` | `void f(int a = 1, int b, int c = 2)` |
| Cannot skip middle defaults | `f(10)` or `f(10, 20)` or `f(10, 20, 30)` | `f(10, , 30)` |

*Table 4. Declaration and call rules for default parameters*

```cpp
// ERROR: default not at rightmost position
void bad(int a = 1, int b, int c = 3);  // b has no default, but a does

// OK: defaults are contiguous from the right
void ok(int a, int b = 2, int c = 3);
```

For the same reason, an ordinary parameter after a default parameter, as in `void calc(int a, int b = 5, int c, int d = 0);`, is a compilation error, while `void calc(int a, int b = 5, int c = 0, int d = 0);` is correct. Default parameters must be grouped at the end.

The arguments passed in a call fill the parameters in order from the left, and the remaining parameters receive their default values. The compiler rewrites calls to `void g(int a, int b = 0, int c = 0, int d = 0);` as follows.

| Call | Call as rewritten by the compiler |
|:-----|:-----|
| `g(10)` | `g(10, 0, 0, 0)` |
| `g(10, 5)` | `g(10, 5, 0, 0)` |
| `g(10, 5, 20)` | `g(10, 5, 20, 0)` |
| `g(10, 5, 20, 30)` | `g(10, 5, 20, 30)` |

*Table 5. Calls to g() with default parameters and the values filled in*

An argument must always be passed for a parameter without a default.

```cpp
void msg(int id, string text = "Hello");   // the default value of text is "Hello"

msg(10);                  // same as msg(10, "Hello")
msg(20, "Good Morning");  // passes 20 to id and "Good Morning" to text
msg();                    // compilation error: no value for the first parameter id
msg("Hello");             // compilation error: a string cannot be passed to the int parameter id
```

### 2.2 Declaration vs Definition

When the function declaration (prototype) and definition are **separate**, the default values go in the **declaration only** and are not written in the definition. This is because the default value of a parameter can be specified only once, and an argument can be omitted only if the prototype visible to the caller has the default. If there is a definition and no prototype, the default is written in the definition, as in `void star(int a = 5) { ... }`.

```cpp
// In header or before main: declaration with defaults
void display(int x, int y = 0, int z = 0);

int main() {
    display(1);          // y=0, z=0
    display(1, 2);       // z=0
    display(1, 2, 3);
    return 0;
}

// Definition: NO defaults here
void display(int x, int y, int z) {
    cout << x << " " << y << " " << z << endl;
}
```

> **Caution:** Specifying defaults in **both** the declaration and definition is a compilation error (redefinition of default argument).

### 2.3 Overloading Ambiguity

Default parameters can cause **ambiguity** with overloaded functions.

```cpp
void func(int a) {
    cout << "one param: " << a << endl;
}

void func(int a, int b = 10) {
    cout << "two params: " << a << ", " << b << endl;
}

int main() {
    func(5);       // ERROR: ambiguous!
    // Could be func(int) or func(int, int) with b defaulting to 10
    // Compiler cannot decide → compilation error

    func(5, 20);   // OK: only func(int, int) matches
    return 0;
}
```

> **Note:** **Tip.** Avoid defining overloads that differ only by a trailing default parameter. If `func(int)` exists, do not add `func(int, int = default)`.

### 2.4 Simplifying Overloads with Default Parameters

Default parameters can merge several overloaded functions into one. The following two constructors can be replaced by one constructor with a default parameter.

```cpp
// Two overloaded constructors
class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int r) { radius = r; }
};

// One constructor with a default parameter
class Circle {
    int radius;
public:
    Circle(int r = 1) { radius = r; }
};
```

The same holds for ordinary functions. `fillLine()`, which prints 25 `'*'` characters on one line, and `fillLine(int n, char c)`, which prints `n` copies of `c` on one line, can be merged into one.

```cpp
#include <iostream>
using namespace std;

void fillLine(int n = 25, char c = '*') {   // print n copies of character c on one line
    for (int i = 0; i < n; i++) cout << c;
    cout << endl;
}

int main() {
    fillLine();           // *************************
    fillLine(10, '%');    // %%%%%%%%%%
    return 0;
}
```

> **Caution:** The merged function must not coexist with the original overloads. `Circle(int r)` and `Circle(int r = 1)` have the same signature, which is a redeclaration error, and with `Circle()` and `Circle(int r = 1)` together, `Circle c;` matches both constructors and is an ambiguity error.

A constructor that allocates a dynamic array can be merged in the same way.

```cpp
class MyVector {
    int *p;
    int size;
public:
    MyVector(int n = 100) {          // merges MyVector() and MyVector(int n)
        p = new int[n];
        size = n;
    }
    ~MyVector() { delete [] p; }
};

MyVector *v1 = new MyVector();       // allocates an array of 100 ints
MyVector *v2 = new MyVector(1024);   // allocates an array of 1024 ints
```

Another way to merge constructors is the C++11 **delegating constructor**. Writing `MyVector() : MyVector(100) { }` makes the parameterless constructor hand initialization over to `MyVector(int n)`.

### 2.5 When Default Arguments Are Evaluated

A default argument is not a value computed once and fixed when the function is defined but an expression evaluated again every time a call omits the argument. Therefore, when a default argument uses a variable, the value at the time of the call is used.

```cpp
#include <iostream>
using namespace std;

int width = 10;                        // global variable

void box(int w = width) {              // the default argument is evaluated on every call
    cout << "w=" << w << endl;
}

int main() {
    box();          // box(width): width is 10 at this point
    width = 30;
    box();          // box(width): width is 30 at this point
    box(5);         // when an argument is given, the default argument is not evaluated
    return 0;
}
```

```text
w=10
w=30
w=5
```

As seen in Table 5, the compiler turns `box()` into the call `box(width)`. This transformation happens at each calling statement, so the second `box()` receives 30. A default argument can be any expression that can be computed at the call site, but it cannot use other parameters of the function. Using an earlier parameter as a default argument, as in `void line(int n, int m = n);`, is a compile error.

### 2.6 Rules for Specifying Default Arguments

Section 2.2 said that default values are written only in the declaration, but the precise rule is "a default argument for the same parameter is specified only once within a scope". A function declaration (prototype) can be written several times, so a later declaration can also add a default to an earlier parameter that does not have one yet.

```cpp
#include <iostream>
using namespace std;

void star(int a, int b = 1);       // first declaration: the default of b is 1
void star(int a = 5, int b);       // a redeclaration adds a default to the earlier a

void star(int a, int b) {          // definition: the defaults are not written again
    for (int i = 0; i < a * b; i++) cout << '*';
    cout << endl;
}

int main() {
    star();          // star(5, 1)
    star(2);         // star(2, 1)
    star(2, 3);      // star(2, 3)
    return 0;
}
```

```text
*****
**
******
```

- The second declaration `void star(int a = 5, int b);` looks as if `b` had no default, but `b = 1` was already set by the first declaration, so it does not break the rule that defaults go from the right. Combined, the two declarations are the same as `star(int a = 5, int b = 1)`.
- Writing `int b = 1` again in the definition specifies the default of the same parameter twice, and Apple clang reports "redefinition of default argument". It is an error even if the value is the same.
- The compiler fills in omitted arguments using only the declarations visible before the calling statement. Default arguments therefore belong in the declarations of the header file that callers include, not in the definition in the implementation file. If the default is written only in the definition, other source files do not see it, and a call such as `star()` does not compile there.

Splitting declarations as in this example is only to show the rule; real code gathers all the defaults in one declaration for readability.

---

<br>

## 3. Static Members

### 3.1 Static Member Variables

A **static member variable** is shared across **all objects** of the class. There is only **one copy** in memory, regardless of how many objects are created.

`static` is originally one of the storage classes of variables and functions: a static entity is created when the program starts and destroyed when the program ends, and its scope follows where it is declared and its access specifier. Class members are divided into two kinds by this property.

- A **static member** is created only once per class and shared by all objects of that class. It is therefore also called a **class member**.
- A **non-static member** is created separately for each object when the object is created. It is therefore also called an **instance member**.

> **Note:** **Analogy.** Just as every person is born with their own eyes, a non-static member exists separately in each object. In contrast, just as air exists before a person is born and is shared by everyone, a static member exists before any object is created and is shared by all objects.

**Declaration and definition:**

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    static int count;   // declaration inside class

    Circle(int r) : radius(r) {
        count++;
    }
    ~Circle() {
        count--;
    }
};

// MUST define and initialize outside the class
int Circle::count = 0;

int main() {
    Circle a(10);
    Circle b(20);
    Circle c(30);
    cout << "Number of circles: " << Circle::count << endl;   // 3
    cout << "Via object: " << a.count << endl;                 // 3 (same value)
    return 0;
}
```

```text
Memory layout:

Static member (ONE copy):
┌───────────────────┐
│ Circle::count = 3 │  ← shared by all objects
└───────────────────┘

Objects (separate copies):
┌───────────┐  ┌───────────┐  ┌───────────┐
│ a         │  │ b         │  │ c         │
│ radius=10 │  │ radius=20 │  │ radius=30 │
└───────────┘  └───────────┘  └───────────┘
```

*Figure 1. Memory layout of a shared static member variable and per-object member variables*

**Key rules:**

| Rule | Detail |
|:-----|:-------|
| Declared with `static` inside the class | `static int count;` |
| Must be defined outside the class | `int ClassName::count = 0;` |
| Shared across all objects | Only one copy exists |
| Accessed via class name or object | `ClassName::count` or `obj.count` |
| Exists even with zero objects | Lifetime is the entire program |

*Table 6. Key rules of static member variables*

| Property | Non-static member | Static member |
|:-----|:-----|:-----|
| Declaration example | `int n;`, `void f();` | `static int n;`, `static void f();` |
| Spatial property | Created separately in each object (instance member) | Created once per class in global space outside the objects (class member) |
| Temporal property | Created when the object is created and destroyed together with the object | Created when the program starts, exists before objects are created and after they are destroyed, and is destroyed when the program ends |
| Sharing | Not shared; each object holds its own value | Shared by all objects of the same class |

*Table 7. Comparison of non-static and static members*

> **Caution:** Forgetting the definition outside the class (`int Circle::count = 0;`) causes a **link error**, not a compiler error. The declaration inside the class only announces existence; the definition allocates memory.

#### `constexpr` vs `const` for Compile-Time Constants

Since C++11, `constexpr` declares values that are guaranteed to be evaluated at **compile time**. This is stricter than `const`, which only means "cannot be modified after initialization" but may still be initialized at runtime.

```cpp
const int maxSize = 100;            // const: value set at initialization, maybe at runtime
constexpr int maxSize = 100;        // constexpr: MUST be known at compile time

const int x = someFunction();       // OK: const allows runtime initialization
// constexpr int y = someFunction(); // ERROR (unless someFunction is constexpr)

constexpr double pi = 3.14159265;   // OK: literal value known at compile time
```

| Keyword | Compile-time guarantee | Initialization from runtime value | Available since |
|:--------|:----------------------|:----------------------------------|:---------------|
| `const` | No (may be runtime) | Yes | C++98 |
| `constexpr` | **Yes** (must be compile-time) | No (only from constexpr expressions) | C++11 |

*Table 8. Comparison of `const` and `constexpr`*

Use `constexpr` when the value is truly a compile-time constant (array sizes, template arguments, etc.). Use `const` when you simply want to prevent modification.

```cpp
constexpr int arraySize = 10;
int arr[arraySize];                  // OK: constexpr works as array size

const int n = getValue();
// int arr2[n];                      // ERROR in standard C++: n is not constexpr
```

#### Inline Variables for Static Member Initialization in Headers (C++17)

Traditionally, static member variables must be defined (and initialized) outside the class in exactly one `.cpp` file. C++17 introduced **`inline` variables**, which allow you to define and initialize a static member variable **directly inside the class** in a header file without causing multiple-definition link errors.

```cpp
// Traditional approach (pre-C++17): requires separate definition in .cpp file
class Circle {
public:
    static int count;   // declaration only
};
int Circle::count = 0;  // definition in exactly one .cpp file

// C++17 approach: inline static variable, define right in the header
class Circle {
public:
    inline static int count = 0;   // declaration + definition in the header
};
// No separate .cpp definition needed!
```

This simplifies header-only libraries and removes a common source of link errors.

### 3.2 Accessing Static Members

Static members are accessed in the following three ways.

| Way | Form | Example |
|:-----|:-----|:-----|
| Object name | `object.staticMember` | `han.sharedMoney = 200;` |
| Object pointer | `objectPointer->staticMember` | `p->addShared(200);` |
| Class name and scope resolution operator | `ClassName::staticMember` | `Person::addShared(50);` |

*Table 9. Three ways to access static members*

Because there is only one static member per class, it can be accessed through the class name, even when no object exists. Non-static members, in contrast, exist separately in each object, so accessing them through the class name, as in `Person::money = 100;` or `Person::addMoney(200);`, is a compilation error.

```cpp
#include <iostream>
using namespace std;

class Person {
public:
    double money;                 // personal money
    void addMoney(int money) {
        this->money += money;
    }

    static int sharedMoney;       // shared fund
    static void addShared(int n) {
        sharedMoney += n;
    }
};

int Person::sharedMoney = 10;     // static member variable definition, initialized to 10

int main() {
    Person::addShared(50);                    // callable without objects, shared fund = 60
    cout << Person::sharedMoney << endl;      // 60

    Person han;
    han.money = 100;                          // han's personal money = 100
    han.sharedMoney = 200;                    // access via object name, shared fund = 200

    Person lee;
    lee.money = 150;                          // lee's personal money = 150
    lee.addMoney(200);                        // lee's personal money = 350
    Person *p = &lee;
    p->addShared(200);                        // access via object pointer, shared fund = 400

    cout << han.money << ' ' << lee.money << endl;              // 100 350
    cout << han.sharedMoney << ' ' << Person::sharedMoney << endl;  // 400 400
    // Person::money = 100;                   // ERROR: non-static member not accessible via class name
    return 0;
}
```

```text
60
100 350
400 400
```

`han.sharedMoney`, `p->addShared()`, and `Person::sharedMoney` all handle the same single variable, so the shared fund changed through `han` carries over directly to the amount added through `lee`. In contrast, `money` exists separately in each object, so `han` has 100 and `lee` has 350.

> **Note:** Access through an object name looks like access to an ordinary member and hides the fact that the member is static. Access through the class name shows clearly that the member is shared.

### 3.3 Static Member Functions

A **static member function** is not associated with any particular object, so it **does not have a `this` pointer**. Therefore the only members it can access by name alone, without specifying an object, are **static members**.

```cpp
#include <iostream>
using namespace std;

class Math {
public:
    static int abs(int x) {
        return (x >= 0) ? x : -x;
    }

    static int max(int a, int b) {
        return (a > b) ? a : b;
    }
};

int main() {
    // Called via class name: no object needed
    cout << Math::abs(-5) << endl;    // 5
    cout << Math::max(3, 7) << endl;  // 7
    return 0;
}
```

**Restrictions of static member functions:**

```cpp
class Example {
    int data;              // non-static member
    static int shared;     // static member
public:
    static void staticFunc() {
        // shared = 10;    // OK: can access static member
        // data = 20;      // ERROR: cannot access non-static member
        // this->data;     // ERROR: no 'this' pointer
    }

    void normalFunc() {
        shared = 10;       // OK: non-static can access static
        data = 20;         // OK: non-static can access non-static
    }
};

int Example::shared = 0;
```

A static member function can access static member variables, other static member functions, and its own local variables and parameters. It cannot access non-static members because a static member function can be called even when no object has been created yet. In the following code, when `main()` calls `PersonError::getMoney()`, no object holding `money` exists yet.

```cpp
class PersonError {
    int money;
public:
    static int getMoney() { return money; }   // compilation error: accesses a non-static member
    void setMoney(int money) { this->money = money; }
};

int main() {
    int n = PersonError::getMoney();   // no PersonError object exists at this point
    PersonError errorKim;
    errorKim.setMoney(100);
}
```

For the same reason, using `this` in a static member function, as in `this->sharedMoney += n;`, is a compilation error, while `sharedMoney += n;` compiles normally. Conversely, a non-static member function can access both static and non-static members, as in `int total() { return money + sharedMoney; }`.

> **Note:** A static member function can still access the non-static members of an object it receives as a parameter, as in `obj.data`. The restriction comes from the lack of `this`, which leaves no object to refer to implicitly.

| Feature | Static Member Function | Non-Static Member Function |
|:--------|:----------------------|:--------------------------|
| `this` pointer | No | Yes |
| Access static members | Yes | Yes |
| Access non-static members | **No** | Yes |
| Call without object | `ClassName::func()` | Not possible |

*Table 10. Comparison of static and non-static member functions*

### 3.4 Use Cases

Static has two main uses. First, it encapsulates global variables and global functions as static members of a class, reducing global names (the `Math` class in Section 3.3). Second, it creates variables shared among objects (object counting below).

**Object counting:**

```cpp
class Student {
    string name;
    static int count;
public:
    Student(string n) : name(n) { count++; }
    ~Student() { count--; }
    static int getCount() { return count; }
};

int Student::count = 0;

int main() {
    Student a("Alice");
    Student b("Bob");
    cout << Student::getCount() << endl;   // 2
    {
        Student c("Carol");
        cout << Student::getCount() << endl;   // 3
    }   // c destroyed here
    cout << Student::getCount() << endl;   // 2
    return 0;
}
```

In the same way, for a `Circle` class that increments the count in its constructor and decrements it in its destructor, creating an object array with `Circle *p = new Circle[10];` runs the constructor 10 times so the count becomes 10, and freeing it with `delete [] p;` runs the destructor 10 times so the count returns to 0.

**Bulletin board with static array:**

```cpp
#include <iostream>
#include <string>
#define MAX_NO 5
using namespace std;

class Board {
    static int no;                  // current number of posts
    static string board[MAX_NO];   // static array of posts
    Board() {}                     // private constructor: prevent object creation
public:
    static void add(string text) {
        if (no >= MAX_NO) {
            cout << "Cannot add! Maximum " << MAX_NO << " posts." << endl;
            return;
        }
        board[no++] = text;
    }

    static void print() {
        if (no == 0) {
            cout << "<No posts registered.>" << endl;
            return;
        }
        for (int i = 0; i < no; i++)
            cout << i << ": " << board[i] << endl;
    }

    static void clear() {
        no = 0;
    }
};

int Board::no = 0;
string Board::board[MAX_NO];

int main() {
    Board::add("Assignment due Friday.");
    Board::add("Lab session moved to Room 301.");
    Board::print();
    // 0: Assignment due Friday.
    // 1: Lab session moved to Room 301.
    Board::clear();
    Board::print();
    // <No posts registered.>
    return 0;
}
```

**Key design points in this pattern:**

| Point | Explanation |
|:------|:-----------|
| Private constructor | Prevents creating `Board` objects; all work goes through static functions |
| Static array + static count | Data exists without any object and is managed entirely through the class name |
| All functions are static | Called without objects, as in `Board::add(...)` and `Board::print()` |

*Table 11. Design points of a bulletin board built only from static members*

### 3.5 Three Uses of static

`static` means different things depending on where it is used. The property of the storage class seen in Section 3.1, namely that the object comes into existence when the program starts or when it is first used and lasts until the program ends, is common to all uses, but the effect differs by what it is attached to.

| Location | Example | Effect |
|:----|:----|:----|
| Local variable inside a function | `static int id = 0;` | Initialized only once, on first execution, and keeps its value after the function ends |
| Global variable or global function at file scope | `static int total;`, `static void helper();` | The name can be used only within the source file that declares it, so it does not collide with the same name in other files |
| Class member | `static int count;`, `static void f();` | Exists once per class rather than per object and is shared by all objects (Sections 3.1-3.3) |

*Table 12. Effect of static by location*

The first use can be checked as follows.

```cpp
#include <iostream>
using namespace std;

int nextId() {
    static int id = 0;           // static local variable: initialized only once, on the first call
    id++;
    return id;
}

int counter() {
    int n = 0;                   // ordinary local variable: created anew on every call
    n++;
    return n;
}

int main() {
    for (int i = 0; i < 3; i++)
        cout << "nextId()=" << nextId() << ", counter()=" << counter() << endl;
    return 0;
}
```

```text
nextId()=1, counter()=1
nextId()=2, counter()=1
nextId()=3, counter()=1
```

The `id` of `nextId()` is initialized to 0 on the first call and does not disappear when the function ends, so it grows by 1 on every call. The `n` of `counter()` is created anew and initialized to 0 on every call, so it always returns 1. A static local variable keeps state between calls more safely than a global variable, because its name can be used only inside that function. It gives, for a single function rather than a class, the same effect as counting objects with a static member variable in Section 3.4.

### 3.6 Initialization and Lifetime of static Members

Section 3.1 said that a static member variable comes into existence when the program starts and is destroyed when it ends. Giving a class a static member variable of class type whose constructor and destructor print messages shows that these moments are before and after `main()`.

```cpp
#include <iostream>
using namespace std;

class Logger {
public:
    Logger() { cout << "Logger created" << endl; }
    ~Logger() { cout << "Logger destroyed" << endl; }
};

class App {
public:
    static Logger log;              // static member variable of class type: declaration
    static const int MAX = 3;       // an integral const static member can be initialized inside the class
    static int count;
};

Logger App::log;                    // definition: the constructor runs here
int App::count = 0;                 // definition and initialization

int main() {
    cout << "main starts, MAX=" << App::MAX << ", count=" << App::count << endl;
    App::count++;
    cout << "main ends, count=" << App::count << endl;
    return 0;
}
```

```text
Logger created
main starts, MAX=3, count=0
main ends, count=1
Logger destroyed
```

- **When it is created:** Because of the definition `Logger App::log;`, `App::log` is created before `main()` starts, like a global object. No `App` object is ever created, yet `log` exists. It follows the same rule as the order of creation of global objects seen in Chapter 3.
- **When it is destroyed:** It is destroyed when the program ends after `main()` returns. When there are several static objects, they are destroyed in the reverse order of creation.
- **Initialization inside the class:** An integral `const` static member, as in `static const int MAX = 3;`, can be given a value inside the class declaration. Static members of a non-integral type or that are not `const` are initialized in the definition outside the class, or use the C++17 `inline` variables of Section 3.1.
- **When the definition is missing:** With only `class A { public: static int n; };` and a use such as `A::n = 1;`, the code compiles, but the link step gives an undefined symbol error, `"A::n", referenced from: _main in ...` (Section 4.4 of Chapter 1). The `static int n;` inside the class is only a declaration, not a definition that allocates memory.

> **Caution:** Static objects within one source file are created in the order of their definitions, but the order of creation between static objects in different source files is not specified. If the constructor of a static object in one file uses a static object in another file, that object may not have been created yet. A common remedy is to turn a static object that depends on others into a static local object inside a function (Section 3.5), so that it is created when first used.

### 3.7 A Pitfall in Counting Objects with a static Member

Increasing a static member in the constructor and decreasing it in the destructor, as in Section 3.4, counts the objects that are alive. However, this method is accurate only if every path that creates an object goes through that constructor. As seen in Chapter 5, passing an object by value or initializing it from another object usually calls not the ordinary constructor but the copy constructor.

```cpp
#include <iostream>
using namespace std;

class Circle {
    static int numOfCircles;       // number of Circle objects alive
    int radius;
public:
    Circle(int r = 1) : radius(r) { numOfCircles++; }
    ~Circle() { numOfCircles--; }
    int getRadius() const { return radius; }
    static int getNum() { return numOfCircles; }
};

int Circle::numOfCircles = 0;

void show(Circle c) {              // call by value: the parameter c is copy-constructed
    cout << "inside show(): " << Circle::getNum() << ", r=" << c.getRadius() << endl;
}

int main() {
    Circle a, b(5);
    cout << "after a, b: " << Circle::getNum() << endl;
    show(a);
    cout << "after show(): " << Circle::getNum() << endl;
    {
        Circle c = a;              // copy construction
        cout << "after c is created: " << Circle::getNum() << endl;
    }
    cout << "after c is destroyed: " << Circle::getNum() << endl;
    return 0;
}
```

```text
after a, b: 2
inside show(): 2, r=1
after show(): 1
after c is created: 1
after c is destroyed: 0
```

| Point in time | Objects actually alive | `numOfCircles` | Reason for the difference |
|:----|:----|:----|:----|
| After `a` and `b` are created | `a`, `b` (2) | 2 | Both objects are created by `Circle(int)` |
| While `show(a)` runs | `a`, `b`, the parameter `c` (3) | 2 | The parameter is created by the compiler-generated copy constructor, which does not increase the count |
| After `show(a)` | `a`, `b` (2) | 1 | The destructor of the parameter runs and decreases the count |
| After `c` in the block is created | `a`, `b`, `c` (3) | 1 | It is copy construction, so the count does not increase |
| After `c` in the block is destroyed | `a`, `b` (2) | 0 | The destructor runs and decreases the count |

*Table 13. Difference between the number of objects and numOfCircles in a Circle without a copy constructor*

Objects created by copying only decrease the count without increasing it, so the value keeps shrinking and becomes 0 even though 2 objects are alive. Copying more objects this way could even make it negative. The fix is to write a copy constructor that also increases the count.

```cpp
Circle(const Circle &other) : radius(other.radius) { numOfCircles++; }   // counts copy construction as well
```

```text
after a, b: 2
inside show(): 3, r=1
after show(): 2
after c is created: 3
after c is destroyed: 2
```

With the copy constructor added, the count is 3 while the parameter is alive and 3 while `c` in the block is alive, and it returns to 2 after each is destroyed. This example shows that code counting the creation and destruction of objects is not complete with the constructor and destructor alone. When a hand-written destructor does some work, the copy constructor must be examined as well so that the work is paired for every object, which is the same principle as the rule of three of Section 4.9 of Chapter 5.

---

<br>

## 4. Common Mistakes

### 4.1 Initializing a static Member Variable in a Constructor's Initializer List

```cpp
class Circle {
    static int count;
public:
    Circle() : count(0) { }       // wrong: a static member is not something to initialize per object
};
int Circle::count = 0;
```

```text
error: member initializer 'count' does not name a non-static data member or base class
```

A member initializer list initializes the members of the object being created, that is, its non-static member variables and its base classes. A static member variable does not belong to an object; there is only one per class, and it has already been initialized when the program starts (Section 3.6), so it cannot appear in an initializer list. Its initial value is given in the definition outside the class, `int Circle::count = 0;`, and to change the value every time an object is created, assign it in the constructor body, as in `count++;`.

### 4.2 Declaring a static Member Function const

```cpp
static int get() const { return count; }   // wrong
```

```text
error: static member function cannot have 'const' qualifier
```

`const` at the end of a member function means "this function does not change the object `this` points to" (Section 2.5 of Chapter 3). A static member function has no `this` (Section 3.3), so there is no object to leave unchanged, and attaching `const` is meaningless and an error. Write `static int get() { return count; }`.

### 4.3 Defining a static Member Variable in a Header File

Putting the definition of a static member variable, `int Counter::n = 0;`, in a header file and including that header in two source files gives the following.

```cpp
// counter.h
#ifndef COUNTER_H
#define COUNTER_H
class Counter {
public:
    static int n;
};
int Counter::n = 0;          // definition placed in the header
#endif
```

```text
$ c++ -std=c++17 a.cpp main.cpp -o app
duplicate symbol 'Counter::n' in:
    .../a-f2831b.o
    .../main-248741.o
ld: 1 duplicate symbols
```

An include guard made with `#ifndef` prevents the same header from being included twice only **within one translation unit** (Section 7 of Chapter 3). `a.cpp` and `main.cpp` are different translation units, so each includes the header once, each object file receives one definition of `Counter::n`, and the linker reports a duplicate symbol error (Section 4.4 of Chapter 1). There are two fixes. Move the definition into a single source file such as `counter.cpp`, or, since C++17, declare and define it together inside the class with `static inline int n = 0;` (Section 3.1). An `inline` variable may have the same definition in several translation units, which are merged into one, so the program fixed in the second way links without errors.

---

<br>

## Summary

| Concept | Key Summary |
|:--------|:-------------|
| Function overloading | Several functions with the same name are defined with different numbers, types, or orders of parameters, and the return type alone does not distinguish them. |
| Overload resolution | The best match is sought in the order exact match, promotion, standard conversion, and an ambiguity error occurs if several functions match at the same rank. |
| Reference parameters and ambiguity | `add(int, int)` and `add(int, int&)` are both exact matches for a call that passes a variable, so the call is ambiguous. |
| Constructors and destructors | Constructors can be overloaded, but a destructor has no parameters and is unique in its class, so it cannot be overloaded. |
| Default parameters | Defaults are specified contiguously from the rightmost parameter, as in `void f(int a, int b = 10)`, and a call cannot omit only a middle argument. |
| Defaults in declaration only | When the prototype and definition are separate, the default value is specified once, in the prototype the caller sees. |
| Default and overloading ambiguity | With both `f(int)` and `f(int, int = 10)`, the call `f(5)` is ambiguous, so this pattern should be avoided. |
| Simplifying overloads with defaults | `Circle()` and `Circle(int r)` can be merged into one `Circle(int r = 1)`, but the merged function cannot coexist with the originals. |
| Static member variable | It is declared with `static` inside the class, exists once per class, and is shared by all objects. |
| Static member variable definition | It must be defined outside the class, as in `int ClassName::count = 0;`, and forgetting the definition causes a link error. |
| Static member access | Static members can be accessed through an object name, an object pointer, or `ClassName::member`, while non-static members cannot be accessed through the class name. |
| Static member function | It has no `this` pointer, so by name alone it can access only static members, and it is called without an object as `ClassName::func()`. |
| Object counting | Incrementing a static count in the constructor and decrementing it in the destructor counts the living objects. |
| Private constructor + static members | This design prevents object creation and makes all data and functions accessible through the class name (Board pattern). |
| `constexpr` vs `const` | `constexpr` (C++11) guarantees compile-time evaluation, while `const` only prevents modification. |
| `inline static` variables | Since C++17, writing `inline static int x = 0;` inside the class removes the need for a separate `.cpp` definition. |
| Steps of overload resolution | Gather the candidate functions with the same name, keep the viable functions to whose parameters the arguments can be converted, and choose the one function with the best conversion rank; if none is uniquely best, the call is an ambiguity error. |
| Name mangling | The compiler distinguishes overloaded functions by symbol names that add the parameter types to the function name, and the return type is usually not part of the symbol name. |
| Evaluation of default arguments | A default argument is evaluated at the time of each call that omits the argument, and an earlier parameter cannot be used as a default argument. |
| Three uses of static | On a local variable it keeps the value between calls, on a global name it makes the name visible only within that file, and on a member it makes the member exist once per class. |
| Declarations that are not distinguished | A typedef name, `const` on a value parameter, and the difference between array and pointer parameters do not create overloads and cause redefinition errors. |
| Rules for specifying default arguments | The default argument of a parameter is specified only once, a later declaration can add defaults to earlier parameters, and defaults belong in the header declaration that callers see. |
| Lifetime of static members | A static member variable defined outside the class is created before `main()` and destroyed when the program ends, and a missing definition causes a link error. |
| static counts and copy construction | Counting that increases in the constructor and decreases in the destructor misses copy-constructed objects, so the copy constructor must also increase the count. |
| static members and initializer lists | A static member variable is not a member of the object, so it cannot be initialized in a constructor's initializer list and is initialized in the definition outside the class. |
| static member functions and const | A static member function has no `this`, so `const` cannot be attached to it. |
| static member definitions in headers | An include guard prevents duplication only within one translation unit, so a static member definition in a header causes a duplicate symbol error across several source files. |

---

<br>

## Self-Check Questions

1. **Overloading condition:** Can `int sum(int a, int b)` and `double sum(int a, int b)` be defined together? Give the reason.

   <details>
   <summary>Answer</summary>

   They cannot. The two functions have the same name and parameter list and differ only in return type. The return type does not distinguish overloads, so for the call `sum(2, 5)` the compiler cannot decide which function to call, and the second definition is a compilation error.

   </details>

2. **Filling in defaults:** For `void g(int a, int b = 0, int c = 0, int d = 0);`, what are the values of the four parameters when `g(10, 5)` is called? And why is `void calc(int a, int b = 5, int c, int d = 0);` an error?

   <details>
   <summary>Answer</summary>

   Arguments fill from the left, so `a = 10` and `b = 5`, and the remaining `c` and `d` get the default value 0. That is, the call is the same as `g(10, 5, 0, 0)`. `calc` is a compilation error because `c`, which has no default, follows `b`, which has one, so the default parameters are not grouped at the end.

   </details>

3. **Type conversion and ambiguity:** Given `float square(float a)` and `double square(double a)`, how are `square(3.0)` and `square(3)` handled?

   <details>
   <summary>Answer</summary>

   `3.0` is a `double`, so `square(double)` is selected as an exact match. `3` is an `int`, and the conversions from `int` to `float` and from `int` to `double` are both standard conversions of the same rank, so the call is ambiguous and a compilation error occurs.

   </details>

4. **Tracing a static member:** In the `Person` example of Section 3.2, what is the value of `Person::sharedMoney` just before `main()` ends, and how does it reach that value?

   <details>
   <summary>Answer</summary>

   It is 400. Starting from the initial value 10, `Person::addShared(50)` makes it 60, `han.sharedMoney = 200;` makes it 200, and `p->addShared(200)` makes it 400. All three ways of access handle the same single variable, so `han.sharedMoney` is also 400.

   </details>

5. **Restriction of static member functions:** Explain, with reference to `this`, why a static member function cannot use a non-static member variable by name alone.

   <details>
   <summary>Answer</summary>

   A static member function is not associated with any particular object, so it has no `this` pointer, and it can be called as `ClassName::func()` even before any object has been created. A non-static member variable exists separately in each object, so it cannot be determined whose variable is meant, and it may not exist yet. Therefore using it by name alone is a compilation error, and it must be accessed through an object received as a parameter, as in `obj.data`.

   </details>

6. **Tracing overload resolution:** Given the three `f()` functions of Section 1.5, how are `f(true)`, `f(2.0f)`, and `f(10u)` handled?

   <details>
   <summary>Answer</summary>

   For `f(true)`, the promotion from `bool` to `int` ranks better than the standard conversion to `double`, so `f(int)` is called and "f(int) 1" is printed. `f(2.0f)` is the promotion from `float` to `double`, so `f(double)` is called. The `unsigned int` of `f(10u)` converts to `int` and to `double` only through standard conversions, so the two functions have the same rank and the call is an ambiguity error, like `f(10L)` in Section 1.5. Note that the conversion from `unsigned int` to `int` is not a promotion.

   </details>

7. **Static local variable:** If `static int id = 0;` in `nextId()` of Section 3.5 is changed to `static int id; id = 0;`, what values do the three calls return?

   <details>
   <summary>Answer</summary>

   All three return 1. `static int id;` is initialized only once, but the following `id = 0;` is an assignment statement, not an initialization, so it runs on every call and resets `id` to 0. Then `id++` makes it 1, so 1 is returned every time.

   </details>

8. **Name mangling:** Under the mangling of Section 1.6, do `void f(int, double)` and `void f(double, int)` get the same symbol name? What about `int g(int)` and `double g(int)`, which differ only in their return type?

   <details>
   <summary>Answer</summary>

   The order of the parameter types goes into the mangled name, so with Apple clang `f(int, double)` and `f(double, int)` become `__Z1fid` and `__Z1fdi`, which differ. The two functions are a normal overload. In contrast, the return type does not go into the name, so `int g(int)` and `double g(int)` would both become `__Z1gi` and could not be distinguished; the compiler reports "functions that differ only in their return type cannot be overloaded" before that point.

   </details>

9. **Declarations that are not distinguished:** Which of the following pairs can be defined together? (a) `void f(int)` and `void f(const int)` (b) `void f(int&)` and `void f(const int&)` (c) `void f(char*)` and `void f(char[])` (d) `void f(int)` and `void f(long)`

   <details>
   <summary>Answer</summary>

   (b) and (d). In (a), `const` on a value parameter is not part of the signature, so they are the same function, and in (c) an array parameter is the same as a pointer parameter, so they are the same function. In (b) the `const` of the object the reference refers to differs, so they are distinguished, and in (d) the parameter types themselves differ.

   </details>

10. **When a static member is created:** In the program of Section 3.6, what happens if `Logger App::log;` is deleted and `main()` does not use `App::log`? What happens if `main()` prints the address of `App::log`?

    <details>
    <summary>Answer</summary>

    If `App::log` is not used, there is no link error even without the definition, and "Logger created" and "Logger destroyed" are not printed, because the declaration inside the class alone does not create an object. If `main()` uses `App::log`, as in `&App::log`, the definition of that symbol cannot be found, and the link step gives an undefined symbol error.

    </details>

11. **Where default arguments go:** If the header `util.h` declares `int area(int w, int h);` and the definition in `util.cpp` is written as `int area(int w, int h = 1) { return w * h; }`, can `main.cpp` call `area(5)`? Give the fix.

    <details>
    <summary>Answer</summary>

    It cannot. When translating `main.cpp`, the compiler sees only the declaration in the included `util.h`, which has no default for `h`, so there is no function that accepts a call with one argument. Move the default into the header declaration, as in `int area(int w, int h = 1);`, and delete `= 1` from the definition. Keeping it in the definition as well would specify the default of the same parameter twice and cause an error in `util.cpp`.

    </details>

12. **static counts and assignment:** For the `Circle` of Section 3.7 with the copy constructor added, what is `numOfCircles` after `Circle a, b; a = b;`? Does the assignment operator also need code that changes the count?

    <details>
    <summary>Answer</summary>

    It is 2. `a` and `b` are created by the constructor, making it 2, and `a = b;` is an assignment that copies the value of `b` into the existing `a`, so no object is created or destroyed. The assignment operator must therefore not change the count, and the compiler-generated assignment operator keeps the count correct as it is.

    </details>
