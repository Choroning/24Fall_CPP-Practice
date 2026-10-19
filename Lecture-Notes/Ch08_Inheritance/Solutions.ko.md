# 8장 연습 문제 풀이 — 상속

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 8

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 점의 색상과 위치 정보](#연습-문제-1-점의-색상과-위치-정보)
- [연습 문제 2: 학생 성적 관리](#연습-문제-2-학생-성적-관리)

---

<br>

## 연습 문제 1: 점의 색상과 위치 정보

**문제:** 좌표 `x`, `y`를 private으로, 위치를 바꾸는 `move()`를 protected로 가진 `Point` 클래스를 수정하지 않고, 이를 상속받는 `ColorPoint` 클래스를 정의하여 주어진 `main()`이 실행되게 한다. `ColorPoint`는 색상과 위치로 초기화하는 생성자, `setPoint()`, `setColor()`, `show()`를 가져야 한다. 전체 문제와 실행 예시는 [연습 문제 1](Exercises.ko.md#연습-문제-1-점의-색상과-위치-정보)에 있다.

### 풀이

`ColorPoint`에 색상을 저장할 `string color`를 추가하고, 생성자 하나에 기본 인자를 주어 `main()`의 세 가지 선언을 모두 처리한다. `Point`에는 기본 생성자가 없으므로 `ColorPoint`의 생성자는 초기화 목록에서 `Point(x, y)`를 반드시 명시적으로 호출해야 한다. 호출하지 않으면 컴파일러가 `Point()`를 호출하도록 컴파일하므로 컴파일 오류가 발생한다([8장 정리본](Concepts_Lecture.ko.md) 3.1절).

| 선언 | 생성자가 받는 인자 | 결과 |
|:----|:----|:----|
| `ColorPoint cp1(5, 5, "RED")` | x=5, y=5, color="RED" | RED, (5, 5) |
| `ColorPoint cp2(5, 5)` | x=5, y=5, color="BLACK" (기본값) | BLACK, (5, 5) |
| `ColorPoint zeroPoint` | x=0, y=0, color="BLACK" (모두 기본값) | BLACK, (0, 0) |

*표 1. `ColorPoint` 객체 선언과 생성자의 기본 인자*

`x`와 `y`는 `Point`의 private 멤버이므로 `ColorPoint`에서 직접 바꿀 수 없다. `setPoint()`는 파생 클래스에서 호출할 수 있는 `Point`의 protected 멤버 함수 `move()`로 위치를 바꾸고, `show()`는 public 멤버 함수 `getX()`와 `getY()`로 좌표를 읽는다.

> **참고:** **`<string>` 헤더.** 이 코드는 `#include <string>` 없이 `string`을 사용한다. 사용한 컴파일러에서는 `<iostream>`이 `<string>`을 함께 포함하므로 컴파일되지만, 표준이 이를 보장하지는 않으므로 이식성을 위해서는 `#include <string>`을 추가하는 것이 안전하다.

작성한 코드는 다음과 같다([`Lab_Q1.cpp`](../../Coursework/Lab08_Inheritance/Lab_Q1.cpp)).

```cpp
/**
 * @file    A10204-1_C08Q1.cpp
 * @brief   Q1. 점의 색상과 위치 정보
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-20
 */

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
    string color;
public:
    ColorPoint(int x = 0, int y = 0, string color = "BLACK") : Point(x, y) {
        this->color = color;
    }
    void setPoint(int x, int y) {
        move(x, y);
    }
    void setColor(string color) {
        this->color = color;
    }
    void show() {
        cout << color << "색으로 (" << getX() << ", " << getY() << ")에 위치한 점입니다." << endl;
    }
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

`g++ -std=c++17 -Wall`로 컴파일하면 경고 없이 컴파일되며, 실행 결과는 다음과 같다.

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.
```

실행 결과는 실행 예시와 같다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`과 그 앞의 빈 줄은 프로그램이 끝난 뒤 Xcode가 출력하는 것이며 프로그램의 출력이 아니다.

---

<br>

## 연습 문제 2: 학생 성적 관리

**문제:** 학년마다 과목이 다른 `FirstGrade`, `SecondGrade`, `ThirdGrade` 클래스에서 공통 요소(이름, 국어, 정보 점수)를 `Student` 클래스로 모으고, 세 클래스가 `Student`를 상속받도록 설계하여 주어진 `main()`이 실행되게 한다. 이름은 private, 과목별 점수는 protected로 둔다. 전체 문제와 클래스 설계도, 실행 예시는 [연습 문제 2](Exercises.ko.md#연습-문제-2-학생-성적-관리)에 있다.

### 풀이

`Student`는 이름 `name`을 private으로, 국어 `kor`와 정보 `pro` 점수를 protected로 가진다. 이름을 반환하는 `getName()`과 세 값을 초기화하는 생성자는 protected로 두었다. 생성자가 protected이므로 `Student` 객체를 단독으로 만들 수는 없고 파생 클래스 객체의 일부로만 생성되며, `getName()`은 파생 클래스의 `display()`에서만 호출된다.

각 파생 클래스는 자기 학년의 과목 점수를 private 멤버로 더하고, 생성자의 초기화 목록에서 `Student(s, kors, pros)`를 호출해 공통 부분을 초기화한다. `Student`에 기본 생성자가 없으므로 이 호출은 반드시 필요하다. `display()`는 private인 `name`을 `getName()`으로 읽고, protected인 `kor`와 `pro`는 직접 읽는다. 과목 구성이 학년마다 다르므로 `getAverage()`와 `display()`는 각 파생 클래스에 따로 정의한다.

`getAverage()`는 합을 `3.0`이나 `4.0`으로 나누어 실수 나눗셈을 한다. `3`으로 나누면 정수 나눗셈이 되어 259 / 3 = 86처럼 소수 부분이 버려진다. `cout`은 기본적으로 유효 숫자 6자리까지 출력하므로 86.3333...은 `86.3333`으로 출력된다.

| 클래스 (학생) | 점수 | 합 | 평균 |
|:----|:----|:----|:----|
| `FirstGrade` (홍길동) | 90, 81, 88 | 259 | 259 / 3.0 = 86.3333 |
| `SecondGrade` (장영실) | 73, 91, 99, 68 | 331 | 331 / 4.0 = 82.75 |
| `ThirdGrade` (정도전) | 81, 53, 80, 96 | 310 | 310 / 4.0 = 77.5 |

*표 2. 학년별 평균 계산*

작성한 코드는 다음과 같다([`Lab_Q2.cpp`](../../Coursework/Lab08_Inheritance/Lab_Q2.cpp)).

```cpp
/**
 * @file    A10204-1_C08Q2.cpp
 * @brief   Q2. 학생 성적 관리
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-20
 */

#include <iostream>
using namespace std;

class Student {
    string name;
protected:
    int kor;
    int pro;
    string getName() {
        return name;
    }
    Student(string s, int kors, int pros) {
        name = s;
        kor = kors;
        pro = pros;
    }
};

class FirstGrade : public Student {
    int mat;
public:
    FirstGrade(string s, int kors, int pros, int mats) : Student(s, kors, pros) {
        mat = mats;
    }
    double getAverage() {
        return (kor + pro + mat) / 3.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "수학: " << mat << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

class SecondGrade : public Student {
    int sci;
    int mor;
public:
    SecondGrade(string s, int kors, int pros, int scis, int mors) : Student(s, kors, pros) {
        sci = scis;
        mor = mors;
    }
    double getAverage() {
        return (kor + pro + sci + mor) / 4.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "과학: " << sci << endl;
        cout << "도덕: " << mor << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

class ThirdGrade : public Student {
    int eng;
    int his;
public:
    ThirdGrade(string s, int kors, int pros, int engs, int hiss) : Student(s, kors, pros) {
        eng = engs;
        his = hiss;
    }
    double getAverage() {
        return (kor + pro + eng + his) / 4.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "영어: " << eng << endl;
        cout << "한국사: " << his << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

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

`g++ -std=c++17 -Wall`로 컴파일하면 경고 없이 컴파일되며, 실행 결과는 다음과 같다.

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
```

실행 결과는 실행 예시와 같다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 출력하는 메시지이다.
