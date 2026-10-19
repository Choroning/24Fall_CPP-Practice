# Chapter 5 Solutions — References and Copy Constructors

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 5

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Various Ways to Call a Function](#exercise-1-various-ways-to-call-a-function)
- [Exercise 2: Call by Reference and Reference Return](#exercise-2-call-by-reference-and-reference-return)
- [Exercise 3: Copy Constructor](#exercise-3-copy-constructor)

---

<br>

## Exercise 1: Various Ways to Call a Function

**Textbook Problem:** Chapter 5 programming exercises 1 and 5

**Problem:** Write the `Circle` class and three functions so that the radii of the `Circle` objects `x` and `y` (radius 10 and 5) are swapped by a reference version and then by a pointer version of `mySwap()`, and `increaseBy(x, y)` increases the radius of `x` by the radius of `y`. `main()` may not be changed except for the two blanks.

### Solution

Because `main()` creates the objects with `Circle x(10), y(5);` and calls `x.show()`, the `Circle` class needs a constructor that takes the radius and a `show()` that prints "반지름이 N인 원" ("a circle of radius N"). It also has `getRadius()` and `setRadius()` so that the three functions can read and change the radius.

The two `mySwap()` functions have the same name and differ only in parameter types, so they can be defined together through function overloading (Chapter 6). The first blank is `mySwap(x, y);`, which calls the reference version, and the second blank is `mySwap(&x, &y);`, which passes addresses and calls the pointer version. The compiler chooses the function by whether the arguments are `Circle` objects or `Circle*` values. `increaseBy()` must also change `x`, so it takes reference parameters.

**Code:** [Lab_Q1.cpp](../../Coursework/Lab05_Functions-and-Copy-Control/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C05Q1.cpp
 * @brief   Q1. 다양한 함수 호출법
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int radius) {
        this->radius = radius;
    }
    void show() {
        cout << "반지름이 " << radius << "인 원\n";
    }
    void setRadius(int radius) {
        this->radius = radius;
    }
    int getRadius() {
        return radius;
    }
};

void mySwap(Circle& a, Circle& b) {
    int temp = a.getRadius();
    a.setRadius(b.getRadius());
    b.setRadius(temp);
}

void mySwap(Circle* a, Circle* b) {
    int temp = a->getRadius();
    a->setRadius(b->getRadius());
    b->setRadius(temp);
}

void increaseBy(Circle& a, Circle& b) {
    a.setRadius(a.getRadius() + b.getRadius());
}

int main() {
    Circle x(10), y(5);  // 각각 반지름이 10, 5인 원 객체 생성
    
    mySwap(x, y);        // 참조 버전의 mySwap() 호출
    cout << "x 객체: "; 
    x.show();
    cout << "y 객체: "; 
    y.show();
    cout << endl;
    
    mySwap(&x, &y);      // 포인터 버전의 mySwap() 호출
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

- **`mySwap(Circle& a, Circle& b)`:** `a` and `b` are aliases of `x` and `y`, so `a.setRadius(...)` changes the radius of `x` itself. Only the radius values are exchanged through the temporary `temp`, so no object is copied.
- **`mySwap(Circle* a, Circle* b)`:** It receives the addresses of `x` and `y` and calls the member functions with `->`. It behaves like the reference version; the only difference is that the call must use `&`.
- **`increaseBy(Circle& a, Circle& b)`:** It sets the radius of `a` to `a.getRadius() + b.getRadius()`. At the last step the radius of `x` is 10 and that of `y` is 5, so `x` becomes 15.

> **Note:** `increaseBy()` does not modify `b`, so taking it as `const Circle& b` would state the intent better. That, however, requires declaring `getRadius()` as `int getRadius() const` so that it can be called on a `const` object.

**Output:** The output is the same as the sample run. The first swap makes `x` 5 and `y` 10, the second swap restores 10 and 5, and after `increaseBy()` `x` is 15. The last line of the sample run, `Program ended with exit code: 0`, is a message Xcode appends after the program ends and is not part of the program's output.

---

<br>

## Exercise 2: Call by Reference and Reference Return

**Textbook Problem:** Chapter 5 programming exercise 6

**Problem:** Write `char& find(char a[], char c, bool& success)`, which searches the string `a` for the character `c`, returns a reference to its location, and reports through the reference parameter `success` whether it was found.

### Solution

The function scans the string from the beginning, and at the first position where `a[i] == c` it sets `success` to `true` and returns `a[i]`. Because the return type is `char&`, `return a[i];` returns a reference to the element's storage rather than its value, and `loc1 = 'm';` in `main()` changes `s[2]` directly. If the character is not found by the end of the string, it sets `success` to `false`.

A function that returns a reference must return a reference to something even when nothing is found. This solution returns a reference to `a[0]`, and the caller must not use the returned reference when `success` is `false`. Returning a reference to a local variable would create a dangling reference ([Chapter 5, Section 3.3](Concepts_Lecture.md#33-reference-return)), so the function must return storage that outlives it, such as an array element.

**Code:** [Lab_Q2.cpp](../../Coursework/Lab05_Functions-and-Copy-Control/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C05Q2.cpp
 * @brief   Q2. 참조에 의한 호출과 참조 반환
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
#include <cstring>
using namespace std;

char& find(char a[], char c, bool& success) {
    unsigned long size = strlen(a);
    for (int i = 0; i < size; i++) {
        if (a[i] == c) {
            success = true;
            return a[i];
        }
    }
    success = false;
    return a[0];
}

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

- **`strlen(a)`:** It computes the string length so that only the characters before the null character are examined. `<cstring>` is included for this function.
- **First call:** In `"Mike"`, `'k'` is at index 2, so `loc1` becomes an alias of `s[2]`, and `loc1 = 'm';` makes `s` `"Mime"`.
- **Second call:** `"Mime"` contains no `'k'`, so `b` becomes `false`, and `main()` prints "k를 발견할 수 없음" ("k not found") and ends.

> **Note:** `size` of type `unsigned long` is compared with `i` of type `int`, which is a comparison between integers of different signedness. The result is the same as long as the string length fits in an `int`, but compiling with GCC and `-Wall` gives a `-Wsign-compare` warning (Apple clang's `-Wall` does not enable this warning). Declaring `i` as `size_t` removes the warning.

**Output:** The output is the same as the sample run (`Program ended with exit code: 0` is Xcode's message).

---

<br>

## Exercise 3: Copy Constructor

**Textbook Problem:** Chapter 5 programming exercise 11

**Problem:** Write the constructor, destructor, `set()`, and deep copy constructor of a `Book` class whose title string is dynamically allocated, so that changing the title and price of `java`, copy-constructed from `cpp`, does not affect `cpp`. `set()` must release and reallocate the memory when the new title is longer.

### Solution

- **Constructor:** It allocates `new char[]` with the title length plus one byte for the null character and copies the title with `strcpy()`.
- **Copy constructor `Book(Book& src)`:** It is a deep copy that allocates new memory of the source title's length and copies the contents. As a result, `java` and `cpp` own different memory, `java.set()` does not change the title of `cpp`, and the two destructors free their own memory.
- **Destructor:** It releases the title memory with `delete[] title;`.
- **`set()`:** If the new title is longer than the current one, it releases the old memory with `delete[]`, allocates memory for the new length, and copies. If it is shorter or equal, it copies into the existing space.

**Code:** [Lab_Q3.cpp](../../Coursework/Lab05_Functions-and-Copy-Control/Lab_Q3.cpp)

```cpp
/**
 * @file    A10204-1_C05Q3.cpp
 * @brief   Q3. 복사 생성자
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char* title;    // 도서 제목 문자열
    int price;      // 도서 가격
public:
    Book(const char* title, int price);
    Book(Book& src);
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};

Book::Book(const char* title, int price) {
    this->price = price;
    this->title = new char [strlen(title) + 1];
    strcpy(this->title, title);
}
Book::Book(Book& src) {
    price = src.price;
    title = new char [strlen(src.title) + 1];
    strcpy(title, src.title);
}
Book::~Book() {
    delete[] title;
    title = nullptr;
}
void Book::set(char* title, int price) {
    this->price = price;
    if (strlen(this->title) < strlen(title)) {
        delete[] this->title;
        this->title = new char[strlen(title) + 1];
    }
    strcpy(this->title, title);
}

int main()
{
    Book cpp("C++가이드", 10000);
    Book java = cpp;  // 복사 생성자 호출
    java.set("Java완전정복", 12000);
    cpp.show();
    java.show();

    return 0;
}
```

When the source file is UTF-8, `strlen("C++가이드")` is 12 and `strlen("Java완전정복")` is 16 (each Hangul syllable takes 3 bytes). `java.set("Java완전정복", 12000)` therefore takes the path that releases the existing 13-byte space and allocates 17 new bytes. Without the copy constructor, the default copy constructor would copy only the `title` pointer, so `cpp` would keep pointing to the memory freed by `set()`, read freed memory in `cpp.show()`, and cause a double free on destruction.

> **Caution:** `java.set("Java완전정복", 12000);` produces a compiler warning (Apple clang: `-Wwritable-strings`; GCC: "ISO C++ forbids converting a string constant to 'char*'"). Since C++11 a string literal is an array of `const char`, and the standard does not allow converting it to a `char*` parameter. The cause is the declaration `void set(char* title, int price)` given in the problem; changing the parameter to `const char*` removes the warning.

> **Note:** A copy constructor that takes a non-`const` reference, like `Book(Book& src)`, is valid, but it cannot copy from a `const Book` object or a temporary ([Chapter 5, Section 5.3](Concepts_Lecture.md#53-copy-constructor-parameter-must-be-a-reference)). The class also has no assignment operator, so an assignment such as `cpp = java;` would bring back the shallow copy problem. This program performs no assignment, so it is unaffected, but following the Rule of Three the assignment operator should be defined as well.

**Output:** The output is the same as the sample run (`Program ended with exit code: 0` is Xcode's message).
