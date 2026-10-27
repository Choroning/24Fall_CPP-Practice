# HW5 코드 총정리 — 참조 리턴과 복사 생성자

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

> **학습 목표:**
> 1. 두 풀이의 클래스 선언부, 멤버 함수 정의부, `main()`을 줄 단위로 읽고 각 부분의 역할을 설명할 수 있다
> 2. `add()`를 연결한 호출에서 각 호출이 어떤 객체에 작용하고 `value`가 어떻게 바뀌는지 추적할 수 있다
> 3. 복사 생성자와 `set()`이 실행될 때 두 `Book` 객체의 `title`이 가리키는 메모리가 어떻게 바뀌는지 추적할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: Accumulator 클래스](#2-q1cpp-accumulator-클래스)
  - [2.1 전체 코드](#21-전체-코드)
  - [2.2 줄 단위 설명](#22-줄-단위-설명)
  - [2.3 실행 추적](#23-실행-추적)
- [3. Q2.cpp: Book 클래스](#3-q2cpp-book-클래스)
  - [3.1 전체 코드](#31-전체-코드)
  - [3.2 줄 단위 설명](#32-줄-단위-설명)
  - [3.3 실행 추적](#33-실행-추적)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

두 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스 선언, 클래스 밖의 멤버 함수 정의, `main()`을 차례로 둔다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW5 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 클래스 | 클래스 밖에서 정의한 멤버 | 입력 |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Accumulator` | `add()` | 없음 |
| [`Q2.cpp`](Solution/Q2.cpp) | `Book` | 생성자, 복사 생성자, 소멸자, `set()` | 없음 |

*표 1. HW5 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: Accumulator 클래스

### 2.1 전체 코드

```cpp
/**
 * @file    A5Q1.cpp
 * @brief   Q1. 자기 참조를 리턴하는 Accumulator 클래스 (10주차 실습 2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-07
 */

#include <iostream>
using namespace std;

class Accumulator {
    int value;
public:
    Accumulator(int value) { this->value = value; }
    Accumulator& add(int n);
    int get() { return value; }
};
Accumulator& Accumulator::add(int n) {
    value += n;
    return *this;
}

int main() {
    Accumulator acc(10);
    acc.add(5).add(6).add(7);   // acc의 value 멤버가 28이 된다.
    cout << acc.get() << endl;  // 28 출력
}
```

### 2.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~9 | `#include <iostream>`, `using namespace std;` | `cout`, `endl`을 쓰기 위해 `<iostream>`을 포함하고 `std::`를 생략한다 |
| 11~12 | `class Accumulator { int value;` | 접근 지정자 없이 선언한 `value`는 `private`이며 누적값을 저장한다 |
| 13~14 | `public:`, 생성자 | 클래스 안에서 정의한 생성자이다. `this->value`는 멤버 변수, 오른쪽의 `value`는 매개변수이다 |
| 15 | `Accumulator& add(int n);` | 리턴 타입이 `Accumulator&`, 즉 `Accumulator` 객체에 대한 참조인 `add()`의 선언이다 |
| 16 | `int get() { return value; }` | 현재 누적값을 리턴한다 |
| 18 | `Accumulator& Accumulator::add(int n)` | 앞의 `Accumulator&`는 리턴 타입이고, `Accumulator::`는 이 함수가 속한 클래스이다 |
| 19 | `value += n;` | 호출한 객체의 `value`에 `n`을 더한다 |
| 20 | `return *this;` | `this`가 가리키는 객체, 즉 호출한 객체 자신을 참조로 리턴한다. 복사본은 만들어지지 않는다 |
| 24 | `Accumulator acc(10);` | 생성자가 호출되어 `acc.value`가 10이 된다 |
| 25 | `acc.add(5).add(6).add(7);` | 세 `add()`가 왼쪽부터 차례로 실행되며, 모두 `acc`에 작용한다. 마지막 호출의 리턴값은 쓰이지 않는다 |
| 26 | `cout << acc.get() << endl;` | `acc.value`인 28을 출력한다 |

*표 2. Q1.cpp의 줄 단위 설명*

### 2.3 실행 추적

| 실행한 호출 | 호출 대상 | 실행 뒤 `acc.value` | 리턴값 |
|:-----|:-----|-----:|:-----|
| `Accumulator acc(10);` | (생성) | 10 | |
| `acc.add(5)` | `acc` | 15 | `acc`에 대한 참조 |
| `.add(6)` | 앞 호출이 리턴한 `acc` | 21 | `acc`에 대한 참조 |
| `.add(7)` | 앞 호출이 리턴한 `acc` | 28 | `acc`에 대한 참조 (쓰이지 않음) |
| `acc.get()` | `acc` | 28 | 28 (출력 `28`) |

*표 3. Q1의 연결 호출에 따른 `acc.value`의 변화*

`add()`가 값으로 리턴한다면 둘째 행부터 호출 대상이 `acc`가 아니라 앞 호출이 만든 임시 복사본이 된다. 그러면 `acc.value`는 15에 머물고 임시 복사본만 21, 28로 바뀌므로 출력은 15이다([HW5 과제 해설 2.2절](Assignment-Explanation.ko.md#22-설계와-해결-방법)).

---

<br>

## 3. Q2.cpp: Book 클래스

### 3.1 전체 코드

```cpp
/**
 * @file    A5Q2.cpp
 * @brief   Q2. 깊은 복사 생성자를 갖춘 Book 클래스 (10주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-07
 */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char* title;
    int price;
public:
    Book(char* title, int price);
    Book(Book& b);
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};
Book::Book(char* title, int price) {
    this->title = new char [strlen(title) + 1];
    strcpy(this->title, title);
    this->price = price;
}
Book::Book(Book& b) {
    this->price = b.price;
    this->title = new char [strlen(b.title) + 1];
    strcpy(this->title, b.title);
}
Book::~Book() {
    if (!title)
        delete [] title;
}
void Book::set(char* title, int price) {
    if (strlen(this->title) < strlen(title)) {
        delete [] this->title;
        this->title = new char [strlen(title) + 1];
    }
    strcpy(this->title, title);
    this->price = price;
}


int main() {
    Book cpp("명품c++", 10000);
    Book java = cpp;
    java.set("명품자바", 12000);
    cpp.show();
    java.show();
}
```

### 3.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~10 | `#include`, `using namespace std;` | `<cstring>`은 `strlen()`과 `strcpy()`를 제공한다 |
| 12~14 | `class Book { char* title; int price;` | `private` 멤버이다. `title`은 힙에 할당한 제목 문자열의 시작 주소를 저장한다 |
| 16 | `Book(char* title, int price);` | 생성자의 선언이다. 문자열 리터럴을 받는 매개변수이지만 `const`가 없다 |
| 17 | `Book(Book& b);` | 복사 생성자의 선언이다. 매개변수는 자기 클래스에 대한 참조이다 |
| 18 | `~Book();` | 소멸자의 선언이다 |
| 19 | `void set(char* title, int price);` | 제목과 가격을 바꾸는 함수의 선언이다 |
| 20 | `void show() { ... }` | 제목, 공백, 가격, `"원"`을 출력하고 줄을 바꾼다 |
| 22~26 | `Book::Book(char* title, int price)` | `strlen(title) + 1`바이트를 할당하고 제목을 복사한 뒤 가격을 저장한다. `this->`로 멤버와 매개변수를 구분한다 |
| 27~31 | `Book::Book(Book& b)` | 가격을 복사하고, `b.title`의 길이만큼 새로 할당하여 내용을 복사한다. 깊은 복사이다 |
| 32~35 | `Book::~Book()` | `if (!title)`은 `title`이 널 포인터일 때만 참이므로, 할당된 제목이 있으면 `delete []`가 실행되지 않는다 |
| 37~40 | `set()`의 `if` 문 | 기존 제목의 길이가 새 제목보다 짧으면 기존 메모리를 해제하고 새 길이에 맞게 다시 할당한다 |
| 41~42 | `strcpy()`, 가격 대입 | 제목을 `this->title`이 가리키는 메모리에 복사하고 가격을 바꾼다 |
| 47 | `Book cpp("명품c++", 10000);` | 생성자가 호출된다. 문자열 리터럴을 `char*`로 넘기므로 경고가 난다 |
| 48 | `Book java = cpp;` | `cpp`로 새 객체를 초기화하므로 복사 생성자가 호출된다 |
| 49 | `java.set("명품자바", 12000);` | 새 제목이 12바이트로 기존 9바이트보다 길어 다시 할당한다. 이 줄도 경고가 난다 |
| 50~51 | `cpp.show(); java.show();` | 두 책의 제목과 가격을 출력한다 |

*표 4. Q2.cpp의 줄 단위 설명*

### 3.3 실행 추적

다음 표에서 블록 A, B, C는 실행 중에 할당되는 세 개의 힙 메모리 블록이다. 괄호 안의 크기는 할당을 요청한 바이트 수이다.

| 실행한 문장 | `cpp` (`title`, `price`) | `java` (`title`, `price`) | 출력 |
|:-----|:-----|:-----|:-----|
| `Book cpp("명품c++", 10000);` | 블록 A `"명품c++"` (10), 10000 | | |
| `Book java = cpp;` | 블록 A, 10000 | 블록 B `"명품c++"` (10), 10000 | |
| `java.set(...)`의 `if` 문 | 블록 A, 10000 | 블록 B를 해제하고 블록 C (13)를 할당 | |
| `java.set(...)`의 나머지 | 블록 A, 10000 | 블록 C `"명품자바"`, 12000 | |
| `cpp.show();` | 블록 A, 10000 | 블록 C, 12000 | `명품c++ 10000원` |
| `java.show();` | 블록 A, 10000 | 블록 C, 12000 | `명품자바 12000원` |
| `main()` 종료 | `~Book()`이 해제하지 않음 | `~Book()`이 해제하지 않음 | |

*표 5. Q2의 `main()` 실행에 따른 두 객체의 상태*

블록 B는 `set()`에서 해제되었지만, 블록 A와 C는 소멸자가 해제하지 않아 프로그램이 끝날 때까지 남는다. `leaks --atExit`가 보고하는 두 누수가 이 두 블록이며, 할당 위치는 각각 22\~26행의 생성자와 36\~43행의 `set()`이다. 복사 생성자가 블록 B를 따로 할당했으므로, `java`의 제목을 바꾸어도 `cpp`의 블록 A는 영향을 받지 않는다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 파일의 구성 | 두 풀이는 헤더 포함, 클래스 선언, 클래스 밖의 멤버 함수 정의, `main()`의 순서로 되어 있다. |
| `*this`의 참조 리턴 | `add()`는 `value += n;` 뒤에 `return *this;`로 호출한 객체를 돌려주므로 `acc.value`는 10, 15, 21, 28로 바뀐다. |
| 복사 생성자의 호출 | `Book java = cpp;`는 같은 클래스의 객체로 새 객체를 초기화하므로 복사 생성자를 호출한다. |
| 블록의 추적 | `cpp`는 처음부터 끝까지 블록 A를 쓰고, `java`는 복사 생성자에서 블록 B를, `set()`에서 블록 C를 받는다. |
| 남는 메모리 | 소멸자의 `if (!title)` 때문에 블록 A와 C가 해제되지 않으며, 이것이 `leaks`가 보고하는 누수이다. |

---

<br>

## 점검 문제

1. **따로 쓴 호출:** Q1의 25행을 `acc.add(5); acc.add(6); acc.add(7);`로 나누어 쓰면 결과가 같은가? 이때는 `add()`의 리턴 타입이 `void`여도 되는가?

   <details>
   <summary>정답</summary>

   결과는 같은 28이다. 세 문장이 모두 `acc`에 대해 직접 `add()`를 호출하므로 리턴값을 쓰지 않고, 리턴 타입이 `void`여도 된다. 리턴 타입이 `void`이면 25행처럼 연결한 호출은 `void`에 멤버 함수를 호출하는 것이 되어 컴파일 오류가 난다.

   </details>

2. **중간 결과:** Q1의 25행을 `cout << acc.add(5).get() << endl;`로 바꾸면 무엇이 출력되고, 그 뒤의 26행은 무엇을 출력하는가?

   <details>
   <summary>정답</summary>

   `acc.add(5)`가 `acc`의 `value`를 15로 만들고 `acc`를 리턴하므로 `get()`은 15를 리턴하여 25행은 15를 출력한다. 26행도 같은 `acc`의 값을 읽으므로 15를 출력한다.

   </details>

3. **다시 할당하지 않는 경우:** Q2의 49행을 `java.set("C++", 12000);`으로 바꾸면 `set()`은 메모리를 다시 할당하는가? 출력은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `"C++"`는 3바이트로 기존 제목 9바이트보다 짧으므로 다시 할당하지 않고, 복사 생성자가 할당한 블록 B에 그대로 복사한다. 블록 B는 `java`만 가리키므로 `cpp`는 바뀌지 않으며, 출력은 `명품c++ 10000원`과 `C++ 12000원`이다.

   </details>

4. **복사 생성자의 다른 호출 형식:** Q2의 48행을 `Book java(cpp);`로 바꾸면 어떤 생성자가 호출되는가?

   <details>
   <summary>정답</summary>

   `Book java = cpp;`와 마찬가지로 `cpp`를 인자로 하는 복사 생성자 `Book(Book& b)`가 호출된다. 두 형식 모두 같은 클래스의 객체로 새 객체를 초기화하는 것이다.

   </details>

5. **소멸자를 고친 뒤의 해제 순서:** Q2의 소멸자를 `delete [] title;`로 고치면 프로그램이 끝날 때 어떤 블록이 어떤 순서로 해제되는가?

   <details>
   <summary>정답</summary>

   객체는 생성의 역순으로 소멸하므로 `java`의 소멸자가 먼저 블록 C를 해제하고, 이어서 `cpp`의 소멸자가 블록 A를 해제한다. 두 객체가 서로 다른 블록을 가리키므로 이중 해제가 생기지 않고, 누수도 생기지 않는다.

   </details>
