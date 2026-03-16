# Chapter 10 — 템플릿과 표준 라이브러리 (STL)

> **최종 수정일:** 2026-03-16

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
- [3. vector (심화)](#3-vector-심화)
  - [3.1 선언과 초기화](#31-선언과-초기화)
  - [3.2 원소 접근](#32-원소-접근)
  - [3.3 변경 연산](#33-변경-연산)
  - [3.4 크기와 용량](#34-크기와-용량)
  - [3.5 2차원 vector](#35-2차원-vector)
  - [3.6 순회 방법](#36-순회-방법)
  - [3.7 함수에 전달하기](#37-함수에-전달하기)
  - [3.8 완성 예제](#38-완성-예제)
- [4. string](#4-string)
  - [4.1 std::string vs C 스타일 char\[\]](#41-stdstring-vs-c-스타일-char)
  - [4.2 생성과 연결](#42-생성과-연결)
  - [4.3 접근과 검색](#43-접근과-검색)
  - [4.4 비교](#44-비교)
  - [4.5 변환](#45-변환)
  - [4.6 순회와 입력](#46-순회와-입력)
- [5. map과 set](#5-map과-set)
  - [5.1 map](#51-map)
  - [5.2 unordered_map](#52-unordered_map)
  - [5.3 set](#53-set)
  - [5.4 unordered_set](#54-unordered_set)
  - [5.5 실전 예제: 단어 빈도 세기](#55-실전-예제-단어-빈도-세기)
- [6. algorithm](#6-algorithm)
  - [6.1 정렬](#61-정렬)
  - [6.2 검색](#62-검색)
  - [6.3 개수 세기와 누적](#63-개수-세기와-누적)
  - [6.4 시퀀스 변경](#64-시퀀스-변경)
  - [6.5 이진 탐색](#65-이진-탐색)
  - [6.6 완성 예제: 점수 기준 학생 정렬](#66-완성-예제-점수-기준-학생-정렬)
- [7. 모던 C++ 기능](#7-모던-c-기능)
  - [7.1 auto (C++11)](#71-auto-c11)
  - [7.2 범위 기반 for (C++11)](#72-범위-기반-for-c11)
  - [7.3 람다 표현식 (C++11)](#73-람다-표현식-c11)
  - [7.4 구조적 바인딩 (C++17)](#74-구조적-바인딩-c17)
  - [7.5 std::optional (C++17)](#75-stdoptional-c17)
  - [7.6 Concepts (C++20)](#76-concepts-c20)
  - [7.7 std::ranges (C++20)](#77-stdranges-c20)
  - [7.8 std::print (C++23)](#78-stdprint-c23)
  - [7.9 C++26 미리보기](#79-c26-미리보기)
- [8. 코딩 테스트를 위한 실전 패턴](#8-코딩-테스트를-위한-실전-패턴)
  - [8.1 EOF까지 입력 읽기](#81-eof까지-입력-읽기)
  - [8.2 사용자 정의 객체 정렬](#82-사용자-정의-객체-정렬)
  - [8.3 map으로 빈도 세기](#83-map으로-빈도-세기)
  - [8.4 set으로 중복 제거](#84-set으로-중복-제거)
  - [8.5 스택과 큐](#85-스택과-큐)
  - [8.6 priority_queue로 Top-K 문제](#86-priority_queue로-top-k-문제)
  - [8.7 pair와 make_pair](#87-pair와-make_pair)
- [9. 템플릿 컴파일 주의사항](#9-템플릿-컴파일-주의사항)
  - [9.1 템플릿 정의는 헤더에 있어야 한다](#91-템플릿-정의는-헤더에-있어야-한다)
  - [9.2 흔한 오류](#92-흔한-오류)
- [10. 흔한 실수 모음](#10-흔한-실수-모음)
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

## 3. vector (심화)

`vector`는 초보자에게 **가장 중요한 STL 컨테이너**이며, 실무 C++ 프로그래밍과 코딩 테스트에서 가장 자주 사용되는 컨테이너이다. 원소가 추가될 때 자동으로 메모리를 관리하고 크기가 늘어나는 동적 배열이다.

**헤더:** `#include <vector>`

### 3.1 선언과 초기화

```cpp
#include <vector>
using namespace std;

vector<int> v1;                    // 빈 벡터
vector<int> v2(5);                 // 원소 5개, 모두 0으로 초기화
vector<int> v3(5, 42);            // 원소 5개, 모두 42로 초기화
vector<int> v4 = {1, 2, 3, 4, 5}; // 초기화 리스트 (C++11)
vector<int> v5{10, 20, 30};       // 중괄호 초기화 (C++11)
vector<int> v6(v4);               // v4의 복사본
vector<int> v7(v4.begin(), v4.begin() + 3); // {1, 2, 3} — 반복자 범위로 생성
```

### 3.2 원소 접근

```cpp
vector<int> v = {10, 20, 30, 40, 50};

v[0];           // 10 — 범위 검사 없음 (범위 밖이면 미정의 동작)
v.at(1);        // 20 — 범위 검사 있음 (잘못된 인덱스면 std::out_of_range 예외)
v.front();      // 10 — 첫 번째 원소
v.back();       // 50 — 마지막 원소
```

| 메서드 | 반환값 | 범위 검사 |
|:-------|:-------|:----------|
| `v[i]` | 인덱스 `i`의 원소 | 없음 (범위 밖이면 미정의 동작) |
| `v.at(i)` | 인덱스 `i`의 원소 | 있음 (`std::out_of_range` 예외 발생) |
| `v.front()` | 첫 번째 원소 | 없음 |
| `v.back()` | 마지막 원소 | 없음 |

> **팁:** 디버깅 시에는 `at()`을 사용하고, 인덱스가 유효함을 확인한 후에는 성능이 중요한 코드에서 `[]`를 사용하라.

### 3.3 변경 연산

```cpp
vector<int> v = {10, 20, 30};

v.push_back(40);                 // {10, 20, 30, 40} — 끝에 추가
v.pop_back();                    // {10, 20, 30} — 끝에서 제거

v.insert(v.begin() + 1, 15);    // {10, 15, 20, 30} — 위치 앞에 삽입
v.erase(v.begin() + 2);         // {10, 15, 30} — 단일 원소 삭제
v.erase(v.begin(), v.begin()+2);// {30} — 범위 [first, last) 삭제

v.clear();                       // {} — 모든 원소 제거

v.resize(5);                     // {0, 0, 0, 0, 0} — 크기를 5로 변경 (0으로 채움)
v.resize(3, 99);                 // {0, 0, 0} — 크기를 3으로 축소 (99는 미사용)
v.resize(5, 99);                 // {0, 0, 0, 99, 99} — 크기를 5로 확장, 새 원소 = 99
```

| 메서드 | 효과 | 시간 복잡도 |
|:-------|:-----|:-----------|
| `push_back(val)` | 끝에 추가 | 분할 상환 O(1) |
| `pop_back()` | 마지막 원소 제거 | O(1) |
| `insert(pos, val)` | `pos` 앞에 삽입 | O(n) |
| `erase(pos)` | `pos` 위치의 원소 제거 | O(n) |
| `erase(first, last)` | `[first, last)` 범위 제거 | O(n) |
| `clear()` | 모든 원소 제거 | O(n) |
| `resize(n)` | 크기를 `n`으로 변경 | O(n) |

### 3.4 크기와 용량

`vector`는 두 가지 별도의 개념이 있다: **size**(현재 저장된 원소 수)와 **capacity**(할당된 메모리 슬롯 수).

```cpp
vector<int> v;
v.reserve(100);     // 100개 원소 공간을 미리 할당 (원소는 생성되지 않음)

cout << v.size();       // 0 — 아직 원소 없음
cout << v.capacity();   // >= 100 — 할당된 공간
cout << v.empty();      // true (1)

v.push_back(1);
v.push_back(2);
cout << v.size();       // 2
cout << v.capacity();   // >= 100 (여전히 미리 할당된 상태)

v.shrink_to_fit();      // capacity를 size에 맞추도록 요청
```

| 메서드 | 반환값 / 효과 |
|:-------|:-------------|
| `size()` | 원소 개수 |
| `capacity()` | 할당된 저장 공간 (>= size) |
| `empty()` | size가 0이면 `true` |
| `reserve(n)` | 최소 `n`개 원소를 위한 공간 미리 할당 |
| `shrink_to_fit()` | capacity를 size에 맞추도록 요청 |

> **성능 팁:** 원소 수를 미리 알고 있다면 루프 전에 `reserve()`를 호출하라. 반복적인 재할당을 피할 수 있다 (재할당 시 기존 원소를 모두 복사해야 한다).

### 3.5 2차원 vector

2차원 벡터는 `vector`의 `vector`이다.

```cpp
// 0으로 초기화된 3x4 행렬 선언
vector<vector<int>> matrix(3, vector<int>(4, 0));

matrix[0][0] = 1;
matrix[1][2] = 5;

// 2차원 벡터 순회
for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
        cout << matrix[i][j] << " ";
    }
    cout << endl;
}

// 동적 행 추가
vector<vector<int>> jagged;
jagged.push_back({1, 2, 3});
jagged.push_back({4, 5});         // 행마다 길이가 다를 수 있음
jagged.push_back({6, 7, 8, 9});
```

### 3.6 순회 방법

벡터를 순회하는 네 가지 일반적인 방법이 있다:

```cpp
vector<int> v = {10, 20, 30, 40};

// 1. 인덱스 기반 루프
for (int i = 0; i < v.size(); i++) {
    cout << v[i] << " ";
}

// 2. 반복자 루프
for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
    cout << *it << " ";
}

// 3. 범위 기반 for (C++11) — 각 원소를 복사
for (int x : v) {
    cout << x << " ";
}

// 4. 참조를 사용한 범위 기반 for (C++11) — 복사 없음, 수정 가능
for (int& x : v) {
    x *= 2;   // 원본 벡터가 수정됨
}
// v는 이제 {20, 40, 60, 80}

// 5. const 참조를 사용한 범위 기반 for — 복사 없음, 읽기 전용
for (const int& x : v) {
    cout << x << " ";
}
```

> **모범 사례:** 읽기 전용 접근에는 `for (const auto& x : v)`를, 원소 수정이 필요할 때는 `for (auto& x : v)`를 사용하라.

### 3.7 함수에 전달하기

비용이 큰 복사를 피하려면 항상 벡터를 **참조**로 전달해야 한다. 함수가 벡터를 읽기만 한다면 `const`를 사용한다.

```cpp
// 읽기 전용 — const 참조로 전달
void printVector(const vector<int>& v) {
    for (const auto& x : v) {
        cout << x << " ";
    }
    cout << endl;
}

// 제자리 수정 — 참조로 전달
void doubleAll(vector<int>& v) {
    for (auto& x : v) {
        x *= 2;
    }
}

// 새 벡터 반환 — 값으로 반환 (이동 시맨틱스로 효율적)
vector<int> createVector(int n) {
    vector<int> result(n);
    for (int i = 0; i < n; i++) result[i] = i + 1;
    return result;   // 복사가 아닌 이동 (C++11)
}
```

### 3.8 완성 예제

**N개의 수를 입력받아 정렬하고 출력:**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cout << "Enter count: ";
    cin >> n;

    vector<int> nums(n);
    cout << "Enter " << n << " numbers: ";
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    sort(nums.begin(), nums.end());

    cout << "Sorted: ";
    for (const auto& x : nums) {
        cout << x << " ";
    }
    cout << endl;

    cout << "Min: " << nums.front() << endl;
    cout << "Max: " << nums.back() << endl;

    return 0;
}
```

```
Enter count: 5
Enter 5 numbers: 30 10 50 20 40
Sorted: 10 20 30 40 50
Min: 10
Max: 50
```

---

<br>

## 4. string

`std::string`은 C++ 표준 라이브러리의 일부이며 문자의 STL 컨테이너처럼 동작한다. 자체 메모리를 관리하고 풍부한 연산 기능을 제공한다.

**헤더:** `#include <string>`

### 4.1 std::string vs C 스타일 char[]

| 특성 | `std::string` | `char[]` / `char*` |
|:----|:--------------|:--------------------|
| 메모리 관리 | 자동 (동적) | 수동 |
| 크기 추적 | `s.size()` / `s.length()` | `strlen(s)` — 호출마다 O(n) |
| 연결 | `s1 + s2` 또는 `s1 += s2` | `strcat()` — 버퍼 관리 필요 |
| 비교 | `==`, `<`, `>` | `strcmp()` |
| 안전성 | `at()`으로 범위 검사 | 버퍼 오버플로우 위험 |
| 대입 | `s1 = s2` | `strcpy()` 사용 필요 |
| 함수 전달 | 참조로 전달 | 포인터로 전달 |

> **권장:** C++ 코드에서는 항상 C 스타일 문자열보다 `std::string`을 사용하라.

### 4.2 생성과 연결

```cpp
#include <string>
using namespace std;

string s1;                     // 빈 문자열 ""
string s2 = "Hello";          // C 문자열 리터럴에서 생성
string s3("World");           // 생성자 형태
string s4(5, 'x');            // "xxxxx" — 'x'를 5번 반복
string s5 = s2;               // s2의 복사본

// 연결
string greeting = s2 + " " + s3;   // "Hello World"
s2 += "!";                         // "Hello!" — 제자리에서 추가
s2.append(" C++");                 // "Hello! C++" — 추가하는 다른 방법
```

### 4.3 접근과 검색

```cpp
string s = "Hello World";

// 개별 문자 접근
s[0];              // 'H'
s.at(4);           // 'o' — 범위 검사 있음

// 부분 문자열
s.substr(0, 5);    // "Hello" — substr(시작_위치, 길이)
s.substr(6);       // "World" — 6번 위치부터 끝까지

// 검색
s.find("World");       // 6 — 첫 번째 발견 위치
s.find("xyz");         // string::npos — 찾지 못함
s.find('o');           // 4 — 첫 번째 'o'
s.find('o', 5);        // 7 — 5번 위치부터 첫 번째 'o'
s.rfind('o');          // 7 — 마지막 발견 위치 (역방향 검색)
```

| 메서드 | 반환값 |
|:-------|:-------|
| `find(str)` | 첫 번째 발견 위치, 없으면 `string::npos` |
| `find(str, pos)` | `pos`부터 검색 시작 |
| `rfind(str)` | 마지막 발견 위치 |
| `substr(pos, len)` | `pos`에서 길이 `len`인 부분 문자열 |
| `substr(pos)` | `pos`부터 끝까지 부분 문자열 |

### 4.4 비교

```cpp
string a = "apple";
string b = "banana";

a == b;     // false
a != b;     // true
a < b;      // true — 사전순 비교 ('a' < 'b')
a > b;      // false

// compare()는 같으면 0, a < b이면 음수, a > b이면 양수 반환
a.compare(b);    // 음수 값
```

### 4.5 변환

```cpp
#include <string>
using namespace std;

// 숫자 -> 문자열
string s1 = to_string(42);       // "42"
string s2 = to_string(3.14);     // "3.140000"

// 문자열 -> 숫자
int n = stoi("123");              // 123
double d = stod("3.14");          // 3.14
long l = stol("1000000");         // 1000000
long long ll = stoll("999999999999"); // 999999999999

// 변환 실패 시 std::invalid_argument 예외 발생
// stoi("abc");   // 예외 발생
```

| 함수 | 변환 |
|:----|:-----|
| `to_string(num)` | 숫자 -> 문자열 |
| `stoi(str)` | 문자열 -> `int` |
| `stol(str)` | 문자열 -> `long` |
| `stoll(str)` | 문자열 -> `long long` |
| `stod(str)` | 문자열 -> `double` |
| `stof(str)` | 문자열 -> `float` |

### 4.6 순회와 입력

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    // 범위 기반 for
    string s = "Hello";
    for (char c : s) {
        cout << c << " ";   // H e l l o
    }
    cout << endl;

    // 문자 수정
    for (char& c : s) {
        c = toupper(c);
    }
    // s는 이제 "HELLO"

    // 입력: cin >>은 단어 하나만 읽음 (공백에서 멈춤)
    string word;
    cin >> word;          // 입력: "Hello World" -> word = "Hello"

    // 입력: getline은 전체 줄을 읽음 (공백 포함)
    string line;
    getline(cin, line);   // 입력: "Hello World" -> line = "Hello World"

    // 주의: cin >>과 getline을 혼용할 때
    int n;
    cin >> n;             // 숫자를 읽고, '\n'이 버퍼에 남음
    cin.ignore();         // 남은 '\n'을 버림
    getline(cin, line);   // 이제 다음 줄 전체를 올바르게 읽음

    return 0;
}
```

> **흔한 함정:** `cin >> variable` 후에 개행 문자가 입력 버퍼에 남는다. 바로 `getline()`을 호출하면 빈 줄을 읽는다. `cin >>`과 `getline()` 사이에 항상 `cin.ignore()`를 호출하라.

---

<br>

## 5. map과 set

### 5.1 map

`map<K, V>`는 키 기준으로 정렬된 **키-값 쌍**을 저장한다. 내부적으로 균형 이진 탐색 트리(레드-블랙 트리)를 사용하여 O(log n) 연산을 제공한다.

**헤더:** `#include <map>`

```cpp
#include <map>
#include <string>
#include <iostream>
using namespace std;

int main() {
    map<string, int> ages;

    // 삽입
    ages["Alice"] = 20;               // [] 연산자 — 삽입 또는 갱신
    ages["Bob"] = 22;
    ages.insert({"Charlie", 25});      // insert() — 키가 이미 있으면 덮어쓰지 않음
    ages.insert(make_pair("Dave", 30));

    // 접근
    cout << ages["Alice"] << endl;     // 20

    // 존재 확인 (중요: []로 확인하면 안 됨 — 기본값 항목이 생성됨!)
    if (ages.find("Eve") != ages.end()) {
        cout << "Found Eve" << endl;
    } else {
        cout << "Eve not found" << endl;   // 이 분기가 실행됨
    }

    if (ages.count("Bob") > 0) {       // count는 map에서 0 또는 1 반환
        cout << "Bob exists" << endl;
    }

    // 삭제
    ages.erase("Charlie");             // 키로 삭제

    // 순회 — 원소가 키 기준으로 정렬됨
    for (auto it = ages.begin(); it != ages.end(); it++) {
        cout << it->first << ": " << it->second << endl;
    }
    // Alice: 20
    // Bob: 22
    // Dave: 30

    // 크기
    cout << "Size: " << ages.size() << endl;   // 3

    return 0;
}
```

| 메서드 | 설명 | 시간 |
|:-------|:-----|:-----|
| `m[key] = val` | 삽입 또는 갱신 | O(log n) |
| `m[key]` | 값 접근 (키가 없으면 기본값 생성!) | O(log n) |
| `m.at(key)` | 값 접근 (키가 없으면 예외 발생) | O(log n) |
| `m.insert({k, v})` | 키가 없을 때만 삽입 | O(log n) |
| `m.find(key)` | 반복자 반환 (없으면 `end()`) | O(log n) |
| `m.count(key)` | 0 또는 1 반환 | O(log n) |
| `m.erase(key)` | 키로 삭제 | O(log n) |
| `m.size()` | 키-값 쌍의 수 | O(1) |

### 5.2 unordered_map

`unordered_map<K, V>`는 트리 대신 **해시 테이블**을 사용한다. **평균 O(1)** 탐색을 제공하지만 원소가 **정렬되지 않는다**.

**헤더:** `#include <unordered_map>`

```cpp
#include <unordered_map>
#include <string>
using namespace std;

unordered_map<string, int> freq;
freq["apple"] = 3;
freq["banana"] = 5;

// map과 동일한 인터페이스: [], find, count, erase, insert, size
```

**언제 어떤 것을 사용할까:**

| | `map` | `unordered_map` |
|:--|:------|:----------------|
| 순서 | 키 기준 정렬 | 순서 없음 |
| 탐색 | O(log n) | 평균 O(1), 최악 O(n) |
| 사용 시점 | 정렬된 순서나 범위 쿼리가 필요할 때 | 가장 빠른 탐색이 필요할 때 |
| 키 요구사항 | `operator<`가 정의되어야 함 | `std::hash`와 `operator==`가 정의되어야 함 |

### 5.3 set

`set<T>`는 **고유한 원소**를 정렬된 순서로 저장한다. 내부적으로 균형 BST를 사용한다.

**헤더:** `#include <set>`

```cpp
#include <set>
#include <iostream>
using namespace std;

int main() {
    set<int> s;

    s.insert(30);
    s.insert(10);
    s.insert(20);
    s.insert(10);     // 중복 — 무시됨

    cout << s.size() << endl;   // 3

    // 소속 확인
    if (s.find(20) != s.end()) {
        cout << "20 exists" << endl;
    }
    if (s.count(99) == 0) {
        cout << "99 not found" << endl;
    }

    // 삭제
    s.erase(10);       // 원소 10 제거

    // 순회 — 원소가 정렬됨
    for (const auto& x : s) {
        cout << x << " ";    // 20 30
    }
    cout << endl;

    return 0;
}
```

| 메서드 | 설명 | 시간 |
|:-------|:-----|:-----|
| `s.insert(val)` | 원소 삽입 (중복이면 무시) | O(log n) |
| `s.find(val)` | 반복자 반환 (없으면 `end()`) | O(log n) |
| `s.count(val)` | 0 또는 1 반환 | O(log n) |
| `s.erase(val)` | 원소 제거 | O(log n) |
| `s.size()` | 원소 개수 | O(1) |

### 5.4 unordered_set

`unordered_set<T>`는 `set`의 해시 기반 버전이다. 평균 O(1) 연산, 순서 없음.

**헤더:** `#include <unordered_set>`

```cpp
#include <unordered_set>
using namespace std;

unordered_set<int> us = {1, 2, 3, 4, 5};
us.insert(3);              // 중복 — 무시됨
us.count(3);               // 1
us.erase(2);               // 원소 2 제거
```

### 5.5 실전 예제: 단어 빈도 세기

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> freq;
    string word;

    cout << "Enter words (Ctrl+D to stop):" << endl;
    while (cin >> word) {
        freq[word]++;      // []는 키가 없으면 값 0인 항목을 만든 후 증가시킴
    }

    cout << "\nWord frequencies:" << endl;
    for (const auto& pair : freq) {
        cout << pair.first << ": " << pair.second << endl;
    }

    return 0;
}
```

```
Enter words (Ctrl+D to stop):
the cat sat on the mat the cat
^D

Word frequencies:
cat: 2
mat: 1
on: 1
sat: 1
the: 3
```

---

<br>

## 6. algorithm

`<algorithm>` 헤더는 반복자 범위에 대해 동작하는 제네릭 함수를 제공한다. 코딩 테스트와 일상적인 C++ 프로그래밍에 필수적이다.

**헤더:** `#include <algorithm>`

### 6.1 정렬

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {30, 10, 50, 20, 40};

// 기본: 오름차순
sort(v.begin(), v.end());          // {10, 20, 30, 40, 50}

// 내림차순 — 비교자 사용
sort(v.begin(), v.end(), greater<int>());  // {50, 40, 30, 20, 10}

// 람다로 커스텀 비교자
sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;                  // 내림차순
});

// 벡터의 일부만 정렬
sort(v.begin(), v.begin() + 3);   // 처음 3개 원소만 정렬
```

> **참고:** `sort`는 IntroSort(퀵소트, 힙소트, 삽입 정렬의 하이브리드)를 사용하며 O(n log n)의 보장된 시간 복잡도를 가진다.

### 6.2 검색

```cpp
vector<int> v = {10, 20, 30, 40, 50};

// find — 첫 번째 일치하는 원소의 반복자 반환, 없으면 end()
auto it = find(v.begin(), v.end(), 30);
if (it != v.end()) {
    cout << "Found at index " << (it - v.begin()) << endl;  // 인덱스 2
}

// min_element / max_element — 최솟값/최댓값의 반복자 반환
auto minIt = min_element(v.begin(), v.end());
auto maxIt = max_element(v.begin(), v.end());
cout << "Min: " << *minIt << endl;   // 10
cout << "Max: " << *maxIt << endl;   // 50
```

### 6.3 개수 세기와 누적

```cpp
#include <algorithm>
#include <numeric>      // accumulate를 위해
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 2, 4, 2, 5};

// count — 값의 출현 횟수
int c = count(v.begin(), v.end(), 2);   // 3

// accumulate — 모든 원소의 합 (<numeric>에서)
int sum = accumulate(v.begin(), v.end(), 0);   // 19
// 세 번째 인자(0)가 초기값

// accumulate — 커스텀 연산으로
int product = accumulate(v.begin(), v.end(), 1, [](int a, int b) {
    return a * b;
});
// 1 * 1 * 2 * 3 * 2 * 4 * 2 * 5 = 480
```

### 6.4 시퀀스 변경

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// reverse
reverse(v.begin(), v.end());       // {5, 4, 3, 2, 1}

// unique + erase 관용구 — 연속 중복 제거
vector<int> w = {1, 1, 2, 3, 3, 3, 4, 4, 5};
// 중요: 모든 중복을 제거하려면 먼저 정렬되어 있어야 함
auto last = unique(w.begin(), w.end());   // 중복을 끝으로 이동
w.erase(last, w.end());                   // 실제로 제거
// w = {1, 2, 3, 4, 5}

// fill
vector<int> u(5);
fill(u.begin(), u.end(), 42);     // {42, 42, 42, 42, 42}
```

### 6.5 이진 탐색

이 함수들은 범위가 **정렬되어 있어야** 한다.

```cpp
vector<int> v = {10, 20, 30, 40, 50};

// binary_search — true/false 반환
bool found = binary_search(v.begin(), v.end(), 30);   // true

// lower_bound — 값 이상인 첫 번째 원소의 반복자
auto lb = lower_bound(v.begin(), v.end(), 25);
cout << *lb << endl;    // 30 (25 이상인 첫 번째 원소)

// upper_bound — 값 초과인 첫 번째 원소의 반복자
auto ub = upper_bound(v.begin(), v.end(), 30);
cout << *ub << endl;    // 40 (30 초과인 첫 번째 원소)

// 위치/인덱스 구하기
int idx = lower_bound(v.begin(), v.end(), 30) - v.begin();   // 2
```

| 함수 | 반환값 | 정렬 필요 |
|:----|:-------|:---------|
| `binary_search(b, e, val)` | `true` / `false` | 예 |
| `lower_bound(b, e, val)` | val 이상인 첫 번째 반복자 | 예 |
| `upper_bound(b, e, val)` | val 초과인 첫 번째 반복자 | 예 |

### 6.6 완성 예제: 점수 기준 학생 정렬

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Student {
    string name;
    int score;
};

int main() {
    vector<Student> students = {
        {"Alice", 85},
        {"Bob", 92},
        {"Charlie", 78},
        {"Dave", 92},
        {"Eve", 88}
    };

    // 점수 내림차순 정렬; 동점이면 이름 오름차순
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) return a.score > b.score;   // 높은 점수 우선
        return a.name < b.name;                              // 동점이면 알파벳순
    });

    for (const auto& s : students) {
        cout << s.name << ": " << s.score << endl;
    }
    // Bob: 92
    // Dave: 92
    // Eve: 88
    // Alice: 85
    // Charlie: 78

    return 0;
}
```

---

<br>

## 7. 모던 C++ 기능

### 7.1 auto (C++11)

`auto` 키워드는 초기화자로부터 컴파일러가 **타입을 추론**하게 한다. 반복자 등 복잡한 타입에 특히 유용하다.

```cpp
auto x = 42;               // int
auto y = 3.14;             // double
auto s = string("hello");  // std::string

// auto 없이 — 장황함
vector<int>::iterator it1 = v.begin();

// auto 사용 — 간결함
auto it2 = v.begin();      // 같은 타입, 자동으로 추론됨

// map과 함께 — 훨씬 깔끔함
map<string, vector<int>> data;
// auto 없이:
for (map<string, vector<int>>::iterator it = data.begin(); it != data.end(); it++) { }
// auto 사용:
for (auto it = data.begin(); it != data.end(); it++) { }
```

> **가이드라인:** 타입이 문맥에서 명확하거나 지나치게 길 때 `auto`를 사용하라. 타입이 불분명할 때는 피하라 (예: `auto result = computeSomething();` — `result`의 타입이 무엇인가?).

### 7.2 범위 기반 for (C++11)

컨테이너를 순회하는 더 깔끔한 문법:

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 값으로 (각 원소를 복사)
for (auto x : v) {
    cout << x << " ";
}

// 참조로 (복사 없음, 수정 가능)
for (auto& x : v) {
    x *= 2;
}

// const 참조로 (복사 없음, 읽기 전용 — 읽기 접근에 권장)
for (const auto& x : v) {
    cout << x << " ";
}
```

`begin()`과 `end()`를 제공하는 모든 컨테이너에서 동작한다: `string`, `map`, `set`, 배열 등.

### 7.3 람다 표현식 (C++11)

람다는 인라인으로 정의되는 **익명 함수**이다. 문법: `[캡처](매개변수) { 본문 }`

```cpp
// 기본 람다
auto greet = [](const string& name) {
    cout << "Hello, " << name << "!" << endl;
};
greet("World");   // Hello, World!

// 반환 타입이 있는 람다
auto add = [](int a, int b) -> int {
    return a + b;
};

// 주변 범위의 변수 캡처
int factor = 3;
auto multiply = [factor](int x) { return x * factor; };  // 값으로 캡처
cout << multiply(10) << endl;   // 30

int total = 0;
auto accumulate_fn = [&total](int x) { total += x; };    // 참조로 캡처
accumulate_fn(5);
accumulate_fn(10);
cout << total << endl;   // 15

// 일반적 용법: 커스텀 정렬 비교자
vector<int> v = {3, 1, 4, 1, 5};
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });  // 내림차순

// 일반적 용법: for_each
for_each(v.begin(), v.end(), [](int x) { cout << x << " "; });
```

**캡처 모드:**

| 캡처 | 의미 |
|:-----|:-----|
| `[]` | 아무것도 캡처하지 않음 |
| `[x]` | `x`를 값으로 캡처 (복사) |
| `[&x]` | `x`를 참조로 캡처 |
| `[=]` | 사용된 모든 변수를 값으로 캡처 |
| `[&]` | 사용된 모든 변수를 참조로 캡처 |
| `[=, &x]` | 모두 값으로, `x`만 참조로 |

### 7.4 구조적 바인딩 (C++17)

> **[C++17]** 이 기능은 C++17 이상에서 사용 가능합니다.

구조적 바인딩을 사용하면 pair, tuple, 구조체를 이름이 있는 변수로 분해할 수 있다.

```cpp
// map에서 — 깔끔한 순회
map<string, int> ages = {{"Alice", 20}, {"Bob", 22}};

for (const auto& [name, age] : ages) {
    cout << name << " is " << age << endl;
}
// 이전 방식:
for (const auto& p : ages) {
    cout << p.first << " is " << p.second << endl;
}

// pair에서
pair<int, string> p = {42, "answer"};
auto [num, text] = p;
cout << num << ": " << text << endl;   // 42: answer

// 구조체에서
struct Point { int x, y; };
Point pt = {3, 4};
auto [px, py] = pt;
```

### 7.5 std::optional (C++17)

> **[C++17]** 이 기능은 C++17 이상에서 사용 가능합니다.

`std::optional<T>`는 값이 있을 수도 없을 수도 있음을 나타낸다. `-1`이나 `nullptr` 같은 특수 센티넬 값 대신 사용하는 안전한 대안이다.

```cpp
#include <optional>
#include <string>
using namespace std;

optional<int> findIndex(const vector<int>& v, int target) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == target) return i;     // 값을 반환
    }
    return nullopt;                        // "값 없음" 반환
}

int main() {
    vector<int> v = {10, 20, 30};

    auto result = findIndex(v, 20);
    if (result.has_value()) {
        cout << "Found at index " << result.value() << endl;  // 1
    }

    // value_or로 간결하게
    int idx = findIndex(v, 99).value_or(-1);   // -1 (찾지 못함)

    return 0;
}
```

### 7.6 Concepts (C++20)

> **[C++20]** 이 기능은 C++20 이상에서 사용 가능합니다.

Concepts는 템플릿 매개변수를 제약하여 타입이 요구사항을 충족하지 않을 때 명확한 오류 메시지를 제공한다.

```cpp
#include <concepts>

template <typename T>
requires std::integral<T>
T gcd(T a, T b) {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 더 짧은 문법:
template <std::integral T>
T gcd2(T a, T b) { /* ... */ }

// auto를 사용한 더 짧은 형태:
auto gcd3(std::integral auto a, std::integral auto b) { /* ... */ }

gcd(12, 8);     // OK — int는 integral
// gcd(3.14, 2.0); // 컴파일 오류 — double은 integral이 아님 (명확한 메시지!)
```

### 7.7 std::ranges (C++20)

> **[C++20]** 이 기능은 C++20 이상에서 사용 가능합니다.

Ranges 라이브러리는 시퀀스를 다루는 더 깔끔한 방법을 제공하며, `begin()`과 `end()`를 따로 전달할 필요가 없다.

```cpp
#include <ranges>
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {5, 3, 1, 4, 2};

// 기존 방식: sort(v.begin(), v.end());
ranges::sort(v);                    // {1, 2, 3, 4, 5}

// Views — 지연 평가, 조합 가능한 변환
auto even = v | views::filter([](int x) { return x % 2 == 0; });
// even은 지연 평가로 생성: 2, 4

auto squared = v | views::transform([](int x) { return x * x; });
// squared는 지연 평가로 생성: 1, 4, 9, 16, 25

// 뷰 조합
auto result = v | views::filter([](int x) { return x > 2; })
                | views::transform([](int x) { return x * 10; });
// result는 지연 평가로 생성: 30, 40, 50
```

### 7.8 std::print (C++23)

> **[C++23]** 이 기능은 C++23 이상에서 사용 가능합니다.

`std::print`는 Python 스타일의 형식화된 출력을 C++에 제공한다.

```cpp
#include <print>

std::print("Hello, {}!\n", "World");           // Hello, World!
std::print("x = {}, y = {}\n", 3, 4);         // x = 3, y = 4
std::println("No need for \\n with println");  // 자동으로 개행 추가
```

> **참고:** 2026년 현재 `std::print`에 대한 컴파일러 지원은 아직 제한적이다. 대부분의 코딩 테스트에서는 C++23 기능을 지원하지 않는다.

### 7.9 C++26 미리보기

> **[C++26]** C++26은 2026년에 비준될 예정이다. 대부분의 기능에 대한 컴파일러 지원은 아직 없다. 아래 내용은 참고용이다.

C++26은 **안전성**, **성능**, **병렬 컴퓨팅**에 초점을 맞춘 대규모 추가 기능을 포함한다.

#### 계약 (Contracts: `pre`, `post`, `contract_assert`)

C++26의 핵심 기능이다. 기존의 `assert()` 매크로를 언어 차원의 선제 조건/후속 조건으로 대체한다.

```cpp
// C++26 계약 문법 (아직 컴파일러 미지원)
int divide(int a, int b)
    pre(b != 0)                     // 선제 조건: b는 0이 아니어야 함
    post(r: r * b == a)             // 후속 조건: 결과 * b == a
{
    return a / b;
}

void process(int* ptr)
    pre(ptr != nullptr)             // 선제 조건: 포인터가 유효해야 함
{
    contract_assert(*ptr > 0);      // 런타임 단언 (assert() 대체)
    // ...
}
```

| 특성 | 기존 (`assert`) | 새로운 방식 (Contracts) |
|:----|:--------------|:-------------------|
| 문법 | 매크로 (`#include <cassert>`) | 언어 키워드 |
| 범위 | 런타임만 | 컴파일 타임 + 런타임 |
| 모듈과 호환 | 불가 (매크로 미내보내기) | 가능 |
| 선제 조건 | 함수 내부에서 수동 검사 | 시그니처에 `pre(조건)` |
| 후속 조건 | 미지원 | `post(결과: 조건)` |

> **핵심:** 계약이 필요한 이유 — C++20 모듈에서는 매크로가 내보내지지(export) 않기 때문에 기존 `assert()`를 모듈 경계에서 사용할 수 없다. `contract_assert`는 이를 대체하는 표준화된 방법이다.

#### 새로운 컨테이너와 라이브러리

| 헤더 | 설명 |
|:----|:----|
| `<inplace_vector>` | 고정 용량 동적 벡터; 힙 할당이지만 재할당 없음 — 배열의 성능과 vector 인터페이스를 결합 |
| `<hive>` | 대량 할당, `union` 기반 효율적 캐싱, 삭제 시 건너뛰기 기법을 통한 고성능 컨테이너 |
| `<debugging>` | 표준화된 디버깅 유틸리티; 디버그 모드 감지, 중단점(breakpoint) 프로그래밍 관리 |
| `<simd>` | CPU 아키텍처(AVX, SSE, NEON 등)에 걸친 통합 SIMD(단일 명령, 다중 데이터) 인터페이스 |
| `<linalg>` | `<mdspan>`을 활용한 선형대수 연산 — 행렬/벡터 곱셈, 변환 |
| `<hazard_pointer>` | RAII 소유권 기반의 잠금 없는(lock-free) 단일 쓰기/다중 읽기 스레드 안전 포인터 |
| `<rcu>` | Read-Copy-Update: 원자적 연산으로 잠금 없이 동시 읽기/쓰기/삭제 |

#### `constexpr` 전면 확대

C++26에서는 사실상 모든 표준 컨테이너와 유틸리티가 `constexpr`이 된다:

```cpp
// C++26: constexpr 컨테이너 (vector, array뿐만 아니라 전부)
constexpr std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
constexpr std::set<int> primes = {2, 3, 5, 7, 11};
constexpr std::deque<int> dq = {1, 2, 3};

// 예외까지 constexpr화
// constexpr 함수에서 컴파일 타임에 throw/catch 가능
```

#### `std::execution` (비동기 스케줄링)

센더(sender), 리시버(receiver), 스케줄러를 포함하는 포괄적인 비동기 프로그래밍 프레임워크:

```cpp
// C++26: std::execution (개념적 — 문법은 변경될 수 있음)
#include <execution>

auto work = std::execution::just(42)
          | std::execution::then([](int x) { return x * 2; })
          | std::execution::then([](int x) { return x + 1; });
// 스케줄: 42 → 84 → 85
```

> **참고:** NVIDIA가 오픈소스 참조 구현을 제공하고 있다: [github.com/NVIDIA/stdexec](https://github.com/NVIDIA/stdexec).

---

<br>

## 8. 코딩 테스트를 위한 실전 패턴

### 8.1 EOF까지 입력 읽기

```cpp
#include <iostream>
using namespace std;

int main() {
    int x;
    // 방법 1: while (cin >> x)
    while (cin >> x) {
        cout << x * 2 << endl;
    }

    // 방법 2: 줄 단위 읽기
    string line;
    while (getline(cin, line)) {
        cout << line << endl;
    }

    // 방법 3: 개수가 주어진 경우
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        // x 처리
    }

    return 0;
}
```

### 8.2 사용자 정의 객체 정렬

```cpp
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

struct Person {
    string name;
    int age;
};

int main() {
    vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 30}};

    // 방법 1: 람다 비교자 (가장 유연함)
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        if (a.age != b.age) return a.age < b.age;
        return a.name < b.name;
    });

    // 방법 2: 구조체에 operator< 오버로딩
    // struct Person {
    //     ...
    //     bool operator<(const Person& other) const {
    //         if (age != other.age) return age < other.age;
    //         return name < other.name;
    //     }
    // };
    // sort(people.begin(), people.end());  // operator< 사용

    return 0;
}
```

### 8.3 map으로 빈도 세기

```cpp
#include <map>
#include <vector>
using namespace std;

vector<int> nums = {1, 3, 2, 3, 1, 1, 2, 4};
map<int, int> freq;

for (int x : nums) {
    freq[x]++;
}
// freq = {1: 3, 2: 2, 3: 2, 4: 1}

// 가장 많이 나온 원소 찾기
int maxVal = 0, maxKey = 0;
for (const auto& [key, val] : freq) {   // C++17 구조적 바인딩
    if (val > maxVal) {
        maxVal = val;
        maxKey = key;
    }
}
// maxKey = 1, maxVal = 3
```

### 8.4 set으로 중복 제거

```cpp
#include <set>
#include <vector>
using namespace std;

vector<int> nums = {1, 3, 2, 3, 1, 1, 2, 4};

// 방법 1: set
set<int> unique_nums(nums.begin(), nums.end());
// unique_nums = {1, 2, 3, 4}

// 방법 2: sort + unique + erase (제자리 수정, 벡터 타입 유지)
sort(nums.begin(), nums.end());
nums.erase(unique(nums.begin(), nums.end()), nums.end());
// nums = {1, 2, 3, 4}
```

### 8.5 스택과 큐

**헤더:** `#include <stack>`, `#include <queue>`

```cpp
#include <stack>
#include <queue>
using namespace std;

// Stack — LIFO (Last In, First Out, 후입선출)
stack<int> st;
st.push(10);
st.push(20);
st.push(30);
cout << st.top() << endl;    // 30
st.pop();                     // 30 제거
cout << st.top() << endl;    // 20
cout << st.size() << endl;   // 2
cout << st.empty() << endl;  // false

// Queue — FIFO (First In, First Out, 선입선출)
queue<int> q;
q.push(10);
q.push(20);
q.push(30);
cout << q.front() << endl;   // 10
cout << q.back() << endl;    // 30
q.pop();                      // 10 제거
cout << q.front() << endl;   // 20
```

| | `stack<T>` | `queue<T>` |
|:--|:-----------|:-----------|
| 추가 | `push(val)` | `push(val)` |
| 제거 | `pop()` | `pop()` |
| 접근 | `top()` | `front()`, `back()` |
| 순서 | LIFO | FIFO |

### 8.6 priority_queue로 Top-K 문제

**헤더:** `#include <queue>`

`priority_queue`는 기본적으로 **최대 힙**이다 — 가장 큰 원소가 항상 맨 위에 있다.

```cpp
#include <queue>
#include <vector>
using namespace std;

// 최대 힙 (기본)
priority_queue<int> maxPQ;
maxPQ.push(30);
maxPQ.push(10);
maxPQ.push(50);
maxPQ.push(20);
cout << maxPQ.top() << endl;   // 50 (최댓값)
maxPQ.pop();
cout << maxPQ.top() << endl;   // 30

// 최소 힙
priority_queue<int, vector<int>, greater<int>> minPQ;
minPQ.push(30);
minPQ.push(10);
minPQ.push(50);
minPQ.push(20);
cout << minPQ.top() << endl;   // 10 (최솟값)

// 크기 K의 최대 힙을 사용하여 Top-K 최솟값 구하기
vector<int> data = {7, 3, 9, 1, 5, 8, 2, 6, 4, 10};
int k = 3;
priority_queue<int> topK;

for (int x : data) {
    topK.push(x);
    if (topK.size() > k) {
        topK.pop();             // 최댓값 제거 — K개의 최솟값만 유지
    }
}
// topK에는 {3, 2, 1} — 가장 작은 3개 원소
```

### 8.7 pair와 make_pair

**헤더:** `#include <utility>` (`<map>`, `<algorithm>` 등에 의해 자동 포함되기도 함)

```cpp
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

pair<int, string> p1 = {42, "answer"};
pair<int, string> p2 = make_pair(42, "answer");  // 동일

cout << p1.first << endl;     // 42
cout << p1.second << endl;    // "answer"

// pair는 사전순으로 비교됨: 먼저 .first로, 같으면 .second로
pair<int, int> a = {1, 5};
pair<int, int> b = {1, 3};
cout << (a > b) << endl;      // true (first가 같고, 5 > 3)

// 일반적 패턴: 정렬 시 원래 인덱스를 보존하기 위해 (값, 인덱스) 쌍 저장
vector<pair<int, int>> indexed;
vector<int> nums = {30, 10, 50, 20};
for (int i = 0; i < nums.size(); i++) {
    indexed.push_back({nums[i], i});
}
sort(indexed.begin(), indexed.end());
// indexed = {{10,1}, {20,3}, {30,0}, {50,2}}
// 최솟값 10은 원래 인덱스 1에 있었음을 알 수 있다
```

---

<br>

## 9. 템플릿 컴파일 주의사항

### 9.1 템플릿 정의는 헤더에 있어야 한다

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

### 9.2 흔한 오류

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

## 10. 흔한 실수 모음

### 10.1 insert/erase 후 반복자 무효화

`vector`에서 원소를 삽입하거나 삭제하면 기존 반복자가 **무효화**될 수 있다. 무효화된 반복자를 사용하면 **미정의 동작**이 발생한다.

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 잘못된 방법 — erase 후 반복자가 무효화됨
for (auto it = v.begin(); it != v.end(); it++) {
    if (*it % 2 == 0) {
        v.erase(it);   // 나쁨: 'it'이 무효화되고, 다음 it++은 미정의 동작
    }
}

// 올바른 방법 — erase가 다음 유효한 반복자를 반환
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0) {
        it = v.erase(it);   // erase가 다음 원소의 반복자를 반환
    } else {
        it++;
    }
}
// v = {1, 3, 5}
```

### 10.2 map의 []는 기본 항목을 생성한다

map의 `[]` 연산자는 키가 존재하지 않으면 **기본 생성된 값으로 새 항목을 삽입**한다. 이것은 매우 흔한 버그의 원인이다.

```cpp
map<string, int> m;

// 잘못된 방법 — 확인만 하려 했는데 {"Alice", 0} 항목이 생성됨!
if (m["Alice"] == 0) {
    cout << "Alice not found" << endl;
}
// m에 이제 {"Alice": 0}이 존재 — 의도하지 않은 부작용!

// 올바른 방법 — find()나 count()로 존재 여부 확인
if (m.find("Alice") == m.end()) {
    cout << "Alice not found" << endl;
}
// 또는
if (m.count("Alice") == 0) {
    cout << "Alice not found" << endl;
}
```

### 10.3 각 STL 컴포넌트의 #include 누락

각 STL 컴포넌트는 자체 헤더가 필요하다. 일부 언어와 달리 C++에는 "모두 임포트" 문이 없다.

```cpp
// 각각 별도의 #include가 필요
#include <vector>           // vector
#include <string>           // string
#include <map>              // map
#include <unordered_map>    // unordered_map
#include <set>              // set
#include <unordered_set>    // unordered_set
#include <algorithm>        // sort, find, reverse 등
#include <numeric>          // accumulate, iota 등
#include <stack>            // stack
#include <queue>            // queue, priority_queue
#include <utility>          // pair, make_pair
#include <optional>         // optional (C++17)
```

> **팁:** 일부 컴파일러(`<bits/stdc++.h>` 사용 시)는 올바른 include 없이도 컴파일될 수 있다. 하지만 이는 비표준이며 이식성이 없다. 항상 올바른 헤더를 포함하라.

### 10.4 범위 기반 for: 복사 vs 참조

```cpp
vector<string> names = {"Alice", "Bob", "Charlie"};

// 복사 — 반복마다 문자열을 복사 (큰 객체에 비용이 큼!)
for (auto x : names) {
    x = "Modified";       // 복사본만 수정, 원본은 변경 안 됨
}
// names는 변경 없음: {"Alice", "Bob", "Charlie"}

// 참조 — 복사 없음, 원본 수정
for (auto& x : names) {
    x = "Modified";       // 원본 벡터가 수정됨
}
// names는 이제: {"Modified", "Modified", "Modified"}

// const 참조 — 복사 없음, 읽기 전용 (읽기 접근에 최적)
for (const auto& x : names) {
    cout << x << endl;    // 효율적이고 안전
    // x = "change";      // 컴파일 오류 — const
}
```

> **경험 법칙:**
> - `for (auto x : v)` — 복사 비용이 적은 타입에만 사용 (`int`, `char`, `double`)
> - `for (const auto& x : v)` — 모든 타입의 읽기 전용 접근에 사용
> - `for (auto& x : v)` — 원소 수정이 필요할 때 사용

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
| `vector` | 동적 배열; `push_back`, `[]`, `at()`, `front()`, `back()`, `size()`, `capacity()`, `reserve()` |
| 2차원 vector | `vector<vector<int>> matrix(rows, vector<int>(cols, 0));` |
| `string` | `std::string`은 C 스타일 `char[]`을 대체; `+`, `find`, `substr`, `stoi`, `to_string` |
| `map` / `unordered_map` | 키-값 저장소; `map`은 정렬 O(log n), `unordered_map`은 해시 평균 O(1) |
| `set` / `unordered_set` | 고유 원소; `set`은 정렬 O(log n), `unordered_set`은 해시 평균 O(1) |
| `sort` | `sort(begin, end)` 또는 `sort(begin, end, 비교자)` 람다 사용 |
| `find`, `count` | 반복자 범위에서 선형 검색과 개수 세기 |
| `binary_search` | 정렬된 범위에서 O(log n) 검색; `lower_bound`, `upper_bound` |
| `accumulate` | `<numeric>`의 합계/리듀스, 초기값 필요 |
| `auto` (C++11) | 타입 추론; 반복자와 복잡한 타입에서 깔끔한 코드 |
| 범위 기반 for (C++11) | `for (const auto& x : container)` — 깔끔한 순회 |
| 람다 (C++11) | `[캡처](매개변수){ 본문 }` — sort, for_each와 함께 인라인 함수 |
| 구조적 바인딩 (C++17) | `auto [key, value] = pair;` — pair, tuple, 구조체 분해 |
| `std::optional` (C++17) | `has_value()`, `value_or()`로 안전한 nullable 값 |
| `stack` / `queue` | LIFO와 FIFO 어댑터; `push`, `pop`, `top`/`front` |
| `priority_queue` | 기본 최대 힙; `greater<>`로 최소 힙; Top-K 문제에 유용 |
| `pair` | `pair<T1,T2>`; 사전순 비교; `sort`와 함께 유용 |
| 헤더에 정의 | 템플릿 전체 정의가 헤더에 있어야 함; `.cpp` 분리 시 링커 오류 |
| 반복자 무효화 | vector의 `insert`/`erase`는 반복자를 무효화; `erase()`의 반환값 사용 |
| `map[]`의 항목 생성 | 존재 여부 확인 시 `[]`가 아닌 `find()`나 `count()` 사용 |
| for의 복사 vs 참조 | `for(auto x:v)` 복사; `for(auto& x:v)` 참조; `for(const auto& x:v)` 읽기 전용 |

---
