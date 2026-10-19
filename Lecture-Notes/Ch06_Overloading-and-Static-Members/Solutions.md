# Chapter 6 Solutions — Static Members and Default Parameters

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-19

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Bulletin Board](#exercise-1-bulletin-board)
- [Exercise 2: Multiplication Table](#exercise-2-multiplication-table)

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
- The static member variables are defined outside the class as `int Board::no = 0;` and `string Board::board[MAX_NO] = {};`. Without these definitions, a linker error occurs ([Chapter 6, Section 3.1](Concepts_Lecture.md#31-static-member-variables)).

**Output:** The output matches the sample run except for one spot at the end. With no posts, `print()` prints `"<등록된 게시물이 없습니다.>\n"`, then breaks the line once more with `endl`, and runs `cout << endl;` again at the end of the function. This leaves two blank lines after the message, while the sample run has only one.

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

**Output:** The output matches the sample run. The table of 3 (1 title line, 9 expression lines, 1 blank line) and the nine tables from 1 to 9 total 110 lines, and the part omitted as `(중략)` in the sample run contains `1 * 4 = 4` through `9 * 6 = 54`.

> **Note:** With a marker value as the default, passing that value directly as in `print99(19)` also prints all tables, and an out-of-range table such as `print99(10)` is printed without any check. Handling the two calls in the problem is enough, but to check the range, add a condition that `i` is between 1 and 9 inclusive.
