# Chapter 6 Exercises — Static Members and Default Parameters

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-19

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Exercise 1: Bulletin Board

**Problem:** Referring to the example code and the sample run, complete the `Board` class so that the `main()` function runs correctly.

**Program Requirements:**

1. The given example code must not be changed.
2. Each member function of the `Board` class must handle the following situations properly. The program must print an error message without terminating abnormally.
   - `add()`: the board is full and no more posts can be added
   - `del()`, `modify()`: the post number received for the operation is invalid
   - `print()`: there are no posts to print

**Example Code:**

```cpp
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

**Sample Run:**

```text
********** [컴퓨터언어II 게시판] **********
0: PL센터 이용확인증 미제출자는 이번주까지 최소 1회 방문하시기 바랍니다.
1: PL센터 이용확인증은 다음주까지 제출하세요.

********** [컴퓨터언어II 게시판] **********
0: PL센터 이용확인증 미제출자는 이번주까지 최소 1회 방문하시기 바랍니다.
1: PL센터 이용확인증은 다음주까지 제출하세요.
2: 6장 이론과제는 다음주 화요일까지 온라인 제출하세요.
3: 6장 실습과제는 이번주 토요일까지 온라인 제출하세요.
4: 수업시간에 확인받지 못한 경우에만 업로드하면 됩니다.

추가 실패! 게시물 개수가 5개를 초과할 수 없습니다.

********** [컴퓨터언어II 게시판] **********
0: PL센터 이용확인증 미제출자는 이번주까지 최소 1회 방문하시기 바랍니다.
1: PL센터 이용확인증은 다음주까지 제출하세요.
2: 6장 이론과제는 다음주 화요일까지 온라인 제출하세요.
3: (수정) 6장 실습과제는 이번주 금요일까지 온라인 제출하세요.
4: 수업시간에 확인받지 못한 경우에만 업로드하면 됩니다.

삭제 실패! 존재하지 않는 게시물입니다.

********** [컴퓨터언어II 게시판] **********
0: PL센터 이용확인증 미제출자는 이번주까지 최소 1회 방문하시기 바랍니다.
1: PL센터 이용확인증은 다음주까지 제출하세요.
2: (수정) 6장 실습과제는 이번주 금요일까지 온라인 제출하세요.
3: 수업시간에 확인받지 못한 경우에만 업로드하면 됩니다.

********** [컴퓨터언어II 게시판] **********
0: PL센터 이용확인증은 다음주까지 제출하세요.
1: (수정) 6장 실습과제는 이번주 금요일까지 온라인 제출하세요.
2: 수업시간에 확인받지 못한 경우에만 업로드하면 됩니다.

수정 실패! 존재하지 않는 게시물입니다.

********** [컴퓨터언어II 게시판] **********
0: PL센터 이용확인증은 다음주까지 제출하세요.
1: (수정) 6장 실습과제는 이번주 금요일까지 온라인 제출하세요.
2: 수업시간에 확인받지 못한 경우에만 업로드하면 됩니다.
3: 실습과제 수행 시 PL센터의 도움을 받을 수 있습니다.

********** [컴퓨터언어II 게시판] **********
<등록된 게시물이 없습니다.>

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-bulletin-board)**

---

<br>

## Exercise 2: Multiplication Table

**Problem:** Referring to the example code and the sample run, write a `print99()` function that prints the multiplication table so that the `main()` function runs correctly.

**Program Requirements:**

1. The given example code must not be changed.
2. Write the `print99()` function using a default parameter.

**Example Code:**

```cpp
#include <iostream>
using namespace std;

int main() {
   print99(3);    // 3단 출력
   print99();     // 1 - 9단(전체) 출력

   return 0;
}
```

**Sample Run:**

```text
3단
3 * 1 = 3
3 * 2 = 6
3 * 3 = 9
3 * 4 = 12
3 * 5 = 15
3 * 6 = 18
3 * 7 = 21
3 * 8 = 24
3 * 9 = 27

1단
1 * 1 = 1
1 * 2 = 2
1 * 3 = 3

... (중략)

9 * 7 = 63
9 * 8 = 72
9 * 9 = 81

Program ended with exit code: 0
```

`(중략)` (omitted) marks a part left out of the sample run for readability and does not appear in the actual output.

**[Solution](Solutions.md#exercise-2-multiplication-table)**
