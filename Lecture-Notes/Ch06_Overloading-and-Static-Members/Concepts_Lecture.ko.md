# 6장 — 함수 중복과 static 멤버

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 6

> **학습 목표:**
> 1. 함수 중복이 성립하는 조건을 설명하고 중복된 함수를 작성할 수 있다
> 2. 디폴트 매개변수를 선언하고 생략된 인자가 채워지는 규칙을 적용할 수 있다
> 3. 형 변환, 참조 매개변수, 디폴트 매개변수로 인한 함수 중복의 모호성을 판별할 수 있다
> 4. static 멤버와 non-static 멤버의 차이를 설명하고 static 멤버를 선언하고 접근할 수 있다
> 5. static 멤버 함수의 제약을 이해하고 전역 함수와 공유 변수를 클래스로 캡슐화할 수 있다

---

## 목차

- [1. 함수 중복](#1-함수-중복)
  - [1.1 개념](#11-개념)
  - [1.2 함수 중복 해석 규칙](#12-함수-중복-해석-규칙)
  - [1.3 예제](#13-예제)
  - [1.4 생성자 중복과 소멸자](#14-생성자-중복과-소멸자)
- [2. 디폴트 매개변수](#2-디폴트-매개변수)
  - [2.1 문법](#21-문법)
  - [2.2 선언과 정의의 분리](#22-선언과-정의의-분리)
  - [2.3 함수 중복과의 모호성](#23-함수-중복과의-모호성)
  - [2.4 디폴트 매개변수로 함수 중복 간소화](#24-디폴트-매개변수로-함수-중복-간소화)
- [3. static 멤버](#3-static-멤버)
  - [3.1 static 멤버 변수](#31-static-멤버-변수)
  - [3.2 static 멤버의 접근 방법](#32-static-멤버의-접근-방법)
  - [3.3 static 멤버 함수](#33-static-멤버-함수)
  - [3.4 활용 사례](#34-활용-사례)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 함수 중복

### 1.1 개념

**함수 중복(function overloading)** 은 **같은 이름**의 함수를 **다른 매개변수 리스트**로 여러 개 정의하는 것이다. 컴파일러가 호출 시 인자를 보고 적절한 함수를 선택한다.

함수 오버로딩이라고도 부른다. 한 이름이 인자에 따라 여러 형태로 동작하므로 다형성(polymorphism)의 한 형태이며, C 언어에서는 같은 이름의 함수를 둘 이상 정의할 수 없다. 함수 중복은 일반 함수 사이와 한 클래스의 멤버 함수 사이에서 할 수 있다.

> **참고:** **상속과 이름 가리기.** 교재는 상속 관계에 있는 기본 클래스와 파생 클래스의 멤버 함수 사이에서도 함수 중복이 가능하다고 설명한다. 그러나 파생 클래스에 같은 이름의 멤버 함수를 하나라도 선언하면 기본 클래스의 같은 이름 함수는 모두 가려지므로(name hiding), 파생 클래스 객체로 호출할 때 두 클래스의 함수가 함께 호출 후보가 되지 않는다. 함께 후보로 만들려면 파생 클래스 안에 `using Base::f;`처럼 기본 클래스의 이름을 끌어온다. 상속은 8장에서 다룬다.

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

함수 중복을 쓰면 비슷한 일을 하는 함수마다 `msg1()`, `msg2(name)`, `msg3(id, name)`처럼 다른 이름을 붙여 기억할 필요 없이 `msg()`, `msg(name)`, `msg(id, name)`처럼 한 이름으로 호출할 수 있어 호출 실수가 줄어든다.

> **핵심:** **리턴 타입만으로는** 중복된 함수를 구분할 수 없다. 같은 이름과 같은 매개변수 리스트를 갖고 리턴 타입만 다른 두 함수는 컴파일 오류를 발생시킨다.

```cpp
int    compute(int x);    // OK
double compute(int x);    // 오류: 위와 매개변수 리스트가 동일
```

**중복된 함수를 구분하는 요소와 구분하지 못하는 요소:**

| 구분 가능 | 구분 불가 |
|:---------|:---------|
| 매개변수의 개수 | 리턴 타입 |
| 매개변수의 타입 | 매개변수 이름 |
| 매개변수 타입의 순서 | |

*표 1. 중복된 함수를 구분하는 요소와 구분하지 못하는 요소*

### 1.2 함수 중복 해석 규칙

중복된 함수가 호출되면, 컴파일러는 다음 우선순위에 따라 최적 일치를 찾는다:

| 우선순위 | 일치 유형 | 설명 |
|:--------|:---------|:----|
| 1 | **정확한 일치 (exact match)** | 인자 타입이 매개변수 타입과 정확히 일치 |
| 2 | **자동 형 승격 (promotion)** | `char`/`short` → `int`, `float` → `double` |
| 3 | **표준 형 변환 (conversion)** | `int` → `double`, `double` → `int` 등 |

*표 2. 함수 중복 해석에서 일치 유형의 우선순위*

그다음 순위로 생성자나 변환 연산자를 이용하는 사용자 정의 변환과 가변 인자(`...`) 일치가 있다. **같은 우선순위**에서 여러 함수가 일치하면 호출이 **모호(ambiguous)** 하여 컴파일 오류가 발생한다.

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

반면 `f(10.0)`처럼 `double` 값을 넘기면 `f(double)`이 정확한 일치이므로 모호하지 않다.

**참조 매개변수로 인한 모호성:**

```cpp
int add(int a, int b)  { return a + b; }
int add(int a, int &b) { b = b + a; return b; }

int s = 10, t = 20;
add(s, t);     // 오류: 값으로 복사하는 것과 참조로 바인딩하는 것이 모두 정확한 일치
add(10, 20);   // OK: 리터럴 20은 int&에 바인딩할 수 없어 add(int, int)만 후보
```

변수 `t`를 넘기면 `int b`로 복사하는 것과 `int &b`로 바인딩하는 것이 모두 정확한 일치이므로 컴파일러가 고를 수 없다. 따라서 매개변수가 값과 참조라는 점만 다른 두 함수는 사실상 함께 중복할 수 없다. 함수 중복의 모호성은 이처럼 형 변환, 참조 매개변수, 디폴트 매개변수(2.3절) 때문에 생긴다.

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

> **주의:** 표준 라이브러리에도 `abs()`가 있으므로, 이 예제를 `<iostream>`, `using namespace std;`와 함께 전역 범위에 그대로 쓰면 구현에 따라 이미 선언된 `abs()`와 충돌하여 컴파일 오류가 날 수 있다. 예를 들어 Apple clang의 libc++에서는 `double abs(double)` 정의가 충돌 오류를 낸다. 실제 코드에서는 `myAbs()`처럼 다른 이름을 쓰거나 3.3절의 `Math` 클래스처럼 static 멤버로 감싼다.

**배열에서 가장 큰 수:**

```cpp
#include <iostream>
using namespace std;

int big(int a, int b) {             // a와 b 중 큰 수 반환
    if (a > b) return a;
    else return b;
}

int big(int a[], int size) {        // 배열 a에서 가장 큰 수 반환
    int res = a[0];
    for (int i = 1; i < size; i++)
        if (res < a[i]) res = a[i];
    return res;
}

int main() {
    int array[5] = {1, 9, -2, 8, 6};
    cout << big(2, 3) << endl;        // 3
    cout << big(array, 5) << endl;    // 9
    return 0;
}
```

두 `big()`은 매개변수의 타입과 의미가 다르므로 함수 중복이 성립한다. `big(2, 3)`은 정수 두 개를 받는 버전을, `big(array, 5)`는 배열 포인터와 크기를 받는 버전을 호출한다.

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

### 1.4 생성자 중복과 소멸자

생성자도 중복할 수 있다. 객체를 만들 때 매개변수를 통해 여러 형태의 초깃값을 넘길 수 있게 하는 것이 목적이다.

```cpp
class Circle {
    int radius;
public:
    Circle() { radius = 1; }          // 매개변수 없는 생성자
    Circle(int r) { radius = r; }     // 반지름을 받는 생성자
};

Circle donut;        // Circle() 호출
Circle pizza(30);    // Circle(int r) 호출
```

표준 라이브러리의 `string` 클래스도 `string()`, `string(const string&)`, `string(const char*)`처럼 생성자를 중복하여 `string str;`, `string copyAddress(address);`, `string address("서울시 성북구 삼선동 389");` 형태의 생성을 모두 지원한다.

반면 소멸자는 중복할 수 없다. 소멸자는 매개변수를 가질 수 없으므로 한 클래스에 오직 하나만 존재한다.

---

<br>

## 2. 디폴트 매개변수

### 2.1 문법

**디폴트 매개변수**는 호출자가 인자를 생략했을 때 사용되는 기본값을 제공한다. 디폴트는 **가장 오른쪽** 매개변수부터 지정해야 한다.

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

*표 3. 디폴트 매개변수의 선언과 호출 규칙*

```cpp
// 오류: 디폴트가 가장 오른쪽에 있지 않음
void bad(int a = 1, int b, int c = 3);  // b에 디폴트 없음, 하지만 a에 있음

// 올바름: 디폴트가 오른쪽부터 연속
void ok(int a, int b = 2, int c = 3);
```

같은 이유로 `void calc(int a, int b = 5, int c, int d = 0);`처럼 디폴트 매개변수 뒤에 일반 매개변수가 오면 컴파일 오류이고, `void calc(int a, int b = 5, int c = 0, int d = 0);`은 올바르다. 디폴트 매개변수는 끝 쪽에 몰려 있어야 한다.

호출에서 넘긴 인자는 왼쪽 매개변수부터 차례로 채워지고, 남은 매개변수에 디폴트 값이 들어간다. 컴파일러는 `void g(int a, int b = 0, int c = 0, int d = 0);`에 대한 호출을 다음과 같이 바꾼다.

| 호출 | 컴파일러가 바꾼 호출 |
|:----|:----|
| `g(10)` | `g(10, 0, 0, 0)` |
| `g(10, 5)` | `g(10, 5, 0, 0)` |
| `g(10, 5, 20)` | `g(10, 5, 20, 0)` |
| `g(10, 5, 20, 30)` | `g(10, 5, 20, 30)` |

*표 4. 디폴트 매개변수를 가진 g()의 호출과 채워지는 값*

디폴트가 없는 매개변수에는 반드시 인자를 넘겨야 한다.

```cpp
void msg(int id, string text = "Hello");   // text의 디폴트 값은 "Hello"

msg(10);                  // msg(10, "Hello")와 같음
msg(20, "Good Morning");  // id에 20, text에 "Good Morning" 전달
msg();                    // 컴파일 오류: 첫 번째 매개변수 id에 값이 없음
msg("Hello");             // 컴파일 오류: 문자열을 int 매개변수 id에 넘길 수 없음
```

### 2.2 선언과 정의의 분리

함수 선언(원형)과 정의가 **별도**일 때, 디폴트 값은 **선언에만** 지정하고 정의에는 쓰지 않는다. 같은 매개변수의 디폴트 값은 한 번만 지정할 수 있고, 호출하는 쪽이 보는 원형에 디폴트가 있어야 인자를 생략할 수 있기 때문이다. 원형 없이 정의만 있으면 `void star(int a = 5) { ... }`처럼 정의에 디폴트를 쓴다.

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

> **주의:** 선언과 정의 **모두**에 디폴트를 지정하면 컴파일 오류(디폴트 인자 재정의)가 발생한다.

### 2.3 함수 중복과의 모호성

디폴트 매개변수는 중복된 함수와 **모호성**을 일으킬 수 있다.

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

> **참고:** **팁.** 뒤쪽 디폴트 매개변수만 다른 중복 함수를 정의하지 말라. `func(int)`가 있으면 `func(int, int = 기본값)`을 추가하지 않는 것이 좋다.

### 2.4 디폴트 매개변수로 함수 중복 간소화

디폴트 매개변수를 쓰면 여러 중복 함수를 하나로 합칠 수 있다. 다음 두 생성자는 디폴트 매개변수를 가진 생성자 하나로 바꿀 수 있다.

```cpp
// 오버로딩한 두 생성자
class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int r) { radius = r; }
};

// 디폴트 매개변수를 가진 생성자 하나
class Circle {
    int radius;
public:
    Circle(int r = 1) { radius = r; }
};
```

일반 함수도 마찬가지이다. 25개의 `'*'`를 한 줄에 출력하는 `fillLine()`과 `n`개의 `c`를 한 줄에 출력하는 `fillLine(int n, char c)`는 하나로 합칠 수 있다.

```cpp
#include <iostream>
using namespace std;

void fillLine(int n = 25, char c = '*') {   // n개의 c 문자를 한 줄에 출력
    for (int i = 0; i < n; i++) cout << c;
    cout << endl;
}

int main() {
    fillLine();           // *************************
    fillLine(10, '%');    // %%%%%%%%%%
    return 0;
}
```

> **주의:** 합친 함수와 원래의 중복 함수를 함께 두면 안 된다. `Circle(int r)`와 `Circle(int r = 1)`은 시그니처가 같아 재선언 오류가 되고, `Circle()`과 `Circle(int r = 1)`을 함께 두면 `Circle c;`가 두 생성자에 모두 일치하여 모호성 오류가 된다.

동적 배열을 할당하는 생성자도 같은 방법으로 합칠 수 있다.

```cpp
class MyVector {
    int *p;
    int size;
public:
    MyVector(int n = 100) {          // MyVector()와 MyVector(int n)을 하나로 합침
        p = new int[n];
        size = n;
    }
    ~MyVector() { delete [] p; }
};

MyVector *v1 = new MyVector();       // 정수 100개 배열 할당
MyVector *v2 = new MyVector(1024);   // 정수 1024개 배열 할당
```

생성자를 합치는 다른 방법으로 C++11의 **위임 생성자(delegating constructor)** 가 있다. `MyVector() : MyVector(100) { }`처럼 쓰면 매개변수 없는 생성자가 초기화를 `MyVector(int n)`에 맡긴다.

---

<br>

## 3. static 멤버

### 3.1 static 멤버 변수

**static 멤버 변수**는 클래스의 **모든 객체가 공유**한다. 객체가 몇 개 생성되든 메모리에 **단 하나**만 존재한다.

`static`은 원래 변수와 함수의 기억 부류(storage class) 중 하나로, static 대상은 프로그램이 시작할 때 생성되어 프로그램이 끝날 때 소멸하고 사용 범위는 선언된 범위와 접근 지정을 따른다. 클래스의 멤버는 이 속성에 따라 다음 두 가지로 나뉜다.

- **static 멤버**는 클래스당 하나만 생성되어 그 클래스의 모든 객체가 공유한다. 그래서 **클래스 멤버(class member)** 라고도 부른다.
- **non-static 멤버**는 객체가 생성될 때 객체마다 따로 생성된다. 그래서 **인스턴스 멤버(instance member)** 라고도 부른다.

> **참고:** **비유.** 사람은 저마다 자기 눈을 가지고 태어나듯 non-static 멤버는 객체마다 따로 있다. 반면 공기는 사람이 태어나기 전부터 있고 모든 사람이 함께 쓰듯, static 멤버는 객체가 생기기 전부터 존재하며 모든 객체가 공유한다.

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

```text
메모리 구조:

static 멤버 (1개만 존재):
┌───────────────────┐
│ Circle::count = 3 │  ← 모든 객체가 공유
└───────────────────┘

객체 (각각 별도 복사본):
┌───────────┐  ┌───────────┐  ┌───────────┐
│ a         │  │ b         │  │ c         │
│ radius=10 │  │ radius=20 │  │ radius=30 │
└───────────┘  └───────────┘  └───────────┘
```

*그림 1. 공유되는 static 멤버 변수와 객체마다 있는 멤버 변수의 메모리 배치*

**핵심 규칙:**

| 규칙 | 상세 |
|:----|:----|
| 클래스 내부에 `static`으로 선언 | `static int count;` |
| 클래스 외부에서 반드시 정의 | `int ClassName::count = 0;` |
| 모든 객체가 공유 | 하나의 사본만 존재 |
| 클래스 이름 또는 객체로 접근 | `ClassName::count` 또는 `obj.count` |
| 객체가 0개여도 존재 | 프로그램 전체 수명 동안 유지 |

*표 5. static 멤버 변수의 핵심 규칙*

| 특성 | non-static 멤버 | static 멤버 |
|:----|:----|:----|
| 선언 예 | `int n;`, `void f();` | `static int n;`, `static void f();` |
| 공간적 특성 | 객체마다 별도로 생성된다 (인스턴스 멤버) | 클래스당 하나만 객체 밖의 전역 공간에 생성된다 (클래스 멤버) |
| 시간적 특성 | 객체가 생성될 때 생기고 객체가 소멸할 때 함께 소멸한다 | 프로그램이 시작할 때 생겨 객체가 생기기 전과 소멸한 뒤에도 존재하고, 프로그램이 끝날 때 소멸한다 |
| 공유 | 공유되지 않고 객체마다 따로 값을 가진다 | 같은 클래스의 모든 객체가 공유한다 |

*표 6. non-static 멤버와 static 멤버의 비교*

> **주의:** 클래스 외부 정의(`int Circle::count = 0;`)를 빠뜨리면 컴파일 오류가 아닌 **링크 오류**가 발생한다. 클래스 내부의 선언은 존재를 알릴 뿐이고, 외부 정의가 실제로 메모리를 할당한다.

#### `constexpr` vs `const`: 컴파일 타임 상수

C++11부터 `constexpr`은 **컴파일 시점**에 반드시 평가되는 값을 선언한다. `const`는 초기화 후 수정할 수 없다는 뜻일 뿐 런타임에 초기화될 수도 있으므로, `constexpr`이 `const`보다 더 엄격하다.

```cpp
const int maxSize = 100;            // const: 초기화 시 값 설정, 런타임일 수 있음
constexpr int maxSize = 100;        // constexpr: 반드시 컴파일 시점에 알려져야 함

const int x = someFunction();       // OK: const는 런타임 초기화 허용
// constexpr int y = someFunction(); // 오류 (someFunction이 constexpr이 아닌 한)

constexpr double pi = 3.14159265;   // OK: 리터럴 값은 컴파일 시점에 알려짐
```

| 키워드 | 컴파일 타임 보장 | 런타임 값으로 초기화 가능? | 도입 시점 |
|:------|:--------------|:---------------------|:---------|
| `const` | 아니오 (런타임일 수 있음) | 예 | C++98 |
| `constexpr` | **예** (반드시 컴파일 타임) | 아니오 (constexpr 식에서만) | C++11 |

*표 7. `const`와 `constexpr`의 비교*

값이 진정한 컴파일 타임 상수(배열 크기, 템플릿 인자 등)일 때는 `constexpr`을 사용한다. 단순히 수정을 방지하려면 `const`를 사용한다.

```cpp
constexpr int arraySize = 10;
int arr[arraySize];                  // OK: constexpr은 배열 크기로 사용 가능

const int n = getValue();
// int arr2[n];                      // 표준 C++에서 오류: n은 constexpr이 아님
```

#### 헤더에서 static 멤버 변수 초기화: inline 변수 (C++17)

전통적으로 static 멤버 변수는 정확히 하나의 `.cpp` 파일에서 클래스 외부에 정의(및 초기화)해야 한다. C++17에서는 **`inline` 변수**가 도입되어, 헤더 파일의 **클래스 내부에서 직접** static 멤버 변수를 정의하고 초기화할 수 있게 되었다. 다중 정의로 인한 링크 오류가 발생하지 않는다.

```cpp
// 전통적 방식 (C++17 이전): 별도의 .cpp 파일에 정의 필요
class Circle {
public:
    static int count;   // 선언만
};
int Circle::count = 0;  // 정확히 하나의 .cpp 파일에서 정의

// C++17 방식: inline static 변수 — 헤더에서 바로 정의
class Circle {
public:
    inline static int count = 0;   // 헤더에서 선언 + 정의
};
// 별도의 .cpp 정의 불필요!
```

이는 헤더 전용(header-only) 라이브러리를 간소화하고, 흔한 링크 오류의 원인을 제거한다.

### 3.2 static 멤버의 접근 방법

static 멤버에는 다음 세 가지 방법으로 접근한다.

| 방법 | 형식 | 예 |
|:----|:----|:----|
| 객체 이름 | `객체.static멤버` | `han.sharedMoney = 200;` |
| 객체 포인터 | `객체포인터->static멤버` | `p->addShared(200);` |
| 클래스 이름과 범위 지정 연산자 | `클래스명::static멤버` | `Person::addShared(50);` |

*표 8. static 멤버에 접근하는 세 가지 방법*

static 멤버는 클래스당 하나뿐이므로 클래스 이름으로 접근할 수 있고, 객체가 하나도 없을 때도 접근할 수 있다. 반면 non-static 멤버는 객체마다 따로 있으므로 `Person::money = 100;`이나 `Person::addMoney(200);`처럼 클래스 이름으로 접근하면 컴파일 오류가 난다.

```cpp
#include <iostream>
using namespace std;

class Person {
public:
    double money;                 // 개인 소유의 돈
    void addMoney(int money) {
        this->money += money;
    }

    static int sharedMoney;       // 공금
    static void addShared(int n) {
        sharedMoney += n;
    }
};

int Person::sharedMoney = 10;     // static 멤버 변수 정의, 10으로 초기화

int main() {
    Person::addShared(50);                    // 객체가 없어도 호출 가능, 공금 = 60
    cout << Person::sharedMoney << endl;      // 60

    Person han;
    han.money = 100;                          // han의 개인 돈 = 100
    han.sharedMoney = 200;                    // 객체 이름으로 접근, 공금 = 200

    Person lee;
    lee.money = 150;                          // lee의 개인 돈 = 150
    lee.addMoney(200);                        // lee의 개인 돈 = 350
    Person *p = &lee;
    p->addShared(200);                        // 객체 포인터로 접근, 공금 = 400

    cout << han.money << ' ' << lee.money << endl;              // 100 350
    cout << han.sharedMoney << ' ' << Person::sharedMoney << endl;  // 400 400
    // Person::money = 100;                   // 오류: non-static 멤버는 클래스 이름으로 접근 불가
    return 0;
}
```

```text
60
100 350
400 400
```

`han.sharedMoney`, `p->addShared()`, `Person::sharedMoney`는 모두 같은 변수 하나를 다루므로 `han`을 통해 바꾼 공금이 `lee`를 통해 더한 공금에 그대로 이어진다. 반면 `money`는 객체마다 따로 있어 `han`은 100, `lee`는 350이다.

> **참고:** 객체 이름으로 접근하는 방식은 보통 멤버처럼 보여 static 멤버라는 사실이 드러나지 않는다. 클래스 이름으로 접근하는 방식이 공유 멤버임을 분명히 보여 준다.

### 3.3 static 멤버 함수

**static 멤버 함수**는 특정 객체에 연결되지 않으므로 **`this` 포인터가 없다**. 따라서 객체를 지정하지 않고 이름만으로 접근할 수 있는 멤버는 **static 멤버**뿐이다.

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

static 멤버 함수가 접근할 수 있는 것은 static 멤버 변수, 다른 static 멤버 함수, 함수의 지역 변수와 매개변수이다. non-static 멤버에 접근할 수 없는 이유는 static 멤버 함수가 객체가 하나도 생성되지 않은 시점에도 호출될 수 있기 때문이다. 다음 코드에서 `main()`이 `PersonError::getMoney()`를 호출하는 시점에는 `money`를 가진 객체가 아직 없다.

```cpp
class PersonError {
    int money;
public:
    static int getMoney() { return money; }   // 컴파일 오류: non-static 멤버 접근
    void setMoney(int money) { this->money = money; }
};

int main() {
    int n = PersonError::getMoney();   // 이 시점에는 PersonError 객체가 없음
    PersonError errorKim;
    errorKim.setMoney(100);
}
```

같은 이유로 static 멤버 함수에서 `this->sharedMoney += n;`처럼 `this`를 쓰면 컴파일 오류이고, `sharedMoney += n;`으로 쓰면 정상이다. 반대로 non-static 멤버 함수는 `int total() { return money + sharedMoney; }`처럼 static 멤버와 non-static 멤버에 모두 접근할 수 있다.

> **참고:** static 멤버 함수도 매개변수로 받은 객체를 통해서는 `obj.data`처럼 그 객체의 non-static 멤버에 접근할 수 있다. 제약은 `this`가 없어 암시적으로 가리킬 객체가 없다는 데서 온다.

| 특성 | static 멤버 함수 | non-static 멤버 함수 |
|:----|:----------------|:-----------------|
| `this` 포인터 | 없음 | 있음 |
| static 멤버 접근 | 가능 | 가능 |
| non-static 멤버 접근 | **불가** | 가능 |
| 객체 없이 호출 | `ClassName::func()` | 불가능 |

*표 9. static 멤버 함수와 non-static 멤버 함수의 비교*

### 3.4 활용 사례

static의 주요 활용은 두 가지이다. 첫째, 전역 변수와 전역 함수를 클래스의 static 멤버로 캡슐화하여 전역 이름을 줄인다(3.3절의 `Math` 클래스). 둘째, 객체 사이에 공유하는 변수를 만든다(아래의 객체 수 관리).

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

같은 방식으로 생성자에서 개수를 늘리고 소멸자에서 줄이는 `Circle` 클래스에서 `Circle *p = new Circle[10];`으로 객체 배열을 만들면 생성자가 10번 실행되어 개수가 10이 되고, `delete [] p;`로 해제하면 소멸자가 10번 실행되어 0으로 돌아온다.

**static 배열을 활용한 게시판:**

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
| private 생성자 | `Board` 객체 생성을 막고, 모든 작업을 static 함수로 처리한다 |
| static 배열 + static 카운트 | 객체 없이 데이터가 존재하며, 클래스 이름으로만 관리한다 |
| 모든 함수가 static | `Board::add(...)`, `Board::print()`처럼 객체 없이 호출한다 |

*표 10. static 멤버만으로 만든 게시판의 설계 포인트*

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:----|:---------|
| 함수 중복 | 같은 이름의 함수를 매개변수의 개수, 타입, 순서를 달리하여 여러 개 정의하며, 리턴 타입만으로는 구분할 수 없다. |
| 함수 중복 해석 | 정확한 일치, 형 승격, 표준 형 변환 순으로 최적 일치를 찾고, 같은 순위에서 여러 함수가 일치하면 모호성 오류가 난다. |
| 참조 매개변수와 모호성 | `add(int, int)`와 `add(int, int&)`는 변수를 넘기는 호출에서 둘 다 정확한 일치가 되어 모호하다. |
| 생성자와 소멸자 | 생성자는 중복할 수 있지만, 매개변수가 없는 소멸자는 클래스에 하나뿐이어서 중복할 수 없다. |
| 디폴트 매개변수 | `void f(int a, int b = 10)`처럼 디폴트는 가장 오른쪽부터 연속으로 지정하고, 호출에서 중간 인자만 생략할 수 없다. |
| 디폴트는 선언에만 | 원형과 정의가 분리된 경우 디폴트 값은 한 번만, 호출하는 쪽이 보는 원형에 지정한다. |
| 디폴트와 함수 중복의 모호성 | `f(int)`와 `f(int, int = 10)`이 함께 있으면 `f(5)` 호출이 모호하므로 이 패턴을 피한다. |
| 디폴트로 함수 중복 간소화 | `Circle()`과 `Circle(int r)`은 `Circle(int r = 1)` 하나로 합칠 수 있지만, 합친 함수와 원래 함수를 함께 둘 수는 없다. |
| static 멤버 변수 | 클래스 내부에 `static`으로 선언하며, 클래스당 하나만 존재하여 모든 객체가 공유한다. |
| static 멤버 변수 정의 | `int ClassName::count = 0;`처럼 클래스 외부에서 정의해야 하며, 빠뜨리면 링크 오류가 난다. |
| static 멤버 접근 | 객체 이름, 객체 포인터, `클래스명::멤버`로 접근할 수 있고, non-static 멤버는 클래스 이름으로 접근할 수 없다. |
| static 멤버 함수 | `this` 포인터가 없어 이름만으로는 static 멤버에만 접근할 수 있고, `ClassName::func()`로 객체 없이 호출한다. |
| 객체 수 관리 | 생성자에서 static 카운트를 늘리고 소멸자에서 줄이면 살아 있는 객체 수를 셀 수 있다. |
| private 생성자와 static 멤버 | 객체 생성을 막고 모든 데이터와 함수를 클래스 이름으로 접근하게 하는 설계이다(Board 패턴). |
| `constexpr`과 `const` | `constexpr`(C++11)은 컴파일 타임 평가를 보장하고, `const`는 수정만 막는다. |
| `inline static` 변수 | C++17부터 `inline static int x = 0;`을 클래스 내부에 쓰면 별도의 `.cpp` 정의가 필요 없다. |

---

<br>

## 점검 문제

1. **함수 중복 조건:** `int sum(int a, int b)`와 `double sum(int a, int b)`를 함께 정의할 수 있는가? 이유를 쓰라.

   <details>
   <summary>정답</summary>

   정의할 수 없다. 두 함수는 이름과 매개변수 리스트가 같고 리턴 타입만 다르다. 리턴 타입은 중복된 함수를 구분하는 요소가 아니므로 `sum(2, 5)` 호출에서 컴파일러가 어느 함수를 부를지 정할 수 없고, 두 번째 정의는 컴파일 오류가 된다.

   </details>

2. **디폴트 값 채우기:** `void g(int a, int b = 0, int c = 0, int d = 0);`에 대해 `g(10, 5)`를 호출하면 네 매개변수의 값은 각각 무엇인가? 또 `void calc(int a, int b = 5, int c, int d = 0);`은 왜 오류인가?

   <details>
   <summary>정답</summary>

   인자는 왼쪽부터 채워지므로 `a = 10`, `b = 5`이고, 남은 `c`와 `d`는 디폴트 값 0이 된다. 즉 `g(10, 5, 0, 0)`과 같다. `calc`는 디폴트가 있는 `b` 뒤에 디폴트가 없는 `c`가 와서 디폴트 매개변수가 끝 쪽에 몰려 있지 않으므로 컴파일 오류이다.

   </details>

3. **형 변환과 모호성:** `float square(float a)`와 `double square(double a)`가 있을 때 `square(3.0)`과 `square(3)`은 각각 어떻게 처리되는가?

   <details>
   <summary>정답</summary>

   `3.0`은 `double`이므로 `square(double)`이 정확한 일치로 선택된다. `3`은 `int`이고, `int`에서 `float`으로 가는 변환과 `double`로 가는 변환은 모두 표준 형 변환으로 같은 순위이므로 호출이 모호하여 컴파일 오류가 난다.

   </details>

4. **static 멤버 값 추적:** 3.2절의 `Person` 예제에서 `main()`이 끝나기 직전 `Person::sharedMoney`의 값과, 그 값이 되는 과정을 쓰라.

   <details>
   <summary>정답</summary>

   400이다. 초기값 10에서 `Person::addShared(50)`으로 60이 되고, `han.sharedMoney = 200;`으로 200이 되며, `p->addShared(200)`으로 400이 된다. 세 접근 방식이 모두 같은 변수 하나를 다루므로 `han.sharedMoney`도 400이다.

   </details>

5. **static 멤버 함수의 제약:** static 멤버 함수에서 non-static 멤버 변수를 이름만으로 사용할 수 없는 이유를 `this`와 관련지어 설명하라.

   <details>
   <summary>정답</summary>

   static 멤버 함수는 특정 객체에 연결되지 않아 `this` 포인터가 없고, 객체가 하나도 생성되지 않은 시점에도 `ClassName::func()`로 호출될 수 있다. non-static 멤버 변수는 객체마다 따로 있으므로 어느 객체의 변수인지 정할 수 없고, 아직 존재하지 않을 수도 있다. 따라서 이름만으로 쓰면 컴파일 오류이고, 매개변수로 받은 객체를 통해 `obj.data`처럼 접근해야 한다.

   </details>
