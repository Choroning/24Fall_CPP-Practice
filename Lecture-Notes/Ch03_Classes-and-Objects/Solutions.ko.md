# 3장 연습 문제 풀이 — 클래스와 객체

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 3

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 클래스 연습](#연습-문제-1-클래스-연습)
- [연습 문제 2: 커피 머신](#연습-문제-2-커피-머신)

---

<br>

## 연습 문제 1: 클래스 연습

**교재:** 3장 실습 문제 1번

**문제:** 주어진 `main()`에서 `Tower myTower;`는 높이 1미터, `Tower seoulTower(100);`은 높이 100미터인 탑 객체를 만들고 `getHeight()`로 높이를 출력한다. 선언과 정의를 분리하고 모든 멤버 변수를 `private`으로 하여 `Tower` 클래스를 작성하라.

### 풀이

`Tower` 객체가 가지는 상태는 높이 하나이므로 `private` 멤버 변수 `int height`를 둔다. `main()`이 `Tower myTower;`와 `Tower seoulTower(100);`으로 객체를 만들므로 매개 변수 없는 생성자와 `int` 하나를 받는 생성자가 모두 필요하다.

- **타겟 생성자 `Tower(int h)`:** 받은 값을 `height`에 저장한다.
- **기본 생성자 `Tower()`:** `Tower::Tower() : Tower(1){}`처럼 위임 생성자로 작성하여 타겟 생성자에 높이 1을 넘긴다. 초기화 코드가 한 곳에만 있게 된다([3장 4.3절](Concepts_Lecture.ko.md#43-위임-생성자)).
- **`getHeight()`:** `private` 멤버 `height`를 외부에서 읽을 수 있게 반환하는 getter이다.

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

`g++ -std=c++17 -Wall`로 경고 없이 컴파일되며, 실행 결과는 실행 예시와 같다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 프로그램의 출력이 아니라 Xcode가 프로그램이 끝난 뒤 표시하는 메시지이다.

---

<br>

## 연습 문제 2: 커피 머신

**교재:** 3장 실습 문제 4번

**문제:** 커피, 물, 설탕의 양을 가진 `CoffeeMachine` 클래스를 작성하라. 에스프레소는 커피 1과 물 1, 아메리카노는 커피 1과 물 2, 설탕 커피는 커피 1, 물 2, 설탕 1을 소비하며, `fill()`은 재료를 채우고 `show()`는 현재 상태를 출력한다. 재료가 부족하면 오류 메시지와 현재 상태를 출력하고, 선언과 정의를 분리하며 모든 멤버 변수를 `private`으로 한다.

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

`g++ -std=c++17 -Wall`로 경고 없이 컴파일되며, 실행 결과는 실행 예시와 같다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 표시하는 메시지이다. 이 `main()`에서는 에스프레소의 재료가 부족한 경우가 나오지 않으므로 `drinkEspresso()`의 오류 메시지는 출력되지 않는다.
