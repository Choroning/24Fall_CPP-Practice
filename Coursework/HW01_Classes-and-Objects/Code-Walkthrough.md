# HW1 Code Walkthrough — Classes and Objects

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 3

> **Learning Objectives:**
> 1. Read the class declaration, the member function definitions, and `main()` of the three solutions line by line and explain the role of each part
> 2. Trace how the member variables of an object change after each statement of `main()` runs
> 3. Find where each member function is defined when definitions inside and outside the class are mixed in one class

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: The Account Class](#2-q1cpp-the-account-class)
  - [2.1 Full Code](#21-full-code)
  - [2.2 Line-by-Line Explanation](#22-line-by-line-explanation)
  - [2.3 Execution Trace](#23-execution-trace)
- [3. Q2.cpp: The Random Class](#3-q2cpp-the-random-class)
  - [3.1 Full Code](#31-full-code)
  - [3.2 Line-by-Line Explanation](#32-line-by-line-explanation)
  - [3.3 Execution Trace](#33-execution-trace)
- [4. Q3.cpp: The Point Class](#4-q3cpp-the-point-class)
  - [4.1 Full Code](#41-full-code)
  - [4.2 Line-by-Line Explanation](#42-line-by-line-explanation)
  - [4.3 Execution Trace](#43-execution-trace)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the three solutions puts, in one file, the header includes, the class declaration, the member function definitions outside the class, and `main()`, in that order. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW1 Assignment Explanation](Assignment-Explanation.md).

| File | Class | Members defined outside the class | Input |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Account` | Constructor, `getOwner()`, `deposit()`, `withdraw()`, `inquiry()` | None |
| [`Q2.cpp`](Solution/Q2.cpp) | `Random` | Constructor, `next()`, `nextInRange()` | None |
| [`Q3.cpp`](Solution/Q3.cpp) | `Point` | Two-argument constructor, `showP()`, `getY()`, `setY()` | The x and y coordinates of `p2` |

*Table 1. Layout of the HW1 solution files*

---

<br>

## 2. Q1.cpp: The Account Class

### 2.1 Full Code

```cpp
/**
 * @file    A1Q1.cpp
 * @brief   Q1. 은행 계좌 Account 클래스 (교재 3장 실습 문제 3번)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
#include <string>
using namespace std;

class Account{
public:
    string name;
    int id;
    int balance;
    
    Account();
    Account(string, int, int);
    string getOwner();
    void deposit(int);
    int withdraw(int);
    int inquiry();
};

Account::Account(string n, int i, int b){
    name = n;
    id = i;
    balance = b;
}

string Account::getOwner(){
    return name;
}

void Account::deposit(int m){
    balance += m;
}

int Account::withdraw(int m){
    balance -= m;
    return m;
}

int Account::inquiry(){
    return balance;
}


int main()
{
    Account a("kitae", 1, 5000);
    a.deposit(50000);
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
    int money = a.withdraw(20000);
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
}
```

### 2.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-10 | `#include`, `using namespace std;` | Includes `<iostream>` for `cout` and `endl` and `<string>` for `string`, and omits `std::` |
| 12-13 | `class Account{ public:` | Declares all the following members as `public` |
| 14-16 | `string name; int id; int balance;` | Member variables that store the owner's name, the account number, and the balance |
| 18 | `Account();` | Declaration of the default constructor. It has no definition, and this program never calls it |
| 19 | `Account(string, int, int);` | Declaration of the three-parameter constructor. Parameter names can be omitted in a prototype |
| 20-23 | Member function prototypes | Prototypes of four functions that give only the return type and the parameter types |
| 26-30 | `Account::Account(string n, int i, int b)` | Assigns the received name, number, and balance to the member variables. `Account::` shows that this function is a member of `Account` |
| 32-34 | `Account::getOwner()` | Returns `name`. Because it returns a `string` by value, a copy of the name is passed |
| 36-38 | `Account::deposit(int m)` | Adds the deposit to the balance with `balance += m;` |
| 40-43 | `Account::withdraw(int m)` | Subtracts `m` from the balance and returns `m`. It does not check the balance |
| 45-47 | `Account::inquiry()` | Returns the current `balance` |
| 52 | `Account a("kitae", 1, 5000);` | Calls the three-parameter constructor, which initializes the three members of `a`. The string literal `"kitae"` is converted to the `string` parameter `n` |
| 53-54 | `deposit()`, first output | Deposits 50000 and prints the return values of `getOwner()` and `inquiry()` |
| 55-56 | `withdraw()`, second output | Withdraws 20000, stores the return value in `money`, and prints again. `money` is not used afterwards |

*Table 2. Line-by-line explanation of Q1.cpp*

### 2.3 Execution Trace

| Statement executed | `name` | `id` | `balance` | Output |
|:-----|:-----|-----:|-----:|:-----|
| `Account a("kitae", 1, 5000);` | `"kitae"` | 1 | 5000 | |
| `a.deposit(50000);` | `"kitae"` | 1 | 55000 | |
| First `cout` | `"kitae"` | 1 | 55000 | `kitae의 잔액은 55000` |
| `int money = a.withdraw(20000);` | `"kitae"` | 1 | 35000 | (`money` is 20000) |
| Second `cout` | `"kitae"` | 1 | 35000 | `kitae의 잔액은 35000` |

*Table 3. State of the object `a` as the `main()` of Q1 runs*

---

<br>

## 3. Q2.cpp: The Random Class

### 3.1 Full Code

```cpp
/**
 * @file    A1Q2.cpp
 * @brief   Q2. 랜덤 정수를 발생시키는 Random 클래스 (교재 3장 실습 문제 5번)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Random{
public:
    Random();
    int next();
    int nextInRange(int, int);
};

Random::Random(){
    srand((unsigned)time(0));
}

int Random::next(){
    return rand();
}

int Random::nextInRange(int s, int f){
    return (rand() % (f - s + 1)) + s;
}


int main()
{
    Random r;
    cout << "-- 0에서 " << RAND_MAX << "까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++){
        int n = r.next();
        cout << n << ' ';
    }
    cout << endl << endl << "-- 2에서 " << "4까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++){
        int n = r.nextInRange(2, 4);
        cout << n << ' ';
    }
    cout << endl;
}
```

### 3.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-11 | `#include`, `using namespace std;` | `<cstdlib>` provides `rand()`, `srand()`, and `RAND_MAX`, and `<ctime>` provides `time()` |
| 13-18 | `class Random{ ... };` | A class with no member variables, only a constructor and two member functions. The state of the random sequence lives in the C library, not in the object |
| 20-22 | `Random::Random()` | Converts `time(0)` to `unsigned` and passes it to `srand()` to set the seed |
| 24-26 | `Random::next()` | Returns the return value of `rand()`, an integer from 0 to `RAND_MAX`, unchanged |
| 28-30 | `Random::nextInRange(int s, int f)` | Makes a value from 0 to `f - s` with `rand() % (f - s + 1)` and adds `s` |
| 35 | `Random r;` | Calls the constructor, which sets the seed |
| 36 | First guidance line | Prints the value of `RAND_MAX` inside the text |
| 37-40 | First `for` loop | Calls `next()` 10 times and prints each number followed by a space |
| 41 | `cout << endl << endl << ...` | Ends the first line, leaves one blank line, and prints the second guidance line |
| 42-45 | Second `for` loop | Calls `nextInRange(2, 4)` 10 times and prints one of 2, 3, and 4 |
| 46 | `cout << endl;` | Ends the last line |

*Table 4. Line-by-line explanation of Q2.cpp*

### 3.3 Execution Trace

If `rand()` returns 7, `nextInRange(2, 4)` returns `7 % 3 + 2 = 1 + 2 = 3`. If `rand()` returns 9, the result is `9 % 3 + 2 = 2`, and if it returns 11, the result is `11 % 3 + 2 = 4`. When the remainder of the return value of `rand()` divided by 3 is 0, 1, or 2, the program prints 2, 3, or 4 respectively, so the printed value is always one of these three values.

---

<br>

## 4. Q3.cpp: The Point Class

### 4.1 Full Code

```cpp
/**
 * @file    A1Q3.cpp
 * @brief   Q3. 접근 제어와 get/set 함수를 갖춘 Point 클래스 (4주차 문제 1-3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
using namespace std;

class Point{
private:
    int x, y;
public:
    Point(){
        x = y = 0;
    }
    Point(int ix, int iy);
    
    void showP();
    
    int getX(){
        return x;
    }
    void setX(int ix){
        x = ix;
    }
    
    int getY();
    void setY(int iy);
    
};

Point::Point(int ix, int iy){
    x = ix;
    y = iy;
}

void Point::showP(){
    cout << "x: " << getX() << ", y: " << getY() << endl;
}

int Point::getY(){
    return y;
}

void Point::setY(int iy){
    y = iy;
}

int main()
{
    Point p1, p2;
    int temp;
    
    cout << "Input p2.x: ";
    cin >> temp;
    p2.setX(temp);
    
    cout << "Input p2.y: ";
    cin >> temp;
    p2.setY(temp);
    
    cout << "p1. ";
    p1.showP();
    cout << "p2. ";
    p2.showP();
}
```

### 4.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 12-13 | `private: int x, y;` | Declares the two coordinates as `private` so that they cannot be accessed directly outside the class |
| 15-17 | `Point(){ x = y = 0; }` | The default constructor defined inside the class, which sets both coordinates to 0 |
| 18 | `Point(int ix, int iy);` | Declaration of the two-argument constructor; its definition is at lines 34-37 |
| 20 | `void showP();` | Declaration of the coordinate output function; its definition is at lines 39-41 |
| 22-27 | `getX()`, `setX()` | The get/set functions of x defined inside the class |
| 29-30 | Declarations of `getY()` and `setY()` | Their definitions are at lines 43-49 |
| 34-37 | `Point::Point(int ix, int iy)` | Assigns the received values to `x` and `y` |
| 39-41 | `Point::showP()` | Gets the coordinates with `getX()` and `getY()` and prints them in the form `x: ..., y: ...` |
| 43-49 | `Point::getY()`, `Point::setY()` | The get/set functions of y defined outside the class |
| 53-54 | `Point p1, p2; int temp;` | Creates the two points with the default constructor and declares a variable for input |
| 56-58 | Reading the x coordinate | Prints the prompt, reads an integer, and stores it in `p2.x` with `setX()` |
| 60-62 | Reading the y coordinate | Stores the value in `p2.y` with `setY()` in the same way. `temp` is reused |
| 64-67 | Output | Prints the name of the point first, and `showP()` continues with the coordinates on the same line |

*Table 5. Line-by-line explanation of Q3.cpp*

### 4.3 Execution Trace

| Statement executed | `p1` | `p2` | `temp` | Output |
|:-----|:-----|:-----|-----:|:-----|
| `Point p1, p2;` | (0, 0) | (0, 0) | undefined | |
| `cin >> temp;` (input `-3`) | (0, 0) | (0, 0) | -3 | `Input p2.x: ` |
| `p2.setX(temp);` | (0, 0) | (-3, 0) | -3 | |
| `cin >> temp;` (input `4`) | (0, 0) | (-3, 0) | 4 | `Input p2.y: ` |
| `p2.setY(temp);` | (0, 0) | (-3, 4) | 4 | |
| `p1.showP();` | (0, 0) | (-3, 4) | 4 | `p1. x: 0, y: 0` |
| `p2.showP();` | (0, 0) | (-3, 4) | 4 | `p2. x: -3, y: 4` |

*Table 6. State of the two points as the `main()` of Q3 runs (inputs `-3`, `4`)*

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Layout of a file | Each of the three solutions consists of header includes, the class declaration, member function definitions outside the class, and `main()`, in that order. |
| Prototype and definition | Parameter names can be omitted in a prototype, and the definition writes the body with `ClassName::`. |
| Tracing object state | The balance of `Account` changes to 5000, 55000, and 35000, and `Point p2` changes to (0, 0), (-3, 0), and (-3, 4). |
| Computing the random range | `nextInRange(2, 4)` maps the remainders 0, 1, and 2 of `rand()` divided by 3 to 2, 3, and 4. |
| Finding definitions | In `Point`, the members whose bodies are inside the class are the default constructor and the get/set functions of x; the others are outside the class. |

---

<br>

## Self-Check Questions

1. **Parameter names in a prototype:** Line 19 of Q1, `Account(string, int, int);`, has no parameter names. Can the names also be omitted in the definition at line 26?

   <details>
   <summary>Answer</summary>

   Syntactically the names can be omitted in the definition too, but then the body has no way to refer to those parameters. The constructor must assign the received values to the member variables, so the definition must name them, as in `n`, `i`, and `b`.

   </details>

2. **Order of output:** Explain how one line on the screen is formed when `p1.showP();` runs after `cout << "p1. ";` at lines 64-65 of Q3.

   <details>
   <summary>Answer</summary>

   Line 64 prints `p1. ` without ending the line. Then `showP()` prints `x: 0, y: 0` on the same line and ends the line with `endl`, so `p1. x: 0, y: 0` appears on one line.

   </details>

3. **Computing a remainder:** In Q2, what is the value of `nextInRange(2, 4)` if `rand()` returns 32767?

   <details>
   <summary>Answer</summary>

   The remainder of 32767 divided by 3 is 1 (32767 = 3 × 10922 + 1), so the value is `1 + 2 = 3`.

   </details>

4. **Choice of constructor:** If the `main()` of Q3 is changed to `Point p1, p2(5, 6);` and the two points are printed without reading any input, what is the result?

   <details>
   <summary>Answer</summary>

   `p1` becomes (0, 0) through the default constructor, and `p2` becomes (5, 6) through the two-argument constructor. The output is `p1. x: 0, y: 0` and `p2. x: 5, y: 6`.

   </details>
