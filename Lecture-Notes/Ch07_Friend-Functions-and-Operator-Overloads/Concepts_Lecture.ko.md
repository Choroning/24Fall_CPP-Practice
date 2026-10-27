# 7장 — 프렌드 함수와 연산자 중복

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

> **학습 목표:**
> 1. 프렌드 함수와 프렌드 클래스의 개념을 이해하고 선언할 수 있다
> 2. 연산자 중복의 개념과 제약을 설명할 수 있다
> 3. 연산자 함수를 멤버 함수와 프렌드 함수로 각각 작성할 수 있다
> 4. 이항 연산자와 단항 연산자를 중복하고 전위와 후위 연산자를 구분하여 작성할 수 있다
> 5. 스트림 연산자와 첨자 연산자를 중복할 수 있다

---

## 목차

- [1. 프렌드](#1-프렌드)
  - [1.1 프렌드 함수](#11-프렌드-함수)
  - [1.2 프렌드 클래스](#12-프렌드-클래스)
  - [1.3 프렌드 선언의 세 가지 형태](#13-프렌드-선언의-세-가지-형태)
  - [1.4 사용 지침](#14-사용-지침)
- [2. 연산자 중복의 기초](#2-연산자-중복의-기초)
  - [2.1 왜 연산자 중복이 필요한가?](#21-왜-연산자-중복이-필요한가)
  - [2.2 개념](#22-개념)
  - [2.3 중복 가능 / 불가능 연산자](#23-중복-가능--불가능-연산자)
  - [2.4 멤버 함수 vs 프렌드 함수: 판단 가이드](#24-멤버-함수-vs-프렌드-함수-판단-가이드)
- [3. 이항 연산자 중복](#3-이항-연산자-중복)
  - [3.1 멤버 함수로 구현](#31-멤버-함수로-구현)
  - [3.2 프렌드 함수로 구현](#32-프렌드-함수로-구현)
  - [3.3 복합 대입 연산자](#33-복합-대입-연산자)
  - [3.4 비교 연산자](#34-비교-연산자)
- [4. 단항 연산자 중복](#4-단항-연산자-중복)
  - [4.1 멤버 함수로 구현](#41-멤버-함수로-구현)
  - [4.2 프렌드 함수로 구현](#42-프렌드-함수로-구현)
  - [4.3 증가 / 감소 연산자](#43-증가--감소-연산자)
  - [4.4 전위 vs 후위: 상세 추적](#44-전위-vs-후위-상세-추적)
- [5. 스트림 연산자 중복](#5-스트림-연산자-중복)
  - [5.1 `operator<<`](#51-operator)
  - [5.2 `operator>>`](#52-operator)
- [6. 첨자 연산자](#6-첨자-연산자)
- [7. 완전한 실행 가능 예제](#7-완전한-실행-가능-예제)
  - [7.1 완전한 Vector2D 클래스](#71-완전한-vector2d-클래스)
  - [7.2 전위 vs 후위 출력 추적](#72-전위-vs-후위-출력-추적)
  - [7.3 operator<<가 프렌드여야 하는 이유](#73-operator가-프렌드여야-하는-이유)
- [8. 흔한 실수](#8-흔한-실수)
  - [8.1 후위에서 `(int)` 더미 매개변수를 빠뜨림](#81-후위에서-int-더미-매개변수를-빠뜨림)
  - [8.2 참조로 리턴해야 할 때 값으로 리턴](#82-참조로-리턴해야-할-때-값으로-리턴)
  - [8.3 복합 대입 연산자에서 `*this` 리턴을 잊음](#83-복합-대입-연산자에서-this-리턴을-잊음)
  - [8.4 중복할 수 없는 연산자를 중복하려고 시도](#84-중복할-수-없는-연산자를-중복하려고-시도)
  - [8.5 operator<<를 프렌드가 아닌 멤버 함수로 만듦](#85-operator를-프렌드가-아닌-멤버-함수로-만듦)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 프렌드

### 1.1 프렌드 함수

**프렌드 함수**는 클래스의 `private` 및 `protected` 멤버에 접근할 수 있도록 허용된 **비멤버 함수**이다. 클래스 내부에서 `friend` 키워드로 선언하지만, 멤버 함수는 **아니다**.

클래스의 멤버로 선언하기에는 어울리지 않지만 클래스의 모든 멤버에 자유롭게 접근해야 하는 외부 함수가 있을 때 그 함수를 프렌드로 선언한다.

```cpp
class Box {
    int width;
public:
    Box(int w) : width(w) {}
    friend void printWidth(const Box &b);   // 프렌드 선언
};

// 정의: 멤버 함수가 아님 (Box:: 범위 지정 없음)
void printWidth(const Box &b) {
    cout << b.width << endl;   // private 멤버에 직접 접근
}
```

| 특성 | 설명 |
|:----|:----|
| 클래스 내부에 `friend`로 선언 | private/protected 멤버 접근 권한 부여 |
| 클래스 외부에서 정의할 때 `ClassName::` 범위 지정을 붙이지 않음 | 멤버 함수가 아니기 때문이며, 클래스 안에서 바로 정의할 수도 있음 |
| `this` 포인터 없음 | 객체를 매개변수로 전달받아야 함 |
| `public` / `private` / `protected` 섹션에 영향받지 않음 | `friend` 선언은 클래스 어디에든 위치 가능 |

*표 1. 프렌드 함수의 특성*

> **핵심:** 프렌드 함수는 private 멤버에 접근할 수 있지만, 클래스의 멤버가 아니다. 객체를 통해 점(`.`) 연산자로 호출할 수 없다.

### 1.2 프렌드 클래스

**클래스 전체**가 다른 클래스의 private 멤버에 접근해야 할 때, **프렌드 클래스**로 선언할 수 있다.

```cpp
class Engine {
    int horsepower;
public:
    Engine(int hp) : horsepower(hp) {}
    friend class Car;   // Car가 Engine의 private 멤버에 접근 가능
};

class Car {
public:
    void showHP(const Engine &e) {
        cout << e.horsepower << endl;   // OK: Car는 Engine의 프렌드
    }
};
```

| 규칙 | 설명 |
|:----|:----|
| 클래스 `A` 안에 `friend class B;` | `B`의 모든 멤버 함수가 `A`의 private/protected 멤버에 접근 가능 |
| 프렌드 관계는 **대칭이 아님** | `A`가 `B`를 프렌드로 선언해도 `B`가 `A`를 프렌드로 선언한 것은 아님 |
| 프렌드 관계는 **전이적이지 않음** | `A`→`B`, `B`→`C`라고 해서 `A`→`C`가 되지 않음 |

*표 2. 프렌드 클래스의 규칙*

### 1.3 프렌드 선언의 세 가지 형태

프렌드로 선언할 수 있는 대상은 전역 함수, 다른 클래스의 특정 멤버 함수, 다른 클래스 전체의 세 가지이다.

| 형태 | `Rect` 클래스 안의 선언 | 접근 권한을 얻는 함수 |
|:----|:----|:----|
| 전역 함수 | `friend bool equals(Rect r, Rect s);` | 외부 함수 `equals()` |
| 다른 클래스의 멤버 함수 | `friend bool RectManager::equals(Rect r, Rect s);` | `RectManager`의 `equals()`만 |
| 다른 클래스 전체 | `friend RectManager;` 또는 `friend class RectManager;` | `RectManager`의 모든 멤버 함수 |

*표 3. 프렌드 선언의 세 가지 형태*

다음은 다른 클래스의 멤버 함수를 프렌드로 선언하는 예이다. `RectManager`의 선언이 `Rect`보다 앞에 오므로, `RectManager::equals()`의 매개변수에 쓰인 `Rect`를 컴파일러가 알 수 있도록 `class Rect;`로 **전방 선언(forward declaration)** 을 둔다. 전방 선언이 없으면 아직 선언되지 않은 이름을 참조하는 컴파일 오류가 발생한다.

```cpp
#include <iostream>
using namespace std;

class Rect;   // 전방 선언: RectManager가 Rect를 먼저 참조하기 때문

class RectManager {
public:
    bool equals(Rect r, Rect s);
};

class Rect {
    int width, height;
public:
    Rect(int width, int height) { this->width = width; this->height = height; }
    friend bool RectManager::equals(Rect r, Rect s);   // 다른 클래스의 멤버 함수를 프렌드로 선언
};

bool RectManager::equals(Rect r, Rect s) {
    return r.width == s.width && r.height == s.height;   // Rect의 private 멤버에 접근
}

int main() {
    Rect a(3, 4), b(3, 4);
    RectManager man;
    if (man.equals(a, b)) cout << "equal" << endl;
    else cout << "not equal" << endl;
}
```

두 사각형의 너비와 높이가 같으므로 실행 결과는 다음과 같다.

```text
equal
```

### 1.4 사용 지침

- `friend`는 **불가피한 경우에만** 사용한다. 캡슐화를 깨뜨려 외부 코드에 private 멤버를 노출하기 때문이다.
- 좌측 피연산자가 자신의 클래스 타입이 아닌 연산자를 중복하면서 private 멤버에 접근해야 할 때 **필수적**이다(예: `ostream& operator<<`).
- 가능하면 멤버 함수를 우선하고, 멤버 함수로 불충분할 때만 `friend`를 사용한다.

---

<br>

## 2. 연산자 중복의 기초

### 2.1 왜 연산자 중복이 필요한가?

연산자 중복이 없으면, 사용자 정의 타입의 연산에 명시적 메서드 호출이 필요하다. 두 접근 방식을 비교하면 다음과 같다.

```cpp
// 연산자 중복 없이: 장황하고 읽기 어려움
Vector2D a(1, 2), b(3, 4), c(5, 6);
Vector2D result = a.add(b).subtract(c);          // 무엇을 계산하는지 알기 어려움
if (result.equals(Vector2D(-1, 0)))
    cout << result.toString() << endl;

// 연산자 중복 사용: 자연스럽고 수학처럼 읽힘
Vector2D a(1, 2), b(3, 4), c(5, 6);
Vector2D result = a + b - c;                     // 명확: (1+3-5, 2+4-6)
if (result == Vector2D(-1, 0))
    cout << result << endl;
```

연산자 중복은 C++을 사용자 정의 타입에 대해 **표현력 있게** 만드는 핵심 기능이다. 클래스의 객체가 기본 타입처럼 동작하게 하여 코드를 더 읽기 쉽고 직관적으로 만든다.

| 중복 없이 | 중복 사용 | 장점 |
|:---------|:---------|:----|
| `a.add(b)` | `a + b` | 수학처럼 읽힘 |
| `a.equals(b)` | `a == b` | 익숙한 구문 |
| `a.multiply(b)` | `a * b` | 시각적 잡음 감소 |
| `cout << a.toString()` | `cout << a` | 스트림과 자연스럽게 통합 |
| `a.addAssign(b)` | `a += b` | 연쇄 호출이 자연스럽게 동작 |

*표 4. 연산자 중복 전후의 표현 비교*

### 2.2 개념

**연산자 중복(Operator Overloading)** 은 사용자 정의 타입(클래스)에 대해 C++ 연산자의 의미를 재정의하는 것이다. `operator` 키워드 뒤에 연산자 기호를 붙여 사용한다.

```cpp
Vector2D operator+(const Vector2D &rhs);   // Vector2D에 대해 + 연산자 중복
```

연산자 함수(operator function)의 일반 형식은 `리턴타입 operator연산자(매개변수리스트);`이며, 연산자 함수를 구현하는 방법은 두 가지이다.

1. 클래스의 멤버 함수로 구현한다.
2. 외부 함수로 구현하고, private 멤버에 접근해야 하면 클래스에 프렌드로 선언한다.

컴파일러는 연산자를 쓴 식을 연산자 함수 호출로 바꾸어 처리한다. 멤버 함수이면 좌측 피연산자 객체의 멤버 함수를 호출하고, 외부 함수이면 피연산자를 모두 인자로 넘긴다.

| 식 | 멤버 함수일 때의 변환 | 외부(프렌드) 함수일 때의 변환 |
|:----|:----|:----|
| `a + b` | `a.operator+(b)` | `operator+(a, b)` |
| `a == b` | `a.operator==(b)` | `operator==(a, b)` |
| `a += b` | `a.operator+=(b)` | `operator+=(a, b)` |
| `2 + a` | 변환 불가 (`int`에는 멤버 함수를 추가할 수 없음) | `operator+(2, a)` |
| `!a`, `++a` | `a.operator!()`, `a.operator++()` | `operator!(a)`, `operator++(a)` |
| `a++` | `a.operator++(0)` | `operator++(a, 0)` |

*표 5. 연산자 식의 연산자 함수 호출 변환*

**제한사항:**

| 제한 | 설명 |
|:----|:----|
| 새로운 연산자를 만들 수 없음 | `operator@`나 `operator$` 등은 불가 |
| **우선순위** 변경 불가 | `*`가 `+`보다 우선하는 것은 그대로 유지 |
| **결합 방향** 변경 불가 | 좌→우 또는 우→좌 결합 방향 그대로 유지 |
| **항(arity)** 변경 불가 | 이항 연산자는 이항으로, 단항 연산자는 단항으로 유지 |
| 피연산자 중 하나는 사용자 정의 타입이어야 함 | `int + int`는 중복할 수 없음 |

*표 6. 연산자 중복의 제한사항*

### 2.3 중복 가능 / 불가능 연산자

| 분류 | 연산자 |
|:----|:------|
| 중복 **가능** | `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `<<` `>>` `[]` `()` `~` `!` `&` `\|` `^` `++` `--` `+=` `-=` `*=` `/=` `%=` `=` `->` `new` `delete` ... |
| 중복 **불가** | `::` (범위 지정), `.` (멤버 접근), `.*` (멤버 포인터), `?:` (삼항), `sizeof`, `typeid` |

*표 7. 중복할 수 있는 연산자와 중복할 수 없는 연산자*

### 2.4 멤버 함수 vs 프렌드 함수: 판단 가이드

| 관점 | 멤버 함수 | 프렌드 함수 |
|:----|:---------|:----------|
| 좌측 피연산자 | 암묵적으로 `*this` | 명시적으로 첫 번째 매개변수 |
| 우측 피연산자 | 매개변수 | 두 번째 매개변수 |
| private 접근 | 가능 (멤버이므로) | 가능 (friend로 선언) |
| 사용 시점 | 좌측 피연산자가 **자신의 클래스** 타입일 때 (예: `obj + 3`) | 좌측 피연산자가 **자신의 클래스가 아닐** 때 (예: `3 + obj`, `cout << obj`) |

*표 8. 멤버 함수와 프렌드 함수로 구현한 연산자 함수의 비교*

```cpp
// 멤버: 좌측 피연산자가 *this
Vector2D Vector2D::operator+(const Vector2D &rhs);

// 프렌드: 양쪽 피연산자 모두 매개변수
friend Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
```

> **핵심:** 좌측 피연산자가 기본 타입이거나 수정할 수 없는 클래스(예: `ostream`)인 경우, **반드시** 프렌드(또는 자유) 함수를 사용해야 한다.

**판단 순서도:**

```text
좌측 피연산자가 자신의 클래스 타입인가?
 |
 +-- 예 --> 연산자가 좌측 피연산자를 수정하는가? (예: +=, ++, =)
 |            |
 |            +-- 예 --> 멤버 함수 (*this를 참조로 리턴)
 |            |
 |            +-- 아니오 --> 멤버 함수 (새 객체를 값으로 리턴)
 |
 +-- 아니오 --> (예: cout << obj,  3 + obj,  cin >> obj)
                |
                private 멤버에 접근해야 하는가?
                |
                +-- 예 --> 프렌드 함수
                |
                +-- 아니오 --> 비멤버 비프렌드 함수
```

*그림 1. 연산자 함수의 구현 방식을 고르는 순서도*

**빠른 참조 표:**

| 표현식 | 좌측 피연산자 | 권장 방식 | 이유 |
|:------|:-----------|:---------|:----|
| `obj + obj2` | 자신의 클래스 | 멤버 함수 | 좌측 피연산자가 `*this` |
| `obj += obj2` | 자신의 클래스 | 멤버 함수 | `*this`를 수정하고 리턴 |
| `3 + obj` | `int` (자신의 클래스 아님) | 프렌드 함수 | `int`에 멤버 함수를 추가할 수 없음 |
| `cout << obj` | `ostream` (자신의 클래스 아님) | 프렌드 함수 | `ostream`에 멤버 함수를 추가할 수 없음 |
| `cin >> obj` | `istream` (자신의 클래스 아님) | 프렌드 함수 | `istream`에 멤버 함수를 추가할 수 없음 |
| `++obj` / `obj++` | 자신의 클래스 | 멤버 함수 | 피연산자가 `*this` |
| `obj[i]` | 자신의 클래스 | 멤버 함수 (필수) | `operator[]`는 반드시 멤버여야 함 |
| `obj = obj2` | 자신의 클래스 | 멤버 함수 (필수) | `operator=`는 반드시 멤버여야 함 |

*표 9. 표현식별로 권장하는 연산자 함수의 구현 방식*

---

<br>

## 3. 이항 연산자 중복

### 3.1 멤버 함수로 구현

좌측 피연산자가 `*this`이고, 우측 피연산자가 매개변수이다. 산술 연산자는 결과를 **값으로** 리턴한다.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D &rhs) const {
        return Vector2D(x + rhs.x, y + rhs.y);
    }
};

// 사용
Vector2D a(1, 2), b(3, 4);
Vector2D c = a + b;   // a.operator+(b) 호출  →  (4, 6)
```

### 3.2 프렌드 함수로 구현

양쪽 피연산자 모두 명시적 매개변수이다. 좌측 피연산자가 클래스 타입이 아닐 때 유용하다.

```cpp
class Color {
    int r, g, b;
public:
    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    // 좌측 피연산자가 int이므로 프렌드 필요
    friend Color operator+(int lhs, const Color &rhs) {
        return Color(lhs + rhs.r, lhs + rhs.g, lhs + rhs.b);
    }
};

// 사용
Color sky(100, 150, 200);
Color c = 50 + sky;   // operator+(50, sky) 호출  →  (150, 200, 250)
```

### 3.3 복합 대입 연산자

복합 대입 연산자(`+=`, `-=`, `*=`)는 좌측 피연산자를 수정하고 `*this`를 **참조로** 리턴한다.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    Vector2D& operator*=(int scalar) {
        x *= scalar;
        y *= scalar;
        return *this;   // 참조로 반환
    }
};

// 사용
Vector2D v(3, 4);
v *= 2;   // v가 (6, 8)이 됨
```

| 리턴 타입 | 이유 |
|:---------|:----|
| `ClassName&` (참조) | 객체가 제자리에서 수정되고, `*this`를 참조로 리턴하므로 `c = a += b`나 `(a += b) += c` 같은 연쇄가 의도대로 동작함 |

*표 10. 복합 대입 연산자의 리턴 타입*

### 3.4 비교 연산자

비교 연산자는 `bool`을 리턴한다. 보통 `const` 멤버 함수로 작성한다.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Vector2D &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

// 사용
Vector2D v1(3, 4), v2(3, 4);
if (v1 == v2)   // 참: 두 성분이 모두 일치
    cout << "같음" << endl;
```

---

<br>

## 4. 단항 연산자 중복

### 4.1 멤버 함수로 구현

단항 연산자를 멤버 함수로 구현할 때 매개변수는 **없으며**, `*this`가 피연산자이다.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    // ~ 는 수직 벡터를 반환 (원본을 수정하지 않음)
    Vector2D operator~() const {
        return Vector2D(-y, x);
    }
};

// 사용
Vector2D v(3, 4);
Vector2D perp = ~v;   // perp = (-4, 3),  v는 변하지 않음
```

논리 부정 연산자 `!`도 같은 방식으로 중복한다. 다음 `Power` 클래스의 `!a`는 `kick`과 `punch`가 모두 0이면 `true`, 아니면 `false`를 리턴한다.

```cpp
class Power {
    int kick, punch;
public:
    Power(int kick = 0, int punch = 0) { this->kick = kick; this->punch = punch; }

    bool operator!() const {   // 매개변수 없음: *this가 피연산자
        return kick == 0 && punch == 0;
    }
};

// 사용
Power a(0, 0), b(5, 5);
if (!a) cout << "a의 파워가 0이다." << endl;   // 출력됨
if (!b) cout << "b의 파워가 0이다." << endl;   // 출력되지 않음
```

### 4.2 프렌드 함수로 구현

프렌드로 구현할 때는 피연산자가 명시적 매개변수이다.

```cpp
class Color {
    int r, g, b;
public:
    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    // ~ 는 반전 색상을 반환
    friend Color operator~(const Color &obj) {
        return Color(255 - obj.r, 255 - obj.g, 255 - obj.b);
    }
};

// 사용
Color red(255, 0, 0);
Color c = ~red;   // c = (0, 255, 255) (시안),  red는 변하지 않음
```

### 4.3 증가 / 감소 연산자

C++은 더미 `int` 매개변수로 **전위**와 **후위** 형태를 구분한다.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    // 전위 --obj: 수정한 후, 수정된 객체를 참조로 반환
    Vector2D& operator--() {
        x--;
        y--;
        return *this;
    }

    // 후위 obj++: 원본을 저장하고, 수정한 후, 저장한 복사본을 값으로 반환
    Vector2D operator++(int) {   // int는 더미 매개변수
        Vector2D old = *this;
        x++;
        y++;
        return old;
    }
};
```

| 형태 | 시그니처 (멤버) | 리턴 | 동작 |
|:----|:-------------|:----|:----|
| 전위 `++obj` / `--obj` | `ClassName& operator++()` | 수정된 객체의 참조 | 먼저 수정, 그 후 리턴 |
| 후위 `obj++` / `obj--` | `ClassName operator++(int)` | 원본(수정 전)의 복사본 | 복사본 저장, 수정, 복사본 리턴 |

*표 11. 전위와 후위 증가, 감소 연산자의 형태*

> **핵심:** 후위 버전의 `int` 매개변수는 **더미**이며 실제로 사용되지 않는다. 컴파일러는 후위 연산 `a++`를 `a.operator++(0)`처럼 의미 없는 정수를 넘겨 호출하며, 이 매개변수의 유일한 목적은 컴파일러가 전위와 후위를 구분할 수 있게 하는 것이다.

**프렌드 함수로 구현:**

```cpp
class Color {
    int r, g, b;
public:
    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    // 전위 ++obj
    friend Color& operator++(Color &obj) {
        obj.r++;
        obj.g++;
        obj.b++;
        return obj;
    }

    // 후위 obj--
    friend Color operator--(Color &obj, int) {
        Color old = obj;
        obj.r--;
        obj.g--;
        obj.b--;
        return old;
    }
};
```

프렌드로 구현할 때는 피연산자를 참조 매개변수(`Color &obj`)로 받아야 원본 객체가 수정된다. 값으로 받으면 함수 안의 복사본만 바뀐다.

### 4.4 전위 vs 후위: 상세 추적

전위와 후위의 차이를 정확히 이해하는 것이 중요하다. 먼저 기본 타입 `int`로 동작을 추적하면 다음과 같다.

```text
 표현식               단계별 설명                        결과
-------------       ----------------------------    -----------
int x = 5;          x가 5로 초기화                      x=5
int a = ++x;        x가 6이 되고, 그 후 a에 값 대입       x=6, a=6
int b = x++;        b에 값(6) 대입, 그 후 x가 7이 됨      x=7, b=6
int c = --x;        x가 6이 되고, 그 후 c에 값 대입       x=6, c=6
int d = x--;        d에 값(6) 대입, 그 후 x가 5가 됨      x=5, d=6
```

*그림 2. `int` 변수에 대한 전위와 후위 연산의 단계별 추적*

**핵심 규칙:**
- **전위** (`++x`): **먼저** 증가시키고, **새로운** 값을 사용한다.
- **후위** (`x++`): **이전** 값을 사용하고, 그 후 증가시킨다.

다음은 같은 동작을 클래스로 구현한 것이다. 구현의 각 단계가 위의 동작을 그대로 반영한다.

```cpp
class Counter {
    int value;
public:
    Counter(int v = 0) : value(v) {}
    int getValue() const { return value; }

    // 전위 ++obj: 먼저 수정, 수정된 객체를 참조로 반환
    Counter& operator++() {
        value++;            // 단계 1: 수정
        return *this;       // 단계 2: 수정된 객체 반환
    }

    // 후위 obj++: 이전 상태 저장, 수정, 이전 상태를 값으로 반환
    Counter operator++(int) {
        Counter old = *this;  // 단계 1: 이전 상태 저장
        value++;              // 단계 2: 수정
        return old;           // 단계 3: 이전 상태(복사본) 반환
    }
};
```

클래스의 추적은 다음과 같다.

```text
 코드                      value (이전)    리턴값          value (이후)
---------------------    ------------   ------   -----------------------
Counter x(5);                 -          -                  5
Counter a = ++x;              5          6        6 (먼저 수정, 새 값 리턴)
Counter b = x++;              6          6       7 (이전 값 리턴, 그 후 수정)
```

*그림 3. `Counter` 객체에 대한 전위와 후위 연산의 추적*

> **참고:** **전위는 참조로, 후위는 값으로 리턴하는 이유.**
> - 전위는 **실제 수정된 객체**를 리턴한다. 객체가 여전히 존재하므로 참조가 안전하고 효율적이다.
> - 후위는 **이전 상태**를 리턴해야 하지만, 이전 상태는 객체 안에 더 이상 존재하지 않으므로 임시 복사본(값으로 리턴)이어야 한다.

---

<br>

## 5. 스트림 연산자 중복

### 5.1 `operator<<`

삽입 연산자 `<<`는 좌측 피연산자가 `ostream`이므로 반드시 **프렌드**(또는 자유 함수)여야 한다.

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    friend ostream& operator<<(ostream &out, const Vector2D &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;   // 연쇄를 위해 out 반환
    }
};

// 사용
Vector2D v(3, 4);
cout << "값: " << v << endl;   // "값: (3, 4)" 출력
```

| 사항 | 설명 |
|:----|:----|
| 좌측 피연산자가 `ostream&` | 자신의 클래스의 멤버가 될 수 없음 |
| `ostream&` 리턴 | 연쇄 가능: `cout << a << b << c` |
| 두 번째 매개변수는 `const ClassName&` | 출력 시 객체를 수정하면 안 됨 |

*표 12. 스트림 삽입 연산자 함수의 형식*

> **참고:** **좌측 피연산자가 자신의 클래스인 `<<`.** `<<`가 반드시 비멤버여야 하는 것은 좌측 피연산자가 `ostream`인 스트림 출력의 경우이다. 좌측 피연산자가 자신의 클래스이면 `<<`도 멤버 함수로 중복할 수 있다. 예를 들어 `Power& Power::operator<<(int n)`이 `kick`과 `punch`에 `n`을 더하고 `*this`를 참조로 리턴하면, `Power a(1, 2); a << 3 << 5 << 6;`은 `a`에 3, 5, 6을 차례로 더해 kick=15, punch=16을 만든다. 참조를 리턴하므로 연쇄된 `<<`가 모두 같은 객체 `a`에 적용된다.

### 5.2 `operator>>`

추출 연산자 `>>`는 같은 패턴을 따르되, `istream`을 사용하고 **비const** 참조를 받는다(객체를 수정하므로).

```cpp
class Vector2D {
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    friend istream& operator>>(istream &in, Vector2D &v) {
        in >> v.x >> v.y;   // "3 4" 형태의 입력을 기대
        return in;
    }
};

// 사용
Vector2D v;
cin >> v;   // 사용자가 "3 4" 입력
```

---

<br>

## 6. 첨자 연산자

`operator[]`는 객체를 배열처럼 접근할 수 있게 한다. 읽기와 쓰기 모두 가능하도록 **참조를** 리턴한다.

```cpp
class Date {
    int year, month, day;
public:
    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}

    int& operator[](int index) {
        switch (index) {
            case 0: return year;
            case 1: return month;
            case 2: return day;
            default: return year;   // 기본값
        }
    }
};

// 사용
Date d(2026, 3, 16);
d[0] = 2025;   // year를 2025로 설정
d[1] = 12;     // month를 12로 설정
d[2] = 25;     // day를 25로 설정
cout << d[0] << "-" << d[1] << "-" << d[2] << endl;   // 2025-12-25
```

| 사항 | 설명 |
|:----|:----|
| 리턴 타입이 `int&` (참조) | 읽기(`x = d[0]`)와 쓰기(`d[0] = 2025`) 모두 가능 |
| 반드시 **멤버 함수**여야 함 | `operator[]`는 프렌드/자유 함수가 될 수 없음 |
| 매개변수는 보통 `int` | 인덱스를 나타냄 |

*표 13. 첨자 연산자 함수의 형식*

---

<br>

## 7. 완전한 실행 가능 예제

### 7.1 완전한 Vector2D 클래스

`operator+`, `operator==`, `operator<<`가 함께 자연스럽게 동작하는 완전한 프로그램은 다음과 같다.

```cpp
#include <iostream>
using namespace std;

class Vector2D {
    int x, y;

public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    // 이항 +  (멤버: 좌측 피연산자가 *this)
    Vector2D operator+(const Vector2D &rhs) const {
        return Vector2D(x + rhs.x, y + rhs.y);
    }

    // 비교 ==  (멤버: const, bool 반환)
    bool operator==(const Vector2D &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    // 스트림 <<  (프렌드: 좌측 피연산자가 ostream이지 Vector2D가 아님)
    friend ostream& operator<<(ostream &out, const Vector2D &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;
    }
};

int main() {
    Vector2D a(1, 2), b(3, 4);
    Vector2D c = a + b;                // (1+3, 2+4) = (4, 6)

    cout << a << " + " << b << " = " << c << endl;   // (1, 2) + (3, 4) = (4, 6)

    if (c == Vector2D(4, 6))
        cout << "같음!" << endl;       // 같음!

    Vector2D d = a + b + Vector2D(5, 4);
    cout << "(1,2) + (3,4) + (5,4) = " << d << endl;  // (1,2) + (3,4) + (5,4) = (9, 10)

    return 0;
}
```

**예상 출력:**

```text
(1, 2) + (3, 4) = (4, 6)
같음!
(1,2) + (3,4) + (5,4) = (9, 10)
```

### 7.2 전위 vs 후위 출력 추적

각 단계의 출력으로 `++x`와 `x++`의 차이를 보여 주는 프로그램은 다음과 같다.

```cpp
#include <iostream>
using namespace std;

class Counter {
    int value;
public:
    Counter(int v = 0) : value(v) {}

    // 전위 ++obj
    Counter& operator++() {
        value++;
        return *this;
    }

    // 후위 obj++
    Counter operator++(int) {
        Counter old = *this;
        value++;
        return old;
    }

    friend ostream& operator<<(ostream &out, const Counter &c) {
        out << c.value;
        return out;
    }
};

int main() {
    Counter x(5);
    cout << "초기값:  x = " << x << endl;

    Counter a = ++x;   // x가 6이 되고, a에 6 대입
    cout << "a = ++x 후:  x = " << x << ", a = " << a << endl;

    Counter b = x++;   // b에 6(이전 x) 대입, 그 후 x가 7이 됨
    cout << "b = x++ 후:  x = " << x << ", b = " << b << endl;

    // 연쇄: 전위는 참조를 반환하므로 가능
    cout << "++x = " << ++x << endl;   // x가 8이 되고, 8 출력

    return 0;
}
```

**예상 출력:**

```text
초기값:  x = 5
a = ++x 후:  x = 6, a = 6
b = x++ 후:  x = 7, b = 6
++x = 8
```

### 7.3 operator<<가 프렌드여야 하는 이유

`operator<<`가 **왜** 멤버 함수가 될 수 없는지, 만약 시도하면 어떻게 되는지 보여 주는 예제는 다음과 같다.

```cpp
#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // 잘못된 방법: operator<<를 멤버 함수로 구현
    // 멤버라면 구문이 이렇게 된다:
    //     point << cout;    // 좌측 피연산자가 *this(Point), 우측이 ostream
    // 이건 거꾸로다! 아무도 point << cout 이라고 쓰지 않는다.
    //
    // ostream& operator<<(ostream &out) {    // 이렇게 하지 말 것
    //     out << "(" << x << ", " << y << ")";
    //     return out;
    // }

    // 올바른 방법: operator<<를 프렌드 함수로 구현
    // 좌측 피연산자가 ostream(Point가 아님)이므로 Point의 멤버가 될 수 없다.
    friend ostream& operator<<(ostream &out, const Point &p) {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }
};

int main() {
    Point p(3, 7);

    // 자연스러운 구문: operator<<가 프렌드이므로 동작
    cout << "Point: " << p << endl;       // Point: (3, 7)

    // 멤버 함수였다면 이렇게 써야 한다:
    // p << cout;   // 보기 흉하고, 부자연스럽고, 연쇄도 깨진다!

    // ostream&를 반환하므로 연쇄가 동작
    Point a(1, 2), b(3, 4);
    cout << a << " and " << b << endl;    // (1, 2) and (3, 4)

    return 0;
}
```

**예상 출력:**

```text
Point: (3, 7)
(1, 2) and (3, 4)
```

**왜 프렌드여야 하는가 (멤버가 아니라):**

| 멤버 함수라면... | 프렌드 함수라면... |
|:---------------|:----------------|
| 구문: `point << cout;` (거꾸로!) | 구문: `cout << point;` (자연스럽다!) |
| 좌측 피연산자가 `Point` (`*this`) | 좌측 피연산자가 `ostream` (첫 번째 매개변수) |
| 다른 출력과 이어 쓸 수 없음: `cout << "Point: " << point;`는 컴파일 오류 | 연쇄 가능: `cout << "Point: " << point << endl;` |
| 모든 C++ 프로그래머가 기대하는 관례를 위반 | 표준 관례를 따름 |

*표 14. 스트림 출력용 operator<<를 멤버와 프렌드로 구현했을 때의 비교*

---

<br>

## 8. 흔한 실수

### 8.1 후위에서 `(int)` 더미 매개변수를 빠뜨림

```cpp
// 잘못된 코드: 둘 다 전위가 된다!
Counter& operator++();       // 이것은 전위 ++obj
Counter operator++();        // 오류: 반환 타입만으로는 중복할 수 없음

// 올바른 코드: int 더미로 구분
Counter& operator++();       // 전위  ++obj
Counter operator++(int);     // 후위 obj++   (int가 더미 매개변수)
```

`int` 매개변수는 실제로 사용되지 않는다. 유일한 목적은 컴파일러가 전위와 후위를 구분할 수 있게 하는 것이다.

### 8.2 참조로 리턴해야 할 때 값으로 리턴

```cpp
// 잘못된 코드: 복합 대입에서 값으로 반환
Vector2D operator+=(const Vector2D &rhs) {   // Vector2D&이어야 함
    x += rhs.x;
    y += rhs.y;
    return *this;   // 복사본을 반환: (a += b) += c에서 두 번째 +=가 임시 객체에 적용됨
}

// 올바른 코드: 참조로 반환
Vector2D& operator+=(const Vector2D &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;   // 실제 객체를 반환: 연쇄가 동작함
}
```

값으로 리턴해도 `a += b += c`는 `a += (b += c)`로 해석되므로 결과가 같다. 그러나 `(a += b) += c`에서는 두 번째 `+=`가 `a`가 아닌 임시 복사본을 수정하므로 `a`에 `c`가 더해지지 않는다.

`operator<<`와 `operator>>`도 마찬가지로 값이 아닌 `ostream&`과 `istream&`으로 리턴해야 한다. 스트림 객체는 복사할 수 없으므로 값으로 리턴하면 컴파일 오류가 발생한다.

### 8.3 복합 대입 연산자에서 `*this` 리턴을 잊음

```cpp
// 잘못된 코드: 객체를 수정하지만 반환을 잊음
void operator+=(const Vector2D &rhs) {   // void 반환: 연쇄 불가!
    x += rhs.x;
    y += rhs.y;
    // 빠짐: return *this;
}

// 올바른 코드:
Vector2D& operator+=(const Vector2D &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;   // a += b += c 가능하게 함
}
```

### 8.4 중복할 수 없는 연산자를 중복하려고 시도

다음 연산자들은 중복할 수 **없다**.

| 연산자 | 이름 |
|:------|:----|
| `::` | 범위 지정 |
| `.` | 멤버 접근 |
| `.*` | 멤버 포인터 접근 |
| `?:` | 삼항 조건 |
| `sizeof` | 객체/타입 크기 |
| `typeid` | 런타임 타입 식별 |

*표 15. 중복할 수 없는 연산자*

이들 중 하나를 중복하려고 하면 컴파일러 오류가 발생한다.

### 8.5 operator<<를 프렌드가 아닌 멤버 함수로 만듦

```cpp
// 잘못된 코드: operator<<를 멤버 함수로 구현
class Vector2D {
    int x, y;
public:
    // 컴파일은 되지만 호출 구문이 거꾸로:
    //     vec << cout;     가 아니라     cout << vec;   이어야 함
    ostream& operator<<(ostream &out) {
        out << "(" << x << ", " << y << ")";
        return out;
    }
};
// 사용:  v << cout;  // 컴파일은 되지만 이상해 보임!

// 올바른 코드: operator<<를 프렌드 함수로 구현
class Vector2D {
    int x, y;
public:
    friend ostream& operator<<(ostream &out, const Vector2D &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;
    }
};
// 사용:  cout << v;  // 자연스럽고 올바름!
```

> **핵심:** 스트림 출력에서 `<<`의 좌측 피연산자는 `ostream`이지 자신의 클래스가 아니다. `ostream`에 멤버 함수를 추가할 수 없으므로, 스트림 출력용 `operator<<`는 **반드시** 프렌드 또는 자유 함수여야 한다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:----|:---------|
| 왜 연산자 중복이 필요한가 | `a.add(b)`를 `a + b`로 쓸 수 있게 하여 사용자 정의 타입을 기본 타입처럼 자연스럽게 사용하게 한다. |
| 프렌드 함수 | 클래스 안에 `friend`로 선언되어 private/protected 멤버에 접근할 수 있는 비멤버 함수이다. |
| 프렌드 클래스 | `friend class B;`는 B의 모든 멤버 함수에 접근을 허용하며, 프렌드 관계는 대칭이 아니고 전이되지도 않는다. |
| 프렌드 선언의 세 가지 형태 | 전역 함수, 다른 클래스의 특정 멤버 함수, 다른 클래스 전체를 프렌드로 선언할 수 있으며, 아직 선언되지 않은 클래스를 참조하려면 전방 선언을 둔다. |
| 프렌드 사용 지침 | 캡슐화를 깨뜨리므로 불가피할 때만 쓰며, 좌측 피연산자가 자신의 클래스가 아닌 일부 연산자 중복에는 필수적이다. |
| 연산자 중복 개념 | `operator` 키워드로 사용자 정의 타입에 대한 연산자를 재정의하며, 새 연산자를 만들거나 우선순위, 결합 방향, 항의 개수를 바꿀 수 없다. |
| 연산자 함수 호출 변환 | `a + b`는 멤버 함수이면 `a.operator+(b)`, 외부 함수이면 `operator+(a, b)`로 바뀌고, `2 + a`는 외부 함수로만 구현할 수 있다. |
| 중복 불가 연산자 | `::`, `.`, `.*`, `?:`, `sizeof`, `typeid`는 중복할 수 없다. |
| 멤버 vs 프렌드 판단 | 좌측 피연산자가 자신의 클래스이면 멤버 함수로, 아니면 비멤버 함수로 작성하고, 비멤버 함수가 private 멤버에 접근해야 하면 프렌드로 선언한다. |
| 이항 연산자 (멤버) | `ReturnType operator+(const ClassName &rhs)` 형식이며 `*this`가 좌측 피연산자이다. |
| 이항 연산자 (프렌드) | `friend ReturnType operator+(const ClassName &lhs, const ClassName &rhs)` 형식이며 두 피연산자가 모두 매개변수이다. |
| 복합 대입 (`+=`, `*=`) | `*this`를 수정하고 `*this`를 참조로 리턴한다. |
| 비교 (`==`, `!=`, `<=`) | `bool`을 리턴하며 보통 `const` 멤버 함수로 작성한다. |
| 단항 연산자 (멤버) | `ReturnType operator~()`처럼 매개변수가 없으며 `*this`가 피연산자이고, `!`도 같은 방식으로 중복한다. |
| 단항 연산자 (프렌드) | `friend ReturnType operator~(const ClassName &obj)`처럼 피연산자가 매개변수이며, 피연산자를 수정하려면 참조로 받는다. |
| 전위 `++`/`--` | `operator++()`는 먼저 수정한 뒤 수정된 객체를 참조로 리턴한다. |
| 후위 `++`/`--` | `operator++(int)`는 복사본을 저장하고 수정한 뒤 복사본을 값으로 리턴하며, `int`는 구분용 더미이다. |
| 전위 vs 후위 추적 | `int x=5; int a=++x;`이면 x=6, a=6이고, 이어서 `int b=x++;`이면 x=7, b=6이다. |
| `operator<<` | 스트림 출력용은 `friend ostream& operator<<(ostream &out, const ClassName &obj)` 형식의 비멤버 함수이며, 연쇄를 위해 `out`을 리턴한다. |
| `operator>>` | `friend istream& operator>>(istream &in, ClassName &obj)` 형식이며 객체를 수정하므로 비const 참조를 받는다. |
| `operator[]` | `int& operator[](int index)`처럼 읽기와 쓰기를 위해 참조를 리턴하며 반드시 멤버 함수여야 한다. |
| 흔한 실수: 후위 더미 | `operator++()`는 전위이고 `operator++(int)`가 후위이며, `int` 더미로 둘을 구분한다. |
| 흔한 실수: 리턴 타입 | 복합 대입과 스트림 연산자는 값이 아닌 참조로 리턴해야 한다. |
| 흔한 실수: `*this` | 복합 대입 연산자는 `return *this;`로 연쇄를 가능하게 해야 한다. |
| 흔한 실수: `<<`를 멤버로 | 스트림 출력용 `operator<<`를 멤버로 만들면 구문이 `obj << cout`으로 뒤집히므로 비멤버(보통 프렌드)로 작성한다. |

---

<br>

## 점검 문제

1. **프렌드 관계:** 클래스 `A`가 `friend class B;`를 선언하고, 클래스 `B`가 `friend class C;`를 선언했다. (1) `B`의 멤버 함수는 `A`의 private 멤버에 접근할 수 있는가? (2) `A`의 멤버 함수는 `B`의 private 멤버에 접근할 수 있는가? (3) `C`의 멤버 함수는 `A`의 private 멤버에 접근할 수 있는가?

   <details>
   <summary>정답</summary>

   (1) 접근할 수 있다. `A`가 `B`를 프렌드로 선언했기 때문이다. (2) 접근할 수 없다. 프렌드 관계는 대칭이 아니므로 `B`가 `A`를 프렌드로 선언하지 않는 한 `A`는 `B`의 private 멤버에 접근할 수 없다. (3) 접근할 수 없다. 프렌드 관계는 전이되지 않으므로 `C`가 `B`의 프렌드라고 해서 `A`의 프렌드가 되지는 않는다.

   </details>

2. **멤버와 비멤버의 선택:** `kick`과 `punch`를 private 멤버로 가진 `Power` 클래스에서 `b = 2 + a;`를 지원하려면 연산자 함수를 어떤 형태로 작성해야 하는가? 컴파일러가 이 식을 어떤 함수 호출로 바꾸는지 함께 쓰라.

   <details>
   <summary>정답</summary>

   좌측 피연산자 `2`가 `int`이므로 `2.operator+(a)` 같은 멤버 함수 호출로 바꿀 수 없다. 컴파일러는 이 식을 `operator+(2, a)`로 바꾸므로 외부 함수 `Power operator+(int op1, Power op2)`를 작성해야 한다. 이 함수는 private 멤버 `kick`과 `punch`에 접근해야 하므로 `Power` 클래스 안에 `friend Power operator+(int op1, Power op2);`로 선언한다.

   </details>

3. **전위와 후위 추적:** 4.4절의 `Counter` 클래스로 `Counter x(5); Counter a = x++; Counter b = ++x;`를 실행한 뒤 `x`, `a`, `b`의 `value`를 쓰라.

   <details>
   <summary>정답</summary>

   `x`는 7, `a`는 5, `b`는 7이다. `x++`는 증가 전의 상태(5)를 복사해 리턴하므로 `a`는 5가 되고 `x`는 6이 된다. 이어서 `++x`는 `x`를 7로 먼저 증가시킨 뒤 수정된 `x`를 리턴하므로 `b`는 7이 된다.

   </details>

4. **참조 리턴과 연쇄:** 5.1절의 참고 상자에서 `Power& Power::operator<<(int n)`의 리턴 타입을 `Power`(값)로 바꾸면 `Power a(1, 2); a << 3 << 5 << 6;`을 실행한 뒤 `a`의 kick과 punch는 얼마가 되는가? 그 이유를 쓰라.

   <details>
   <summary>정답</summary>

   kick은 4, punch는 5가 된다. 식은 `((a << 3) << 5) << 6`으로 해석된다. 첫 번째 `a << 3`은 `a` 자신을 수정해 (4, 5)로 만들지만 그 복사본을 리턴하므로, 이어지는 `<< 5`와 `<< 6`은 `a`가 아닌 임시 복사본에 더해진다. 참조를 리턴하면 세 번의 `<<`가 모두 `a`에 적용되어 kick=15, punch=16이 된다.

   </details>

5. **중복 규칙:** 다음 연산자 중 중복할 수 없는 것과 반드시 멤버 함수로 중복해야 하는 것을 각각 고르라: `+`, `::`, `[]`, `<<`, `?:`, `=`, `sizeof`.

   <details>
   <summary>정답</summary>

   중복할 수 없는 연산자는 `::`, `?:`, `sizeof`이다. 반드시 멤버 함수로 중복해야 하는 연산자는 `[]`와 `=`이다. `+`와 `<<`는 멤버 함수와 비멤버 함수 중 어느 쪽으로도 중복할 수 있으며, 좌측 피연산자의 타입에 따라 고른다.

   </details>
