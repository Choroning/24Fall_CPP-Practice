# Chapter 10 — 템플릿과 표준 라이브러리 (STL)

> **최종 수정일:** 2026-03-16
>
> **교재:** *명품 C++ Programming (개정판)* — 10장

---

## 목차

- [1. 함수 템플릿](#1-함수-템플릿)
  - [1.1 제네릭 프로그래밍](#11-제네릭-프로그래밍)
  - [1.2 문법](#12-문법)
  - [1.3 템플릿 구체화](#13-템플릿-구체화)
  - [1.4 다중 템플릿 매개변수](#14-다중-템플릿-매개변수)
  - [1.5 예제](#15-예제)
- [2. 클래스 템플릿](#2-클래스-템플릿)
  - [2.1 문법](#21-문법)
  - [2.2 구체화](#22-구체화)
  - [2.3 템플릿에서의 연산자 오버로딩](#23-템플릿에서의-연산자-오버로딩)
  - [2.4 디폴트 템플릿 매개변수](#24-디폴트-템플릿-매개변수)
- [3. STL 개요](#3-stl-개요)
  - [3.1 컨테이너](#31-컨테이너)
  - [3.2 반복자](#32-반복자)
  - [3.3 알고리즘](#33-알고리즘)
  - [3.4 vector 예제](#34-vector-예제)
- [4. 템플릿 컴파일 주의사항](#4-템플릿-컴파일-주의사항)
  - [4.1 템플릿 정의는 헤더에 있어야 한다](#41-템플릿-정의는-헤더에-있어야-한다)
  - [4.2 흔한 오류](#42-흔한-오류)
- [요약](#요약)

---

<br>

## 1. 함수 템플릿

### 1.1 제네릭 프로그래밍

**제네릭 프로그래밍(Generic Programming)**은 함수를 한 번만 작성하면 **어떤 타입에든** 동작하게 하는 것이다. 템플릿 없이는 동일한 로직을 각 타입(`int`, `double`, `char` 등)마다 중복 작성해야 한다.

```cpp
// 템플릿 없이 — 코드 중복
int maxInt(int a, int b) { return (a > b) ? a : b; }
double maxDouble(double a, double b) { return (a > b) ? a : b; }
char maxChar(char a, char b) { return (a > b) ? a : b; }
```

템플릿은 컴파일러가 타입별 함수를 자동으로 생성하게 하여 이러한 중복을 제거한다.

### 1.2 문법

함수 템플릿은 `template` 키워드와 꺾쇠괄호 안의 **템플릿 매개변수 목록**으로 선언한다.

```cpp
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}
```

- `template <typename T>` — 타입 매개변수 `T` 하나를 가진 템플릿 선언.
- `typename`과 `class`는 이 문맥에서 동일하다: `template <class T>`도 가능.
- `T`는 함수 호출 시 실제 타입으로 대체될 **자리 표시자(placeholder)**.

### 1.3 템플릿 구체화

템플릿 함수가 특정 타입으로 호출되면, 컴파일러가 해당 타입에 대한 **구체적인 함수**를 생성한다. 이 과정을 **구체화(instantiation)**라 한다.

**암시적 구체화** — 컴파일러가 인자 타입으로부터 `T`를 추론:

```cpp
int result1 = myMax(3, 7);        // T를 int로 추론 → myMax<int>(3, 7)
double result2 = myMax(3.5, 7.2); // T를 double로 추론 → myMax<double>(3.5, 7.2)
```

**명시적 구체화** — 프로그래머가 `T`를 직접 지정:

```cpp
double result3 = myMax<double>(3, 7.2);  // T를 명시적으로 double로 지정
```

> **핵심:** 템플릿에 사용된 각 타입마다 컴파일러가 별도의 구체 함수를 생성한다. `myMax<int>`와 `myMax<double>`은 컴파일된 바이너리에서 서로 다른 두 함수이다.

### 1.4 다중 템플릿 매개변수

템플릿은 하나 이상의 타입 매개변수를 가질 수 있다:

```cpp
template <typename T1, typename T2>
void printPair(T1 a, T2 b) {
    cout << a << ", " << b << endl;
}

printPair(10, 3.14);       // T1=int, T2=double
printPair("Hello", 42);    // T1=const char*, T2=int
```

### 1.5 예제

**제네릭 swap:**

```cpp
template <typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int x = 1, y = 2;
mySwap(x, y);       // x=2, y=1

string s1 = "hi", s2 = "bye";
mySwap(s1, s2);     // s1="bye", s2="hi"
```

**제네릭 배열 출력:**

```cpp
template <typename T>
void printArray(T arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << "  ";
    }
    cout << endl;
}

int nums[] = {1, 2, 3, 4};
printArray(nums, 4);        // 1  2  3  4

double vals[] = {1.1, 2.2, 3.3};
printArray(vals, 3);        // 1.1  2.2  3.3
```

**제네릭 배열 역순:**

```cpp
template <typename T>
void reverseArray(T arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        T temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

int data[] = {1, 10, 100, 1000};
reverseArray(data, 4);      // {1000, 100, 10, 1}
```

---

<br>

## 2. 클래스 템플릿

### 2.1 문법

클래스 템플릿은 하나 이상의 타입이 매개변수화된 클래스를 정의한다. 클래스 정의 앞에 `template <typename T>`를 붙인다.

```cpp
template <typename T>
class Container {
    T data;
    int size;
public:
    Container(T d, int s) : data(d), size(s) { }
    T getData();
    void setData(T d);
};
```

클래스 **외부**에서 멤버함수를 정의할 때는 템플릿 접두사를 반복하고 `ClassName<T>::`를 사용한다:

```cpp
template <typename T>
T Container<T>::getData() {
    return data;
}

template <typename T>
void Container<T>::setData(T d) {
    data = d;
}
```

### 2.2 구체화

클래스 템플릿이 특정 타입으로 사용될 때마다 컴파일러는 **별도의 클래스**를 생성한다.

```cpp
Container<int> c1(42, 1);       // int용 Container 클래스 생성
Container<string> c2("hello", 5); // string용 Container 클래스 생성
Container<double> c3(3.14, 1);  // double용 Container 클래스 생성
```

`Container<int>`와 `Container<string>`은 완전히 다른 클래스이다 — 런타임에 코드를 공유하지 않는다.

### 2.3 템플릿에서의 연산자 오버로딩

템플릿 클래스에서도 일반 클래스처럼 연산자를 오버로딩할 수 있다. 흔한 필요는 비교 연산자를 오버로딩하여 `bigger()` 같은 템플릿 함수가 사용자 정의 타입에서도 동작하게 하는 것이다.

```cpp
template <typename T>
class Rectangle {
    T width, height;
public:
    Rectangle(T w = 1, T h = 1) : width(w), height(h) { }
    T getArea() { return width * height; }
    bool operator>(const Rectangle<T>& other) const {
        return this->getArea() > other.getArea();
    }
};

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

Rectangle<double> r1(10.2, 23.5);
Rectangle<double> r2(20.4, 13.7);
Rectangle<double> r3 = bigger(r1, r2);  // operator>를 사용하여 비교
cout << r3.getArea() << endl;            // 279.48 (r2의 면적)
```

> **핵심:** 연산자(`>`, `==`, `<` 등)에 의존하는 템플릿 함수를 사용할 때, 타입 `T`는 해당 연산자가 정의되어 있어야 한다. 기본 타입은 자동으로 지원되지만, 사용자 정의 클래스는 직접 오버로딩해야 한다.

### 2.4 디폴트 템플릿 매개변수

함수 매개변수와 마찬가지로, 템플릿 매개변수에도 **기본값(default)**을 지정할 수 있다:

```cpp
template <typename T = int>
class Stack {
    T data[100];
    int top;
public:
    Stack() : top(-1) { }
    void push(T item) { data[++top] = item; }
    T pop() { return data[top--]; }
};

Stack<> s1;            // T는 int로 기본 설정
Stack<double> s2;      // T는 double
Stack<string> s3;      // T는 string
```

> **참고:** 기본값을 사용할 때도 꺾쇠괄호는 필요하다: `Stack<>`이지 `Stack`만 쓸 수 없다.

---

<br>

## 3. STL 개요

**표준 템플릿 라이브러리(Standard Template Library, STL)**는 C++ 표준 라이브러리의 핵심 부분이다. 제네릭하고 재사용 가능한 컴포넌트를 **컨테이너**, **반복자**, **알고리즘** 세 가지 범주로 제공한다.

### 3.1 컨테이너

컨테이너는 객체의 컬렉션을 저장하는 자료구조이다.

| 컨테이너 | 헤더 | 설명 |
|:---------|:------|:-----|
| `vector` | `<vector>` | 동적 배열; 연속 메모리; 빠른 임의 접근; 크기 변경 가능 |
| `list` | `<list>` | 이중 연결 리스트; 어디서든 빠른 삽입/삭제; 임의 접근 불가 |
| `map` | `<map>` | 키-값 쌍이 키 기준 정렬; 키로 빠른 검색 (`O(log n)`) |
| `set` | `<set>` | 정렬된 고유 원소; 빠른 소속 검사 (`O(log n)`) |

```cpp
#include <vector>
#include <map>

vector<int> nums = {1, 2, 3};
map<string, int> ages = {{"Alice", 20}, {"Bob", 22}};
```

### 3.2 반복자

**반복자(Iterator)**는 컨테이너를 순회하는 추상적인 포인터이다. 각 컨테이너는 자체 반복자 타입을 제공한다.

```cpp
vector<int> v = {10, 20, 30};

vector<int>::iterator it;
for (it = v.begin(); it != v.end(); it++) {
    cout << *it << " ";    // 포인터처럼 역참조
}
// 출력: 10 20 30
```

| 연산 | 의미 |
|:----|:-----|
| `begin()` | 첫 번째 원소를 가리키는 반복자 |
| `end()` | 마지막 원소 다음을 가리키는 반복자 |
| `*it` | 역참조 — 원소에 접근 |
| `it++` | 다음 원소로 이동 |

### 3.3 알고리즘

`<algorithm>` 헤더는 반복자와 함께 동작하는 제네릭 함수를 제공한다.

| 알고리즘 | 설명 | 예시 |
|:---------|:-----|:-----|
| `sort` | 범위 내 원소를 정렬 | `sort(v.begin(), v.end());` |
| `find` | 범위 내에서 원소를 검색 | `find(v.begin(), v.end(), 20);` |
| `for_each` | 각 원소에 함수를 적용 | `for_each(v.begin(), v.end(), print);` |

```cpp
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int main() {
    vector<int> v = {30, 10, 20};

    sort(v.begin(), v.end());    // {10, 20, 30}

    auto it = find(v.begin(), v.end(), 20);
    if (it != v.end()) {
        cout << "Found: " << *it << endl;  // Found: 20
    }

    return 0;
}
```

### 3.4 vector 예제

`vector`는 가장 많이 사용되는 STL 컨테이너이다. 자동으로 크기가 증가하는 동적 배열이다.

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;

    // 원소 추가
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // 크기
    cout << "Size: " << v.size() << endl;     // 3

    // 인덱스로 접근
    cout << "v[0] = " << v[0] << endl;        // 10
    cout << "v[1] = " << v[1] << endl;        // 20

    // 반복자 루프
    cout << "Iterator: ";
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;    // 10 20 30

    // 범위 기반 for문 (C++11)
    cout << "Range-based: ";
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;    // 10 20 30

    return 0;
}
```

| 연산 | 설명 |
|:----|:-----|
| `push_back(val)` | 끝에 원소 추가 |
| `size()` | 원소 개수 반환 |
| `v[i]` | 인덱스 `i`의 원소에 접근 |
| `begin()` / `end()` | 순회를 위한 반복자 |
| `pop_back()` | 마지막 원소 제거 |
| `clear()` | 모든 원소 제거 |

---

<br>

## 4. 템플릿 컴파일 주의사항

### 4.1 템플릿 정의는 헤더에 있어야 한다

일반 클래스와 달리, **템플릿 정의는 `.h`와 `.cpp` 파일로 분리할 수 없다**. 컴파일러가 구체 코드를 생성하려면 구체화 시점에 완전한 템플릿 정의를 볼 수 있어야 한다.

```cpp
// MyTemplate.h — 올바른 방법: 모든 것을 헤더에 작성
template <typename T>
class MyTemplate {
    T data;
public:
    MyTemplate(T d) : data(d) { }
    T getData();
};

template <typename T>
T MyTemplate<T>::getData() {
    return data;     // 정의도 헤더에 있어야 함
}
```

템플릿 멤버함수 정의를 `.cpp` 파일에 넣으면, 다른 번역 단위에서 템플릿을 구체화할 수 없어 링커가 **"undefined reference"** 오류를 발생시킨다.

```
// 잘못된 방법: MyTemplate.cpp — 링커 오류 발생
template <typename T>
T MyTemplate<T>::getData() {
    return data;
}
// MyTemplate<int>를 사용하는 다른 .cpp 파일에서 링크 실패!
```

> **원칙:** 템플릿 클래스 선언과 멤버함수 정의를 모두 **헤더 파일**에 두어야 한다.

### 4.2 흔한 오류

**사용자 정의 타입에 연산자 누락:**

```cpp
template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;   // 타입 T에 operator>가 필요
    else return b;
}

class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) { }
};

Point p1(1, 2), p2(3, 4);
// bigger(p1, p2);   // 컴파일 오류 — Point에 operator>가 없음
```

해결하려면 `Point` 클래스에 `operator>`를 오버로딩한다:

```cpp
class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) { }
    bool operator>(const Point& other) const {
        return (x + y) > (other.x + other.y);
    }
};

Point p1(1, 2), p2(3, 4);
Point p3 = bigger(p1, p2);  // 이제 동작 — operator> 사용
```

**타입 불일치:**

```cpp
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}

// myMax(3, 7.5);    // 컴파일 오류 — T가 int와 double 동시에 될 수 없음
myMax<double>(3, 7.5);  // OK — T를 명시적으로 double로 지정
```

> **팁:** 단일 타입 매개변수 템플릿 함수에 서로 다른 타입의 인자를 전달하면 컴파일러가 `T`를 추론할 수 없다. 명시적 구체화를 사용하거나 두 번째 템플릿 매개변수를 추가해야 한다.

---

<br>

## 요약

| 개념 | 핵심 요점 |
|:-----|:---------|
| 제네릭 프로그래밍 | 한 번 작성하면 모든 타입에 동작; 코드 중복 제거 |
| 함수 템플릿 문법 | 함수 앞에 `template <typename T>`; `T`는 타입 자리 표시자 |
| 템플릿 구체화 | 컴파일러가 타입별 구체 함수 생성; 암시적 또는 명시적 |
| 다중 매개변수 | `template <typename T1, typename T2>`로 서로 다른 타입 처리 |
| 클래스 템플릿 문법 | `template <typename T> class Name { };` 멤버 정의 시 `Name<T>::` 사용 |
| 클래스 구체화 | `Name<int> obj;` 각 타입마다 별도의 클래스 생성 |
| 템플릿 연산자 오버로딩 | 사용자 정의 타입은 템플릿 로직이 사용하는 연산자를 정의해야 함 (예: `operator>`) |
| 디폴트 템플릿 매개변수 | `template <typename T = int>` — 지정하지 않으면 기본 타입 사용 |
| STL 컨테이너 | `vector`, `list`, `map`, `set` — 제네릭 자료구조 |
| 반복자 | 컨테이너 순회를 위한 추상 포인터 (`begin()`, `end()`, `*it`) |
| 알고리즘 | `sort`, `find`, `for_each` — 반복자 범위에 대한 제네릭 함수 |
| `vector` | 동적 배열; `push_back`, `size`, `[]`, 반복자 및 범위 기반 for문 |
| 헤더에 정의 | 템플릿 전체 정의가 헤더에 있어야 함; `.cpp` 분리 시 링커 오류 |
| 흔한 오류 | 사용자 정의 타입의 연산자 누락; 단일 매개변수에 타입 불일치 |

---
