# 4장 실습 — 객체 포인터, 동적 메모리, this, 문자열

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **최종 수정일:** 2026-11-18

> **학습 목표:**
> 1. 객체 포인터로 객체 배열의 원소를 차례로 다루고 `p[i]`와 `*(p + i)`의 관계를 설명할 수 있다
> 2. 실행 중에 크기가 정해지는 배열이 표준 C++에서 허용되지 않는 이유를 설명하고 `new`와 `delete[]`로 바꿀 수 있다
> 3. 매개변수와 멤버 변수의 이름이 같을 때 생기는 논리 오류를 찾고 `this`로 고칠 수 있다
> 4. `string`의 비교, 변환, 부분 문자열, 삽입, 삭제, 검색, 치환 함수를 사용할 수 있다
> 5. 동적 객체 배열을 멤버로 가진 클래스의 생성자와 소멸자를 작성할 수 있다

---

## 목차

- [1. 개요](#1-개요)
- [2. 객체 포인터와 객체 배열](#2-객체-포인터와-객체-배열)
  - [2.1 포인터로 객체 배열 다루기](#21-포인터로-객체-배열-다루기)
  - [2.2 복소수 클래스 연습](#22-복소수-클래스-연습)
- [3. 정적 할당과 동적 할당](#3-정적-할당과-동적-할당)
  - [3.1 실행 중에 크기가 정해지는 배열](#31-실행-중에-크기가-정해지는-배열)
  - [3.2 new와 delete[]로 바꾸기](#32-new와-delete로-바꾸기)
- [4. this 포인터](#4-this-포인터)
  - [4.1 매개변수와 멤버의 이름이 같을 때](#41-매개변수와-멤버의-이름이-같을-때)
  - [4.2 this로 멤버 구별하기](#42-this로-멤버-구별하기)
- [5. string 클래스](#5-string-클래스)
  - [5.1 첫 글자를 맨 뒤로 보내기](#51-첫-글자를-맨-뒤로-보내기)
  - [5.2 비교, 변환, 수정 함수](#52-비교-변환-수정-함수)
- [6. 동적 객체 배열을 가진 클래스](#6-동적-객체-배열을-가진-클래스)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 개요

> **참고:** 객체 포인터, `new`와 `delete`, 동적 객체 배열, `this` 포인터, `string` 클래스의 이론은 [4장 강의 노트](Concepts_Lecture.ko.md)에서 다룬다. 이 실습은 그 내용을 코드로 확인하고, 동적 메모리를 쓰는 클래스를 직접 작성한다.

| 주제 | 위치 |
|:-----|:-----|
| 객체 포인터로 객체 배열 다루기, 복소수 클래스 연습 | 2절 |
| `Complex` 객체 배열을 포인터로 다루기 | [HW2 문제 1](../../Coursework/HW02_Object-Pointers-and-This/Original/Assignment.ko.md#문제-1-5주차-강의자료-문제-5-2) |
| 실행 중에 크기가 정해지는 배열과 동적 할당 | 3절 |
| `this` 포인터가 필요한 경우 | 4절 |
| `string`의 입력과 주요 멤버 함수 | 5절 |
| 동적으로 만든 `Complex` 객체, `Member` 클래스, 정수 5개의 평균, 면적이 100 이상인 원의 개수 | [HW3 문제 1\~4](../../Coursework/HW03_Dynamic-Memory/Original/Assignment.ko.md) |
| 정수 배열을 동적으로 할당하는 `Sample` 클래스, `Circle`과 `CircleManager` 클래스 | [4장 연습 문제 1, 2](Exercises.ko.md) |
| 가족 구성원을 동적 배열로 관리하는 `Family` 클래스 | 6절 |

*표 1. 실습의 주제와 위치*

모든 코드는 C++17 표준으로 컴파일하여 실행한다(예: `g++ -std=c++17 -Wall`).

---

<br>

## 2. 객체 포인터와 객체 배열

### 2.1 포인터로 객체 배열 다루기

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int r) { radius = r; }
    void setRadius(int r) { radius = r; }
    double getArea() { return 3.14 * radius * radius; }
};

int main() {
    Circle circleArray[3] = { Circle(10), Circle(20), Circle() };  // 원소마다 생성자 지정

    Circle* p = circleArray;            // 배열 이름은 첫 원소의 주소
    for (int i = 0; i < 3; i++) {
        cout << "Circle " << i << ": " << p->getArea() << endl;
        p++;                            // 다음 원소로 이동
    }

    p = circleArray;
    p[2].setRadius(30);                 // p[i]는 *(p + i)와 같음
    cout << "Circle 2: " << (*(p + 2)).getArea() << endl;
}
```

```text
Circle 0: 314
Circle 1: 1256
Circle 2: 3.14
Circle 2: 2826
```

- **배열의 초기화:** 중괄호 안에 원소마다 생성자 호출을 적으면 `circleArray[0]`은 `Circle(10)`, `circleArray[1]`은 `Circle(20)`, `circleArray[2]`는 기본 생성자 `Circle()`로 만들어진다. 초기화 목록이 없으면 세 원소 모두 기본 생성자로 만들어지므로, 객체 배열을 선언하려면 기본 생성자가 있어야 한다.
- **포인터 연산:** `p++`는 주소를 1바이트가 아니라 `sizeof(Circle)`만큼 늘려 다음 원소를 가리키게 한다. `p->getArea()`는 `(*p).getArea()`와 같다.
- **첨자와 포인터:** `p[2]`는 `*(p + 2)`와 같은 원소이다. 포인터를 옮기지 않고 첨자로 원소를 고를 수도 있다.

### 2.2 복소수 클래스 연습

**문제:** 실수부와 허수부를 가진 복소수 클래스 `Complex`를 다음과 같이 정의하고, 아래 순서대로 동작하는 `main()`을 작성한다.

1. `private` 정수 멤버 변수 `real`, `image`
2. 기본 생성자: `real`, `image`를 각각 1로 초기화
3. 매개변수 2개 생성자: 두 정수 인자로 `real`, `image`를 초기화
4. `real`, `image`를 각각 수정하는 set 멤버 함수
5. `real`과 `image`를 복소수 형태로 출력하는 `showC()`

`main()`은 객체 `vx`를 기본 생성자로, `vy`를 인자 (-3, 2)로 만들고, `vy`의 주소를 포인터 `p`에 담는다. 두 객체를 출력한 뒤 `vx`의 `real`을 4로, `p`로 `vy`의 `image`를 9로 바꾸고 다시 출력한다.

```cpp
#include <iostream>
using namespace std;

class Complex {
    int real, image;
public:
    Complex() { real = 1; image = 1; }
    Complex(int r, int i) { real = r; image = i; }
    void setReal(int r) { real = r; }
    void setImage(int i) { image = i; }
    void showC() { cout << real << " + " << image << "i" << endl; }
};

int main() {
    Complex vx;                         // 기본 생성자: 1 + 1i
    Complex vy(-3, 2);
    Complex* p = &vy;                   // vy의 주소를 가리키는 포인터

    vx.showC();
    vy.showC();
    cout << "--------------" << endl;
    vx.setReal(4);
    p->setImage(9);                     // 포인터로 vy의 image 갱신
    vx.showC();
    p->showC();
}
```

```text
1 + 1i
-3 + 2i
--------------
4 + 1i
-3 + 9i
```

`p->setImage(9)`는 `p`가 가리키는 객체 `vy`의 멤버 함수를 호출하므로 `vy`의 `image`가 바뀐다. 포인터는 객체의 사본이 아니라 원래 객체를 가리킨다. 같은 클래스로 객체 배열을 만들어 포인터로 다루는 문제는 [HW2 문제 1](../../Coursework/HW02_Object-Pointers-and-This/Original/Assignment.ko.md#문제-1-5주차-강의자료-문제-5-2)이다.

---

<br>

## 3. 정적 할당과 동적 할당

### 3.1 실행 중에 크기가 정해지는 배열

다음 코드는 배열의 크기를 키보드로 입력받아 배열을 만든다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int size;
    cout << "input size: ";
    cin >> size;
    int data[size];                     // 실행 중에 정해지는 크기의 배열
    for (int i = 0; i < size; i++)
        cin >> data[i];
}
```

```text
warning: ISO C++ forbids variable length array 'data' [-Wvla]
warning: variable length arrays in C++ are a Clang extension [-Wvla-cxx-extension]
```

첫 줄은 GCC(`-pedantic`), 둘째 줄은 Clang의 경고이다. 표준 C++에서 배열의 크기는 컴파일할 때 알 수 있는 상수 식이어야 하므로 `int data[size];`는 표준이 허용하지 않는 코드이다. Visual Studio(MSVC)는 이 코드를 컴파일 오류로 처리하고, GCC와 Clang은 C99의 가변 길이 배열(variable length array)을 확장 기능으로 받아들여 컴파일하되 경고를 낸다. 이식성 있는 코드를 쓰려면 이 확장 기능에 기대지 않는다.

| 구분 | 정적 할당 | 동적 할당 |
|:-----|:-----|:-----|
| 메모리 영역 | 스택 | 힙 |
| 크기가 정해지는 시점 | 컴파일 시점 | 실행 시점 |
| 할당과 반환 | 함수가 시작하고 끝날 때 자동 | `new`로 할당하고 `delete`로 반환 |
| 예 | 일반적인 지역 변수, 크기가 상수인 배열 | `new int[size]`, `new Circle(30)` |

*표 2. 정적 할당과 동적 할당의 비교*

### 3.2 new와 delete[]로 바꾸기

크기를 실행 중에 정해야 하는 배열은 힙에 동적으로 할당한다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int size;
    cout << "input size: ";
    cin >> size;
    int* p = new int[size];             // 힙에서 size개의 int 할당
    for (int i = 0; i < size; i++) {
        cout << i + 1 << " data input: ";
        cin >> p[i];
    }
    cout << "=== DATA ===" << endl;
    for (int i = 0; i < size; i++)
        cout << p[i] << endl;
    delete[] p;                         // 배열로 할당한 메모리는 delete[]로 반환
    p = nullptr;
}
```

```text
input size: 3
1 data input: 10
2 data input: 20
3 data input: 30
=== DATA ===
10
20
30
```

- `new int[size]`는 힙에서 `int` `size`개의 연속된 공간을 할당하고 첫 원소의 주소를 리턴한다. 포인터 `p`도 배열 이름처럼 `p[i]`로 쓸 수 있다.
- `new[]`로 할당한 메모리는 반드시 `delete[]`로 반환한다. `delete p;`로 반환하면 정의되지 않은 동작이다. 반환한 뒤 `p`에 `nullptr`를 넣어 두면 반환된 메모리를 실수로 다시 쓰는 일을 막는 데 도움이 된다.
- 동적 할당한 메모리를 반환하지 않으면 프로그램이 끝날 때까지 그 메모리를 다시 쓸 수 없는 메모리 누수가 생긴다(4장 강의 노트 4.1절). 현대 C++에서는 `std::vector<int> data(size);`처럼 메모리를 스스로 관리하는 컨테이너를 쓰는 것이 일반적이다(10장).

---

<br>

## 4. this 포인터

### 4.1 매개변수와 멤버의 이름이 같을 때

> **용어 정의:** **this 포인터(this pointer)** 는 멤버 함수 안에서 그 함수를 호출한 객체 자신을 가리키는 포인터로, 컴파일러가 멤버 함수에 숨은 매개변수로 넣는다.

다음 `Circle`은 생성자와 `setRadius()`의 매개변수 이름을 멤버 변수와 같은 `radius`로 지었다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { radius = 1; }
    Circle(int radius) { radius = radius; }               // 매개변수에 자기 자신을 대입
    void setRadius(int radius) { radius = radius; }       // 멤버는 바뀌지 않음
    int getRadius() { return radius; }
};

int main() {
    Circle c1;
    Circle c2(2);
    c1.setRadius(4);
    c2.setRadius(5);
    cout << c1.getRadius() << endl;
    cout << c2.getRadius() << endl;
}
```

```text
warning: explicitly assigning value of variable of type 'int' to itself; did you mean to assign to member 'radius'? [-Wself-assign]
warning: explicitly assigning value of variable of type 'int' to itself; did you mean to assign to member 'radius'? [-Wself-assign]
```

함수 안에서 이름 `radius`는 가장 가까운 범위의 매개변수를 가리키므로 `radius = radius;`는 매개변수에 자기 자신을 대입할 뿐 멤버 변수를 바꾸지 않는다. 문법 오류가 아니므로 컴파일은 되고, Clang은 위와 같이 경고한다. 실행하면 `c1`은 기본 생성자가 정한 1을 그대로 출력하고, `c2`의 `radius`는 한 번도 초기화되지 않아 그 값을 읽는 것은 정의되지 않은 동작이다.

### 4.2 this로 멤버 구별하기

해결 방법은 두 가지이다. 하나는 매개변수 이름을 `r`처럼 멤버와 다르게 짓는 것이고, 다른 하나는 `this->radius`로 멤버를 명시하는 것이다.

```cpp
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() { this->radius = 1; }
    Circle(int radius) { this->radius = radius; }         // 멤버 = 매개변수
    void setRadius(int radius) { this->radius = radius; }
    int getRadius() { return radius; }
};

int main() {
    Circle c1;
    Circle c2(2);
    c1.setRadius(4);
    c2.setRadius(5);
    cout << c1.getRadius() << endl;
    cout << c2.getRadius() << endl;
}
```

```text
4
5
```

- 컴파일러는 `c1.setRadius(4)`를 `setRadius(&c1, 4)`처럼 객체의 주소를 숨은 매개변수 `this`로 넘기는 호출로 바꾼다. 그래서 같은 함수라도 `c1`에서 호출하면 `this`는 `c1`을, `c2`에서 호출하면 `c2`를 가리킨다.
- `this`는 멤버 함수 안에서만 쓸 수 있다. 멤버가 아닌 함수에는 객체가 없고, `static` 멤버 함수는 객체 없이 호출될 수 있으므로 `this`가 없다(6장).
- `this`는 멤버 함수가 객체 자신의 주소나 참조를 리턴할 때에도 쓰이며, 이는 연산자 중복에서 자주 필요하다(5장의 참조 리턴, 7장).

---

<br>

## 5. string 클래스

### 5.1 첫 글자를 맨 뒤로 보내기

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cout << "문자열을 입력하세요(한글 안됨) " << endl;
    getline(cin, s, '\n');              // 공백까지 포함하여 한 줄 입력
    int len = s.length();
    string first = s.substr(0, 1);      // 맨 앞의 문자 1개
    string sub = s.substr(1, len - 1);  // 나머지 문자들
    s = sub + first;                    // 앞 문자를 맨 뒤로 보냄
    cout << s << endl;
}
```

```text
문자열을 입력하세요(한글 안됨) 
You
ouY
```

- `getline(cin, s, '\n')`은 줄바꿈 문자 앞까지 공백을 포함한 한 줄을 읽는다. `cin >> s`는 공백에서 멈추므로 한 단어만 읽는다.
- `substr(pos, n)`은 `pos`번 위치부터 문자 `n`개를 새 `string`으로 리턴하고, `+`는 두 문자열을 이어 붙인다.
- **한글이 안 되는 이유:** `string`은 문자가 아니라 바이트(`char`)의 열이다. UTF-8에서 한글 한 글자는 3바이트이므로 `substr(0, 1)`은 첫 글자의 첫 바이트만 떼어 내고, 그 바이트를 뒤로 옮기면 글자가 깨진다. `length()`도 글자 수가 아니라 바이트 수를 리턴한다.

### 5.2 비교, 변환, 수정 함수

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string sFirst = "Hello";
    string sSnd = "Hi";
    string data = "123";

    if (sFirst.compare(sSnd) == 0)
        cout << "Same String" << endl;
    else
        cout << "Different String!" << endl;

    cout << "Length of sFirst: " << sFirst.length() << endl;
    cout << "string - integers method:" << stoi(data) + 100 << endl;

    sSnd = sFirst.substr(0, 2);         // 0번부터 2개: "He"
    cout << "1: " << sSnd << endl;
    sSnd.append("~!");                  // 끝에 덧붙임: "He~!"
    cout << "2: " << sSnd << endl;
    sSnd.insert(1, "i!");               // 1번 위치에 삽입: "Hi!e~!"
    cout << "3: " << sSnd << endl;
    sSnd.erase(3, 2);                   // 3번부터 2개 삭제: "Hi!!"
    cout << "4: " << sSnd << endl;
    int n = sSnd.find("Hi");            // 처음 나타나는 위치: 0
    cout << n << endl;
    sSnd.replace(n, 3, sFirst);         // n번부터 3개를 "Hello"로 바꿈: "Hello!"
    cout << "5: " << sSnd << endl;
}
```

```text
Different String!
Length of sFirst: 5
string - integers method:223
1: He
2: He~!
3: Hi!e~!
4: Hi!!
0
5: Hello!
```

| 함수 | 하는 일 |
|:-----|:-----|
| `compare(s)` | 사전 순서로 비교하여 같으면 0, 앞서면 음수, 뒤서면 양수 리턴 |
| `length()` | 바이트 수 리턴 (`size()`와 같음) |
| `stoi(s)` | 숫자로 된 문자열을 `int`로 변환 (C++11) |
| `substr(pos, n)` | `pos`번부터 `n`개의 부분 문자열 리턴 |
| `append(s)` | 끝에 `s`를 덧붙임 |
| `insert(pos, s)` | `pos`번 위치 앞에 `s`를 삽입 |
| `erase(pos, n)` | `pos`번부터 `n`개를 삭제 |
| `find(s)` | `s`가 처음 나타나는 위치 리턴, 없으면 `string::npos` |
| `replace(pos, n, s)` | `pos`번부터 `n`개를 `s`로 바꿈 |

*표 3. 예제에 쓰인 `string` 멤버 함수와 변환 함수*

- `"Hello"`와 `"Hi"`는 둘째 글자 `'e'`(101)와 `'i'`(105)에서 갈리며, `'e'`가 앞서므로 `compare()`는 음수를 리턴한다.
- `insert(1, "i!")`는 `"He~!"`의 1번 위치(`'e'`) 앞에 끼워 `"Hi!e~!"`를 만들고, `erase(3, 2)`는 3번 `'e'`와 4번 `'~'`를 지워 `"Hi!!"`를 만든다.
- `find()`는 찾지 못하면 `string::npos`를 리턴하는데, 이 값을 `int`에 담으면 -1처럼 보이므로 실무 코드에서는 결과를 `string::size_type`(또는 `size_t`)에 담고 `npos`와 비교한다.

---

<br>

## 6. 동적 객체 배열을 가진 클래스

**문제:** `Person`은 사람을, `Family`는 가족을 추상화한 클래스이다. 아래 `main()`이 실행 예시처럼 동작하도록 두 클래스에 빠진 멤버 변수와 멤버 함수를 추가한다.

```cpp
class Person {
    string name;
public:
    Person() { name = ""; }
    Person(string name) { this->name = name; }
};

class Family {
    Person* p;                          // Person 배열 포인터
    int size;                           // 가족 구성원 수
public:
    Family(string name, int size);      // size개의 Person 배열 동적 생성
    void show();                        // 모든 가족 구성원 출력
    ~Family();
};
```

```cpp
int main() {
    Family* simpson = new Family("Simpson", 3);
    simpson->setName(0, "Mr. Simpson");
    simpson->setName(1, "Mrs. Simpson");
    simpson->setName(2, "Bart Simpson");
    simpson->show();
    delete simpson;
}
```

`main()`이 쓰는 `setName(int, string)`이 `Family`에 없고, 가족 이름을 저장할 멤버 변수와 `Person`의 이름을 바꾸고 읽을 함수도 없다. 이를 추가한 전체 코드는 다음과 같다.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
    string name;
public:
    Person() { name = ""; }
    Person(string name) { this->name = name; }
    void setName(string name) { this->name = name; }     // 추가한 멤버 함수
    string getName() { return name; }                     // 추가한 멤버 함수
};

class Family {
    string name;                        // 추가한 멤버 변수: 가족 이름
    Person* p;                          // Person 배열 포인터
    int size;                           // 가족 구성원 수
public:
    Family(string name, int size);
    void setName(int index, string name) { p[index].setName(name); }
    void show();
    ~Family();
};

Family::Family(string name, int size) {
    this->name = name;
    this->size = size;
    p = new Person[size];               // size개의 Person을 동적 생성
}

void Family::show() {
    cout << name << " 가족은 다음과 같이 " << size << "명입니다." << endl;
    for (int i = 0; i < size; i++)
        cout << p[i].getName() << '\t';
    cout << endl;
}

Family::~Family() {
    delete[] p;                         // 동적 배열 반환
}

int main() {
    Family* simpson = new Family("Simpson", 3);
    simpson->setName(0, "Mr. Simpson");
    simpson->setName(1, "Mrs. Simpson");
    simpson->setName(2, "Bart Simpson");
    simpson->show();
    delete simpson;
}
```

```text
Simpson 가족은 다음과 같이 3명입니다.
Mr. Simpson	Mrs. Simpson	Bart Simpson	
```

- **동적 객체 배열과 기본 생성자:** `new Person[size]`는 원소마다 기본 생성자 `Person()`을 호출하므로 `Person`에 기본 생성자가 있어야 한다. 이름은 나중에 `setName()`으로 정한다.
- **소멸 순서:** `delete simpson;`은 먼저 `Family`의 소멸자를 호출하고, 소멸자 안의 `delete[] p;`가 `Person` 세 개의 소멸자를 호출한 뒤 배열 메모리를 반환한다. 그다음 `Family` 객체 자신의 메모리가 반환된다. 소멸자가 `delete[] p;`를 하지 않으면 `Family` 객체가 사라져도 `Person` 배열은 남아 메모리 누수가 된다.
- **위임:** `Family::setName()`은 이름을 직접 바꾸지 않고 해당 `Person`의 `setName()`에 맡긴다. `Person`의 `name`은 `private`이므로 `Family`도 `Person`의 `public` 함수를 거쳐야 한다.

> **주의:** 이 `Family`는 포인터 멤버 `p`를 가지므로, `Family a = *simpson;`처럼 객체를 복사하면 두 객체가 같은 `Person` 배열을 가리키고 두 소멸자가 같은 배열을 두 번 반환한다. 포인터 멤버를 가진 클래스의 복사는 5장의 깊은 복사 생성자로 다룬다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 객체 배열의 초기화 | 중괄호 안에 원소마다 생성자를 지정할 수 있고, 지정하지 않은 원소는 기본 생성자로 만들어진다. |
| 객체 포인터 연산 | `p++`는 `sizeof(클래스)`만큼 이동하여 다음 원소를 가리키며, `p[i]`는 `*(p + i)`와 같다. |
| 가변 길이 배열 | 실행 중에 크기가 정해지는 `int data[size];`는 표준 C++이 허용하지 않으며, `new int[size]`나 `vector`로 대신한다. |
| `new[]`와 `delete[]` | 힙에 할당한 배열은 `delete[]`로 반환하고, 반환하지 않으면 메모리 누수가 생긴다. |
| `this` 포인터 | 멤버 함수를 호출한 객체의 주소를 담은 숨은 매개변수로, 매개변수와 멤버의 이름이 같을 때 `this->멤버`로 구별한다. |
| `string`의 바이트 단위 | `length()`와 `substr()`은 바이트 단위로 동작하므로 UTF-8 한글처럼 여러 바이트로 된 문자를 자르면 깨진다. |
| `string` 멤버 함수 | `compare`, `substr`, `append`, `insert`, `erase`, `find`, `replace`로 문자열을 비교하고 수정하며, `stoi`로 정수로 바꾼다. |
| 동적 배열을 가진 클래스 | 생성자에서 `new[]`로 배열을 만들고 소멸자에서 `delete[]`로 반환하며, 배열 원소의 클래스에는 기본 생성자가 필요하다. |

---

<br>

## 점검 문제

1. 2.1절의 `circleArray`를 `Circle circleArray[3];`으로 선언하고 `Circle` 클래스에서 기본 생성자 `Circle()`을 지우면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   컴파일 오류가 난다. 초기화 목록 없이 객체 배열을 선언하면 원소마다 기본 생성자가 호출되는데, 매개변수 1개 생성자 `Circle(int r)`이 있으므로 컴파일러가 기본 생성자를 자동으로 넣지 않고, 호출할 기본 생성자가 없다. `{ Circle(10), Circle(20), Circle(30) }`처럼 모든 원소의 생성자를 지정하면 기본 생성자 없이도 컴파일된다.

   </details>

2. 3.2절의 프로그램에서 `delete[] p;`를 `delete p;`로 바꾸면 무엇이 문제인가?

   <details>
   <summary>정답</summary>

   `new[]`로 할당한 메모리를 `delete`로 반환하는 것은 정의되지 않은 동작이다. 컴파일은 되고 `int` 배열에서는 겉으로 문제가 드러나지 않을 수도 있지만, 표준은 결과를 보장하지 않는다. 원소가 소멸자를 가진 객체라면 첫 원소의 소멸자만 호출되거나 프로그램이 비정상 종료될 수 있다. 할당과 반환은 `new`와 `delete`, `new[]`와 `delete[]`의 짝을 맞춘다.

   </details>

3. 4.1절의 `Circle`에서 `setRadius()`만 `void setRadius(int r) { radius = r; }`로 고치고 생성자는 그대로 두면 출력은 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `c1.setRadius(4)`와 `c2.setRadius(5)`가 멤버를 올바르게 바꾸므로 4와 5가 출력된다. 생성자 `Circle(int radius) { radius = radius; }`는 여전히 멤버를 초기화하지 않지만, `c2`의 `radius`는 읽히기 전에 `setRadius(5)`로 값이 정해지므로 이 실행에서는 문제가 드러나지 않는다. 다만 `c2`를 만든 직후 `getRadius()`를 호출하면 초기화되지 않은 값을 읽게 되므로 생성자도 고쳐야 한다.

   </details>

4. `string s = "Hello World";`에서 `s.find("o")`, `s.substr(6, 5)`, `s.erase(5, 6)` 뒤의 `s`는 각각 무엇인가?

   <details>
   <summary>정답</summary>

   `s.find("o")`는 처음 나타나는 `'o'`의 위치인 4를 리턴한다(H, e, l, l, o의 다섯째). `s.substr(6, 5)`는 6번 `'W'`부터 다섯 글자인 `"World"`를 리턴하며 `s`는 바뀌지 않는다. `s.erase(5, 6)`은 5번의 공백부터 여섯 글자인 `" World"`를 지우므로 `s`는 `"Hello"`가 된다.

   </details>

5. 6절의 `Family` 소멸자에서 `delete[] p;`를 빼면 `delete simpson;`을 실행할 때 어떤 일이 생기는가?

   <details>
   <summary>정답</summary>

   `Family` 객체의 메모리는 반환되지만, 생성자에서 `new Person[size]`로 할당한 `Person` 배열은 반환되지 않는다. 그 배열을 가리키던 포인터 `p`는 `Family` 객체와 함께 사라지므로 다시는 그 메모리에 접근하거나 반환할 수 없는 메모리 누수가 된다. `Person`의 소멸자도 호출되지 않아 각 `Person`의 `string`이 가진 메모리도 함께 남는다.

   </details>
