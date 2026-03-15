# Chapter 09 — 가상함수와 추상 클래스

> **최종 수정일:** 2026-03-16
>
> **교재:** *명품 C++ Programming (개정판)* — 9장

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
  - [2.5 동작 원리 — vtable과 vptr](#25-동작-원리--vtable과-vptr)
  - [2.6 override 키워드](#26-override-키워드)
- [3. 가상 소멸자](#3-가상-소멸자)
  - [3.1 문제점](#31-문제점)
  - [3.2 완전한 예제 — 메모리 누수](#32-완전한-예제--메모리-누수)
  - [3.3 원칙](#33-원칙)
- [4. 추상 클래스](#4-추상-클래스)
  - [4.1 순수 가상함수](#41-순수-가상함수)
  - [4.2 추상 클래스](#42-추상-클래스)
  - [4.3 구체 클래스](#43-구체-클래스)
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

---

<br>

## 1. 업캐스팅과 다운캐스팅

### 1.1 업캐스팅

**업캐스팅(Upcasting)**은 파생 클래스의 포인터(또는 참조)를 기본 클래스의 포인터(또는 참조)로 변환하는 것이다. 모든 파생 객체는 기본 객체의 일종(is-a 관계)이므로 **암시적**으로 변환되며, 항상 **안전**하다.

```cpp
class Base { };
class Derived : public Base { };

Derived d;
Base* p = &d;       // 업캐스팅 — 암시적, 항상 안전
Base& r = d;        // 참조를 이용한 업캐스팅
```

기본 클래스 포인터 `p`를 통해서는 **기본 클래스의 멤버만 접근 가능**하다. 실제 객체가 `Derived`이더라도 컴파일러는 `Base*` 타입만 보고 접근 범위를 제한한다.

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

**다운캐스팅(Downcasting)**은 기본 클래스 포인터를 파생 클래스 포인터로 변환하는 것이다. **명시적 캐스트**가 필요하며, 실제 객체가 해당 파생 타입이 아닌 경우 **위험**하다.

```cpp
Base* p = new Derived;
Derived* dp = (Derived*)p;   // C 스타일 다운캐스트 — 컴파일되지만 위험
```

**안전한 다운캐스팅**을 위해 `dynamic_cast`를 사용한다. 런타임에 타입을 검사하고, 변환이 불가능하면 `nullptr`을 반환한다(기본 클래스에 가상함수가 하나 이상 있어야 함).

```cpp
Base* p = new Base;
Derived* dp = dynamic_cast<Derived*>(p);  // 런타임 검사
if (dp == nullptr) {
    cout << "캐스트 실패 — p가 Derived를 가리키지 않음" << endl;
}
```

> **주의:** `static_cast`나 C 스타일 캐스트로 다운캐스팅하면 런타임 검사를 건너뛴다. 실제 객체가 파생 타입이 아닌 경우 파생 멤버에 접근하면 **정의되지 않은 동작(undefined behavior)**이 발생한다.

---

<br>

## 2. 가상함수

### 2.1 왜 가상함수가 필요한가?

그림판 프로그램을 만든다고 상상해 보자. `Shape` 기본 클래스가 있고, `Circle`, `Rectangle`, `Triangle` 같은 파생 클래스가 있다. 모든 도형을 `Shape*` 포인터 배열에 저장하고, 하나의 반복문으로 모두 그리고 싶다:

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

**virtual 없이:** `canvas[i]->draw()` 호출 시 항상 `Shape::draw()`가 호출된다. 컴파일러는 `Shape*` 타입만 보기 때문이다. 실제 객체가 `Circle`이든 아니든 상관없이, 컴파일 시점에 이미 `Shape::draw()`를 호출하기로 결정했다. 모든 도형이 똑같은 것을 그린다. 이래서는 쓸모가 없다.

**virtual 있으면:** `canvas[i]->draw()` 호출 시 실제 객체에 맞는 **올바른** `draw()`가 호출된다. 원이면 `Circle::draw()`, 사각형이면 `Rectangle::draw()` 등이다. 런타임에 실제 객체 타입에 따라 결정된다.

이것이 **OOP 다형성의 핵심**이다: 기본 클래스 포인터로 코드를 작성하면서도, 각 파생 타입에 맞는 동작이 자동으로 수행된다. 가상함수 없이는 상속이 단순한 코드 공유에 불과하지만, 가상함수가 있으면 상속이 *대체 가능성(substitutability)*을 가능하게 한다.

### 2.2 virtual 없는 경우의 문제점

`virtual` 없이 함수를 선언하면, 호출되는 함수는 **포인터의 타입**에 의해 결정된다. 이를 **정적 바인딩(static binding)** 또는 **이른 바인딩(early binding)**이라 하며, 컴파일 시점에 결정된다.

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

### 2.3 virtual 키워드

기본 클래스 함수에 `virtual`을 붙이면 **동적 바인딩(dynamic binding)** 또는 **늦은 바인딩(late binding)**이 활성화된다. 런타임에 실제 객체의 타입에 따라 호출할 함수가 결정된다.

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

### 2.4 정적 바인딩 vs 동적 바인딩

정적 바인딩과 동적 바인딩의 차이를 이해하는 것이 핵심이다. 이 둘은 "어떤 함수를 호출할지"의 결정이 **언제** 이루어지는지를 결정한다.

**정적 바인딩 (컴파일 타임 바인딩)**
- 컴파일러가 **포인터/참조의 타입**을 기준으로 호출할 함수를 결정한다.
- 비가상(non-virtual) 함수의 기본 동작이다.
- 빠르지만(런타임 조회 없음), 실제 객체 타입을 무시한다.

**동적 바인딩 (런타임 바인딩)**
- 프로그램이 런타임에 **실제 객체 타입**을 기준으로 호출할 함수를 결정한다.
- `virtual` 키워드로 활성화된다.
- 약간 느리지만(vtable 조회), 진정한 다형성을 가능하게 한다.

`virtual` 키워드는 함수를 정적 바인딩에서 동적 바인딩으로 전환하는 스위치이다.

**나란히 비교 — 같은 코드, 다른 동작:**

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

> **핵심:** 유일한 차이는 기본 클래스의 `virtual` 키워드뿐이다. 상속, 포인터 타입, 함수 호출 — 나머지는 모두 동일하다. `virtual`이 다형성을 작동시키는 것이다.

### 2.5 동작 원리 — vtable과 vptr

C++는 동적 바인딩을 **vtable(가상함수 테이블)** 메커니즘으로 구현한다:

1. **vtable** — 가상함수를 가진 각 클래스는 함수 포인터 테이블을 갖는다. 이 테이블은 각 가상함수를 해당 클래스의 올바른 구현과 매핑한다.
2. **vptr** — 가상함수를 가진 클래스의 각 객체는 자신의 클래스 vtable을 가리키는 숨겨진 포인터(`vptr`)를 포함한다.
3. **런타임 조회** — 기본 클래스 포인터로 가상함수를 호출하면, `vptr`을 따라 vtable에서 함수를 찾아 올바른 버전을 호출한다.

**간단한 vtable 도식 (가상함수 1개):**

```
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

**상세 vtable 도식 — 여러 가상함수를 가진 Shape 계층 구조:**

`class Shape`에 `virtual draw()`와 `virtual area()`가 있고, 파생 클래스 `Circle`과 `Rectangle`이 있는 경우:

```
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

핵심 통찰: **vtable은 클래스당 하나**(해당 클래스의 모든 객체가 공유)이지만, **vptr은 객체당 하나**(각 객체가 자기 클래스의 vtable을 가리키는 포인터를 갖고 있음)이다. `Circle`을 생성하면 vptr이 `Circle`의 vtable을 가리키고, `Rectangle`을 생성하면 vptr이 `Rectangle`의 vtable을 가리킨다. 이것이 같은 `p->draw()` 호출이 실제 객체에 따라 다른 결과를 내는 이유이다.

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

> **권장 사항:** 가상함수를 오버라이딩할 때는 항상 `override`를 사용하자. 런타임 비용 없이 컴파일 시점 안전장치를 제공한다.

---

<br>

## 3. 가상 소멸자

### 3.1 문제점

기본 클래스 포인터를 통해 파생 객체를 `delete`할 때, 소멸자가 **가상이 아니면** 기본 클래스 소멸자만 호출된다. 파생 클래스 소멸자가 호출되지 않아 파생 클래스에서 할당한 **자원이 누수**될 수 있다.

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

### 3.2 완전한 예제 — 메모리 누수

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

"버그" 경우에서 `[BadDerived] 해제: leak`이 출력되지 않는 것에 주목하자 — 메모리가 누수된다. "정상" 경우에서는 두 소멸자가 올바른 순서로 실행된다.

### 3.3 원칙

클래스에 **가상함수가 하나라도 있으면**, 소멸자도 반드시 **가상**으로 선언해야 한다. 이렇게 하면 기본 클래스 포인터를 통한 `delete` 시 파생 소멸자가 먼저 호출되고, 이어서 기본 소멸자가 호출된다.

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

> **경험 법칙:** 가상함수를 가진 기본 클래스로 사용되는 클래스는 반드시 `virtual` 소멸자를 가져야 한다. 이것은 C++에서 가장 중요한 모범 사례 중 하나이다.

---

<br>

## 4. 추상 클래스

### 4.1 순수 가상함수

**순수 가상함수(pure virtual function)**는 기본 클래스에 구현이 없는 함수이다. 함수 선언 끝에 `= 0`을 붙여 선언하며, 파생 클래스에서 **반드시 구현**해야 한다.

```cpp
class Shape {
public:
    virtual double getArea() = 0;   // 순수 가상함수 — 본체 없음
    virtual void draw() = 0;        // 순수 가상함수
};
```

### 4.2 추상 클래스

**순수 가상함수가 하나 이상** 있는 클래스를 **추상 클래스(abstract class)**라 한다. 추상 클래스의 특징:

- **인스턴스화 불가** — 직접 객체를 생성할 수 없다.
- **포인터와 참조는 가능** — `Shape* p;`는 유효하다.
- **순수하지 않은 멤버함수 포함 가능** — 부분적인 구현을 제공할 수 있다.
- **인터페이스/계약 역할** — 파생 클래스가 구현해야 할 것을 정의한다.

```cpp
// Shape s;           // 컴파일 오류 — Shape은 추상 클래스
Shape* p;             // OK — 추상 클래스에 대한 포인터
Shape& r = *p;        // OK — 추상 클래스에 대한 참조
```

### 4.3 구체 클래스

**구체 클래스(concrete class)**는 추상 기본 클래스로부터 상속받은 **모든 순수 가상함수를 구현**한 파생 클래스이다. 구체 클래스만 인스턴스화할 수 있다.

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

---

<br>

## 5. 다형성

### 5.1 개념

**다형성(Polymorphism)**은 "하나의 인터페이스, 여러 구현"을 의미한다. 기본 클래스 포인터가 런타임에 실제 객체 타입에 따라 서로 다른 파생 클래스 구현을 호출할 수 있다.

```
            Base* p
               │
     ┌─────────┼─────────┐
     ▼         ▼         ▼
  DerivedA  DerivedB  DerivedC
   func()    func()    func()
```

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
  넓이 = 19.6350
```

이 설계의 강점: 나중에 `Pentagon` 클래스를 추가하더라도, 새 클래스를 작성하고 `draw()`와 `area()`를 오버라이딩하기만 하면 된다. `printShapeInfo()` 함수와 `main()`의 반복문은 **수정이 전혀 필요 없다**. 이것이 개방-폐쇄 원칙(Open-Closed Principle)의 실현이다: 확장에는 열려 있고, 수정에는 닫혀 있다.

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
10 리터 = 2.64172 갤런
섭씨 100도 = 화씨 212도
42.195 km = 26.2188 마일
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

파생 객체를 기본 타입의 매개변수에 **값으로** 전달하면, 파생 부분이 **잘려나간다** — 기본 부분만 복사된다. 객체가 실제로 `Base`가 되어버리므로 가상함수가 기대대로 동작하지 않는다.

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

**원칙:** 다형성을 사용할 때는 반드시 객체를 **포인터** 또는 **참조**로 전달해야 한다. 절대 값으로 전달하지 말 것.

### 6.5 오버라이딩 vs 오버로딩 vs 하이딩

이 세 개념은 자주 혼동된다. 의미가 매우 다르다:

| | 오버라이딩 (Overriding) | 오버로딩 (Overloading) | 하이딩 (Hiding) |
|:--|:-----------|:------------|:-------|
| **정의** | 같은 이름, 같은 매개변수, 기본 함수가 `virtual` | 같은 이름, **다른** 매개변수, 같은 스코프 | 같은 이름, 기본 함수가 `virtual`이 **아님** |
| **위치** | 파생 클래스가 기본 가상함수를 재구현 | 같은 클래스(또는 같은 스코프) 내 | 파생 클래스에 기본과 같은 이름의 함수 |
| **효과** | 동적 바인딩 — 런타임에 올바른 버전 호출 | 컴파일러가 인자 타입으로 컴파일 시 선택 | 기본 버전이 숨겨짐; 파생 버전만 보임 |
| **virtual 필요?** | 예 | 아니오 | 아니오 (이것이 오버라이딩이 아닌 하이딩인 이유) |

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

> **주의:** 하이딩은 거의 항상 버그이다. 오버라이딩하려 했다면 기본에 `virtual`을, 파생에 `override`를 사용하자. 컴파일러가 `override`가 아무것과도 일치하지 않는다고 말하면, 오버라이딩이 아니라 하이딩을 하고 있을 수 있다.

---

<br>

## 요약

| 개념 | 핵심 요점 |
|:-----|:---------|
| 업캐스팅 | 파생 포인터/참조를 기본으로 변환; 암시적이며 안전; 기본 멤버만 접근 가능 |
| 다운캐스팅 | 기본 포인터를 파생으로 변환; 명시적 캐스트 필요; `dynamic_cast`로 안전하게 |
| 정적 바인딩 | `virtual` 없이는 포인터 타입이 호출 함수를 결정 (컴파일 시점) |
| 동적 바인딩 | `virtual` 있으면 실제 객체 타입이 호출 함수를 결정 (런타임) |
| `virtual` 키워드 | 정적 바인딩에서 동적 바인딩으로 전환하는 스위치; 진정한 다형성을 가능하게 함 |
| vtable / vptr | vtable은 클래스당 하나(함수 포인터 테이블); vptr은 객체당 하나(자기 클래스의 vtable을 가리킴) |
| `override` (C++11) | 컴파일러가 기본 클래스에 일치하는 가상함수가 있는지 확인; 오타 방지 |
| 가상 소멸자 | 가상함수가 있는 클래스는 반드시 가상 소멸자 필요; 올바른 정리 보장 |
| 순수 가상함수 | `virtual void func() = 0;` — 본체 없음; 파생 클래스에서 반드시 구현 |
| 추상 클래스 | 순수 가상함수가 하나 이상; 인스턴스화 불가; 인터페이스 역할 |
| 구체 클래스 | 모든 순수 가상함수를 구현; 인스턴스화 가능 |
| 다형성 | 하나의 인터페이스, 여러 구현; 기본 포인터가 올바른 파생 버전 호출 |
| 객체 슬라이싱 | 파생을 기본 매개변수에 값으로 전달하면 파생 데이터 손실; 포인터/참조를 사용할 것 |
| 오버라이딩 vs 하이딩 | 오버라이딩은 `virtual` 필요하며 동적 바인딩; 하이딩은 비가상이며 정적 바인딩 |

---
