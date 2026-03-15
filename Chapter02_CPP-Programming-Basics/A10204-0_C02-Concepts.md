# Chapter 02 — C++ Programming Basics

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *명품 C++ Programming (Revised Edition)* — Chapter 2

---

## Table of Contents

- [1. First C++ Program](#1-first-c-program)
  - [1.1 Program Structure](#11-program-structure)
  - [1.2 Comments](#12-comments)
  - [1.3 `main` Function](#13-main-function)
- [2. Console Output](#2-console-output)
  - [2.1 `cout` and `<<`](#21-cout-and-)
  - [2.2 `endl` vs `'\n'`](#22-endl-vs-n)
  - [2.3 `<<` Operator Details](#23--operator-details)
- [3. Console Input](#3-console-input)
  - [3.1 `cin` and `>>`](#31-cin-and-)
  - [3.2 `getline`](#32-getline)
  - [3.3 `cin` vs `scanf`](#33-cin-vs-scanf)
- [4. Namespace](#4-namespace)
  - [4.1 Concept](#41-concept)
  - [4.2 `using namespace std`](#42-using-namespace-std)
- [5. C++ String](#5-c-string)
- [6. Modern Variable Declaration with `auto`](#6-modern-variable-declaration-with-auto)
- [Summary](#summary)

---

<br>

## 1. First C++ Program

### 1.1 Program Structure

Every C++ program follows a basic structure. Here is the simplest complete program, annotated line by line:

```cpp
// hello.cpp
#include <iostream>       // C++ I/O stream library
using namespace std;      // use the std namespace

int main()                // entry point function
{
    cout << "Hello" << endl;   // output to console
    return 0;             // exit code to OS
}
```

```
C++ Program Structure
├── Preprocessor Directives   #include <iostream>
├── Namespace Declaration     using namespace std;
├── main Function             int main() { ... }
│   ├── Statements            cout << "Hello" << endl;
│   └── Return Statement      return 0;
└── Other Functions           (defined later in the course)
```

| Part | Role |
|:-----|:-----|
| `#include <iostream>` | Include the I/O stream library so `cout` and `cin` can be used |
| `using namespace std;` | Allow direct use of `std` names without the `std::` prefix |
| `int main()` | Define the entry point; OS calls this function first |
| `{ ... }` | Braces delimit the function body |
| `cout << "Hello" << endl;` | Output `"Hello"` followed by a newline to the console |
| `return 0;` | Return exit code 0 (normal termination) to the OS |

> **Key Point:** Every statement inside a function must end with a semicolon (`;`). Preprocessor directives (`#include`) do **not** end with a semicolon.

### 1.2 Comments

Comments are ignored by the compiler. They exist solely for human readers. C++ uses the same comment styles as C.

| Style | Syntax | Scope |
|:------|:-------|:------|
| Block comment | `/* ... */` | Can span multiple lines |
| Line comment | `// ...` | From `//` to end of line |

```cpp
/* This is a block comment.
   It can span multiple lines. */

// This is a line comment.

int x = 10;  // inline comment after code
```

> **Warning:** Block comments **cannot be nested**. `/* outer /* inner */ still inside? */` causes a compile error because `*/` closes the comment at the first match.

### 1.3 `main` Function

The `main` function is the **entry point** of every C++ program. When the OS runs a compiled program, execution begins at `main`.

| Form | Standard? | Notes |
|:-----|:---------:|:------|
| `int main()` | Yes | Standard form; returns `int` |
| `int main(void)` | Yes | Explicit empty parameter list |
| `void main()` | **No** | Non-standard; some compilers accept it but it is not portable |

```cpp
int main()        // standard form
{
    return 0;     // 0 = success
}
```

In C++, if `return 0;` is omitted at the end of `main`, the compiler implicitly returns `0`. However, it is good practice to write it explicitly for clarity.

> **Warning:** `void main()` is **not** standard C++. Although some compilers (e.g., older MSVC) accept it, it should be avoided for portability. Always use `int main()`.

---

<br>

## 2. Console Output

### 2.1 `cout` and `<<`

`cout` is a predefined object of type `ostream` that is connected to the standard output (screen). The `<<` operator is the **stream insertion operator** — it inserts data into the output stream.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "Hello, C++!" << endl;
    return 0;
}
```

| Component | Description |
|:----------|:------------|
| `cout` | `ostream` object connected to the screen (declared in `<iostream>`) |
| `<<` | Stream insertion operator — sends data to the output stream |
| `endl` | Inserts a newline character **and** flushes the output buffer |

**Chaining** — multiple `<<` operators can be chained in a single statement:

```cpp
int age = 20;
double gpa = 3.8;

cout << "Age: " << age << ", GPA: " << gpa << endl;
// Output: Age: 20, GPA: 3.8
```

Unlike C's `printf`, `cout` **automatically detects the type** of the data being output. There is no need for format specifiers like `%d` or `%f`.

```cpp
// C style — must specify format
printf("Age: %d, GPA: %.1f\n", age, gpa);

// C++ style — type detected automatically
cout << "Age: " << age << ", GPA: " << gpa << endl;
```

> **Tip:** `cout` is type-safe. If you accidentally pass the wrong type, the compiler will either convert it safely or produce a compile error. With `printf`, mismatched format specifiers cause undefined behavior.

#### Modern Output Alternatives: `std::format` and `std::print`

> **[C++20]** This feature requires C++20 or later.

C++20 introduced `std::format`, which provides Python-like string formatting. It combines the type safety of `cout` with the readability of format strings:

```cpp
#include <format>
#include <iostream>
using namespace std;

int age = 20;
double gpa = 3.8;

// C++20: std::format returns a formatted string
cout << format("Age: {}, GPA: {:.1f}", age, gpa) << endl;
// Output: Age: 20, GPA: 3.8
```

> **[C++23]** This feature requires C++23 or later.

C++23 goes further with `std::print`, which writes directly to the output without needing `cout`:

```cpp
#include <print>

int age = 20;
double gpa = 3.8;

// C++23: std::print — direct output with format syntax
std::print("Age: {}, GPA: {:.1f}\n", age, gpa);
// Output: Age: 20, GPA: 3.8
```

| Method | Standard | Header | Type-safe | Format control |
|:-------|:---------|:-------|:---------:|:--------------:|
| `printf` | C | `<cstdio>` | No | `%d`, `%f` |
| `cout <<` | C++98 | `<iostream>` | Yes | Manipulators |
| `std::format` | C++20 | `<format>` | Yes | `{}` placeholders |
| `std::print` | C++23 | `<print>` | Yes | `{}` placeholders |

### 2.2 `endl` vs `'\n'`

Both `endl` and `'\n'` produce a newline, but they differ in one important way:

| Method | Newline? | Flushes Buffer? |
|:-------|:--------:|:---------------:|
| `endl` | Yes | Yes |
| `'\n'` | Yes | No |

```cpp
cout << "Line 1" << endl;   // newline + flush
cout << "Line 2" << '\n';   // newline only (no flush)
cout << "Line 3\n";         // same as above (newline in string literal)
```

**Buffer flushing** means the data in the output buffer is immediately written to the screen. Without flushing, the data may stay in the buffer until it fills up or the program ends.

> **Tip:** For performance-sensitive code (e.g., outputting millions of lines), prefer `'\n'` over `endl`. The repeated buffer flushing caused by `endl` can significantly slow down output.

### 2.3 `<<` Operator Details

The `<<` operator can output all basic types:

| Type | Example |
|:-----|:--------|
| `bool` | `cout << true;` → `1` (or `true` with `boolalpha`) |
| `char` | `cout << 'A';` → `A` |
| `int` | `cout << 42;` → `42` |
| `double` | `cout << 3.14;` → `3.14` |
| `string` | `cout << "Hello";` → `Hello` |

**Expressions and function calls** can be used directly with `<<`:

```cpp
int a = 5, b = 3;
cout << a + b << endl;         // Output: 8
cout << (a > b) << endl;       // Output: 1 (true)
```

**Operator precedence issue** — the `<<` operator has **lower precedence** than comparison operators (`>`, `<`, `>=`, `<=`):

```cpp
cout << 5 > 3;            // ERROR — parsed as (cout << 5) > 3
cout << (5 > 3) << endl;  // OK — Output: 1
```

The expression `cout << 5 > 3` is interpreted as `(cout << 5) > 3`, which tries to compare the `ostream` object with `3` — a type error.

> **Warning:** When outputting the result of a comparison or arithmetic expression, always wrap the expression in **parentheses** to avoid precedence issues: `cout << (5 > 3)`.

---

<br>

## 3. Console Input

### 3.1 `cin` and `>>`

`cin` is a predefined object of type `istream` that is connected to the standard input (keyboard). The `>>` operator is the **stream extraction operator** — it extracts data from the input stream.

```cpp
#include <iostream>
using namespace std;

int main() {
    int num;
    cout << "Enter a number: ";
    cin >> num;
    cout << "You entered: " << num << endl;
    return 0;
}
```

| Component | Description |
|:----------|:------------|
| `cin` | `istream` object connected to the keyboard (declared in `<iostream>`) |
| `>>` | Stream extraction operator — reads data from the input stream |

**How `>>` reads input:**
- Skips leading whitespace (spaces, tabs, newlines).
- Reads characters until the next whitespace delimiter.
- Converts the read characters to the target variable's type.

**Chaining** — multiple `>>` operators can be chained:

```cpp
int a, b;
cout << "Enter two numbers: ";
cin >> a >> b;    // reads two integers separated by whitespace
cout << "Sum: " << a + b << endl;
```

```
Input:  10 20
Output: Sum: 30
```

> **Key Point:** `cin >> a >> b;` reads two space/tab/newline-separated tokens from the input. The first token goes into `a`, the second into `b`.

### 3.2 `getline`

The `>>` operator reads only until the next whitespace, which makes it unsuitable for reading strings that contain spaces. The `getline` function reads an **entire line** including spaces.

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string name;
    cout << "Enter your full name: ";
    getline(cin, name);
    cout << "Hello, " << name << "!" << endl;
    return 0;
}
```

```
Input:  John Doe
Output: Hello, John Doe!
```

**Mixing `>>` and `getline`** — a common pitfall:

When `>>` reads a value, it leaves the trailing newline (`'\n'`) in the input buffer. If `getline` is called next, it reads this leftover newline and returns an empty string.

**Solution:** Use `cin.ignore()` to discard the leftover newline before calling `getline`.

```cpp
int age;
string name;

cout << "Enter your age: ";
cin >> age;                    // reads the number, leaves '\n' in buffer

cin.ignore();                  // discard the leftover '\n'

cout << "Enter your name: ";
getline(cin, name);            // now reads correctly
cout << age << " " << name << endl;
```

> **Warning:** Forgetting `cin.ignore()` between `>>` and `getline` is one of the most common C++ beginner mistakes. The `getline` call will appear to be "skipped" because it immediately reads the leftover newline.

**Step-by-step visualization of the problem:**

```
User types: 25[Enter]John Doe[Enter]

Step 1: cin >> age;
  Buffer before: "25\nJohn Doe\n"
  Reads: 25
  Buffer after:  "\nJohn Doe\n"    ← '\n' remains!

Step 2 (WITHOUT cin.ignore()):  getline(cin, name);
  Buffer before: "\nJohn Doe\n"
  Reads: ""                          ← reads up to the first '\n', which is immediate
  Buffer after:  "John Doe\n"
  Result: name is an EMPTY string!

Step 2 (WITH cin.ignore()):  getline(cin, name);
  cin.ignore() discards '\n'
  Buffer before: "John Doe\n"
  Reads: "John Doe"                  ← correct!
  Buffer after:  ""
```

**Robust version with `cin.ignore`:** For maximum safety, use `cin.ignore(numeric_limits<streamsize>::max(), '\n')` which discards everything up to and including the next newline, not just a single character:

```cpp
#include <limits>   // for numeric_limits

cin >> age;
cin.ignore(numeric_limits<streamsize>::max(), '\n');   // discard rest of line
getline(cin, name);
```

### 3.3 `cin` vs `scanf`

| Feature | `cin` (C++) | `scanf` (C) |
|:--------|:------------|:------------|
| Type safety | Type detected automatically | Must specify format (`%d`, `%f`) |
| Format specifiers | Not needed | Required |
| Address-of operator `&` | Not needed for basic types | Required (`scanf("%d", &x)`) |
| String with spaces | Use `getline` | Use `fgets` |
| Error handling | Check stream state (`cin.fail()`) | Check return value |

```cpp
// C style
int x;
scanf("%d", &x);      // must use %d and &

// C++ style
int x;
cin >> x;              // no format specifier, no &
```

> **Tip:** In C++, prefer `cin`/`cout` over `scanf`/`printf`. They are type-safe, extensible to user-defined types, and integrate naturally with C++ streams.

---

<br>

## 4. Namespace

### 4.1 Concept

A **namespace** is a declarative region that provides a scope for identifiers (names). Its primary purpose is to **prevent name collisions** — different libraries may define functions or classes with the same name, and namespaces keep them separate.

The C++ standard library places all its names inside the `std` namespace.

```cpp
#include <iostream>

int main() {
    std::cout << "Hello" << std::endl;   // fully qualified name
    return 0;
}
```

| Syntax | Meaning |
|:-------|:--------|
| `std::cout` | `cout` defined in the `std` namespace |
| `std::endl` | `endl` defined in the `std` namespace |
| `std::string` | `string` class defined in the `std` namespace |

Without the `std::` prefix, the compiler does not know where `cout` is defined and will report an error.

### 4.2 `using namespace std`

To avoid writing `std::` before every standard library name, you can use a **using directive**:

```cpp
using namespace std;   // import all names from std into the current scope
```

After this declaration, you can use `cout`, `cin`, `endl`, `string`, `vector`, etc. directly.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "No std:: prefix needed" << endl;
    return 0;
}
```

**Selective using declaration** — import only specific names:

```cpp
#include <iostream>
using std::cout;
using std::endl;

int main() {
    cout << "Only cout and endl imported" << endl;
    // cin would still need std::cin
    return 0;
}
```

| Approach | Pros | Cons |
|:---------|:-----|:-----|
| `std::cout` (fully qualified) | No ambiguity; clear origin | Verbose |
| `using namespace std;` | Concise; convenient | May cause name collisions in large projects |
| `using std::cout;` (selective) | Balance of convenience and safety | Must list each name individually |

> **Tip:** For small programs and learning exercises, `using namespace std;` is perfectly fine. In large projects and header files, prefer fully qualified names or selective `using` declarations to avoid name collisions.

---

<br>

## 5. C++ String

C++ provides the `std::string` class as a modern, safer alternative to C-style character arrays (`char[]`).

```cpp
#include <string>     // required for std::string
#include <iostream>
using namespace std;

int main() {
    string greeting = "Hello, C++!";
    cout << greeting << endl;
    return 0;
}
```

**`std::string` vs C-style `char[]`:**

| Feature | `std::string` (C++) | `char[]` (C) |
|:--------|:--------------------|:-------------|
| Header | `#include <string>` | `#include <cstring>` |
| Declaration | `string s = "Hello";` | `char s[] = "Hello";` |
| Concatenation | `s1 + s2` | `strcat(s1, s2)` |
| Comparison | `s1 == s2` | `strcmp(s1, s2) == 0` |
| Length | `s.length()` or `s.size()` | `strlen(s)` |
| Dynamic size | Yes (grows automatically) | No (fixed at declaration) |
| Safe? | Yes (bounds-managed) | No (manual buffer management) |

**Common operations:**

```cpp
string a = "Hello";
string b = "World";

// Concatenation
string c = a + ", " + b + "!";   // "Hello, World!"

// Comparison
if (a == b) { /* ... */ }         // works naturally with ==

// Length
cout << c.length() << endl;       // 13

// Input with getline (reads entire line including spaces)
string line;
getline(cin, line);

// Accessing individual characters
cout << a[0] << endl;             // 'H'
```

> **Key Point:** `std::string` is introduced briefly here. Detailed string operations, methods, and iterators will be covered in later chapters. For now, know that `string` is safer and more convenient than `char[]` in C++.

---

<br>

## 6. Modern Variable Declaration with `auto`

Since C++11, the `auto` keyword lets the compiler **deduce the type** of a variable from its initializer. This reduces redundancy and makes code more maintainable.

```cpp
auto x = 42;           // int
auto pi = 3.14;        // double
auto name = "Hello"s;  // std::string (with s suffix, requires "using namespace std::literals")
auto greeting = string("Hello");  // std::string

// Especially useful with complex types
vector<int> nums = {1, 2, 3};
auto it = nums.begin();   // vector<int>::iterator — much shorter with auto
```

**When to use `auto`:**

| Use `auto` | Use explicit type |
|:-----------|:-----------------|
| When the type is obvious from the right-hand side | When the type is not obvious and readability suffers |
| With complex types (iterators, lambda, etc.) | When you intentionally want a different type (e.g., `int x = 3.14;` truncates) |
| With range-based for loops: `for (auto& item : vec)` | In public API / function signatures (clarity matters) |

> **Note:** `auto` does not mean the variable is untyped or dynamically typed. The type is determined at **compile time** and is fixed thereafter. C++ remains a statically-typed language.

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Program Structure | `#include <iostream>` → `using namespace std;` → `int main()` → statements → `return 0;` |
| Comments | `/* block */` and `// line`; same as C; block comments cannot nest |
| `main` Function | Entry point; returns `int`; `void main()` is non-standard |
| `cout` and `<<` | `ostream` object for output; stream insertion operator; auto type detection; chaining supported |
| `endl` vs `'\n'` | `endl` flushes the buffer; `'\n'` does not; prefer `'\n'` for performance |
| `<<` Precedence | Lower than comparison operators; use parentheses: `cout << (5 > 3)` |
| `cin` and `>>` | `istream` object for input; stream extraction operator; skips whitespace; chaining supported |
| `getline` | Reads entire line including spaces; use `cin.ignore()` after `>>` before `getline` |
| `cin` vs `scanf` | `cin` is type-safe, no format specifiers, no `&` needed for basic types |
| Namespace | Prevents name collisions; `std` contains standard library; `std::cout` |
| `using namespace std` | Shorthand to avoid `std::` prefix; `using std::cout` for selective import |
| `std::string` | Modern alternative to `char[]`; `+` for concat, `==` for compare, `.length()` for size |
| `std::format` / `std::print` | Modern output formatting (C++20/C++23); `{}` placeholders; type-safe like `cout` but with format strings |
| `auto` | Compiler deduces the type from the initializer (C++11); useful for iterators and complex types; still statically typed |

---
