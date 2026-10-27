# Chapter 6 Solutions — Static Members and Default Parameters

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 6

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Bulletin Board](#exercise-1-bulletin-board)
- [Exercise 2: Multiplication Table](#exercise-2-multiplication-table)
- [Exercise 3: Statements About Function Overloading](#exercise-3-statements-about-function-overloading)
- [Exercise 4: Functions That Cannot Be Declared Together](#exercise-4-functions-that-cannot-be-declared-together)
- [Exercise 5: When Overloaded Functions Are Bound](#exercise-5-when-overloaded-functions-are-bound)
- [Exercise 6: Calling a Function with a Default Parameter](#exercise-6-calling-a-function-with-a-default-parameter)
- [Exercise 7: Overloaded Functions Whose Calls Can Be Ambiguous](#exercise-7-overloaded-functions-whose-calls-can-be-ambiguous)
- [Exercise 8: Output of a Constructor with a Default Parameter](#exercise-8-output-of-a-constructor-with-a-default-parameter)
- [Exercise 9: The Problem with Overloading `area()`](#exercise-9-the-problem-with-overloading-area)
- [Exercise 10: Calling a static Member Function](#exercise-10-calling-a-static-member-function)

---

<br>

## Exercise 1: Bulletin Board

**Problem:** Complete the static member functions `add()`, `del()`, `clear()`, `print()`, and `modify()` of a `Board` class that keeps post strings in a static array. Adding to a full board, deleting or modifying a nonexistent post number, and printing an empty board must print an error message without ending the program. The full problem is in [Exercise 1](Exercises.md#exercise-1-bulletin-board).

### Solution

The constructor of `Board` is private, so no object can be created, and the data `no` and `board` and all functions are static. Therefore `main()` calls the functions only through the class name, as in `Board::add(...)`, and the board data exists only once in the whole program. `no` is both the current number of posts and the index of the slot for the next post, so valid post numbers range from 0 to `no - 1`.

**Code:** [Lab_Q1.cpp](../../Coursework/Lab06_Overloading-and-Static-Members/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C06Q1.cpp
 * @brief   Q1. 게시판
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
#include <string>
#define MAX_NO 5        // 게시물의 최대 개수
using namespace std;

class Board {
    static int no;       // 현재 게시물의 개수
    static string board[MAX_NO];
    Board() {}           // private 접근 제어로 외부에서 객체 생성 방지
public:
    static void add(string t);
    static void del(int i);
    static void clear();
    static void print();
    static void modify(int no, string newtext);
};

// 각각의 멤버 함수 정의
void Board::add(string t) {
    if (no < MAX_NO) {
        board[no] = t;
        no++;
    } else {
        cout << "추가 실패! 게시물 개수가 " << MAX_NO << "개를 초과할 수 없습니다.\n" << endl;
    }
}

void Board::del(int i) {
    if ((board[i] != "") && (MAX_NO >= i)) {
        board[i] = "";
        
        if (i < no) {
            for (int j = i; j < no - 1; j++) {
                board[j] = board[j+1];
            }
            board[no] = "";
        }
        no--;
    } else {
        cout << "삭제 실패! 존재하지 않는 게시물입니다.\n" << endl;
    }
}

void Board::clear() {
    for (int i = 0; i < MAX_NO; i++) {
        if (board[i] != "") {
            board[i] = "";
        }
    }
    no = 0;
}

void Board::print() {
    int count = 0;
    cout << "********** [컴퓨터언어II 게시판] **********" << endl;
    
    for (int i = 0; i < no; i++) {
        cout << i << ": " << board[i] << endl;
        count++;
    }
    if (!count) {
        cout << "<등록된 게시물이 없습니다.>\n" << endl;
    }
    cout << endl;
}

void Board::modify(int no, string newtext) {
    if (no >= Board::no) {
        cout << "수정 실패! 존재하지 않는 게시물입니다.\n" << endl;
    } else {
        board[no] = newtext;
    }
}


// static 멤버 변수 외부 선언
int Board::no = 0;
string Board::board[MAX_NO] = {};


int main() {
    Board::add("PL센터 이용확인증 미제출자는 이번주까지 최소 1회 방문하시기 바랍니다.");
    Board::add("PL센터 이용확인증은 다음주까지 제출하세요.");
    Board::print();

    Board::add("6장 이론과제는 다음주 화요일까지 온라인 제출하세요.");
    Board::add("6장 실습과제는 이번주 토요일까지 온라인 제출하세요.");
    Board::add("수업시간에 확인받지 못한 경우에만 업로드하면 됩니다.");
    Board::print();

    // add() 오류 사례 - 게시물 최대 개수 초과
    Board::add("소스코드는 압축 없이 개별 파일로 업로드하세요.");
    Board::modify(3, "(수정) 6장 실습과제는 이번주 금요일까지 온라인 제출하세요.");
    Board::print();

    Board::del(5);       // del() 오류 사례 - 존재하지 않는 게시물 삭제 요청
    Board::del(2);
    Board::print();
    Board::del(0);
    Board::print();

    // modify() 오류 사례 - 존재하지 않는 게시물
    Board::modify(4, "프로그램 제출기한 엄수바랍니다.");
    Board::add("실습과제 수행 시 PL센터의 도움을 받을 수 있습니다.");
    Board::print();
    Board::clear();
    Board::print();

    return 0;
}
```

Each function works as follows.

- `add(t)` stores `t` in `board[no]` and increments `no` if `no < MAX_NO`. If the board is full, it prints an error message, and the `\n` and `endl` at the end of the message leave one blank line after it.
- `del(i)` empties slot `i` and, if `i` is less than `no`, shifts the following posts forward by one slot, then decrements `no`. Thus in the output after `del(2)`, the former posts 3 and 4 become posts 2 and 3.
- `clear()` sets every slot to the empty string and resets `no` to 0.
- `print()` prints the title line and posts 0 through `no - 1`, and prints `<등록된 게시물이 없습니다.>` (no posts registered) if it printed none.
- `modify(no, newtext)` replaces the string in that slot if the number is less than `Board::no`. Because the parameter name `no` hides the static member `no`, the static member is written as `Board::no`.
- The static member variables are defined outside the class as `int Board::no = 0;` and `string Board::board[MAX_NO] = {};`. Without these definitions, a link error occurs ([Chapter 6, Section 3.1](Concepts_Lecture.md#31-static-member-variables)).

**Output:** The output differs from the sample run in one spot at the end. With no posts, `print()` prints `"<등록된 게시물이 없습니다.>\n"`, then breaks the line once more with `endl`, and runs `cout << endl;` again at the end of the function. This leaves two blank lines after the message, while the sample run has only one.

> **Caution:** **Number check in `del()`.** The condition `(board[i] != "") && (MAX_NO >= i)` has three problems. First, `&&` evaluates from the left, so `board[i]` is read before the range is checked. In an array of size 5, `del(5)` reads `board[5]`, which is out of bounds and undefined behavior; running under AddressSanitizer (`-fsanitize=address`) reports a global-buffer-overflow on this line. That the same "삭제 실패! 존재하지 않는 게시물입니다." (delete failed, nonexistent post) message as in the sample run appears is a coincidence. Second, `MAX_NO >= i` lets `i == 5` through and does not reject negative numbers. Third, validity is judged by `board[i] != ""` rather than `i < no`, so if an old string remains in a slot at or beyond `no`, deleting a nonexistent post such as `del(no)` is treated as a success and `no` is wrongly decremented. For example, after adding `A`, `B`, `C`, `D`, `E` and running `del(2)` and `del(0)`, a copy of `E` remains in `board[3]`, so `del(3)` does not fail and makes the last post `E` disappear from the list.

> **Caution:** **Slot cleared after deletion.** The slot to clear after shifting is `board[no - 1]`, where the last post was, but the code clears `board[no]`. Running `del(2)` on a full board (`no == 5`) writes to `board[5]`, which is out of bounds and undefined behavior, and even otherwise a copy of the last post remains in `board[no - 1]` and causes the third problem above. In addition, `modify()` checks only `no >= Board::no` and does not reject negative numbers.

The following is a `del()` with both problems fixed. It is a corrected implementation for comparison, not the code in the Lab file; run with the same `main()`, it produces the same output without any out-of-bounds access.

```cpp
void Board::del(int i) {
    if (i < 0 || i >= no) {
        cout << "삭제 실패! 존재하지 않는 게시물입니다.\n" << endl;
        return;
    }
    for (int j = i; j < no - 1; j++)
        board[j] = board[j + 1];
    board[no - 1] = "";
    no--;
}
```

---

<br>

## Exercise 2: Multiplication Table

**Problem:** Using a default parameter, write a `print99()` function such that `print99(3)` prints only the table of 3 and `print99()` prints all tables from 1 to 9. The full problem is in [Exercise 2](Exercises.md#exercise-2-multiplication-table).

### Solution

Handling both calls with one function requires a default value that distinguishes the call with the argument omitted. The solution sets the default to 19, a value never used as a table number, and prints all tables if `i` is 19 and only table `i` otherwise. Any value outside 1-9 can serve as this marker, and using 0 is also common.

**Code:** [Lab_Q2.cpp](../../Coursework/Lab06_Overloading-and-Static-Members/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C06Q2.cpp
 * @brief   Q2. 구구단
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
using namespace std;

void print99(int i = 19) {
    
    if (i != 19) {
        cout << i << "단" << endl;
        for (int j = 1; j < 10; j++) {
            cout << i << " * " << j << " = " << i * j << endl;
        }
        cout << endl;
    } else {
        for (int k = 1; k < 10; k++) {
            cout << k << "단" << endl;
            for (int j = 1; j < 10; j++) {
                cout << k << " * " << j << " = " << k * j << endl;
            }
            cout << endl;
        }
    }
}

int main() {
    print99(3);     // 3단 출력
    print99();      // 1 - 9단(전체) 출력
    
    return 0;
}
```

- `print99(3)` has `i = 3`, so the first branch prints the title `3단` (table of 3) and the expressions from `3 * 1 = 3` to `3 * 9 = 27`, then a blank line.
- `print99()` is rewritten by the compiler as `print99(19)`, so the second branch varies `k` from 1 to 9 and prints nine tables in the same format.

**Output:** The table of 3 (1 title line, 9 expression lines, 1 blank line) and the nine tables from 1 to 9 total 110 lines, and the part omitted as `(중략)` in the sample run contains `1 * 4 = 4` through `9 * 6 = 54`.

> **Note:** With a marker value as the default, passing that value directly as in `print99(19)` also prints all tables, and an out-of-range table such as `print99(10)` is printed without any check. Handling the two calls in the problem is enough, but to check the range, add a condition that `i` is between 1 and 9 inclusive.

---

<br>

## Exercise 3: Statements About Function Overloading

**Textbook Problem:** Chapter 6 theory problem 2

**Problem:** Choose the wrong statement about function overloading. The options are in [Exercise 3](Exercises.md#exercise-3-statements-about-function-overloading).

### Solution

**Answer:** ③

**Explanation:** Member functions of a class can be overloaded by the same rules as ordinary functions; overloaded constructors are the typical example ([Chapter 6, Section 1.4](Concepts_Lecture.md#14-constructor-overloading-and-destructors)). The others are correct. C cannot define two or more functions with the same name (①), in C++ functions with the same name are overloaded if the types or numbers of their parameters differ (②), and two functions that differ only in their return types cannot be told apart by a call statement, so they are a compile error rather than overloading (④) ([Chapter 6, Section 1.1](Concepts_Lecture.md#11-concept)).

---

<br>

## Exercise 4: Functions That Cannot Be Declared Together

**Textbook Problem:** Chapter 6 theory problem 4

**Problem:** Choose the two functions among the four that cannot be declared together in one program. The options are in [Exercise 4](Exercises.md#exercise-4-functions-that-cannot-be-declared-together).

### Solution

**Answer:** ①, ②

**Explanation:** `void g();` and `int g();` have the same parameter list and differ only in their return types. The call statement `g();` alone cannot decide which of the two is meant, so declaring them together is a compile error. `double g(int x)` in ③ and `double g(int x, int y)` in ④ differ from each other and from ① and ② in the number of parameters, so declaring either of them together with ① or ② is valid overloading.

---

<br>

## Exercise 5: When Overloaded Functions Are Bound

**Textbook Problem:** Chapter 6 theory problem 6

**Problem:** Choose when an overloaded function is bound to its call statement. The options are in [Exercise 5](Exercises.md#exercise-5-when-overloaded-functions-are-bound).

### Solution

**Answer:** ②

**Explanation:** The compiler compares the number and types of the arguments in the call statement with the parameters of the overloaded functions and decides at compile time which function to call ([Chapter 6, Section 1.2](Concepts_Lecture.md#12-resolution-rules)). This is called static binding, and at run time the program only calls the function already decided. Dynamic binding, which decides the function to call at run time, is covered with virtual functions in Chapter 9.

---

<br>

## Exercise 6: Calling a Function with a Default Parameter

**Textbook Problem:** Chapter 6 theory problem 8

**Problem:** Choose the inappropriate call for `int add(int a, double b = 10.0);`. The full problem and code are in [Exercise 6](Exercises.md#exercise-6-calling-a-function-with-a-default-parameter).

### Solution

**Answer:** ②

**Explanation:** `add()` has two parameters, so it can take at most two arguments. `add(10, 3.14, 10)` in ② has three arguments, so no function matches and a compile error occurs. ① uses the default value 10.0 for `b` and is called as `add(10, 10.0)`, and ③ and ④ pass both arguments. A call that does not use the return value, as in ③, is also syntactically fine ([Chapter 6, Section 2.1](Concepts_Lecture.md#21-syntax)).

---

<br>

## Exercise 7: Overloaded Functions Whose Calls Can Be Ambiguous

**Textbook Problem:** Chapter 6 theory problem 10

**Problem:** Choose the pair among four pairs of overloaded functions whose calls can become ambiguous. The options are in [Exercise 7](Exercises.md#exercise-7-overloaded-functions-whose-calls-can-be-ambiguous).

### Solution

**Answer:** ②

**Explanation:** With `int add(int a, int b)` and `int add(int a, int &b)`, calling `add(x, y)` with `int x, y;` makes the second argument fit `int`, taken by value, and `int&`, taken by reference, equally well. The two conversions cannot be ranked, so the compiler reports the error `call to 'add' is ambiguous` ([Chapter 6, Section 2.3](Concepts_Lecture.md#23-overloading-ambiguity)). The other pairs are not ambiguous.

- **①:** The numbers of arguments differ, so `add(x)` and `add(x, y)` call different functions.
- **③:** If the second argument is an integer, the `int b` version is called; if it is an address, the `int* b` version.
- **④:** If the second argument is an address (`&y`), the `int* b` version is called; if it is a variable (`y`), the `int &b` version.

---

<br>

## Exercise 8: Output of a Constructor with a Default Parameter

**Textbook Problem:** Chapter 6 theory problem 12

**Problem:** Give the output when three objects are created of a class with a constructor that has a default parameter and a two-parameter constructor. The full problem and code are in [Exercise 8](Exercises.md#exercise-8-output-of-a-constructor-with-a-default-parameter).

### Solution

**Answer:** (1) `0`, (2) `3`, (3) `5hello`

**Explanation:** `Overloading a;` has no arguments, so `Overloading(int x = 0)` is called with the default value 0 and prints 0. `Overloading b(3);` calls the same constructor with the argument 3, and `Overloading c(5, "hello");` has two arguments, so it calls the second constructor, which prints 5 and `hello` together. If the three objects are created in turn, the output has no line breaks or spaces, so `035hello` appears on the screen in one run.

---

<br>

## Exercise 9: The Problem with Overloading `area()`

**Textbook Problem:** Chapter 6 theory problem 14

**Problem:** Show with a call example that the overloading of `float area(float f);` and `double area(double d);` is problematic. The full problem and code are in [Exercise 9](Exercises.md#exercise-9-the-problem-with-overloading-area).

### Solution

**Answer:** A call with an integer argument, as in `area(3);`, is an ambiguous call and gives a compile error.

**Explanation:** The integer 3 exactly matches neither `float` nor `double`. Converting `int` to `float` and converting it to `double` are both standard conversions of the same rank, so the compiler cannot judge either to be better. If the compiler picked one arbitrarily, the result could differ from the programmer's intent, so it judges such a call ambiguous and reports the error `call to 'area' is ambiguous`. `area(3.0)` exactly matches the `double` version and `area(3.0f)` the `float` version, so they are not ambiguous.

---

<br>

## Exercise 10: Calling a static Member Function

**Textbook Problem:** Chapter 6 theory problem 16

**Problem:** Choose the wrong line among four lines that call the static member function `getA()`. The full problem and code are in [Exercise 10](Exercises.md#exercise-10-calling-a-static-member-function).

### Solution

**Answer:** ③

**Explanation:** The left side of the scope resolution operator `::` must be a class name, so `StaticTest::getA()` (①) is correct, but `s::getA()`, which uses an object name, is a compile error. A static member function can also be called through an object, so `s.getA()` (②) and `p->getA()` (④) are correct as well ([Chapter 6, Section 3.2](Concepts_Lecture.md#32-accessing-static-members)). Even when it is called through an object, the single static member of the class is used regardless of that object.

> **Note:** This class only declares `static int a;` and does not define it. To link a program that actually calls `getA()`, a definition such as `int StaticTest::a;` must be placed outside the class; without it the code compiles, but the linker reports that it cannot find `StaticTest::a` ([Chapter 6, Section 3.1](Concepts_Lecture.md#31-static-member-variables)).
