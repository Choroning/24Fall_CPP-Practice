# Chapter 10 Solutions — Templates and the Standard Library (STL)

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

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

---

<br>

## Exercise 3: Template Argument Deduction

**Problem:** For `template <class T> T bigger(T a, T b)`, decide whether the four calls `bigger(3, 7)`, `bigger(3, 7.5)`, `bigger<double>(3, 7.5)`, and `bigger('a', 'b')` compile, and give `T` and the output for the calls that compile and the reason for the calls that do not. The program is in [Exercise 3](Exercises.md#exercise-3-template-argument-deduction).

### Solution

The compiler determines `T` from the types of the arguments in a call to a template function. This process is called template argument deduction, and when both parameters are `T`, the types deduced from the two arguments must be exactly the same. Deduction does not consider conversions such as converting `int` to `double`.

| Call | Deduction result | Result |
|:----|:----|:----|
| (1) `bigger(3, 7)` | Both arguments are `int`, so `T = int` | Prints 7 |
| (2) `bigger(3, 7.5)` | The first argument gives `int` and the second gives `double`, which conflict | Compile error |
| (3) `bigger<double>(3, 7.5)` | `T` is given explicitly as `double`, so nothing is deduced; 3 is converted to `double` | Prints 7.5 |
| (4) `bigger('a', 'b')` | Both arguments are `char`, so `T = char` | Prints b |

With (2) present, Apple clang reports "no matching function for call to 'bigger'" with the explanation "candidate template ignored: deduced conflicting types for parameter 'T' ('int' vs. 'double')". A template whose deduction fails drops out of the candidates, and since there is no other candidate, there is no function to call. Giving the template argument explicitly with `<double>`, as in (3), is the same as calling the ordinary function `bigger<double>(double, double)`, so the argument 3 becomes 3.0 by an ordinary conversion. With `T = char` in (4), `'a' > 'b'` compares the character codes 97 and 98, so `'b'` is returned, and a `char` is printed as a character.

The result of running the program with (2) removed is as follows.

```text
7
7.5
b
```

---

<br>

## Exercise 4: Choosing Between a Template Function and an Ordinary Function

**Problem:** With the function template `print(T)` and the ordinary function `print(int)` together, explain the output of `print(5)`, `print(5.0)`, `print('c')`, and `print<>(5)` and which function is chosen. The program is in [Exercise 4](Exercises.md#exercise-4-choosing-between-a-template-function-and-an-ordinary-function).

### Solution

```text
int 5
template 5
template c
template 5
```

The compiler puts both the function deduced from the template and the ordinary function among the candidates and chooses with the overload resolution rules of Chapter 6. When the conversion ranks are equal, the ordinary function takes precedence over the template function ([Chapter 10, Section 1.5](Concepts_Lecture.md#15-overloading-template-functions-with-ordinary-functions)).

- **(1) `print(5)`:** The candidates are `print<int>(int)` from the template with `T = int` and the ordinary function `print(int)`. Both are exact matches, so the ordinary function takes precedence and `int 5` is printed.
- **(2) `print(5.0)`:** The template is an exact match with `T = double`, while the ordinary function `print(int)` needs a standard conversion from `double` to `int`. The template ranks better, so `template 5` is printed. The `double` value 5.0 is printed as `5` in the default format of `cout`.
- **(3) `print('c')`:** The template is an exact match with `T = char`, while the ordinary function needs a promotion from `char` to `int`. An exact match ranks better than a promotion, so the template is chosen and `template c` is printed. The ordinary function takes precedence only when the ranks are equal.
- **(4) `print<>(5)`:** The empty angle brackets `<>` mean that only template functions are candidates. The ordinary function drops out, and the template with `T = int` deduced is called, printing `template 5`.

---

<br>

## Exercise 5: A Generic Circular Queue

**Problem:** Write a circular queue `MyQueue<T>` that holds at most 4 elements as a class template providing `push()`, `pop()`, and `size()`. The requirements, `main()`, and the sample run are in [Exercise 5](Exercises.md#exercise-5-a-generic-circular-queue).

### Solution

**Managing the circular array:** `front` is the position of the next element to remove, `rear` is the position where the next element goes, and `count` is the current number of elements. Inserting stores into `data[rear]` and moves `rear` to `(rear + 1) % 4`, and removing reads `data[front]` and moves `front` to `(front + 1) % 4`. Because of the remainder operation `% 4`, a position wraps from 3 back to 0, so the slots emptied by removals at the front can be reused. Whether the queue is full or empty cannot be told from `front` and `rear` alone (both cases have `front == rear`), so `count` decides it. This is the same principle as the array-based circular queue of the Chapter 8 lab, with only the element type generalized to `T`.

**Effect of making it a template:** `MyQueue<int>` and `MyQueue<string>` are different classes instantiated from the same code. `pop()` hands back the removed element not as the return value but through the reference parameter `T &x`, so the `bool` return value can report success at the same time (returning results through reference parameters, Section 3.2 of Chapter 5).

```cpp
#include <iostream>
#include <string>
using namespace std;

template <class T>
class MyQueue {
    T data[4];                          // circular array that holds the elements
    int front = 0;                      // position of the next element to remove
    int rear = 0;                       // position where the next element goes
    int count = 0;                      // current number of elements
public:
    bool push(T x) {
        if (count == 4) return false;   // full
        data[rear] = x;
        rear = (rear + 1) % 4;          // wraps to 0 at the end
        count++;
        return true;
    }
    bool pop(T &x) {                    // hands back the removed element through a reference parameter
        if (count == 0) return false;   // empty
        x = data[front];
        front = (front + 1) % 4;
        count--;
        return true;
    }
    int size() const { return count; }
};

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

**Result of execution:**

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

| Point in time | `front` | `rear` | `count` | `data` |
|:----|:----|:----|:----|:----|
| After inserting 10, 20, 30, 40 | 0 | 0 | 4 | 10, 20, 30, 40 |
| When trying to insert 50 | 0 | 0 | 4 | Unchanged (`full`) |
| After removing 10 | 1 | 0 | 3 | (10), 20, 30, 40 |
| After inserting 60 | 1 | 1 | 4 | 60, 20, 30, 40 |

*Table 1. State changes of `MyQueue<int>`*

60 goes into `data[0]`, which was emptied when 10 was removed from the front. The following loop then removes 20, 30, and 40 starting from `front` 1, and `front` wraps to 0 to remove 60.

---

<br>

## Exercise 6: Tracing Modifying Operations on a vector

**Problem:** Write the size and elements of a `vector<int>` at (1)-(5) after `push_back()`, `insert()`, `erase()`, `pop_back()`, and `resize()` are applied in turn. The program is in [Exercise 6](Exercises.md#exercise-6-tracing-modifying-operations-on-a-vector).

### Solution

```text
size=3 : 10 20 30
size=4 : 10 15 20 30
size=3 : 15 20 30
size=2 : 15 20
size=4 : 15 20 0 0
```

- **(2)** `v.insert(v.begin() + 1, 15)` inserts 15 at index 1, between 10 and 20. The elements after it, 20 and 30, move back by one position.
- **(3)** `v.erase(v.begin())` erases the first element, 10, and pulls the rest forward by one position.
- **(4)** `pop_back()` erases the last element, 30.
- **(5)** `resize(4)` grows the size to 4, and the two new elements become 0 by value initialization of `int`.

`insert()` and `erase()` take positions as iterators, not indices. Also, inserting or erasing near the front requires moving every element after that point, so with n elements, insertion and deletion near the front take time proportional to the number of elements. The output does not include the capacity `capacity()` because the way the capacity grows differs by implementation ([Chapter 10, Section 4.4](Concepts_Lecture.md#44-size-and-capacity)).

---

<br>

## Exercise 7: An Error in a Loop That Erases Elements

**Problem:** Explain and fix the error in code that tries to erase even numbers from `vector<int> v = {1, 2, 3, 4, 5, 6};` by calling `v.erase(it);` inside `for (auto it = v.begin(); it != v.end(); it++)`. The program is in [Exercise 7](Exercises.md#exercise-7-an-error-in-a-loop-that-erases-elements).

### Solution

**Cause of the error:** Running `v.erase(it)` invalidates `it` and every iterator that pointed to the elements after it ([Chapter 10, Section 11.1](Concepts_Lecture.md#111-iterator-invalidation-after-inserterase)). The loop, however, applies `it++` to the invalidated `it` and dereferences it again, which is undefined behavior. After an erasure the following elements are pulled forward, so even if `it` happens to point to the same place, `it++` skips over one of the pulled-forward elements. When the last element, 6, is erased, `it` sits at the `end()` position, and the following `it++` moves past `end()`, so the test `it != v.end()` does not end the loop and memory outside the array is read.

Compiled with Apple clang on macOS, this program may terminate abnormally without printing anything, and running it with AddressSanitizer (`-fsanitize=address`) turned on reports a `container-overflow` error. Because this is undefined behavior, the result may differ from one environment to another.

**Fixed code:** `erase()` returns a valid iterator pointing to the element right after the erased one. When an element is erased, that return value is stored in `it`; only when nothing is erased does `++it` advance.

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5, 6};
    for (auto it = v.begin(); it != v.end(); ) {
        if (*it % 2 == 0)
            it = v.erase(it);         // receive the position after the erased element
        else
            ++it;                     // advance only when nothing was erased
    }
    for (int x : v) cout << x << ' ';
    cout << endl;
    return 0;
}
```

```text
1 3 5 
```

Leaving the third part of the `for` empty is the key. The `if` and the `else` share the job of moving the iterator, so after an erasure the check continues directly at the next element reported by `erase()`, and the iterator advances one step only when nothing was erased.

---

<br>

## Exercise 8: Printing Word Frequencies in Sorted Order

**Problem:** Count the frequencies of a word array with a `map<string, int>`, then print them in descending order of frequency, with ties in dictionary order, using `sort()` and a lambda expression. The word array and the sample run are in [Exercise 8](Exercises.md#exercise-8-printing-word-frequencies-in-sorted-order).

### Solution

**Counting frequencies:** If `w` is not in the `map`, `freq[w]++` first creates an entry with the value 0 and then adds 1; if it is there, it adds 1 to its value ([Chapter 10, Section 11.2](Concepts_Lecture.md#112-using--on-map-creates-default-entry)). Thanks to this property, a word that appears for the first time needs no special handling.

**Sorting:** A `map` keeps its entries in the order of the keys (the words) and cannot be sorted by value (frequency). So all the entries of the `map` are copied into a `vector<pair<string, int>>`, which is then sorted with `sort()`. The comparison lambda returns `true` when `a` must come before `b`. If the frequencies differ, the larger frequency comes first (`a.second > b.second`); if they are equal, the word that comes first in the dictionary comes first (`a.first < b.first`).

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    string words[] = {"cat", "dog", "cat", "bird", "dog", "cat", "ant"};
    map<string, int> freq;
    for (const string &w : words)
        freq[w]++;                                    // a missing word is created as 0 and then incremented

    vector<pair<string, int>> items(freq.begin(), freq.end());
    sort(items.begin(), items.end(),
         [](const pair<string, int> &a, const pair<string, int> &b) {
             if (a.second != b.second) return a.second > b.second;   // descending frequency
             return a.first < b.first;                               // dictionary order on ties
         });
    for (const auto &p : items)
        cout << p.first << ' ' << p.second << endl;
    return 0;
}
```

```text
cat 3
dog 2
ant 1
bird 1
```

The order in the `map` is ant(1), bird(1), cat(3), dog(2). After sorting, cat with frequency 3 and dog with 2 come first, and ant and bird, which tie at 1, are placed in dictionary order.

---

<br>

## Exercise 9: Captures in Lambda Expressions

**Problem:** Write the result of running a lambda that captures by value, `byValue`, a lambda that captures by reference, `byRef`, and a lambda that changes a captured variable, `addTo`, and explain why. The program is in [Exercise 9](Exercises.md#exercise-9-captures-in-lambda-expressions).

### Solution

```text
10 50
8 10 80
```

- `[x]` copies the value of `x` **at the time the lambda is created**, 1, into the lambda. Changing `x` to 5 or 8 afterward does not affect the copy held by `byValue`, so `byValue()` always returns 10.
- `[&x]` refers to `x` itself, so `byRef()` reads `x` **at the time of the call**. On the first line `x` is 5, so it returns 50; on the second line `x` is 8, so it returns 80.
- `addTo` also captures `x` by reference, so `addTo(3)` changes the `x` of `main()` from 5 to 8. A variable captured by value is a copy inside the lambda that cannot be changed by default, and even when it is made changeable the outer variable is unaffected, so a lambda that changes an outer variable must capture it by reference ([Chapter 10, Section 8.3](Concepts_Lecture.md#83-lambda-expressions-c11)).
