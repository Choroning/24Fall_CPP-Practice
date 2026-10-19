# 9장 — 가상함수와 추상 클래스

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 9

> **학습 목표:**
> 1. 상속 관계에서 함수 재정의(하이딩)와 오버라이딩을 구분할 수 있다
> 2. 가상함수가 동적 바인딩을 일으키는 원리를 vtable과 vptr로 설명할 수 있다
> 3. 가상 소멸자가 필요한 이유를 설명하고 올바르게 선언할 수 있다
> 4. 순수 가상함수로 추상 클래스를 정의하고 이를 상속하는 구체 클래스를 구현할 수 있다
> 5. 기본 클래스 포인터 배열과 가상함수를 활용하여 다형적인 프로그램을 작성할 수 있다

---

## 목차

- [1. 업캐스팅과 다운캐스팅](#1-업캐스팅과-다운캐스팅)
  - [1.1 업캐스팅](#11-업캐스팅)
  - [1.2 다운캐스팅](#12-다운캐스팅)
- [2. 가상함수](#2-가상함수)
  - [2.1 왜 가상함수가 필요한가?](#21-왜-가상함수가-필요한가)
  - [2.2 virtual 없는 경우의 문제점](#22-virtual-없는-경우의-문제점)
  - [2.3 virtual 키워드](#23-virtual-키워드)
  - [2.4 정적 바인딩 vs 동적 바인딩](#24-정적-바인딩-vs-동적-바인딩)
  - [2.5 동작 원리: vtable과 vptr](#25-동작-원리-vtable과-vptr)
  - [2.6 override 키워드](#26-override-키워드)
  - [2.7 오버라이딩의 성공 조건](#27-오버라이딩의-성공-조건)
  - [2.8 가상함수의 상속](#28-가상함수의-상속)
  - [2.9 final 지시어](#29-final-지시어)
  - [2.10 범위 지정 연산자를 이용한 기본 클래스 가상함수 호출](#210-범위-지정-연산자를-이용한-기본-클래스-가상함수-호출)
- [3. 가상 소멸자](#3-가상-소멸자)
  - [3.1 문제점](#31-문제점)
  - [3.2 완전한 예제: 메모리 누수](#32-완전한-예제-메모리-누수)
  - [3.3 원칙](#33-원칙)
- [4. 추상 클래스](#4-추상-클래스)
  - [4.1 순수 가상함수](#41-순수-가상함수)
  - [4.2 추상 클래스](#42-추상-클래스)
  - [4.3 구체 클래스](#43-구체-클래스)
  - [4.4 추상 클래스 구현 예제](#44-추상-클래스-구현-예제)
- [5. 다형성](#5-다형성)
  - [5.1 개념](#51-개념)
  - [5.2 완전한 Shape 계층 구조 예제](#52-완전한-shape-계층-구조-예제)
  - [5.3 완전한 Converter 예제](#53-완전한-converter-예제)
- [6. 흔한 실수](#6-흔한-실수)
  - [6.1 가상 소멸자 누락](#61-가상-소멸자-누락)
  - [6.2 추상 클래스 인스턴스화 시도](#62-추상-클래스-인스턴스화-시도)
  - [6.3 순수 가상함수 오버라이딩 누락](#63-순수-가상함수-오버라이딩-누락)
  - [6.4 객체 슬라이싱](#64-객체-슬라이싱)
  - [6.5 오버라이딩 vs 오버로딩 vs 하이딩](#65-오버라이딩-vs-오버로딩-vs-하이딩)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 업캐스팅과 다운캐스팅

### 1.1 업캐스팅

**업캐스팅(Upcasting)** 은 파생 클래스의 포인터(또는 참조)를 기본 클래스의 포인터(또는 참조)로 변환하는 것이다. 모든 파생 객체는 기본 객체의 일종(is-a 관계)이므로 **암시적**으로 변환되며, 항상 **안전**하다.

```cpp
class Base { };
class Derived : public Base { };

Derived d;
Base* p = &d;       // 업캐스팅 — 암시적, 항상 안전
Base& r = d;        // 참조를 이용한 업캐스팅
```

기본 클래스 포인터 `p`를 통해서는 **기본 클래스의 멤버에만 접근할 수 있다**. 실제 객체가 `Derived`이더라도 컴파일러는 `Base*` 타입만 보고 접근 범위를 제한한다.

```cpp
class Shape {
public:
    void draw() { cout << "Shape" << endl; }
};

class Circle : public Shape {
public:
    void draw() { cout << "Circle" << endl; }
    double getRadius() { return 5.0; }
};

Circle c;
Shape* p = &c;      // 업캐스팅
p->draw();           // "Shape" — Shape::draw() 호출 (정적 바인딩)
// p->getRadius();   // 컴파일 오류 — Shape에 getRadius()가 없음
```

> **핵심:** 업캐스팅 후에는 포인터 타입에 따라 접근 가능한 멤버가 결정된다. 파생 클래스에만 있는 멤버는 기본 클래스 포인터로 접근할 수 없다.

### 1.2 다운캐스팅

**다운캐스팅(Downcasting)** 은 기본 클래스 포인터를 파생 클래스 포인터로 변환하는 것이다. **명시적 캐스트**가 필요하며, 실제 객체가 해당 파생 타입이 아닌 경우 **위험**하다.

```cpp
Base* p = new Derived;
Derived* dp = (Derived*)p;   // C 스타일 다운캐스트 — 컴파일되지만 위험
```

**안전한 다운캐스팅**을 위해 `dynamic_cast`를 사용한다. 런타임에 타입을 검사하고, 포인터 변환이 불가능하면 `nullptr`을 반환한다(기본 클래스에 가상함수가 하나 이상 있어야 함). 참조로 변환할 때 실패하면 `std::bad_cast` 예외가 발생한다.

```cpp
Base* p = new Base;
Derived* dp = dynamic_cast<Derived*>(p);  // 런타임 검사
if (dp == nullptr) {
    cout << "캐스트 실패 — p가 Derived를 가리키지 않음" << endl;
}
```

> **주의:** `static_cast`나 C 스타일 캐스트로 다운캐스팅하면 런타임 검사를 건너뛴다. 실제 객체가 파생 타입이 아닌 경우 파생 멤버에 접근하면 **정의되지 않은 동작(undefined behavior)** 이 발생한다.

---

<br>

## 2. 가상함수

### 2.1 왜 가상함수가 필요한가?

그림판 프로그램을 예로 든다. `Shape` 기본 클래스가 있고, `Circle`, `Rectangle`, `Triangle` 같은 파생 클래스가 있다. 모든 도형을 `Shape*` 포인터 배열에 저장하고, 하나의 반복문으로 모두 그리려고 한다:

```cpp
Shape* canvas[100];
canvas[0] = new Circle(10, 20, 5);
canvas[1] = new Rectangle(0, 0, 30, 40);
canvas[2] = new Triangle(5, 5, 10, 20, 15, 5);
// ...

for (int i = 0; i < count; i++) {
    canvas[i]->draw();   // 어떤 draw()가 호출될까?
}
```

**virtual 없이:** `canvas[i]->draw()` 호출 시 항상 `Shape::draw()`가 호출된다. 컴파일러는 `Shape*` 타입만 보기 때문이다. 실제 객체가 `Circle`이든 아니든 상관없이, 컴파일 시점에 이미 `Shape::draw()`를 호출하기로 결정했다. 모든 도형이 똑같은 것을 그리므로 도형마다 다른 그림을 그린다는 목적을 이룰 수 없다.

**virtual 있으면:** `canvas[i]->draw()` 호출 시 실제 객체에 맞는 **올바른** `draw()`가 호출된다. 원이면 `Circle::draw()`, 사각형이면 `Rectangle::draw()` 등이다. 런타임에 실제 객체 타입에 따라 결정된다.

이것이 **OOP 다형성의 핵심**이다: 기본 클래스 포인터로 코드를 작성하면서도, 각 파생 타입에 맞는 동작이 자동으로 수행된다. 가상함수 없이는 상속이 단순한 코드 공유에 불과하지만, 가상함수가 있으면 상속이 **대체 가능성(substitutability)** 을 가능하게 한다.

### 2.2 virtual 없는 경우의 문제점

`virtual` 없이 함수를 선언하면, 호출되는 함수는 **포인터의 타입**에 의해 결정된다. 이를 **정적 바인딩(static binding)** 또는 **이른 바인딩(early binding)** 이라 하며, 컴파일 시점에 결정된다.

```cpp
class Base {
public:
    void func() { cout << "Base::func()" << endl; }
};

class Derived : public Base {
public:
    void func() { cout << "Derived::func()" << endl; }
};

Base* p = new Derived;
p->func();    // 출력: "Base::func()" — 정적 바인딩!
delete p;
```

포인터 타입이 `Base*`이므로, `p`가 `Derived` 객체를 가리키고 있더라도 `Base::func()`이 호출된다. 이는 다형성의 목적을 달성하지 못한다.

교재는 이처럼 기본 클래스의 비가상 함수와 원형이 같은 함수를 파생 클래스에 다시 작성하는 것을 **함수 재정의(redefine)** 라고 부른다. 이때 파생 객체 안에는 두 `func()`가 모두 존재하며, `Derived*`로 호출하면 `Derived::func()`이, `Base*`로 호출하면 `Base::func()`이 실행된다. 6.5절에서는 이를 하이딩으로 다룬다.

### 2.3 virtual 키워드

기본 클래스 함수에 `virtual`을 붙이면 **동적 바인딩(dynamic binding)** 또는 **늦은 바인딩(late binding)** 이 활성화된다. 런타임에 실제 객체의 타입에 따라 호출할 함수가 결정된다.

```cpp
class Base {
public:
    virtual void func() { cout << "Base::func()" << endl; }
};

class Derived : public Base {
public:
    void func() { cout << "Derived::func()" << endl; }  // 오버라이딩
};

Base* p = new Derived;
p->func();    // 출력: "Derived::func()" — 동적 바인딩!
delete p;
```

기본 클래스에서 한 번 `virtual`로 선언하면, 모든 파생 클래스에서 해당 함수는 자동으로 가상함수로 유지된다(명시적으로 `virtual`을 다시 쓰지 않아도 됨).

이처럼 파생 클래스에서 기본 클래스의 가상함수와 같은 원형의 함수를 다시 정의하는 것을 **함수 오버라이딩(function overriding)** 이라 하며, 다형성의 한 형태이다. 오버라이딩한 뒤에도 파생 객체 안에는 `Base::func()`이 남아 있지만, 기본 클래스 포인터로 호출해도 항상 `Derived::func()`이 실행되므로 기본 클래스의 함수는 존재감을 잃는다. 파생 객체에서 기본 클래스의 버전을 호출하려면 2.10절의 범위 지정 연산자를 써야 한다.

### 2.4 정적 바인딩 vs 동적 바인딩

정적 바인딩과 동적 바인딩의 차이를 이해하는 것이 핵심이다. 두 방식은 호출할 함수를 **언제** 결정하는지가 다르다.

**정적 바인딩 (컴파일 타임 바인딩)**
- 컴파일러가 **포인터/참조의 타입**을 기준으로 호출할 함수를 결정한다.
- 비가상(non-virtual) 함수의 기본 동작이다.
- 빠르지만(런타임 조회 없음), 실제 객체 타입을 무시한다.

**동적 바인딩 (런타임 바인딩)**
- 프로그램이 런타임에 **실제 객체 타입**을 기준으로 호출할 함수를 결정한다.
- `virtual` 키워드로 활성화된다.
- 약간 느리지만(vtable 조회), 진정한 다형성을 가능하게 한다.

`virtual` 키워드는 함수를 정적 바인딩에서 동적 바인딩으로 전환하는 스위치이다.

**나란히 비교(같은 코드, 다른 동작):**

```cpp
#include <iostream>
using namespace std;

// ============ virtual 없음 (정적 바인딩) ============
class AnimalStatic {
public:
    void speak() { cout << "..." << endl; }
};
class DogStatic : public AnimalStatic {
public:
    void speak() { cout << "멍멍!" << endl; }
};
class CatStatic : public AnimalStatic {
public:
    void speak() { cout << "야옹!" << endl; }
};

// ============ virtual 있음 (동적 바인딩) ============
class AnimalDynamic {
public:
    virtual void speak() { cout << "..." << endl; }
    virtual ~AnimalDynamic() { }
};
class DogDynamic : public AnimalDynamic {
public:
    void speak() override { cout << "멍멍!" << endl; }
};
class CatDynamic : public AnimalDynamic {
public:
    void speak() override { cout << "야옹!" << endl; }
};

int main() {
    // --- 정적 바인딩 ---
    AnimalStatic* s1 = new DogStatic;
    AnimalStatic* s2 = new CatStatic;
    s1->speak();   // "..."  (틀림! AnimalStatic::speak 호출)
    s2->speak();   // "..."  (틀림! AnimalStatic::speak 호출)
    delete s1; delete s2;

    // --- 동적 바인딩 ---
    AnimalDynamic* d1 = new DogDynamic;
    AnimalDynamic* d2 = new CatDynamic;
    d1->speak();   // "멍멍!"  (정확! DogDynamic::speak 호출)
    d2->speak();   // "야옹!"  (정확! CatDynamic::speak 호출)
    delete d1; delete d2;

    return 0;
}
```

출력:

```text
...
...
멍멍!
야옹!
```

> **핵심:** 유일한 차이는 기본 클래스의 `virtual` 키워드뿐이다. 상속, 포인터 타입, 함수 호출 등 나머지는 모두 동일하다. `virtual`이 다형성을 작동시키는 것이다.

### 2.5 동작 원리: vtable과 vptr

C++ 컴파일러는 일반적으로 동적 바인딩을 **vtable(가상함수 테이블)** 메커니즘으로 구현한다. C++ 표준이 이 방식을 정하지는 않지만, 주요 컴파일러가 모두 이 방식을 쓴다:

1. **vtable:** 가상함수를 가진 각 클래스는 함수 포인터 테이블을 갖는다. 이 테이블은 각 가상함수를 해당 클래스의 올바른 구현과 매핑한다.
2. **vptr:** 가상함수를 가진 클래스의 각 객체는 자신의 클래스 vtable을 가리키는 숨겨진 포인터(`vptr`)를 포함한다.
3. **런타임 조회:** 기본 클래스 포인터로 가상함수를 호출하면, `vptr`을 따라 vtable에서 함수를 찾아 올바른 버전을 호출한다.

**간단한 vtable 도식 (가상함수 1개):**

```text
┌────────────────┐         ┌──────────────────────┐
│  Base 객체      │         │   Base vtable        │
│  ┌──────────┐  │         │   func → Base::func  │
│  │  vptr   ─┼──┼────────▶│                      │
│  └──────────┘  │         └──────────────────────┘
└────────────────┘

┌────────────────┐         ┌───────────────────────────┐
│ Derived 객체    │         │   Derived vtable          │
│  ┌──────────┐  │         │   func → Derived::func    │
│  │  vptr   ─┼──┼────────▶│                           │
│  └──────────┘  │         └───────────────────────────┘
└────────────────┘
```

*그림 1. 가상함수가 하나인 Base 객체와 Derived 객체의 vptr과 vtable*

**상세 vtable 도식(여러 가상함수를 가진 Shape 계층 구조):**

`class Shape`에 `virtual draw()`와 `virtual area()`가 있고, 파생 클래스 `Circle`과 `Rectangle`이 있는 경우:

```text
 Shape* p = new Circle(5.0);
 p->draw();   // C++는 Circle::draw()를 어떻게 찾는가?

 1단계: p를 따라 Circle 객체로 이동
 2단계: 객체에서 숨겨진 vptr을 읽음
 3단계: vptr을 따라 Circle의 vtable로 이동
 4단계: vtable에서 draw()를 조회 → Circle::draw
 5단계: Circle::draw() 호출

 Circle 객체 (메모리):               Circle의 vtable:
 ┌──────────────────┐             ┌──────────────────────────┐
 │  vptr ───────────┼────────────▶│ draw → Circle::draw      │
 │  radius = 5.0    │             │ area → Circle::area      │
 └──────────────────┘             │ ~dtor → Circle::~Circle  │
                                  └──────────────────────────┘

 Rectangle 객체 (메모리):            Rectangle의 vtable:
 ┌──────────────────┐             ┌───────────────────────────────┐
 │  vptr ───────────┼────────────▶│ draw → Rectangle::draw        │
 │  width = 3.0     │             │ area → Rectangle::area        │
 │  height = 4.0    │             │ ~dtor → Rectangle::~Rectangle │
 └──────────────────┘             └───────────────────────────────┘

 Shape 객체 (메모리):                Shape의 vtable:
 ┌──────────────────┐             ┌──────────────────────────┐
 │  vptr ───────────┼────────────▶│ draw → Shape::draw       │
 └──────────────────┘             │ area → Shape::area       │
                                  │ ~dtor → Shape::~Shape    │
                                  └──────────────────────────┘
```

*그림 2. Shape 계층 구조에서 가상함수 호출을 찾는 단계와 클래스별 vtable*

> **핵심:** **vtable은 클래스당 하나**(해당 클래스의 모든 객체가 공유)이지만, **vptr은 객체당 하나**(각 객체가 자기 클래스의 vtable을 가리키는 포인터를 갖고 있음)이다. `Circle`을 생성하면 vptr이 `Circle`의 vtable을 가리키고, `Rectangle`을 생성하면 vptr이 `Rectangle`의 vtable을 가리킨다. 이것이 같은 `p->draw()` 호출이 실제 객체에 따라 다른 결과를 내는 이유이다.

> **참고:** vtable 메커니즘은 객체당 포인터 하나, 가상함수 호출 시 간접 호출 한 번의 약간의 오버헤드를 추가한다. 이것이 런타임 다형성의 비용이다.

### 2.6 override 키워드

C++11에서 도입된 `override` 키워드는 파생 클래스 함수가 기본 클래스의 가상함수를 오버라이딩한다는 것을 명시한다. 컴파일러가 기본 클래스에 일치하는 가상함수가 있는지 확인하여, 이름 오타나 매개변수 불일치를 컴파일 시점에 잡아준다.

```cpp
class Base {
public:
    virtual void func(int x) { }
};

class Derived : public Base {
public:
    void func(int x) override { }    // OK — Base::func(int)와 일치
    // void func(double x) override; // 컴파일 오류 — 일치하는 기본 함수 없음
    // void Fun(int x) override;     // 컴파일 오류 — 이름 오타
};
```

> **참고:** **권장 사항.** 가상함수를 오버라이딩할 때는 항상 `override`를 쓰는 것이 좋다. 런타임 비용 없이 컴파일 시점 안전장치를 제공한다.

예를 들어 `Shape`의 `virtual void draw()`를 오버라이딩하려다 `Rect`에 `void drow()`라고 잘못 쓰면, `override`가 없을 때는 새 함수가 추가될 뿐 오류가 나지 않으므로 `Shape*`로 호출한 `draw()`는 `Shape::draw()`를 실행한다. `void drow() override;`라고 쓰면 일치하는 가상함수가 없으므로 컴파일 오류로 실수가 드러난다.

### 2.7 오버라이딩의 성공 조건

오버라이딩이 성립하려면 파생 클래스 함수의 **이름, 매개변수의 타입과 개수, 반환 타입**이 기본 클래스의 가상함수와 모두 일치해야 한다.

```cpp
class Base {
public:
    virtual void fail();
    virtual void success();
    virtual void g(int);
};

class Derived : public Base {
public:
    virtual int fail();           // 오버라이딩 실패: 반환 타입이 달라 컴파일 오류
    virtual void success();       // 오버라이딩 성공
    virtual void g(int, double);  // 매개변수가 달라 새로운 함수: 정상 컴파일
};
```

- 반환 타입만 다른 `fail()`은 컴파일 오류이다. 다만 기본 클래스의 가상함수가 기본 클래스에 대한 포인터나 참조를 반환할 때 파생 클래스에서 파생 클래스에 대한 포인터나 참조를 반환하도록 바꾸는 **공변 반환 타입(covariant return type)** 은 허용된다.
- 가상함수의 `virtual` 키워드는 상속되므로 파생 클래스에서 생략할 수 있다. `Derived`의 `virtual void success();`를 `void success();`로 써도 같은 오버라이딩이다.
- 가상함수의 접근 지정은 `private`, `protected`, `public` 중 자유롭게 할 수 있다. 4.4절의 두 번째 `Calculator` 예제는 순수 가상함수 `calc()`를 `protected`로 둔다.

> **참고:** 교재는 `g(int, double)`을 오버로딩 사례로 소개한다. 그러나 기본 클래스와 파생 클래스는 스코프가 다르므로, 파생 클래스에 같은 이름의 함수를 선언하면 기본 클래스의 `g(int)`는 `Derived`의 스코프에서 가려진다(하이딩, 6.5절). 따라서 `Derived` 객체 `d`로 `d.g(1)`을 호출하면 컴파일 오류가 나고, `Base*`를 통해 `g(1)`을 호출해야 `Base::g(int)`가 실행된다. 두 함수를 오버로딩처럼 함께 쓰려면 `Derived` 안에 `using Base::g;`를 선언한다.

### 2.8 가상함수의 상속

가상함수는 상속 계층이 몇 단계이든 가상함수로 남는다. 다음 예제(교재 예제 9-3)에서 `Base`, `Derived`, `GrandDerived`는 차례로 상속 관계에 있고, 세 포인터가 모두 같은 `GrandDerived` 객체를 가리킨다.

```cpp
#include <iostream>
using namespace std;

class Base {
public:
    virtual void f() { cout << "Base::f() called" << endl; }
};

class Derived : public Base {
public:
    void f() { cout << "Derived::f() called" << endl; }
};

class GrandDerived : public Derived {
public:
    void f() { cout << "GrandDerived::f() called" << endl; }
};

int main() {
    GrandDerived g;
    Base* bp;
    Derived* dp;
    GrandDerived* gp;

    bp = dp = gp = &g;

    bp->f();   // 동적 바인딩
    dp->f();   // 동적 바인딩
    gp->f();
}
```

출력:

```text
GrandDerived::f() called
GrandDerived::f() called
GrandDerived::f() called
```

`Derived`와 `GrandDerived`의 `f()`에는 `virtual`이 없지만 `Base::f()`가 가상이므로 둘 다 가상함수이다. 따라서 포인터 타입이 `Base*`이든 `Derived*`이든 동적 바인딩으로 실제 객체인 `GrandDerived`의 `f()`가 실행된다.

파생 클래스가 가상함수를 오버라이딩하지 않으면, 상속 계층에서 가장 가까운 기본 클래스가 정의한 함수가 사용된다.

```cpp
#include <iostream>
using namespace std;

class B {
public:
    virtual void who() { cout << "Base\n"; }
};

class D1 : public B {
public:
    void who() { cout << "First derivation\n"; }
};

class D2 : public B {
    // who()를 오버라이딩하지 않는다.
};

int main() {
    B base_obj;
    B* p;
    D1 D1_obj;
    D2 D2_obj;

    p = &base_obj;
    p->who();   // B의 who() 호출
    p = &D1_obj;
    p->who();   // D1의 who() 호출
    p = &D2_obj;
    p->who();   // D2에 who()가 없으므로 B의 who() 호출
    return 0;
}
```

출력:

```text
Base
First derivation
Base
```

동적 바인딩은 기본 클래스의 멤버 함수 안에서 가상함수를 호출할 때도 일어난다. 다음 예제에서 `paint()`는 `Shape`의 비가상 함수이지만, 그 안의 `draw()` 호출은 `this->draw()`와 같으므로 `this`가 가리키는 실제 객체의 `draw()`가 실행된다.

```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    void paint() {
        draw();   // this->draw(): 동적 바인딩
    }
    virtual void draw() {
        cout << "Shape::draw() called" << endl;
    }
};

class Circle : public Shape {
public:
    virtual void draw() {
        cout << "Circle::draw() called" << endl;
    }
};

int main() {
    Circle circle;
    Shape* pShape = &circle;   // 업캐스팅
    pShape->paint();
}
```

출력:

```text
Circle::draw() called
```

`Shape` 객체에서 `paint()`를 호출하면 `Shape::draw() called`가 출력된다. 이처럼 기본 클래스의 코드가 파생 클래스에서 구현할 함수를 호출하게 할 수 있으며, 4.4절의 추상 클래스 예제가 이 성질을 이용한다.

### 2.9 final 지시어

C++11에서 도입된 `final` 지시어는 두 위치에 쓸 수 있다. `override`와 `final`은 정확히는 예약어가 아니라 특정 위치에서만 특별한 의미를 갖는 식별자이며, 교재는 둘을 지시어라고 부른다.

1. **가상함수의 원형 바로 뒤:** 파생 클래스는 그 가상함수를 오버라이딩할 수 없다.
2. **클래스 이름 뒤:** 다른 클래스가 그 클래스를 상속할 수 없다(상속 차단).

```cpp
class Shape {
public:
    virtual void draw() final;   // 더 이상 오버라이딩할 수 없음
};

class Rect : public Shape {
public:
    void draw();                 // 컴파일 오류: final 함수를 오버라이딩하려 함
};
```

```cpp
class Shape final {
    // ...
};

class Rect : public Shape {      // 컴파일 오류: Shape는 상속할 수 없음
    // ...
};
```

```cpp
class Shape {
    // ...
};

class Rect final : public Shape {   // Rect는 Shape를 상속할 수 있음
    // ...
};

class RoundRect : public Rect {     // 컴파일 오류: Rect는 상속할 수 없음
    // ...
};
```

### 2.10 범위 지정 연산자를 이용한 기본 클래스 가상함수 호출

범위 지정 연산자 `::`를 써서 `기본클래스::가상함수()` 형태로 호출하면 동적 바인딩이 일어나지 않고 정적 바인딩으로 기본 클래스의 함수가 실행된다. 파생 클래스에서 오버라이딩한 함수가 기본 클래스의 기능을 그대로 이용하면서 기능을 덧붙일 때 주로 쓴다.

```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw() {
        cout << "--Shape--";
    }
};

class Circle : public Shape {
public:
    virtual void draw() {
        Shape::draw();   // 기본 클래스의 draw() 호출 (정적 바인딩)
        cout << "Circle" << endl;
    }
};

int main() {
    Circle circle;
    Shape* pShape = &circle;

    pShape->draw();          // 동적 바인딩: Circle::draw() 실행
    pShape->Shape::draw();   // 정적 바인딩: Shape::draw()만 실행
}
```

출력:

```text
--Shape--Circle
--Shape--
```

`pShape->draw()`는 동적 바인딩으로 `Circle::draw()`를 실행하고, 그 안의 `Shape::draw()`가 정적 바인딩으로 `--Shape--`를 먼저 출력한다. `pShape->Shape::draw()`는 처음부터 정적 바인딩이므로 `--Shape--`만 출력한다.

---

<br>

## 3. 가상 소멸자

### 3.1 문제점

기본 클래스 포인터를 통해 파생 객체를 `delete`할 때, 소멸자가 **가상이 아니면** 일반적인 컴파일러에서는 기본 클래스 소멸자만 호출된다(C++ 표준은 이 경우를 정의되지 않은 동작으로 규정한다). 파생 클래스 소멸자가 호출되지 않아 파생 클래스에서 할당한 **자원이 누수**될 수 있다.

```cpp
class Base {
public:
    ~Base() { cout << "~Base" << endl; }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; cout << "~Derived" << endl; }
};

Base* p = new Derived;
delete p;    // 출력: "~Base"만! ~Derived 호출 안 됨 → 메모리 누수
```

### 3.2 완전한 예제: 메모리 누수

문제와 해결을 나란히 보여주는 완전한 실행 가능 프로그램:

```cpp
#include <iostream>
#include <cstring>
using namespace std;

// ============ 버그: 비가상 소멸자 ============
class BadBase {
public:
    ~BadBase() { cout << "[BadBase] 소멸자" << endl; }
};

class BadDerived : public BadBase {
    char* name;
public:
    BadDerived(const char* n) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        cout << "[BadDerived] 할당: " << name << endl;
    }
    ~BadDerived() {
        cout << "[BadDerived] 해제: " << name << endl;
        delete[] name;
    }
};

// ============ 수정: 가상 소멸자 ============
class GoodBase {
public:
    virtual ~GoodBase() { cout << "[GoodBase] 소멸자" << endl; }
};

class GoodDerived : public GoodBase {
    char* name;
public:
    GoodDerived(const char* n) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        cout << "[GoodDerived] 할당: " << name << endl;
    }
    ~GoodDerived() override {
        cout << "[GoodDerived] 해제: " << name << endl;
        delete[] name;
    }
};

int main() {
    cout << "--- 가상 소멸자 없음 (버그) ---" << endl;
    BadBase* bp = new BadDerived("leak");
    delete bp;   // ~BadBase만 실행! "leak"은 해제 안 됨
    cout << endl;

    cout << "--- 가상 소멸자 있음 (정상) ---" << endl;
    GoodBase* gp = new GoodDerived("safe");
    delete gp;   // ~GoodDerived 먼저 실행 후 ~GoodBase 실행
    cout << endl;

    return 0;
}
```

출력:

```text
--- 가상 소멸자 없음 (버그) ---
[BadDerived] 할당: leak
[BadBase] 소멸자

--- 가상 소멸자 있음 (정상) ---
[GoodDerived] 할당: safe
[GoodDerived] 해제: safe
[GoodBase] 소멸자
```

가상 소멸자가 없는 경우에는 `[BadDerived] 해제: leak`이 출력되지 않는다. `BadDerived`의 소멸자가 실행되지 않아 `name`이 가리키는 메모리가 누수된 것이다. 가상 소멸자가 있는 경우에는 두 소멸자가 파생 클래스, 기본 클래스의 올바른 순서로 실행된다.

### 3.3 원칙

클래스에 **가상함수가 하나라도 있으면** 그 클래스는 기본 클래스 포인터로 다루어질 가능성이 크므로, 소멸자도 **가상**으로 선언해야 한다. 이렇게 하면 기본 클래스 포인터를 통한 `delete` 시 동적 바인딩으로 파생 소멸자가 먼저 호출되고, 파생 소멸자가 끝나면 기본 소멸자가 호출된다.

```cpp
class Base {
public:
    virtual ~Base() { cout << "~Base" << endl; }
    virtual void func() { }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; cout << "~Derived" << endl; }
};

Base* p = new Derived;
delete p;    // 출력: "~Derived" 후 "~Base" — 정상!
```

> **참고:** **경험 법칙.** 가상함수를 가진 기본 클래스로 사용되는 클래스는 반드시 `virtual` 소멸자를 가져야 한다. 이것은 C++에서 가장 중요한 모범 사례 중 하나이다.

---

<br>

## 4. 추상 클래스

### 4.1 순수 가상함수

**순수 가상함수(pure virtual function)** 는 기본 클래스에 구현이 없고 선언만 있는 가상함수이다. 함수 원형 끝에 `= 0`을 붙여 선언하며, 파생 클래스가 객체를 만들 수 있는 구체 클래스(4.3절)가 되려면 이 함수를 **반드시 구현해야** 한다. 기본 클래스의 가상함수가 파생 클래스에서 재정의할 함수의 형태를 알려 주는 역할만 하고 실행할 코드가 필요 없을 때 순수 가상함수로 선언한다.

```cpp
class Shape {
public:
    virtual double getArea() = 0;   // 순수 가상함수 — 본체 없음
    virtual void draw() = 0;        // 순수 가상함수
};
```

### 4.2 추상 클래스

**순수 가상함수가 하나 이상** 있는 클래스를 **추상 클래스(abstract class)** 라 한다. 추상 클래스의 특징:

- **인스턴스화 불가:** 온전한 클래스가 아니므로 직접 객체를 생성할 수 없다.
- **포인터와 참조는 가능:** `Shape* p;`는 유효하다.
- **순수하지 않은 멤버함수 포함 가능:** 부분적인 구현을 제공할 수 있으며, 모든 멤버함수를 순수 가상함수로 선언할 필요는 없다. 순수하지 않은 멤버함수 안에서 순수 가상함수를 호출할 수도 있다(2.8절의 `paint()`와 같은 방식).
- **인터페이스/계약 역할:** 파생 클래스가 구현해야 할 함수의 원형을 보여 준다. 추상 클래스는 객체를 만들기 위한 것이 아니라 상속에서 기본 클래스 역할을 하기 위한 것이다.

```cpp
// Shape s;           // 컴파일 오류 — Shape은 추상 클래스
Shape* p;             // OK — 추상 클래스에 대한 포인터
Shape& r = *p;        // OK — 추상 클래스에 대한 참조
```

### 4.3 구체 클래스

**구체 클래스(concrete class)** 는 추상 기본 클래스로부터 상속받은 **모든 순수 가상함수를 구현**한 파생 클래스이다. 구체 클래스만 인스턴스화할 수 있다.

```cpp
class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) { }
    double getArea() override { return 3.14159 * radius * radius; }
    void draw() override { cout << "Drawing Circle" << endl; }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) { }
    double getArea() override { return width * height; }
    void draw() override { cout << "Drawing Rectangle" << endl; }
};

Circle c(5.0);        // OK — Circle은 구체 클래스
Rectangle r(3.0, 4.0); // OK — Rectangle은 구체 클래스
```

> **핵심:** 파생 클래스가 모든 순수 가상함수를 오버라이딩하지 않으면 여전히 추상 클래스이며, 인스턴스화할 수 없다.

### 4.4 추상 클래스 구현 예제

교재 예제 9-6은 순수 가상함수만으로 이루어진 추상 클래스 `Calculator`를 상속받아 `GoodCalc` 클래스를 구현한다. `GoodCalc`는 세 순수 가상함수를 모두 구현하므로 구체 클래스이다.

```cpp
#include <iostream>
using namespace std;

class Calculator {
public:
    virtual int add(int a, int b) = 0;              // 두 정수의 합 반환
    virtual int subtract(int a, int b) = 0;         // 두 정수의 차 반환
    virtual double average(int a[], int size) = 0;  // 배열 a의 평균 반환, size는 배열의 크기
};

class GoodCalc : public Calculator {
public:
    int add(int a, int b) { return a + b; }
    int subtract(int a, int b) { return a - b; }
    double average(int a[], int size) {
        double sum = 0;
        for (int i = 0; i < size; i++)
            sum += a[i];
        return sum / size;
    }
};

int main() {
    int a[] = {1, 2, 3, 4, 5};
    Calculator* p = new GoodCalc();
    cout << p->add(2, 3) << endl;
    cout << p->subtract(2, 3) << endl;
    cout << p->average(a, 5) << endl;
    delete p;
}
```

출력:

```text
5
-1
3
```

> **참고:** `Calculator`에는 가상 소멸자가 없으므로, `Calculator*`로 `GoodCalc` 객체를 `delete`하는 마지막 줄은 3.1절의 정의되지 않은 동작에 해당한다. `GoodCalc`에 해제할 자원이 없어 이 예제에서는 문제가 드러나지 않지만, `Calculator`에 `virtual ~Calculator() { }`를 추가하는 것이 올바르다.

교재 예제 9-7의 `Calculator`는 사용자 입력과 결과 출력을 담당하는 `run()`을 구현하고, 실제 계산만 순수 가상함수 `calc()`로 남겨 둔다. 파생 클래스 `Adder`와 `Subtractor`는 `calc()`만 구현하면 된다. `run()` 안의 `calc(a, b)` 호출은 2.8절의 `paint()`처럼 실제 객체의 `calc()`로 동적 바인딩된다.

```cpp
#include <iostream>
using namespace std;

class Calculator {
    void input() {
        cout << "정수 2개를 입력하세요>> ";
        cin >> a >> b;
    }
protected:
    int a, b;
    virtual int calc(int a, int b) = 0;   // 두 정수의 계산 결과 반환
public:
    void run() {
        input();
        cout << "계산된 값은 " << calc(a, b) << endl;
    }
};

class Adder : public Calculator {
protected:
    int calc(int a, int b) {   // 순수 가상함수 구현
        return a + b;
    }
};

class Subtractor : public Calculator {
protected:
    int calc(int a, int b) {   // 순수 가상함수 구현
        return a - b;
    }
};

int main() {
    Adder adder;
    Subtractor subtractor;
    adder.run();
    subtractor.run();
}
```

실행 예시(두 번 모두 `5 3`을 입력한 경우):

```text
정수 2개를 입력하세요>> 5 3
계산된 값은 8
정수 2개를 입력하세요>> 5 3
계산된 값은 2
```

---

<br>

## 5. 다형성

### 5.1 개념

**다형성(Polymorphism)** 은 "하나의 인터페이스, 여러 구현"을 의미한다. 기본 클래스 포인터가 런타임에 실제 객체 타입에 따라 서로 다른 파생 클래스 구현을 호출할 수 있다.

```text
            Base* p
               │
     ┌─────────┼─────────┐
     ▼         ▼         ▼
  DerivedA  DerivedB  DerivedC
   func()    func()    func()
```

*그림 3. 하나의 기본 클래스 포인터가 여러 파생 클래스의 func()를 호출하는 구조*

`p->func()` 호출 시, 포인터 타입이 아닌 `p`가 실제로 가리키는 객체 타입에 따라 올바른 파생 버전이 호출된다.

### 5.2 완전한 Shape 계층 구조 예제

`Shape` 계층 구조를 보여주는 완전한 실행 가능 프로그램이다. 가상 `draw()`와 `area()`, 기본 포인터 배열을 통한 다형적 동작, 그리고 올바른 가상 소멸자 사용법을 보여준다.

```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw() = 0;            // 순수 가상함수
    virtual double area() = 0;          // 순수 가상함수
    virtual ~Shape() { }                // 가상 소멸자
};

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) { }
    void draw() override {
        cout << "원 그리기 (반지름=" << radius << ")" << endl;
    }
    double area() override {
        return 3.14159265 * radius * radius;
    }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) { }
    void draw() override {
        cout << "사각형 그리기 (" << width << "x" << height << ")" << endl;
    }
    double area() override {
        return width * height;
    }
};

class Triangle : public Shape {
    double base, height;
public:
    Triangle(double b, double h) : base(b), height(h) { }
    void draw() override {
        cout << "삼각형 그리기 (밑변=" << base << ", 높이=" << height << ")" << endl;
    }
    double area() override {
        return 0.5 * base * height;
    }
};

// 이 함수는 어떤 Shape에도 동작한다 — 과거, 현재, 미래의 Shape 모두.
// Circle, Rectangle, Triangle에 대해 알 필요가 없다.
void printShapeInfo(Shape* s) {
    s->draw();
    cout << "  넓이 = " << s->area() << endl;
}

int main() {
    const int N = 4;
    Shape* canvas[N];
    canvas[0] = new Circle(5.0);
    canvas[1] = new Rectangle(3.0, 4.0);
    canvas[2] = new Triangle(6.0, 3.0);
    canvas[3] = new Circle(2.5);

    // 하나의 반복문으로 모든 도형 타입을 동일하게 처리
    for (int i = 0; i < N; i++) {
        printShapeInfo(canvas[i]);
    }

    // 정리 — 가상 소멸자가 올바른 삭제를 보장
    for (int i = 0; i < N; i++) {
        delete canvas[i];
    }

    return 0;
}
```

출력:

```text
원 그리기 (반지름=5)
  넓이 = 78.5398
사각형 그리기 (3x4)
  넓이 = 12
삼각형 그리기 (밑변=6, 높이=3)
  넓이 = 9
원 그리기 (반지름=2.5)
  넓이 = 19.635
```

이 설계의 강점: 나중에 `Pentagon` 클래스를 추가하더라도, 새 클래스를 작성하고 `draw()`와 `area()`를 오버라이딩하기만 하면 된다. `printShapeInfo()` 함수와 `main()`의 반복문은 **수정이 전혀 필요 없다**. 이것이 개방-폐쇄 원칙(Open-Closed Principle)의 실현이다: 확장에는 열려 있고, 수정에는 닫혀 있다.

교재는 같은 원리를 연결 리스트로 보여 준다. `Shape`에 다음 도형을 가리키는 `next` 포인터와 `add()`, `getNext()`, 그리고 `draw()`를 호출하는 `paint()`를 두고, `Circle`, `Rect`, `Line` 객체를 `add()`로 차례로 연결한다. 시작 포인터부터 `Shape*` 포인터로 리스트를 따라가며 `p->paint()`를 호출하면 각 객체가 오버라이딩한 `draw()`가 실행되어 `Circle`, `Rectangle`, `Circle`, `Line`, `Rectangle`이 차례로 출력된다. 도형을 삭제할 때도 `Shape`의 가상 소멸자 덕분에 각 파생 객체가 올바르게 소멸된다.

### 5.3 완전한 Converter 예제

추상 클래스와 다형성을 활용한 완전한 실행 가능 단위 변환 시스템이다. 추상 기본 클래스 `Converter`가 인터페이스를 정의하고, 각 구체 파생 클래스가 특정 변환을 구현한다. `Converter*` 포인터 배열로 모든 변환기를 동일하게 처리한다.

```cpp
#include <iostream>
using namespace std;

class Converter {
protected:
    double before, after;
public:
    Converter(double val) : before(val), after(0) { }
    virtual void compute() = 0;    // 순수 가상함수
    virtual void display() = 0;    // 순수 가상함수
    virtual ~Converter() { }
};

class LiterToGallon : public Converter {
public:
    LiterToGallon(double val) : Converter(val) { }
    void compute() override { after = before / 3.7854; }
    void display() override {
        cout << before << " 리터 = " << after << " 갤런" << endl;
    }
};

class CelsiusToFahrenheit : public Converter {
public:
    CelsiusToFahrenheit(double val) : Converter(val) { }
    void compute() override { after = before * 9.0 / 5.0 + 32; }
    void display() override {
        cout << "섭씨 " << before << "도 = 화씨 " << after << "도" << endl;
    }
};

class KmToMiles : public Converter {
public:
    KmToMiles(double val) : Converter(val) { }
    void compute() override { after = before * 0.621371; }
    void display() override {
        cout << before << " km = " << after << " 마일" << endl;
    }
};

class KgToPounds : public Converter {
public:
    KgToPounds(double val) : Converter(val) { }
    void compute() override { after = before * 2.20462; }
    void display() override {
        cout << before << " kg = " << after << " 파운드" << endl;
    }
};

int main() {
    const int N = 4;
    Converter* conv[N];
    conv[0] = new LiterToGallon(10.0);
    conv[1] = new CelsiusToFahrenheit(100.0);
    conv[2] = new KmToMiles(42.195);
    conv[3] = new KgToPounds(70.0);

    // 하나의 반복문으로 모든 변환기 타입을 처리
    for (int i = 0; i < N; i++) {
        conv[i]->compute();
        conv[i]->display();
    }

    for (int i = 0; i < N; i++) {
        delete conv[i];
    }

    return 0;
}
```

출력:

```text
10 리터 = 2.64173 갤런
섭씨 100도 = 화씨 212도
42.195 km = 26.2187 마일
70 kg = 154.323 파운드
```

새로운 단위 변환(예: `InchToCm`)을 추가할 때는 새 파생 클래스만 작성하면 된다. `main()`의 반복문 로직은 수정할 필요가 없다.

---

<br>

## 6. 흔한 실수

### 6.1 가상 소멸자 누락

다형성을 사용할 때(기본 포인터로 파생 객체를 삭제할 때), 기본 클래스 소멸자를 `virtual`로 선언하지 않으면 파생 소멸자가 호출되지 않는다. 이것은 아마도 C++에서 가장 흔한 가상함수 관련 버그일 것이다.

```cpp
class Base {
public:
    virtual void doWork() = 0;
    ~Base() { }                  // 버그: virtual이 아님!
};
class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[1000]; }
    ~Derived() { delete[] data; }
    void doWork() override { }
};

Base* p = new Derived;
delete p;   // ~Derived가 호출되지 않음 → 매번 4000바이트 누수
```

주석의 4000바이트는 `int`가 4바이트인 환경에서 `new int[1000]`이 할당하는 크기이다.

**수정:** 가상함수가 있는 클래스에는 반드시 `virtual ~Base() { }` (또는 `virtual ~Base() = default;`)로 선언한다.

### 6.2 추상 클래스 인스턴스화 시도

추상 클래스는 인스턴스화할 수 없다. 컴파일러가 거부한다.

```cpp
class Shape {
public:
    virtual void draw() = 0;
};

// Shape s;          // 컴파일 오류: 추상 타입 'Shape'의 변수를 선언할 수 없음
// new Shape();      // 컴파일 오류: 같은 이유

Shape* p;            // OK — 추상 클래스에 대한 포인터는 가능
```

### 6.3 순수 가상함수 오버라이딩 누락

파생 클래스가 기본 클래스의 **모든** 순수 가상함수를 오버라이딩하지 않으면, 해당 파생 클래스도 **여전히 추상 클래스**이며 인스턴스화할 수 없다.

```cpp
class Shape {
public:
    virtual void draw() = 0;
    virtual double area() = 0;
    virtual ~Shape() { }
};

class IncompleteCircle : public Shape {
    double radius;
public:
    IncompleteCircle(double r) : radius(r) { }
    void draw() override { /* ... */ }
    // area()를 오버라이딩하는 것을 잊었다!
};

// IncompleteCircle c(5.0);  // 컴파일 오류! IncompleteCircle은 여전히 추상 클래스
                              // area()가 오버라이딩되지 않았기 때문
```

**수정:** 모든 순수 가상함수를 오버라이딩하거나, 의도적으로 추상 클래스로 유지한다(예: 중간 기본 클래스로 사용).

### 6.4 객체 슬라이싱

파생 객체를 기본 타입의 매개변수에 **값으로** 전달하면, 파생 부분이 **잘려 나가고** 기본 부분만 복사된다. 매개변수 객체가 실제로 기본 클래스 타입의 객체가 되므로 가상함수가 기대대로 동작하지 않는다.

```cpp
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void speak() { cout << "..." << endl; }
    virtual ~Animal() { }
};

class Dog : public Animal {
public:
    void speak() override { cout << "멍멍!" << endl; }
};

// 잘못됨: Animal을 값으로 받음 — 슬라이싱 발생!
void makeItSpeak_bad(Animal a) {
    a.speak();      // 항상 Animal::speak() 호출, Dog::speak()는 절대 호출 안 됨
}

// 올바름: Animal을 참조로 받음 — 슬라이싱 없음
void makeItSpeak_good(Animal& a) {
    a.speak();      // 올바른 오버라이딩 호출
}

// 역시 올바름: Animal을 포인터로 받음
void makeItSpeak_ptr(Animal* a) {
    a->speak();     // 올바른 오버라이딩 호출
}

int main() {
    Dog d;
    makeItSpeak_bad(d);    // "..."    — 슬라이싱됨! Dog 부분 손실
    makeItSpeak_good(d);   // "멍멍!" — 정확
    makeItSpeak_ptr(&d);   // "멍멍!" — 정확
    return 0;
}
```

**원칙:** 다형성을 사용할 때는 반드시 객체를 **포인터** 또는 **참조**로 전달해야 하며, 값으로 전달하지 않는다.

### 6.5 오버라이딩 vs 오버로딩 vs 하이딩

이 세 개념은 자주 혼동된다. 의미가 매우 다르다:

| | 오버라이딩 (Overriding) | 오버로딩 (Overloading) | 하이딩 (Hiding) |
|:--|:-----------|:------------|:-------|
| **정의** | 같은 이름, 같은 매개변수, 기본 함수가 `virtual` | 같은 이름, **다른** 매개변수, 같은 스코프 | 같은 이름이지만 오버라이딩이 아님(기본 함수가 `virtual`이 **아니거나** 매개변수가 다름) |
| **위치** | 파생 클래스가 기본 가상함수를 재구현 | 같은 클래스(또는 같은 스코프) 내 | 파생 클래스에 기본과 같은 이름의 함수 |
| **효과** | 동적 바인딩으로 런타임에 올바른 버전 호출 | 컴파일러가 인자 타입으로 컴파일 시 선택 | 기본 버전이 숨겨짐; 파생 버전만 보임 |
| **virtual 필요?** | 예 | 아니오 | 아니오 (원형이 같은 비가상 함수를 다시 정의하면 오버라이딩이 아닌 하이딩이 된다) |

*표 1. 오버라이딩, 오버로딩, 하이딩의 비교*

```cpp
class Base {
public:
    virtual void func(int x) { cout << "Base::func(int)" << endl; }
    void nonVirtual(int x)   { cout << "Base::nonVirtual(int)" << endl; }
};

class Derived : public Base {
public:
    // 오버라이딩: 같은 이름, 같은 매개변수, 기본이 virtual
    void func(int x) override { cout << "Derived::func(int)" << endl; }

    // 하이딩: 같은 이름, 같은 매개변수, 하지만 기본이 virtual이 아님
    void nonVirtual(int x) { cout << "Derived::nonVirtual(int)" << endl; }

    // 오버로딩: 같은 이름, 다른 매개변수 (Derived 스코프 내)
    void func(double x) { cout << "Derived::func(double)" << endl; }
};

int main() {
    Derived d;
    Base* bp = &d;

    bp->func(1);          // "Derived::func(int)"   — 오버라이딩 (동적 바인딩)
    bp->nonVirtual(1);    // "Base::nonVirtual(int)" — 하이딩 (정적 바인딩; 기본 버전 호출)

    d.func(3.14);         // "Derived::func(double)" — 오버로딩 (컴파일 시 결정)
    d.func(1);            // "Derived::func(int)"    — 오버라이딩
    d.nonVirtual(1);      // "Derived::nonVirtual(int)" — 하이딩 (Derived 객체에서 파생 버전 호출)
}
```

> **주의:** 하이딩은 거의 항상 버그이다. 오버라이딩하려 했다면 기본에 `virtual`을, 파생에 `override`를 써야 한다. 컴파일러가 `override`가 아무것과도 일치하지 않는다고 말하면, 오버라이딩이 아니라 하이딩을 하고 있을 수 있다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:---------|
| 업캐스팅 | 파생 클래스의 포인터나 참조를 기본 클래스 타입으로 바꾸는 변환으로, 암시적이고 안전하지만 기본 클래스의 멤버에만 접근할 수 있다. |
| 다운캐스팅 | 기본 클래스 포인터를 파생 클래스 포인터로 바꾸는 변환으로, 명시적 캐스트가 필요하며 `dynamic_cast`를 쓰면 런타임에 안전하게 검사할 수 있다. |
| 정적 바인딩 | `virtual`이 없으면 포인터 타입이 호출할 함수를 컴파일 시점에 결정한다. |
| 동적 바인딩 | `virtual`이 있으면 실제 객체 타입이 호출할 함수를 런타임에 결정한다. |
| `virtual` 키워드 | 함수 호출을 정적 바인딩에서 동적 바인딩으로 바꾸는 스위치로, 진정한 다형성을 가능하게 한다. |
| vtable / vptr | vtable은 클래스마다 하나 있는 함수 포인터 테이블이고, vptr은 객체마다 하나 있으며 자기 클래스의 vtable을 가리킨다. |
| `override` (C++11) | 컴파일러가 기본 클래스에 일치하는 가상함수가 있는지 확인하여 이름 오타와 원형 불일치를 잡아 준다. |
| 오버라이딩의 성공 조건 | 이름, 매개변수의 타입과 개수, 반환 타입이 모두 일치해야 오버라이딩이 성립하며, `virtual` 키워드는 상속되므로 파생 클래스에서 생략할 수 있다. |
| 가상함수의 상속 | 가상함수는 상속 계층이 몇 단계이든 가상으로 남고, 오버라이딩하지 않은 파생 클래스에서는 가장 가까운 기본 클래스의 함수가 호출된다. |
| `final` (C++11) | 가상함수 원형 뒤에 붙이면 그 함수의 오버라이딩을 금지하고, 클래스 이름 뒤에 붙이면 그 클래스의 상속을 금지한다. |
| 범위 지정 연산자 호출 | `기본클래스::가상함수()` 형태로 호출하면 정적 바인딩으로 기본 클래스의 함수가 실행된다. |
| 가상 소멸자 | 가상함수가 있는 클래스는 소멸자를 가상으로 선언해야 기본 클래스 포인터로 `delete`할 때 파생 클래스의 소멸자까지 올바르게 실행된다. |
| 순수 가상함수 | `virtual void func() = 0;`처럼 선언만 하는 가상함수로, 파생 클래스가 구체 클래스가 되려면 반드시 구현해야 한다. |
| 추상 클래스 | 순수 가상함수가 하나 이상 있는 클래스로, 인스턴스화할 수 없고 파생 클래스가 구현할 함수의 원형을 보여 주는 인터페이스 역할을 한다. |
| 구체 클래스 | 상속받은 모든 순수 가상함수를 구현한 클래스로, 인스턴스화할 수 있다. |
| 다형성 | 하나의 인터페이스로 여러 구현을 다루는 성질로, 기본 클래스 포인터가 실제 객체에 맞는 파생 클래스의 함수를 호출한다. |
| 객체 슬라이싱 | 파생 객체를 기본 클래스 타입의 매개변수에 값으로 전달하면 파생 부분이 잘려 나가므로 포인터나 참조로 전달해야 한다. |
| 오버라이딩 vs 하이딩 | 오버라이딩은 기본 클래스의 가상함수와 원형이 같은 함수를 다시 정의하여 동적 바인딩되고, 하이딩은 오버라이딩이 아닌 같은 이름의 함수가 기본 클래스의 함수를 가려 정적 바인딩된다. |

---

<br>

## 점검 문제

1. **바인딩 판단:** 다음 코드에서 `p->f()`와 `p->g()`는 각각 무엇을 출력하는가?

   ```cpp
   class A {
   public:
       void f() { cout << "A::f()" << endl; }
       virtual void g() { cout << "A::g()" << endl; }
   };
   class B : public A {
   public:
       void f() { cout << "B::f()" << endl; }
       void g() override { cout << "B::g()" << endl; }
   };

   A* p = new B;
   p->f();
   p->g();
   ```

   <details>
   <summary>정답</summary>

   `p->f()`는 `A::f()`, `p->g()`는 `B::g()`를 출력한다. `f()`는 비가상 함수이므로 포인터 타입 `A*`에 따라 컴파일 시점에 `A::f()`로 정적 바인딩된다. `g()`는 가상함수이므로 런타임에 `p`가 실제로 가리키는 `B` 객체의 `g()`로 동적 바인딩된다.

   </details>

2. **가상 소멸자:** 1번의 `A`에 `~A() { cout << "~A()" << endl; }`, `B`에 `~B() { cout << "~B()" << endl; }`가 있을 때 `delete p;`를 실행하면 어떤 소멸자가 어떤 순서로 실행되는가? `~A()`가 비가상인 경우와 `virtual ~A()`인 경우를 각각 답하라.

   <details>
   <summary>정답</summary>

   `~A()`가 비가상이면 소멸자 호출도 포인터 타입에 따라 정적 바인딩되므로 일반적인 컴파일러에서는 `~A()`만 실행되고 `~B()`는 실행되지 않는다(표준상 정의되지 않은 동작). `virtual ~A()`이면 동적 바인딩으로 `~B()`가 먼저 실행되고, `~B()`가 끝난 뒤 `~A()`가 실행된다.

   </details>

3. **추상 클래스 판별:** 다음 선언 가운데 컴파일 오류가 나는 것을 모두 고르고 이유를 쓰라.

   ```cpp
   class Shape {
   public:
       virtual void draw() = 0;
       virtual double area() = 0;
   };
   class Circle : public Shape {
   public:
       void draw() override { }
       double area() override { return 0; }
   };
   class Half : public Shape {
   public:
       void draw() override { }
   };

   Shape s;          // (a)
   Circle c;         // (b)
   Half h;           // (c)
   Shape* p = &c;    // (d)
   ```

   <details>
   <summary>정답</summary>

   (a)와 (c)가 컴파일 오류이다. `Shape`는 순수 가상함수를 가진 추상 클래스이므로 객체를 만들 수 없다. `Half`는 `area()`를 오버라이딩하지 않았으므로 여전히 추상 클래스이다. `Circle`은 두 순수 가상함수를 모두 구현한 구체 클래스이므로 (b)는 정상이고, 추상 클래스에 대한 포인터는 선언할 수 있으므로 (d)도 정상이다.

   </details>

4. **오버라이딩 조건:** `Base`에 `virtual void f(int);`가 있을 때 `Derived`에 다음 함수를 선언하면 각각 오버라이딩, 컴파일 오류, 새 함수(하이딩) 중 무엇인가? (a) `void f(int);` (b) `int f(int);` (c) `void f(double);`

   <details>
   <summary>정답</summary>

   (a)는 오버라이딩이다. 이름, 매개변수, 반환 타입이 모두 일치하며, `virtual` 키워드는 상속되므로 생략해도 가상함수이다. (b)는 매개변수가 같고 반환 타입만 다르므로 컴파일 오류이다. (c)는 매개변수가 다르므로 오버라이딩이 아닌 새 함수이며, `Derived`의 스코프에서 `Base::f(int)`를 가린다.

   </details>

5. **범위 지정 연산자:** 2.10절의 예제에서 `Circle::draw()` 안의 `Shape::draw();`를 `draw();`로 바꾸면 `pShape->draw()`를 실행할 때 어떤 일이 일어나는가?

   <details>
   <summary>정답</summary>

   `Circle::draw()` 안의 `draw()`는 `this->draw()`와 같고 `draw()`가 가상함수이므로, 동적 바인딩으로 `Circle::draw()` 자신이 다시 호출된다. 종료 조건이 없는 무한 재귀가 되어 결국 스택 오버플로로 프로그램이 비정상 종료된다. 기본 클래스의 버전을 실행하려면 `Shape::draw()`처럼 범위 지정 연산자로 정적 바인딩을 지시해야 한다.

   </details>

---
