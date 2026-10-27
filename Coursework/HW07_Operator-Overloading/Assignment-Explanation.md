# HW7 Assignment Explanation — Operator Overloading

[Repository Guide](../../README.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

> **Learning Objectives:**
> 1. Explain how an operator expression is turned into a call to a member operator function, and write `+=`, `-=`, and `==` as member functions of a class
> 2. Explain how the result of a chained expression differs when a compound assignment operator returns a copy of the object and when it returns a reference
> 3. Overload operator functions with different parameter types and determine which function is called from the argument types and the implicit conversions
> 4. Design the copy constructor, the destructor, the assignment operator, and several operator functions together for a string class with dynamic memory

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Assignment Layout and How to Run](#1-assignment-layout-and-how-to-run)
- [2. Q1. The Book Class with Compound Assignment Operators](#2-q1-the-book-class-with-compound-assignment-operators)
  - [2.1 Requirements](#21-requirements)
  - [2.2 Design and Solution](#22-design-and-solution)
  - [2.3 Output and Review](#23-output-and-review)
- [3. Q2. The Book Class with Two Comparison Operators](#3-q2-the-book-class-with-two-comparison-operators)
  - [3.1 Requirements](#31-requirements)
  - [3.2 Design and Solution](#32-design-and-solution)
  - [3.3 Output and Review](#33-output-and-review)
- [4. Q3. The MyString Class That Overloads Several Operators](#4-q3-the-mystring-class-that-overloads-several-operators)
  - [4.1 Requirements](#41-requirements)
  - [4.2 Design and Solution](#42-design-and-solution)
  - [4.3 Output and Review](#43-output-and-review)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Assignment Layout and How to Run

This assignment practices the operator overloading of Chapter 7. Q1 and Q2 are [Lab 2-2] and [Lab 3] of the Week 12 lecture slides, and Q3 is free code that uses operator overloading. The assignment handout is in [`Original/Assignment.md`](Original/Assignment.md), and a walkthrough of each program from start to finish is in the [HW7 Code Walkthrough](Code-Walkthrough.md).

```text
Coursework/HW07_Operator-Overloading/
├── Original/
│   ├── Assignment.md        # assignment handout (English)
│   └── Assignment.ko.md     # assignment handout (Korean)
└── Solution/
    ├── Q1.cpp               # the Book class that overloads += and -= as member functions
    ├── Q2.cpp               # the Book class with two == operators that compare with a string and with a book
    └── Q3.cpp               # the MyString class that overloads =, +, <, ==, +=, and []
```

*Figure 1. Directory layout of HW7*

Each solution is compiled and run as follows. All three files compile with `g++ -std=c++11 -Wall` and `g++ -std=c++17 -Wall` under Apple clang 21 on macOS.

```bash
cd Coursework/HW07_Operator-Overloading/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

| File | Compilation |
|:-----|:-----|
| `Q1.cpp` | No warnings |
| `Q2.cpp` | No warnings |
| `Q3.cpp` | No warnings |

*Table 1. Compilation results of the HW7 solutions*

When a program runs in Xcode, the last console line, `Program ended with exit code: 0`, is not program output but a message Xcode shows after the program ends. The `main()` of Q1 has no `return` statement, but in C++ reaching the end of `main()` is treated as returning 0, so the exit code is 0. The `main()` functions of Q2 and Q3 end with `return 0;`.

---

<br>

## 2. Q1. The Book Class with Compound Assignment Operators

### 2.1 Requirements

Add the `+=` and `-=` operator functions to the `Book` class as member functions so that `a += 500;` raises the price of the book `a` by 500 won and `b -= 500;` lowers the price of the book `b` by 500 won. The problem also asks that code assigning the result of the operation to another object, such as `c = a += 500;`, be taken into account. Each expression is turned into a call to an operator function as follows.

| Expression | Call to the operator function | What it does |
|:-----|:-----|:-----|
| `a += 500` | `a.operator+=(500)` | Adds 500 to `price` of `a` |
| `b -= 500` | `b.operator-=(500)` | Subtracts 500 from `price` of `b` |
| `c = a += 500` | `c.operator=(a.operator+=(500))` | The assignment operators associate from right to left, so the price of `a` is raised first and the result is assigned to `c` |

*Table 2. Operator expressions of Q1 and the calls to the operator functions*

### 2.2 Design and Solution

The program includes `<iostream>` for input/output and uses the standard namespace by default with `using namespace std;`. The `Book` class represents one book and holds the title `title`, the price `price`, and the number of pages `pages` as `private` member variables. The three members are declared at the very beginning of the class without an access specifier, so they are `private` by the default access of a `class`. The constructor, `show()`, the get/set functions, and the two operator functions are under `public`.

- **Constructor:** the default values of the parameters `title`, `price`, and `pages` are `""`, 0, and 0. Because the parameters have the same names as the member variables, the member variables are referred to through the `this` pointer, as in `this->title = title;`.
- **`show()`:** prints the title, the price, and the number of pages of the book on one line.
- **`getTitle()`, `getPrice()`, `setPrice()`:** get functions that return the title and the price and a set function that changes the price. They are not used in this `main()`; they are needed when code outside the class, such as the ordinary function `Sub()` of [Lab 2-1], changes the `private` member `price`.
- **`operator+=(int op)`:** raises the price with `this->price += op;` and returns the object with `return *this;`.
- **`operator-=(int op)`:** lowers the price with `this->price -= op;` and returns the object in the same way.

Because the two operator functions are member functions, the left operand object becomes `this`, and the right operand 500 is passed as the parameter `op` ([Chapter 7, Section 2.2](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#22-concept)). `main()` creates `a` and `b`, changes the prices with `a += 500;` and `b -= 500;`, and prints the information of the two books with `show()`. The price of `a` changes from 20000 to 20500, and the price of `b` from 30000 to 29500.

Judging by `return *this;` alone, the functions seem to return the object itself, but their return type is `Book`, so what is actually returned is a new object copied from `*this`. To return a reference to the object itself, the return type must be `Book&`. The difference shows up in expressions that use the result of the operation again.

- **`c = a += 500;`:** `a += 500` runs first and changes the price of `a`, and the returned copy is assigned to `c`. The copy has the same contents as `a` after the change, so this expression works as intended even when a copy is returned. The `c = a += 500;` that the problem asks for compiles with the current code, and the price of `c` becomes 20500, the same as that of `a`.
- **`(a += 500) += 100;`:** the second `+=` is applied to the temporary copy returned by the first `+=`, so the price of `a` rises by only 500. If this statement runs when `a` costs 20500 won, `a` becomes 21000 won, not 21100 won. If the return type is changed to `Book&`, the second `+=` is also applied to `a`, and the price becomes 21100 won.

For this reason a compound assignment operator usually returns a reference to `*this`, as in `Book& operator+=(int op)`. Returning a reference applies every chained operation to the same object and also removes the cost of copying the whole object, including its `string` member, on every return ([Chapter 7, Section 3.3](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#33-compound-assignment), [Chapter 7, Section 8](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#8-common-mistakes)).

### 2.3 Output and Review

```text
청춘 20500원 300페이지
미래 29500원 500페이지
```

The output gives the title, the price in won, and the number of pages of each book in Korean. The prices of the two books are 20500 won and 29500 won, as in the sample run of the problem. The solution uses `string` without including `<string>`; it compiles because `<iostream>` of libc++ on macOS includes `<string>` indirectly. The standard does not guarantee this, so portable code includes `<string>` directly. The same holds for Q2 and Q3.

---

<br>

## 3. Q2. The Book Class with Two Comparison Operators

### 3.1 Requirements

Add two `==` operator functions to the `Book` class as member functions. `a == "명품 C++"` compares the title of the book with a string, and according to the comment in `main()`, `a == b` compares all of the title, the price, and the number of pages of the two books. The further question asks how to provide a `!=` operator function as a member function.

| Expression | Call to the operator function | What is compared |
|:-----|:-----|:-----|
| `a == "명품 C++"` | `a.operator==(string("명품 C++"))` | The title of `a` and the string |
| `a == b` | `a.operator==(b)` | The two books (according to the comment, title, price, and pages) |

*Table 3. The two `==` expressions of Q2 and the calls to the operator functions*

### 3.2 Design and Solution

The member variables, the constructor, and `show()` of the `Book` class are the same as in Q1, and the only get function is `getTitle()`. The `==` operator function is overloaded as two functions with different parameter types.

- **`operator==(string op)`:** returns `true` if `this->title == op` and `false` otherwise.
- **`operator==(Book op)`:** returns `true` if `this->title == op.title` and `false` otherwise. `op` is an object of the same class, so the member function can access its `private` member directly, as in `op.title`.

`main()` creates `a` and `b`, which differ only in the title and have the same price and number of pages. `a == "명품 C++"` is true, so the program prints `명품 C++ 입니다.`; `a == b` is false because the titles differ, so it prints `두 책은 다른 책입니다.`.

The right operand of `a == "명품 C++"` is a string literal, that is, a `const char` array. One user-defined conversion through a constructor of `string` turns this value into a `string`, so `operator==(string)` is called. To become a `Book`, it would need two user-defined conversions, from `const char*` to `string` and from `string` to `Book`, but an implicit conversion allows only one user-defined conversion, so `operator==(Book)` is not a candidate ([Chapter 6, Section 1.2](../../Lecture-Notes/Ch06_Overloading-and-Static-Members/Concepts_Lecture.md#12-resolution-rules)). Indeed, if `operator==(string)` is removed, the expression no longer compiles and gives the error `invalid operands to binary expression ('Book' and 'const char[11]')`.

Three points can be considered to make the code more precise.

- **The members compared:** the comment in `main()` says that the title, the price, and the number of pages are all compared, but `operator==(Book)` compares only the title. In this `main()` the titles of the two books differ, so the output is correct, but a book with the same title and a different price and number of pages, such as `Book c("명품 C++", 10000, 100);`, is also judged to be the same book in `a == c`. To compare as the comment says, write `return title == op.title && price == op.price && pages == op.pages;`.
- **How the parameters are passed:** the two functions take `string` and `Book` by value, so the string and the object are copied on every call. A comparison operator does not change its operands, so it usually takes a `const` reference and is declared as a `const` member function, as in `bool operator==(const Book& op) const` ([Chapter 7, Section 3.4](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#34-comparison-operators)).
- **Simplifying the conditional:** `if (condition) return true; else return false;` is the same as returning the value of the comparison directly, as in `return this->title == op;`.

The `!=` operator of the further question can be implemented by using the `==` already defined and negating its result.

```cpp
bool operator != (Book op) {
    return !(*this == op);
}
```

`*this == op` calls `this->operator==(op)`, so if the comparison criterion of `==` changes, `!=` follows it and the two operators always give opposite results. If this function is added to the `Book` class of Q2, `a != b` is `true`, and for the `c` above, `a != c` is `false`. The `!=` that compares with a string is made in the same way as `bool operator != (string op) { return !(*this == op); }`.

### 3.3 Output and Review

```text
명품 C++ 입니다.
두 책은 다른 책입니다.
```

The output, which reads "This is 명품 C++." and "The two books are different books." in Korean, matches the sample run of the problem. The second line results from the different titles of `a` and `b`; if the program runs with two books that have the same title and differ only in price, the current code prints `두 책이 같은 책입니다.` (the two books are the same book).

---

<br>

## 4. Q3. The MyString Class That Overloads Several Operators

### 4.1 Requirements

Freely write code that uses operator overloading with an example different from the textbook. The solution creates a `MyString` class that stores a C string in dynamic memory and overloads the following six operators as member functions.

| Operator | Operator function | What it does |
|:-----|:-----|:-----|
| `=` | `MyString& operator=(const MyString&)` | Assigns by deep copy and returns a reference to `*this` |
| `+` | `MyString operator+(const MyString&)` | Returns by value a new object that joins the two strings |
| `<` | `bool operator<(const MyString&)` | Compares the lexicographic order with `strcmp()` |
| `==` | `bool operator==(const MyString&)` | Checks with `strcmp()` whether the two strings are equal |
| `+=` | `MyString& operator+=(const MyString&)` | Appends a string at the end and returns a reference to `*this` |
| `[]` | `char& operator[](int)` | Checks the range of the index and returns a reference to the character at that position |

*Table 4. Operators overloaded in Q3*

### 4.2 Design and Solution

The program includes `<iostream>` for input/output and `<cstring>` for `strlen()`, `strcpy()`, `strcat()`, and `strcmp()`. The `MyString` class represents one string and holds as `private` member variables the pointer `str` to the dynamic array that stores the string and the length of the string `len`. The constructor, the copy constructor, the destructor, the six operator functions, and `show()` are under `public`.

- **Constructor:** takes a `const char*` (default value `""`), finds the length with `strlen()`, allocates `len + 1` `char` elements so that the null character also fits, and copies the string with `strcpy()`.
- **Copy constructor:** does not copy the pointer of the other object as it is but performs a deep copy, which allocates new memory and copies the contents of the string.
- **Destructor:** frees the allocated array with `delete[]`.
- **`operator=`:** first filters out assignment to itself with `this == &s`. It then frees the existing array, allocates a new array, and copies the string, and it returns a reference to `*this` so that chains such as `s1 = s2 = s3` work. Without the self-assignment check, `s1 = s1;` would free its own array first and then read the freed array.
- **`operator+`:** copies the two strings one after the other into a temporary array whose size is the sum of the two lengths plus 1, creates the result object `result` from that array, frees the temporary array, and returns `result` by value. The two operands do not change.
- **`operator<`, `operator==`:** use the property that `strcmp()` returns a negative value when the first string comes first lexicographically and 0 when the two strings are equal.
- **`operator+=`:** builds the joined string in the same way as `+`, but frees the existing array and makes `str` point to the new array, which changes the object itself.
- **`operator[]`:** if the index is not at least 0 and less than `len`, prints `Index out of range` and ends the program with `exit(1)`. Within the range it returns a reference to `str[index]`, so it can also be used to change a character, as in `s2[0] = 'w';` ([Chapter 7, Section 6](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#6-subscript-operator)).
- **`show()`:** creates a `std::string` from `str` and returns it so that it can be printed with `cout`.

Because the class uses `new` in the constructor and `delete[]` in the destructor, the copy constructor and the assignment operator are defined together with them. If the compiler-generated version of either of them were used, two objects would point to the same array and a double free would follow, so this design follows the rule of three of Chapter 5 ([Chapter 5, Section 4.5](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.md#45-deep-copy)).

`main()` creates `s1` (`"Hello "`), `s2` (`"World!"`), and the empty string `s3` and prints the initial state; it then joins two strings with `s3 = s1 + s2;`, appends a string with `s1 += "C++";`, compares the lexicographic order with `a < b`, reads the first character with `s2[0]`, and checks whether two strings are equal with `str1 == str2`. When the program ends, the destructor of each object frees its dynamic memory.

The right operand of `s1 += "C++";` is a string literal, but `operator+=` takes a `const MyString&`. The constructor `MyString(const char* s = "")` is not marked `explicit`, so it serves as a converting constructor: the compiler creates a temporary `MyString` object from `"C++"`, passes it, and destroys the temporary when the statement ends. For the same reason `s1 + "!"` also compiles. However, `"Hi " + s1` is a compile error, because its left operand is not a `MyString` and the member function `operator+` cannot be a candidate. To also support expressions with a string literal on the left, `+` must be overloaded as a non-member function (a friend function), as in HW8 Q1 ([Chapter 7, Section 2.4](../../Lecture-Notes/Ch07_Friend-Functions-and-Operator-Overloads/Concepts_Lecture.md#24-member-function-vs-friend-function-decision-guide)).

Four points can be considered to make the code more robust.

- **Header includes:** `exit()` is declared in `<cstdlib>` and `std::string` in `<string>`. With libc++ on macOS, `<iostream>` includes these headers indirectly, so the code compiles, but the standard does not guarantee this, so including them directly is more portable.
- **`const` member functions:** `<`, `==`, `+`, and `show()` do not change the object, but they are not `const` member functions, so they cannot be used on `const` objects. For `const MyString c("Cherry");`, `a < c` compiles because the parameter is a `const MyString&`, but `c < a` gives the error `invalid operands to binary expression ('const MyString' and 'MyString')`. Declaring the function as `bool operator<(const MyString& s) const` makes both expressions compile.
- **Using `exit()`:** `exit(1)` ends the program immediately, so the destructors of the local objects of `main()` are not called. The operating system reclaims the memory of the process, so this is not a problem in this program, but to let the caller handle the error, the usual way is to report it with C++ exception handling (`throw` with `try` and `catch`).
- **An unnecessary check:** the `if(str)` in the destructor and the assignment operator is always true because `str` always points to an allocated array, and `delete[]` does nothing for a null pointer, so writing only `delete[] str;` without the check is enough.

### 4.3 Output and Review

```text
[초기 상태]
s1: Hello
s2: World!
s3:

[연산 수행]
s1 + s2 = Hello World!
s1 += 'C++' = Hello C++
Apple은 Banana보다 사전순으로 앞에 있습니다.
s2의 첫 번째 문자: W
두 문자열이 같습니다.
```

In the output, the headings read "[Initial State]" and "[Operations]", and the last three lines read "Apple comes before Banana in lexicographic order.", "The first character of s2: W", and "The two strings are equal." in Korean. The `s1:` line ends with the space of `"Hello "`, and the `s3:` line ends with the space of the output string `"s3: "`. The single quotes on the line `s1 += 'C++' = Hello C++` are characters contained in the output string `"s1 += 'C++' = "`. `s3` becomes `"Hello World!"`, and `s1` becomes `"Hello C++"` with `"C++"` appended to `"Hello "`. Although it does not show in this output, running the program with `leaks --atExit` on macOS reports 0 leaks, and building and running it with AddressSanitizer (`-fsanitize=address`) reports no errors, so every allocated array is freed exactly once.

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Calling a member operator function | `a += 500` is turned into `a.operator+=(500)`, so the left operand becomes `this` and the right operand is passed as the parameter. |
| Return of a compound assignment operator | Returning `Book` yields a copy, so the second operation of `(a += 500) += 100` is applied to a temporary object; returning `Book&` applies the chain to the same object. |
| Overloading operator functions | `operator==(string)` and `operator==(Book)`, which have different parameter types, can coexist, and the argument type decides which function is called. |
| Limits of implicit conversion | An implicit conversion allows only one user-defined conversion, so a string literal can become a `string` but not a `Book`. |
| Implementing `!=` | Implementing it by negating the result of the existing `==`, as in `return !(*this == op);`, keeps the two operators always consistent. |
| A dynamic string class | A class with a pointer member has a copy constructor and an assignment operator that deep-copy and a destructor that frees the array. |
| Converting constructor | `MyString(const char*)`, which is not `explicit`, turns the string literal in `s1 += "C++"` into a temporary `MyString` object. |

---

<br>

## Self-Check Questions

1. **Calling an operator function directly:** Write `b -= 500;` of Q1 as a direct call to the operator function.

   <details>
   <summary>Answer</summary>

   It is `b.operator-=(500);`. `operator-=` is a member function, so it is called as a member function of the left operand `b`, and 500 is passed as the parameter `op`. Written this way, the result is the same as `b -= 500;`.

   </details>

2. **The result of a chain:** If `(a += 500) += 100;` and `a.show();` are added at the end of the `main()` of Q1, what does the last line print? How does this change if the return type is changed to `Book&`?

   <details>
   <summary>Answer</summary>

   `a` goes from 20500 won to 21000 won through `a += 500`, but `+= 100` is applied to the returned copy, so `청춘 21000원 300페이지` is printed. If the return type is `Book&`, the second `+=` is also applied to `a`, and `청춘 21100원 300페이지` is printed.

   </details>

3. **The comparison criterion:** If `Book c("명품 C++", 10000, 100);` is created in Q2 and `a == c` is tested, what is the result? How would you change the code so that it works as the comment in `main()` says?

   <details>
   <summary>Answer</summary>

   `operator==(Book)` compares only the titles, so the result is `true`. To compare all of the title, the price, and the number of pages as the comment says, change the body of the function to `return title == op.title && price == op.price && pages == op.pages;`. Then `a == c` becomes `false`.

   </details>

4. **Choosing the function:** If `operator==(string)` is removed in Q2, what happens to `a == "명품 C++"`? What about `a == string("명품 C++")`?

   <details>
   <summary>Answer</summary>

   `a == "명품 C++"` becomes a compile error, because the string literal would need two user-defined conversions through `string` to become a `Book`, while an implicit conversion allows only one. `a == string("명품 C++")` needs only one conversion, from `string` to `Book` through the constructor, so it compiles, and it is `true` because the titles are the same.

   </details>

5. **The left operand:** Why does `s1 + "!"` compile in Q3 while `"Hi " + s1` does not, and how would you fix it?

   <details>
   <summary>Answer</summary>

   The member function `operator+` is called as a member of the left operand, so the left operand must be a `MyString`. `s1 + "!"` becomes `s1.operator+("!")`, and `"!"` becomes a `MyString` through the converting constructor, but the left operand of `"Hi " + s1` is a `const char` array, on which no member function can be called. If `+` is overloaded as a non-member function that takes both operands as parameters, as in `friend MyString operator+(const MyString& a, const MyString& s);`, the string literal on the left also becomes a `MyString` through the converting constructor, and `"Hi " + s1` works, giving `"Hi Hello C++"`.

   </details>
