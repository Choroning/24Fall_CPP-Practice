# 10장 연습 문제 — 템플릿과 표준 라이브러리 (STL)

[레포지토리 안내](../../README.ko.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **최종 수정일:** 2026-11-19

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

연습 문제 1과 2는 위 과목의 실습 문제이고, 연습 문제 3\~9는 10장의 개념을 확인하도록 따로 만든 문제이다.

---

<br>

## 목차

- [연습 문제 1: 제네릭 함수](#연습-문제-1-제네릭-함수)
- [연습 문제 2: 제네릭 클래스와 구체화 디버깅](#연습-문제-2-제네릭-클래스와-구체화-디버깅)
- [연습 문제 3: 템플릿 인자의 추론](#연습-문제-3-템플릿-인자의-추론)
- [연습 문제 4: 템플릿 함수와 일반 함수의 선택](#연습-문제-4-템플릿-함수와-일반-함수의-선택)
- [연습 문제 5: 제네릭 원형 큐](#연습-문제-5-제네릭-원형-큐)
- [연습 문제 6: vector의 변경 연산 추적](#연습-문제-6-vector의-변경-연산-추적)
- [연습 문제 7: 원소를 지우는 반복문의 오류](#연습-문제-7-원소를-지우는-반복문의-오류)
- [연습 문제 8: 단어 빈도의 정렬 출력](#연습-문제-8-단어-빈도의-정렬-출력)
- [연습 문제 9: 람다 표현식의 캡처](#연습-문제-9-람다-표현식의-캡처)

---

<br>

## 연습 문제 1: 제네릭 함수

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수를 완성하고, 정상적으로 실행되도록 `reverseArray()`, `display()` 함수를 작성하라.

**프로그램 요구사항:**

1. 주어진 예시 코드는 변경할 수 없다.
2. 아래 함수를 템플릿을 활용한 제네릭 함수로 작성하라.
   - `reverseArray()` 함수는 배열에 저장된 원소를 역순으로 재정렬하는 기능을 수행한다.
   - `display()` 함수는 배열의 내용을 출력하는 기능을 수행한다.

**예시 코드:**

```cpp
#include <iostream>
using namespace std;

// reverseArray() 정의
[ 빈칸 ]

// display() 정의
[ 빈칸 ]

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
    [ 빈칸 ]
    cout << "str = " << str << endl;

    return 0;
}
```

**실행 예시:**

```text
x = 1000  100  10  1
y = 6.6  5.5  4.4  3.3  2.2  1.1
str = lufrewop si ++C

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-1-제네릭-함수)**

---

<br>

## 연습 문제 2: 제네릭 클래스와 구체화 디버깅

**문제:** 주어진 예시 코드는 다음과 같이 실행되는 것을 목표로 작성되었다.

```text
20과 50 중 더 큰 값은 50
rect1과 rect2 중 더 큰 것의 면적은 279.48

Program ended with exit code: 0
```

그러나 알 수 없는 원인으로 컴파일 오류가 발생하여 정상적으로 실행되지 않는다. 프로그램이 정상적으로 실행되도록 완성하고, 오류 원인을 찾아 디버깅하라.

아래의 예시 코드에서 발생하는 오류는 대표적으로 두 가지 방법을 통해 해결할 수 있다.

**예시 코드:**

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

// 멤버함수 getArea() 정의
[ 빈칸 ]

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

int main() {
    int a = 20, b = 50, c;

    c = bigger(a, b);
    cout << "20과 50 중 더 큰 값은 " << c << endl;

    // double형 Rectangle 객체 rect1, rect2, rect3 선언
    // 단, rect1의 width와 height은 각각 10.2와 23.5, rect2는 20.4와 13.7로 초기화
    [ 빈칸 ]

    rect3 = bigger(rect1, rect2);
    cout << "rect1과 rect2 중 더 큰 것의 면적은 " << rect3.getArea() << endl;

    return 0;
}
```

**[풀이](Solutions.ko.md#연습-문제-2-제네릭-클래스와-구체화-디버깅)**

---

<br>

## 연습 문제 3: 템플릿 인자의 추론

**문제:** 다음 프로그램에서 (1)\~(4)의 각 호출이 컴파일되는지 판단하고, 컴파일되는 호출은 `T`가 어떤 타입으로 정해지며 무엇을 출력하는지 쓰라. 컴파일되지 않는 호출은 그 이유를 쓰라.

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

**[풀이](Solutions.ko.md#연습-문제-3-템플릿-인자의-추론)**

---

<br>

## 연습 문제 4: 템플릿 함수와 일반 함수의 선택

**문제:** 다음 프로그램의 출력을 쓰고, (1)\~(4)의 각 호출에서 템플릿 함수와 일반 함수 중 어느 것이 선택되는지 이유와 함께 설명하라.

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

**[풀이](Solutions.ko.md#연습-문제-4-템플릿-함수와-일반-함수의-선택)**

---

<br>

## 연습 문제 5: 제네릭 원형 큐

**문제:** 원소를 최대 4개까지 담는 원형 큐를 클래스 템플릿 `MyQueue<T>`로 작성하라. 다음 `main()`이 실행 예시와 같이 출력되어야 한다.

**프로그램 요구사항:**

1. 원소는 크기 4인 배열 `T data[4]`에 저장하고, 배열의 끝에 닿으면 처음으로 돌아가 빈 칸을 다시 쓴다.
2. `bool push(T x)`는 큐가 가득 차 있으면 `false`를, 아니면 `x`를 넣고 `true`를 리턴한다.
3. `bool pop(T &x)`는 큐가 비어 있으면 `false`를, 아니면 가장 먼저 넣은 원소를 꺼내 `x`에 저장하고 `true`를 리턴한다.
4. `int size() const`는 현재 원소 수를 리턴한다.

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

**실행 예시:**

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

**[풀이](Solutions.ko.md#연습-문제-5-제네릭-원형-큐)**

---

<br>

## 연습 문제 6: vector의 변경 연산 추적

**문제:** 다음 프로그램이 (1)\~(5)에서 출력하는 내용을 쓰라.

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

**[풀이](Solutions.ko.md#연습-문제-6-vector의-변경-연산-추적)**

---

<br>

## 연습 문제 7: 원소를 지우는 반복문의 오류

**문제:** 다음 프로그램은 `vector`에서 짝수를 모두 지우고 `1 3 5`를 출력하려는 것이다. 이 코드의 오류를 설명하고, 같은 반복문 구조를 유지하면서 올바르게 고쳐라.

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5, 6};
    for (auto it = v.begin(); it != v.end(); it++) {
        if (*it % 2 == 0)
            v.erase(it);              // 짝수를 지우려는 코드
    }
    for (int x : v) cout << x << ' ';
    cout << endl;
    return 0;
}
```

**[풀이](Solutions.ko.md#연습-문제-7-원소를-지우는-반복문의-오류)**

---

<br>

## 연습 문제 8: 단어 빈도의 정렬 출력

**문제:** 문자열 배열 `{"cat", "dog", "cat", "bird", "dog", "cat", "ant"}`에 나오는 단어의 빈도를 `map<string, int>`로 센 뒤, 빈도가 큰 단어부터 출력하라. 빈도가 같으면 사전 순서로 출력한다. 정렬에는 `<algorithm>`의 `sort()`와 람다 표현식을 사용한다.

**실행 예시:**

```text
cat 3
dog 2
ant 1
bird 1
```

**[풀이](Solutions.ko.md#연습-문제-8-단어-빈도의-정렬-출력)**

---

<br>

## 연습 문제 9: 람다 표현식의 캡처

**문제:** 다음 프로그램의 출력을 쓰고, 두 줄의 각 값이 나오는 이유를 캡처 방식으로 설명하라.

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

**[풀이](Solutions.ko.md#연습-문제-9-람다-표현식의-캡처)**
