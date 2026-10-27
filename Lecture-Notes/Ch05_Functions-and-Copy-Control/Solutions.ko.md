# 5장 연습 문제 풀이 — 참조와 복사 생성자

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 5

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 다양한 함수 호출법](#연습-문제-1-다양한-함수-호출법)
- [연습 문제 2: 참조에 의한 호출과 참조 리턴](#연습-문제-2-참조에-의한-호출과-참조-리턴)
- [연습 문제 3: 복사 생성자](#연습-문제-3-복사-생성자)
- [교재 이론 문제](#교재-이론-문제)

---

<br>

## 연습 문제 1: 다양한 함수 호출법

**교재 문항:** 5장 실습 문제 1, 5번

**문제:** 반지름이 10, 5인 `Circle` 객체 `x`, `y`의 반지름을 참조 버전과 포인터 버전의 `mySwap()`으로 차례로 교환하고, `increaseBy(x, y)`로 `x`의 반지름을 `y`의 반지름만큼 늘리도록 `Circle` 클래스와 세 함수를 작성한다. `main()`은 두 군데의 빈칸 외에는 바꿀 수 없다.

### 풀이

`main()`이 `Circle x(10), y(5);`로 객체를 만들고 `x.show()`를 호출하므로, `Circle` 클래스에는 반지름을 받는 생성자와 "반지름이 N인 원"을 출력하는 `show()`가 필요하다. 세 함수가 반지름을 읽고 바꿀 수 있도록 `getRadius()`와 `setRadius()`도 둔다.

두 `mySwap()`은 이름이 같고 매개변수 타입만 다르므로 함수 중복(6장)으로 함께 정의할 수 있다. 첫 번째 빈칸에는 참조 버전을 부르는 `mySwap(x, y);`를, 두 번째 빈칸에는 주소를 넘겨 포인터 버전을 부르는 `mySwap(&x, &y);`를 쓴다. 컴파일러는 인자가 `Circle` 객체인지 `Circle*`인지를 보고 호출할 함수를 고른다. `increaseBy()`도 `x`를 바꿔야 하므로 참조 매개변수로 받는다.

**코드:** [Lab_Q1.cpp](../../Coursework/Lab05_Functions-and-Copy-Control/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C05Q1.cpp
 * @brief   Q1. 다양한 함수 호출법
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int radius) {
        this->radius = radius;
    }
    void show() {
        cout << "반지름이 " << radius << "인 원\n";
    }
    void setRadius(int radius) {
        this->radius = radius;
    }
    int getRadius() {
        return radius;
    }
};

void mySwap(Circle& a, Circle& b) {
    int temp = a.getRadius();
    a.setRadius(b.getRadius());
    b.setRadius(temp);
}

void mySwap(Circle* a, Circle* b) {
    int temp = a->getRadius();
    a->setRadius(b->getRadius());
    b->setRadius(temp);
}

void increaseBy(Circle& a, Circle& b) {
    a.setRadius(a.getRadius() + b.getRadius());
}

int main() {
    Circle x(10), y(5);  // 각각 반지름이 10, 5인 원 객체 생성
    
    mySwap(x, y);        // 참조 버전의 mySwap() 호출
    cout << "x 객체: "; 
    x.show();
    cout << "y 객체: "; 
    y.show();
    cout << endl;
    
    mySwap(&x, &y);      // 포인터 버전의 mySwap() 호출
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

- **`mySwap(Circle& a, Circle& b)`:** `a`, `b`는 `x`, `y`의 별명이므로 `a.setRadius(...)`는 곧 `x`의 반지름을 바꾼다. 반지름 값만 임시 변수 `temp`로 교환하므로 객체를 복사하지 않는다.
- **`mySwap(Circle* a, Circle* b)`:** `x`, `y`의 주소를 받아 `->`로 멤버 함수를 호출한다. 동작은 참조 버전과 같고 호출할 때 `&`를 써야 한다는 점만 다르다.
- **`increaseBy(Circle& a, Circle& b)`:** `a`의 반지름을 `a.getRadius() + b.getRadius()`로 설정한다. 마지막 단계에서 `x`의 반지름은 10, `y`의 반지름은 5이므로 `x`가 15가 된다.

> **참고:** `increaseBy()`는 `b`를 바꾸지 않으므로 `const Circle& b`로 받는 편이 의도를 더 잘 드러낸다. 다만 그러려면 `const` 객체에 대해 호출할 수 있도록 `getRadius()`를 `int getRadius() const`로 선언해야 한다.

**실행 결과:** 첫 교환으로 `x`가 5, `y`가 10이 되고, 두 번째 교환으로 다시 10, 5가 되며, `increaseBy()` 뒤에 `x`가 15가 된다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 프로그램이 끝난 뒤 덧붙이는 메시지이며 프로그램의 출력이 아니다.

---

<br>

## 연습 문제 2: 참조에 의한 호출과 참조 리턴

**교재 문항:** 5장 실습 문제 6번

**문제:** 문자열 `a`에서 문자 `c`를 찾아 그 위치에 대한 참조를 리턴하고, 찾았는지를 참조 매개변수 `success`로 알리는 `char& find(char a[], char c, bool& success)`를 작성한다.

### 풀이

문자열을 앞에서부터 검사하다가 `a[i] == c`인 첫 위치에서 `success`를 `true`로 설정하고 `a[i]`를 리턴한다. 리턴 타입이 `char&`이므로 `return a[i];`는 `a[i]`의 값이 아니라 그 원소 공간에 대한 참조를 리턴하며, `main()`의 `loc1 = 'm';`은 `s[2]`를 직접 바꾼다. 문자열 끝까지 찾지 못하면 `success`를 `false`로 설정한다.

참조를 리턴하는 함수는 찾지 못한 경우에도 무언가에 대한 참조를 리턴해야 한다. 이 풀이는 `a[0]`에 대한 참조를 리턴하며, 호출하는 쪽은 `success`가 `false`이면 리턴된 참조를 쓰지 않아야 한다. 지역 변수에 대한 참조를 리턴하면 댕글링 참조가 되므로([5장 3.3절](Concepts_Lecture.ko.md#33-참조-리턴)), 배열 원소처럼 함수가 끝난 뒤에도 남아 있는 공간을 리턴해야 한다.

**코드:** [Lab_Q2.cpp](../../Coursework/Lab05_Functions-and-Copy-Control/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C05Q2.cpp
 * @brief   Q2. 참조에 의한 호출과 참조 반환
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
#include <cstring>
using namespace std;

char& find(char a[], char c, bool& success) {
    unsigned long size = strlen(a);
    for (int i = 0; i < size; i++) {
        if (a[i] == c) {
            success = true;
            return a[i];
        }
    }
    success = false;
    return a[0];
}

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

- **`strlen(a)`:** 문자열 길이를 구해 널 문자 앞까지만 검사한다. 이 함수를 쓰기 위해 `<cstring>`을 포함한다.
- **첫 번째 호출:** `"Mike"`에서 `'k'`는 인덱스 2에 있으므로 `loc1`이 `s[2]`의 별명이 되고, `loc1 = 'm';`으로 `s`가 `"Mime"`이 된다.
- **두 번째 호출:** `"Mime"`에는 `'k'`가 없으므로 `b`가 `false`가 되고, `main()`은 "k를 발견할 수 없음"을 출력하고 끝난다.

> **참고:** `unsigned long` 타입의 `size`와 `int` 타입의 `i`를 비교하므로 부호가 다른 정수끼리의 비교가 된다. 문자열 길이가 `int` 범위 안이면 결과는 같지만, GCC로 `-Wall` 컴파일하면 `-Wsign-compare` 경고가 나온다(Apple clang의 `-Wall`은 이 경고를 켜지 않는다). `i`를 `size_t`로 선언하면 경고가 사라진다.

---

<br>

## 연습 문제 3: 복사 생성자

**교재 문항:** 5장 실습 문제 11번

**문제:** 동적 할당한 제목 문자열을 가진 `Book` 클래스의 생성자, 소멸자, `set()`, 깊은 복사 생성자를 작성하여, `cpp`로부터 복사 생성한 `java`의 제목과 가격을 바꿔도 `cpp`가 영향을 받지 않게 한다. `set()`은 새 제목이 더 길면 메모리를 반납하고 다시 할당해야 한다.

### 풀이

- **생성자:** 제목 길이에 널 문자 1바이트를 더한 크기로 `new char[]`를 할당하고 `strcpy()`로 제목을 복사한다.
- **복사 생성자 `Book(Book& src)`:** 원본 제목의 길이만큼 새 메모리를 할당하고 내용을 복사하는 깊은 복사이다. 그래서 `java`와 `cpp`는 서로 다른 메모리를 가지며, `java.set()`이 `cpp`의 제목을 바꾸지 않고 두 소멸자가 각자의 메모리를 해제한다.
- **소멸자:** `delete[] title;`로 제목 메모리를 반납한다.
- **`set()`:** 새 제목이 현재 제목보다 길면 기존 메모리를 `delete[]`로 반납하고 새 길이에 맞게 다시 할당한 뒤 복사한다. 짧거나 같으면 기존 공간에 그대로 복사한다.

**코드:** [Lab_Q3.cpp](../../Coursework/Lab05_Functions-and-Copy-Control/Lab_Q3.cpp)

```cpp
/**
 * @file    A10204-1_C05Q3.cpp
 * @brief   Q3. 복사 생성자
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
    char* title;    // 도서 제목 문자열
    int price;      // 도서 가격
public:
    Book(const char* title, int price);
    Book(Book& src);
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};

Book::Book(const char* title, int price) {
    this->price = price;
    this->title = new char [strlen(title) + 1];
    strcpy(this->title, title);
}
Book::Book(Book& src) {
    price = src.price;
    title = new char [strlen(src.title) + 1];
    strcpy(title, src.title);
}
Book::~Book() {
    delete[] title;
    title = nullptr;
}
void Book::set(char* title, int price) {
    this->price = price;
    if (strlen(this->title) < strlen(title)) {
        delete[] this->title;
        this->title = new char[strlen(title) + 1];
    }
    strcpy(this->title, title);
}

int main()
{
    Book cpp("C++가이드", 10000);
    Book java = cpp;  // 복사 생성자 호출
    java.set("Java완전정복", 12000);
    cpp.show();
    java.show();

    return 0;
}
```

소스 파일이 UTF-8이면 `strlen("C++가이드")`는 12, `strlen("Java완전정복")`은 16이다(한글 한 글자가 3바이트). 따라서 `java.set("Java완전정복", 12000)`은 기존 13바이트 공간을 반납하고 17바이트를 새로 할당하는 경로를 거친다. 복사 생성자가 없었다면 디폴트 복사 생성자가 `title` 포인터만 복사하므로, `set()`이 해제한 메모리를 `cpp`가 계속 가리키다가 `cpp.show()`에서 해제된 메모리를 읽고 소멸 시 이중 해제가 일어난다.

> **주의:** `java.set("Java완전정복", 12000);`은 컴파일 경고를 낸다(Apple clang: `-Wwritable-strings`, GCC: "ISO C++ forbids converting a string constant to 'char*'"). C++11부터 문자열 리터럴은 `const char` 배열이므로 `char*` 매개변수에 넘기는 변환을 표준이 허용하지 않기 때문이다. 원인은 문제에 주어진 `void set(char* title, int price)` 선언이며, 매개변수를 `const char*`로 바꾸면 경고가 사라진다.

> **참고:** `Book(Book& src)`처럼 `const`가 없는 참조를 받아도 올바른 복사 생성자이지만, 이 경우 `const Book` 객체나 임시 객체로부터는 복사할 수 없다([5장 5.3절](Concepts_Lecture.ko.md#53-복사-생성자의-매개변수는-참조)). 또 이 클래스에는 대입 연산자가 없으므로 `cpp = java;`처럼 치환하면 얕은 복사 문제가 다시 생긴다. 이 프로그램은 치환을 하지 않으므로 문제가 없지만, 3의 법칙에 따르면 대입 연산자도 함께 정의하는 것이 좋다.

---

<br>

## 교재 이론 문제

**교재:** 5장 연습문제의 이론 문제 중 짝수 번 문항

### 이론 문제 2

**문제:** 값, 주소, 참조, 묵시에 의한 호출 중 일반적으로 함수 호출 비용이 가장 큰 것을 고르라.

**정답:** ①

값에 의한 호출은 실인자의 값을 매개변수에 복사한다. `int` 같은 기본 타입은 복사 비용이 작지만, 객체를 값으로 넘기면 매개변수 객체를 만들기 위해 복사 생성자가 실행되고 함수가 끝날 때 그 객체의 소멸자도 실행된다. 객체가 클수록 멤버를 복사하는 시간과 스택 공간이 늘어난다([5장 2.1절](Concepts_Lecture.ko.md#21-값에-의한-호출로-객체-전달)).

- **② 주소에 의한 호출:** 포인터 값(주소) 하나만 복사하므로 객체의 크기와 관계없이 비용이 일정하다.
- **③ 참조에 의한 호출:** 매개변수가 실인자의 별명이 되므로 객체를 복사하지 않는다. 컴파일러는 보통 주소를 넘기는 방식으로 구현하므로 비용은 주소에 의한 호출과 비슷하다.
- **④ 묵시에 의한 호출:** C++의 인자 전달 방식이 아니다. C++의 인자 전달 방식은 값, 주소, 참조에 의한 호출의 세 가지이다.

### 이론 문제 4

**문제:** `void f(int p[]);`와 `void f(int *p);`, 그리고 `void f(int *p);`와 `void f(int &p);`가 각각 같은 선언인지 판단하라.

**정답:** (1) 두 선언은 같다. (2) 두 선언은 다르다.

(1) 함수의 매개변수를 배열 타입으로 선언하면 컴파일러는 그 타입을 원소에 대한 포인터 타입으로 바꾼다. 따라서 `int p[]`는 `int *p`와 정확히 같은 타입이고, 두 선언은 같은 함수 `void f(int*)`를 선언한다. 두 함수의 본체를 모두 정의하면 함수 중복이 아니라 같은 함수를 두 번 정의한 것이 되어 `redefinition of 'f'` 오류가 난다. `int p[10]`처럼 크기를 적어도 그 크기는 무시된다. 배열 표기 `p[]`는 배열의 시작 주소가 전달된다는 의도를 읽는 사람에게 드러낼 뿐이다.

(2) `int *p`는 포인터 매개변수이므로 `f(&n)`처럼 주소를 넘기는 주소에 의한 호출이고, `int &p`는 참조 매개변수이므로 `f(n)`처럼 변수를 넘기면 `p`가 `n`의 별명이 되는 참조에 의한 호출이다. 전자는 포인터가 가리키는 값을 `*p`로 다루고, 후자는 별명 `p`로 원본을 직접 다룬다([5장 1.4절](Concepts_Lecture.ko.md#14-c에서-c로-포인터-vs-참조)). 두 매개변수의 타입(`int*`와 `int&`)이 다르므로 두 함수는 서로 다른 함수이며, 한 프로그램에 함께 정의하면 함수 중복이 성립한다. 이때 `f(&n)`은 포인터 버전을, `f(n)`은 참조 버전을 호출한다.

### 이론 문제 6

**문제:** 배열의 각 원소를 제곱하는 `square()`를 호출한 뒤 배열을 출력하는 프로그램의 실행 결과를 구하라.

**정답:** `1 4 9`

`square(m, 3)`에서 배열 이름 `m`은 첫 원소의 주소로 변환되어 전달되고, 매개변수 `int n[]`은 이론 문제 4의 (1)처럼 `int *n`과 같다. 따라서 `n[i]`는 `main()`의 배열 `m`의 원소 자체를 가리키며, `square()` 안에서 제곱한 값이 `m`에 그대로 남는다. 이것은 배열을 주소에 의한 호출로 넘긴 것이다. 호출 뒤 `m`은 `{1, 4, 9}`가 되고, 반복문이 각 원소 뒤에 공백 하나를 붙여 출력하므로 실제 출력은 `1 4 9 `(마지막에 공백 포함)이다.

### 이론 문제 8

**문제:** `char c;`, `char *p = &c;`, `char q = c;`, `char &r = c;`가 선언되어 있을 때 `c`에 `'a'`를 기록하지 못하는 문장을 고르라.

**정답:** ②

`char q = c;`는 `c`의 값을 복사하여 초기화한 별개의 변수 `q`를 만든다. 그러므로 `q = 'a';`는 `q`만 바꾸고 `c`에는 영향이 없다.

- **① `c = 'a';`:** `c`에 직접 대입한다.
- **③ `r = 'a';`:** `r`은 `c`에 대한 참조, 즉 `c`의 별명이므로 `r`에 대입하면 `c`가 바뀐다.
- **④ `*p = 'a';`:** `p`는 `c`의 주소를 가지므로 `*p`는 `c` 자체이다.

### 이론 문제 10

**문제:** 전역 배열 `array`의 원소에 대한 참조를 리턴하는 `f()`가 있을 때, 네 문장을 각각 따로 실행한 뒤 `array`의 내용을 구하라.

**정답:**

- (1) `{0, 2, 4, 6, 8, 10, 12, 14, 16, 100}`
- (2) `{0, 4, 6, 8, 10, 12, 14, 16, 18, 18}`
- (3) `{0, 2, 4, 6, 8, 10, 12, 14, 16, 18}` (변하지 않는다)
- (4) `{0, 2, 4, 6, 0, 10, 12, 14, 16, 18}`

`f(n)`은 `array[n]`에 대한 참조를 리턴하므로 호출식 `f(n)` 자체가 `array[n]`의 별명이 되어 대입문의 왼쪽에 올 수 있다. `array`는 전역 배열이어서 함수가 끝난 뒤에도 존재하므로, 지역 변수의 참조를 리턴하는 경우와 달리 안전하다([5장 3.3절](Concepts_Lecture.ko.md#33-참조-리턴)).

- **(1):** `f(9)`는 `array[9]`이므로 `array[9]`에 100이 저장된다.
- **(2):** `i`가 1부터 8까지 변하며 `array[i] = array[i] + 2`를 실행한다. 1~8번 원소만 2씩 커지고 0번 원소(0)와 9번 원소(18)는 그대로이다.
- **(3):** `v`는 참조가 아닌 일반 `int` 변수이므로 `f(0)`이 가리키는 `array[0]`의 값 0을 복사해 받는다. `v = 100;`은 `v`만 바꾼다. `int& v = f(0);`으로 선언했다면 `v`가 `array[0]`의 별명이 되어 `array[0]`이 100이 된다.
- **(4):** 안쪽 `f(2)`가 먼저 실행되어 `array[2]`, 즉 값 4를 내놓고, 이 값이 바깥 `f()`의 매개변수 `n`에 복사된다. 바깥 `f(4)`는 `array[4]`의 참조를 리턴하므로 `array[4]`에 0이 저장된다. 바뀌는 것은 `array[2]`가 아니라 `array[4]`이다.

> **참고:** 이 코드를 `#include <iostream>`, `using namespace std;`와 함께 컴파일하면 구현에 따라 전역 이름 `array`가 `std::array`와 충돌할 수 있다. Apple clang 21(libc++)에서는 `<iostream>`이 `std::array`의 선언을 함께 들여오므로 `reference to 'array' is ambiguous` 오류가 난다. 배열 이름을 바꾸거나 `::array`처럼 전역 범위를 명시하면 해결된다.

### 이론 문제 12

**문제:** 값 매개변수를 받아 참조를 리턴하는 `big1()`과 참조 매개변수를 받아 참조를 리턴하는 `big2()`를 차례로 호출하고 리턴된 참조에 100을 대입할 때 `x`, `y`가 어떻게 변하는지 예측하고 이유를 설명하라.

**정답:** `x`는 1, `y`는 100이 된다. `big1()`이 리턴한 참조를 통한 `z = 100;`은 `x`와 `y`를 바꾸지 못하며 정의되지 않은 동작이고, `big2()`가 리턴한 참조를 통한 `w = 100;`이 `y`를 100으로 바꾼다.

- **`big1(x, y)`:** 값에 의한 호출이므로 `x`, `y`의 값이 지역 매개변수 `a`(1)와 `b`(2)에 복사된다. `a > b`가 거짓이므로 `b`에 대한 참조를 리턴하는데, `b`는 함수가 끝나면 사라지는 지역 변수이다. 따라서 `z`는 이미 소멸한 공간을 가리키는 댕글링 참조가 되고, `z = 100;`은 정의되지 않은 동작이다([5장 5.1절](Concepts_Lecture.ko.md#51-지역-변수에-대한-참조-리턴)). `a`, `b`는 복사본이었으므로 `x`와 `y`는 이 대입으로 바뀌지 않는다. 컴파일러도 이 함수에서 `reference to stack memory associated with parameter 'b' returned` 경고(`a`에 대해서도 같은 경고)를 출력한다.
- **`big2(x, y)`:** 참조에 의한 호출이므로 `a`와 `b`는 `x`와 `y`의 별명이다. `a > b`가 거짓이므로 `b`, 즉 `y`에 대한 참조를 리턴하고, `int& w = big2(x, y);`는 `w`를 `y`의 별명으로 만든다. 그러므로 `w = 100;`을 실행하면 `y`가 100이 된다. `x`는 1 그대로이다.

두 함수의 차이는 리턴하는 참조가 가리키는 대상의 수명이다. 참조를 리턴하려면 그 대상이 함수보다 오래 살아 있어야 하므로, `big2()`처럼 호출자의 변수에 연결된 참조 매개변수를 리턴하는 것은 올바르고 `big1()`처럼 값 매개변수를 참조로 리턴하는 것은 잘못이다. 실행하면 `big1()` 뒤에는 `x=1`, `y=2`, `big2()` 뒤에는 `x=1`, `y=100`이 출력되고 `&w`와 `&y`가 같다.

### 이론 문제 14

**문제:** 클래스 `MyClass`에 대해 네 경우 중 복사 생성자가 필요하지 않은 것을 고르라.

**정답:** ②

`void f(MyClass *p);`는 객체의 주소를 받는 함수이므로 `f(&a)`처럼 호출하면 포인터 값만 복사되고 새 객체는 만들어지지 않는다. 따라서 복사 생성자가 실행되지 않는다([5장 2.2절](Concepts_Lecture.ko.md#22-주소와-참조에-의한-객체-전달)).

- **① `MyClass a = f();`:** `f()`가 객체를 값으로 리턴하면 리턴되는 객체의 복사본이 만들어지고, 그 객체로 `a`를 초기화하므로 복사 생성자가 쓰인다.
- **③ `MyClass b = a;`:** 같은 타입의 객체로 새 객체를 초기화하므로 복사 생성자가 호출된다.
- **④ `MyClass b(a);`:** ③과 같은 초기화를 괄호로 쓴 형태이며 복사 생성자가 호출된다.

> **참고:** ①에서 컴파일러는 리턴 과정의 복사를 생략(copy elision)할 수 있다. C++17부터는 `return MyClass();`처럼 임시 객체를 리턴하고 그 결과로 같은 타입의 객체를 초기화하는 경우 복사 생략이 보장되어, 복사 생성자를 `= delete`로 지운 클래스도 `-std=c++17`에서는 컴파일되고 `-std=c++14`에서는 `call to deleted constructor` 오류가 난다. `MyClass t; return t;`처럼 이름 있는 지역 객체를 리턴하면 생략은 허용될 뿐 보장되지 않으므로 복사(또는 이동) 생성자가 있어야 한다.

### 이론 문제 16

**문제:** 복사 생성자에 대한 설명 중 틀린 것을 고르라. 보기 ①은 복사 생성자의 중복 선언에 관한 설명이다.

**정답:** ①

교재는 복사 생성자의 매개변수가 자기 클래스에 대한 참조 하나이며, 한 클래스에 복사 생성자를 하나만 선언할 수 있다고 설명한다([5장 4.1절](Concepts_Lecture.ko.md#41-개념)). 이 규칙에 따르면 복사 생성자를 중복하여 선언할 수 있다는 보기 ①이 틀린 설명이다.

> **참고:** 교재의 규칙은 단순화한 것이다. 표준 C++에서 복사 생성자는 첫 매개변수가 `X&`, `const X&`, `volatile X&`, `const volatile X&` 중 하나이고 나머지 매개변수가 없거나 모두 디폴트 값을 가진 생성자이다. 따라서 매개변수의 형태가 하나로 고정되어 있지 않으며, `X(X&)`와 `X(const X&)`를 한 클래스에 함께 선언할 수 있다. 이때 `X c(a);`는 `const`가 아닌 객체 `a`에 더 잘 맞는 `X(X&)`를, `X d(b);`는 `const` 객체 `b`를 받을 수 있는 `X(const X&)`를 호출한다. 실제 코드에서는 보통 `const X&` 버전 하나만 둔다.

### 이론 문제 18

**문제:** 포인터 멤버 `pName`, `pId`와 `double` 멤버 `grade`를 가진 `Student` 클래스에 대해 컴파일러가 삽입하는 디폴트 복사 생성자를 쓰라.

**정답:**

```cpp
Student(const Student& s) {   // 교재의 표기로는 Student(Student& s)
    this->pName = s.pName;
    this->pId = s.pId;
    this->grade = s.grade;
}
```

디폴트 복사 생성자는 원본 객체의 멤버를 하나씩 그대로 복사하는 멤버 단위 복사, 즉 얕은 복사를 수행한다([5장 4.2절](Concepts_Lecture.ko.md#42-디폴트-복사-생성자)). 포인터 멤버 `pName`, `pId`는 가리키는 `string` 객체가 아니라 주소만 복사되므로, 원본과 사본이 같은 `string` 객체를 공유한다. 교재는 이 생성자를 `const` 없이 `Student(Student& s)`로 쓰지만, 모든 멤버(포인터와 `double`)를 `const` 객체에서 복사할 수 있으므로 컴파일러가 실제로 선언하는 원형은 `Student(const Student& s)`이다. `const Student` 객체로 다른 `Student`를 초기화해도 컴파일되는 것으로 이를 확인할 수 있다.

또 컴파일러가 만드는 복사 생성자는 본체에서 대입하는 것이 아니라 각 멤버를 원본의 멤버로 직접 초기화한다. 즉 `Student(const Student& s) : pName(s.pName), pId(s.pId), grade(s.grade) {}`처럼 멤버 초기화 리스트를 쓴 것과 같다. 포인터와 `double` 멤버에서는 대입과 초기화의 결과가 같지만, 참조 멤버나 `const` 멤버가 있으면 대입으로는 쓸 수 없으므로 초기화 리스트 형태가 정확한 설명이다.

> **주의:** `Student`가 생성자에서 `new`로 `string`을 할당하고 소멸자에서 `delete`한다면, 디폴트 복사 생성자로 만든 사본과 원본이 소멸할 때 같은 메모리를 두 번 해제한다. 이런 클래스에는 포인터가 가리키는 내용까지 새로 할당하여 복사하는 깊은 복사 생성자를 직접 작성해야 한다([5장 4.3절](Concepts_Lecture.ko.md#43-얕은-복사의-문제)).
