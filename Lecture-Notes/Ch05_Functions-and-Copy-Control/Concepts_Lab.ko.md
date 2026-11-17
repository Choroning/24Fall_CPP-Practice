# 5장 실습 — 참조, 참조 리턴, 복사 생성자

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **최종 수정일:** 2026-11-18

> **학습 목표:**
> 1. 객체를 값으로 전달할 때 생성자 메시지는 한 번, 소멸자 메시지는 두 번 출력되는 이유를 복사 생성자로 설명할 수 있다
> 2. 포인터와 참조의 차이를 주소 비교로 확인하고 참조로 초기화한 참조가 무엇을 가리키는지 설명할 수 있다
> 3. 참조를 리턴하는 함수의 호출이 대입문의 왼쪽에 올 수 있는 이유를 설명할 수 있다
> 4. 참조 매개변수로 결과를, 리턴값으로 성공 여부를 돌려주는 함수를 작성할 수 있다
> 5. 복사 생성자를 직접 작성하고 원본과 사본이 독립적임을 확인할 수 있다

---

## 목차

- [1. 개요](#1-개요)
- [2. 객체의 전달과 복사 생성자](#2-객체의-전달과-복사-생성자)
  - [2.1 값에 의한 호출](#21-값에-의한-호출)
  - [2.2 주소에 의한 호출](#22-주소에-의한-호출)
- [3. 참조 변수와 참조 리턴](#3-참조-변수와-참조-리턴)
  - [3.1 포인터와 참조의 비교](#31-포인터와-참조의-비교)
  - [3.2 참조 리턴](#32-참조-리턴)
  - [3.3 배열 원소의 참조 리턴](#33-배열-원소의-참조-리턴)
- [4. 참조 매개변수로 결과 돌려주기](#4-참조-매개변수로-결과-돌려주기)
- [5. 복사 생성자 작성](#5-복사-생성자-작성)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 개요

> **참고:** 값, 주소, 참조에 의한 호출, 참조 변수와 참조 리턴, 복사 생성자와 얕은 복사, 깊은 복사의 이론은 [5장 강의 노트](Concepts_Lecture.ko.md)에서 다룬다. 이 실습은 각 개념을 짧은 코드로 실행하여 출력으로 확인한다.

| 주제 | 위치 |
|:-----|:-----|
| 값과 주소에 의한 객체 전달에서 생성자와 소멸자의 실행 | 2절 |
| 참조 변수, 참조 리턴, 배열 원소의 참조 리턴 분석 | 3절 |
| 참조 매개변수로 평균을 돌려주는 함수 | 4절 |
| 복사 생성자의 작성과 실행 | 5절 |
| 포인터 멤버를 가진 `Person` 클래스의 얕은 복사와 깊은 복사 | [5장 강의 노트 4.3절, 4.6절](Concepts_Lecture.ko.md#43-얕은-복사의-문제) |
| 원의 반지름을 더하는 `increaseBy()` 함수의 수정 | [5장 연습 문제 1](Exercises.ko.md#연습-문제-1-다양한-함수-호출법) |
| 참조 매개변수로 값을 꺼내는 정수 스택 `MyIntStack` | [HW4 문제 1](../../Coursework/HW04_References/Original/Assignment.ko.md#문제-1-9주차-강의자료-실습-문제-2-myintstack-코드-구현-및-분석) |
| 값을 누적하는 `Accumulator`, 깊은 복사 생성자를 가진 `Book` | [HW5 문제 1, 2](../../Coursework/HW05_Reference-Return-and-Copy-Constructors/Original/Assignment.ko.md) |

*표 1. 실습의 주제와 위치*

모든 코드는 C++17 표준으로 컴파일하여 실행한다(예: `g++ -std=c++17 -Wall`).

---

<br>

## 2. 객체의 전달과 복사 생성자

### 2.1 값에 의한 호출

다음 `Circle`은 생성자와 소멸자가 실행될 때마다 메시지를 출력한다. `increase()`는 받은 원의 반지름을 1 늘리려 한다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle();
    Circle(int r);
    ~Circle();
    double getArea() { return 3.14 * radius * radius; }
    int getRadius() { return radius; }
    void setRadius(int radius) { this->radius = radius; }
};

Circle::Circle() {
    radius = 1;
    cout << "생성자 실행 radius = " << radius << endl;
}

Circle::Circle(int radius) {
    this->radius = radius;
    cout << "생성자 실행 radius = " << radius << endl;
}

Circle::~Circle() {
    cout << "소멸자 실행 radius = " << radius << endl;
}

void increase(Circle c) {               // 값에 의한 호출: c는 waffle의 사본
    int r = c.getRadius();
    c.setRadius(r + 1);
}

int main() {
    Circle waffle(30);
    increase(waffle);
    cout << waffle.getRadius() << endl;
}
```

```text
생성자 실행 radius = 30
소멸자 실행 radius = 31
30
소멸자 실행 radius = 30
```

- `increase(waffle)`을 호출하면 매개변수 `c`가 `Circle c = waffle;`처럼 `waffle`의 내용으로 초기화된다. `c`는 `waffle`과 별개의 객체이므로 `c`의 반지름을 31로 바꾸어도 `waffle`은 30 그대로이다.
- 생성자 메시지는 `waffle`을 만들 때 한 번만 출력되지만 소멸자 메시지는 `c`(31)와 `waffle`(30)에서 두 번 출력된다. 객체가 만들어질 때는 반드시 생성자가 실행되는데, `c`를 만들 때 실행된 것은 메시지를 출력하는 두 생성자가 아니라 컴파일러가 자동으로 넣은 **복사 생성자**이다. 복사 생성자는 멤버를 하나씩 복사할 뿐 메시지를 출력하지 않는다.
- `c`는 `increase()`의 지역 객체이므로 함수가 끝날 때 먼저 소멸하고, `waffle`은 `main()`이 끝날 때 소멸한다.

### 2.2 주소에 의한 호출

`increase()`가 객체의 주소를 받도록 바꾸면 사본이 만들어지지 않는다. 클래스와 생성자, 소멸자는 2.1절과 같다.

```cpp
void increase(Circle* p) {              // 주소에 의한 호출: p는 waffle을 가리킴
    int r = p->getRadius();
    p->setRadius(r + 1);
}

int main() {
    Circle waffle(30);
    increase(&waffle);
    cout << waffle.getRadius() << endl;
}
```

```text
생성자 실행 radius = 30
31
소멸자 실행 radius = 31
```

매개변수 `p`는 `Circle* p = &waffle;`처럼 `waffle`의 주소로 초기화되는 포인터 변수일 뿐 새 객체가 아니므로, 생성자도 소멸자도 추가로 실행되지 않는다. `p->setRadius()`는 `waffle` 자체를 바꾸므로 31이 출력된다. 참조 매개변수 `void increase(Circle& c)`를 쓰면 같은 효과를 포인터 문법 없이 얻는다(5장 강의 노트 2.2절).

---

<br>

## 3. 참조 변수와 참조 리턴

### 3.1 포인터와 참조의 비교

> **용어 정의:** **참조(reference)** 는 이미 존재하는 변수나 객체에 붙이는 또 하나의 이름(별명)으로, 새 메모리 공간을 할당하지 않고 초기화된 대상을 공유한다.

학생을 이름 대신 학번으로 부를 수 있듯이, 참조는 같은 대상을 다른 이름으로 부르게 한다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int test = 3;
    int* p = &test;                     // 포인터: test의 주소를 저장하는 별도의 변수
    int& r = test;                      // 참조: test의 다른 이름

    cout << test << ' ' << *p << ' ' << r << endl;
    cout << boolalpha;
    cout << (&test == p) << ' ' << (&r == &test) << ' ' << ((void*)&p == (void*)&test) << endl;
    r = r + 2;                          // test가 바뀜
    cout << test << ' ' << r << endl;

    int data = 3;
    int& ref = data;
    int& alias = ref;                   // 참조로 초기화한 참조도 data를 가리킴
    alias = 5;
    cout << data << ' ' << ref << ' ' << alias << endl;
    cout << (&alias == &data) << endl;
}
```

```text
3 3 3
true true false
5 5
5 5 5
true
```

- `p`에는 `test`의 주소가 들어 있으므로 `&test == p`이다. 참조 `r`의 주소 `&r`도 `test`의 주소와 같다. 참조는 자기 공간을 갖지 않고 `test`의 공간을 함께 쓰기 때문이다.
- 반면 포인터 `p`는 그 자체가 주소를 담는 변수이므로 자기 주소 `&p`는 `test`의 주소와 다르다.
- `int& alias = ref;`는 `ref`의 값 3을 복사하는 것이 아니라 `ref`가 가리키는 대상, 곧 `data`를 가리키는 참조를 하나 더 만든다. 그래서 `alias = 5;`는 `data`를 5로 바꾼다.

### 3.2 참조 리턴

```cpp
#include <iostream>
using namespace std;

int& refReturn(int& ref) {
    return ref;                         // 매개변수가 가리키는 변수의 참조를 리턴
}

int main() {
    int data = 3;
    cout << "1. data: " << data << endl;
    refReturn(data) = 5;                // 리턴된 참조, 곧 data에 5를 대입
    cout << "2. data: " << data << endl;
    int tmp = refReturn(data);          // 리턴된 참조가 가리키는 값을 복사
    cout << "3. tmp: " << tmp << endl;
}
```

```text
1. data: 3
2. data: 5
3. tmp: 5
```

참조를 리턴하는 함수의 호출 `refReturn(data)`는 값이 아니라 `data`라는 변수 자체를 나타내므로 대입문의 왼쪽에 올 수 있다. 반면 `int tmp = refReturn(data);`의 `tmp`는 참조가 아니라 일반 변수이므로, 리턴된 참조가 가리키는 값 5가 `tmp`에 복사된다.

### 3.3 배열 원소의 참조 리턴

**분석:** 다음 코드에서 `refReturn(a, 2) = 5;`가 컴파일되고 배열을 바꾸는 이유를 설명한다.

```cpp
#include <iostream>
#define SIZE 3
using namespace std;

int& refReturn(int* p, int index);

int main() {
    int i, a[SIZE] = { 1, 2, 3 };
    for (i = 0; i < SIZE; i++)
        cout << a[i] << ", ";
    cout << endl;

    refReturn(a, 2) = 5;                // a[2]에 5를 대입
    for (i = 0; i < SIZE; i++)
        cout << a[i] << ", ";
    cout << endl;
}

int& refReturn(int* p, int index) {
    int& r = p[index];                  // r은 a[index]의 별명
    return r;
}
```

```text
1, 2, 3, 
1, 2, 5, 
```

- `refReturn(a, 2)`에서 `p`는 `a`의 첫 원소를 가리키고, `p[2]`는 `a[2]`이다. `int& r = p[2];`로 `r`은 `a[2]`의 별명이 된다.
- `r`을 참조로 리턴하므로 호출식 `refReturn(a, 2)`는 `a[2]` 자체를 나타내고, `= 5`는 `a[2]`에 5를 대입한다.
- 참조 `r`은 함수의 지역 이름이지만 가리키는 대상 `a[2]`는 `main()`의 배열이므로 함수가 끝난 뒤에도 살아 있다. 함수 안의 지역 변수를 참조로 리턴하면 함수가 끝나면서 그 변수가 사라져 댕글링 참조가 되므로 안 된다(5장 강의 노트 5.1절).

---

<br>

## 4. 참조 매개변수로 결과 돌려주기

**문제:** 정수 배열의 평균을 참조 매개변수 `avg`로 돌려주고, 리턴값으로는 계산의 성공 여부를 돌려주는 `average()`를 작성한다. 배열의 크기가 0 이하이면 실패이다.

```cpp
#include <iostream>
using namespace std;

bool average(int a[], int size, int& avg) {
    if (size <= 0)
        return false;                   // 실패: avg는 바꾸지 않음
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += a[i];
    avg = sum / size;                   // 참조 매개변수로 평균 전달
    return true;                        // 성공 여부는 리턴값으로 전달
}

int main() {
    int x[] = { 0, 1, 2, 3, 4, 5 };
    int avg;
    if (average(x, 6, avg))
        cout << "평균은 " << avg << endl;
    else
        cout << "매개 변수 오류" << endl;

    if (average(x, -2, avg))
        cout << "평균은 " << avg << endl;
    else
        cout << "매개 변수 오류" << endl;
}
```

```text
평균은 2
매개 변수 오류
```

- 함수는 리턴값을 하나만 가질 수 있으므로, 결과값과 성공 여부를 함께 알리려면 하나는 리턴값으로, 다른 하나는 참조 매개변수로 돌려준다.
- 평균 15 / 6은 정수 나눗셈이므로 2이다. 두 번째 호출은 `false`를 리턴하므로 `avg`의 값은 의미가 없고, 호출한 쪽은 리턴값을 먼저 확인해야 한다.

---

<br>

## 5. 복사 생성자 작성

> **용어 정의:** **복사 생성자(copy constructor)** 는 같은 클래스의 객체로 새 객체를 초기화할 때 호출되는 생성자로, 자기 클래스에 대한 참조를 매개변수로 가진다.

```cpp
#include <iostream>
using namespace std;

class Circle {
public:
    int radius;
    Circle(Circle& c);                  // 복사 생성자 선언
    Circle(int radius) {
        this->radius = radius;
        cout << "인자 1개 생성자: " << radius << endl;
    }
    ~Circle() { cout << "소멸자: " << radius << endl; }
    double getArea() { return 3.14 * radius * radius; }
};

Circle::Circle(Circle& c) {             // 복사 생성자 정의
    this->radius = c.radius;
    cout << "복사 생성자 실행 radius = " << radius << endl;
}

int main() {
    Circle src(30);
    Circle dest(src);                   // Circle dest = src;와 같은 복사 생성
    cout << "원본의 면적 = " << src.getArea() << endl;
    cout << "사본의 면적 = " << dest.getArea() << endl;
    dest.radius = 10;
    cout << "사본의 면적 = " << dest.getArea() << endl;
}
```

```text
인자 1개 생성자: 30
복사 생성자 실행 radius = 30
원본의 면적 = 2826
사본의 면적 = 2826
사본의 면적 = 314
소멸자: 10
소멸자: 30
```

- `Circle dest(src);`는 `src`를 인자로 복사 생성자를 호출한다. 이 복사 생성자는 출력문을 빼면 컴파일러가 자동으로 넣는 디폴트 복사 생성자와 같은 일, 곧 멤버를 하나씩 복사하는 일을 한다. 그래서 이 복사 생성자를 지워도 프로그램은 메시지 한 줄만 빼고 똑같이 동작한다.
- `dest.radius = 10;`은 사본만 바꾸고 원본 `src`는 그대로이다. `radius`처럼 값을 직접 담는 멤버만 있으면 멤버 단위의 복사로 원본과 사본이 완전히 분리된다.
- 매개변수는 반드시 참조여야 한다. `Circle(Circle c)`처럼 값으로 받으면 인자를 매개변수로 복사하기 위해 또 복사 생성자를 호출해야 하므로 끝없는 재귀가 되어, 컴파일러가 이 선언을 거부한다(5장 강의 노트 5.3절). 원본을 바꾸지 않으므로 `Circle(const Circle& c)`로 선언하는 것이 일반적이다.

> **주의:** 멤버가 동적으로 할당한 메모리를 가리키는 포인터이면 디폴트 복사 생성자의 멤버 단위 복사로는 원본과 사본이 같은 메모리를 공유하게 되므로, 새 메모리를 할당하여 내용을 복사하는 깊은 복사 생성자를 작성해야 한다(5장 강의 노트 4.3절\~4.6절).

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 값에 의한 객체 전달 | 매개변수 객체는 복사 생성자로 만들어지므로 사용자 생성자의 메시지는 출력되지 않고, 함수가 끝날 때 소멸자는 실행된다. |
| 주소에 의한 객체 전달 | 매개변수는 포인터일 뿐 새 객체가 아니므로 생성자와 소멸자가 추가로 실행되지 않고 원래 객체가 바뀐다. |
| 포인터와 참조 | 포인터는 주소를 담는 별도의 변수로 자기 주소를 가지지만, 참조는 대상의 다른 이름이어서 주소가 대상과 같다. |
| 참조로 초기화한 참조 | `int& alias = ref;`는 `ref`가 가리키는 원래 변수를 가리키는 참조를 하나 더 만든다. |
| 참조 리턴 | 참조를 리턴하는 함수의 호출은 변수 자체를 나타내므로 대입문의 왼쪽에 올 수 있지만, 지역 변수를 참조로 리턴하면 안 된다. |
| 참조 매개변수 | 결과는 참조 매개변수로, 성공 여부는 리턴값으로 돌려주면 함수가 두 가지 정보를 함께 전달할 수 있다. |
| 복사 생성자 | 자기 클래스에 대한 참조를 매개변수로 가지며, 작성하지 않으면 컴파일러가 멤버 단위로 복사하는 디폴트 복사 생성자를 넣는다. |

---

<br>

## 점검 문제

1. 2.1절의 `increase()`를 `void increase(Circle& c)`로 바꾸면 출력은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `생성자 실행 radius = 30`, `31`, `소멸자 실행 radius = 31`이 출력된다. 참조 매개변수 `c`는 `waffle`의 다른 이름일 뿐 새 객체가 아니므로 복사 생성자도 소멸자도 실행되지 않고, `c.setRadius(31)`이 `waffle` 자체를 바꾼다. 결과는 2.2절의 주소에 의한 호출과 같지만 함수 안에서 `->` 대신 `.`을 쓴다.

   </details>

2. 3.1절에서 `int& r = test;` 대신 `int& r;`로 선언하고 다음 줄에서 `r = test;`를 쓰면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `int& r;`에서 컴파일 오류가 난다. 참조는 선언할 때 반드시 대상으로 초기화해야 하며, 한번 정한 대상을 바꿀 수 없다. 초기화된 참조에 `r = test;`를 쓰는 것은 대상을 바꾸는 것이 아니라, 참조가 이미 가리키는 변수에 `test`의 값을 대입하는 것이다.

   </details>

3. 다음 함수를 3.3절의 `refReturn` 대신 쓰면 무엇이 문제인가?

   ```cpp
   int& refReturn(int* p, int index) {
       int value = p[index];
       return value;
   }
   ```

   <details>
   <summary>정답</summary>

   `value`는 `p[index]`의 값을 복사한 지역 변수이므로, 그 참조를 리턴하면 함수가 끝나 `value`가 사라진 뒤에도 참조가 남는 댕글링 참조가 된다. `refReturn(a, 2) = 5;`는 이미 사라진 공간에 쓰는 정의되지 않은 동작이며, 설령 실행되더라도 `a[2]`는 바뀌지 않는다. 컴파일러는 대개 지역 변수의 참조를 리턴한다고 경고한다.

   </details>

4. 5절의 `main()`에 `Circle copy2 = dest;`를 마지막 줄로 추가하면 출력의 마지막 부분은 어떻게 바뀌는가?

   <details>
   <summary>정답</summary>

   `Circle copy2 = dest;`도 같은 클래스의 객체로 새 객체를 초기화하므로 복사 생성자가 호출되어 `복사 생성자 실행 radius = 10`이 출력된다. `main()`이 끝나면 생성의 반대 순서로 `copy2`, `dest`, `src`가 소멸하므로 `소멸자: 10`, `소멸자: 10`, `소멸자: 30`이 출력된다.

   </details>
