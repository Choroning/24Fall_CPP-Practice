# Chapter 5 Exercises — References and Copy Constructors

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Various Ways to Call a Function](#exercise-1-various-ways-to-call-a-function)
- [Exercise 2: Call by Reference and Reference Return](#exercise-2-call-by-reference-and-reference-return)
- [Exercise 3: Copy Constructor](#exercise-3-copy-constructor)
- [Textbook Theory Problems](#textbook-theory-problems)

---

<br>

## Exercise 1: Various Ways to Call a Function

**Textbook Problem:** Chapter 5 programming exercises 1 and 5

**Problem:** Referring to the example code and the sample run, write two versions of `mySwap()`, one whose parameters are references and one whose parameters are pointers, and write `increaseBy()`, so that `main()` runs correctly.

**Program Requirements:**

1. Fill in the [ 빈칸 ] (blank) parts of `main()` without changing anything else.
2. If needed, you may define additional constructors for the `Circle` class.

**Example Code:**

```cpp
#include <iostream>
using namespace std;

int main() {
   Circle x(10), y(5);  // 각각 반지름이 10, 5인 원 객체 생성

   [ 빈칸 ]              // 참조 버전의 mySwap() 호출
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   cout << endl;

   [ 빈칸 ]              // 포인터 버전의 mySwap() 호출
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   cout << endl;

   increaseBy(x, y);    // x 객체를 y 객체만큼 증가
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   
   return 0;
}
```

**Sample Run:**

```text
x 객체: 반지름이 5인 원
y 객체: 반지름이 10인 원

x 객체: 반지름이 10인 원
y 객체: 반지름이 5인 원

x 객체: 반지름이 15인 원
y 객체: 반지름이 5인 원

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-various-ways-to-call-a-function)**

---

<br>

## Exercise 2: Call by Reference and Reference Return

**Textbook Problem:** Chapter 5 programming exercise 6

**Problem:** Referring to the example code and the sample run, write the function `find()` so that `main()` runs correctly. The prototype of `find()` is as follows.

```cpp
char& find(char a[], char c, bool& success);
```

`find()` searches the string `a` for the character `c`, returns a reference to the location that holds `c`, and sets the reference parameter `success` to `true`. If `c` is not found, it sets the reference parameter `success` to `false`.

**Example Code:**

```cpp
#include <iostream>
using namespace std;

int main() {
   char s[] = "Mike";
   bool b;

   char& loc1 = find(s, 'k', b);
   if (b == false) {
      cout << "k를 발견할 수 없음" << endl;
      return 0;
   }
   loc1 = 'm';          // 'k' 위치에 'm' 기록
   cout << s << endl;

   char& loc2 = find(s, 'k', b);
   if (b == false) {
      cout << "k를 발견할 수 없음" << endl;
      return 0;
   }
   loc2 = 't';          // 'k' 위치에 't' 기록
   cout << s << endl;

   return 0;
}
```

**Sample Run:**

```text
Mime
k를 발견할 수 없음

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-2-call-by-reference-and-reference-return)**

---

<br>

## Exercise 3: Copy Constructor

**Textbook Problem:** Chapter 5 programming exercise 11

**Problem:** Referring to the example code and the sample run, write the constructor, the destructor, the `set()` function, and the copy constructor of the `Book` class so that `main()` runs correctly.

**Program Requirements:**

- When implementing `set()`, if the new title is longer than the current one, release the current dynamically allocated memory and allocate new memory that fits the length of the new title.
- Write the copy constructor as a deep copy so that the program runs without errors.

**Example Code:**

```cpp
#include <iostream>
using namespace std;

class Book {
   char* title;   // 도서 제목 문자열
   int price;     // 도서 가격
public:
   Book(const char* title, int price);
   ~Book();
   void set(char* title, int price);
   void show() {
      cout << title << ' ' << price << "원" << endl;
   }
};

int main() {
   Book cpp("C++가이드", 10000);
   Book java = cpp;  // 복사 생성자 호출
   java.set("Java완전정복", 12000);
   cpp.show();
   java.show();
   
   return 0;
}
```

**Sample Run:**

```text
C++가이드 10000원
Java완전정복 12000원

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-3-copy-constructor)**

---

<br>

## Textbook Theory Problems

**Textbook:** Even-numbered theory problems of the Chapter 5 exercises

### Theory Problem 2

In general, which kind of function call carries the greatest cost?

- ① Call by value
- ② Call by address
- ③ Call by reference
- ④ Call by implication

**[Solution](Solutions.md#theory-problem-2)**

### Theory Problem 4

Are the following two function declarations the same?

(1) `void f(int p[]);` and `void f(int *p);`

(2) `void f(int *p);` and `void f(int &p);`

**[Solution](Solutions.md#theory-problem-4)**

### Theory Problem 6

What is the output of the following program?

```cpp
#include <iostream>
#include <string>
using namespace std;

void square(int n[], int size) {
    for(int i = 0; i < size; i++)
        n[i] = n[i] * n[i];
}

int main() {
    int m[3] = {1, 2, 3};
    square(m, 3);
    for(int i = 0; i < 3; i++)
        cout << m[i] << ' ';
}
```

**[Solution](Solutions.md#theory-problem-6)**

### Theory Problem 8

Which statement fails to store the character `'a'` in the variable `c`?

```cpp
char c;
char *p = &c;
char q = c;
char &r = c;
```

- ① `c = 'a';`
- ② `q = 'a';`
- ③ `r = 'a';`
- ④ `*p = 'a';`

**[Solution](Solutions.md#theory-problem-8)**

### Theory Problem 10

What happens to the array `array` when each of the following statements is executed separately?

```cpp
int array[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
int& f(int n) {
    return array[n];
}
```

(1) `f(9) = 100;`

(2) `for(int i = 1; i < 9; i++) f(i) = f(i) + 2;`

(3) `int v = f(0); v = 100;`

(4) `f(f(2)) = 0;`

**[Solution](Solutions.md#theory-problem-10)**

### Theory Problem 12

There are two similar-looking functions as follows.

```cpp
int& big1(int a, int b) {
    if(a > b) return a;
    else return b;
}
int& big2(int& a, int& b) {
    if(a > b) return a;
    else return b;
}
```

Predict how the values of `x` and `y` change when the following code runs, and explain why.

```cpp
int x = 1, y = 2;
int& z = big1(x, y);
z = 100;
int& w = big2(x, y);
w = 100;
```

**[Solution](Solutions.md#theory-problem-12)**

### Theory Problem 14

Given a class `MyClass`, in which case is a copy constructor not needed?

- ① `MyClass a = f();` (when `f()` returns a `MyClass` object)
- ② `void f(MyClass *p);`
- ③ `MyClass b = a;` (`a` is of type `MyClass`)
- ④ `MyClass b(a);` (`a` is of type `MyClass`)

**[Solution](Solutions.md#theory-problem-14)**

### Theory Problem 16

Which statement about copy constructors is wrong? Choice ① states that several copy constructors can be declared in one class by overloading.

**[Solution](Solutions.md#theory-problem-16)**

### Theory Problem 18

What default copy constructor does the compiler insert into the following class?

```cpp
class Student {
    string *pName;
    string *pId;
    double grade;
};
```

**[Solution](Solutions.md#theory-problem-18)**
