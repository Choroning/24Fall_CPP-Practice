# 2장 연습 문제 — C++ 프로그래밍의 기본

[레포지토리 안내](../../README.ko.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 2

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 교재 이론 문제

**교재:** 2장 연습문제의 이론 문제 중 짝수 번 문항

### 이론 문제 2

C++에서 `main()` 함수에 대한 설명 중 틀린 것은?

- ① C++ 표준에서 정한 `main()` 함수의 리턴 타입은 `int`이다.
- ② `void main()`으로 작성해도 대부분의 컴파일러에서는 처리된다.
- ③ `main()` 함수는 반드시 `return` 문을 가지고 있어야 한다.
- ④ `main()` 함수는 반드시 정수 0을 리턴할 필요가 없다.

**[풀이](Solutions.ko.md#이론-문제-2)**

### 이론 문제 4

다음 코드는 C 컴파일러로 컴파일하면 컴파일 오류가 발생하지만 C++ 컴파일러로 컴파일하면 정상적으로 컴파일된다.

```cpp
int a;
a = 4;
int square = a * a;
```

- (1) C 컴파일러로 컴파일할 때 어떤 컴파일 오류가 발생하는가?
- (2) C++ 컴파일러로 컴파일할 때 정상적으로 컴파일되는 것은 C++ 언어의 어떤 특성 때문인가?
- (3) 이 특징이 가진 장단점은 무엇인가?

**[풀이](Solutions.ko.md#이론-문제-4)**

### 이론 문제 6

다음 프로그램에 컴파일 오류가 발생하지 않도록 빈칸을 채워라.

(1)

```cpp
#include <iostream>
______________________
int main() {
   int count = 0;
   std::cin >> count;
   cout << count + 1;
   return 0;
}
```

(2)

```cpp
#include <iostream>
______________________
int main() {
   cout << "I love C++" << endl;
   cout << "I love programming";
   return 0;
}
```

**[풀이](Solutions.ko.md#이론-문제-6)**

### 이론 문제 8

다음 C++ 프로그램 코드에서 틀린 부분이 있으면 수정하라.

- (1) `using std::cin;`
- (2) `int year = 1; // year은 년도를 나타낸다.`
- (3) `int n=1; cout >> n+200;`
- (4) `int year = 2014; cout << 2014+"년";`

**[풀이](Solutions.ko.md#이론-문제-8)**

### 이론 문제 10

C++ 문자열에 대한 다음 질문에 O, X로 답하라.

- (1) C 스트링이란 C 언어에서 문자열을 다루는 방식이다. ( )
- (2) C++에서 C 스트링 방식의 문자열이 사용된다. ( )
- (3) C++에서는 문자열을 다루기 위해 `string` 클래스가 이용된다. ( )
- (4) `char name[]="C++";`이 컴파일되면 `name[]` 배열의 크기가 3이 된다. ( )
- (5) `char name[10]; cin >> name;`을 실행하면 공백 문자를 포함하여 키보드로부터 최대 9개의 문자를 읽을 수 있다. ( )

**[풀이](Solutions.ko.md#이론-문제-10)**

### 이론 문제 12

다음 프로그램이 있다.

```cpp
#include <iostream>
int main() {
   char name[20];
   std::cout << "이름을 입력하세요?";
   std::cin >> name;
   std::cout << name << "님 환영합니다";
   return 0;
}
```

(1) 프로그램을 실행하고 다음과 같이 키보드로 Kitae를 입력한 결과는 무엇인가?

```text
이름을 입력하세요?Kitae
```

(2) 프로그램을 실행하고 다음과 같이 키보드로 Kitae Hwang을 입력한 결과는 무엇인가?

```text
이름을 입력하세요?Kitae Hwang
```

**[풀이](Solutions.ko.md#이론-문제-12)**

### 이론 문제 14

`char buf[100];`이 선언되어 있다고 가정하고, 다음과 같이 `<Enter>` 키가 입력될 때까지 문자열을 읽는 코드로 잘못된 것은 무엇인가?

```text
I love C++<Enter>
```

- ① `cin >> buf;`
- ② `cin.getline(buf, 11);`
- ③ `cin.getline(buf, 20, '\n');`
- ④ `cin.getline(buf, 11, '.');`

**[풀이](Solutions.ko.md#이론-문제-14)**

### 이론 문제 16

C++ 표준 라이브러리가 모두 선언된 이름 공간은 무엇인가?

**[풀이](Solutions.ko.md#이론-문제-16)**

### 이론 문제 18

C++ 표준에서 `cin`, `cout` 객체는 어떤 헤더 파일에 선언되어 있는가?

**[풀이](Solutions.ko.md#이론-문제-18)**

### 이론 문제 20

다음 출력 결과와 같은 코드를 작성하고자 한다. 다음 C++ 프로그램을 완성하라.

```cpp
#include <iostream>
using namespace std;
int main() {
   for(int n=0; n<4; n++) {
      ______________________
      ______________________
      ______________________
   }
}
```

```text
*
**
***
****
```

**[풀이](Solutions.ko.md#이론-문제-20)**
