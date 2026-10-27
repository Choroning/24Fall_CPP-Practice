# Chapter 5 Exercises — References and Copy Constructors

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

Exercises 1-3 are lab problems, and Exercises 4-12 are the even-numbered theory problems (2-18) of Chapter 5 of the textbook.

---

<br>

## Table of Contents

- [Exercise 1: Various Ways to Call a Function](#exercise-1-various-ways-to-call-a-function)
- [Exercise 2: Call by Reference and Reference Return](#exercise-2-call-by-reference-and-reference-return)
- [Exercise 3: Copy Constructor](#exercise-3-copy-constructor)
- [Exercise 4: The Cost of Function Calls](#exercise-4-the-cost-of-function-calls)
- [Exercise 5: Whether Two Function Declarations Are the Same](#exercise-5-whether-two-function-declarations-are-the-same)
- [Exercise 6: Output of a Function That Takes an Array](#exercise-6-output-of-a-function-that-takes-an-array)
- [Exercise 7: The Statement That Fails to Store a Character](#exercise-7-the-statement-that-fails-to-store-a-character)
- [Exercise 8: A Function That Returns a Reference to an Array Element](#exercise-8-a-function-that-returns-a-reference-to-an-array-element)
- [Exercise 9: Comparing Two Functions That Return References](#exercise-9-comparing-two-functions-that-return-references)
- [Exercise 10: The Case That Does Not Need a Copy Constructor](#exercise-10-the-case-that-does-not-need-a-copy-constructor)
- [Exercise 11: Statements About Copy Constructors](#exercise-11-statements-about-copy-constructors)
- [Exercise 12: The Default Copy Constructor Inserted by the Compiler](#exercise-12-the-default-copy-constructor-inserted-by-the-compiler)

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

## Exercise 4: The Cost of Function Calls

**Textbook Problem:** Chapter 5 theory problem 2

**Problem:** In general, which kind of function call carries the greatest cost?

- ① Call by value
- ② Call by address
- ③ Call by reference
- ④ Call by implication

**[Solution](Solutions.md#exercise-4-the-cost-of-function-calls)**

---

<br>

## Exercise 5: Whether Two Function Declarations Are the Same

**Textbook Problem:** Chapter 5 theory problem 4

**Problem:** Are the following two function declarations the same?

(1) `void f(int p[]);` and `void f(int *p);`

(2) `void f(int *p);` and `void f(int &p);`

**[Solution](Solutions.md#exercise-5-whether-two-function-declarations-are-the-same)**

---

<br>

## Exercise 6: Output of a Function That Takes an Array

**Textbook Problem:** Chapter 5 theory problem 6

**Problem:** What is the output of the following program?

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

**[Solution](Solutions.md#exercise-6-output-of-a-function-that-takes-an-array)**

---

<br>

## Exercise 7: The Statement That Fails to Store a Character

**Textbook Problem:** Chapter 5 theory problem 8

**Problem:** Which statement fails to store the character `'a'` in the variable `c`?

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

**[Solution](Solutions.md#exercise-7-the-statement-that-fails-to-store-a-character)**

---

<br>

## Exercise 8: A Function That Returns a Reference to an Array Element

**Textbook Problem:** Chapter 5 theory problem 10

**Problem:** What happens to the array `array` when each of the following statements is executed separately?

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

**[Solution](Solutions.md#exercise-8-a-function-that-returns-a-reference-to-an-array-element)**

---

<br>

## Exercise 9: Comparing Two Functions That Return References

**Textbook Problem:** Chapter 5 theory problem 12

**Problem:** There are two similar-looking functions as follows.

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

**[Solution](Solutions.md#exercise-9-comparing-two-functions-that-return-references)**

---

<br>

## Exercise 10: The Case That Does Not Need a Copy Constructor

**Textbook Problem:** Chapter 5 theory problem 14

**Problem:** Given a class `MyClass`, in which case is a copy constructor not needed?

- ① `MyClass a = f();` (when `f()` returns a `MyClass` object)
- ② `void f(MyClass *p);`
- ③ `MyClass b = a;` (`a` is of type `MyClass`)
- ④ `MyClass b(a);` (`a` is of type `MyClass`)

**[Solution](Solutions.md#exercise-10-the-case-that-does-not-need-a-copy-constructor)**

---

<br>

## Exercise 11: Statements About Copy Constructors

**Textbook Problem:** Chapter 5 theory problem 16

**Problem:** Which statement about copy constructors is wrong? Choice ① states that several copy constructors can be declared in one class by overloading.

**[Solution](Solutions.md#exercise-11-statements-about-copy-constructors)**

---

<br>

## Exercise 12: The Default Copy Constructor Inserted by the Compiler

**Textbook Problem:** Chapter 5 theory problem 18

**Problem:** What default copy constructor does the compiler insert into the following class?

```cpp
class Student {
    string *pName;
    string *pId;
    double grade;
};
```

**[Solution](Solutions.md#exercise-12-the-default-copy-constructor-inserted-by-the-compiler)**
