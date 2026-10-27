# 5장 연습 문제 — 참조와 복사 생성자

[레포지토리 안내](../../README.ko.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 다양한 함수 호출법](#연습-문제-1-다양한-함수-호출법)
- [연습 문제 2: 참조에 의한 호출과 참조 반환](#연습-문제-2-참조에-의한-호출과-참조-반환)
- [연습 문제 3: 복사 생성자](#연습-문제-3-복사-생성자)
- [교재 이론 문제](#교재-이론-문제)

---

<br>

## 연습 문제 1: 다양한 함수 호출법

**교재 문항:** 5장 실습 문제 1, 5번

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `mySwap()` 함수를 매개변수가 참조인 버전과 포인터인 버전의 두 가지로 작성하고, `increaseBy()` 함수를 작성하라.

**프로그램 요구사항:**

1. `main()` 함수의 [ 빈칸 ]을 채우되, 다른 내용은 변경할 수 없다.
2. 필요하면 `Circle` 클래스의 생성자를 추가로 정의할 수 있다.

**예시 코드:**

```cpp
#include <iostream>
using namespace std;

int main() {
   Circle x(10), y(5);  // 각각 반지름이 10, 5인 원 객체 생성

   [ 빈칸 ]              // 참조 버전의 mySwap() 호출
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   cout << endl;

   [ 빈칸 ]              // 포인터 버전의 mySwap() 호출
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   cout << endl;

   increaseBy(x, y);    // x 객체를 y 객체만큼 증가
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   
   return 0;
}
```

**실행 예시:**

```text
x 객체: 반지름이 5인 원
y 객체: 반지름이 10인 원

x 객체: 반지름이 10인 원
y 객체: 반지름이 5인 원

x 객체: 반지름이 15인 원
y 객체: 반지름이 5인 원

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-1-다양한-함수-호출법)**

---

<br>

## 연습 문제 2: 참조에 의한 호출과 참조 반환

**교재 문항:** 5장 실습 문제 6번

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `find()` 함수를 작성하라. `find()` 함수의 원형은 다음과 같다.

```cpp
char& find(char a[], char c, bool& success);
```

`find()`는 문자열 `a`에서 문자 `c`를 찾아, 문자 `c`가 있는 공간에 대한 참조를 반환하고 참조 매개변수 `success`를 `true`로 설정한다. 문자 `c`를 찾을 수 없는 경우 참조 매개변수 `success`를 `false`로 설정한다.

**예시 코드:**

```cpp
#include <iostream>
using namespace std;

int main() {
   char s[] = "Mike";
   bool b;

   char& loc1 = find(s, 'k', b);
   if (b == false) {
      cout << "k를 발견할 수 없음" << endl;
      return 0;
   }
   loc1 = 'm';          // 'k' 위치에 'm' 기록
   cout << s << endl;

   char& loc2 = find(s, 'k', b);
   if (b == false) {
      cout << "k를 발견할 수 없음" << endl;
      return 0;
   }
   loc2 = 't';          // 'k' 위치에 't' 기록
   cout << s << endl;

   return 0;
}
```

**실행 예시:**

```text
Mime
k를 발견할 수 없음

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-2-참조에-의한-호출과-참조-반환)**

---

<br>

## 연습 문제 3: 복사 생성자

**교재 문항:** 5장 실습 문제 11번

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `Book` 클래스의 생성자, 소멸자, `set()` 함수와 복사 생성자를 작성하라.

**프로그램 요구사항:**

- `set()` 함수를 구현할 때, 변경되는 책 제목의 길이가 기존보다 길면 기존 동적 할당 영역을 반납하고 새로운 제목의 길이에 맞게 다시 동적 할당을 받도록 작성하라.
- 프로그램 실행 시 오류가 발생하지 않도록 복사 생성자는 깊은 복사로 작성하라.

**예시 코드:**

```cpp
#include <iostream>
using namespace std;

class Book {
   char* title;   // 도서 제목 문자열
   int price;     // 도서 가격
public:
   Book(const char* title, int price);
   ~Book();
   void set(char* title, int price);
   void show() {
      cout << title << ' ' << price << "원" << endl;
   }
};

int main() {
   Book cpp("C++가이드", 10000);
   Book java = cpp;  // 복사 생성자 호출
   java.set("Java완전정복", 12000);
   cpp.show();
   java.show();
   
   return 0;
}
```

**실행 예시:**

```text
C++가이드 10000원
Java완전정복 12000원

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-3-복사-생성자)**

---

<br>

## 교재 이론 문제

**교재:** 5장 연습문제의 이론 문제 중 짝수 번 문항

### 이론 문제 2

일반적으로 함수 호출 시 가장 비용(cost) 부담이 큰 것은?

- ① 값에 의한 호출
- ② 주소에 의한 호출
- ③ 참조에 의한 호출
- ④ 묵시에 의한 호출

**[풀이](Solutions.ko.md#이론-문제-2)**

### 이론 문제 4

다음 두 함수 선언은 같은 것인가?

(1) `void f(int p[]);`와 `void f(int *p);`

(2) `void f(int *p);`와 `void f(int &p);`

**[풀이](Solutions.ko.md#이론-문제-4)**

### 이론 문제 6

다음 프로그램의 실행 결과는 무엇인가?

```cpp
#include <iostream>
#include <string>
using namespace std;

void square(int n[], int size) {
    for(int i = 0; i < size; i++)
        n[i] = n[i] * n[i];
}

int main() {
    int m[3] = {1, 2, 3};
    square(m, 3);
    for(int i = 0; i < 3; i++)
        cout << m[i] << ' ';
}
```

**[풀이](Solutions.ko.md#이론-문제-6)**

### 이론 문제 8

변수 `c`에 문자 `'a'`를 기록하지 못하는 것은?

```cpp
char c;
char *p = &c;
char q = c;
char &r = c;
```

- ① `c = 'a';`
- ② `q = 'a';`
- ③ `r = 'a';`
- ④ `*p = 'a';`

**[풀이](Solutions.ko.md#이론-문제-8)**

### 이론 문제 10

다음의 각 문제가 별도로 실행될 때 `array` 배열은 어떻게 되는가?

```cpp
int array[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
int& f(int n) {
    return array[n];
}
```

(1) `f(9) = 100;`

(2) `for(int i = 1; i < 9; i++) f(i) = f(i) + 2;`

(3) `int v = f(0); v = 100;`

(4) `f(f(2)) = 0;`

**[풀이](Solutions.ko.md#이론-문제-10)**

### 이론 문제 12

비슷하게 생긴 다음 두 함수가 있다.

```cpp
int& big1(int a, int b) {
    if(a > b) return a;
    else return b;
}
int& big2(int& a, int& b) {
    if(a > b) return a;
    else return b;
}
```

다음 코드를 실행하였을 때 `x`, `y`의 값이 어떻게 변하는지 예측하고, 그 이유를 설명하라.

```cpp
int x = 1, y = 2;
int& z = big1(x, y);
z = 100;
int& w = big2(x, y);
w = 100;
```

**[풀이](Solutions.ko.md#이론-문제-12)**

### 이론 문제 14

클래스 `MyClass`가 있다고 할 때, 복사 생성자가 필요한 경우가 아닌 것은?

- ① `MyClass a = f();` (`f()`가 `MyClass` 객체를 리턴하는 경우)
- ② `void f(MyClass *p);`
- ③ `MyClass b = a;` (`a`는 `MyClass` 타입)
- ④ `MyClass b(a);` (`a`는 `MyClass` 타입)

**[풀이](Solutions.ko.md#이론-문제-14)**

### 이론 문제 16

복사 생성자에 대한 설명 중 틀린 것은? 보기 ①은 한 클래스에 복사 생성자를 여러 개 중복하여 선언할 수 있다는 설명이다.

**[풀이](Solutions.ko.md#이론-문제-16)**

### 이론 문제 18

다음 클래스에서 컴파일러가 삽입하는 디폴트 복사 생성자는 무엇인가?

```cpp
class Student {
    string *pName;
    string *pId;
    double grade;
};
```

**[풀이](Solutions.ko.md#이론-문제-18)**
