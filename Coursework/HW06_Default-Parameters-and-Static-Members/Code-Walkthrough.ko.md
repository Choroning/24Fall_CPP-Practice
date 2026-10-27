# HW6 코드 총정리 — 디폴트 매개변수와 static 멤버

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 6

> **학습 목표:**
> 1. 네 풀이의 클래스 선언부, 클래스 밖의 정의, `main()`을 줄 단위로 읽고 각 부분의 역할을 설명할 수 있다
> 2. 디폴트 매개변수를 갖는 생성자 호출이 실제로 어떤 인자로 바뀌는지 쓸 수 있다
> 3. 생성자와 소멸자가 호출될 때마다 static 멤버 변수의 값이 어떻게 바뀌는지 문장 단위로 추적할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: 디폴트 매개변수를 갖는 Person 생성자](#2-q1cpp-디폴트-매개변수를-갖는-person-생성자)
- [3. Q2.cpp: numPerson으로 회원 수를 세는 Person 클래스](#3-q2cpp-numperson으로-회원-수를-세는-person-클래스)
- [4. Q3.cpp: static 멤버 함수로 이루어진 Random 클래스](#4-q3cpp-static-멤버-함수로-이루어진-random-클래스)
- [5. Q4.cpp: count로 객체 수를 세는 Circle 클래스](#5-q4cpp-count로-객체-수를-세는-circle-클래스)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

네 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스 선언, 클래스 밖의 정의, `main()`을 차례로 둔다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW6 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 클래스 | 핵심 멤버 | 클래스 밖의 정의 |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Person` | 디폴트 매개변수를 갖는 생성자 | 생성자 |
| [`Q2.cpp`](Solution/Q2.cpp) | `Person` | `static int numPerson`, `static void nPerson()` | 세 생성자, 소멸자, `nPerson()`, `numPerson` |
| [`Q3.cpp`](Solution/Q3.cpp) | `Random` | static 멤버 함수 네 개 | `nextInt()`, `nextAlphabet()`, `nextDouble()` |
| [`Q4.cpp`](Solution/Q4.cpp) | `Circle` | `static int count` | 소멸자, `count` |

*표 1. HW6 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: 디폴트 매개변수를 갖는 Person 생성자

```cpp
/**
 * @file    A6Q1.cpp
 * @brief   Q1. 디폴트 매개변수를 갖는 Person 생성자 (11주차 실습 2-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
public:
    Person(int id = 1, string name = "Grace", double weight = 20.5);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

Person::Person(int id, string name, double weight) {
    this->id = id;
    this->name = name;
    this->weight = weight;
}

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 11~14 | `int id; double weight; string name;` | 접근 지정자가 없으므로 세 멤버는 `private`이다 |
| 16 | `Person(int id = 1, string name = "Grace", double weight = 20.5);` | 디폴트 값은 이 선언에만 쓴다 |
| 17 | `show()` | `id weight name` 순서로 공백으로 구분하여 출력한다 |
| 20~24 | `Person::Person(int id, string name, double weight)` | 디폴트 값 없이 정의하며, `this->`로 멤버에 대입한다 |
| 27 | `Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);` | 한 선언문에서 인자 수가 다른 세 객체를 만든다 |
| 28~30 | 세 `show()` | 세 객체의 정보를 한 줄씩 출력한다 |

*표 2. Q1.cpp의 줄 단위 설명*

| 작성한 생성 | 실제 호출 | 출력 |
|:-----|:-----|:-----|
| `grace` | `Person(1, "Grace", 20.5)` | `1 20.5 Grace` |
| `ashley(2, "Ashley")` | `Person(2, "Ashley", 20.5)` | `2 20.5 Ashley` |
| `helen(3, "Helen", 32.5)` | `Person(3, "Helen", 32.5)` | `3 32.5 Helen` |

*표 3. Q1의 생성자 호출과 디폴트 값이 채워진 인자*

---

<br>

## 3. Q2.cpp: numPerson으로 회원 수를 세는 Person 클래스

```cpp
/**
 * @file    A6Q2.cpp
 * @brief   Q2. static 멤버로 회원 수를 세는 Person 클래스 (11주차 실습 2-3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
    static int numPerson;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    ~Person();
    void show() { cout << id << ' ' << weight << ' ' << name << ' ' << numPerson << endl; }
    static void nPerson();
};
Person::Person() {
    id = 1;
    weight = 20.5;
    name = "Grace";
    numPerson++;
}
Person::Person(int id, string name) {
    this->id = id;
    weight = 20.5;
    this->name = name;
    numPerson++;
}
Person::Person(int id, string name, double weight) {
    this->id = id;
    this->weight = weight;
    this->name = name;
    numPerson++;
}
Person::~Person() {
    numPerson--;
}
void Person::nPerson() {
    cout << numPerson << endl;
}

int Person::numPerson = 0;

int main() {
    Person grace;
    grace.show();
    
    Person helen(13, "Helen", 32.5);
    helen.show();
    
    Person *p = new Person[2] { {43, "Tera"}, {98, "Hera"} };
    p[0].show();
    p[1].show();
    Person::nPerson();
    
    delete [] p;
    Person::nPerson();
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 15 | `static int numPerson;` | 클래스에 하나만 있는 회원 수의 선언이다 |
| 17~19 | 세 생성자의 선언 | 인자 0개, 2개, 3개의 생성을 각각 맡는다 |
| 20 | `~Person();` | 소멸자의 선언이다 |
| 21 | `show()` | 세 멤버 뒤에 `numPerson`을 출력한다 |
| 22 | `static void nPerson();` | 회원 수만 출력하는 static 멤버 함수의 선언이다 |
| 24~41 | 세 생성자의 정의 | 멤버를 초기화한 뒤 각각 `numPerson++`를 실행한다 |
| 42~44 | `Person::~Person()` | `numPerson--`를 실행한다 |
| 45~47 | `Person::nPerson()` | `numPerson`을 출력한다. 정의에는 `static`을 다시 쓰지 않는다 |
| 49 | `int Person::numPerson = 0;` | static 멤버 변수의 정의와 초기화이다 |
| 52~56 | `grace`, `helen` | 기본 생성자와 세 번째 생성자로 만들고 각각 출력한다 |
| 58 | `new Person[2] { {43, "Tera"}, {98, "Hera"} }` | 두 번째 생성자로 원소 두 개를 만든다 |
| 59~61 | 두 `show()`와 `nPerson()` | 배열의 원소와 회원 수를 출력한다 |
| 63~64 | `delete [] p;`와 `nPerson()` | 두 원소를 소멸시킨 뒤 회원 수를 출력한다 |

*표 4. Q2.cpp의 줄 단위 설명*

| 실행한 문장 | 호출되는 함수 | 실행 뒤 `numPerson` | 출력 |
|:-----|:-----|-----:|:-----|
| 49행의 정의 | 없음 | 0 | |
| `Person grace;` | `Person()` | 1 | |
| `grace.show();` | | 1 | `1 20.5 Grace 1` |
| `Person helen(13, "Helen", 32.5);` | `Person(int, string, double)` | 2 | |
| `helen.show();` | | 2 | `13 32.5 Helen 2` |
| `new Person[2] {...}` | `Person(int, string)` 두 번 | 4 | |
| `p[0].show();`, `p[1].show();` | | 4 | `43 20.5 Tera 4`, `98 20.5 Hera 4` |
| `Person::nPerson();` | | 4 | `4` |
| `delete [] p;` | `~Person()` 두 번 (`p[1]`, `p[0]`) | 2 | |
| `Person::nPerson();` | | 2 | `2` |
| `main()`의 끝 | `~Person()` 두 번 (`helen`, `grace`) | 0 | |

*표 5. Q2의 실행에 따른 numPerson의 값*

---

<br>

## 4. Q3.cpp: static 멤버 함수로 이루어진 Random 클래스

```cpp
/**
 * @file    A6Q3.cpp
 * @brief   Q3. static 멤버로 이루어진 Random 클래스 (11주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Random {
public:
    // 항상 다른 랜덤수를 발생시키기
    // 위한 seed를 설정하는 함수
    static void seed() {
        srand((unsigned)time(0));
    }
    
    //min과 max 사이의 랜덤 정수 리턴
    static int nextInt(int min = 0, int max = 32767);
    static char nextAlphabet();

    // 0보다 크거나 같고 1보다 적은 실수 리턴
    static double nextDouble();
};

int Random::nextInt(int min, int max) {
    return min + rand() % (max - min + 1);
}
char Random::nextAlphabet() {
    if (rand() % 2)
        return 'a' + rand() % 26;
    else
        return 'A' + rand() % 26;
}
double Random::nextDouble() {
    return (double) rand() / RAND_MAX;
}

int main() {
    Random::seed();
    cout << "1에서 100까지 랜덤한 정수 10개를 출력합니다" << endl;
    for (int i = 0; i < 10; i++)
        cout << Random::nextInt(1, 100) << ' ';
    cout << endl;
    
    cout << "랜덤한 알파벳 10개를 출력합니다" << endl;
    for (int i = 0; i < 10; i++)
        cout << Random::nextAlphabet() << ' ';
    cout << endl;
    
    cout << "랜덤한 실수 10개를 출력합니다" << endl;
    for (int i = 0; i < 5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
    for (int i = 0; i < 5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~10 | `<iostream>`, `<cstdlib>`, `<ctime>` | `rand()`, `srand()`, `RAND_MAX`, `time()`을 쓰기 위해 포함한다 |
| 17~19 | `static void seed()` | 문제가 준 함수로, 현재 시각을 시드로 정한다 |
| 22 | `static int nextInt(int min = 0, int max = 32767);` | 디폴트 값을 갖는 선언이다 |
| 23, 26 | `nextAlphabet()`, `nextDouble()`의 선언 | 정의는 클래스 밖에 있다 |
| 29~31 | `Random::nextInt()` | `min + rand() % (max - min + 1)`을 리턴한다 |
| 32~37 | `Random::nextAlphabet()` | `rand() % 2`로 소문자와 대문자를 고르고, 다시 `rand() % 26`으로 글자를 고른다 |
| 38~40 | `Random::nextDouble()` | `(double) rand() / RAND_MAX`를 리턴한다 |
| 43 | `Random::seed();` | 객체 없이 시드를 한 번 정한다 |
| 44~47 | 정수 10개 | `Random::nextInt(1, 100)`을 10번 호출하여 출력한다 |
| 49~52 | 알파벳 10개 | `Random::nextAlphabet()`의 `char` 리턴값을 문자로 출력한다 |
| 54~60 | 실수 10개 | `Random::nextDouble()`을 5개씩 두 줄로 출력한다 |

*표 6. Q3.cpp의 줄 단위 설명*

`nextAlphabet()`에서 첫 `rand()`가 7을, 두 번째 `rand()`가 30을 리턴하면 `7 % 2`가 1이므로 소문자 쪽이 선택되고 `'a' + 30 % 26 = 'a' + 4 = 'e'`가 리턴된다. 첫 `rand()`가 짝수이면 대문자 쪽이 선택되어 같은 두 번째 값에서 `'E'`가 리턴된다.

---

<br>

## 5. Q4.cpp: count로 객체 수를 세는 Circle 클래스

```cpp
/**
 * @file    A6Q4.cpp
 * @brief   Q4. static 멤버로 객체 수를 세는 Circle 클래스 (11주차 실습 4)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
    static int count;
public:
    Circle() {
        radius = 0;
        count++;
    }
    Circle (int r) {
        radius = r;
        count++;
    }
    ~Circle();
    void show() {
        cout << "Radius: " << radius << ", Count: " << count << endl;
    }
};
Circle::~Circle() {
    count--;
}
int Circle::count = 0;

int main() {
    Circle a(5);
    a.show();
    
    Circle *p = new Circle[2];
    
    for (int i = 0; i < 2; i++)
        p[i].show();
    
    a.show();
    
    delete[] p;
    p = nullptr;
    a.show();
}
```

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 12~13 | `int radius; static int count;` | 반지름과 클래스에 하나만 있는 객체 수이다 |
| 15~22 | 두 생성자 | 반지름을 0이나 받은 값으로 정하고 `count++`를 실행한다 |
| 23 | `~Circle();` | 소멸자의 선언이며, 정의는 28~30행에 있다 |
| 24~26 | `show()` | `Radius: ..., Count: ...` 형식으로 출력한다 |
| 28~30 | `Circle::~Circle()` | `count--`를 실행한다 |
| 31 | `int Circle::count = 0;` | static 멤버 변수의 정의와 초기화이다 |
| 34~35 | `Circle a(5);`와 출력 | 객체 하나를 만들고 출력한다 |
| 37 | `Circle *p = new Circle[2];` | 기본 생성자로 원소 두 개를 만든다 |
| 39~42 | 원소 출력과 `a.show()` | 세 객체가 있는 상태를 출력한다 |
| 44~46 | `delete[] p;`, `p = nullptr;`, `a.show()` | 두 원소를 소멸시키고 포인터를 비운 뒤 출력한다 |

*표 7. Q4.cpp의 줄 단위 설명*

| 실행한 문장 | 실행 뒤 `count` | 출력 |
|:-----|-----:|:-----|
| `Circle a(5);` | 1 | |
| `a.show();` | 1 | `Radius: 5, Count: 1` |
| `new Circle[2]` | 3 | |
| `p[0].show();`, `p[1].show();` | 3 | `Radius: 0, Count: 3` 두 줄 |
| `a.show();` | 3 | `Radius: 5, Count: 3` |
| `delete[] p;` | 1 | |
| `a.show();` | 1 | `Radius: 5, Count: 1` |
| `main()`의 끝 | 0 | |

*표 8. Q4의 실행에 따른 count의 값*

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 디폴트 값이 채워진 호출 | Q1의 `ashley(2, "Ashley")`는 `Person(2, "Ashley", 20.5)`로 호출된다. |
| static 멤버의 선언과 정의 | 클래스 안의 `static int numPerson;`은 선언이고, 49행의 `int Person::numPerson = 0;`이 정의이다. |
| 개수의 추적 | Q2의 `numPerson`은 1, 2, 4, 2를 거쳐 `main()`이 끝날 때 0이 되고, Q4의 `count`는 1, 3, 1을 거쳐 0이 된다. |
| static 멤버 함수의 호출 | `Random::seed()`, `Random::nextInt(1, 100)`, `Person::nPerson()`은 모두 객체 없이 클래스 이름으로 호출한다. |
| 소멸 순서 | `delete []`는 배열의 원소를 생성의 역순으로, `main()`의 끝은 지역 객체를 선언의 역순으로 소멸시킨다. |

---

<br>

## 점검 문제

1. **실제 인자:** Q1의 `main()`에 `Person bob(7);`을 추가하면 어떤 인자로 생성자가 호출되며 `bob.show()`는 무엇을 출력하는가?

   <details>
   <summary>정답</summary>

   `Person(7, "Grace", 20.5)`로 호출되어 `7 20.5 Grace`를 출력한다. 인자가 하나뿐이므로 `id`만 채워지고 `name`과 `weight`에는 디폴트 값이 들어간다.

   </details>

2. **정의의 `static`:** Q2의 45행을 `static void Person::nPerson() {`으로 바꾸면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   컴파일 오류가 난다. `static`은 클래스 안의 선언에만 쓰며, 클래스 밖의 멤버 함수 정의에 다시 쓸 수 없다.

   </details>

3. **추적:** Q4의 `main()`에서 37행을 `Circle *p = new Circle[3];`, 39행의 반복을 `i < 3`으로 바꾸면 각 줄의 `Count`는 어떻게 되는가?

   <details>
   <summary>정답</summary>

   첫 줄은 `Count: 1`, 배열의 세 원소와 그다음 `a.show()`는 `Count: 4`, `delete[] p;` 뒤의 마지막 줄은 `Count: 1`이다.

   </details>

4. **문자의 계산:** Q3의 `nextAlphabet()`에서 첫 `rand()`가 4를, 두 번째 `rand()`가 27을 리턴하면 어떤 문자가 리턴되는가?

   <details>
   <summary>정답</summary>

   `4 % 2`가 0이므로 대문자 쪽이 선택되고, `27 % 26`이 1이므로 `'A' + 1`인 `'B'`가 리턴된다.

   </details>
