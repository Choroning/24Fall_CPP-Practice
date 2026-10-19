# Chapter 3 Solutions — Classes and Objects

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 3

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Class Practice](#exercise-1-class-practice)
- [Exercise 2: Coffee Machine](#exercise-2-coffee-machine)

---

<br>

## Exercise 1: Class Practice

**Textbook:** Chapter 3 Programming Exercise 1

**Problem:** In the given `main()`, `Tower myTower;` creates a tower object 1 meter high and `Tower seoulTower(100);` creates one 100 meters high, and `getHeight()` prints each height. Write the `Tower` class with its declaration and definition separated and every member variable `private`.

### Solution

The only state a `Tower` object has is its height, so it has a `private` member variable `int height`. Because `main()` creates objects with `Tower myTower;` and `Tower seoulTower(100);`, both a parameterless constructor and a constructor taking one `int` are needed.

- **Target constructor `Tower(int h)`:** Stores the received value in `height`.
- **Default constructor `Tower()`:** Written as a delegating constructor, `Tower::Tower() : Tower(1){}`, which passes the height 1 to the target constructor. The initialization code then exists in only one place ([Chapter 3, Section 4.3](Concepts_Lecture.md#43-delegating-constructors)).
- **`getHeight()`:** A getter that returns the `private` member `height` so that it can be read from outside.

The class declaration contains only the member variable and the member function prototypes, and the member functions are defined outside the class with `Tower::`, separating the declaration from the definition. `main()` is exactly as given in the problem.

**Code:** [Lab_Q1.cpp](../../Coursework/Lab03_Classes-and-Objects/Lab_Q1.cpp)

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

**Output:**

```text
높이는 1미터
높이는 100미터
```

The program compiles without warnings with `g++ -std=c++17 -Wall`, and its output, which reads "The height is 1 meter" and "The height is 100 meters" in Korean, matches the sample run. The last line of the sample run, `Program ended with exit code: 0`, is not program output but a message Xcode displays after the program ends.

---

<br>

## Exercise 2: Coffee Machine

**Textbook:** Chapter 3 Programming Exercise 4

**Problem:** Write a `CoffeeMachine` class that holds amounts of coffee, water, and sugar. An espresso consumes 1 coffee and 1 water, an Americano 1 coffee and 2 water, and a sugar coffee 1 coffee, 2 water, and 1 sugar; `fill()` refills the ingredients, and `show()` prints the current state. If the ingredients are insufficient, print an error message and the current state; separate the declaration from the definition and make every member variable `private`.

### Solution

The state of the coffee machine is the amounts of the three ingredients, so it has the `private` member variables `coffeeAmount`, `waterAmount`, and `sugarAmount`, initialized in the constructor `CoffeeMachine(int c, int w, int s)`.

- **`drinkAmericano()`, `drinkSugarCoffee()`, `drinkEspresso()`:** Each first checks, with a condition joined by `&&`, that every required ingredient is sufficient. If so, it reduces each ingredient by its consumption; if any is short, it prints an error message without reducing anything and calls `show()` (requirement 4). The `\n` before the message produces the blank line in the sample run.
- **`show()`:** Prints in the same format as the sample run, such as `커피머신 상태: 커피 = 4   물 = 6   설탕 = 2` ("Coffee machine state: coffee = 4, water = 6, sugar = 2"), with three spaces between values.
- **`fill()`:** Fills all three ingredients to 10.

The amounts change as follows with the calls in `main()`.

| Call | Coffee | Water | Sugar | Result |
|:-----|:----:|:----:|:----:|:-----|
| `CoffeeMachine java(5, 8, 2)` | 5 | 8 | 2 | Initialized |
| `drinkAmericano()` | 4 | 6 | 2 | Success |
| `drinkSugarCoffee()` | 3 | 4 | 1 | Success |
| `drinkEspresso()` | 2 | 3 | 1 | Success |
| `drinkSugarCoffee()` | 1 | 1 | 0 | Success |
| `drinkSugarCoffee()` | 1 | 1 | 0 | Failure: needs 2 water and 1 sugar, has 1 water and 0 sugar |
| `drinkAmericano()` | 1 | 1 | 0 | Failure: needs 2 water, has 1 water |
| `fill()` | 10 | 10 | 10 | Refilled |

*Table 1. Changes in ingredient amounts with the calls in `main()`*

**Code:** [Lab_Q2.cpp](../../Coursework/Lab03_Classes-and-Objects/Lab_Q2.cpp)

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

**Output:**

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

The program compiles without warnings with `g++ -std=c++17 -Wall`, and its output matches the sample run; the error messages read "Cannot make a sugar coffee because of insufficient ingredients" and "Cannot make an Americano because of insufficient ingredients" in Korean. The last line of the sample run, `Program ended with exit code: 0`, is a message displayed by Xcode. This `main()` never runs short of espresso ingredients, so the error message of `drinkEspresso()` is not printed.
