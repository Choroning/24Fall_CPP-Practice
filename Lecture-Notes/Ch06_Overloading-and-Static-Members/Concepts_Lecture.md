# Chapter 6 — Overloading and Static Members

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-10-27
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
- [2. Default Parameters](#2-default-parameters)
  - [2.1 Syntax](#21-syntax)
  - [2.2 Declaration vs Definition](#22-declaration-vs-definition)
  - [2.3 Overloading Ambiguity](#23-overloading-ambiguity)
  - [2.4 Simplifying Overloads with Default Parameters](#24-simplifying-overloads-with-default-parameters)
- [3. Static Members](#3-static-members)
  - [3.1 Static Member Variables](#31-static-member-variables)
  - [3.2 Accessing Static Members](#32-accessing-static-members)
  - [3.3 Static Member Functions](#33-static-member-functions)
  - [3.4 Use Cases](#34-use-cases)
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
         // ambiguous — compiler cannot choose
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

*Table 3. Declaration and call rules for default parameters*

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

*Table 4. Calls to g() with default parameters and the values filled in*

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
// In header or before main — declaration with defaults
void display(int x, int y = 0, int z = 0);

int main() {
    display(1);          // y=0, z=0
    display(1, 2);       // z=0
    display(1, 2, 3);
    return 0;
}

// Definition — NO defaults here
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

*Table 5. Key rules of static member variables*

| Property | Non-static member | Static member |
|:-----|:-----|:-----|
| Declaration example | `int n;`, `void f();` | `static int n;`, `static void f();` |
| Spatial property | Created separately in each object (instance member) | Created once per class in global space outside the objects (class member) |
| Temporal property | Created when the object is created and destroyed together with the object | Created when the program starts, exists before objects are created and after they are destroyed, and is destroyed when the program ends |
| Sharing | Not shared; each object holds its own value | Shared by all objects of the same class |

*Table 6. Comparison of non-static and static members*

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

| Keyword | Compile-time guarantee | Can initialize from runtime value? | Available since |
|:--------|:----------------------|:----------------------------------|:---------------|
| `const` | No (may be runtime) | Yes | C++98 |
| `constexpr` | **Yes** (must be compile-time) | No (only from constexpr expressions) | C++11 |

*Table 7. Comparison of `const` and `constexpr`*

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

// C++17 approach: inline static variable — define right in the header
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

*Table 8. Three ways to access static members*

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
    // Called via class name — no object needed
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

*Table 9. Comparison of static and non-static member functions*

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
    Board() {}                     // private constructor — prevent object creation
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

*Table 10. Design points of a bulletin board built only from static members*

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
