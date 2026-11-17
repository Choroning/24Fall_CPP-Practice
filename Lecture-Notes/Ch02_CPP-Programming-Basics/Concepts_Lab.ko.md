# 2장 실습 — C++ 기초 리뷰

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **최종 수정일:** 2026-11-18

> **학습 목표:**
> 1. 리터럴과 변수, 대입 연산자의 왼쪽과 오른쪽을 구분하고 `bool` 값의 입출력을 설명할 수 있다
> 2. 산술, 관계, 논리, 복합 대입, 증감, 조건 연산자의 결과를 계산할 수 있다
> 3. `if`, `else if`, `switch`와 `for`, `while`, `do-while`, `break`, `continue`로 흐름을 제어할 수 있다
> 4. 배열의 인덱스 범위를 지키고 포인터로 변수와 배열 원소에 접근할 수 있다
> 5. 매개변수와 리턴값을 가진 함수를 정의하고 배열과 포인터를 인자로 넘길 수 있다

---

## 목차

- [1. 개요](#1-개요)
- [2. 데이터](#2-데이터)
  - [2.1 리터럴, 변수, 대입](#21-리터럴-변수-대입)
  - [2.2 자료형과 bool](#22-자료형과-bool)
- [3. 연산자](#3-연산자)
  - [3.1 산술, 관계, 논리 연산자](#31-산술-관계-논리-연산자)
  - [3.2 복합 대입 연산자](#32-복합-대입-연산자)
  - [3.3 증감 연산자](#33-증감-연산자)
  - [3.4 조건 연산자](#34-조건-연산자)
- [4. 조건문](#4-조건문)
- [5. 반복문](#5-반복문)
  - [5.1 for, while, do-while](#51-for-while-do-while)
  - [5.2 break와 continue](#52-break와-continue)
- [6. 배열](#6-배열)
- [7. 포인터](#7-포인터)
- [8. 함수](#8-함수)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 개요

> **참고:** C++ 프로그램의 기본 요소, `cout`과 `cin`을 이용한 콘솔 입출력, 이름 공간과 `std::`는 [2장 강의 노트](Concepts_Lecture.ko.md)에서 다룬다. 교재는 C 언어의 기본 문법(데이터, 연산자, 제어문, 배열, 포인터, 함수)을 이미 안다고 보고 설명하므로, 이 실습은 그 내용을 C++ 코드로 빠르게 되짚는다.

| 주제 | 위치 |
|:-----|:-----|
| 주석, `main()`, `#include <iostream>`, `cout`과 `<<`, 이름 공간, `cin`과 `>>` | [2장 강의 노트 1\~4절](Concepts_Lecture.ko.md) |
| 리터럴, 변수, 대입, 자료형 | 2절 |
| 연산자 | 3절 |
| 조건문과 반복문 | 4절, 5절 |
| 배열, 포인터, 함수 | 6\~8절 |

*표 1. 실습의 주제와 위치*

모든 코드는 C++17 표준으로 컴파일하여 실행한다(예: `g++ -std=c++17 -Wall`). 이 C++ 기본 문법을 충분히 이해하지 못했다면 C 언어 교재로 따로 보충해야 이후 장을 따라갈 수 있다.

---

<br>

## 2. 데이터

### 2.1 리터럴, 변수, 대입

> **용어 정의:** **리터럴(literal)** 은 `2`, `'a'`, `"Hello"`처럼 코드에 직접 쓴 값으로, 변수에 대입되거나 연산의 피연산자나 함수의 인자로 쓰인다.

> **용어 정의:** **변수(variable)** 는 자료형과 이름(식별자)을 묶은 것으로, 프로그램의 데이터를 담는 하나 이상의 메모리 위치를 나타낸다.

`int val_1 = 2;`에서 `val_1`은 변수, `2`는 리터럴이다. `=`의 왼쪽은 값을 저장할 메모리 위치를 나타내므로 **L-value**(left-hand side), 오른쪽은 값을 나타내므로 **R-value**(right-hand side)라 한다.

- 수학에서 `=`는 왼쪽과 오른쪽이 같다는 뜻의 등호이다.
- 프로그래밍 언어에서 `=`는 오른쪽의 값을 왼쪽의 메모리 위치에 저장하는 **대입 연산자(assignment operator)** 이다. 항상 오른쪽을 먼저 계산하고 그 결과를 왼쪽에 저장하므로 `sum = sum + 1;`은 "sum의 현재 값에 1을 더해 다시 sum에 저장한다"는 뜻이다.

```cpp
int val_1 = 2;      // 선언과 동시에 값 대입: 초기화
int val_2 = 3;
int sum;            // 선언만: 메모리만 확보하고 초기화하지 않음
sum = val_1 + val_2;
```

```text
         메모리
       ┌─────────┐
val_1  │    2    │
       ├─────────┤
val_2  │    3    │
       ├─────────┤
sum    │ ??? → 5 │   선언 직후에는 쓰레기 값
       └─────────┘
```

*그림 1. 선언과 초기화에 따른 변수의 메모리 상태*

초기화하지 않은 지역 변수 `sum`에는 그 메모리에 이전에 남아 있던 쓰레기 값(garbage)이 들어 있으며, 대입하기 전에 읽으면 정의되지 않은 동작이다.

### 2.2 자료형과 bool

**자료형(data type)** 은 변수와 함수 매개변수가 차지할 메모리의 크기, 값을 다루는 방법, 동작을 정한다. C++의 기본 자료형에는 `void`, `char`, `short`, `int`, `long`, `long long`, `float`, `double`, `bool`이 있다. C++은 `bool` 타입과 상수 `true`, `false`를 키워드로 제공한다.

```cpp
#include <iostream>
using namespace std;

int main() {
    bool test = true;
    cout << "1. bool: " << test << ", " << true << endl;
    test = false;
    cout << "2. bool: " << test << ", " << false << endl;
    if (true)
        cout << "TRUE" << endl;
    if (false)
        cout << "FALSE" << endl;
    if (-1)                             // 0이 아닌 값은 참
        cout << "TRUE::-1" << endl;
}
```

```text
1. bool: 1, 1
2. bool: 0, 0
TRUE
TRUE::-1
```

- `cout`은 `bool` 값을 기본적으로 `1`(참), `0`(거짓)으로 출력한다. `cout << boolalpha`를 먼저 보내면 `true`, `false`로 출력한다.
- 조건식에서는 0이 거짓, 0이 아닌 모든 값이 참이므로 `if (-1)`도 참이다.

---

<br>

## 3. 연산자

**연산자(operator)** 는 특정한 수학적 또는 논리적 동작을 나타내는 기호이다. 예를 들어 `*`는 곱셈을 나타내는 산술 연산자이고, `&&`는 논리곱을 나타내는 논리 연산자이다.

### 3.1 산술, 관계, 논리 연산자

| 종류 | 연산자 | 뜻 |
|:-----|:-----|:-----|
| 산술 | `+`, `-`, `*`, `/` | 덧셈, 뺄셈, 곱셈, 나눗셈 (정수끼리의 `/`는 몫) |
| 산술 | `%` | 왼쪽을 오른쪽으로 나눈 나머지 |
| 관계 | `==`, `!=` | 두 값이 같으면 참, 다르면 참 |
| 관계 | `>`, `<`, `>=`, `<=` | 크기 비교가 성립하면 참 |
| 논리 | `&&` | 두 피연산자가 모두 참일 때만 참 |
| 논리 | `\|\|` | 피연산자 중 하나라도 참이면 참 |
| 논리 | `!` | 피연산자의 참과 거짓을 뒤집음 |

*표 2. 산술, 관계, 논리 연산자*

```cpp
#include <iostream>
using namespace std;

int main() {
    int sum, apple = 7, mango = 3;

    sum = apple + mango;
    cout << "sum: " << sum << endl;
    cout << "+ : " << apple + mango << endl;
    cout << "/ : " << apple / mango << endl;            // 정수 나눗셈: 몫
    cout << "% : " << apple % mango << endl;            // 나머지
    cout << "--------------" << endl;

    cout << "== : " << (apple == mango) << endl;
    cout << "!= : " << (apple != mango) << endl;
    cout << "<= : " << (apple <= mango) << endl;
    cout << "&& : " << ((apple != mango) && (apple <= mango)) << endl;
    cout << "|| : " << ((apple == mango) || (apple >= mango)) << endl;
    cout << " ! : " << !(true) << endl;
}
```

```text
sum: 10
+ : 10
/ : 2
% : 1
--------------
== : 0
!= : 1
<= : 0
&& : 0
|| : 1
 ! : 0
```

- 7 / 3은 정수 나눗셈이므로 몫 2, 7 % 3은 나머지 1이다.
- 관계식과 논리식은 괄호로 감싸 출력한다. `cout << apple == mango`처럼 쓰면 `<<`가 `==`보다 우선순위가 높아 의도와 다르게 해석된다.
- `(apple != mango) && (apple <= mango)`는 앞이 참(1)이지만 뒤가 거짓(0)이므로 거짓이다.

### 3.2 복합 대입 연산자

| 연산자 | 예 | 뜻 |
|:-----|:-----|:-----|
| `=` | `a = b` | `b`의 값을 `a`에 대입 |
| `+=` | `a += b` | `a = a + b` |
| `-=` | `a -= b` | `a = a - b` |
| `*=` | `a *= b` | `a = a * b` |
| `/=` | `a /= b` | `a = a / b` |
| `%=` | `a %= b` | `a = a % b` |

*표 3. 대입 연산자와 복합 대입 연산자*

### 3.3 증감 연산자

`++`와 `--`는 피연산자를 1 늘리거나 줄인다. 연산자가 앞에 붙는 전위형(`++a`)은 값을 먼저 바꾼 뒤 그 값을 쓰고, 뒤에 붙는 후위형(`a++`)은 현재 값을 쓴 뒤 바꾼다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 2, b = 3;

    a++;
    ++b;
    cout << "[1] a: " << a << ", b: " << b << endl;

    b = ++a;                            // a를 먼저 증가시킨 뒤 대입
    cout << "[2] a: " << a << ", b: " << b << endl;

    b = a++;                            // a의 현재 값을 대입한 뒤 증가
    cout << "[3] a: " << a << ", b: " << b << endl;

    b = ++a + 2;
    cout << "[4] a: " << a << ", b: " << b << endl;

    b = a++ + 2;
    cout << "[5] a: " << a << ", b: " << b << endl;
}
```

```text
[1] a: 3, b: 4
[2] a: 4, b: 4
[3] a: 5, b: 4
[4] a: 6, b: 8
[5] a: 7, b: 8
```

- [1] 단독으로 쓴 `a++`와 `++b`는 차이가 없고 둘 다 1씩 는다.
- [2] `b = ++a`는 `a`를 4로 만든 뒤 4를 `b`에 대입한다.
- [3] `b = a++`는 `a`의 현재 값 4를 `b`에 대입한 뒤 `a`를 5로 만든다.
- [4] `++a + 2`는 6 + 2 = 8, [5] `a++ + 2`는 6 + 2 = 8을 대입한 뒤 `a`를 7로 만든다.

### 3.4 조건 연산자

조건 연산자 `조건 ? 식1 : 식2`는 조건이 참이면 식1의 값, 거짓이면 식2의 값이 된다. 피연산자가 셋인 유일한 연산자이므로 삼항 연산자라고도 한다.

```cpp
int number = -4;
string result;
result = (number > 0) ? "Positive Number!" : "Negative Number!";   // "Negative Number!"
```

> **참고:** 이 식은 `number`가 0일 때도 `"Negative Number!"`를 고른다. 0은 양수도 음수도 아니므로, 세 경우를 구별하려면 `(number > 0) ? "Positive" : (number < 0) ? "Negative" : "Zero"`처럼 조건 연산자를 겹치거나 `if`문을 쓴다.

---

<br>

## 4. 조건문

C++은 수학의 관계 조건(`a < b`, `a == b` 등)을 써서 상황마다 다른 동작을 하게 하는 조건문을 제공한다.

- `if`는 조건이 참일 때 실행할 블록을 정한다.
- `else`는 같은 조건이 거짓일 때 실행할 블록을 정한다.
- `else if`는 앞의 조건이 거짓일 때 검사할 새 조건을 정한다.
- `switch`는 한 값에 따라 여러 블록 중 하나를 고른다.

다음은 점수를 입력받아 학점을 출력하는 코드를 `switch`로 쓴 것이다. 같은 일을 `if (score >= 90) ... else if (score >= 80) ... else ...`로도 쓸 수 있다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int score;
    cout << "Enter your test score: ";
    cin >> score;

    if (score >= 0 && score <= 100) {
        switch (score / 10) {           // 십의 자리로 분기
        case 10:
        case 9:                         // 10과 9는 같은 문장을 실행
            cout << "Grade: A \n";
            break;
        case 8:
            cout << "Grade: B \n";
            break;
        default:
            cout << "Grade: F \n";
            break;
        }
    }
    else
        cout << "[Input error] Exit the program!" << endl;
}
```

| 입력 | 출력 |
|:-----|:-----|
| 100 | `Grade: A` |
| 85 | `Grade: B` |
| 42 | `Grade: F` |
| 120 | `[Input error] Exit the program!` |

*표 4. 입력한 점수에 따른 학점 출력*

- `score / 10`은 정수 나눗셈이므로 85는 8, 100은 10이 된다. `switch`는 이 값과 같은 `case`로 이동한다.
- `case 10:` 다음에 문장도 `break`도 없으므로 실행은 아래의 `case 9:`로 이어진다. 이렇게 `break`가 없으면 다음 `case`로 넘어가는 성질을 이용해 여러 값에 같은 문장을 실행한다.
- 각 `case`의 끝에 `break`를 빠뜨리면 다음 `case`의 문장까지 실행되므로, 의도한 경우가 아니면 반드시 `break`를 둔다.

---

<br>

## 5. 반복문

### 5.1 for, while, do-while

반복문은 종료 조건이 거짓(0)이 되거나 `break`로 빠져나올 때까지 문장을 0번 이상 되풀이한다. 1부터 4까지 출력하고 합하는 일을 반복문 없이 쓰면 `sum = sum + 1;`, `sum = sum + 2;`처럼 같은 코드를 늘어놓아야 하고, 범위가 1부터 10,000이 되면 감당할 수 없다. 값이 변하는 변수 `i`와 반복문을 쓰면 다음 세 가지만 정하면 된다.

1. `i`의 시작값(초기식): `i = 1`
2. `i`가 언제까지 변하는지(조건식): `i < 5`이면 계속, 5가 되면 끝
3. `i`의 변화량(증감식): `i++`

| 반복문 | 조건을 검사하는 곳 | 초기식 | 증감식 |
|:-----|:-----|:-----|:-----|
| `while` | 반복의 처음 | 없음 | 없음 |
| `do-while` | 반복의 끝 | 없음 | 없음 |
| `for` | 반복의 처음 | 있음 | 있음 |
| 범위 기반 `for` | 반복의 처음 | 있음 | 있음 |

*표 5. C++의 네 가지 반복문*

`for (①; ②; ③) { 할 일; }`은 `①; while (②) { 할 일; ③; }`과 같다. `do-while`은 조건을 끝에서 검사하므로 몸체가 적어도 한 번 실행된다. 다음 코드는 두 정수 사이의 모든 정수의 합을 `for`와 `do-while`로 각각 구한다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int i, a, b, large, small, sum = 0;
    cout << "Please enter two integers >> ";
    cin >> a >> b;

    if (a > b) { large = a; small = b; }
    else { large = b; small = a; }

    for (i = small; i <= large; i++)    // small부터 large까지의 합
        sum += i;
    cout << small << "에서 " << large << "까지 합은 " << sum << endl;

    sum = 0;
    i = small;
    do {                                // 몸체를 먼저 한 번 실행한 뒤 조건 검사
        sum += i;
        i++;
    } while (i <= large);
    cout << "do-while: " << sum << endl;
}
```

```text
Please enter two integers >> 7 3
3에서 7까지 합은 25
do-while: 25
```

### 5.2 break와 continue

- **break:** 가장 가까운 반복문(또는 `switch`)을 즉시 빠져나간다. `while (true)`처럼 조건이 항상 참인 무한 반복은 `break`로만 끝난다.
- **continue:** 반복의 나머지 부분을 건너뛰고 다음 반복으로 넘어간다. `for`에서는 증감식으로, `while`에서는 조건식으로, `do-while`에서는 끝의 조건식으로 이동한다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int i = 1;
    while (true) {
        i++;
        if (i % 2 == 1)
            continue;                   // 홀수이면 아래를 건너뛰고 조건으로
        cout << i << endl;
        if (i == 6)
            break;                      // 반복을 끝냄
    }
}
```

```text
2
4
6
```

> **주의:** `while`에서 `continue`가 증감 문장보다 앞에 있으면 증감이 실행되지 않아 무한 반복이 될 수 있다. 위 코드는 `i++`를 반복의 맨 앞에 두어 이를 피했다. 같은 상황에서 `for`는 `continue` 뒤에도 증감식을 실행한다.

---

<br>

## 6. 배열

**배열(array)** 은 같은 타입의 객체를 메모리의 연속된 영역에 차례로 둔 것이다. C 스타일 배열은 C++에서도 그대로 쓰이지만 많은 버그의 원인이므로, 현대 C++에서는 `std::vector`나 `std::array`를 권장한다(10장).

```cpp
int n[10];                              // 정수 10개 크기의 배열
double d[] = { 0.1, 0.2, 0.5, 3.9 };    // 크기는 초깃값의 개수인 4
n[10] = 20;                             // 오류: 인덱스는 0~9만 가능
d[-1] = 9.9;                            // 오류: 음수 인덱스는 쓸 수 없음
int m[2][5];                            // 2행 5열의 2차원 배열
m[2][0] = 5;                            // 오류: 행 인덱스는 0~1만 가능
```

크기가 N인 배열의 인덱스는 0부터 N - 1까지이다. 범위를 벗어난 인덱스는 컴파일 오류가 아니라 실행 중에 다른 메모리를 읽고 쓰는 정의되지 않은 동작이므로 더욱 주의해야 한다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int i, x[3];
    double y[] = { 1.1, 2.2, -3.4, 5.7 };   // 크기는 초깃값의 개수인 4

    for (i = 0; i < 3; i++) {
        cout << "[" << i + 1 << "] data input: ";
        cin >> x[i];
    }
    for (i = 0; i < 3; i++)
        cout << x[i] << endl;
    cout << "========" << endl;

    y[0] = y[0] + 8;
    for (i = 0; i < 4; i++)
        cout << y[i] << endl;
}
```

```text
[1] data input: 3
[2] data input: -6
[3] data input: 2
3
-6
2
========
9.1
2.2
-3.4
5.7
```

---

<br>

## 7. 포인터

> **용어 정의:** **포인터(pointer)** 는 객체의 메모리 주소를 저장하는 변수이다.

포인터는 힙에 새 객체를 할당하거나, 함수에 함수를 넘기거나, 배열 같은 자료 구조의 원소를 차례로 훑는 데 쓰인다. 그러나 날 포인터(raw pointer)는 심각한 오류의 원인이 되므로, 현대 C++은 객체 할당에는 스마트 포인터, 자료 구조의 순회에는 반복자, 함수 전달에는 람다 식을 제공한다(4장, 10장).

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 3, b;
    int* p = &a;                        // 포인터 선언과 초기화

    cout << "[a] " << a << ", [*p] " << *p << endl;
    cout << boolalpha << "p == &a: " << (p == &a) << endl;

    *p = *p + 2;                        // p가 가리키는 a를 바꿈
    b = *p;
    cout << "[a] " << a << ", [b] " << b << endl;

    int arr[3] = { 1, -3 };             // 나머지 원소는 0
    int* q = arr;                       // 배열 이름은 첫 원소의 주소
    cout << "arr == &arr[0]: " << (arr == &arr[0]) << endl;
    for (int i = 0; i < 3; i++)
        cout << q[i] << ' ';            // 첨자로 접근
    cout << endl;
    for (int i = 0; i < 3; i++) {
        cout << *q << ' ';              // 포인터를 옮기며 접근
        q++;
    }
    cout << endl;
}
```

```text
[a] 3, [*p] 3
p == &a: true
[a] 5, [b] 5
arr == &arr[0]: true
1 -3 0 
1 -3 0 
```

- `&a`는 `a`의 주소, `*p`는 `p`가 가리키는 곳의 값이다. `*p = *p + 2;`는 `a`를 5로 바꾼다.
- 포인터를 선언하면서 가리킬 대상이 없을 때는 `int* p = nullptr;`로 초기화하여 임의의 주소를 담지 않게 한다.
- 초깃값이 원소 수보다 적으면 나머지 원소는 0으로 초기화되므로 `arr[2]`는 0이다.

---

<br>

## 8. 함수

**함수(function)** 는 어떤 작업을 하는 코드 블록이다. 호출한 쪽에서 인자(argument)를 넘길 수 있도록 매개변수(parameter)를 정의할 수 있고, 결과를 리턴값으로 돌려줄 수 있다. 함수 정의의 매개변수 타입과 호출할 때 넘기는 인자의 타입은 서로 맞아야 한다. 수학의 함수 y = x + 3을 C++로 옮기려면 함수의 이름(`getPlus`), 넘겨받을 정보(`int x`), 할 일(`y = x + 3`), 돌려줄 값(`return y;`)과 그 타입(`int`)을 정하면 된다.

```cpp
#include <iostream>
using namespace std;

int getQuad(int a, int x);                      // 함수 원형
double getSum(double* ob, int size);
bool equalArray(int* p, int* q, int size);

int main() {
    int a = 1, x = 3, result;
    result = getQuad(a, x);                     // 인자 2개, 리턴값을 result에 저장
    cout << "1. result: " << result << '\n';
    cout << "2. result: " << getQuad(2, 4) << endl;

    double d[4] = { 1.1, -3.2, 5.6 };
    cout << getSum(d, 4) << endl;

    int s[] = { 1, 2, 3, 4, 5 };
    int t[] = { 1, 2, 3, 4, 5 };
    if (equalArray(s, t, 5))
        cout << "arrays equal" << '\n';
    else
        cout << "arrays not equal" << '\n';
}

int getQuad(int a, int x) {                     // y = a^2 + x - 3
    int y;
    y = a * a + x - 3;
    return y;
}

double getSum(double* ob, int size) {           // 배열 원소의 합
    double sum = 0;
    for (int i = 0; i < size; i++)
        sum = sum + ob[i];
    return sum;
}

bool equalArray(int* p, int* q, int size) {     // 두 배열의 원소를 차례로 비교
    for (int i = 0; i < size; i++) {
        if (*p != *q)
            return false;
        p++;                                    // 다음 원소를 가리킴
        q++;
    }
    return true;
}
```

```text
1. result: 1
2. result: 5
3.5
arrays equal
```

- **함수 원형:** `main()`보다 뒤에 정의한 함수를 `main()`에서 호출하려면 앞에 원형(선언)을 두어 컴파일러에 이름과 매개변수, 리턴 타입을 알린다.
- **getQuad:** a = 1, x = 3이면 1 + 3 - 3 = 1, a = 2, x = 4이면 4 + 4 - 3 = 5이다.
- **배열 인자:** 배열을 넘기면 첫 원소의 주소가 전달되므로 매개변수는 `double* ob`나 `double ob[]`로 쓰며, 함수는 배열의 크기를 알 수 없어 `size`를 따로 받는다. `d`의 네 번째 원소는 0이므로 합은 1.1 - 3.2 + 5.6 = 3.5이다.
- **bool 리턴:** `equalArray()`는 다른 원소를 만나는 즉시 `false`를 리턴하고, 끝까지 같으면 `true`를 리턴한다. 리턴값이 참과 거짓이므로 `if`의 조건에 바로 쓸 수 있다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 리터럴과 변수 | 리터럴은 코드에 직접 쓴 값이고, 변수는 자료형과 이름으로 메모리 위치를 나타내며, 초기화하지 않은 지역 변수는 쓰레기 값을 가진다. |
| 대입 | `=`는 오른쪽(R-value)을 먼저 계산하여 왼쪽(L-value)의 메모리에 저장하는 연산자이다. |
| `bool` | `true`와 `false`는 `cout`에서 1과 0으로 출력되며, 조건식에서는 0이 아닌 모든 값이 참이다. |
| 연산자 | 정수끼리의 `/`는 몫, `%`는 나머지이며, 전위 증감은 바꾼 뒤 쓰고 후위 증감은 쓴 뒤 바꾼다. |
| 조건문 | `if`, `else if`, `else`와 `switch`로 분기하며, `switch`의 `case`는 `break`가 없으면 다음 `case`로 이어진다. |
| 반복문 | `while`과 `for`는 처음에, `do-while`은 끝에서 조건을 검사하며, `break`는 반복을 끝내고 `continue`는 다음 반복으로 넘어간다. |
| 배열 | 크기 N인 배열의 인덱스는 0\~N - 1이며, 범위를 벗어난 접근은 정의되지 않은 동작이다. |
| 포인터 | 주소를 저장하는 변수로, `&`로 주소를 얻고 `*`로 가리키는 값에 접근하며, 배열 이름은 첫 원소의 주소이다. |
| 함수 | 원형으로 미리 선언하고, 배열은 첫 원소의 주소와 크기를 함께 넘긴다. |

---

<br>

## 점검 문제

1. `int a = 5, b;` 다음에 `b = a++ * 2;`와 `b = ++a * 2;`를 차례로 실행하면 각 문장 뒤의 `a`와 `b`는 얼마인가?

   <details>
   <summary>정답</summary>

   `b = a++ * 2;`는 `a`의 현재 값 5에 2를 곱한 10을 `b`에 대입한 뒤 `a`를 6으로 만든다. 이어서 `b = ++a * 2;`는 `a`를 7로 만든 뒤 7 × 2 = 14를 `b`에 대입한다. 따라서 첫 문장 뒤에는 a = 6, b = 10, 둘째 문장 뒤에는 a = 7, b = 14이다.

   </details>

2. 4절의 `switch`에서 `case 8:`의 `break;`를 지우고 85를 입력하면 무엇이 출력되는가?

   <details>
   <summary>정답</summary>

   `Grade: B`와 `Grade: F`가 차례로 출력된다. `score / 10`은 8이어서 `case 8:`의 문장을 실행한 뒤, `break`가 없으므로 다음 레이블인 `default:`의 문장까지 이어서 실행하고 그곳의 `break`에서 `switch`를 빠져나간다.

   </details>

3. 5.1절의 `do-while`을 `while (i <= large) { sum += i; i++; }`로 바꾸면 결과가 달라지는 입력이 있는가?

   <details>
   <summary>정답</summary>

   이 프로그램에서는 없다. `small`은 항상 `large` 이하이므로 처음부터 조건 `i <= large`가 참이고, 두 반복문 모두 몸체를 같은 횟수만큼 실행한다. 두 반복문이 달라지는 것은 처음에 조건이 거짓일 때이다. 그때 `while`은 몸체를 한 번도 실행하지 않지만 `do-while`은 한 번 실행한다.

   </details>

4. 배열 `int x[3] = { 4, 5, 6 };`과 `int* p = x;`가 있을 때 `*(p + 2)`, `p[1]`, `*p + 2`의 값은 각각 무엇인가?

   <details>
   <summary>정답</summary>

   `*(p + 2)`는 `p`에서 두 원소 뒤인 `x[2]`의 값 6이다. `p[1]`은 `*(p + 1)`과 같으므로 `x[1]`의 값 5이다. `*p + 2`는 `*`가 `+`보다 먼저 적용되므로 `x[0]`의 값 4에 2를 더한 6이다.

   </details>
