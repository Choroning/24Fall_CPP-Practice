# Chapter 03 — 클래스와 객체

> **최종 수정일:** 2026-03-16

---

## 목차

- [1. 객체지향 개념](#1-객체지향-개념)
  - [1.1 객체](#11-객체)
  - [1.2 클래스 vs 객체](#12-클래스-vs-객체)
  - [1.3 C에서 C++로 -- struct vs class](#13-c에서-c로----struct-vs-class)
- [2. 클래스 정의](#2-클래스-정의)
  - [2.1 문법](#21-문법)
  - [2.2 멤버 변수](#22-멤버-변수)
  - [2.3 멤버 함수](#23-멤버-함수)
  - [2.4 접근 지정자](#24-접근-지정자)
- [3. 객체 생성과 사용](#3-객체-생성과-사용)
  - [3.1 선언](#31-선언)
  - [3.2 객체 배열](#32-객체-배열)
- [4. 생성자와 소멸자](#4-생성자와-소멸자)
  - [4.1 생성자](#41-생성자)
  - [4.2 생성자 오버로딩](#42-생성자-오버로딩)
  - [4.3 기본 생성자](#43-기본-생성자)
  - [4.4 소멸자](#44-소멸자)
  - [4.5 실행 순서](#45-실행-순서)
- [5. 인라인 함수](#5-인라인-함수)
- [6. C++ 구조체](#6-c-구조체)
- [7. 헤더 파일과 소스 파일 분리](#7-헤더-파일과-소스-파일-분리)
- [8. 완전한 실행 예제](#8-완전한-실행-예제)
  - [8.1 Circle 클래스 -- 생성자, getter, 넓이 계산](#81-circle-클래스----생성자-getter-넓이-계산)
  - [8.2 생성자와 소멸자의 실행 순서](#82-생성자와-소멸자의-실행-순서)
  - [8.3 멤버 함수 -- 클래스 내부 vs 외부 정의](#83-멤버-함수----클래스-내부-vs-외부-정의)
- [9. 흔한 실수 모음](#9-흔한-실수-모음)
- [요약](#요약)

---

<br>

## 1. 객체지향 개념

### 1.1 객체

객체지향 프로그래밍(OOP)에서 **모든 것은 객체로 모델링**된다. 객체는 다음 두 가지를 하나로 묶는다:

- **상태(state)** — **멤버 변수**(필드)에 저장되는 데이터
- **행위(behavior)** — **멤버 함수**(메서드)로 정의되는 동작

**캡슐화(encapsulation)**는 내부 구현을 숨기고 잘 정의된 인터페이스만 외부에 공개하는 원칙이다. 외부에서는 공개된 메서드를 통해서만 객체와 상호작용하며, 내부 데이터의 구조나 변경 방식을 알 필요가 없다.

```
┌──────────────────────────┐
│           객체            │
│                          │
│  ┌────────────────────┐  │
│  │  상태 (private)     │  │
│  │  - 멤버 변수         │  │
│  └────────────────────┘  │
│                          │
│  ┌────────────────────┐  │
│  │  행위 (public)      │  │
│  │  - 멤버 함수         │  │
│  └────────────────────┘  │
└──────────────────────────┘
```

#### 캡슐화가 왜 중요한가?

캡슐화가 없으면 누구나 데이터에 **잘못된 값**을 넣을 수 있고, 버그를 막을 방법이 없다:

```cpp
// 나쁜 예: radius가 public — 보호 장치 없음
class Circle {
public:
    int radius;
    double getArea() { return 3.14 * radius * radius; }
};

Circle c;
c.radius = -5;             // 아무도 막지 않는다!
cout << c.getArea();        // 78.5 출력 — "음수" 반지름이 양수 넓이를 만들지만
                            // 의미 없는 결과. 객체가 잘못된 상태에 놓임.
```

캡슐화를 적용하면 클래스가 setter를 통해 **데이터를 검증**하여 잘못된 상태를 방지한다:

```cpp
// 좋은 예: radius는 private — setter가 규칙을 강제
class Circle {
    int radius;
public:
    void setRadius(int r) {
        if (r > 0)
            radius = r;     // 양수만 허용
        else
            radius = 1;     // 안전한 기본값으로 대체
    }
    double getArea() { return 3.14 * radius * radius; }
};

Circle c;
c.setRadius(-5);            // setter가 -5를 거부, radius를 1로 설정
cout << c.getArea();         // 3.14 출력 — 항상 유효한 결과
```

#### 실생활 비유 -- 은행 계좌

**은행 계좌**를 생각해 보자. 잔액(balance)은 비공개 데이터이다. 은행 금고에 들어가서 장부의 숫자를 직접 바꿀 수는 없다. 반드시 **입금**과 **출금** 절차를 거쳐야 하며, 이 절차에서 거래의 유효성을 검사한다 (예: 잔액보다 많은 금액은 출금 불가).

```cpp
class BankAccount {
    int balance;               // private — 직접 접근 불가
public:
    BankAccount() { balance = 0; }
    void deposit(int amount) {
        if (amount > 0)        // 검증: 양수만 입금 가능
            balance += amount;
    }
    bool withdraw(int amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;       // 성공
        }
        return false;          // 실패: 잘못된 금액 또는 잔액 부족
    }
    int getBalance() { return balance; }
};
```

이것이 실전에서의 캡슐화이다: **데이터는 보호**되고, 모든 변경은 **통제되고 검증된 메서드**를 통해서만 이루어진다.

### 1.2 클래스 vs 객체

| | 클래스 | 객체 |
|:--|:------|:-----|
| 정의 | 설계도 / 틀 | 클래스로부터 생성된 실체(인스턴스) |
| 메모리 | 메모리에 할당되지 않음 | 생성 시 메모리에 할당됨 |
| 비유 | 쿠키 틀 | 쿠키 |
| 개수 | 클래스 정의는 하나 | 하나의 클래스로 여러 객체 생성 가능 |

각 객체는 멤버 변수의 **독립적인 사본**을 가진다. 한 객체의 데이터를 변경해도 다른 객체에는 영향을 주지 않는다.

```cpp
class Circle {
    int radius;       // 각 Circle 객체가 자신만의 radius를 가짐
public:
    double getArea();
};

Circle a, b;          // 하나의 클래스로 생성된 두 개의 독립적인 객체
```

### 1.3 C에서 C++로 -- struct vs class

C를 먼저 배웠다면 `struct`를 멤버 함수나 접근 제어 없이 순수 데이터 컨테이너로만 사용했을 것이다. C++에서 `class`는 struct 개념을 훨씬 확장한다.

| 특성 | C `struct` | C++ `class` |
|:-----|:-----------|:------------|
| 데이터 멤버 | 있음 | 있음 |
| 멤버 함수 | 없음 (별도 함수 사용) | 있음 (내부에 포함) |
| 접근 제어 (`private`/`public`) | 없음 (모두 공개) | 있음 (기본값 `private`) |
| 생성자 / 소멸자 | 없음 | 있음 |
| 캡슐화 | 프로그래머의 주의에만 의존 | 컴파일러가 강제 |

**나란히 비교 -- 같은 Circle 문제를 C와 C++로:**

**C 방식 (struct + 별도 함수):**

```c
#include <stdio.h>

struct Circle {
    int radius;          /* 모든 데이터가 공개 — 제한 방법 없음 */
};

double getArea(struct Circle c) {    /* 함수가 struct와 분리됨 */
    return 3.14 * c.radius * c.radius;
}

int main() {
    struct Circle c;
    c.radius = 5;        /* 직접 접근 — 검증 불가 */
    printf("Area: %.2f\n", getArea(c));

    c.radius = -3;       /* 아무것도 막지 않는다! */
    printf("Area: %.2f\n", getArea(c));
    return 0;
}
```

**C++ 방식 (캡슐화된 class):**

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;                      // 기본적으로 private
public:
    Circle(int r) {                  // 생성자가 객체를 초기화
        radius = (r > 0) ? r : 1;
    }
    double getArea() {               // 멤버 함수 — 데이터와 함께 묶임
        return 3.14 * radius * radius;
    }
};

int main() {
    Circle c(5);                     // 생성자로 초기화
    cout << "Area: " << c.getArea() << endl;

    // c.radius = -3;               // 컴파일 에러! radius는 private
    return 0;
}
```

> **핵심 인사이트:** C에서는 데이터의 유효성을 유지하는 것이 전적으로 프로그래머의 주의에 달려 있다. C++에서는 접근 제어와 생성자를 통해 컴파일러가 이를 강제하여, 실행 시점이 아닌 컴파일 시점에 실수를 잡아낸다.

---

<br>

## 2. 클래스 정의

### 2.1 문법

```cpp
class ClassName {
private:       // 접근 지정자
    // private 멤버
protected:
    // protected 멤버
public:
    // public 멤버
};             // ← 세미콜론 필수!
```

- `class` 키워드로 클래스를 정의한다.
- 멤버는 **접근 지정자** (`private`, `protected`, `public`) 아래에 그룹화된다.
- 접근 지정자를 생략하면 **기본값은 `private`**이다.
- 닫는 중괄호 뒤에 **세미콜론**을 반드시 붙여야 한다.

### 2.2 멤버 변수

멤버 변수는 객체의 **상태**(데이터)를 저장한다. 캡슐화를 위해 일반적으로 `private`으로 선언한다.

```cpp
class Rectangle {
private:
    int width;
    int height;
};
```

### 2.3 멤버 함수

멤버 함수는 객체의 **행위**를 정의한다. 외부에서 객체와 상호작용할 수 있도록 일반적으로 `public`으로 선언한다.

멤버 함수는 두 가지 방식으로 정의할 수 있다:

**클래스 내부에서 정의 (암시적 인라인):**

```cpp
class Circle {
    int radius;
public:
    double getArea() {
        return 3.14 * radius * radius;
    }
};
```

**클래스 외부에서 범위 지정 연산자 `::`를 사용하여 정의:**

```cpp
class Circle {
    int radius;
public:
    double getArea();
};

double Circle::getArea() {
    return 3.14 * radius * radius;
}
```

### 2.4 접근 지정자

| 지정자 | 접근 가능 범위 | 일반적 용도 |
|:-------|:-------------|:-----------|
| `private` | 같은 클래스 내부에서만 | 멤버 변수, 내부 도우미 함수 |
| `protected` | 같은 클래스 + 파생 클래스 | 하위 클래스와 공유할 멤버 |
| `public` | 어디서든 | 인터페이스 (멤버 함수, getter) |

```cpp
class Account {
private:
    int balance;          // Account 자신의 메서드만 접근 가능
protected:
    int accountNumber;    // Account + 파생 클래스에서 접근 가능
public:
    int getBalance() {    // 누구나 호출 가능
        return balance;
    }
};
```

> **핵심:** 클래스 외부에서 `private` 멤버에 접근하면 컴파일러가 오류를 발생시킨다. 이것이 C++가 컴파일 시점에 캡슐화를 강제하는 방식이다.

---

<br>

## 3. 객체 생성과 사용

### 3.1 선언

객체를 선언하면 모든 멤버 변수에 대한 메모리가 할당된다. **점 연산자** (`.`)로 public 멤버에 접근한다.

```cpp
Circle c;               // 객체 생성 — 메모리 할당
c.setRadius(5);         // 점 연산자로 멤버 함수 호출
double area = c.getArea();
```

> **경고:** 클래스 외부에서 `private` 멤버에 직접 접근할 수 없다:
> ```cpp
> c.radius = 5;  // ❌ 컴파일 에러 — radius는 private
> ```
> public setter를 사용하거나 생성자를 통해 값을 전달해야 한다.

### 3.2 객체 배열

객체 배열은 여러 개의 독립적인 인스턴스를 생성한다. 각 원소는 별개의 객체이다.

```cpp
Circle circles[3];     // 3개의 Circle 객체, 각각 독립적인 radius 보유

circles[0].setRadius(1);
circles[1].setRadius(2);
circles[2].setRadius(3);
```

배열 생성 시 각 원소에 대해 **기본 생성자**가 호출된다.

---

<br>

## 4. 생성자와 소멸자

### 4.1 생성자

**생성자(constructor)**는 객체 생성 시 초기화를 수행하는 특수 멤버 함수이다.

- 클래스와 **같은 이름**을 가진다.
- **리턴 타입이 없다** (`void`도 아님).
- 객체 생성 시 **자동으로 호출**된다.
- **오버로딩**이 가능하다 (매개변수가 다른 여러 생성자 정의).

#### 생성자가 왜 필요한가?

생성자가 없으면 객체의 멤버 변수에 **쓰레기 값**(이전에 그 메모리에 있던 값)이 들어 있다. 초기화되지 않은 객체를 사용하면 예측 불가능한 동작이 발생한다:

```cpp
class Circle {
    int radius;    // 생성자를 정의하지 않음
public:
    double getArea() { return 3.14 * radius * radius; }
};

Circle c;
cout << c.getArea();   // radius가 쓰레기 값 (예: -858993460) → 의미 없는 결과!
                       // 컴파일과 실행은 되지만, 출력이 잘못됨.
```

생성자는 모든 객체가 **유효한 상태에서 시작**하는 것을 보장한다. 프로그래머가 "초기화 함수를 호출해야 한다"는 것을 기억할 필요 없이, 초기화가 자동으로 이루어진다.

```cpp
class Circle {
    int radius;
public:
    Circle() {              // 기본 생성자 (매개변수 없음)
        radius = 1;
    }
    Circle(int r) {         // 매개변수가 있는 생성자
        radius = r;
    }
    double getArea() {
        return 3.14 * radius * radius;
    }
};

Circle a;        // Circle() 호출 → radius = 1
Circle b(10);    // Circle(int) 호출 → radius = 10
```

### 4.2 생성자 오버로딩

**매개변수 목록이 다르면** (개수 또는 타입) 여러 생성자를 함께 정의할 수 있다.

```cpp
class Point {
    int x, y;
public:
    Point() { x = 0; y = 0; }
    Point(int a) { x = a; y = a; }
    Point(int a, int b) { x = a; y = b; }
};

Point p1;         // (0, 0)
Point p2(5);      // (5, 5)
Point p3(3, 7);   // (3, 7)
```

### 4.3 기본 생성자

프로그래머가 **생성자를 하나도 정의하지 않으면**, 컴파일러가 아무 일도 하지 않는 **기본 생성자**를 자동으로 제공한다 (멤버 변수는 초기화되지 않음).

그러나 **어떤 생성자라도 하나 정의하면**, 컴파일러는 기본 생성자를 **제공하지 않는다**. 기본 생성자가 필요하면 직접 정의해야 한다.

```cpp
class Box {
    int size;
public:
    Box(int s) { size = s; }   // 매개변수가 있는 생성자만 정의
};

Box a;       // ❌ 컴파일 에러 — 기본 생성자 없음!
Box b(10);   // ✅ 정상
```

> **핵심:** `Box(int s)`를 정의했지만 `Box()`도 필요하다면, 반드시 둘 다 작성해야 한다.

### 4.4 소멸자

**소멸자(destructor)**는 객체가 소멸될 때 (스코프를 벗어나거나 `delete`될 때) 자동으로 호출된다.

- 이름: `~ClassName()`
- 매개변수 없음, 리턴 타입 없음.
- **오버로딩 불가** (클래스당 소멸자는 하나뿐).
- **정리 작업** 용도 (동적 메모리 해제, 파일 닫기 등).

```cpp
class MyArray {
    int *data;
public:
    MyArray(int size) {
        data = new int[size];
    }
    ~MyArray() {
        delete[] data;    // 정리: 힙 메모리 해제
    }
};
```

### 4.5 실행 순서

- 객체는 **선언 순서대로 생성**된다.
- 객체는 **역순으로 소멸**된다 (마지막에 생성된 것이 먼저 소멸).
- 지역 객체는 포함하는 **스코프가 종료**될 때 소멸된다.

```cpp
void func() {
    Circle a(1);     // 1번째 생성
    Circle b(2);     // 2번째 생성
}                    // b 먼저 소멸, 그 다음 a 소멸
```

```
생성 순서: a → b
소멸 순서: b → a
```

---

<br>

## 5. 인라인 함수

**인라인 함수(inline function)**는 함수 호출 위치에 함수 본문을 삽입하여 함수 호출 오버헤드를 피하도록 컴파일러에게 요청하는 것이다.

```cpp
inline int square(int n) {
    return n * n;
}
```

- **작고 자주 호출되는** 함수에 적합하다.
- 크거나 복잡한 함수에는 컴파일러가 `inline` 힌트를 무시할 수 있다.
- **클래스 본문 내부에서 정의된** 멤버 함수는 암시적으로 인라인이다.

```cpp
class Circle {
    int radius;
public:
    // 암시적 인라인 — 클래스 내부에서 정의
    double getArea() { return 3.14 * radius * radius; }
};
```

> **참고:** `inline`은 명령이 아니라 힌트이다. 현대 컴파일러는 키워드와 관계없이 유리한 경우 자동으로 인라인을 수행한다.

---

<br>

## 6. C++ 구조체

C++에서 `struct`는 `class`와 거의 동일하다. **유일한 차이**는 기본 접근 수준이다.

| 특성 | `struct` | `class` |
|:----|:--------:|:-------:|
| 기본 접근 수준 | `public` | `private` |
| 멤버 함수 가능 | 가능 | 가능 |
| 생성자/소멸자 가능 | 가능 | 가능 |
| 상속 가능 | 가능 | 가능 |

```cpp
struct Point {
    int x, y;        // 기본적으로 public
    void print() {
        cout << x << ", " << y << endl;
    }
};

Point p;
p.x = 3;            // ✅ 정상 — 기본이 public
```

> **관례:** 대부분 public 멤버를 가진 단순 데이터 구조에는 `struct`를, 캡슐화와 접근 제어가 중요한 경우에는 `class`를 사용한다.

---

<br>

## 7. 헤더 파일과 소스 파일 분리

규모가 큰 프로젝트에서는 클래스 **선언**을 헤더 파일(`.h`)에, 멤버 함수 **정의**를 소스 파일(`.cpp`)에 분리한다.

**`Circle.h`** — 클래스 선언:

```cpp
#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
    int radius;
public:
    Circle();
    Circle(int r);
    double getArea();
};

#endif
```

**`Circle.cpp`** — 멤버 함수 정의:

```cpp
#include "Circle.h"

Circle::Circle() {
    radius = 1;
}

Circle::Circle(int r) {
    radius = r;
}

double Circle::getArea() {
    return 3.14 * radius * radius;
}
```

**인클루드 가드(include guard)**는 같은 헤더가 여러 번 포함되는 것을 방지한다:

| 방법 | 문법 |
|:----|:----|
| 전통적 방법 | `#ifndef CIRCLE_H` / `#define CIRCLE_H` / `#endif` |
| 현대적 방법 | `#pragma once` (널리 지원, 간결) |

> **팁:** `#pragma once`는 C++ 표준은 아니지만 주요 컴파일러(GCC, Clang, MSVC) 모두 지원한다.

---

<br>

## 8. 완전한 실행 예제

아래 프로그램들은 독립적으로 실행 가능하다. 아무 파일에 복사하여 `g++ -o program filename.cpp`로 컴파일하고 `./program`으로 실행할 수 있다.

### 8.1 Circle 클래스 -- 생성자, getter, 넓이 계산

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() {
        radius = 1;
    }
    Circle(int r) {
        radius = (r > 0) ? r : 1;    // 검증: 양수가 아니면 거부
    }
    int getRadius() {
        return radius;
    }
    void setRadius(int r) {
        if (r > 0)
            radius = r;
    }
    double getArea() {
        return 3.14 * radius * radius;
    }
};

int main() {
    Circle c1;          // 기본 생성자 → radius = 1
    Circle c2(5);       // 매개변수 생성자 → radius = 5
    Circle c3(-3);      // 잘못된 값 → 생성자가 radius = 1로 설정

    cout << "c1: radius=" << c1.getRadius() << ", area=" << c1.getArea() << endl;
    cout << "c2: radius=" << c2.getRadius() << ", area=" << c2.getArea() << endl;
    cout << "c3: radius=" << c3.getRadius() << ", area=" << c3.getArea() << endl;

    c2.setRadius(10);
    cout << "c2 after setRadius(10): area=" << c2.getArea() << endl;

    return 0;
}
```

**예상 출력:**

```
c1: radius=1, area=3.14
c2: radius=5, area=78.5
c3: radius=1, area=3.14
c2 after setRadius(10): area=314
```

### 8.2 생성자와 소멸자의 실행 순서

```cpp
#include <iostream>
using namespace std;

class Tracer {
    string name;
public:
    Tracer(string n) : name(n) {
        cout << "  Constructor: " << name << " created" << endl;
    }
    ~Tracer() {
        cout << "  Destructor:  " << name << " destroyed" << endl;
    }
};

void innerFunction() {
    cout << "[Entering innerFunction]" << endl;
    Tracer t3("C");
    Tracer t4("D");
    cout << "[Leaving innerFunction]" << endl;
}

int main() {
    cout << "[Entering main]" << endl;
    Tracer t1("A");
    Tracer t2("B");

    innerFunction();

    cout << "[Leaving main]" << endl;
    return 0;
}
```

**예상 출력:**

```
[Entering main]
  Constructor: A created
  Constructor: B created
[Entering innerFunction]
  Constructor: C created
  Constructor: D created
[Leaving innerFunction]
  Destructor:  D destroyed
  Destructor:  C destroyed
[Leaving main]
  Destructor:  B destroyed
  Destructor:  A destroyed
```

주목: 각 스코프 안에서 소멸은 생성의 **역순**으로 일어난다.

### 8.3 멤버 함수 -- 클래스 내부 vs 외부 정의

```cpp
#include <iostream>
using namespace std;

class Rectangle {
    int width, height;
public:
    Rectangle(int w, int h) {
        width = w;
        height = h;
    }

    // 클래스 내부에서 정의 (암시적 인라인)
    int getWidth() { return width; }
    int getHeight() { return height; }

    // 여기서는 선언만 — 정의는 아래에서
    int getArea();
    void print();
};

// 클래스 외부에서 범위 지정 연산자 ::를 사용하여 정의
int Rectangle::getArea() {
    return width * height;
}

void Rectangle::print() {
    cout << width << " x " << height << " = " << getArea() << endl;
}

int main() {
    Rectangle r1(3, 4);
    Rectangle r2(10, 5);

    r1.print();     // 3 x 4 = 12
    r2.print();     // 10 x 5 = 50

    return 0;
}
```

**예상 출력:**

```
3 x 4 = 12
10 x 5 = 50
```

> **언제 어떤 방식을 쓸까?** getter/setter 같은 짧은 한 줄 함수는 클래스 내부에서 정의하는 것이 일반적이다. 긴 함수는 클래스 선언의 가독성을 위해 외부에서 정의한다.

---

<br>

## 9. 흔한 실수 모음

이 섹션은 클래스를 처음 사용할 때 가장 자주 발생하는 오류를 정리한다. 미리 알아두면 디버깅 시간을 절약할 수 있다.

### 9.1 클래스 정의 끝에 세미콜론 빠뜨리기

```cpp
class Circle {
    int radius;
public:
    double getArea() { return 3.14 * radius * radius; }
}           // 잘못됨: 세미콜론 누락

int main() { ... }
```

컴파일러가 클래스 정의 **뒤쪽 줄**을 가리키는 혼란스러운 에러를 출력한다. 반드시 `};`으로 끝내야 한다.

```cpp
class Circle {
    int radius;
public:
    double getArea() { return 3.14 * radius * radius; }
};          // 올바름
```

### 9.2 클래스 외부에서 private 멤버에 접근

```cpp
Circle c;
c.radius = 5;   // 잘못됨: 컴파일 에러 — radius는 private
```

**해결:** public setter를 사용하거나 생성자를 통해 값을 전달한다.

```cpp
Circle c(5);           // 생성자로 전달
// 또는
c.setRadius(5);        // public setter로 전달
```

### 9.3 다른 생성자를 정의한 후 기본 생성자가 사라짐

**어떤** 생성자라도 하나 정의하면, 컴파일러가 자동으로 제공하던 기본 생성자가 사라진다.

```cpp
class Box {
    int size;
public:
    Box(int s) { size = s; }
};

Box b;       // 잘못됨: 컴파일 에러 — 기본 생성자 없음
```

**해결:** 기본 생성자가 필요하면 직접 정의한다.

```cpp
class Box {
    int size;
public:
    Box() { size = 0; }           // 이제 작동함
    Box(int s) { size = s; }
};

Box b;       // OK — Box() 호출
```

### 9.4 생성자를 일반 함수처럼 호출

생성자는 **객체 생성 시에만** 호출된다. 이미 만들어진 객체에서 다시 호출할 수 없다.

```cpp
Circle c;
c.Circle(5);    // 잘못됨: 생성자를 이렇게 호출할 수 없음
```

**해결:** 생성 시 초기화하거나, setter로 나중에 값을 변경한다.

```cpp
Circle c(5);         // 올바름: 생성 시 생성자 호출
// 나중에:
c.setRadius(10);     // 올바름: setter로 수정
```

### 9.5 클래스 정의와 객체 생성을 혼동

클래스 정의만으로는 객체가 **생성되지 않는다**. 해당 클래스 타입의 변수를 선언해야 비로소 메모리가 할당된다.

```cpp
class Circle {
    int radius;
public:
    Circle(int r) { radius = r; }
};
// 아직 Circle 객체가 존재하지 않음!

Circle c(5);    // 이제서야 객체가 생성되고 메모리가 할당됨
```

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 객체 | 상태(멤버 변수)와 행위(멤버 함수)를 하나로 묶음 |
| 캡슐화 | 내부는 숨기고(`private`), 인터페이스만 공개(`public`) |
| 클래스 vs 객체 | 클래스 = 설계도(메모리 없음); 객체 = 인스턴스(메모리 할당) |
| 클래스 문법 | `class Name { ... };` — 세미콜론 필수; 기본 접근은 `private` |
| 멤버 변수 | 객체의 상태를 저장; 일반적으로 `private` |
| 멤버 함수 | 객체의 행위를 정의; 클래스 내부 또는 외부(`::`)에서 정의 |
| 접근 지정자 | `private`(클래스 내부만), `protected`(클래스 + 파생), `public`(어디서든) |
| 객체 생성 | `ClassName obj;`로 메모리 할당; 점 연산자로 접근 |
| 객체 배열 | `ClassName arr[n];` — 각 원소는 독립적; 기본 생성자 호출 |
| 생성자 | 클래스와 같은 이름, 리턴 타입 없음, 자동 호출, 오버로딩 가능 |
| 기본 생성자 | 생성자를 하나도 정의하지 않은 경우에만 컴파일러가 제공 |
| 소멸자 | `~ClassName()`, 매개변수 없음, 오버로딩 불가, 객체 소멸 시 호출 |
| 실행 순서 | 선언 순서대로 생성; 역순으로 소멸 |
| 인라인 함수 | `inline` 또는 클래스 내부 정의; 컴파일러가 호출을 본문으로 대체 |
| C++ `struct` | `class`와 동일하나 기본 접근이 `public` |
| 파일 분리 | `.h`에 선언, `.cpp`에 정의; 인클루드 가드 사용 |

---
