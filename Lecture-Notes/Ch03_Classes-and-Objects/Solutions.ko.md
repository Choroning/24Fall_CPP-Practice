# 3장 연습 문제 풀이 — 클래스와 객체

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 3

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

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

**문제:** 주어진 `main()`에서 `Tower myTower;`는 높이 1미터, `Tower seoulTower(100);`은 높이 100미터인 탑 객체를 만들고 `getHeight()`로 높이를 출력한다. 선언과 정의를 분리하고 모든 멤버 변수를 `private`으로 하여 `Tower` 클래스를 작성하라. 문제 전문과 실행 예시는 [연습 문제 1](Exercises.ko.md#연습-문제-1-클래스-연습)에 있다.

### 풀이

`Tower` 객체가 가지는 상태는 높이 하나이므로 `private` 멤버 변수 `int height`를 둔다. `main()`이 `Tower myTower;`와 `Tower seoulTower(100);`으로 객체를 만들므로 매개변수 없는 생성자와 `int` 하나를 받는 생성자가 모두 필요하다.

- **타겟 생성자 `Tower(int h)`:** 받은 값을 `height`에 저장한다.
- **기본 생성자 `Tower()`:** `Tower::Tower() : Tower(1){}`처럼 위임 생성자로 작성하여 타겟 생성자에 높이 1을 넘긴다. 초기화 코드가 한 곳에만 있게 된다([3장 4.3절](Concepts_Lecture.ko.md#43-위임-생성자)).
- **`getHeight()`:** `private` 멤버 `height`를 외부에서 읽을 수 있게 리턴하는 getter이다.

클래스 선언부에는 멤버 변수와 멤버 함수의 원형만 두고, 멤버 함수는 클래스 밖에서 `Tower::`를 붙여 정의하여 선언과 정의를 분리했다. `main()`은 문제에 주어진 내용 그대로이다.

**코드:** [Lab_Q1.cpp](../../Coursework/Lab03_Classes-and-Objects/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C03Q1.cpp
 * @brief   Q1. 클래스 연습
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-26
 */

#include <iostream>
using namespace std;

class Tower {
private:
    int height;
public:
    Tower();
    Tower(int h);
    int getHeight();
};

Tower::Tower() : Tower(1){}

Tower::Tower(int h) {
    height = h;
}

int Tower::getHeight() {
    return height;
}


int main() {
    Tower myTower;          // 1미터
    Tower seoulTower(100);  // 100미터
    cout << "높이는 " << myTower.getHeight() << "미터" << endl;
    cout << "높이는 " << seoulTower.getHeight() << "미터" << endl;
    
    return 0;
}
```

**실행 결과:**

```text
높이는 1미터
높이는 100미터
```

실행 예시의 마지막 줄 `Program ended with exit code: 0`은 프로그램의 출력이 아니라 Xcode가 프로그램이 끝난 뒤 표시하는 메시지이다.

---

<br>

## 연습 문제 2: 커피 머신

**교재 문항:** 3장 실습 문제 4번

**문제:** 커피, 물, 설탕의 양을 가진 `CoffeeMachine` 클래스를 작성하라. 에스프레소는 커피 1과 물 1, 아메리카노는 커피 1과 물 2, 설탕 커피는 커피 1, 물 2, 설탕 1을 소비하며, `fill()`은 재료를 채우고 `show()`는 현재 상태를 출력한다. 재료가 부족하면 오류 메시지와 현재 상태를 출력하고, 선언과 정의를 분리하며 모든 멤버 변수를 `private`으로 한다. 문제 전문과 실행 예시는 [연습 문제 2](Exercises.ko.md#연습-문제-2-커피-머신)에 있다.

### 풀이

커피 머신의 상태는 세 재료의 양이므로 `private` 멤버 변수 `coffeeAmount`, `waterAmount`, `sugarAmount`를 두고, 생성자 `CoffeeMachine(int c, int w, int s)`에서 초기화한다.

- **`drinkAmericano()`, `drinkSugarCoffee()`, `drinkEspresso()`:** 필요한 재료가 모두 충분한지 `&&`로 묶은 조건으로 먼저 검사한다. 충분하면 각 재료를 소비량만큼 줄이고, 하나라도 부족하면 재료를 줄이지 않은 채 오류 메시지를 출력하고 `show()`를 호출한다(요구사항 4). 메시지 앞의 `\n`이 실행 예시의 빈 줄을 만든다.
- **`show()`:** `커피머신 상태: 커피 = 4   물 = 6   설탕 = 2`처럼 실행 예시와 같은 형식(값 사이에 공백 세 칸)으로 출력한다.
- **`fill()`:** 세 재료를 모두 10으로 채운다.

`main()`의 호출에 따라 재료의 양은 다음과 같이 바뀐다.

| 호출 | 커피 | 물 | 설탕 | 결과 |
|:-----|:----:|:----:|:----:|:-----|
| `CoffeeMachine java(5, 8, 2)` | 5 | 8 | 2 | 초기화 |
| `drinkAmericano()` | 4 | 6 | 2 | 성공 |
| `drinkSugarCoffee()` | 3 | 4 | 1 | 성공 |
| `drinkEspresso()` | 2 | 3 | 1 | 성공 |
| `drinkSugarCoffee()` | 1 | 1 | 0 | 성공 |
| `drinkSugarCoffee()` | 1 | 1 | 0 | 실패: 물 2와 설탕 1이 필요하지만 물 1, 설탕 0 |
| `drinkAmericano()` | 1 | 1 | 0 | 실패: 물 2가 필요하지만 물 1 |
| `fill()` | 10 | 10 | 10 | 채움 |

*표 1. `main()`의 호출에 따른 재료 양의 변화*

**코드:** [Lab_Q2.cpp](../../Coursework/Lab03_Classes-and-Objects/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C03Q2.cpp
 * @brief   Q2. 커피 머신
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-26
 */

#include <iostream>
using namespace std;

class CoffeeMachine {
private:
    int coffeeAmount;
    int waterAmount;
    int sugarAmount;
public:
    CoffeeMachine(int c, int w, int s);
    void drinkAmericano();
    void drinkSugarCoffee();
    void drinkEspresso();
    void show();
    void fill();
};

CoffeeMachine::CoffeeMachine(int c, int w, int s) {
    coffeeAmount = c;
    waterAmount = w;
    sugarAmount = s;
}

void CoffeeMachine::drinkAmericano() {
    if (coffeeAmount >= 1 && waterAmount >= 2) {
        coffeeAmount -= 1;
        waterAmount -= 2;
    } else {
        cout << "\n재료가 부족하여 아메리카노를 만들 수 없습니다.\n";
        show();
    }
}

void CoffeeMachine::drinkSugarCoffee() {
    if (coffeeAmount >= 1 && waterAmount >= 2 && sugarAmount >= 1) {
        coffeeAmount -= 1;
        waterAmount -= 2;
        sugarAmount -= 1;
    } else {
        cout << "\n재료가 부족하여 설탕커피를 만들 수 없습니다.\n";
        show();
    }
}

void CoffeeMachine::drinkEspresso() {
    if (coffeeAmount >= 1 && waterAmount >= 1) {
        coffeeAmount -= 1;
        waterAmount -= 1;
    } else {
        cout << "\n재료가 부족하여 에스프레소를 만들 수 없습니다.\n";
        show();
    }
}

void CoffeeMachine::show() {
    cout << "커피머신 상태: 커피 = " << coffeeAmount << "   물 = " << waterAmount << "   설탕 = " << sugarAmount << endl;
}

void CoffeeMachine::fill() {
    coffeeAmount = 10;
    waterAmount = 10;
    sugarAmount = 10;
}


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

**실행 결과:**

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
```

실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 표시하는 메시지이다. 이 `main()`에서는 에스프레소의 재료가 부족한 경우가 나오지 않으므로 `drinkEspresso()`의 오류 메시지는 출력되지 않는다.

---

<br>

## 연습 문제 3: 클래스와 객체에 대한 설명

**교재 문항:** 3장 이론 문제 2번

**문제:** 클래스와 객체에 관한 네 설명 중 틀린 것을 고르라. 보기는 [연습 문제 3](Exercises.ko.md#연습-문제-3-클래스와-객체에-대한-설명)에 있다.

### 풀이

**정답:** ③

**해설:** 용도에 따라 다르지만, 일반적으로 멤버 변수는 캡슐화 원칙에 따라 `private`으로 두어 외부에서 직접 바꾸지 못하게 하고, 외부에 제공할 기능만 `public` 멤버 함수로 공개하는 것이 바람직하다([3장 2.4절](Concepts_Lecture.ko.md#24-접근-지정자)). 나머지 설명은 옳다. 클래스로부터 만들어진 객체를 실체 또는 인스턴스라 부르고(①), 클래스는 객체를 만드는 설계도이며(②), 클래스는 멤버 변수와 멤버 함수로 이루어진다(④).

---

<br>

## 연습 문제 4: 전역 변수와 전역 함수의 캡슐화

**교재 문항:** 3장 이론 문제 4번

**문제:** 전역 변수 `age`와 전역 함수 `older()`를 캡슐화하라. 문제 전문과 코드는 [연습 문제 4](Exercises.ko.md#연습-문제-4-전역-변수와-전역-함수의-캡슐화)에 있다.

### 풀이

**정답:** `age`와 `older()`를 하나의 클래스로 묶는다. `Circle` 클래스는 이미 캡슐화되어 있으므로 그대로 둔다.

```cpp
class Age {
    int age;
public:
    void older() {
        age++;
    }
    void setAge(int a) {
        age = a;
    }
    int getAge() {
        return age;
    }
};
```

**해설:** 캡슐화는 데이터와 그 데이터를 다루는 함수를 하나의 클래스로 묶고, 데이터를 외부의 접근으로부터 보호하는 것이다. 원래 코드의 `age`는 전역 변수이므로 프로그램의 어느 함수든 바꿀 수 있다. 클래스 `Age`로 묶으면 `age`는 접근 지정자가 없어 `private` 멤버가 되고, 외부는 `older()`, `setAge()`, `getAge()`를 통해서만 나이를 다룬다. `age`를 초기화하는 방법이 `setAge()`뿐이므로, 생성자를 두어 객체를 만들 때 나이를 정하게 하면 초기화하지 않은 값을 읽는 실수를 막을 수 있다.

---

<br>

## 연습 문제 5: `Tower` 클래스의 틀린 부분 수정

**교재 문항:** 3장 이론 문제 6번

**문제:** `int height = 20;`과 생성자 안의 `return;`이 있는 `Tower` 클래스의 틀린 부분을 수정하라. 문제 전문과 코드는 [연습 문제 5](Exercises.ko.md#연습-문제-5-tower-클래스의-틀린-부분-수정)에 있다.

### 풀이

**정답:**

```cpp
class Tower {
    int height;
public:
    Tower() { height = 10; }
};
```

**해설:** 클래스 선언부에서 멤버 변수를 `int height = 20;`처럼 초기화하는 것은 C++11 이전의 표준에서는 허용되지 않는다. 그래서 초기값은 생성자에서 정한다. 생성자는 리턴 타입이 없으므로 값을 리턴할 수 없다. 값 없는 `return;`은 생성자를 끝내는 문장으로 허용되지만, 몸체의 마지막에 두는 것은 의미가 없으므로 지운다.

> **참고:** C++11부터는 **멤버 초기화(default member initializer)** 가 허용되므로 원래 코드도 `-std=c++11` 이후에서는 컴파일된다. 이때 `height`는 먼저 20으로 초기화된 뒤 생성자 몸체에서 10으로 바뀌므로, 객체의 높이는 10이다. `-std=c++98`로 컴파일하면 Clang은 `default member initializer for non-static data member is a C++11 extension` 경고를 낸다([3장 4.4절](Concepts_Lecture.ko.md#44-멤버-변수의-초기화-방법)).

---

<br>

## 연습 문제 6: `Calendar` 클래스의 구현

**교재 문항:** 3장 이론 문제 8번

**문제:** `Calendar` 클래스의 생성자와 `getYear()`를 클래스 밖에서 구현하라. 문제 전문과 코드는 [연습 문제 6](Exercises.ko.md#연습-문제-6-calendar-클래스의-구현)에 있다.

### 풀이

**정답:**

```cpp
Calendar::Calendar() {
    year = 10;
}
int Calendar::getYear() {
    return year;
}
```

**해설:** 클래스 밖에서 멤버 함수를 정의할 때는 `Calendar::`처럼 클래스 이름과 범위 지정 연산자를 붙인다. 생성자는 리턴 타입 없이 `Calendar::Calendar()`로, `getYear()`는 선언과 같은 리턴 타입 `int`를 붙여 정의한다. 생성자는 `Calendar::Calendar() : year(10) {}`처럼 멤버 초기화 리스트로 쓸 수도 있다.

---

<br>

## 연습 문제 7: 소멸자에 대한 설명

**교재 문항:** 3장 이론 문제 10번

**문제:** 소멸자에 관한 설명에서 틀린 부분을 지적하라. 보기는 [연습 문제 7](Exercises.ko.md#연습-문제-7-소멸자에-대한-설명)에 있다.

### 풀이

**정답:** ③

**해설:** 소멸자는 매개변수를 가질 수 없고, 리턴 타입도 없으며, `~클래스이름()` 형태 하나만 선언할 수 있다. 따라서 소멸자는 중복할 수 없다. 객체가 소멸될 때 자동으로 호출되므로 호출하는 쪽이 값을 전달할 방법이 없고 그럴 필요도 없다([3장 4.6절](Concepts_Lecture.ko.md#46-소멸자)). 나머지 부분은 옳다. 소멸자를 선언하지 않으면 컴파일러가 아무 일도 하지 않는 기본 소멸자를 만든다(④).

---

<br>

## 연습 문제 8: 객체의 생성과 소멸 순서

**교재 문항:** 3장 이론 문제 12번

**문제:** 전역 객체 `c`, 함수 `f()`의 지역 객체 `b`, 멤버 함수 `test()`의 지역 객체 `a`가 생성되고 소멸되는 순서를 쓰라. 문제 전문과 코드는 [연습 문제 8](Exercises.ko.md#연습-문제-8-객체의-생성과-소멸-순서)에 있다.

### 풀이

**정답:** `c` 생성, `b` 생성, `a` 생성, `a` 소멸, `b` 소멸, `c` 소멸

**해설:** 전역 객체는 `main()`이 시작되기 전에 생성되고 `main()`이 끝난 뒤 소멸된다. 지역 객체는 선언문이 실행될 때 생성되고, 그 객체가 속한 함수가 끝날 때 소멸된다([3장 4.7절](Concepts_Lecture.ko.md#47-실행-순서)).

| 시점 | 일어나는 일 |
|:-----|:-----|
| `main()` 시작 전 | 전역 객체 `c` 생성 |
| `f()`의 첫 문장 | `b` 생성 |
| `b.test()`의 첫 문장 | `a` 생성 |
| `test()`의 끝 | `a` 소멸 |
| `f()`의 끝 | `b` 소멸 |
| `main()` 종료 뒤 | `c` 소멸 |

*표 2. 연습 문제 8의 객체 생성과 소멸 시점*

---

<br>

## 연습 문제 9: `private` 멤버에 접근하는 오류

**교재 문항:** 3장 이론 문제 14번

**문제:** `private` 멤버 `channels`에 `main()`이 직접 값을 대입하는 프로그램의 오류를 지적하고 수정하라. 문제 전문과 코드는 [연습 문제 9](Exercises.ko.md#연습-문제-9-private-멤버에-접근하는-오류)에 있다.

### 풀이

**정답:** `LG.channels = 200;`이 오류이다. `channels`는 접근 지정자 없이 선언되었으므로 `private` 멤버이고, 클래스 밖인 `main()`에서 접근할 수 없다. `LG.colors = 60000;`은 `colors`가 `public`이므로 문제가 없다.

```cpp
class TV {
    int channels;
public:
    int colors;
    TV() { channels = 256; }
    TV(int a, int b) { channels = a; colors = b; }
    void setChannels(int c) { channels = c; }
};
int main() {
    TV LG;
    LG.setChannels(200);
    LG.colors = 60000;
    TV Samsung(100, 50000);
}
```

**해설:** `channels`를 바꾸려면 `public` 멤버 함수 `setChannels()`를 추가하고 `main()`에서 이를 호출한다. `channels`를 `public`으로 옮겨도 컴파일은 되지만, 그러면 캡슐화가 깨진다. 이 프로그램에서 고칠 곳은 이 한 곳뿐이며, 두 생성자는 `public`이므로 `TV LG;`와 `TV Samsung(100, 50000);`은 문제가 없다.

---

<br>

## 연습 문제 10: 인라인 함수의 장단점

**교재 문항:** 3장 이론 문제 16번

**문제:** 인라인 함수의 장단점에 관한 설명 중 옳은 것을 고르라. 보기는 [연습 문제 10](Exercises.ko.md#연습-문제-10-인라인-함수의-장단점)에 있다.

### 풀이

**정답:** ②

**해설:** 인라인 함수는 호출하는 곳에 함수의 코드를 확장하여 넣으므로, 함수 호출에 따르는 오버헤드(인자 전달, 복귀 주소 저장, 점프)가 없어져 실행 속도가 빨라진다([3장 5절](Concepts_Lecture.ko.md#5-인라인-함수)). 대신 호출하는 곳마다 코드가 복사되므로 프로그램의 크기는 커지며(④는 틀림), 컴파일러가 확장 작업을 더 해야 하므로 컴파일 속도가 빨라지지 않는다(①은 틀림). 작성 시간과는 관계가 없다(③은 틀림).

---

<br>

## 연습 문제 11: 인라인으로 처리하기에 알맞은 함수

**교재 문항:** 3장 이론 문제 18번

**문제:** 네 함수 중 컴파일러가 인라인으로 처리하기에 가장 바람직한 것을 고르라. 문제 전문과 코드는 [연습 문제 11](Exercises.ko.md#연습-문제-11-인라인으로-처리하기에-알맞은-함수)에 있다.

### 풀이

**정답:** ①

**해설:** 인라인 확장은 코드가 짧고 자주 호출되는 함수에 적합하다. ①의 `big()`은 조건 연산자 하나로 이루어진 짧은 함수이므로 가장 알맞다. ②는 자기 자신을 호출하는 재귀 함수이므로 호출하는 곳에 몸체를 끝없이 펼칠 수 없고, ③은 반복문을 가져 몸체가 길고 실행 시간에 비해 호출 오버헤드의 비중이 작다. ④는 `static` 지역 변수를 가지며, 이런 함수도 표준상 인라인으로 선언할 수는 있지만 호출마다 같은 변수를 공유해야 하므로 확장의 이득이 적다. `inline`은 컴파일러에 대한 요청일 뿐이므로, 컴파일러는 ②~④ 같은 함수를 인라인으로 처리하지 않을 수 있다.

---

<br>

## 연습 문제 12: 구조체를 클래스로 바꾸기

**교재 문항:** 3장 이론 문제 20번

**문제:** `struct Family`를 같은 의미의 클래스로 작성하라. 문제 전문과 코드는 [연습 문제 12](Exercises.ko.md#연습-문제-12-구조체를-클래스로-바꾸기)에 있다.

### 풀이

**정답:**

```cpp
class Family {
    char tel[11];
public:
    int count;
    char address[20];
    Family();
};
```

**해설:** C++ 구조체와 클래스의 차이는 기본 접근 지정뿐이다. 구조체는 접근 지정자가 없는 멤버를 `public`으로, 클래스는 `private`으로 취급한다([3장 6절](Concepts_Lecture.ko.md#6-c-구조체)). 구조체의 `count`와 `address`는 접근 지정자 앞에 있어 `public`이므로, 클래스에서는 `public:` 아래에 둔다. `tel`은 구조체에서 `private`이었으므로 클래스에서는 접근 지정자 없이 맨 앞에 두면 된다.
