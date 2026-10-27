# Chapter 4 Exercises — Using Objects

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

Exercises 1 and 2 are lab problems, and Exercises 3-11 are the even-numbered theory problems (2-18) of Chapter 4 of the textbook.

---

<br>

## Table of Contents

- [Exercise 1: Class with a Dynamic Array](#exercise-1-class-with-a-dynamic-array)
- [Exercise 2: Dynamic Creation and Destruction of Objects](#exercise-2-dynamic-creation-and-destruction-of-objects)
- [Exercise 3: Creating and Returning a `Rect` Object Dynamically](#exercise-3-creating-and-returning-a-rect-object-dynamically)
- [Exercise 4: Creating an Array of `Rect` Objects Dynamically](#exercise-4-creating-an-array-of-rect-objects-dynamically)
- [Exercise 5: Calling Member Functions Through an Object Pointer](#exercise-5-calling-member-functions-through-an-object-pointer)
- [Exercise 6: Creating and Returning a `Palette` Object Dynamically](#exercise-6-creating-and-returning-a-palette-object-dynamically)
- [Exercise 7: Returning an Object Array with `delete`](#exercise-7-returning-an-object-array-with-delete)
- [Exercise 8: Uses of `this`](#exercise-8-uses-of-this)
- [Exercise 9: Situations That Cause Memory Leaks](#exercise-9-situations-that-cause-memory-leaks)
- [Exercise 10: The Header File of the `string` Class](#exercise-10-the-header-file-of-the-string-class)
- [Exercise 11: C-Strings and the `string` Class](#exercise-11-c-strings-and-the-string-class)

---

<br>

## Exercise 1: Class with a Dynamic Array

**Textbook Problem:** Chapter 4 programming exercise 4

**Problem:** Referring to the example code and the sample run, complete the `Sample` class so that the `main()` function runs correctly.

**Program Requirements:**

- Use `try-catch` statements in the `main()` function and the `Sample` class so that no error occurs even in the following situations.
   - Dynamic allocation fails.
   - The integer array size entered by the user is less than or equal to 0.

**Example Code:**

```cpp
#include <iostream>
using namespace std;

class Sample {
   int *p = nullptr;
   int size;
public:
   Sample(int n) {     // 생성자
      size = n;
      p = new int [size];
   }
   void read();        // 동적 할당한 정수 배열 p에 사용자로부터 정수 입력
   void write();       // 정수 배열의 내용 출력
   int big();          // 정수 배열에서 가장 큰 수를 반환
   ~Sample();          // 소멸자
};

int main() {
   int input;
    
   cout << "생성할 정수 배열 사이즈를 입력하세요: ";
   cin >> input;
   Sample s(input);     // 크기 input의 정수 배열을 가진 Sample 객체 생성
   s.read();            // 사용자로부터 정수 배열을 입력받아 저장
   s.write();           // 정수 배열의 내용 출력
   cout << "가장 큰 수는 " << s.big() << endl;
    
   return 0;
}
```

**Sample Run:**

```text
생성할 정수 배열 사이즈를 입력하세요: 10
100 4 -2 9 55 300 44 38 99 -500
가장 큰 수는 300

Program ended with exit code: 0
```

```text
생성할 정수 배열 사이즈를 입력하세요: 0
[오류] 사이즈가 너무 작습니다!

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-class-with-a-dynamic-array)**

---

<br>

## Exercise 2: Dynamic Creation and Destruction of Objects

**Textbook Problem:** Chapter 4 programming exercise 12

**Problem:** Referring to the example code, complete the `Circle` and `CircleManager` classes and write the `main()` function so that the program runs exactly as in the sample run.

**Program Requirements:**

- The given declarations of the `Circle` and `CircleManager` classes in the example code must not be changed.
- Use `try-catch` statements in the `main()` function and the `Circle` and `CircleManager` class declarations so that no error occurs even in the following situations.
   - Dynamic allocation fails.
   - The number of circles entered by the user is less than or equal to 0.

**Example Code:**

```cpp
#include <iostream>
#define PI 3.14
using namespace std;

class Circle {
   int radius;    // 원의 반지름
   string name;   // 원의 이름
public:
   void setCircle(string name, int radius);  // 이름과 반지름 설정
   double getArea();
   string getName();
};

class CircleManager {
   Circle *p;     // Circle 배열에 대한 포인터
   int size;      // 배열의 크기
public:
   CircleManager(int size);   // 사용자로부터 입력받은 size 값 크기의 배열을 동적 생성
   ~CircleManager();
   void searchByName(); // 사용자로부터 원의 이름을 입력받아 면적 출력
   void searchByArea(); // 사용자로부터 면적을 입력받아 면적보다 큰 원의 이름 출력
};
```

**Sample Run:**

```text
원의 개수 >> 4
원 1의 이름과 반지름 >> 빈대떡 10
원 2의 이름과 반지름 >> 도넛 2
원 3의 이름과 반지름 >> 초코파이 1
원 4의 이름과 반지름 >> 피자 15
검색하고자 하는 원의 이름 >> 도넛
도넛의 면적은 12.56
최소 면적을 정수로 입력하세요 >> 10
10보다 큰 원을 검색합니다.
빈대떡의 면적은 314, 도넛의 면적은 12.56, 피자의 면적은 706.5,

Program ended with exit code: 0
```

```text
원의 개수 >> -1
[오류] 사이즈가 너무 작습니다!

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-2-dynamic-creation-and-destruction-of-objects)**

---

<br>

## Exercise 3: Creating and Returning a `Rect` Object Dynamically

**Textbook Problem:** Chapter 4 theory problem 2

Theory problems 1-5 of the textbook use the following `Rect` class, which abstracts a rectangle by its width and height.

```cpp
class Rect {
    int width, height;
public:
    Rect(int w, int h) { width = w; height = h; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getArea();
};
int Rect::getArea() {
    return width * height;
}
```

**Problem:** The goal is to write code that reads a width and a height from the user, creates a `Rect` object dynamically, and prints its area. Fill in the blanks (1)-(3) as described below.

```cpp
int main() {
    Rect *q;
    int w, h;
    cin >> w >> h;   // 사각형의 폭과 높이를 w, h에 입력받는다
    // (1)
    // (2)
    // (3)
}
```

- (1) Dynamically create a `Rect` object that represents a rectangle of size `w`×`h` and assign it to the pointer variable `q`.
- (2) Print the area of the rectangle through the pointer `q`.
- (3) Return the created object.

**[Solution](Solutions.md#exercise-3-creating-and-returning-a-rect-object-dynamically)**

---

<br>

## Exercise 4: Creating an Array of `Rect` Objects Dynamically

**Textbook Problem:** Chapter 4 theory problem 4

This problem uses the `Rect` class of [Exercise 3](#exercise-3-creating-and-returning-a-rect-object-dynamically).

**Problem:** The following code, which creates an array of `Rect` objects, causes a compile error. How must the `Rect` class be changed so that no compile error occurs?

```cpp
Rect *p = new Rect[10];
```

**[Solution](Solutions.md#exercise-4-creating-an-array-of-rect-objects-dynamically)**

---

<br>

## Exercise 5: Calling Member Functions Through an Object Pointer

**Textbook Problem:** Chapter 4 theory problem 6

**Problem:** For a `Cube` class with the `public` member function `getVolume()`, there is the following code. Which of the statements that follow causes a compile error?

```cpp
Cube c;
Cube *p = &c;
```

- ① `c.getVolume();`
- ② `p->getVolume();`
- ③ `(*p).getVolume();`
- ④ `c->getVolume();`

**[Solution](Solutions.md#exercise-5-calling-member-functions-through-an-object-pointer)**

---

<br>

## Exercise 6: Creating and Returning a `Palette` Object Dynamically

**Textbook Problem:** Chapter 4 theory problem 8

**Problem:** What does the following program print when it runs?

```cpp
#include <iostream>
#include <string>
using namespace std;

class Color {
    string c;
public:
    Color() { c = "white"; cout << "기본생성자" << endl; }
    Color(string c) { this->c = c; cout << "매개변수생성자" << endl; }
    ~Color() { cout << "소멸자" << endl; }
};

class Palette {
    Color *p;
public:
    Palette() { p = new Color[3]; }
    ~Palette() { delete [] p; }
};

int main() {
    Palette *p = new Palette();
    delete p;
}
```

**[Solution](Solutions.md#exercise-6-creating-and-returning-a-palette-object-dynamically)**

---

<br>

## Exercise 7: Returning an Object Array with `delete`

**Textbook Problem:** Chapter 4 theory problem 10

**Problem:** What is wrong with the following code?

```cpp
Cube *p = new Cube [4];
delete p;
```

**[Solution](Solutions.md#exercise-7-returning-an-object-array-with-delete)**

---

<br>

## Exercise 8: Uses of `this`

**Textbook Problem:** Chapter 4 theory problem 12

**Problem:** Which statement about the use of `this` is wrong?

- ① `this` cannot be used in functions other than the member functions of a class.
- ② `this` cannot be used in `static` member functions.
- ③ `this` cannot be used in constructors.
- ④ Some member functions return `this`.

**[Solution](Solutions.md#exercise-8-uses-of-this)**

---

<br>

## Exercise 9: Situations That Cause Memory Leaks

**Textbook Problem:** Chapter 4 theory problem 14

**Problem:** In what situations does a memory leak occur?

**[Solution](Solutions.md#exercise-9-situations-that-cause-memory-leaks)**

---

<br>

## Exercise 10: The Header File of the `string` Class

**Textbook Problem:** Chapter 4 theory problem 16

**Problem:** Which header file is needed to use the `string` class?

- ① `<string>`
- ② `<string.h>`
- ③ `<cstring>`
- ④ `<iostream>`

**[Solution](Solutions.md#exercise-10-the-header-file-of-the-string-class)**

---

<br>

## Exercise 11: C-Strings and the `string` Class

**Textbook Problem:** Chapter 4 theory problem 18

**Problem:** Consider handling strings. Which statement about C-strings and the `string` class is wrong?

- ① A C-string represents a string with an array of characters.
- ② The `string` class turns a string into an object, so it is easier to use than a C-string.
- ③ The `string` class is good, but it is not part of standard C++, so it is better avoided where possible.
- ④ The `string` class handles only strings and has no functionality for manipulating characters, such as converting uppercase to lowercase.

**[Solution](Solutions.md#exercise-11-c-strings-and-the-string-class)**
