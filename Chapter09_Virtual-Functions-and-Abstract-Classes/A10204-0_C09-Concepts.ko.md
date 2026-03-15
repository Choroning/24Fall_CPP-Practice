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
  - [2.1 virtual 없는 경우의 문제점](#21-virtual-없는-경우의-문제점)
  - [2.2 virtual 키워드](#22-virtual-키워드)
  - [2.3 동작 원리](#23-동작-원리)
  - [2.4 override 키워드](#24-override-키워드)
- [3. 가상 소멸자](#3-가상-소멸자)
  - [3.1 문제점](#31-문제점)
  - [3.2 원칙](#32-원칙)
- [4. 추상 클래스](#4-추상-클래스)
  - [4.1 순수 가상함수](#41-순수-가상함수)
  - [4.2 추상 클래스](#42-추상-클래스)
  - [4.3 구체 클래스](#43-구체-클래스)
- [5. 다형성](#5-다형성)
  - [5.1 개념](#51-개념)
  - [5.2 기본 클래스 포인터 배열](#52-기본-클래스-포인터-배열)
  - [5.3 실전 예제](#53-실전-예제)
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

### 2.1 virtual 없는 경우의 문제점

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

### 2.2 virtual 키워드

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

### 2.3 동작 원리

C++는 동적 바인딩을 **vtable(가상함수 테이블)** 메커니즘으로 구현한다:

1. **vtable** — 가상함수를 가진 각 클래스는 함수 포인터 테이블을 갖는다. 이 테이블은 각 가상함수를 해당 클래스의 올바른 구현과 매핑한다.
2. **vptr** — 가상함수를 가진 클래스의 각 객체는 자신의 클래스 vtable을 가리키는 숨겨진 포인터(`vptr`)를 포함한다.
3. **런타임 조회** — 기본 클래스 포인터로 가상함수를 호출하면, `vptr`을 따라 vtable에서 함수를 찾아 올바른 버전을 호출한다.

```
┌────────────────┐         ┌─────────────────────┐
│  Base 객체      │         │   Base vtable        │
│  ┌──────────┐  │         │   func → Base::func  │
│  │  vptr   ─┼──┼────────▶│                      │
│  └──────────┘  │         └─────────────────────┘
└────────────────┘

┌────────────────┐         ┌──────────────────────────┐
│ Derived 객체    │         │   Derived vtable          │
│  ┌──────────┐  │         │   func → Derived::func    │
│  │  vptr   ─┼──┼────────▶│                           │
│  └──────────┘  │         └──────────────────────────┘
└────────────────┘
```

> **참고:** vtable 메커니즘은 객체당 포인터 하나, 가상함수 호출 시 간접 호출 한 번의 약간의 오버헤드를 추가한다. 이것이 런타임 다형성의 비용이다.

### 2.4 override 키워드

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

### 3.2 원칙

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
     ┌────┼────┐
     ▼    ▼    ▼
  DerivedA  DerivedB  DerivedC
  func()    func()    func()
```

`p->func()` 호출 시, 포인터 타입이 아닌 `p`가 실제로 가리키는 객체 타입에 따라 올바른 파생 버전이 호출된다.

### 5.2 기본 클래스 포인터 배열

다형성의 대표적인 패턴은 **기본 클래스 포인터의 배열**을 만들어, 각 원소가 서로 다른 파생 객체를 가리키게 하는 것이다. 하나의 반복문으로 모든 객체를 동일하게 처리할 수 있다.

```cpp
Shape* arr[3];
arr[0] = new Circle(5.0);
arr[1] = new Rectangle(3.0, 4.0);
arr[2] = new Circle(2.5);

for (int i = 0; i < 3; i++) {
    cout << "Area: " << arr[i]->getArea() << endl;  // 올바른 버전 호출
    arr[i]->draw();                                   // 올바른 버전 호출
}

for (int i = 0; i < 3; i++) {
    delete arr[i];   // 가상 소멸자가 올바른 정리를 보장
}
```

출력:

```text
Area: 78.5398
Drawing Circle
Area: 12
Drawing Rectangle
Area: 19.6350
Drawing Circle
```

### 5.3 실전 예제

추상 클래스와 다형성을 활용한 단위 변환 시스템. 추상 기본 클래스 `Converter`가 인터페이스를 정의하고, 각 파생 클래스가 특정 변환을 구현한다.

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

int main() {
    Converter* conv[2];
    conv[0] = new LiterToGallon(10.0);
    conv[1] = new CelsiusToFahrenheit(100.0);

    for (int i = 0; i < 2; i++) {
        conv[i]->compute();
        conv[i]->display();
    }

    for (int i = 0; i < 2; i++) {
        delete conv[i];
    }

    return 0;
}
```

출력:

```text
10 리터 = 2.64172 갤런
섭씨 100도 = 화씨 212도
```

---

<br>

## 요약

| 개념 | 핵심 요점 |
|:-----|:---------|
| 업캐스팅 | 파생 포인터/참조를 기본으로 변환; 암시적이며 안전; 기본 멤버만 접근 가능 |
| 다운캐스팅 | 기본 포인터를 파생으로 변환; 명시적 캐스트 필요; `dynamic_cast`로 안전하게 |
| 정적 바인딩 | `virtual` 없이는 포인터 타입이 호출 함수를 결정 (컴파일 시점) |
| `virtual` 키워드 | 동적 바인딩 활성화; 실제 객체 타입이 호출 함수를 결정 (런타임) |
| vtable / vptr | 각 클래스는 함수 포인터 테이블(vtable)을, 각 객체는 vptr을 가짐 |
| `override` (C++11) | 컴파일러가 기본 클래스에 일치하는 가상함수가 있는지 확인; 오타 방지 |
| 가상 소멸자 | 가상함수가 있는 클래스는 반드시 가상 소멸자 필요; 올바른 정리 보장 |
| 순수 가상함수 | `virtual void func() = 0;` — 본체 없음; 파생 클래스에서 반드시 구현 |
| 추상 클래스 | 순수 가상함수가 하나 이상; 인스턴스화 불가; 인터페이스 역할 |
| 구체 클래스 | 모든 순수 가상함수를 구현; 인스턴스화 가능 |
| 다형성 | 하나의 인터페이스, 여러 구현; 기본 포인터가 올바른 파생 버전 호출 |
| 기본 포인터 배열 | `Base* arr[n];` — 하나의 반복문으로 서로 다른 파생 객체를 처리 |

---
