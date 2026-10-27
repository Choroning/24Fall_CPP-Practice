# HW7 과제 안내문 — 연산자 중복

[레포지토리 안내](../../../README.ko.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **최종 수정일:** 2026-10-27

컴퓨터언어실습Ⅱ 3분반의 7차 과제이다. 12주차 강의자료의 실습 문제 두 개와 연산자 중복에 관한 자유 코드 문제 하나로 이루어진다.

- **제출 기한:** 2024-11-25 (월) 13:00, 수업 시작 전까지이다. 기한이 지나면 과제가 자동으로 비공개된다.
- **제출 방법:** 과목이 배포한 과제 제출 양식(.hwp)에 작성하여 블랙보드의 과제란에 제출한다.
- **작성 내용:** 코드 문제는 코드와 함께 반드시 코드 분석 내용을 작성해야 한다.

---

<br>

## 문제 1. 12주차 강의자료 [실습 2-2]

[실습 2]에 쓰이는 `Book` 클래스는 책의 제목, 가격, 쪽수를 멤버 변수로 가지며 다음과 같이 정의된다.

```cpp
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
        cout << title << ' ' << price << "원 " << pages << " 페이지" << endl;
    }

    string getTitle() { return title; }

    int getPrice(void) { return price; }

    void setPrice(int p) { price = p; }
};
```

다음 `main()` 함수가 실행되도록 `Book` 클래스에 기능을 추가하라. `-=`와 `+=` 연산자 함수를 `Book` 클래스의 멤버 함수로 구현하며, `c = a += 500;`과 같은 코드도 동작하도록 고려해야 한다.

```cpp
int main() {
    Book a("청춘", 20000, 300), b("미래", 30000, 500);
    a += 500; // 책 a의 가격 500원 증가
    b -= 500; // 책 b의 가격 500원 감소

    a.show();
    b.show();
}
```

**실행 예시**

```text
청춘 20500원 300 페이지
미래 29500원 500 페이지
```

이 문제는 앞의 [실습 2-1]을 이어받는다. [실습 2-1]은 같은 `Book` 클래스에 `Sub(a, 500);`과 `Sub(b, 1000);`으로 두 책의 가격을 줄이는 `main()`을 주고, `Sub()`를 일반 함수로 구현하게 한다. [실습 2-2]는 그 기능을 연산자 중복으로 바꾸어 구현하는 문제이다.

---

<br>

## 문제 2. 12주차 강의자료 [실습 3]

[실습 3]에 쓰이는 `Book` 클래스는 [실습 2]의 클래스에서 `getPrice()`와 `setPrice()`를 뺀 것이다.

```cpp
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
        cout << title << ' ' << price << "원 " << pages << " 페이지" << endl;
    }

    string getTitle() {
        return title;
    }
};
```

다음 `main()` 함수가 실행되도록, 두 가지 상황에 맞는 `==` 연산자 함수를 `Book` 클래스의 멤버 함수로 작성하라.

```cpp
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

**실행 예시**

```text
명품 C++ 입니다.
두 책은 다른 책입니다.
```

참조 질문으로, `!=` 연산자 함수를 `Book` 클래스의 멤버 함수로 제공한다면 어떻게 구현해야 하는지를 묻는다.

---

<br>

## 문제 3. 연산자 중복 관련 자유 코드

연산자 중복을 활용하는 코드를 자유롭게 작성하고 분석하라. 교재에 실린 코드와 다른 예를 활용한다. 정해진 클래스나 `main()` 함수, 실행 예시는 없다.
