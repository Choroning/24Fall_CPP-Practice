# Chapter 3 Exercises — Classes and Objects

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 3

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Exercise 1: Class Practice

**Textbook:** Chapter 3 Programming Exercise 1

Referring to the example code, write the `Tower` class so that the output of the `main()` function matches the sample run.

**Program Requirements**

1. The contents of the `main()` function given in the example code must not be changed.
2. Write the declaration and the definition of the class separately.
3. Set the access control of every member variable to `private`.

**Example Code**

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

**Sample Run**

```text
높이는 1미터
높이는 100미터

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-class-practice)**

---

<br>

## Exercise 2: Coffee Machine

**Textbook:** Chapter 3 Programming Exercise 4

Referring to the example code, write the `CoffeeMachine` class so that the output of the `main()` function matches the sample run. One espresso consumes 1 unit each of coffee and water, an Americano consumes 1 unit of coffee and 2 of water, and a sugar coffee consumes 1 unit of coffee, 2 of water, and 1 of sugar.

**Program Requirements**

1. The contents of the `main()` function given in the example code must not be changed.
2. Write the declaration and the definition of the class separately.
3. Set the access control of every member variable to `private`.
4. If the ingredients are insufficient, print an error message and then print the current state of the coffee machine.

**Example Code**

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

**Sample Run**

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

**[Solution](Solutions.md#exercise-2-coffee-machine)**
