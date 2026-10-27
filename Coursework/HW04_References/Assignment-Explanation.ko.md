# HW4 과제 해설 — 참조

[레포지토리 안내](../../README.ko.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

> **학습 목표:**
> 1. 배열과 꼭대기 인덱스로 정수 스택을 구현하고, 푸시와 팝의 성공 여부와 꺼낸 값을 함께 돌려주는 방법을 설명할 수 있다
> 2. 참조 변수가 원본 변수의 별명이라는 것을 이용하여 참조로 값을 바꾸고 교환할 수 있다
> 3. `const` 참조 매개변수가 복사 없이 값을 읽으면서 변경을 막는 이유를 설명할 수 있다
> 4. 멤버를 참조로 리턴하는 getter의 효과와, `const` 멤버 함수로 중복한 getter가 선택되는 규칙을 설명할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 과제 구성과 실행 방법](#1-과제-구성과-실행-방법)
- [2. Q1. 참조 매개변수로 값을 꺼내는 MyIntStack 클래스](#2-q1-참조-매개변수로-값을-꺼내는-myintstack-클래스)
  - [2.1 요구사항](#21-요구사항)
  - [2.2 설계와 해결 방법](#22-설계와-해결-방법)
  - [2.3 실행 결과와 검토](#23-실행-결과와-검토)
- [3. Q2. 참조와 관련한 자유 문제 1](#3-q2-참조와-관련한-자유-문제-1)
  - [3.1 요구사항](#31-요구사항)
  - [3.2 설계와 해결 방법](#32-설계와-해결-방법)
  - [3.3 실행 결과와 검토](#33-실행-결과와-검토)
- [4. Q3. 참조를 클래스와 연계한 자유 문제 2](#4-q3-참조를-클래스와-연계한-자유-문제-2)
  - [4.1 요구사항](#41-요구사항)
  - [4.2 설계와 해결 방법](#42-설계와-해결-방법)
  - [4.3 실행 결과와 검토](#43-실행-결과와-검토)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 과제 구성과 실행 방법

이 과제는 5장의 참조 변수, 참조 매개변수, 참조 리턴을 연습한다. Q1은 9주차 강의자료의 [실습 문제 2]이고, Q2와 Q3은 참조를 주제로 자유롭게 작성한 코드이다. 과제 안내문은 [`Original/Assignment.ko.md`](Original/Assignment.ko.md)에 있고, 각 코드를 처음부터 끝까지 따라가는 설명은 [HW4 코드 총정리](Code-Walkthrough.ko.md)에 있다.

```text
Coursework/HW04_References/
├── Original/
│   ├── Assignment.md        # 과제 안내문 (영어)
│   └── Assignment.ko.md     # 과제 안내문 (한국어)
└── Solution/
    ├── Q1.cpp               # 참조 매개변수로 값을 꺼내는 MyIntStack 클래스
    ├── Q2.cpp               # 참조 변수와 참조 매개변수의 자유 코드
    └── Q3.cpp               # 참조를 리턴하는 getter를 가진 Person 클래스의 자유 코드
```

*그림 1. HW4 과제의 디렉토리 구성*

각 풀이는 다음과 같이 컴파일하고 실행한다. 세 파일 모두 macOS의 Apple clang 21에서 `g++ -std=c++11 -Wall`과 `g++ -std=c++17 -Wall`로 컴파일되며, 입력을 받지 않는다.

```bash
cd Coursework/HW04_References/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

| 파일 | 컴파일 결과 |
|:-----|:-----|
| `Q1.cpp` | 경고 없음 |
| `Q2.cpp` | 경고 1개: `constRef`를 쓰지 않는다는 `-Wunused-variable` |
| `Q3.cpp` | 경고 없음 |

*표 1. HW4 풀이의 컴파일 결과*

세 프로그램은 동적 메모리를 쓰지 않으며, AddressSanitizer와 `leaks --atExit`로 실행해도 오류나 누수가 보고되지 않는다. Xcode에서 실행할 때 콘솔의 마지막 줄에 나타나는 `Program ended with exit code: 0`은 프로그램이 출력한 것이 아니라 Xcode가 프로그램이 끝난 뒤 표시하는 문구이다.

---

<br>

## 2. Q1. 참조 매개변수로 값을 꺼내는 MyIntStack 클래스

### 2.1 요구사항

배열 `p`와 꼭대기 인덱스 `tos`로 정수 스택 `MyIntStack`을 구현한다. `push(n)`은 `n`을 저장하고 성공 여부를 리턴하며, `pop(n)`은 꺼낸 값을 참조 매개변수 `n`에 저장하고 성공 여부를 리턴한다. 강의자료의 한 쪽짜리 판을 따라 스택의 크기는 매크로 상수 `MAX`(2)이고, `main()`은 푸시와 팝을 각각 `MAX + 1`번 시도하여 가득 찬 스택과 빈 스택에서의 동작을 확인한다.

### 2.2 설계와 해결 방법

입출력을 위해 `<iostream>`을 포함하고, 스택의 최대 크기를 `#define MAX 2`로 정의한다. `MyIntStack` 클래스는 저장 공간인 정수 배열 `p[MAX]`와 꼭대기 위치 `tos`를 `private` 멤버로 두고, `public` 아래에 생성자, `push()`, `pop()`의 원형을 두며, 몸체는 클래스 밖에서 정의했다.

- **생성자:** `tos`를 0으로 만들어 빈 스택을 만든다. `tos`는 다음에 값을 넣을 칸의 인덱스이면서 지금 쌓인 값의 개수이다.
- **`push(int n)`:** `tos < MAX`이면 아직 공간이 있으므로 `p[tos]`에 `n`을 저장하고 `tos`를 1 늘린 뒤 `true`를 리턴한다. 가득 차 있으면 아무것도 바꾸지 않고 `false`를 리턴한다.
- **`pop(int& n)`:** `tos > 0`이면 꺼낼 값이 있으므로 `tos`를 1 줄이고 `p[tos]`를 `n`에 저장한 뒤 `true`를 리턴한다. 비어 있으면 `false`를 리턴한다.

`push()`는 `tos`를 쓴 뒤에 늘리고, `pop()`은 `tos`를 줄인 뒤에 읽는다. 이 순서 때문에 마지막에 넣은 값이 먼저 나오는 **후입선출(LIFO, last in first out)** 이 된다. `main()`은 첫 반복에서 0, 1을 푸시하고 세 번째 시도에서 실패 메시지를 출력하며, 구분선을 출력한 뒤 두 번째 반복에서 1, 0을 팝하고 세 번째 시도에서 실패 메시지를 출력한다.

`pop()`이 참조 매개변수를 쓰는 이유는 함수가 두 가지 결과를 돌려주어야 하기 때문이다. 리턴값으로는 성공 여부(`bool`)를 돌려주고, 꺼낸 정수는 호출한 쪽의 변수 `n`에 직접 써 넣는다. 매개변수 `int& n`은 `main()`의 `n`의 별명이므로 `pop()` 안에서 `n = p[tos];`를 실행하면 `main()`의 `n`이 바뀐다([5장 3.2절](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.ko.md#32-참조-매개변수)). 매개변수를 `int n`으로 받으면 값에 의한 호출이 되어 복사본만 바뀌므로 `main()`의 `n`은 초기화되지 않은 값 그대로 남는다. 같은 일을 포인터로 하려면 `bool pop(int* n)`으로 선언하고 `*n = p[tos];`로 쓰며 `a.pop(&n)`으로 호출해야 하는데, 참조를 쓰면 호출하는 쪽이 `&`를 붙이지 않아도 된다.

다음 두 가지는 이 설계의 성질이다.

- **배열의 크기:** 문제의 원래 크기는 10이지만, 코드는 강의자료의 한 쪽짜리 판을 따라 `MAX`를 2로 두었다. `#define MAX 10`으로 바꾸면 0부터 9까지 푸시된 뒤 `11번째 시도 stack full`이 출력되고, 9부터 0까지 팝된 뒤 `11번째 시도 stack empty`가 출력된다. 크기를 매크로 상수 하나로 정해 두었으므로 배열 선언, `push()`의 조건, `main()`의 반복 횟수가 함께 바뀐다.
- **실패할 때의 `n`:** `pop()`이 `false`를 리턴할 때는 `n`에 아무것도 쓰지 않는다. 따라서 호출한 쪽은 리턴값을 먼저 확인한 뒤에만 `n`을 써야 하며, `main()`도 `if (a.pop(n))`일 때만 `n`을 출력한다.

### 2.3 실행 결과와 검토

```text
0 1 
3번째 시도 stack full
------------------------
1 0 
3번째 시도 stack empty
```

첫 줄의 0과 1은 푸시에 성공한 값의 에코이고, 세 번째 푸시는 `tos`가 이미 `MAX`(2)이므로 실패한다. 팝은 마지막에 넣은 1부터 꺼내 `1 0`을 출력하고, 세 번째 팝은 `tos`가 0이므로 실패한다.

---

<br>

## 3. Q2. 참조와 관련한 자유 문제 1

### 3.1 요구사항

참조와 관련된 코드를 자유롭게 작성하고 분석한다. 풀이 코드는 참조 변수, 참조 매개변수, `const` 참조 매개변수의 세 가지 쓰임을 보인다.

### 3.2 설계와 해결 방법

입출력을 위해 `<iostream>`을 포함하고, 참조를 쓰는 세 함수를 선언한다.

| 함수 | 매개변수 | 하는 일 |
|:-----|:-----|:-----|
| `swapNumbers(int& a, int& b)` | 참조 매개변수 2개 | 임시 변수 `temp`로 두 인자의 값을 교환한다 |
| `modifyValue(int& value)` | 참조 매개변수 | 인자의 값을 2배로 만든다 |
| `printValue(const int& value)` | `const` 참조 매개변수 | 인자의 값을 출력한다 |

*표 2. Q2의 세 함수*

`main()`은 참조의 쓰임을 차례로 보인다. 먼저 `int& ref = number;`로 `number`의 별명 `ref`를 만들고, `ref = 100;`을 실행한 뒤 `number`를 출력하여 참조를 통한 변경이 원본에 반영됨을 보인다. 참조 변수는 선언할 때 반드시 초기화해야 하며, 한 번 정한 대상을 다른 변수로 바꿀 수 없다([5장 3.1절](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.ko.md#31-선언)). 그다음 `num1`과 `num2`를 `swapNumbers()`로 교환하고, `value`를 `modifyValue()`로 두 배로 만든다. 두 함수의 매개변수가 참조이므로 함수 안의 변경이 `main()`의 변수에 그대로 반영된다. 마지막으로 `printValue(value)`로 값을 출력한다.

`printValue()`의 매개변수 `const int&`는 두 가지 뜻을 가진다. 참조이므로 인자를 복사하지 않고 원본을 읽으며, `const`이므로 함수 안에서 `value = 0;`처럼 원본을 바꾸려 하면 컴파일 오류가 난다. 또 `const` 참조는 `printValue(7)`처럼 변수가 아닌 값(리터럴이나 식의 결과)도 받을 수 있다. `int`처럼 작은 타입은 값으로 받아도 복사 비용이 거의 없으므로, `const` 참조의 이점은 `string`이나 큰 클래스 객체를 넘길 때 커진다.

> **참고:** 43행의 `const int& constRef = value;`는 `value`에 대한 `const` 참조를 만들지만 이후에 쓰이지 않으며, 바로 다음 줄의 `printValue()`도 `constRef`가 아니라 `value`를 인자로 받는다. 그래서 `-Wall`이 `unused variable 'constRef'` 경고를 낸다. `printValue(constRef);`로 호출하면 경고 없이 같은 결과가 나온다. 어느 쪽이든 `printValue()`의 매개변수가 `const int&`이므로 함수 안에서는 원본을 바꿀 수 없다.

### 3.3 실행 결과와 검토

```text
Original number: 42
After changing through reference: 100

Before swap - num1: 10, num2: 20
After swap - num1: 20, num2: 10

Before modification: 5
After modification: 10
Value is: 10
```

`ref`를 바꾸자 `number`가 42에서 100이 되었고, 교환 뒤 `num1`과 `num2`가 바뀌었으며, `modifyValue()` 뒤 `value`가 5에서 10이 되었다. 세 결과 모두 참조가 원본을 가리키기 때문에 나온다.

---

<br>

## 4. Q3. 참조를 클래스와 연계한 자유 문제 2

### 4.1 요구사항

참조를 클래스와 연계하여 쓰는 코드를 자유롭게 작성하고 분석한다. 풀이 코드는 이름과 나이를 가진 `Person` 클래스로 `const` 참조 매개변수, 참조 리턴, `const` 멤버 함수를 보인다.

### 4.2 설계와 해결 방법

입출력과 문자열 처리를 위해 `<iostream>`과 `<string>`을 포함한다. `Person` 클래스는 `private` 멤버 `name`과 `age`를 두고 다음 멤버 함수를 갖는다.

- **생성자:** `Person(const string& n = "", int a = 0) : name(n), age(a) {}`이다. 매개변수에 디폴트 값이 있어 인자 없이도, 하나나 둘을 주어도 객체를 만들 수 있고, 멤버 초기화 리스트로 두 멤버를 초기화한다.
- **getter 두 쌍:** `string& getName()`과 `int& getAge()`는 멤버에 대한 참조를 리턴하고, `const string& getName() const`와 `const int& getAge() const`는 `const` 참조를 리턴한다. 이름과 매개변수가 같아도 뒤에 붙은 `const`가 다르므로 함수 중복이 성립한다.
- **setter:** `setName(const string& n)`과 `setAge(const int& a)`는 `const` 참조로 받은 값을 멤버에 대입한다.
- **`isOlderThan(const Person& other) const`:** 자기 나이가 `other`의 나이보다 많은지 리턴한다. 같은 클래스의 멤버 함수이므로 다른 객체의 `private` 멤버 `other.age`에도 접근할 수 있다.
- **`printInfo() const`:** 이름과 나이를 출력한다.

클래스 밖의 `celebrateBirthday(Person& person)`은 `person.getAge()++;`로 나이를 1 늘린다. `getAge()`가 `int&`, 즉 객체 안의 `age` 자체를 리턴하므로 `++`가 멤버를 직접 바꾼다. `getAge()`가 `int`를 값으로 리턴했다면 `getAge()++`는 임시 값에 대한 증가가 되어 컴파일 오류가 난다([5장 3.3절](../../Lecture-Notes/Ch05_Functions-and-Copy-Control/Concepts_Lecture.ko.md#33-참조-반환)).

`main()`은 `person1`("Alice", 25)과 `person2`("Bob", 30)를 만들고, `person1.getName() = "Alice Smith";`로 참조를 통해 이름을 바꾼 뒤, `setName("Alice Johnson")`과 `setAge(26)`으로 다시 바꾼다. 두 사람의 정보를 출력하고 `isOlderThan()`으로 나이를 비교하며, `celebrateBirthday(person1)` 뒤에 `person1`을 다시 출력한다.

중복한 getter 중 어느 것이 호출되는지는 객체의 `const` 여부로 정해진다. `person1`과 `person2`는 `const`가 아니므로 `main()`의 호출은 모두 `const`가 아닌 판을 부른다. `const Person p3("Carol", 40);`처럼 `const` 객체를 만들면 `p3.getName()`은 `const` 판을 부르며, `p3.getAge()++;`는 `const int&`를 통해 값을 바꾸려 하므로 `cannot assign to return value because function 'getAge' returns a const value` 오류가 난다. 이처럼 `const` 판의 getter는 `const` 객체에서도 값을 읽을 수 있게 하면서 변경은 막는다.

> **주의:** `const`가 아닌 참조를 리턴하는 getter는 `private` 멤버를 바깥에서 마음대로 바꾸게 한다. `person1.getName() = "Alice Smith";`와 `getAge()++`는 setter를 거치지 않고 멤버를 바꾸므로, setter에서 나이가 음수인지 검사하는 등의 규칙을 두어도 그 규칙을 피해 갈 수 있다. 이 코드는 참조 리턴의 효과를 보이기 위해 이 방식을 쓴 것이며, 캡슐화를 지키려면 getter는 값이나 `const` 참조를 리턴하고 변경은 setter나 `celebrateBirthday()` 같은 멤버 함수로 하는 편이 낫다.

### 4.3 실행 결과와 검토

```text
Person 1: Name: Alice Johnson, Age: 26
Person 2: Name: Bob, Age: 30
Bob is older or same age

After birthday celebration:
Name: Alice Johnson, Age: 27
```

참조로 바꾼 이름 `"Alice Smith"`는 곧이어 `setName()`으로 덮어쓰였으므로 출력에 나타나지 않는다. 26은 30보다 크지 않으므로 `isOlderThan()`이 `false`를 리턴하여 `else` 쪽 문장이 출력되고, `celebrateBirthday()`가 참조를 통해 나이를 27로 늘렸다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 배열 스택 | `tos`는 다음에 넣을 칸의 인덱스이자 쌓인 개수이며, 푸시는 쓴 뒤 늘리고 팝은 줄인 뒤 읽어 후입선출이 된다. |
| 참조 매개변수로 결과 전달 | `pop(int& n)`은 성공 여부를 리턴값으로, 꺼낸 값을 호출한 쪽의 변수에 직접 전달한다. |
| 참조 변수 | 선언할 때 초기화하는 원본의 별명이며, 참조를 바꾸면 원본이 바뀐다. |
| `const` 참조 매개변수 | 복사 없이 원본을 읽고 변경은 막으며, 리터럴 같은 값도 인자로 받을 수 있다. |
| 참조 리턴 | `int& getAge()`의 리턴값은 멤버 자체이므로 `getAge()++`처럼 대입과 증가의 대상이 될 수 있다. |
| `const` 멤버 함수의 중복 | `const` 객체에는 `const` 판의 getter가, 일반 객체에는 일반 판의 getter가 선택된다. |

---

<br>

## 점검 문제

1. **실패 처리:** Q1에서 `MAX`를 3으로 바꾸면 출력은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   푸시는 0, 1, 2에 성공하고 네 번째 시도에서 실패하므로 `0 1 2 `와 `4번째 시도 stack full`이 출력된다. 구분선 뒤에 팝은 2, 1, 0을 꺼내고 네 번째 시도에서 실패하므로 `2 1 0 `과 `4번째 시도 stack empty`가 출력된다.

   </details>

2. **값으로 받는 매개변수:** Q1의 `pop()`을 `bool pop(int n)`으로 바꾸면 두 번째 반복의 출력은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `pop()` 안의 `n`은 복사본이므로 `main()`의 `n`은 바뀌지 않는다. `main()`의 `n`은 초기화되지 않은 지역 변수이므로 성공한 두 번의 팝에서 정해지지 않은 값을 출력하게 되며, 이는 정의되지 않은 동작이다. 꺼낸 값을 호출한 쪽에 돌려줄 수 없으므로 참조나 포인터 매개변수가 필요하다.

   </details>

3. **참조의 재지정:** Q2의 `ref = 100;` 뒤에 `int other = 7; ref = other;`를 실행하면 `ref`가 `other`를 가리키게 되는가?

   <details>
   <summary>정답</summary>

   그렇지 않다. 참조는 초기화할 때 정한 대상을 바꿀 수 없으므로 `ref = other;`는 `number`에 7을 대입하는 문장이다. 실행 뒤 `number`는 7이고 `other`도 7이다.

   </details>

4. **`const` 매개변수:** Q2의 `printValue()` 안에 `value *= 2;`를 넣으면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `value`는 `const int&`이므로 이를 통해 원본을 바꿀 수 없어 컴파일 오류가 난다.

   </details>

5. **getter의 선택:** Q3의 `celebrateBirthday()`의 매개변수를 `const Person& person`으로 바꾸면 컴파일되는가?

   <details>
   <summary>정답</summary>

   컴파일되지 않는다. `person`이 `const`이므로 `person.getAge()`는 `const int&`를 리턴하는 `const` 판이 선택되고, `const` 값을 `++`로 바꾸려 하므로 오류가 난다.

   </details>
