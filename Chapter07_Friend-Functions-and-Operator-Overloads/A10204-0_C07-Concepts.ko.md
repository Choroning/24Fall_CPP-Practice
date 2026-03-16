# Chapter 07 — 프렌드 함수와 연산자 중복

> **최종 수정일:** 2026-03-16

---

## 목차

- [1. 프렌드](#1-프렌드)
  - [1.1 프렌드 함수](#11-프렌드-함수)
  - [1.2 프렌드 클래스](#12-프렌드-클래스)
  - [1.3 사용 지침](#13-사용-지침)
- [2. 연산자 중복의 기초](#2-연산자-중복의-기초)
  - [2.1 왜 연산자 중복이 필요한가?](#21-왜-연산자-중복이-필요한가)
  - [2.2 개념](#22-개념)
  - [2.3 중복 가능 / 불가능 연산자](#23-중복-가능--불가능-연산자)
  - [2.4 멤버 함수 vs 프렌드 함수 — 판단 가이드](#24-멤버-함수-vs-프렌드-함수--판단-가이드)
- [3. 이항 연산자 중복](#3-이항-연산자-중복)
  - [3.1 멤버 함수로 구현](#31-멤버-함수로-구현)
  - [3.2 프렌드 함수로 구현](#32-프렌드-함수로-구현)
  - [3.3 복합 대입 연산자](#33-복합-대입-연산자)
  - [3.4 비교 연산자](#34-비교-연산자)
- [4. 단항 연산자 중복](#4-단항-연산자-중복)
  - [4.1 멤버 함수로 구현](#41-멤버-함수로-구현)
  - [4.2 프렌드 함수로 구현](#42-프렌드-함수로-구현)
  - [4.3 증가 / 감소 연산자](#43-증가--감소-연산자)
  - [4.4 전위 vs 후위 — 상세 추적](#44-전위-vs-후위--상세-추적)
- [5. 스트림 연산자 중복](#5-스트림-연산자-중복)
  - [5.1 `operator<<`](#51-operator)
  - [5.2 `operator>>`](#52-operator-1)
- [6. 첨자 연산자](#6-첨자-연산자)
- [7. 완전한 실행 가능 예제](#7-완전한-실행-가능-예제)
  - [7.1 완전한 Vector2D 클래스](#71-완전한-vector2d-클래스)
  - [7.2 전위 vs 후위 출력 추적](#72-전위-vs-후위-출력-추적)
  - [7.3 operator<<가 프렌드여야 하는 이유](#73-operator가-프렌드여야-하는-이유)
- [8. 흔한 실수](#8-흔한-실수)
- [요약](#요약)

---

<br>

## 1. 프렌드

### 1.1 프렌드 함수

**프렌드 함수**는 클래스의 `private` 및 `protected` 멤버에 접근할 수 있도록 허용된 **비멤버 함수**이다. 클래스 내부에서 `friend` 키워드로 선언하지만, 멤버 함수는 **아니다**.

```cpp
class Box {
    int width;
public:
    Box(int w) : width(w) {}
    friend void printWidth(const Box &b);   // 프렌드 선언
};

// 정의 — 멤버 함수가 아님 (Box:: 범위 지정 없음)
void printWidth(const Box &b) {
    cout << b.width << endl;   // private 멤버에 직접 접근
}
```

| 특성 | 설명 |
|:----|:----|
| 클래스 내부에 `friend`로 선언 | private/protected 멤버 접근 권한 부여 |
| 클래스 외부에서 `ClassName::` 범위 지정 없이 정의 | 멤버 함수가 아님 |
| `this` 포인터 없음 | 객체를 매개변수로 전달받아야 함 |
| `public` / `private` / `protected` 섹션에 영향받지 않음 | `friend` 선언은 클래스 어디에든 위치 가능 |

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

### 1.3 사용 지침

- `friend`는 **꼭 필요한 경우에만** 사용하라 — 캡슐화를 깨뜨려 외부 코드에 private 멤버를 노출한다.
- 좌측 피연산자가 자신의 클래스 타입이 아닌 경우의 연산자 중복에 **필수적**이다 (예: `ostream& operator<<`).
- 가능하면 멤버 함수를 우선하고, 멤버 함수로 불충분할 때만 `friend`를 사용하라.

---

<br>

## 2. 연산자 중복의 기초

### 2.1 왜 연산자 중복이 필요한가?

연산자 중복이 없으면, 사용자 정의 타입의 연산에 명시적 메서드 호출이 필요하다. 두 접근 방식을 비교해 보자:

```cpp
// 연산자 중복 없이 — 장황하고 읽기 어려움
Vector2D a(1, 2), b(3, 4), c(5, 6);
Vector2D result = a.add(b).subtract(c);          // 이게 뭘 계산하는 건지?
if (result.equals(Vector2D(-1, 0)))
    cout << result.toString() << endl;

// 연산자 중복 사용 — 자연스럽고 수학처럼 읽힘
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

### 2.2 개념

**연산자 중복(Operator Overloading)**은 사용자 정의 타입(클래스)에 대해 C++ 연산자의 의미를 재정의하는 것이다. `operator` 키워드 뒤에 연산자 기호를 붙여 사용한다.

```cpp
Vector2D operator+(const Vector2D &rhs);   // Vector2D에 대해 + 연산자 중복
```

**제한사항:**

| 제한 | 설명 |
|:----|:----|
| 새로운 연산자를 만들 수 없음 | `operator@`나 `operator$` 등은 불가 |
| **우선순위** 변경 불가 | `*`가 `+`보다 우선하는 것은 그대로 유지 |
| **결합 방향** 변경 불가 | 좌→우 또는 우→좌 결합 방향 그대로 유지 |
| **항(arity)** 변경 불가 | 이항 연산자는 이항으로, 단항 연산자는 단항으로 유지 |
| 피연산자 중 하나는 사용자 정의 타입이어야 함 | `int + int`는 중복할 수 없음 |

### 2.3 중복 가능 / 불가능 연산자

| 분류 | 연산자 |
|:----|:------|
| 중복 **가능** | `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `<<` `>>` `[]` `()` `~` `!` `&` `\|` `^` `++` `--` `+=` `-=` `*=` `/=` `%=` `=` `->` `new` `delete` ... |
| 중복 **불가** | `::` (범위 지정), `.` (멤버 접근), `.*` (멤버 포인터), `?:` (삼항), `sizeof`, `typeid` |

### 2.4 멤버 함수 vs 프렌드 함수 — 판단 가이드

| 관점 | 멤버 함수 | 프렌드 함수 |
|:----|:---------|:----------|
| 좌측 피연산자 | 암묵적으로 `*this` | 명시적으로 첫 번째 매개변수 |
| 우측 피연산자 | 매개변수 | 두 번째 매개변수 |
| private 접근 | 가능 (멤버이므로) | 가능 (friend로 선언) |
| 사용 시점 | 좌측 피연산자가 **자신의 클래스** 타입일 때 (예: `obj + 3`) | 좌측 피연산자가 **자신의 클래스가 아닐** 때 (예: `3 + obj`, `cout << obj`) |

```cpp
// 멤버: 좌측 피연산자가 *this
Vector2D Vector2D::operator+(const Vector2D &rhs);

// 프렌드: 양쪽 피연산자 모두 매개변수
friend Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
```

> **핵심:** 좌측 피연산자가 기본 타입이거나 수정할 수 없는 클래스(예: `ostream`)인 경우, **반드시** 프렌드(또는 자유) 함수를 사용해야 한다.

**판단 순서도:**

```
좌측 피연산자가 자신의 클래스 타입인가?
 |
 +-- 예 --> 연산자가 좌측 피연산자를 수정하는가? (예: +=, ++, =)
 |            |
 |            +-- 예 --> 멤버 함수 (*this를 참조로 반환)
 |            |
 |            +-- 아니오 --> 멤버 함수 (새 객체를 값으로 반환)
 |
 +-- 아니오 --> (예: cout << obj,  3 + obj,  cin >> obj)
                |
                private 멤버에 접근해야 하는가?
                |
                +-- 예 --> 프렌드 함수
                |
                +-- 아니오 --> 비멤버 비프렌드 함수
```

**빠른 참조 표:**

| 표현식 | 좌측 피연산자 | 권장 방식 | 이유 |
|:------|:-----------|:---------|:----|
| `obj + obj2` | 자신의 클래스 | 멤버 함수 | 좌측 피연산자가 `*this` |
| `obj += obj2` | 자신의 클래스 | 멤버 함수 | `*this`를 수정하고 반환 |
| `3 + obj` | `int` (자신의 클래스 아님) | 프렌드 함수 | `int`에 멤버 함수를 추가할 수 없음 |
| `cout << obj` | `ostream` (자신의 클래스 아님) | 프렌드 함수 | `ostream`에 멤버 함수를 추가할 수 없음 |
| `cin >> obj` | `istream` (자신의 클래스 아님) | 프렌드 함수 | `istream`에 멤버 함수를 추가할 수 없음 |
| `++obj` / `obj++` | 자신의 클래스 | 멤버 함수 | 좌측 피연산자가 `*this` |
| `obj[i]` | 자신의 클래스 | 멤버 함수 (필수) | `operator[]`는 반드시 멤버여야 함 |
| `obj = obj2` | 자신의 클래스 | 멤버 함수 (필수) | `operator=`는 반드시 멤버여야 함 |

---

<br>

## 3. 이항 연산자 중복

### 3.1 멤버 함수로 구현

좌측 피연산자가 `*this`이고, 우측 피연산자가 매개변수이다. 산술 연산자는 **값으로 반환**한다.

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

복합 대입 연산자(`+=`, `-=`, `*=`)는 좌측 피연산자를 수정하고 **참조로 `*this`를 반환**한다.

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

| 반환 타입 | 이유 |
|:---------|:----|
| `ClassName&` (참조) | 객체가 제자리에서 수정됨; `*this` 반환으로 연쇄 가능 (`a += b += c`) |

### 3.4 비교 연산자

비교 연산자는 `bool`을 반환한다. 보통 `const` 멤버 함수로 작성한다.

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

단항 연산자를 멤버 함수로 구현할 때 매개변수는 **없다** — `*this`가 피연산자이다.

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

| 형태 | 시그니처 (멤버) | 반환 | 동작 |
|:----|:-------------|:----|:----|
| 전위 `++obj` / `--obj` | `ClassName& operator++()` | 수정된 객체의 참조 | 먼저 수정, 그 후 반환 |
| 후위 `obj++` / `obj--` | `ClassName operator++(int)` | 원본(수정 전)의 복사본 | 복사본 저장, 수정, 복사본 반환 |

> **핵심:** 후위 버전의 `int` 매개변수는 **더미**이다 — 실제로 사용되지 않는다. 유일한 목적은 컴파일러가 전위와 후위를 구분할 수 있게 하는 것이다.

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

### 4.4 전위 vs 후위 — 상세 추적

전위와 후위의 차이를 정확히 이해하는 것이 중요하다. 먼저 기본 타입 `int`로 직관을 쌓아 보자:

```
 표현식               단계별 설명                        결과
-------------       ----------------------------    -----------
int x = 5;          x가 5로 초기화                      x=5
int a = ++x;        x가 6이 되고, 그 후 a에 값 대입       x=6, a=6
int b = x++;        b에 값(6) 대입, 그 후 x가 7이 됨      x=7, b=6
int c = --x;        x가 6이 되고, 그 후 c에 값 대입       x=6, c=6
int d = x--;        d에 값(6) 대입, 그 후 x가 5가 됨      x=5, d=6
```

**핵심 규칙:**
- **전위** (`++x`): **먼저** 증가시키고, **새로운** 값을 사용한다.
- **후위** (`x++`): **이전** 값을 사용하고, 그 후 증가시킨다.

이제 클래스로 구현한 동일한 동작 — 구현이 위의 동작을 어떻게 반영하는지 주목하자:

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

클래스의 추적 표:

```
 코드                      value (이전)    반환값          value (이후)
---------------------    ------------   ------   -----------------------
Counter x(5);                 —          —                  5
Counter a = ++x;              5          6        6 (먼저 수정, 새 값 반환)
Counter b = x++;              6          6       7 (이전 값 반환, 그 후 수정)
```

> **왜 전위는 참조로 반환하고 후위는 값으로 반환하는가?**
> - 전위는 **실제 수정된 객체**를 반환한다 — 객체가 여전히 존재하므로 참조가 안전하고 효율적이다.
> - 후위는 **이전 상태**를 반환해야 한다 — 하지만 이전 상태는 객체 안에 더 이상 존재하지 않으므로 임시 복사본(값으로 반환)이어야 한다.

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
| `ostream&` 반환 | 연쇄 가능: `cout << a << b << c` |
| 두 번째 매개변수는 `const ClassName&` | 출력 시 객체를 수정하면 안 됨 |

### 5.2 `operator>>`

추출 연산자 `>>`는 같은 패턴을 따르되, `istream`을 사용하고 **비const** 참조를 받는다 (객체를 수정하므로).

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

`operator[]`는 객체를 배열처럼 접근할 수 있게 한다. 읽기와 쓰기 모두 가능하도록 **참조를 반환**한다.

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
| 반환 타입이 `int&` (참조) | 읽기(`x = d[0]`)와 쓰기(`d[0] = 2025`) 모두 가능 |
| 반드시 **멤버 함수**여야 함 | `operator[]`는 프렌드/자유 함수가 될 수 없음 |
| 매개변수는 보통 `int` | 인덱스를 나타냄 |

---

<br>

## 7. 완전한 실행 가능 예제

### 7.1 완전한 Vector2D 클래스

`operator+`, `operator==`, `operator<<`가 함께 자연스럽게 동작하는 완전한 프로그램:

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

```
(1, 2) + (3, 4) = (4, 6)
같음!
(1,2) + (3,4) + (5,4) = (9, 10)
```

### 7.2 전위 vs 후위 출력 추적

각 단계에서 출력으로 `++x`와 `x++`의 차이를 명확히 보여주는 프로그램:

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

```
초기값:  x = 5
a = ++x 후:  x = 6, a = 6
b = x++ 후:  x = 7, b = 6
++x = 8
```

### 7.3 operator<<가 프렌드여야 하는 이유

`operator<<`가 **왜** 멤버 함수가 될 수 없는지, 만약 시도하면 어떻게 되는지 보여주는 예제:

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

    // 자연스러운 구문 — operator<<가 프렌드이므로 동작
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

```
Point: (3, 7)
(1, 2) and (3, 4)
```

**왜 프렌드여야 하는가 (멤버가 아니라):**

| 멤버 함수라면... | 프렌드 함수라면... |
|:---------------|:----------------|
| 구문: `point << cout;` (거꾸로!) | 구문: `cout << point;` (자연스럽다!) |
| 좌측 피연산자가 `Point` (`*this`) | 좌측 피연산자가 `ostream` (첫 번째 매개변수) |
| 연쇄 불가: `point << cout << endl;` 실패 | 연쇄 가능: `cout << point << endl;` |
| 모든 C++ 프로그래머가 기대하는 관례를 위반 | 표준 관례를 따름 |

---

<br>

## 8. 흔한 실수

### 실수 1: 후위에서 `(int)` 더미 매개변수를 빠뜨림

```cpp
// 잘못된 코드: 둘 다 전위가 된다!
Counter& operator++();       // 이것은 전위 ++obj
Counter operator++();        // 오류: 반환 타입만으로는 중복할 수 없음

// 올바른 코드: int 더미로 구분
Counter& operator++();       // 전위  ++obj
Counter operator++(int);     // 후위 obj++   (int가 더미 매개변수)
```

`int` 매개변수는 실제로 사용되지 않는다 — 유일한 목적은 컴파일러가 전위와 후위를 구분할 수 있게 하는 것이다.

### 실수 2: 참조로 반환해야 할 때 값으로 반환

```cpp
// 잘못된 코드: 복합 대입에서 값으로 반환
Vector2D operator+=(const Vector2D &rhs) {   // Vector2D&이어야 함
    x += rhs.x;
    y += rhs.y;
    return *this;   // 복사본을 반환 — 연쇄 (a += b += c)가 깨짐
}

// 올바른 코드: 참조로 반환
Vector2D& operator+=(const Vector2D &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;   // 실제 객체를 반환 — 연쇄가 동작함
}
```

`operator<<`와 `operator>>`도 마찬가지이다 — 값이 아닌 `ostream&`과 `istream&`으로 반환해야 한다.

### 실수 3: 복합 대입 연산자에서 `*this` 반환을 잊음

```cpp
// 잘못된 코드: 객체를 수정하지만 반환을 잊음
void operator+=(const Vector2D &rhs) {   // void 반환 — 연쇄 불가!
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

### 실수 4: 중복할 수 없는 연산자를 중복하려고 시도

다음 연산자들은 중복할 수 **없다**:

| 연산자 | 이름 |
|:------|:----|
| `::` | 범위 지정 |
| `.` | 멤버 접근 |
| `.*` | 멤버 포인터 접근 |
| `?:` | 삼항 조건 |
| `sizeof` | 객체/타입 크기 |
| `typeid` | 런타임 타입 식별 |

이들 중 하나를 중복하려고 하면 컴파일러 오류가 발생한다.

### 실수 5: operator<<를 프렌드가 아닌 멤버 함수로 만듦

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

**기억하라:** `<<`의 좌측 피연산자는 `ostream`이지 자신의 클래스가 아니다. `ostream`에 멤버 함수를 추가할 수 없으므로, `operator<<`는 **반드시** 프렌드 또는 자유 함수여야 한다.

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 왜 연산자 중복이 필요한가 | `a.add(b)`를 `a + b`로 변환; 사용자 정의 타입을 기본 타입처럼 자연스럽게 사용 가능 |
| 프렌드 함수 | private/protected 접근 권한이 있는 비멤버 함수; 클래스 내에 `friend`로 선언 |
| 프렌드 클래스 | `friend class B;`로 B의 모든 멤버 함수에 접근 허용; 대칭 아님, 전이적 아님 |
| 프렌드 사용 지침 | 꼭 필요할 때만 사용; 캡슐화를 깨뜨림; 특정 연산자 중복에 필수 |
| 연산자 중복 개념 | 사용자 정의 타입에 대해 연산자 재정의; `operator` 키워드; 새 연산자 생성 불가, 우선순위/결합방향/항 변경 불가 |
| 중복 불가 연산자 | `::`, `.`, `.*`, `?:`, `sizeof`, `typeid` |
| 멤버 vs 프렌드 판단 | 좌측이 자신의 클래스? 멤버. 좌측이 자신의 클래스가 아닌 경우? 프렌드. private 필요? 프렌드. 그 외? 비멤버 비프렌드. |
| 이항 연산자 (멤버) | `ReturnType operator+(const ClassName &rhs)`; `*this`가 좌측 피연산자 |
| 이항 연산자 (프렌드) | `friend ReturnType operator+(const ClassName &lhs, const ClassName &rhs)` |
| 복합 대입 (`+=`, `*=`) | `*this`를 수정하고 `*this`를 참조로 반환 |
| 비교 (`==`, `!=`, `<=`) | `bool` 반환; 보통 `const` 멤버 함수 |
| 단항 연산자 (멤버) | `ReturnType operator~()`; 매개변수 없음; `*this`가 피연산자 |
| 단항 연산자 (프렌드) | `friend ReturnType operator~(const ClassName &obj)` |
| 전위 `++`/`--` | `operator++()` 참조 반환; 먼저 수정, 그 후 반환 |
| 후위 `++`/`--` | `operator++(int)` 값 반환; 복사본 저장, 수정, 복사본 반환; `int`는 더미 |
| 전위 vs 후위 추적 | `int x=5; int a=++x;`이면 x=6,a=6. `int b=x++;`이면 x=7,b=6. 전위는 참조 반환, 후위는 값 반환. |
| `operator<<` | `friend ostream& operator<<(ostream &out, const ClassName &obj)`; 반드시 프렌드; 연쇄를 위해 `out` 반환 |
| `operator>>` | `friend istream& operator>>(istream &in, ClassName &obj)`; 비const 참조 |
| `operator[]` | `int& operator[](int index)`; 읽기/쓰기를 위해 참조 반환; 반드시 멤버 함수 |
| 흔한 실수: 후위 더미 | `operator++()`는 전위; `operator++(int)`가 후위 — `int` 더미로 구분 |
| 흔한 실수: 반환 타입 | 복합 대입과 스트림 연산자는 값이 아닌 **참조**로 반환해야 함 |
| 흔한 실수: `*this` | 복합 대입 연산자는 반드시 `return *this;`로 연쇄를 가능하게 해야 함 |
| 흔한 실수: `<<`를 멤버로 | `operator<<`를 멤버로 만들면 `obj << cout` 구문이 됨 — 항상 프렌드를 사용 |

---
