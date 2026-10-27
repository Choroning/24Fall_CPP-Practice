# HW5 과제 안내문 — 참조 리턴과 복사 생성자

[레포지토리 안내](../../../README.ko.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **최종 수정일:** 2026-10-27

컴퓨터언어실습Ⅱ 3분반의 5차 과제이다. 10주차 강의자료의 실습 문제 두 개로 이루어진다.

- **제출 기한:** 2024-11-11 (월) 13:00, 수업 시작 전까지이다. 기한이 지나면 과제가 자동으로 비공개된다.
- **제출 방법:** 과목이 배포한 과제 제출 양식(.hwp)에 작성하여 블랙보드의 과제란에 제출한다.
- **작성 내용:** 코드 문제는 코드와 함께 반드시 코드 분석 내용을 작성해야 한다.
- **안내:** Visual Studio 2022에서는 함수 정의에 `const`를 추가해야 할 수 있으며, 필요하면 추가한다.

---

<br>

## 문제 1. 10주차 강의자료 [실습 2]

클래스 `Accumulator`는 `add()` 함수로 값을 계속 누적하는 클래스이며, 다음과 같이 선언된다. `Accumulator` 클래스를 구현하라.

```cpp
class Accumulator {
    int value;
public:
    Accumulator(int value) { this->value = value; }
    Accumulator& add(int n);
    int get() { return value; }
};
```

`Accumulator`는 다음 `main()` 함수에서 사용된다.

```cpp
int main() {
    Accumulator acc(10);
    acc.add(5).add(6).add(7); // acc의 value 멤버가 28이 된다.
    cout << acc.get() << endl; // 28 출력
}
```

세 번의 `add()`는 [1] `acc.add(5)`, [2] `acc.add(6)`, [3] `acc.add(7)`의 순서로 호출된다.

**실행 예시**

```text
28
```

---

<br>

## 문제 2. 10주차 강의자료 [실습 3]

책의 이름과 가격을 저장하는 다음 `Book` 클래스에 대한 물음에 답하라.

```cpp
class Book {
    char *title; // 제목 문자열
    int price; // 가격
public:
    Book(char* title, int price);
    Book(Book& b); // 깊은 복사 생성자 작성
    ~Book();
    void set(char* title, int price);
    void show() { cout << title << ' ' << price << "원" << endl; }
};

int main() {
    Book cpp("명품C++", 10000);
    Book java = cpp;
    java.set("명품자바", 12000);
    cpp.show();
    java.show();
}
```

1. `Book` 클래스의 생성자, 소멸자, `set()` 함수를 구현하라.
2. 컴파일러가 자동으로 추가하는 디폴트 복사 생성자의 코드는 무엇인가?
3. 프로그래머가 복사 생성자의 정의를 제공하지 않으면, 이 코드를 실행할 때 문제가 발생한다. 어떤 문제인가?
4. 오류가 발생하지 않도록 복사 생성자를 정의하라.

**실행 예시**

```text
명품C++ 10000원
명품자바 12000원
```

Visual Studio에서는 `strcpy()` 때문에 생기는 컴파일 오류를 막기 위해 파일의 앞부분에 `#define _CRT_SECURE_NO_WARNINGS`를 둔다.
