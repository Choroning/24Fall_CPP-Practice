# HW8 코드 총정리 — 프렌드 함수와 연산자 중복

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

> **학습 목표:**
> 1. 세 풀이의 클래스 선언부, 프렌드 선언, 연산자 함수의 정의, `main()`을 줄 단위로 읽고 각 부분의 역할을 설명할 수 있다
> 2. 연산자 식이 어떤 함수 호출로 바뀌는지 쓰고, 호출마다 객체의 상태를 추적할 수 있다
> 3. 주석으로 남긴 프렌드 함수 버전이 멤버 함수 버전과 어떻게 대응하는지 줄 단위로 비교할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: 정수와 Circle의 덧셈](#2-q1cpp-정수와-circle의-덧셈)
- [3. Q2.cpp: 전위와 후위 ++](#3-q2cpp-전위와-후위-)
- [4. Q3.cpp: Matrix의 >>와 <<](#4-q3cpp-matrix의-와-)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

세 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스 선언, 연산자 함수의 정의, `main()`을 차례로 둔다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW8 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 클래스 | 중복한 연산자 | 구현 방식 |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Circle` | `int + Circle` | 클래스 밖에서 정의한 프렌드 함수 |
| [`Q2.cpp`](Solution/Q2.cpp) | `Circle` | 전위 `++`, 후위 `++` | 클래스 밖에서 정의한 프렌드 함수 |
| [`Q3.cpp`](Solution/Q3.cpp) | `Matrix` | `>>`, `<<` | 멤버 함수 (프렌드 함수 버전은 주석) |

*표 1. HW8 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: 정수와 Circle의 덧셈

```cpp
/**
 * @file    A8Q1.cpp
 * @brief   Q1. 프렌드 함수로 중복한 정수 + Circle 연산 (13주차 실습 1)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle (int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
    friend Circle operator+ (int op1, Circle op2);
};

Circle operator+ (int op1, Circle op2) {
    Circle tmp;
    tmp.radius = op1 + op2.radius;
    return tmp;
}

int main() {
    Circle a(5), b(4);
    b = 1 + a;
    a.show();
    b.show();
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 12 | `int radius;` | `private` 멤버인 반지름이다 |
| 14~16 | `Circle (int radius = 0)` | 디폴트 매개변수를 갖는 생성자로, `Circle tmp;`처럼 인자 없이도 만들 수 있다 |
| 17~19 | `show()` | `radius = 5 인 원` 형식으로 출력한다 |
| 20 | `friend Circle operator+ (int op1, Circle op2);` | 외부 함수 `operator+`에 `private` 멤버의 접근을 허락한다 |
| 23~27 | `Circle operator+ (int op1, Circle op2)` | `tmp.radius`에 `op1 + op2.radius`를 저장하고 `tmp`를 값으로 리턴한다 |
| 30 | `Circle a(5), b(4);` | 반지름 5와 4의 원을 만든다 |
| 31 | `b = 1 + a;` | `operator+(1, a)`가 호출되고, 리턴된 임시 객체가 `b`에 대입된다 |
| 32~33 | 두 `show()` | `a`와 `b`를 출력한다 |

*표 2. Q1.cpp의 줄 단위 설명*

`b = 1 + a;`가 실행될 때 `a`가 `op2`로 복사되고(반지름 5), `tmp`는 기본 생성자로 반지름 0에서 시작하여 `1 + 5 = 6`이 된다. `tmp`가 리턴되어 `b`에 대입되므로 `b`의 반지름은 4에서 6이 되고, `a`는 5 그대로이다.

---

<br>

## 3. Q2.cpp: 전위와 후위 ++

```cpp
/**
 * @file    A8Q2.cpp
 * @brief   Q2. 프렌드 함수로 중복한 전위, 후위 ++ 연산자 (13주차 실습 2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle (int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
    friend Circle operator++(Circle& op);
    friend Circle operator++(Circle& op, int notUsed);
};

Circle operator++(Circle& op) {
    op.radius++;
    return op;
}
Circle operator++(Circle& op, int notUsed) {
    Circle tmp = op;
    op.radius++;
    return tmp;
}

int main() {
    Circle a(5), b(4);
    ++a;        // 반지름을 1 증가시킴
    b = a++;    // 반지름을 1 증가시킴
    a.show();
    b.show();
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 20 | `friend Circle operator++(Circle& op);` | 전위 `++`의 프렌드 선언이다 |
| 21 | `friend Circle operator++(Circle& op, int notUsed);` | 후위 `++`의 프렌드 선언으로, `int` 매개변수가 전위와 구분하는 표시이다 |
| 24~27 | 전위 `operator++` | `op.radius`를 늘리고 증가한 뒤의 `op`를 복사하여 리턴한다 |
| 28~32 | 후위 `operator++` | `tmp`에 증가하기 전의 `op`를 복사해 두고, `op.radius`를 늘린 뒤 `tmp`를 리턴한다 |
| 35 | `Circle a(5), b(4);` | 반지름 5와 4의 원을 만든다 |
| 36 | `++a;` | `operator++(a)`가 호출되어 `a`가 6이 되며, 리턴값은 쓰이지 않는다 |
| 37 | `b = a++;` | `operator++(a, 0)`이 호출되어 `a`가 7이 되고, 리턴된 6이 `b`에 대입된다 |
| 38~39 | 두 `show()` | `a`와 `b`를 출력한다 |

*표 3. Q2.cpp의 줄 단위 설명*

| 실행한 문장 | 호출 | 리턴값의 반지름 | 실행 뒤 `a` | 실행 뒤 `b` |
|:-----|:-----|-----:|-----:|-----:|
| `Circle a(5), b(4);` | 생성자 두 번 | | 5 | 4 |
| `++a;` | `operator++(a)` | 6 | 6 | 4 |
| `b = a++;` | `operator++(a, 0)` | 6 | 7 | 6 |

*표 4. Q2의 실행에 따른 두 객체의 반지름*

---

<br>

## 4. Q3.cpp: Matrix의 >>와 <<

```cpp
/**
 * @file    A8Q3.cpp
 * @brief   Q3. Matrix 클래스의 <<, >> 연산자 (13주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-28
 */

#include <iostream>
using namespace std;

class Matrix {
    int ar[4];
public:
    Matrix() { for (int i = 0; i < 4; i++) ar[i] = 0; }
    Matrix(int a1, int a2, int b1, int b2) {
        ar[0] = a1;
        ar[1] = a2;
        ar[2] = b1;
        ar[3] = b2;
    }
    void show() {
        cout << "Matrix = { ";
        for (int i = 0; i < 4; i++) cout << ar[i] << ' ';
        cout << "}" << endl;
    }
    
    // (1) <<, >>를 Matrix의 멤버 함수로 구현
    int* operator >> (int op[]) {
        for (int i = 0; i < 4; i++) {
            op[i] = ar[i];
        }
        return op;
    }
    Matrix& operator << (int op[]) {
        for (int i = 0; i < 4; i++) {
            ar[i] = op[i];
        }
        return *this;
    }
    
    /* (2) <<, >>를 Matrix의 프렌드 함수로 구현
    friend int* operator>>(Matrix& op1, int op2[]) {
        for (int i = 0; i < 4; i++)
            op2[i] = op1.ar[i];
        return op2;
    }
        
    friend Matrix& operator<<(Matrix& op1, int op2[]) {
        for (int i = 0; i < 4; i++)
            op1.ar[i] = op2[i];
        return op1;
    }*/
};

int main() {
    Matrix a(4, 3, 2, 1), b;
    int x[4], y[4] = { 1, 2, 3, 4 };    // 2차원 행렬의 4개의 원소 값
    a >> x;     // a의 각 원소를 배열 x에 복사. x[]는 { 4, 3, 2, 1 }
    b << y;     // 배열 y의 원소 값을 b의 각 원소에 설정
    
    for (int i = 0; i < 4; i++)
        cout << x[i] << ' ';    // x[] 출력
    cout << endl;
    b.show();
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 12 | `int ar[4];` | 2×2 행렬의 네 원소를 저장하는 `private` 배열이다 |
| 14~20 | 두 생성자 | 네 원소를 0으로, 또는 받은 네 값으로 초기화한다 |
| 21~25 | `show()` | `Matrix = { ... }` 형식으로 원소를 출력한다 |
| 27~33 | 멤버 `operator >>` | `ar`의 원소를 `op`에 복사하고 `op`를 리턴한다 |
| 34~39 | 멤버 `operator <<` | `op`의 원소를 `ar`에 복사하고 `*this`의 참조를 리턴한다 |
| 41~52 | 블록 주석 안의 프렌드 버전 | `Matrix& op1`을 첫 번째 매개변수로 받아 같은 일을 한다 |
| 56~57 | 객체와 배열의 선언 | `a`는 (4, 3, 2, 1), `b`는 0 행렬, `y`는 {1, 2, 3, 4}이며 `x`는 아직 값이 없다 |
| 58 | `a >> x;` | `a.operator>>(x)`가 `x`를 {4, 3, 2, 1}로 채운다 |
| 59 | `b << y;` | `b.operator<<(y)`가 `b`의 원소를 {1, 2, 3, 4}로 바꾼다 |
| 61~64 | 출력 | `x`의 원소와 `b`를 출력한다 |

*표 5. Q3.cpp의 줄 단위 설명*

| 멤버 함수 버전 | 프렌드 함수 버전 | 다른 점 |
|:-----|:-----|:-----|
| `int* operator >> (int op[])` | `friend int* operator>>(Matrix& op1, int op2[])` | 왼쪽 피연산자가 `this`가 아니라 첫 번째 매개변수이다 |
| `op[i] = ar[i];` | `op2[i] = op1.ar[i];` | 멤버를 매개변수를 통해 가리킨다 |
| `Matrix& operator << (int op[])` | `friend Matrix& operator<<(Matrix& op1, int op2[])` | 같은 이유로 매개변수가 하나 늘어난다 |
| `return *this;` | `return op1;` | 리턴하는 객체는 같고 가리키는 방법만 다르다 |

*표 6. Q3의 멤버 함수 버전과 프렌드 함수 버전의 대응*

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 식과 호출 | `1 + a`는 `operator+(1, a)`, `++a`는 `operator++(a)`, `a++`는 `operator++(a, 0)`, `a >> x`는 `a.operator>>(x)`로 바뀐다. |
| Q1의 추적 | `b = 1 + a;` 뒤에 `a`는 5, `b`는 6이다. |
| Q2의 추적 | `a`는 5, 6, 7로 바뀌고, `b`는 후위 `++`가 리턴한 증가하기 전의 값 6을 받는다. |
| Q3의 추적 | `x`는 `a`의 원소 {4, 3, 2, 1}을 받고, `b`는 `y`의 원소 {1, 2, 3, 4}로 바뀐다. |
| 두 버전의 대응 | 프렌드 함수 버전은 `this` 대신 첫 번째 매개변수로 왼쪽 피연산자를 받고, 하는 일과 리턴값은 같다. |

---

<br>

## 점검 문제

1. **임시 객체의 초기값:** Q1의 24행 `Circle tmp;`가 컴파일되는 이유는 무엇인가?

   <details>
   <summary>정답</summary>

   14행의 생성자가 `int radius = 0`이라는 디폴트 매개변수를 가지므로 인자 없이 호출할 수 있어 기본 생성자의 역할을 한다. 디폴트 값이 없으면 인자 없는 생성자가 없으므로 `Circle tmp;`는 컴파일 오류가 된다.

   </details>

2. **리턴값의 사용:** Q2의 37행을 `b = ++a;`로 바꾸면 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   36행 뒤에 `a`는 6이고, `b = ++a;`에서 `a`가 7이 된 뒤 증가한 값 7이 `b`에 대입된다. 출력은 `radius = 7 인 원`이 두 번이다.

   </details>

3. **연쇄:** Q3의 59행을 `b << y << x;`로 바꾸면 64행은 무엇을 출력하는가?

   <details>
   <summary>정답</summary>

   `b << y`가 `b`의 참조를 리턴하므로 이어서 `b << x`가 실행되어 `b`의 원소가 `x`의 값 {4, 3, 2, 1}로 바뀐다. 64행은 `Matrix = { 4 3 2 1 }`을 출력한다.

   </details>

4. **리턴값을 배열로 쓰기:** Q3에서 `cout << (a >> x)[2];`를 실행하면 무엇이 출력되는가?

   <details>
   <summary>정답</summary>

   `a >> x`는 `x`에 {4, 3, 2, 1}을 복사한 뒤 `x`의 주소를 리턴하므로 `(a >> x)[2]`는 `x[2]`이고 2가 출력된다.

   </details>
