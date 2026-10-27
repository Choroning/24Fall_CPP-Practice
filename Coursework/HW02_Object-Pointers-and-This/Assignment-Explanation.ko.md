# HW2 과제 해설 — 객체 포인터와 this 포인터

[레포지토리 안내](../../README.ko.md) | [English](Assignment-Explanation.md) | [한국어](Assignment-Explanation.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

> **학습 목표:**
> 1. 객체 배열의 이름을 객체 포인터에 대입하고, 포인터 연산으로 배열 원소의 멤버 함수를 호출할 수 있다
> 2. 객체 포인터로 멤버에 접근하는 `p->멤버`와 `(*p).멤버`가 같은 뜻이며, 뒤의 형식에 괄호가 필요한 이유를 설명할 수 있다
> 3. 크기가 고정된 배열과 `new`로 만든 동적 배열을 비교하고, 가변 길이 배열이 표준 C++에 속하지 않음을 설명할 수 있다
> 4. `delete[]`로 동적 배열을 반환한 뒤 포인터에 `nullptr`를 대입하는 이유를 댕글링 포인터와 연결하여 설명할 수 있다
> 5. 멤버 함수가 `this`를 리턴하여 호출을 이어 쓰는 방식을 `*this`를 참조로 리턴하는 방식과 비교할 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 과제 구성과 실행 방법](#1-과제-구성과-실행-방법)
- [2. Q1. 포인터로 다루는 Complex 객체 배열](#2-q1-포인터로-다루는-complex-객체-배열)
  - [2.1 요구사항](#21-요구사항)
  - [2.2 설계와 해결 방법](#22-설계와-해결-방법)
  - [2.3 실행 결과와 검토](#23-실행-결과와-검토)
- [3. Q2. 포인터로 객체를 참조하는 Person 클래스](#3-q2-포인터로-객체를-참조하는-person-클래스)
  - [3.1 요구사항](#31-요구사항)
  - [3.2 설계와 해결 방법](#32-설계와-해결-방법)
  - [3.3 실행 결과와 검토](#33-실행-결과와-검토)
- [4. Q3. 정적 배열과 동적 배열](#4-q3-정적-배열과-동적-배열)
  - [4.1 요구사항](#41-요구사항)
  - [4.2 설계와 해결 방법](#42-설계와-해결-방법)
  - [4.3 실행 결과와 검토](#43-실행-결과와-검토)
- [5. Q4. this 포인터를 리턴하는 Counter 클래스](#5-q4-this-포인터를-리턴하는-counter-클래스)
  - [5.1 요구사항](#51-요구사항)
  - [5.2 설계와 해결 방법](#52-설계와-해결-방법)
  - [5.3 실행 결과와 검토](#53-실행-결과와-검토)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 과제 구성과 실행 방법

이 과제는 4장의 객체 포인터, 동적 배열, `this` 포인터를 연습한다. Q1은 5주차 강의자료의 [문제 5-2]이고, Q2와 Q4는 주제만 정해진 자유 코드이며, Q3은 5주차 강의자료에 실린 동적 메모리 코드를 분석하는 문항이다. 과제 안내문은 [`Original/Assignment.ko.md`](Original/Assignment.ko.md)에 있고, 각 코드를 처음부터 끝까지 따라가는 설명은 [HW2 코드 총정리](Code-Walkthrough.ko.md)에 있다.

```text
Coursework/HW02_Object-Pointers-and-This/
├── Original/
│   ├── Assignment.md        # 과제 안내문 (영어)
│   └── Assignment.ko.md     # 과제 안내문 (한국어)
└── Solution/
    ├── Q1.cpp               # 포인터로 다루는 Complex 객체 배열
    ├── Q2.cpp               # 포인터로 객체를 참조하는 Person 클래스
    ├── Q3.cpp               # 분석 대상인 강의자료의 정적 배열과 동적 배열 코드
    └── Q4.cpp               # this 포인터를 리턴하는 Counter 클래스
```

*그림 1. HW2 과제의 디렉토리 구성*

각 풀이는 다음과 같이 컴파일하고 실행한다. 네 파일 모두 macOS의 Apple clang 21에서 `g++ -std=c++11 -Wall`과 `g++ -std=c++17 -Wall`로 컴파일된다. Q3은 크기를 입력받으므로 실행한 뒤 정수 하나를 입력한다.

```bash
cd Coursework/HW02_Object-Pointers-and-This/Solution
g++ -std=c++17 -Wall -o q1 Q1.cpp
./q1
```

| 파일 | 컴파일 결과 |
|:-----|:-----|
| `Q1.cpp` | 경고 없음 |
| `Q2.cpp` | 경고 없음 |
| `Q3.cpp` | 경고 없음 |
| `Q4.cpp` | 경고 없음 |

*표 1. HW2 풀이의 컴파일 결과*

Xcode에서 실행할 때 콘솔의 마지막 줄에 나타나는 `Program ended with exit code: 0`은 프로그램이 출력한 것이 아니라 Xcode가 프로그램이 끝난 뒤 표시하는 문구이다. Q2를 제외한 세 `main()`에는 `return` 문이 없지만, C++에서는 `main()`의 끝에 도달하면 0을 반환한 것으로 처리하므로 종료 코드는 0이다.

---

<br>

## 2. Q1. 포인터로 다루는 Complex 객체 배열

### 2.1 요구사항

`private` 정수 멤버 `real`, `image`를 갖는 복소수 클래스 `Complex`를 정의한다. 기본 생성자는 두 멤버를 1로, 매개변수 2개 생성자는 받은 인자로 초기화하며, 두 멤버를 각각 수정하는 set 함수와 복소수 형태로 출력하는 `showC()`를 둔다. `main()`은 크기가 `MAX`(2)인 객체 배열과 그 주소를 받은 포인터 `p`로 다음 일을 한다.

| `main()` 뼈대의 주석 | 대응하는 코드 |
|:-----|:-----|
| 배열 크기 `MAX`, 초기값 없는 `Complex` 배열 객체 `data` 생성 | `Complex data[MAX];` |
| 배열 `data`의 주소를 할당받는 포인터 `p` 생성 | `Complex *p = data;` |
| 객체 배열 `data`의 정보를 `for` 문으로 출력 | `data[i].showC();` |
| 첫 번째 배열 객체의 `real`을 3으로 업데이트 | `data[0].setReal(3);` |
| 두 번째 배열 객체의 `image`를 포인터 `p`로 -8로 업데이트 | `(*(p + 1)).setImage(-8);` |
| 포인터 `p`로 객체 배열의 정보를 `for` 문으로 출력 | `(p + i)->showC();` |

*표 2. Q1의 `main()` 뼈대와 대응하는 코드*

### 2.2 설계와 해결 방법

입출력을 위해 `<iostream>`을 포함하고, 매크로 상수 `MAX`를 2로 정의하며, `using namespace std;`로 표준 네임스페이스를 기본으로 쓴다. `Complex` 클래스는 복소수를 표현한다. 접근 지정자 없이 선언한 `real`과 `image`는 `class`의 기본 접근 지정이 `private`이므로 `private` 멤버가 되고, `public` 아래에 두 생성자와 `setReal()`, `setImage()`, `showC()`를 클래스 안에서 선언과 동시에 정의했다.

- **기본 생성자:** `real = image = 1;`로 두 멤버를 1로 초기화한다.
- **매개변수 2개 생성자:** 받은 실수부와 허수부를 각 멤버에 대입한다. 이 `main()`에서는 쓰이지 않는다.
- **`setReal()`, `setImage()`:** 실수부와 허수부를 각각 받은 값으로 바꾼다.
- **`showC()`:** `real << " + " << image << "i"`로 복소수를 `a + bi` 형태로 출력한다.

`main()`은 `Complex data[MAX];`로 객체 배열을 만든다. 초기값이 없으므로 두 원소 모두 기본 생성자로 생성되어 (1, 1)에서 시작한다([4장 3.1절](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.ko.md#31-정적-객체-배열)). `Complex *p = data;`에서 배열 이름 `data`는 첫 원소의 주소로 변환되므로 `p`는 `data[0]`을 가리킨다. 첫 번째 `for` 문은 `data[i].showC()`로 두 원소를 출력하고, 구분선을 출력한 뒤 `data[0].setReal(3);`으로 첫 원소의 실수부를 3으로 바꾼다. `(*(p + 1)).setImage(-8);`은 `p + 1`이 가리키는 두 번째 원소를 `*`로 역참조한 뒤 점 연산자로 `setImage()`를 호출하여 허수부를 -8로 바꾼다. 포인터에 1을 더하면 주소는 1바이트가 아니라 원소 하나의 크기인 `sizeof(Complex)`(이 환경에서 8바이트)만큼 늘어나므로 `p + 1`은 `&data[1]`과 같다. 마지막 `for` 문은 `(p + i)->showC()`로 포인터를 통해 두 원소를 다시 출력한다([4장 1.1절](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.ko.md#11-객체에-대한-포인터)).

다음 두 가지는 포인터 표기와 출력 형식에 관한 점이다.

- **역참조에 괄호가 필요한 이유:** 점 연산자 `.`는 역참조 연산자 `*`보다 우선순위가 높다. 따라서 바깥 괄호 없이 `*(p + 1).setImage(-8);`이라고 쓰면 `*((p + 1).setImage(-8))`로 해석되어 포인터에 점 연산자를 쓴 것이 되므로 컴파일 오류가 난다. Clang은 이 경우 `member reference type 'Complex *' is a pointer; did you mean to use '->'?`라고 알린다. `(*(p + 1)).setImage(-8)`, `(p + 1)->setImage(-8)`, `p[1].setImage(-8)`은 모두 같은 호출이다.
- **음수 허수부의 출력:** `showC()`는 부호를 따로 처리하지 않으므로 허수부가 -8이면 `1 + -8i`로 출력된다. 문제의 실행 예시도 같은 형식이므로 요구사항에는 맞는다. `1 - 8i`처럼 출력하려면 `showC()`에서 `image`가 음수인지 검사하여, 음수이면 `" - "`와 `-image`를 출력해야 한다.

### 2.3 실행 결과와 검토

```text
1 + 1i
1 + 1i
---------------
3 + 1i
1 + -8i
```

첫 두 줄은 기본 생성자가 만든 (1, 1)이고, 마지막 두 줄은 `data[0]`의 실수부와 `data[1]`의 허수부가 바뀐 결과이다.

---

<br>

## 3. Q2. 포인터로 객체를 참조하는 Person 클래스

### 3.1 요구사항

포인터로 객체를 가리키고, 그 포인터를 통해 객체의 멤버에 접근하는 코드를 자유롭게 작성한다. 풀이는 이름과 나이를 가진 `Person` 클래스를 정의하고, 두 번째 객체를 가리키는 포인터로 자기소개 함수와 나이를 바꾸는 함수를 호출한다.

### 3.2 설계와 해결 방법

입출력과 문자열 처리를 위해 `<iostream>`과 `<string>`을 포함하고, `using namespace std;`로 표준 네임스페이스를 기본으로 쓴다. `Person` 클래스는 사람을 표현하며, `private` 멤버로 이름 `name`과 나이 `age`를 두고 `public` 아래에 생성자, `introduce()`, `setAge()`를 둔다.

- **생성자:** `Person(string n, int a) : name(n), age(a) {}`는 **멤버 초기화 리스트(member initializer list)** 로 멤버가 만들어지는 시점에 `name`을 `n`으로, `age`를 `a`로 초기화한다. 생성자 몸체에서 대입하는 방식과 결과는 같다([3장 4.4절](../../Lecture-Notes/Ch03_Classes-and-Objects/Concepts_Lecture.ko.md#44-멤버-변수의-초기화-방법)).
- **`introduce()`:** 이름과 나이를 넣은 자기소개 문장을 출력한다.
- **`setAge()`:** 나이를 받은 값으로 바꾼다.

`main()`은 이름과 나이가 다른 두 객체 `person1`(20살)과 `person2`(25살)를 만들고, `Person* personPtr = &person2;`로 `person2`의 주소를 포인터에 저장한다. `person1.introduce();`는 객체로 직접, `personPtr->introduce();`는 포인터를 통해 `person2`의 멤버 함수를 호출한다. 이어서 `(*personPtr).setAge(31);`로 포인터를 역참조한 뒤 점 연산자로 `setAge()`를 호출하여 `person2`의 나이를 31로 바꾸고, 다시 `personPtr->introduce();`로 바뀐 나이가 반영된 자기소개를 출력한다. `p->f()`와 `(*p).f()`는 같은 뜻이며, 화살표 연산자가 더 간결하다([4장 1.1절](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.ko.md#11-객체에-대한-포인터)).

다음 두 가지는 객체 포인터를 쓸 때 알아 둘 성질이다.

- **포인터는 객체를 복사하지 않는다:** `personPtr`는 `person2`의 주소만 가지므로, 포인터로 바꾼 나이는 `person2` 자체의 나이이다. 반면 `Person copy = person2;`처럼 객체를 복사하면 `copy.setAge(31);`은 복사본만 바꾸고 `person2`의 나이는 25로 남는다.
- **초기화하지 않은 포인터:** `Person* personPtr;`처럼 주소를 대입하지 않은 포인터로 `personPtr->introduce()`를 호출하면 정의되지 않은 동작이다. 포인터는 선언과 동시에 유효한 객체의 주소나 `nullptr`로 초기화하는 것이 안전하다.

### 3.3 실행 결과와 검토

```text
안녕하세요, 제 이름은 김철수이고 20살입니다.
안녕하세요, 제 이름은 엘리자베스이고 25살입니다.
안녕하세요, 제 이름은 엘리자베스이고 31살입니다.
```

둘째 줄과 셋째 줄은 모두 포인터를 통해 `person2`의 `introduce()`를 호출한 결과이며, 그 사이의 `setAge(31)` 호출로 나이만 25에서 31로 바뀌었다.

---

<br>

## 4. Q3. 정적 배열과 동적 배열

### 4.1 요구사항

5주차 강의자료에 실린 동적 메모리의 할당과 반환 코드를 분석한다. 코드를 작성하는 문항이 아니므로 `Solution/Q3.cpp`는 강의자료의 코드이다. 이 코드는 배열의 크기를 입력받아, 크기가 고정된 배열과 입력받은 크기의 동적 배열을 각각 채우고 출력한다.

### 4.2 설계와 해결 방법

답은 이 코드를 정적 배열과 동적 배열의 차이를 보이는 프로그램으로 분석한다. 코드의 흐름은 다음과 같다.

1. `size`에 배열의 크기를 입력받는다.
2. 크기가 4로 고정된 정적 배열 `arrStatic`을 `{ 1, 3, 5 }`로 초기화하고 네 원소를 출력한다. 배열의 크기는 컴파일 시간에 정해져야 하므로 입력받은 `size` 대신 상수 4를 쓴다.
3. `new int[size]`로 힙에 `int` 배열을 `size`개 할당하고 그 주소를 `int*` 포인터 `pDynamic`에 저장한다. 실행 중에 입력받은 값으로 배열의 크기를 정할 수 있다는 점이 정적 배열과 다르다([4장 2.3절](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.ko.md#23-동적-배열)).
4. 첫 번째 `for` 문으로 각 원소에 `i * 3`을 저장하고, 두 번째 `for` 문으로 출력한다.
5. `delete[] pDynamic;`으로 배열 형태로 할당한 메모리를 반환한다. `new`로 할당한 메모리를 반환하지 않으면 메모리 누수가 생기며, `new[]`로 할당한 메모리는 `delete`가 아니라 `delete[]`로 반환해야 한다([4장 2.1절](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.ko.md#21-new와-delete)).
6. 반환한 뒤 `pDynamic`에 `nullptr`를 대입한다. 이미 반환한 메모리를 가리키는 포인터를 **댕글링 포인터(dangling pointer)** 라고 하며, 이를 잊고 사용하면 정의되지 않은 동작이 일어나므로 포인터를 널 포인터로 만들어 둔다([4장 4.2절](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.ko.md#42-댕글링-포인터)).

코드의 세 부분은 다음과 같이 더 정확하게 설명할 수 있다.

- **주석으로 처리한 배열 선언:** 표준 C++에서 배열의 크기는 컴파일 시간에 값이 정해지는 상수식이어야 하므로, 실행 중에 입력받는 `size`로 `int arrStatic[size];`를 선언할 수 없다. 이런 배열을 **가변 길이 배열(variable-length array, VLA)** 이라 하며, C99에서 도입되었지만 C++ 표준에는 없다. GCC와 Clang은 확장 기능으로 받아들이고(Clang은 `-Wvla-cxx-extension` 경고를 낸다), MSVC는 컴파일 오류로 처리한다. 주석 그대로 `int arrStatic[size] = { };`처럼 초기화까지 하면 Clang 21은 `variable-sized object may not be initialized` 오류를 낸다. 강의자료가 덧붙인 대로, 실행 중에 크기를 정하려면 동적 배열이나 `vector`를 쓴다.
- **나머지 원소의 0 초기화:** `int arrStatic[4] = { 1, 3, 5 };`처럼 초기값이 원소 수보다 적으면 나머지 원소는 0으로 초기화되므로 첫 줄은 `1 3 5 0`이다. 반면 `new int[size]`로 할당한 원소는 초기화되지 않으므로 이 코드처럼 값을 저장한 뒤에 읽어야 한다. `new int[size]()`나 C++11의 `new int[size]{}`로 할당하면 모든 원소가 0으로 초기화된다.
- **`nullptr`와 `NULL`:** 주석으로 남긴 `pDynamic = NULL;`과 실제로 쓴 `pDynamic = nullptr;`은 둘 다 포인터를 널 포인터로 만든다. 그러나 `NULL`은 정수 0으로 정의될 수 있는 매크로이므로, `f(int)`와 `f(char*)`가 함께 중복되어 있으면 `f(NULL)`은 모호한 호출이 되어 Clang 21과 GCC 16 모두에서 컴파일 오류가 난다. C++11의 키워드 `nullptr`는 포인터로만 변환되므로 `f(nullptr)`는 포인터 버전을 호출한다. 또한 `nullptr` 대입은 그 포인터만 보호하며, 같은 메모리를 가리키던 다른 포인터는 여전히 댕글링 포인터로 남는다.

### 4.3 실행 결과와 검토

`5`를 입력하면 다음과 같이 출력된다.

```text
Input size: 5
1 3 5 0 
0 3 6 9 12 
```

각 줄 끝에 공백이 하나 남는 것은 수마다 뒤에 `' '`를 출력하기 때문이다. macOS의 `leaks --atExit`로 실행하면 누수가 보고되지 않아, 할당한 배열이 모두 반환되었음을 확인할 수 있다. 0을 입력하면 원소가 없는 배열이 할당되어 둘째 줄은 빈 줄이 되고, 음수를 입력하면 `new int[size]`가 메모리를 할당하지 못해 예외(이 환경에서는 `std::bad_alloc`)가 발생하며, 이를 처리하지 않았으므로 프로그램이 비정상 종료된다.

---

<br>

## 5. Q4. this 포인터를 리턴하는 Counter 클래스

### 5.1 요구사항

`this` 포인터를 사용하는 코드를 자유롭게 작성한다. 풀이는 값을 1씩 늘리고 줄이는 `Counter` 클래스를 정의하고, 멤버 함수가 `this`를 리턴하게 하여 한 문장에서 여러 멤버 함수를 이어서 호출한다.

### 5.2 설계와 해결 방법

입출력을 위해 `<iostream>`을 포함하고, `using namespace std;`로 표준 네임스페이스를 기본으로 쓴다. `Counter` 클래스는 계수기를 표현하며, `private` 멤버 `count`에 현재 값을 저장하고 `public` 아래에 생성자, `increment()`, `decrement()`, `display()`를 둔다.

- **생성자:** 멤버 초기화 리스트 `: count(0)`으로 `count`를 0으로 초기화한다.
- **`increment()`, `decrement()`:** `this->count`를 1 늘리거나 줄인 뒤 `this`, 즉 멤버 함수가 호출된 객체의 주소를 `Counter*`로 리턴한다.
- **`display()`:** `Count: ` 뒤에 현재 값을 출력한다.

`this`는 멤버 함수가 호출된 객체를 가리키는 포인터이다([4장 1.2절](../../Lecture-Notes/Ch04_Pointers-Arrays-and-Dynamic-Memory/Concepts_Lecture.ko.md#12-this-포인터)). `c.increment()`는 `c`의 `count`를 1로 만들고 `&c`를 리턴하므로, `c.increment()->increment()`는 같은 객체 `c`의 `increment()`를 다시 호출한다. 이처럼 멤버 함수가 객체 자신을 돌려주어 호출을 한 문장에 이어 쓰는 방식을 **메서드 체이닝(method chaining)** 이라 한다. `main()`의 첫 문장은 `count`를 0에서 1, 2로 바꾼 뒤 `display()`로 `Count: 2`를 출력하고, 둘째 문장은 1로 줄인 뒤 `Count: 1`을 출력한다. 매개변수와 멤버의 이름이 겹치지 않으므로 이 코드의 `this->count`는 `count`라고만 써도 같은 멤버를 가리킨다.

다음 세 가지는 리턴 타입에 따라 달라지는 성질이다.

- **참조 리턴과의 비교:** `Counter& increment() { count++; return *this; }`처럼 객체 자신을 참조로 리턴하면 `c.increment().increment().display();`처럼 점 연산자로 이어 쓸 수 있고 결과도 같다. 7장의 연산자 중복과 HW5 Q1의 `Accumulator`가 이 형식을 쓴다.
- **값 리턴의 함정:** 리턴 타입을 `Counter`로 하고 `*this`를 리턴하면 호출마다 객체의 복사본이 리턴된다. 이때 `c.increment().increment().display();`는 `Count: 2`를 출력하지만, 두 번째 `increment()`는 복사본에 적용되므로 `c`의 `count`는 1에 머문다. 따라서 이어지는 `c.decrement().display();`는 `Count: 0`을 출력한다.
- **연결의 끝:** `display()`는 `void`를 리턴하므로 연결의 마지막에만 올 수 있다. `c.increment()->display()->increment();`는 `void`에 `->`를 쓴 것이므로 컴파일 오류이다.

### 5.3 실행 결과와 검토

```text
Count: 2
Count: 1
```

출력은 코드의 두 주석에 적은 값과 같다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 객체 배열과 포인터 | 배열 이름은 첫 원소의 주소로 변환되므로 `Complex *p = data;`에서 `p + i`는 `i`번째 원소를 가리킨다. |
| 화살표 연산자 | `p->f()`는 `(*p).f()`와 같으며, `.`가 `*`보다 우선순위가 높으므로 뒤의 형식에는 괄호가 필요하다. |
| 가변 길이 배열 | 표준 C++에서 배열 크기는 상수식이어야 하므로, 실행 중에 크기를 정하려면 `new`로 만든 동적 배열이나 `vector`를 쓴다. |
| 부분 초기화 | 배열의 초기값이 원소 수보다 적으면 나머지 원소는 0으로 초기화된다. |
| `delete[]`와 `nullptr` | `new[]`로 받은 메모리는 `delete[]`로 반환하고, 반환한 뒤 포인터에 `nullptr`를 대입해 댕글링 포인터의 사용을 막는다. |
| `this` 리턴 | 멤버 함수가 `this`를 리턴하면 `->`로, `*this`를 참조로 리턴하면 `.`로 같은 객체에 호출을 이어 쓸 수 있다. |

---

<br>

## 점검 문제

1. **포인터 연산:** Q1에서 `p`가 `data`를 가리킬 때 `(p + 1)->setReal(7);`을 실행하면 어느 원소의 어떤 값이 바뀌는가? 같은 호출을 첨자로 쓰면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `p + 1`은 두 번째 원소 `data[1]`을 가리키므로 `data[1]`의 `real`이 7이 된다. `p[1]`은 `*(p + 1)`과 같은 뜻이므로 `p[1].setReal(7);`이나 `data[1].setReal(7);`로 써도 같은 호출이다.

   </details>

2. **기본 생성자가 없는 경우:** Q1의 `Complex`에서 기본 생성자를 지우면 `Complex data[MAX];`는 어떻게 되는가?

   <details>
   <summary>정답</summary>

   매개변수 2개 생성자가 정의되어 있으므로 컴파일러는 기본 생성자를 만들어 주지 않는다. 초기값 없는 객체 배열은 원소마다 기본 생성자를 호출해야 하므로 컴파일 오류가 난다. `Complex data[MAX] = { Complex(1, 1), Complex(1, 1) };`처럼 원소마다 생성자를 지정하면 컴파일된다.

   </details>

3. **부분 초기화:** Q3에서 `int arrStatic[4] = { 1, 3, 5 };`를 `int arrStatic[4] = { };`로 바꾸면 첫 줄에 무엇이 출력되는가?

   <details>
   <summary>정답</summary>

   초기값이 하나도 없으므로 네 원소가 모두 0으로 초기화되어 `0 0 0 0 `이 출력된다. 반면 `= { }` 없이 `int arrStatic[4];`로 선언한 지역 배열은 초기화되지 않으므로 출력값을 예측할 수 없다.

   </details>

4. **`nullptr` 대입의 범위:** Q3에서 `delete[] pDynamic;` 앞에 `int* q = pDynamic;`을 추가하면, `pDynamic = nullptr;`을 실행한 뒤의 `q`는 어떤 상태인가?

   <details>
   <summary>정답</summary>

   `q`는 여전히 반환된 메모리의 주소를 가지므로 댕글링 포인터이다. `nullptr` 대입은 대입한 포인터 `pDynamic`만 바꾸며, `q`로 원소를 읽거나 다시 `delete[]`하면 정의되지 않은 동작이다.

   </details>

5. **값 리턴:** Q4의 `increment()`와 `decrement()`가 `Counter`를 값으로 리턴하고 `return *this;`를 쓰도록 바꾼 뒤, `main()`을 `c.increment().increment().display();`와 `c.decrement().display();`로 쓰면 출력은 무엇인가?

   <details>
   <summary>정답</summary>

   `Count: 2`와 `Count: 0`이다. 첫 `increment()`만 `c`에 적용되어 `c`의 `count`는 1이 되고, 두 번째 `increment()`와 `display()`는 복사본에 적용되어 2를 출력한다. 이어서 `c.decrement()`가 `c`의 `count`를 0으로 줄인 복사본을 리턴하므로 `Count: 0`이 출력된다.

   </details>
