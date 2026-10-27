# HW7 Code Walkthrough — Operator Overloading

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

> **Learning Objectives:**
> 1. Read the class declarations, the operator functions, and `main()` of the three solutions line by line and explain the role of each part
> 2. Trace the order in which operator functions, constructors, and destructors are called when an operator expression runs
> 3. Find in the code how the return type of an operator function and the way its parameters are passed affect the state of objects and the copies made

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: Compound Assignment Operators of the Book Class](#2-q1cpp-compound-assignment-operators-of-the-book-class)
  - [2.1 Full Code](#21-full-code)
  - [2.2 Line-by-Line Explanation](#22-line-by-line-explanation)
  - [2.3 Execution Trace](#23-execution-trace)
- [3. Q2.cpp: Comparison Operators of the Book Class](#3-q2cpp-comparison-operators-of-the-book-class)
  - [3.1 Full Code](#31-full-code)
  - [3.2 Line-by-Line Explanation](#32-line-by-line-explanation)
  - [3.3 Execution Trace](#33-execution-trace)
- [4. Q3.cpp: The MyString Class](#4-q3cpp-the-mystring-class)
  - [4.1 Full Code](#41-full-code)
  - [4.2 Line-by-Line Explanation](#42-line-by-line-explanation)
  - [4.3 Execution Trace](#43-execution-trace)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the three solutions puts, in one file, the header includes, the class declaration, and `main()`, in that order, and every member function is defined with its body inside the class. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW7 Assignment Explanation](Assignment-Explanation.md).

| File | Class | Overloaded operators | Input |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Book` | `+=`, `-=` | None |
| [`Q2.cpp`](Solution/Q2.cpp) | `Book` | `==` (one with a `string` parameter and one with a `Book` parameter) | None |
| [`Q3.cpp`](Solution/Q3.cpp) | `MyString` | `=`, `+`, `<`, `==`, `+=`, `[]` | None |

*Table 1. Layout of the HW7 solution files*

---

<br>

## 2. Q1.cpp: Compound Assignment Operators of the Book Class

### 2.1 Full Code

```cpp
/**
 * @file    A7Q1.cpp
 * @brief   Q1. 멤버 함수로 중복한 +=, -= 연산자 (12주차 실습 2-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
using namespace std;

class Book {
    string title;
    int price;
    int pages;
public:
    Book (string title = "", int price = 0, int pages = 0) {
        this->title = title;
        this->price = price;
        this->pages = pages;
    }
    void show() {
        cout << title << ' ' << price << "원 " << pages << "페이지" << endl;
    }
    
    string getTitle() { return title; }
    int getPrice() { return price; }
    void setPrice(int p) { price = p; }
    Book operator += (int op) {
        this->price += op;
        return *this;
    }
    Book operator -= (int op) {
        this->price -= op;
        return *this;
    }
};

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    a += 500;
    b -= 500;
    
    a.show();
    b.show();
}
```

### 2.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-9 | `#include <iostream>`, `using namespace std;` | Includes `<iostream>` for `cout`, `endl`, and `string`, and omits `std::`. `<string>` is not included |
| 11-14 | `class Book { string title; int price; int pages;` | The three member variables declared without an access specifier are `private` by the default access of a `class` |
| 15 | `public:` | Declares the following members as `public` |
| 16-20 | `Book (string title = "", int price = 0, int pages = 0)` | A constructor with default parameters. The parameters have the same names as the member variables, so the member variables are referred to with `this->` in the assignments |
| 21-23 | `void show()` | Prints the title, a space, the price, `"원 "`, the number of pages, and `"페이지"` in turn and ends the line |
| 25-27 | `getTitle()`, `getPrice()`, `setPrice()` | Get/set functions defined inside the class; they are not called in this `main()` |
| 28-31 | `Book operator += (int op)` | Adds `op` to `price` and returns `*this`. Because the return type is `Book`, a copy of the object is returned |
| 32-35 | `Book operator -= (int op)` | Subtracts `op` from `price` and returns a copy of `*this` |
| 39 | `Book a("청춘", 20000, 300), b("미래", 30000, 500);` | Creates two books, giving all three parameters |
| 40 | `a += 500;` | Calls `a.operator+=(500)`, and `a.price` becomes 20500. The returned copy is not used and is destroyed when the statement ends |
| 41 | `b -= 500;` | Calls `b.operator-=(500)`, and `b.price` becomes 29500 |
| 43-44 | `a.show();`, `b.show();` | Prints the information of each book on one line with the changed prices |

*Table 2. Line-by-line explanation of Q1.cpp*

### 2.3 Execution Trace

| Statement executed | `a.price` | `b.price` | Output |
|:-----|-----:|-----:|:-----|
| `Book a(...), b(...);` | 20000 | 30000 | |
| `a += 500;` | 20500 | 30000 | (the returned copy is destroyed at once) |
| `b -= 500;` | 20500 | 29500 | (the returned copy is destroyed at once) |
| `a.show();` | 20500 | 29500 | `청춘 20500원 300페이지` |
| `b.show();` | 20500 | 29500 | `미래 29500원 500페이지` |

*Table 3. Prices of the two books as the `main()` of Q1 runs*

`title` and `pages` are set in the constructor and never change afterwards. This `main()` does not use the return values of the two operator functions, so the difference between returning a copy and returning a reference does not show in the output.

---

<br>

## 3. Q2.cpp: Comparison Operators of the Book Class

### 3.1 Full Code

```cpp
/**
 * @file    A7Q2.cpp
 * @brief   Q2. 두 가지 == 연산자를 갖춘 Book 클래스 (12주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
using namespace std;

class Book {
    string title;
    int price;
    int pages;
public:
    Book(string title = "", int price = 0, int pages = 0) {
        this->title = title;
        this->price = price;
        this->pages = pages;
    }
    void show() {
        cout << title << ' ' << price << "원 " << pages << "페이지" << endl;
    }
    string getTitle() {
        return title;
    }
    bool operator == (string op) {
        if (this->title == op)
            return true;
        else
            return false;
    }
    bool operator == (Book op) {
        if (this->title == op.title)
            return true;
        else
            return false;
    }
};

int main() {
    Book a("명품 C++", 30000, 500), b("고품 C++", 30000, 500);
    
    // 책 title 비교
    if (a == "명품 C++")
        cout << "명품 C++ 입니다." << endl;
    
    // title, price, pages 모두 비교
    if (a == b)
        cout << "두 책이 같은 책입니다." << endl;
    else
        cout << "두 책은 다른 책입니다." << endl;
    
    return 0;
}
```

### 3.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-9 | `#include <iostream>`, `using namespace std;` | Same as Q1 |
| 11-20 | Member variables and constructor | The same three `private` member variables and constructor with default parameters as in Q1 |
| 21-23 | `void show()` | The same output function as in Q1; it is not called in this `main()` |
| 24-26 | `string getTitle()` | Returns the title; it is not called in this `main()` |
| 27-32 | `bool operator == (string op)` | Takes the string `op` by value and returns `true` if it equals `this->title` and `false` otherwise |
| 33-38 | `bool operator == (Book op)` | Takes another book `op` by value and compares only the titles. The price and the number of pages are not compared |
| 42 | `Book a("명품 C++", 30000, 500), b("고품 C++", 30000, 500);` | Creates two books that differ only in the title and have the same price and number of pages |
| 44-46 | `if (a == "명품 C++")` | A temporary `string` is made from the string literal and passed to `a.operator==(string)`. The result is `true`, so line 46 is printed |
| 48-52 | `if (a == b) ... else ...` | `a.operator==(Book)` is called with `op` copied from `b`. The titles differ, so the result is `false`, and line 52 in the `else` branch is printed |
| 54 | `return 0;` | Returns the exit code 0 |

*Table 4. Line-by-line explanation of Q2.cpp*

### 3.3 Execution Trace

| Condition | Function called | Values compared | Result | Output |
|:-----|:-----|:-----|:-----|:-----|
| `a == "명품 C++"` | `operator==(string)` | `"명품 C++"` and `"명품 C++"` | `true` | `명품 C++ 입니다.` |
| `a == b` | `operator==(Book)` | `"명품 C++"` and `"고품 C++"` | `false` | `두 책은 다른 책입니다.` |

*Table 5. Evaluation of the two conditions of Q2*

Both functions take their parameter by value, so the `string` parameter `op` is created in the first condition and the `Book` parameter `op` copied from `b` in the second, and each is destroyed when the function ends. The copy of `Book` uses the default copy constructor generated by the compiler, and the member `title` is copied by the copy constructor of `string`.

---

<br>

## 4. Q3.cpp: The MyString Class

### 4.1 Full Code

```cpp
/**
 * @file    A7Q3.cpp
 * @brief   Q3. 연산자 중복 관련 자유 코드 (MyString 클래스)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
#include <cstring>
using namespace std;

class MyString {
    char* str;
    int len;
public:
    // 생성자
    MyString(const char* s = "") {
        len = strlen(s);
        str = new char[len + 1];
        strcpy(str, s);
    }
    
    // 복사 생성자
    MyString(const MyString& s) {
        len = s.len;
        str = new char[len + 1];
        strcpy(str, s.str);
    }
    
    // 소멸자
    ~MyString() {
        if(str) delete[] str;
    }
    
    // = 연산자 중복
    MyString& operator=(const MyString& s) {
        if(this == &s) return *this;
        if(str) delete[] str;
        len = s.len;
        str = new char[len + 1];
        strcpy(str, s.str);
        return *this;
    }
    
    // + 연산자 중복 (문자열 연결)
    MyString operator+(const MyString& s) {
        char* temp = new char[len + s.len + 1];
        strcpy(temp, str);
        strcat(temp, s.str);
        MyString result(temp);
        delete[] temp;
        return result;
    }
    
    // < 연산자 중복 (사전식 비교)
    bool operator<(const MyString& s) {
        return strcmp(str, s.str) < 0;
    }
    
    // == 연산자 중복
    bool operator==(const MyString& s) {
        return strcmp(str, s.str) == 0;
    }

    // += 연산자 중복
    MyString& operator+=(const MyString& s) {
        char* temp = new char[len + s.len + 1];
        strcpy(temp, str);
        strcat(temp, s.str);
        if(str) delete[] str;
        str = temp;
        len = len + s.len;
        return *this;
    }
    
    // [] 연산자 중복 (인덱스로 문자 접근)
    char& operator[](int index) {
        if(index < 0 || index >= len) {
            cout << "Index out of range" << endl;
            exit(1);
        }
        return str[index];
    }
    
    string show() {
        return str;
    }
};

int main() {
    MyString s1("Hello ");
    MyString s2("World!");
    MyString s3;
    
    cout << "[초기 상태]" << endl;
    cout << "s1: " << s1.show() << endl;
    cout << "s2: " << s2.show() << endl;
    cout << "s3: " << s3.show() << endl << endl;
    
    cout << "[연산 수행]" << endl;
    // + 연산자 사용
    s3 = s1 + s2;
    cout << "s1 + s2 = " << s3.show() << endl;
    
    // += 연산자 사용
    s1 += "C++";
    cout << "s1 += 'C++' = " << s1.show() << endl;
    
    // < 연산자 사용 (사전식 비교)
    MyString a("Apple");
    MyString b("Banana");
    if(a < b)
        cout << a.show() << "은 " << b.show() << "보다 사전순으로 앞에 있습니다." << endl;
    
    // [] 연산자 사용
    cout << "s2의 첫 번째 문자: " << s2[0] << endl;
    
    // == 연산자 사용
    MyString str1("Test");
    MyString str2("Test");
    if(str1 == str2)
        cout << "두 문자열이 같습니다." << endl;
    
    return 0;
}
```

### 4.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-10 | `#include`, `using namespace std;` | `<iostream>` provides input/output, and `<cstring>` provides `strlen()`, `strcpy()`, `strcat()`, and `strcmp()` |
| 12-14 | `class MyString { char* str; int len;` | The pointer to the dynamic array and the length of the string without the null character; both are `private` |
| 17-21 | `MyString(const char* s = "")` | Finds `len`, allocates `len + 1` `char` elements, and copies `s`. Created without an argument, the object holds the empty string |
| 24-28 | `MyString(const MyString& s)` | A deep-copy constructor that allocates a new array of the length of `s` and copies the contents |
| 31-33 | `~MyString()` | Frees the array that `str` points to with `delete[]` |
| 36-37 | Self-assignment check of `operator=` | If `this` and `&s` are equal, does nothing and returns `*this` |
| 38-42 | Body of `operator=` | Frees the existing array, allocates a new array, copies the string of `s`, and returns a reference to `*this` |
| 46-49 | Temporary array of `operator+` | Puts its own string into an array one larger than the sum of the two lengths with `strcpy()` and then the string of `s` with `strcat()` |
| 50-52 | `MyString result(temp); delete[] temp; return result;` | Creates the result object from the temporary array, frees the temporary array, and returns the result by value |
| 56-58 | `bool operator<(const MyString& s)` | `true` if the result of `strcmp()` is negative, that is, if its own string comes first lexicographically |
| 61-63 | `bool operator==(const MyString& s)` | `true` if the result of `strcmp()` is 0, that is, if the two strings are equal |
| 66-69 | Temporary array of `operator+=` | Builds the joined string in the new array `temp` in the same way as `operator+` |
| 70-73 | `if(str) delete[] str; str = temp; len = len + s.len; return *this;` | Frees the existing array, switches to the new array, updates the length, and returns a reference to `*this` |
| 77-81 | Range check of `operator[]` | If the index is less than 0 or at least `len`, prints a message and ends the program with `exit(1)` |
| 82 | `return str[index];` | Returns a reference to the character within the range, so it can be used for both reading and writing |
| 85-87 | `string show()` | Creates a `std::string` from `str`, which is a `char*`, and returns it |
| 91-93 | Creating `s1`, `s2`, `s3` | The constructor is called three times, with `"Hello "`, `"World!"`, and the default argument `""` |
| 95-98 | Printing the initial state | Prints the three strings and leaves one blank line with `endl << endl` at line 98 |
| 100 | `cout << "[연산 수행]" << endl;` | Prints the heading line of the operations part |
| 102-103 | `s3 = s1 + s2;` | Assigns the temporary object made by `s1.operator+(s2)` with `s3.operator=()` and prints the result |
| 106-107 | `s1 += "C++";` | Passes a temporary `MyString` made by the converting constructor to `s1.operator+=()`, changing `s1` to `"Hello C++"` |
| 110-113 | `a < b` | `"Apple"` comes before `"Banana"` lexicographically, so line 113 is printed |
| 116 | `s2[0]` | `s2.operator[](0)` returns a reference to the first character `'W'` |
| 119-122 | `str1 == str2` | The two objects point to different arrays but have the same contents, so the result is `true` |
| 124 | `return 0;` | As `main()` ends, the destructors of the seven local objects are called in the reverse order of creation |

*Table 6. Line-by-line explanation of Q3.cpp*

### 4.3 Execution Trace

| Statement executed | Functions called (in order) | Result |
|:-----|:-----|:-----|
| Lines 91-93 | Constructor three times | `s1` is `"Hello "` (length 6), `s2` is `"World!"` (length 6), `s3` is `""` (length 0) |
| `s3 = s1 + s2;` | `s1.operator+(s2)`, the constructor inside it (`result`), `s3.operator=(temporary)`, destructor of the temporary | `s3` is `"Hello World!"` (length 12) |
| `s1 += "C++";` | Constructor (temporary `"C++"`), `s1.operator+=(temporary)`, destructor of the temporary | `s1` is `"Hello C++"` (length 9) |
| `a < b` | Constructor twice (lines 110-111), `a.operator<(b)` | `strcmp("Apple", "Banana")` is negative, so `true` |
| `s2[0]` | `s2.operator[](0)` | `'W'` |
| `str1 == str2` | Constructor twice (lines 119-120), `str1.operator==(str2)` | `strcmp("Test", "Test")` is 0, so `true` |
| After `return 0;` | Destructor seven times, in the order `str2`, `str1`, `b`, `a`, `s3`, `s2`, `s1` | Every allocated array is freed |

*Table 7. Function calls as the `main()` of Q3 runs*

In `s3 = s1 + s2;`, `operator+` returns the local object `result` by value. Apple clang omits the copy through the named return value optimization (NRVO), which builds `result` directly in the place of the return value, so the copy constructor is not called, as in the table above. If the program is compiled with `-std=c++11` and the option `-fno-elide-constructors`, which turns copy elision off, the copy constructor that copies `result` into a temporary and the destructor of `result` are each called once more. The output is the same in either case.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Layout of a file | Each of the three solutions consists of header includes, a class declaration that defines every member function inside the class, and `main()`, in that order. |
| Reading the return type | `Book operator+=` of Q1 returns a copy of the object, while `MyString& operator+=` of Q3 returns a reference to the object itself. |
| Value parameters | `operator==(Book op)` of Q2 creates `op` copied from `b` on every call and destroys it when the function ends. |
| Lifetime of temporaries | The temporary `MyString` objects created in `s3 = s1 + s2` and `s1 += "C++"` free their arrays through the destructor when the statement ends. |
| Order of destruction | Local objects are destroyed in the reverse order of creation, so in Q3 the arrays are freed in turn from `str2` to `s1`. |

---

<br>

## Self-Check Questions

1. **A discarded return value:** What happens to the object returned by `a += 500;` at line 40 of Q1, and does it affect the price of `a`?

   <details>
   <summary>Answer</summary>

   The return type is `Book`, so a temporary object copied from `*this` is created, and it is destroyed unused when the statement ends. `a.price` has already been changed by `this->price += op;` inside the function, so the price of `a` stays 20500 even after the temporary is destroyed.

   </details>

2. **Copying a parameter:** Is `b` copied when `a == b` at line 49 of Q2 runs? How would you change line 33 so that it is not copied?

   <details>
   <summary>Answer</summary>

   It is copied. `op` of `operator==(Book op)` is a value parameter, so it is created by copying `b` with the default copy constructor. If line 33 is changed to `bool operator == (const Book& op)`, `op` becomes a reference to `b`, and no copy is made.

   </details>

3. **Length and array size:** After `s1 += "C++";` at line 106 of Q3 finishes, what are the value of `s1.len` and the size of the array that `s1.str` points to?

   <details>
   <summary>Answer</summary>

   `s1.len` is 6 + 3 = 9. Line 67 allocated `len + s.len + 1`, that is, 10 `char` elements, so the size of the array is 10, and the last element holds the null character.

   </details>

4. **An index out of range:** What happens if line 116 of Q3 is changed to `cout << "s2의 첫 번째 문자: " << s2[6] << endl;`?

   <details>
   <summary>Answer</summary>

   `s2.len` is 6, so the index 6 is caught by `index >= len` at line 78. Since C++17 the left operand of `<<` is evaluated first, so `s2의 첫 번째 문자: ` is printed first, `Index out of range` follows on the same line, and `exit(1)` ends the program with the exit code 1. Index 6 holds the null character, but the range check allows only the characters of the string. Line 117 and the following lines are not executed.

   </details>
