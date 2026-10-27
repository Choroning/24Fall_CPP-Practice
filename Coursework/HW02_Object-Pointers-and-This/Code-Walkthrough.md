# HW2 Code Walkthrough — Object Pointers and the this Pointer

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **Learning Objectives:**
> 1. Read the class definitions and `main()` of the four solutions line by line and find where object pointers and `this` are used
> 2. Trace how the elements of an object array and the target of a pointer change with each statement
> 3. Explain when a dynamic array is allocated and released and the state of the pointer afterwards
> 4. Trace which object each call applies to when member functions that return `this` are called in a chain

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: An Array of Complex Objects and a Pointer](#2-q1cpp-an-array-of-complex-objects-and-a-pointer)
  - [2.1 Full Code](#21-full-code)
  - [2.2 Line-by-Line Explanation](#22-line-by-line-explanation)
  - [2.3 Execution Trace](#23-execution-trace)
- [3. Q2.cpp: The Person Class and an Object Pointer](#3-q2cpp-the-person-class-and-an-object-pointer)
  - [3.1 Full Code](#31-full-code)
  - [3.2 Line-by-Line Explanation](#32-line-by-line-explanation)
  - [3.3 Execution Trace](#33-execution-trace)
- [4. Q3.cpp: Static Arrays and Dynamic Arrays](#4-q3cpp-static-arrays-and-dynamic-arrays)
  - [4.1 Full Code](#41-full-code)
  - [4.2 Line-by-Line Explanation](#42-line-by-line-explanation)
  - [4.3 Execution Trace](#43-execution-trace)
- [5. Q4.cpp: The Counter Class and the this Pointer](#5-q4cpp-the-counter-class-and-the-this-pointer)
  - [5.1 Full Code](#51-full-code)
  - [5.2 Line-by-Line Explanation](#52-line-by-line-explanation)
  - [5.3 Execution Trace](#53-execution-trace)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the four solutions puts, in one file, the header includes, the class definition, and `main()`, in that order; only Q3 is the code of the slides, which consists of a single `main()` without a class. The three classes declare and define all their member functions together inside the class. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW2 Assignment Explanation](Assignment-Explanation.md).

| File | Class | Use of pointers | Input |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Complex` | `Complex *p`, which points to the object array `data` | None |
| [`Q2.cpp`](Solution/Q2.cpp) | `Person` | `Person* personPtr`, which points to the object `person2` | None |
| [`Q3.cpp`](Solution/Q3.cpp) | None | `int* pDynamic`, which points to the dynamic array made by `new int[size]` | The array size `size` |
| [`Q4.cpp`](Solution/Q4.cpp) | `Counter` | `this`, returned by the member functions | None |

*Table 1. Layout of the HW2 solution files*

---

<br>

## 2. Q1.cpp: An Array of Complex Objects and a Pointer

### 2.1 Full Code

```cpp
/**
 * @file    A2Q1.cpp
 * @brief   Q1. 포인터로 다루는 Complex 객체 배열 (5주차 문제 5-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
#define MAX 2
using namespace std;

class Complex{
    int real, image;
public:
    Complex(){
        real = image = 1;
    }
    Complex(int ir, int ii){
        real = ir;
        image = ii;
    }
    
    void setReal(int ir){
        real = ir;
    }
    void setImage(int ii){
        image = ii;
    }
    void showC(){
        cout << real << " + " << image << "i" << endl;
    }
};

int main()
{
    Complex data[MAX];
    Complex *p = data;
    
    for (int i = 0; i < MAX; i++){
        data[i].showC();
    }
    cout << "---------------" << endl;
    
    data[0].setReal(3);
    (*(p + 1)).setImage(-8);
    
    for (int i = 0; i < MAX; i++){
        (p + i)->showC();
    }
}
```

### 2.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-10 | `#include <iostream>`, `#define MAX 2`, `using namespace std;` | Includes `<iostream>` for `cout` and `endl`, makes the preprocessor replace every following `MAX` with 2, and omits `std::` |
| 12-13 | `class Complex{ int real, image;` | The two members declared without an access specifier are `private` by the default access of a `class` |
| 14 | `public:` | Declares the following constructors and member functions as `public` |
| 15-17 | `Complex(){ real = image = 1; }` | The default constructor. Assignment associates from right to left, so 1 is assigned to `image` and then that value to `real` |
| 18-21 | `Complex(int ir, int ii)` | Assigns the two received integers to `real` and `image`. It is never called in this program |
| 23-28 | `setReal()`, `setImage()` | Change the real part and the imaginary part, respectively, to the received value |
| 29-31 | `showC()` | Prints the real part, `" + "`, the imaginary part, and `"i"` in a row and ends the line |
| 36 | `Complex data[MAX];` | Becomes `Complex data[2];` after preprocessing; the default constructor is called for `data[0]` and then `data[1]` |
| 37 | `Complex *p = data;` | The array name is converted to the address of the first element, so `p` holds `&data[0]` |
| 39-41 | First `for` loop | Calls `showC()` of `data[0]` and `data[1]` with the array name and a subscript |
| 42 | Separator output | Prints 15 `-` characters and ends the line |
| 44 | `data[0].setReal(3);` | Changes the real part of the first element to 3 |
| 45 | `(*(p + 1)).setImage(-8);` | Dereferences `data[1]`, which `p + 1` points to, and changes its imaginary part to -8 |
| 47-49 | Second `for` loop | Prints the two elements through the pointer with `(p + i)->showC()` |

*Table 2. Line-by-line explanation of Q1.cpp*

### 2.3 Execution Trace

| Statement executed | `data[0]` | `data[1]` | Output |
|:-----|:-----|:-----|:-----|
| `Complex data[MAX];` | (1, 1) | (1, 1) | |
| First `for` loop | (1, 1) | (1, 1) | `1 + 1i`, `1 + 1i` |
| Separator output | (1, 1) | (1, 1) | `---------------` |
| `data[0].setReal(3);` | (3, 1) | (1, 1) | |
| `(*(p + 1)).setImage(-8);` | (3, 1) | (1, -8) | |
| Second `for` loop | (3, 1) | (1, -8) | `3 + 1i`, `1 + -8i` |

*Table 3. (`real`, `image`) of the two elements as the `main()` of Q1 runs*

When the second `for` loop starts, the array and the pointer look as follows. `p` does not change after line 37, and `p + i` points to the element that is `i` elements away from where `p` points.

```text
   p              p + 1
   │                │
   ▼                ▼
┌───────────────┬───────────────┐
│ data[0]       │ data[1]       │
│ real  = 3     │ real  = 1     │
│ image = 1     │ image = -8    │
└───────────────┴───────────────┘
```

*Figure 1. The array data and the pointer p just before the second for loop*

---

<br>

## 3. Q2.cpp: The Person Class and an Object Pointer

### 3.1 Full Code

```cpp
/**
 * @file    A2Q2.cpp
 * @brief   Q2. 포인터로 객체를 참조하는 자유 코드
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {}
    void introduce() {
        cout << "안녕하세요, 제 이름은 " << name << "이고 " << age << "살입니다." << endl;
    }
    void setAge(int newAge) {
        age = newAge;
    }
};

int main() {
    Person person1("김철수", 20);
    Person person2("엘리자베스", 25);
    Person* personPtr = &person2;
    
    person1.introduce();
    personPtr->introduce();

    (*personPtr).setAge(31);
    personPtr->introduce();

    return 0;
}
```

### 3.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 8-10 | `#include`, `using namespace std;` | Includes `<iostream>` for input/output and `<string>` for the string member, and omits `std::` |
| 12-14 | `class Person { string name; int age;` | The name and the age declared before the access specifier are `private` members |
| 16 | `Person(string n, int a) : name(n), age(a) {}` | A constructor that initializes `name` and `age` with a member initializer list; its body is empty |
| 17-19 | `introduce()` | Prints a self-introduction sentence that contains the name and the age |
| 20-22 | `setAge(int newAge)` | Changes `age` to `newAge` |
| 26-27 | Creating `person1` and `person2` | Creates the two objects by giving a name and an age to the constructor. The string literals are converted to the `string` parameter `n` |
| 28 | `Person* personPtr = &person2;` | Stores in the object pointer the address of `person2` obtained with the address-of operator `&` |
| 30 | `person1.introduce();` | Calls the member function of `person1` with the object name and the dot operator |
| 31 | `personPtr->introduce();` | Calls the member function of `person2` with the arrow operator |
| 33 | `(*personPtr).setAge(31);` | Calls `setAge(31)` with the dot operator on `person2`, obtained by dereferencing the pointer |
| 34 | `personPtr->introduce();` | Prints the self-introduction of `person2` with the changed age |
| 36 | `return 0;` | Returns 0 to signal that the program ended normally |

*Table 4. Line-by-line explanation of Q2.cpp*

### 3.3 Execution Trace

| Statement executed | `person1.age` | `person2.age` | `personPtr` | Age printed |
|:-----|-----:|-----:|:-----|:-----|
| Lines 26-27 | 20 | 25 | | |
| `Person* personPtr = &person2;` | 20 | 25 | `&person2` | |
| `person1.introduce();` | 20 | 25 | `&person2` | 20 (`person1`) |
| `personPtr->introduce();` | 20 | 25 | `&person2` | 25 (`person2`) |
| `(*personPtr).setAge(31);` | 20 | 31 | `&person2` | |
| `personPtr->introduce();` | 20 | 31 | `&person2` | 31 (`person2`) |

*Table 5. The ages of the two objects and the pointer as the `main()` of Q2 runs*

---

<br>

## 4. Q3.cpp: Static Arrays and Dynamic Arrays

### 4.1 Full Code

```cpp
/**
 * @file    A2Q3.cpp
 * @brief   Q3. 정적 배열과 동적 배열 (5주차 강의자료 p.15 코드 분석)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
using namespace std;

int main()
{
    int size;
    cout << "Input size: ";
    cin >> size;
    
    // int arrStatic[size] = { }; 일반 변수: 가능한가?
    int arrStatic[4] = { 1, 3, 5 };
    
    for (int i = 0; i < 4; i++)
        cout << arrStatic[i] << ' ';
    cout << endl;
    
    // 동적 메모리는 가변 길이 배열 선언 가능
    int* pDynamic = new int[size];
    
    for (int i = 0; i < size; i++)
        pDynamic[i] = i * 3;
    
    for (int i = 0; i < size; i++)
        cout << pDynamic[i] << ' ';
    cout << endl;
    
    delete[] pDynamic;
    // pDynamic = NULL;
    pDynamic = nullptr;
}
```

### 4.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 13-15 | `int size;`, the prompt, `cin >> size;` | Reads the size of the array |
| 17 | `// int arrStatic[size] = { }; ...` | A comment that asks whether an array whose size is an ordinary variable can be declared. Standard C++ does not allow it |
| 18 | `int arrStatic[4] = { 1, 3, 5 };` | Creates an array of size 4 and initializes its first three elements to 1, 3, and 5 and the remaining element to 0 |
| 20-22 | `for` loop and `cout << endl;` | Prints the four elements separated by spaces and ends the line. The body of the `for` loop is the single statement on line 21 |
| 24-25 | The comment, `int* pDynamic = new int[size];` | As the comment says, makes an array with a size chosen at run time. It allocates `size` elements of `int` on the heap and stores the address of the first element in `pDynamic`; the elements are not initialized |
| 27-28 | First `for` loop | Stores `i * 3` in `pDynamic[i]`. `pDynamic[i]` is the same as `*(pDynamic + i)` |
| 30-32 | Second `for` loop and `cout << endl;` | Prints the elements of the dynamic array separated by spaces and ends the line |
| 34 | `delete[] pDynamic;` | Releases the whole array allocated with `new[]`. From this point `pDynamic` is a dangling pointer |
| 35-36 | `// pDynamic = NULL;`, `pDynamic = nullptr;` | Makes the pointer a null pointer with the C++11 `nullptr` instead of the C-style `NULL` |

*Table 6. Line-by-line explanation of Q3.cpp*

### 4.3 Execution Trace

| Statement executed | `arrStatic` | Array that `pDynamic` points to | Output |
|:-----|:-----|:-----|:-----|
| `cin >> size;` (input `5`) | | | `Input size: ` |
| `int arrStatic[4] = { 1, 3, 5 };` | {1, 3, 5, 0} | | |
| Lines 20-22 | {1, 3, 5, 0} | | `1 3 5 0 ` |
| `new int[size]` | {1, 3, 5, 0} | Five uninitialized elements | |
| Lines 27-28 | {1, 3, 5, 0} | {0, 3, 6, 9, 12} | |
| Lines 30-32 | {1, 3, 5, 0} | {0, 3, 6, 9, 12} | `0 3 6 9 12 ` |
| `delete[] pDynamic;` | {1, 3, 5, 0} | Released (`pDynamic` is a dangling pointer) | |
| `pDynamic = nullptr;` | {1, 3, 5, 0} | None (`pDynamic` is a null pointer) | |

*Table 7. State of the two arrays as the `main()` of Q3 runs (input `5`)*

---

<br>

## 5. Q4.cpp: The Counter Class and the this Pointer

### 5.1 Full Code

```cpp
/**
 * @file    A2Q4.cpp
 * @brief   Q4. this 포인터를 리턴하는 메서드 체이닝 자유 코드
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
using namespace std;

class Counter {
    int count;
public:
    Counter() : count(0) {}
    Counter* increment() {
        this->count++;
        return this;
    }
    Counter* decrement() {
        this->count--;
        return this;
    }
    void display() {
        cout << "Count: " << this->count << endl;
    }
};

int main() {
    Counter c;
    
    c.increment()->increment()->display();  // Count: 2
    c.decrement()->display();  // Count: 1
}
```

### 5.2 Line-by-Line Explanation

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 11-12 | `class Counter { int count;` | Declares the `private` member `count`, which stores the current value |
| 14 | `Counter() : count(0) {}` | A constructor that initializes `count` to 0 with a member initializer list |
| 15-18 | `Counter* increment()` | Increases `this->count` by 1 and returns `this`, the address of the object it was called on |
| 19-22 | `Counter* decrement()` | Decreases `this->count` by 1 and returns `this` |
| 23-25 | `void display()` | Prints `Count: ` and the current value. It returns nothing |
| 29 | `Counter c;` | The constructor is called and `c.count` becomes 0 |
| 31 | `c.increment()->increment()->display();` | The calls run from left to right. Each next function is called with `->` on the `&c` returned by the previous call, so all three calls apply to `c` |
| 32 | `c.decrement()->display();` | Decreases `c.count` by 1 and then calls `display()` through the returned `&c` |

*Table 8. Line-by-line explanation of Q4.cpp*

### 5.3 Execution Trace

| Call | Target | `c.count` | Return value | Output |
|:-----|:-----|-----:|:-----|:-----|
| `Counter c;` | | 0 | | |
| `c.increment()` | `c` | 1 | `&c` | |
| `->increment()` | `c` | 2 | `&c` | |
| `->display()` | `c` | 2 | None | `Count: 2` |
| `c.decrement()` | `c` | 1 | `&c` | |
| `->display()` | `c` | 1 | None | `Count: 1` |

*Table 9. Call targets and `c.count` as the `main()` of Q4 runs*

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Layout of a file | The three classes define all their member functions inside the class, and Q3 consists of `main()` only, without a class. |
| Tracing element state | The two elements of Q1 change from (1, 1) and (1, 1) to (3, 1) and (1, -8). |
| Pointer and original | The age changed through `personPtr` in Q2 is the age of `person2` itself, which changes from 25 to 31. |
| Lifetime of a dynamic array | The dynamic array of Q3 is allocated at line 25 and released at line 34, and the pointer becomes a null pointer at line 36. |
| Chaining calls | Each member function of Q4 returns `&c`, so every call in one statement applies to the same object `c`. |

---

<br>

## Self-Check Questions

1. **Number of constructor calls:** How many times, and in what order, is the default constructor called at line 36 of Q1, `Complex data[MAX];`?

   <details>
   <summary>Answer</summary>

   `MAX` is 2, so it is called twice, for `data[0]` and then `data[1]`. Because there are no initial values, the two-parameter constructor is not called.

   </details>

2. **A different input:** If `3` is entered in Q3, what are the second and third lines of the output?

   <details>
   <summary>Answer</summary>

   The second line is `1 3 5 0 ` regardless of the input, and the third line is `0 3 6 ` because 0, 3, and 6 are stored in the three elements. The first line is `Input size: ` followed by the entered `3`.

   </details>

3. **A longer chain:** If line 31 of Q4 is changed to `c.increment()->increment()->increment()->display();`, what are the two lines printed?

   <details>
   <summary>Answer</summary>

   `count` grows from 0 to 1, 2, and 3, and `Count: 3` is printed; then line 32 decreases it from 3 to 2 and prints `Count: 2`.

   </details>

4. **The object name instead of the pointer:** If line 33 of Q2 is changed to `person2.setAge(31);`, does the output of line 34 change?

   <details>
   <summary>Answer</summary>

   It does not. `personPtr` points to `person2`, so `(*personPtr).setAge(31)` and `person2.setAge(31)` call the same member function of the same object. Line 34 still prints the self-introduction with the age 31.

   </details>
