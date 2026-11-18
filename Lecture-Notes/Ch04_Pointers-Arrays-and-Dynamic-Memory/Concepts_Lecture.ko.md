# 4장 — 포인터, 배열, 동적 메모리

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-11-19
>
> 명품 C++ Programming, 황기태, Ch 4

> **학습 목표:**
> 1. 객체 포인터를 선언하고 화살표 연산자로 멤버에 접근할 수 있다
> 2. 객체 배열을 선언하고 초기화하며 생성자와 소멸자의 실행 순서를 설명할 수 있다
> 3. `new`와 `delete`로 메모리, 배열, 객체, 객체 배열을 동적으로 할당하고 반환할 수 있다
> 4. `this` 포인터의 개념, 필요한 경우, 제약 사항을 설명할 수 있다
> 5. 메모리 누수와 댕글링 포인터를 피하고 스마트 포인터로 자원을 관리할 수 있다
> 6. `string` 클래스로 문자열을 생성, 입력, 검색, 변경할 수 있다

---

## 목차

- [1. 객체 포인터](#1-객체-포인터)
  - [1.1 객체에 대한 포인터](#11-객체에-대한-포인터)
  - [1.2 this 포인터](#12-this-포인터)
  - [1.3 객체 자신을 리턴하는 멤버 함수와 연쇄 호출](#13-객체-자신을-리턴하는-멤버-함수와-연쇄-호출)
  - [1.4 객체 포인터와 const](#14-객체-포인터와-const)
- [2. 동적 메모리](#2-동적-메모리)
  - [2.1 new와 delete](#21-new와-delete)
  - [2.2 동적 객체](#22-동적-객체)
  - [2.3 동적 배열](#23-동적-배열)
  - [2.4 new와 malloc()의 차이 확인](#24-new와-malloc의-차이-확인)
- [3. 객체 배열](#3-객체-배열)
  - [3.1 정적 객체 배열](#31-정적-객체-배열)
  - [3.2 2차원 객체 배열](#32-2차원-객체-배열)
  - [3.3 동적 객체 배열](#33-동적-객체-배열)
  - [3.4 객체 배열의 생성과 소멸 추적](#34-객체-배열의-생성과-소멸-추적)
  - [3.5 객체 배열의 메모리 배치와 포인터 연산](#35-객체-배열의-메모리-배치와-포인터-연산)
- [4. 메모리 관리](#4-메모리-관리)
  - [4.1 메모리 누수](#41-메모리-누수)
  - [4.2 댕글링 포인터](#42-댕글링-포인터)
  - [4.3 소멸자의 책임](#43-소멸자의-책임)
  - [4.4 delete를 잘못 사용했을 때의 결과](#44-delete를-잘못-사용했을-때의-결과)
- [5. 예외 처리](#5-예외-처리)
- [6. 스마트 포인터 (현대 C++)](#6-스마트-포인터-현대-c)
  - [6.1 RAII 개념](#61-raii-개념)
  - [6.2 `std::unique_ptr`: 단독 소유](#62-stdunique_ptr-단독-소유)
  - [6.3 `std::shared_ptr`: 공유 소유](#63-stdshared_ptr-공유-소유)
  - [6.4 `make_unique`와 `make_shared`](#64-make_unique와-make_shared)
  - [6.5 포인터 종류의 선택](#65-포인터-종류의-선택)
- [7. string 클래스](#7-string-클래스)
  - [7.1 string 객체의 생성과 입출력](#71-string-객체의-생성과-입출력)
  - [7.2 주요 멤버 함수와 연산자](#72-주요-멤버-함수와-연산자)
  - [7.3 string 객체의 동적 생성과 문자열 처리](#73-string-객체의-동적-생성과-문자열-처리)
  - [7.4 덧셈 문자열의 계산](#74-덧셈-문자열의-계산)
  - [7.5 문자열의 검색과 대치](#75-문자열의-검색과-대치)
  - [7.6 string 연산이 하는 일과 비용](#76-string-연산이-하는-일과-비용)
- [요약](#요약)
- [점검 문제](#점검-문제)

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

```text
스택
┌──────────┐      ┌──────────────┐
│  p ──────┼─────►│  Circle c    │
│ (포인터)   │      │  radius = 10 │
└──────────┘      └──────────────┘
```

*그림 1. 포인터 p가 객체 c를 가리키는 모습*

객체 포인터는 C 언어의 포인터와 같이 주소를 값으로 가지는 변수이므로, 선언만 하고 주소를 대입하지 않은 포인터로 멤버에 접근하면 안 된다. 같은 포인터에 다른 객체의 주소를 대입하면(`p = &pizza;`) 이후의 `p->getArea()`는 그 객체의 멤버 함수를 호출한다.

> **핵심:** `p->member`는 `(*p).member`의 축약형이다. 가독성을 위해 화살표 연산자를 주로 사용한다.

### 1.2 this 포인터

모든 non-static 멤버 함수에는 **현재 객체**(해당 함수가 호출된 객체)를 가리키는 암시적 포인터 `this`가 존재한다.

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

- **이름 충돌 해소:** 매개변수 이름과 멤버 변수 이름이 같을 때 둘을 구분한다. `this` 없이 `radius = radius;`라고 쓰면 두 이름 모두 매개변수를 가리키므로 멤버 변수는 바뀌지 않는다.
- **현재 객체 리턴:** 멤버 함수에서 `return *this;`로 객체 자신을, `return this;`로 객체 자신의 주소를 리턴한다. 이 방식은 7장의 연산자 중복에서 자주 쓰인다.

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

> **참고:** `this`는 포인터이므로 `this->member`로 접근한다 (`this.member`가 아님). `ClassName` 메서드 내부에서 `this`의 타입은 `ClassName*`이다(const 멤버 함수 안에서는 `const ClassName*`).

`this`는 개발자가 선언하는 변수가 아니라 컴파일러가 멤버 함수에 묵시적으로 삽입하는 매개변수이다. 따라서 객체마다 `this`의 값이 다르다. `c1.setRadius(4)`를 호출하면 `setRadius()` 안의 `this`는 `c1`을, `c2.setRadius(5)`를 호출하면 `c2`를 가리킨다. 컴파일러는 멤버 함수와 그 호출을 개념적으로 다음과 같이 변환한다.

```cpp
// 개발자가 작성한 코드
class Sample {
    int a;
public:
    void setA(int x) { this->a = x; }
};
Sample ob;
ob.setA(5);

// 컴파일러가 변환한 모습 (개념적인 표현)
void setA(Sample* this, int x) { this->a = x; }
setA(&ob, 5);    // ob의 주소가 this 매개변수로 전달된다
```

`this`에는 다음 제약이 있다.

- 멤버 함수가 아닌 함수(전역 함수 등)에서는 사용할 수 없다. 그 함수는 특정 객체와 관련이 없기 때문이다.
- static 멤버 함수에서는 사용할 수 없다. static 멤버 함수는 객체 없이도 호출될 수 있어 가리킬 객체가 없을 수 있기 때문이다(static 멤버는 6장에서 다룬다).

### 1.3 객체 자신을 리턴하는 멤버 함수와 연쇄 호출

1.2절에서 `return *this;`로 객체 자신을 리턴할 수 있다고 했다. 리턴 타입을 참조(`Point&`)로 하면 호출한 객체 자체가 결과가 되므로, 그 결과에 다시 멤버 함수를 호출하는 **연쇄 호출**이 가능하다. 리턴 타입을 값(`Point`)으로 하면 결과는 객체의 복사본이 되어 연쇄 호출의 의미가 달라진다.

```cpp
#include <iostream>
using namespace std;

class Point {
    int x = 0, y = 0;
public:
    Point &setX(int x) { this->x = x; return *this; }   // 자기 자신의 참조를 리턴
    Point &setY(int y) { this->y = y; return *this; }
    Point moveBy(int d) { x += d; y += d; return *this; } // 자기 자신의 복사본을 리턴
    void show() const { cout << "(" << x << ", " << y << ")" << endl; }
};

int main() {
    Point p;
    p.setX(3).setY(4);          // p.setX(3)의 결과가 p 자신이므로 이어서 p.setY(4)
    p.show();
    p.moveBy(1).moveBy(1);      // 두 번째 moveBy()는 첫 번째가 리턴한 복사본에 적용
    p.show();
    return 0;
}
```

```text
(3, 4)
(4, 5)
```

| 문장 | 첫 호출이 바꾸는 객체 | 둘째 호출이 바꾸는 객체 | 실행 뒤 `p` |
|:----|:----|:----|:----|
| `p.setX(3).setY(4);` | `p` (x = 3) | `p` (첫 호출이 리턴한 `p`의 참조) | (3, 4) |
| `p.moveBy(1).moveBy(1);` | `p` (3, 4)에서 (4, 5) | 첫 호출이 리턴한 임시 복사본 (5, 6) | (4, 5) |

*표 1. 참조 리턴과 값 리턴에 따른 연쇄 호출의 결과*

- `setX()`의 `*this`는 `this`가 가리키는 객체, 곧 `p` 자신이고, 리턴 타입이 `Point&`이므로 복사 없이 `p`를 그대로 돌려준다. 따라서 `p.setX(3).setY(4)`는 `p.setX(3); p.setY(4);`와 같다.
- `moveBy()`는 리턴 타입이 `Point`이므로 `*this`의 복사본을 리턴한다. 첫 `moveBy(1)`은 `p`를 (4, 5)로 바꾸지만, 둘째 `moveBy(1)`은 그 복사본에 적용되어 복사본만 (5, 6)이 되고 문장이 끝나면 사라진다. 그래서 `p`는 (4, 5)에 머문다.

객체를 바꾸는 멤버 함수를 이어 부르게 하려면 참조를 리턴해야 한다. 7장의 `+=`, `=`, 전위 `++` 연산자가 참조를 리턴하는 것도 같은 이유이다.

### 1.4 객체 포인터와 const

포인터 선언에 `const`를 붙이는 위치에 따라, 포인터가 가리키는 객체를 바꿀 수 없게 하거나 포인터 자체를 바꿀 수 없게 한다. 규칙은 C와 같으며, 객체 포인터에서는 호출할 수 있는 멤버 함수가 함께 제한된다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r) : radius(r) { }
    int getRadius() const { return radius; }     // const 멤버 함수
    void setRadius(int r) { radius = r; }
};

int main() {
    Circle a(1), b(2);
    const Circle *p = &a;      // 가리키는 객체를 바꿀 수 없는 포인터
    Circle *const q = &a;      // 포인터 자체를 바꿀 수 없는 포인터
    cout << p->getRadius() << endl;
    p = &b;                    // 가능: p가 다른 객체를 가리키게 한다
    q->setRadius(10);          // 가능: q가 가리키는 a를 바꾼다
    // p->setRadius(5);        // 오류: const Circle에 비const 멤버 함수
    // q = &b;                 // 오류: q는 다른 객체를 가리킬 수 없다
    cout << p->getRadius() << ' ' << a.getRadius() << endl;
    return 0;
}
```

```text
1
2 10
```

| 선언 | 읽는 방법 | 포인터를 다른 객체로 바꾸기 | 가리키는 객체를 바꾸기 |
|:----|:----|:----|:----|
| `const Circle *p` | `p`는 `const Circle`을 가리킨다 | 가능 (`p = &b;`) | 불가 (`const` 멤버 함수만 호출) |
| `Circle *const q` | `q`는 상수 포인터이다 | 불가 | 가능 (`q->setRadius(10);`) |
| `const Circle *const r` | 상수 포인터가 `const Circle`을 가리킨다 | 불가 | 불가 |

*표 2. const의 위치에 따른 객체 포인터의 제한*

`const`가 `*`의 왼쪽에 있으면 가리키는 대상이, 오른쪽에 있으면 포인터 변수 자체가 상수이다. 주석을 풀어 `p->setRadius(5);`를 쓰면 Apple clang은 "'this' argument to member function 'setRadius' has type 'const Circle', but function is not marked const"라는 오류를 내고, `q = &b;`를 쓰면 "cannot assign to variable 'q' with const-qualified type 'Circle *const'"라는 오류를 낸다. 앞의 오류는 3장 2.5절의 const 멤버 함수와 같은 이유이다. `p`를 통해 호출할 수 있는 것은 `getRadius()`처럼 `const`로 선언한 멤버 함수뿐이다.

`const Circle *p = &a;`라고 해서 `a` 자체가 상수가 되는 것은 아니다. `a`는 여전히 `a.setRadius(10)`이나 다른 포인터 `q`로 바꿀 수 있으며, 실제로 출력에서 `q->setRadius(10)` 뒤에 `a`의 반지름이 10이 되었다. `const` 포인터는 "이 포인터를 통해서는 바꾸지 않겠다"는 약속이며, 함수의 매개변수를 `const Circle *`로 선언하면 함수가 인자로 받은 객체를 바꾸지 않는다는 것을 컴파일러가 보장한다. 5장의 const 참조도 같은 역할을 한다.

---

<br>

## 2. 동적 메모리

### 2.1 new와 delete

**정적 할당(static allocation)** 은 변수나 배열을 선언하여 크기가 미리 정해진 메모리를 받는 방식이다. 필요한 양을 프로그램 작성 시에 예측할 수 없으면 실행 중에 필요한 만큼 메모리를 받아야 하는데, 이를 **동적 할당(dynamic allocation)** 이라 한다. 동적으로 할당하는 메모리는 프로세스의 힙 영역에서 나온다. C 언어는 `malloc()`/`free()` 라이브러리 함수를 쓰고, C++는 기본 연산자인 `new`와 `delete`를 쓴다.

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
| `void*` 리턴, C++에서는 캐스팅 필요 | 올바른 타입 리턴 |
| 생성자 호출 안 함 | 생성자 호출 |
| 소멸자 호출 안 함 | 소멸자 호출 |

*표 3. C의 malloc/free와 C++의 new/delete 비교*

> **핵심:** C++에서는 `malloc`/`free` 대신 `new`/`delete`를 사용한다. `new`는 생성자를, `delete`는 소멸자를 호출하지만, `malloc`과 `free`는 둘 다 호출하지 않는다.

> **참고:** **현대 C++.** 직접 `new`/`delete`를 사용하는 것은 현대 C++에서 **레거시 스타일**로 간주된다. C++11부터 **스마트 포인터**(`std::unique_ptr`, `std::shared_ptr`)가 동적 메모리를 관리하는 권장 방법이다. 스마트 포인터는 더 이상 필요하지 않을 때 자동으로 메모리를 해제하여, 메모리 누수, 댕글링 포인터, 이중 해제 등의 버그를 원천적으로 방지한다. 자세한 내용은 [6절](#6-스마트-포인터-현대-c)을 참조한다.

할당과 동시에 괄호 안의 값으로 초기화할 수 있다.

```cpp
int *pInt = new int(20);        // 20으로 초기화된 int 할당
char *pChar = new char('a');    // 'a'로 초기화된 char 할당
```

`delete`에는 `new`로 받은 주소만 한 번 전달해야 한다. 다음 두 경우는 정의되지 않은 동작이며 대개 실행 시간 오류로 나타난다.

```cpp
int n;
int *p = &n;
delete p;      // 오류: p가 가리키는 메모리는 동적으로 할당받은 것이 아니다

int *q = new int;
delete q;      // 정상적인 반환
delete q;      // 오류: 이미 반환한 메모리를 다시 반환할 수 없다
```

> **[시스템프로그래밍]** 시스템프로그래밍에서 배우는 프로세스의 주소 공간은 코드, 전역 데이터, 힙, 스택 영역으로 나뉘며, 힙의 블록은 동적 메모리 할당기가 관리한다. `new`는 보통 이 할당기(C 런타임의 `malloc()` 구현)에서 블록을 받고 `delete`는 블록을 돌려주는데, 할당기는 블록마다 크기와 사용 여부 같은 관리 정보를 두고 반환된 블록을 다시 내주므로, 위 코드처럼 할당받지 않은 주소나 이미 반환한 주소를 넘기면 그 관리 정보가 깨질 수 있다.

### 2.2 동적 객체

객체를 힙에 동적으로 생성할 수 있다. `new`가 생성자를 호출하고, `delete`가 소멸자를 호출한다.

```cpp
Circle *p = new Circle(10);   // 힙 할당 + 생성자 Circle(10) 호출
cout << p->getArea();          // 화살표 연산자로 접근
delete p;                      // 소멸자 호출 + 메모리 해제
```

```text
스택                  힙
┌──────────┐         ┌──────────────┐
│  p ──────┼────────►│  Circle      │
│ (포인터)   │         │  radius = 10 │
└──────────┘         └──────────────┘
                    ↑ new로 할당 / delete로 해제
```

*그림 2. 스택의 포인터 p와 힙에 생성된 Circle 객체*

`new Circle`처럼 인자 없이 쓰면 기본 생성자가, `new Circle(30)`처럼 인자를 주면 매개변수가 있는 생성자가 호출된다. 여러 객체를 동적으로 생성했을 때 `delete`의 순서는 생성 순서와 관계없이 원하는 대로 정할 수 있다. 반복문 안에서 객체를 생성하고 사용한 뒤 바로 `delete`하는 다음 예에서 `delete p;`를 빠뜨리면 반복할 때마다 메모리 누수가 생긴다(4.1절).

```cpp
int radius;
while (true) {
    cout << "정수 반지름 입력(음수이면 종료)>> ";
    cin >> radius;
    if (radius < 0) break;              // 음수가 입력되면 종료
    Circle *p = new Circle(radius);     // 동적 객체 생성
    cout << "원의 면적은 " << p->getArea() << endl;
    delete p;                           // 객체 반환
}
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

배열 크기는 실행 중에 정할 수 있다. 예를 들어 `cin >> n;`으로 개수를 입력받은 뒤 `int *p = new int[n];`으로 정확히 그만큼의 배열을 할당할 수 있다.

**객체 배열:**

```cpp
Circle *circles = new Circle[3];   // 기본 생성자 3번 호출
circles[0].setRadius(1);
circles[1].setRadius(2);
circles[2].setRadius(3);
delete[] circles;                  // 소멸자 3번 호출 후 메모리 해제
```

> **주의:** `new`는 `delete`와, `new[]`는 `delete[]`와 반드시 짝을 맞춰야 한다. `new[]`로 할당한 메모리에 `delete`를 사용하면 **정의되지 않은 동작**이 발생한다.

배열을 할당하면서 `new int[10](20)`처럼 모든 원소를 한 값으로 초기화하는 문법은 없다. 이 식과 `new int(20)[10]`은 C++17까지 컴파일 오류이다(C++20부터는 `new int[10](20)`이 허용되지만 첫 원소만 20이 되고 나머지는 0이 된다). C++11부터는 중괄호로 원소별 초기값을 줄 수 있다.

```cpp
int *a = new int[3]{1, 2, 3};   // C++11: 1, 2, 3으로 초기화
int *b = new int[5]{};          // 모든 원소를 0으로 초기화
delete[] a;
delete[] b;
```

### 2.4 new와 malloc()의 차이 확인

표 3에서 `new`는 생성자를 호출하고 `malloc()`은 호출하지 않는다고 했다. 생성자와 소멸자가 메시지를 출력하는 클래스로 두 방법을 비교하면 다음과 같다.

```cpp
#include <iostream>
#include <cstdlib>
using namespace std;

class Counter {
    int value;
public:
    Counter() : value(100) { cout << "생성자 실행, value=" << value << endl; }
    ~Counter() { cout << "소멸자 실행" << endl; }
};

int main() {
    cout << "[new / delete]" << endl;
    Counter *p = new Counter;                          // 메모리 할당 + 생성자
    delete p;                                          // 소멸자 + 메모리 반환

    cout << "[malloc / free]" << endl;
    Counter *q = (Counter *)malloc(sizeof(Counter));   // 메모리 할당만
    free(q);                                           // 메모리 반환만
    cout << "끝" << endl;
    return 0;
}
```

```text
[new / delete]
생성자 실행, value=100
소멸자 실행
[malloc / free]
끝
```

`new Counter`는 `Counter` 크기의 메모리를 할당한 뒤 생성자를 호출하여 `value`를 100으로 초기화하고, `delete p`는 소멸자를 호출한 뒤 메모리를 반환한다. `malloc(sizeof(Counter))`는 같은 크기의 메모리를 줄 뿐 생성자를 호출하지 않으므로 메시지가 없고, `q`가 가리키는 메모리의 `value`에는 정해지지 않은 값이 들어 있다. 생성자가 실행되지 않았으므로 이 메모리는 아직 `Counter` 객체라고 할 수 없다. `free(q)`도 소멸자를 호출하지 않는다.

| 구분 | `new` / `delete` | `malloc()` / `free()` |
|:----|:----|:----|
| 크기 지정 | 타입으로 정해짐 (`new Counter`) | 바이트 수를 직접 계산 (`sizeof(Counter)`) |
| 리턴 타입 | `Counter*` | `void*`이므로 형 변환이 필요 |
| 생성자와 소멸자 | 호출함 | 호출하지 않음 |
| 할당 실패 | 예외를 던짐(5절) | `NULL`을 리턴 |
| 짝 | `new`는 `delete`, `new[]`는 `delete[]` | `malloc()`은 `free()` |

*표 4. new와 malloc()의 비교*

`string`처럼 생성자에서 내부 메모리를 준비하는 멤버를 가진 클래스는 `malloc()`으로 할당하면 그 준비가 일어나지 않아, 멤버를 사용하는 순간 정의되지 않은 동작이 된다. 또 두 방식의 짝을 섞으면 안 된다. `new`로 할당한 메모리를 `free()`로 반환한 프로그램을 AddressSanitizer로 실행하면(macOS에서는 `ASAN_OPTIONS=alloc_dealloc_mismatch=1`이 필요하다) `alloc-dealloc-mismatch (operator new vs free)`가 보고된다. C++에서 객체를 동적으로 만들 때는 언제나 `new`와 `delete`를 쓴다.

---

<br>

## 3. 객체 배열

### 3.1 정적 객체 배열

정적(스택) 객체 배열은 각 원소에 대해 **기본 생성자**를 호출한다.

```cpp
Circle arr[3];    // 3개 원소 각각에 기본 생성자 호출
```

객체 배열을 선언하면 배열 전체를 위한 공간이 할당된 뒤 `arr[0]`, `arr[1]`, `arr[2]`의 순서로 각 원소의 생성자가 실행된다. 배열이 소멸할 때는 생성의 반대 순서인 `arr[2]`, `arr[1]`, `arr[0]`의 순서로 소멸자가 실행된다. `Circle circleArray[3](5);`처럼 모든 원소에 같은 인자를 주는 선언은 C++17까지 컴파일 오류이다.

매개변수가 있는 생성자를 호출하려면 원소마다 생성자를 지정하는 **중괄호 초기화 목록**을 사용한다:

```cpp
Circle arr[3] = { Circle(1), Circle(2), Circle(3) };
```

`Circle circleArray[3] = { Circle(10), Circle(20), Circle() };`처럼 일부 원소에 `Circle()`을 쓰면 그 원소는 기본 생성자로 생성된다.

배열의 각 원소는 독립적인 객체이다:

```cpp
arr[0].setRadius(10);   // arr[0]만 영향을 받음
cout << arr[1].getArea();
```

배열 이름은 첫 원소의 주소이므로 `Circle *p = arr;`로 객체 포인터에 대입한 뒤 `p->getArea()`와 `p++`를 반복하여 원소를 차례로 접근할 수도 있다.

### 3.2 2차원 객체 배열

객체 배열도 2차원으로 선언할 수 있다. `Circle circles[2][3];`은 2행 3열, 6개의 `Circle` 객체를 만들고 각 원소에 기본 생성자를 호출한다.

```cpp
Circle circles[2][3];          // 기본 생성자 6번 호출
circles[0][0].setRadius(1);
circles[0][1].setRadius(2);
circles[0][2].setRadius(3);
circles[1][0].setRadius(4);
circles[1][1].setRadius(5);
circles[1][2].setRadius(6);

for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++)
        cout << "Circle [" << i << "," << j << "]의 면적은 " << circles[i][j].getArea() << endl;
```

```text
           열 0              열 1              열 2
행 0   circles[0][0]    circles[0][1]    circles[0][2]
       radius = 1       radius = 2       radius = 3
행 1   circles[1][0]    circles[1][1]    circles[1][2]
       radius = 4       radius = 5       radius = 6
```

*그림 3. 2행 3열 Circle 배열의 원소와 반지름*

출력은 반지름 1~6에 대한 면적 3.14, 12.56, 28.26, 50.24, 78.5, 113.04이다. 선언과 동시에 행마다 중괄호로 묶어 초기화할 수도 있다.

```cpp
Circle circles[2][3] = { { Circle(1), Circle(2), Circle(3) },
                         { Circle(4), Circle(5), Circle(6) } };
```

### 3.3 동적 객체 배열

동적 객체 배열은 `new[]`로 할당한다. 초기화 목록 없이 `new Circle[n]`으로 할당하면 각 원소에 **기본 생성자**가 호출되며, 이 형식으로는 원소마다 인자를 전달할 수 없다. C++11부터는 `new Circle[3]{ Circle(1), Circle(2), Circle(3) }`처럼 중괄호 초기화 목록으로 원소별 생성자를 지정할 수 있다.

```cpp
Circle *p = new Circle[4];    // 기본 생성자 4번 호출
p[0].setRadius(5);            // 첨자 연산자로 접근
p[1].setRadius(10);
delete[] p;                   // 소멸자 4번 호출
```

> **핵심:** 초기화 목록 없는 `new ClassName[n]`은 클래스에 **기본 생성자**가 있어야 한다 (명시적으로 정의하거나 컴파일러가 제공). 매개변수가 있는 생성자만 존재하면 컴파일 오류가 발생한다.

동적 배열의 원소는 첨자(`p[i]`) 대신 포인터 연산으로도 접근할 수 있다. `p->setRadius(10)`은 `p[0]`, `(p+1)->setRadius(20)`은 `p[1]`에 대한 호출이다. `delete[] p;`를 실행하면 정적 배열과 마찬가지로 생성의 반대 순서(`p[3]`, `p[2]`, `p[1]`, `p[0]`)로 각 원소의 소멸자가 실행된 뒤 메모리가 반환된다.

### 3.4 객체 배열의 생성과 소멸 추적

3.1절과 3.3절의 규칙, 곧 원소는 첨자 순서대로 생성되고 그 역순으로 소멸한다는 규칙을 생성자와 소멸자가 메시지를 출력하는 `Circle`로 확인하면 다음과 같다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { radius = 1; cout << "생성 " << radius << endl; }
    Circle(int r) { radius = r; cout << "생성 " << radius << endl; }
    ~Circle() { cout << "소멸 " << radius << endl; }
};

int main() {
    cout << "[정적 배열]" << endl;
    {
        Circle arr[3] = { Circle(10), Circle(20) };   // arr[2]는 기본 생성자
    }                                                 // 블록이 끝나면 arr가 소멸한다
    cout << "[동적 배열]" << endl;
    Circle *p = new Circle[2];                        // 두 원소 모두 기본 생성자
    delete[] p;
    return 0;
}
```

```text
[정적 배열]
생성 10
생성 20
생성 1
소멸 1
소멸 20
소멸 10
[동적 배열]
생성 1
생성 1
소멸 1
소멸 1
```

- 초기화 목록에 원소가 2개만 있으므로 `arr[0]`과 `arr[1]`은 `Circle(int)`로, 목록에 없는 `arr[2]`는 기본 생성자로 생성된다. 원소 수보다 적은 초기화 목록을 쓸 때에도 나머지 원소를 위해 기본 생성자가 있어야 한다.
- 중괄호 블록 `{ }` 안에 선언한 `arr`는 블록이 끝나는 곳에서 소멸한다. 소멸 순서는 `arr[2]`, `arr[1]`, `arr[0]`으로 생성의 역순이다.
- `new Circle[2]`는 기본 생성자를 두 번 호출하고, `delete[] p`는 소멸자를 두 번 호출한다. `delete[]`가 몇 번 소멸자를 불러야 하는지 알 수 있는 것은, 일반적인 구현에서 `new[]`가 할당할 때 원소 수를 함께 기록해 두기 때문이다. 그래서 `new[]`로 받은 메모리는 반드시 `delete[]`로 반환한다(4.4절).

### 3.5 객체 배열의 메모리 배치와 포인터 연산

객체 배열의 원소는 메모리에 빈틈없이 연속으로 놓이며, 각 원소는 `sizeof(Circle)` 바이트를 차지한다. 3장 2.6절에서 본 것처럼 객체에는 멤버 변수만 들어가므로, `int` 하나를 가진 `Circle`의 원소는 4바이트씩 떨어져 있다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r = 1) : radius(r) { }
    int getRadius() const { return radius; }
};

int main() {
    Circle arr[3] = { Circle(10), Circle(20), Circle(30) };
    Circle *p = arr;                                     // 배열 이름은 첫 원소의 주소
    for (int i = 0; i < 3; i++) {
        cout << "arr[" << i << "]: 시작 위치 +" << (char *)&arr[i] - (char *)arr
             << "바이트, radius=" << (p + i)->getRadius() << endl;
    }
    cout << "sizeof(arr)=" << sizeof(arr) << ", 원소 수=" << sizeof(arr) / sizeof(arr[0]) << endl;
    return 0;
}
```

```text
arr[0]: 시작 위치 +0바이트, radius=10
arr[1]: 시작 위치 +4바이트, radius=20
arr[2]: 시작 위치 +8바이트, radius=30
sizeof(arr)=12, 원소 수=3
```

```text
 arr (Circle 3개, 12바이트)
   +0        +4        +8        +12
   ┌─────────┬─────────┬─────────┐
   │ arr[0]  │ arr[1]  │ arr[2]  │
   │ r = 10  │ r = 20  │ r = 30  │
   └─────────┴─────────┴─────────┘
   ▲         ▲         ▲
   p         p + 1     p + 2
```

*그림 4. Circle 배열의 원소 배치와 포인터 연산*

- 포인터에 정수를 더하는 `p + i`는 주소에 `i`를 더하는 것이 아니라 `i * sizeof(Circle)` 바이트를 더한다. 그래서 `p + 1`은 4바이트 뒤인 `arr[1]`을 가리키며, `(p + i)->getRadius()`와 `arr[i].getRadius()`는 같은 원소를 다룬다. 이 규칙은 C의 포인터 연산과 같다.
- 주소의 차이를 바이트 단위로 보려고 `(char *)`로 바꾸어 뺐다. `char`의 크기는 1바이트이므로 `char*`끼리의 차이는 바이트 수이다. `&arr[1] - &arr[0]`처럼 `Circle*`끼리 빼면 원소 수인 1이 나온다.
- `sizeof(arr)`는 배열 전체의 크기 12바이트이고, `sizeof(arr) / sizeof(arr[0])`로 원소 수 3을 구할 수 있다. 이 계산은 배열 이름에 대해서만 맞으며, `sizeof(p)`는 포인터 자체의 크기(64비트 환경에서 8바이트)이므로 원소 수를 구하는 데 쓸 수 없다.

---

<br>

## 4. 메모리 관리

### 4.1 메모리 누수

**메모리 누수(memory leak)** 는 동적으로 할당된 메모리를 해제하지 않을 때 발생한다. 할당된 메모리를 가리키는 포인터를 잃어버리면 해제할 방법이 없다.

```cpp
void leak() {
    int *p = new int[1000];
    // ... delete[] p를 잊음
}   // p가 스코프를 벗어남: 메모리 누수!
```

```cpp
int *p = new int(10);
p = new int(20);        // ❌ 원래 메모리(10)가 누수됨: 가리키는 포인터 없음
delete p;               // 두 번째 할당만 해제
```

> **주의:** 모든 `new`에는 대응하는 `delete`가 있어야 한다. 포인터를 잃으면 프로그램 종료 시까지 해당 메모리를 회수할 수 없다.

프로그램이 종료되면 운영체제가 그 프로세스의 메모리를 모두 회수하므로 누수된 메모리도 이때 회수된다. 그러나 서버처럼 오래 실행되는 프로그램에서는 누수가 쌓여 사용할 수 있는 메모리가 줄어든다.

### 4.2 댕글링 포인터

**댕글링 포인터(dangling pointer)** 는 이미 해제된 메모리를 가리키는 포인터이다. 사용하면 **정의되지 않은 동작**이 발생한다.

```cpp
int *p = new int(42);
delete p;             // 메모리 해제
cout << *p;           // ❌ 정의되지 않은 동작: 댕글링 포인터!
```

**예방법:** `delete` 후 포인터를 `nullptr`로 설정한다. 다만 같은 메모리를 가리키던 다른 포인터는 여전히 댕글링 포인터로 남으므로, 이 방법은 `nullptr`를 대입한 그 포인터만 보호한다.

```cpp
int *p = new int(42);
delete p;
p = nullptr;          // 안전: "유효한 대상 없음"을 명확히 표시

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

```text
   생성자 (new)                소멸자 (delete)
       │                           │
       ▼                           ▼
┌─ String obj ─┐    ──►    ┌─ String obj ─┐
│ buf ──► 힙    │           │ buf ──► 해제  │
│ length = 5   │           │ length = 5   │
└──────────────┘           └──────────────┘
```

*그림 5. 생성자에서 할당하고 소멸자에서 해제하는 String 객체의 버퍼*

> **참고:** **원칙.** 클래스의 생성자에서 `new`를 사용했다면, 소멸자에서 `delete`를 사용해야 한다.

### 4.4 delete를 잘못 사용했을 때의 결과

2.1절과 2.3절에서 정의되지 않은 동작이라고 한 세 가지 실수는 컴파일러가 대부분 잡지 못하고, 실행해도 바로 드러나지 않을 수 있다. 메모리 오류 검사 도구인 AddressSanitizer(`-fsanitize=address` 옵션으로 켠다)로 컴파일하여 실행하면 각 실수가 일어난 지점에서 프로그램이 멈추고 오류의 종류가 출력된다. 다음은 macOS의 Apple clang에서 각 프로그램의 첫 오류 줄이다.

| 실수 | 코드 | AddressSanitizer의 보고 |
|:----|:----|:----|
| `new`로 할당하지 않은 메모리를 `delete` | `int n = 10; int *p = &n; delete p;` | `attempting free on address which was not malloc()-ed` |
| 같은 메모리를 두 번 `delete` | `int *p = new int(10); delete p; delete p;` | `attempting double-free` |
| `new[]`로 받은 메모리를 `delete`로 반환 | `int *p = new int[10]; delete p;` | `alloc-dealloc-mismatch (operator new [] vs operator delete)` |

*표 5. delete를 잘못 사용한 코드와 AddressSanitizer의 보고*

- 첫 번째 실수의 `p`는 스택에 있는 지역 변수 `n`의 주소를 가진다. 스택의 메모리는 함수가 끝날 때 자동으로 사라지므로 `delete`로 반환할 대상이 아니다.
- 두 번째 실수는 첫 `delete p;` 뒤에도 `p`가 같은 주소를 가지고 있어서 생긴다(4.2절의 댕글링 포인터). 첫 `delete` 뒤에 `p = nullptr;`을 두면 두 번째 `delete p;`는 널 포인터를 반환하는 것이 되어 아무 일도 하지 않는다.
- 세 번째 실수는 macOS에서 기본 설정으로는 보고되지 않고, 환경 변수 `ASAN_OPTIONS=alloc_dealloc_mismatch=1`을 주어야 보고된다. 대신 이 경우는 Apple clang이 경고 옵션 없이도 "'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'?"라는 경고를 내므로, 컴파일러의 경고를 읽는 것만으로도 찾을 수 있다.

이런 도구는 프로그램이 실제로 실행한 경로에서 일어난 오류만 찾는다. 오류가 보고되지 않았다고 해서 프로그램에 메모리 오류가 없다는 뜻은 아니므로, `new`와 `delete`의 짝을 코드에서 직접 확인하는 습관이 먼저이다.

---

<br>

## 5. 예외 처리

C++는 실행 중에 발생한 예외 상황을 처리하기 위해 `try`-`catch`를 제공한다. 입력 검증과 동적 할당 실패 처리에 유용하다.

```cpp
try {
    // 예외가 발생할 수 있는 코드
    int *p = new int[1000000000];   // 실패할 수 있음
}
catch (bad_alloc &e) {
    cout << "메모리 할당 실패: " << e.what() << endl;
}
```

> **주의:** 표준 C++의 `new`는 할당에 실패하면 `nullptr`를 리턴하지 않고 `bad_alloc` 예외를 던진다. 따라서 `int *p = new int[n]; if (!p) { ... }`처럼 리턴값을 검사하는 코드는 실패를 잡지 못한다. 리턴값으로 실패를 확인하려면 `<new>` 헤더의 `new (nothrow) int[n]`을 써야 하며, 이 형식만 실패 시 `nullptr`를 리턴한다.

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

**입력 검증 예시:**

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

*표 6. 예외 처리 키워드와 역할*

> **참고:** 예외 처리는 여기서 간략히 소개한다. 사용자 정의 예외 클래스, 예외 안전성 보장 등 고급 패턴은 이 장의 범위를 벗어나며, 예외 처리는 13장에서 자세히 다룬다.

---

<br>

## 6. 스마트 포인터 (현대 C++)

C++11부터 표준 라이브러리의 `<memory>` 헤더에서 **스마트 포인터**를 제공하며, 이를 통해 메모리 관리를 자동화할 수 있다. 스마트 포인터는 **RAII (Resource Acquisition Is Initialization, 자원 획득은 초기화이다)** 원칙을 따른다: 자원(힙 메모리)을 스택의 객체 수명에 연결하여, 스마트 포인터가 스코프를 벗어나면 자동으로 관리하는 메모리를 해제한다.

### 6.1 RAII 개념

**RAII**란 자원을 객체의 생성자에서 획득하고 소멸자에서 해제하도록 하여 자원의 수명을 객체의 수명에 묶는 기법이다. 그 객체가 스택에 할당된 지역 객체이면 스코프를 벗어날 때 소멸자가 자동으로 호출되므로, 예외가 발생하더라도 정리가 보장된다.

```text
           직접 포인터 (레거시)                 스마트 포인터 (현대)
        ─────────────────────           ─────────────────────
할당:    int *p = new int(42);           auto p = make_unique<int>(42);
사용:    *p = 100;                       *p = 100;
해제:    delete p;  ← 직접 기억해야 함       // p가 스코프를 벗어나면 자동 해제
위험:    delete 잊음 → 누수                누수 위험 없음
        delete 후 사용 → UB              소유권이 명확
```

*그림 6. 직접 포인터와 스마트 포인터의 자원 관리 비교*

### 6.2 `std::unique_ptr`: 단독 소유

`unique_ptr`은 객체를 **단독으로 소유**한다. 복사할 수 없고, 이동(move)만 가능하다. 스코프를 벗어나면 관리하는 객체를 삭제한다. 소유권의 이전과 사용자 지정 삭제자는 14장 7절에서 다룬다.

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

### 6.3 `std::shared_ptr`: 공유 소유

`shared_ptr`은 **여러 포인터**가 같은 객체의 소유권을 공유할 수 있게 한다. 내부 **참조 카운트**가 해당 객체를 가리키는 `shared_ptr` 인스턴스의 수를 추적한다. 마지막 `shared_ptr`이 소멸되면 객체가 삭제된다. 두 객체가 서로를 `shared_ptr`로 가리킬 때 생기는 순환 참조와 이를 끊는 `weak_ptr`는 14장 7절에서 다룬다.

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

*표 7. 스마트 포인터를 만드는 팩토리 함수*

이 팩토리 함수들이 `new`를 직접 사용하는 것보다 권장되는 이유:
1. 여러 할당이 포함된 식에서 메모리 누수를 방지한다(함수 인자의 평가 순서 규칙이 바뀐 C++17 이전에 특히 중요했다).
2. 더 간결하고 읽기 쉽다.
3. `make_shared`는 더 효율적이다 (단일 메모리 할당).

```cpp
// 레거시 스타일 (지양)
unique_ptr<Circle> p(new Circle(10));

// 현대적 스타일 (권장)
auto p = make_unique<Circle>(10);     // C++14
auto q = make_shared<Circle>(20);     // C++11
```

### 6.5 포인터 종류의 선택

| 포인터 타입 | 사용 시점 |
|:----------|:---------|
| `unique_ptr` | 단일하고 명확한 소유자; 일반적인 기본 선택 |
| `shared_ptr` | 여러 소유자가 같은 자원을 공유해야 할 때 |
| 직접 포인터 (비소유) | 소유 없이 관찰할 때 (예: 포인터를 저장하지 않는 함수 매개변수) |
| 직접 `new`/`delete` | 레거시 코드, 또는 드문 저수준 시나리오 (커스텀 할당자, placement new) |

*표 8. 포인터 종류별 사용 시점*

> **참고:** **현대 C++의 원칙.** `new`나 `delete`를 쓰기 전에 스마트 포인터로 대체할 수 있는지 먼저 검토한다. 대부분의 경우 대체할 수 있다.

---

<br>

## 7. string 클래스

C++에서 문자열은 두 가지 방법으로 다룬다. 하나는 C 언어와 같이 `'\0'`으로 끝나는 `char` 배열인 **C-스트링**이고, 다른 하나는 C++ 표준 라이브러리의 **`string` 클래스** 객체이다. `string` 클래스는 `<string>` 헤더에 선언되어 있으며, 크기가 필요에 따라 늘어나는 가변 크기 문자열을 저장하고 복사, 비교, 길이 계산 같은 연산을 멤버 함수와 연산자로 제공한다.

```cpp
#include <string>
using namespace std;

string str = "I love ";   // 'I', ' ', 'l', 'o', 'v', 'e', ' '의 7개 문자
str.append("C++.");       // str은 "I love C++."이 된다. 11개 문자
```

### 7.1 string 객체의 생성과 입출력

| 생성자 | 설명 |
|:------|:----|
| `string()` | 빈 문자열을 가진 객체 생성 |
| `string(const string& str)` | 객체 `str`을 복사한 새 객체 생성 |
| `string(const char* s)` | C-스트링 `s`를 복사하여 객체 생성 |
| `string(const char* s, int n)` | 문자 배열 `s`에서 앞의 `n`개 문자를 복사하여 객체 생성 |

*표 9. string 클래스의 주요 생성자*

```cpp
string str;                            // 빈 문자열
string address("서울시 성북구 삼선동 389");  // 문자열 리터럴로 초기화
string copyAddress(address);           // address를 복사
char text[] = {'L', 'o', 'v', 'e', ' ', 'C', '+', '+', '\0'};  // C-스트링
string title(text);                    // "Love C++"

cout << str << endl;                   // 빈 문자열이므로 빈 줄만 출력
cout << address << endl;               // 서울시 성북구 삼선동 389
cout << title << endl;                 // Love C++
```

`string` 객체는 `cout <<`으로 출력하고 `cin >>`으로 입력한다. `cin >>`은 공백 문자에서 입력을 끊으므로 `Han Hong Jin`을 입력하면 `Han`만 저장된다. 공백을 포함한 한 줄을 입력받으려면 `<string>` 헤더의 `getline(cin, name, '\n')`을 쓴다. 세 번째 인자는 입력의 끝으로 삼을 문자이며 생략하면 `'\n'`이다.

```cpp
string names[5];
for (int i = 0; i < 5; i++) {
    cout << "이름 >> ";
    getline(cin, names[i], '\n');      // 공백을 포함한 한 줄 입력
}
string latter = names[0];
for (int i = 1; i < 5; i++)
    if (latter < names[i])             // 사전 순으로 비교
        latter = names[i];
cout << "사전에서 가장 뒤에 나오는 문자열은 " << latter << endl;
```

### 7.2 주요 멤버 함수와 연산자

| 멤버 함수 | 사용 예 | 결과 |
|:---------|:-------|:----|
| `append(str)`, `append(str, pos, n)` | `string s("I"); s.append(" love "); s.append("really you", 7, 3);` | `"I love you"` |
| `insert(pos, str)` | `string s("I love C++"); s.insert(2, "really ");` | `"I really love C++"` |
| `replace(pos, n, str)` | `string s("I really love C++"); s.replace(2, 11, "study");` | `"I study C++"` |
| `size()`, `length()`, `capacity()` | `string s("I study C++"); s.length();` | 11 (`capacity()`는 구현에 따라 다름) |
| `erase(pos, n)`, `clear()` | `string s("I study C++"); s.erase(0, 7);` 다음 `s.clear();` | `" C++"`, 다음 `""` |
| `substr(pos, n)`, `substr(pos)` | `string s = "I love C++"; s.substr(2, 4); s.substr(2);` | `"love"`, `"love C++"` |
| `find(str)`, `find(str, pos)` | `string s = "I love love C++"; s.find("love"); s.find("love", 3); s.find("C#");` | 2, 7, `string::npos` |
| `rfind(str)` | `string s = "I love love C++"; s.rfind("love");` | 7 |
| `at(pos)` | `string s("I love C++"); s.at(7);` | `'C'` |

*표 10. string 클래스의 주요 멤버 함수*

`find()`는 찾는 문자열이 처음 나타나는 인덱스를 리턴하고, 찾지 못하면 `string::npos`를 리턴한다. `find()`의 리턴 타입은 부호 없는 정수(`size_t`)이므로 실패 검사는 `s.find(x) == string::npos`로 쓰는 것이 정확하다. 결과를 `int`에 저장하면 `npos`가 -1로 변환되므로 `index == -1`로 검사하는 코드도 흔히 동작한다.

| 연산자 | 사용 예 | 연산자 | 사용 예 |
|:------|:-------|:------|:-------|
| `=` | `s1 = s2;` | `==` | `s1 == s2` |
| `[]` | `char c = s[1];` | `!=` | `s1 != s2` |
| `+` | `s = s1 + s2;` | `<` | `s1 < s2` |
| `+=` | `s1 += s2;` | `>` | `s1 > s2` |
| `<<` | `cout << s1;` | `<=` | `s1 <= s2` |
| `>>` | `cin >> s1;` | `>=` | `s1 >= s2` |

*표 11. string 클래스의 연산자*

비교 연산자는 문자열을 사전 순으로 비교한다. `[]`는 범위를 검사하지 않지만 `at()`은 범위를 벗어난 인덱스에 대해 `out_of_range` 예외를 던진다.

### 7.3 string 객체의 동적 생성과 문자열 처리

`string` 객체도 `new`와 `delete`로 동적으로 생성하고 반환할 수 있다.

```cpp
string *p = new string("C++");   // string 객체 동적 생성
cout << *p;                      // "C++" 출력
p->append(" Great!!");           // p가 가리키는 문자열이 "C++ Great!!"이 됨
cout << *p;                      // "C++ Great!!" 출력
delete p;                        // string 객체 반환
```

다음은 공백을 포함한 문자열을 입력받아 한 문자씩 왼쪽으로 회전시키며 출력하는 예이다. 맨 앞 문자 하나를 `substr(0, 1)`로, 나머지를 `substr(1, len-1)`로 분리한 뒤 `+`로 이어 붙인다.

```cpp
string s;
getline(cin, s, '\n');                    // 예: I love you
int len = s.length();
for (int i = 0; i < len; i++) {
    string first = s.substr(0, 1);        // 맨 앞의 문자 1개
    string sub = s.substr(1, len - 1);    // 나머지 문자들
    s = sub + first;                      // 두 문자열을 연결
    cout << s << endl;                    // " love youI", "love youI ", ...
}
```

`len`번 회전하면 문자열은 원래대로 돌아오므로 마지막 줄은 입력과 같은 `I love you`이다. 같은 방식으로 `find()`와 `substr()`을 조합하면 `66+2+8+55+100` 같은 덧셈식을 `+`마다 잘라 `stoi()`로 정수로 바꾸어 합(231)을 구할 수 있고, `find()`와 `replace()`를 반복하면 문자열 속의 단어를 모두 다른 단어로 바꿀 수 있다.

### 7.4 덧셈 문자열의 계산

`find()`와 `substr()`, 문자열을 정수로 바꾸는 `stoi()`를 함께 쓰면 `66+2+8+55+100`처럼 입력된 덧셈식을 계산할 수 있다. 프로그램은 `+`를 찾을 때마다 바로 앞까지를 잘라 정수로 바꾸어 더한다.

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cout << "7+23+5+100+25와 같이 덧셈 문자열을 입력하세요." << endl;
    getline(cin, s, '\n');                     // 문자열 입력
    int sum = 0;
    int startIndex = 0;                        // 문자열 내에 검색할 시작 인덱스
    while (true) {
        int fIndex = s.find('+', startIndex);
        if (fIndex == -1) {                    // '+' 문자를 발견할 수 없음
            string part = s.substr(startIndex);
            if (part == "") break;             // "2+3+", 즉 +로 끝나는 경우
            cout << part << endl;
            sum += stoi(part);                 // 문자열을 수로 변환하여 더하기
            break;
        }
        int count = fIndex - startIndex;       // 서브스트링으로 자를 문자 개수
        string part = s.substr(startIndex, count);   // startIndex부터 count개의 문자로 서브스트링 만들기
        cout << part << endl;
        sum += stoi(part);                     // 문자열을 수로 변환하여 더하기
        startIndex = fIndex + 1;               // 검색을 시작할 인덱스 전진
    }
    cout << "숫자들의 합은 " << sum << endl;
    return 0;
}
```

```text
7+23+5+100+25와 같이 덧셈 문자열을 입력하세요.
66+2+8+55+100
66
2
8
55
100
숫자들의 합은 231
```

둘째 줄은 키보드로 입력한 내용이다. `s = "66+2+8+55+100"`에 대해 반복문이 진행되는 과정은 다음과 같다.

| 반복 | `startIndex` | `fIndex` | 잘라 낸 `part` | `sum` |
|:----|:----|:----|:----|:----|
| 1 | 0 | 2 | `substr(0, 2)` = "66" | 66 |
| 2 | 3 | 4 | `substr(3, 1)` = "2" | 68 |
| 3 | 5 | 6 | `substr(5, 1)` = "8" | 76 |
| 4 | 7 | 9 | `substr(7, 2)` = "55" | 131 |
| 5 | 10 | -1 | `substr(10)` = "100" | 231 |

*표 12. 덧셈 문자열 66+2+8+55+100을 계산하는 반복의 추적*

다섯 번째 반복에서는 더 이상 `+`가 없으므로 `find()`가 실패하고, `substr(10)`이 남은 문자열 전체 "100"을 돌려준다. 입력이 `2+3+`처럼 `+`로 끝나면 마지막 `part`가 빈 문자열이 되므로, 빈 문자열을 `stoi()`에 넘기지 않도록 그 전에 반복을 끝낸다.

`find()`의 리턴 타입은 부호 없는 정수이고 실패하면 `string::npos`를 리턴한다(7.2절). 이 코드는 리턴값을 `int` 변수에 저장한 뒤 -1과 비교하는데, 흔히 쓰는 플랫폼에서는 `npos`를 `int`로 바꾼 값이 -1이 되어 의도대로 동작한다. 타입을 정확히 맞추려면 `size_t fIndex = s.find('+', startIndex);`로 받고 `fIndex == string::npos`로 검사한다.

### 7.5 문자열의 검색과 대치

`find()`와 `replace()`를 반복하면 문자열 안에 나타나는 모든 단어를 다른 단어로 바꿀 수 있다. 다음 프로그램은 `&`가 입력될 때까지 여러 줄을 입력받은 뒤, 찾을 문자열과 바꿀 문자열을 입력받아 모두 바꾼다.

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cout << "여러 줄의 문자열을 입력하세요. 입력의 끝은 &문자입니다." << endl;
    getline(cin, s, '&');                      // &가 나올 때까지 여러 줄을 입력
    cin.ignore();                              // & 뒤에 따라오는 <Enter> 키를 제거
    string f, r;
    cout << endl << "find: ";
    getline(cin, f, '\n');                     // 검색할 문자열 입력
    cout << "replace: ";
    getline(cin, r, '\n');                     // 대치할 문자열 입력
    int startIndex = 0;
    while (true) {
        int fIndex = s.find(f, startIndex);    // startIndex부터 문자열 f 검색
        if (fIndex == -1)
            break;                             // 문자열 s의 끝까지 변경하였음
        s.replace(fIndex, f.length(), r);      // fIndex부터 f의 길이만큼을 r로 변경
        startIndex = fIndex + r.length();
    }
    cout << s << endl;
    return 0;
}
```

```text
여러 줄의 문자열을 입력하세요. 입력의 끝은 &문자입니다.
It's now or never, come hold me tight. Kiss me my darling, be mine tonight
Tomorrow will be too late. It's now or never, my love won't wait&

find: now
replace: Right Now
It's Right Now or never, come hold me tight. Kiss me my darling, be mine tonight
Tomorrow will be too late. It's Right Now or never, my love won't wait
```

- `getline(cin, s, '&')`는 세 번째 인자로 준 문자 `&`를 만날 때까지 줄바꿈을 포함한 모든 문자를 `s`에 저장한다. `&` 자체는 입력에서 꺼내지지만 `s`에 들어가지 않는다.
- `&` 뒤에 친 Enter 키의 줄바꿈 문자는 입력 버퍼에 남아 있다. 이것을 `cin.ignore()`로 하나 버리지 않으면, 다음의 `getline(cin, f, '\n')`이 그 줄바꿈을 바로 읽어 `f`가 빈 문자열이 된다.
- `replace(fIndex, f.length(), r)`는 `fIndex` 위치부터 `f`의 길이만큼을 `r`로 바꾼다. 다음 검색은 바꾼 문자열 `r`의 바로 뒤인 `fIndex + r.length()`부터 시작한다. `startIndex`를 `fIndex + 1`로 두면, 이 예처럼 `r`("Right Now")이 `f`("now")를 포함하지 않을 때는 문제가 없지만, `r`이 `f`를 포함하면 방금 바꾼 부분 안에서 `f`를 다시 찾아 끝없이 바꾸게 된다.

### 7.6 string 연산이 하는 일과 비용

`string`의 연산은 모두 간단한 기호와 함수 이름으로 쓰지만, 내부에서 하는 일의 양은 서로 다르다. 문자열의 길이를 `n`이라 하면 다음과 같이 정리할 수 있다.

| 연산 | 내부에서 하는 일 | 일의 양 |
|:----|:----|:----|
| `s.size()`, `s.length()` | 객체가 저장해 둔 길이를 읽는다 | 길이와 무관하게 일정 |
| `s[i]`, `s.at(i)` | `i`번째 문자의 위치를 계산한다 | 일정 |
| `s += 'x'` | 끝에 한 글자를 덧붙이고, 공간이 모자라면 더 큰 공간으로 옮긴다 | 보통 일정, 옮길 때는 `n`에 비례 |
| `s.insert(0, "ab")`, `s.erase(0, 2)` | 위치 뒤의 문자를 모두 밀거나 당긴다 | `n`에 비례 |
| `s.find("ab")` | 앞에서부터 차례로 비교한다 | 단순한 구현에서 최악의 경우 `n`과 찾는 문자열 길이의 곱에 비례 |
| `s1 + s2` | 두 문자열을 담을 새 객체를 만들어 복사한다 | 두 길이의 합에 비례 |

*표 13. string 연산의 내부 동작과 일의 양*

C 스트링의 `strlen()`은 널 문자를 찾을 때까지 문자를 하나씩 세므로 길이에 비례하는 시간이 걸리지만, `string`의 일반적인 구현은 길이를 멤버로 저장해 두므로 `size()`가 길이와 무관하게 바로 리턴된다. 반복문의 조건에 `strlen(s)`를 쓰면 반복할 때마다 문자열 전체를 다시 세는 반면, `s.size()`는 그런 비용이 없다.

`+=`가 덧붙일 공간을 어떻게 마련하는지는 용량 `capacity()`의 변화로 볼 수 있다.

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    size_t lastCap = s.capacity();
    cout << "처음: size=" << s.size() << " capacity=" << lastCap << endl;
    for (int i = 0; i < 100; i++) {
        s += 'x';                                        // 한 글자씩 덧붙인다
        if (s.capacity() != lastCap) {                   // 용량이 바뀐 순간만 출력
            lastCap = s.capacity();
            cout << "size=" << s.size() << "에서 capacity=" << lastCap << endl;
        }
    }
    return 0;
}
```

```text
처음: size=0 capacity=22
size=23에서 capacity=47
size=48에서 capacity=95
size=96에서 capacity=191
```

이 구현(macOS의 libc++)에서는 빈 문자열도 22글자까지 담을 공간을 가지고 있고, 공간이 모자랄 때마다 용량을 약 두 배로 늘린다. 100글자를 덧붙이는 동안 더 큰 공간으로 옮기는 일은 세 번만 일어나고, 나머지 97번은 이미 있는 공간에 한 글자를 쓰기만 한다. 그래서 `+=`를 여러 번 해도 전체 비용은 덧붙인 글자 수에 대략 비례한다. 처음 용량과 늘리는 비율은 구현마다 다르며, 덧붙일 글자 수를 미리 알면 `s.reserve(100)`으로 공간을 한 번에 확보할 수 있다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:----|:---------|
| 객체 포인터 | `ClassName *p = &obj;`로 선언하고 `p->member()` 또는 `(*p).member()`로 멤버에 접근한다. |
| `this` 포인터 | 현재 객체를 가리키는 암시적 포인터이며, `this->member`로 매개변수와 멤버의 이름 충돌을 해소한다. |
| `this`의 제약 | `this`는 non-static 멤버 함수에만 있으며 전역 함수와 static 멤버 함수에서는 쓸 수 없다. |
| `new` / `delete` | 힙 메모리를 할당하고 해제하며, `new`는 생성자를, `delete`는 소멸자를 호출한다. |
| 동적 객체 | `new ClassName(args)`로 힙에 생성하고 `delete p`로 소멸시킨다. |
| 동적 배열 | `new Type[n]`으로 할당한 배열은 `delete[] arr`로 해제하며, `delete`와 `new[]`를 섞어 쓰면 안 된다. |
| 정적 객체 배열 | `ClassName arr[n];`은 각 원소에 기본 생성자를 호출하며, 중괄호 초기화 목록으로 원소별 생성자를 지정할 수 있다. |
| 생성과 소멸 순서 | 객체 배열의 원소는 인덱스 순서로 생성되고 그 반대 순서로 소멸한다. |
| 2차원 객체 배열 | `ClassName arr[r][c];`로 선언하고 `arr[i][j]`로 접근하며, 행마다 중괄호로 묶어 초기화할 수 있다. |
| 동적 객체 배열 | 초기화 목록 없는 `new ClassName[n]`은 기본 생성자가 필요하며 원소는 `p[i]`로 접근한다. |
| 메모리 누수 | `delete`를 빠뜨리거나 포인터를 잃으면 그 힙 메모리를 프로그램 종료 전까지 회수할 수 없다. |
| 댕글링 포인터 | 해제한 메모리를 가리키는 포인터를 사용하면 정의되지 않은 동작이 되므로, 해제 후 `nullptr`를 대입한다. |
| 소멸자의 책임 | 생성자에서 `new`를 사용하면 소멸자에서 `delete`로 해제해야 한다. |
| 예외 처리 | `throw`로 예외를 발생시키고 `try { } catch (type e) { }`로 처리하며, 표준 `new`는 실패 시 `bad_alloc`을 던진다. |
| RAII | 자원 획득은 초기화이다라는 뜻으로, 자원의 수명을 객체의 수명에 묶어 소멸자에서 자동으로 해제한다. |
| `unique_ptr` | 단독 소유를 표현하며 복사할 수 없고 이동만 가능하고, 스코프를 벗어나면 자동으로 삭제되며 `make_unique`로 만드는 것이 권장된다. |
| `shared_ptr` | 참조 카운트로 소유권을 공유하며 마지막 소유자가 소멸할 때 자동으로 삭제되고, `make_shared`로 만드는 것이 권장된다. |
| 스마트 vs 직접 포인터 | 현대 C++에서는 스마트 포인터(C++11)가 직접 `new`/`delete`를 대체하며, 직접 `new`/`delete`는 레거시 스타일이다. |
| `string` 클래스 | `<string>`의 가변 크기 문자열 클래스로, `getline()`으로 한 줄을 입력받고 `find()`, `substr()`, `replace()` 같은 멤버 함수와 `+`, `==`, `<` 같은 연산자를 제공한다. |
| 객체 배열의 생성과 소멸 | 원소는 첨자 순서대로 생성되고 역순으로 소멸하며, 초기화 목록에 없는 원소는 기본 생성자로 생성된다. |
| delete의 오용 | `new`로 받지 않은 메모리의 `delete`, 이중 `delete`, `new[]`와 `delete`의 혼용은 정의되지 않은 동작이며, AddressSanitizer와 컴파일러 경고로 찾을 수 있다. |
| 덧셈 문자열의 계산 | `find()`로 `+`의 위치를 찾고 `substr()`로 숫자 부분을 잘라 `stoi()`로 정수로 바꾸어 더한다. |
| 검색과 대치 | `find()`와 `replace()`를 반복하되, 다음 검색은 바꾼 문자열의 바로 뒤에서 시작한다. |
| 연쇄 호출 | 멤버 함수가 `*this`를 참조로 리턴하면 `p.setX(3).setY(4)`처럼 같은 객체에 이어서 호출할 수 있고, 값으로 리턴하면 둘째 호출은 복사본에 적용된다. |
| new와 malloc()의 차이 | `new`는 타입에 맞는 크기를 할당하고 생성자를 호출하며 `delete`는 소멸자를 호출하지만, `malloc()`과 `free()`는 메모리만 다루므로 C++ 객체에는 쓰지 않는다. |
| 객체 배열의 메모리 배치 | 원소는 `sizeof(클래스)`씩 연속으로 놓이고, `p + i`는 `i`개 원소만큼 떨어진 원소를 가리킨다. |
| string 연산의 비용 | `size()`와 `[]`는 길이와 무관하게 일정하고, `insert()`, `erase()`, `+`는 길이에 비례하며, `+=`는 용량을 늘려 가며 덧붙인다. |
| 객체 포인터와 const | `const Circle *p`는 가리키는 객체를 바꿀 수 없어 `const` 멤버 함수만 호출할 수 있고, `Circle *const q`는 포인터 자체를 다른 객체로 바꿀 수 없다. |

---

<br>

## 점검 문제

1. **배열 해제:** `Circle *p = new Circle[3];`으로 할당한 배열을 `delete p;`로 해제하면 어떤 문제가 생기는가?

   <details>
   <summary>정답</summary>

   `new[]`로 할당한 메모리를 `delete`로 해제하면 정의되지 않은 동작이다. 원소 3개의 소멸자가 모두 호출된다는 보장이 없고 메모리 반환도 올바르지 않을 수 있다. `delete[] p;`로 해제해야 하며, 그러면 `p[2]`, `p[1]`, `p[0]` 순서로 소멸자가 실행된 뒤 메모리가 반환된다.

   </details>

2. **기본 생성자:** `Circle` 클래스에 `Circle(int r)` 생성자만 선언되어 있을 때 `Circle arr[3];`과 `new Circle[3]`은 컴파일되는가? 컴파일되지 않는다면 두 가지 해결 방법을 쓰라.

   <details>
   <summary>정답</summary>

   둘 다 컴파일 오류이다. 생성자를 하나라도 선언하면 컴파일러가 기본 생성자를 자동으로 만들지 않으므로, 각 원소를 만들 기본 생성자가 없다. 해결 방법은 (1) `Circle() { radius = 1; }` 같은 기본 생성자를 추가하거나, (2) `Circle arr[3] = { Circle(1), Circle(2), Circle(3) };` 또는 C++11의 `new Circle[3]{ Circle(1), Circle(2), Circle(3) }`처럼 원소마다 생성자를 지정하는 것이다.

   </details>

3. **생성과 소멸 순서:** 생성자와 소멸자에서 반지름을 출력하는 `Circle` 클래스로 `{ Circle c[3] = { Circle(7), Circle(8), Circle(9) }; }` 블록을 실행하면 생성자와 소멸자는 어떤 순서로 실행되는가?

   <details>
   <summary>정답</summary>

   생성자는 `c[0]`, `c[1]`, `c[2]`의 순서로 반지름 7, 8, 9에 대해 실행되고, 블록을 벗어날 때 소멸자는 반대 순서인 반지름 9, 8, 7의 순서로 실행된다.

   </details>

4. **this의 제약:** static 멤버 함수 안에서 `this`를 사용할 수 없는 이유는 무엇인가?

   <details>
   <summary>정답</summary>

   `this`는 멤버 함수를 호출한 객체의 주소가 전달되는 묵시적 매개변수이다. static 멤버 함수는 `ClassName::f()`처럼 객체 없이도 호출될 수 있으므로 전달될 객체 주소가 없고, 따라서 `this`가 존재하지 않는다.

   </details>

5. **string 멤버 함수:** `string s = "I love love C++";`일 때 `s.find("love")`, `s.find("love", 3)`, `s.rfind("love")`, `s.substr(2, 4)`의 결과를 쓰라.

   <details>
   <summary>정답</summary>

   `s.find("love")`는 처음 나타나는 위치인 2, `s.find("love", 3)`은 인덱스 3부터 검색하므로 두 번째 `love`의 위치인 7, `s.rfind("love")`는 마지막으로 나타나는 위치인 7, `s.substr(2, 4)`는 인덱스 2부터 4개 문자인 `"love"`이다.

   </details>

6. **덧셈 문자열 추적:** 7.4절의 프로그램에 `10+5+`를 입력하면 출력되는 줄과 합은 무엇인가? 반복이 끝나는 이유와 함께 쓰라.

   <details>
   <summary>정답</summary>

   10과 5가 한 줄씩 출력되고 합은 15이다. 첫 반복에서 `fIndex`는 2이므로 "10"을 더하고 `startIndex`가 3이 된다. 둘째 반복에서 `fIndex`는 4이므로 "5"를 더하고 `startIndex`가 5가 된다. 셋째 반복에서는 `+`가 더 없으므로 `substr(5)`를 구하는데, 문자열의 길이가 5이므로 빈 문자열이 되어 `part == ""` 조건으로 반복이 끝난다.

   </details>

7. **동적 객체 배열:** 3.4절의 `Circle`로 `Circle *p = new Circle[3]; delete p;`를 실행하면 어떤 문제가 생기는가?

   <details>
   <summary>정답</summary>

   `new[]`로 할당한 메모리를 `delete`로 반환했으므로 정의되지 않은 동작이다. `delete`는 배열의 원소 수를 고려하지 않으므로 소멸자가 세 번 호출된다는 보장이 없고, 메모리 반환 자체도 잘못될 수 있다. `delete[] p;`로 반환해야 세 원소의 소멸자가 역순으로 호출되고 메모리가 올바르게 반환된다.

   </details>

8. **연쇄 호출:** 1.3절의 `Point`에서 `setY()`의 리턴 타입을 `Point`(값)로 바꾸면 `Point q; q.setX(1).setY(2).setX(5); q.show();`의 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   `(1, 2)`이다. `q.setX(1)`은 `q`의 참조를 리턴하므로 `setY(2)`는 `q`에 적용되어 `q`는 (1, 2)가 된다. 그런데 `setY()`가 값을 리턴하므로 그 결과는 `q`의 복사본이고, 마지막 `setX(5)`는 이 복사본에 적용되어 `q`에는 영향을 주지 않는다.

   </details>

9. **포인터 연산:** `Box` 객체의 크기가 16바이트이고 `Box arr[4];`의 시작 주소가 0x1000이라 하자. `Box *p = arr + 2;`의 값과 `p - arr`의 값은 무엇인가?

   <details>
   <summary>정답</summary>

   `p`는 0x1000 + 2 × 16 = 0x1020이고, `p - arr`은 2이다. 포인터에 정수를 더하면 원소의 크기를 곱한 바이트 수만큼 주소가 움직이고, 같은 배열을 가리키는 두 포인터의 차이는 바이트 수가 아니라 원소 수로 계산된다.

   </details>

10. **new와 malloc:** 2.4절의 `Counter`에 `int get() const { return value; }`를 추가하고 `Counter *q = (Counter *)malloc(sizeof(Counter)); cout << q->get();`을 실행하면 100이 출력된다고 보장할 수 있는가?

    <details>
    <summary>정답</summary>

    보장할 수 없다. `malloc()`은 메모리만 할당하고 생성자를 호출하지 않으므로 `value`는 100으로 초기화되지 않았고, 그 메모리에는 정해지지 않은 값이 들어 있다. 생성자가 실행되지 않은 메모리를 객체처럼 쓰는 것이므로 결과는 실행할 때마다 다를 수 있다. 생성자가 실행되게 하려면 `Counter *q = new Counter;`로 만들고 `delete q;`로 반환한다.

    </details>

11. **const 포인터:** `void print(const Circle *p)` 안에서 `p->getRadius()`와 `p->setRadius(3)`, `p = nullptr;` 중 컴파일되는 것을 모두 고르라. 1.4절의 `Circle`을 쓴다.

    <details>
    <summary>정답</summary>

    `p->getRadius()`와 `p = nullptr;`이 컴파일된다. `p`는 `const Circle`을 가리키므로 `const` 멤버 함수인 `getRadius()`만 호출할 수 있고, 비`const` 멤버 함수 `setRadius()`는 호출할 수 없다. 포인터 변수 `p` 자체는 상수가 아니므로 다른 주소를 대입할 수 있지만, `p`는 값으로 받은 매개변수이므로 그 대입은 호출한 쪽의 포인터에 영향을 주지 않는다.

    </details>
