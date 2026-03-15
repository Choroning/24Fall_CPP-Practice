# Chapter 06 — 함수 오버로딩과 static 멤버

> **최종 수정일:** 2026-03-16
>
> **교재 참조:** *명품 C++ Programming (개정판)* — 황기태 저, 6장

---

## 목차

- [1. 함수 오버로딩](#1-함수-오버로딩)
  - [1.1 개념](#11-개념)
  - [1.2 오버로딩 해석 규칙](#12-오버로딩-해석-규칙)
  - [1.3 예제](#13-예제)
- [2. 디폴트 매개변수](#2-디폴트-매개변수)
  - [2.1 문법](#21-문법)
  - [2.2 선언과 정의의 분리](#22-선언과-정의의-분리)
  - [2.3 오버로딩과의 모호성](#23-오버로딩과의-모호성)
- [3. static 멤버](#3-static-멤버)
  - [3.1 static 멤버 변수](#31-static-멤버-변수)
  - [3.2 static 멤버 함수](#32-static-멤버-함수)
  - [3.3 활용 사례](#33-활용-사례)
- [요약](#요약)

---

<br>

## 1. 함수 오버로딩

### 1.1 개념

**함수 오버로딩(function overloading)**은 **같은 이름**의 함수를 **다른 매개변수 리스트**로 여러 개 정의하는 것이다. 컴파일러가 호출 시 인수를 보고 적절한 함수를 선택한다.

```cpp
#include <iostream>
using namespace std;

int sum(int a, int b) {
    return a + b;
}

double sum(double a, double b) {
    return a + b;
}

int sum(int a, int b, int c) {
    return a + b + c;
}

int main() {
    cout << sum(1, 2) << endl;        // int sum(int, int) 호출 → 3
    cout << sum(1.5, 2.5) << endl;    // double sum(double, double) 호출 → 4
    cout << sum(1, 2, 3) << endl;     // int sum(int, int, int) 호출 → 6
    return 0;
}
```

> **핵심:** **반환 타입만으로는** 오버로딩을 구분할 수 없다. 같은 이름과 같은 매개변수 리스트를 갖고 반환 타입만 다른 두 함수는 컴파일 오류를 발생시킨다.

```cpp
int    compute(int x);    // OK
double compute(int x);    // 오류: 위와 매개변수 리스트가 동일
```

**오버로딩을 구분하는 요소와 구분하지 못하는 요소:**

| 구분 가능 | 구분 불가 |
|:---------|:---------|
| 매개변수의 개수 | 반환 타입 |
| 매개변수의 타입 | 매개변수 이름 |
| 매개변수 타입의 순서 | |

### 1.2 오버로딩 해석 규칙

오버로딩된 함수가 호출되면, 컴파일러는 다음 우선순위에 따라 최적 일치를 찾는다:

| 우선순위 | 일치 유형 | 설명 |
|:--------|:---------|:----|
| 1 | **정확한 일치 (exact match)** | 인수 타입이 매개변수 타입과 정확히 일치 |
| 2 | **자동 형 승격 (promotion)** | `char`/`short` → `int`, `float` → `double` |
| 3 | **표준 형 변환 (conversion)** | `int` → `double`, `double` → `int` 등 |

**같은 우선순위**에서 여러 함수가 일치하면 호출이 **모호(ambiguous)**하여 컴파일 오류가 발생한다.

```cpp
void print(int x)    { cout << "int: " << x << endl; }
void print(double x) { cout << "double: " << x << endl; }
void print(char x)   { cout << "char: " << x << endl; }

print(10);       // 정확한 일치 → print(int)
print(3.14);     // 정확한 일치 → print(double)
print('A');      // 정확한 일치 → print(char)
print(3.14f);    // 형 승격: float → double → print(double)
```

**모호성 예제:**

```cpp
void f(float x)  { cout << "float" << endl; }
void f(double x) { cout << "double" << endl; }

f(10);   // 오류: int → float과 int → double 모두 표준 형 변환
         // 모호함 — 컴파일러가 선택 불가
```

### 1.3 예제

**절대값:**

```cpp
int abs(int x) {
    return (x >= 0) ? x : -x;
}

double abs(double x) {
    return (x >= 0) ? x : -x;
}

int main() {
    cout << abs(-5) << endl;      // 5 (int 버전)
    cout << abs(-3.14) << endl;   // 3.14 (double 버전)
    return 0;
}
```

**다양한 타입 출력:**

```cpp
#include <iostream>
#include <string>
using namespace std;

void print(int x)          { cout << "int: " << x << endl; }
void print(string s)       { cout << "string: " << s << endl; }
void print(int x, int y)   { cout << "two ints: " << x << ", " << y << endl; }

int main() {
    print(42);             // int: 42
    print("hello");        // string: hello
    print(3, 7);           // two ints: 3, 7
    return 0;
}
```

---

<br>

## 2. 디폴트 매개변수

### 2.1 문법

**디폴트 매개변수**는 호출자가 인수를 생략했을 때 사용되는 기본값을 제공한다. 디폴트는 **가장 오른쪽** 매개변수부터 지정해야 한다.

```cpp
#include <iostream>
using namespace std;

void greet(string name, string msg = "Hello", int times = 1) {
    for (int i = 0; i < times; i++)
        cout << msg << ", " << name << "!" << endl;
}

int main() {
    greet("Alice");                  // Hello, Alice!        (msg="Hello", times=1)
    greet("Bob", "Hi");             // Hi, Bob!              (times=1)
    greet("Carol", "Hey", 3);       // Hey, Carol! (x3)
    return 0;
}
```

**규칙:**

| 규칙 | 올바른 예 | 잘못된 예 |
|:----|:---------|:---------|
| 디폴트는 가장 오른쪽부터 | `void f(int a, int b = 1, int c = 2)` | `void f(int a = 1, int b, int c = 2)` |
| 중간 디폴트 생략 불가 | `f(10)` 또는 `f(10, 20)` 또는 `f(10, 20, 30)` | `f(10, , 30)` |

```cpp
// 오류: 디폴트가 가장 오른쪽에 있지 않음
void bad(int a = 1, int b, int c = 3);  // b에 디폴트 없음, 하지만 a에 있음

// 올바름: 디폴트가 오른쪽부터 연속
void ok(int a, int b = 2, int c = 3);
```

### 2.2 선언과 정의의 분리

함수 선언(원형)과 정의가 **별도**일 때, 디폴트 값은 **선언에만** 지정해야 한다. 정의에는 지정하지 않는다.

```cpp
// 헤더 또는 main 이전 — 디폴트가 포함된 선언
void display(int x, int y = 0, int z = 0);

int main() {
    display(1);          // y=0, z=0
    display(1, 2);       // z=0
    display(1, 2, 3);
    return 0;
}

// 정의 — 여기에는 디폴트를 쓰지 않음
void display(int x, int y, int z) {
    cout << x << " " << y << " " << z << endl;
}
```

> **경고:** 선언과 정의 **모두**에 디폴트를 지정하면 컴파일 오류(디폴트 인수 재정의)가 발생한다.

### 2.3 오버로딩과의 모호성

디폴트 매개변수는 오버로딩된 함수와 **모호성**을 일으킬 수 있다.

```cpp
void func(int a) {
    cout << "매개변수 1개: " << a << endl;
}

void func(int a, int b = 10) {
    cout << "매개변수 2개: " << a << ", " << b << endl;
}

int main() {
    func(5);       // 오류: 모호!
    // func(int)일 수도 있고 func(int, int)에서 b가 10으로 기본값일 수도 있음
    // 컴파일러가 결정 불가 → 컴파일 오류

    func(5, 20);   // OK: func(int, int)만 일치
    return 0;
}
```

> **팁:** 뒤쪽 디폴트 매개변수만 다른 오버로딩을 정의하지 말라. `func(int)`가 있으면 `func(int, int = 기본값)`을 추가하지 않는 것이 좋다.

---

<br>

## 3. static 멤버

### 3.1 static 멤버 변수

**static 멤버 변수**는 클래스의 **모든 객체가 공유**한다. 객체가 몇 개 생성되든 메모리에 **단 하나**만 존재한다.

**선언과 정의:**

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    static int count;   // 클래스 내부에서 선언

    Circle(int r) : radius(r) {
        count++;
    }
    ~Circle() {
        count--;
    }
};

// 클래스 외부에서 반드시 정의 및 초기화
int Circle::count = 0;

int main() {
    Circle a(10);
    Circle b(20);
    Circle c(30);
    cout << "원의 개수: " << Circle::count << endl;   // 3
    cout << "객체로 접근: " << a.count << endl;         // 3 (같은 값)
    return 0;
}
```

```
메모리 구조:

static 멤버 (1개만 존재):
┌──────────────────┐
│ Circle::count = 3 │  ← 모든 객체가 공유
└──────────────────┘

객체 (각각 별도 복사본):
┌──────────┐  ┌──────────┐  ┌──────────┐
│ a        │  │ b        │  │ c        │
│ radius=10│  │ radius=20│  │ radius=30│
└──────────┘  └──────────┘  └──────────┘
```

**핵심 규칙:**

| 규칙 | 상세 |
|:----|:----|
| 클래스 내부에 `static`으로 선언 | `static int count;` |
| 클래스 외부에서 반드시 정의 | `int ClassName::count = 0;` |
| 모든 객체가 공유 | 하나의 사본만 존재 |
| 클래스 이름 또는 객체로 접근 | `ClassName::count` 또는 `obj.count` |
| 객체가 0개여도 존재 | 프로그램 전체 수명 동안 유지 |

> **경고:** 클래스 외부 정의(`int Circle::count = 0;`)를 빠뜨리면 컴파일 오류가 아닌 **링커 오류**가 발생한다. 클래스 내부의 선언은 존재를 알릴 뿐이고, 외부 정의가 실제로 메모리를 할당한다.

### 3.2 static 멤버 함수

**static 멤버 함수**는 **static 멤버만** 접근할 수 있다. 특정 객체에 연결되지 않으므로 **`this` 포인터가 없다**.

```cpp
#include <iostream>
using namespace std;

class Math {
public:
    static int abs(int x) {
        return (x >= 0) ? x : -x;
    }

    static int max(int a, int b) {
        return (a > b) ? a : b;
    }
};

int main() {
    // 클래스 이름으로 호출 — 객체 불필요
    cout << Math::abs(-5) << endl;    // 5
    cout << Math::max(3, 7) << endl;  // 7
    return 0;
}
```

**static 멤버 함수의 제약:**

```cpp
class Example {
    int data;              // 비static 멤버
    static int shared;     // static 멤버
public:
    static void staticFunc() {
        // shared = 10;    // OK: static 멤버 접근 가능
        // data = 20;      // 오류: 비static 멤버 접근 불가
        // this->data;     // 오류: this 포인터 없음
    }

    void normalFunc() {
        shared = 10;       // OK: 비static에서 static 접근 가능
        data = 20;         // OK: 비static에서 비static 접근 가능
    }
};

int Example::shared = 0;
```

| 특성 | static 멤버 함수 | 비static 멤버 함수 |
|:----|:----------------|:-----------------|
| `this` 포인터 | 없음 | 있음 |
| static 멤버 접근 | 가능 | 가능 |
| 비static 멤버 접근 | **불가** | 가능 |
| 객체 없이 호출 | `ClassName::func()` | 불가능 |

### 3.3 활용 사례

**객체 수 관리:**

```cpp
class Student {
    string name;
    static int count;
public:
    Student(string n) : name(n) { count++; }
    ~Student() { count--; }
    static int getCount() { return count; }
};

int Student::count = 0;

int main() {
    Student a("Alice");
    Student b("Bob");
    cout << Student::getCount() << endl;   // 2
    {
        Student c("Carol");
        cout << Student::getCount() << endl;   // 3
    }   // c가 여기서 소멸
    cout << Student::getCount() << endl;   // 2
    return 0;
}
```

**static 배열을 활용한 게시판 (수업 문제 기반):**

```cpp
#include <iostream>
#include <string>
#define MAX_NO 5
using namespace std;

class Board {
    static int no;                  // 현재 게시물 수
    static string board[MAX_NO];   // 게시물의 static 배열
    Board() {}                     // private 생성자 — 객체 생성 방지
public:
    static void add(string text) {
        if (no >= MAX_NO) {
            cout << "추가 실패! 게시물 최대 " << MAX_NO << "개입니다." << endl;
            return;
        }
        board[no++] = text;
    }

    static void print() {
        if (no == 0) {
            cout << "<등록된 게시물이 없습니다.>" << endl;
            return;
        }
        for (int i = 0; i < no; i++)
            cout << i << ": " << board[i] << endl;
    }

    static void clear() {
        no = 0;
    }
};

int Board::no = 0;
string Board::board[MAX_NO];

int main() {
    Board::add("과제 제출 기한은 금요일입니다.");
    Board::add("실습 장소가 301호로 변경되었습니다.");
    Board::print();
    // 0: 과제 제출 기한은 금요일입니다.
    // 1: 실습 장소가 301호로 변경되었습니다.
    Board::clear();
    Board::print();
    // <등록된 게시물이 없습니다.>
    return 0;
}
```

**이 패턴의 핵심 설계 포인트:**

| 포인트 | 설명 |
|:------|:----|
| private 생성자 | `Board` 객체 생성을 막음 — 모든 상호작용은 static 함수를 통해 |
| static 배열 + static 카운트 | 객체 없이 데이터 존재; 클래스 이름으로만 관리 |
| 모든 함수가 static | `Board::add(...)`, `Board::print()` — 객체 없이 호출 |

---

<br>

## 요약

| 개념 | 핵심 정리 |
|:----|:---------|
| 함수 오버로딩 | 같은 이름, 다른 매개변수 리스트; 반환 타입만으로는 구분 불가 |
| 오버로딩 해석 | 정확한 일치 > 형 승격 > 형 변환; 같은 우선순위에서 여러 일치 시 모호성 오류 |
| 디폴트 매개변수 | `void f(int a, int b = 10)` — 디폴트는 가장 오른쪽부터; 중간 생략 불가 |
| 디폴트는 선언에만 | 원형과 정의가 분리된 경우 디폴트는 원형에만 지정 |
| 디폴트 + 오버로딩 모호성 | `f(int)` vs `f(int, int = 10)` → `f(5)` 호출 시 모호 — 이 패턴을 피할 것 |
| static 멤버 변수 | 모든 객체가 공유; 1개만 존재; 클래스 내 `static` 선언; 클래스 외부에서 정의 |
| static 멤버 변수 정의 | `int ClassName::count = 0;` 클래스 외부 — 빠뜨리면 링커 오류 |
| static 멤버 함수 | `this` 포인터 없음; static 멤버만 접근 가능; `ClassName::func()`로 호출 |
| 객체 수 관리 | 생성자에서 static count 증가, 소멸자에서 감소 |
| private 생성자 + static 멤버 | 객체 생성 방지; 모든 데이터/함수를 클래스 이름으로 접근 (Board 패턴) |

---
