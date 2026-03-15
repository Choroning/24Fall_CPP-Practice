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
  - [1.4 C에서 C++로: 포인터 vs 참조](#14-c에서-c로-포인터-vs-참조)
  - [1.5 왜 참조인가? 동기와 가이드라인](#15-왜-참조인가-동기와-가이드라인)
  - [1.6 전체 swap 예제: 세 가지 방법 비교](#16-전체-swap-예제-세-가지-방법-비교)
  - [1.7 비교 표](#17-비교-표)
- [2. 참조 변수](#2-참조-변수)
  - [2.1 선언](#21-선언)
  - [2.2 참조 매개변수](#22-참조-매개변수)
  - [2.3 참조 반환](#23-참조-반환)
  - [2.4 참조 반환 전체 예제](#24-참조-반환-전체-예제)
- [3. 복사 생성자](#3-복사-생성자)
  - [3.1 개념](#31-개념)
  - [3.2 디폴트 복사 생성자](#32-디폴트-복사-생성자)
  - [3.3 얕은 복사의 문제](#33-얕은-복사의-문제)
  - [3.4 얕은 복사 vs 깊은 복사: 메모리 시각화](#34-얕은-복사-vs-깊은-복사-메모리-시각화)
  - [3.5 깊은 복사](#35-깊은-복사)
  - [3.6 깊은 복사 전체 예제: Person 클래스](#36-깊은-복사-전체-예제-person-클래스)
- [4. 흔한 실수](#4-흔한-실수)
  - [4.1 지역 변수에 대한 참조 반환](#41-지역-변수에-대한-참조-반환)
  - [4.2 포인터 멤버가 있는데 깊은 복사를 잊는 경우](#42-포인터-멤버가-있는데-깊은-복사를-잊는-경우)
  - [4.3 복사 생성자의 매개변수는 반드시 const 참조](#43-복사-생성자의-매개변수는-반드시-const-참조)
  - [4.4 참조 선언과 주소 연산자의 구분](#44-참조-선언과-주소-연산자의-구분)
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
│  x = 10 → 11 │  ← 지역 복사본만 변경
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
│  x ≡ a        │  ← x는 a 그 자체 (같은 메모리)
│  x = 10 → 11  │
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

### 1.4 C에서 C++로: 포인터 vs 참조

C 언어에서는 호출자의 변수를 수정하려면 **포인터를 사용할 수밖에 없었다**. 다른 방법이 없었다. C++은 같은 목적을 **더 깔끔하게** 달성할 수 있는 **참조**를 도입했다.

**동일한 swap 함수 -- C (포인터) vs C++ (참조) 나란히 비교:**

```cpp
/* ===== C 버전 (포인터) ===== */              /* ===== C++ 버전 (참조) ===== */

#include <stdio.h>                            #include <iostream>
                                              using namespace std;

void swap(int *a, int *b) {                   void swap(int &a, int &b) {
    int temp = *a;    // 역참조                int temp = a;     // 직접 접근
    *a = *b;          // 역참조                a = b;            // 직접 접근
    *b = temp;        // 역참조                b = temp;         // 직접 접근
}                                             }

int main() {                                  int main() {
    int x = 10, y = 20;                          int x = 10, y = 20;
    swap(&x, &y);    // 주소 전달                 swap(x, y);      // 변수만 전달
    printf("%d %d\n", x, y);                      cout << x << " " << y << endl;
    return 0;                                     return 0;
}                                             }
```

**핵심 차이점:**

| 측면 | C (포인터) | C++ (참조) |
|:----|:----------|:----------|
| 매개변수 선언 | `int *a` | `int &a` |
| 호출 문법 | `swap(&x, &y)` | `swap(x, y)` |
| 함수 내부 접근 | `*a`, `*b` (역참조 필요) | `a`, `b` (직접, 일반 변수처럼) |
| NULL 위험 | 있음 (`swap(NULL, &y)` 컴파일됨) | 없음 (참조는 유효한 객체에 바인딩) |
| 가독성 | `*` 기호가 여기저기 산재 | 깔끔, 값에 의한 호출처럼 보임 |

> **요점:** C에서 왔다면 참조를 "자동으로 역참조되고 절대 NULL이 될 수 없는 포인터"로 생각하면 된다. C++에서 "원본을 수정하고 싶다"를 표현하는 방법이다.

### 1.5 왜 참조인가? 동기와 가이드라인

**포인터가 이미 있는데 C++은 왜 참조를 추가했는가?**

참조는 포인터보다 **안전**하고 **간결**하다:

| 속성 | 포인터 | 참조 |
|:----|:------|:----|
| NULL/nullptr 가능? | 가능 -- 사용 전 반드시 검사 | **불가** -- 항상 유효한 객체를 참조 |
| 다른 곳으로 재지정 가능? | 가능 -- 실수로 재바인딩 위험 | **불가** -- 바인딩이 영구적 |
| `*`로 값 접근 필요? | 필요 -- 깜빡하면 버그 | **불필요** -- 일반 변수처럼 사용 |
| 연산 가능? | 가능 (`p++`, `p+3`) -- 강력하지만 위험 | **불가** -- 의도치 않은 메모리 이동 방지 |
| 초기화 안 한 상태? | 가능 (`int *p;`는 와일드 포인터) | **불가** -- 선언 시 반드시 초기화 |

**언제 무엇을 사용할 것인가:**

| 상황 | 권장 |
|:----|:-----|
| 호출자의 변수를 수정 | **참조** -- 더 깔끔하고 안전 |
| 큰 객체를 전달 (읽기 전용) | **const 참조** (`const MyClass &obj`) |
| 선택적 매개변수 (없을 수 있음) | **포인터** -- `nullptr`로 "없음"을 표현 가능 |
| 다른 객체로 재지정 필요 | **포인터** -- 참조는 재바인딩 불가 |
| 배열 순회나 포인터 산술 | **포인터** -- 참조에는 산술 연산 없음 |
| 소유권 이전 / 동적 메모리 | **포인터** (또는 모던 C++의 스마트 포인터) |
| 대부분의 경우 기본 선택 | **참조** |

### 1.6 전체 swap 예제: 세 가지 방법 비교

세 가지 매개변수 전달 방식을 모두 비교하는 완전한 실행 가능 프로그램:

```cpp
#include <iostream>
using namespace std;

// 방법 1: 값에 의한 호출 — 원본을 교환하지 않음
void swapByValue(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    cout << "  swapByValue 내부: a=" << a << ", b=" << b << endl;
}

// 방법 2: 포인터에 의한 호출 — 역참조를 통해 교환
void swapByPointer(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    cout << "  swapByPointer 내부: *a=" << *a << ", *b=" << *b << endl;
}

// 방법 3: 참조에 의한 호출 — 직접 교환
void swapByReference(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
    cout << "  swapByReference 내부: a=" << a << ", b=" << b << endl;
}

int main() {
    int x, y;

    // 테스트 1: 값에 의한 호출
    x = 10; y = 20;
    cout << "swapByValue 호출 전:     x=" << x << ", y=" << y << endl;
    swapByValue(x, y);
    cout << "swapByValue 호출 후:     x=" << x << ", y=" << y << endl;
    cout << endl;

    // 테스트 2: 포인터에 의한 호출
    x = 10; y = 20;
    cout << "swapByPointer 호출 전:   x=" << x << ", y=" << y << endl;
    swapByPointer(&x, &y);
    cout << "swapByPointer 호출 후:   x=" << x << ", y=" << y << endl;
    cout << endl;

    // 테스트 3: 참조에 의한 호출
    x = 10; y = 20;
    cout << "swapByReference 호출 전: x=" << x << ", y=" << y << endl;
    swapByReference(x, y);
    cout << "swapByReference 호출 후: x=" << x << ", y=" << y << endl;

    return 0;
}
```

```
출력:
swapByValue 호출 전:     x=10, y=20
  swapByValue 내부: a=20, b=10
swapByValue 호출 후:     x=10, y=20        ← 교환되지 않음! (복사본이 변경됨)

swapByPointer 호출 전:   x=10, y=20
  swapByPointer 내부: *a=20, *b=10
swapByPointer 호출 후:   x=20, y=10        ← 포인터를 통해 교환됨

swapByReference 호출 전: x=10, y=20
  swapByReference 내부: a=20, b=10
swapByReference 호출 후: x=20, y=10        ← 참조를 통해 교환됨 (가장 깔끔)
```

### 1.7 비교 표

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

### 2.4 참조 반환 전체 예제

참조 반환을 보여주는 완전한 실행 가능 프로그램이다. 함수가 배열 요소에 대한 참조를 반환하고, 호출자가 반환된 참조를 통해 원본 배열을 수정한다.

```cpp
#include <iostream>
using namespace std;

int scores[] = {90, 85, 78, 92, 88};  // 5개 점수의 전역 배열

// 주어진 인덱스의 요소에 대한 참조를 반환
int& getScore(int index) {
    return scores[index];
}

void printScores() {
    cout << "점수: ";
    for (int i = 0; i < 5; i++) {
        cout << scores[i];
        if (i < 4) cout << ", ";
    }
    cout << endl;
}

int main() {
    printScores();                 // 점수: 90, 85, 78, 92, 88

    // 반환값을 lvalue로 사용 — 반환된 참조를 통해 배열 수정
    getScore(1) = 100;            // scores[1]이 85에서 100으로 변경
    printScores();                 // 점수: 90, 100, 78, 92, 88

    // 반환된 참조를 참조 변수에 저장
    int &ref = getScore(3);       // ref는 scores[3]의 별명
    cout << "ref = " << ref << endl;   // ref = 92

    ref = 50;                     // ref를 수정하면 scores[3]이 수정됨
    printScores();                 // 점수: 90, 100, 78, 50, 88

    // 증가 연산자와 함께 사용
    getScore(0) += 5;             // scores[0]이 90에서 95로 변경
    printScores();                 // 점수: 95, 100, 78, 50, 88

    return 0;
}
```

```
출력:
점수: 90, 85, 78, 92, 88
점수: 90, 100, 78, 92, 88
ref = 92
점수: 90, 100, 78, 50, 88
점수: 95, 100, 78, 50, 88
```

> **왜 유용한가?** 참조 반환을 사용하면 `array[i] = value;`와 같은 깔끔한 API를 만들 수 있다. `std::vector`와 `std::string`의 `operator[]`가 내부적으로 정확히 이 방식으로 동작한다.

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

Book cpp("C++", 10000);      Book java = cpp;

 cpp                          java
┌─────────────┐              ┌─────────────┐
│ title ──────┼─────┐        │ title ──────┼──────┐
│ price=10000 │     │        │ price=10000 │      │
└─────────────┘     │        └─────────────┘      │
                    ▼                             ▼
              ┌──────────┐  <──────────────── 같은 메모리!
              │  "C++"   │
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

### 3.4 얕은 복사 vs 깊은 복사: 메모리 시각화

다음 ASCII 다이어그램은 포인터 멤버를 가진 객체를 복사하는 전체 과정을 추적한다.

**1단계 -- 복사 전: 원본 객체가 존재**

```
  스택 (STACK)                     힙 (HEAP)
┌─────────────────┐
│  original       │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────────>│ "Alice"         │
│ │ age = 25    │ │         │ (6 바이트)        │
│ └─────────────┘ │         └─────────────────┘
└─────────────────┘              주소: 0x1000
```

**2a단계 -- 얕은 복사 후 (디폴트 복사 생성자): 위험**

```
  스택 (STACK)                     힙 (HEAP)
┌─────────────────┐
│  original       │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────┐    │ "Alice"         │
│ │ age = 25    │ │    ├───>│ (6 바이트)        │  ← 공유! 둘 다 여기를 가리킴!
│ └─────────────┘ │    │    └─────────────────┘
└─────────────────┘    │         주소: 0x1000
                       │
┌─────────────────┐    │
│  copy           │    │
│ ┌─────────────┐ │    │
│ │ name ───────┼─┼────┘
│ │ age = 25    │ │
│ └─────────────┘ │
└─────────────────┘

문제 1: copy.name[0] = 'B';  →  original도 "Blice"로 변경!
문제 2: ~copy()가 delete[] 0x1000 실행
         ~original()이 delete[] 0x1000 다시 실행  →  크래시 (이중 해제)
```

**2b단계 -- 깊은 복사 후 (사용자 정의 복사 생성자): 안전**

```
  스택 (STACK)                     힙 (HEAP)
┌─────────────────┐
│  original       │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────────>│ "Alice"         │
│ │ age = 25    │ │         │ (6 바이트)        │
│ └─────────────┘ │         └─────────────────┘
└─────────────────┘              주소: 0x1000

┌─────────────────┐
│  copy           │
│ ┌─────────────┐ │         ┌─────────────────┐
│ │ name ───────┼─┼────────>│ "Alice"         │  ← 별도 할당
│ │ age = 25    │ │         │ (6 바이트)        │
│ └─────────────┘ │         └─────────────────┘
└─────────────────┘              주소: 0x2000

copy.name[0] = 'B';  →  copy는 "Blice", original은 여전히 "Alice"
~copy()가 delete[] 0x2000 실행     ← 안전
~original()이 delete[] 0x1000 실행 ← 안전, 다른 메모리
```

### 3.5 깊은 복사

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

Book cpp("C++", 10000);       Book java = cpp;

 cpp                           java
┌─────────────┐               ┌─────────────┐
│ title ──────┼──┐            │ title ──────┼──┐
│ price=10000 │  │            │ price=10000 │  │
└─────────────┘  │            └─────────────┘  │
                 ▼                             ▼
            ┌──────────┐                  ┌──────────┐
            │  "C++"   │                  │  "C++"   │ ← 별도의 메모리
            └──────────┘                  └──────────┘

각 소멸자가 자신의 메모리를 해제 — 충돌 없음.
```

> **경험 법칙:** 클래스가 동적 메모리를 할당(`new` 사용)한다면 반드시 다음을 구현해야 한다:
> 1. **복사 생성자** (깊은 복사)
> 2. **소멸자** (메모리 `delete`)
> 3. **대입 연산자** (복사 생성자와 같은 이유)
>
> 이것을 **3의 법칙(Rule of Three)**이라 한다.

### 3.6 깊은 복사 전체 예제: Person 클래스

동적으로 할당된 `name` (`char*`)을 가진 `Person` 클래스의 완전한 실행 가능 프로그램이다. 얕은 복사에서 무엇이 잘못되는지, 깊은 복사가 어떻게 해결하는지를 출력 추적과 함께 보여준다.

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Person {
    char *name;
    int age;
public:
    // 생성자
    Person(const char *n, int a) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        age = a;
        cout << "  [생성자] 생성: " << name << " (나이 " << age << ")" << endl;
        cout << "    name 할당 주소: " << (void*)name << endl;
    }

    // 깊은 복사 생성자
    Person(const Person &other) {
        name = new char[strlen(other.name) + 1];  // 새로운 메모리 할당
        strcpy(name, other.name);                   // 내용 복사
        age = other.age;
        cout << "  [복사 생성자] 복사: " << name << " (나이 " << age << ")" << endl;
        cout << "    원본 name 주소: " << (void*)other.name << endl;
        cout << "    복사본 name 주소: " << (void*)name << endl;
    }

    // 소멸자
    ~Person() {
        cout << "  [소멸자] 삭제: " << name
             << " 주소 " << (void*)name << endl;
        delete[] name;
    }

    // 이름 수정 — 독립성 확인용
    void setName(const char *n) {
        delete[] name;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    void show() const {
        cout << "  " << name << " (나이 " << age << ")" << endl;
    }
};

int main() {
    cout << "=== alice 생성 ===" << endl;
    Person alice("Alice", 25);

    cout << endl << "=== alice를 복사하여 bob 생성 ===" << endl;
    Person bob = alice;   // 깊은 복사 생성자 호출

    cout << endl << "=== 복사 후 ===" << endl;
    alice.show();         // Alice (나이 25)
    bob.show();           // Alice (나이 25)

    cout << endl << "=== bob의 이름을 Bob으로 변경 ===" << endl;
    bob.setName("Bob");
    alice.show();         // Alice (나이 25) — 변경 없음! (깊은 복사 성공)
    bob.show();           // Bob (나이 25)

    cout << endl << "=== 소멸자 실행 ===" << endl;
    return 0;
}
```

```
출력:
=== alice 생성 ===
  [생성자] 생성: Alice (나이 25)
    name 할당 주소: 0x1000          (주소는 실행 시마다 다름)

=== alice를 복사하여 bob 생성 ===
  [복사 생성자] 복사: Alice (나이 25)
    원본 name 주소: 0x1000
    복사본 name 주소: 0x2000           ← 다른 주소 (깊은 복사!)

=== 복사 후 ===
  Alice (나이 25)
  Alice (나이 25)

=== bob의 이름을 Bob으로 변경 ===
  Alice (나이 25)                       ← alice는 영향 없음
  Bob (나이 25)

=== 소멸자 실행 ===
  [소멸자] 삭제: Bob 주소 0x2080     ← bob 고유의 메모리
  [소멸자] 삭제: Alice 주소 0x1000   ← alice 고유의 메모리 (크래시 없음!)
```

> **깊은 복사 생성자를 제거(주석 처리)하고** 다시 실행해 보라. 디폴트 얕은 복사를 사용하면 `bob`의 이름을 변경할 때 `alice`도 함께 망가지고, 소멸 시 이중 해제로 프로그램이 크래시된다.

---

<br>

## 4. 흔한 실수

### 4.1 지역 변수에 대한 참조 반환

함수가 지역 변수에 대한 참조를 반환하면, 변수는 함수 종료 시 소멸된다. 호출자는 **댕글링 참조**를 받게 되며, 이를 사용하면 **정의되지 않은 동작(undefined behavior)**이 발생한다.

```cpp
// 버그: 댕글링 참조
int& createValue() {
    int x = 42;
    return x;       // x는 여기서 소멸됨!
}

int main() {
    int &ref = createValue();
    cout << ref << endl;   // 정의되지 않은 동작! 42, 쓰레기값, 또는 크래시.
    return 0;
}
```

**해결:** 값으로 반환하거나, 함수보다 오래 사는 것(클래스 멤버, 전역 변수, static 변수, 힙 할당 데이터)에 대한 참조를 반환한다.

```cpp
// OK: static 변수에 대한 참조 반환
int& createValue() {
    static int x = 42;   // static — 프로그램 종료까지 존재
    return x;
}

// OK: 값으로 반환 (단순 타입에는 이것으로 충분)
int createValueByVal() {
    int x = 42;
    return x;   // 복사본을 반환 — 안전
}
```

### 4.2 포인터 멤버가 있는데 깊은 복사를 잊는 경우

클래스에 포인터 멤버가 있는데 복사 생성자를 구현하지 않으면, 컴파일러의 디폴트 얕은 복사가 소멸 시 크래시를 유발한다.

```cpp
class Buffer {
    int *data;
    int size;
public:
    Buffer(int s) : size(s) {
        data = new int[size];
    }
    ~Buffer() {
        delete[] data;   // 두 객체가 같은 메모리를 삭제하려 함!
    }
    // 버그: 복사 생성자 미정의 → 디폴트 얕은 복사
};

int main() {
    Buffer a(10);
    Buffer b = a;    // 얕은 복사: b.data == a.data (같은 포인터!)
    return 0;        // ~b()가 data 삭제, ~a()가 같은 data 삭제 → 크래시
}
```

**규칙:** 생성자에서 `new`를 사용하고 소멸자에서 `delete`를 사용한다면, **반드시** 복사 생성자(와 대입 연산자)를 작성해야 한다. 이것이 3의 법칙이다.

### 4.3 복사 생성자의 매개변수는 반드시 const 참조

복사 생성자는 매개변수를 **반드시** **const 참조**로 받아야 한다. 값으로 받으려고 하면 어떻게 되는지 보자:

```cpp
class Bad {
    int x;
public:
    Bad(int v) : x(v) {}

    // 잘못됨: 매개변수를 값으로 전달
    Bad(Bad other) {     // ← 이것을 호출하려면 컴파일러가 'other'를 복사해야 하는데...
        x = other.x;     //    그러면 이 복사 생성자를 다시 호출하고...
    }                     //    또 복사가 필요하고... → 무한 재귀
};
```

컴파일러는 `Bad(Bad other)`를 오류로 거부한다. 매개변수는 반드시:

```cpp
Bad(const Bad &other) {   // const 참조 — 호출 시 복사 불필요
    x = other.x;
}
```

**왜 `const`인가?** 두 가지 이유:
1. 원본 객체를 수정하지 않겠다고 약속한다 (복사 *원본*이므로 당연하다).
2. `const` 객체와 임시 객체로부터의 복사를 허용한다.

### 4.4 참조 선언과 주소 연산자의 구분

`&` 기호는 문맥에 따라 **두 가지 다른 의미**를 가진다. 많은 초보자가 이것을 혼동한다.

```cpp
int a = 10;

// 문맥 1: 선언 — &는 "~에 대한 참조"
int &ref = a;      // ref는 a에 대한 참조(별명)

// 문맥 2: 표현식 — &는 "~의 주소"
int *ptr = &a;     // ptr은 a의 주소를 저장
```

**구분하는 방법:**

| 문맥 | 문법 | 의미 | 예시 |
|:----|:-----|:----|:----|
| 타입 선언 시 | `Type &name = ...;` | 참조 (별명) | `int &ref = a;` |
| 매개변수 목록에서 | `void f(Type &param)` | 참조 매개변수 | `void swap(int &a, int &b)` |
| 표현식에서 | `&variable` | 주소 연산자 | `int *p = &a;` |
| 표현식에서 | `*pointer` | 역참조 연산자 | `cout << *p;` |

**간단한 규칙:** `&`가 **타입 이름 옆에** (선언에서) 나타나면 "참조"이다. `&`가 **변수 이름 앞에** (표현식에서) 나타나면 "주소"이다.

```cpp
int a = 10;
int &ref = a;       // 참조 선언 — ref는 a 그 자체
int *ptr = &a;      // 주소 연산 — ptr은 a의 주소를 가짐

// 비슷해 보이지만 근본적으로 다르다:
ref = 20;           // a를 20으로 변경 (ref는 별명)
*ptr = 30;          // a를 30으로 변경 (ptr을 역참조)
cout << &ref;       // a의 주소 출력 (별명의 주소 = a의 주소)
cout << ptr;        // a의 주소 출력 (ptr이 주소를 가짐)
```

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 값에 의한 호출 | 함수가 복사본을 받음; 원본 변경 불가; C와 동일 |
| 주소에 의한 호출 (포인터) | 함수가 포인터를 받음; `*`로 원본 수정; 호출자가 `&` 전달; C와 동일 |
| 참조에 의한 호출 | C++ 전용; 함수가 참조 `&`를 받음; 원본의 별명; 포인터보다 깔끔한 문법 |
| C에서 C++로 전환 | C에서는 호출자 변수 수정에 포인터가 유일한 방법; C++ 참조가 더 깔끔한 대안 |
| 왜 참조인가? | NULL 불가, 재바인딩 불가, `*` 문법 불필요; 기본적으로 참조 사용, nullptr/재지정 필요시에만 포인터 |
| 참조 변수 | `int &ref = a;` — 별명, 같은 주소; 반드시 초기화; 재바인딩 불가 |
| 참조 매개변수 | `void f(int &x)` — 가장 흔한 용도; 포인터 문법 없이 호출자 변수 수정 |
| 참조 반환 | `char& at(int i)` — 반환값이 lvalue; 대입 가능; 지역 변수 참조 반환 금지 |
| 복사 생성자 | `ClassName(const ClassName &other)`; 객체 초기화, 값 전달, 값 반환 시 호출 |
| 디폴트 복사 생성자 | 컴파일러가 생성; 멤버 단위 얕은 복사; 기본 타입 멤버만 있을 때 정상 동작 |
| 얕은 복사 문제 | 포인터 멤버가 같은 메모리를 공유; 이중 해제 크래시; 댕글링 포인터 |
| 깊은 복사 | 사용자 정의 복사 생성자; 새 메모리를 할당하고 내용 복사; `new`를 사용하는 클래스에 필수 |
| 3의 법칙 | 사용자 정의 소멸자, 복사 생성자, 대입 연산자 중 하나가 필요하면 셋 다 필요할 가능성이 높음 |
| 댕글링 참조 | 지역 변수에 대한 참조를 반환하면 안 됨; 함수 반환 시 소멸 |
| 복사 생성자는 `const &` 필수 | 값으로 전달하면 무한 재귀; `const`는 const 객체로부터의 복사를 허용 |
| `&`의 이중 의미 | 선언에서 = 참조; 표현식에서 = 주소 연산; 문맥이 의미를 결정 |

---
