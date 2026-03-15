# Chapter 10 — Templates and the Standard Library (STL)

> **Last Updated:** 2026-03-16
>
> **Textbook Reference:** *Myungpoom C++ Programming (Revised Edition)* — Chapter 10

---

## Table of Contents

- [1. Function Templates](#1-function-templates)
  - [1.1 Generic Programming](#11-generic-programming)
  - [1.2 Syntax](#12-syntax)
  - [1.3 Template Instantiation](#13-template-instantiation)
  - [1.4 Multiple Template Parameters](#14-multiple-template-parameters)
  - [1.5 Examples](#15-examples)
- [2. Class Templates](#2-class-templates)
  - [2.1 Syntax](#21-syntax)
  - [2.2 Instantiation](#22-instantiation)
  - [2.3 Operator Overloading in Templates](#23-operator-overloading-in-templates)
  - [2.4 Default Template Parameters](#24-default-template-parameters)
- [3. vector (In Depth)](#3-vector-in-depth)
  - [3.1 Declaration and Initialization](#31-declaration-and-initialization)
  - [3.2 Element Access](#32-element-access)
  - [3.3 Modifiers](#33-modifiers)
  - [3.4 Size and Capacity](#34-size-and-capacity)
  - [3.5 2D vector](#35-2d-vector)
  - [3.6 Iterating](#36-iterating)
  - [3.7 Passing to Functions](#37-passing-to-functions)
  - [3.8 Complete Example](#38-complete-example)
- [4. string](#4-string)
  - [4.1 std::string vs C-style char\[\]](#41-stdstring-vs-c-style-char)
  - [4.2 Construction and Concatenation](#42-construction-and-concatenation)
  - [4.3 Access and Search](#43-access-and-search)
  - [4.4 Comparison](#44-comparison)
  - [4.5 Conversion](#45-conversion)
  - [4.6 Iteration and Input](#46-iteration-and-input)
- [5. map and set](#5-map-and-set)
  - [5.1 map](#51-map)
  - [5.2 unordered_map](#52-unordered_map)
  - [5.3 set](#53-set)
  - [5.4 unordered_set](#54-unordered_set)
  - [5.5 Practical Example: Word Frequency](#55-practical-example-word-frequency)
- [6. algorithm](#6-algorithm)
  - [6.1 Sorting](#61-sorting)
  - [6.2 Searching](#62-searching)
  - [6.3 Counting and Accumulating](#63-counting-and-accumulating)
  - [6.4 Modifying Sequences](#64-modifying-sequences)
  - [6.5 Binary Search](#65-binary-search)
  - [6.6 Complete Example: Sort Students by Score](#66-complete-example-sort-students-by-score)
- [7. Modern C++ Features](#7-modern-c-features)
  - [7.1 auto (C++11)](#71-auto-c11)
  - [7.2 Range-based for (C++11)](#72-range-based-for-c11)
  - [7.3 Lambda Expressions (C++11)](#73-lambda-expressions-c11)
  - [7.4 Structured Bindings (C++17)](#74-structured-bindings-c17)
  - [7.5 std::optional (C++17)](#75-stdoptional-c17)
  - [7.6 Concepts (C++20)](#76-concepts-c20)
  - [7.7 std::ranges (C++20)](#77-stdranges-c20)
  - [7.8 std::print (C++23)](#78-stdprint-c23)
  - [7.9 C++26 Preview](#79-c26-preview)
- [8. Practical Patterns for Coding Tests](#8-practical-patterns-for-coding-tests)
  - [8.1 Reading Input Until EOF](#81-reading-input-until-eof)
  - [8.2 Sorting Custom Objects](#82-sorting-custom-objects)
  - [8.3 Frequency Counting with map](#83-frequency-counting-with-map)
  - [8.4 Unique Elements with set](#84-unique-elements-with-set)
  - [8.5 Stack and Queue](#85-stack-and-queue)
  - [8.6 priority_queue for Top-K Problems](#86-priority_queue-for-top-k-problems)
  - [8.7 pair and make_pair](#87-pair-and-make_pair)
- [9. Template Compilation Notes](#9-template-compilation-notes)
  - [9.1 Template Definition Must Be in Header](#91-template-definition-must-be-in-header)
  - [9.2 Common Errors](#92-common-errors)
- [10. Common Mistakes](#10-common-mistakes)
- [Summary](#summary)

---

<br>

## 1. Function Templates

### 1.1 Generic Programming

**Generic programming** allows you to write a function once and have it work for **any type**. Without templates, you would need to duplicate the same logic for each type (`int`, `double`, `char`, etc.).

```cpp
// Without templates — duplicated code
int maxInt(int a, int b) { return (a > b) ? a : b; }
double maxDouble(double a, double b) { return (a > b) ? a : b; }
char maxChar(char a, char b) { return (a > b) ? a : b; }
```

Templates eliminate this redundancy by letting the compiler generate type-specific versions automatically.

### 1.2 Syntax

A function template is declared with the `template` keyword followed by a **template parameter list** in angle brackets.

```cpp
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}
```

- `template <typename T>` — declares a template with one type parameter `T`.
- `typename` and `class` are interchangeable in this context: `template <class T>` is equivalent.
- `T` is a **placeholder** that will be replaced with the actual type when the function is called.

### 1.3 Template Instantiation

When a template function is called with a specific type, the compiler generates a **concrete function** for that type. This process is called **instantiation**.

**Implicit instantiation** — the compiler deduces `T` from the argument types:

```cpp
int result1 = myMax(3, 7);        // T deduced as int → myMax<int>(3, 7)
double result2 = myMax(3.5, 7.2); // T deduced as double → myMax<double>(3.5, 7.2)
```

**Explicit instantiation** — the programmer specifies `T` directly:

```cpp
double result3 = myMax<double>(3, 7.2);  // T explicitly set to double
```

> **Key Point:** Each distinct type used with a template causes the compiler to generate a separate concrete function. `myMax<int>` and `myMax<double>` are two different functions in the compiled binary.

### 1.4 Multiple Template Parameters

A template can have more than one type parameter:

```cpp
template <typename T1, typename T2>
void printPair(T1 a, T2 b) {
    cout << a << ", " << b << endl;
}

printPair(10, 3.14);       // T1=int, T2=double
printPair("Hello", 42);    // T1=const char*, T2=int
```

### 1.5 Examples

**Generic swap:**

```cpp
template <typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int x = 1, y = 2;
mySwap(x, y);       // x=2, y=1

string s1 = "hi", s2 = "bye";
mySwap(s1, s2);     // s1="bye", s2="hi"
```

**Generic print array:**

```cpp
template <typename T>
void printArray(T arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << "  ";
    }
    cout << endl;
}

int nums[] = {1, 2, 3, 4};
printArray(nums, 4);        // 1  2  3  4

double vals[] = {1.1, 2.2, 3.3};
printArray(vals, 3);        // 1.1  2.2  3.3
```

**Generic reverse array:**

```cpp
template <typename T>
void reverseArray(T arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        T temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

int data[] = {1, 10, 100, 1000};
reverseArray(data, 4);      // {1000, 100, 10, 1}
```

---

<br>

## 2. Class Templates

### 2.1 Syntax

A class template defines a class where one or more types are parameterized. The class definition is prefixed with `template <typename T>`.

```cpp
template <typename T>
class Container {
    T data;
    int size;
public:
    Container(T d, int s) : data(d), size(s) { }
    T getData();
    void setData(T d);
};
```

Member functions defined **outside** the class must repeat the template prefix and use `ClassName<T>::`:

```cpp
template <typename T>
T Container<T>::getData() {
    return data;
}

template <typename T>
void Container<T>::setData(T d) {
    data = d;
}
```

### 2.2 Instantiation

Each time a class template is used with a specific type, the compiler generates a **separate class**.

```cpp
Container<int> c1(42, 1);       // generates Container class for int
Container<string> c2("hello", 5); // generates Container class for string
Container<double> c3(3.14, 1);  // generates Container class for double
```

`Container<int>` and `Container<string>` are entirely different classes — they do not share code at runtime.

### 2.3 Operator Overloading in Templates

Template classes can overload operators just like regular classes. A common need is to overload comparison operators so that template functions like `bigger()` can work with custom types.

```cpp
template <typename T>
class Rectangle {
    T width, height;
public:
    Rectangle(T w = 1, T h = 1) : width(w), height(h) { }
    T getArea() { return width * height; }
    bool operator>(const Rectangle<T>& other) const {
        return this->getArea() > other.getArea();
    }
};

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

Rectangle<double> r1(10.2, 23.5);
Rectangle<double> r2(20.4, 13.7);
Rectangle<double> r3 = bigger(r1, r2);  // uses operator> to compare
cout << r3.getArea() << endl;            // 279.48 (area of r2)
```

> **Key Point:** When using a template function that relies on operators (like `>`, `==`, `<`), the type `T` must have those operators defined. For built-in types this is automatic; for custom classes you must overload them.

### 2.4 Default Template Parameters

Template parameters can have **default values**, just like function parameters:

```cpp
template <typename T = int>
class Stack {
    T data[100];
    int top;
public:
    Stack() : top(-1) { }
    void push(T item) { data[++top] = item; }
    T pop() { return data[top--]; }
};

Stack<> s1;            // T defaults to int
Stack<double> s2;      // T is double
Stack<string> s3;      // T is string
```

> **Note:** When using the default, you still need the angle brackets: `Stack<>`, not just `Stack`.

---

<br>

## 3. vector (In Depth)

`vector` is the **most important STL container** for beginners and arguably the most frequently used container in real-world C++ and coding tests. It is a dynamic array that manages its own memory and grows automatically as elements are added.

**Header:** `#include <vector>`

### 3.1 Declaration and Initialization

```cpp
#include <vector>
using namespace std;

vector<int> v1;                    // empty vector
vector<int> v2(5);                 // 5 elements, all initialized to 0
vector<int> v3(5, 42);            // 5 elements, all initialized to 42
vector<int> v4 = {1, 2, 3, 4, 5}; // initializer list (C++11)
vector<int> v5{10, 20, 30};       // brace initialization (C++11)
vector<int> v6(v4);               // copy of v4
vector<int> v7(v4.begin(), v4.begin() + 3); // {1, 2, 3} — from iterator range
```

### 3.2 Element Access

```cpp
vector<int> v = {10, 20, 30, 40, 50};

v[0];           // 10 — no bounds checking (undefined behavior if out of range)
v.at(1);        // 20 — bounds-checked (throws std::out_of_range if invalid)
v.front();      // 10 — first element
v.back();       // 50 — last element
```

| Method | Returns | Bounds Check |
|:-------|:--------|:-------------|
| `v[i]` | Element at index `i` | No (undefined behavior if out of range) |
| `v.at(i)` | Element at index `i` | Yes (throws `std::out_of_range`) |
| `v.front()` | First element | No |
| `v.back()` | Last element | No |

> **Tip:** Use `at()` during debugging; use `[]` in performance-critical code after you have verified the index is valid.

### 3.3 Modifiers

```cpp
vector<int> v = {10, 20, 30};

v.push_back(40);                 // {10, 20, 30, 40} — add to end
v.pop_back();                    // {10, 20, 30} — remove from end

v.insert(v.begin() + 1, 15);    // {10, 15, 20, 30} — insert before position
v.erase(v.begin() + 2);         // {10, 15, 30} — erase single element
v.erase(v.begin(), v.begin()+2);// {30} — erase range [first, last)

v.clear();                       // {} — remove all elements

v.resize(5);                     // {0, 0, 0, 0, 0} — resize to 5 (fill with 0)
v.resize(3, 99);                 // {0, 0, 0} — shrink to 3 (99 unused here)
v.resize(5, 99);                 // {0, 0, 0, 99, 99} — grow to 5, new elements = 99
```

| Method | Effect | Time Complexity |
|:-------|:-------|:----------------|
| `push_back(val)` | Add to end | Amortized O(1) |
| `pop_back()` | Remove last element | O(1) |
| `insert(pos, val)` | Insert before `pos` | O(n) |
| `erase(pos)` | Remove element at `pos` | O(n) |
| `erase(first, last)` | Remove range `[first, last)` | O(n) |
| `clear()` | Remove all elements | O(n) |
| `resize(n)` | Resize to `n` elements | O(n) |

### 3.4 Size and Capacity

`vector` has two separate concepts: **size** (number of elements currently stored) and **capacity** (total allocated memory slots).

```cpp
vector<int> v;
v.reserve(100);     // pre-allocate space for 100 elements (no elements created)

cout << v.size();       // 0 — no elements yet
cout << v.capacity();   // >= 100 — allocated space
cout << v.empty();      // true (1)

v.push_back(1);
v.push_back(2);
cout << v.size();       // 2
cout << v.capacity();   // >= 100 (still pre-allocated)

v.shrink_to_fit();      // request to reduce capacity to match size
```

| Method | Returns / Effect |
|:-------|:-----------------|
| `size()` | Number of elements |
| `capacity()` | Allocated storage (>= size) |
| `empty()` | `true` if size is 0 |
| `reserve(n)` | Pre-allocate space for at least `n` elements |
| `shrink_to_fit()` | Request to reduce capacity to size |

> **Performance Tip:** If you know the number of elements in advance, call `reserve()` before the loop. This avoids repeated reallocations (each reallocation copies all existing elements).

### 3.5 2D vector

A 2D vector is simply a `vector` of `vector`s.

```cpp
// Declare a 3x4 matrix initialized to 0
vector<vector<int>> matrix(3, vector<int>(4, 0));

matrix[0][0] = 1;
matrix[1][2] = 5;

// Iterate over 2D vector
for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
        cout << matrix[i][j] << " ";
    }
    cout << endl;
}

// Dynamic row addition
vector<vector<int>> jagged;
jagged.push_back({1, 2, 3});
jagged.push_back({4, 5});         // rows can have different lengths
jagged.push_back({6, 7, 8, 9});
```

### 3.6 Iterating

There are four common ways to iterate over a vector:

```cpp
vector<int> v = {10, 20, 30, 40};

// 1. Index-based loop
for (int i = 0; i < v.size(); i++) {
    cout << v[i] << " ";
}

// 2. Iterator loop
for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
    cout << *it << " ";
}

// 3. Range-based for (C++11) — copies each element
for (int x : v) {
    cout << x << " ";
}

// 4. Range-based for with reference (C++11) — no copy, can modify
for (int& x : v) {
    x *= 2;   // modifies the original vector
}
// v is now {20, 40, 60, 80}

// 5. Range-based for with const reference — no copy, read-only
for (const int& x : v) {
    cout << x << " ";
}
```

> **Best Practice:** Use `for (const auto& x : v)` for read-only access and `for (auto& x : v)` when you need to modify elements.

### 3.7 Passing to Functions

Always pass vectors by **reference** to avoid expensive copies. Use `const` if the function only reads the vector.

```cpp
// Read-only — pass by const reference
void printVector(const vector<int>& v) {
    for (const auto& x : v) {
        cout << x << " ";
    }
    cout << endl;
}

// Modify in place — pass by reference
void doubleAll(vector<int>& v) {
    for (auto& x : v) {
        x *= 2;
    }
}

// Return a new vector — return by value (move semantics make this efficient)
vector<int> createVector(int n) {
    vector<int> result(n);
    for (int i = 0; i < n; i++) result[i] = i + 1;
    return result;   // moved, not copied (C++11)
}
```

### 3.8 Complete Example

**Read N numbers, sort them, and print:**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cout << "Enter count: ";
    cin >> n;

    vector<int> nums(n);
    cout << "Enter " << n << " numbers: ";
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    sort(nums.begin(), nums.end());

    cout << "Sorted: ";
    for (const auto& x : nums) {
        cout << x << " ";
    }
    cout << endl;

    cout << "Min: " << nums.front() << endl;
    cout << "Max: " << nums.back() << endl;

    return 0;
}
```

```
Enter count: 5
Enter 5 numbers: 30 10 50 20 40
Sorted: 10 20 30 40 50
Min: 10
Max: 50
```

---

<br>

## 4. string

`std::string` is part of the C++ standard library and behaves like an STL container of characters. It manages its own memory and provides a rich set of operations.

**Header:** `#include <string>`

### 4.1 std::string vs C-style char[]

| Feature | `std::string` | `char[]` / `char*` |
|:--------|:--------------|:--------------------|
| Memory management | Automatic (dynamic) | Manual |
| Size tracking | `s.size()` / `s.length()` | `strlen(s)` — O(n) each call |
| Concatenation | `s1 + s2` or `s1 += s2` | `strcat()` — must manage buffer |
| Comparison | `==`, `<`, `>` | `strcmp()` |
| Safety | Bounds-checked with `at()` | Buffer overflow risk |
| Assignment | `s1 = s2` | Must use `strcpy()` |
| Pass to function | By reference | By pointer |

> **Recommendation:** Always prefer `std::string` over C-style strings in C++ code.

### 4.2 Construction and Concatenation

```cpp
#include <string>
using namespace std;

string s1;                     // empty string ""
string s2 = "Hello";          // from C-string literal
string s3("World");           // constructor form
string s4(5, 'x');            // "xxxxx" — 5 copies of 'x'
string s5 = s2;               // copy of s2

// Concatenation
string greeting = s2 + " " + s3;   // "Hello World"
s2 += "!";                         // "Hello!" — append in place
s2.append(" C++");                 // "Hello! C++" — another way to append
```

### 4.3 Access and Search

```cpp
string s = "Hello World";

// Access individual characters
s[0];              // 'H'
s.at(4);           // 'o' — bounds-checked

// Substring
s.substr(0, 5);    // "Hello" — substr(start_pos, length)
s.substr(6);       // "World" — from position 6 to end

// Search
s.find("World");       // 6 — position of first occurrence
s.find("xyz");         // string::npos — not found
s.find('o');           // 4 — first 'o'
s.find('o', 5);        // 7 — first 'o' starting from position 5
s.rfind('o');          // 7 — last occurrence (reverse find)
```

| Method | Returns |
|:-------|:--------|
| `find(str)` | Position of first occurrence, or `string::npos` if not found |
| `find(str, pos)` | Search starting from `pos` |
| `rfind(str)` | Position of last occurrence |
| `substr(pos, len)` | Substring of length `len` starting at `pos` |
| `substr(pos)` | Substring from `pos` to end |

### 4.4 Comparison

```cpp
string a = "apple";
string b = "banana";

a == b;     // false
a != b;     // true
a < b;      // true — lexicographic comparison ('a' < 'b')
a > b;      // false

// compare() returns 0 if equal, negative if a < b, positive if a > b
a.compare(b);    // negative value
```

### 4.5 Conversion

```cpp
#include <string>
using namespace std;

// Number to string
string s1 = to_string(42);       // "42"
string s2 = to_string(3.14);     // "3.140000"

// String to number
int n = stoi("123");              // 123
double d = stod("3.14");          // 3.14
long l = stol("1000000");         // 1000000
long long ll = stoll("999999999999"); // 999999999999

// These throw std::invalid_argument if conversion fails
// stoi("abc");   // throws exception
```

| Function | Conversion |
|:---------|:-----------|
| `to_string(num)` | Number -> string |
| `stoi(str)` | String -> `int` |
| `stol(str)` | String -> `long` |
| `stoll(str)` | String -> `long long` |
| `stod(str)` | String -> `double` |
| `stof(str)` | String -> `float` |

### 4.6 Iteration and Input

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    // Range-based for
    string s = "Hello";
    for (char c : s) {
        cout << c << " ";   // H e l l o
    }
    cout << endl;

    // Modify characters
    for (char& c : s) {
        c = toupper(c);
    }
    // s is now "HELLO"

    // Input: cin >> reads a single word (stops at whitespace)
    string word;
    cin >> word;          // input: "Hello World" -> word = "Hello"

    // Input: getline reads the entire line (including spaces)
    string line;
    getline(cin, line);   // input: "Hello World" -> line = "Hello World"

    // CAUTION: mixing cin >> and getline
    int n;
    cin >> n;             // reads number, leaves '\n' in buffer
    cin.ignore();         // discard the leftover '\n'
    getline(cin, line);   // now reads the full next line correctly

    return 0;
}
```

> **Common Pitfall:** After `cin >> variable`, a newline character remains in the input buffer. If you call `getline()` next, it reads an empty line. Always call `cin.ignore()` between `cin >>` and `getline()`.

---

<br>

## 5. map and set

### 5.1 map

`map<K, V>` stores **key-value pairs** sorted by key. Internally it uses a balanced binary search tree (red-black tree), providing O(log n) operations.

**Header:** `#include <map>`

```cpp
#include <map>
#include <string>
#include <iostream>
using namespace std;

int main() {
    map<string, int> ages;

    // Insert
    ages["Alice"] = 20;               // [] operator — inserts or updates
    ages["Bob"] = 22;
    ages.insert({"Charlie", 25});      // insert() — does NOT overwrite if key exists
    ages.insert(make_pair("Dave", 30));

    // Access
    cout << ages["Alice"] << endl;     // 20

    // Check existence (IMPORTANT: do NOT use [] to check — it creates a default entry!)
    if (ages.find("Eve") != ages.end()) {
        cout << "Found Eve" << endl;
    } else {
        cout << "Eve not found" << endl;   // this branch executes
    }

    if (ages.count("Bob") > 0) {       // count returns 0 or 1 for map
        cout << "Bob exists" << endl;
    }

    // Erase
    ages.erase("Charlie");             // remove by key

    // Iterate — elements are ordered by key
    for (auto it = ages.begin(); it != ages.end(); it++) {
        cout << it->first << ": " << it->second << endl;
    }
    // Alice: 20
    // Bob: 22
    // Dave: 30

    // Size
    cout << "Size: " << ages.size() << endl;   // 3

    return 0;
}
```

| Method | Description | Time |
|:-------|:------------|:-----|
| `m[key] = val` | Insert or update | O(log n) |
| `m[key]` | Access value (creates default if key missing!) | O(log n) |
| `m.at(key)` | Access value (throws if key missing) | O(log n) |
| `m.insert({k, v})` | Insert only if key does not exist | O(log n) |
| `m.find(key)` | Returns iterator (or `end()` if not found) | O(log n) |
| `m.count(key)` | Returns 0 or 1 | O(log n) |
| `m.erase(key)` | Remove by key | O(log n) |
| `m.size()` | Number of key-value pairs | O(1) |

### 5.2 unordered_map

`unordered_map<K, V>` uses a **hash table** instead of a tree. It provides **O(1) average** lookup but elements are **not sorted**.

**Header:** `#include <unordered_map>`

```cpp
#include <unordered_map>
#include <string>
using namespace std;

unordered_map<string, int> freq;
freq["apple"] = 3;
freq["banana"] = 5;

// Same interface as map: [], find, count, erase, insert, size
```

**When to use which:**

| | `map` | `unordered_map` |
|:--|:------|:----------------|
| Ordering | Sorted by key | No ordering |
| Lookup | O(log n) | O(1) average, O(n) worst |
| Use when | You need sorted order or range queries | You need fastest possible lookup |
| Key requirement | Must support `operator<` | Must support `std::hash` and `operator==` |

### 5.3 set

`set<T>` stores **unique elements** in sorted order. Internally uses a balanced BST.

**Header:** `#include <set>`

```cpp
#include <set>
#include <iostream>
using namespace std;

int main() {
    set<int> s;

    s.insert(30);
    s.insert(10);
    s.insert(20);
    s.insert(10);     // duplicate — ignored

    cout << s.size() << endl;   // 3

    // Check membership
    if (s.find(20) != s.end()) {
        cout << "20 exists" << endl;
    }
    if (s.count(99) == 0) {
        cout << "99 not found" << endl;
    }

    // Erase
    s.erase(10);       // remove element 10

    // Iterate — elements are sorted
    for (const auto& x : s) {
        cout << x << " ";    // 20 30
    }
    cout << endl;

    return 0;
}
```

| Method | Description | Time |
|:-------|:------------|:-----|
| `s.insert(val)` | Insert element (ignored if duplicate) | O(log n) |
| `s.find(val)` | Returns iterator (or `end()` if not found) | O(log n) |
| `s.count(val)` | Returns 0 or 1 | O(log n) |
| `s.erase(val)` | Remove element | O(log n) |
| `s.size()` | Number of elements | O(1) |

### 5.4 unordered_set

`unordered_set<T>` is the hash-based version of `set`. O(1) average operations, no ordering.

**Header:** `#include <unordered_set>`

```cpp
#include <unordered_set>
using namespace std;

unordered_set<int> us = {1, 2, 3, 4, 5};
us.insert(3);              // duplicate — ignored
us.count(3);               // 1
us.erase(2);               // remove element 2
```

### 5.5 Practical Example: Word Frequency

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> freq;
    string word;

    cout << "Enter words (Ctrl+D to stop):" << endl;
    while (cin >> word) {
        freq[word]++;      // [] creates entry with value 0 if key is new, then increments
    }

    cout << "\nWord frequencies:" << endl;
    for (const auto& pair : freq) {
        cout << pair.first << ": " << pair.second << endl;
    }

    return 0;
}
```

```
Enter words (Ctrl+D to stop):
the cat sat on the mat the cat
^D

Word frequencies:
cat: 2
mat: 1
on: 1
sat: 1
the: 3
```

---

<br>

## 6. algorithm

The `<algorithm>` header provides generic functions that operate on iterator ranges. These are essential for coding tests and everyday C++ programming.

**Header:** `#include <algorithm>`

### 6.1 Sorting

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {30, 10, 50, 20, 40};

// Default: ascending order
sort(v.begin(), v.end());          // {10, 20, 30, 40, 50}

// Descending order with comparator
sort(v.begin(), v.end(), greater<int>());  // {50, 40, 30, 20, 10}

// Custom comparator with lambda
sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;                  // descending
});

// Sort part of the vector
sort(v.begin(), v.begin() + 3);   // sort only first 3 elements
```

> **Note:** `sort` uses IntroSort (hybrid of quicksort, heapsort, and insertion sort) with O(n log n) guaranteed time complexity.

### 6.2 Searching

```cpp
vector<int> v = {10, 20, 30, 40, 50};

// find — returns iterator to first match, or end() if not found
auto it = find(v.begin(), v.end(), 30);
if (it != v.end()) {
    cout << "Found at index " << (it - v.begin()) << endl;  // index 2
}

// min_element / max_element — returns iterator to min/max
auto minIt = min_element(v.begin(), v.end());
auto maxIt = max_element(v.begin(), v.end());
cout << "Min: " << *minIt << endl;   // 10
cout << "Max: " << *maxIt << endl;   // 50
```

### 6.3 Counting and Accumulating

```cpp
#include <algorithm>
#include <numeric>      // for accumulate
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 2, 4, 2, 5};

// count — count occurrences of a value
int c = count(v.begin(), v.end(), 2);   // 3

// accumulate — sum all elements (from <numeric>)
int sum = accumulate(v.begin(), v.end(), 0);   // 19
// The third argument (0) is the initial value

// accumulate with custom operation
int product = accumulate(v.begin(), v.end(), 1, [](int a, int b) {
    return a * b;
});
// 1 * 1 * 2 * 3 * 2 * 4 * 2 * 5 = 480
```

### 6.4 Modifying Sequences

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// reverse
reverse(v.begin(), v.end());       // {5, 4, 3, 2, 1}

// unique + erase idiom — remove consecutive duplicates
vector<int> w = {1, 1, 2, 3, 3, 3, 4, 4, 5};
// IMPORTANT: vector must be sorted first for unique to remove ALL duplicates
auto last = unique(w.begin(), w.end());   // moves duplicates to end
w.erase(last, w.end());                   // actually remove them
// w = {1, 2, 3, 4, 5}

// fill
vector<int> u(5);
fill(u.begin(), u.end(), 42);     // {42, 42, 42, 42, 42}
```

### 6.5 Binary Search

These functions require the range to be **sorted**.

```cpp
vector<int> v = {10, 20, 30, 40, 50};

// binary_search — returns true/false
bool found = binary_search(v.begin(), v.end(), 30);   // true

// lower_bound — iterator to first element >= value
auto lb = lower_bound(v.begin(), v.end(), 25);
cout << *lb << endl;    // 30 (first element >= 25)

// upper_bound — iterator to first element > value
auto ub = upper_bound(v.begin(), v.end(), 30);
cout << *ub << endl;    // 40 (first element > 30)

// Find position/index
int idx = lower_bound(v.begin(), v.end(), 30) - v.begin();   // 2
```

| Function | Returns | Requires Sorted |
|:---------|:--------|:----------------|
| `binary_search(b, e, val)` | `true` / `false` | Yes |
| `lower_bound(b, e, val)` | Iterator to first >= val | Yes |
| `upper_bound(b, e, val)` | Iterator to first > val | Yes |

### 6.6 Complete Example: Sort Students by Score

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Student {
    string name;
    int score;
};

int main() {
    vector<Student> students = {
        {"Alice", 85},
        {"Bob", 92},
        {"Charlie", 78},
        {"Dave", 92},
        {"Eve", 88}
    };

    // Sort by score descending; if same score, by name ascending
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) return a.score > b.score;   // higher score first
        return a.name < b.name;                              // alphabetical if tied
    });

    for (const auto& s : students) {
        cout << s.name << ": " << s.score << endl;
    }
    // Bob: 92
    // Dave: 92
    // Eve: 88
    // Alice: 85
    // Charlie: 78

    return 0;
}
```

---

<br>

## 7. Modern C++ Features

### 7.1 auto (C++11)

The `auto` keyword lets the compiler **deduce the type** from the initializer. This is especially useful with complex types like iterators.

```cpp
auto x = 42;               // int
auto y = 3.14;             // double
auto s = string("hello");  // std::string

// Without auto — verbose
vector<int>::iterator it1 = v.begin();

// With auto — concise
auto it2 = v.begin();      // same type, deduced automatically

// With map — much cleaner
map<string, vector<int>> data;
// Without auto:
for (map<string, vector<int>>::iterator it = data.begin(); it != data.end(); it++) { }
// With auto:
for (auto it = data.begin(); it != data.end(); it++) { }
```

> **Guideline:** Use `auto` when the type is obvious from context or overly verbose. Avoid it when the type is not clear (e.g., `auto result = computeSomething();` — what type is `result`?).

### 7.2 Range-based for (C++11)

A cleaner syntax for iterating over containers:

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// By value (copies each element)
for (auto x : v) {
    cout << x << " ";
}

// By reference (no copy, can modify)
for (auto& x : v) {
    x *= 2;
}

// By const reference (no copy, read-only — RECOMMENDED for read access)
for (const auto& x : v) {
    cout << x << " ";
}
```

Works with any container that provides `begin()` and `end()`, including `string`, `map`, `set`, arrays, etc.

### 7.3 Lambda Expressions (C++11)

A lambda is an **anonymous function** defined inline. Syntax: `[capture](parameters) { body }`

```cpp
// Basic lambda
auto greet = [](const string& name) {
    cout << "Hello, " << name << "!" << endl;
};
greet("World");   // Hello, World!

// Lambda with return type
auto add = [](int a, int b) -> int {
    return a + b;
};

// Capture variables from surrounding scope
int factor = 3;
auto multiply = [factor](int x) { return x * factor; };  // capture by value
cout << multiply(10) << endl;   // 30

int total = 0;
auto accumulate_fn = [&total](int x) { total += x; };    // capture by reference
accumulate_fn(5);
accumulate_fn(10);
cout << total << endl;   // 15

// Common use: custom sort comparator
vector<int> v = {3, 1, 4, 1, 5};
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });  // descending

// Common use: for_each
for_each(v.begin(), v.end(), [](int x) { cout << x << " "; });
```

**Capture modes:**

| Capture | Meaning |
|:--------|:--------|
| `[]` | Capture nothing |
| `[x]` | Capture `x` by value (copy) |
| `[&x]` | Capture `x` by reference |
| `[=]` | Capture all used variables by value |
| `[&]` | Capture all used variables by reference |
| `[=, &x]` | All by value, except `x` by reference |

### 7.4 Structured Bindings (C++17)

> **[C++17]** This feature requires C++17 or later.

Structured bindings allow you to decompose a pair, tuple, or struct into named variables.

```cpp
// With map — clean iteration
map<string, int> ages = {{"Alice", 20}, {"Bob", 22}};

for (const auto& [name, age] : ages) {
    cout << name << " is " << age << endl;
}
// Instead of:
for (const auto& p : ages) {
    cout << p.first << " is " << p.second << endl;
}

// With pair
pair<int, string> p = {42, "answer"};
auto [num, text] = p;
cout << num << ": " << text << endl;   // 42: answer

// With struct
struct Point { int x, y; };
Point pt = {3, 4};
auto [px, py] = pt;
```

### 7.5 std::optional (C++17)

> **[C++17]** This feature requires C++17 or later.

`std::optional<T>` represents a value that may or may not be present. It is a safe alternative to using special sentinel values (like `-1` or `nullptr`).

```cpp
#include <optional>
#include <string>
using namespace std;

optional<int> findIndex(const vector<int>& v, int target) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == target) return i;     // return a value
    }
    return nullopt;                        // return "no value"
}

int main() {
    vector<int> v = {10, 20, 30};

    auto result = findIndex(v, 20);
    if (result.has_value()) {
        cout << "Found at index " << result.value() << endl;  // 1
    }

    // Shorthand with value_or
    int idx = findIndex(v, 99).value_or(-1);   // -1 (not found)

    return 0;
}
```

### 7.6 Concepts (C++20)

> **[C++20]** This feature requires C++20 or later.

Concepts constrain template parameters, providing clear error messages when a type does not meet requirements.

```cpp
#include <concepts>

template <typename T>
requires std::integral<T>
T gcd(T a, T b) {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Shorter syntax:
template <std::integral T>
T gcd2(T a, T b) { /* ... */ }

// Even shorter with auto:
auto gcd3(std::integral auto a, std::integral auto b) { /* ... */ }

gcd(12, 8);     // OK — int is integral
// gcd(3.14, 2.0); // compile error — double is not integral (clear message!)
```

### 7.7 std::ranges (C++20)

> **[C++20]** This feature requires C++20 or later.

The Ranges library provides a cleaner way to work with sequences, eliminating the need to pass `begin()` and `end()` separately.

```cpp
#include <ranges>
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {5, 3, 1, 4, 2};

// Instead of: sort(v.begin(), v.end());
ranges::sort(v);                    // {1, 2, 3, 4, 5}

// Views — lazy, composable transformations
auto even = v | views::filter([](int x) { return x % 2 == 0; });
// even lazily produces: 2, 4

auto squared = v | views::transform([](int x) { return x * x; });
// squared lazily produces: 1, 4, 9, 16, 25

// Compose views
auto result = v | views::filter([](int x) { return x > 2; })
                | views::transform([](int x) { return x * 10; });
// result lazily produces: 30, 40, 50
```

### 7.8 std::print (C++23)

> **[C++23]** This feature requires C++23 or later.

`std::print` brings Python-like formatted output to C++.

```cpp
#include <print>

std::print("Hello, {}!\n", "World");           // Hello, World!
std::print("x = {}, y = {}\n", 3, 4);         // x = 3, y = 4
std::println("No need for \\n with println");  // adds newline automatically
```

> **Note:** As of 2026, compiler support for `std::print` is still limited. Most coding tests do not support C++23 features.

### 7.9 C++26 Preview

> **[C++26]** C++26 is expected to be ratified in 2026. Compiler support is not yet available for most features. The information below is for awareness only.

C++26 brings significant additions focused on **safety**, **performance**, and **parallel computing**.

#### Contracts (`pre`, `post`, `contract_assert`)

The headline feature of C++26. Contracts replace the old `assert()` macro with language-level preconditions and postconditions.

```cpp
// C++26 Contracts syntax (not yet supported by compilers)
int divide(int a, int b)
    pre(b != 0)                     // precondition: b must not be zero
    post(r: r * b == a)             // postcondition: result * b == a
{
    return a / b;
}

void process(int* ptr)
    pre(ptr != nullptr)             // precondition: pointer must be valid
{
    contract_assert(*ptr > 0);      // runtime assertion (replaces assert())
    // ...
}
```

| Feature | Old (`assert`) | New (Contracts) |
|:--------|:--------------|:----------------|
| Syntax | Macro (`#include <cassert>`) | Language keyword |
| Scope | Runtime only | Compile-time + runtime |
| Works with modules | No (macros not exported) | Yes |
| Precondition | Manual check inside function | `pre(condition)` in signature |
| Postcondition | Not supported | `post(result: condition)` |

> **Key Point:** Contracts are necessary because C++20 modules don't export macros — the old `assert()` can't be used across module boundaries. `contract_assert` is the standardized replacement.

#### New Containers and Libraries

| Header | Description |
|:-------|:------------|
| `<inplace_vector>` | Fixed-capacity dynamic vector; heap-allocated but no reallocation — combines array performance with vector interface |
| `<hive>` | High-performance container with bulk allocation, efficient caching via `union`, and skip-on-iterate for deleted elements |
| `<debugging>` | Standardized debugging utilities; detect debug mode, manage breakpoints programmatically |
| `<simd>` | Unified SIMD (Single Instruction, Multiple Data) interface across CPU architectures (AVX, SSE, NEON, etc.) |
| `<linalg>` | Linear algebra operations using `<mdspan>` — matrix/vector multiplication, transformations |
| `<hazard_pointer>` | Lock-free, thread-safe single-writer / multi-reader pointer with RAII ownership |
| `<rcu>` | Read-Copy-Update: lock-free concurrent read/write/delete with atomic operations |

#### `constexpr` Everywhere

In C++26, virtually all standard containers and utilities become `constexpr`:

```cpp
// C++26: constexpr containers (not just vector and array)
constexpr std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
constexpr std::set<int> primes = {2, 3, 5, 7, 11};
constexpr std::deque<int> dq = {1, 2, 3};

// Even exceptions become constexpr
// constexpr functions can now throw and catch at compile time
```

#### `std::execution` (Async Scheduling)

A comprehensive async programming framework with senders, receivers, and schedulers:

```cpp
// C++26: std::execution (conceptual — syntax may evolve)
#include <execution>

auto work = std::execution::just(42)
          | std::execution::then([](int x) { return x * 2; })
          | std::execution::then([](int x) { return x + 1; });
// Schedules: 42 → 84 → 85
```

> **Note:** NVIDIA provides an open-source reference implementation at [github.com/NVIDIA/stdexec](https://github.com/NVIDIA/stdexec).

---

<br>

## 8. Practical Patterns for Coding Tests

### 8.1 Reading Input Until EOF

```cpp
#include <iostream>
using namespace std;

int main() {
    int x;
    // Method 1: while (cin >> x)
    while (cin >> x) {
        cout << x * 2 << endl;
    }

    // Method 2: reading lines
    string line;
    while (getline(cin, line)) {
        cout << line << endl;
    }

    // Method 3: known count
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        // process x
    }

    return 0;
}
```

### 8.2 Sorting Custom Objects

```cpp
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

struct Person {
    string name;
    int age;
};

int main() {
    vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 30}};

    // Method 1: Lambda comparator (most flexible)
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        if (a.age != b.age) return a.age < b.age;
        return a.name < b.name;
    });

    // Method 2: Overload operator< in the struct
    // struct Person {
    //     ...
    //     bool operator<(const Person& other) const {
    //         if (age != other.age) return age < other.age;
    //         return name < other.name;
    //     }
    // };
    // sort(people.begin(), people.end());  // uses operator<

    return 0;
}
```

### 8.3 Frequency Counting with map

```cpp
#include <map>
#include <vector>
using namespace std;

vector<int> nums = {1, 3, 2, 3, 1, 1, 2, 4};
map<int, int> freq;

for (int x : nums) {
    freq[x]++;
}
// freq = {1: 3, 2: 2, 3: 2, 4: 1}

// Find the most frequent element
int maxVal = 0, maxKey = 0;
for (const auto& [key, val] : freq) {   // C++17 structured bindings
    if (val > maxVal) {
        maxVal = val;
        maxKey = key;
    }
}
// maxKey = 1, maxVal = 3
```

### 8.4 Unique Elements with set

```cpp
#include <set>
#include <vector>
using namespace std;

vector<int> nums = {1, 3, 2, 3, 1, 1, 2, 4};

// Method 1: set
set<int> unique_nums(nums.begin(), nums.end());
// unique_nums = {1, 2, 3, 4}

// Method 2: sort + unique + erase (in-place, preserves vector type)
sort(nums.begin(), nums.end());
nums.erase(unique(nums.begin(), nums.end()), nums.end());
// nums = {1, 2, 3, 4}
```

### 8.5 Stack and Queue

**Header:** `#include <stack>` and `#include <queue>`

```cpp
#include <stack>
#include <queue>
using namespace std;

// Stack — LIFO (Last In, First Out)
stack<int> st;
st.push(10);
st.push(20);
st.push(30);
cout << st.top() << endl;    // 30
st.pop();                     // removes 30
cout << st.top() << endl;    // 20
cout << st.size() << endl;   // 2
cout << st.empty() << endl;  // false

// Queue — FIFO (First In, First Out)
queue<int> q;
q.push(10);
q.push(20);
q.push(30);
cout << q.front() << endl;   // 10
cout << q.back() << endl;    // 30
q.pop();                      // removes 10
cout << q.front() << endl;   // 20
```

| | `stack<T>` | `queue<T>` |
|:--|:-----------|:-----------|
| Add | `push(val)` | `push(val)` |
| Remove | `pop()` | `pop()` |
| Access | `top()` | `front()`, `back()` |
| Order | LIFO | FIFO |

### 8.6 priority_queue for Top-K Problems

**Header:** `#include <queue>`

`priority_queue` is a **max-heap** by default — the largest element is always on top.

```cpp
#include <queue>
#include <vector>
using namespace std;

// Max-heap (default)
priority_queue<int> maxPQ;
maxPQ.push(30);
maxPQ.push(10);
maxPQ.push(50);
maxPQ.push(20);
cout << maxPQ.top() << endl;   // 50 (largest)
maxPQ.pop();
cout << maxPQ.top() << endl;   // 30

// Min-heap
priority_queue<int, vector<int>, greater<int>> minPQ;
minPQ.push(30);
minPQ.push(10);
minPQ.push(50);
minPQ.push(20);
cout << minPQ.top() << endl;   // 10 (smallest)

// Top-K smallest elements using max-heap of size K
vector<int> data = {7, 3, 9, 1, 5, 8, 2, 6, 4, 10};
int k = 3;
priority_queue<int> topK;

for (int x : data) {
    topK.push(x);
    if (topK.size() > k) {
        topK.pop();             // remove the largest — keep only K smallest
    }
}
// topK contains: {3, 2, 1} — the 3 smallest elements
```

### 8.7 pair and make_pair

**Header:** `#include <utility>` (also included automatically by `<map>`, `<algorithm>`, etc.)

```cpp
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

pair<int, string> p1 = {42, "answer"};
pair<int, string> p2 = make_pair(42, "answer");  // equivalent

cout << p1.first << endl;     // 42
cout << p1.second << endl;    // "answer"

// Pairs are compared lexicographically: first by .first, then by .second
pair<int, int> a = {1, 5};
pair<int, int> b = {1, 3};
cout << (a > b) << endl;      // true (same first, 5 > 3)

// Common pattern: store (value, index) pairs for sorting with original indices
vector<pair<int, int>> indexed;
vector<int> nums = {30, 10, 50, 20};
for (int i = 0; i < nums.size(); i++) {
    indexed.push_back({nums[i], i});
}
sort(indexed.begin(), indexed.end());
// indexed = {{10,1}, {20,3}, {30,0}, {50,2}}
// Now you know: smallest value 10 was originally at index 1
```

---

<br>

## 9. Template Compilation Notes

### 9.1 Template Definition Must Be in Header

Unlike regular classes, **template definitions cannot be separated** into `.h` and `.cpp` files in the usual way. The compiler needs to see the full template definition at the point of instantiation to generate the concrete code.

```cpp
// MyTemplate.h — CORRECT: put everything in the header
template <typename T>
class MyTemplate {
    T data;
public:
    MyTemplate(T d) : data(d) { }
    T getData();
};

template <typename T>
T MyTemplate<T>::getData() {
    return data;     // definition must also be in the header
}
```

If you put the template member function definitions in a `.cpp` file, the linker will produce an **"undefined reference"** error because the compiler cannot instantiate the template when it compiles other translation units.

```
// BAD: MyTemplate.cpp — will cause linker error
template <typename T>
T MyTemplate<T>::getData() {
    return data;
}
// Other .cpp files using MyTemplate<int> will fail to link!
```

> **Rule:** Keep both template class declarations and member function definitions in the **header file**.

### 9.2 Common Errors

**Missing operator for custom types:**

```cpp
template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;   // requires operator> for type T
    else return b;
}

class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) { }
};

Point p1(1, 2), p2(3, 4);
// bigger(p1, p2);   // compile error — Point has no operator>
```

To fix this, overload `operator>` in the `Point` class:

```cpp
class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) { }
    bool operator>(const Point& other) const {
        return (x + y) > (other.x + other.y);
    }
};

Point p1(1, 2), p2(3, 4);
Point p3 = bigger(p1, p2);  // now works — uses operator>
```

**Type mismatch:**

```cpp
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}

// myMax(3, 7.5);    // compile error — T cannot be both int and double
myMax<double>(3, 7.5);  // OK — explicitly set T to double
```

> **Tip:** When a template function is called with arguments of different types and the template has a single type parameter, the compiler cannot deduce `T`. Either use explicit instantiation or add a second template parameter.

---

<br>

## 10. Common Mistakes

### 10.1 Iterator Invalidation After insert/erase

Inserting or erasing elements in a `vector` can **invalidate** existing iterators. Using an invalidated iterator causes **undefined behavior**.

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// WRONG — iterator invalidated after erase
for (auto it = v.begin(); it != v.end(); it++) {
    if (*it % 2 == 0) {
        v.erase(it);   // BAD: 'it' is now invalid, and the next it++ is undefined
    }
}

// CORRECT — erase returns the next valid iterator
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0) {
        it = v.erase(it);   // erase returns iterator to next element
    } else {
        it++;
    }
}
// v = {1, 3, 5}
```

### 10.2 Using [] on map Creates Default Entry

The `[]` operator on a map **inserts a default-constructed value** if the key does not exist. This is a very common source of bugs.

```cpp
map<string, int> m;

// WRONG — this creates an entry {"Alice", 0} even though we just wanted to check!
if (m["Alice"] == 0) {
    cout << "Alice not found" << endl;
}
// m now contains {"Alice": 0} — unintended side effect!

// CORRECT — use find() or count() to check existence
if (m.find("Alice") == m.end()) {
    cout << "Alice not found" << endl;
}
// or
if (m.count("Alice") == 0) {
    cout << "Alice not found" << endl;
}
```

### 10.3 Forgetting #include for Each STL Component

Each STL component requires its own header. Unlike some languages, C++ does not have a single "import all" statement.

```cpp
// Each needs its own #include
#include <vector>           // vector
#include <string>           // string
#include <map>              // map
#include <unordered_map>    // unordered_map
#include <set>              // set
#include <unordered_set>    // unordered_set
#include <algorithm>        // sort, find, reverse, etc.
#include <numeric>          // accumulate, iota, etc.
#include <stack>            // stack
#include <queue>            // queue, priority_queue
#include <utility>          // pair, make_pair
#include <optional>         // optional (C++17)
```

> **Tip:** Some compilers (especially with `<bits/stdc++.h>`) may compile even without the correct includes. But this is non-standard and non-portable. Always include the correct headers.

### 10.4 Range-based for: Copy vs Reference

```cpp
vector<string> names = {"Alice", "Bob", "Charlie"};

// COPY — each iteration copies the string (expensive for large objects!)
for (auto x : names) {
    x = "Modified";       // modifies the copy, not the original
}
// names is unchanged: {"Alice", "Bob", "Charlie"}

// REFERENCE — no copy, modifies the original
for (auto& x : names) {
    x = "Modified";       // modifies the original vector
}
// names is now: {"Modified", "Modified", "Modified"}

// CONST REFERENCE — no copy, read-only (BEST for read access)
for (const auto& x : names) {
    cout << x << endl;    // efficient, safe
    // x = "change";      // compile error — const
}
```

> **Rule of Thumb:**
> - `for (auto x : v)` — use only for cheap-to-copy types (`int`, `char`, `double`)
> - `for (const auto& x : v)` — use for read-only access to any type
> - `for (auto& x : v)` — use when you need to modify elements

---

<br>

## Summary

| Concept | Key Takeaway |
|:--------|:-------------|
| Generic programming | Write once, works for any type; eliminates code duplication |
| Function template syntax | `template <typename T>` before function; `T` is type placeholder |
| Template instantiation | Compiler generates concrete function per type; implicit or explicit |
| Multiple parameters | `template <typename T1, typename T2>` for different types |
| Class template syntax | `template <typename T> class Name { };` member definitions use `Name<T>::` |
| Class instantiation | `Name<int> obj;` generates a separate class for each type |
| Operator overloading in templates | Custom types must define operators used by template logic (e.g., `operator>`) |
| Default template parameters | `template <typename T = int>` — default type if none specified |
| `vector` | Dynamic array; `push_back`, `[]`, `at()`, `front()`, `back()`, `size()`, `capacity()`, `reserve()` |
| 2D vector | `vector<vector<int>> matrix(rows, vector<int>(cols, 0));` |
| `string` | `std::string` replaces C-style `char[]`; `+`, `find`, `substr`, `stoi`, `to_string` |
| `map` / `unordered_map` | Key-value store; `map` is sorted O(log n), `unordered_map` is hash O(1) average |
| `set` / `unordered_set` | Unique elements; `set` is sorted O(log n), `unordered_set` is hash O(1) average |
| `sort` | `sort(begin, end)` or `sort(begin, end, comparator)` with lambda |
| `find`, `count` | Linear search and count on iterator ranges |
| `binary_search` | O(log n) search on sorted ranges; `lower_bound`, `upper_bound` |
| `accumulate` | Sum/reduce from `<numeric>` with initial value |
| `auto` (C++11) | Type deduction; cleaner code with iterators and complex types |
| Range-based for (C++11) | `for (const auto& x : container)` — clean iteration |
| Lambda (C++11) | `[capture](params){ body }` — inline functions for sort, for_each |
| Structured bindings (C++17) | `auto [key, value] = pair;` — decompose pairs, tuples, structs |
| `std::optional` (C++17) | Safe nullable values with `has_value()`, `value_or()` |
| `stack` / `queue` | LIFO and FIFO adapters; `push`, `pop`, `top`/`front` |
| `priority_queue` | Max-heap by default; `greater<>` for min-heap; useful for top-K |
| `pair` | `pair<T1,T2>`; compared lexicographically; useful with `sort` |
| Templates in headers | Full template definition must be in the header; `.cpp` separation causes linker errors |
| Iterator invalidation | `insert`/`erase` on vector invalidates iterators; use return value of `erase()` |
| `map[]` creates entries | Use `find()` or `count()` to check existence, not `[]` |
| Copy vs reference in for | `for(auto x:v)` copies; `for(auto& x:v)` references; `for(const auto& x:v)` read-only |

---
