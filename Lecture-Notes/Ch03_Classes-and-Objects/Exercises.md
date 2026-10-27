# Chapter 3 Exercises — Classes and Objects

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 3

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

Exercises 1 and 2 are lab problems, and Exercises 3-12 are the even-numbered theory problems (2-20) of Chapter 3 of the textbook.

---

<br>

## Table of Contents

- [Exercise 1: Class Practice](#exercise-1-class-practice)
- [Exercise 2: Coffee Machine](#exercise-2-coffee-machine)
- [Exercise 3: Statements About Classes and Objects](#exercise-3-statements-about-classes-and-objects)
- [Exercise 4: Encapsulating a Global Variable and a Global Function](#exercise-4-encapsulating-a-global-variable-and-a-global-function)
- [Exercise 5: Correcting the `Tower` Class](#exercise-5-correcting-the-tower-class)
- [Exercise 6: Implementing the `Calendar` Class](#exercise-6-implementing-the-calendar-class)
- [Exercise 7: Statements About Destructors](#exercise-7-statements-about-destructors)
- [Exercise 8: The Order of Object Creation and Destruction](#exercise-8-the-order-of-object-creation-and-destruction)
- [Exercise 9: An Error in Accessing a `private` Member](#exercise-9-an-error-in-accessing-a-private-member)
- [Exercise 10: Advantages and Disadvantages of Inline Functions](#exercise-10-advantages-and-disadvantages-of-inline-functions)
- [Exercise 11: The Function Best Suited to Inlining](#exercise-11-the-function-best-suited-to-inlining)
- [Exercise 12: Rewriting a Structure as a Class](#exercise-12-rewriting-a-structure-as-a-class)

---

<br>

## Exercise 1: Class Practice

**Textbook Problem:** Chapter 3 programming exercise 1

**Problem:** Referring to the example code, write the `Tower` class so that the output of the `main()` function matches the sample run.

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

**Textbook Problem:** Chapter 3 programming exercise 4

**Problem:** Referring to the example code, write the `CoffeeMachine` class so that the output of the `main()` function matches the sample run. One espresso consumes 1 unit each of coffee and water, an Americano consumes 1 unit of coffee and 2 of water, and a sugar coffee consumes 1 unit of coffee, 2 of water, and 1 of sugar.

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

---

<br>

## Exercise 3: Statements About Classes and Objects

**Textbook Problem:** Chapter 3 theory problem 2

**Problem:** Which statement about classes and objects is wrong?

- ① An object is also called an instance.
- ② A class is like a blueprint or a mold for creating objects.
- ③ For the members of a class, `public` access is preferable to `private`.
- ④ A class consists of function members and variable members.

**[Solution](Solutions.md#exercise-3-statements-about-classes-and-objects)**

---

<br>

## Exercise 4: Encapsulating a Global Variable and a Global Function

**Textbook Problem:** Chapter 3 theory problem 4

**Problem:** Fix the parts of the following C++ program that lack encapsulation so that it is encapsulated.

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

**[Solution](Solutions.md#exercise-4-encapsulating-a-global-variable-and-a-global-function)**

---

<br>

## Exercise 5: Correcting the `Tower` Class

**Textbook Problem:** Chapter 3 theory problem 6

**Problem:** The following code is an example of writing a `Tower` class. Fix the wrong parts.

```cpp
class Tower {
    int height = 20;
public:
    Tower() { height = 10; return; }
};
```

**[Solution](Solutions.md#exercise-5-correcting-the-tower-class)**

---

<br>

## Exercise 6: Implementing the `Calendar` Class

**Textbook Problem:** Chapter 3 theory problem 8

**Problem:** The following is the declaration of the `Calendar` class. Implement the constructor that initializes `year` to 10 and `getYear()`, which returns the value of `year`.

```cpp
class Calendar {
private:
    int year;
public:
    Calendar();
    int getYear();
};
```

**[Solution](Solutions.md#exercise-6-implementing-the-calendar-class)**

---

<br>

## Exercise 7: Statements About Destructors

**Textbook Problem:** Chapter 3 theory problem 10

**Problem:** The following is a description of destructors. Point out which of the underlined parts ①-④ is wrong.

A destructor is ① a member function called automatically when an object is destroyed, and ② it must be declared with the name of the class preceded by `~`. ③ A destructor with parameters can be written to pass meaningful values at destruction, and if no destructor is declared, ④ a default destructor is generated automatically.

**[Solution](Solutions.md#exercise-7-statements-about-destructors)**

---

<br>

## Exercise 8: The Order of Object Creation and Destruction

**Textbook Problem:** Chapter 3 theory problem 12

**Problem:** In the following program, in what order are the objects `a`, `b`, and `c` created and destroyed?

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

**[Solution](Solutions.md#exercise-8-the-order-of-object-creation-and-destruction)**

---

<br>

## Exercise 9: An Error in Accessing a `private` Member

**Textbook Problem:** Chapter 3 theory problem 14

**Problem:** Point out the error in the following program and fix it.

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

**[Solution](Solutions.md#exercise-9-an-error-in-accessing-a-private-member)**

---

<br>

## Exercise 10: Advantages and Disadvantages of Inline Functions

**Textbook Problem:** Chapter 3 theory problem 16

**Problem:** Which statement about the advantages and disadvantages of inline functions is correct?

- ① Using inline functions improves the compilation speed.
- ② Using inline functions improves the execution speed of the program.
- ③ Using inline functions improves the time needed to write the program.
- ④ Using inline functions is effective because the program becomes smaller.

**[Solution](Solutions.md#exercise-10-advantages-and-disadvantages-of-inline-functions)**

---

<br>

## Exercise 11: The Function Best Suited to Inlining

**Textbook Problem:** Chapter 3 theory problem 18

**Problem:** An `inline` declaration is not binding. Which of the following functions is most suitable for the compiler to handle inline?

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

**[Solution](Solutions.md#exercise-11-the-function-best-suited-to-inlining)**

---

<br>

## Exercise 12: Rewriting a Structure as a Class

**Textbook Problem:** Chapter 3 theory problem 20

**Problem:** Write the following C++ structure as a class with the same meaning.

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

**[Solution](Solutions.md#exercise-12-rewriting-a-structure-as-a-class)**
