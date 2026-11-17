# 8장 실습 — 상속을 이용한 클래스 확장

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **최종 수정일:** 2026-11-18

> **학습 목표:**
> 1. 기본 클래스를 `public`으로 상속받아 멤버를 추가한 파생 클래스를 작성할 수 있다
> 2. 파생 클래스의 생성자에서 기본 클래스의 생성자를 골라 호출할 수 있다
> 3. 기본 클래스의 `protected` 멤버를 파생 클래스의 `public` 함수로 감싸 제공할 수 있다
> 4. 큐의 선입선출 동작을 설명하고 배열 기반의 원형 큐를 상속으로 구현할 수 있다

---

## 목차

- [1. 개요](#1-개요)
- [2. 이름을 가진 원](#2-이름을-가진-원)
- [3. 색을 가진 점](#3-색을-가진-점)
- [4. 배열을 상속받은 큐](#4-배열을-상속받은-큐)
  - [4.1 큐](#41-큐)
  - [4.2 선형 큐의 문제와 원형 큐](#42-선형-큐의-문제와-원형-큐)
  - [4.3 MyQueue의 구현](#43-myqueue의-구현)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 개요

> **참고:** 상속의 문법, 업캐스팅, `private`, `protected`, `public` 상속과 접근 지정, 생성자와 소멸자의 호출 순서, 다중 상속과 가상 상속은 [8장 강의 노트](Concepts_Lecture.ko.md)에서 다룬다. 이 실습은 이미 있는 클래스를 상속받아 기능을 더하는 세 가지 파생 클래스를 작성한다.

| 주제 | 위치 |
|:-----|:-----|
| `Circle`을 상속받아 이름을 더한 `NamedCircle` | 2절 |
| `Point`를 상속받아 색을 더하고 `protected` 함수로 위치를 바꾸는 `ColorPoint` | 3절 |
| 정수 배열 `BaseArray`를 상속받아 큐로 동작하는 `MyQueue` | 4절 |
| 전위와 후위 `++`를 프렌드 함수로 작성하는 `Circle` | [HW8 문제 2](../../Coursework/HW08_Friend-Functions-and-Operator-Overloading/Original/Assignment.ko.md#문제-2-13주차-강의자료-실습-2) |
| 위치와 색상 정보를 가진 `ColorPoint`, 학년별 성적을 처리하는 `Student` 계층 | [8장 연습 문제 1, 2](Exercises.ko.md) |

*표 1. 실습의 주제와 위치*

모든 코드는 C++17 표준으로 컴파일하여 실행한다(예: `g++ -std=c++17 -Wall`).

---

<br>

## 2. 이름을 가진 원

**문제:** 다음 `Circle` 클래스를 `public`으로 상속받아, `private` 멤버 변수 `name`(`string`)과 생성자, `show()`를 가진 `NamedCircle` 클래스를 작성한다. `main()`은 실행 예시처럼 출력한다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int radius = 0) {
        this->radius = radius;
    }
    int getRadius() { return radius; }
    void setRadius(int radius) {
        this->radius = radius;
    }
    double getArea() {
        return 3.14 * radius * radius;
    }
};

class NamedCircle : public Circle {
    string name;
public:
    NamedCircle(int radius, string name) : Circle(radius) {   // 기본 클래스 생성자 선택
        this->name = name;
    }
    void show() {
        cout << "반지름이 " << getRadius() << "인 " << name << endl;
    }
};

int main() {
    NamedCircle waffle(3, "waffle");    // 반지름이 3이고 이름이 waffle인 원
    waffle.show();
}
```

```text
반지름이 3인 waffle
```

- **기본 클래스 생성자의 선택:** `: Circle(radius)`는 `NamedCircle`의 생성자가 실행되기 전에 기본 클래스 부분을 `Circle(int)`로 초기화하라는 뜻이다. 이를 쓰지 않으면 기본 클래스의 기본 생성자가 호출되는데, 이 `Circle`에서는 디폴트 매개변수 때문에 `Circle(0)`이 되어 반지름이 0인 원이 만들어진다(8장 강의 노트 3.1절).
- **private 멤버의 상속:** `radius`는 `Circle`의 `private` 멤버이므로 `NamedCircle` 객체 안에 있기는 하지만 `NamedCircle`의 멤버 함수가 직접 접근할 수 없다. 그래서 `show()`는 상속받은 `public` 함수 `getRadius()`로 반지름을 읽는다.

---

<br>

## 3. 색을 가진 점

**문제:** 다음 `Point` 클래스를 `public`으로 상속받아, `private` 멤버 변수 `color`(`string`)와 생성자, `show()`, `setPoint()`, `setColor()`를 가진 `ColorPoint` 클래스를 작성한다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Point {
    int x, y;
public:
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int getX() { return x; }
    int getY() { return y; }
protected:
    void move(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class ColorPoint : public Point {
    string color;
public:
    ColorPoint(int x, int y, string color) : Point(x, y) {
        this->color = color;
    }
    void setPoint(int x, int y) { move(x, y); }       // protected 멤버는 파생 클래스에서 호출 가능
    void setColor(string color) { this->color = color; }
    void show() {
        cout << color << "색으로 (" << getX() << "," << getY() << ")에 위치한 점입니다." << endl;
    }
};

int main() {
    ColorPoint cp(5, 5, "RED");
    cp.setPoint(10, 20);
    cp.setColor("BLUE");
    cp.show();
}
```

```text
BLUE색으로 (10,20)에 위치한 점입니다.
```

- **기본 생성자가 없는 기본 클래스:** `Point`에는 매개변수 2개 생성자만 있고 기본 생성자가 없다. 따라서 `ColorPoint`의 생성자는 반드시 `: Point(x, y)`로 기본 클래스 생성자를 지정해야 하며, 빠뜨리면 컴파일 오류가 난다.
- **protected 멤버:** `move()`는 `protected`이므로 `main()` 같은 외부 함수는 `cp.move(10, 20)`을 호출할 수 없지만, 파생 클래스 `ColorPoint`의 멤버 함수 `setPoint()`는 호출할 수 있다. 위치를 바꾸는 기능을 파생 클래스에만 열어 두고, 외부에는 파생 클래스가 정한 `setPoint()`로만 제공하는 설계이다.

---

<br>

## 4. 배열을 상속받은 큐

**문제:** 정수 배열을 관리하는 `BaseArray`를 상속받아 큐로 동작하는 `MyQueue` 클래스를 작성한다. `MyQueue`는 다음 `main()`처럼 쓰인다.

```cpp
int main() {
    MyQueue mQ(100);
    int n;
    cout << "큐에 삽입할 5개의 정수를 입력하라>> ";
    for (int i = 0; i < 5; i++) {
        cin >> n;
        mQ.enqueue(n);                  // 큐에 삽입
    }
    cout << "큐의 용량:" << mQ.capacity() << ", 큐의 크기:" << mQ.length() << endl;
    cout << "큐의 원소를 순서대로 제거하여 출력한다>> ";
    while (mQ.length() != 0) {
        cout << mQ.dequeue() << ' ';    // 큐에서 제거하여 출력
    }
    cout << endl << "큐의 현재 크기 : " << mQ.length() << endl;
}
```

### 4.1 큐

> **용어 정의:** **큐(queue)** 는 먼저 넣은 원소를 먼저 꺼내는 선입선출(first-in first-out, FIFO) 방식의 자료 구조로, 원소는 뒤쪽(rear)으로 들어가고 앞쪽(front)에서 나온다.

매표소 앞에서 먼저 온 손님부터 줄을 세워 차례로 서비스하는 것이 큐의 동작이다. 큐는 공항의 비행기나 은행의 손님처럼 대기열을 시뮬레이션하거나, 통신에서 데이터 패킷을 모델링하거나, 프린터와 컴퓨터 사이의 버퍼로 쓰인다. 스택과 마찬가지로 많은 알고리즘이 도구로 쓴다.

```mermaid
graph LR
    IN(["enqueue"]) --> R["rear"]
    R --- E3["8"]
    E3 --- E2["6"]
    E2 --- E1["3"]
    E1 --- F["front"]
    F --> OUT(["dequeue"])
```

*그림 1. 뒤쪽으로 넣고 앞쪽으로 꺼내는 큐*

### 4.2 선형 큐의 문제와 원형 큐

크기 4인 배열에서 `front`와 `rear`를 인덱스로 쓰는 큐에 원소를 넣고 빼면 다음과 같다.

```text
enqueue(3)    [3][ ][ ][ ]    front = 0, size = 1
enqueue(6)    [3][6][ ][ ]    front = 0, size = 2
enqueue(8)    [3][6][8][ ]    front = 0, size = 3
dequeue() 3   [ ][6][8][ ]    front = 1, size = 2
enqueue(11)   [ ][6][8][11]   front = 1, size = 3
```

`dequeue()`로 비워진 0번 자리는 `rear`가 배열 끝에 이르면 더 이상 쓰이지 않는다. 크기가 3인데도 다음 `enqueue()`를 할 자리가 없어지므로, 넣고 빼기를 반복하면 큐를 계속 쓸 수 없다. 이를 해결하는 것이 배열의 끝 다음을 처음으로 이어 붙여 생각하는 **원형 큐(circular queue)** 이다. 다음 빈 자리의 인덱스는 `(front + size) % capacity`로 구하고, 꺼낸 뒤에는 `front = (front + 1) % capacity`로 앞을 옮긴다.

```text
enqueue(13)   [13][6][8][11]  front = 1, size = 4    (1 + 3) % 4 = 0번 자리
```

### 4.3 MyQueue의 구현

`BaseArray`는 크기가 정해진 정수 배열을 동적으로 할당하고, 원소를 넣고 읽는 함수를 파생 클래스에만 `protected`로 제공한다.

```cpp
#include <iostream>
using namespace std;

class BaseArray {
    int capacity;                       // 배열의 크기
    int* mem;                           // 정수 배열의 메모리
protected:
    BaseArray(int capacity = 100) {
        this->capacity = capacity;
        mem = new int[capacity];
    }
    ~BaseArray() { delete[] mem; }
    void put(int index, int val) { mem[index] = val; }
    int get(int index) { return mem[index]; }
    int getCapacity() { return capacity; }
};

class MyQueue : public BaseArray {
    int front;                          // 다음에 꺼낼 원소의 인덱스
    int size;                           // 현재 원소의 개수
public:
    MyQueue(int capacity) : BaseArray(capacity) {
        front = 0;
        size = 0;
    }
    void enqueue(int n) {
        if (size == getCapacity()) {
            cout << "queue full" << endl;
            return;
        }
        int rear = (front + size) % getCapacity();   // 원형으로 다음 빈 자리
        put(rear, n);
        size++;
    }
    int dequeue() {
        if (size == 0) {
            cout << "queue empty" << endl;
            return -1;
        }
        int n = get(front);
        front = (front + 1) % getCapacity();         // 원형으로 앞을 한 칸 이동
        size--;
        return n;
    }
    int capacity() { return getCapacity(); }
    int length() { return size; }
};
```

`main()`에 정수 1, 3, -5, 7, 9를 입력한 실행 결과는 다음과 같다.

```text
큐에 삽입할 5개의 정수를 입력하라>> 1 3 -5 7 9
큐의 용량:100, 큐의 크기:5
큐의 원소를 순서대로 제거하여 출력한다>> 1 3 -5 7 9 
큐의 현재 크기 : 0
```

- **상속의 역할:** `MyQueue`는 메모리 관리(할당, 반환)와 원소 접근을 `BaseArray`에 맡기고, 큐의 규칙(어디에 넣고 어디서 꺼낼지)만 구현한다. `BaseArray`의 생성자와 소멸자가 `protected`이므로 `BaseArray` 객체를 직접 만들 수는 없고, 파생 클래스를 통해서만 쓴다.
- **front와 size:** `rear`를 따로 두지 않고 `front`와 `size`로 계산하면, 가득 찬 상태(`size == capacity`)와 빈 상태(`size == 0`)를 헷갈리지 않고 구별할 수 있다.
- **원형 동작의 확인:** 크기 4인 큐에 3, 6, 8을 넣고 하나를 꺼낸 뒤 11, 13을 넣으면, 13은 비워진 0번 자리에 들어가고 그다음 15는 가득 차서 거부된다. 남은 원소를 모두 꺼내면 6, 8, 11, 13의 순서로 나온다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 파생 클래스의 작성 | 기본 클래스를 `public`으로 상속받아 멤버 변수와 멤버 함수를 더하며, 기본 클래스의 `private` 멤버는 상속받은 `public` 함수로 다룬다. |
| 기본 클래스 생성자의 선택 | 파생 클래스 생성자의 초기화 목록 `: Base(인자)`로 기본 클래스 생성자를 고르며, 기본 클래스에 기본 생성자가 없으면 반드시 지정해야 한다. |
| protected 멤버 | 외부 함수는 접근할 수 없지만 파생 클래스의 멤버 함수는 접근할 수 있어, 파생 클래스에만 여는 기능에 쓴다. |
| 큐 | 선입선출 자료 구조로, 뒤쪽으로 넣고 앞쪽에서 꺼낸다. |
| 원형 큐 | 배열 끝 다음을 처음으로 이어 인덱스를 `% capacity`로 계산하여, 꺼내서 비워진 자리를 다시 쓴다. |

---

<br>

## 점검 문제

1. 2절의 `NamedCircle` 생성자에서 `: Circle(radius)`를 지우면 출력은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   기본 클래스 생성자를 지정하지 않으면 `Circle`의 기본 생성자가 호출된다. 이 `Circle`의 생성자는 `Circle(int radius = 0)`이므로 인자 없이 호출될 수 있어 반지름이 0이 되고, `반지름이 0인 waffle`이 출력된다. 생성자 몸체에서 `setRadius(radius)`를 호출하면 반지름을 맞출 수 있지만, 초기화 목록으로 처음부터 올바른 값으로 만드는 편이 자연스럽다.

   </details>

2. 3절의 `main()`에서 `cp.move(10, 20);`을 호출하면 어떻게 되는가? `ColorPoint`를 `Point`의 `protected` 상속으로 바꾸면 `cp.getX()`는 호출할 수 있는가?

   <details>
   <summary>정답</summary>

   `move()`는 `Point`의 `protected` 멤버이고 `public` 상속에서도 `protected`로 남으므로, 외부 함수인 `main()`에서 호출하면 컴파일 오류가 난다. `protected` 상속으로 바꾸면 `Point`의 `public` 멤버 `getX()`가 `ColorPoint`에서 `protected`가 되므로 `main()`에서 `cp.getX()`도 호출할 수 없다. `ColorPoint`의 멤버 함수 안에서는 여전히 호출할 수 있다(8장 강의 노트 2.4절).

   </details>

3. 4.3절의 크기 4인 `MyQueue`에 5, 6, 7, 8을 넣고 두 개를 꺼낸 뒤 9, 10을 넣으면 배열의 각 자리와 `front`는 어떻게 되는가?

   <details>
   <summary>정답</summary>

   5, 6, 7, 8을 넣으면 0\~3번에 차례로 들어가고 `front = 0`, `size = 4`이다. 두 개를 꺼내면 5, 6이 나오고 `front = 2`, `size = 2`이다. 9의 자리는 (2 + 2) % 4 = 0번, 10의 자리는 (2 + 3) % 4 = 1번이므로 배열은 [9, 10, 7, 8], `front = 2`, `size = 4`가 된다. 이후 꺼내는 순서는 7, 8, 9, 10이다.

   </details>
