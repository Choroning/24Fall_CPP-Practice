# 8장 연습 문제 풀이 — 상속

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 8

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 점의 색상과 위치 정보](#연습-문제-1-점의-색상과-위치-정보)
- [연습 문제 2: 학생 성적 관리](#연습-문제-2-학생-성적-관리)
- [교재 이론 문제](#교재-이론-문제)

---

<br>

## 연습 문제 1: 점의 색상과 위치 정보

**문제:** 좌표 `x`, `y`를 private으로, 위치를 바꾸는 `move()`를 protected로 가진 `Point` 클래스를 수정하지 않고, 이를 상속받는 `ColorPoint` 클래스를 정의하여 주어진 `main()`이 실행되게 한다. `ColorPoint`는 색상과 위치로 초기화하는 생성자, `setPoint()`, `setColor()`, `show()`를 가져야 한다. 전체 문제와 실행 예시는 [연습 문제 1](Exercises.ko.md#연습-문제-1-점의-색상과-위치-정보)에 있다.

### 풀이

`ColorPoint`에 색상을 저장할 `string color`를 추가하고, 생성자 하나에 디폴트 매개변수를 두어 `main()`의 세 가지 선언을 모두 처리한다. `Point`에는 기본 생성자가 없으므로 `ColorPoint`의 생성자는 멤버 초기화 리스트에서 `Point(x, y)`를 반드시 명시적으로 호출해야 한다. 호출하지 않으면 컴파일러가 `Point()`를 호출하도록 컴파일하므로 컴파일 오류가 발생한다([8장 3.1절](Concepts_Lecture.ko.md#31-생성자-체인)).

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

실행 결과는 다음과 같다.

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.
```

실행 예시의 마지막 줄 `Program ended with exit code: 0`과 그 앞의 빈 줄은 프로그램이 끝난 뒤 Xcode가 출력하는 것이며 프로그램의 출력이 아니다.

---

<br>

## 연습 문제 2: 학생 성적 관리

**문제:** 학년마다 과목이 다른 `FirstGrade`, `SecondGrade`, `ThirdGrade` 클래스에서 공통 요소(이름, 국어, 정보 점수)를 `Student` 클래스로 모으고, 세 클래스가 `Student`를 상속받도록 설계하여 주어진 `main()`이 실행되게 한다. 이름은 private, 과목별 점수는 protected로 둔다. 전체 문제와 클래스 설계도, 실행 예시는 [연습 문제 2](Exercises.ko.md#연습-문제-2-학생-성적-관리)에 있다.

### 풀이

`Student`는 이름 `name`을 private으로, 국어 `kor`와 정보 `pro` 점수를 protected로 가진다. 이름을 리턴하는 `getName()`과 세 값을 초기화하는 생성자는 protected로 두었다. 생성자가 protected이므로 `Student` 객체를 단독으로 만들 수는 없고 파생 클래스 객체의 일부로만 생성되며, `getName()`은 파생 클래스의 `display()`에서만 호출된다.

각 파생 클래스는 자기 학년의 과목 점수를 private 멤버로 더하고, 생성자의 멤버 초기화 리스트에서 `Student(s, kors, pros)`를 호출해 공통 부분을 초기화한다. `Student`에 기본 생성자가 없으므로 이 호출은 반드시 필요하다. `display()`는 private인 `name`을 `getName()`으로 읽고, protected인 `kor`와 `pro`는 직접 읽는다. 과목 구성이 학년마다 다르므로 `getAverage()`와 `display()`는 각 파생 클래스에 따로 정의한다.

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

실행 결과는 다음과 같다.

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

실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 출력하는 메시지이다.

---

<br>

## 교재 이론 문제

**교재:** 8장 연습문제의 이론 문제 중 짝수 번 문항

### 이론 문제 2

**문제:** 객체 지향의 상속 개념과 가장 가까운 짝을 고르라.

**정답:** ①

상속은 기본 클래스의 특성을 물려받고 새로운 특성을 덧붙여 파생 클래스를 만드는 is-a 관계이다([8장 1.1절](Concepts_Lecture.ko.md#11-상속이-필요한-이유)). 스마트 TV는 TV의 기능을 모두 가지면서 인터넷 접속 같은 기능을 더한 TV의 일종이므로 상속 관계에 가장 가깝다. 천 소파와 가죽 소파는 같은 소파의 서로 다른 종류일 뿐 한쪽이 다른 쪽을 물려받지 않고, CPU와 PC, 엔진과 변속기는 부품과 전체 또는 부품끼리의 관계(has-a)이다.

### 이론 문제 4

**문제:** `public`, `private`, `protected` 상속이 섞인 클래스에서 컴파일 오류가 나는 접근을 고르라.

**정답:** ④

`D`는 `B`를 `protected`로 상속하므로, `B`에서 `public`이던 `w`(원래 `A`의 멤버)와 `x`가 `D` 안에서 `protected`가 된다. 따라서 클래스 밖인 `d.w = 10;`은 접근할 수 없다([8장 2.2절](Concepts_Lecture.ko.md#22-protected-상속)). ①의 `a.w`는 `public` 멤버이고, ②의 `b.w`는 `public` 상속이므로 `public`으로 남는다. ③의 `c.y`는 `C`가 직접 선언한 `public` 멤버이므로 `A`를 `private`으로 상속한 것과 관계없이 접근할 수 있다.

### 이론 문제 6

**문제:** `B`가 `A`를 `protected`로 상속할 때 `main()`의 네 줄 중 컴파일 오류가 나는 줄을 모두 고르라.

**정답:** ①, ②, ④

- **① `a.x = 3;`:** `x`는 `A`의 `private` 멤버이므로 클래스 밖에서 접근할 수 없다.
- **② `b.y = 3;`:** `y`는 `B`의 `private` 멤버이다.
- **③ `a.setX(5);`:** `setX()`는 `A`의 `public` 멤버이므로 옳다.
- **④ `b.setX(5);`:** `B`가 `A`를 `protected`로 상속하여 `setX()`가 `B` 안에서 `protected`가 되었으므로 클래스 밖에서 호출할 수 없다. `B`의 멤버 함수 `setXY()` 안에서는 `setX(x)`를 호출할 수 있다.

### 이론 문제 8

**문제:** `A`를 상속하는 `B`, `C`와 `B`를 상속하는 `D`에 대해 업캐스팅, 멤버, 접근, 다운캐스팅에 관한 네 물음에 답하라.

**정답:**

- (1) ③. 업캐스팅은 파생 클래스의 포인터를 기본 클래스의 포인터에 대입하는 것이다. `B`와 `C`는 둘 다 `A`를 상속하지만 서로 상속 관계가 아니므로 `bp = cp;`는 업캐스팅도 다운캐스팅도 아니며 컴파일 오류이다.
- (2) `w`, `x`, `z`. `D`는 직접 선언한 `z`와 함께 `B`의 `x`, `B`가 `A`에게서 물려받은 `w`를 가진다.
- (3) ③. `ap = dp;`로 업캐스팅한 `ap`는 `A` 타입의 포인터이므로 `A`에 선언된 멤버 `w`만 접근할 수 있고, `x`에는 접근할 수 없다.
- (4) `dp = (D*)ap;`처럼 명시적으로 형 변환한다.

기본 클래스 포인터를 파생 클래스 포인터에 대입하는 다운캐스팅은 명시적 형 변환이 필요하다([8장 1.4절](Concepts_Lecture.ko.md#14-상속과-객체-포인터)). (4)에서는 바로 앞 줄이 `ap`에 `D` 객체의 주소를 넣었으므로 형 변환한 `dp`가 실제로 `D` 객체를 가리켜 안전하다. `ap`가 `D`가 아닌 객체를 가리키는데 다운캐스팅하여 `D`의 멤버에 접근하면 정의되지 않은 동작이 된다.

### 이론 문제 10

**문제:** 기본 생성자가 없는 기본 클래스 `A`를 상속한 `B`의 생성자에서 생기는 오류를 찾고, 정해진 출력이 나오도록 두 생성자를 고치라.

**정답:**

(1) `B()`와 `B(int x)` 두 생성자 모두에서 컴파일 오류가 난다. 파생 클래스의 생성자가 기본 클래스의 생성자를 지정하지 않으면 컴파일러는 기본 클래스의 기본 생성자 `A()`를 호출하려 하는데, `A`에는 `A(int x)`만 있고 기본 생성자가 없다.

(2), (3)은 멤버 초기화 리스트로 호출할 기본 클래스 생성자와 인자를 지정한다.

```cpp
B() : A(20) { cout << "생성자 B" << endl; }
B(int x) : A(x + 20) { cout << "생성자 B " << x << endl; }
```

파생 클래스의 객체가 생성될 때는 기본 클래스의 생성자가 먼저 실행된 뒤 파생 클래스의 생성자가 실행된다([8장 3.1절](Concepts_Lecture.ko.md#31-생성자-체인)). `B b;`는 `A(20)`을 실행한 뒤 `B()`의 몸체를 실행하여 `생성자 A 20`, `생성자 B`를 출력한다. `B b(15);`는 받은 15에 20을 더한 35로 `A`의 생성자를 호출하므로 `생성자 A 35`, `생성자 B 15`를 출력한다. 생성자 몸체 안에 `A(20);`이라고 쓰면 기본 클래스 부분을 초기화하는 것이 아니라 이름 없는 임시 `A` 객체를 하나 더 만드는 문장이 되므로, 기본 생성자가 없다는 오류는 사라지지 않는다.

### 이론 문제 12

**문제:** 다중 상속에 관한 설명 중 틀린 것을 고르라.

**정답:** ④

다중 상속으로 같은 이름의 멤버를 두 경로로 물려받아 생기는 모호성은 실행 시간이 아니라 컴파일 시간에 발견되며, 이론 문제 16의 클래스에서 `smartTV.screenSize`에 접근하면 Clang은 `non-static member 'screenSize' found in multiple base-class subobjects of type 'TV'` 오류를 낸다. 가상 상속은 `virtual` 키워드로 공통 기본 클래스의 멤버를 하나만 두게 하는 상속 방식이며, 동적 바인딩과는 다른 개념이다([8장 5절](Concepts_Lecture.ko.md#5-다중-상속)). 나머지 설명은 옳다. 자바는 클래스의 다중 상속을 허용하지 않고 인터페이스로 비슷한 효과를 낸다(①).

### 이론 문제 14

**문제:** 두 클래스와 세 클래스를 동시에 상속받는 `HiPen`과 `OmniPen`을 선언하라.

**정답:**

```cpp
class HiPen : public Pen, public Eraser { ... };                // (1)
class OmniPen : public Pen, public Eraser, public Lock { ... };  // (2)
```

다중 상속은 콜론 뒤에 기본 클래스를 쉼표로 구분하여 나열하며, 기본 클래스마다 상속 접근 지정자를 붙인다. 접근 지정자를 생략하면 `class`에서는 `private` 상속이 되므로, 기본 클래스의 `public` 멤버를 바깥에서 쓰려면 각각 `public`을 적는다. 생성자는 나열한 순서대로, 즉 `Pen`, `Eraser`, `Lock`의 순서로 호출된다.

### 이론 문제 16

**문제:** `TV`를 상속하는 `ColorTV`와 `InternetTV`의 상속 관계가 가진 잠재적 문제를 지적하고 고치라.

**정답:** 나중에 `ColorTV`와 `InternetTV`를 함께 상속하는 클래스(예: `SmartTV`)를 만들면 `TV`의 멤버 `screenSize`가 두 경로로 중복 상속된다. 이를 막기 위해 두 클래스가 `TV`를 가상 상속하도록 고친다.

```cpp
class TV {
public: int screenSize;
};
class ColorTV : virtual public TV {
public: int color;
};
class InternetTV : virtual public TV {
public: string ipAddr;
};
class SmartTV : public ColorTV, public InternetTV { };
```

가상 상속을 쓰지 않으면 `SmartTV` 객체 안에 `ColorTV`를 통한 `TV` 부분과 `InternetTV`를 통한 `TV` 부분이 따로 생긴다. 그러면 `screenSize`가 두 개가 되어 메모리가 낭비되고, `smartTV.screenSize`가 어느 쪽인지 정할 수 없어 컴파일 오류가 난다. 두 중간 클래스가 `virtual public TV`로 상속하면 `SmartTV` 안에 `TV` 부분이 하나만 만들어져 `screenSize`도 하나가 된다([8장 5절](Concepts_Lecture.ko.md#5-다중-상속)). 이 문제는 다이아몬드 모양의 상속 구조에서 생기므로 흔히 다이아몬드 문제라고 부른다.
