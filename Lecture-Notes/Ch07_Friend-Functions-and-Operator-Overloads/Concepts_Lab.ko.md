# 7장 실습 — 프렌드 함수와 연산자 중복

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **최종 수정일:** 2026-11-19

> **학습 목표:**
> 1. 객체를 수정하는 일반 함수에서 매개변수를 참조로 받아야 하는 이유를 설명할 수 있다
> 2. 클래스의 `private` 멤버에 `public` 함수로 접근하는 방법과 프렌드 선언으로 접근하는 방법을 비교할 수 있다
> 3. 일반 함수로 구현한 기능이 연산자 중복으로 바뀌는 과정을 설명할 수 있다

---

## 목차

- [1. 개요](#1-개요)
- [2. 객체를 수정하는 일반 함수](#2-객체를-수정하는-일반-함수)
  - [2.1 참조 매개변수와 public 함수](#21-참조-매개변수와-public-함수)
  - [2.2 값으로 받을 때의 문제](#22-값으로-받을-때의-문제)
  - [2.3 프렌드 함수로 바꾸기](#23-프렌드-함수로-바꾸기)
  - [2.4 포인터로 받는 경우](#24-포인터로-받는-경우)
- [3. 일반 함수에서 연산자 함수로](#3-일반-함수에서-연산자-함수로)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 개요

> **참고:** 프렌드 함수와 프렌드 클래스, 프렌드 선언의 세 형태와 전방 선언, 이항, 단항, 스트림 연산자의 중복은 [7장 강의 노트](Concepts_Lecture.ko.md)에서 다룬다. 이 실습은 일반 함수로 객체를 수정하는 데서 출발하여, 같은 기능이 프렌드 함수와 연산자 함수로 이어지는 과정을 따라간다.

| 주제 | 위치 |
|:-----|:-----|
| 책의 가격을 줄이는 일반 함수 `Sub()`, 프렌드 함수로 바꾼 `Sub()` | 2절 |
| 같은 기능을 `+=`, `-=` 연산자로 구현하는 `Book` 클래스 | [HW7 문제 1](../../Coursework/HW07_Operator-Overloading/Original/Assignment.ko.md#문제-1-12주차-강의자료-실습-2-2) |
| 문자열과 비교하는 `==`, 다른 책과 비교하는 `==` | [HW7 문제 2](../../Coursework/HW07_Operator-Overloading/Original/Assignment.ko.md#문제-2-12주차-강의자료-실습-3) |
| `static` 멤버로 현재 객체 수를 세는 `Circle` | [HW6 문제 4](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.ko.md#문제-4-11주차-강의자료-실습-4) |
| `1 + a`를 가능하게 하는 `+`, 프렌드 함수로 쓴 전위와 후위 `++`, `Matrix`의 `<<`와 `>>` | [HW8 문제 1\~3](../../Coursework/HW08_Friend-Functions-and-Operator-Overloading/Original/Assignment.ko.md) |
| 분수, 복소수, 시간 클래스의 연산자 중복 | [7장 연습 문제 1\~3](Exercises.ko.md) |

*표 1. 실습의 주제와 위치*

모든 코드는 C++17 표준으로 컴파일하여 실행한다(예: `g++ -std=c++17 -Wall`).

---

<br>

## 2. 객체를 수정하는 일반 함수

**문제:** 다음 `Book` 클래스와 `main()`이 실행되도록, 책의 가격을 주어진 금액만큼 줄이는 일반 함수 `Sub()`를 작성한다.

```cpp
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
        cout << title << ' ' << price << "원 " << pages << " 페이지" << endl;
    }
    string getTitle() { return title; }
    int getPrice(void) { return price; }
    void setPrice(int p) { price = p; }
};

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    Sub(a, 500);                        // 책 a의 가격 500원 감소
    Sub(b, 1000);                       // 책 b의 가격 1000원 감소
    a.show();
    b.show();
}
```

### 2.1 참조 매개변수와 public 함수

```cpp
void Sub(Book& b, int amount) {         // 참조로 받아 원래 객체를 바꿈
    b.setPrice(b.getPrice() - amount);  // public 함수로 price에 접근
}
```

```text
청춘 19500원 300 페이지
미래 29000원 500 페이지
```

- `Sub()`는 `Book`의 멤버가 아닌 일반 함수이므로 `private` 멤버 `price`에 직접 접근할 수 없다. 그래서 `public` 함수 `getPrice()`로 현재 가격을 읽고 `setPrice()`로 새 가격을 쓴다.
- 매개변수 `Book& b`는 호출한 쪽의 객체 `a`, `b`의 별명이므로, `setPrice()`가 원래 객체의 가격을 바꾼다.

### 2.2 값으로 받을 때의 문제

매개변수를 `Book b`로 바꾸면 다음과 같다.

```cpp
void Sub(Book b, int amount) { b.setPrice(b.getPrice() - amount); }
```

```text
청춘 20000원 300 페이지
```

값에 의한 호출에서는 `b`가 인자 `a`의 사본이므로, 사본의 가격만 줄고 함수가 끝나면 사본은 사라진다. `a`의 가격은 20000원 그대로이다(5장 실습 2.1절). 객체를 수정하는 함수는 객체를 참조나 포인터로 받아야 한다.

### 2.3 프렌드 함수로 바꾸기

`Book`이 get/set 함수를 제공하지 않거나, 함수가 여러 `private` 멤버를 함께 다뤄야 한다면 그 함수를 클래스의 프렌드로 선언할 수 있다.

```cpp
#include <iostream>
#include <string>
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
        cout << title << ' ' << price << "원 " << pages << " 페이지" << endl;
    }
    friend void Sub(Book& b, int amount);   // Sub()에 private 멤버 접근 허용
};

void Sub(Book& b, int amount) {
    b.price -= amount;                  // 프렌드이므로 price에 직접 접근
}

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    Sub(a, 500);
    Sub(b, 1000);
    a.show();
    b.show();
}
```

```text
청춘 19500원 300 페이지
미래 29000원 500 페이지
```

- `friend void Sub(Book& b, int amount);`는 클래스 안에 있지만 `Sub()`를 멤버로 만드는 것이 아니라, 외부 함수 `Sub()`에 `private` 멤버에 접근할 권한을 주는 선언이다. 그래서 `Sub()`의 정의 앞에 `Book::`를 붙이지 않는다.
- 프렌드 선언은 캡슐화에 예외를 만드는 것이므로, `public` 함수로 충분한 경우에는 2.1절처럼 공개된 인터페이스를 쓰는 편이 클래스의 내부 구현을 바꾸기 쉽다.

### 2.4 포인터로 받는 경우

5장의 주소에 의한 호출처럼 객체의 주소를 받아도 원래 객체를 바꿀 수 있다.

```cpp
void Sub(Book *p, int amount) {         // 주소로 받아 원래 객체를 바꿈
    if (p == nullptr) return;           // 가리키는 객체가 없으면 아무것도 하지 않음
    p->setPrice(p->getPrice() - amount);
}

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    Sub(&a, 500);                       // 책 a의 주소를 넘긴다
    Sub(&b, 1000);
    Sub(nullptr, 100);                  // 대상이 없는 호출도 안전하다
    a.show();
    b.show();
}
```

```text
청춘 19500원 300 페이지
미래 29000원 500 페이지
```

- 호출하는 쪽이 `&a`처럼 주소를 넘기므로, 코드만 보아도 `Sub()`가 `a`를 바꿀 수 있다는 사실이 드러난다. 참조 버전의 `Sub(a, 500)`은 값에 의한 호출과 모양이 같아 호출만 보고는 구별되지 않는다.
- 함수 안에서는 `.` 대신 `->`로 멤버 함수를 호출한다.
- 포인터는 `nullptr`일 수 있으므로 역참조하기 전에 검사해야 한다. 참조는 항상 어떤 객체에 바인딩되어 있으므로 이런 검사가 필요 없다.

| 매개변수 | 호출 | 원래 객체의 변경 | 대상이 없는 호출 |
|:-----|:-----|:-----|:-----|
| `Book b` | `Sub(a, 500)` | 불가 (사본만 바뀜) | 불가 |
| `Book& b` | `Sub(a, 500)` | 가능 | 불가 |
| `Book *p` | `Sub(&a, 500)` | 가능 | `Sub(nullptr, 500)`으로 가능 |

*표 2. Sub()의 매개변수 형태에 따른 차이*

대상 객체가 반드시 있어야 하는 이 문제에서는 참조 버전이 가장 간단하다. 포인터 버전은 대상이 없을 수도 있는 경우, 예를 들어 검색에 실패하면 `nullptr`를 넘기는 경우에 알맞다.

---

<br>

## 3. 일반 함수에서 연산자 함수로

`Sub(a, 500)`은 함수 호출의 모양이어서 "a의 가격을 500 줄인다"는 뜻이 이름에만 담긴다. C++은 이런 함수에 연산자 기호를 붙이는 **연산자 중복(operator overloading)** 을 허용한다. 연산자 함수의 이름은 `operator` 다음에 연산자 기호를 붙인 것이며, 다음 두 방법으로 구현한다(7장 강의 노트 2.4절).

| 방법 | 선언 | `a -= 500`의 해석 |
|:-----|:-----|:-----|
| 멤버 함수 | `X& operator-=(int n);` | `a.operator-=(500)` |
| 프렌드 함수 | `friend X& operator-=(X& x, int n);` | `operator-=(a, 500)` |

*표 3. 연산자 함수를 구현하는 두 방법*

표의 `X`는 임의의 클래스이다. 2.3절의 `Sub()`처럼 객체를 바꾸는 일반 함수는 이런 연산자 함수로 바꿀 수 있으며, 복합 대입 연산자를 멤버 함수와 프렌드 함수로 구현하는 방법은 7장 강의 노트 3.3절에서 다룬다. `Book`의 `+=`, `-=`를 멤버 함수로 구현하는 문제가 [HW7 문제 1](../../Coursework/HW07_Operator-Overloading/Original/Assignment.ko.md#문제-1-12주차-강의자료-실습-2-2)이다.

> **참고:** 연산자 중복은 기존 연산자에 새 의미를 더할 뿐이며, 연산자의 피연산자 개수, 우선순위, 결합 방향은 바꿀 수 없다. 또 피연산자 중 적어도 하나는 사용자 정의 타입이어야 하므로 `int`끼리의 `+`의 의미는 바꿀 수 없다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 객체를 수정하는 함수 | 값으로 받으면 사본만 바뀌므로, 원래 객체를 바꾸려면 참조나 포인터로 받는다. |
| public 인터페이스 | 일반 함수는 `private` 멤버에 직접 접근할 수 없으므로 클래스가 제공하는 get/set 함수를 거친다. |
| 프렌드 함수 | 클래스 안의 `friend` 선언은 외부 함수에 `private` 멤버의 접근 권한을 줄 뿐 그 함수를 멤버로 만들지 않는다. |
| 연산자 함수 | `operator` 다음에 연산자 기호를 붙인 함수로, 멤버 함수나 프렌드 함수로 구현하며 연산자의 우선순위와 피연산자 개수는 바꿀 수 없다. |
| 포인터 매개변수 | 객체의 주소를 받아도 원래 객체를 바꿀 수 있으며, 호출에 `&`가 드러나고 `nullptr`를 허용하는 대신 역참조 전에 검사가 필요하다. |

---

<br>

## 점검 문제

1. 2.3절의 `Book`에서 `friend` 선언을 지우면 어떤 오류가 생기는가? 오류 없이 같은 동작을 하려면 어떻게 고치면 되는가?

   <details>
   <summary>정답</summary>

   `Sub()`가 `b.price`에 접근하는 줄에서 `price`가 `private`이라는 컴파일 오류가 난다. 프렌드 선언 없이 같은 동작을 하려면 2.1절처럼 `Book`에 `public` 함수 `getPrice()`, `setPrice()`를 두고 `Sub()`가 이를 쓰게 하거나, `Sub()`를 `Book`의 멤버 함수로 만들어 `a.Sub(500)`처럼 호출한다.

   </details>

2. 프렌드 함수 `Sub()`를 `Book` 클래스 안의 `public:` 영역에 두든 `private:` 영역에 두든 차이가 있는가?

   <details>
   <summary>정답</summary>

   차이가 없다. 프렌드 선언은 멤버가 아니므로 접근 지정자의 영향을 받지 않으며, 클래스 안 어디에 두어도 그 함수에 같은 접근 권한을 준다. 읽기 쉽도록 보통 클래스의 앞쪽이나 `public:` 영역에 모아 둔다.

   </details>

3. `a -= 500`을 멤버 함수로 구현할 때와 프렌드 함수로 구현할 때, 각각 매개변수는 몇 개인가? 그 이유는?

   <details>
   <summary>정답</summary>

   멤버 함수는 매개변수가 1개(`int amount`), 프렌드 함수는 2개(`Book& b`, `int amount`)이다. 멤버 함수는 왼쪽 피연산자 `a`가 함수를 호출한 객체, 곧 `this`가 가리키는 객체가 되므로 오른쪽 피연산자만 매개변수로 받는다. 프렌드 함수는 멤버가 아니어서 `this`가 없으므로 두 피연산자를 모두 매개변수로 받는다.

   </details>

4. 2.4절의 `Sub(Book *p, int amount)`에서 `if (p == nullptr) return;`을 지우고 `Sub(nullptr, 100);`을 호출하면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `p->getPrice()`가 널 포인터를 역참조하므로 정의되지 않은 동작이 되며, 보통 프로그램이 비정상 종료한다. 포인터 매개변수는 `nullptr`가 넘어올 수 있으므로 역참조하기 전에 검사하거나, 대상이 반드시 있어야 하는 함수라면 참조 매개변수로 바꾼다.

   </details>
