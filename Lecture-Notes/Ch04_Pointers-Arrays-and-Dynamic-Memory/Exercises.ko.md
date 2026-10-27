# 4장 연습 문제 — 객체 활용

[레포지토리 안내](../../README.ko.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 연습 문제 1: 동적 배열을 가진 클래스

**교재 문항:** 4장 실습 문제 4번

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `Sample` 클래스를 완성하라.

**프로그램 요구사항:**

- `main()` 함수와 `Sample` 클래스에 `try-catch`문을 이용하여 다음 상황에서도 오류가 발생하지 않도록 예외를 처리한다.
   - 동적 할당에 실패하는 경우
   - 사용자로부터 입력받은 정수 배열 크기가 0보다 작거나 같은 경우

**예시 코드:**

```cpp
#include <iostream>
using namespace std;

class Sample {
   int *p = nullptr;
   int size;
public:
   Sample(int n) {     // 생성자
      size = n;
      p = new int [size];
   }
   void read();        // 동적 할당한 정수 배열 p에 사용자로부터 정수 입력
   void write();       // 정수 배열의 내용 출력
   int big();          // 정수 배열에서 가장 큰 수를 반환
   ~Sample();          // 소멸자
};

int main() {
   int input;
    
   cout << "생성할 정수 배열 사이즈를 입력하세요: ";
   cin >> input;
   Sample s(input);     // 크기 input의 정수 배열을 가진 Sample 객체 생성
   s.read();            // 사용자로부터 정수 배열을 입력받아 저장
   s.write();           // 정수 배열의 내용 출력
   cout << "가장 큰 수는 " << s.big() << endl;
    
   return 0;
}
```

**실행 예시:**

```text
생성할 정수 배열 사이즈를 입력하세요: 10
100 4 -2 9 55 300 44 38 99 -500
가장 큰 수는 300

Program ended with exit code: 0
```

```text
생성할 정수 배열 사이즈를 입력하세요: 0
[오류] 사이즈가 너무 작습니다!

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-1-동적-배열을-가진-클래스)**

---

<br>

## 연습 문제 2: 객체의 동적 생성 및 소멸

**교재 문항:** 4장 실습 문제 12번

**문제:** 예시 코드를 참고하여 실행 예시와 동일하게 실행되도록 `Circle`, `CircleManager` 클래스를 완성하고 `main()` 함수를 작성하라.

**프로그램 요구사항:**

- 예시 코드에 주어진 `Circle`, `CircleManager` 클래스 선언부의 내용은 변경할 수 없다.
- `main()` 함수와 `Circle`, `CircleManager` 클래스 선언부에 `try-catch`문을 이용하여 다음 상황에서도 오류가 발생하지 않도록 예외를 처리한다.
   - 동적 할당에 실패하는 경우
   - 사용자로부터 입력받은 원의 개수가 0보다 작거나 같은 경우

**예시 코드:**

```cpp
#include <iostream>
#define PI 3.14
using namespace std;

class Circle {
   int radius;    // 원의 반지름
   string name;   // 원의 이름
public:
   void setCircle(string name, int radius);  // 이름과 반지름 설정
   double getArea();
   string getName();
};

class CircleManager {
   Circle *p;     // Circle 배열에 대한 포인터
   int size;      // 배열의 크기
public:
   CircleManager(int size);   // 사용자로부터 입력받은 size 값 크기의 배열을 동적 생성
   ~CircleManager();
   void searchByName(); // 사용자로부터 원의 이름을 입력받아 면적 출력
   void searchByArea(); // 사용자로부터 면적을 입력받아 면적보다 큰 원의 이름 출력
};
```

**실행 예시:**

```text
원의 개수 >> 4
원 1의 이름과 반지름 >> 빈대떡 10
원 2의 이름과 반지름 >> 도넛 2
원 3의 이름과 반지름 >> 초코파이 1
원 4의 이름과 반지름 >> 피자 15
검색하고자 하는 원의 이름 >> 도넛
도넛의 면적은 12.56
최소 면적을 정수로 입력하세요 >> 10
10보다 큰 원을 검색합니다.
빈대떡의 면적은 314, 도넛의 면적은 12.56, 피자의 면적은 706.5,

Program ended with exit code: 0
```

```text
원의 개수 >> -1
[오류] 사이즈가 너무 작습니다!

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-2-객체의-동적-생성-및-소멸)**

---

<br>

## 교재 이론 문제

**교재:** 4장 연습문제의 이론 문제 중 짝수 번 문항

교재의 이론 문제 1~5번은 폭과 높이로 사각형을 추상화한 다음 `Rect` 클래스를 쓴다.

```cpp
class Rect {
    int width, height;
public:
    Rect(int w, int h) { width = w; height = h; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getArea();
};
int Rect::getArea() {
    return width * height;
}
```

### 이론 문제 2

사용자로부터 폭과 높이를 입력받아 `Rect` 객체를 동적으로 생성하고 면적을 출력하는 코드를 작성하려 한다. 다음 물음에 따라 빈칸 (1)~(3)을 채우라.

```cpp
int main() {
    Rect *q;
    int w, h;
    cin >> w >> h;   // 사각형의 폭과 높이를 w, h에 입력받는다
    // (1)
    // (2)
    // (3)
}
```

- (1) 포인터 변수 `q`에 `w`×`h` 크기의 사각형을 표현하는 `Rect` 객체를 동적으로 생성한다.
- (2) 포인터 `q`를 이용하여 사각형의 면적을 출력한다.
- (3) 생성한 객체를 반환한다.

**[풀이](Solutions.ko.md#이론-문제-2)**

### 이론 문제 4

`Rect` 객체의 배열을 생성하는 다음 코드는 컴파일 오류가 발생한다. 컴파일 오류가 발생하지 않게 하려면 `Rect` 클래스를 어떻게 수정해야 하는가?

```cpp
Rect *p = new Rect[10];
```

**[풀이](Solutions.ko.md#이론-문제-4)**

### 이론 문제 6

`public` 멤버 함수 `getVolume()`을 가진 `Cube` 클래스에 대해 다음 코드가 있다. 이어지는 문장 중 컴파일 오류가 발생하는 것은?

```cpp
Cube c;
Cube *p = &c;
```

- ① `c.getVolume();`
- ② `p->getVolume();`
- ③ `(*p).getVolume();`
- ④ `c->getVolume();`

**[풀이](Solutions.ko.md#이론-문제-6)**

### 이론 문제 8

다음 프로그램이 실행될 때 출력되는 결과는 무엇인가?

```cpp
#include <iostream>
#include <string>
using namespace std;

class Color {
    string c;
public:
    Color() { c = "white"; cout << "기본생성자" << endl; }
    Color(string c) { this->c = c; cout << "매개변수생성자" << endl; }
    ~Color() { cout << "소멸자" << endl; }
};

class Palette {
    Color *p;
public:
    Palette() { p = new Color[3]; }
    ~Palette() { delete [] p; }
};

int main() {
    Palette *p = new Palette();
    delete p;
}
```

**[풀이](Solutions.ko.md#이론-문제-8)**

### 이론 문제 10

다음 코드의 문제점은 무엇인가?

```cpp
Cube *p = new Cube [4];
delete p;
```

**[풀이](Solutions.ko.md#이론-문제-10)**

### 이론 문제 12

`this`의 활용에 대해 잘못 설명한 것은?

- ① `this`는 클래스의 멤버 함수 외의 다른 함수에서는 사용할 수 없다.
- ② `this`는 `static` 멤버 함수에는 사용할 수 없다.
- ③ `this`는 생성자에서 사용할 수 없다.
- ④ 어떤 멤버 함수에서는 `this`를 리턴하기도 한다.

**[풀이](Solutions.ko.md#이론-문제-12)**

### 이론 문제 14

메모리 누수란 어떤 상황에서 발생하는가?

**[풀이](Solutions.ko.md#이론-문제-14)**

### 이론 문제 16

`string` 클래스를 사용하기 위해 필요한 헤더 파일은 무엇인가?

- ① `<string>`
- ② `<string.h>`
- ③ `<cstring>`
- ④ `<iostream>`

**[풀이](Solutions.ko.md#이론-문제-16)**

### 이론 문제 18

문자열을 다루고자 한다. C-스트링과 `string` 클래스에 대한 설명으로 틀린 것은?

- ① C-스트링은 문자의 배열을 이용하여 문자열을 표현한다.
- ② `string` 클래스가 문자열을 객체화하므로 C-스트링보다 사용하기 쉽다.
- ③ `string` 클래스가 좋기는 하지만 C++의 표준이 아니므로 가급적 사용하지 않는 것이 좋다.
- ④ `string` 클래스는 문자열만 다루며, 대문자를 소문자로 변환하는 등 문자를 조작하는 기능은 없다.

**[풀이](Solutions.ko.md#이론-문제-18)**
