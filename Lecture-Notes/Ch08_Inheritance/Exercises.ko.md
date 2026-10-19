# 8장 연습 문제 — 상속

[레포지토리 안내](../../README.ko.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **최종 수정일:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 8

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 연습 문제 1: 점의 색상과 위치 정보

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `ColorPoint` 클래스를 정의하라. `ColorPoint` 클래스는 `Point` 클래스의 파생 클래스이다. 교재 8장 실습 문제 3, 4번을 바탕으로 한 문제이다.

**프로그램 요구사항**

- [ 빈칸 ]을 완성하되, 주어진 예시 코드는 변경할 수 없다.

**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    Point(int x, int y) { this->x = x; this->y = y; }
    int getX() { return x; }
    int getY() { return y; }
protected:
    void move(int x, int y) { this->x = x; this->y = y; }
};

class ColorPoint : public Point {
    // ColorPoint 클래스 정의
    [ 빈칸 ]
};

int main() {
    ColorPoint cp1(5, 5, "RED");  // 색상 "RED", 위치 (5, 5)로 초기화
    cp1.show();
    cp1.setPoint(10, 20);         // 위치 (10, 20)으로 설정
    cp1.setColor("BLUE");         // 색상 "BLUE"로 설정
    cp1.show();

    ColorPoint cp2(5, 5);         // 색상 "BLACK", 위치 (5, 5)로 초기화
    cp2.show();
    cp2.setPoint(3, 7);           // 위치 (3, 7)로 설정
    cp2.setColor("GREEN");        // 색상 "GREEN"으로 설정
    cp2.show();

    ColorPoint zeroPoint;         // 색상 "BLACK", 위치 (0, 0)로 초기화
    zeroPoint.show();

    return 0;
}
```

**실행 예시**

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-1-점의-색상과-위치-정보)**

---

<br>

## 연습 문제 2: 학생 성적 관리

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 상속 구조를 고려하여 `Student` 클래스와 파생 클래스를 완성하라. `Student` 클래스는 세 파생 클래스 `FirstGrade`, `SecondGrade`, `ThirdGrade`에서 중복되는 요소를 가지고 있으며, 세 파생 클래스는 `Student` 클래스 상속을 통해 구현된다.

![그림 1. FirstGrade, SecondGrade, ThirdGrade 클래스의 멤버 변수와 멤버 함수](../../images/Ch08_Grade-Classes.png)

*그림 1. FirstGrade, SecondGrade, ThirdGrade 클래스의 멤버 변수와 멤버 함수*

세 클래스는 모두 이름(`name`), 국어(`kor`), 정보(`pro`) 점수를 멤버 변수로 가지며, `FirstGrade`는 수학(`mat`), `SecondGrade`는 과학(`sci`)과 도덕(`mor`), `ThirdGrade`는 영어(`eng`)와 한국사(`his`) 점수를 더 가진다. 멤버 함수는 세 클래스 모두 생성자, 이름을 반환하는 `getName()`, 성적 평균을 반환하는 `getAverage()`, 성적을 출력하는 `display()`이다.

그림 1에서 빨간색으로 표시한 요소(이름)는 `private`, 초록색으로 표시한 요소(과목별 점수)는 `protected`로 접근을 제어한다.

**프로그램 요구사항**

- 주어진 예시 코드는 변경할 수 없다.

**예시 코드**

```cpp
#include <iostream>
using namespace std;

int main() {
    // 이름, 국어, 정보, 수학 점수 초기화
    FirstGrade f("홍길동", 90, 81, 88);

    // 이름, 국어, 정보, 과학, 도덕 점수 초기화
    SecondGrade s("장영실", 73, 91, 99, 68);
    
    // 이름, 국어, 정보, 영어, 한국사 점수 초기화
    ThirdGrade t("정도전", 81, 53, 80, 96);
    
    f.display();
    s.display();
    t.display();
    
    return 0;
}
```

**실행 예시**

```text
이름: 홍길동
국어: 90
정보: 81
수학: 88
---------------
평균: 86.3333

이름: 장영실
국어: 73
정보: 91
과학: 99
도덕: 68
---------------
평균: 82.75

이름: 정도전
국어: 81
정보: 53
영어: 80
한국사: 96
---------------
평균: 77.5

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-2-학생-성적-관리)**
