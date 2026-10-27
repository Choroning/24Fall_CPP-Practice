# HW1 코드 총정리 — 클래스와 객체

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 3

> **학습 목표:**
> 1. 세 풀이의 클래스 선언부, 멤버 함수 정의부, `main()`을 줄 단위로 읽고 각 부분의 역할을 설명할 수 있다
> 2. `main()`의 각 문장이 실행된 뒤 객체의 멤버 변수 값이 어떻게 바뀌는지 추적할 수 있다
> 3. 같은 클래스 안에서 클래스 안의 정의와 클래스 밖의 정의가 섞여 있을 때 각 멤버 함수가 어디에 정의되었는지 찾을 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: Account 클래스](#2-q1cpp-account-클래스)
  - [2.1 전체 코드](#21-전체-코드)
  - [2.2 줄 단위 설명](#22-줄-단위-설명)
  - [2.3 실행 추적](#23-실행-추적)
- [3. Q2.cpp: Random 클래스](#3-q2cpp-random-클래스)
  - [3.1 전체 코드](#31-전체-코드)
  - [3.2 줄 단위 설명](#32-줄-단위-설명)
  - [3.3 실행 추적](#33-실행-추적)
- [4. Q3.cpp: Point 클래스](#4-q3cpp-point-클래스)
  - [4.1 전체 코드](#41-전체-코드)
  - [4.2 줄 단위 설명](#42-줄-단위-설명)
  - [4.3 실행 추적](#43-실행-추적)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

세 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스 선언, 클래스 밖의 멤버 함수 정의, `main()`을 차례로 둔다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW1 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 클래스 | 클래스 밖에서 정의한 멤버 | 입력 |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Account` | 생성자, `getOwner()`, `deposit()`, `withdraw()`, `inquiry()` | 없음 |
| [`Q2.cpp`](Solution/Q2.cpp) | `Random` | 생성자, `next()`, `nextInRange()` | 없음 |
| [`Q3.cpp`](Solution/Q3.cpp) | `Point` | 인자 2개 생성자, `showP()`, `getY()`, `setY()` | `p2`의 x, y 좌표 |

*표 1. HW1 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: Account 클래스

### 2.1 전체 코드

```cpp
/**
 * @file    A1Q1.cpp
 * @brief   Q1. 은행 계좌 Account 클래스 (교재 3장 실습 문제 3번)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
#include <string>
using namespace std;

class Account{
public:
    string name;
    int id;
    int balance;
    
    Account();
    Account(string, int, int);
    string getOwner();
    void deposit(int);
    int withdraw(int);
    int inquiry();
};

Account::Account(string n, int i, int b){
    name = n;
    id = i;
    balance = b;
}

string Account::getOwner(){
    return name;
}

void Account::deposit(int m){
    balance += m;
}

int Account::withdraw(int m){
    balance -= m;
    return m;
}

int Account::inquiry(){
    return balance;
}


int main()
{
    Account a("kitae", 1, 5000);
    a.deposit(50000);
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
    int money = a.withdraw(20000);
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
}
```

### 2.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~10 | `#include`, `using namespace std;` | `cout`, `endl`을 쓰기 위해 `<iostream>`을, `string`을 쓰기 위해 `<string>`을 포함하고 `std::`를 생략한다 |
| 12~13 | `class Account{ public:` | 이후의 멤버를 모두 `public`으로 선언한다 |
| 14~16 | `string name; int id; int balance;` | 계좌 주인의 이름, 계좌 번호, 잔액을 저장하는 멤버 변수이다 |
| 18 | `Account();` | 기본 생성자의 선언이다. 정의가 없으며 이 프로그램은 호출하지 않는다 |
| 19 | `Account(string, int, int);` | 매개변수 3개 생성자의 선언이다. 원형에서는 매개변수 이름을 생략할 수 있다 |
| 20~23 | 멤버 함수 원형 | 리턴 타입과 매개변수 타입만 적은 네 함수의 원형이다 |
| 26~30 | `Account::Account(string n, int i, int b)` | 받은 이름, 번호, 잔액을 멤버 변수에 대입한다. `Account::`가 이 함수가 `Account`의 멤버임을 나타낸다 |
| 32~34 | `Account::getOwner()` | `name`을 리턴한다. `string`을 값으로 리턴하므로 이름의 복사본이 전달된다 |
| 36~38 | `Account::deposit(int m)` | `balance += m;`으로 잔액에 입금액을 더한다 |
| 40~43 | `Account::withdraw(int m)` | 잔액에서 `m`을 빼고 `m`을 리턴한다. 잔액은 검사하지 않는다 |
| 45~47 | `Account::inquiry()` | 현재 `balance`를 리턴한다 |
| 52 | `Account a("kitae", 1, 5000);` | 매개변수 3개 생성자가 호출되어 `a`의 세 멤버가 초기화된다. 문자열 리터럴 `"kitae"`는 `string` 매개변수 `n`으로 변환된다 |
| 53~54 | `deposit()`, 첫 출력 | 50000원을 입금한 뒤 `getOwner()`와 `inquiry()`의 리턴값을 출력한다 |
| 55~56 | `withdraw()`, 둘째 출력 | 20000원을 출금하고 리턴값을 `money`에 저장한 뒤 다시 출력한다. `money`는 이후에 쓰이지 않는다 |

*표 2. Q1.cpp의 줄 단위 설명*

### 2.3 실행 추적

| 실행한 문장 | `name` | `id` | `balance` | 출력 |
|:-----|:-----|-----:|-----:|:-----|
| `Account a("kitae", 1, 5000);` | `"kitae"` | 1 | 5000 | |
| `a.deposit(50000);` | `"kitae"` | 1 | 55000 | |
| 첫 번째 `cout` | `"kitae"` | 1 | 55000 | `kitae의 잔액은 55000` |
| `int money = a.withdraw(20000);` | `"kitae"` | 1 | 35000 | (`money`는 20000) |
| 두 번째 `cout` | `"kitae"` | 1 | 35000 | `kitae의 잔액은 35000` |

*표 3. Q1의 `main()` 실행에 따른 객체 `a`의 상태*

---

<br>

## 3. Q2.cpp: Random 클래스

### 3.1 전체 코드

```cpp
/**
 * @file    A1Q2.cpp
 * @brief   Q2. 랜덤 정수를 발생시키는 Random 클래스 (교재 3장 실습 문제 5번)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Random{
public:
    Random();
    int next();
    int nextInRange(int, int);
};

Random::Random(){
    srand((unsigned)time(0));
}

int Random::next(){
    return rand();
}

int Random::nextInRange(int s, int f){
    return (rand() % (f - s + 1)) + s;
}


int main()
{
    Random r;
    cout << "-- 0에서 " << RAND_MAX << "까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++){
        int n = r.next();
        cout << n << ' ';
    }
    cout << endl << endl << "-- 2에서 " << "4까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++){
        int n = r.nextInRange(2, 4);
        cout << n << ' ';
    }
    cout << endl;
}
```

### 3.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~11 | `#include`, `using namespace std;` | `<cstdlib>`은 `rand()`, `srand()`, `RAND_MAX`를, `<ctime>`은 `time()`을 제공한다 |
| 13~18 | `class Random{ ... };` | 멤버 변수 없이 생성자와 두 멤버 함수만 가진 클래스이다. 난수열의 상태는 객체가 아니라 C 라이브러리 안에 있다 |
| 20~22 | `Random::Random()` | `time(0)`을 `unsigned`로 변환하여 `srand()`에 넘겨 시드를 정한다 |
| 24~26 | `Random::next()` | `rand()`의 리턴값, 즉 0 이상 `RAND_MAX` 이하의 정수를 그대로 리턴한다 |
| 28~30 | `Random::nextInRange(int s, int f)` | `rand() % (f - s + 1)`로 0 이상 `f - s` 이하의 값을 만들고 `s`를 더한다 |
| 35 | `Random r;` | 생성자가 호출되어 시드가 정해진다 |
| 36 | 첫 안내 문구 | `RAND_MAX`의 값을 문구 안에 출력한다 |
| 37~40 | 첫 번째 `for` 문 | `next()`를 10번 호출하여 수마다 뒤에 공백을 붙여 출력한다 |
| 41 | `cout << endl << endl << ...` | 첫 줄을 끝내고 빈 줄을 하나 둔 뒤 두 번째 안내 문구를 출력한다 |
| 42~45 | 두 번째 `for` 문 | `nextInRange(2, 4)`를 10번 호출하여 2, 3, 4 중 하나를 출력한다 |
| 46 | `cout << endl;` | 마지막 줄을 끝낸다 |

*표 4. Q2.cpp의 줄 단위 설명*

### 3.3 실행 추적

`rand()`가 7을 리턴했다고 하면 `nextInRange(2, 4)`는 `7 % 3 + 2 = 1 + 2 = 3`을 리턴한다. `rand()`가 9를 리턴하면 `9 % 3 + 2 = 2`, 11을 리턴하면 `11 % 3 + 2 = 4`이다. `rand()`의 리턴값을 3으로 나눈 나머지가 0, 1, 2이면 각각 2, 3, 4가 출력되므로, 출력되는 값은 언제나 이 세 값 중 하나이다.

---

<br>

## 4. Q3.cpp: Point 클래스

### 4.1 전체 코드

```cpp
/**
 * @file    A1Q3.cpp
 * @brief   Q3. 접근 제어와 get/set 함수를 갖춘 Point 클래스 (4주차 문제 1-3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
using namespace std;

class Point{
private:
    int x, y;
public:
    Point(){
        x = y = 0;
    }
    Point(int ix, int iy);
    
    void showP();
    
    int getX(){
        return x;
    }
    void setX(int ix){
        x = ix;
    }
    
    int getY();
    void setY(int iy);
    
};

Point::Point(int ix, int iy){
    x = ix;
    y = iy;
}

void Point::showP(){
    cout << "x: " << getX() << ", y: " << getY() << endl;
}

int Point::getY(){
    return y;
}

void Point::setY(int iy){
    y = iy;
}

int main()
{
    Point p1, p2;
    int temp;
    
    cout << "Input p2.x: ";
    cin >> temp;
    p2.setX(temp);
    
    cout << "Input p2.y: ";
    cin >> temp;
    p2.setY(temp);
    
    cout << "p1. ";
    p1.showP();
    cout << "p2. ";
    p2.showP();
}
```

### 4.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 12~13 | `private: int x, y;` | 두 좌표를 `private`으로 선언하여 클래스 밖에서 직접 접근하지 못하게 한다 |
| 15~17 | `Point(){ x = y = 0; }` | 클래스 안에서 정의한 기본 생성자로, 두 좌표를 0으로 만든다 |
| 18 | `Point(int ix, int iy);` | 인자 2개 생성자의 선언이며, 정의는 34~37행에 있다 |
| 20 | `void showP();` | 좌표 출력 함수의 선언이며, 정의는 39~41행에 있다 |
| 22~27 | `getX()`, `setX()` | 클래스 안에서 정의한 x의 get/set 함수이다 |
| 29~30 | `getY()`, `setY()`의 선언 | 정의는 43~49행에 있다 |
| 34~37 | `Point::Point(int ix, int iy)` | 받은 값을 `x`, `y`에 대입한다 |
| 39~41 | `Point::showP()` | `getX()`, `getY()`로 좌표를 가져와 `x: ..., y: ...` 형식으로 출력한다 |
| 43~49 | `Point::getY()`, `Point::setY()` | 클래스 밖에서 정의한 y의 get/set 함수이다 |
| 53~54 | `Point p1, p2; int temp;` | 두 점을 기본 생성자로 만들고 입력용 변수를 둔다 |
| 56~58 | x 좌표 입력 | 프롬프트를 출력하고 정수를 읽어 `setX()`로 `p2.x`에 저장한다 |
| 60~62 | y 좌표 입력 | 같은 방법으로 `setY()`로 `p2.y`에 저장한다. `temp`는 다시 쓰인다 |
| 64~67 | 출력 | 점의 이름을 먼저 출력하고 `showP()`가 같은 줄에 좌표를 이어서 출력한다 |

*표 5. Q3.cpp의 줄 단위 설명*

### 4.3 실행 추적

| 실행한 문장 | `p1` | `p2` | `temp` | 출력 |
|:-----|:-----|:-----|-----:|:-----|
| `Point p1, p2;` | (0, 0) | (0, 0) | 미정 | |
| `cin >> temp;` (`-3` 입력) | (0, 0) | (0, 0) | -3 | `Input p2.x: ` |
| `p2.setX(temp);` | (0, 0) | (-3, 0) | -3 | |
| `cin >> temp;` (`4` 입력) | (0, 0) | (-3, 0) | 4 | `Input p2.y: ` |
| `p2.setY(temp);` | (0, 0) | (-3, 4) | 4 | |
| `p1.showP();` | (0, 0) | (-3, 4) | 4 | `p1. x: 0, y: 0` |
| `p2.showP();` | (0, 0) | (-3, 4) | 4 | `p2. x: -3, y: 4` |

*표 6. Q3의 `main()` 실행에 따른 두 점의 상태 (입력 `-3`, `4`)*

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 파일의 구성 | 세 풀이는 헤더 포함, 클래스 선언, 클래스 밖의 멤버 함수 정의, `main()`의 순서로 되어 있다. |
| 원형과 정의 | 원형에서는 매개변수 이름을 생략할 수 있고, 정의에서 `클래스명::`을 붙여 몸체를 쓴다. |
| 객체 상태의 추적 | `Account`의 잔액은 5000, 55000, 35000으로 바뀌고, `Point p2`는 (0, 0), (-3, 0), (-3, 4)로 바뀐다. |
| 난수 범위의 계산 | `nextInRange(2, 4)`는 `rand()`를 3으로 나눈 나머지 0, 1, 2를 각각 2, 3, 4로 옮긴다. |
| 정의 위치 찾기 | `Point`에서 몸체가 클래스 안에 있는 멤버는 기본 생성자와 x의 get/set이고, 나머지는 클래스 밖에 있다. |

---

<br>

## 점검 문제

1. **원형의 매개변수 이름:** Q1의 19행 `Account(string, int, int);`에는 매개변수 이름이 없다. 26행의 정의에서도 이름을 생략할 수 있는가?

   <details>
   <summary>정답</summary>

   정의에서도 문법상 이름을 생략할 수는 있지만, 그러면 몸체에서 그 매개변수를 가리킬 방법이 없다. 생성자는 받은 값을 멤버 변수에 대입해야 하므로 정의에서는 `n`, `i`, `b`처럼 이름을 붙여야 한다.

   </details>

2. **출력 순서:** Q3의 64~65행에서 `cout << "p1. ";` 다음에 `p1.showP();`가 실행될 때 화면의 한 줄이 어떻게 만들어지는지 설명하라.

   <details>
   <summary>정답</summary>

   64행이 `p1. `을 출력하고 줄을 바꾸지 않는다. 이어서 `showP()`가 같은 줄에 `x: 0, y: 0`을 출력하고 `endl`로 줄을 바꾸므로 한 줄에 `p1. x: 0, y: 0`이 나타난다.

   </details>

3. **나머지 계산:** Q2에서 `rand()`가 32767을 리턴하면 `nextInRange(2, 4)`의 값은 무엇인가?

   <details>
   <summary>정답</summary>

   32767을 3으로 나눈 나머지는 1(32767 = 3 × 10922 + 1)이므로 `1 + 2 = 3`이다.

   </details>

4. **생성자의 선택:** Q3의 `main()`을 `Point p1, p2(5, 6);`으로 바꾸고 입력을 받지 않은 채 두 점을 출력하면 결과는 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `p1`은 기본 생성자로 (0, 0)이 되고, `p2`는 인자 2개 생성자로 (5, 6)이 된다. 출력은 `p1. x: 0, y: 0`과 `p2. x: 5, y: 6`이다.

   </details>
