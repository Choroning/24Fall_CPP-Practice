# Chapter 08 — 상속

> **최종 수정일:** 2026-03-16

---

## 목차

- [1. 상속의 개념](#1-상속의-개념)
  - [1.1 상속이 필요한 이유](#11-상속이-필요한-이유)
  - [1.2 문법](#12-문법)
  - [1.3 상속되는 것](#13-상속되는-것)
- [2. 상속과 접근 제어](#2-상속과-접근-제어)
  - [2.1 `public` 상속](#21-public-상속)
  - [2.2 `protected` 상속](#22-protected-상속)
  - [2.3 `private` 상속](#23-private-상속)
  - [2.4 접근 제어 표](#24-접근-제어-표)
- [3. 상속에서의 생성자와 소멸자](#3-상속에서의-생성자와-소멸자)
  - [3.1 생성자 체인](#31-생성자-체인)
  - [3.2 소멸자 체인](#32-소멸자-체인)
  - [3.3 출력 추적 예제](#33-출력-추적-예제)
- [4. 멤버 함수 오버라이딩](#4-멤버-함수-오버라이딩)
  - [4.1 오버라이딩](#41-오버라이딩)
  - [4.2 기본 클래스 버전 호출](#42-기본-클래스-버전-호출)
- [5. 다중 상속](#5-다중-상속)
- [6. 현대적 상속 기능 (C++11)](#6-현대적-상속-기능-c11)
  - [6.1 `final` 키워드](#61-final-키워드)
  - [6.2 `override` 키워드](#62-override-키워드)
  - [6.3 `using`을 사용한 생성자 상속](#63-using을-사용한-생성자-상속)
- [요약](#요약)

---

<br>

## 1. 상속의 개념

### 1.1 상속이 필요한 이유

**상속(Inheritance)** 은 새로운 클래스(파생 클래스)가 기존 클래스(기본 클래스)를 재사용하고 확장할 수 있게 한다.

| 장점 | 설명 |
|:----|:----|
| 코드 재사용 | 공통 데이터와 함수를 기본 클래스에 한 번만 작성 |
| IS-A 관계 | `Car`**는** `Vehicle`**이다**; `Manager`**는** `Employee`**이다** |
| 확장성 | 기본 클래스를 수정하지 않고 파생 클래스에서 새 기능 추가 |

**용어:**

| 용어 | 동의어 |
|:----|:------|
| 기본 클래스 (Base class) | 부모 클래스 (Parent), 슈퍼 클래스 (Super) |
| 파생 클래스 (Derived class) | 자식 클래스 (Child), 서브 클래스 (Sub) |

```
   기본 (부모)
       ↑
   파생 (자식)
```

### 1.2 문법

```cpp
class Derived : public Base {
    // 추가 멤버
};
```

- 콜론 `:` 뒤에 접근 지정자(`public`, `protected`, `private`)와 기본 클래스 이름을 적는다.
- **`public` 상속** 이 가장 일반적이며 IS-A 관계를 유지한다.

```cpp
class Vehicle {
    string brand;
    int year;
public:
    Vehicle(string b, int y) : brand(b), year(y) {}
    string getBrand() { return brand; }
    int getYear() { return year; }
protected:
    void updateYear(int y) { year = y; }
};

class Car : public Vehicle {
    int numDoors;
public:
    Car(string b = "Unknown", int y = 2024, int d = 4)
        : Vehicle(b, y), numDoors(d) {}
    void setDoors(int d) { numDoors = d; }
    void setYear(int y) { updateYear(y); }   // 기본 클래스의 protected 멤버 호출
    void show() {
        cout << getBrand() << " (" << getYear() << "), "
             << numDoors << "도어" << endl;
    }
};
```

### 1.3 상속되는 것

기본 클래스의 모든 멤버가 파생 클래스에 상속된다. 단, **접근 가능 여부** 는 접근 지정자에 따라 다르다.

| 기본 클래스 멤버 접근 | 파생 클래스에서 접근 가능? |
|:-------------------|:---------------------|
| `public` | 가능 — 직접 접근 가능 |
| `protected` | 가능 — 직접 접근 가능 |
| `private` | **불가** — 상속은 되지만 직접 접근 불가; public/protected getter를 통해 접근 |

```cpp
class Base {
    int secret;        // private
protected:
    int shared;        // protected
public:
    int open;          // public
    int getSecret() { return secret; }
};

class Derived : public Base {
public:
    void test() {
        // cout << secret;     // 오류: Base의 private 멤버
        cout << shared;        // OK: protected는 접근 가능
        cout << open;          // OK: public은 접근 가능
        cout << getSecret();   // OK: private 멤버에 대한 public getter
    }
};
```

> **핵심:** `private` 멤버는 상속된다(메모리에 존재함). 하지만 파생 클래스에서 직접 접근할 수 없다. 기본 클래스에 정의된 getter를 통해 접근해야 한다.

---

<br>

## 2. 상속과 접근 제어

**상속 접근 지정자** 는 기본 클래스의 멤버가 파생 클래스에서 어떻게 노출되는지를 결정한다.

### 2.1 `public` 상속

가장 일반적인 형태. 기본 클래스의 접근 수준이 파생 클래스에서 유지된다.

```cpp
class Derived : public Base { };
```

| 기본 클래스 멤버 | 파생 클래스에서 |
|:-------------|:------------|
| `public` | `public` |
| `protected` | `protected` |
| `private` | 접근 불가 |

### 2.2 `protected` 상속

기본 클래스의 public 멤버가 파생 클래스에서 `protected`가 된다. 실무에서 거의 사용하지 않는다.

```cpp
class Derived : protected Base { };
```

| 기본 클래스 멤버 | 파생 클래스에서 |
|:-------------|:------------|
| `public` | `protected` |
| `protected` | `protected` |
| `private` | 접근 불가 |

### 2.3 `private` 상속

기본 클래스의 접근 가능한 모든 멤버가 파생 클래스에서 `private`가 된다. 거의 사용하지 않는다.

```cpp
class Derived : private Base { };
```

| 기본 클래스 멤버 | 파생 클래스에서 |
|:-------------|:------------|
| `public` | `private` |
| `protected` | `private` |
| `private` | 접근 불가 |

### 2.4 접근 제어 표

| 기본 클래스 멤버 | `public` 상속 | `protected` 상속 | `private` 상속 |
|:-------------|:------------|:---------------|:-------------|
| `public` | `public` | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private` | 접근 불가 | 접근 불가 | 접근 불가 |

> **핵심:** 상속 유형에 관계없이, 기본 클래스의 `private` 멤버는 파생 클래스에서 **절대** 직접 접근할 수 없다. `public` 상속만이 IS-A 관계를 유지한다.

---

<br>

## 3. 상속에서의 생성자와 소멸자

### 3.1 생성자 체인

파생 객체가 생성될 때, **기본 클래스 생성자가 먼저** 실행되고, 그다음 파생 클래스 생성자가 실행된다. 기본 클래스 생성자를 명시적으로 호출하지 않으면, 기본 클래스의 **기본 생성자(default constructor)** 가 사용된다.

```cpp
class Base {
public:
    Base() { cout << "Base()" << endl; }
    Base(int x) { cout << "Base(" << x << ")" << endl; }
};

class Derived : public Base {
public:
    // 명시적으로 Base(int) 호출
    Derived(int x, int y) : Base(x) {
        cout << "Derived(" << x << ", " << y << ")" << endl;
    }
};

// 사용
Derived d(10, 20);
// 출력:
//   Base(10)
//   Derived(10, 20)
```

| 상황 | 결과 |
|:----|:----|
| 기본 생성자를 명시적으로 호출하지 않을 때 | 기본 클래스의 **기본 생성자** 가 자동 호출됨 |
| `Derived(args) : Base(args)` 명시적 호출 | 지정한 기본 클래스 생성자가 호출됨 |
| 기본 클래스에 기본 생성자가 없고 파생에서 호출하지 않을 때 | **컴파일 오류** |

### 3.2 소멸자 체인

소멸자는 생성의 **역순** 으로 호출된다: 파생 클래스 소멸자가 먼저 실행되고, 그다음 기본 클래스 소멸자가 실행된다.

```cpp
class Base {
public:
    ~Base() { cout << "~Base()" << endl; }
};

class Derived : public Base {
public:
    ~Derived() { cout << "~Derived()" << endl; }
};

// Derived 객체가 범위를 벗어날 때:
//   ~Derived()
//   ~Base()
```

| 순서 | 생성 | 소멸 |
|:----|:----|:----|
| 첫 번째 | 기본 클래스 생성자 | 파생 클래스 소멸자 |
| 두 번째 | 파생 클래스 생성자 | 기본 클래스 소멸자 |

### 3.3 출력 추적 예제

```cpp
#include <iostream>
using namespace std;

class Employee {
    string name;
protected:
    double salary;
    string department;
public:
    Employee(string n, double sal, string dept)
        : name(n), salary(sal), department(dept) {
        cout << "Employee 생성자" << endl;
    }
    ~Employee() { cout << "Employee 소멸자" << endl; }
    string getName() { return name; }
    double getSalary() { return salary; }
    string getDepartment() { return department; }
};

class Manager : public Employee {
    int teamSize;
public:
    Manager(string n, double sal, string dept, int ts)
        : Employee(n, sal, dept), teamSize(ts) {
        cout << "Manager 생성자" << endl;
    }
    ~Manager() { cout << "Manager 소멸자" << endl; }
    double getBonus() {
        return salary * 0.15;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "부서: " << department << endl;
        cout << "급여: " << salary << endl;
        cout << "팀 규모: " << teamSize << endl;
        cout << "---------------" << endl;
        cout << "보너스: " << getBonus() << endl << endl;
    }
};

int main() {
    Manager m("홍길동", 75000, "개발부", 8);
    m.display();
    return 0;
}
```

**출력:**

```text
Employee 생성자
Manager 생성자
이름: 홍길동
부서: 개발부
급여: 75000
팀 규모: 8
---------------
보너스: 11250
Manager 소멸자
Employee 소멸자
```

> **핵심:** 생성은 기본 → 파생 순서로 진행된다. 소멸은 파생 → 기본 순서로 진행된다. 이렇게 해야 기본 멤버에 의존하는 파생 멤버가 올바르게 초기화되고 정리된다.

---

<br>

## 4. 멤버 함수 오버라이딩

### 4.1 오버라이딩

파생 클래스가 기본 클래스와 **같은 이름과 시그니처** 를 가진 함수를 정의하면, 파생 버전이 기본 버전을 **숨긴다**(오버라이딩한다).

```cpp
class Base {
public:
    void show() { cout << "Base::show()" << endl; }
};

class Derived : public Base {
public:
    void show() { cout << "Derived::show()" << endl; }   // Base::show()를 오버라이딩
};

int main() {
    Derived d;
    d.show();   // Derived::show() 호출
}
```

| 동작 | 설명 |
|:----|:----|
| 같은 이름과 매개변수 | 파생 버전이 기본 버전을 숨김 |
| 파생 객체를 통해 호출 | 파생 버전이 실행됨 |
| 기본 버전은 여전히 존재 | 존재하지만 직접 접근이 숨겨짐 |

### 4.2 기본 클래스 버전 호출

범위 지정 연산자 `Base::functionName()`을 사용하여 파생 클래스에서 기본 클래스 버전을 명시적으로 호출할 수 있다.

```cpp
class Derived : public Base {
public:
    void show() {
        Base::show();   // 기본 버전을 명시적으로 호출
        cout << "Derived::show()" << endl;
    }
};
```

클래스 외부에서 파생 객체를 통해 호출할 수도 있다:

```cpp
Derived d;
d.Base::show();   // Base::show()를 직접 호출
```

---

<br>

## 5. 다중 상속

C++은 **다중 상속** 을 지원한다. 하나의 클래스가 둘 이상의 기본 클래스로부터 상속받는 것이다.

```cpp
class A {
public:
    void funcA() { cout << "A" << endl; }
};

class B {
public:
    void funcB() { cout << "B" << endl; }
};

class C : public A, public B {
    // C는 A와 B 모두로부터 상속
};

int main() {
    C obj;
    obj.funcA();   // A로부터
    obj.funcB();   // B로부터
}
```

**다이아몬드 문제:** 두 기본 클래스가 공통 조상을 공유하면, 파생 클래스가 조상의 멤버를 **두 벌** 상속받을 수 있다.

```
      Base
     /    \
    A      B
     \    /
       C        ← C가 Base 멤버의 복사본 두 개를 가짐
```

**가상 상속(virtual inheritance)** 으로 공통 기본 클래스의 공유 복사본 하나만 유지할 수 있다:

```cpp
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B { };   // Base의 복사본 하나만 존재
```

> **핵심:** 다중 상속은 복잡성을 높이고 모호성을 유발할 수 있다. 대부분의 실무 상황에서는 권장하지 않는다. 가능하면 컴포지션이나 단일 상속을 사용하라.

---

<br>

## 6. 현대적 상속 기능 (C++11)

### 6.1 `final` 키워드

C++11에서는 클래스의 **추가 상속을 방지** 하거나 가상 함수의 **추가 오버라이딩을 방지** 하는 `final` 지정자가 도입되었다.

**클래스 상속 방지:**

```cpp
class Base final {
    // ...
};

// class Derived : public Base { };  // 오류: Base가 final로 표시됨
```

**함수 오버라이딩 방지:**

```cpp
class Animal {
public:
    virtual void speak() { }
};

class Dog : public Animal {
public:
    void speak() final { cout << "멍!" << endl; }  // 더 이상 오버라이딩 불가
};

class Puppy : public Dog {
public:
    // void speak() { }   // 오류: Dog::speak()가 final로 표시됨
};
```

| 사용법 | 효과 |
|:------|:----|
| `class MyClass final { };` | 어떤 클래스도 `MyClass`를 상속할 수 없음 |
| `void func() final;` | 어떤 파생 클래스도 `func()`를 오버라이딩할 수 없음 |

`final`의 활용:
- **안전성:** 중요한 클래스의 의도치 않은 확장을 방지.
- **성능:** 컴파일러가 `final` 메서드에 대한 호출을 비가상화(devirtualize)하여 가상 함수 오버헤드를 제거할 수 있다.

### 6.2 `override` 키워드

C++11에서는 함수가 기본 클래스의 가상 함수를 **오버라이딩하려는 의도** 임을 컴파일러에게 알리는 `override` 지정자도 도입되었다. 함수가 실제로 아무것도 오버라이딩하지 않으면 (예: 함수명 오타나 매개변수 불일치) 컴파일러가 오류를 발생시킨다.

```cpp
class Base {
public:
    virtual void display() const { }
};

class Derived : public Base {
public:
    void display() const override { }   // OK: Base::display()를 올바르게 오버라이딩
    // void dispaly() const override { } // 오류: 오타 — Base에 오버라이딩할 함수 없음
};
```

> **팁:** 가상 함수를 오버라이딩할 때는 항상 `override`를 사용하라. 컴파일 시점에 미묘한 버그를 잡아준다. 이 키워드는 9장 (가상 함수와 추상 클래스)에서 자세히 다룬다.

### 6.3 `using`을 사용한 생성자 상속

C++11에서는 `using` 선언을 통해 파생 클래스가 기본 클래스의 **모든 생성자를 상속** 받을 수 있다. 수동으로 전달 생성자를 작성할 필요가 없어진다.

```cpp
class Base {
public:
    Base(int x) { cout << "Base(int): " << x << endl; }
    Base(int x, int y) { cout << "Base(int, int): " << x << ", " << y << endl; }
    Base(string s) { cout << "Base(string): " << s << endl; }
};

class Derived : public Base {
public:
    using Base::Base;   // Base의 모든 생성자를 상속

    // Derived만의 생성자도 추가 가능
    Derived() : Base(0) { cout << "Derived 기본 생성자" << endl; }
};

int main() {
    Derived d1(42);          // 상속된 생성자를 통해 Base(int) 호출
    Derived d2(1, 2);        // 상속된 생성자를 통해 Base(int, int) 호출
    Derived d3("hello");     // 상속된 생성자를 통해 Base(string) 호출
    Derived d4;              // Derived 자체의 기본 생성자 호출
    return 0;
}
```

**`using Base::Base` 없이는** 각 기본 클래스 생성자에 대해 전달 생성자를 작성해야 한다:

```cpp
// 번거로운 수동 전달 (C++11 이전 스타일)
class Derived : public Base {
public:
    Derived(int x) : Base(x) { }
    Derived(int x, int y) : Base(x, y) { }
    Derived(string s) : Base(s) { }
};
```

> **참고:** 상속된 생성자는 기본 클래스 부분만 초기화한다. 파생 클래스에 초기화가 필요한 추가 멤버 변수가 있다면, 자체 생성자를 제공하거나 기본 멤버 초기화자(default member initializer)를 사용해야 한다.

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 상속이 필요한 이유 | 코드 재사용, IS-A 관계, 확장성; 기본(부모) 클래스에서 파생(자식) 클래스로 |
| 문법 | `class Derived : public Base { };` — `public`이 가장 일반적인 상속 유형 |
| 상속되는 것 | 모든 멤버가 상속됨; `private`는 직접 접근 불가; `protected`는 파생에서 접근 가능 |
| `public` 상속 | public→public, protected→protected, private→접근 불가 |
| `protected` 상속 | public→protected, protected→protected; 거의 사용하지 않음 |
| `private` 상속 | 접근 가능한 모든 멤버→private; 거의 사용하지 않음 |
| 접근 제어 표 | 상속 유형에 관계없이 기본 클래스의 `private` 멤버는 직접 접근 불가 |
| 생성자 체인 | 기본 생성자가 먼저 호출, 그다음 파생; `Derived(args) : Base(args)` 명시적 호출 |
| 소멸자 체인 | 파생 소멸자가 먼저, 그다음 기본; 생성의 역순 |
| 오버라이딩 | 파생이 같은 시그니처로 기본 함수를 재정의; 기본 버전을 숨김 |
| 기본 버전 호출 | 파생 내부에서 `Base::functionName()`; 외부에서 `obj.Base::func()` |
| 다중 상속 | `class C : public A, public B { };` — 다이아몬드 문제; 가상 상속; 실무에서 비권장 |
| `final` (C++11) | `class X final`은 상속 방지; `void f() final`은 오버라이딩 방지; 컴파일러 최적화 가능 |
| `override` (C++11) | 기본 가상 함수를 오버라이딩하려는 의도를 표시; 시그니처 불일치를 컴파일 시점에 포착 |
| 생성자 상속 (C++11) | `using Base::Base;`로 기본 클래스의 모든 생성자를 상속; 수동 전달 생성자 불필요 |

---
