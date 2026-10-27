# HW4 Code Walkthrough — References

[Repository Guide](../../README.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

> **Learning Objectives:**
> 1. Read the class declarations, the function definitions, and `main()` of the three solutions line by line and explain the role of each part
> 2. Trace how the array and `tos` change with every push and pop on the stack
> 3. Trace statement by statement how assignments through references change the original variables and the members of objects

**Problem Source:** Prof. Jonguk Lee, Computer Language and Training Ⅱ (DCCS104-03)

---

## Table of Contents

- [1. Overall Layout](#1-overall-layout)
- [2. Q1.cpp: The MyIntStack Class](#2-q1cpp-the-myintstack-class)
- [3. Q2.cpp: Reference Variables and Reference Parameters](#3-q2cpp-reference-variables-and-reference-parameters)
- [4. Q3.cpp: The Person Class That Returns References](#4-q3cpp-the-person-class-that-returns-references)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)

---

<br>

## 1. Overall Layout

Each of the three solutions puts, in one file, the header includes, the definitions of a class or functions, and `main()`, in that order. The Doxygen comment at the top of each file (lines 1-6) records the file name, the problem, the author, and the submission date and has no effect on how the program runs. The requirements of the problems and the design decisions are covered in the [HW4 Assignment Explanation](Assignment-Explanation.md).

| File | What it defines | Use of references |
|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | The `MyIntStack` class | The reference parameter of `pop(int& n)` |
| [`Q2.cpp`](Solution/Q2.cpp) | `swapNumbers()`, `modifyValue()`, `printValue()` | A reference variable, reference parameters, a `const` reference parameter |
| [`Q3.cpp`](Solution/Q3.cpp) | The `Person` class, `celebrateBirthday()` | Reference returns, `const` reference returns, `const` reference parameters |

*Table 1. Layout of the HW4 solution files*

---

<br>

## 2. Q1.cpp: The MyIntStack Class

```cpp
/**
 * @file    A4Q1.cpp
 * @brief   Q1. 참조 매개변수로 값을 꺼내는 MyIntStack 클래스 (9주차 문제 2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
#define MAX 2
using namespace std;

class MyIntStack {
    int p[MAX];
    int tos; // 스택의 꼭대기를 가리키는 인덱스
public:
    MyIntStack();
    bool push(int n);
    // 정수n푸시, 꽉차있으면 false, 아니면 true 리턴
    bool pop(int &n);
    // 팝하여 n에 저장, 스택이 비어 있으면 false, 아니면 true 리턴
};

MyIntStack::MyIntStack() {
    tos = 0;
}

bool MyIntStack::push(int n) {
    if (tos < MAX) {
        p[tos] = n;
        tos++;
        return true;
    }
    else
        return false;
}
bool MyIntStack::pop(int& n) {
    if (tos > 0) {
        tos--;
        n = p[tos];
        return true;
    }
    else
        return false;
}

int main() {
    MyIntStack a;
    for (int i = 0; i < MAX + 1; i++) {
        if(a.push(i))
            cout << i << ' '; // 푸시된 값 에코
        else
            cout << endl << i + 1 << "번째 시도 stack full" << endl;
    }
    cout << "------------------------" << endl;
    
    int n;
    for (int i = 0; i < MAX + 1; i++) {
        if(a.pop(n))
            cout << n << ' ';
        else
            cout << endl << i + 1 << "번째 시도 stack empty";
    }
    
    cout << endl;
    return 0;
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 9 | `#define MAX 2` | The macro constant that sets the size of the stack |
| 12-14 | `int p[MAX]; int tos;` | The array that stores the values and `tos`, which is both the index of the next slot and the number of stacked values |
| 16-20 | Prototypes of the constructor, `push()`, and `pop()` | The comments are the descriptions attached to the declaration in the slides |
| 23-25 | `MyIntStack::MyIntStack()` | Sets `tos` to 0 so that the stack starts empty |
| 27-35 | `MyIntStack::push(int n)` | If there is space, stores in `p[tos]`, increments `tos`, and returns `true`; otherwise returns `false` |
| 36-44 | `MyIntStack::pop(int& n)` | If there is a value, decrements `tos`, stores `p[tos]` in the reference parameter `n`, and returns `true`; otherwise returns `false` |
| 48-53 | First `for` loop | Tries to push 0 through `MAX`, printing the value on success and the attempt number with a message on failure |
| 54 | Separator | Separates the push results from the pop results |
| 56-62 | `int n;` and the second `for` loop | Tries to pop `MAX + 1` times, printing the value held in `n` on success and a message on failure |
| 64-65 | `cout << endl; return 0;` | Ends the last line and returns 0 |

*Table 2. Line-by-line explanation of Q1.cpp*

| Call | Result | `p[0]` | `p[1]` | `tos` | `n` | Output |
|:-----|:-----|-----:|-----:|-----:|-----:|:-----|
| `push(0)` | `true` | 0 | undefined | 1 | | `0 ` |
| `push(1)` | `true` | 0 | 1 | 2 | | `1 ` |
| `push(2)` | `false` | 0 | 1 | 2 | | New line, `3번째 시도 stack full` |
| `pop(n)` | `true` | 0 | 1 | 1 | 1 | `1 ` |
| `pop(n)` | `true` | 0 | 1 | 0 | 0 | `0 ` |
| `pop(n)` | `false` | 0 | 1 | 0 | 0 | New line, `3번째 시도 stack empty` |

*Table 3. State of the stack after each push and pop in Q1*

A pop does not erase the value; it only decrements `tos`, so 0 and 1 remain in the array. Only the slots before `tos` count as the contents of the stack, and the next push overwrites the slot.

---

<br>

## 3. Q2.cpp: Reference Variables and Reference Parameters

```cpp
/**
 * @file    A4Q2.cpp
 * @brief   Q2. 참조와 관련한 자유 문제 1
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
using namespace std;

void swapNumbers(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void modifyValue(int& value) {
    value *= 2;
}

void printValue(const int& value) {
    cout << "Value is: " << value << endl;
}

int main() {
    int number = 42;
    int& ref = number;
    
    cout << "Original number: " << number << endl;
    ref = 100;
    cout << "After changing through reference: " << number << endl;
    
    int num1 = 10, num2 = 20;
    cout << "\nBefore swap - num1: " << num1 << ", num2: " << num2 << endl;
    swapNumbers(num1, num2);
    cout << "After swap - num1: " << num1 << ", num2: " << num2 << endl;
    
    int value = 5;
    cout << "\nBefore modification: " << value << endl;
    modifyValue(value);
    cout << "After modification: " << value << endl;
    
    const int& constRef = value;
    printValue(value);
    
    return 0;
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 11-15 | `swapNumbers(int& a, int& b)` | `a` and `b` are aliases of the arguments, so the swap through the temporary variable `temp` changes the arguments themselves |
| 17-19 | `modifyValue(int& value)` | `value *= 2;` doubles the argument |
| 21-23 | `printValue(const int& value)` | Reads and prints the argument without copying it; the function cannot change the value |
| 26-27 | `int number = 42; int& ref = number;` | Initializes `ref` as an alias of `number` |
| 29-31 | Output, `ref = 100;`, output | Shows that the assignment through `ref` made `number` 100 |
| 33-36 | Swapping `num1` and `num2` | Prints the values before and after the swap. The `\n` at the start of the string makes a blank line |
| 38-41 | Doubling `value` | Prints the value before and after `modifyValue()` |
| 43 | `const int& constRef = value;` | Creates a `const` reference that is never used afterwards, which causes the `-Wunused-variable` warning |
| 44 | `printValue(value);` | Prints `Value is: 10` |

*Table 4. Line-by-line explanation of Q2.cpp*

| Statement executed | `number` | `num1` | `num2` | `value` |
|:-----|-----:|-----:|-----:|-----:|
| Lines 26-27 | 42 | | | |
| `ref = 100;` | 100 | | | |
| Line 33 | 100 | 10 | 20 | |
| `swapNumbers(num1, num2);` | 100 | 20 | 10 | |
| Line 38 | 100 | 20 | 10 | 5 |
| `modifyValue(value);` | 100 | 20 | 10 | 10 |

*Table 5. Values of the variables as Q2 runs*

---

<br>

## 4. Q3.cpp: The Person Class That Returns References

```cpp
/**
 * @file    A4Q3.cpp
 * @brief   Q3. 참조를 클래스와 연계한 자유 문제 2
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
public:
    Person(const string& n = "", int a = 0) : name(n), age(a) {}
    
    string& getName() { return name; }
    int& getAge() { return age; }
    const string& getName() const { return name; }
    const int& getAge() const { return age; }
    void setName(const string& n) { name = n; }
    void setAge(const int& a) { age = a; }
    bool isOlderThan(const Person& other) const {
        return this->age > other.age;
    }
    void printInfo() const {
        cout << "Name: " << name << ", Age: " << age << endl;
    }
};

void celebrateBirthday(Person& person) {
    person.getAge()++;
}

int main() {
    Person person1("Alice", 25);
    Person person2("Bob", 30);
    
    person1.getName() = "Alice Smith";
    
    person1.setName("Alice Johnson");
    person1.setAge(26);
    
    cout << "Person 1: ";
    person1.printInfo();
    cout << "Person 2: ";
    person2.printInfo();
    
    if (person1.isOlderThan(person2)) {
        cout << person1.getName() << " is older" << endl;
    } else {
        cout << person2.getName() << " is older or same age" << endl;
    }
    
    celebrateBirthday(person1);
    cout << "\nAfter birthday celebration:" << endl;
    person1.printInfo();
    
    return 0;
}
```

| Lines | Code | Explanation |
|:-----|:-----|:-----|
| 13-15 | `private: string name; int age;` | Keeps the name and the age as `private` members |
| 17 | `Person(const string& n = "", int a = 0) : name(n), age(a) {}` | A constructor with default parameters and a member initializer list |
| 19-20 | `string& getName()`, `int& getAge()` | Getters that return references to the members themselves |
| 21-22 | `const` versions of the getters | Called on `const` objects and return `const` references |
| 23-24 | `setName()`, `setAge()` | Assign the values received as `const` references to the members |
| 25-27 | `isOlderThan(const Person& other) const` | Returns the result of comparing its own age with `other.age` |
| 28-30 | `printInfo() const` | Prints in the form `Name: ..., Age: ...` |
| 33-35 | `celebrateBirthday(Person& person)` | Applies `++` to the reference returned by `getAge()` to increase the age by 1 |
| 38-39 | Creating `person1` and `person2` | Initializes them with ("Alice", 25) and ("Bob", 30) |
| 41 | `person1.getName() = "Alice Smith";` | Changes `name` by assigning to the returned reference |
| 43-44 | `setName()`, `setAge()` | Change the name and the age again |
| 46-49 | Two `printInfo()` calls | Print with `Person 1: ` and `Person 2: ` in front |
| 51-55 | Branch on `isOlderThan()` | 26 > 30 is false, so the `else` branch runs |
| 57-59 | `celebrateBirthday(person1);` and output | Prints `person1`, whose age is now 27 |

*Table 6. Line-by-line explanation of Q3.cpp*

| Statement executed | `person1` | `person2` |
|:-----|:-----|:-----|
| Lines 38-39 | ("Alice", 25) | ("Bob", 30) |
| `person1.getName() = "Alice Smith";` | ("Alice Smith", 25) | ("Bob", 30) |
| `person1.setName("Alice Johnson");` | ("Alice Johnson", 25) | ("Bob", 30) |
| `person1.setAge(26);` | ("Alice Johnson", 26) | ("Bob", 30) |
| `celebrateBirthday(person1);` | ("Alice Johnson", 27) | ("Bob", 30) |

*Table 7. State of the two objects as Q3 runs (`name`, `age`)*

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| State of the stack | `tos` in Q1 grows to 0, 1, and 2 and shrinks to 1 and 0; a pop does not erase array values but only moves `tos`. |
| Passing results | `pop()` returns the success status and writes the value taken out into `n` of `main()` through the reference parameter. |
| Changes through aliases | `ref`, `swapNumbers()`, and `modifyValue()` in Q2 all change the original variables through aliases. |
| An unused reference | `constRef` at line 43 is only declared and never used, which causes the warning. |
| Tracing reference returns | `person1` in Q3 becomes ("Alice Johnson", 27) through an assignment to a reference, the setters, and `++` through a reference. |

---

<br>

## Self-Check Questions

1. **Traces in the array:** If `a.push(7)` is called after the second loop of Q1 ends, what happens to `p[0]`, `p[1]`, and `tos`?

   <details>
   <summary>Answer</summary>

   `tos` is 0, so 7 is stored in `p[0]` and `tos` becomes 1. `p[1]` still holds the 1 pushed earlier, but it is a slot after `tos`, so it does not count as the contents of the stack.

   </details>

2. **Line breaks around the messages:** Line 52 of Q1 puts `endl` before and after the failure message, while line 61 puts it only before. How does this difference show in the output?

   <details>
   <summary>Answer</summary>

   Line 52 ends the line after the message, so the separator of line 54 starts on the next line. Line 61 does not end the line after the message, but after the loop `cout << endl;` at line 64 ends the last line.

   </details>

3. **Tracing a swap:** If `swapNumbers(num1, num2);` in Q2 is changed to `swapNumbers(num1, num1);`, what happens to `num1`?

   <details>
   <summary>Answer</summary>

   Both `a` and `b` become aliases of `num1`. 10 is stored in `temp`, `a = b;` assigns `num1` its own value, and `b = temp;` assigns 10 again, so `num1` stays 10.

   </details>

4. **Return type of a getter:** If line 20 of Q3 is changed to `int getAge() { return age; }`, which line has what problem?

   <details>
   <summary>Answer</summary>

   A compile error occurs at `person.getAge()++;` on line 34. An `int` returned by value is a temporary value and cannot be changed with `++`.

   </details>
