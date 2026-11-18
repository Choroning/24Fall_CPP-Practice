# Chapter 10 Exercises — Templates and the Standard Library (STL)

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-11-19

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

Exercises 1 and 2 are lab problems of the course above, and Exercises 3-9 are separately written problems that check the concepts of Chapter 10.

---

<br>

## Table of Contents

- [Exercise 1: Generic Functions](#exercise-1-generic-functions)
- [Exercise 2: Generic Classes and Debugging Instantiation](#exercise-2-generic-classes-and-debugging-instantiation)
- [Exercise 3: Template Argument Deduction](#exercise-3-template-argument-deduction)
- [Exercise 4: Choosing Between a Template Function and an Ordinary Function](#exercise-4-choosing-between-a-template-function-and-an-ordinary-function)
- [Exercise 5: A Generic Circular Queue](#exercise-5-a-generic-circular-queue)
- [Exercise 6: Tracing Modifying Operations on a vector](#exercise-6-tracing-modifying-operations-on-a-vector)
- [Exercise 7: An Error in a Loop That Erases Elements](#exercise-7-an-error-in-a-loop-that-erases-elements)
- [Exercise 8: Printing Word Frequencies in Sorted Order](#exercise-8-printing-word-frequencies-in-sorted-order)
- [Exercise 9: Captures in Lambda Expressions](#exercise-9-captures-in-lambda-expressions)

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

---

<br>

## Exercise 3: Template Argument Deduction

**Problem:** In the following program, decide whether each call (1)-(4) compiles. For each call that compiles, give the type `T` becomes and what is printed; for each call that does not compile, explain why.

```cpp
#include <iostream>
using namespace std;

template <class T>
T bigger(T a, T b) { return (a > b) ? a : b; }

int main() {
    cout << bigger(3, 7) << endl;            // (1)
    cout << bigger(3, 7.5) << endl;          // (2)
    cout << bigger<double>(3, 7.5) << endl;  // (3)
    cout << bigger('a', 'b') << endl;        // (4)
    return 0;
}
```

**[Solution](Solutions.md#exercise-3-template-argument-deduction)**

---

<br>

## Exercise 4: Choosing Between a Template Function and an Ordinary Function

**Problem:** Write the output of the following program, and explain for each call (1)-(4) whether the template function or the ordinary function is chosen, and why.

```cpp
#include <iostream>
using namespace std;

template <class T>
void print(T x) { cout << "template " << x << endl; }

void print(int x) { cout << "int " << x << endl; }

int main() {
    print(5);       // (1)
    print(5.0);     // (2)
    print('c');     // (3)
    print<>(5);     // (4)
    return 0;
}
```

**[Solution](Solutions.md#exercise-4-choosing-between-a-template-function-and-an-ordinary-function)**

---

<br>

## Exercise 5: A Generic Circular Queue

**Problem:** Write a circular queue that holds at most 4 elements as the class template `MyQueue<T>`. The following `main()` must print the output of the sample run.

**Program requirements:**

1. Elements are stored in an array of size 4, `T data[4]`, and when the end of the array is reached, storage wraps around to the beginning and reuses empty slots.
2. `bool push(T x)` returns `false` if the queue is full; otherwise it inserts `x` and returns `true`.
3. `bool pop(T &x)` returns `false` if the queue is empty; otherwise it removes the element inserted first, stores it in `x`, and returns `true`.
4. `int size() const` returns the current number of elements.

```cpp
int main() {
    MyQueue<int> qi;
    for (int i = 1; i <= 5; i++)
        cout << "push " << i << ": " << (qi.push(i * 10) ? "ok" : "full") << endl;
    int v;
    qi.pop(v); cout << "pop " << v << endl;
    qi.push(60);
    while (qi.pop(v)) cout << v << ' ';
    cout << endl;

    MyQueue<string> qs;
    qs.push("C++");
    qs.push("STL");
    string s;
    while (qs.pop(s)) cout << s << ' ';
    cout << endl;
    return 0;
}
```

**Sample run:**

```text
push 1: ok
push 2: ok
push 3: ok
push 4: ok
push 5: full
pop 10
20 30 40 60 
C++ STL 
```

**[Solution](Solutions.md#exercise-5-a-generic-circular-queue)**

---

<br>

## Exercise 6: Tracing Modifying Operations on a vector

**Problem:** Write what the following program prints at (1)-(5).

```cpp
#include <iostream>
#include <vector>
using namespace std;

void show(const vector<int> &v) {
    cout << "size=" << v.size() << " :";
    for (int x : v) cout << ' ' << x;
    cout << endl;
}

int main() {
    vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    show(v);                          // (1)
    v.insert(v.begin() + 1, 15);
    show(v);                          // (2)
    v.erase(v.begin());
    show(v);                          // (3)
    v.pop_back();
    show(v);                          // (4)
    v.resize(4);
    show(v);                          // (5)
    return 0;
}
```

**[Solution](Solutions.md#exercise-6-tracing-modifying-operations-on-a-vector)**

---

<br>

## Exercise 7: An Error in a Loop That Erases Elements

**Problem:** The following program tries to erase all even numbers from a `vector` and print `1 3 5`. Explain the error in this code and fix it correctly while keeping the same loop structure.

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5, 6};
    for (auto it = v.begin(); it != v.end(); it++) {
        if (*it % 2 == 0)
            v.erase(it);              // code that tries to erase even numbers
    }
    for (int x : v) cout << x << ' ';
    cout << endl;
    return 0;
}
```

**[Solution](Solutions.md#exercise-7-an-error-in-a-loop-that-erases-elements)**

---

<br>

## Exercise 8: Printing Word Frequencies in Sorted Order

**Problem:** Count the frequencies of the words in the string array `{"cat", "dog", "cat", "bird", "dog", "cat", "ant"}` with a `map<string, int>`, and then print the words from the highest frequency down. Words with the same frequency are printed in dictionary order. Use `sort()` from `<algorithm>` and a lambda expression for the sorting.

**Sample run:**

```text
cat 3
dog 2
ant 1
bird 1
```

**[Solution](Solutions.md#exercise-8-printing-word-frequencies-in-sorted-order)**

---

<br>

## Exercise 9: Captures in Lambda Expressions

**Problem:** Write the output of the following program, and explain each value on the two lines in terms of the capture method.

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 1;
    auto byValue = [x]() { return x * 10; };
    auto byRef = [&x]() { return x * 10; };
    x = 5;
    cout << byValue() << ' ' << byRef() << endl;
    auto addTo = [&x](int n) { x += n; };
    addTo(3);
    cout << x << ' ' << byValue() << ' ' << byRef() << endl;
    return 0;
}
```

**[Solution](Solutions.md#exercise-9-captures-in-lambda-expressions)**
