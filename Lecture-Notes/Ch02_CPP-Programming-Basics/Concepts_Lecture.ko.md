# 2장 — C++ 프로그래밍의 기본

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-11-19
>
> 명품 C++ Programming, 황기태, Ch 2

> **학습 목표:**
> 1. C++ 프로그램의 기본 구조와 `main()` 함수, 주석, `#include` 지시문의 역할을 설명할 수 있다
> 2. `cout`과 `<<` 연산자로 여러 타입의 데이터를 화면에 출력할 수 있다
> 3. `cin`과 `>>` 연산자, `getline`으로 키보드 입력을 받을 수 있다
> 4. 네임스페이스의 개념을 설명하고 `std::` 접두사와 `using` 지시문을 사용할 수 있다
> 5. C 스트링과 `string` 클래스로 문자열을 입력받고 다룰 수 있다
> 6. 전처리기의 헤더 파일 확장과 헤더 파일, 라이브러리의 역할을 구분하여 설명할 수 있다

---

## 목차

- [1. 첫 번째 C++ 프로그램](#1-첫-번째-c-프로그램)
  - [1.1 프로그램 구조](#11-프로그램-구조)
  - [1.2 주석](#12-주석)
  - [1.3 `main` 함수](#13-main-함수)
  - [1.4 `#include <iostream>`](#14-include-iostream)
- [2. 콘솔 출력](#2-콘솔-출력)
  - [2.1 `cout`과 `<<`](#21-cout과-)
  - [2.2 `endl` vs `'\n'`](#22-endl-vs-n)
  - [2.3 `<<` 연산자 세부사항](#23--연산자-세부사항)
  - [2.4 bool 타입](#24-bool-타입)
  - [2.5 `<<`가 이어지는 원리와 출력 타입의 결정](#25-가-이어지는-원리와-출력-타입의-결정)
- [3. 콘솔 입력](#3-콘솔-입력)
  - [3.1 `cin`과 `>>`](#31-cin과-)
  - [3.2 `getline`](#32-getline)
  - [3.3 `cin` vs `scanf`](#33-cin-vs-scanf)
  - [3.4 실행문 중간의 변수 선언](#34-실행문-중간의-변수-선언)
  - [3.5 입력이 끝날 때까지 읽기](#35-입력이-끝날-때까지-읽기)
  - [3.6 문자 하나를 입력받는 두 방법](#36-문자-하나를-입력받는-두-방법)
- [4. 네임스페이스](#4-네임스페이스)
  - [4.1 개념](#41-개념)
  - [4.2 `using namespace std`](#42-using-namespace-std)
  - [4.3 using 지시어가 일으키는 이름 충돌](#43-using-지시어가-일으키는-이름-충돌)
  - [4.4 이름 공간의 중첩과 별칭](#44-이름-공간의-중첩과-별칭)
- [5. C++ 문자열](#5-c-문자열)
  - [5.1 C 스트링](#51-c-스트링)
  - [5.2 `string` 클래스](#52-string-클래스)
  - [5.3 C 스트링과 string 객체의 복사와 비교](#53-c-스트링과-string-객체의-복사와-비교)
  - [5.4 C 스트링을 다룰 때의 흔한 실수](#54-c-스트링을-다룰-때의-흔한-실수)
  - [5.5 문자열을 입력받는 네 가지 방법의 비교](#55-문자열을-입력받는-네-가지-방법의-비교)
- [6. `auto`를 사용한 현대적 변수 선언](#6-auto를-사용한-현대적-변수-선언)
- [7. 헤더 파일과 전처리기](#7-헤더-파일과-전처리기)
  - [7.1 전처리기의 헤더 확장](#71-전처리기의-헤더-확장)
  - [7.2 `<iostream>`과 `<iostream.h>`](#72-iostream과-iostreamh)
  - [7.3 `#include <헤더파일>`과 `#include "헤더파일"`](#73-include-헤더파일과-include-헤더파일)
  - [7.4 헤더 파일과 라이브러리](#74-헤더-파일과-라이브러리)
- [요약](#요약)
- [점검 문제](#점검-문제)
- [참고 문헌](#참고-문헌)

---

<br>

## 1. 첫 번째 C++ 프로그램

### 1.1 프로그램 구조

모든 C++ 프로그램은 기본적인 구조를 따른다. 가장 간단한 완전한 프로그램을 한 줄씩 주석으로 설명하면 다음과 같다.

```cpp
// hello.cpp
#include <iostream>       // C++ 입출력 스트림 라이브러리
using namespace std;      // std 네임스페이스 사용

int main()                // 진입점 함수
{
    cout << "Hello" << endl;   // 콘솔에 출력
    return 0;             // OS에 종료 코드 반환
}
```

```mermaid
graph TD
    P["C++ 프로그램 구조"] --> D["전처리기 지시문<br>#include #lt;iostream#gt;"]
    P --> N["네임스페이스 선언<br>using namespace std;"]
    P --> M["main 함수<br>int main() { ... }"]
    M --> S["실행문<br>cout #lt;#lt; #quot;Hello#quot; #lt;#lt; endl;"]
    M --> R["리턴문<br>return 0;"]
    P --> O["기타 함수<br>(사용자가 정의한 함수)"]
```

*그림 1. C++ 프로그램의 구성 요소*

| 구성 요소 | 역할 |
|:---------|:-----|
| `#include <iostream>` | 입출력 스트림 라이브러리를 포함하여 `cout`과 `cin`을 사용할 수 있게 함 |
| `using namespace std;` | `std::` 접두사 없이 `std` 이름을 직접 사용할 수 있게 함 |
| `int main()` | 진입점 정의; OS가 가장 먼저 호출하는 함수 |
| `{ ... }` | 중괄호로 함수 본문을 구분 |
| `cout << "Hello" << endl;` | `"Hello"`를 출력한 뒤 줄바꿈을 콘솔에 출력 |
| `return 0;` | 정상 종료를 의미하는 종료 코드 0을 OS에 리턴 |

*표 1. C++ 프로그램 구성 요소의 역할*

> **핵심:** 함수 내부의 모든 문장은 세미콜론(`;`)으로 끝나야 한다. 전처리기 지시문(`#include`)에는 세미콜론을 붙이지 **않는다**.

### 1.2 주석

주석은 개발자가 프로그램에 대한 설명이나 특이 사항을 자유롭게 적은 메모이다. 컴파일러가 무시하므로 프로그램의 실행에 영향을 주지 않으며, 사람이 읽기 위해 존재한다. C++은 C와 동일한 주석 형식을 사용한다.

| 종류 | 문법 | 범위 |
|:----|:----|:----|
| 블록 주석 | `/* ... */` | 여러 줄에 걸칠 수 있음 |
| 줄 주석 | `// ...` | `//`부터 줄 끝까지 |

*표 2. C++ 주석의 종류*

```cpp
/* 이것은 블록 주석이다.
   여러 줄에 걸칠 수 있다. */

// 이것은 줄 주석이다.

int x = 10;  // 코드 뒤에 붙이는 인라인 주석
```

> **주의:** 블록 주석은 **중첩할 수 없다**. `/* 외부 /* 내부 */ 아직 안쪽? */`는 컴파일 오류를 일으킨다. `*/`가 첫 번째 매칭에서 주석을 닫기 때문이다.

### 1.3 `main` 함수

`main` 함수는 모든 C++ 프로그램의 **진입점(entry point)** 이다. OS가 컴파일된 프로그램을 실행하면 `main`부터 실행이 시작되고, `main`이 종료하면 프로그램이 종료된다.

| 형태 | 표준 여부 | 비고 |
|:----|:----:|:----|
| `int main()` | 예 | 표준 형태; `int`를 리턴 |
| `int main(void)` | 예 | 명시적 빈 매개변수 목록 |
| `void main()` | **아니오** | 비표준; 일부 컴파일러가 허용하나 이식 불가 |

*표 3. `main` 함수의 형태와 표준 여부*

```cpp
int main()        // 표준 형태
{
    return 0;     // 0 = 성공
}
```

`main`의 리턴값은 OS에 전달되는 종료 코드이다. 0은 정상 종료를 뜻하며, 오류를 알리기 위해 0이 아닌 값을 리턴할 수도 있다. C++에서 `main` 끝에 `return 0;`을 생략하면 컴파일러가 자동으로 `0`을 리턴한다. 이 규칙은 `main`에만 적용되며, 명확성을 위해 명시적으로 작성하는 것이 좋다.

> **주의:** `void main()`은 표준 C++이 **아니다**. 일부 컴파일러(예: MSVC)가 확장 기능으로 허용하지만, 이식성을 위해 피해야 한다. 항상 `int main()`을 사용한다.

### 1.4 `#include <iostream>`

`#include <iostream>`은 컴파일 전에 `<iostream>` 헤더 파일의 내용을 소스에 펼쳐 넣으라고 **전처리기(preprocessor)** 에 내리는 지시이다. `<iostream>` 헤더 파일에는 표준 입출력을 위한 클래스(`ios`, `istream`, `ostream`, `iostream`)와 객체(`cout`, `cin`), `<<`와 `>>` 연산자 함수가 선언되어 있다. 구 버전의 C++에서 쓰던 `<iostream.h>`와의 차이와 전처리 과정은 7절에서 다룬다.

---

<br>

## 2. 콘솔 출력

### 2.1 `cout`과 `<<`

`cout`은 표준 출력(화면)에 연결된 `ostream` 타입의 미리 정의된 객체이다. `<<` 연산자는 데이터를 출력 스트림에 삽입하는 **스트림 삽입 연산자**이다.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "Hello, C++!" << endl;
    return 0;
}
```

| 구성 요소 | 설명 |
|:---------|:----|
| `cout` | 화면에 연결된 `ostream` 객체 (`<iostream>`에 선언됨) |
| `<<` | 스트림 삽입 연산자; 데이터를 출력 스트림에 전달 |
| `endl` | 줄바꿈 문자를 삽입하고 출력 버퍼를 **비움(flush)** |

*표 4. 화면 출력의 구성 요소*

`<<`는 원래 C++의 비트 시프트 연산자이며, `ostream` 클래스에서 스트림 삽입 연산자로 재정의(연산자 중복)되었다. 오른쪽 피연산자를 왼쪽의 스트림 객체에 삽입하므로, `cout`에 삽입한 데이터는 `cout`에 연결된 화면에 출력된다. `cout`은 `std` 네임스페이스에 선언되어 있으므로 `using` 지시문이 없으면 `std::cout`으로 써야 한다(4절).

**체이닝:** 하나의 문장에서 여러 `<<` 연산자를 연결할 수 있다.

```cpp
int age = 20;
double gpa = 3.8;

cout << "나이: " << age << ", 학점: " << gpa << endl;
// 출력: 나이: 20, 학점: 3.8
```

C의 `printf`와 달리, `cout`은 출력할 데이터의 **타입을 자동으로 인식**한다. `%d`나 `%f` 같은 형식 지정자가 필요 없다.

```cpp
// C 스타일: 형식을 지정해야 함
printf("나이: %d, 학점: %.1f\n", age, gpa);

// C++ 스타일: 타입이 자동 인식됨
cout << "나이: " << age << ", 학점: " << gpa << endl;
```

> **참고:** **타입 안전성.** `cout`은 타입 안전하다. 잘못된 타입을 전달하면 컴파일러가 안전하게 변환하거나 컴파일 오류를 발생시킨다. `printf`의 경우 형식 지정자가 불일치하면 정의되지 않은 동작이 발생한다.

#### 현대적 출력 대안: `std::format`과 `std::print`

> **참고:** **C++20.** 이 기능은 C++20 이상이 필요하다.

C++20에서는 Python 스타일의 문자열 포맷팅을 제공하는 `std::format`이 도입되었다. `cout`의 타입 안전성과 서식 문자열의 가독성을 결합한다.

```cpp
#include <format>
#include <iostream>
using namespace std;

int age = 20;
double gpa = 3.8;

// C++20: std::format은 서식이 적용된 문자열을 반환
cout << format("나이: {}, 학점: {:.1f}", age, gpa) << endl;
// 출력: 나이: 20, 학점: 3.8
```

> **참고:** **C++23.** 이 기능은 C++23 이상이 필요하다.

C++23에서는 `cout` 없이 직접 출력하는 `std::print`가 추가되었다.

```cpp
#include <print>

int age = 20;
double gpa = 3.8;

// C++23: std::print, 서식 구문으로 직접 출력
std::print("나이: {}, 학점: {:.1f}\n", age, gpa);
// 출력: 나이: 20, 학점: 3.8
```

| 방식 | 표준 | 헤더 | 타입 안전 | 서식 제어 |
|:----|:----|:----|:--------:|:--------:|
| `printf` | C | `<cstdio>` | 아니오 | `%d`, `%f` |
| `cout <<` | C++98 | `<iostream>` | 예 | 조작자(manipulator) |
| `std::format` | C++20 | `<format>` | 예 | `{}` 자리 표시자 |
| `std::print` | C++23 | `<print>` | 예 | `{}` 자리 표시자 |

*표 5. 출력 방식의 비교*

### 2.2 `endl` vs `'\n'`

`endl`과 `'\n'` 모두 줄바꿈을 생성하지만, 중요한 차이가 하나 있다.

| 방법 | 줄바꿈 여부 | 버퍼 비움 여부 |
|:----|:------:|:--------:|
| `endl` | 예 | 예 |
| `'\n'` | 예 | 아니오 |

*표 6. `endl`과 `'\n'`의 차이*

```cpp
cout << "Line 1" << endl;   // 줄바꿈 + 버퍼 비움
cout << "Line 2" << '\n';   // 줄바꿈만 (버퍼 비우지 않음)
cout << "Line 3\n";         // 위와 동일 (문자열 리터럴 안의 줄바꿈)
```

**버퍼 비움(flush)** 이란 출력 버퍼에 있는 데이터를 즉시 화면에 쓰는 것이다. 비우지 않으면 데이터가 버퍼가 가득 차거나 프로그램이 종료될 때까지 버퍼에 남아 있을 수 있다. `endl`처럼 스트림에 작용하여 동작을 바꾸는 것을 **조작자(manipulator)** 라 한다.

> **참고:** **출력 성능.** 성능이 중요한 코드(예: 수백만 줄 출력)에서는 `endl`보다 `'\n'`을 선호한다. `endl`이 반복적으로 버퍼를 비우면 출력 속도가 크게 느려질 수 있다.

### 2.3 `<<` 연산자 세부사항

`<<` 연산자는 `bool`, `char`, `short`, `int`, `long`, `float`, `double` 같은 모든 기본 타입과 문자열을 출력할 수 있다.

| 타입 | 예시 |
|:----|:----|
| `bool` | `cout << true;` → `1` (`boolalpha` 사용 시 `true`) |
| `char` | `cout << 'A';` → `A` |
| `int` | `cout << 42;` → `42` |
| `double` | `cout << 3.14;` → `3.14` |
| 문자열 (`const char*`) | `cout << "Hello";` → `Hello` |

*표 7. 타입별 `<<` 출력 예시*

**식과 함수 호출**을 `<<`와 함께 직접 사용할 수 있다.

```cpp
int a = 5, b = 3;
cout << a + b << endl;         // 출력: 8
cout << (a > b) << endl;       // 출력: 1 (true)
```

다음 프로그램은 여러 타입의 값, 식, 함수의 리턴값을 한 번에 출력한다.

```cpp
#include <iostream>

double area(int r) {           // 반지름이 r인 원의 면적을 반환
    return 3.14 * r * r;
}

int main() {
    int n = 3;
    char c = '#';
    std::cout << c << 5.5 << '-' << n << "hello" << true << std::endl;
    std::cout << "n + 5 = " << n + 5 << '\n';
    std::cout << "면적은 " << area(n);   // 함수 area()의 반환값 출력
}
```

```text
#5.5-3hello1
n + 5 = 8
면적은 28.26
```

`true`는 `1`로 출력되고, 값 사이에 공백이 자동으로 들어가지 않는다는 점에 주의한다.

**연산자 우선순위 문제:** `<<` 연산자는 비교 연산자(`>`, `<`, `>=`, `<=`)보다 **우선순위가 높다**.

```cpp
cout << 5 > 3;            // 오류: (cout << 5) > 3 으로 해석됨
cout << (5 > 3) << endl;  // 정상, 출력: 1
```

`cout << 5 > 3`은 `(cout << 5) > 3`으로 해석되어, `ostream` 객체를 `3`과 비교하려 하므로 타입 오류가 발생한다.

> **주의:** 비교 연산처럼 `<<`보다 우선순위가 낮은 연산의 결과를 출력할 때는 반드시 **괄호**로 감싸서 우선순위 문제를 방지한다: `cout << (5 > 3)`. 산술 연산(`+`, `-`, `*`, `/`)은 `<<`보다 우선순위가 높아 괄호 없이도 바르게 출력되지만, 괄호로 감싸면 의도가 분명해진다.

### 2.4 bool 타입

표 7의 첫 행에 나온 `bool`은 참과 거짓 두 값만 가지는 C++의 기본 타입이다. 값은 리터럴 `true`와 `false`로 쓰며, 비교 연산과 논리 연산의 결과가 `bool`이다.

> **용어 정의:** **bool 타입(bool type)** 은 `true`와 `false` 두 값만 가지는 C++의 기본 타입으로, 조건식의 결과를 나타낸다. 정수와 섞어 쓰면 `true`는 1, `false`는 0으로 바뀌고, 정수를 `bool`로 바꾸면 0은 `false`, 0이 아닌 값은 `true`가 된다.

```cpp
#include <iostream>
using namespace std;

int main() {
    bool isAdult = true;
    bool isEmpty = false;
    bool fromInt = 7;                 // 0이 아닌 정수는 true로 바뀐다
    int fromBool = true + true;       // true는 1, false는 0으로 바뀐다
    cout << isAdult << ' ' << isEmpty << ' ' << fromInt << ' ' << fromBool << endl;
    cout << boolalpha << isAdult << ' ' << (3 > 5) << endl;
    cout << "sizeof(bool)=" << sizeof(bool) << endl;
    return 0;
}
```

```text
1 0 1 2
true false
sizeof(bool)=1
```

- `bool fromInt = 7;`은 0이 아닌 정수이므로 `true`가 되고, 출력하면 `1`이다. 7이라는 값은 보존되지 않는다.
- `true + true`는 두 `bool`이 `int` 1로 바뀌어 더해지므로 2이다.
- `cout`은 기본적으로 `bool`을 `1`, `0`으로 출력하며, 조작자 `boolalpha`를 한 번 보내면 그 뒤로는 `true`, `false`로 출력한다. `(3 > 5)`의 괄호는 2.3절의 우선순위 문제 때문에 필요하다.
- `bool`의 크기는 이 환경에서 1바이트이다. 참과 거짓만 저장하므로 1비트면 충분하지만, 메모리는 바이트 단위로 주소를 매기므로 보통 1바이트를 차지한다. 크기는 구현이 정한다.

C 언어에는 처음에 불(Boolean) 타입이 없어 `int`의 0과 0이 아닌 값으로 참과 거짓을 나타냈고, C99에서 `_Bool` 타입과 `<stdbool.h>`의 매크로 `bool`, `true`, `false`가 추가되었다. C++에서는 `bool`, `true`, `false`가 처음부터 예약어이므로 헤더 없이 쓸 수 있다. C++에서도 `if (count)`처럼 정수를 조건으로 쓸 수 있는 것은 정수가 `bool`로 바뀌기 때문이며, 의도를 분명히 하려면 `if (count != 0)`처럼 비교식을 쓴다.

### 2.5 `<<`가 이어지는 원리와 출력 타입의 결정

`cout << a << b`가 왼쪽부터 차례로 출력되는 것은 `<<`가 왼쪽부터 결합하고, `cout << a`라는 식의 결과가 `cout` 자신이기 때문이다. 이 식은 `(cout << a) << b`로 해석되며, 괄호 안이 먼저 `a`를 출력한 뒤 `cout`을 결과로 내놓으면, 그 `cout`에 다시 `<< b`가 적용된다.

```cpp
#include <iostream>
using namespace std;

int main() {
    ostream &r = (cout << "A");      // cout << "A"의 결과는 cout 자신이다
    r << "B" << endl;                 // r은 cout의 다른 이름

    cout << 'A' + 1 << endl;          // char와 int의 덧셈: 결과는 int
    cout << char('A' + 1) << endl;    // char로 바꾸어 출력

    const char *s = "Hi";
    cout << s << ' ' << *s << endl;   // char 포인터는 문자열을, *s는 첫 문자를 출력
    return 0;
}
```

```text
AB
66
B
Hi H
```

- `ostream &r = (cout << "A");`는 `cout << "A"`의 결과를 `ostream` 참조 `r`에 받는다. `r`은 `cout` 자체의 다른 이름이므로 `r << "B"`는 화면의 같은 줄에 `B`를 이어 출력하여 `AB`가 된다. `ostream`은 `cout`의 타입이며, `&`로 선언한 참조는 5장에서 다룬다. `<<`가 자기 자신을 결과로 돌려주도록 만드는 방법은 7장의 연산자 중복에서 다룬다.
- `<<`는 출력할 값의 **타입**을 보고 출력 방법을 고른다. `'A' + 1`은 `char`가 `int`로 바뀌어 더해지므로 결과가 `int` 66이고, 정수로 출력된다. 문자 `B`를 출력하려면 `char('A' + 1)`처럼 결과를 `char`로 바꾼다.
- `const char*` 타입의 `s`는 주소를 담은 포인터이지만, `<<`는 `char` 포인터를 C 스트링의 시작으로 보고 널 문자까지의 문자열 `Hi`를 출력한다. `*s`는 `char` 타입의 첫 문자 `H`이다. 반면 `int*` 같은 다른 포인터를 출력하면 주소 값이 16진수로 출력된다.

printf의 형식 지정자는 프로그래머가 타입을 알려 주는 방식이어서, `%d`에 `double`을 넘기는 것 같은 불일치가 실행 중의 잘못된 출력으로 이어진다. `<<`는 컴파일러가 타입을 보고 출력 방법을 고르므로 이런 불일치가 생기지 않는다. 대신 위의 `'A' + 1`처럼 식의 타입이 생각과 다르면 출력도 생각과 다르므로, 식의 타입을 정확히 아는 것이 중요하다.

---

<br>

## 3. 콘솔 입력

### 3.1 `cin`과 `>>`

`cin`은 표준 입력(키보드)에 연결된 `istream` 타입의 미리 정의된 객체이다. `>>` 연산자는 입력 스트림에서 데이터를 추출하는 **스트림 추출 연산자**이다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int num;
    cout << "숫자를 입력하세요: ";
    cin >> num;
    cout << "입력한 값: " << num << endl;
    return 0;
}
```

| 구성 요소 | 설명 |
|:---------|:----|
| `cin` | 키보드에 연결된 `istream` 객체 (`<iostream>`에 선언됨) |
| `>>` | 스트림 추출 연산자; 입력 스트림에서 데이터를 읽음 |

*표 8. 키보드 입력의 구성 요소*

`>>`도 원래 비트 시프트 연산자이며, `<iostream>`에서 스트림 추출 연산자로 재정의되어 입력 스트림에서 값을 읽어 변수에 저장한다.

**`>>`가 입력을 읽는 방법:**
- 선행 공백(스페이스, 탭, 줄바꿈)을 건너뛴다.
- 다음 공백 구분자까지 문자를 읽는다.
- 읽은 문자를 대상 변수의 타입으로 변환한다.

`cin`은 입력 버퍼를 내장하고 있다. 사용자가 누른 키는 `<Enter>` 키가 입력될 때까지 입력 버퍼에 쌓이며, 그동안 `<Backspace>` 키로 입력한 키를 지울 수 있다. `<Enter>` 키가 입력되면 비로소 `>>` 연산자가 입력 버퍼의 값을 변수에 전달한다.

**체이닝:** 여러 `>>` 연산자를 연결할 수 있다.

```cpp
int a, b;
cout << "두 수를 입력하세요: ";
cin >> a >> b;    // 공백으로 구분된 두 정수를 읽음
cout << "합: " << a + b << endl;
```

```text
입력:  10 20
출력: 합: 30
```

> **핵심:** `cin >> a >> b;`는 공백, 탭, 줄바꿈으로 구분된 두 개의 토큰을 입력에서 읽는다. 첫 번째 토큰은 `a`에, 두 번째는 `b`에 저장된다.

`int` 변수에 숫자가 아닌 글자를 입력하는 경우처럼 입력이 변수의 타입과 맞지 않을 때 `cin`이 어떤 상태가 되는지는 11장의 입출력 스트림에서 다룬다.

### 3.2 `getline`

`>>` 연산자는 다음 공백까지만 읽으므로, 공백이 포함된 문자열을 읽기에는 적합하지 않다. `getline` 함수는 공백을 포함한 **한 줄 전체**를 `string` 객체에 읽는다.

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string name;
    cout << "이름을 입력하세요: ";
    getline(cin, name);
    cout << "안녕하세요, " << name << "!" << endl;
    return 0;
}
```

```text
입력:  홍길동
출력: 안녕하세요, 홍길동!
```

**`>>`와 `getline` 혼용의 함정:**

`>>`가 값을 읽으면 뒤따르는 줄바꿈(`'\n'`)이 입력 버퍼에 남는다. 이후 `getline`을 호출하면 이 남은 줄바꿈을 읽고 빈 문자열을 리턴한다.

**해결 방법:** `getline`을 호출하기 전에 `cin.ignore()`를 사용하여 남은 줄바꿈을 버린다.

```cpp
int age;
string name;

cout << "나이를 입력하세요: ";
cin >> age;                    // 숫자를 읽고, '\n'은 버퍼에 남음

cin.ignore();                  // 남은 '\n'을 버림

cout << "이름을 입력하세요: ";
getline(cin, name);            // 이제 올바르게 읽힘
cout << age << " " << name << endl;
```

> **주의:** `>>`와 `getline` 사이에 `cin.ignore()`를 빠뜨리는 것은 C++ 초보자가 가장 흔히 하는 실수 중 하나이다. `getline` 호출이 남은 줄바꿈을 즉시 읽어 건너뛴 것처럼 보인다.

**문제 발생 과정을 단계별로 시각화:**

```text
사용자 입력: 25[Enter]홍길동[Enter]

1단계: cin >> age;
  버퍼 (이전): "25\n홍길동\n"
  읽은 값: 25
  버퍼 (이후): "\n홍길동\n"    ← '\n'이 남아 있음!

2단계 (cin.ignore() 없이):  getline(cin, name);
  버퍼 (이전): "\n홍길동\n"
  읽은 값: ""                    ← 첫 번째 '\n'까지 읽음 (즉시 만남)
  버퍼 (이후): "홍길동\n"
  결과: name은 빈 문자열!

2단계 (cin.ignore() 사용):  getline(cin, name);
  cin.ignore()가 '\n'을 버림
  버퍼 (이전): "홍길동\n"
  읽은 값: "홍길동"              ← 정상!
  버퍼 (이후): ""
```

*그림 2. `cin.ignore()` 유무에 따른 입력 버퍼의 변화*

**더 안전한 버전:** 최대한 안전하게 하려면, 단일 문자가 아닌 다음 줄바꿈까지의 모든 내용을 버리는 `cin.ignore(numeric_limits<streamsize>::max(), '\n')`를 사용한다.

```cpp
#include <limits>   // numeric_limits에 필요

cin >> age;
cin.ignore(numeric_limits<streamsize>::max(), '\n');   // 나머지 줄 전체 버림
getline(cin, name);
```

### 3.3 `cin` vs `scanf`

| 특성 | `cin` (C++) | `scanf` (C) |
|:----|:-----------|:-----------|
| 타입 안전성 | 타입 자동 인식 | 형식을 지정해야 함 (`%d`, `%f`) |
| 형식 지정자 | 불필요 | 필수 |
| 주소 연산자 `&` | 기본 타입에 불필요 | 필수 (`scanf("%d", &x)`) |
| 공백 포함 문자열 | `getline` 사용 | `fgets` 사용 |
| 오류 처리 | 스트림 상태 확인 (`cin.fail()`) | 리턴값 확인 |

*표 9. `cin`과 `scanf`의 비교*

```cpp
// C 스타일
int x;
scanf("%d", &x);      // %d와 &를 사용해야 함

// C++ 스타일
int x;
cin >> x;              // 형식 지정자 없음, & 없음
```

> **참고:** **C++ 입출력의 권장.** C++에서는 `scanf`/`printf`보다 `cin`/`cout`을 선호한다. 타입 안전하고, 사용자 정의 타입으로 확장 가능하며, C++ 스트림과 자연스럽게 통합된다.

### 3.4 실행문 중간의 변수 선언

C++에서는 변수를 블록의 앞부분뿐 아니라 실행문 중간 어디서나 선언할 수 있다. C도 C99부터 같은 방식을 허용하지만, 그 이전의 C(C89)는 블록의 앞부분에서만 변수를 선언하게 했다.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "너비를 입력하세요>>";
    int width;                       // 실행문 중간에 변수 선언
    cin >> width;
    cout << "높이를 입력하세요>>";
    int height;
    cin >> height;
    int area = width * height;       // 사각형의 면적 계산
    cout << "면적은 " << area << "\n";
}
```

```text
너비를 입력하세요>>3
높이를 입력하세요>>5
면적은 15
```

- **장점:** 변수 선언부와 실행문 사이를 오가는 번거로움이 없고, 변수를 사용하기 직전에 선언하므로 변수 이름의 타이핑 오류를 줄인다.
- **단점:** 선언된 변수를 한눈에 보기 어렵고, 코드 사이에 흩어진 변수를 찾기 어렵다.

타이핑 오류를 줄이는 효과는 다음 두 코드를 비교하면 알 수 있다. 모든 변수를 앞부분에서 선언하면, `time`에 5를 저장하려다 `timer`로 잘못 입력해도 `timer`가 이미 선언되어 있으므로 컴파일 오류 없이 잘못된 실행 결과만 나온다.

```cpp
int time, timer;
// ...
timer = 5;    // time에 5를 저장하려다 timer로 잘못 입력. 컴파일 오류가 발생하지 않음
// ...
timer = 3;
```

변수를 사용하기 직전에 선언하면, 같은 실수를 했을 때 `timer`가 아직 선언되지 않았으므로 컴파일 오류가 발생하여 실수를 미리 발견한다.

```cpp
int time;
timer = 5;    // timer가 아직 선언되지 않았으므로 컴파일 오류 발생
// ...
int timer;
timer = 3;
```

### 3.5 입력이 끝날 때까지 읽기

`cin >> x`라는 식 자체도 값을 가진다. 읽기에 성공하면 그 식을 조건으로 썼을 때 참이 되고, 입력이 끝났거나 `x`의 타입에 맞지 않는 값이 나오면 거짓이 된다. 이 성질을 이용하면 개수를 모르는 입력을 끝까지 읽을 수 있다.

```cpp
#include <iostream>
using namespace std;

int main() {
    int x, sum = 0, count = 0;
    while (cin >> x) {                // 정수를 읽는 데 성공하는 동안 반복
        sum += x;
        count++;
    }
    cout << count << "개, 합 " << sum << endl;
    return 0;
}
```

```text
$ printf '10 20\n30\n' | ./readsum
3개, 합 60
$ printf '10 20 abc 30\n' | ./readsum
2개, 합 30
```

- 첫 번째 실행에서 `>>`는 공백과 줄바꿈을 건너뛰며(3.1절) 10, 20, 30을 차례로 읽고, 입력의 끝(EOF)에서 읽기에 실패하여 반복이 끝난다. 키보드로 입력할 때는 macOS와 Linux에서 Ctrl+D, Windows에서 Ctrl+Z를 눌러 입력의 끝을 알린다.
- 두 번째 실행에서는 `abc`를 정수로 읽을 수 없으므로 그 자리에서 읽기가 실패하고 반복이 끝난다. `abc` 뒤의 30은 읽히지 않는다. `cin`은 실패한 뒤 오류 상태가 되어 그 뒤의 입력도 모두 실패한다.

`printf '...' |`는 앞 명령의 출력을 프로그램의 표준 입력으로 넘기는 셸의 기능으로, 키보드 대신 미리 정한 입력을 줄 때 쓴다. `cin`의 오류 상태를 확인하고 되돌리는 방법은 11장에서 다룬다.

### 3.6 문자 하나를 입력받는 두 방법

`char` 변수에 `>>`로 입력하면 3.1절의 규칙대로 공백 문자를 건너뛰고 공백이 아닌 첫 문자를 읽는다. 공백과 줄바꿈까지 그대로 읽어야 할 때는 멤버 함수 `cin.get()`을 쓴다.

```cpp
#include <iostream>
using namespace std;

int main() {
    char a, b, c;
    cin >> a >> b;                    // 공백을 건너뛰고 문자 하나씩 읽는다
    cin.get(c);                       // 공백을 포함하여 다음 문자 하나를 그대로 읽는다
    cout << '[' << a << "][" << b << "][" << c << ']' << endl;
    return 0;
}
```

```text
$ printf 'x y z\n' | ./getc
[x][y][ ]
```

입력 버퍼에는 `x`, 공백, `y`, 공백, `z`, 줄바꿈이 차례로 들어 있다. `cin >> a`는 `x`를 읽고, `cin >> b`는 공백을 건너뛰고 `y`를 읽는다. 다음의 `cin.get(c)`는 건너뛰지 않으므로 `y` 바로 뒤의 공백을 읽어, 세 번째 괄호 안에 빈칸이 출력된다. `z`와 줄바꿈은 버퍼에 남는다. 3.2절에서 `>>` 뒤에 남은 줄바꿈이 `getline`에 읽힌 것도 같은 원리이며, 공백을 건너뛰는 입력과 건너뛰지 않는 입력을 섞어 쓸 때는 버퍼에 남은 문자를 생각해야 한다.

---

<br>

## 4. 네임스페이스

### 4.1 개념

**네임스페이스(namespace, 이름 공간)** 는 식별자(이름)에 범위를 제공하는 선언 영역이다. 주된 목적은 **이름 충돌을 방지**하는 것이다. 서로 다른 라이브러리가 동일한 이름의 함수나 클래스를 정의할 수 있으며, 네임스페이스가 이들을 분리한다.

이름 충돌은 여러 명이 프로젝트를 나누어 개발할 때나, 오픈 소스 또는 다른 사람이 작성한 소스나 목적 파일을 가져와 컴파일하거나 링크할 때 생기며, 해결하는 데 많은 시간과 노력이 든다. 같은 아파트 단지에 마이클이 여러 명 살면 1동 마이클, 2동 마이클처럼 동을 붙여 구분하듯이, 네임스페이스는 이름 앞에 소속을 붙여 같은 이름을 구분한다.

개발자는 `namespace` 키워드로 자신만의 네임스페이스를 만들 수 있으며, 그 안에 선언된 이름은 다른 네임스페이스의 이름과 별도로 구분된다. 네임스페이스 안의 이름은 `네임스페이스::이름` 형식으로 사용한다.

```cpp
// mike.h
namespace mike {             // mike라는 네임스페이스 생성
    int f();
    int g();
}

// kitae.cpp
#include "mike.h"

namespace kitae {            // kitae라는 네임스페이스 생성
    void f() { /* ... */ }
    void m() {
        f();                 // 같은 네임스페이스의 kitae::f() 호출
        mike::f();           // mike 네임스페이스의 f() 호출
    }
}
```

kitae와 mike가 각자 작성한 `f()` 함수를 네임스페이스 없이 한 프로그램에 합치면 같은 이름이 두 번 정의되어 오류가 발생한다. 위처럼 각자의 네임스페이스에 넣으면 `kitae::f()`와 `mike::f()`로 구분되어 충돌하지 않는다.

C++ 표준 라이브러리는 모든 이름을 `std` 네임스페이스 안에 배치한다.

```cpp
#include <iostream>

int main() {
    std::cout << "Hello" << std::endl;   // 완전한 한정 이름
    return 0;
}
```

| 문법 | 의미 |
|:----|:----|
| `std::cout` | `std` 네임스페이스에 정의된 `cout` |
| `std::endl` | `std` 네임스페이스에 정의된 `endl` |
| `std::string` | `std` 네임스페이스에 정의된 `string` 클래스 |

*표 10. `std::` 접두사를 붙인 이름의 의미*

`std::` 접두사가 없으면 컴파일러가 `cout`의 정의 위치를 알 수 없어 오류를 보고한다.

### 4.2 `using namespace std`

모든 표준 라이브러리 이름 앞에 `std::`를 쓰지 않으려면 **using 지시문**을 사용한다.

```cpp
using namespace std;   // std의 모든 이름을 현재 범위로 가져옴
```

이 선언 이후에는 `cout`, `cin`, `endl`, `string`, `vector` 등을 직접 사용할 수 있다. `<iostream>`에 선언된 이름은 모두 `std` 네임스페이스 안에 있으므로, `<iostream>`을 쓰는 프로그램은 보통 `#include <iostream>` 다음에 `using namespace std;`를 둔다.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "std:: 접두사가 필요 없음" << endl;
    return 0;
}
```

**선택적 using 선언:** 특정 이름만 가져온다.

```cpp
#include <iostream>
using std::cout;
using std::endl;

int main() {
    cout << "cout과 endl만 가져옴" << endl;
    // cin은 여전히 std::cin으로 써야 함
    return 0;
}
```

| 방법 | 장점 | 단점 |
|:----|:----|:----|
| `std::cout` (완전 한정) | 모호함 없음; 출처 명확 | 장황함 |
| `using namespace std;` | 간결; 편리 | 대규모 프로젝트에서 이름 충돌 가능 |
| `using std::cout;` (선택적) | 편의성과 안전성의 균형 | 각 이름을 개별적으로 나열해야 함 |

*표 11. 표준 라이브러리 이름을 쓰는 방법의 비교*

> **참고:** **using 지시문의 사용 범위.** 작은 프로그램과 학습용 예제에서는 `using namespace std;`를 사용해도 무방하다. 대규모 프로젝트와 헤더 파일에서는 이름 충돌을 방지하기 위해 완전 한정 이름이나 선택적 `using` 선언을 선호한다.

### 4.3 using 지시어가 일으키는 이름 충돌

`using namespace std;`는 `std`에 있는 모든 이름을 이름 앞에 `std::`를 붙이지 않고 쓸 수 있게 한다. 이때 `std` 안에는 프로그래머가 알지 못하는 이름도 많으므로, 프로그래머가 만든 이름과 우연히 같은 이름이 있으면 충돌이 생긴다. 다음 프로그램은 전역 변수 `count`를 만들었는데, `<algorithm>`에는 원소의 개수를 세는 함수 `std::count`가 있다.

```cpp
#include <iostream>
#include <algorithm>
using namespace std;

int count = 0;                  // 전역 변수 count

int main() {
    count++;                    // 전역 count인가, std::count인가?
    cout << count << endl;
    return 0;
}
```

```text
clash.cpp:8:5: error: reference to 'count' is ambiguous
clash.cpp:5:5: note: candidate found by name lookup is 'count'
.../__algorithm/count.h:86:1: note: candidate found by name lookup is 'std::count'
```

using 지시어는 `std`의 이름을 현재 범위에 새로 선언하는 것이 아니라 이름을 찾을 때 `std`도 함께 찾게 할 뿐이다. 그래서 `int count = 0;`이라는 선언 자체는 오류가 아니고, `count`를 실제로 사용하는 8번 줄에서 두 후보가 함께 발견되어 모호하다는 오류가 난다 [1]. 해결 방법은 다음과 같다.

- 전역 변수를 `::count`처럼 전역 범위 지정으로 가리킨다.
- `using namespace std;` 대신 4.2절의 `using std::cout;`, `using std::endl;`처럼 필요한 이름만 가져온다.
- 프로그래머의 이름을 `counter`처럼 표준 라이브러리와 겹치지 않는 이름으로 바꾼다.

`<algorithm>`을 포함하지 않아도 다른 헤더가 내부에서 `<algorithm>`을 포함하면 같은 충돌이 생길 수 있으므로, 이 문제는 어떤 헤더를 포함했는지만으로 예측하기 어렵다. 표 11에서 대규모 프로젝트일수록 using 지시어를 피하는 이유가 이것이다.

### 4.4 이름 공간의 중첩과 별칭

이름 공간 안에 다른 이름 공간을 선언할 수 있으며, 그 안의 이름은 바깥 이름 공간부터 차례로 `::`로 이어 가리킨다. 같은 이름 공간은 여러 번 나누어 열 수 있다. 이름이 길어지면 `namespace 별칭 = 이름공간;`으로 짧은 다른 이름을 붙인다.

```cpp
#include <iostream>
using namespace std;

namespace company {
    namespace graphics {               // 중첩된 이름 공간
        void draw() { cout << "company::graphics::draw()" << endl; }
    }
}

namespace company {                    // 같은 이름 공간을 다시 연다
    namespace audio {
        void play() { cout << "company::audio::play()" << endl; }
    }
}

namespace cg = company::graphics;      // 이름 공간 별칭

int main() {
    company::graphics::draw();
    cg::draw();                        // 별칭으로 같은 함수를 호출
    company::audio::play();
    return 0;
}
```

```text
company::graphics::draw()
company::graphics::draw()
company::audio::play()
```

두 번째 `namespace company`는 새 이름 공간을 만드는 것이 아니라 앞의 `company`를 다시 여는 것이므로, 두 선언은 같은 이름 공간 `company`에 `graphics`와 `audio`를 차례로 더한다. 별칭 `cg`는 새 이름 공간을 만드는 것이 아니라 `company::graphics`를 가리키는 다른 이름이므로, `cg::draw()`와 `company::graphics::draw()`는 같은 함수이다.

> **참고:** **한 줄로 쓰는 중첩 선언.** C++17부터는 `namespace company::audio { }`처럼 중첩된 이름 공간을 한 줄로 선언할 수 있으며, 이 선언은 `namespace company { namespace audio { } }`와 같은 뜻이다 [1]. 위 프로그램의 두 번째 선언을 이 형태로 바꾸어도 출력은 같다.

---

<br>

## 5. C++ 문자열

C++에서 문자열을 표현하는 방식은 두 가지이다. C 언어처럼 `'\0'`으로 끝나는 문자 배열을 쓰는 **C 스트링** 방식과, C++ 표준 라이브러리의 **`string` 클래스**를 쓰는 방식이다.

### 5.1 C 스트링

**C 스트링**은 널 문자 `'\0'`으로 끝나는 `char` 배열이다. 끝에 `'\0'`이 없으면 문자열이 아니라 단순한 문자 배열이다.

```cpp
char name1[6] = {'G', 'r', 'a', 'c', 'e', '\0'};  // 문자열 "Grace"
char name2[5] = {'G', 'r', 'a', 'c', 'e'};        // 문자열이 아닌 단순 문자 배열
char name5[10] = "Grace";                          // 남은 원소는 '\0'으로 초기화
```

```text
name5   [0]  [1]  [2]  [3]  [4]  [5]  [6]  [7]  [8]  [9]
        'G'  'r'  'a'  'c'  'e'  '\0' '\0' '\0' '\0' '\0'
        └─────── "Grace" ───────┘
```

*그림 3. 문자열 "Grace"로 초기화한 배열 name5의 원소*

C 스트링은 C 언어의 문자열 함수(`strcmp()`, `strlen()`, `strcpy()` 등)로 다룬다. 이 함수들은 `<cstring>` 또는 `<string.h>` 헤더 파일에 선언되어 있으며, C++ 표준 방식인 `<cstring>`을 쓰는 것이 바람직하다.

**`cin`으로 문자열 입력:** `cin >> name;`은 키보드에서 읽은 문자열을 `char` 배열 `name`에 저장하고 끝에 `'\0'`을 붙인다. `>>`는 공백 문자를 만나면 입력을 끝내므로, 빈칸이 포함된 문자열은 첫 단어만 읽는다.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "이름을 입력하세요>>";
    char name[11];                   // 영문 10자까지 저장 가능
    cin >> name;                     // 공백을 만나기 전까지 읽음
    cout << "이름은 " << name << "입니다\n";
}
```

```text
이름을 입력하세요>>마이클
이름은 마이클입니다
```

```text
이름을 입력하세요>>마 이 클
이름은 마입니다
```

C++17까지의 `>>`는 배열의 크기를 확인하지 않으므로 배열보다 긴 문자열을 입력하면 배열 밖의 메모리를 덮어쓴다. C++20부터는 배열 크기보다 하나 적은 개수의 문자까지만 읽는다. 한글은 인코딩에 따라 한 글자가 2바이트(CP949) 또는 3바이트(UTF-8)를 차지하므로, 저장할 수 있는 한글 글자 수는 영문보다 적다.

다음 프로그램은 C 스트링과 `strcmp()`로 암호를 비교하여, 암호 `C++`가 입력되면 종료한다.

```cpp
#include <iostream>
#include <cstring>                   // strcmp() 사용을 위한 헤더 파일
using namespace std;

int main() {
    char password[11];
    cout << "프로그램을 종료하려면 암호를 입력하세요." << endl;
    while (true) {
        cout << "암호>>";
        cin >> password;
        if (strcmp(password, "C++") == 0) {
            cout << "프로그램을 정상 종료합니다." << endl;
            break;
        }
        else
            cout << "암호가 틀립니다~~" << endl;
    }
}
```

```text
프로그램을 종료하려면 암호를 입력하세요.
암호>>Java
암호가 틀립니다~~
암호>>C
암호가 틀립니다~~
암호>>C++
프로그램을 정상 종료합니다.
```

**`cin.getline()`으로 공백을 포함한 입력:** 빈칸이 포함된 문자열을 C 스트링으로 입력받으려면 `cin.getline(char buf[], int size, char delimitChar)`를 쓴다. `buf`에 최대 `size - 1`개의 문자를 읽고, `delimitChar`를 만나면 입력을 멈추며, 끝에 `'\0'`을 붙인다. `delimitChar`는 입력 버퍼에서 꺼내 버리고 배열에는 저장하지 않으며, 기본값은 `'\n'`(`<Enter>` 키)이다.

```cpp
char address[100];
cin.getline(address, 100, '\n');     // 최대 99개의 문자를 읽음. <Enter>를 만나면 입력 중단
```

```text
입력: Seoul Korea<Enter>

address  [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] ... [99]
         'S' 'e' 'o' 'u' 'l' ' ' 'K' 'o' 'r' 'e' 'a'  '\0' ...
```

*그림 4. `cin.getline()`으로 Seoul Korea를 읽은 배열 address*

### 5.2 `string` 클래스

C++은 C 스타일 문자 배열(`char[]`)의 현대적이고 안전한 대안으로 `std::string` 클래스를 제공한다. `string`은 `<string>` 헤더 파일에 선언된 C++ 표준 클래스로, 문자열 크기에 맞게 내부 버퍼를 스스로 조절하므로 크기 제약이 없고, 문자열 복사, 비교, 수정을 위한 다양한 함수와 연산자를 제공한다. C 스트링보다 다루기 쉬우므로 C++에서는 `string`의 사용을 권장한다.

```cpp
#include <string>     // std::string 사용에 필요
#include <iostream>
using namespace std;

int main() {
    string greeting = "Hello, C++!";
    cout << greeting << endl;
    return 0;
}
```

**`std::string`과 C 스타일 `char[]`의 비교:**

| 특성 | `std::string` (C++) | `char[]` (C) |
|:----|:-------------------|:-------------|
| 헤더 | `#include <string>` | `#include <cstring>` |
| 선언 | `string s = "Hello";` | `char s[] = "Hello";` |
| 연결 | `s1 + s2` | `strcat(s1, s2)` |
| 비교 | `s1 == s2` | `strcmp(s1, s2) == 0` |
| 길이 | `s.length()` 또는 `s.size()` | `strlen(s)` |
| 동적 크기 | 예 (자동으로 확장) | 아니오 (선언 시 고정) |
| 안전성 | 예 (범위 관리됨) | 아니오 (수동 버퍼 관리) |

*표 12. `std::string`과 `char[]`의 비교*

**주요 연산:**

```cpp
string a = "Hello";
string b = "World";

// 연결
string c = a + ", " + b + "!";   // "Hello, World!"

// 비교
if (a == b) { /* ... */ }         // == 로 자연스럽게 비교

// 길이
cout << c.length() << endl;       // 13

// getline으로 입력 (공백 포함 한 줄 전체 읽기)
string line;
getline(cin, line);

// 개별 문자 접근
cout << a[0] << endl;             // 'H'
```

다음 프로그램은 `+`로 문자열을 연결하고, `[]`로 문자에 접근하고, `getline()`으로 빈칸이 포함된 문자열을 입력받아 `==`로 비교한다. `getline()`은 `string` 객체로 한 줄을 입력받기 위해 제공되는 전역 함수이다.

```cpp
#include <iostream>
#include <string>                    // string 클래스를 사용하기 위한 헤더 파일
using namespace std;

int main() {
    string song("Falling in love with you");
    string elvis("Elvis Presley");
    string singer;

    cout << song + "를 부른 가수는";                    // +로 문자열 연결
    cout << "(힌트 : 첫글자는 " << elvis[0] << ")?";    // [] 연산자로 문자 접근
    getline(cin, singer);                                // 빈칸을 포함한 문자열 입력
    if (singer == elvis)                                 // 문자열 비교
        cout << "맞았습니다.";
    else
        cout << "틀렸습니다. " + elvis + "입니다." << endl;
}
```

```text
Falling in love with you를 부른 가수는(힌트 : 첫글자는 E)?Elvis Pride
틀렸습니다. Elvis Presley입니다.
```

> **핵심:** `std::string`은 여기서 간략히 소개한다. 자세한 문자열 연산과 멤버 함수는 4장, 반복자는 10장에서 다룬다. 지금은 C++에서 `string`이 `char[]`보다 안전하고 편리하다는 점만 알아두면 된다.

### 5.3 C 스트링과 string 객체의 복사와 비교

C 스트링과 `string` 객체는 복사와 비교에서 동작이 다르다. C 스트링을 가리키는 포인터를 복사하면 주소만 복사되어 두 이름이 같은 배열을 가리키지만, `string` 객체를 복사하면 내용이 복사된 독립된 객체가 생긴다.

```cpp
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main() {
    char c1[10] = "cat";
    char *c2 = c1;                    // 포인터 복사: 같은 배열을 가리킨다
    c2[0] = 'b';
    cout << c1 << ' ' << c2 << endl;

    string s1 = "cat";
    string s2 = s1;                   // string 복사: 독립된 사본
    s2[0] = 'b';
    cout << s1 << ' ' << s2 << endl;

    string a = "apple", b = "banana", z = "Zoo";
    cout << boolalpha << (a < b) << ' ' << (z < a) << ' ' << a.compare(b) << endl;
    return 0;
}
```

```text
bat bat
cat bat
true true -1
```

```text
 C 스트링                                  string 객체

 c1 ┌───┬───┬───┬────┬─────┐               s1 ┌───────┐
    │'b'│'a'│'t'│'\0'│ ... │                  │ "cat" │
    └───┴───┴───┴────┴─────┘                  └───────┘
      ▲                                    s2 ┌───────┐
 c2 ──┘ (같은 배열을 가리킴)                     │ "bat" │  (별도의 내용)
                                              └───────┘
```

*그림 5. 포인터 복사와 string 객체 복사의 차이*

- `c2[0] = 'b'`는 `c1`과 같은 배열을 바꾸므로 `c1`도 `bat`이 된다. 반면 `s2`는 `s1`의 내용을 복사하여 만든 다른 객체이므로 `s2[0] = 'b'`가 `s1`에 영향을 주지 않는다. 이 차이는 5장의 얕은 복사와 깊은 복사로 이어진다. `string`은 깊은 복사를 하도록 만들어진 클래스이다.
- `string`의 비교 연산자는 문자열을 앞 글자부터 문자 코드로 비교하는 사전 순서를 따른다. "apple"과 "banana"는 첫 글자 `a`(97)와 `b`(98)에서 순서가 정해지므로 `a < b`가 참이다. "Zoo"가 "apple"보다 작은 것은 대문자 `Z`(90)의 코드가 소문자 `a`(97)보다 작기 때문이며, 실제 사전과 다르다.
- `compare()`는 두 문자열이 같으면 0, 호출한 쪽이 앞서면 음수, 뒤서면 양수를 리턴한다. 이 구현에서는 -1이 출력되었지만 표준이 보장하는 것은 부호뿐이다.

### 5.4 C 스트링을 다룰 때의 흔한 실수

**`==`로 C 스트링을 비교함.** 두 `char` 배열에 같은 문자열이 들어 있어도 `pw == input`은 거짓이다.

```cpp
char pw[10] = "C++";
char input[10] = "C++";
if (pw == input) cout << "== 같다" << endl;          // 배열의 주소를 비교한다
else cout << "== 다르다" << endl;
if (strcmp(pw, input) == 0) cout << "strcmp 같다" << endl;   // 내용을 비교한다
```

```text
== 다르다
strcmp 같다
```

식에 쓰인 배열 이름은 첫 원소의 주소로 바뀌므로 `pw == input`은 서로 다른 두 배열의 주소를 비교하여 언제나 거짓이다. Apple clang은 경고 옵션 없이도 "array comparison always evaluates to false"라는 경고를 낸다. C 스트링의 내용은 `strcmp()`로 비교하고, `string` 객체는 `==`로 비교한다.

**배열보다 긴 문자열을 `strcpy()`로 복사함.** `char name[5];`에 `strcpy(name, "Christopher");`를 실행하면 널 문자를 포함한 12바이트가 5바이트 배열에 복사되어 배열 밖의 스택 메모리를 덮어쓴다. 이 경우처럼 복사할 길이를 컴파일할 때 알 수 있으면 Apple clang이 "'strcpy' will always overflow; destination buffer has size 5, but the source string has length 12 (including NUL byte)"라고 경고하고, AddressSanitizer로 실행하면 `stack-buffer-overflow`가 보고된다. 길이를 실행 중에 알게 되는 입력은 경고 없이 넘어가므로 더 위험하다. 크기를 신경 쓰지 않아도 되는 `string`을 쓰는 것이 가장 안전하다.

### 5.5 문자열을 입력받는 네 가지 방법의 비교

지금까지 본 문자열 입력 방법은 저장하는 곳(C 스트링 또는 `string`)과 입력을 멈추는 곳(공백 또는 줄 끝)에 따라 네 가지로 나뉜다. 한 프로그램에서 네 방법을 차례로 쓰고 세 줄을 입력하면, 각 방법이 입력 버퍼의 어디까지를 가져가는지 확인할 수 있다.

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    char word[10], line[20];
    string s1, s2;
    cin >> word;                      // (1) 공백 앞까지
    cin.ignore();                     // 남은 공백 하나를 버린다
    cin.getline(line, 20);            // (2) 줄 끝까지, 최대 19글자
    cin >> s1;                        // (3) 공백 앞까지, 길이 제한 없음
    cin.ignore();                     // 남은 줄바꿈을 버린다
    getline(cin, s2);                 // (4) 줄 끝까지, 길이 제한 없음
    cout << '[' << word << "][" << line << "][" << s1 << "][" << s2 << ']' << endl;
    return 0;
}
```

```text
$ printf 'Hello big world\nC++ Programming\nSeoul Korea\n' | ./strin
[Hello][big world][C++][Programming]
```

1. `cin >> word`는 첫 줄의 `Hello`를 읽고 그 뒤의 공백 앞에서 멈춘다.
2. `cin.ignore()`가 그 공백을 버린 뒤, `cin.getline(line, 20)`이 줄 끝까지의 `big world`를 읽고 줄바꿈 문자를 꺼내 버린다.
3. `cin >> s1`은 둘째 줄의 `C++`를 읽고 공백 앞에서 멈춘다.
4. `cin.ignore()`가 공백을 버리고, `getline(cin, s2)`가 줄 끝까지의 `Programming`을 읽는다. 셋째 줄 `Seoul Korea`는 읽히지 않고 버퍼에 남는다.

| 방법 | 저장 위치 | 멈추는 곳 | 멈춘 문자의 처리 | 길이 제한 |
|:----|:----|:----|:----|:----|
| `cin >> word` | `char` 배열 | 공백, 탭, 줄바꿈 | 버퍼에 남긴다 | 배열 크기(C++17까지는 검사하지 않음, 5.1절) |
| `cin.getline(buf, n)` | `char` 배열 | 줄 끝(또는 지정한 문자) | 꺼내서 버린다 | 최대 `n - 1`글자 |
| `cin >> s` | `string` | 공백, 탭, 줄바꿈 | 버퍼에 남긴다 | 없음 |
| `getline(cin, s)` | `string` | 줄 끝(또는 지정한 문자) | 꺼내서 버린다 | 없음 |

*표 13. 문자열을 입력받는 네 가지 방법*

표에서 `>>`는 멈춘 공백이나 줄바꿈을 버퍼에 남기고, `getline` 계열은 줄바꿈을 꺼내서 버린다는 차이가 3.2절의 함정을 만든다. `>>` 다음에 `getline`을 쓰면 남은 줄바꿈 때문에 빈 문자열을 읽고, `getline` 다음의 `>>`는 앞의 공백을 건너뛰므로 문제가 없다. 공백이 포함된 한 줄 전체를 길이 걱정 없이 받으려면 `getline(cin, s)`가 가장 안전하다.

---

<br>

## 6. `auto`를 사용한 현대적 변수 선언

C++11부터 `auto` 키워드를 사용하면 컴파일러가 초기화 값으로부터 변수의 **타입을 추론**한다. 이는 중복을 줄이고 코드 유지보수성을 높인다.

```cpp
auto x = 42;           // int
auto pi = 3.14;        // double
auto name = "Hello"s;  // std::string (s 접미사, std::literals 필요: using namespace std;로도 쓸 수 있음)
auto greeting = string("Hello");  // std::string

// 복잡한 타입에서 특히 유용
vector<int> nums = {1, 2, 3};
auto it = nums.begin();   // vector<int>::iterator: auto로 훨씬 간결
```

**`auto` 사용 시점:**

| `auto` 사용 | 명시적 타입 사용 |
|:-----------|:--------------|
| 우변에서 타입이 명확할 때 | 타입이 불명확하여 가독성이 떨어질 때 |
| 복잡한 타입 (반복자, 람다 등) | 의도적으로 다른 타입을 원할 때 (예: `int x = 3.14;`로 절삭) |
| 범위 기반 for 루프: `for (auto& item : vec)` | 공개 API / 함수 시그니처 (명확성이 중요) |

*표 14. `auto`와 명시적 타입의 사용 시점*

> **참고:** `auto`는 변수가 타입이 없거나 동적 타입이라는 뜻이 아니다. 타입은 **컴파일 시점**에 결정되며 이후 고정된다. C++은 여전히 정적 타입 언어이다.

---

<br>

## 7. 헤더 파일과 전처리기

### 7.1 전처리기의 헤더 확장

`#include <iostream>`을 만나면 전처리기는 `<iostream>` 헤더 파일의 내용을 그 자리에 펼쳐 넣는다. `<iostream>`은 다시 `<istream>`, `<ostream>`, `<ios>` 같은 헤더를 포함하므로, 전처리기는 이들을 모두 펼친 확장된 소스를 만들고 컴파일러는 이 확장된 소스를 컴파일한다. 어떤 헤더가 어떤 헤더를 포함하는지는 컴파일러의 구현마다 다르다.

```mermaid
flowchart LR
    S["SimpleC++.cpp"] --> P["전처리기"]
    H["iostream, istream, ostream, ios 헤더 파일"] --> P
    P --> X["확장된 SimpleC++.cpp"]
    X --> C["컴파일러"]
```

*그림 6. 전처리기가 헤더 파일을 펼쳐 확장된 소스를 만드는 과정*

`iostream` 같은 표준 헤더 파일은 확장자가 없는 텍스트 파일이며, 컴파일러가 설치된 폴더 아래의 `include` 폴더에 있다. 예를 들어 Visual Studio 2017 Community에서는 `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.10.25017\include`에 있다.

### 7.2 `<iostream>`과 `<iostream.h>`

표준 이전의 구 버전 C++에서는 `#include <iostream.h>`를 썼고, 이름이 `std` 네임스페이스 없이 선언되어 있었다. 표준 C++(C++98 이후)에서는 `#include <iostream>`을 쓰고, 이름이 `std` 네임스페이스에 있으므로 `using namespace std;`나 `std::` 접두사와 함께 쓴다. `<iostream.h>`는 Visual Studio 6.0 같은 구 버전 컴파일러에서만 쓸 수 있다.

### 7.3 `#include <헤더파일>`과 `#include "헤더파일"`

- **`#include <헤더파일>`:** 컴파일러가 설치된 폴더(시스템 include 경로)에서 헤더 파일을 찾는다. `<iostream>`처럼 컴파일러가 제공하는 헤더 파일에 쓴다.
- **`#include "헤더파일"`:** 개발자의 프로젝트 폴더와 개발자가 컴파일 옵션으로 지정한 include 폴더에서 헤더 파일을 찾고, 찾지 못하면 `<>`와 같은 위치에서 다시 찾는다. 개발자가 작성한 헤더 파일에 쓴다.

### 7.4 헤더 파일과 라이브러리

`<cstring>` 헤더 파일에는 `strcpy()` 함수의 코드가 들어 있지 않고 함수의 원형(prototype)만 선언되어 있다. 함수의 코드는 컴파일된 바이너리 형태로 라이브러리 파일(Visual Studio의 경우 `lib` 폴더의 `libcmt.lib` 등)에 들어 있으며, 링크할 때 실행 파일에 들어간다.

헤더 파일이 필요한 이유는 컴파일러가 사용자 프로그램의 `strcpy()` 호출 구문이 정확한지(인자의 개수와 타입, 리턴 타입) 확인해야 하기 때문이다. 마찬가지로 `cout`과 `cin`은 `<iostream>`에 선언된 객체이며, 실제 객체는 표준 라이브러리에 들어 있다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:----|:---------|
| 프로그램 구조 | `#include <iostream>`, `using namespace std;`, `int main()`, 실행문, `return 0;`의 순서로 구성된다. |
| 주석 | `/* 블록 */`과 `// 줄`의 두 형식이 있으며 C와 같고, 블록 주석은 중첩할 수 없다. |
| `main` 함수 | 프로그램의 진입점으로 `int`를 리턴하며, `void main()`은 비표준이다. |
| `#include <iostream>` | 전처리기가 입출력 클래스와 `cout`, `cin` 객체가 선언된 헤더를 소스에 펼쳐 넣게 한다. |
| `cout`과 `<<` | `cout`은 출력용 `ostream` 객체이고, `<<`는 타입을 자동 인식하는 스트림 삽입 연산자로 체이닝할 수 있다. |
| `endl` vs `'\n'` | `endl`은 버퍼를 비우고 `'\n'`은 비우지 않으므로, 성능을 위해서는 `'\n'`을 선호한다. |
| `<<` 우선순위 | `<<`는 비교 연산자보다 우선순위가 높으므로 `cout << (5 > 3)`처럼 괄호를 쓴다. |
| `cin`과 `>>` | `cin`은 입력 버퍼를 가진 `istream` 객체이고, `>>`는 공백을 건너뛰며 값을 읽는 스트림 추출 연산자로 체이닝할 수 있다. |
| `getline` | 공백을 포함한 한 줄 전체를 `string`에 읽으며, `>>` 다음에 쓸 때는 먼저 `cin.ignore()`로 남은 줄바꿈을 버린다. |
| `cin` vs `scanf` | `cin`은 타입 안전하고 형식 지정자가 필요 없으며, 기본 타입에 `&`를 붙이지 않는다. |
| 변수 선언 위치 | C++에서는 실행문 중간 어디서나 변수를 선언할 수 있어 사용 직전에 선언하면 타이핑 오류를 줄인다. |
| 네임스페이스 | 이름 충돌을 방지하는 선언 영역이며, 표준 라이브러리는 `std`에 있어 `std::cout`처럼 쓴다. |
| `using namespace std` | `std::` 접두사를 생략하게 하며, `using std::cout`으로 특정 이름만 가져올 수도 있다. |
| C 스트링 | `'\0'`으로 끝나는 `char` 배열이며, `<cstring>`의 함수로 다루고 `cin.getline()`으로 공백을 포함해 입력받는다. |
| `std::string` | `char[]`의 현대적 대안으로, `+`로 연결하고 `==`로 비교하며 `.length()`로 길이를 확인한다. |
| `std::format` / `std::print` | C++20과 C++23의 현대적 출력 서식으로, `{}` 자리 표시자를 쓰며 `cout`처럼 타입 안전하다. |
| `auto` | 컴파일러가 초기화 값에서 타입을 추론하며(C++11), 반복자 같은 복잡한 타입에 유용하고 여전히 정적 타입이다. |
| 헤더 파일과 라이브러리 | 헤더 파일에는 함수의 원형이 선언되어 있고, 함수의 코드는 라이브러리에 있어 링크할 때 실행 파일에 들어간다. |
| `<>`와 `""` | `<>`는 컴파일러가 제공하는 헤더를, `""`는 개발자의 프로젝트 폴더부터 헤더를 찾는다. |
| using 지시어의 이름 충돌 | `using namespace std;` 아래에서 프로그래머의 이름이 `std`의 이름과 같으면 그 이름을 사용하는 곳에서 모호성 오류가 나며, 범위 지정이나 선택적 using 선언으로 피한다. |
| 이름 공간의 중첩과 별칭 | 이름 공간은 중첩할 수 있고 여러 번 나누어 열 수 있으며, `namespace x = a::b;`로 별칭을 붙인다. |
| bool 타입 | `true`와 `false`만 가지는 기본 타입으로, 정수와 섞으면 1과 0으로 바뀌고 0이 아닌 정수는 `true`가 되며, `boolalpha`로 `true`, `false`를 출력한다. |
| 입력이 끝날 때까지 읽기 | `while (cin >> x)`는 읽기에 성공하는 동안 반복하고, 입력의 끝이나 타입이 맞지 않는 값에서 끝난다. |
| 복사와 비교의 차이 | C 스트링의 포인터 복사는 같은 배열을 공유하지만 `string` 복사는 독립된 사본을 만들며, `string`은 사전 순서로 비교된다. |
| C 스트링의 흔한 실수 | `==`는 배열의 주소를 비교하므로 `strcmp()`를 쓰고, `strcpy()`는 배열의 크기를 확인하지 않아 넘침이 생길 수 있다. |
| `<<`의 연쇄 | `cout << a`의 결과가 `cout` 자신이므로 `cout << a << b`는 `(cout << a) << b`로 차례로 출력되며, 출력 방법은 값의 타입으로 정해진다. |
| 문자 하나의 입력 | `cin >> ch`는 공백을 건너뛰고 읽으며, `cin.get(ch)`는 공백과 줄바꿈을 포함한 다음 문자를 그대로 읽는다. |
| 문자열 입력 방법의 비교 | `>>`는 공백 앞까지 읽고 멈춘 문자를 버퍼에 남기며, `getline` 계열은 줄 끝까지 읽고 줄바꿈을 버리고, `string`에 저장하는 방법은 길이 제한이 없다. |

---

<br>

## 점검 문제

1. **연산자 우선순위:** `cout << 5 > 3;`이 컴파일 오류가 되는 이유를 설명하고, `5 > 3`의 결과를 출력하도록 고쳐 쓰라.

   <details>
   <summary>정답</summary>

   `<<`는 `>`보다 우선순위가 높으므로 문장은 `(cout << 5) > 3`으로 해석된다. `cout << 5`의 결과는 `ostream` 객체이며 이를 정수 `3`과 비교할 수 없으므로 컴파일 오류가 발생한다. `cout << (5 > 3);`으로 쓰면 `1`이 출력된다.

   </details>

2. **출력 결과:** `int n = 3; char c = '#';`일 때 `cout << c << 5.5 << '-' << n << "hello" << true;`의 출력을 쓰라.

   <details>
   <summary>정답</summary>

   `#5.5-3hello1`이다. 값 사이에 공백이 들어가지 않고, `true`는 `boolalpha`를 쓰지 않으면 `1`로 출력된다.

   </details>

3. **`>>`와 `getline`의 혼용:** `cin >> age;` 다음에 `getline(cin, name);`을 호출하면 이름을 입력할 기회 없이 `name`이 빈 문자열이 된다. 원인과 해결 방법을 쓰라.

   <details>
   <summary>정답</summary>

   `cin >> age;`는 숫자만 읽고 사용자가 누른 `<Enter>`의 줄바꿈 `'\n'`을 입력 버퍼에 남긴다. `getline`은 첫 번째 `'\n'`까지 읽으므로 남은 줄바꿈을 바로 만나 빈 문자열을 리턴한다. `getline` 앞에서 `cin.ignore();`나 `cin.ignore(numeric_limits<streamsize>::max(), '\n');`로 남은 줄바꿈을 버리면 된다.

   </details>

4. **C 스트링 입력:** `char name[11]; cin >> name;`에 `Kim Minsu`를 입력하면 `name`에 무엇이 저장되는가? 공백을 포함한 한 줄 전체를 읽는 방법 두 가지를 쓰라.

   <details>
   <summary>정답</summary>

   `>>`는 공백을 만나면 입력을 끝내므로 `name`에는 `"Kim"`이 저장된다. 공백을 포함해 읽으려면 C 스트링에는 `cin.getline(name, 11);`을 쓰고, `string` 객체에는 `getline(cin, str);`을 쓴다.

   </details>

5. **헤더 파일과 라이브러리:** `<cstring>` 헤더 파일에 `strcpy()` 함수의 코드가 들어 있는가? 함수의 코드는 어디에 있으며, 헤더 파일은 왜 필요한가?

   <details>
   <summary>정답</summary>

   들어 있지 않다. 헤더 파일에는 `strcpy()`의 원형만 선언되어 있고, 함수의 코드는 컴파일된 바이너리로 라이브러리 파일에 들어 있다가 링크할 때 실행 파일에 들어간다. 헤더 파일은 컴파일러가 `strcpy()` 호출의 인자 개수와 타입, 리턴 타입이 맞는지 확인하는 데 필요하다.

   </details>

6. **이름 충돌:** 4.3절의 프로그램에서 `count++;`와 `cout << count << endl;`의 `count`를 모두 `::count`로 바꾸면 컴파일되는가? 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   컴파일되고 1이 출력된다. `::count`는 전역 범위에 선언된 이름만 가리키므로 using 지시어로 함께 찾게 된 `std::count`는 후보가 되지 않는다. 전역 변수 `count`는 0에서 1이 된다.

   </details>

7. **bool 변환:** `bool b = -1; int n = b + false; cout << boolalpha << b << ' ' << n;`의 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   `true 1`이다. -1은 0이 아니므로 `b`는 `true`가 된다. `b + false`에서 `true`는 1, `false`는 0으로 바뀌므로 `n`은 1이다. `boolalpha`는 `bool` 값에만 영향을 주므로 `n`은 `1`로 출력된다.

   </details>

8. **입력 반복:** 3.5절의 프로그램에 `5 7 8.5 9`를 입력하면 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   `3개, 합 20`이다. 5와 7을 읽은 뒤, 세 번째 읽기는 `8.5`에서 정수 부분 8을 읽고 성공한다. 네 번째 읽기는 남은 `.5`의 `.`이 정수가 될 수 없어 실패하므로 반복이 끝나고, 9는 읽히지 않는다. 합은 5 + 7 + 8 = 20이다.

   </details>

9. **출력 타입:** `char c = 'a'; cout << c << ' ' << c + 1 << ' ' << char(c + 1) << endl;`의 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   `a 98 b`이다. `c`는 `char`이므로 문자 `a`로 출력된다. `c + 1`은 `char`가 `int`로 바뀌어 더해지므로 `'a'`의 코드 97에 1을 더한 `int` 98이 되어 정수로 출력된다. `char(c + 1)`은 그 결과를 다시 `char`로 바꾸므로 문자 `b`로 출력된다.

   </details>

10. **문자열 입력:** 5.5절의 프로그램에 `Hi`, `C++ Programming`, `Seoul Korea`의 세 줄을 입력하면 출력은 무엇인가?

    <details>
    <summary>정답</summary>

    `[Hi][C++ Programming][Seoul][Korea]`이다. `cin >> word`가 `Hi`를 읽은 뒤 버퍼에는 첫 줄의 줄바꿈이 남고, `cin.ignore()`가 그 줄바꿈을 버린다. 그래서 `cin.getline(line, 20)`은 둘째 줄 전체인 `C++ Programming`을 읽는다. 이어서 `cin >> s1`이 셋째 줄의 `Seoul`을 읽고, `cin.ignore()`가 공백을 버린 뒤 `getline(cin, s2)`가 `Korea`를 읽는다. 첫 줄에 공백이 없어도 `cin.ignore()`가 버리는 문자가 공백에서 줄바꿈으로 바뀔 뿐이다.

    </details>

---

<br>

## 참고 문헌

1. cppreference.com, "Namespaces," *cppreference.com*. https://en.cppreference.com/w/cpp/language/namespace
