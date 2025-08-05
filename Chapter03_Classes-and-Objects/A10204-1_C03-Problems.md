# Chapter 3. 클래스 연습

**문항 출처:** *임영희 교수, 컴퓨터언어Ⅱ 교수자 (DCCS102-04)* <br>
**Source:** *Prof. Younghee&nbsp;Im, Instructor of COMPUTER LANGUAGE Ⅱ (DCCS102-04)*

---

## Q1. 클래스 연습

예시 코드를 참고하여 `main()`함수의 실행 결과가 실행 예시와 같도록 `Tower` 클래스를 작성하시오.


<br>**프로그램 요구사항**

1. 예시 코드에 주어진 `main()`함수의 내용은 변경할 수 없습니다.
2. 클래스의 선언과 정의를 분리하여 작성하시오.
3. 모든 멤버 변수의 접근 제어는 `private`로 설정하시오.


<br>**예시 코드**

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


<br>**실행 예시**

```text
높이는 1미터
높이는 100미터

Program ended with exit code: 0
```



---

## Q2. 커피 머신

예시 코드를 참고하여 `main()`함수의 실행 결과가 실행 예시와 같도록 `CoffeeMachine` 클래스를 작성하시오. 에스프레소 한 잔에는 커피와 물이 각각 1씩 소비되고, 아메리카노의 경우 커피는 1, 물은 2가 소비되고, 설탕 커피는 커피 1, 물 2, 설탕 1이 소비된다.


<br>**프로그램 요구사항**

1. 예시 코드에 주어진 `main()`함수의 내용은 변경할 수 없습니다.
2. 클래스의 선언과 정의를 분리하여 작성하시오.
3. 모든 멤버 변수의 접근 제어는 `private`로 설정하시오.
4. 재료가 부족하면 오류 메시지를 출력하고, 현재 커피 상태를 출력하시오.


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

int main() {
   CoffeeMachine java(5, 8, 2);  // 커피 5, 물 8, 설탕 2로 초기화

   java.drinkAmericano();
   java.show()

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


<br>**실행 예시**

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



---
