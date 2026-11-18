# 9장 실습 — 가상 함수와 추상 클래스의 활용

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **최종 수정일:** 2026-11-19

> **학습 목표:**
> 1. 추상 클래스가 정한 실행 흐름에 맞추어 순수 가상 함수를 재정의하는 파생 클래스를 작성할 수 있다
> 2. 기본 클래스 포인터의 배열로 서로 다른 파생 클래스 객체를 다루고 동적 바인딩의 결과를 설명할 수 있다
> 3. 가상 함수를 순수 가상 함수로 바꾸어 기본 클래스를 추상 클래스로 만들 수 있다
> 4. 추상 클래스로 정의한 인터페이스를 구현하는 동적 배열 기반의 스택을 작성할 수 있다

---

## 목차

- [1. 개요](#1-개요)
- [2. 단위 변환기](#2-단위-변환기)
- [3. 사각형에 내접하는 도형](#3-사각형에-내접하는-도형)
  - [3.1 가상 함수로 넓이 구하기](#31-가상-함수로-넓이-구하기)
  - [3.2 Shape를 추상 클래스로 바꾸기](#32-shape를-추상-클래스로-바꾸기)
- [4. 추상 스택과 정수 스택](#4-추상-스택과-정수-스택)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 개요

> **참고:** 업캐스팅, 가상 함수와 동적 바인딩, 재정의, 가상 소멸자, 순수 가상 함수와 추상 클래스의 이론은 [9장 강의 노트](Concepts_Lecture.ko.md)에서 다룬다. 이 실습은 주어진 기본 클래스나 추상 클래스를 상속받아 가상 함수를 재정의하는 세 가지 문제를 푼다.

| 주제 | 위치 |
|:-----|:-----|
| 추상 클래스 `Converter`를 상속받아 원을 달러로 바꾸는 `WonToDollar` | 2절 |
| `Shape`를 상속받는 `Oval`, `Rect`, `Triangular`와 추상 클래스로 바꾼 `Shape` | 3절 |
| 추상 클래스 `AbstractStack`을 구현하는 `IntStack` | 4절 |
| 가상 함수로 학년별 평균을 구하는 `Student` 계층, 추상 클래스 `Convert`를 상속받은 단위 변환 | [9장 연습 문제 1, 2](Exercises.ko.md) |

*표 1. 실습의 주제와 위치*

모든 코드는 C++17 표준으로 컴파일하여 실행한다(예: `g++ -std=c++17 -Wall`).

---

<br>

## 2. 단위 변환기

**문제:** 단위를 바꾸는 추상 클래스 `Converter`가 다음과 같다. 이를 상속받아 원화를 달러로 환산하는 `WonToDollar` 클래스를 작성한다. 생성자의 인자는 1달러에 해당하는 원화이다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Converter {
protected:
    double ratio;
    virtual double convert(double src) = 0;     // src를 다른 단위로 변환
    virtual string getSourceString() = 0;       // 원래 단위의 명칭
    virtual string getDestString() = 0;         // 바꿀 단위의 명칭
public:
    Converter(double ratio) { this->ratio = ratio; }
    void run() {
        double src;
        cout << getSourceString() << "을 " << getDestString() << "로 바꿉니다. ";
        cout << getSourceString() << "을 입력하세요>> ";
        cin >> src;
        cout << "변환 결과 : " << convert(src) << getDestString() << endl;
    }
};

class WonToDollar : public Converter {
public:
    WonToDollar(double ratio) : Converter(ratio) { }
protected:
    double convert(double src) override { return src / ratio; }   // 원 / (1달러당 원)
    string getSourceString() override { return "원"; }
    string getDestString() override { return "달러"; }
};

int main() {
    WonToDollar wd(1100);               // 1달러에 1100원
    wd.run();
}
```

```text
원을 달러로 바꿉니다. 원을 입력하세요>> 100000
변환 결과 : 90.9091달러
```

- **틀을 정하는 기본 클래스:** `run()`은 입력을 받고 변환하여 출력하는 흐름을 정해 두고, 단위마다 달라지는 부분(변환식과 단위의 이름)만 순수 가상 함수로 남긴다. 파생 클래스는 그 세 함수만 재정의하면 된다.
- **동적 바인딩:** `run()`은 `Converter`의 함수이지만, `wd.run()` 안의 `convert(src)` 호출은 가상 함수이므로 실제 객체인 `WonToDollar`의 `convert()`로 연결된다. 100000 / 1100 ≈ 90.9091이다.
- **접근 지정:** 세 가상 함수는 `protected`이므로 외부에서 직접 호출할 수 없고, `run()`을 통해서만 쓰인다. 재정의한 함수도 같은 이유로 `protected`에 둔다. `override`는 기본 클래스의 가상 함수를 재정의한다는 뜻을 컴파일러가 확인하게 한다(9장 강의 노트 2.6절).

---

<br>

## 3. 사각형에 내접하는 도형

다음 `Shape`는 너비 `width`, 높이 `height`인 사각형에 내접하는 도형을 표현한다.

```cpp
class Shape {
protected:
    string name;                        // 도형의 이름
    int width, height;                  // 도형이 내접하는 사각형의 너비와 높이
public:
    Shape(string n = "", int w = 0, int h = 0) {
        name = n; width = w; height = h;
    }
    virtual double getArea() { return 0; }      // 의미 없는 값 리턴
    string getName() { return name; }
};
```

### 3.1 가상 함수로 넓이 구하기

**문제:** `Shape`를 상속받아 타원 `Oval`, 사각형 `Rect`, 삼각형 `Triangular` 클래스를 작성하여 다음 `main()`이 실행 예시처럼 출력하게 한다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Shape {
protected:
    string name;                        // 도형의 이름
    int width, height;                  // 도형이 내접하는 사각형의 너비와 높이
public:
    Shape(string n = "", int w = 0, int h = 0) {
        name = n; width = w; height = h;
    }
    virtual double getArea() { return 0; }      // 의미 없는 값 리턴
    string getName() { return name; }
    virtual ~Shape() { }
};

class Oval : public Shape {
public:
    Oval(string n, int w, int h) : Shape(n, w, h) { }
    double getArea() override { return 3.14 * width * height; }
};

class Rect : public Shape {
public:
    Rect(string n, int w, int h) : Shape(n, w, h) { }
    double getArea() override { return width * height; }
};

class Triangular : public Shape {
public:
    Triangular(string n, int w, int h) : Shape(n, w, h) { }
    double getArea() override { return width * height / 2.0; }
};

int main() {
    Shape* p[3];
    p[0] = new Oval("빈대떡", 10, 20);
    p[1] = new Rect("찰떡", 30, 40);
    p[2] = new Triangular("토스트", 30, 40);
    for (int i = 0; i < 3; i++)
        cout << p[i]->getName() << " 넓이는 " << p[i]->getArea() << endl;
    for (int i = 0; i < 3; i++)
        delete p[i];
}
```

```text
빈대떡 넓이는 628
찰떡 넓이는 1200
토스트 넓이는 600
```

- **기본 클래스 포인터의 배열:** `p`의 원소는 모두 `Shape*`이지만 가리키는 객체는 각각 `Oval`, `Rect`, `Triangular`이다. `getArea()`가 가상 함수이므로 `p[i]->getArea()`는 실행 중에 실제 객체의 함수로 연결되어 도형마다 다른 넓이를 낸다. `getName()`은 가상이 아니지만 세 클래스가 재정의하지 않았으므로 `Shape`의 함수가 그대로 쓰인다.
- **가상 소멸자의 추가:** 주어진 `Shape`에는 소멸자가 없지만, 위 코드는 `virtual ~Shape() { }`를 더했다. `delete p[i]`는 `Shape*`로 파생 클래스 객체를 지우는데, 기본 클래스의 소멸자가 가상이 아니면 이는 정의되지 않은 동작이다. 기본 클래스 포인터로 파생 객체를 지울 클래스에는 가상 소멸자를 둔다(9장 강의 노트 3절).

> **참고:** 타원의 넓이 628은 3.14 × 10 × 20으로, `width`와 `height`를 타원의 두 반지름으로 보고 계산한 값이다. 너비 10, 높이 20인 사각형에 내접하는 타원의 두 반지름은 5와 10이므로, 이름 그대로의 의미대로 계산하면 넓이는 3.14 × 5 × 10 = 157이다. 삼각형(밑변 30, 높이 40에서 600)과 사각형(1200)은 내접 도형의 넓이와 일치한다.

### 3.2 Shape를 추상 클래스로 바꾸기

**문제:** `Shape`를 추상 클래스로 만든 뒤 3.1절의 `main()`이 그대로 실행되게 한다.

`Shape::getArea()`는 "의미 없는 값"을 리턴하는 함수였다. 넓이를 정할 수 없는 일반적인 도형에 억지로 0을 주는 대신, 이를 순수 가상 함수로 바꾸면 `Shape`는 추상 클래스가 된다. 3.1절의 코드에서 한 줄만 바꾼다.

```cpp
    virtual double getArea() = 0;               // 순수 가상 함수: Shape는 추상 클래스
```

```text
빈대떡 넓이는 628
찰떡 넓이는 1200
토스트 넓이는 600
```

- `main()`은 `Shape` 객체를 만들지 않고 `Shape*`만 쓰므로 그대로 실행되며, 출력도 3.1절과 같다.
- 추상 클래스로는 객체를 만들 수 없으므로 `Shape s("x", 1, 1);`은 `cannot declare variable 's' to be of abstract type 'Shape'` 오류가 된다. 파생 클래스가 `getArea()`를 재정의하지 않으면 그 파생 클래스도 추상 클래스로 남아 객체를 만들 수 없다. 넓이를 반드시 구현하도록 파생 클래스에 강제하는 효과가 있다.

---

<br>

## 4. 추상 스택과 정수 스택

**문제:** 다음 추상 클래스 `AbstractStack`을 상속받는 `IntStack`을 작성한다. `IntStack`은 `private` 멤버로 스택의 크기 `capacity`, 저장된 정수의 개수 `tos`(top of stack), 동적으로 할당한 스택 메모리 `int* p`를 가지며, 정수 인자 하나를 받는 생성자, 소멸자, 상속받은 순수 가상 함수의 정의를 가진다.

```cpp
#include <iostream>
using namespace std;

class AbstractStack {
public:
    virtual bool push(int n) = 0;       // n을 푸시, 스택이 가득 차면 false 리턴
    virtual bool pop(int& n) = 0;       // 팝한 정수를 n에 저장, 비어 있으면 false 리턴
    virtual int size() = 0;             // 현재 저장된 정수의 개수 리턴
    virtual ~AbstractStack() { }
};

class IntStack : public AbstractStack {
    int capacity;                       // 스택의 크기
    int tos;                            // 저장된 정수의 개수 (top of stack)
    int* p;                             // 동적으로 할당한 스택 메모리
public:
    IntStack(int capacity) {
        this->capacity = capacity;
        tos = 0;
        p = new int[capacity];
    }
    ~IntStack() { delete[] p; }
    bool push(int n) override {
        if (tos == capacity)
            return false;
        p[tos++] = n;                   // tos 자리에 넣고 tos 증가
        return true;
    }
    bool pop(int& n) override {
        if (tos == 0)
            return false;
        n = p[--tos];                   // tos를 줄이고 그 자리의 값을 꺼냄
        return true;
    }
    int size() override { return tos; }
};

int main() {
    IntStack a(2);
    for (int i = 0; i < 4; i++) {       // 처음 2개는 push 성공, 다음 2개는 실패
        if (a.push(i))
            cout << "push 성공" << endl;
        else
            cout << "스택 full" << endl;
    }
    cout << "===============" << endl;
    int n;
    for (int i = 0; i < 3; i++) {       // 처음 2개는 pop 성공, 다음 1개는 실패
        if (a.pop(n))
            cout << "pop 성공 " << n << endl;
        else
            cout << "스택 empty" << endl;
    }
}
```

```text
push 성공
push 성공
스택 full
스택 full
===============
pop 성공 1
pop 성공 0
스택 empty
```

- **인터페이스와 구현의 분리:** `AbstractStack`은 스택이 할 수 있는 일(넣기, 꺼내기, 크기)만 정하고, 저장 방법은 정하지 않는다. `IntStack`은 동적 배열로 이를 구현한다. 스택을 쓰는 코드가 `AbstractStack&`나 `AbstractStack*`만 다루면, 연결 리스트로 구현한 다른 스택으로 바꾸어도 그 코드를 고칠 필요가 없다.
- **tos의 의미:** `tos`는 저장된 정수의 개수이자 다음에 넣을 자리의 인덱스이다. `push()`는 `p[tos]`에 넣고 `tos`를 늘리며, `pop()`은 `tos`를 먼저 줄이고 `p[tos]`를 꺼낸다. 마지막에 넣은 1이 먼저 나오는 후입선출(LIFO) 순서가 된다.
- **참조 매개변수로 결과 전달:** `pop(int& n)`은 꺼낸 값을 참조 매개변수로, 성공 여부를 리턴값으로 돌려준다(5장 실습 4절).
- **가상 소멸자:** `AbstractStack`에 가상 소멸자를 두었으므로 `AbstractStack*`로 `IntStack` 객체를 지워도 `IntStack`의 소멸자가 호출되어 `p`가 반환된다.

`IntStack`은 생성자에서 `new int[capacity]`로 메모리를 할당하고 소멸자에서 반환하지만, 복사 생성자와 대입 연산자는 작성하지 않았다. 위의 `main()`은 객체를 복사하지 않으므로 문제가 없지만, 다음처럼 `IntStack` 객체를 복사하면 5장 4.3절의 얕은 복사 문제가 그대로 생긴다.

```cpp
int main() {
    IntStack a(2);
    a.push(7);
    IntStack b = a;                     // 디폴트 복사 생성자: b.p와 a.p가 같은 메모리를 가리킨다
    b.push(9);                          // a의 메모리에 9를 쓴다
    int n;
    a.pop(n);
    cout << n << endl;
    return 0;                           // b, a 순으로 소멸하며 같은 메모리를 두 번 delete[]
}
```

```text
7
```

`b`는 `a`의 `capacity`, `tos`, `p`를 그대로 복사하므로 두 스택이 배열 하나를 함께 쓴다. `b.push(9)`는 공유한 배열의 1번 자리에 9를 쓰고 `b.tos`만 2로 만든다. `a.tos`는 여전히 1이므로 `a.pop(n)`은 0번 자리의 7을 꺼낸다. 출력은 정상처럼 보이지만, `main()`이 끝날 때 `b`와 `a`의 소멸자가 같은 배열을 두 번 `delete[]`하는 정의되지 않은 동작이 생긴다. macOS에서는 프로그램이 비정상 종료하며, AddressSanitizer로 실행하면 `attempting double-free`가 보고된다. 3의 법칙(5장 4.9절)에 따라 `IntStack`에 깊은 복사를 하는 복사 생성자와 대입 연산자를 두거나, 복사할 필요가 없다면 두 함수를 `= delete`로 선언하여 복사를 금지한다(14장 4.4절).

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 틀을 정하는 추상 클래스 | 기본 클래스가 실행 흐름을 정하고 바뀌는 부분만 순수 가상 함수로 남기면, 파생 클래스는 그 함수만 재정의한다. |
| 동적 바인딩 | 기본 클래스 포인터로 가상 함수를 호출하면 실행 중에 실제 객체의 함수가 선택된다. |
| 추상 클래스로의 변환 | 의미 없는 값을 리턴하던 가상 함수를 `= 0`인 순수 가상 함수로 바꾸면 객체를 만들 수 없고, 파생 클래스에 구현을 강제한다. |
| 가상 소멸자 | 기본 클래스 포인터로 파생 객체를 `delete`하려면 기본 클래스의 소멸자가 가상이어야 한다. |
| 인터페이스의 구현 | 추상 클래스는 할 일만 정하고, 파생 클래스가 동적 배열 같은 구체적인 저장 방법으로 구현한다. |
| 동적 메모리를 가진 구현 클래스의 복사 | `IntStack`처럼 생성자에서 할당하고 소멸자에서 반환하는 클래스는 복사하면 배열을 공유하고 이중 해제가 생기므로, 3의 법칙에 따라 복사 함수를 작성하거나 삭제한다. |

---

<br>

## 점검 문제

1. 2절의 `Converter`를 상속받아 섭씨를 화씨로 바꾸는 `CelsiusToFahrenheit`를 작성할 때, 재정의해야 하는 함수와 `convert()`의 식은 무엇인가? 이 변환에서 `ratio`는 어떻게 쓰이는가?

   <details>
   <summary>정답</summary>

   `convert()`, `getSourceString()`, `getDestString()`의 세 순수 가상 함수를 재정의해야 한다. 섭씨 c를 화씨로 바꾸는 식은 c × 9 / 5 + 32이므로 `return src * 9 / 5 + 32;`로 쓴다. 이 변환은 비율 하나로 표현되지 않으므로 `ratio`를 쓰지 않으며, 생성자에서 `Converter(0)`처럼 아무 값이나 넘겨도 된다. 단위 이름은 `"섭씨"`, `"화씨"`처럼 리턴한다.

   </details>

2. 3.1절에서 `Shape::getArea()` 앞의 `virtual`을 지우면 출력은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   세 줄 모두 `넓이는 0`이 출력된다. `getArea()`가 가상이 아니면 `p[i]->getArea()`는 포인터의 타입인 `Shape`의 함수로 컴파일할 때 정해지는 정적 바인딩이 되어, 실제 객체와 관계없이 `Shape::getArea()`가 0을 리턴한다. 이때 파생 클래스의 `getArea()`에 붙은 `override`는 재정의할 가상 함수가 없으므로 컴파일 오류가 되며, `override`를 지워야 컴파일된다.

   </details>

3. 4절의 `main()`에서 `pop()` 반복을 `for (int i = 0; i < 3; i++)`에서 `while (a.size() > 0)`으로 바꾸면 출력의 아래쪽은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   스택에 1과 0이 남아 있으므로 `pop 성공 1`, `pop 성공 0`이 출력되고, 크기가 0이 되면 반복이 끝나므로 `스택 empty`는 출력되지 않는다. `size()`로 남은 개수를 먼저 확인하면 꺼낼 수 없는 `pop()` 호출을 하지 않게 된다.

   </details>
