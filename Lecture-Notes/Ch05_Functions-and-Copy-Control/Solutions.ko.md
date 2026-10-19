# 5장 연습 문제 풀이 — 참조와 복사 생성자

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 5

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 다양한 함수 호출법](#연습-문제-1-다양한-함수-호출법)
- [연습 문제 2: 참조에 의한 호출과 참조 반환](#연습-문제-2-참조에-의한-호출과-참조-반환)
- [연습 문제 3: 복사 생성자](#연습-문제-3-복사-생성자)

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

**실행 결과:** 출력은 실행 예시와 같다. 첫 교환으로 `x`가 5, `y`가 10이 되고, 두 번째 교환으로 다시 10, 5가 되며, `increaseBy()` 뒤에 `x`가 15가 된다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 프로그램이 끝난 뒤 덧붙이는 메시지이며 프로그램의 출력이 아니다.

---

<br>

## 연습 문제 2: 참조에 의한 호출과 참조 반환

**교재 문항:** 5장 실습 문제 6번

**문제:** 문자열 `a`에서 문자 `c`를 찾아 그 위치에 대한 참조를 반환하고, 찾았는지를 참조 매개변수 `success`로 알리는 `char& find(char a[], char c, bool& success)`를 작성한다.

### 풀이

문자열을 앞에서부터 검사하다가 `a[i] == c`인 첫 위치에서 `success`를 `true`로 설정하고 `a[i]`를 반환한다. 반환 타입이 `char&`이므로 `return a[i];`는 `a[i]`의 값이 아니라 그 원소 공간에 대한 참조를 반환하며, `main()`의 `loc1 = 'm';`은 `s[2]`를 직접 바꾼다. 문자열 끝까지 찾지 못하면 `success`를 `false`로 설정한다.

참조를 반환하는 함수는 찾지 못한 경우에도 무언가에 대한 참조를 반환해야 한다. 이 풀이는 `a[0]`에 대한 참조를 반환하며, 호출하는 쪽은 `success`가 `false`이면 반환된 참조를 쓰지 않아야 한다. 지역 변수에 대한 참조를 반환하면 댕글링 참조가 되므로([5장 3.3절](Concepts_Lecture.ko.md#33-참조-반환)), 배열 원소처럼 함수가 끝난 뒤에도 남아 있는 공간을 반환해야 한다.

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

**실행 결과:** 출력은 실행 예시와 같다(`Program ended with exit code: 0`은 Xcode의 메시지이다).

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

**실행 결과:** 출력은 실행 예시와 같다(`Program ended with exit code: 0`은 Xcode의 메시지이다).
