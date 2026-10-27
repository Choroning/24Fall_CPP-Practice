# Chapter 4 Solutions — Using Objects

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Class with a Dynamic Array](#exercise-1-class-with-a-dynamic-array)
- [Exercise 2: Dynamic Creation and Destruction of Objects](#exercise-2-dynamic-creation-and-destruction-of-objects)
- [Textbook Theory Problems](#textbook-theory-problems)

---

<br>

## Exercise 1: Class with a Dynamic Array

**Textbook Problem:** Chapter 4 programming exercise 4

**Problem:** Complete `read()`, `write()`, `big()`, and the destructor of the `Sample` class, whose constructor dynamically allocates an integer array of the size entered by the user, and handle allocation failure and an array size of 0 or less with `try-catch`. The full problem and sample runs are in [Exercise 1](Exercises.md#exercise-1-class-with-a-dynamic-array).

### Solution

**Approach:** Separate where an error is detected from where it is handled. The constructor detects a size error and throws an exception, and `new` reports an allocation failure with a `bad_alloc` exception. `main()` catches both, prints an error message, and exits normally.

- **Constructor:** It checks the size first and throws the `int` value 1 if the size is 0 or less; only otherwise does it allocate the array with `new int [size]`. The size must be checked before `new` for two reasons. Calling `new` with a negative size throws an exception of the `bad_alloc` family, so a size error would be reported as an allocation failure, and with size 0 the allocation succeeds but yields an array with no elements, so `p[0]` in `big()` reads outside the array.
- **`read()` and `write()`:** `read()` reads `size` integers in turn with `cin >>`, and `write()` prints each number followed by one space and then ends the line.
- **`big()`:** It takes the first element `p[0]` as the initial maximum and compares it with the remaining elements using the conditional operator. Because it starts from the first element, it finds the correct maximum even if every number is negative.
- **Destructor:** The array was allocated with `new []`, so it is freed with `delete [] p;`, and `p` is set to `nullptr`.
- **`main()`:** Everything from creating the object to printing goes in the `try` block; `catch (bad_alloc)` handles allocation failure and `catch (int e)` handles the size error. When a constructor throws, the object is considered never constructed and its destructor is not called, so an unallocated array is never passed to `delete []`.

**Code:** [Lab_Q1.cpp](../../Coursework/Lab04_Pointers-Arrays-and-Dynamic-Memory/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C04Q1.cpp
 * @brief   Q1. 동적 배열을 가진 클래스
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Sample {
    int *p = nullptr;
    int size;
public:
    Sample(int n) {
        size = n;
        if (size <= 0) {
            throw 1;
        } else {
            p = new int [size];
        }
    }
    void read();
    void write();
    int big();
    ~Sample();
};

void Sample::read() {
    for (int i = 0; i < size; i++) {
        cin >> p[i];
    }
}
void Sample::write() {
    for (int i = 0; i < size; i++) {
        cout << p[i] << ' ';
    }
    cout << endl;
}
int Sample::big() {
    int max = p[0];
    for (int i = 1; i < size; i++) {
        max = (p[i] > max) ? p[i] : max;
    }
    return max;
}
Sample::~Sample() {
    delete [] p;
    p = nullptr;
}

int main() {
    int input;
    try {
        cout << "생성할 정수 배열 사이즈를 입력하세요: ";
        cin >> input;
        Sample s(input);
        s.read();
        s.write();
        cout << "가장 큰 수는 " << s.big() << endl;
    }
    catch (bad_alloc) {
        cout << "[오류] 동적할당에 실패했습니다!\n";
        return 0;
    }
    catch (int e) {
        cout << "[오류] 사이즈가 너무 작습니다!\n";
        return 0;
    }
    
    return 0;
}
```

**Output:** Entering the same values as in the two sample runs gives the same results. When run in a terminal, however, the line of numbers typed by the user is followed by the same numbers printed by `write()`, whereas the sample run shows this line only once. Because `write()` prints a space after each number, the printed line ends with one trailing space. The last line of the sample runs, `Program ended with exit code: 0`, is not program output but the exit message added by the Xcode console.

```text
생성할 정수 배열 사이즈를 입력하세요: 10
100 4 -2 9 55 300 44 38 99 -500
100 4 -2 9 55 300 44 38 99 -500 
가장 큰 수는 300
```

> **Note:** `bad_alloc` is declared in the `<new>` header, and this code compiles on implementations where `<iostream>` includes `<new>` internally. For portability, add `#include <new>`. Also, catching an exception by value as in `catch (bad_alloc)` copies the exception object, so the convention is to catch standard exceptions by reference, as in `catch (const bad_alloc &e)`, and check the message with `e.what()` when needed.

---

<br>

## Exercise 2: Dynamic Creation and Destruction of Objects

**Textbook Problem:** Chapter 4 programming exercise 12

**Problem:** Complete the `Circle` and `CircleManager` classes without changing the given declarations. The constructor of `CircleManager` dynamically creates a `Circle` array with as many elements as the number of circles and reads the name and radius of each circle; the class provides `searchByName()`, which looks up the area by name, and `searchByArea()`, which prints the circles larger than an entered minimum area. Handle a circle count of 0 or less and dynamic allocation failure with `try-catch`. The full problem and sample runs are in [Exercise 2](Exercises.md#exercise-2-dynamic-creation-and-destruction-of-objects).

### Solution

**Approach:** `CircleManager` takes charge of creating, using, and freeing the array of `Circle` objects. When a dynamic object array is created with `new Circle [size]` and no initializer list, each element is created with the default constructor ([Chapter 4, Section 3.3](Concepts_Lecture.md#33-dynamic-object-arrays)), so the array is created first and the entered values are then set on each element with `setCircle()`.

- **`Circle`:** It declares no constructor, so the compiler provides a default constructor, which is what lets `new Circle [size]` compile. `setCircle()` has parameters with the same names as the members, so it uses `this->radius` and `this->name` to refer to the members ([Chapter 4, Section 1.2](Concepts_Lecture.md#12-this-pointer)). `getArea()` computes $\pi r^2$ with the macro `PI` (3.14).
- **Constructor:** It checks the number of circles and throws the `int` value 1 if it is 0 or less; otherwise it allocates the array and reads each circle's name and radius with `cin >>`. Because names are read with `cin >>`, each name must be a single word without spaces.
- **Destructor:** `delete [] p;` frees the array, running the destructor of each element in the reverse order of creation.
- **`searchByName()`:** It finds every circle whose name matches the entered name and prints its area. If no name matches, it prints nothing.
- **`searchByArea()`:** It prints, on one line separated by `", "`, the name and area of each circle whose area is greater than the minimum area entered as an integer.

**Code:** [Lab_Q2.cpp](../../Coursework/Lab04_Pointers-Arrays-and-Dynamic-Memory/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C04Q2.cpp
 * @brief   Q2. 객체의 동적 생성 및 소멸
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

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

void Circle::setCircle(string name, int radius) {
    this->radius = radius;
    this->name = name;
}
double Circle::getArea() {
    return PI * radius * radius;
}
string Circle::getName() {
    return name;
}


class CircleManager {
   Circle *p;     // Circle 배열에 대한 포인터
   int size;      // 배열의 크기
public:
   CircleManager(int size);   // 사용자로부터 입력받은 size 값 크기의 배열을 동적 생성
   ~CircleManager();
   void searchByName(); // 사용자로부터 원의 이름을 입력받아 면적 출력
   void searchByArea(); // 사용자로부터 면적을 입력받아 면적보다 큰 원의 이름 출력
};

CircleManager::CircleManager(int size) {
    this->size = size;
    
    if (this->size <= 0) {
        throw 1;
    } else {
        p = new Circle [this->size];
        
        for (int i = 0; i < this->size; i++) {
            string temp1 = "";
            int temp2;
            
            cout << "원 " << i + 1 << "의 이름과 반지름 >> ";
            cin >> temp1 >> temp2;
            
            p[i].setCircle(temp1, temp2);
        }
    }
}
CircleManager::~CircleManager() {
    delete [] p;
    p = nullptr;
}
void CircleManager::searchByName() {
    string temp3 = "";
    cout << "검색하고자 하는 원의 이름 >> ";
    cin >> temp3;
    
    for (int i = 0; i < size; i++) {
        if (p[i].getName() == temp3) {
            cout << temp3 << "의 면적은 " << p[i].getArea() << endl;
        }
    }
}
void CircleManager::searchByArea() {
    int temp4;
    cout << "최소 면적을 정수로 입력하세요 >> ";
    cin >> temp4;
    cout << temp4 << "보다 큰 원을 검색합니다.\n";
    for (int i = 0; i < size; i++) {
        if (p[i].getArea() > temp4) {
            cout << p[i].getName() << "의 면적은 " << p[i].getArea() << ", ";
        }
    }
    cout << endl;
}


int main(void)
{
    try {
        int val;
        cout << "원의 개수 >> ";
        cin >> val;
        CircleManager c(val);
        c.searchByName();
        c.searchByArea();
    }
    catch (bad_alloc) {
        cout << "[오류] 동적할당에 실패했습니다!\n";
        return 0;
    }
    catch (int e) {
        cout << "[오류] 사이즈가 너무 작습니다!\n";
        return 0;
    }
    
    return 0;
}
```

**Output:** Identical to the two sample runs. The areas of the circles with radii 10, 2, 1, and 15 are 314, 12.56, 3.14, and 706.5, so 빈대떡, 도넛, and 피자, whose areas exceed the minimum area 10, are printed. The code also prints `", "` after the last circle, so the line ends with a comma, as in the sample run.

> **Note:** When the number of circles is 0 or less, the constructor throws before initializing `p`. The destructor of an object whose construction did not finish is not called, so `delete [] p` is never executed on the uninitialized `p`.

---

<br>

## Textbook Theory Problems

**Textbook:** Even-numbered theory problems of the Chapter 4 exercises

### Theory Problem 2

**Problem:** Write three lines that dynamically create a `Rect` object with the width and height that were read, print its area, and return the object.

**Answer:**

```cpp
q = new Rect(w, h);              // (1)
cout << q->getArea() << endl;    // (2)
delete q;                        // (3)
```

`new Rect(w, h)` in (1) creates a `Rect` object on the heap, calling the two-parameter constructor, and returns the address of the object ([Chapter 4, Section 2.2](Concepts_Lecture.md#22-dynamic-objects)). (2) calls a member function through a pointer, so it uses `->`; `(*q).getArea()` is the same call. The problem asks for the area to be printed, so the return value of `q->getArea()` must be printed with `cout`. `delete q;` in (3) calls the destructor of the object and then returns the memory. After the return, `q = nullptr;` can be added so that the pointer does not remain dangling.

### Theory Problem 4

**Problem:** Change the `Rect` class so that `new Rect[10]` does not cause a compile error.

**Answer:** Add a default constructor without parameters to the `public` part of the `Rect` class.

```cpp
Rect() { width = 1; height = 1; }
```

When an array of objects is created, as in `new Rect[10]`, the default constructor is called for each element ([Chapter 4, Section 3.3](Concepts_Lecture.md#33-dynamic-object-arrays)). `Rect` has only the two-parameter constructor, and the compiler does not generate a default constructor for a class that declares any constructor, so there is no way to create the elements and an error occurs. With the default constructor added, all ten elements are created as 1×1 rectangles. Instead of a default constructor, default parameters can also be given to the existing constructor, as in `Rect(int w = 1, int h = 1)`.

### Theory Problem 6

**Problem:** Choose the statement among the four after `Cube c; Cube *p = &c;` that causes a compile error.

**Answer:** ④

`c` is an object, not a pointer, so the arrow operator `->` cannot be used on it. An object calls a member with the dot operator, as in `c.getVolume()` (①), and a pointer with `p->getVolume()` (②) or with the dot operator after dereferencing, `(*p).getVolume()` (③) ([Chapter 4, Section 1.1](Concepts_Lecture.md#11-pointer-to-object)).

### Theory Problem 8

**Problem:** Give the output when a `Palette` object is created dynamically and returned.

**Answer:**

```text
기본생성자
기본생성자
기본생성자
소멸자
소멸자
소멸자
```

`new Palette()` calls the constructor of `Palette`, which creates an array of three `Color` objects with `new Color[3]`, calling the default constructor for each element. So "기본생성자" (default constructor) is printed three times. `delete p;` calls the destructor of `Palette`, and `delete [] p;` in that destructor calls the destructor of `Color` for the three elements of the array, so "소멸자" (destructor) is printed three times. The destructors of the array elements are called in the reverse order of construction, that is, `p[2]`, `p[1]`, `p[0]`. The parameterized constructor is never called.

### Theory Problem 10

**Problem:** Give the problem with code that returns an array allocated with `new Cube [4]` by `delete p;`.

**Answer:** An array allocated with `new []` must be returned with `delete [] p;`, but it was returned with `delete p;`.

`new` must be paired with `delete` and `new []` with `delete []`; a mismatched pair is undefined behavior ([Chapter 4, Section 2.3](Concepts_Lecture.md#23-dynamic-arrays)). `delete []` calls the destructors of all four elements and then returns the whole array, but `delete` is the operation for returning a single object, so the destructors of the other elements are not called and the memory is not returned correctly. Testing with a `Cube` that has a destructor, Clang warns `'delete' applied to a pointer that was allocated with 'new[]'` (`-Wmismatched-new-delete`), and when run, the destructor is called only once and the program terminates abnormally at the step that returns the memory. If the elements are objects that own dynamic memory, the memory of the elements whose destructors were not called remains as a leak.

### Theory Problem 12

**Problem:** Choose the wrong statement about the use of `this`.

**Answer:** ③

`this` is a pointer holding the address of the object on which a member function was called, and it can be used in every non-static member function, constructors included. In constructors it is commonly used to refer to a member when a parameter has the same name as the member variable, as in `this->width = width;` ([Chapter 4, Section 1.2](Concepts_Lecture.md#12-this-pointer)). The others are correct. `this` is meaningful only inside member functions (①), a static member function is called without a particular object and so has no `this` (②), and member functions sometimes return `this` or `*this` to chain calls (④).

### Theory Problem 14

**Problem:** Describe the situations in which a memory leak occurs.

**Answer:** It occurs when dynamically allocated memory is not returned after it is no longer needed, or can no longer be returned.

A **memory leak** is memory left over that the program can neither use nor return until it ends, because heap memory allocated with `new` was not returned with `delete` and every pointer to it was lost ([Chapter 4, Section 4.1](Concepts_Lecture.md#41-memory-leak)). Typical cases are the following.

- A function ends without `delete` for memory that is no longer needed, and the local pointer that stored the address disappears.
- Another address is assigned to the pointer that held the address of dynamic memory, and the original address is lost.
- A dynamically allocated array is returned with `delete` instead of `delete []`.
- The destructor of an object that owns dynamic memory does not return that memory.

As leaks accumulate, the usable memory shrinks, and a long-running program slows down or fails to allocate. The operating system reclaims the memory when the program ends, but not before.

### Theory Problem 16

**Problem:** Choose the header file needed to use the `string` class.

**Answer:** ①

The `string` class is declared in the header `<string>` of the C++ Standard Library ([Chapter 4, Section 7](Concepts_Lecture.md#7-the-string-class)). `<string.h>` and `<cstring>` are headers of the C library that declare C-string functions such as `strcpy()` and `strlen()`. Including `<iostream>` may make `string` usable depending on the implementation, but the standard does not guarantee it, so a file that uses `string` includes `<string>` itself.

### Theory Problem 18

**Problem:** Choose the wrong statement about C-strings and the `string` class.

**Answer:** ③

The `string` class is a standard class that belongs to the C++ Standard Library, so there is no reason to avoid it; it is in fact recommended over C-strings. The other statements are correct. A C-string represents a string as a `char` array terminated by the null character (`'\0'`) (①), and the `string` class is easy to use because it provides resizing, assignment, concatenation, and comparison through member functions and operators of an object (②). The `string` class has no member function that converts between uppercase and lowercase (④), so to convert characters, `toupper()` and `tolower()` from `<cctype>` are applied to each character ([Chapter 4, Section 7.2](Concepts_Lecture.md#72-main-member-functions-and-operators)).
