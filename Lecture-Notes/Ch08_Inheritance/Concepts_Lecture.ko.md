# 8장 — 상속

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 8

> **학습 목표:**
> 1. 상속의 개념을 이해하고 파생 클래스를 선언할 수 있다
> 2. 업캐스팅과 다운캐스팅으로 상속과 객체 포인터의 관계를 설명할 수 있다
> 3. protected 접근 지정과 public, protected, private 상속의 차이를 설명할 수 있다
> 4. 상속 관계에서 생성자와 소멸자의 실행 순서를 추적할 수 있다
> 5. 다중 상속을 선언하고 가상 상속으로 다이아몬드 문제를 해결할 수 있다
> 6. final, override, 상속 생성자 같은 C++11의 상속 기능을 사용할 수 있다

---

## 목차

- [1. 상속의 개념](#1-상속의-개념)
  - [1.1 상속이 필요한 이유](#11-상속이-필요한-이유)
  - [1.2 문법](#12-문법)
  - [1.3 상속되는 것](#13-상속되는-것)
  - [1.4 상속과 객체 포인터](#14-상속과-객체-포인터)
- [2. 상속과 접근 제어](#2-상속과-접근-제어)
  - [2.1 `public` 상속](#21-public-상속)
  - [2.2 `protected` 상속](#22-protected-상속)
  - [2.3 `private` 상속](#23-private-상속)
  - [2.4 접근 제어 표](#24-접근-제어-표)
- [3. 상속에서의 생성자와 소멸자](#3-상속에서의-생성자와-소멸자)
  - [3.1 생성자 체인](#31-생성자-체인)
  - [3.2 소멸자 체인](#32-소멸자-체인)
  - [3.3 출력 추적 예제](#33-출력-추적-예제)
  - [3.4 여러 단계 상속의 생성자와 소멸자](#34-여러-단계-상속의-생성자와-소멸자)
- [4. 멤버 함수 오버라이딩](#4-멤버-함수-오버라이딩)
  - [4.1 오버라이딩](#41-오버라이딩)
  - [4.2 기본 클래스 버전 호출](#42-기본-클래스-버전-호출)
- [5. 다중 상속](#5-다중-상속)
- [6. 현대적 상속 기능 (C++11)](#6-현대적-상속-기능-c11)
  - [6.1 `final` 키워드](#61-final-키워드)
  - [6.2 `override` 키워드](#62-override-키워드)
  - [6.3 `using`을 사용한 생성자 상속](#63-using을-사용한-생성자-상속)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 상속의 개념

### 1.1 상속이 필요한 이유

**상속(Inheritance)** 은 새로운 클래스(파생 클래스)가 기존 클래스(기본 클래스)를 재사용하고 확장할 수 있게 한다.

상속은 클래스 사이에 정의되는 관계이며, 객체 사이에는 상속 관계가 없다. 파생 클래스는 기본 클래스의 속성과 기능을 물려받고 자신만의 속성과 기능을 추가하므로, 기본 클래스에서 파생 클래스로 갈수록 클래스의 개념이 구체화된다.

| 장점 | 설명 |
|:----|:----|
| 코드 재사용 | 공통 데이터와 함수를 기본 클래스에 한 번만 작성 |
| IS-A 관계 | `Car`**는** `Vehicle`**이다**; `Manager`**는** `Employee`**이다** |
| 확장성 | 기본 클래스를 수정하지 않고 파생 클래스에서 새 기능 추가 |

*표 1. 상속의 장점*

**용어:**

| 용어 | 동의어 |
|:----|:------|
| 기본 클래스 (Base class) | 부모 클래스 (Parent), 슈퍼 클래스 (Super) |
| 파생 클래스 (Derived class) | 자식 클래스 (Child), 서브 클래스 (Sub) |

*표 2. 기본 클래스와 파생 클래스의 동의어*

```text
   기본 (부모)
       ↑
   파생 (자식)
```

*그림 1. 기본 클래스와 파생 클래스의 상속 관계*

### 1.2 문법

```cpp
class Derived : public Base {
    // 추가 멤버
};
```

- 콜론 `:` 뒤에 접근 지정자(`public`, `protected`, `private`)와 기본 클래스 이름을 적는다.
- **`public` 상속**이 가장 일반적이며 IS-A 관계를 유지한다.
- `class Student : public Person`을 다시 `class StudentWorker : public Student`로 상속받으면, `StudentWorker`는 `Student`의 멤버와 함께 `Student`가 `Person`에게서 물려받은 멤버도 물려받는다.

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

기본 클래스의 모든 멤버가 파생 클래스에 상속된다. 단, **접근 가능 여부**는 접근 지정자에 따라 다르다.

| 기본 클래스 멤버 접근 | 파생 클래스에서 접근 가능? |
|:-------------------|:---------------------|
| `public` | 가능 (직접 접근 가능) |
| `protected` | 가능 (직접 접근 가능) |
| `private` | **불가**: 상속은 되지만 직접 접근할 수 없으며 public/protected getter를 통해 접근 |

*표 3. 기본 클래스 멤버의 접근 지정에 따른 파생 클래스의 접근 가능 여부*

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

`protected` 멤버는 선언된 클래스와 그 파생 클래스의 멤버 함수에서만 접근할 수 있다. 파생 클래스가 아닌 다른 클래스나 외부 함수(예: `main()`)에서는 기본 클래스 객체를 통해서든 파생 클래스 객체를 통해서든 `protected` 멤버에 접근할 수 없다. 위 예에서 `main()`이 `Derived d;`를 만든 뒤 `d.shared = 1;`을 쓰면 컴파일 오류가 발생한다.

> **핵심:** `private` 멤버는 상속된다(메모리에 존재함). 하지만 파생 클래스에서 직접 접근할 수 없다. 기본 클래스에 정의된 getter를 통해 접근해야 한다.

### 1.4 상속과 객체 포인터

파생 클래스 객체는 기본 클래스 부분을 포함하므로, 기본 클래스 포인터로 파생 클래스 객체를 가리킬 수 있다.

- **업캐스팅(up-casting)** 은 파생 클래스 포인터를 기본 클래스 포인터에 대입하는 것이다. 사람을 동물로 보는 것처럼 항상 안전하므로 명시적 형 변환이 필요 없다. 다만 기본 클래스 포인터로는 기본 클래스에 선언된 멤버만 접근할 수 있다.
- **다운캐스팅(down-casting)** 은 기본 클래스 포인터를 파생 클래스 포인터에 대입하는 것이다. 명시적 형 변환이 반드시 필요하며, 그 포인터가 실제로 파생 클래스 객체를 가리킬 때만 안전하다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Point {
    int x, y;
public:
    void set(int x, int y) { this->x = x; this->y = y; }
    void showPoint() { cout << "(" << x << "," << y << ")" << endl; }
};

class ColorPoint : public Point {
    string color;
public:
    void setColor(string color) { this->color = color; }
    void showColorPoint() { cout << color << ":"; showPoint(); }
};

int main() {
    ColorPoint cp;
    ColorPoint *pDer = &cp;
    Point *pBase = pDer;            // 업캐스팅: 형 변환 없이 가능

    pDer->set(3, 4);
    pBase->showPoint();             // (3,4)
    pDer->setColor("Red");
    pDer->showColorPoint();         // Red:(3,4)
    // pBase->showColorPoint();     // 컴파일 오류: Point에는 showColorPoint()가 없음

    ColorPoint *pDer2 = (ColorPoint *)pBase;   // 다운캐스팅: 명시적 형 변환 필요
    pDer2->showColorPoint();        // Red:(3,4)
    return 0;
}
```

실행 결과는 다음과 같다.

```text
(3,4)
Red:(3,4)
Red:(3,4)
```

```text
                ColorPoint cp
pDer  ──▶  ┌──────────────────────┐
pBase ──▶  │ x, y                 │  Point 부분: pBase로 접근 가능
           │ set(), showPoint()   │
           ├──────────────────────┤
           │ color                │  ColorPoint 부분: pDer로만 접근 가능
           │ setColor(),          │
           │ showColorPoint()     │
           └──────────────────────┘
```

*그림 2. 파생 클래스 객체의 구성과 포인터 타입에 따른 접근 범위*

| 구분 | 대입 방향 | 형 변환 | 포인터로 접근할 수 있는 멤버 |
|:----|:----|:----|:----|
| 업캐스팅 | 파생 클래스 포인터 → 기본 클래스 포인터 | 필요 없음 | 기본 클래스의 멤버 |
| 다운캐스팅 | 기본 클래스 포인터 → 파생 클래스 포인터 | 명시적 형 변환 필요 | 파생 클래스의 멤버까지 |

*표 4. 업캐스팅과 다운캐스팅의 비교*

> **주의:** 다운캐스팅한 포인터가 실제로는 파생 클래스 객체가 아닌 기본 클래스 객체를 가리키고 있으면, 파생 클래스 멤버에 접근하는 순간 정의되지 않은 동작이 된다. 다운캐스팅은 포인터가 가리키는 객체의 실제 타입을 알 때만 쓴다.

---

<br>

## 2. 상속과 접근 제어

**상속 접근 지정자**는 기본 클래스의 멤버가 파생 클래스에서 어떻게 노출되는지를 결정한다. 상속을 선언할 때 `public`, `protected`, `private` 중 하나를 지정하며, 이것이 기본 클래스의 `protected`, `public` 멤버의 접근 속성을 파생 클래스에서 어떻게 계승할지 정한다.

### 2.1 `public` 상속

가장 일반적인 형태이며, 기본 클래스의 접근 수준이 파생 클래스에서 그대로 유지된다.

```cpp
class Derived : public Base { };
```

| 기본 클래스 멤버 | 파생 클래스에서 |
|:-------------|:------------|
| `public` | `public` |
| `protected` | `protected` |
| `private` | 접근 불가 |

*표 5. `public` 상속에서 멤버의 접근 지정 변화*

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

*표 6. `protected` 상속에서 멤버의 접근 지정 변화*

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

*표 7. `private` 상속에서 멤버의 접근 지정 변화*

### 2.4 접근 제어 표

| 기본 클래스 멤버 | `public` 상속 | `protected` 상속 | `private` 상속 |
|:-------------|:------------|:---------------|:-------------|
| `public` | `public` | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private` | 접근 불가 | 접근 불가 | 접근 불가 |

*표 8. 상속 유형별 기본 클래스 멤버의 접근 지정 변화*

상속이 여러 단계로 중첩되면 각 단계의 상속 유형이 차례로 적용된다. 예를 들어 `class Derived : private Base`이면 `Base`의 public, protected 멤버가 `Derived` 안에서 private이 되므로, `Derived` 자신의 멤버 함수는 그 멤버를 호출할 수 있지만 `Derived`를 다시 상속받은 클래스의 멤버 함수는 호출할 수 없다.

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
| 기본 클래스 생성자를 명시적으로 호출하지 않을 때 | 기본 클래스의 **기본 생성자**가 자동 호출됨 |
| `Derived(args) : Base(args)` 명시적 호출 | 지정한 기본 클래스 생성자가 호출됨 |
| 기본 클래스에 기본 생성자가 없고 파생에서 호출하지 않을 때 | **컴파일 오류** |

*표 9. 파생 클래스 생성자가 호출하는 기본 클래스 생성자*

파생 클래스 생성자에 기본 클래스 생성자를 지정하지 않으면, 컴파일러는 그 생성자에 기본 클래스의 기본 생성자를 호출하는 코드(`: A()`)를 묵시적으로 삽입한다. 매개변수를 가진 파생 클래스 생성자도 마찬가지이다.

```cpp
class A {
public:
    A() { cout << "생성자 A" << endl; }
    A(int x) { cout << "매개변수생성자 A" << x << endl; }
};

class B : public A {
public:
    B() { cout << "생성자 B" << endl; }                     // B() : A()로 컴파일됨
    B(int x) { cout << "매개변수생성자 B" << x << endl; }   // B(int x) : A()로 컴파일됨
    B(int x, int y) : A(x + 3) { cout << "매개변수생성자 B" << x << endl; }   // A(int)를 명시적으로 선택
};

// 사용
B b1(5);      // 출력: 생성자 A, 매개변수생성자 B5
B b2(5, 0);   // 출력: 매개변수생성자 A8, 매개변수생성자 B5
```

### 3.2 소멸자 체인

소멸자는 생성의 **역순**으로 호출된다. 파생 클래스 소멸자가 먼저 실행되고, 그다음 기본 클래스 소멸자가 실행된다.

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

*표 10. 생성자와 소멸자의 실행 순서*

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

### 3.4 여러 단계 상속의 생성자와 소멸자

상속이 여러 단계이면 생성자는 가장 위의 기본 클래스부터, 소멸자는 가장 아래의 파생 클래스부터 실행된다. `C c;`를 선언하면 컴파일러는 `C()`가 `B()`를, `B()`가 `A()`를 먼저 호출하도록 컴파일하므로, 호출은 C, B, A 순서로 일어나지만 생성자 본문은 A, B, C 순서로 실행된다.

```cpp
#include <iostream>
using namespace std;

class A {
public:
    A() { cout << "생성자 A" << endl; }
    ~A() { cout << "소멸자 A" << endl; }
};

class B : public A {
public:
    B() { cout << "생성자 B" << endl; }
    ~B() { cout << "소멸자 B" << endl; }
};

class C : public B {
public:
    C() { cout << "생성자 C" << endl; }
    ~C() { cout << "소멸자 C" << endl; }
};

int main() {
    C c;        // c 생성
    return 0;   // c 소멸
}
```

실행 결과는 다음과 같다.

```text
생성자 A
생성자 B
생성자 C
소멸자 C
소멸자 B
소멸자 A
```

생성자의 매개변수도 각 단계의 멤버 초기화 리스트를 따라 위로 전달된다. 다음 예에서 `SmartTV`의 생성자는 32를 `WideTV(size, true)`로 넘기고, `WideTV`의 생성자는 이를 다시 `TV(size)`로 넘긴다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class TV {
    int size;   // 스크린 크기
public:
    TV() { size = 20; }
    TV(int size) { this->size = size; }
    int getSize() { return size; }
};

class WideTV : public TV {   // TV를 상속받는 WideTV
    bool videoIn;
public:
    WideTV(int size, bool videoIn) : TV(size) {
        this->videoIn = videoIn;
    }
    bool getVideoIn() { return videoIn; }
};

class SmartTV : public WideTV {   // WideTV를 상속받는 SmartTV
    string ipAddr;   // 인터넷 주소
public:
    SmartTV(string ipAddr, int size) : WideTV(size, true) {
        this->ipAddr = ipAddr;
    }
    string getIpAddr() { return ipAddr; }
};

int main() {
    // 32인치 크기에 "192.0.0.1"의 인터넷 주소를 가지는 스마트 TV 객체 생성
    SmartTV htv("192.0.0.1", 32);
    cout << "size=" << htv.getSize() << endl;
    cout << "videoIn=" << boolalpha << htv.getVideoIn() << endl;
    cout << "IP=" << htv.getIpAddr() << endl;
}
```

실행 결과는 다음과 같다.

```text
size=32
videoIn=true
IP=192.0.0.1
```

`htv` 객체 안에서 TV 부분의 `size`는 32, WideTV 부분의 `videoIn`은 `true`, SmartTV 부분의 `ipAddr`은 `"192.0.0.1"`이 된다. `boolalpha`는 `bool` 값을 1, 0 대신 `true`, `false`로 출력하게 하는 조작자이다.

---

<br>

## 4. 멤버 함수 오버라이딩

### 4.1 오버라이딩

파생 클래스가 기본 클래스와 **같은 이름과 시그니처**를 가진 함수를 정의하면, 파생 버전이 기본 버전을 **숨긴다**.

```cpp
class Base {
public:
    void show() { cout << "Base::show()" << endl; }
};

class Derived : public Base {
public:
    void show() { cout << "Derived::show()" << endl; }   // Base::show()를 재정의
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

*표 11. 파생 클래스에서 같은 이름의 함수를 다시 정의했을 때의 동작*

> **주의:** **함수 재정의와 오버라이딩.** 교재는 가상함수가 아닌 함수를 파생 클래스에서 같은 형식으로 다시 정의하는 것을 **함수 재정의(redefinition)** 라고 부르고, **오버라이딩(overriding)** 은 [9장](../Ch09_Virtual-Functions-and-Abstract-Classes/Concepts_Lecture.ko.md)에서 다루는 가상함수를 재정의하는 경우에 쓴다. 함수 재정의에서는 호출할 함수가 포인터나 객체의 선언 타입으로 정해지므로, 1.4절처럼 기본 클래스 포인터로 `Derived` 객체를 가리킨 뒤 `show()`를 호출하면 `Base::show()`가 실행된다. 또 이름 숨김은 매개변수와 관계없이 이름만으로 일어나므로, 파생 클래스에 같은 이름의 함수가 있으면 매개변수가 다른 기본 클래스의 같은 이름 함수도 파생 클래스 객체로 직접 호출할 수 없다.

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

클래스 외부에서 파생 객체를 통해 호출할 수도 있다.

```cpp
Derived d;
d.Base::show();   // Base::show()를 직접 호출
```

---

<br>

## 5. 다중 상속

C++은 **다중 상속**을 지원한다. 하나의 클래스가 둘 이상의 기본 클래스로부터 상속받는 것이다.

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

파생 클래스의 멤버 함수 안에서도 두 기본 클래스의 멤버를 직접 호출할 수 있다. 예를 들어 `MP3`와 `MobilePhone`을 다중 상속받은 `MusicPhone`의 `dial()`은 `MP3`의 `play()`로 음악을 재생하고 `MobilePhone`의 `sendCall()`로 전화를 걸 수 있다.

**다이아몬드 문제:** 두 기본 클래스가 공통 조상을 공유하면, 파생 클래스가 조상의 멤버를 **두 벌** 상속받을 수 있다.

```text
      Base
     /    \
    A      B
     \    /
       C        ← C가 Base 멤버의 복사본 두 개를 가짐
```

*그림 3. 다이아몬드 형태의 다중 상속*

이때 `C` 객체로 `Base`의 멤버에 접근하면 두 복사본 중 어느 것을 가리키는지 모호하므로 컴파일 오류가 발생한다.

**가상 상속(virtual inheritance)** 으로 공통 기본 클래스의 공유 복사본 하나만 유지할 수 있다. 파생 클래스를 선언할 때 기본 클래스 앞에 `virtual`을 붙이면, 파생 클래스의 객체가 생성될 때 가상 기본 클래스의 멤버는 한 번만 생성된다.

```cpp
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B { };   // Base의 복사본 하나만 존재
```

> **핵심:** 다중 상속은 복잡성을 높이고 모호성을 유발할 수 있다. 대부분의 실무 상황에서는 권장하지 않으며, 가능하면 컴포지션이나 단일 상속을 사용한다.

---

<br>

## 6. 현대적 상속 기능 (C++11)

### 6.1 `final` 키워드

C++11에서는 클래스가 **더 이상 상속되지 않게** 하거나 가상함수가 **더 이상 오버라이딩되지 않게** 하는 `final` 지정자가 도입되었다.

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

*표 12. `final` 지정자의 사용법과 효과*

`final`의 활용:
- **안전성:** 중요한 클래스가 의도치 않게 확장되는 것을 막는다.
- **성능:** 컴파일러가 `final` 메서드에 대한 호출을 비가상화(devirtualize)하여 가상함수 오버헤드를 제거할 수 있다.

### 6.2 `override` 키워드

C++11에서는 `override` 지정자도 도입되었다. 이 지정자는 함수가 기본 클래스의 가상함수를 오버라이딩하려는 함수임을 컴파일러에게 알린다. 함수가 실제로 아무것도 오버라이딩하지 않으면(예: 함수명 오타나 매개변수 불일치) 컴파일러가 오류를 발생시킨다.

```cpp
class Base {
public:
    virtual void display() const { }
};

class Derived : public Base {
public:
    void display() const override { }   // OK: Base::display()를 올바르게 오버라이딩
    // void dispaly() const override { } // 오류: 오타, Base에 오버라이딩할 함수 없음
};
```

> **참고:** **팁.** 가상함수를 오버라이딩할 때는 항상 `override`를 사용하는 것이 좋다. 컴파일 시점에 미묘한 버그를 잡아 준다. 이 키워드는 [9장(가상함수와 추상 클래스)](../Ch09_Virtual-Functions-and-Abstract-Classes/Concepts_Lecture.ko.md)에서 자세히 다룬다.

### 6.3 `using`을 사용한 생성자 상속

C++11에서는 `using` 선언을 통해 파생 클래스가 기본 클래스의 생성자를 **상속받을** 수 있다. 수동으로 전달 생성자를 작성할 필요가 없어진다.

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

실행 결과는 다음과 같다. `d4`는 `Derived()`가 멤버 초기화 리스트에서 `Base(0)`을 먼저 호출하므로 `Base(int): 0`이 먼저 출력된다.

```text
Base(int): 42
Base(int, int): 1, 2
Base(string): hello
Base(int): 0
Derived 기본 생성자
```

**`using Base::Base` 없이는** 각 기본 클래스 생성자에 대해 전달 생성자를 작성해야 한다.

```cpp
// 번거로운 수동 전달 (C++11 이전 스타일)
class Derived : public Base {
public:
    Derived(int x) : Base(x) { }
    Derived(int x, int y) : Base(x, y) { }
    Derived(string s) : Base(s) { }
};
```

> **참고:** 상속된 생성자는 기본 클래스 부분만 초기화한다. 파생 클래스에 초기화가 필요한 추가 멤버 변수가 있다면, 자체 생성자를 제공하거나 기본 멤버 초기화자(default member initializer)를 사용해야 한다. 또 기본 클래스의 복사 생성자와 이동 생성자는 `using` 선언으로 상속되지 않는다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:----|:---------|
| 상속이 필요한 이유 | 기본(부모) 클래스의 멤버를 파생(자식) 클래스가 물려받아 코드 재사용, IS-A 관계, 확장성을 얻는다. |
| 문법 | `class Derived : public Base { };`처럼 선언하며, `public`이 가장 일반적인 상속 유형이다. |
| 상속되는 것 | 모든 멤버가 상속되지만 `private` 멤버는 직접 접근할 수 없고, `protected` 멤버는 파생 클래스의 멤버 함수에서만 접근할 수 있다. |
| 상속과 객체 포인터 | 업캐스팅은 형 변환 없이 가능하지만 기본 클래스 멤버만 접근할 수 있고, 다운캐스팅은 명시적 형 변환이 필요하다. |
| `public` 상속 | public은 public으로, protected는 protected로 유지되고, private은 접근할 수 없다. |
| `protected` 상속 | public과 protected가 모두 protected가 되며, 거의 사용하지 않는다. |
| `private` 상속 | 접근 가능한 모든 멤버가 private이 되며, 거의 사용하지 않는다. |
| 접근 제어 표 | 상속 유형에 관계없이 기본 클래스의 `private` 멤버는 직접 접근할 수 없다. |
| 생성자 체인 | 기본 클래스 생성자가 먼저, 파생 클래스 생성자가 나중에 실행되며, `Derived(args) : Base(args)`로 기본 클래스 생성자를 명시적으로 선택한다. |
| 기본 생성자의 묵시적 호출 | 기본 클래스 생성자를 지정하지 않으면 컴파일러가 `: Base()` 호출을 삽입하므로, 기본 클래스에 기본 생성자가 없으면 컴파일 오류가 발생한다. |
| 소멸자 체인 | 파생 클래스 소멸자가 먼저, 기본 클래스 소멸자가 나중에 실행되어 생성의 역순을 따른다. |
| 여러 단계 상속 | 생성자 본문은 가장 위의 기본 클래스부터, 소멸자는 가장 아래의 파생 클래스부터 실행되고, 매개변수는 멤버 초기화 리스트를 따라 위로 전달된다. |
| 오버라이딩 | 파생 클래스가 같은 이름의 함수를 다시 정의하면 기본 버전을 숨기며, 가상함수가 아니면 교재는 이를 함수 재정의라고 부른다. |
| 기본 버전 호출 | 파생 클래스 안에서는 `Base::functionName()`, 외부에서는 `obj.Base::func()`로 호출한다. |
| 다중 상속 | `class C : public A, public B { };`처럼 선언하며, 다이아몬드 문제는 가상 상속으로 해결하고 실무에서는 권장하지 않는다. |
| `final` (C++11) | `class X final`은 상속을, `void f() final`은 오버라이딩을 막으며, 컴파일러 최적화를 가능하게 한다. |
| `override` (C++11) | 기본 가상함수를 오버라이딩하려는 의도를 표시하여 시그니처 불일치를 컴파일 시점에 잡는다. |
| 생성자 상속 (C++11) | `using Base::Base;`로 기본 클래스의 생성자를 상속받아 수동 전달 생성자를 쓰지 않아도 된다. |

---

<br>

## 점검 문제

1. **업캐스팅:** 1.4절의 `Point`와 `ColorPoint`에서 `ColorPoint cp; Point *pBase = &cp;` 다음에 `pBase->setColor("Red");`를 쓰면 컴파일되는가? 이유와 함께 `cp`의 색을 설정하는 방법을 쓰라.

   <details>
   <summary>정답</summary>

   컴파일되지 않는다. `pBase`의 타입이 `Point *`이므로 `pBase`로는 `Point`에 선언된 멤버만 접근할 수 있고, `setColor()`는 `ColorPoint`에만 있다. `cp.setColor("Red");`처럼 객체로 직접 호출하거나, `pBase`가 실제로 `ColorPoint` 객체를 가리키므로 `((ColorPoint *)pBase)->setColor("Red");`처럼 다운캐스팅한 뒤 호출하면 된다.

   </details>

2. **생성자 매개변수 전달:** 다음 코드에서 `C c(3, 5, 2);`를 실행한 뒤 `A`의 `x`, `B`의 `y`, `C`의 `m` 값과 세 생성자 본문의 실행 순서를 쓰라.

   ```cpp
   class A {
       int x;
   public:
       A() { x = 0; }
       A(int x) { this->x = x; }
   };
   class B : public A {
       int y;
   public:
       B(int x, int y) : A(x + 5) { this->y = y; }
   };
   class C : public B {
       int m;
   public:
       C(int x, int y, int z) : B(x, y) { m = x * y * z; }
   };
   ```

   <details>
   <summary>정답</summary>

   `x`는 8, `y`는 5, `m`은 30이다. `C(3, 5, 2)`가 `B(3, 5)`를, `B(3, 5)`가 `A(3 + 5)`, 곧 `A(8)`을 호출하므로 호출은 C, B, A 순서이다. 생성자 본문은 A가 먼저 실행되어 `x = 8`, 다음으로 B가 `y = 5`, 마지막으로 C가 `m = 3 * 5 * 2 = 30`을 저장한다.

   </details>

3. **`private` 상속:** 다음 코드에서 컴파일 오류가 발생하는 줄을 모두 고르고 이유를 쓰라.

   ```cpp
   class Base {
       int a;
   protected:
       void setA(int a) { this->a = a; }
   public:
       void showA() { cout << a; }
   };
   class Derived : private Base {
       int b;
   protected:
       void setB(int b) { this->b = b; }
   public:
       void showB() { cout << b; }
   };
   int main() {
       Derived x;
       x.a = 5;        // (1)
       x.setA(10);     // (2)
       x.showA();      // (3)
       x.b = 10;       // (4)
       x.setB(10);     // (5)
       x.showB();      // (6)
   }
   ```

   <details>
   <summary>정답</summary>

   (1)부터 (5)까지 모두 오류이고 (6)만 컴파일된다. (1)의 `a`는 `Base`의 private 멤버이므로 어디서도 접근할 수 없다. (2)와 (3)의 `setA()`, `showA()`는 private 상속으로 `Derived` 안에서 private이 되었으므로 `main()`에서 호출할 수 없다. (4)의 `b`는 `Derived`의 private 멤버이고, (5)의 `setB()`는 `Derived`의 protected 멤버이므로 외부 함수인 `main()`에서 접근할 수 없다. (6)의 `showB()`만 `Derived`의 public 멤버이다. 상속 유형을 `protected`로 바꾸어도 같은 줄에서 오류가 발생한다.

   </details>

4. **다이아몬드 문제:** `class Base { public: int x; };`를 `class A : public Base {};`와 `class B : public Base {};`가 상속하고, `class C : public A, public B {};`로 다중 상속했다. `C c; c.x = 1;`이 컴파일되지 않는 이유와 해결 방법을 쓰라.

   <details>
   <summary>정답</summary>

   `C` 객체 안에 `A`를 거친 `Base`와 `B`를 거친 `Base`가 하나씩, 모두 두 개 들어 있으므로 `c.x`가 어느 `Base`의 `x`인지 모호하여 컴파일 오류가 발생한다. `A`와 `B`를 `class A : virtual public Base {};`, `class B : virtual public Base {};`처럼 가상 상속으로 선언하면 `C` 객체에 `Base`가 한 번만 생성되어 `c.x`가 하나로 정해진다.

   </details>

5. **함수 재정의와 포인터:** `Base`와 `Derived`가 4.1절처럼 가상함수가 아닌 `show()`를 각각 정의할 때, `Derived d; Base *p = &d; p->show();`는 무엇을 출력하는가?

   <details>
   <summary>정답</summary>

   `Base::show()`를 출력한다. 가상함수가 아닌 함수는 포인터의 선언 타입으로 호출할 함수가 정해지므로, `p`가 `Derived` 객체를 가리켜도 `Base *` 타입이면 `Base::show()`가 호출된다. 가리키는 객체의 실제 타입에 따라 `Derived::show()`가 호출되게 하려면 9장의 가상함수가 필요하다.

   </details>
