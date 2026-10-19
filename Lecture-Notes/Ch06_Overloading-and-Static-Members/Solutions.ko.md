# 6장 연습 문제 풀이 — static 멤버와 디폴트 매개변수

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-19

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 게시판](#연습-문제-1-게시판)
- [연습 문제 2: 구구단](#연습-문제-2-구구단)

---

<br>

## 연습 문제 1: 게시판

**문제:** 게시물 문자열을 static 배열에 보관하는 `Board` 클래스의 static 멤버 함수 `add()`, `del()`, `clear()`, `print()`, `modify()`를 완성한다. 게시판이 가득 찬 상태의 추가, 존재하지 않는 번호의 삭제와 수정, 게시물이 없는 상태의 출력에서는 프로그램을 끝내지 않고 오류 메시지를 출력한다. 전체 문제는 [연습 문제 1](Exercises.ko.md#연습-문제-1-게시판)에 있다.

### 풀이

`Board`의 생성자는 private이므로 객체를 만들 수 없고, 데이터 `no`, `board`와 모든 함수가 static이다. 따라서 `main()`은 `Board::add(...)`처럼 클래스 이름으로만 함수를 호출하며, 게시판 데이터는 프로그램 전체에 하나만 존재한다. `no`는 현재 게시물 수이면서 다음 게시물이 들어갈 칸의 인덱스이므로, 유효한 게시물 번호는 0부터 `no - 1`까지이다.

**코드:** [Lab_Q1.cpp](../../Coursework/Lab06_Overloading-and-Static-Members/Lab_Q1.cpp)

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

각 함수는 다음과 같이 동작한다.

- `add(t)`는 `no < MAX_NO`이면 `board[no]`에 `t`를 저장하고 `no`를 1 늘린다. 가득 찼으면 오류 메시지를 출력하며, 메시지 끝의 `\n`과 `endl` 때문에 메시지 뒤에 빈 줄이 하나 생긴다.
- `del(i)`는 `i`번 칸을 비우고, `i`가 `no`보다 작으면 뒤의 게시물을 한 칸씩 앞으로 당긴 뒤 `no`를 1 줄인다. 그래서 `del(2)` 다음 출력에서 원래 3번, 4번 게시물이 2번, 3번이 된다.
- `clear()`는 모든 칸을 빈 문자열로 만들고 `no`를 0으로 되돌린다.
- `print()`는 제목 줄과 0번부터 `no - 1`번까지의 게시물을 출력하고, 하나도 출력하지 않았으면 `<등록된 게시물이 없습니다.>`를 출력한다.
- `modify(no, newtext)`는 번호가 `Board::no`보다 작으면 그 칸의 문자열을 바꾼다. 매개변수 이름 `no`가 static 멤버 `no`를 가리므로 static 멤버는 `Board::no`로 지정한다.
- static 멤버 변수는 클래스 밖에서 `int Board::no = 0;`, `string Board::board[MAX_NO] = {};`로 정의한다. 이 정의가 없으면 링커 오류가 난다([6장 3.1절](Concepts_Lecture.ko.md#31-static-멤버-변수)).

**실행 결과:** 출력은 실행 예시와 같고, 마지막 부분 하나만 다르다. 게시물이 없을 때 `print()`는 `"<등록된 게시물이 없습니다.>\n"`을 출력한 뒤 `endl`로 줄을 한 번 더 바꾸고, 함수 끝에서 `cout << endl;`을 다시 실행한다. 그래서 안내 메시지 뒤에 빈 줄이 두 개 생기며, 실행 예시에는 한 개만 있다.

> **주의:** **`del()`의 번호 검사.** 조건 `(board[i] != "") && (MAX_NO >= i)`에는 세 가지 문제가 있다. 첫째, `&&`는 왼쪽부터 평가하므로 범위를 확인하기 전에 `board[i]`를 읽는다. 크기가 5인 배열에서 `del(5)`는 범위 밖의 `board[5]`를 읽으므로 정의되지 않은 동작이며, AddressSanitizer(`-fsanitize=address`)로 실행하면 이 줄에서 global-buffer-overflow가 보고된다. 실행 예시와 같은 "삭제 실패! 존재하지 않는 게시물입니다." 메시지가 나온 것은 우연이다. 둘째, `MAX_NO >= i`는 `i == 5`를 통과시키고 음수 번호도 막지 못한다. 셋째, 유효성을 `i < no`가 아니라 `board[i] != ""`로 판단하므로, `no` 이상의 칸에 이전 문자열이 남아 있으면 `del(no)`처럼 존재하지 않는 게시물의 삭제가 성공한 것으로 처리되고 `no`가 잘못 줄어든다. 예를 들어 `A`, `B`, `C`, `D`, `E`를 추가한 뒤 `del(2)`, `del(0)`을 실행하면 `board[3]`에 `E`의 복사본이 남아, `del(3)`이 실패하지 않고 목록의 마지막 게시물 `E`를 사라지게 한다.

> **주의:** **삭제 후 비우는 칸.** 게시물을 당긴 뒤 비워야 하는 칸은 마지막 게시물이 있던 `board[no - 1]`인데, 코드는 `board[no]`를 비운다. 게시판이 가득 찬 상태(`no == 5`)에서 `del(2)`를 실행하면 범위 밖의 `board[5]`에 쓰게 되어 정의되지 않은 동작이고, 그렇지 않을 때도 마지막 게시물의 복사본이 `board[no - 1]`에 남아 위의 셋째 문제를 일으킨다. 또 `modify()`는 `no >= Board::no`만 검사하므로 음수 번호를 막지 못한다.

다음은 두 문제를 고친 `del()`이다. Lab 파일의 코드가 아니라 비교를 위해 고친 구현이며, 같은 `main()`으로 실행하면 출력은 그대로이고 범위 밖 접근이 없어진다.

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

## 연습 문제 2: 구구단

**문제:** 디폴트 매개변수를 사용하여 `print99(3)`은 3단만, `print99()`는 1단부터 9단까지 전체를 출력하는 `print99()` 함수를 작성한다. 전체 문제는 [연습 문제 2](Exercises.ko.md#연습-문제-2-구구단)에 있다.

### 풀이

하나의 함수로 두 가지 호출을 처리하려면 인수를 생략한 경우를 구별할 디폴트 값이 필요하다. 풀이는 단으로 쓰이지 않는 값 19를 디폴트로 정해, `i`가 19이면 전체 단을, 그렇지 않으면 `i`단만 출력한다. 1~9 밖의 값이면 어느 것이든 이 표지 역할을 할 수 있으며, 0을 쓰는 방법도 흔하다.

**코드:** [Lab_Q2.cpp](../../Coursework/Lab06_Overloading-and-Static-Members/Lab_Q2.cpp)

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

- `print99(3)`은 `i = 3`이므로 첫 번째 분기에서 `3단` 제목과 `3 * 1 = 3`부터 `3 * 9 = 27`까지의 식을 출력한 뒤 빈 줄을 출력한다.
- `print99()`는 컴파일러가 `print99(19)`로 바꾸므로 두 번째 분기에서 `k`를 1부터 9까지 바꾸며 같은 형식으로 아홉 단을 출력한다.

**실행 결과:** 출력은 실행 예시와 같다. 3단(제목 1줄, 식 9줄, 빈 줄 1줄)과 1단부터 9단까지의 아홉 단을 합쳐 모두 110줄이며, 실행 예시에서 `(중략)`으로 생략한 부분에는 `1 * 4 = 4`부터 `9 * 6 = 54`까지가 들어간다.

> **참고:** 표지 값을 디폴트로 쓰는 방식에서는 `print99(19)`처럼 그 값을 직접 넘겨도 전체 단이 출력되고, `print99(10)`처럼 범위를 벗어난 단도 검사 없이 출력된다. 문제의 두 호출만 다루면 충분하지만, 범위를 검사하려면 `i`가 1 이상 9 이하인지 확인하는 조건을 추가한다.
