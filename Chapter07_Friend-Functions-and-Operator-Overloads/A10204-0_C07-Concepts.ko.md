# Chapter 07 — 프렌드 함수와 연산자 중복

> **최종 수정일:** 2026-03-16
>
> **교재 참조:** *명품 C++ Programming 개정판* — 7장

---

## 목차

- [1. 프렌드](#1-프렌드)
  - [1.1 프렌드 함수](#11-프렌드-함수)
  - [1.2 프렌드 클래스](#12-프렌드-클래스)
  - [1.3 사용 지침](#13-사용-지침)
- [2. 연산자 중복의 기초](#2-연산자-중복의-기초)
  - [2.1 개념](#21-개념)
  - [2.2 중복 가능 / 불가능 연산자](#22-중복-가능--불가능-연산자)
  - [2.3 멤버 함수 vs 프렌드 함수](#23-멤버-함수-vs-프렌드-함수)
- [3. 이항 연산자 중복](#3-이항-연산자-중복)
  - [3.1 멤버 함수로 구현](#31-멤버-함수로-구현)
  - [3.2 프렌드 함수로 구현](#32-프렌드-함수로-구현)
  - [3.3 복합 대입 연산자](#33-복합-대입-연산자)
  - [3.4 비교 연산자](#34-비교-연산자)
- [4. 단항 연산자 중복](#4-단항-연산자-중복)
  - [4.1 멤버 함수로 구현](#41-멤버-함수로-구현)
  - [4.2 프렌드 함수로 구현](#42-프렌드-함수로-구현)
  - [4.3 증가 / 감소 연산자](#43-증가--감소-연산자)
- [5. 스트림 연산자 중복](#5-스트림-연산자-중복)
  - [5.1 `operator<<`](#51-operator)
  - [5.2 `operator>>`](#52-operator-1)
- [6. 첨자 연산자](#6-첨자-연산자)
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

### 2.1 개념

**연산자 중복(Operator Overloading)**은 사용자 정의 타입(클래스)에 대해 C++ 연산자의 의미를 재정의하는 것이다. `operator` 키워드 뒤에 연산자 기호를 붙여 사용한다.

```cpp
Fraction operator+(const Fraction &rhs);   // Fraction에 대해 + 연산자 중복
```

**제한사항:**

| 제한 | 설명 |
|:----|:----|
| 새로운 연산자를 만들 수 없음 | `operator@`나 `operator$` 등은 불가 |
| **우선순위** 변경 불가 | `*`가 `+`보다 우선하는 것은 그대로 유지 |
| **결합 방향** 변경 불가 | 좌→우 또는 우→좌 결합 방향 그대로 유지 |
| **항(arity)** 변경 불가 | 이항 연산자는 이항으로, 단항 연산자는 단항으로 유지 |
| 피연산자 중 하나는 사용자 정의 타입이어야 함 | `int + int`는 중복할 수 없음 |

### 2.2 중복 가능 / 불가능 연산자

| 분류 | 연산자 |
|:----|:------|
| 중복 **가능** | `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `<<` `>>` `[]` `()` `~` `!` `&` `\|` `^` `++` `--` `+=` `-=` `*=` `/=` `%=` `=` `->` `new` `delete` ... |
| 중복 **불가** | `::` (범위 지정), `.` (멤버 접근), `.*` (멤버 포인터), `?:` (삼항), `sizeof`, `typeid` |

### 2.3 멤버 함수 vs 프렌드 함수

| 관점 | 멤버 함수 | 프렌드 함수 |
|:----|:---------|:----------|
| 좌측 피연산자 | 암묵적으로 `*this` | 명시적으로 첫 번째 매개변수 |
| 우측 피연산자 | 매개변수 | 두 번째 매개변수 |
| private 접근 | 가능 (멤버이므로) | 가능 (friend로 선언) |
| 사용 시점 | 좌측 피연산자가 **자신의 클래스** 타입일 때 (예: `obj + 3`) | 좌측 피연산자가 **자신의 클래스가 아닐** 때 (예: `3 + obj`, `cout << obj`) |

```cpp
// 멤버: 좌측 피연산자가 *this
Fraction Fraction::operator+(const Fraction &rhs);

// 프렌드: 양쪽 피연산자 모두 매개변수
friend Fraction operator+(const Fraction &lhs, const Fraction &rhs);
```

> **핵심:** 좌측 피연산자가 기본 타입이거나 수정할 수 없는 클래스(예: `ostream`)인 경우, **반드시** 프렌드(또는 자유) 함수를 사용해야 한다.

---

<br>

## 3. 이항 연산자 중복

### 3.1 멤버 함수로 구현

좌측 피연산자가 `*this`이고, 우측 피연산자가 매개변수이다. 산술 연산자는 **값으로 반환**한다.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    Fraction operator+(const Fraction &rhs) const {
        return Fraction(num * rhs.denom + rhs.num * denom,
                        denom * rhs.denom);
    }
};

// 사용
Fraction a(1, 2), b(1, 3);
Fraction c = a + b;   // a.operator+(b) 호출  →  5/6
```

### 3.2 프렌드 함수로 구현

양쪽 피연산자 모두 명시적 매개변수이다. 좌측 피연산자가 클래스 타입이 아닐 때 유용하다.

```cpp
class Complex {
    int real, imag;
public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}

    // 좌측 피연산자가 int이므로 프렌드 필요
    friend Complex operator+(int lhs, const Complex &rhs) {
        return Complex(lhs + rhs.real, rhs.imag);
    }
};

// 사용
Complex b(2, -3);
Complex c = 10 + b;   // operator+(10, b) 호출  →  12-3i
```

### 3.3 복합 대입 연산자

복합 대입 연산자(`+=`, `-=`, `*=`)는 좌측 피연산자를 수정하고 **참조로 `*this`를 반환**한다.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    Fraction& operator*=(const Fraction &rhs) {
        num *= rhs.num;
        denom *= rhs.denom;
        return *this;   // 참조로 반환
    }
};

// 사용
Fraction f1(3, 4), f2(2, 5);
f1 *= f2;   // f1이 6/20이 됨
```

| 반환 타입 | 이유 |
|:---------|:----|
| `ClassName&` (참조) | 객체가 제자리에서 수정됨; `*this` 반환으로 연쇄 가능 (`a += b += c`) |

### 3.4 비교 연산자

비교 연산자는 `bool`을 반환한다. 보통 `const` 멤버 함수로 작성한다.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    bool operator==(const Fraction &rhs) const {
        // 교차 곱으로 비교: a/b == c/d ↔ a*d == c*b
        return num * rhs.denom == rhs.num * denom;
    }
};

// 사용
Fraction f2(2, 5), f5(6, 15);
if (f2 == f5)   // 참: 2*15 == 6*5
    cout << "같음" << endl;
```

---

<br>

## 4. 단항 연산자 중복

### 4.1 멤버 함수로 구현

단항 연산자를 멤버 함수로 구현할 때 매개변수는 **없다** — `*this`가 피연산자이다.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    // ~ 는 역수를 반환 (원본을 수정하지 않음)
    Fraction operator~() const {
        return Fraction(denom, num);
    }
};

// 사용
Fraction f(2, 5);
Fraction inv = ~f;   // inv = 5/2,  f는 변하지 않음
```

### 4.2 프렌드 함수로 구현

프렌드로 구현할 때는 피연산자가 명시적 매개변수이다.

```cpp
class Complex {
    int real, imag;
public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}

    // ~ 는 켤레복소수를 반환
    friend Complex operator~(const Complex &obj) {
        return Complex(obj.real, -obj.imag);
    }
};

// 사용
Complex b(2, -3);
Complex c = ~b;   // c = 2+3i,  b는 변하지 않음
```

### 4.3 증가 / 감소 연산자

C++은 더미 `int` 매개변수로 **전위**와 **후위** 형태를 구분한다.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    // 전위 --obj: 수정한 후, 수정된 객체를 참조로 반환
    Fraction& operator--() {
        num -= denom;   // 1을 뺌 (denom/denom)
        return *this;
    }

    // 후위 obj++: 원본을 저장하고, 수정한 후, 저장한 복사본을 값으로 반환
    Fraction operator++(int) {   // int는 더미 매개변수
        Fraction old = *this;
        num += denom;   // 1을 더함 (denom/denom)
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
class Complex {
    int real, imag;
public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}

    // 전위 ++obj
    friend Complex& operator++(Complex &obj) {
        obj.real++;
        return obj;
    }

    // 후위 obj--
    friend Complex operator--(Complex &obj, int) {
        Complex old = obj;
        obj.real--;
        return old;
    }
};
```

---

<br>

## 5. 스트림 연산자 중복

### 5.1 `operator<<`

삽입 연산자 `<<`는 좌측 피연산자가 `ostream`이므로 반드시 **프렌드**(또는 자유 함수)여야 한다.

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    friend ostream& operator<<(ostream &out, const Fraction &f) {
        out << f.num << "/" << f.denom;
        return out;   // 연쇄를 위해 out 반환
    }
};

// 사용
Fraction f(3, 4);
cout << "값: " << f << endl;   // "값: 3/4" 출력
```

| 사항 | 설명 |
|:----|:----|
| 좌측 피연산자가 `ostream&` | 자신의 클래스의 멤버가 될 수 없음 |
| `ostream&` 반환 | 연쇄 가능: `cout << a << b << c` |
| 두 번째 매개변수는 `const ClassName&` | 출력 시 객체를 수정하면 안 됨 |

### 5.2 `operator>>`

추출 연산자 `>>`는 같은 패턴을 따르되, `istream`을 사용하고 **비const** 참조를 받는다 (객체를 수정하므로).

```cpp
class Fraction {
    int num, denom;
public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {}

    friend istream& operator>>(istream &in, Fraction &f) {
        char slash;
        in >> f.num >> slash >> f.denom;   // "3/4" 형태의 입력을 기대
        return in;
    }
};

// 사용
Fraction f;
cin >> f;   // 사용자가 "3/4" 입력
```

---

<br>

## 6. 첨자 연산자

`operator[]`는 객체를 배열처럼 접근할 수 있게 한다. 읽기와 쓰기 모두 가능하도록 **참조를 반환**한다.

```cpp
class Time {
    int hour, min, sec;
public:
    Time(int h = 0, int m = 0, int s = 0) : hour(h), min(m), sec(s) {}

    int& operator[](int index) {
        switch (index) {
            case 0: return hour;
            case 1: return min;
            case 2: return sec;
            default: return hour;   // 기본값
        }
    }
};

// 사용
Time t(10, 30, 45);
t[0] = 3;    // hour를 3으로 설정
t[1] = 9;    // min을 9로 설정
t[2] = 27;   // sec을 27로 설정
cout << t[0] << ":" << t[1] << ":" << t[2] << endl;   // 3:9:27
```

| 사항 | 설명 |
|:----|:----|
| 반환 타입이 `int&` (참조) | 읽기(`x = t[0]`)와 쓰기(`t[0] = 3`) 모두 가능 |
| 반드시 **멤버 함수**여야 함 | `operator[]`는 프렌드/자유 함수가 될 수 없음 |
| 매개변수는 보통 `int` | 인덱스를 나타냄 |

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 프렌드 함수 | private/protected 접근 권한이 있는 비멤버 함수; 클래스 내에 `friend`로 선언 |
| 프렌드 클래스 | `friend class B;`로 B의 모든 멤버 함수에 접근 허용; 대칭 아님, 전이적 아님 |
| 프렌드 사용 지침 | 꼭 필요할 때만 사용; 캡슐화를 깨뜨림; 특정 연산자 중복에 필수 |
| 연산자 중복 개념 | 사용자 정의 타입에 대해 연산자 재정의; `operator` 키워드; 새 연산자 생성 불가, 우선순위/결합방향/항 변경 불가 |
| 중복 불가 연산자 | `::`, `.`, `.*`, `?:`, `sizeof`, `typeid` |
| 멤버 vs 프렌드 | 멤버: `*this`가 좌측 피연산자; 프렌드: 양쪽 피연산자 모두 매개변수; 좌측이 자신의 클래스가 아니면 프렌드 사용 |
| 이항 연산자 (멤버) | `ReturnType operator+(const ClassName &rhs)`; `*this`가 좌측 피연산자 |
| 이항 연산자 (프렌드) | `friend ReturnType operator+(const ClassName &lhs, const ClassName &rhs)` |
| 복합 대입 (`+=`, `*=`) | `*this`를 수정하고 `*this`를 참조로 반환 |
| 비교 (`==`, `!=`, `<=`) | `bool` 반환; 보통 `const` 멤버 함수 |
| 단항 연산자 (멤버) | `ReturnType operator~()`; 매개변수 없음; `*this`가 피연산자 |
| 단항 연산자 (프렌드) | `friend ReturnType operator~(const ClassName &obj)` |
| 전위 `++`/`--` | `operator++()` 참조 반환; 먼저 수정, 그 후 반환 |
| 후위 `++`/`--` | `operator++(int)` 값 반환; 복사본 저장, 수정, 복사본 반환; `int`는 더미 |
| `operator<<` | `friend ostream& operator<<(ostream &out, const ClassName &obj)`; 반드시 프렌드; 연쇄를 위해 `out` 반환 |
| `operator>>` | `friend istream& operator>>(istream &in, ClassName &obj)`; 비const 참조 |
| `operator[]` | `int& operator[](int index)`; 읽기/쓰기를 위해 참조 반환; 반드시 멤버 함수 |

---
