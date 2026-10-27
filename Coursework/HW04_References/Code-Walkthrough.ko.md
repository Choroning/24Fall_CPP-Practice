# HW4 코드 총정리 — 참조

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

> **학습 목표:**
> 1. 세 풀이의 클래스 선언부, 함수 정의부, `main()`을 줄 단위로 읽고 각 부분의 역할을 설명할 수 있다
> 2. 스택의 푸시와 팝이 실행될 때마다 배열과 `tos`가 어떻게 바뀌는지 추적할 수 있다
> 3. 참조를 통한 대입이 원본 변수와 객체의 멤버를 어떻게 바꾸는지 문장 단위로 추적할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: MyIntStack 클래스](#2-q1cpp-myintstack-클래스)
- [3. Q2.cpp: 참조 변수와 참조 매개변수](#3-q2cpp-참조-변수와-참조-매개변수)
- [4. Q3.cpp: 참조를 리턴하는 Person 클래스](#4-q3cpp-참조를-리턴하는-person-클래스)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

세 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스나 함수의 정의, `main()`을 차례로 둔다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW4 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 정의한 것 | 참조의 쓰임 |
|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `MyIntStack` 클래스 | `pop(int& n)`의 참조 매개변수 |
| [`Q2.cpp`](Solution/Q2.cpp) | `swapNumbers()`, `modifyValue()`, `printValue()` | 참조 변수, 참조 매개변수, `const` 참조 매개변수 |
| [`Q3.cpp`](Solution/Q3.cpp) | `Person` 클래스, `celebrateBirthday()` | 참조 리턴, `const` 참조 리턴, `const` 참조 매개변수 |

*표 1. HW4 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: MyIntStack 클래스

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

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 9 | `#define MAX 2` | 스택의 크기를 정하는 매크로 상수이다 |
| 12~14 | `int p[MAX]; int tos;` | 값을 저장할 배열과, 다음에 넣을 칸의 인덱스이자 쌓인 개수인 `tos`이다 |
| 16~20 | 생성자, `push()`, `pop()`의 원형 | 주석은 강의자료의 선언에 붙은 설명이다 |
| 23~25 | `MyIntStack::MyIntStack()` | `tos`를 0으로 만들어 빈 스택으로 시작한다 |
| 27~35 | `MyIntStack::push(int n)` | 공간이 있으면 `p[tos]`에 저장하고 `tos`를 늘린 뒤 `true`, 없으면 `false`를 리턴한다 |
| 36~44 | `MyIntStack::pop(int& n)` | 값이 있으면 `tos`를 줄이고 `p[tos]`를 참조 매개변수 `n`에 저장한 뒤 `true`, 없으면 `false`를 리턴한다 |
| 48~53 | 첫 번째 `for` 문 | 0부터 `MAX`까지 푸시를 시도하여, 성공하면 값을, 실패하면 시도 번호와 메시지를 출력한다 |
| 54 | 구분선 | 푸시 결과와 팝 결과를 나눈다 |
| 56~62 | `int n;`과 두 번째 `for` 문 | 팝을 `MAX + 1`번 시도하여, 성공하면 `n`에 담긴 값을, 실패하면 메시지를 출력한다 |
| 64~65 | `cout << endl; return 0;` | 마지막 줄을 끝내고 0을 리턴한다 |

*표 2. Q1.cpp의 줄 단위 설명*

| 호출 | 결과 | `p[0]` | `p[1]` | `tos` | `n` | 출력 |
|:-----|:-----|-----:|-----:|-----:|-----:|:-----|
| `push(0)` | `true` | 0 | 미정 | 1 | | `0 ` |
| `push(1)` | `true` | 0 | 1 | 2 | | `1 ` |
| `push(2)` | `false` | 0 | 1 | 2 | | 줄바꿈, `3번째 시도 stack full` |
| `pop(n)` | `true` | 0 | 1 | 1 | 1 | `1 ` |
| `pop(n)` | `true` | 0 | 1 | 0 | 0 | `0 ` |
| `pop(n)` | `false` | 0 | 1 | 0 | 0 | 줄바꿈, `3번째 시도 stack empty` |

*표 3. Q1의 푸시와 팝에 따른 스택의 상태*

팝은 값을 지우지 않고 `tos`만 줄이므로 배열에는 0과 1이 그대로 남는다. `tos`보다 앞의 칸만 스택의 내용으로 취급되며, 다음 푸시가 그 칸을 덮어쓴다.

---

<br>

## 3. Q2.cpp: 참조 변수와 참조 매개변수

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

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 11~15 | `swapNumbers(int& a, int& b)` | `a`와 `b`가 인자의 별명이므로 임시 변수 `temp`를 거친 교환이 인자 자체를 바꾼다 |
| 17~19 | `modifyValue(int& value)` | `value *= 2;`가 인자를 두 배로 만든다 |
| 21~23 | `printValue(const int& value)` | 복사 없이 인자를 읽어 출력하며, 함수 안에서 값을 바꿀 수 없다 |
| 26~27 | `int number = 42; int& ref = number;` | `ref`를 `number`의 별명으로 초기화한다 |
| 29~31 | 출력, `ref = 100;`, 출력 | `ref`를 통한 대입으로 `number`가 100이 된 것을 보인다 |
| 33~36 | `num1`, `num2`의 교환 | 교환 전후의 값을 출력한다. 문자열 앞의 `\n`이 빈 줄을 만든다 |
| 38~41 | `value`의 두 배 | `modifyValue()` 전후의 값을 출력한다 |
| 43 | `const int& constRef = value;` | `const` 참조를 만들지만 이후에 쓰이지 않아 `-Wunused-variable` 경고가 난다 |
| 44 | `printValue(value);` | `Value is: 10`을 출력한다 |

*표 4. Q2.cpp의 줄 단위 설명*

| 실행한 문장 | `number` | `num1` | `num2` | `value` |
|:-----|-----:|-----:|-----:|-----:|
| 26~27행 | 42 | | | |
| `ref = 100;` | 100 | | | |
| 33행 | 100 | 10 | 20 | |
| `swapNumbers(num1, num2);` | 100 | 20 | 10 | |
| 38행 | 100 | 20 | 10 | 5 |
| `modifyValue(value);` | 100 | 20 | 10 | 10 |

*표 5. Q2의 실행에 따른 변수의 값*

---

<br>

## 4. Q3.cpp: 참조를 리턴하는 Person 클래스

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

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 13~15 | `private: string name; int age;` | 이름과 나이를 `private` 멤버로 둔다 |
| 17 | `Person(const string& n = "", int a = 0) : name(n), age(a) {}` | 디폴트 매개변수와 멤버 초기화 리스트를 쓴 생성자이다 |
| 19~20 | `string& getName()`, `int& getAge()` | 멤버 자체에 대한 참조를 리턴하는 getter이다 |
| 21~22 | `const` 판의 getter | `const` 객체에서 호출되며 `const` 참조를 리턴한다 |
| 23~24 | `setName()`, `setAge()` | `const` 참조로 받은 값을 멤버에 대입한다 |
| 25~27 | `isOlderThan(const Person& other) const` | 자기 나이와 `other.age`를 비교한 결과를 리턴한다 |
| 28~30 | `printInfo() const` | `Name: ..., Age: ...` 형식으로 출력한다 |
| 33~35 | `celebrateBirthday(Person& person)` | `getAge()`가 리턴한 참조에 `++`를 적용하여 나이를 1 늘린다 |
| 38~39 | `person1`, `person2` 생성 | ("Alice", 25)와 ("Bob", 30)으로 초기화한다 |
| 41 | `person1.getName() = "Alice Smith";` | 리턴된 참조에 대입하여 `name`을 바꾼다 |
| 43~44 | `setName()`, `setAge()` | 이름과 나이를 다시 바꾼다 |
| 46~49 | 두 `printInfo()` | 앞에 `Person 1: `, `Person 2: `를 붙여 출력한다 |
| 51~55 | `isOlderThan()`에 따른 분기 | 26 > 30이 거짓이므로 `else` 쪽이 실행된다 |
| 57~59 | `celebrateBirthday(person1);`와 출력 | 나이가 27이 된 `person1`을 출력한다 |

*표 6. Q3.cpp의 줄 단위 설명*

| 실행한 문장 | `person1` | `person2` |
|:-----|:-----|:-----|
| 38~39행 | ("Alice", 25) | ("Bob", 30) |
| `person1.getName() = "Alice Smith";` | ("Alice Smith", 25) | ("Bob", 30) |
| `person1.setName("Alice Johnson");` | ("Alice Johnson", 25) | ("Bob", 30) |
| `person1.setAge(26);` | ("Alice Johnson", 26) | ("Bob", 30) |
| `celebrateBirthday(person1);` | ("Alice Johnson", 27) | ("Bob", 30) |

*표 7. Q3의 실행에 따른 두 객체의 상태 (`name`, `age`)*

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 스택의 상태 | Q1의 `tos`는 0, 1, 2로 늘었다가 1, 0으로 줄며, 팝은 배열의 값을 지우지 않고 `tos`만 옮긴다. |
| 결과의 전달 | `pop()`은 성공 여부를 리턴하고 꺼낸 값은 참조 매개변수로 `main()`의 `n`에 쓴다. |
| 별명을 통한 변경 | Q2의 `ref`, `swapNumbers()`, `modifyValue()`는 모두 별명을 통해 원본 변수를 바꾼다. |
| 쓰이지 않는 참조 | 43행의 `constRef`는 선언만 되고 쓰이지 않아 경고의 원인이 된다. |
| 참조 리턴의 추적 | Q3의 `person1`은 참조 대입, setter, 참조를 통한 `++`를 거쳐 ("Alice Johnson", 27)이 된다. |

---

<br>

## 점검 문제

1. **배열의 흔적:** Q1의 두 번째 반복이 끝난 뒤 `a.push(7)`을 호출하면 `p[0]`, `p[1]`, `tos`는 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `tos`가 0이므로 `p[0]`에 7이 저장되고 `tos`는 1이 된다. `p[1]`에는 앞서 푸시한 1이 남아 있지만 `tos`보다 뒤의 칸이므로 스택의 내용으로 취급되지 않는다.

   </details>

2. **메시지 앞의 줄바꿈:** Q1의 52행은 실패 메시지 앞뒤에 `endl`을 두고 61행은 앞에만 둔다. 출력에서 이 차이는 어떻게 나타나는가?

   <details>
   <summary>정답</summary>

   52행은 메시지 뒤에서 줄을 바꾸므로 54행의 구분선이 다음 줄에서 시작한다. 61행은 메시지 뒤에서 줄을 바꾸지 않지만, 반복이 끝난 뒤 64행의 `cout << endl;`이 마지막 줄을 끝낸다.

   </details>

3. **교환의 추적:** Q2의 `swapNumbers(num1, num2);`를 `swapNumbers(num1, num1);`로 바꾸면 `num1`은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `a`와 `b`가 모두 `num1`의 별명이 된다. `temp`에 10을 저장하고 `a = b;`는 `num1`에 자기 값을 대입하며, `b = temp;`는 다시 10을 대입하므로 `num1`은 10 그대로이다.

   </details>

4. **getter의 리턴 타입:** Q3의 20행을 `int getAge() { return age; }`로 바꾸면 어느 줄에서 어떤 문제가 생기는가?

   <details>
   <summary>정답</summary>

   34행의 `person.getAge()++;`에서 컴파일 오류가 난다. 값으로 리턴한 `int`는 임시 값이어서 `++`로 바꿀 수 없기 때문이다.

   </details>
