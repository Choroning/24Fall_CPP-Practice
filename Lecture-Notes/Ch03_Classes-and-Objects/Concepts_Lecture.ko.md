# 3장 — 클래스와 객체

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-11-19
>
> 명품 C++ Programming, 황기태, Ch 3

> **학습 목표:**
> 1. 실세계의 객체와 C++ 객체의 관계, 캡슐화의 목적을 설명할 수 있다
> 2. 멤버 변수와 멤버 함수를 가진 C++ 클래스를 선언부와 구현부로 나누어 작성하고 객체를 생성하여 활용할 수 있다
> 3. 생성자, 위임 생성자, 소멸자를 작성하고 객체의 생성과 소멸 순서를 설명할 수 있다
> 4. `private`, `protected`, `public` 접근 지정자의 차이를 설명할 수 있다
> 5. 인라인 함수의 목적과 장단점, C++ 구조체와 클래스의 차이를 설명할 수 있다
> 6. 클래스를 헤더 파일과 cpp 파일로 분리하여 프로그램을 작성할 수 있다

---

## 목차

- [1. 객체지향 개념](#1-객체지향-개념)
  - [1.1 객체](#11-객체)
  - [1.2 클래스 vs 객체](#12-클래스-vs-객체)
  - [1.3 C에서 C++로: struct vs class](#13-c에서-c로-struct-vs-class)
- [2. 클래스 정의](#2-클래스-정의)
  - [2.1 문법](#21-문법)
  - [2.2 멤버 변수](#22-멤버-변수)
  - [2.3 멤버 함수](#23-멤버-함수)
  - [2.4 접근 지정자](#24-접근-지정자)
  - [2.5 const 멤버 함수](#25-const-멤버-함수)
  - [2.6 객체의 메모리 배치와 크기](#26-객체의-메모리-배치와-크기)
  - [2.7 접근 제어의 단위와 검사 시점](#27-접근-제어의-단위와-검사-시점)
- [3. 객체 생성과 사용](#3-객체-생성과-사용)
  - [3.1 선언](#31-선언)
  - [3.2 객체 배열](#32-객체-배열)
- [4. 생성자와 소멸자](#4-생성자와-소멸자)
  - [4.1 생성자](#41-생성자)
  - [4.2 생성자 중복](#42-생성자-중복)
  - [4.3 위임 생성자](#43-위임-생성자)
  - [4.4 멤버 변수의 초기화 방법](#44-멤버-변수의-초기화-방법)
  - [4.5 기본 생성자](#45-기본-생성자)
  - [4.6 소멸자](#46-소멸자)
  - [4.7 실행 순서](#47-실행-순서)
  - [4.8 멤버 초기화 순서가 만드는 오류](#48-멤버-초기화-순서가-만드는-오류)
  - [4.9 객체의 종류에 따른 생성과 소멸 시점](#49-객체의-종류에-따른-생성과-소멸-시점)
  - [4.10 생성자의 종류와 컴파일러가 만드는 멤버 함수](#410-생성자의-종류와-컴파일러가-만드는-멤버-함수)
  - [4.11 생성자와 멤버 함수로 지키는 불변 조건](#411-생성자와-멤버-함수로-지키는-불변-조건)
- [5. 인라인 함수](#5-인라인-함수)
- [6. C++ 구조체](#6-c-구조체)
- [7. 헤더 파일과 소스 파일 분리](#7-헤더-파일과-소스-파일-분리)
- [8. 완전한 실행 예제](#8-완전한-실행-예제)
  - [8.1 Circle 클래스: 생성자, getter, 넓이 계산](#81-circle-클래스-생성자-getter-넓이-계산)
  - [8.2 생성자와 소멸자의 실행 순서](#82-생성자와-소멸자의-실행-순서)
  - [8.3 멤버 함수: 클래스 내부 vs 외부 정의](#83-멤버-함수-클래스-내부-vs-외부-정의)
- [9. 흔한 실수 모음](#9-흔한-실수-모음)
  - [9.1 클래스 정의 끝에 세미콜론 빠뜨리기](#91-클래스-정의-끝에-세미콜론-빠뜨리기)
  - [9.2 클래스 외부에서 private 멤버에 접근](#92-클래스-외부에서-private-멤버에-접근)
  - [9.3 다른 생성자를 정의한 후 기본 생성자가 사라짐](#93-다른-생성자를-정의한-후-기본-생성자가-사라짐)
  - [9.4 생성자를 일반 함수처럼 호출](#94-생성자를-일반-함수처럼-호출)
  - [9.5 클래스 정의와 객체 생성을 혼동](#95-클래스-정의와-객체-생성을-혼동)
  - [9.6 클래스 밖에서 멤버 함수를 정의할 때 클래스 이름을 빠뜨림](#96-클래스-밖에서-멤버-함수를-정의할-때-클래스-이름을-빠뜨림)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 객체지향 개념

### 1.1 객체

객체지향 프로그래밍(OOP)에서 **모든 것은 객체로 모델링**된다. TV, 의자, 책, 집, 카메라, 컴퓨터처럼 세상의 모든 것이 객체이다. 객체는 다음 두 가지를 하나로 묶는다.

- **상태(state)**: **멤버 변수**(필드)에 저장되는 데이터이다.
- **행위(behavior)**: **멤버 함수**(메서드)로 정의되는 동작이다.

| 구분 | TV 객체의 예 |
|:-----|:-----|
| 상태 | 켜짐 또는 꺼짐 여부, 현재 방송 중인 채널(channel), 현재 출력되는 음량(volume) |
| 행위 | 켜기(power on), 끄기(power off), 채널 증가, 채널 감소, 음량 증가, 음량 감소 |

*표 1. TV 객체의 상태와 행위*

**캡슐화(encapsulation)** 는 내부 구현을 숨기고 잘 정의된 인터페이스만 외부에 공개하는 원칙이다. 외부에서는 공개된 메서드를 통해서만 객체와 상호작용하며, 내부 데이터의 구조나 변경 방식을 알 필요가 없다.

캡슐에 든 약은 어떤 색이고 어떤 성분인지 보이지 않으며 외부로부터 안전하다. 마찬가지로 객체를 캡슐로 싸서 내부를 보호하고 볼 수 없게 하는 것이 캡슐화이며, 그 목적은 객체 안의 데이터에 대한 보안과 보호, 외부 접근의 제한이다. 다만 객체의 일부는 외부와 정보를 주고받는 인터페이스로 공개한다. TV는 내부 회로를 감추지만 리모컨 객체와 통신하기 위해 전원, 밝기, 채널, 음량 조절 버튼을 노출한다.

```mermaid
graph TD
    subgraph OBJ["객체"]
        S["상태 (private)<br>멤버 변수"]
        B["행위 (public)<br>멤버 함수"]
    end
    S ~~~ B
```

*그림 1. 상태와 행위를 묶은 객체의 구조*

#### 캡슐화가 중요한 이유

캡슐화가 없으면 누구나 데이터에 **잘못된 값**을 넣을 수 있고, 버그를 막을 방법이 없다.

```cpp
// 나쁜 예: radius가 public, 보호 장치 없음
class Circle {
public:
    int radius;
    double getArea() { return 3.14 * radius * radius; }
};

Circle c;
c.radius = -5;             // 아무도 막지 않는다!
cout << c.getArea();        // 78.5 출력: "음수" 반지름이 양수 넓이를 만들지만
                            // 의미 없는 결과. 객체가 잘못된 상태에 놓임.
```

캡슐화를 적용하면 클래스가 setter를 통해 **데이터를 검증**하여 잘못된 상태를 방지한다.

```cpp
// 좋은 예: radius는 private, setter가 규칙을 강제
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
cout << c.getArea();         // 3.14 출력: 항상 유효한 결과
```

#### 실생활 비유: 은행 계좌

**은행 계좌**를 예로 들면, 잔액(balance)은 비공개 데이터이다. 은행 금고에 들어가서 장부의 숫자를 직접 바꿀 수는 없다. 반드시 **입금**과 **출금** 절차를 거쳐야 하며, 이 절차에서 거래의 유효성을 검사한다 (예: 잔액보다 많은 금액은 출금 불가).

```cpp
class BankAccount {
    int balance;               // private: 직접 접근 불가
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

이것이 실전에서의 캡슐화이다. **데이터는 보호**되고, 모든 변경은 **통제되고 검증된 메서드**를 통해서만 이루어진다.

### 1.2 클래스 vs 객체

| | 클래스 | 객체 |
|:--|:------|:-----|
| 정의 | 설계도 / 틀 | 클래스로부터 생성된 실체(인스턴스) |
| 메모리 | 메모리에 할당되지 않음 | 생성 시 메모리에 할당됨 |
| 비유 | 쿠키 틀 | 쿠키 |
| 개수 | 클래스 정의는 하나 | 하나의 클래스로 여러 객체 생성 가능 |

*표 2. 클래스와 객체의 비교*

클래스는 객체를 만들어 내기 위해 정의한 설계도이며, 객체도 실체도 아니다. 객체는 생성될 때 클래스의 모양을 그대로 가지고 태어나며, 메모리에 생성되므로 **실체(instance)** 라고도 부른다. 각 객체는 멤버 변수의 **독립적인 사본**을 가진다. 한 객체의 데이터를 변경해도 다른 객체에는 영향을 주지 않는다.

```cpp
class Circle {
    int radius;       // 각 Circle 객체가 자신만의 radius를 가짐
public:
    double getArea();
};

Circle a, b;          // 하나의 클래스로 생성된 두 개의 독립적인 객체
```

### 1.3 C에서 C++로: struct vs class

C를 먼저 배웠다면 `struct`를 멤버 함수나 접근 제어 없이 순수 데이터 컨테이너로만 사용했을 것이다. C++에서 `class`는 struct 개념을 훨씬 확장한다.

| 특성 | C `struct` | C++ `class` |
|:-----|:-----------|:------------|
| 데이터 멤버 | 있음 | 있음 |
| 멤버 함수 | 없음 (별도 함수 사용) | 있음 (내부에 포함) |
| 접근 제어 (`private`/`public`) | 없음 (모두 공개) | 있음 (기본값 `private`) |
| 생성자 / 소멸자 | 없음 | 있음 |
| 캡슐화 | 프로그래머의 주의에만 의존 | 컴파일러가 강제 |

*표 3. C의 struct와 C++의 class 비교*

**나란히 비교: 같은 Circle 문제를 C와 C++로**

**C 방식 (struct + 별도 함수):**

```c
#include <stdio.h>

struct Circle {
    int radius;          /* 모든 데이터가 공개: 제한 방법 없음 */
};

double getArea(struct Circle c) {    /* 함수가 struct와 분리됨 */
    return 3.14 * c.radius * c.radius;
}

int main() {
    struct Circle c;
    c.radius = 5;        /* 직접 접근: 검증 불가 */
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
    double getArea() {               // 멤버 함수: 데이터와 함께 묶임
        return 3.14 * radius * radius;
    }
};

int main() {
    Circle c(5);                     // 생성자로 초기화
    cout << "Area: " << c.getArea() << endl;

    // c.radius = -3;               // 컴파일 오류! radius는 private
    return 0;
}
```

> **핵심:** C에서는 데이터의 유효성을 유지하는 것이 전적으로 프로그래머의 주의에 달려 있다. C++에서는 접근 제어로 데이터에 대한 잘못된 직접 접근을 컴파일 시점에 잡아내고, 생성자와 setter가 값을 검증하여 객체가 유효한 상태를 유지하게 한다.

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
- 접근 지정자를 생략하면 **기본값은 `private`** 이다.
- 닫는 중괄호 뒤에 **세미콜론**을 반드시 붙여야 한다.

클래스는 **클래스 선언부(class declaration)** 와 **클래스 구현부(class implementation)** 로 작성한다. 선언부는 `class` 키워드로 클래스를 선언하고 멤버 변수와 멤버 함수를 선언하며, 멤버 함수는 보통 원형(prototype) 형태로 선언한다. 구현부에는 선언부에 선언된 모든 멤버 함수를 구현(정의)한다. 선언부와 구현부를 분리하는 이유는 클래스를 다른 파일에서 활용하기 위해서이다(7절).

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

멤버 함수는 두 가지 방식으로 정의할 수 있다.

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

외부 정의 `double Circle::getArea()`는 리턴 타입, 클래스 이름, 범위 지정 연산자 `::`, 멤버 함수 이름과 매개변수의 순서로 쓴다. `Circle::`는 이 함수가 `Circle` 클래스의 멤버임을 나타낸다.

### 2.4 접근 지정자

| 지정자 | 접근 가능 범위 | 일반적 용도 |
|:-------|:-------------|:-----------|
| `private` | 같은 클래스 내부에서만 | 멤버 변수, 내부 도우미 함수 |
| `protected` | 같은 클래스 + 파생 클래스 | 하위 클래스와 공유할 멤버 |
| `public` | 어디서든 | 인터페이스 (멤버 함수, getter) |

*표 4. 접근 지정자별 접근 가능 범위*

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

C++의 캡슐화 전략은 객체의 상태를 나타내는 멤버 변수와 중요한 멤버를 다른 클래스나 객체가 접근할 수 없도록 보호하고, 외부와의 인터페이스를 위한 일부 멤버만 접근을 허용하는 것이다. 접근 지정자는 한 클래스 안에서 여러 번, 어떤 순서로든 쓸 수 있으며, 각 지정자는 다음 지정자가 나올 때까지의 멤버에 적용된다. `protected`는 상속과 함께 8장에서 자세히 다룬다.

다음 코드에서 컴파일 오류가 발생하는 곳은 `main()`의 (1), (3), (5)이다. 생성자도 멤버이므로 `private`으로 선언된 생성자는 클래스 외부에서 호출할 수 없다. 반면 멤버 함수 안에서는 `private` 멤버인 `a`와 `f()`에도 자유롭게 접근할 수 있다.

```cpp
class PrivateAccessError {
private:
    int a;
    void f();
    PrivateAccessError();
public:
    int b;
    PrivateAccessError(int x);
    void g();
};

PrivateAccessError::PrivateAccessError() { a = 1; b = 1; }
PrivateAccessError::PrivateAccessError(int x) { a = x; b = x; }
void PrivateAccessError::f() { a = 5; b = 5; }    // 멤버 함수는 private 멤버에 접근 가능
void PrivateAccessError::g() { a = 6; b = 6; }

int main() {
    PrivateAccessError objA;         // (1) 오류: PrivateAccessError()는 private
    PrivateAccessError objB(100);    // (2) 정상
    objB.a = 10;                     // (3) 오류: a는 private
    objB.b = 20;                     // (4) 정상
    objB.f();                        // (5) 오류: f()는 private
    objB.g();                        // (6) 정상
}
```

### 2.5 const 멤버 함수

멤버 함수 중에는 `getArea()`처럼 멤버 변수를 읽기만 하는 함수와 `setRadius()`처럼 멤버 변수를 바꾸는 함수가 있다. 읽기만 하는 함수의 원형 끝에 `const`를 붙이면, 컴파일러가 그 함수 안에서 멤버 변수를 바꾸지 않는지 검사하고, `const`로 선언된 객체에 대해서도 그 함수를 호출할 수 있게 한다.

> **용어 정의:** **const 멤버 함수(const member function)** 는 `double getArea() const;`처럼 원형 끝에 `const`를 붙여, 함수 안에서 그 객체의 멤버 변수를 바꾸지 않겠다고 선언한 멤버 함수이다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r) { radius = r; }
    double getArea() const { return 3.14 * radius * radius; }   // const 멤버 함수: 멤버를 바꾸지 않는다
    void setRadius(int r) { radius = r; }                       // 멤버를 바꾸는 함수
};

int main() {
    const Circle unit(1);           // const 객체
    cout << unit.getArea() << endl; // const 멤버 함수는 호출할 수 있다
    // unit.setRadius(2);           // 오류: const 객체에 비const 멤버 함수를 호출할 수 없다
    return 0;
}
```

```text
3.14
```

- `const Circle unit(1);`로 만든 객체는 생성된 뒤 바뀌면 안 되므로, 컴파일러는 이 객체에 대해 `const` 멤버 함수만 호출하게 한다. 주석을 풀어 `unit.setRadius(2);`를 쓰면 Apple clang은 "'this' argument to member function 'setRadius' has type 'const Circle', but function is not marked const"라는 오류를 낸다.
- `const` 멤버 함수 안에서 `radius = 3;`처럼 멤버를 바꾸려 하면 "cannot assign to non-static data member within const member function"이라는 오류가 난다. `const` 멤버 함수 안의 `this`는 `const Circle *` 타입이기 때문이다(4장).
- `const` 객체는 직접 선언하는 경우보다 5장의 const 참조 매개변수(`void print(const Circle &c)`)로 받는 경우가 훨씬 많다. 그 함수 안에서 `c.getArea()`를 호출하려면 `getArea()`가 `const` 멤버 함수여야 하므로, 멤버 변수를 바꾸지 않는 멤버 함수는 처음부터 `const`로 선언하는 것이 좋다.

### 2.6 객체의 메모리 배치와 크기

클래스로 객체를 만들 때 메모리에 객체마다 따로 놓이는 것은 **멤버 변수**뿐이다. 멤버 함수의 코드는 클래스에 하나만 있어 모든 객체가 함께 쓰며, 객체 안에 함수의 복사본이 들어가지 않는다. 어느 객체의 멤버 변수를 다룰지는 멤버 함수를 호출할 때 넘어가는 그 객체의 주소, 곧 4장의 `this`로 정해진다.

```cpp
#include <iostream>
#include <cstddef>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r = 1) : radius(r) { }
    double getArea() const { return 3.14 * radius * radius; }
};

struct Box {
    char tag;        // 1바이트
    int width;       // 4바이트
    double height;   // 8바이트
};

int main() {
    Circle arr[2];
    cout << "sizeof(int)=" << sizeof(int) << ", sizeof(Circle)=" << sizeof(Circle) << endl;
    cout << "&arr[1] - &arr[0] = " << (char *)&arr[1] - (char *)&arr[0] << " bytes" << endl;
    cout << "sizeof(Box)=" << sizeof(Box) << ", offsets: tag=" << offsetof(Box, tag)
         << " width=" << offsetof(Box, width) << " height=" << offsetof(Box, height) << endl;
    return 0;
}
```

```text
sizeof(int)=4, sizeof(Circle)=4
&arr[1] - &arr[0] = 4 bytes
sizeof(Box)=16, offsets: tag=0 width=4 height=8
```

`int` 멤버 하나만 가진 `Circle`의 크기는 `int`와 같은 4바이트이다. 생성자와 `getArea()`가 있어도 크기는 늘지 않는다. 배열의 두 원소 `arr[0]`, `arr[1]`의 주소 차이도 4바이트이므로, 객체들이 멤버 변수만큼의 공간을 차지하며 연속으로 놓인다.

```text
 Circle arr[2]                      코드 영역 (클래스에 하나)
┌──────────────┐                  ┌──────────────────────┐
│ arr[0]       │                  │ Circle::Circle(int)  │
│  radius = 1  │  4바이트          │ Circle::getArea()    │
├──────────────┤                  └──────────────────────┘
│ arr[1]       │                    두 객체가 함께 사용
│  radius = 1  │  4바이트
└──────────────┘
```

*그림 2. Circle 객체 배열과 멤버 함수 코드의 배치*

`Box`는 1 + 4 + 8 = 13바이트의 멤버를 가지지만 크기는 16바이트이다. `offsetof`로 각 멤버의 시작 위치를 보면 `tag`는 0, `width`는 4, `height`는 8이다. CPU가 `int`를 4의 배수 주소에서, `double`을 8의 배수 주소에서 읽도록 정렬하기 위해, 컴파일러가 `tag` 뒤에 3바이트의 빈 공간(padding)을 넣었기 때문이다. 이 정렬 규칙은 C의 구조체와 같으며, 정확한 크기와 정렬은 플랫폼에 따라 다를 수 있다(위의 값은 macOS arm64의 Apple clang 기준이다). 멤버를 큰 타입부터 선언하면 빈 공간이 줄어드는 경우가 많다.

### 2.7 접근 제어의 단위와 검사 시점

접근 지정자는 **클래스 단위**로 적용된다. 어떤 멤버 함수가 `private` 멤버에 접근할 수 있는지는 그 함수가 어느 클래스의 멤버인지로 정해지며, 접근하려는 객체가 `this`인지 다른 객체인지는 따지지 않는다. 따라서 멤버 함수는 매개변수로 받은 같은 클래스의 다른 객체의 `private` 멤버에도 접근할 수 있다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;                                    // private
public:
    Circle(int r) : radius(r) { }
    bool isBigger(const Circle &other) const {
        return radius > other.radius;              // 같은 클래스의 다른 객체의 private 멤버
    }
};

int main() {
    Circle a(5), b(3);
    cout << boolalpha << a.isBigger(b) << endl;
    // cout << a.radius << endl;                   // 오류: main()은 private 멤버에 접근할 수 없다
    return 0;
}
```

```text
true
```

- `a.isBigger(b)` 안에서 `radius`는 `this->radius`, 곧 `a`의 반지름 5이고, `other.radius`는 `b`의 반지름 3이다. `isBigger()`가 `Circle`의 멤버 함수이므로 `b`의 `private` 멤버도 읽을 수 있고, 5 > 3이므로 `true`가 출력된다. `boolalpha`는 `bool` 값을 `1`, `0` 대신 `true`, `false`로 출력하게 한다.
- 주석을 풀어 `main()`에서 `a.radius`를 쓰면 Apple clang은 "'radius' is a private member of 'Circle'"이라는 오류를 낸다. `main()`은 `Circle`의 멤버가 아니기 때문이다.

접근 제어는 **컴파일 시간에만** 검사된다. 컴파일러는 이름을 쓰는 위치가 그 멤버에 접근할 권한이 있는지를 검사하여 권한이 없으면 컴파일을 거부할 뿐이고, 실행 중에 메모리를 보호하는 장치는 아니다. `private` 멤버도 2.6절의 그림처럼 객체 안의 메모리에 그대로 있으며, `sizeof`로 잰 객체의 크기에 포함된다. 접근 지정자의 목적은 악의적인 접근을 막는 보안이 아니라, 클래스를 사용하는 코드가 내부 구현에 의존하지 않도록 하여 잘못된 사용을 컴파일 단계에서 막고, 내부 구현을 바꾸어도 사용하는 코드를 고치지 않아도 되게 하는 것이다(1.1절의 캡슐화).

C의 구조체는 모든 멤버가 공개되어 있어, 구조체를 사용하는 모든 코드가 멤버에 직접 의존한다(1.3절). 구조체의 멤버 이름이나 표현 방식을 바꾸면 그 구조체를 쓰는 코드를 모두 찾아 고쳐야 한다. C++ 클래스에서 멤버 변수를 `private`으로 두고 공개 멤버 함수로만 다루게 하면, 고쳐야 할 범위가 그 클래스의 멤버 함수로 한정된다.

---

<br>

## 3. 객체 생성과 사용

### 3.1 선언

객체를 선언하면 모든 멤버 변수에 대한 메모리가 할당된다. **점 연산자** (`.`)로 public 멤버에 접근한다.

```cpp
Circle c;               // 객체 생성: 메모리 할당
c.setRadius(5);         // 점 연산자로 멤버 함수 호출
double area = c.getArea();
```

> **주의:** 클래스 외부에서 `private` 멤버에 직접 접근할 수 없다:
> ```cpp
> c.radius = 5;  // ❌ 컴파일 오류: radius는 private
> ```
> public setter를 사용하거나 생성자를 통해 값을 전달해야 한다.

다음 프로그램은 `Circle` 객체 `donut`과 `pizza`를 생성하고, 점 연산자로 각 객체의 멤버 변수와 멤버 함수에 접근한다. 객체 사용법만 보이기 위해 `radius`를 `public`으로 두었으며, 실제로는 1.1절처럼 `private`으로 두는 것이 바람직하다.

```cpp
#include <iostream>
using namespace std;

class Circle {                       // Circle 선언부
public:
    int radius;
    double getArea();
};

double Circle::getArea() {           // Circle 구현부
    return 3.14 * radius * radius;
}

int main() {
    Circle donut;                    // 객체 donut 생성
    donut.radius = 1;                // donut의 멤버 변수 접근
    double area = donut.getArea();   // donut의 멤버 함수 호출
    cout << "donut 면적은 " << area << endl;

    Circle pizza;
    pizza.radius = 30;
    area = pizza.getArea();
    cout << "pizza 면적은 " << area << endl;
}
```

```text
donut 면적은 3.14
pizza 면적은 2826
```

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

**생성자(constructor)** 는 객체 생성 시 초기화를 수행하는 특수 멤버 함수이다.

- 클래스와 **같은 이름**을 가진다.
- **리턴 타입이 없다** (`void`도 아님).
- 객체 생성 시 **자동으로 호출**되며, 객체마다 오직 한 번만 실행된다. 프로그래머가 임의로 호출할 수 없다.
- **중복**이 가능하다 (매개변수가 다른 여러 생성자 정의). 객체를 생성할 때는 그중 하나만 실행된다.

생성자의 목적은 객체가 생성될 때 필요한 초기화를 하는 것이다. 멤버 변수의 값을 초기화하는 것 외에도 메모리 할당, 파일 열기, 네트워크 연결 같은 작업을 생성자에서 한다. 같은 탁구공 생산 장치에서 나온 공이 서로 다른 색으로 칠해져 나오듯이, 같은 클래스로 만든 객체도 생성자가 받은 값에 따라 서로 다르게 초기화된다.

#### 생성자가 필요한 이유

생성자가 없으면 지역 객체의 멤버 변수에 **쓰레기 값**(이전에 그 메모리에 있던 값)이 들어 있다. 초기화되지 않은 객체를 사용하면 예측 불가능한 동작이 발생한다.

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

생성자는 모든 객체가 **유효한 상태에서 시작**하는 것을 보장한다. 초기화 함수를 따로 호출해야 한다는 것을 프로그래머가 기억할 필요 없이, 초기화가 자동으로 이루어진다.

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

### 4.2 생성자 중복

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

### 4.3 위임 생성자

여러 생성자에 같은 초기화 코드가 중복되면, 객체 초기화를 전담하는 **타겟 생성자(target constructor)** 하나를 두고 다른 생성자가 이를 호출하게 하여 코드를 간소화할 수 있다. 타겟 생성자를 호출하여 객체 초기화를 맡기는 생성자를 **위임 생성자(delegating constructor)** 라 한다(C++11). 위임 생성자는 생성자 서두의 콜론(`:`) 뒤에 타겟 생성자를 호출한다.

```cpp
#include <iostream>
using namespace std;

class Circle {
public:
    int radius;
    Circle();                        // 위임 생성자
    Circle(int r);                   // 타겟 생성자
    double getArea();
};

Circle::Circle() : Circle(1) { }     // 타겟 생성자 Circle(int r)를 호출하며 r에 1 전달

Circle::Circle(int r) {              // 객체 초기화를 전담
    radius = r;
    cout << "반지름 " << radius << " 원 생성" << endl;
}

double Circle::getArea() {
    return 3.14 * radius * radius;
}

int main() {
    Circle donut;                    // 매개변수 없는 생성자 호출
    double area = donut.getArea();
    cout << "donut 면적은 " << area << endl;

    Circle pizza(30);                // 매개변수 있는 생성자 호출
    area = pizza.getArea();
    cout << "pizza 면적은 " << area << endl;
}
```

```text
반지름 1 원 생성
donut 면적은 3.14
반지름 30 원 생성
pizza 면적은 2826
```

`Circle donut;`은 `Circle()`을 호출하고, `Circle()`은 다시 `Circle(1)`을 호출하므로 출력 문장을 두 생성자에 중복해서 쓰지 않아도 두 객체 모두 생성 메시지를 출력한다.

### 4.4 멤버 변수의 초기화 방법

생성자에서 멤버 변수를 초기화하는 방법은 세 가지이다.

**(1) 생성자 본문에서 대입:**

```cpp
Point::Point() { x = 0; y = 0; }
Point::Point(int a, int b) { x = a; y = b; }
```

**(2) 생성자 서두의 멤버 초기화 리스트(member initializer list):**

```cpp
Point::Point() : x(0), y(0) { }              // 멤버 변수 x, y를 0으로 초기화
Point::Point(int a, int b) : x(a), y(b) { }  // x는 a로, y는 b로 초기화
```

**(3) 클래스 선언부에서 직접 초기화 (C++11):**

```cpp
class Point {
    int x = 0, y = 0;                // 클래스 선언부에서 x, y를 0으로 직접 초기화
public:
    // ...
};
```

(1)은 멤버 변수가 먼저 만들어진 뒤 값을 대입하고, (2)는 멤버 변수가 만들어지는 시점에 바로 초기화한다. `const` 멤버나 참조 멤버처럼 대입할 수 없는 멤버는 (2)나 (3)으로만 초기화할 수 있다. 멤버 초기화 리스트의 멤버는 리스트에 쓴 순서가 아니라 클래스에 선언된 순서대로 초기화된다.

다음은 멤버 초기화 리스트와 위임 생성자를 함께 쓴 예이다.

```cpp
#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    Point();
    Point(int a, int b);
    void show() { cout << "(" << x << ", " << y << ")" << endl; }
};

Point::Point() : Point(0, 0) { }     // 위임 생성자
Point::Point(int a, int b)           // 타겟 생성자
    : x(a), y(b) { }                 // 멤버 초기화 리스트

int main() {
    Point origin;
    Point target(10, 20);
    origin.show();
    target.show();
}
```

```text
(0, 0)
(10, 20)
```

### 4.5 기본 생성자

객체가 생성될 때는 반드시 생성자가 호출된다. 매개변수가 없는 생성자를 **기본 생성자(default constructor)** 또는 디폴트 생성자라 한다.

프로그래머가 **생성자를 하나도 정의하지 않으면**, 컴파일러가 아무 일도 하지 않는 **기본 생성자**를 자동으로 제공한다 (기본 타입의 멤버 변수는 초기화되지 않음).

그러나 **어떤 생성자라도 하나 정의하면**, 컴파일러는 기본 생성자를 **제공하지 않는다**. 기본 생성자가 필요하면 직접 정의해야 한다.

```cpp
class Box {
    int size;
public:
    Box(int s) { size = s; }   // 매개변수가 있는 생성자만 정의
};

Box a;       // ❌ 컴파일 오류: 기본 생성자 없음!
Box b(10);   // ✅ 정상
```

> **핵심:** `Box(int s)`를 정의했지만 `Box()`도 필요하다면, 반드시 둘 다 작성해야 한다.

### 4.6 소멸자

**소멸자(destructor)** 는 객체가 소멸될 때 (스코프를 벗어나거나 `delete`될 때, 전역 객체는 프로그램이 종료될 때) 자동으로 호출된다. 객체의 메모리가 소멸되기 직전에 오직 한 번 호출되며, 프로그래머가 임의로 호출하지 않는다.

- 이름: `~ClassName()`
- 매개변수 없음, 리턴 타입 없음 (어떤 값도 리턴할 수 없음).
- **중복 불가** (클래스당 소멸자는 하나뿐).
- **정리 작업** 용도 (동적 메모리 해제, 파일 저장과 닫기, 네트워크 닫기 등).
- 소멸자를 선언하지 않으면 컴파일러가 아무 일도 하지 않고 리턴하는 기본 소멸자를 자동으로 만든다.

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

### 4.7 실행 순서

- 객체는 **선언 순서대로 생성**된다.
- 객체는 **역순으로 소멸**된다 (마지막에 생성된 것이 먼저 소멸).
- 지역 객체는 포함하는 **스코프가 종료**될 때 소멸된다.

```cpp
void func() {
    Circle a(1);     // 1번째 생성
    Circle b(2);     // 2번째 생성
}                    // b 먼저 소멸, 그 다음 a 소멸
```

```mermaid
graph LR
    subgraph CON["생성 순서"]
        ca["a"] --> cb["b"]
    end
    subgraph DES["소멸 순서"]
        db["b"] --> da["a"]
    end
```

*그림 3. 지역 객체 a, b의 생성 순서와 소멸 순서*

객체는 선언된 위치에 따라 지역 객체와 전역 객체로 나뉜다.

- **지역 객체:** 함수 안에 선언된 객체이다. 함수가 실행되어 선언문에 도달할 때 순서대로 생성되고, 함수가 종료하면 생성된 순서의 역순으로 소멸된다.
- **전역 객체:** 함수 바깥에 선언된 객체이다. `main()`이 시작되기 전에 프로그램에 선언된 순서대로 생성되고, 프로그램이 종료할 때 생성된 순서의 역순으로 소멸된다. 순서가 보장되는 것은 한 소스 파일 안에서이며, 여러 소스 파일에 걸친 전역 객체의 생성 순서는 정해져 있지 않다.
- **동적 객체:** `new`로 생성한 객체는 `new`가 실행되는 순간 생성되고 `delete`가 실행될 때 소멸된다(4장).

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() : Circle(1) { }
    Circle(int r) {
        radius = r;
        cout << "반지름 " << radius << " 원 생성" << endl;
    }
    ~Circle() {
        cout << "반지름 " << radius << " 원 소멸" << endl;
    }
};

Circle globalDonut(1000);            // 전역 객체
Circle globalPizza(2000);

void f() {
    Circle fDonut(100);              // f()의 지역 객체
    Circle fPizza(200);
}

int main() {
    Circle mainDonut;                // main()의 지역 객체
    Circle mainPizza(30);
    f();
}
```

```text
반지름 1000 원 생성
반지름 2000 원 생성
반지름 1 원 생성
반지름 30 원 생성
반지름 100 원 생성
반지름 200 원 생성
반지름 200 원 소멸
반지름 100 원 소멸
반지름 30 원 소멸
반지름 1 원 소멸
반지름 2000 원 소멸
반지름 1000 원 소멸
```

```text
프로그램 로딩    globalDonut 생성 → globalPizza 생성
main() 시작      mainDonut 생성 → mainPizza 생성
f() 실행         fDonut 생성 → fPizza 생성
f() 종료         fPizza 소멸 → fDonut 소멸
main() 종료      mainPizza 소멸 → mainDonut 소멸
프로그램 종료    globalPizza 소멸 → globalDonut 소멸
```

*그림 4. 전역 객체와 지역 객체의 생성과 소멸 과정*

### 4.8 멤버 초기화 순서가 만드는 오류

4.4절에서 본 것처럼 멤버 초기화 리스트의 멤버는 리스트에 쓴 순서가 아니라 클래스에 선언된 순서대로 초기화된다. 한 멤버의 초깃값을 다른 멤버로 계산하면 이 규칙 때문에 오류가 생길 수 있다.

```cpp
#include <iostream>
using namespace std;

class Square {
    int area;                     // 먼저 선언됨: 먼저 초기화된다
    int side;
public:
    Square(int s) : side(s), area(side * side) { }   // 잘못: area를 초기화할 때 side는 아직 초기화 전
    void show() { cout << "side=" << side << " area=" << area << endl; }
};

int main() {
    Square sq(4);
    sq.show();
    return 0;
}
```

리스트에는 `side(s)`가 먼저 적혀 있지만 `area`가 먼저 선언되었으므로 `area(side * side)`가 먼저 실행된다. 이 순간 `side`에는 아직 값이 들어가지 않았으므로 `area`에는 정해지지 않은 값이 들어가며, 출력되는 `area`는 실행할 때마다 다를 수 있다. Apple clang은 `-Wall`로 컴파일하면 다음 경고를 낸다.

```text
order.cpp:8:21: warning: field 'side' will be initialized after field 'area' [-Wreorder-ctor]
order.cpp:8:35: warning: field 'side' is uninitialized when used here [-Wuninitialized]
```

선언 순서를 `int side; int area;`로 바꾸면 `side`가 먼저 4로 초기화되고 `area`가 16이 되어 `side=4 area=16`이 출력된다. 다른 멤버에 의존하지 않도록 `area(s * s)`처럼 매개변수로 계산하는 방법도 있다. 초기화 리스트는 선언 순서와 같은 순서로 쓰고, 컴파일러의 경고를 켜 두면 이런 오류를 미리 찾을 수 있다.

### 4.9 객체의 종류에 따른 생성과 소멸 시점

4.7절의 지역 객체와 전역 객체에 더해, 함수 안의 static 지역 객체와 `new`로 만드는 동적 객체까지 한 프로그램에 모으면 생성과 소멸의 시점을 한눈에 비교할 수 있다. `Tracer`는 생성자와 소멸자에서 자기 이름을 출력한다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Tracer {
    string name;
public:
    Tracer(string n) : name(n) { cout << "  생성 " << name << endl; }
    ~Tracer() { cout << "  소멸 " << name << endl; }
};

Tracer g("전역");                       // 전역 객체: main() 전에 생성

void f() {
    static Tracer s("static 지역");      // 처음 호출될 때 한 번만 생성
    Tracer local("f의 지역");             // 호출될 때마다 생성, 리턴할 때 소멸
}

int main() {
    cout << "main 시작" << endl;
    f();
    f();
    Tracer *p = new Tracer("동적");      // new가 실행될 때 생성
    {
        Tracer block("블록 지역");
    }                                    // 블록이 끝날 때 소멸
    delete p;                            // delete가 실행될 때 소멸
    cout << "main 끝" << endl;
    return 0;
}
```

```text
  생성 전역
main 시작
  생성 static 지역
  생성 f의 지역
  소멸 f의 지역
  생성 f의 지역
  소멸 f의 지역
  생성 동적
  생성 블록 지역
  소멸 블록 지역
  소멸 동적
main 끝
  소멸 static 지역
  소멸 전역
```

| 객체 | 생성 시점 | 소멸 시점 |
|:----|:----|:----|
| 전역 객체 `g` | `main()`이 시작하기 전 | `main()`이 끝난 뒤, 프로그램이 끝날 때 |
| static 지역 객체 `s` | 그 선언문이 처음 실행될 때(첫 번째 `f()` 호출) 한 번만 | `main()`이 끝난 뒤, 전역 객체보다 먼저(생성의 역순) |
| 지역 객체 `local` | 선언문이 실행될 때마다(`f()`를 호출할 때마다) | 함수가 리턴할 때마다 |
| 블록 안의 지역 객체 `block` | 블록 안의 선언문이 실행될 때 | 블록의 닫는 중괄호 `}` |
| 동적 객체 `*p` | `new`가 실행될 때 | `delete`가 실행될 때(4장) |

*표 5. 객체의 종류에 따른 생성과 소멸 시점*

- `f()`를 두 번 호출했지만 `static 지역`은 한 번만 생성된다. 두 번째 호출에서는 선언문을 건너뛰고 이미 있는 객체를 쓴다. 이 객체는 `f()`가 리턴해도 사라지지 않고, `main()`이 끝난 뒤에 소멸한다.
- 프로그램이 끝날 때 static 지역 객체와 전역 객체는 생성의 역순으로 소멸한다. `전역`이 먼저 생성되고 `static 지역`이 나중에 생성되었으므로 `static 지역`이 먼저 소멸한다.
- 동적 객체는 스코프와 관계없이 `delete`를 실행하는 순간 소멸한다. `delete p;`를 빠뜨리면 `소멸 동적`은 끝까지 출력되지 않는다. 전역 객체나 지역 객체와 달리 컴파일러가 소멸 시점을 정해 주지 않기 때문이다.

### 4.10 생성자의 종류와 컴파일러가 만드는 멤버 함수

지금까지 본 생성자와 5장에서 다룰 복사 생성자를 모으면 다음과 같다. 클래스가 직접 선언하지 않으면 컴파일러가 만들어 주는 것도 있고, 만들어 주지 않는 것도 있다.

| 종류 | 형태 | 컴파일러가 자동으로 만드는 조건 |
|:----|:----|:----|
| 기본 생성자 | `Circle()` | 생성자를 하나도 선언하지 않았을 때만(4.5절) |
| 매개변수가 있는 생성자 | `Circle(int r)` | 만들지 않음 |
| 위임 생성자 | `Circle() : Circle(1) { }` | 만들지 않음 (다른 생성자에 초기화를 맡기는 작성 방법, 4.3절) |
| 복사 생성자 | `Circle(const Circle &c)` | 선언하지 않으면 멤버 단위로 복사하는 버전을 만든다(5장) |
| 소멸자 | `~Circle()` | 선언하지 않으면 본문이 비어 있는 디폴트 소멸자를 만든다(4.6절) |

*표 6. 생성자와 소멸자의 종류와 자동 생성 조건*

어느 생성자가 호출되는지는 객체를 만드는 문장의 인자로 정해진다. `Circle a;`는 기본 생성자를, `Circle b(5);`는 `Circle(int)`를, `Circle c = b;`는 복사 생성자를 호출한다. 마지막 문장에 `=`가 있지만 대입이 아니라 초기화인 이유는 5장에서 다룬다.

> **참고:** **`= default`.** 매개변수가 있는 생성자를 선언한 뒤에도 컴파일러가 만드는 기본 생성자가 필요하면, C++11부터 `Circle() = default;`로 그 버전을 명시적으로 요청할 수 있다. 이 생성자는 기본 타입 멤버를 초기화하지 않으므로, 4.4절의 (3)처럼 `int radius = 1;`과 같이 선언부에서 초깃값을 주는 방법과 함께 쓴다. `= default`와 `= delete`의 규칙은 14장 4.4절에서 다룬다.

### 4.11 생성자와 멤버 함수로 지키는 불변 조건

캡슐화(1.1절)의 실제 효과는 객체가 언제나 올바른 상태에 있도록 보장할 수 있다는 데 있다. 객체가 살아 있는 동안 항상 참이어야 하는 조건을 **불변 조건(invariant)** 이라고 한다. 반지름이 1 이상이어야 한다는 `Circle`의 조건은 멤버 변수를 `private`으로 두고, 값을 바꾸는 모든 통로(생성자와 `setRadius()`)에서 같은 검사를 하면 지킬 수 있다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;                                  // 불변 조건: radius는 언제나 1 이상
public:
    Circle(int r) { setRadius(r); }              // 생성자도 같은 검사를 거친다
    void setRadius(int r) {
        if (r < 1) {
            cout << "  잘못된 반지름 " << r << ", 1로 바꿈" << endl;
            r = 1;
        }
        radius = r;
    }
    int getRadius() const { return radius; }
};

int main() {
    Circle a(5), b(-3);
    a.setRadius(0);
    cout << a.getRadius() << ' ' << b.getRadius() << endl;
    return 0;
}
```

```text
  잘못된 반지름 -3, 1로 바꿈
  잘못된 반지름 0, 1로 바꿈
1 1
```

생성자가 직접 대입하지 않고 `setRadius()`를 호출하므로, 객체를 만들 때와 나중에 값을 바꿀 때 같은 검사가 적용된다. `b(-3)`과 `a.setRadius(0)`은 모두 1로 고쳐진다. `radius`가 `private`이므로 클래스 밖의 코드가 `a.radius = -5;`처럼 검사를 건너뛸 길이 없다. C의 구조체였다면 어느 코드든 멤버에 직접 잘못된 값을 넣을 수 있으므로, 같은 조건을 지키려면 구조체를 쓰는 모든 곳에서 검사해야 한다(1.3절). 잘못된 값을 고쳐 넣는 대신 객체의 생성 자체를 실패로 알리려면 13장의 예외를 쓴다.

---

<br>

## 5. 인라인 함수

함수를 호출하면 돌아올 주소와 CPU 레지스터 값을 저장하고, 매개변수를 스택에 넣고, 함수가 끝나면 리턴값을 임시 장소에 저장하고 저장했던 레지스터 값과 돌아갈 주소를 복구하는 부가 작업이 필요하다. 크기가 작은 함수는 실행 시간에 비해 이 호출 오버헤드가 상대적으로 크다. 다음 프로그램은 한 줄짜리 함수 `odd()`를 10000번 호출한다.

```cpp
#include <iostream>
using namespace std;

int odd(int x) {
    return (x % 2);
}

int main() {
    int sum = 0;
    for (int i = 1; i <= 10000; i++) {   // 1에서 10000까지의 홀수의 합 계산
        if (odd(i))                      // odd()가 10000번 호출됨
            sum += i;
    }
    cout << sum;
}
```

```text
25000000
```

**인라인 함수(inline function)** 는 함수 호출 위치에 함수 본문을 삽입하여 함수 호출 오버헤드를 피하도록 컴파일러에게 요청하는 것이다.

```cpp
inline int square(int n) {
    return n * n;
}
```

`odd()`를 `inline int odd(int x)`로 선언하면 컴파일러는 `if (odd(i))`를 `if ((i % 2))`로 확장한 소스를 컴파일하므로 호출 오버헤드가 사라진다. 매크로와 비슷하지만 전처리기가 아니라 컴파일러가 처리하며, 인자의 타입 검사도 이루어진다.

- **작고 자주 호출되는** 함수에 적합하다. C++ 클래스에는 짧은 멤버 함수가 많으므로 인라인 함수가 실행 속도 향상에 도움이 된다.
- 크거나 복잡한 함수에는 컴파일러가 `inline` 힌트를 무시할 수 있다. 재귀 함수, 긴 함수, `static` 변수나 반복문, `goto` 문을 가진 함수는 컴파일러가 인라인 요청을 받아들이지 않을 수 있다.
- **클래스 본문 내부에서 정의된** 멤버 함수는 생성자를 포함하여 암시적으로 인라인이다. 이를 자동 인라인 함수라 하며, `inline`을 따로 쓸 필요가 없다.

인라인 함수의 장점은 프로그램의 실행 시간이 빨라지는 것이고, 단점은 호출 위치마다 코드가 삽입되어 컴파일된 전체 코드의 크기가 커지는 것이다. 교재는 코드 크기가 통계적으로 최대 30%까지 늘어난다고 설명하므로, 짧은 함수만 인라인으로 선언하는 것이 좋다.

```cpp
class Circle {
    int radius;
public:
    // 암시적 인라인: 클래스 내부에서 정의
    double getArea() { return 3.14 * radius * radius; }
};
```

> **참고:** `inline`은 명령이 아니라 힌트이다. 현대 컴파일러는 키워드와 관계없이 유리한 경우 자동으로 인라인을 수행한다. 현대 C++에서 `inline`의 중요한 의미는 같은 함수 정의가 여러 소스 파일에 포함되어도 오류가 되지 않게 하는 것이며, 덕분에 인라인 함수는 헤더 파일에 정의할 수 있다.

---

<br>

## 6. C++ 구조체

C++에서 `struct`는 상속, 멤버, 접근 지정 등 모든 면에서 `class`와 같다. **유일한 차이**는 기본 접근 지정이다. 멤버의 기본 접근 지정과 상속의 기본 접근 지정이 `struct`는 `public`, `class`는 `private`이다.

| 특성 | `struct` | `class` |
|:----|:--------:|:-------:|
| 기본 접근 수준 | `public` | `private` |
| 멤버 함수 가능 | 가능 | 가능 |
| 생성자/소멸자 가능 | 가능 | 가능 |
| 상속 가능 | 가능 | 가능 |

*표 7. C++ struct와 class의 비교*

```cpp
struct Point {
    int x, y;        // 기본적으로 public
    void print() {
        cout << x << ", " << y << endl;
    }
};

Point p;
p.x = 3;            // ✅ 정상: 기본이 public
```

> **참고:** **관례.** 대부분 public 멤버를 가진 단순 데이터 구조에는 `struct`를, 캡슐화와 접근 제어가 중요한 경우에는 `class`를 사용한다.

C++가 구조체를 수용한 이유는 C 언어와의 호환성 때문이다. C의 구조체를 100% 수용하여 C 소스를 그대로 가져다 쓸 수 있게 했다. C++에서는 구조체 객체를 생성할 때 `struct` 키워드를 생략한다. C에서처럼 `struct StructName stObj;`로 써도 되지만, C++에서는 `StructName stObj;`로 쓰는 것이 보통이다.

다음은 `Circle` 클래스를 C++ 구조체로 다시 작성한 예이다.

```cpp
#include <iostream>
using namespace std;

struct StructCircle {                // C++ 구조체 선언
private:
    int radius;
public:
    StructCircle(int r) { radius = r; }   // 구조체의 생성자
    double getArea();
};

double StructCircle::getArea() {
    return 3.14 * radius * radius;
}

int main() {
    StructCircle waffle(3);          // struct 키워드 없이 객체 생성
    cout << "면적은 " << waffle.getArea();
}
```

```text
면적은 28.26
```

---

<br>

## 7. 헤더 파일과 소스 파일 분리

규모가 큰 프로젝트에서는 클래스 **선언**을 헤더 파일(`.h`)에, 멤버 함수 **정의**를 소스 파일(`.cpp`)에 분리한다. 클래스마다 선언부는 헤더 파일에, 구현부는 그 헤더 파일을 include하는 cpp 파일에 저장하고, `main()` 같은 전역 함수는 다른 cpp 파일에 두어 필요한 헤더 파일을 include한다. 이렇게 나누는 목적은 클래스의 재사용이다.

**`Circle.h`** (클래스 선언):

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

**`Circle.cpp`** (멤버 함수 정의):

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

각 cpp 파일은 따로 컴파일되어 목적 파일이 되고, 링커가 목적 파일들을 합쳐 하나의 실행 파일을 만든다.

```mermaid
flowchart LR
    H["Circle.h"] -. include .-> C1["Circle.cpp"]
    H -. include .-> C2["main.cpp"]
    C1 -->|컴파일| O1["Circle.obj"]
    C2 -->|컴파일| O2["main.obj"]
    O1 -->|링킹| E["main.exe"]
    O2 -->|링킹| E
```

*그림 5. 분리된 헤더 파일과 cpp 파일이 실행 파일로 만들어지는 과정*

**인클루드 가드(include guard)** 는 같은 헤더가 여러 번 포함되는 것을 방지한다. 가드가 없는 `Circle.h`를 한 소스 파일에서 두 번 이상 include하면 `Circle` 클래스가 두 번 선언되어 컴파일 오류가 발생한다(Visual C++의 `error C2011: 'Circle': 'class' 형식 재정의`, GCC의 `redefinition of 'class Circle'`). `#ifndef CIRCLE_H`로 시작하는 조건 컴파일 문을 쓰면 첫 번째 include에서만 `CIRCLE_H`가 정의되면서 클래스 선언이 포함되고, 그 뒤의 include에서는 내용이 건너뛰어진다. 조건 컴파일 상수는 다른 상수와 충돌하지 않도록 클래스 이름으로 짓는 것이 좋다.

| 방법 | 문법 |
|:----|:----|
| 전통적 방법 | `#ifndef CIRCLE_H` / `#define CIRCLE_H` / `#endif` |
| 현대적 방법 | `#pragma once` (널리 지원, 간결) |

*표 8. 인클루드 가드를 작성하는 방법*

> **참고:** **`#pragma once`의 지원.** `#pragma once`는 C++ 표준은 아니지만 주요 컴파일러(GCC, Clang, MSVC) 모두 지원한다.

다음은 한 파일에 있던 `Adder` 클래스와 `Calculator` 클래스를 헤더 파일과 cpp 파일로 나눈 예이다. 두 개의 정수를 입력받아 더한 결과를 출력한다.

```cpp
// ===== Adder.h =====
#ifndef ADDER_H
#define ADDER_H

class Adder {                        // 덧셈 모듈 클래스
    int op1, op2;
public:
    Adder(int a, int b);
    int process();
};

#endif

// ===== Calculator.h =====
#ifndef CALCULATOR_H
#define CALCULATOR_H

class Calculator {                   // 계산기 클래스
public:
    void run();
};

#endif
```

```cpp
// ===== Adder.cpp =====
#include "Adder.h"

Adder::Adder(int a, int b) {
    op1 = a; op2 = b;
}

int Adder::process() {
    return op1 + op2;
}

// ===== Calculator.cpp =====
#include <iostream>
using namespace std;

#include "Calculator.h"
#include "Adder.h"

void Calculator::run() {
    cout << "두 개의 수를 입력하세요>>";
    int a, b;
    cin >> a >> b;                   // 정수 두 개 입력
    Adder adder(a, b);               // 덧셈기 생성
    cout << adder.process();         // 덧셈 계산
}

// ===== main.cpp =====
#include "Calculator.h"

int main() {
    Calculator calc;                 // calc 객체 생성
    calc.run();                      // 계산기 시작
}
```

```text
두 개의 수를 입력하세요>>5 -20
-15
```

인클루드 가드가 없으면 실제로 어떤 일이 생기는지 확인할 수 있다. 가드가 없는 `Circle.h`를 한 소스 파일에서 두 번 include하면 Apple clang은 `./Circle.h:1:7: error: redefinition of 'Circle'`이라는 오류를 낸다. 전처리기가 헤더의 내용을 두 번 복사하므로 컴파일러에게는 같은 클래스를 두 번 선언한 코드로 보이기 때문이다.

인클루드 가드로도 막을 수 없는 오류가 있다. 멤버 함수를 헤더 파일 안에서 클래스 밖에 정의하고, 그 헤더를 두 소스 파일이 포함하면 다음과 같다.

```cpp
// Circle2.h
#ifndef CIRCLE_H
#define CIRCLE_H
class Circle {
    int radius;
public:
    Circle(int r);
    int getRadius();
};
int Circle::getRadius() { return radius; }   // 헤더 안, 클래스 밖의 정의
#endif
```

```text
$ c++ -std=c++17 Circle2.cpp main2.cpp -o m2
duplicate symbol 'Circle::getRadius()' in:
    .../main2-724ee7.o
    .../Circle2-e1a384.o
ld: 1 duplicate symbols
```

인클루드 가드는 한 번역 단위 안에서의 중복만 막는다. `Circle2.cpp`와 `main2.cpp`는 각자 헤더를 한 번씩 포함하므로, 두 목적 파일에 `Circle::getRadius()`의 정의가 하나씩 들어가 링크할 때 중복 기호가 된다(1장 4.4절). 이것이 멤버 함수의 정의를 cpp 파일에 두는 이유이다. 헤더에 정의를 두어야 한다면 클래스 선언 안에 정의하거나(5절의 자동 인라인 함수), 정의 앞에 `inline`을 붙인다. 인라인 함수는 여러 번역 단위에 같은 정의가 있어도 되도록 허용되어 있다.

---

<br>

## 8. 완전한 실행 예제

아래 프로그램들은 독립적으로 실행 가능하다. 아무 파일에 복사하여 `g++ -o program filename.cpp`로 컴파일하고 `./program`으로 실행할 수 있다.

### 8.1 Circle 클래스: 생성자, getter, 넓이 계산

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

```text
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

```text
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

각 스코프 안에서 소멸은 생성의 **역순**으로 일어난다는 점에 주목할 필요가 있다. 이 예제의 `Tracer`는 `string`을 쓰므로 엄밀하게는 `#include <string>`이 필요하다. GCC와 Clang의 표준 라이브러리에서는 `<iostream>`이 `<string>`을 간접적으로 포함하여 그대로 컴파일되지만, 표준이 이를 보장하지는 않는다.

### 8.3 멤버 함수: 클래스 내부 vs 외부 정의

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

    // 여기서는 선언만: 정의는 아래에서
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

```text
3 x 4 = 12
10 x 5 = 50
```

> **참고:** **정의 위치의 선택.** getter/setter 같은 짧은 한 줄 함수는 클래스 내부에서 정의하는 것이 일반적이다. 긴 함수는 클래스 선언의 가독성을 위해 외부에서 정의한다.

---

<br>

## 9. 흔한 실수 모음

이 절은 클래스를 처음 사용할 때 가장 자주 발생하는 오류를 정리한다. 미리 알아두면 디버깅 시간을 절약할 수 있다.

### 9.1 클래스 정의 끝에 세미콜론 빠뜨리기

```cpp
class Circle {
    int radius;
public:
    double getArea() { return 3.14 * radius * radius; }
}           // 잘못됨: 세미콜론 누락

int main() { ... }
```

컴파일러는 닫는 중괄호 뒤에 `;`가 없다는 오류(Clang의 "expected ';' after class" 등)를 내며, 클래스 정의 **뒤쪽 줄**을 가리키는 오류가 함께 이어지기도 한다. 반드시 `};`으로 끝내야 한다.

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
c.radius = 5;   // 잘못됨: 컴파일 오류, radius는 private
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

Box b;       // 잘못됨: 컴파일 오류, 기본 생성자 없음
```

**해결:** 기본 생성자가 필요하면 직접 정의한다.

```cpp
class Box {
    int size;
public:
    Box() { size = 0; }           // 이제 작동함
    Box(int s) { size = s; }
};

Box b;       // OK: Box() 호출
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

### 9.6 클래스 밖에서 멤버 함수를 정의할 때 클래스 이름을 빠뜨림

```cpp
class Circle { int radius; public: double getArea(); };
double getArea() { return 3.14 * radius * radius; }   // 잘못: Circle::이 없음
```

```text
noscope.cpp:2:34: error: use of undeclared identifier 'radius'
```

`double getArea()`라고만 쓰면 컴파일러는 이것을 `Circle`의 멤버 함수가 아니라 이름이 같은 전역 함수의 정의로 본다. 전역 함수에는 `radius`라는 멤버가 없으므로 "선언되지 않은 식별자"라는 오류가 난다. 멤버 함수를 클래스 밖에서 정의할 때는 `double Circle::getArea() { ... }`처럼 범위 지정 연산자 `::`로 소속 클래스를 밝힌다. 이 오류를 고치지 않고 `radius`를 쓰지 않는 함수였다면 컴파일은 되지만, 클래스의 `getArea()`는 정의되지 않은 채 남아 호출하는 순간 링크 오류가 난다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:----|:---------|
| 객체 | 상태(멤버 변수)와 행위(멤버 함수)를 하나로 묶은 것이다. |
| 캡슐화 | 내부는 `private`으로 숨기고 외부와의 인터페이스만 `public`으로 공개하여 데이터를 보호한다. |
| 클래스 vs 객체 | 클래스는 메모리를 차지하지 않는 설계도이고, 객체는 메모리에 할당되는 실체(인스턴스)이다. |
| 클래스 문법 | `class Name { ... };`처럼 쓰며 세미콜론이 필수이고, 기본 접근 지정은 `private`이다. |
| 선언부와 구현부 | 선언부에 멤버를 선언하고, 구현부에 `리턴타입 클래스::함수()` 형식으로 멤버 함수를 정의한다. |
| 멤버 변수 | 객체의 상태를 저장하며 일반적으로 `private`으로 선언한다. |
| 멤버 함수 | 객체의 행위를 정의하며 클래스 내부 또는 외부(`::`)에서 정의한다. |
| 접근 지정자 | `private`은 클래스 내부에서만, `protected`는 클래스와 파생 클래스에서, `public`은 어디서든 접근할 수 있다. |
| 객체 생성 | `ClassName obj;`로 메모리를 할당하고 점 연산자로 멤버에 접근한다. |
| 객체 배열 | `ClassName arr[n];`의 각 원소는 독립된 객체이며 각각 기본 생성자가 호출된다. |
| 생성자 | 클래스와 같은 이름에 리턴 타입이 없고, 객체 생성 시 자동으로 한 번 호출되며 중복할 수 있다. |
| 위임 생성자 | `Circle() : Circle(1) { }`처럼 타겟 생성자를 호출하여 초기화 코드의 중복을 없앤다. |
| 멤버 초기화 | 생성자 본문의 대입, 멤버 초기화 리스트, 클래스 선언부의 직접 초기화로 멤버 변수를 초기화한다. |
| 기본 생성자 | 매개변수가 없는 생성자이며, 생성자를 하나도 정의하지 않은 경우에만 컴파일러가 제공한다. |
| 소멸자 | `~ClassName()` 형식으로 매개변수가 없고 중복할 수 없으며, 객체가 소멸될 때 호출된다. |
| 실행 순서 | 지역 객체는 함수 실행 중, 전역 객체는 `main()` 전에 선언 순서대로 생성되고 각각 역순으로 소멸된다. |
| 인라인 함수 | `inline`으로 선언하거나 클래스 내부에 정의한 함수로, 컴파일러가 호출을 본문으로 대체할 수 있으며 코드 크기가 커진다. |
| C++ `struct` | `class`와 같지만 기본 접근 지정이 `public`이며, C와의 호환성을 위해 존재한다. |
| 파일 분리 | `.h`에 선언, `.cpp`에 정의를 두고, 인클루드 가드로 중복 include를 막는다. |
| const 멤버 함수 | 원형 끝에 `const`를 붙인 멤버 함수는 멤버 변수를 바꿀 수 없고, `const` 객체와 const 참조로 받은 객체에 대해서도 호출할 수 있다. |
| 멤버 초기화 순서 | 멤버는 초기화 리스트의 순서가 아니라 클래스에 선언된 순서대로 초기화되므로, 다른 멤버로 초깃값을 계산할 때는 선언 순서에 유의한다. |
| 객체의 메모리 배치 | 객체에는 멤버 변수만 들어가고 멤버 함수의 코드는 클래스에 하나만 있으며, 멤버 사이에는 정렬을 위한 빈 공간이 들어갈 수 있다. |
| 객체의 생성과 소멸 시점 | 전역 객체는 `main()` 전에, static 지역 객체는 처음 실행될 때, 지역 객체는 선언문마다 생성되고, 동적 객체는 `new`와 `delete`가 실행될 때 생성되고 소멸한다. |
| 생성자의 종류 | 기본 생성자는 생성자를 하나도 선언하지 않을 때만 자동으로 만들어지고, 복사 생성자와 소멸자는 선언하지 않으면 컴파일러가 만든다. |
| 헤더의 멤버 함수 정의 | 인클루드 가드는 한 번역 단위 안의 중복만 막으므로, 헤더에서 클래스 밖에 정의한 멤버 함수는 여러 소스 파일에서 중복 기호 오류를 내며 `inline`이나 cpp 파일로 해결한다. |
| 불변 조건 | 객체가 살아 있는 동안 항상 참이어야 하는 조건으로, 멤버 변수를 `private`으로 두고 생성자와 값을 바꾸는 멤버 함수에서 같은 검사를 하여 지킨다. |

---

<br>

## 점검 문제

1. **기본 생성자:** 다음 클래스에서 `Box a;`가 컴파일 오류가 되는 이유를 설명하고, `Box a;`를 허용하도록 고치는 방법을 두 가지 쓰라.

   ```cpp
   class Box {
       int size;
   public:
       Box(int s) { size = s; }
   };
   ```

   <details>
   <summary>정답</summary>

   생성자 `Box(int s)`를 정의했으므로 컴파일러가 기본 생성자를 자동으로 만들지 않고, `Box a;`가 호출할 매개변수 없는 생성자가 없기 때문이다. `Box() { size = 0; }`처럼 기본 생성자를 직접 정의하거나, `Box() : Box(0) { }`처럼 타겟 생성자 `Box(int s)`에 초기화를 맡기는 위임 생성자를 정의하면 된다.

   </details>

2. **생성과 소멸 순서:** 전역 객체 `g`, `main()`의 지역 객체 `a`와 `b`(선언 순서대로), `main()`이 호출하는 함수 `f()`의 지역 객체 `c`가 있다. `main()`이 `a`, `b`를 선언한 뒤 `f()`를 호출할 때, 네 객체의 생성 순서와 소멸 순서를 쓰라.

   <details>
   <summary>정답</summary>

   생성 순서는 `g`(`main()` 시작 전), `a`, `b`, `c`(`f()` 실행 중)이다. 소멸 순서는 `c`(`f()` 종료 시), `b`, `a`(`main()` 종료 시), `g`(프로그램 종료 시)이다.

   </details>

3. **접근 지정:** 2.4절의 `PrivateAccessError` 클래스에서 `PrivateAccessError objA;`는 오류인데 `PrivateAccessError objB(100);`은 정상인 이유를 설명하라. 또 멤버 함수 `g()` 안의 `a = 6;`이 정상인 이유를 쓰라.

   <details>
   <summary>정답</summary>

   `objA`는 매개변수 없는 생성자 `PrivateAccessError()`를 호출해야 하는데 이 생성자가 `private`이므로 `main()`에서 호출할 수 없고, `objB(100)`이 호출하는 `PrivateAccessError(int x)`는 `public`이므로 호출할 수 있다. `g()`는 같은 클래스의 멤버 함수이므로 `private` 멤버 `a`에 접근할 수 있다.

   </details>

4. **멤버 초기화 리스트:** `Point::Point(int a, int b) : x(a), y(b) { }`와 `Point::Point(int a, int b) { x = a; y = b; }`의 차이를 설명하고, 반드시 멤버 초기화 리스트(또는 클래스 선언부의 직접 초기화)를 써야 하는 경우를 하나 쓰라.

   <details>
   <summary>정답</summary>

   앞의 것은 멤버 변수가 만들어지는 시점에 `a`, `b`로 바로 초기화하고, 뒤의 것은 멤버 변수가 먼저 만들어진 뒤 생성자 본문에서 값을 대입한다. `const` 멤버 변수나 참조 멤버 변수는 만들어진 뒤 대입할 수 없으므로 멤버 초기화 리스트나 클래스 선언부의 직접 초기화로 초기화해야 한다.

   </details>

5. **인클루드 가드:** 인클루드 가드가 없는 `Circle.h`를 `main.cpp`에서 두 번 include하면 어떤 일이 생기는가? `#ifndef CIRCLE_H`, `#define CIRCLE_H`, `#endif`가 이를 어떻게 막는지 설명하라.

   <details>
   <summary>정답</summary>

   `Circle` 클래스의 선언이 두 번 포함되어 클래스 재정의 컴파일 오류가 발생한다. 첫 번째 include에서는 `CIRCLE_H`가 정의되어 있지 않으므로 `#define CIRCLE_H`와 클래스 선언이 포함되고, 두 번째 include에서는 `CIRCLE_H`가 이미 정의되어 있으므로 `#ifndef`부터 `#endif`까지의 내용이 건너뛰어진다.

   </details>

6. **const 멤버 함수:** 2.5절의 `Circle`에 `void show() { cout << radius << endl; }`를 추가하고 `void print(const Circle &c) { c.show(); }`를 작성하면 어떻게 되는가? 고치는 방법도 쓰라.

   <details>
   <summary>정답</summary>

   컴파일 오류가 난다. `c`는 const 참조이므로 `c`를 통해서는 `const` 멤버 함수만 호출할 수 있는데, `show()`는 `const`로 선언되지 않았다. `show()`는 멤버 변수를 읽기만 하므로 `void show() const { cout << radius << endl; }`로 선언하면 컴파일된다.

   </details>

7. **멤버 초기화 순서:** `class P { int b; int a; public: P() : a(1), b(a + 1) { } };`에서 `P p;`를 만든 뒤 `a`와 `b`의 값은 정해지는가?

   <details>
   <summary>정답</summary>

   `a`는 1로 정해지지만 `b`는 정해지지 않는다. 멤버는 선언 순서인 `b`, `a`의 순서로 초기화되므로, `b(a + 1)`이 실행될 때 `a`는 아직 초기화되지 않았다. 따라서 `b`에는 정해지지 않은 값에 1을 더한 값이 들어간다. 선언 순서를 `int a; int b;`로 바꾸면 `a`가 1, `b`가 2가 된다.

   </details>

8. **객체의 크기:** `class P { int x; int y; public: void set(int a, int b); int sum(); };`에서 `int`가 4바이트일 때 `sizeof(P)`는 얼마인가? 멤버 함수를 10개 더 추가하면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   8바이트이다. 객체에는 멤버 변수 `x`, `y`만 들어가고, 두 `int`는 모두 4의 배수 위치에 놓이므로 빈 공간도 없다. 멤버 함수는 객체 안에 들어가지 않으므로 10개를 더 추가해도 `sizeof(P)`는 8바이트 그대로이다(가상함수는 9장에서 다루는 예외이다).

   </details>

9. **생성과 소멸 시점:** 4.9절의 `main()`에서 `f();`를 세 번 호출하도록 바꾸면 "생성 static 지역"과 "생성 f의 지역"은 각각 몇 번 출력되는가? "소멸 static 지역"은 언제 출력되는가?

   <details>
   <summary>정답</summary>

   "생성 static 지역"은 1번, "생성 f의 지역"은 3번 출력된다. static 지역 객체는 선언문이 처음 실행될 때 한 번만 생성되고, 지역 객체는 호출할 때마다 생성되어 리턴할 때 소멸한다. "소멸 static 지역"은 `main 끝`이 출력된 뒤 프로그램이 끝날 때, "소멸 전역"보다 먼저 출력된다.

   </details>

10. **클래스 단위의 접근:** 2.7절의 `Circle`에 전역 함수 `bool same(const Circle &x, const Circle &y) { return x.radius == y.radius; }`를 추가하면 컴파일되는가? 멤버 함수 `bool same(const Circle &y) const { return radius == y.radius; }`는 어떤가?

    <details>
    <summary>정답</summary>

    전역 함수는 컴파일되지 않는다. `same()`이 `Circle`의 멤버가 아니므로 `x.radius`와 `y.radius`가 모두 private 멤버 접근 오류가 된다. 멤버 함수 버전은 컴파일된다. 접근 권한은 클래스 단위로 주어지므로 `Circle`의 멤버 함수는 매개변수로 받은 다른 `Circle` 객체 `y`의 `private` 멤버에도 접근할 수 있다. 전역 함수에 접근 권한을 주려면 7장의 프렌드 선언이 필요하다.

    </details>

11. **인클루드 가드와 중복 정의:** 7절의 `Circle2.h`에 인클루드 가드가 있는데도 중복 기호 오류가 나는 이유를 설명하고, 헤더 파일을 고쳐 해결하는 방법 두 가지를 쓰라.

    <details>
    <summary>정답</summary>

    인클루드 가드는 한 번역 단위 안에서 같은 헤더가 두 번 포함되는 것만 막는다. `Circle2.cpp`와 `main2.cpp`는 서로 다른 번역 단위이므로 각자 헤더를 포함하여 두 목적 파일에 `Circle::getRadius()`의 정의가 하나씩 들어가고, 링커가 이를 중복 기호로 보고한다. 헤더 안에서 고치려면 정의를 클래스 선언 안으로 옮겨 자동 인라인 함수로 만들거나, 정의 앞에 `inline`을 붙여 `inline int Circle::getRadius() { return radius; }`로 쓴다.

    </details>

12. **생성자의 자동 생성:** `class T { int v; public: T(int x) : v(x) { } };`에 대해 `T a(1);`, `T b;`, `T c = a;` 가운데 컴파일되지 않는 문장과 그 이유를 쓰라.

    <details>
    <summary>정답</summary>

    `T b;`만 컴파일되지 않는다. 매개변수가 있는 생성자 `T(int)`를 선언했으므로 컴파일러는 기본 생성자를 만들지 않고, 인자 없이 객체를 만들 생성자가 없다. `T a(1);`은 `T(int)`를 호출하고, `T c = a;`는 선언하지 않아도 컴파일러가 만드는 복사 생성자를 호출하므로 컴파일된다. `T b;`를 허용하려면 `T() : v(0) { }`처럼 기본 생성자를 추가한다.

    </details>
