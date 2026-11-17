# 6장 실습 — 함수 중복과 디폴트 매개변수

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lab.md) | [한국어](Concepts_Lab.ko.md)

> **최종 수정일:** 2026-11-18

> **학습 목표:**
> 1. 함수 중복이 성립하는 조건을 설명하고 리턴 타입만 다른 함수가 중복되지 않는 이유를 설명할 수 있다
> 2. 같은 동작을 함수 중복과 디폴트 매개변수의 두 방법으로 구현하고 비교할 수 있다
> 3. 생성자 중복으로 여러 형태의 초깃값을 받는 클래스를 작성할 수 있다

---

## 목차

- [1. 개요](#1-개요)
- [2. 함수 중복과 디폴트 매개변수](#2-함수-중복과-디폴트-매개변수)
  - [2.1 함수 중복의 성립 조건](#21-함수-중복의-성립-조건)
  - [2.2 기준값을 넘지 않는 큰 값](#22-기준값을-넘지-않는-큰-값)
- [3. 생성자 중복](#3-생성자-중복)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 개요

> **참고:** 함수 중복과 중복 해석 규칙, 디폴트 매개변수, `static` 멤버의 이론과 예제(공금을 함께 쓰는 `Person`, `static` 함수만 가진 `Math`, 살아 있는 원의 개수를 세는 `Circle`)는 [6장 강의 노트](Concepts_Lecture.ko.md)에서 다룬다. 이 실습은 같은 문제를 함수 중복과 디폴트 매개변수로 각각 풀어 두 방법을 비교한다.

| 주제 | 위치 |
|:-----|:-----|
| 함수 중복의 성립 조건, 함수 중복과 디폴트 매개변수로 구현한 `big()` | 2절 |
| 생성자 중복으로 완성하는 `Person` 클래스 | 3절 |
| 디폴트 매개변수를 갖는 생성자 하나로 완성하는 `Person` 클래스 | [HW6 문제 1](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.ko.md#문제-1-11주차-강의자료-실습-2-2) |
| `static` 멤버로 회원 수를 세는 `Person`, `static` 함수로 이루어진 `Random`, 현재 객체 수를 세는 `Circle` | [HW6 문제 2\~4](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.ko.md) |
| `static` 멤버로 게시물을 관리하는 `Board`, 디폴트 매개변수로 구구단을 출력하는 `print99()` | [6장 연습 문제 1, 2](Exercises.ko.md) |

*표 1. 실습의 주제와 위치*

모든 코드는 C++17 표준으로 컴파일하여 실행한다(예: `g++ -std=c++17 -Wall`).

---

<br>

## 2. 함수 중복과 디폴트 매개변수

### 2.1 함수 중복의 성립 조건

> **용어 정의:** **함수 중복(function overloading)** 은 이름이 같고 매개변수의 타입이나 개수가 다른 함수들을 같은 범위에 함께 정의하는 것이다.

함수 중복은 보통 함수들 사이, 한 클래스의 멤버 함수들 사이, 상속 관계에 있는 기본 클래스와 파생 클래스의 멤버 함수들 사이에서 가능하며, C 언어에서는 허용되지 않는다. 컴파일러는 호출에 쓰인 인자의 타입과 개수로 어느 함수를 부를지 정하므로, 리턴 타입만 다른 두 함수는 중복될 수 없다.

```cpp
int sum(int a, int b) { return a + b; }
double sum(int a, int b) { return (double)(a + b); }   // 리턴 타입만 다름
int main() { return sum(2, 5); }
```

```text
error: ambiguating new declaration of 'double sum(int, int)'
error: functions that differ only in their return type cannot be overloaded
```

첫 줄은 GCC, 둘째 줄은 Clang의 오류이다. `sum(2, 5)`만 보고는 어느 `sum()`을 부를지 정할 수 없으므로, 두 번째 선언 자체가 오류가 된다.

### 2.2 기준값을 넘지 않는 큰 값

**문제:** 두 정수 중 큰 값을 리턴하되, 큰 값이 기준값보다 크면 기준값을 리턴하는 `big()`을 작성한다. 기준값을 주지 않으면 100을 쓴다. 다음 `main()`이 실행되도록 (1) 함수 중복으로 `big()` 두 개를, (2) 디폴트 매개변수로 `big()` 하나를 작성한다.

```cpp
int main() {
    int x = big(3, 5);          // 큰 값 5가 기준값 100보다 작으므로 5
    int y = big(300, 60);       // 큰 값 300이 기준값 100보다 크므로 100
    int z = big(30, 60, 50);    // 큰 값 60이 기준값 50보다 크므로 50
    cout << x << ' ' << y << ' ' << z << endl;
    return 0;
}
```

**(1) 함수 중복**

```cpp
#include <iostream>
using namespace std;

int big(int a, int b, int max) {        // 큰 값이 max보다 크면 max 리턴
    int m = (a > b) ? a : b;
    return (m > max) ? max : m;
}

int big(int a, int b) {                 // 기준값을 100으로 두는 중복 함수
    return big(a, b, 100);
}

int main() {
    int x = big(3, 5);
    int y = big(300, 60);
    int z = big(30, 60, 50);
    cout << x << ' ' << y << ' ' << z << endl;
    return 0;
}
```

```text
5 100 50
```

**(2) 디폴트 매개변수**

```cpp
#include <iostream>
using namespace std;

int big(int a, int b, int max = 100) {  // max를 생략하면 100
    int m = (a > b) ? a : b;
    return (m > max) ? max : m;
}

int main() {
    int x = big(3, 5);
    int y = big(300, 60);
    int z = big(30, 60, 50);
    cout << x << ' ' << y << ' ' << z << endl;
    return 0;
}
```

```text
5 100 50
```

- **함수 중복:** 인자가 2개인 호출은 `big(int, int)`에, 3개인 호출은 `big(int, int, int)`에 연결된다. 인자 2개 버전이 인자 3개 버전을 호출하게 하면 비교 논리를 한 곳에만 둘 수 있다.
- **디폴트 매개변수:** `max = 100`은 인자가 넘어오지 않을 때 `max`에 100을 넣으라는 뜻이다. 컴파일러는 `big(3, 5)`를 `big(3, 5, 100)`으로 바꾸어 호출한다. 함수 정의가 하나뿐이므로 코드가 짧다.
- **함께 쓸 때의 모호성:** 두 방법을 한 프로그램에 함께 두면, 곧 `int big(int, int)`와 `int big(int, int, int = 100)`이 공존하면 `big(3, 5)`가 두 함수 모두에 맞아 모호한 호출 오류가 된다(6장 강의 노트 2.3절).
- **디폴트 매개변수의 위치:** 디폴트 값은 오른쪽 끝의 매개변수부터 차례로만 줄 수 있다. `int big(int a = 0, int b, int max)`처럼 중간을 건너뛸 수 없다.

---

<br>

## 3. 생성자 중복

**문제:** `Person` 클래스의 정의와 `main()`은 다음과 같다. 세 생성자를 중복으로 작성하여, 실행 예시처럼 인자를 받지 않은 값은 기본값(번호 1, 몸무게 20.5, 이름 `"Grace"`)을 갖게 한다.

```cpp
class Person {
    int id;
    double weight;
    string name;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
```

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

Person::Person() {                      // 인자 없음: 모든 값을 기본값으로
    id = 1;
    weight = 20.5;
    name = "Grace";
}

Person::Person(int id, string name) {   // 몸무게만 기본값으로
    this->id = id;
    this->name = name;
    weight = 20.5;
}

Person::Person(int id, string name, double weight) {
    this->id = id;
    this->name = name;
    this->weight = weight;
}

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
```

```text
1 20.5 Grace
2 20.5 Ashley
3 32.5 Helen
```

- `grace`는 인자가 없으므로 `Person()`, `ashley`는 인자 2개로 `Person(int, string)`, `helen`은 인자 3개로 `Person(int, string, double)`이 호출된다. `ashley(2, "Ashley")`의 `"Ashley"`는 문자열 리터럴이지만 `string`으로 변환되어 두 번째 생성자에 맞는다.
- 세 생성자에 기본값 20.5가 흩어져 있어, 기본값을 바꾸려면 여러 곳을 고쳐야 한다. 같은 동작을 디폴트 매개변수를 갖는 생성자 하나로 줄이는 문제가 [HW6 문제 1](../../Coursework/HW06_Default-Parameters-and-Static-Members/Original/Assignment.ko.md#문제-1-11주차-강의자료-실습-2-2)이다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 함수 중복의 조건 | 이름이 같고 매개변수의 타입이나 개수가 달라야 하며, 리턴 타입만 다른 함수는 중복될 수 없다. |
| 함수 중복과 디폴트 매개변수 | 인자 개수만 다른 중복 함수들은 디폴트 매개변수를 가진 함수 하나로 줄일 수 있지만, 두 방법을 함께 두면 호출이 모호해질 수 있다. |
| 디폴트 매개변수의 위치 | 디폴트 값은 오른쪽 끝의 매개변수부터 차례로 지정하며, 생략한 인자는 컴파일러가 디폴트 값으로 채운다. |
| 생성자 중복 | 매개변수가 다른 여러 생성자로 객체를 여러 형태로 초기화하며, 호출할 생성자는 인자의 개수와 타입으로 정해진다. |

---

<br>

## 점검 문제

1. 2.2절의 두 방법 중 디폴트 매개변수 버전에 `int big(int a, int b) { return (a > b) ? a : b; }`를 추가하면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `int big(int, int)`와 `int big(int, int, int = 100)`이 공존하므로 함수 정의 자체는 허용되지만, `big(3, 5)`처럼 인자 2개로 호출하면 두 함수가 모두 맞아 컴파일러가 하나를 고를 수 없으므로 모호한 호출 오류가 난다. 인자 3개인 `big(30, 60, 50)`은 두 번째 함수에만 맞으므로 문제가 없다.

   </details>

2. `void show(int a = 1, int b);`와 `void show(int a, int b = 1);` 중 컴파일되는 선언은 무엇인가? 그 이유는?

   <details>
   <summary>정답</summary>

   `void show(int a, int b = 1);`만 컴파일된다. 디폴트 값은 오른쪽 끝의 매개변수부터 차례로 지정해야 하는데, 첫 번째 선언은 왼쪽의 `a`에만 디폴트 값이 있고 그 오른쪽의 `b`에는 없다. 생략한 인자는 오른쪽부터 채워지므로 `show(5)`에서 5가 `a`에 들어가고 `b`가 비게 되어 의미가 성립하지 않는다.

   </details>

3. 3절의 `main()`에 `Person tom(7);`을 추가하면 어떻게 되는가? 컴파일되게 하려면 어떤 생성자를 더하면 되는가?

   <details>
   <summary>정답</summary>

   `int` 하나를 받는 생성자가 없으므로 맞는 생성자를 찾지 못해 컴파일 오류가 난다. `int` 하나를 받는 생성자 `Person(int id)`를 추가하여 번호만 인자로 정하고 몸무게와 이름은 기본값으로 정하면 `Person tom(7);`이 컴파일된다.

   </details>
