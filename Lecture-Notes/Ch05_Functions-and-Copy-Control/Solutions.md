# Chapter 5 Solutions — References and Copy Constructors

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

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

**Output:** The first swap makes `x` 5 and `y` 10, the second swap restores 10 and 5, and after `increaseBy()` `x` is 15. The last line of the sample run, `Program ended with exit code: 0`, is a message Xcode appends after the program ends and is not part of the program's output.

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

---

<br>

## Textbook Theory Problems

**Textbook:** Even-numbered theory problems of the Chapter 5 exercises

### Theory Problem 2

**Problem:** Among call by value, by address, by reference, and by implication, choose the kind of call that generally costs the most.

**Answer:** ①

Call by value copies the value of the actual argument into the parameter. For a built-in type such as `int` the copying cost is small, but passing an object by value runs the copy constructor to create the parameter object, and the destructor of that object also runs when the function ends. The larger the object, the more time and stack space the copying of its members takes ([Chapter 5, Section 2.1](Concepts_Lecture.md#21-passing-an-object-by-value)).

- **② Call by address:** It copies only one pointer value (an address), so the cost is constant regardless of the size of the object.
- **③ Call by reference:** The parameter becomes an alias of the actual argument, so the object is not copied. Compilers usually implement it by passing an address, so the cost is similar to call by address.
- **④ Call by implication:** This is not an argument-passing method of C++. C++ has three: call by value, by address, and by reference.

### Theory Problem 4

**Problem:** Decide whether `void f(int p[]);` and `void f(int *p);`, and then `void f(int *p);` and `void f(int &p);`, are the same declarations.

**Answer:** (1) The two declarations are the same. (2) The two declarations are different.

(1) When a function parameter is declared with an array type, the compiler adjusts the type to a pointer to the element type. So `int p[]` is exactly the same type as `int *p`, and both declarations declare the same function, `void f(int*)`. Defining the bodies of both is not overloading but defining the same function twice, which gives the error `redefinition of 'f'`. Even with a size, as in `int p[10]`, the size is ignored. The array notation `p[]` only shows the reader the intent that the starting address of an array is passed.

(2) `int *p` is a pointer parameter, so passing an address, as in `f(&n)`, is call by address, while `int &p` is a reference parameter, so passing a variable, as in `f(n)`, makes `p` an alias of `n`, which is call by reference. The former handles the pointed-to value through `*p`, and the latter handles the original directly through the alias `p` ([Chapter 5, Section 1.4](Concepts_Lecture.md#14-from-c-to-c-pointers-vs-references)). The parameter types (`int*` and `int&`) differ, so the two are different functions, and defining both in one program is valid overloading. In that case `f(&n)` calls the pointer version and `f(n)` the reference version.

### Theory Problem 6

**Problem:** Find the output of a program that calls `square()`, which squares each element of an array, and then prints the array.

**Answer:** `1 4 9`

In `square(m, 3)`, the array name `m` is converted to the address of its first element and passed, and the parameter `int n[]` is the same as `int *n`, as in (1) of theory problem 4. So `n[i]` refers to the elements of the array `m` of `main()` themselves, and the values squared in `square()` remain in `m`. This is passing an array by address. After the call `m` is `{1, 4, 9}`, and the loop prints a space after each element, so the actual output is `1 4 9 ` (with a trailing space).

### Theory Problem 8

**Problem:** Given `char c;`, `char *p = &c;`, `char q = c;`, and `char &r = c;`, choose the statement that fails to store `'a'` in `c`.

**Answer:** ②

`char q = c;` creates a separate variable `q` initialized with a copy of the value of `c`. So `q = 'a';` changes only `q` and has no effect on `c`.

- **① `c = 'a';`:** assigns to `c` directly.
- **③ `r = 'a';`:** `r` is a reference to `c`, that is, an alias of `c`, so assigning to `r` changes `c`.
- **④ `*p = 'a';`:** `p` holds the address of `c`, so `*p` is `c` itself.

### Theory Problem 10

**Problem:** Given `f()`, which returns a reference to an element of the global array `array`, find the contents of `array` after each of four statements is executed separately.

**Answer:**

- (1) `{0, 2, 4, 6, 8, 10, 12, 14, 16, 100}`
- (2) `{0, 4, 6, 8, 10, 12, 14, 16, 18, 18}`
- (3) `{0, 2, 4, 6, 8, 10, 12, 14, 16, 18}` (unchanged)
- (4) `{0, 2, 4, 6, 0, 10, 12, 14, 16, 18}`

`f(n)` returns a reference to `array[n]`, so the call expression `f(n)` itself becomes an alias of `array[n]` and can appear on the left side of an assignment. `array` is a global array that still exists after the function ends, so, unlike returning a reference to a local variable, this is safe ([Chapter 5, Section 3.3](Concepts_Lecture.md#33-reference-return)).

- **(1):** `f(9)` is `array[9]`, so 100 is stored in `array[9]`.
- **(2):** `i` goes from 1 to 8, running `array[i] = array[i] + 2`. Only elements 1-8 grow by 2, and element 0 (0) and element 9 (18) stay the same.
- **(3):** `v` is an ordinary `int` variable, not a reference, so it receives a copy of 0, the value of `array[0]` referred to by `f(0)`. `v = 100;` changes only `v`. Had it been declared `int& v = f(0);`, `v` would be an alias of `array[0]`, and `array[0]` would become 100.
- **(4):** The inner `f(2)` runs first and yields `array[2]`, that is, the value 4, which is copied into the parameter `n` of the outer `f()`. The outer `f(4)` returns a reference to `array[4]`, so 0 is stored in `array[4]`. What changes is `array[4]`, not `array[2]`.

> **Note:** When this code is compiled with `#include <iostream>` and `using namespace std;`, the global name `array` may, depending on the implementation, conflict with `std::array`. With Apple clang 21 (libc++), `<iostream>` brings in the declaration of `std::array` as well, so the error `reference to 'array' is ambiguous` occurs. Renaming the array or naming the global scope explicitly, as in `::array`, solves it.

### Theory Problem 12

**Problem:** Calling in turn `big1()`, which takes value parameters and returns a reference, and `big2()`, which takes reference parameters and returns a reference, and assigning 100 through each returned reference, predict how `x` and `y` change and explain why.

**Answer:** `x` stays 1 and `y` becomes 100. `z = 100;` through the reference returned by `big1()` cannot change `x` or `y` and is undefined behavior, and `w = 100;` through the reference returned by `big2()` changes `y` to 100.

- **`big1(x, y)`:** It is call by value, so the values of `x` and `y` are copied into the local parameters `a` (1) and `b` (2). `a > b` is false, so it returns a reference to `b`, but `b` is a local variable that disappears when the function ends. So `z` becomes a dangling reference to space that no longer exists, and `z = 100;` is undefined behavior ([Chapter 5, Section 5.1](Concepts_Lecture.md#51-returning-reference-to-local-variable)). `a` and `b` were copies, so this assignment does not change `x` or `y`. The compiler also warns about this function with `reference to stack memory associated with parameter 'b' returned` (and the same warning for `a`).
- **`big2(x, y)`:** It is call by reference, so `a` and `b` are aliases of `x` and `y`. `a > b` is false, so it returns a reference to `b`, that is, to `y`, and `int& w = big2(x, y);` makes `w` an alias of `y`. So running `w = 100;` makes `y` 100. `x` stays 1.

The difference between the two functions is the lifetime of what the returned reference refers to. To return a reference, its target must live longer than the function, so returning a reference parameter bound to the caller's variable, as `big2()` does, is correct, and returning a value parameter by reference, as `big1()` does, is wrong. Running it prints `x=1`, `y=2` after `big1()` and `x=1`, `y=100` after `big2()`, and `&w` equals `&y`.

### Theory Problem 14

**Problem:** For a class `MyClass`, choose among four cases the one in which a copy constructor is not needed.

**Answer:** ②

`void f(MyClass *p);` is a function that receives the address of an object, so calling it as `f(&a)` copies only a pointer value and creates no new object. The copy constructor therefore does not run ([Chapter 5, Section 2.2](Concepts_Lecture.md#22-passing-an-object-by-address-and-by-reference)).

- **① `MyClass a = f();`:** If `f()` returns an object by value, a copy of the returned object is made and `a` is initialized with it, so the copy constructor is used.
- **③ `MyClass b = a;`:** A new object is initialized with an object of the same type, so the copy constructor is called.
- **④ `MyClass b(a);`:** The same initialization as ③ written with parentheses; the copy constructor is called.

> **Note:** In ①, the compiler may omit the copy in the return process (copy elision). Since C++17, copy elision is guaranteed when a temporary object is returned, as in `return MyClass();`, and an object of the same type is initialized with the result, so even a class whose copy constructor is deleted with `= delete` compiles with `-std=c++17`, while `-std=c++14` gives the error `call to deleted constructor`. When a named local object is returned, as in `MyClass t; return t;`, elision is only allowed, not guaranteed, so a copy (or move) constructor must exist.

### Theory Problem 16

**Problem:** Choose the wrong statement about copy constructors. Choice ① is a statement about declaring copy constructors by overloading.

**Answer:** ①

The textbook explains that the parameter of a copy constructor is a single reference to its own class and that only one copy constructor can be declared in a class ([Chapter 5, Section 4.1](Concepts_Lecture.md#41-concept)). By this rule, choice ①, which says that copy constructors can be declared by overloading, is the wrong statement.

> **Note:** The textbook's rule is a simplification. In standard C++, a copy constructor is a constructor whose first parameter is one of `X&`, `const X&`, `volatile X&`, and `const volatile X&` and whose other parameters are absent or all have default values. The form of the parameter is therefore not fixed to one, and `X(X&)` and `X(const X&)` can be declared together in one class. In that case `X c(a);` calls `X(X&)`, which fits the non-`const` object `a` better, and `X d(b);` calls `X(const X&)`, which can accept the `const` object `b`. Real code usually keeps only the `const X&` version.

### Theory Problem 18

**Problem:** Write the default copy constructor that the compiler inserts into a `Student` class with the pointer members `pName` and `pId` and the `double` member `grade`.

**Answer:**

```cpp
Student(const Student& s) {   // 교재의 표기로는 Student(Student& s)
    this->pName = s.pName;
    this->pId = s.pId;
    this->grade = s.grade;
}
```

The default copy constructor performs member-wise copying, copying the members of the source object one by one as they are, that is, a shallow copy ([Chapter 5, Section 4.2](Concepts_Lecture.md#42-default-copy-constructor)). For the pointer members `pName` and `pId`, only the addresses are copied, not the `string` objects they point to, so the source and the copy share the same `string` objects. The textbook writes this constructor without `const`, as `Student(Student& s)`, but all members (the pointers and the `double`) can be copied from a `const` object, so the prototype the compiler actually declares is `Student(const Student& s)`. This can be confirmed by the fact that initializing another `Student` from a `const Student` object compiles.

Moreover, the copy constructor generated by the compiler does not assign in the body but initializes each member directly from the member of the source. That is, it is the same as writing `Student(const Student& s) : pName(s.pName), pId(s.pId), grade(s.grade) {}` with a member initializer list. For pointer and `double` members, assignment and initialization give the same result, but if there were reference or `const` members, assignment could not be used, so the initializer-list form is the exact description.

> **Caution:** If `Student` allocated `string` objects with `new` in its constructor and released them with `delete` in its destructor, the copy made by the default copy constructor and the source would free the same memory twice when they are destroyed. Such a class needs a deep-copy constructor, written by hand, that allocates and copies the pointed-to contents as well ([Chapter 5, Section 4.3](Concepts_Lecture.md#43-shallow-copy-problem)).
