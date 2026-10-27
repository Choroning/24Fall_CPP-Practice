# HW4 Assignment Handout — References

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the fourth assignment of section 3 of Computer Language and Training Ⅱ. It consists of one lab problem from the Week 9 lecture slides and two free problems on references.

- **Deadline:** 2024-11-04 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden in the assignment area.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.

---

<br>

## Problem 1. Week 9 Lecture Slides [Lab Problem 2]: Implementing and Analysing MyIntStack

Implement the stack class `MyIntStack`, which stores integers, according to the following declaration. The stack can hold at most 10 integers.

```cpp
class MyIntStack {
    int p[10];
    int tos; // 스택의 꼭대기를 가리키는 인덱스
public:
    MyIntStack();
    bool push(int n); // 정수 n 푸시. 꽉 차 있으면 false, 아니면 true 리턴
    bool pop(int &n); // 팝하여 n에 저장. 스택이 비어 있으면 false, 아니면 true 리턴
};
```

The code should use the array as a stack, and the meaning of a stack can be looked up on the internet. (The comments say: `tos` is the index of the top of the stack; `push()` pushes the integer `n` and returns false if the stack is full, otherwise true; `pop()` pops a value into `n` and returns false if the stack is empty, otherwise true.) What each member function does is as follows.

- **Constructor:** When the object is first created, nothing is stacked yet, so it sets `tos` to 0.
- **`push()`:** stores the integer passed in the call on the stack and returns `true` if the store succeeds. If the stack has reached its maximum and cannot store more, it returns `false`.
- **`pop()`:** passes the stacked value to the reference parameter and returns `true` if it succeeds. If there is nothing left to take out, it returns `false`.

`main()` is as follows. To check what happens beyond the range that can be stored (10 values), it tries 11 pushes and 11 pops instead of 10.

```cpp
int main() {
    MyIntStack a;
    for (int i = 0; i < 11; i++) {   // 11개를 푸시한다
        if (a.push(i))
            cout << i << ' ';        // 푸시된 값 에코
        else
            cout << endl << i + 1 << " 번째 stack full" << endl;
    }
    int n;
    for (int i = 0; i < 11; i++) {   // 11개를 팝한다
        if (a.pop(n))
            cout << n << ' ';        // 팝한 값 출력
        else
            cout << endl << i + 1 << " 번째 stack empty";
    }
    cout << endl;
    return 0;
}
```

For ease of explanation, the slides also show a version that gathers the same code on one page. This version reduces the stack size from 10 to 2 and keeps it in the macro constant `MAX` (`#define MAX 2`), repeats both `for` loops `MAX + 1` times, changes the messages to `번째 시도 stack full` and `번째 시도 stack empty` ("stack full/empty at the N-th attempt"), and prints a separator line between the two loops.

**Sample Run** (the version with `MAX` equal to 2)

```text
0 1
3 번째 시도 stack full
-----------------------
1 0
3 번째 시도 stack empty
```

---

<br>

## Problem 2. Free Problem 1 on References

Write any code related to references and submit the code with its analysis.

---

<br>

## Problem 3. Free Problem 2 Combining References with Classes

Write any code that uses references together with a class and submit the code with its analysis.
