# 3장 연습 문제 — 클래스와 객체

[레포지토리 안내](../../README.ko.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 3

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

연습 문제 1과 2는 실습 문제이고, 연습 문제 3\~12는 교재 3장 이론 문제의 짝수 번 문항(2\~20번)이다.

---

<br>

## 목차

- [연습 문제 1: 클래스 연습](#연습-문제-1-클래스-연습)
- [연습 문제 2: 커피 머신](#연습-문제-2-커피-머신)
- [연습 문제 3: 클래스와 객체에 대한 설명](#연습-문제-3-클래스와-객체에-대한-설명)
- [연습 문제 4: 전역 변수와 전역 함수의 캡슐화](#연습-문제-4-전역-변수와-전역-함수의-캡슐화)
- [연습 문제 5: `Tower` 클래스의 틀린 부분 수정](#연습-문제-5-tower-클래스의-틀린-부분-수정)
- [연습 문제 6: `Calendar` 클래스의 구현](#연습-문제-6-calendar-클래스의-구현)
- [연습 문제 7: 소멸자에 대한 설명](#연습-문제-7-소멸자에-대한-설명)
- [연습 문제 8: 객체의 생성과 소멸 순서](#연습-문제-8-객체의-생성과-소멸-순서)
- [연습 문제 9: `private` 멤버에 접근하는 오류](#연습-문제-9-private-멤버에-접근하는-오류)
- [연습 문제 10: 인라인 함수의 장단점](#연습-문제-10-인라인-함수의-장단점)
- [연습 문제 11: 인라인으로 처리하기에 알맞은 함수](#연습-문제-11-인라인으로-처리하기에-알맞은-함수)
- [연습 문제 12: 구조체를 클래스로 바꾸기](#연습-문제-12-구조체를-클래스로-바꾸기)

---

<br>

## 연습 문제 1: 클래스 연습

**교재 문항:** 3장 실습 문제 1번

**문제:** 예시 코드를 참고하여 `main()` 함수의 실행 결과가 실행 예시와 같도록 `Tower` 클래스를 작성하라.

**프로그램 요구사항**

1. 예시 코드에 주어진 `main()` 함수의 내용은 변경할 수 없다.
2. 클래스의 선언과 정의를 분리하여 작성하라.
3. 모든 멤버 변수의 접근 제어는 `private`로 설정하라.

**예시 코드**

```cpp
#include <iostream>
using namespace std;

int main() {
   Tower myTower;          // 1미터
   Tower seoulTower(100);  // 100미터
   cout << "높이는 " << myTower.getHeight() << "미터" << endl;
   cout << "높이는 " << seoulTower.getHeight() << "미터" << endl;

   return 0;
}
```

**실행 예시**

```text
높이는 1미터
높이는 100미터

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-1-클래스-연습)**

---

<br>

## 연습 문제 2: 커피 머신

**교재 문항:** 3장 실습 문제 4번

**문제:** 예시 코드를 참고하여 `main()` 함수의 실행 결과가 실행 예시와 같도록 `CoffeeMachine` 클래스를 작성하라. 에스프레소 한 잔에는 커피와 물이 각각 1씩 소비되고, 아메리카노의 경우 커피는 1, 물은 2가 소비되고, 설탕 커피는 커피 1, 물 2, 설탕 1이 소비된다.

**프로그램 요구사항**

1. 예시 코드에 주어진 `main()` 함수의 내용은 변경할 수 없다.
2. 클래스의 선언과 정의를 분리하여 작성하라.
3. 모든 멤버 변수의 접근 제어는 `private`로 설정하라.
4. 재료가 부족하면 오류 메시지를 출력하고, 현재 커피 머신의 상태를 출력하라.

**예시 코드**

```cpp
#include <iostream>
using namespace std;

int main() {
   CoffeeMachine java(5, 8, 2);  // 커피 5, 물 8, 설탕 2로 초기화

   java.drinkAmericano();
   java.show();

   java.drinkSugarCoffee();
   java.show();

   java.drinkEspresso();
   java.show();

   java.drinkSugarCoffee();
   java.show();

   java.drinkSugarCoffee();

   java.drinkAmericano();

   java.fill();
   java.show();

   return 0;
}
```

**실행 예시**

```text
커피머신 상태: 커피 = 4   물 = 6   설탕 = 2
커피머신 상태: 커피 = 3   물 = 4   설탕 = 1
커피머신 상태: 커피 = 2   물 = 3   설탕 = 1
커피머신 상태: 커피 = 1   물 = 1   설탕 = 0

재료가 부족하여 설탕커피를 만들 수 없습니다.
커피머신 상태: 커피 = 1   물 = 1   설탕 = 0

재료가 부족하여 아메리카노를 만들 수 없습니다.
커피머신 상태: 커피 = 1   물 = 1   설탕 = 0
커피머신 상태: 커피 = 10   물 = 10   설탕 = 10

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-2-커피-머신)**

---

<br>

## 연습 문제 3: 클래스와 객체에 대한 설명

**교재 문항:** 3장 이론 문제 2번

**문제:** 클래스와 객체에 관한 설명 중 틀린 것은?

- ① 객체를 실체 혹은 인스턴스(instance)라고 부른다.
- ② 클래스는 객체를 생성하기 위한 설계도 혹은 틀과 같다.
- ③ 클래스의 멤버들은 `private`보다 `public` 접근 지정이 바람직하다.
- ④ 클래스는 함수 멤버와 변수 멤버로 이루어진다.

**[풀이](Solutions.ko.md#연습-문제-3-클래스와-객체에-대한-설명)**

---

<br>

## 연습 문제 4: 전역 변수와 전역 함수의 캡슐화

**교재 문항:** 3장 이론 문제 4번

**문제:** 다음 C++ 프로그램에서 캡슐화가 부족한 부분을 수정하여 캡슐화하라.

```cpp
int age;
void older() {
    age++;
}
class Circle {
    int radius;
public:
    double getArea();
};
```

**[풀이](Solutions.ko.md#연습-문제-4-전역-변수와-전역-함수의-캡슐화)**

---

<br>

## 연습 문제 5: `Tower` 클래스의 틀린 부분 수정

**교재 문항:** 3장 이론 문제 6번

**문제:** 다음 코드는 `Tower` 클래스를 작성한 사례이다. 틀린 부분을 수정하라.

```cpp
class Tower {
    int height = 20;
public:
    Tower() { height = 10; return; }
};
```

**[풀이](Solutions.ko.md#연습-문제-5-tower-클래스의-틀린-부분-수정)**

---

<br>

## 연습 문제 6: `Calendar` 클래스의 구현

**교재 문항:** 3장 이론 문제 8번

**문제:** 다음은 `Calendar` 클래스의 선언부이다. `year`를 10으로 초기화하는 생성자와 `year` 값을 리턴하는 `getYear()`를 구현하라.

```cpp
class Calendar {
private:
    int year;
public:
    Calendar();
    int getYear();
};
```

**[풀이](Solutions.ko.md#연습-문제-6-calendar-클래스의-구현)**

---

<br>

## 연습 문제 7: 소멸자에 대한 설명

**교재 문항:** 3장 이론 문제 10번

**문제:** 다음은 소멸자에 관한 설명이다. 밑줄 친 ①~④ 중 틀린 부분을 지적하라.

소멸자는 ① 객체가 소멸되는 시점에 자동으로 호출되는 멤버 함수로서, ② 클래스의 이름 앞에 `~`를 붙인 이름으로 선언되어야 한다. ③ 매개변수 있는 소멸자를 작성하여 소멸 시에 의미 있는 값을 전달할 수 있으며, 소멸자가 선언되어 있지 않으면 ④ 기본 소멸자가 자동으로 생성된다.

**[풀이](Solutions.ko.md#연습-문제-7-소멸자에-대한-설명)**

---

<br>

## 연습 문제 8: 객체의 생성과 소멸 순서

**교재 문항:** 3장 이론 문제 12번

**문제:** 다음 프로그램에서 객체 `a`, `b`, `c`가 생성되고 소멸되는 순서는 무엇인가?

```cpp
class House {
    int numOfRooms;
    int size;
public:
    House(int n, int s) { numOfRooms = n; size = s; }
    void test() {
        House a(1, 10);
    }
};
void f() {
    House b(2, 20);
    b.test();
}
House c(3, 30);
int main() {
    f();
}
```

**[풀이](Solutions.ko.md#연습-문제-8-객체의-생성과-소멸-순서)**

---

<br>

## 연습 문제 9: `private` 멤버에 접근하는 오류

**교재 문항:** 3장 이론 문제 14번

**문제:** 다음 프로그램의 오류를 지적하고 수정하라.

```cpp
class TV {
    int channels;
public:
    int colors;
    TV() { channels = 256; }
    TV(int a, int b) { channels = a; colors = b; }
};
int main() {
    TV LG;
    LG.channels = 200;
    LG.colors = 60000;
    TV Samsung(100, 50000);
}
```

**[풀이](Solutions.ko.md#연습-문제-9-private-멤버에-접근하는-오류)**

---

<br>

## 연습 문제 10: 인라인 함수의 장단점

**교재 문항:** 3장 이론 문제 16번

**문제:** 인라인 함수의 장단점을 설명한 것 중 옳은 것은?

- ① 인라인 함수를 사용하면 컴파일 속도가 향상된다.
- ② 인라인 함수를 이용하면 프로그램의 실행 속도가 향상된다.
- ③ 인라인 함수를 사용하면 프로그램 작성 시간이 향상된다.
- ④ 인라인 함수를 사용하면 프로그램의 크기가 작아져서 효과적이다.

**[풀이](Solutions.ko.md#연습-문제-10-인라인-함수의-장단점)**

---

<br>

## 연습 문제 11: 인라인으로 처리하기에 알맞은 함수

**교재 문항:** 3장 이론 문제 18번

**문제:** `inline` 선언은 강제 사항이 아니다. 다음 함수 중에서 컴파일러가 인라인으로 처리하기에 가장 바람직한 것은?

```cpp
// ①
inline int big(int a, int b) {
    return a > b ? a : b;
}
// ②
inline int sum(int a, int b) {
    if (a >= b) return a;
    else return a + sum(a + 1, b);
}
// ③
inline void add(int a, int b) {
    int sum = 0;
    for (int n = a; n < b; n++)
        sum += n;
}
// ④
inline int add(int a) {
    static int x = 0;
    x += a;
    return x;
}
```

**[풀이](Solutions.ko.md#연습-문제-11-인라인으로-처리하기에-알맞은-함수)**

---

<br>

## 연습 문제 12: 구조체를 클래스로 바꾸기

**교재 문항:** 3장 이론 문제 20번

**문제:** 다음 C++ 구조체를 동일한 의미를 가지는 클래스로 작성하라.

```cpp
struct Family {
    int count;
    char address[20];
public:
    Family();
private:
    char tel[11];
};
```

**[풀이](Solutions.ko.md#연습-문제-12-구조체를-클래스로-바꾸기)**
