# HW2 코드 총정리 — 객체 포인터와 this 포인터

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **학습 목표:**
> 1. 네 풀이의 클래스 정의와 `main()`을 줄 단위로 읽고, 객체 포인터와 `this`가 쓰이는 곳을 찾을 수 있다
> 2. 객체 배열의 원소와 포인터가 가리키는 대상이 문장마다 어떻게 바뀌는지 추적할 수 있다
> 3. 동적 배열이 할당되고 반환되는 시점과 그 뒤의 포인터 상태를 설명할 수 있다
> 4. `this`를 리턴하는 멤버 함수를 이어서 호출할 때 각 호출이 어느 객체에 적용되는지 추적할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: Complex 객체 배열과 포인터](#2-q1cpp-complex-객체-배열과-포인터)
  - [2.1 전체 코드](#21-전체-코드)
  - [2.2 줄 단위 설명](#22-줄-단위-설명)
  - [2.3 실행 추적](#23-실행-추적)
- [3. Q2.cpp: Person 클래스와 객체 포인터](#3-q2cpp-person-클래스와-객체-포인터)
  - [3.1 전체 코드](#31-전체-코드)
  - [3.2 줄 단위 설명](#32-줄-단위-설명)
  - [3.3 실행 추적](#33-실행-추적)
- [4. Q3.cpp: 정적 배열과 동적 배열](#4-q3cpp-정적-배열과-동적-배열)
  - [4.1 전체 코드](#41-전체-코드)
  - [4.2 줄 단위 설명](#42-줄-단위-설명)
  - [4.3 실행 추적](#43-실행-추적)
- [5. Q4.cpp: Counter 클래스와 this 포인터](#5-q4cpp-counter-클래스와-this-포인터)
  - [5.1 전체 코드](#51-전체-코드)
  - [5.2 줄 단위 설명](#52-줄-단위-설명)
  - [5.3 실행 추적](#53-실행-추적)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

네 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스 정의, `main()`을 차례로 두며, Q3만은 클래스 없이 `main()` 하나로 된 강의자료의 코드이다. 세 클래스는 멤버 함수를 모두 클래스 안에서 선언과 동시에 정의한다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW2 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 클래스 | 포인터의 쓰임 | 입력 |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Complex` | 객체 배열 `data`를 가리키는 `Complex *p` | 없음 |
| [`Q2.cpp`](Solution/Q2.cpp) | `Person` | 객체 `person2`를 가리키는 `Person* personPtr` | 없음 |
| [`Q3.cpp`](Solution/Q3.cpp) | 없음 | `new int[size]`로 만든 동적 배열을 가리키는 `int* pDynamic` | 배열 크기 `size` |
| [`Q4.cpp`](Solution/Q4.cpp) | `Counter` | 멤버 함수가 리턴하는 `this` | 없음 |

*표 1. HW2 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: Complex 객체 배열과 포인터

### 2.1 전체 코드

```cpp
/**
 * @file    A2Q1.cpp
 * @brief   Q1. 포인터로 다루는 Complex 객체 배열 (5주차 문제 5-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
#define MAX 2
using namespace std;

class Complex{
    int real, image;
public:
    Complex(){
        real = image = 1;
    }
    Complex(int ir, int ii){
        real = ir;
        image = ii;
    }
    
    void setReal(int ir){
        real = ir;
    }
    void setImage(int ii){
        image = ii;
    }
    void showC(){
        cout << real << " + " << image << "i" << endl;
    }
};

int main()
{
    Complex data[MAX];
    Complex *p = data;
    
    for (int i = 0; i < MAX; i++){
        data[i].showC();
    }
    cout << "---------------" << endl;
    
    data[0].setReal(3);
    (*(p + 1)).setImage(-8);
    
    for (int i = 0; i < MAX; i++){
        (p + i)->showC();
    }
}
```

### 2.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~10 | `#include <iostream>`, `#define MAX 2`, `using namespace std;` | `cout`과 `endl`을 위해 `<iostream>`을 포함하고, 전처리기가 이후의 `MAX`를 모두 2로 바꾸게 하며, `std::`를 생략한다 |
| 12~13 | `class Complex{ int real, image;` | 접근 지정자 없이 선언한 두 멤버는 `class`의 기본 접근 지정에 따라 `private`이다 |
| 14 | `public:` | 이후의 생성자와 멤버 함수를 `public`으로 선언한다 |
| 15~17 | `Complex(){ real = image = 1; }` | 기본 생성자이다. 대입은 오른쪽부터 결합하므로 `image`에 1을 대입한 뒤 그 값을 `real`에 대입한다 |
| 18~21 | `Complex(int ir, int ii)` | 받은 두 정수를 `real`, `image`에 대입한다. 이 프로그램에서는 호출되지 않는다 |
| 23~28 | `setReal()`, `setImage()` | 실수부와 허수부를 각각 받은 값으로 바꾼다 |
| 29~31 | `showC()` | 실수부, `" + "`, 허수부, `"i"`를 이어서 출력하고 줄을 바꾼다 |
| 36 | `Complex data[MAX];` | 전처리 후 `Complex data[2];`가 되며, `data[0]`, `data[1]`의 순서로 기본 생성자가 호출된다 |
| 37 | `Complex *p = data;` | 배열 이름이 첫 원소의 주소로 변환되어 `p`는 `&data[0]`을 가진다 |
| 39~41 | 첫 번째 `for` 문 | 배열 이름과 첨자로 `data[0]`, `data[1]`의 `showC()`를 호출한다 |
| 42 | 구분선 출력 | `-` 15개를 출력하고 줄을 바꾼다 |
| 44 | `data[0].setReal(3);` | 첫 원소의 실수부를 3으로 바꾼다 |
| 45 | `(*(p + 1)).setImage(-8);` | `p + 1`이 가리키는 `data[1]`을 역참조하여 허수부를 -8로 바꾼다 |
| 47~49 | 두 번째 `for` 문 | `(p + i)->showC()`로 포인터를 통해 두 원소를 출력한다 |

*표 2. Q1.cpp의 줄 단위 설명*

### 2.3 실행 추적

| 실행한 문장 | `data[0]` | `data[1]` | 출력 |
|:-----|:-----|:-----|:-----|
| `Complex data[MAX];` | (1, 1) | (1, 1) | |
| 첫 번째 `for` 문 | (1, 1) | (1, 1) | `1 + 1i`, `1 + 1i` |
| 구분선 출력 | (1, 1) | (1, 1) | `---------------` |
| `data[0].setReal(3);` | (3, 1) | (1, 1) | |
| `(*(p + 1)).setImage(-8);` | (3, 1) | (1, -8) | |
| 두 번째 `for` 문 | (3, 1) | (1, -8) | `3 + 1i`, `1 + -8i` |

*표 3. Q1의 `main()` 실행에 따른 두 원소의 (`real`, `image`)*

두 번째 `for` 문이 시작될 때 배열과 포인터는 다음과 같다. `p`는 37행 이후 바뀌지 않으며, `p + i`는 `p`가 가리키는 위치에서 원소 `i`개만큼 떨어진 원소를 가리킨다.

```text
   p              p + 1
   │                │
   ▼                ▼
┌───────────────┬───────────────┐
│ data[0]       │ data[1]       │
│ real  = 3     │ real  = 1     │
│ image = 1     │ image = -8    │
└───────────────┴───────────────┘
```

*그림 1. 두 번째 for 문 직전의 배열 data와 포인터 p*

---

<br>

## 3. Q2.cpp: Person 클래스와 객체 포인터

### 3.1 전체 코드

```cpp
/**
 * @file    A2Q2.cpp
 * @brief   Q2. 포인터로 객체를 참조하는 자유 코드
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {}
    void introduce() {
        cout << "안녕하세요, 제 이름은 " << name << "이고 " << age << "살입니다." << endl;
    }
    void setAge(int newAge) {
        age = newAge;
    }
};

int main() {
    Person person1("김철수", 20);
    Person person2("엘리자베스", 25);
    Person* personPtr = &person2;
    
    person1.introduce();
    personPtr->introduce();

    (*personPtr).setAge(31);
    personPtr->introduce();

    return 0;
}
```

### 3.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~10 | `#include`, `using namespace std;` | 입출력을 위해 `<iostream>`을, 문자열 멤버를 위해 `<string>`을 포함하고 `std::`를 생략한다 |
| 12~14 | `class Person { string name; int age;` | 접근 지정자 앞에 선언한 이름과 나이는 `private` 멤버이다 |
| 16 | `Person(string n, int a) : name(n), age(a) {}` | 멤버 초기화 리스트로 `name`과 `age`를 초기화하는 생성자이며, 몸체는 비어 있다 |
| 17~19 | `introduce()` | 이름과 나이를 넣은 자기소개 문장을 출력한다 |
| 20~22 | `setAge(int newAge)` | `age`를 `newAge`로 바꾼다 |
| 26~27 | `person1`, `person2` 생성 | 생성자에 이름과 나이를 주어 두 객체를 만든다. 문자열 리터럴은 `string` 매개변수 `n`으로 변환된다 |
| 28 | `Person* personPtr = &person2;` | 주소 연산자 `&`로 얻은 `person2`의 주소를 객체 포인터에 저장한다 |
| 30 | `person1.introduce();` | 객체 이름과 점 연산자로 `person1`의 멤버 함수를 호출한다 |
| 31 | `personPtr->introduce();` | 화살표 연산자로 `person2`의 멤버 함수를 호출한다 |
| 33 | `(*personPtr).setAge(31);` | 포인터를 역참조하여 얻은 `person2`에 점 연산자로 `setAge(31)`을 호출한다 |
| 34 | `personPtr->introduce();` | 바뀐 나이가 반영된 `person2`의 자기소개를 출력한다 |
| 36 | `return 0;` | 0을 리턴하여 프로그램이 정상 종료되었음을 알린다 |

*표 4. Q2.cpp의 줄 단위 설명*

### 3.3 실행 추적

| 실행한 문장 | `person1.age` | `person2.age` | `personPtr` | 출력되는 나이 |
|:-----|-----:|-----:|:-----|:-----|
| 26~27행 | 20 | 25 | | |
| `Person* personPtr = &person2;` | 20 | 25 | `&person2` | |
| `person1.introduce();` | 20 | 25 | `&person2` | 20 (`person1`) |
| `personPtr->introduce();` | 20 | 25 | `&person2` | 25 (`person2`) |
| `(*personPtr).setAge(31);` | 20 | 31 | `&person2` | |
| `personPtr->introduce();` | 20 | 31 | `&person2` | 31 (`person2`) |

*표 5. Q2의 `main()` 실행에 따른 두 객체의 나이와 포인터*

---

<br>

## 4. Q3.cpp: 정적 배열과 동적 배열

### 4.1 전체 코드

```cpp
/**
 * @file    A2Q3.cpp
 * @brief   Q3. 정적 배열과 동적 배열 (5주차 강의자료 p.15 코드 분석)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
using namespace std;

int main()
{
    int size;
    cout << "Input size: ";
    cin >> size;
    
    // int arrStatic[size] = { }; 일반 변수: 가능한가?
    int arrStatic[4] = { 1, 3, 5 };
    
    for (int i = 0; i < 4; i++)
        cout << arrStatic[i] << ' ';
    cout << endl;
    
    // 동적 메모리는 가변 길이 배열 선언 가능
    int* pDynamic = new int[size];
    
    for (int i = 0; i < size; i++)
        pDynamic[i] = i * 3;
    
    for (int i = 0; i < size; i++)
        cout << pDynamic[i] << ' ';
    cout << endl;
    
    delete[] pDynamic;
    // pDynamic = NULL;
    pDynamic = nullptr;
}
```

### 4.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 13~15 | `int size;`, 프롬프트, `cin >> size;` | 배열의 크기를 입력받는다 |
| 17 | `// int arrStatic[size] = { }; ...` | 크기가 일반 변수인 배열을 선언할 수 있는지 묻는 주석이다. 표준 C++에서는 선언할 수 없다 |
| 18 | `int arrStatic[4] = { 1, 3, 5 };` | 크기 4의 배열을 만들고 앞의 세 원소를 1, 3, 5로, 나머지 원소를 0으로 초기화한다 |
| 20~22 | `for` 문과 `cout << endl;` | 네 원소를 공백으로 구분하여 출력하고 줄을 바꾼다. `for` 문의 몸체는 21행 한 문장이다 |
| 24~25 | 주석, `int* pDynamic = new int[size];` | 주석대로 실행 중에 정한 크기로 배열을 만든다. 힙에 `int` 원소 `size`개를 할당하고 첫 원소의 주소를 `pDynamic`에 저장하며, 원소는 초기화되지 않는다 |
| 27~28 | 첫 번째 `for` 문 | `pDynamic[i]`에 `i * 3`을 저장한다. `pDynamic[i]`는 `*(pDynamic + i)`와 같다 |
| 30~32 | 두 번째 `for` 문과 `cout << endl;` | 동적 배열의 원소를 공백으로 구분하여 출력하고 줄을 바꾼다 |
| 34 | `delete[] pDynamic;` | `new[]`로 할당한 배열 전체를 반환한다. 이 시점부터 `pDynamic`은 댕글링 포인터이다 |
| 35~36 | `// pDynamic = NULL;`, `pDynamic = nullptr;` | C 스타일의 `NULL` 대신 C++11의 `nullptr`로 포인터를 널 포인터로 만든다 |

*표 6. Q3.cpp의 줄 단위 설명*

### 4.3 실행 추적

| 실행한 문장 | `arrStatic` | `pDynamic`이 가리키는 배열 | 출력 |
|:-----|:-----|:-----|:-----|
| `cin >> size;` (`5` 입력) | | | `Input size: ` |
| `int arrStatic[4] = { 1, 3, 5 };` | {1, 3, 5, 0} | | |
| 20~22행 | {1, 3, 5, 0} | | `1 3 5 0 ` |
| `new int[size]` | {1, 3, 5, 0} | 초기화되지 않은 원소 5개 | |
| 27~28행 | {1, 3, 5, 0} | {0, 3, 6, 9, 12} | |
| 30~32행 | {1, 3, 5, 0} | {0, 3, 6, 9, 12} | `0 3 6 9 12 ` |
| `delete[] pDynamic;` | {1, 3, 5, 0} | 반환됨 (`pDynamic`은 댕글링 포인터) | |
| `pDynamic = nullptr;` | {1, 3, 5, 0} | 없음 (`pDynamic`은 널 포인터) | |

*표 7. Q3의 `main()` 실행에 따른 두 배열의 상태 (입력 `5`)*

---

<br>

## 5. Q4.cpp: Counter 클래스와 this 포인터

### 5.1 전체 코드

```cpp
/**
 * @file    A2Q4.cpp
 * @brief   Q4. this 포인터를 리턴하는 메서드 체이닝 자유 코드
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
using namespace std;

class Counter {
    int count;
public:
    Counter() : count(0) {}
    Counter* increment() {
        this->count++;
        return this;
    }
    Counter* decrement() {
        this->count--;
        return this;
    }
    void display() {
        cout << "Count: " << this->count << endl;
    }
};

int main() {
    Counter c;
    
    c.increment()->increment()->display();  // Count: 2
    c.decrement()->display();  // Count: 1
}
```

### 5.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 11~12 | `class Counter { int count;` | 현재 값을 저장하는 `private` 멤버 `count`를 선언한다 |
| 14 | `Counter() : count(0) {}` | 멤버 초기화 리스트로 `count`를 0으로 초기화하는 생성자이다 |
| 15~18 | `Counter* increment()` | `this->count`를 1 늘리고, 호출된 객체의 주소 `this`를 리턴한다 |
| 19~22 | `Counter* decrement()` | `this->count`를 1 줄이고 `this`를 리턴한다 |
| 23~25 | `void display()` | `Count: `와 현재 값을 출력한다. 아무것도 리턴하지 않는다 |
| 29 | `Counter c;` | 생성자가 호출되어 `c.count`가 0이 된다 |
| 31 | `c.increment()->increment()->display();` | 왼쪽부터 차례로 호출된다. 각 호출이 리턴한 `&c`에 다음 함수를 `->`로 호출하므로 세 호출이 모두 `c`에 적용된다 |
| 32 | `c.decrement()->display();` | `c.count`를 1 줄인 뒤 리턴된 `&c`로 `display()`를 호출한다 |

*표 8. Q4.cpp의 줄 단위 설명*

### 5.3 실행 추적

| 호출 | 호출 대상 | `c.count` | 리턴값 | 출력 |
|:-----|:-----|-----:|:-----|:-----|
| `Counter c;` | | 0 | | |
| `c.increment()` | `c` | 1 | `&c` | |
| `->increment()` | `c` | 2 | `&c` | |
| `->display()` | `c` | 2 | 없음 | `Count: 2` |
| `c.decrement()` | `c` | 1 | `&c` | |
| `->display()` | `c` | 1 | 없음 | `Count: 1` |

*표 9. Q4의 `main()` 실행에 따른 호출 대상과 `c.count`*

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 파일의 구성 | 세 클래스는 멤버 함수를 모두 클래스 안에서 정의하고, Q3은 클래스 없이 `main()`만으로 되어 있다. |
| 원소 상태의 추적 | Q1의 두 원소는 (1, 1), (1, 1)에서 (3, 1), (1, -8)로 바뀐다. |
| 포인터와 원본 | Q2의 `personPtr`로 바꾼 나이는 `person2` 자체의 나이이며, 25에서 31로 바뀐다. |
| 동적 배열의 수명 | Q3의 동적 배열은 25행에서 할당되어 34행에서 반환되며, 36행에서 포인터가 널 포인터가 된다. |
| 호출의 연결 | Q4의 각 멤버 함수가 `&c`를 리턴하므로 한 문장의 모든 호출이 같은 객체 `c`에 적용된다. |

---

<br>

## 점검 문제

1. **생성자 호출 횟수:** Q1의 36행 `Complex data[MAX];`에서 기본 생성자는 몇 번, 어떤 순서로 호출되는가?

   <details>
   <summary>정답</summary>

   `MAX`가 2이므로 두 번 호출되며, `data[0]`, `data[1]`의 순서이다. 초기값이 없으므로 매개변수 2개 생성자는 호출되지 않는다.

   </details>

2. **다른 입력:** Q3에 `3`을 입력하면 출력의 둘째 줄과 셋째 줄은 무엇인가?

   <details>
   <summary>정답</summary>

   둘째 줄은 입력과 관계없이 `1 3 5 0 `이고, 셋째 줄은 원소 3개에 0, 3, 6이 저장되므로 `0 3 6 `이다. 첫 줄은 `Input size: `와 입력한 `3`이다.

   </details>

3. **연결을 늘린 경우:** Q4의 31행을 `c.increment()->increment()->increment()->display();`로 바꾸면 출력되는 두 줄은 무엇인가?

   <details>
   <summary>정답</summary>

   `count`가 0에서 1, 2, 3으로 늘어난 뒤 `Count: 3`이 출력되고, 32행이 3에서 2로 줄인 뒤 `Count: 2`를 출력한다.

   </details>

4. **포인터 대신 객체 이름:** Q2의 33행을 `person2.setAge(31);`로 바꾸면 34행의 출력은 달라지는가?

   <details>
   <summary>정답</summary>

   달라지지 않는다. `personPtr`가 `person2`를 가리키므로 `(*personPtr).setAge(31)`과 `person2.setAge(31)`은 같은 객체의 같은 멤버 함수를 호출한다. 34행은 그대로 31살의 자기소개를 출력한다.

   </details>
