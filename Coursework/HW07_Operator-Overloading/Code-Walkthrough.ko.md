# HW7 코드 총정리 — 연산자 중복

[레포지토리 안내](../../README.ko.md) | [English](Code-Walkthrough.md) | [한국어](Code-Walkthrough.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

> **학습 목표:**
> 1. 세 풀이의 클래스 선언, 연산자 함수, `main()`을 줄 단위로 읽고 각 부분의 역할을 설명할 수 있다
> 2. 연산자 식이 실행될 때 호출되는 연산자 함수, 생성자, 소멸자의 순서를 추적할 수 있다
> 3. 연산자 함수의 리턴 타입과 매개변수의 전달 방식이 객체의 상태와 복사에 주는 영향을 코드에서 찾을 수 있다

**문항 출처:** 이종욱 교수, 컴퓨터언어실습Ⅱ (DCCS104-03)

---

## 목차

- [1. 전체 구성](#1-전체-구성)
- [2. Q1.cpp: Book 클래스의 복합 대입 연산자](#2-q1cpp-book-클래스의-복합-대입-연산자)
  - [2.1 전체 코드](#21-전체-코드)
  - [2.2 줄 단위 설명](#22-줄-단위-설명)
  - [2.3 실행 추적](#23-실행-추적)
- [3. Q2.cpp: Book 클래스의 비교 연산자](#3-q2cpp-book-클래스의-비교-연산자)
  - [3.1 전체 코드](#31-전체-코드)
  - [3.2 줄 단위 설명](#32-줄-단위-설명)
  - [3.3 실행 추적](#33-실행-추적)
- [4. Q3.cpp: MyString 클래스](#4-q3cpp-mystring-클래스)
  - [4.1 전체 코드](#41-전체-코드)
  - [4.2 줄 단위 설명](#42-줄-단위-설명)
  - [4.3 실행 추적](#43-실행-추적)
- [요약](#요약)
- [점검 문제](#점검-문제)

---

<br>

## 1. 전체 구성

세 풀이는 모두 하나의 파일 안에 헤더 포함, 클래스 선언, `main()`을 차례로 두며, 멤버 함수는 모두 클래스 안에서 몸체까지 정의한다. 파일 머리의 Doxygen 주석(1~6행)은 파일 이름, 문제, 작성자, 제출일을 적은 것이며 프로그램의 동작과는 관계가 없다. 문제의 요구사항과 설계 판단은 [HW7 과제 해설](Assignment-Explanation.ko.md)에 정리하였다.

| 파일 | 클래스 | 중복한 연산자 | 입력 |
|:-----|:-----|:-----|:-----|
| [`Q1.cpp`](Solution/Q1.cpp) | `Book` | `+=`, `-=` | 없음 |
| [`Q2.cpp`](Solution/Q2.cpp) | `Book` | `==` (매개변수가 `string`인 것과 `Book`인 것) | 없음 |
| [`Q3.cpp`](Solution/Q3.cpp) | `MyString` | `=`, `+`, `<`, `==`, `+=`, `[]` | 없음 |

*표 1. HW7 풀이 파일의 구성*

---

<br>

## 2. Q1.cpp: Book 클래스의 복합 대입 연산자

### 2.1 전체 코드

```cpp
/**
 * @file    A7Q1.cpp
 * @brief   Q1. 멤버 함수로 중복한 +=, -= 연산자 (12주차 실습 2-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
using namespace std;

class Book {
    string title;
    int price;
    int pages;
public:
    Book (string title = "", int price = 0, int pages = 0) {
        this->title = title;
        this->price = price;
        this->pages = pages;
    }
    void show() {
        cout << title << ' ' << price << "원 " << pages << "페이지" << endl;
    }
    
    string getTitle() { return title; }
    int getPrice() { return price; }
    void setPrice(int p) { price = p; }
    Book operator += (int op) {
        this->price += op;
        return *this;
    }
    Book operator -= (int op) {
        this->price -= op;
        return *this;
    }
};

int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    a += 500;
    b -= 500;
    
    a.show();
    b.show();
}
```

### 2.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~9 | `#include <iostream>`, `using namespace std;` | `cout`, `endl`, `string`을 쓰기 위해 `<iostream>`을 포함하고 `std::`를 생략한다. `<string>`은 포함하지 않는다 |
| 11~14 | `class Book { string title; int price; int pages;` | 접근 지정자 없이 선언한 세 멤버 변수는 `class`의 기본 접근 지정에 따라 `private`이다 |
| 15 | `public:` | 이후의 멤버를 `public`으로 선언한다 |
| 16~20 | `Book (string title = "", int price = 0, int pages = 0)` | 디폴트 매개변수를 가진 생성자이다. 매개변수가 멤버 변수와 이름이 같으므로 `this->`로 멤버 변수를 가리켜 대입한다 |
| 21~23 | `void show()` | 제목, 공백, 가격, `"원 "`, 쪽수, `"페이지"`를 차례로 출력하고 줄을 바꾼다 |
| 25~27 | `getTitle()`, `getPrice()`, `setPrice()` | 클래스 안에서 정의한 get/set 함수이며, 이 `main()`에서는 호출되지 않는다 |
| 28~31 | `Book operator += (int op)` | `price`에 `op`를 더하고 `*this`를 리턴한다. 리턴 타입이 `Book`이므로 객체의 복사본이 리턴된다 |
| 32~35 | `Book operator -= (int op)` | `price`에서 `op`를 빼고 `*this`의 복사본을 리턴한다 |
| 39 | `Book a("청춘", 20000, 300), b("미래", 30000, 500);` | 매개변수 3개를 모두 지정하여 두 책을 만든다 |
| 40 | `a += 500;` | `a.operator+=(500)`이 호출되어 `a.price`가 20500이 된다. 리턴된 복사본은 쓰이지 않고 문장이 끝날 때 소멸한다 |
| 41 | `b -= 500;` | `b.operator-=(500)`이 호출되어 `b.price`가 29500이 된다 |
| 43~44 | `a.show();`, `b.show();` | 바뀐 가격으로 두 책의 정보를 한 줄씩 출력한다 |

*표 2. Q1.cpp의 줄 단위 설명*

### 2.3 실행 추적

| 실행한 문장 | `a.price` | `b.price` | 출력 |
|:-----|-----:|-----:|:-----|
| `Book a(...), b(...);` | 20000 | 30000 | |
| `a += 500;` | 20500 | 30000 | (리턴된 복사본은 바로 소멸) |
| `b -= 500;` | 20500 | 29500 | (리턴된 복사본은 바로 소멸) |
| `a.show();` | 20500 | 29500 | `청춘 20500원 300페이지` |
| `b.show();` | 20500 | 29500 | `미래 29500원 500페이지` |

*표 3. Q1의 `main()` 실행에 따른 두 책의 가격*

`title`과 `pages`는 생성자에서 정해진 뒤 바뀌지 않는다. 두 연산자 함수의 리턴값을 쓰지 않으므로 이 `main()`에서는 복사본을 리턴하는 것과 참조를 리턴하는 것의 차이가 출력에 나타나지 않는다.

---

<br>

## 3. Q2.cpp: Book 클래스의 비교 연산자

### 3.1 전체 코드

```cpp
/**
 * @file    A7Q2.cpp
 * @brief   Q2. 두 가지 == 연산자를 갖춘 Book 클래스 (12주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
using namespace std;

class Book {
    string title;
    int price;
    int pages;
public:
    Book(string title = "", int price = 0, int pages = 0) {
        this->title = title;
        this->price = price;
        this->pages = pages;
    }
    void show() {
        cout << title << ' ' << price << "원 " << pages << "페이지" << endl;
    }
    string getTitle() {
        return title;
    }
    bool operator == (string op) {
        if (this->title == op)
            return true;
        else
            return false;
    }
    bool operator == (Book op) {
        if (this->title == op.title)
            return true;
        else
            return false;
    }
};

int main() {
    Book a("명품 C++", 30000, 500), b("고품 C++", 30000, 500);
    
    // 책 title 비교
    if (a == "명품 C++")
        cout << "명품 C++ 입니다." << endl;
    
    // title, price, pages 모두 비교
    if (a == b)
        cout << "두 책이 같은 책입니다." << endl;
    else
        cout << "두 책은 다른 책입니다." << endl;
    
    return 0;
}
```

### 3.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~9 | `#include <iostream>`, `using namespace std;` | Q1과 같다 |
| 11~20 | 멤버 변수와 생성자 | Q1과 같은 `private` 멤버 변수 세 개와 디폴트 매개변수를 가진 생성자이다 |
| 21~23 | `void show()` | Q1과 같은 출력 함수이며, 이 `main()`에서는 호출되지 않는다 |
| 24~26 | `string getTitle()` | 제목을 리턴하며, 이 `main()`에서는 호출되지 않는다 |
| 27~32 | `bool operator == (string op)` | 문자열 `op`를 값으로 받아 `this->title`과 같으면 `true`, 다르면 `false`를 리턴한다 |
| 33~38 | `bool operator == (Book op)` | 다른 책 `op`를 값으로 받아 제목만 비교한다. 가격과 쪽수는 비교하지 않는다 |
| 42 | `Book a("명품 C++", 30000, 500), b("고품 C++", 30000, 500);` | 제목만 다르고 가격과 쪽수가 같은 두 책을 만든다 |
| 44~46 | `if (a == "명품 C++")` | 문자열 리터럴로 임시 `string`이 만들어져 `a.operator==(string)`에 전달된다. 결과가 `true`이므로 46행이 출력된다 |
| 48~52 | `if (a == b) ... else ...` | `b`를 복사한 `op`로 `a.operator==(Book)`이 호출된다. 제목이 달라 `false`이므로 `else`의 52행이 출력된다 |
| 54 | `return 0;` | 종료 코드 0을 반환한다 |

*표 4. Q2.cpp의 줄 단위 설명*

### 3.3 실행 추적

| 조건식 | 호출되는 함수 | 비교하는 값 | 결과 | 출력 |
|:-----|:-----|:-----|:-----|:-----|
| `a == "명품 C++"` | `operator==(string)` | `"명품 C++"`과 `"명품 C++"` | `true` | `명품 C++ 입니다.` |
| `a == b` | `operator==(Book)` | `"명품 C++"`과 `"고품 C++"` | `false` | `두 책은 다른 책입니다.` |

*표 5. Q2의 두 조건식의 평가*

두 함수 모두 매개변수를 값으로 받으므로, 첫 번째 조건식에서는 `string` 매개변수 `op`가, 두 번째 조건식에서는 `b`를 복사한 `Book` 매개변수 `op`가 만들어지고 함수가 끝날 때 소멸한다. `Book`의 복사에는 컴파일러가 만든 기본 복사 생성자가 쓰이며, 멤버 `title`은 `string`의 복사 생성자로 복사된다.

---

<br>

## 4. Q3.cpp: MyString 클래스

### 4.1 전체 코드

```cpp
/**
 * @file    A7Q3.cpp
 * @brief   Q3. 연산자 중복 관련 자유 코드 (MyString 클래스)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
#include <cstring>
using namespace std;

class MyString {
    char* str;
    int len;
public:
    // 생성자
    MyString(const char* s = "") {
        len = strlen(s);
        str = new char[len + 1];
        strcpy(str, s);
    }
    
    // 복사 생성자
    MyString(const MyString& s) {
        len = s.len;
        str = new char[len + 1];
        strcpy(str, s.str);
    }
    
    // 소멸자
    ~MyString() {
        if(str) delete[] str;
    }
    
    // = 연산자 중복
    MyString& operator=(const MyString& s) {
        if(this == &s) return *this;
        if(str) delete[] str;
        len = s.len;
        str = new char[len + 1];
        strcpy(str, s.str);
        return *this;
    }
    
    // + 연산자 중복 (문자열 연결)
    MyString operator+(const MyString& s) {
        char* temp = new char[len + s.len + 1];
        strcpy(temp, str);
        strcat(temp, s.str);
        MyString result(temp);
        delete[] temp;
        return result;
    }
    
    // < 연산자 중복 (사전식 비교)
    bool operator<(const MyString& s) {
        return strcmp(str, s.str) < 0;
    }
    
    // == 연산자 중복
    bool operator==(const MyString& s) {
        return strcmp(str, s.str) == 0;
    }

    // += 연산자 중복
    MyString& operator+=(const MyString& s) {
        char* temp = new char[len + s.len + 1];
        strcpy(temp, str);
        strcat(temp, s.str);
        if(str) delete[] str;
        str = temp;
        len = len + s.len;
        return *this;
    }
    
    // [] 연산자 중복 (인덱스로 문자 접근)
    char& operator[](int index) {
        if(index < 0 || index >= len) {
            cout << "Index out of range" << endl;
            exit(1);
        }
        return str[index];
    }
    
    string show() {
        return str;
    }
};

int main() {
    MyString s1("Hello ");
    MyString s2("World!");
    MyString s3;
    
    cout << "[초기 상태]" << endl;
    cout << "s1: " << s1.show() << endl;
    cout << "s2: " << s2.show() << endl;
    cout << "s3: " << s3.show() << endl << endl;
    
    cout << "[연산 수행]" << endl;
    // + 연산자 사용
    s3 = s1 + s2;
    cout << "s1 + s2 = " << s3.show() << endl;
    
    // += 연산자 사용
    s1 += "C++";
    cout << "s1 += 'C++' = " << s1.show() << endl;
    
    // < 연산자 사용 (사전식 비교)
    MyString a("Apple");
    MyString b("Banana");
    if(a < b)
        cout << a.show() << "은 " << b.show() << "보다 사전순으로 앞에 있습니다." << endl;
    
    // [] 연산자 사용
    cout << "s2의 첫 번째 문자: " << s2[0] << endl;
    
    // == 연산자 사용
    MyString str1("Test");
    MyString str2("Test");
    if(str1 == str2)
        cout << "두 문자열이 같습니다." << endl;
    
    return 0;
}
```

### 4.2 줄 단위 설명

| 줄 | 코드 | 설명 |
|:-----|:-----|:-----|
| 8~10 | `#include`, `using namespace std;` | `<iostream>`은 입출력을, `<cstring>`은 `strlen()`, `strcpy()`, `strcat()`, `strcmp()`를 제공한다 |
| 12~14 | `class MyString { char* str; int len;` | 동적 배열을 가리키는 포인터와 널 문자를 뺀 문자열의 길이이며, 둘 다 `private`이다 |
| 17~21 | `MyString(const char* s = "")` | `len`을 구하고 `len + 1`개의 `char`를 할당하여 `s`를 복사한다. 인자 없이 만들면 빈 문자열이 된다 |
| 24~28 | `MyString(const MyString& s)` | `s`의 길이만큼 새 배열을 할당하여 내용을 복사하는 깊은 복사 생성자이다 |
| 31~33 | `~MyString()` | `str`이 가리키는 배열을 `delete[]`로 해제한다 |
| 36~37 | `operator=`의 자기 대입 검사 | `this`와 `&s`가 같으면 아무것도 하지 않고 `*this`를 리턴한다 |
| 38~42 | `operator=`의 몸체 | 기존 배열을 해제하고, 새 배열을 할당하여 `s`의 문자열을 복사한 뒤 `*this`의 참조를 리턴한다 |
| 46~49 | `operator+`의 임시 배열 | 두 길이의 합보다 1 큰 배열에 `strcpy()`로 자신의 문자열을, `strcat()`으로 `s`의 문자열을 이어서 넣는다 |
| 50~52 | `MyString result(temp); delete[] temp; return result;` | 임시 배열로 결과 객체를 만들고 임시 배열을 해제한 뒤 결과를 값으로 리턴한다 |
| 56~58 | `bool operator<(const MyString& s)` | `strcmp()`의 결과가 음수이면, 즉 자신의 문자열이 사전식으로 앞이면 `true`이다 |
| 61~63 | `bool operator==(const MyString& s)` | `strcmp()`의 결과가 0이면, 즉 두 문자열이 같으면 `true`이다 |
| 66~69 | `operator+=`의 임시 배열 | `operator+`와 같은 방법으로 이은 문자열을 새 배열 `temp`에 만든다 |
| 70~73 | `if(str) delete[] str; str = temp; len = len + s.len; return *this;` | 기존 배열을 해제하고 새 배열로 바꾼 뒤 길이를 갱신하고 `*this`의 참조를 리턴한다 |
| 77~81 | `operator[]`의 범위 검사 | 인덱스가 0보다 작거나 `len` 이상이면 메시지를 출력하고 `exit(1)`로 종료한다 |
| 82 | `return str[index];` | 범위 안의 문자에 대한 참조를 리턴하므로 읽기와 쓰기에 모두 쓸 수 있다 |
| 85~87 | `string show()` | `char*`인 `str`로 `std::string`을 만들어 리턴한다 |
| 91~93 | `s1`, `s2`, `s3`의 생성 | `"Hello "`, `"World!"`, 디폴트 인자 `""`로 생성자가 세 번 호출된다 |
| 95~98 | 초기 상태 출력 | 세 문자열을 출력하고, 98행의 `endl << endl`로 빈 줄을 하나 둔다 |
| 100 | `cout << "[연산 수행]" << endl;` | 연산 부분의 제목 줄을 출력한다 |
| 102~103 | `s3 = s1 + s2;` | `s1.operator+(s2)`가 만든 임시 객체를 `s3.operator=()`로 대입하고 결과를 출력한다 |
| 106~107 | `s1 += "C++";` | 변환 생성자로 만든 임시 `MyString`을 `s1.operator+=()`에 넘겨 `s1`을 `"Hello C++"`로 바꾼다 |
| 110~113 | `a < b` | `"Apple"`이 `"Banana"`보다 사전식으로 앞이므로 113행이 출력된다 |
| 116 | `s2[0]` | `s2.operator[](0)`이 첫 문자 `'W'`의 참조를 리턴한다 |
| 119~122 | `str1 == str2` | 두 객체는 서로 다른 배열을 가리키지만 내용이 같으므로 `true`이다 |
| 124 | `return 0;` | `main()`이 끝나면서 지역 객체 일곱 개의 소멸자가 만들어진 순서의 반대로 호출된다 |

*표 6. Q3.cpp의 줄 단위 설명*

### 4.3 실행 추적

| 실행한 문장 | 호출되는 함수 (순서대로) | 결과 |
|:-----|:-----|:-----|
| 91~93행 | 생성자 3번 | `s1`은 `"Hello "`(길이 6), `s2`는 `"World!"`(길이 6), `s3`은 `""`(길이 0) |
| `s3 = s1 + s2;` | `s1.operator+(s2)`, 그 안의 생성자(`result`), `s3.operator=(임시 객체)`, 임시 객체의 소멸자 | `s3`은 `"Hello World!"`(길이 12) |
| `s1 += "C++";` | 생성자(임시 객체 `"C++"`), `s1.operator+=(임시 객체)`, 임시 객체의 소멸자 | `s1`은 `"Hello C++"`(길이 9) |
| `a < b` | 생성자 2번(110~111행), `a.operator<(b)` | `strcmp("Apple", "Banana")`가 음수이므로 `true` |
| `s2[0]` | `s2.operator[](0)` | `'W'` |
| `str1 == str2` | 생성자 2번(119~120행), `str1.operator==(str2)` | `strcmp("Test", "Test")`가 0이므로 `true` |
| `return 0;` 뒤 | 소멸자 7번: `str2`, `str1`, `b`, `a`, `s3`, `s2`, `s1`의 순서 | 할당한 배열이 모두 해제된다 |

*표 7. Q3의 `main()` 실행에 따른 함수 호출*

`s3 = s1 + s2;`에서 `operator+`는 지역 객체 `result`를 값으로 리턴한다. Apple clang은 이름 있는 반환값 최적화(NRVO)로 `result`를 리턴값이 놓일 자리에 직접 만들어 복사를 생략하므로, 위 표처럼 복사 생성자가 호출되지 않는다. 복사 생략을 끄는 `-fno-elide-constructors` 옵션을 주고 `-std=c++11`로 컴파일하면 `result`를 임시 객체로 복사하는 복사 생성자와 `result`의 소멸자가 한 번씩 더 호출된다. 어느 경우에나 출력은 같다.

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 파일의 구성 | 세 풀이는 헤더 포함, 멤버 함수를 모두 클래스 안에서 정의한 클래스 선언, `main()`의 순서로 되어 있다. |
| 리턴 타입 읽기 | Q1의 `Book operator+=`는 객체의 복사본을, Q3의 `MyString& operator+=`는 객체 자신의 참조를 리턴한다. |
| 값 매개변수 | Q2의 `operator==(Book op)`는 호출될 때마다 `b`를 복사한 `op`를 만들고 함수가 끝날 때 소멸시킨다. |
| 임시 객체의 수명 | `s3 = s1 + s2`와 `s1 += "C++"`에서 만들어진 임시 `MyString`은 문장이 끝날 때 소멸자로 배열을 해제한다. |
| 소멸 순서 | 지역 객체는 만들어진 순서의 반대로 소멸하므로 Q3에서는 `str2`부터 `s1`까지 차례로 배열이 해제된다. |

---

<br>

## 점검 문제

1. **버려지는 리턴값:** Q1의 40행 `a += 500;`이 리턴한 객체는 어떻게 되며, `a`의 가격에 영향을 주는가?

   <details>
   <summary>정답</summary>

   리턴 타입이 `Book`이므로 `*this`를 복사한 임시 객체가 만들어지고, 쓰이지 않은 채 문장이 끝날 때 소멸한다. `a.price`는 함수 안의 `this->price += op;`에서 이미 바뀌었으므로, 임시 객체가 소멸해도 `a`의 가격은 20500으로 남는다.

   </details>

2. **매개변수의 복사:** Q2의 49행 `a == b`가 실행될 때 `b`는 복사되는가? 복사되지 않게 하려면 33행을 어떻게 고치는가?

   <details>
   <summary>정답</summary>

   복사된다. `operator==(Book op)`의 `op`는 값 매개변수이므로 기본 복사 생성자로 `b`를 복사하여 만들어진다. 33행을 `bool operator == (const Book& op)`로 고치면 `op`가 `b`를 가리키는 참조가 되어 복사가 일어나지 않는다.

   </details>

3. **길이와 배열 크기:** Q3의 106행 `s1 += "C++";`이 끝난 뒤 `s1.len`의 값과 `s1.str`이 가리키는 배열의 크기는 얼마인가?

   <details>
   <summary>정답</summary>

   `s1.len`은 6 + 3 = 9이다. 67행에서 `len + s.len + 1`, 즉 10개의 `char`를 할당했으므로 배열의 크기는 10이며, 마지막 칸에 널 문자가 들어 있다.

   </details>

4. **범위 밖의 인덱스:** Q3의 116행을 `cout << "s2의 첫 번째 문자: " << s2[6] << endl;`로 바꾸면 어떻게 되는가?

   <details>
   <summary>정답</summary>

   `s2.len`이 6이므로 인덱스 6은 78행의 `index >= len`에 걸린다. C++17부터는 `<<`의 왼쪽 피연산자가 먼저 평가되므로 `s2의 첫 번째 문자: `가 먼저 출력되고, 이어서 같은 줄에 `Index out of range`가 출력된 뒤 `exit(1)`로 프로그램이 종료 코드 1로 끝난다. 인덱스 6에는 널 문자가 있지만, 범위 검사는 문자열의 문자만 허용한다. 117행 이후는 실행되지 않는다.

   </details>
