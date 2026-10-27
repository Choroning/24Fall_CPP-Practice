# HW3 코드 총정리 — 동적 메모리

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **학습 목표:**
> 1. 네 풀이의 클래스 선언부, 멤버 함수 정의부, `main()`을 줄 단위로 읽고 각 부분의 역할을 설명할 수 있다
> 2. `new`와 `delete`가 실행되는 시점에 힙의 객체와 포인터 변수가 어떻게 바뀌는지 추적할 수 있다
> 3. 반복문이 동적 배열의 원소를 읽고 누적하는 과정을 원소 단위로 추적할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: 동적으로 생성한 Complex 객체](#2-q1cpp-동적으로-생성한-complex-객체)
- [3. Q2.cpp: Member 클래스](#3-q2cpp-member-클래스)
- [4. Q3.cpp: 동적 정수 배열의 평균](#4-q3cpp-동적-정수-배열의-평균)
- [5. Q4.cpp: Circle 객체의 동적 배열](#5-q4cpp-circle-객체의-동적-배열)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

네 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스 정의(Q3은 없음), `main()`을 차례로 둔다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW3 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 클래스 | 동적으로 할당하는 것 | 입력 |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Complex` | `Complex` 객체 2개 (`new Complex`) | 없음 |
| [`Q2.cpp`](Solution/Q2.cpp) | `Member` | 없음 | 없음 |
| [`Q3.cpp`](Solution/Q3.cpp) | 없음 | `int` 5개의 배열 (`new int[5]`) | 정수 5개 |
| [`Q4.cpp`](Solution/Q4.cpp) | `Circle` | `Circle` 객체 `size`개의 배열 (`new Circle [size]`) | 원의 개수와 반지름 |

*표 1. HW3 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: 동적으로 생성한 Complex 객체

```cpp
/**
 * @file    A3Q1.cpp
 * @brief   Q1. 동적으로 생성한 Complex 객체 (6주차 문제 6-1)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Complex {
    int real, image;
public:
    Complex() {
        real = image = 1;
    }
    Complex(int real, int image) {
        this->real = real;
        this->image = image;
    }
    void setReal(int real) {
        this->real = real;
    }
    void setImage(int image) {
        this->image = image;
    }
    void pCom() {
        cout << real << " + " << image << "i\n";
    }
};

int main()
{
    Complex* a = new Complex;
    Complex* b = new Complex;
    a->pCom();
    b->pCom();
    cout << "---------------\n";
    (*a).setReal(4);
    (*b).setImage(9);
    (*a).pCom();
    (*b).pCom();
    
    delete a;
    a = nullptr;
    delete b;
    b = nullptr;
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 11~12 | `class Complex { int real, image;` | 접근 지정자가 없으므로 `real`, `image`는 `private` 멤버이다 |
| 14~16 | `Complex()` | 기본 생성자로, 두 멤버를 1로 초기화한다 |
| 17~20 | `Complex(int real, int image)` | 매개변수가 멤버를 가리므로 `this->`로 멤버를 가리켜 대입한다 |
| 21~26 | `setReal()`, `setImage()` | 같은 방식으로 멤버 하나를 받은 값으로 바꾼다 |
| 27~29 | `pCom()` | `실수부 + 허수부i` 형식으로 출력하고 `\n`으로 줄을 바꾼다 |
| 34~35 | `new Complex` 두 번 | 힙에 객체를 하나씩 만들고 기본 생성자를 호출하며, 주소를 `a`, `b`에 저장한다 |
| 36~38 | `a->pCom();`, `b->pCom();`, 구분선 | 화살표 연산자로 두 객체의 초기 상태를 출력한다 |
| 39~40 | `(*a).setReal(4);`, `(*b).setImage(9);` | 포인터를 역참조한 객체에 `.`으로 멤버 함수를 호출한다 |
| 41~42 | `(*a).pCom();`, `(*b).pCom();` | 바뀐 값을 출력한다 |
| 44~47 | `delete`와 `nullptr` 대입 | 두 객체를 반환하고 포인터를 널 포인터로 만든다 |

*표 2. Q1.cpp의 줄 단위 설명*

| 실행한 문장 | `*a` | `*b` | 출력 |
|:-----|:-----|:-----|:-----|
| 34~35행의 `new Complex` | (1, 1) | (1, 1) | |
| 36~38행 | (1, 1) | (1, 1) | `1 + 1i`, `1 + 1i`, 구분선 |
| `(*a).setReal(4);` | (4, 1) | (1, 1) | |
| `(*b).setImage(9);` | (4, 1) | (1, 9) | |
| 41~42행 | (4, 1) | (1, 9) | `4 + 1i`, `1 + 9i` |
| 44~47행 | 반환됨 | 반환됨 | (`a`, `b`는 `nullptr`) |

*표 3. Q1의 실행에 따른 두 동적 객체의 상태 (실수부, 허수부)*

---

<br>

## 3. Q2.cpp: Member 클래스

```cpp
/**
 * @file    A3Q2.cpp
 * @brief   Q2. 회원 정보를 관리하는 Member 클래스 (6주차 문제 6-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Member {
    string name;
    string id;
    int age;
public:
    Member() {
        name = "noname";
        id = "none";
        age = 0;
    }
    Member(string name, string id, int age) {
        this->name = name;
        this->id = id;
        this->age = age;
    }
    void setName(string name) {
        this->name = name;
    }
    void setId(string id) {
        this->id = id;
    }
    void setAge(int age) {
        this->age = age;
    }
    
    string getName() {
        return name;
    }
    string getId() {
        return id;
    }
    int getAge() {
        return age;
    }
    void printM() {
        cout << name << ", " << id << ", " << age << endl;
    }
};

int main()
{
    Member Kim;
    Member Lee("Lee Hi", "2016001", 22);
    Kim.printM();
    Lee.printM();
    Kim.setName("Kim Haha");
    Kim.setId("2016020");
    Kim.setAge(29);
    Kim.printM();
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 11~14 | `class Member { string name; string id; int age;` | 이름, 아이디, 나이를 `private` 멤버로 둔다 |
| 16~20 | `Member()` | 기본 생성자로, 몸체에서 `"noname"`, `"none"`, 0을 대입한다 |
| 21~25 | `Member(string name, string id, int age)` | 매개변수 3개 생성자로, `this->`로 멤버에 대입한다 |
| 26~34 | `setName()`, `setId()`, `setAge()` | 멤버 하나씩을 받은 값으로 바꾼다 |
| 36~44 | `getName()`, `getId()`, `getAge()` | 멤버의 값을 리턴한다. 이 `main()`에서는 쓰이지 않는다 |
| 45~47 | `printM()` | `name, id, age` 형식으로 한 줄을 출력한다 |
| 52~53 | `Member Kim;`, `Member Lee(...)` | `Kim`은 기본 생성자로, `Lee`는 매개변수 3개 생성자로 만든다 |
| 54~55 | 두 `printM()` | 두 객체의 초기 상태를 출력한다 |
| 56~59 | set 함수 세 번과 `printM()` | `Kim`의 세 멤버를 바꾼 뒤 다시 출력한다 |

*표 4. Q2.cpp의 줄 단위 설명*

| 실행한 문장 | `Kim` | `Lee` | 출력 |
|:-----|:-----|:-----|:-----|
| 52~53행 | (`"noname"`, `"none"`, 0) | (`"Lee Hi"`, `"2016001"`, 22) | |
| 54~55행 | 같음 | 같음 | `noname, none, 0`, `Lee Hi, 2016001, 22` |
| 56~58행 | (`"Kim Haha"`, `"2016020"`, 29) | 같음 | |
| 59행 | 같음 | 같음 | `Kim Haha, 2016020, 29` |

*표 5. Q2의 실행에 따른 두 객체의 상태 (`name`, `id`, `age`)*

---

<br>

## 4. Q3.cpp: 동적 정수 배열의 평균

```cpp
/**
 * @file    A3Q3.cpp
 * @brief   Q3. 동적 정수 배열로 구하는 평균 (6주차 문제 6-4)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

int main()
{
    int ave = 0;
    int* p = new int[5];
    
    cout << "정수 5개 입력 >> ";
    for (int i = 0; i < 5; i++){
        cin >> p[i];
        ave += p[i];
    }
    
    cout << "평균 " << ave / 5.0 << endl;
    
    delete [] p;
    p = nullptr;
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 13 | `int ave = 0;` | 입력값의 합을 누적할 변수이다 |
| 14 | `int* p = new int[5];` | 힙에 `int` 5개의 배열을 할당하고 첫 원소의 주소를 `p`에 저장한다 |
| 16 | 프롬프트 출력 | 줄을 바꾸지 않으므로 입력이 프롬프트 뒤에 이어진다 |
| 17~20 | `for` 문 | `cin >> p[i];`로 `i`번째 원소에 입력값을 저장하고 `ave`에 더한다 |
| 22 | `ave / 5.0` | `ave`가 `double`로 변환되어 실수 나눗셈의 결과가 출력된다 |
| 24~25 | `delete [] p;`, `p = nullptr;` | 배열을 반환하고 포인터를 널 포인터로 만든다 |

*표 6. Q3.cpp의 줄 단위 설명*

| `i` | 입력한 값 `p[i]` | 반복 뒤의 `ave` |
|-----:|-----:|-----:|
| 0 | 1 | 1 |
| 1 | 2 | 3 |
| 2 | 4 | 7 |
| 3 | 5 | 12 |
| 4 | 10 | 22 |

*표 7. 입력 `1 2 4 5 10`에 대한 Q3의 반복 추적*

반복이 끝난 뒤 22행은 `22 / 5.0 = 4.4`를 계산하여 `평균 4.4`를 출력한다.

---

<br>

## 5. Q4.cpp: Circle 객체의 동적 배열

```cpp
/**
 * @file    A3Q4.cpp
 * @brief   Q4. Circle 객체의 동적 배열 (6주차 문제 6-5)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    void setRadius(int radius);
    double getArea();
};

void Circle::setRadius(int radius) {
    this->radius = radius;
}

double Circle::getArea(){
    return 3.14 * radius * radius;
}

int main(void)
{
    Circle* p;
    int size, count = 0;
    cout << "원의 개수 >> ";
    cin >> size;
    p = new Circle [size];
    
    for (int i = 0; i < size; i++) {
        int temp;
        cout << "원 " << i + 1 << "의 반지름 >> ";
        cin >> temp;
        p[i].setRadius(temp);
        
        if (p[i].getArea() >= 100)
            count++;
    }
    cout << "면적이 100보다 큰 원은 " << count << "개 입니다.\n";
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 11~16 | `class Circle { ... };` | 문제가 준 클래스로, `private` 멤버 `radius`와 두 멤버 함수의 원형을 둔다 |
| 18~20 | `Circle::setRadius(int radius)` | `this->radius = radius;`로 멤버에 대입한다 |
| 22~24 | `Circle::getArea()` | `3.14 * radius * radius`를 리턴한다 |
| 28~29 | `Circle* p; int size, count = 0;` | 배열을 가리킬 포인터, 원의 개수, 면적이 100 이상인 원의 개수를 선언한다 |
| 30~31 | 개수 입력 | 원의 개수를 `size`에 입력받는다 |
| 32 | `p = new Circle [size];` | 실행 중에 정한 개수만큼 `Circle` 객체를 힙에 만들며, 원소마다 기본 생성자가 호출된다 |
| 34~42 | `for` 문 | 원마다 반지름을 `temp`에 입력받아 설정하고, 면적이 100 이상이면 `count`를 늘린다 |
| 43 | 결과 출력 | `count`를 넣은 문장을 출력한다. 이 뒤에 `delete [] p;`가 없어 배열이 반환되지 않는다 |

*표 8. Q4.cpp의 줄 단위 설명*

| `i` | 반지름 | 면적 `getArea()` | 100 이상 | 반복 뒤의 `count` |
|-----:|-----:|-----:|:-----|-----:|
| 0 | 3 | 28.26 | 아니오 | 0 |
| 1 | 7 | 153.86 | 예 | 1 |
| 2 | 2 | 12.56 | 아니오 | 1 |
| 3 | 9 | 254.34 | 예 | 2 |
| 4 | 1 | 3.14 | 아니오 | 2 |

*표 9. 입력 5, 3, 7, 2, 9, 1에 대한 Q4의 반복 추적*

반복이 끝난 뒤 43행은 `면적이 100보다 큰 원은 2개 입니다.`를 출력한다. 배열 `p`는 프로그램이 끝날 때까지 반환되지 않으며, 이 블록이 `leaks --atExit`가 보고하는 누수 1개(32바이트)이다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 파일의 구성 | Q1, Q2, Q4는 클래스 정의와 `main()`으로, Q3은 `main()`만으로 이루어진다. |
| 동적 객체의 수명 | `new Complex`로 만든 객체는 `delete`가 실행될 때까지 남으며, `delete` 뒤의 포인터는 `nullptr`로 바꾸어 둔다. |
| 포인터로 멤버 호출 | `a->pCom()`과 `(*a).pCom()`은 같은 호출이다. |
| 배열 원소의 접근 | `p[i]`는 `*(p + i)`이므로 동적 배열도 일반 배열과 같은 문법으로 읽고 쓴다. |
| 누적과 평균 | Q3의 `ave`는 1, 3, 7, 12, 22로 늘어나고, 평균은 출력할 때 `22 / 5.0`으로 계산된다. |
| 반환하지 않은 배열 | Q4의 `new Circle [size]`는 대응하는 `delete []`가 없어 누수로 남는다. |

---

<br>

## 점검 문제

1. **포인터의 값:** Q1의 44행 `delete a;`가 실행된 직후, 45행이 실행되기 전에 `a`의 값은 무엇인가?

   <details>
   <summary>정답</summary>

   `delete`는 포인터가 가리키는 객체만 반환하고 포인터 변수의 값은 바꾸지 않으므로, `a`에는 반환된 객체의 주소가 그대로 남아 있다. 이 상태의 `a`가 댕글링 포인터이며, 45행이 `nullptr`를 대입한다.

   </details>

2. **쓰이지 않는 함수:** Q2의 get 함수 세 개를 지우면 프로그램의 출력은 달라지는가?

   <details>
   <summary>정답</summary>

   달라지지 않는다. `main()`과 `printM()`은 get 함수를 호출하지 않는다. 다만 문제가 get 함수를 요구하므로 풀이에는 남겨 두어야 한다.

   </details>

3. **반복 추적:** Q3에 `10 20 30 40 51`을 입력하면 마지막 반복 뒤의 `ave`와 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   `ave`는 10, 30, 60, 100, 151로 늘어나 151이 되고, `151 / 5.0`인 `평균 30.2`가 출력된다.

   </details>

4. **원소의 초기 상태:** Q4의 32행이 실행된 직후, 34행의 반복이 시작되기 전에 `p[0].getArea()`를 호출하면 어떤 값이 나오는가?

   <details>
   <summary>정답</summary>

   `Circle`에는 생성자가 없어 컴파일러가 제공하는 기본 생성자가 `radius`를 초기화하지 않으므로, 이때의 `radius`는 정해지지 않은 값이다. 그 값을 읽는 것은 정의되지 않은 동작이며 결과를 예측할 수 없다. 이 코드는 반복 안에서 `setRadius()`로 값을 먼저 정한 뒤 `getArea()`를 호출하므로 이 문제가 생기지 않는다.

   </details>
