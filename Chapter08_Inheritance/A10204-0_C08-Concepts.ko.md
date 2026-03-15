# Chapter 08 — 상속

> **최종 수정일:** 2026-03-16
>
> **교재 참조:** *명품 C++ Programming 개정판* — 8장

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
- [요약](#요약)

---

<br>

## 1. 상속의 개념

### 1.1 상속이 필요한 이유

**상속(Inheritance)**은 새로운 클래스(파생 클래스)가 기존 클래스(기본 클래스)를 재사용하고 확장할 수 있게 한다.

| 장점 | 설명 |
|:----|:----|
| 코드 재사용 | 공통 데이터와 함수를 기본 클래스에 한 번만 작성 |
| IS-A 관계 | `ColorPoint`**는** `Point`**이다**; `FirstGrade`**는** `Student`**이다** |
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
- **`public` 상속**이 가장 일반적이며 IS-A 관계를 유지한다.

```cpp
class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) {}
    int getX() { return x; }
    int getY() { return y; }
protected:
    void move(int x, int y) { this->x = x; this->y = y; }
};

class ColorPoint : public Point {
    string color;
public:
    ColorPoint(int x = 0, int y = 0, string c = "BLACK")
        : Point(x, y), color(c) {}
    void setColor(string c) { color = c; }
    void setPoint(int x, int y) { move(x, y); }   // 기본 클래스의 protected 멤버 호출
    void show() {
        cout << color << "색으로 (" << getX() << ", " << getY()
             << ")에 위치한 점입니다." << endl;
    }
};
```

### 1.3 상속되는 것

기본 클래스의 모든 멤버가 파생 클래스에 상속된다. 단, **접근 가능 여부**는 접근 지정자에 따라 다르다.

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

**상속 접근 지정자**는 기본 클래스의 멤버가 파생 클래스에서 어떻게 노출되는지를 결정한다.

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

파생 객체가 생성될 때, **기본 클래스 생성자가 먼저** 실행되고, 그다음 파생 클래스 생성자가 실행된다. 기본 클래스 생성자를 명시적으로 호출하지 않으면, 기본 클래스의 **기본 생성자(default constructor)**가 사용된다.

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
| 기본 생성자를 명시적으로 호출하지 않을 때 | 기본 클래스의 **기본 생성자**가 자동 호출됨 |
| `Derived(args) : Base(args)` 명시적 호출 | 지정한 기본 클래스 생성자가 호출됨 |
| 기본 클래스에 기본 생성자가 없고 파생에서 호출하지 않을 때 | **컴파일 오류** |

### 3.2 소멸자 체인

소멸자는 생성의 **역순**으로 호출된다: 파생 클래스 소멸자가 먼저 실행되고, 그다음 기본 클래스 소멸자가 실행된다.

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

class Student {
    string name;
protected:
    int korean, info;
public:
    Student(string n, int kor, int inf)
        : name(n), korean(kor), info(inf) {
        cout << "Student 생성자" << endl;
    }
    ~Student() { cout << "Student 소멸자" << endl; }
    string getName() { return name; }
    int getKorean() { return korean; }
    int getInfo() { return info; }
};

class FirstGrade : public Student {
    int math;
public:
    FirstGrade(string n, int kor, int inf, int m)
        : Student(n, kor, inf), math(m) {
        cout << "FirstGrade 생성자" << endl;
    }
    ~FirstGrade() { cout << "FirstGrade 소멸자" << endl; }
    double getAverage() {
        return (korean + info + math) / 3.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << korean << endl;
        cout << "정보: " << info << endl;
        cout << "수학: " << math << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

int main() {
    FirstGrade f("홍길동", 90, 81, 88);
    f.display();
    return 0;
}
```

**출력:**

```text
Student 생성자
FirstGrade 생성자
이름: 홍길동
국어: 90
정보: 81
수학: 88
---------------
평균: 86.3333
FirstGrade 소멸자
Student 소멸자
```

> **핵심:** 생성은 기본 → 파생 순서로 진행된다. 소멸은 파생 → 기본 순서로 진행된다. 이렇게 해야 기본 멤버에 의존하는 파생 멤버가 올바르게 초기화되고 정리된다.

---

<br>

## 4. 멤버 함수 오버라이딩

### 4.1 오버라이딩

파생 클래스가 기본 클래스와 **같은 이름과 시그니처**를 가진 함수를 정의하면, 파생 버전이 기본 버전을 **숨긴다**(오버라이딩한다).

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

**다이아몬드 문제:** 두 기본 클래스가 공통 조상을 공유하면, 파생 클래스가 조상의 멤버를 **두 벌** 상속받을 수 있다.

```
      Base
     /    \
    A      B
     \    /
       C        ← C가 Base 멤버의 복사본 두 개를 가짐
```

**가상 상속(virtual inheritance)**으로 공통 기본 클래스의 공유 복사본 하나만 유지할 수 있다:

```cpp
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B { };   // Base의 복사본 하나만 존재
```

> **핵심:** 다중 상속은 복잡성을 높이고 모호성을 유발할 수 있다. 대부분의 실무 상황에서는 권장하지 않는다. 가능하면 컴포지션이나 단일 상속을 사용하라.

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

---
