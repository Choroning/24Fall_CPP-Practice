# Chapter 3 Solutions — Classes and Objects

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 3

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Class Practice](#exercise-1-class-practice)
- [Exercise 2: Coffee Machine](#exercise-2-coffee-machine)
- [Textbook Theory Problems](#textbook-theory-problems)

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

The output reads "The height is 1 meter" and "The height is 100 meters" in Korean. The last line of the sample run, `Program ended with exit code: 0`, is not program output but a message Xcode displays after the program ends.

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

The error messages in the output read "Cannot make a sugar coffee because of insufficient ingredients" and "Cannot make an Americano because of insufficient ingredients" in Korean. The last line of the sample run, `Program ended with exit code: 0`, is a message displayed by Xcode. This `main()` never runs short of espresso ingredients, so the error message of `drinkEspresso()` is not printed.

---

<br>

## Textbook Theory Problems

**Textbook:** Even-numbered theory problems of the Chapter 3 exercises

### Theory Problem 2

**Problem:** Choose the wrong statement among four statements about classes and objects.

**Answer:** ③

It depends on the use, but in general member variables should be `private`, following the principle of encapsulation, so that outside code cannot change them directly, and only the functionality to be offered to the outside should be made public as `public` member functions ([Chapter 3, Section 2.4](Concepts_Lecture.md#24-access-specifiers)). The other statements are correct. An object made from a class is called an entity or an instance (①), a class is a blueprint for making objects (②), and a class consists of member variables and member functions (④).

### Theory Problem 4

**Problem:** Encapsulate the global variable `age` and the global function `older()`.

**Answer:** Group `age` and `older()` into one class. The `Circle` class is already encapsulated, so it stays as it is.

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

Encapsulation means grouping data and the functions that handle the data into one class and protecting the data from outside access. In the original code `age` is a global variable, so any function of the program can change it. Grouped into the class `Age`, `age` has no access specifier and becomes a `private` member, and outside code handles the age only through `older()`, `setAge()`, and `getAge()`. Since `setAge()` is the only way to initialize `age`, adding a constructor that sets the age when an object is created prevents the mistake of reading an uninitialized value.

### Theory Problem 6

**Problem:** Fix the wrong parts of a `Tower` class with `int height = 20;` and a `return;` inside the constructor.

**Answer:**

```cpp
class Tower {
    int height;
public:
    Tower() { height = 10; }
};
```

Initializing a member variable in the class declaration, as in `int height = 20;`, is not allowed by the standards before C++11. The initial value is therefore set in the constructor. A constructor has no return type, so it cannot return a value. A `return;` without a value is allowed as a statement that ends the constructor, but at the end of the body it has no meaning, so it is removed.

> **Note:** Since C++11, a **default member initializer** is allowed, so the original code also compiles with `-std=c++11` and later. In that case `height` is first initialized to 20 and then changed to 10 in the constructor body, so the height of the object is 10. Compiling with `-std=c++98` makes Clang warn `default member initializer for non-static data member is a C++11 extension` ([Chapter 3, Section 4.4](Concepts_Lecture.md#44-ways-to-initialize-member-variables)).

### Theory Problem 8

**Problem:** Implement the constructor and `getYear()` of the `Calendar` class outside the class.

**Answer:**

```cpp
Calendar::Calendar() {
    year = 10;
}
int Calendar::getYear() {
    return year;
}
```

When a member function is defined outside the class, the class name and the scope resolution operator are attached, as in `Calendar::`. The constructor is defined without a return type as `Calendar::Calendar()`, and `getYear()` with the same return type as its declaration, `int`. The constructor can also be written with a member initializer list, as in `Calendar::Calendar() : year(10) {}`.

### Theory Problem 10

**Problem:** Point out the wrong part of the description of destructors.

**Answer:** ③

A destructor cannot have parameters, has no return type, and can be declared only in the single form `~ClassName()`. Destructors therefore cannot be overloaded. A destructor is called automatically when an object is destroyed, so the caller has no way to pass values, and there is no need to ([Chapter 3, Section 4.6](Concepts_Lecture.md#46-destructor)). The other parts are correct. If no destructor is declared, the compiler generates a default destructor that does nothing (④).

### Theory Problem 12

**Problem:** Give the order in which the global object `c`, the local object `b` of the function `f()`, and the local object `a` of the member function `test()` are created and destroyed.

**Answer:** `c` created, `b` created, `a` created, `a` destroyed, `b` destroyed, `c` destroyed

A global object is created before `main()` starts and destroyed after `main()` ends. A local object is created when its declaration statement runs and destroyed when the function it belongs to ends ([Chapter 3, Section 4.7](Concepts_Lecture.md#47-execution-order)).

| When | What happens |
|:-----|:-----|
| Before `main()` starts | Global object `c` created |
| First statement of `f()` | `b` created |
| First statement of `b.test()` | `a` created |
| End of `test()` | `a` destroyed |
| End of `f()` | `b` destroyed |
| After `main()` ends | `c` destroyed |

*Table 2. When the objects of theory problem 12 are created and destroyed*

### Theory Problem 14

**Problem:** Point out and fix the error in a program in which `main()` assigns a value directly to the `private` member `channels`.

**Answer:** `LG.channels = 200;` is the error. `channels` is declared without an access specifier, so it is a `private` member and cannot be accessed from `main()`, which is outside the class. `LG.colors = 60000;` is fine because `colors` is `public`.

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

To change `channels`, add the `public` member function `setChannels()` and call it from `main()`. Moving `channels` to `public` would also compile, but it would break encapsulation. This is the only place to fix in this program; both constructors are `public`, so `TV LG;` and `TV Samsung(100, 50000);` are fine.

### Theory Problem 16

**Problem:** Choose the correct statement about the advantages and disadvantages of inline functions.

**Answer:** ②

An inline function inserts its code at the place of each call, so the overhead of a function call (passing arguments, saving the return address, jumping) disappears and the program runs faster ([Chapter 3, Section 5](Concepts_Lecture.md#5-inline-functions)). In exchange, the code is copied at every call site, so the program becomes larger (④ is wrong), and the compiler has more expansion work to do, so compilation does not become faster (① is wrong). It has nothing to do with the time needed to write the program (③ is wrong).

### Theory Problem 18

**Problem:** Choose the function among the four that is most suitable for the compiler to handle inline.

**Answer:** ①

Inline expansion suits functions whose code is short and which are called often. `big()` in ① is a short function made of a single conditional operator, so it is the most suitable. ② is a recursive function that calls itself, so its body cannot be expanded endlessly at the call site, and ③ has a loop, so its body is long and the call overhead is small compared with the execution time. ④ has a `static` local variable; such a function can be declared inline under the standard, but every call must share the same variable, so expansion gains little. `inline` is only a request to the compiler, so the compiler may decline to handle functions such as ②-④ inline.

### Theory Problem 20

**Problem:** Write `struct Family` as a class with the same meaning.

**Answer:**

```cpp
class Family {
    char tel[11];
public:
    int count;
    char address[20];
    Family();
};
```

The only difference between a C++ structure and a class is the default access. A structure treats members without an access specifier as `public`, and a class treats them as `private` ([Chapter 3, Section 6](Concepts_Lecture.md#6-c-struct)). `count` and `address` in the structure come before any access specifier and are `public`, so in the class they go under `public:`. `tel` was `private` in the structure, so in the class it can simply go first, without an access specifier.
