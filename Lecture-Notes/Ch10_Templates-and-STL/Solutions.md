# Chapter 10 Solutions — Templates

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-27

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Generic Functions](#exercise-1-generic-functions)
- [Exercise 2: Generic Classes and Debugging Instantiation](#exercise-2-generic-classes-and-debugging-instantiation)

---

<br>

## Exercise 1: Generic Functions

**Problem:** Write `reverseArray()`, which rearranges the elements of an array in reverse order, and `display()`, which prints the contents of an array, as generic functions using templates, and fill the blank in `main()` with a call that reverses the string `"C++ is powerful"` stored in the `char` array `str`. The given example code must not be changed. The example code and the sample run are in [Exercise 1](Exercises.md#exercise-1-generic-functions).

### Solution

**The `reverseArray()` template:** Declared as `template <typename T> void reverseArray(T arr[], int size)`, it is instantiated twice: `T` is deduced as `int` for `reverseArray(x, 4)` and as `double` for `reverseArray(y, 6)` ([Chapter 10, Section 1.3](Concepts_Lecture.md#13-template-instantiation)). The body swaps the `i`-th element from the front with the `i`-th element from the back (`arr[size - 1 - i]`) through a temporary variable `tmp` of type `T`, and it swaps only while `i < size/2`, that is, over the front half of the array. Swapping all the way to the end would swap the front-half elements back in the back half and restore the original order. With 4 elements it swaps the two pairs (0, 3) and (1, 2), with 6 elements the three pairs (0, 5), (1, 4), and (2, 3), and with an odd number of elements the middle element stays in place.

**The `display()` template:** Because it only reads the array, its parameter is declared as `const T arr[]`; it prints each element followed by two spaces and then ends the line.

**The call that reverses the string:** `str` is a `char` array of size 20, but the actual string `"C++ is powerful"` has 15 characters followed by the null character `'\0'`. Calling the template as `reverseArray(str, 20)` would also reverse the trailing null characters, making `str[0]` equal to `'\0'`, so `str` would print as an empty string. The solution therefore overloads the template with a separate ordinary function `void reverseArray(char arr[])` that takes only a `char` array. This function counts the characters into `size` until it reaches the null character and then swaps only the first 15 characters in the same way, and the blank is filled with `reverseArray(str);`. Because the two-parameter template does not match a one-argument call, the ordinary function is called ([Chapter 10, Section 1.5](Concepts_Lecture.md#15-overloading-template-functions-with-ordinary-functions)). Including `<cstring>` and calling the template as `reverseArray(str, strlen(str));` instead of writing a separate ordinary function gives the same result.

**Code:** [`Lab_Q1.cpp`](../../Coursework/Lab10_Templates-and-STL/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C10Q1.cpp
 * @brief   Q1. 제네릭 함수
 * @author  박철원 (Cheolwon Park)
 * @date    2024-12-04
 */

#include <iostream>
using namespace std;

// reverseArray() 정의
template <typename T>
void reverseArray(T arr [], int size) {
    T tmp;
    for (int i = 0; i < size/2; i++) {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

void reverseArray(char arr []) {
    int size = 0;
    while(arr[size])
        size++;
    
    char tmp;
    for (int i = 0; i < size/2; i++) {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

// display() 정의
template <typename T>
void display(const T arr [], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << "  ";
    }
    cout << endl;
}

int main() {
   int x[] = { 1, 10, 100, 1000 };

   reverseArray(x, 4);
   cout << "x = ";
   display(x, 4);

   double y[] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6 };
   reverseArray(y, 6);
   cout << "y = ";
   display(y, 6);

   char str[20] = "C++ is powerful";
   // str을 뒤집는 reverseArray() 호출문
   reverseArray(str);
   cout << "str = " << str << endl;

   return 0;
}
```

**Output:** Running the program prints the following. Because `display()` also prints two spaces after the last element, the `x` and `y` lines end with spaces that are not visible on the screen. The last line of the sample run, `Program ended with exit code: 0`, is not printed by the program; it is a message that the development environment (Xcode) shows after the program ends.

```text
x = 1000  100  10  1  
y = 6.6  5.5  4.4  3.3  2.2  1.1  
str = lufrewop si ++C
```

---

<br>

## Exercise 2: Generic Classes and Debugging Instantiation

**Problem:** Define the member function `getArea()` of the class template `Rectangle` outside the class, and fill the blanks in `main()` by declaring the `double` `Rectangle` objects `rect1` (10.2, 23.5), `rect2` (20.4, 13.7), and `rect3`. A compile error then occurs at `rect3 = bigger(rect1, rect2);`, so find its cause and fix it in two ways. The example code and the target output are in [Exercise 2](Exercises.md#exercise-2-generic-classes-and-debugging-instantiation).

### Solution

**Filling the blanks:** A member function of a class template defined outside the class repeats the template declaration and names its class with `Rectangle<T>::` ([Chapter 10, Section 2.1](Concepts_Lecture.md#21-syntax)). `getArea()` returns `width * height`. The objects are declared as `Rectangle<double> rect1(10.2, 23.5), rect2(20.4, 13.7), rect3;`, and `rect3` gets width and height 1 from the default arguments of the constructor.

**Cause of the error:** `bigger(a, b)` is instantiated with `T = int`, and `int` has the `>` operator, so it causes no problem. `bigger(rect1, rect2)`, however, is instantiated with `T = Rectangle<double>`, and `if (a > b)` in the instantiated body compares two `Rectangle<double>` objects with `>`. The `Rectangle` class has no `operator>`, so this instantiation fails with a compile error. The template itself has no syntax error; the error arises because the type used for the instantiation does not support an operation that the template body requires ([Chapter 10, Section 2.3](Concepts_Lecture.md#23-operator-overloading-in-templates), [Chapter 10, Section 10.2](Concepts_Lecture.md#102-common-errors)).

**Method 1, operator overloading:** Add to the `Rectangle` class a member function `bool operator>(Rectangle op2)` that compares the areas of two objects. Then `a > b` in `bigger<Rectangle<double>>` is interpreted as `a.operator>(b)`, and the instantiation succeeds. Because this operator is a member of the class template, it is generated together with each instantiated class such as `Rectangle<double>` or `Rectangle<int>`.

**Method 2, a more specialized `bigger()` template:** Add the function template `Rectangle<T> bigger(Rectangle<T> a, Rectangle<T> b)`, which takes two `Rectangle<T>` objects and returns the one with the larger area using the conditional operator. For `bigger(rect1, rect2)`, the original template (`T = Rectangle<double>`) and this template (`T = double`) both match exactly and equally well, but the compiler chooses this more specialized template, so the original body that compares objects with `>` is never instantiated ([Chapter 10, Section 1.5](Concepts_Lecture.md#15-overloading-template-functions-with-ordinary-functions)).

The solution code contains both methods. In that case `bigger(rect1, rect2)` calls the Method 2 template, and the `operator>` of Method 1 is not used by this call. Keeping only one of the two methods still compiles in either case and prints the same result.

**Computing the result:** The area of `rect1` is 10.2 × 23.5 = 239.7 and the area of `rect2` is 20.4 × 13.7 = 279.48, so `bigger()` returns `rect2`, which is assigned to `rect3`. The area computed in `double` is 279.47999..., but printed with the default `cout` precision (6 significant digits) it becomes 279.48.

**Code:** [`Lab_Q2.cpp`](../../Coursework/Lab10_Templates-and-STL/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C10Q2.cpp
 * @brief   Q2. 제네릭 클래스와 구체화 디버깅
 * @author  박철원 (Cheolwon Park)
 * @date    2024-12-10
 */

#include <iostream>
using namespace std;

template <typename T>
class Rectangle {
    T width;
    T height;
public:
    Rectangle(T w = 1, T h = 1) { width = w; height = h; }
    T getArea();
    
    /*
     * [방법1] 연산자 중복을 통한 해결
     * 이때, 제네릭 함수는 되도록 멤버함수로 중복하는 것이 더 좋은 방법이다.
     * (불가능한 케이스 제외)
     */
    bool operator > (Rectangle op2) {       
        if (getArea() > op2.getArea())
            return true;
        else
            return false;
    }
};

// 멤버함수 getArea() 정의
template <typename T>
T Rectangle<T>::getArea() {
    return width * height;
}

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

/*
 * [방법2] 제네릭 함수 bigger() 정의
 */
template <typename T>
Rectangle<T> bigger(Rectangle<T> a, Rectangle<T> b) {
    return (a.getArea() > b.getArea()) ? a : b;
}

int main() {
    int a = 20, b = 50, c;

    c = bigger(a, b);
    cout << "20과 50 중 더 큰 값은 " << c << endl;
    
    // double형 Rectangle 객체 rect1, rect2, rect3 선언
    // 단, rect1의 width와 height은 각각 10.2와 23.5, rect2는 20.4와 13.7로 초기화
    Rectangle<double> rect1(10.2, 23.5), rect2(20.4, 13.7), rect3;
    
    rect3 = bigger(rect1, rect2);
    cout << "rect1과 rect2 중 더 큰 것의 면적은 " << rect3.getArea() << endl;

    return 0;
}
```

**Output:** Running the program prints the following. As in Exercise 1, the last line `Program ended with exit code: 0` is a message shown by the development environment (Xcode).

```text
20과 50 중 더 큰 값은 50
rect1과 rect2 중 더 큰 것의 면적은 279.48
```
