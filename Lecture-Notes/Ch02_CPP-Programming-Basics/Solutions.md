# Chapter 2 Solutions — C++ Programming Basics

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 2

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Statements About the `main()` Function](#exercise-1-statements-about-the-main-function)
- [Exercise 2: Declaring Variables After Statements](#exercise-2-declaring-variables-after-statements)
- [Exercise 3: Using `cout` Without `std::`](#exercise-3-using-cout-without-std)
- [Exercise 4: Correcting C++ Code](#exercise-4-correcting-c-code)
- [Exercise 5: Statements About C++ Strings](#exercise-5-statements-about-c-strings)
- [Exercise 6: Reading a Name with `cin`](#exercise-6-reading-a-name-with-cin)
- [Exercise 7: Reading a String with Spaces](#exercise-7-reading-a-string-with-spaces)
- [Exercise 8: The Namespace of the Standard Library](#exercise-8-the-namespace-of-the-standard-library)
- [Exercise 9: The Header File That Declares `cin` and `cout`](#exercise-9-the-header-file-that-declares-cin-and-cout)
- [Exercise 10: Printing Stars with Nested `for` Statements](#exercise-10-printing-stars-with-nested-for-statements)

---

<br>

## Exercise 1: Statements About the `main()` Function

**Textbook Problem:** Chapter 2 theory problem 2

**Problem:** Choose the wrong statement among four statements about the return type of `main()` and its `return` statement. The options are in [Exercise 1](Exercises.md#exercise-1-statements-about-the-main-function).

### Solution

**Answer:** ③

**Explanation:** The C++ standard allows the `return` statement to be omitted for the `main()` function only. When execution reaches the end of `main()`, the effect is the same as executing `return 0;`, so the program returns exit code 0 to the operating system ([Chapter 2, Section 1.3](Concepts_Lecture.md#13-main-function)). This exception applies only to `main()`; if any other function with a return type reaches its end without a `return` statement, the behavior is undefined.

The following program has no `return` statement, yet it compiles without warnings with `g++ -std=c++17 -Wall`, and its exit code after running is 0.

```cpp
#include <iostream>
int main() {
    std::cout << "no return statement\n";
}
```

The other statements are as follows.

- ① The C++ standard defines the return type of `main()` as `int`.
- ② `void main()` is not standard and is accepted only by compilers that allow it as an extension, such as Visual C++. Clang reports the error "'main' must return 'int'", so the statement does not hold for some compilers. For portability, always write `int main()`.
- ④ The return value of `main()` is the exit code passed to the operating system, and 0 means normal termination. A nonzero value can be returned to report an error, so `main()` does not have to return 0.

---

<br>

## Exercise 2: Declaring Variables After Statements

**Textbook Problem:** Chapter 2 theory problem 4

**Problem:** Explain why code that declares the variable `square` after the statement `a = 4;` is an error in C but valid in C++, and give the advantages and disadvantages of this feature. The full problem and code are in [Exercise 2](Exercises.md#exercise-2-declaring-variables-after-statements).

### Solution

**Answer:**

- (1) In C under the C89 (C90) standard, a variable declaration may appear only at the beginning of a block, that is, before the statements, so a compile error occurs at the third line, `int square = a * a;`, which follows the statement `a = 4;`.
- (2) In C++, a variable declaration is itself a statement, so a variable can be declared anywhere among the statements of a block.
- (3) The advantage is that a variable can be declared at the point where it is used, which makes the code convenient to write and reduces both the trouble of moving between the declaration part and the statements and typing errors in variable names. The disadvantage is that the declarations are scattered throughout the code, so it is hard to see the declared variables at a glance and to find a needed variable.

The error of (1) comes from the rules of C89. Since C99, C also allows variable declarations among statements, so current C compilers compile this code with their default settings. Compiling the following C program with a Clang option that strictly applies the C89 rules gives an error, while compiling it with `-std=c99` succeeds, and the program prints 16.

```c
#include <stdio.h>

int main(void) {
    int a;
    a = 4;
    int square = a * a;
    printf("%d\n", square);
    return 0;
}
```

```text
$ clang -std=c89 -pedantic-errors square.c
square.c:6:9: error: mixing declarations and code is a C99 extension [-Werror,-Wdeclaration-after-statement]
```

The scope of a variable runs from the point of its declaration to the end of its block. Therefore, if a variable is declared right before it is used and another variable name is mistyped, that name is not yet declared, so the mistake is caught early as a compile error. Conversely, if all variables are declared at the beginning of the block, the mistyped name is also an already declared variable, so only a wrong result appears without any error ([Chapter 2, Section 3.4](Concepts_Lecture.md#34-declaring-variables-between-statements)).

---

<br>

## Exercise 3: Using `cout` Without `std::`

**Textbook Problem:** Chapter 2 theory problem 6

**Problem:** Write the statement for the blank after `#include <iostream>` so that two programs that use `cout` without `std::` compile. The full problem and code are in [Exercise 3](Exercises.md#exercise-3-using-cout-without-std).

### Solution

**Answer:** (1) `using std::cout;` (2) `using namespace std;`

**Explanation:** `cout`, `cin`, and `endl` are all names declared in the `std` namespace, so using them without `std::` requires a using declaration or a using directive ([Chapter 2, Section 4.2](Concepts_Lecture.md#42-using-namespace-std)). If the blank is left empty, Clang reports the error "use of undeclared identifier 'cout'" for the `cout` of (1).

- (1) This program writes `cin` as `std::cin` and uses only `cout` without `std::`, so the using declaration `using std::cout;`, which brings in `cout` alone, fits. It also compiles with `using namespace std;`.
- (2) Both `cout` and `endl` are used without `std::`, so the using directive `using namespace std;`, which makes all the names of `std` usable, is written. Two using declarations on one line, as in `using std::cout; using std::endl;`, also work.

With the blanks filled, program (1) prints the entered integer plus 1 (entering 4 prints 5).

---

<br>

## Exercise 4: Correcting C++ Code

**Textbook Problem:** Chapter 2 theory problem 8

**Problem:** Find and correct the wrong parts in four lines of C++ code. The full problem is in [Exercise 4](Exercises.md#exercise-4-correcting-c-code).

### Solution

**Answer:** (1) and (2) have no error. Correct (3) to `int n=1; cout << n+200;` and (4) to `int year = 2014; cout << 2014 << "년";`.

- (1) A correct using declaration that brings in only `cin` from the `std` namespace.
- (2) A statement that declares an `int` variable and initializes it to 1; everything from `//` to the end of the line is a comment, so there is no error.
- (3) `cout` is an output stream, so the stream insertion operator `<<` must be used. `>>` is the extraction operator used with input streams such as `cin`, and no `>>` operation is defined between `ostream` and `int`, so the compile error "invalid operands to binary expression" occurs. The corrected code prints 201, the value of `n+200`.
- (4) `2014+"년"` does not concatenate strings. The string literal `"년"` converts to the address of its first character, so the expression becomes pointer arithmetic that points 2014 bytes past that address, which goes outside the array and is undefined behavior. It compiles, but Clang warns "adding 'int' to a string does not append to the string". To print an integer followed by a string, split them with `<<` as in `cout << 2014 << "년";`, which prints `2014년` ("2014 year" in Korean). To use the declared variable, `cout << year << "년";` also works.

---

<br>

## Exercise 5: Statements About C++ Strings

**Textbook Problem:** Chapter 2 theory problem 10

**Problem:** Answer O or X to five statements about C-strings and the `string` class. The full problem is in [Exercise 5](Exercises.md#exercise-5-statements-about-c-strings).

### Solution

**Answer:** (1) O, (2) O, (3) O, (4) X, (5) X

- (1) O. A C-string represents a string, as in the C language, by a `char` array that ends with the null character `'\0'` ([Chapter 2, Section 5.1](Concepts_Lecture.md#51-c-strings)).
- (2) O. C++ supports C-strings as they are for compatibility with C, and string literals such as `"C++"` are also C-strings.
- (3) O. The `string` class of the C++ standard library (the `<string>` header) handles strings as objects ([Chapter 2, Section 5.2](Concepts_Lecture.md#52-the-string-class)).
- (4) X. A null character `'\0'` is appended to the end of the string literal `"C++"`, so `name[]` becomes an array of size 4 that stores `'C'`, `'+'`, `'+'`, and `'\0'`. Printing `sizeof(name)` gives 4.
- (5) X. `>>` stops reading at a whitespace character, so it cannot read whitespace as part of the input. A string with spaces is read with `cin.getline()`.

> **Note:** The maximum number of characters in (5) depends on the version of the C++ standard. Up to C++17, `>>` does not know the size of the array, so entering 15 characters without spaces into `char name[10]` stores all 15 characters and overwrites memory outside the array. Since C++20, the array is taken by reference and its size is known, so only up to 9 characters are stored and the rest stay in the input buffer. With Apple clang, compiling with `-std=c++17` and with `-std=c++20` and entering `ABCDEFGHIJKLMNO` stores strings of length 15 and 9, respectively.

---

<br>

## Exercise 6: Reading a Name with `cin`

**Textbook Problem:** Chapter 2 theory problem 12

**Problem:** Give the results of entering Kitae and Kitae Hwang into a program that reads a name with `cin >> name` and prints a welcome message. The full problem and code are in [Exercise 6](Exercises.md#exercise-6-reading-a-name-with-cin).

### Solution

**Answer:** In both (1) and (2), `Kitae님 환영합니다` ("Welcome, Kitae" in Korean) is printed.

**Explanation:** (1) The entered `Kitae` is stored in `name`, so `Kitae님 환영합니다` is printed. The program prints no newline, but the `<Enter>` the user pressed is reflected on the screen, so the output appears on the next line.

```text
이름을 입력하세요?Kitae
Kitae님 환영합니다
```

(2) The `>>` operator skips leading whitespace and then reads only up to just before the next whitespace character, so only `Kitae` is stored in `name`, and the space after it and `Hwang` remain in the input buffer together with the newline of `<Enter>`. The output is therefore the same as in (1).

```text
이름을 입력하세요?Kitae Hwang
Kitae님 환영합니다
```

The program ends without reading the `Hwang` left in the buffer. To read the whole name including the space, use `std::cin.getline(name, 20);` instead of `std::cin >> name;`.

---

<br>

## Exercise 7: Reading a String with Spaces

**Textbook Problem:** Chapter 2 theory problem 14

**Problem:** Choose the wrong one among four pieces of code that read `I love C++<Enter>` into `char buf[100]`. The full problem and code are in [Exercise 7](Exercises.md#exercise-7-reading-a-string-with-spaces).

### Solution

**Answer:** ①

**Explanation:** `cin >> buf;` stops reading at a whitespace character, so only `I` is stored in `buf`, and it cannot read the whole line entered up to `<Enter>`.

`cin.getline(buf, size, delimitChar)` reads at most `size - 1` characters, stops when it meets `delimitChar` after removing that character from the input buffer, and appends `'\0'` at the end. The default value of the third argument is `'\n'` ([Chapter 2, Section 5.1](Concepts_Lecture.md#51-c-strings)).

- ② `cin.getline(buf, 11);` uses `'\n'` as the delimiter and reads at most 10 characters. `I love C++` is exactly 10 characters including the spaces, so all of them are stored, and the following `'\n'` is also removed.
- ③ `cin.getline(buf, 20, '\n');` reads at most 19 characters and stops at `<Enter>`, so the whole `I love C++` is stored.
- ④ `cin.getline(buf, 11, '.');` also stops after reading 10 characters, so `I love C++` is stored in `buf`.

> **Note:** ④ stops not because of `<Enter>` but because of the maximum number of characters. If `getline()` stores `size - 1` characters before it meets the delimiter, it sets the failure state (failbit) of the stream, so after ④ runs, `cin.fail()` is true and `'\n'` remains in the input buffer. All later input fails until the state is cleared with `cin.clear()`. In contrast, in ② the character right after the 10 stored characters is the delimiter `'\n'`, so the stream does not enter the failure state. If the input were longer than 10 characters, both ② and ④ would store only the first 10, so ② and ④ read the whole line in this problem only because the input is exactly 10 characters long.

---

<br>

## Exercise 8: The Namespace of the Standard Library

**Textbook Problem:** Chapter 2 theory problem 16

**Problem:** Name the namespace in which all the names of the C++ standard library are declared. The full problem is in [Exercise 8](Exercises.md#exercise-8-the-namespace-of-the-standard-library).

### Solution

**Answer:** `std`

**Explanation:** The C++ standard library declares all its names, such as `cout`, `cin`, `endl`, `string`, and `vector`, inside the `std` namespace. Therefore a standard library name is written with the namespace attached, as in `std::cout`, or by the name alone after declaring `using std::cout;` or `using namespace std;` ([Chapter 2, Section 4.1](Concepts_Lecture.md#41-concept)). Namespaces prevent conflicts between identical names when several people develop a program in parts or when libraries written by others are used.

---

<br>

## Exercise 9: The Header File That Declares `cin` and `cout`

**Textbook Problem:** Chapter 2 theory problem 18

**Problem:** Name the header file in which the `cin` and `cout` objects are declared in standard C++. The full problem is in [Exercise 9](Exercises.md#exercise-9-the-header-file-that-declares-cin-and-cout).

### Solution

**Answer:** `<iostream>`

**Explanation:** The `<iostream>` header file declares the classes for standard input and output (`ios`, `istream`, `ostream`, `iostream`), the objects `cin` and `cout`, and the `<<` and `>>` operator functions ([Chapter 2, Section 1.4](Concepts_Lecture.md#14-include-iostream)). `cin` is an object of type `istream` connected to the keyboard, and `cout` is an object of type `ostream` connected to the screen. The header file contains only declarations, and the actual objects are in the standard C++ library and are connected at link time ([Chapter 2, Section 7.4](Concepts_Lecture.md#74-header-files-and-libraries)). Standard C++ does not use `<iostream.h>` with `.h`.

---

<br>

## Exercise 10: Printing Stars with Nested `for` Statements

**Textbook Problem:** Chapter 2 theory problem 20

**Problem:** Fill in the three blank lines inside the outer `for` statement to complete a program that prints `*` one to four per line, increasing line by line. The full problem and code are in [Exercise 10](Exercises.md#exercise-10-printing-stars-with-nested-for-statements).

### Solution

**Answer:**

```cpp
for(int i=0; i<=n; i++) {
   cout << "*";
}
cout << endl;
```

**Explanation:** The program with the blanks filled and its output are as follows.

```cpp
#include <iostream>
using namespace std;
int main() {
   for(int n=0; n<4; n++) {
      for(int i=0; i<=n; i++) {
         cout << "*";
      }
      cout << endl;
   }
}
```

**Output:**

```text
*
**
***
****
```

The `n` of the outer `for` statement changes from 0 to 3 and determines the line to print. The inner `for` statement repeats `n + 1` times as `i` goes from 0 to `n`, so it prints 1, 2, 3, and 4 `*` characters when `n` is 0, 1, 2, and 3, and after the inner loop ends, `endl` moves to a new line. Braces can be omitted from a `for` statement whose body is a single statement, so writing the three lines `for(int i=0; i<=n; i++)`, `cout << "*";`, and `cout << endl;` to fit the three blank lines gives the same result.
