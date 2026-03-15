# Chapter 05 — 함수와 복사 제어

> **최종 수정일:** 2026-03-16
>
> **교재 참조:** *명품 C++ Programming (개정판)* — 황기태 저, 5장

---

## 목차

- [1. 값, 참조, 포인터에 의한 호출](#1-값-참조-포인터에-의한-호출)
  - [1.1 값에 의한 호출 (Call by Value)](#11-값에-의한-호출-call-by-value)
  - [1.2 주소에 의한 호출 (Call by Address)](#12-주소에-의한-호출-call-by-address)
  - [1.3 참조에 의한 호출 (Call by Reference)](#13-참조에-의한-호출-call-by-reference)
  - [1.4 비교 표](#14-비교-표)
- [2. 참조 변수](#2-참조-변수)
  - [2.1 선언](#21-선언)
  - [2.2 참조 매개변수](#22-참조-매개변수)
  - [2.3 참조 반환](#23-참조-반환)
- [3. 복사 생성자](#3-복사-생성자)
  - [3.1 개념](#31-개념)
  - [3.2 디폴트 복사 생성자](#32-디폴트-복사-생성자)
  - [3.3 얕은 복사의 문제](#33-얕은-복사의-문제)
  - [3.4 깊은 복사](#34-깊은-복사)
- [요약](#요약)

---

<br>

## 1. 값, 참조, 포인터에 의한 호출

### 1.1 값에 의한 호출 (Call by Value)

함수가 인수의 **복사본**을 전달받는다. 함수 내부에서 매개변수를 수정해도 원본 변수에는 **영향을 주지 않는다**. C 언어와 동일한 방식이다.

```cpp
#include <iostream>
using namespace std;

void addOne(int x) {
    x = x + 1;   // 지역 복사본만 수정
    cout << "함수 내부: x = " << x << endl;
}

int main() {
    int a = 10;
    addOne(a);
    cout << "함수 외부: a = " << a << endl;   // a는 여전히 10
    return 0;
}
```

```
출력:
함수 내부: x = 11
함수 외부: a = 10
```

```
값에 의한 호출
┌──────────────┐
│ main         │
│  a = 10      │  ← 원본 변경 없음
└──────┬───────┘
       │ 복사
       ▼
┌──────────────┐
│ addOne       │
│  x = 10 → 11│  ← 지역 복사본만 변경
└──────────────┘
```

### 1.2 주소에 의한 호출 (Call by Address)

함수가 원본 변수의 **포인터**를 전달받는다. `*`로 역참조하여 원본을 수정할 수 있다. 호출자는 `&`로 주소를 전달해야 한다. C 언어와 동일한 방식이다.

```cpp
#include <iostream>
using namespace std;

void addOne(int *p) {
    *p = *p + 1;   // 포인터를 통해 원본 수정
}

int main() {
    int a = 10;
    addOne(&a);     // a의 주소를 전달
    cout << "a = " << a << endl;   // a는 이제 11
    return 0;
}
```

```
주소에 의한 호출 (포인터)
┌───────────────┐
│ main          │
│  a = 10 → 11  │  ← 원본이 수정됨
└───────┬───────┘
        │ &a (주소)
        ▼
┌───────────────┐
│ addOne        │
│  p = &a       │
│  *p = 10 → 11 │
└───────────────┘
```

### 1.3 참조에 의한 호출 (Call by Reference)

**C++ 전용.** 함수가 원본 변수에 대한 **참조** `&`를 전달받는다. 참조는 원본과 동일한 메모리를 가리키는 별명(alias)이다. 호출 시 `*`나 `&`가 필요 없어 포인터보다 문법이 깔끔하다.

```cpp
#include <iostream>
using namespace std;

void addOne(int &x) {   // x는 원본에 대한 참조
    x = x + 1;          // 원본을 직접 수정
}

int main() {
    int a = 10;
    addOne(a);           // 호출 시 &가 필요 없음
    cout << "a = " << a << endl;   // a는 이제 11
    return 0;
}
```

```
참조에 의한 호출
┌───────────────┐
│ main          │
│  a = 10 → 11  │  ← 원본이 수정됨
└───────┬───────┘
        │ 참조 (별명)
        ▼
┌───────────────┐
│ addOne        │
│  x ≡ a       │  ← x는 a 그 자체 (같은 메모리)
│  x = 10 → 11 │
└───────────────┘
```

**swap 예제 — 참조 vs 포인터:**

```cpp
// 참조 버전 (C++ 스타일) — 깔끔
void swapRef(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// 포인터 버전 (C 스타일) — 동작하지만 장황함
void swapPtr(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 10, y = 20;
    swapRef(x, y);     // 깔끔한 호출
    swapPtr(&x, &y);   // 주소를 전달해야 함
    return 0;
}
```

### 1.4 비교 표

| 특성 | 값에 의한 호출 | 주소에 의한 호출 (포인터) | 참조에 의한 호출 |
|:----|:------------|:---------------------|:-------------|
| **매개변수 선언** | `void f(int x)` | `void f(int *p)` | `void f(int &x)` |
| **호출 문법** | `f(a)` | `f(&a)` | `f(a)` |
| **원본 수정 가능?** | 불가 | 가능 (`*p`를 통해) | 가능 (직접) |
| **NULL 가능?** | 해당 없음 | 가능 (`nullptr`) | 불가 (유효한 객체를 참조해야 함) |
| **접근 문법** | `x` | `*p` | `x` |
| **언어** | C, C++ 공통 | C, C++ 공통 | **C++ 전용** |
| **적합한 상황** | 읽기 전용, 작은 타입 | 선택적 매개변수(NULL 가능), 배열 | 원본 수정, 큰 객체 |

---

<br>

## 2. 참조 변수

### 2.1 선언

참조 변수는 기존 변수에 대한 **별명(alias)**이다. 원본과 같은 메모리 주소를 공유한다.

**문법:**

```cpp
int original = 10;
int &ref = original;   // ref는 original의 별명
```

**규칙:**

| 규칙 | 설명 |
|:----|:----|
| 선언 시 반드시 초기화 | `int &ref;`만으로는 오류 |
| 재바인딩 불가 | 한 번 바인딩되면 `ref`는 항상 `original`을 참조 |
| 같은 메모리 주소 | `&ref == &original`은 `true` |
| `ref`를 통한 변경은 `original`에 반영 | 둘은 같은 변수 |

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int &ref = a;

    cout << "a = " << a << ", ref = " << ref << endl;       // 10, 10
    cout << "&a = " << &a << ", &ref = " << &ref << endl;   // 같은 주소

    ref = 20;
    cout << "a = " << a << endl;   // 20 (ref를 통해 a가 변경됨)

    a = 30;
    cout << "ref = " << ref << endl;   // 30 (a의 변경이 ref에 반영됨)

    return 0;
}
```

> **경고:** 참조는 재바인딩할 수 없다. 참조에 대한 `=` 연산자는 바인딩을 변경하는 것이 아니라 **값을 복사**한다.

```cpp
int a = 10, b = 50;
int &ref = a;   // ref는 a에 바인딩됨
ref = b;        // ref를 b에 재바인딩하는 것이 아님; b의 값을 a에 복사
// a는 이제 50, ref는 여전히 a를 참조
```

### 2.2 참조 매개변수

참조의 가장 흔한 용도이다. 함수의 매개변수가 호출자의 인수에 대한 별명이 된다.

```cpp
#include <iostream>
using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 10, y = 20;
    swap(x, y);
    cout << "x = " << x << ", y = " << y << endl;   // x = 20, y = 10
    return 0;
}
```

> **핵심:** 참조 매개변수를 사용하면 포인터 문법 없이도 호출자의 변수를 수정할 수 있다. C++에서 "출력" 매개변수에 선호되는 관용구이다.

### 2.3 참조 반환

함수가 변수에 대한 **참조**를 반환할 수 있다. 반환값이 **lvalue**가 되므로 대입문의 왼쪽에 올 수 있다.

```cpp
#include <iostream>
using namespace std;

char& charAt(char str[], int index) {
    return str[index];   // 요소에 대한 참조를 반환
}

int main() {
    char name[] = "Mike";
    cout << name << endl;          // Mike

    charAt(name, 0) = 'L';        // 반환값에 대입!
    cout << name << endl;          // Like

    char &ref = charAt(name, 2);
    ref = 'n';
    cout << name << endl;          // Line

    return 0;
}
```

> **경고:** **지역 변수**에 대한 참조를 반환하면 안 된다. 지역 변수는 함수 반환 시 소멸되므로 **댕글링 참조(dangling reference)**가 된다.

```cpp
int& bad() {
    int local = 10;
    return local;      // 위험: local은 반환 후 소멸됨
}

int main() {
    int &ref = bad();  // ref는 댕글링 참조 — 정의되지 않은 동작
    return 0;
}
```

---

<br>

## 3. 복사 생성자

### 3.1 개념

**복사 생성자**는 기존 객체를 복사하여 새로운 객체를 생성한다. 다음 세 가지 상황에서 호출된다:

| 상황 | 예시 |
|:----|:----|
| 다른 객체로 초기화 | `MyClass b = a;` 또는 `MyClass b(a);` |
| 객체를 값으로 전달 | `void f(MyClass obj);`를 `f(a);`로 호출 |
| 객체를 값으로 반환 | `MyClass f() { MyClass obj; return obj; }` |

**원형:**

```cpp
ClassName(const ClassName &other);
```

- 매개변수는 반드시 **const 참조**여야 한다. 값으로 전달하면 복사 생성자가 인수를 복사하기 위해 자기 자신을 호출하여 **무한 재귀**가 발생한다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r) : radius(r) {}
    Circle(const Circle &other) : radius(other.radius) {
        cout << "복사 생성자 호출됨" << endl;
    }
    void show() { cout << "반지름: " << radius << endl; }
};

int main() {
    Circle a(10);
    Circle b(a);       // 복사 생성자 호출됨
    Circle c = a;      // 복사 생성자 호출됨 (Circle c(a)와 동일)
    b.show();          // 반지름: 10
    c.show();          // 반지름: 10
    return 0;
}
```

### 3.2 디폴트 복사 생성자

복사 생성자를 정의하지 않으면 컴파일러가 자동으로 생성한다. **디폴트 복사 생성자**는 **멤버 단위 복사(member-by-member copy)**, 즉 **얕은 복사(shallow copy)**를 수행한다.

```cpp
class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) {}
    // 복사 생성자를 정의하지 않음 — 컴파일러가 제공
    void show() { cout << "(" << x << ", " << y << ")" << endl; }
};

int main() {
    Point a(3, 4);
    Point b = a;   // 디폴트 복사 생성자: b.x = a.x, b.y = a.y
    b.show();      // (3, 4)
    return 0;
}
```

> **핵심:** 기본 타입 멤버만 있는 클래스에서는 디폴트 복사 생성자가 올바르게 동작한다. 문제는 클래스에 **포인터 멤버**가 있을 때 발생한다.

### 3.3 얕은 복사의 문제

클래스에 **포인터 멤버**가 있으면, 디폴트 복사 생성자는 포인터가 가리키는 데이터가 아니라 **포인터 값(주소)**을 복사한다. 두 객체가 **같은 메모리**를 가리키게 된다.

```cpp
class Book {
    char *title;
    int price;
public:
    Book(const char *t, int p) {
        title = new char[strlen(t) + 1];
        strcpy(title, t);
        price = p;
    }
    ~Book() {
        delete[] title;   // 동적 메모리 해제
    }
};
```

```
얕은 복사 (디폴트 복사 생성자):

Book cpp("C++", 10000);     Book java = cpp;

  cpp                         java
┌──────────┐               ┌──────────┐
│ title ───┼──────┐        │ title ───┼──────┐
│ price=10000│     │        │ price=10000│     │
└──────────┘     │        └──────────┘     │
                  ▼                          ▼
              ┌──────────┐  ← 같은 메모리!
              │ "C++"    │
              └──────────┘

java 소멸 → delete[] title → 메모리 해제
cpp  소멸 → delete[] title → 이중 해제! (크래시)
```

**얕은 복사의 문제점:**

| 문제 | 설명 |
|:----|:----|
| 메모리 공유 | 두 객체가 같은 데이터를 수정 — 의도하지 않은 부작용 |
| 이중 해제 (double free) | 두 소멸자가 같은 메모리를 `delete[]` — 크래시 발생 |
| 댕글링 포인터 | 한 객체가 소멸된 후 다른 객체는 해제된 메모리를 가리킴 |

### 3.4 깊은 복사

얕은 복사 문제를 해결하려면 **새로운 메모리를 할당**하고 **내용을 복사**하는 **사용자 정의 복사 생성자**를 작성해야 한다.

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char *title;
    int price;
public:
    Book(const char *t, int p) {
        title = new char[strlen(t) + 1];
        strcpy(title, t);
        price = p;
    }

    // 깊은 복사 생성자
    Book(const Book &other) {
        title = new char[strlen(other.title) + 1];   // 새로운 메모리 할당
        strcpy(title, other.title);                   // 내용 복사
        price = other.price;
    }

    ~Book() {
        delete[] title;
    }

    void show() {
        cout << title << " " << price << "원" << endl;
    }
};

int main() {
    Book cpp("C++ 가이드", 10000);
    Book java = cpp;   // 깊은 복사 생성자 호출
    cpp.show();        // C++ 가이드 10000원
    java.show();       // C++ 가이드 10000원 — 독립적인 복사본
    return 0;          // 두 소멸자 모두 안전하게 실행
}
```

```
깊은 복사 (사용자 정의 복사 생성자):

Book cpp("C++", 10000);     Book java = cpp;

  cpp                         java
┌──────────┐               ┌──────────┐
│ title ───┼──┐            │ title ───┼──┐
│ price=10000│ │            │ price=10000│ │
└──────────┘ │            └──────────┘ │
              ▼                          ▼
          ┌──────────┐             ┌──────────┐
          │ "C++"    │             │ "C++"    │  ← 별도의 메모리
          └──────────┘             └──────────┘

각 소멸자가 자신의 메모리를 해제 — 충돌 없음.
```

> **경험 법칙:** 클래스가 동적 메모리를 할당(`new` 사용)한다면 반드시 다음을 구현해야 한다:
> 1. **복사 생성자** (깊은 복사)
> 2. **소멸자** (메모리 `delete`)
> 3. **대입 연산자** (복사 생성자와 같은 이유)
>
> 이것을 **3의 법칙(Rule of Three)**이라 한다.

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 값에 의한 호출 | 함수가 복사본을 받음; 원본 변경 불가; C와 동일 |
| 주소에 의한 호출 (포인터) | 함수가 포인터를 받음; `*`로 원본 수정; 호출자가 `&` 전달; C와 동일 |
| 참조에 의한 호출 | C++ 전용; 함수가 참조 `&`를 받음; 원본의 별명; 포인터보다 깔끔한 문법 |
| 참조 변수 | `int &ref = a;` — 별명, 같은 주소; 반드시 초기화; 재바인딩 불가 |
| 참조 매개변수 | `void f(int &x)` — 가장 흔한 용도; 포인터 문법 없이 호출자 변수 수정 |
| 참조 반환 | `char& at(int i)` — 반환값이 lvalue; 대입 가능; 지역 변수 참조 반환 금지 |
| 복사 생성자 | `ClassName(const ClassName &other)`; 객체 초기화, 값 전달, 값 반환 시 호출 |
| 디폴트 복사 생성자 | 컴파일러가 생성; 멤버 단위 얕은 복사; 기본 타입 멤버만 있을 때 정상 동작 |
| 얕은 복사 문제 | 포인터 멤버가 같은 메모리를 공유; 이중 해제 크래시; 댕글링 포인터 |
| 깊은 복사 | 사용자 정의 복사 생성자; 새 메모리를 할당하고 내용 복사; `new`를 사용하는 클래스에 필수 |
| 3의 법칙 | 사용자 정의 소멸자, 복사 생성자, 대입 연산자 중 하나가 필요하면 셋 다 필요할 가능성이 높음 |

---
