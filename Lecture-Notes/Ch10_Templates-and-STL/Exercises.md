# Chapter 10 Exercises — Templates

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-19

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Exercise 1: Generic Functions

**Problem:** Referring to the example code and the sample run, complete the `main()` function, and write the `reverseArray()` and `display()` functions so that the program runs correctly.

**Program Requirements:**

1. The given example code must not be changed.
2. Write the following functions as generic functions using templates.
   - The `reverseArray()` function rearranges the elements stored in an array in reverse order.
   - The `display()` function prints the contents of an array.

**Example Code:**

```cpp
#include <iostream>
using namespace std;

// Define reverseArray()
[ blank ]

// Define display()
[ blank ]

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
    // Call reverseArray() to reverse str
    [ blank ]
    cout << "str = " << str << endl;

    return 0;
}
```

**Sample Run:**

```text
x = 1000  100  10  1
y = 6.6  5.5  4.4  3.3  2.2  1.1
str = lufrewop si ++C

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-generic-functions)**

---

<br>

## Exercise 2: Generic Classes and Debugging Instantiation

**Problem:** The given example code was written to run as follows.

```text
20과 50 중 더 큰 값은 50
rect1과 rect2 중 더 큰 것의 면적은 279.48

Program ended with exit code: 0
```

However, a compile error of unknown cause occurs, and the program does not run correctly. Complete the program so that it runs correctly, and find and debug the cause of the error.

The error in the example code below can be fixed in two typical ways.

**Example Code:**

```cpp
#include <iostream>
using namespace std;

template <typename T>
class Rectangle {
    T width;
    T height;
public:
    Rectangle(T w = 1, T h = 1) { width = w; height = h; }
    T getArea();
};

// Define the member function getArea()
[ blank ]

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

int main() {
    int a = 20, b = 50, c;

    c = bigger(a, b);
    cout << "20과 50 중 더 큰 값은 " << c << endl;

    // Declare double Rectangle objects rect1, rect2, and rect3
    // rect1 is initialized with width 10.2 and height 23.5, and rect2 with 20.4 and 13.7
    [ blank ]

    rect3 = bigger(rect1, rect2);
    cout << "rect1과 rect2 중 더 큰 것의 면적은 " << rect3.getArea() << endl;

    return 0;
}
```

**[Solution](Solutions.md#exercise-2-generic-classes-and-debugging-instantiation)**
