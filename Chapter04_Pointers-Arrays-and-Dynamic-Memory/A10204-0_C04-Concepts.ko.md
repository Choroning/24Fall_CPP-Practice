# Chapter 04 — 포인터, 배열, 동적 메모리

> **최종 수정일:** 2026-03-16

---

## 목차

- [1. 객체 포인터](#1-객체-포인터)
  - [1.1 객체에 대한 포인터](#11-객체에-대한-포인터)
  - [1.2 this 포인터](#12-this-포인터)
- [2. 동적 메모리](#2-동적-메모리)
  - [2.1 new와 delete](#21-new와-delete)
  - [2.2 동적 객체](#22-동적-객체)
  - [2.3 동적 배열](#23-동적-배열)
- [3. 객체 배열](#3-객체-배열)
  - [3.1 정적 객체 배열](#31-정적-객체-배열)
  - [3.2 동적 객체 배열](#32-동적-객체-배열)
- [4. 메모리 관리](#4-메모리-관리)
  - [4.1 메모리 누수](#41-메모리-누수)
  - [4.2 댕글링 포인터](#42-댕글링-포인터)
  - [4.3 소멸자의 책임](#43-소멸자의-책임)
- [5. 예외 처리](#5-예외-처리)
- [6. 스마트 포인터 (현대 C++)](#6-스마트-포인터-현대-c)
  - [6.1 RAII 개념](#61-raii-개념)
  - [6.2 `std::unique_ptr` — 단독 소유](#62-stdunique_ptr--단독-소유)
  - [6.3 `std::shared_ptr` — 공유 소유](#63-stdshared_ptr--공유-소유)
  - [6.4 `make_unique`와 `make_shared`](#64-make_unique와-make_shared)
  - [6.5 어떤 것을 사용할까](#65-어떤-것을-사용할까)
- [요약](#요약)

---

<br>

## 1. 객체 포인터

### 1.1 객체에 대한 포인터

포인터는 객체의 주소를 저장할 수 있다. 포인터를 통해 멤버에 접근할 때는 **화살표 연산자** (`->`)를 사용하거나, `*`로 역참조한 후 점 연산자를 사용한다.

```cpp
Circle c(10);
Circle *p = &c;

p->getArea();      // 화살표 연산자
(*p).getArea();    // 역참조 + 점 연산자 (결과 동일)
```

```
스택
┌──────────┐      ┌──────────────┐
│  p ──────┼─────►│  Circle c    │
│ (포인터)   │      │  radius = 10 │
└──────────┘      └──────────────┘
```

> **핵심:** `p->member`는 `(*p).member`의 축약형이다. 가독성을 위해 화살표 연산자를 주로 사용한다.

### 1.2 this 포인터

모든 비정적(non-static) 멤버 함수에는 **현재 객체**(해당 함수가 호출된 객체)를 가리키는 암시적 포인터 `this`가 존재한다.

```cpp
class Circle {
    int radius;
public:
    Circle(int radius) {
        this->radius = radius;   // 매개변수와 멤버 변수 이름이 같을 때 구분
    }
};
```

`this`의 주요 용도:

- **이름 충돌 해소** — 매개변수 이름과 멤버 변수 이름이 같을 때 구분.
- **현재 객체 반환** — 멤버 함수에서 `return *this;`로 자기 자신을 반환.

```cpp
class Counter {
    int count;
public:
    Counter(int count) {
        this->count = count;       // this->count = 멤버, count = 매개변수
    }
    Counter& increment() {
        count++;
        return *this;              // 현재 객체 반환
    }
};
```

> **참고:** `this`는 포인터이므로 `this->member`로 접근한다 (`this.member`가 아님). `ClassName` 메서드 내부에서 `this`의 타입은 `ClassName*`이다.

---

<br>

## 2. 동적 메모리

### 2.1 new와 delete

C++에서는 `new`로 **힙(heap)** 에 메모리를 할당하고, `delete`로 해제한다. 스택에 할당된 변수와 달리, 힙 메모리는 명시적으로 해제할 때까지 유지된다.

```cpp
int *p = new int;        // 힙에 int 하나 할당
*p = 42;
delete p;                // 해제

int *arr = new int[100]; // int 100개 배열 할당
arr[0] = 1;
delete[] arr;            // 배열 해제 (delete가 아닌 delete[] 사용!)
```

| C (malloc/free) | C++ (new/delete) |
|:----------------|:-----------------|
| `int *p = (int*)malloc(sizeof(int));` | `int *p = new int;` |
| `free(p);` | `delete p;` |
| `void*` 반환, 캐스팅 필요 | 올바른 타입 반환 |
| 생성자 호출 안 함 | 생성자 호출 |
| 소멸자 호출 안 함 | 소멸자 호출 |

> **핵심:** C++에서는 `malloc`/`free` 대신 `new`/`delete`를 사용한다. `new`는 생성자를, `delete`는 소멸자를 호출하지만, `malloc`과 `free`는 둘 다 호출하지 않는다.

> **참고 (현대 C++):** 직접 `new`/`delete`를 사용하는 것은 현대 C++에서 **레거시 스타일** 로 간주된다. C++11부터 **스마트 포인터**(`std::unique_ptr`, `std::shared_ptr`)가 동적 메모리를 관리하는 권장 방법이다. 스마트 포인터는 더 이상 필요하지 않을 때 자동으로 메모리를 해제하여, 메모리 누수, 댕글링 포인터, 이중 해제 등의 버그를 원천적으로 방지한다. 자세한 내용은 아래 [6. 스마트 포인터 (현대 C++)](#6-스마트-포인터-현대-c) 섹션을 참조한다.

### 2.2 동적 객체

객체를 힙에 동적으로 생성할 수 있다. `new`가 생성자를 호출하고, `delete`가 소멸자를 호출한다.

```cpp
Circle *p = new Circle(10);   // 힙 할당 + 생성자 Circle(10) 호출
cout << p->getArea();          // 화살표 연산자로 접근
delete p;                      // 소멸자 호출 + 메모리 해제
```

```
스택                  힙
┌──────────┐         ┌──────────────┐
│  p ──────┼────────►│  Circle      │
│ (포인터)   │         │  radius = 10 │
└──────────┘         └──────────────┘
                    ↑ new로 할당 / delete로 해제
```

### 2.3 동적 배열

`new[]`로 힙에 배열을 할당하고, `delete[]`로 해제한다.

**기본 타입 배열:**

```cpp
int *arr = new int[5];     // 초기화되지 않은 int 5개
arr[0] = 10;
arr[1] = 20;
delete[] arr;              // 전체 배열 해제
```

**객체 배열:**

```cpp
Circle *circles = new Circle[3];   // 기본 생성자 3번 호출
circles[0].setRadius(1);
circles[1].setRadius(2);
circles[2].setRadius(3);
delete[] circles;                  // 소멸자 3번 호출 후 메모리 해제
```

> **경고:** `new`는 `delete`와, `new[]`는 `delete[]`와 반드시 짝을 맞춰야 한다. `new[]`로 할당한 메모리에 `delete`를 사용하면 **정의되지 않은 동작** 이 발생한다.

---

<br>

## 3. 객체 배열

### 3.1 정적 객체 배열

정적(스택) 객체 배열은 각 원소에 대해 **기본 생성자** 를 호출한다.

```cpp
Circle arr[3];    // 3개 원소 각각에 기본 생성자 호출
```

매개변수가 있는 생성자를 호출하려면 **초기화 리스트** 를 사용한다:

```cpp
Circle arr[3] = { Circle(1), Circle(2), Circle(3) };
```

배열의 각 원소는 독립적인 객체이다:

```cpp
arr[0].setRadius(10);   // arr[0]만 영향을 받음
cout << arr[1].getArea();
```

### 3.2 동적 객체 배열

동적 객체 배열은 `new[]`로 할당한다. **기본 생성자만** 호출할 수 있으며, 할당 시 개별 원소에 인자를 전달할 수 없다.

```cpp
Circle *p = new Circle[4];    // 기본 생성자 4번 호출
p[0].setRadius(5);            // 첨자 연산자로 접근
p[1].setRadius(10);
delete[] p;                   // 소멸자 4번 호출
```

> **핵심:** `new ClassName[n]`은 클래스에 **기본 생성자** 가 있어야 한다 (명시적으로 정의하거나 컴파일러가 제공). 매개변수가 있는 생성자만 존재하면 컴파일 오류가 발생한다.

---

<br>

## 4. 메모리 관리

### 4.1 메모리 누수

**메모리 누수(memory leak)** 는 동적으로 할당된 메모리를 해제하지 않을 때 발생한다. 할당된 메모리를 가리키는 포인터를 잃어버리면 해제할 방법이 없다.

```cpp
void leak() {
    int *p = new int[1000];
    // ... delete[] p를 잊음
}   // p가 스코프를 벗어남 — 메모리 누수!
```

```cpp
int *p = new int(10);
p = new int(20);        // ❌ 원래 메모리(10)가 누수됨 — 가리키는 포인터 없음
delete p;               // 두 번째 할당만 해제
```

> **경고:** 모든 `new`에는 대응하는 `delete`가 있어야 한다. 포인터를 잃으면 프로그램 종료 시까지 해당 메모리를 회수할 수 없다.

### 4.2 댕글링 포인터

**댕글링 포인터(dangling pointer)** 는 이미 해제된 메모리를 가리키는 포인터이다. 사용하면 **정의되지 않은 동작** 이 발생한다.

```cpp
int *p = new int(42);
delete p;             // 메모리 해제
cout << *p;           // ❌ 정의되지 않은 동작 — 댕글링 포인터!
```

**예방법:** `delete` 후 포인터를 `nullptr`로 설정한다.

```cpp
int *p = new int(42);
delete p;
p = nullptr;          // 안전 — "유효한 대상 없음"을 명확히 표시

if (p != nullptr) {
    cout << *p;       // 이 블록은 건너뜀
}
```

### 4.3 소멸자의 책임

생성자에서 `new`로 메모리를 할당했다면, 소멸자에서 **반드시** `delete`로 해제해야 한다. 이것이 객체 소멸 시 메모리 누수를 방지하는 방법이다.

```cpp
class String {
    char *buf;
    int length;
public:
    String(const char *s) {
        length = strlen(s);
        buf = new char[length + 1];    // 생성자에서 할당
        strcpy(buf, s);
    }
    ~String() {
        delete[] buf;                  // 소멸자에서 해제
    }
};
```

```
   생성자 (new)                소멸자 (delete)
       │                           │
       ▼                           ▼
┌─ String obj ─┐    ──►    ┌─ String obj ─┐
│ buf ──► 힙    │           │ buf ──► 해제  │
│ length = 5   │           │ length = 5   │
└──────────────┘           └──────────────┘
```

> **원칙:** 클래스의 생성자에서 `new`를 사용했다면, 소멸자에서 `delete`를 사용해야 한다.

---

<br>

## 5. 예외 처리

C++는 런타임 오류를 처리하기 위해 `try`-`catch`를 제공한다. 입력 검증과 동적 할당 실패 처리에 유용하다.

```cpp
try {
    // 예외가 발생할 수 있는 코드
    int *p = new int[1000000000];   // 실패할 수 있음
}
catch (bad_alloc &e) {
    cout << "메모리 할당 실패: " << e.what() << endl;
}
```

**기본 문법:**

```cpp
try {
    // 위험한 코드
    throw exception_value;    // 명시적으로 예외 발생
}
catch (type1 e1) {
    // type1 예외 처리
}
catch (type2 e2) {
    // type2 예외 처리
}
```

**예시 — 입력 검증:**

```cpp
int divide(int a, int b) {
    if (b == 0)
        throw "Division by zero!";
    return a / b;
}

int main() {
    try {
        cout << divide(10, 0) << endl;
    }
    catch (const char *msg) {
        cout << "오류: " << msg << endl;   // "오류: Division by zero!"
    }
    return 0;
}
```

| 키워드 | 역할 |
|:------|:----|
| `try` | 예외가 발생할 수 있는 코드를 감싸는 블록 |
| `throw` | 예외를 발생시킴 (catch 블록에 값을 전달) |
| `catch` | 특정 타입의 예외를 처리하는 블록 |

> **참고:** 예외 처리는 여기서 간략히 소개한다. 사용자 정의 예외 클래스, 예외 안전성 보장 등 고급 패턴은 이후 장에서 다룬다.

---

<br>

## 6. 스마트 포인터 (현대 C++)

C++11부터 표준 라이브러리의 `<memory>` 헤더에서 **스마트 포인터** 를 제공하며, 이를 통해 메모리 관리를 자동화할 수 있다. 스마트 포인터는 **RAII (Resource Acquisition Is Initialization, 자원 획득은 초기화이다)** 원칙을 따른다: 자원(힙 메모리)을 스택의 객체 수명에 연결하여, 스마트 포인터가 스코프를 벗어나면 자동으로 관리하는 메모리를 해제한다.

### 6.1 RAII 개념

**RAII** 란 자원 정리를 스택에 할당된 객체의 소멸자가 담당하는 것이다. 이는 예외가 발생하더라도 정리를 보장한다.

```
           직접 포인터 (레거시)                 스마트 포인터 (현대)
        ─────────────────────           ─────────────────────
할당:    int *p = new int(42);           auto p = make_unique<int>(42);
사용:    *p = 100;                       *p = 100;
해제:    delete p;  ← 직접 기억해야 함       // p가 스코프를 벗어나면 자동 해제
위험:    delete 잊음 → 누수                누수 위험 없음
        delete 후 사용 → UB              소유권이 명확
```

### 6.2 `std::unique_ptr` — 단독 소유

`unique_ptr`은 객체를 **단독으로 소유** 한다. 복사할 수 없고, 이동(move)만 가능하다. 스코프를 벗어나면 관리하는 객체를 삭제한다.

```cpp
#include <memory>
#include <iostream>
using namespace std;

int main() {
    // C++14: make_unique 사용 (권장)
    unique_ptr<int> p = make_unique<int>(42);
    cout << *p << endl;   // 42

    // unique_ptr은 복사 불가
    // unique_ptr<int> q = p;          // 오류: 복사 불가
    unique_ptr<int> q = move(p);       // OK: 소유권 이전
    // p는 이제 nullptr; q가 int를 소유

    cout << *q << endl;   // 42
    // q가 스코프를 벗어남 → 메모리 자동 해제
    return 0;
}
```

**클래스와 함께 사용:**

```cpp
unique_ptr<Circle> c = make_unique<Circle>(10);
cout << c->getArea() << endl;    // 직접 포인터처럼 -> 사용
// c가 스코프를 벗어나면 delete가 자동 호출됨
```

### 6.3 `std::shared_ptr` — 공유 소유

`shared_ptr`은 **여러 포인터** 가 같은 객체의 소유권을 공유할 수 있게 한다. 내부 **참조 카운트** 가 해당 객체를 가리키는 `shared_ptr` 인스턴스의 수를 추적한다. 마지막 `shared_ptr`이 소멸되면 객체가 삭제된다.

```cpp
#include <memory>
#include <iostream>
using namespace std;

int main() {
    // C++11: make_shared 사용 (권장)
    shared_ptr<int> a = make_shared<int>(100);
    cout << a.use_count() << endl;   // 1

    {
        shared_ptr<int> b = a;       // 소유권 공유
        cout << a.use_count() << endl;   // 2
        cout << *b << endl;              // 100
    }   // b가 스코프를 벗어남 → 참조 카운트가 1로 감소

    cout << a.use_count() << endl;   // 1
    // a가 스코프를 벗어남 → 참조 카운트 0 → 메모리 해제
    return 0;
}
```

### 6.4 `make_unique`와 `make_shared`

| 팩토리 함수 | 표준 | 용도 |
|:----------|:----|:----|
| `std::make_shared<T>(args...)` | C++11 | `shared_ptr<T>` 생성; 객체 + 제어 블록을 단일 할당으로 처리 |
| `std::make_unique<T>(args...)` | C++14 | `unique_ptr<T>` 생성; `new`보다 예외 안전한 대안 |

이 팩토리 함수들이 `new`를 직접 사용하는 것보다 권장되는 이유:
1. 여러 할당이 포함된 식에서 메모리 누수를 방지한다.
2. 더 간결하고 읽기 쉽다.
3. `make_shared`는 더 효율적이다 (단일 메모리 할당).

```cpp
// 레거시 스타일 (지양)
unique_ptr<Circle> p(new Circle(10));

// 현대적 스타일 (권장)
auto p = make_unique<Circle>(10);     // C++14
auto q = make_shared<Circle>(20);     // C++11
```

### 6.5 어떤 것을 사용할까

| 포인터 타입 | 사용 시점 |
|:----------|:---------|
| `unique_ptr` | 단일하고 명확한 소유자; 일반적인 기본 선택 |
| `shared_ptr` | 여러 소유자가 같은 자원을 공유해야 할 때 |
| 직접 포인터 (비소유) | 소유 없이 관찰할 때 (예: 포인터를 저장하지 않는 함수 매개변수) |
| 직접 `new`/`delete` | 레거시 코드, 또는 드문 저수준 시나리오 (커스텀 할당자, placement new) |

> **현대 C++의 원칙:** `new`나 `delete`를 쓰려고 한다면, 스마트 포인터로 대체할 수 있는지 먼저 생각하라. 대부분의 경우 가능하다.

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 객체 포인터 | `ClassName *p = &obj;` — `p->member()` 또는 `(*p).member()`로 접근 |
| `this` 포인터 | 현재 객체를 가리키는 암시적 포인터; `this->member`로 이름 충돌 해소 |
| `new` / `delete` | 힙 할당/해제; `new`는 생성자 호출, `delete`는 소멸자 호출 |
| 동적 객체 | `new ClassName(args)`로 힙에 생성; `delete p`로 소멸 |
| 동적 배열 | `new Type[n]` + `delete[] arr`; `delete`와 `new[]`를 혼용하지 말 것 |
| 정적 객체 배열 | `ClassName arr[n];` — 각 원소에 기본 생성자; 초기화 리스트로 매개변수 전달 |
| 동적 객체 배열 | `new ClassName[n]` — 기본 생성자 필수; `p[i]`로 접근 |
| 메모리 누수 | `delete` 누락; 포인터 분실 = 회수 불가능한 힙 메모리 |
| 댕글링 포인터 | `delete` 후 포인터 사용; 해제 후 `nullptr`로 설정 |
| 소멸자의 책임 | 생성자에서 `new` 사용 → 소멸자에서 `delete` 필수 |
| 예외 처리 | `try { } catch(type e) { }`; `throw`로 발생; 할당 실패 처리에 유용 |
| RAII | 자원 획득은 초기화이다; 자원의 수명을 스택 객체의 수명에 연결 |
| `unique_ptr` | 단독 소유; 복사 불가, 이동만 가능; 스코프 종료 시 자동 삭제; `make_unique` 권장 |
| `shared_ptr` | 참조 카운팅 기반 공유 소유; 마지막 소유자 소멸 시 자동 삭제; `make_shared` 권장 |
| 스마트 vs 직접 포인터 | 스마트 포인터(C++11)가 현대 C++에서 직접 `new`/`delete`를 대체; 직접 `new`/`delete`는 레거시 스타일 |

---
