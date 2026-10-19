# Chapter 5 Exercises — References and Copy Constructors

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 5

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

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
