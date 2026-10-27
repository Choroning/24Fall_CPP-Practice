# 9장 연습 문제 풀이 — 가상함수와 추상 클래스

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 9

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 가상함수를 사용한 학생 성적 관리](#연습-문제-1-가상함수를-사용한-학생-성적-관리)
- [연습 문제 2: 가상함수를 사용한 단위 변환](#연습-문제-2-가상함수를-사용한-단위-변환)
- [연습 문제 3: 가상함수를 재정의하는 것의 이름](#연습-문제-3-가상함수를-재정의하는-것의-이름)
- [연습 문제 4: 함수 중복, 오버라이딩, 연산자 중복의 객체 지향 특성](#연습-문제-4-함수-중복-오버라이딩-연산자-중복의-객체-지향-특성)
- [연습 문제 5: 가상함수의 판별과 호출 결과](#연습-문제-5-가상함수의-판별과-호출-결과)
- [연습 문제 6: `g()`에서 여러 `f()` 호출하기](#연습-문제-6-g에서-여러-f-호출하기)
- [연습 문제 7: 가상 소멸자가 없는 기본 클래스](#연습-문제-7-가상-소멸자가-없는-기본-클래스)
- [연습 문제 8: 순수 가상함수에 대한 설명](#연습-문제-8-순수-가상함수에-대한-설명)
- [연습 문제 9: 순수 가상함수를 구현하지 않은 파생 클래스](#연습-문제-9-순수-가상함수를-구현하지-않은-파생-클래스)

---

<br>

## 연습 문제 1: 가상함수를 사용한 학생 성적 관리

**문제:** 세 학년의 공통 요소를 가진 `Student` 클래스와 이를 상속하는 `FirstGrade`, `SecondGrade`, `ThirdGrade`를 클래스 설계도대로 완성하고, 기본 클래스 포인터 배열로 네 학생의 성적을 출력한 뒤 동적으로 할당한 객체를 해제하도록 `main()`의 빈칸을 채운다. `getAverage()`는 순수 가상함수로 선언하고, `Student::displayGrade()`는 이름과 국어, 정보 성적을 출력한다. 설계도와 실행 예시는 [연습 문제 1](Exercises.ko.md#연습-문제-1-가상함수를-사용한-학생-성적-관리)에 있다.

### 풀이

**클래스 설계:**

- `Student`는 세 학년이 공통으로 가지는 이름 `name`(`private`)과 국어, 정보 성적 `kor`, `pro`(`protected`)를 가진다. `kor`와 `pro`는 파생 클래스의 `getAverage()`가 평균을 계산할 때 직접 써야 하므로 `protected`로 두고, `name`은 `Student::displayGrade()`에서만 출력하므로 `private`로 둔다.
- 학년마다 과목 수가 달라 `Student`에는 `getAverage()`로 구현할 내용이 없으므로 `virtual double getAverage() = 0;`으로 선언한다. 이 때문에 `Student`는 추상 클래스가 되어 직접 객체를 만들 수 없고, 세 파생 클래스는 각자 `getAverage()`를 구현하여 구체 클래스가 된다.
- `displayGrade()`는 `Student`에서 공통 부분(이름, 국어, 정보)을 출력하는 가상함수로 정의하고, 파생 클래스에서 오버라이딩한다. 파생 클래스의 `displayGrade()`는 학년 표시를 출력한 뒤 범위 지정 연산자로 `Student::displayGrade()`를 호출하여 공통 부분을 출력하고([9장 2.10절](Concepts_Lecture.ko.md#210-범위-지정-연산자를-이용한-기본-클래스-가상함수-호출)의 정적 바인딩), 이어서 학년별 과목과 평균을 출력한다.
- 평균은 `(kor + pro + mat) / 3.0`처럼 실수 리터럴로 나누어 정수 나눗셈을 피한다. `/ 3`으로 나누면 고길동의 평균이 73.3333이 아니라 73으로 출력된다.
- 소멸자 `~Student()`를 `virtual`로 선언하여 `Student*`로 `delete`할 때 파생 클래스의 소멸자부터 올바르게 실행되게 한다([9장 3절](Concepts_Lecture.ko.md#3-가상-소멸자)). 파생 클래스의 소멸자와 오버라이딩한 함수에는 `override`를 붙여 컴파일러가 원형 일치를 검사하게 한다.

**`main()`의 빈칸:** 반복문 안에서 `p[i]->displayGrade();`를 호출하면 `displayGrade()`가 가상함수이므로 `p[i]`가 실제로 가리키는 학년 객체의 함수가 동적 바인딩으로 실행된다. 해제는 반복문으로 `delete p[i];`를 네 번 실행한다.

**코드:** [`Lab_Q1.cpp`](../../Coursework/Lab09_Virtual-Functions-and-Abstract-Classes/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C09Q1.cpp
 * @brief   Q1. 가상함수를 사용한 학생 성적 관리
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-27
 */

#include <iostream>
#include <string>
using namespace std;

class Student {
    string name;
protected:
    int kor, pro;
public:
    Student(string name, int kor, int pro) {
        this->name = name;
        this->kor = kor;
        this->pro = pro;
    }
    virtual double getAverage()  = 0;
    virtual void displayGrade() {
        cout << "이름: " << name << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
    }
    virtual ~Student() {}
};

class FirstGrade : public Student {
    int mat;
public:
    FirstGrade(string name, int kor, int pro, int mat) : Student(name, kor, pro) {
        this->mat = mat;
    }
    double getAverage() override {
        return (kor + pro + mat) / 3.0;
    }
    void displayGrade() override {
        cout << "[1학년]" << endl;
        Student::displayGrade();
        cout << "수학: " << mat << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~FirstGrade() override {}
};

class SecondGrade : public Student {
    int sci, mor;
public:
    SecondGrade(string name, int kor, int pro, int sci, int mor) : Student(name, kor, pro) {
        this->sci = sci;
        this->mor = mor;
    }
    double getAverage() override {
        return (kor + pro + sci + mor) / 4.0;
    }
    void displayGrade() override {
        cout << "[2학년]" << endl;
        Student::displayGrade();
        cout << "과학: " << sci << endl;
        cout << "도덕: " << mor << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~SecondGrade() override {}
};

class ThirdGrade : public Student {
    int eng, his;
public:
    ThirdGrade(string name, int kor, int pro, int eng, int his) : Student(name, kor, pro) {
        this->eng = eng;
        this->his = his;
    }
    double getAverage() override {
        return (kor + pro + eng + his) / 4.0;
    }
    void displayGrade() override {
        cout << "[3학년]" << endl;
        Student::displayGrade();
        cout << "영어: " << eng << endl;
        cout << "한국사: " << his << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~ThirdGrade() override {}
};

int main() {
    Student* p[4];
    p[0] = new FirstGrade("고길동", 80, 70, 70);
    p[1] = new SecondGrade("둘리", 70, 60, 90, 70);
    p[2] = new ThirdGrade("또치", 90, 100, 90, 100);
    p[3] = new SecondGrade("도우너", 80, 90, 60, 100);
    
    for (int i = 0; i < 4; i++) {
        // 기본 클래스 포인터 p를 사용하여 displayGrade()를 올바르게 구현하시오.
        p[i]->displayGrade();
    }

    // delete 연산자를 사용해 동적할당 영역에 대한 반환 코드를 작성하시오.
    for (int i = 0; i < 4; i++) {
        delete p[i];
    }
    
    return 0;
}
```

**실행 결과:** 실행하면 다음과 같이 출력된다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 프로그램이 출력한 것이 아니라 개발 환경(Xcode)이 프로그램 종료 후 표시하는 메시지이다.

```text
[1학년]
이름: 고길동
국어: 80
정보: 70
수학: 70
---------------
평균: 73.3333

[2학년]
이름: 둘리
국어: 70
정보: 60
과학: 90
도덕: 70
---------------
평균: 72.5

[3학년]
이름: 또치
국어: 90
정보: 100
영어: 90
한국사: 100
---------------
평균: 95

[2학년]
이름: 도우너
국어: 80
정보: 90
과학: 60
도덕: 100
---------------
평균: 82.5
```

---

<br>

## 연습 문제 2: 가상함수를 사용한 단위 변환

**문제:** 단위 변환 클래스의 공통 요소를 가진 `Convert` 클래스와 이를 상속하는 `LtoG`, `CtoF`, `MtoKm`을 클래스 설계도대로 완성하고, 기본 클래스 포인터 배열로 세 값을 변환하여 출력한 뒤 객체를 해제하도록 `main()`의 빈칸을 채운다. `compute()`와 `display()`는 순수 가상함수로 선언한다. 설계도, 변환 공식, 실행 예시는 [연습 문제 2](Exercises.ko.md#연습-문제-2-가상함수를-사용한-단위-변환)에 있다.

### 풀이

**클래스 설계:**

- `Convert`는 변환 전후의 값 `before`, `after`를 `protected`로 가져, 파생 클래스의 `compute()`와 `display()`가 직접 읽고 쓸 수 있게 한다. 생성자는 변환 전 값을 받아 `before`를 초기화한다.
- 변환 방법과 출력 형식은 단위마다 다르므로 `compute()`와 `display()`를 `= 0`을 붙인 순수 가상함수로 선언한다. 따라서 `Convert`는 추상 클래스가 되고, 세 파생 클래스는 두 함수를 모두 구현하여 구체 클래스가 된다.
- 파생 클래스의 생성자는 멤버 초기화 리스트 `: Convert(val)`로 기본 클래스 생성자에 값을 넘긴다. 설계도에 따라 파생 클래스는 멤버변수를 따로 두지 않는다.
- `CtoF::compute()`는 공식 $F = C \times 9 / 5 + 32$를 `(9.0/5.0) * before + 32`로 계산한다. `9/5`로 쓰면 정수 나눗셈으로 1이 되므로 실수 리터럴을 쓴다.
- 소멸자 `~Convert()`를 `virtual`로 선언하여 `Convert*`로 `delete`할 때 파생 클래스의 소멸자부터 실행되게 한다.

**`main()`의 빈칸:** 반복문 안에서 `p[i]->compute();`로 변환하고 `p[i]->display();`로 결과를 출력한다. 두 함수 모두 가상함수이므로 `p[i]`가 가리키는 실제 객체(`MtoKm`, `LtoG`, `CtoF`)의 함수가 동적 바인딩으로 실행된다. 해제는 반복문으로 `delete p[i];`를 세 번 실행한다.

**검산:** $23.2 \times 1.609344 = 37.3367808$, $10.3 / 3.7854 = 2.72098\ldots$, $24.5 \times 9 / 5 + 32 = 76.1$이다. `cout`은 기본적으로 유효 숫자 6자리까지 출력하므로 앞의 두 값은 37.3368과 2.72098로 표시된다.

**코드:** [`Lab_Q2.cpp`](../../Coursework/Lab09_Virtual-Functions-and-Abstract-Classes/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C09Q2.cpp
 * @brief   Q2. 가상함수를 사용한 단위 변환
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-27
 */

#include <iostream>
using namespace std;

class Convert {
protected:
    double before, after;
public:
    Convert(double before) {
        this->before = before;
    }
    virtual void compute() = 0;
    virtual void display() = 0;
    virtual ~Convert() {}
};

class LtoG : public Convert {
public:
    LtoG(double val) : Convert(val) {}
    void compute() override {
        after = before / 3.7854;
    }
    void display() override {
        cout << before << " 리터 = " << after << " 갤런" << endl;
    }
    ~LtoG() override {}
};

class CtoF : public Convert {
public:
    CtoF(double val) : Convert(val) {}
    void compute() override {
        after = (9.0/5.0) * before + 32;
    }
    void display() override {
        cout << "섭씨 " << before << "도 = 화씨 " << after << "도" << endl;
    }
    ~CtoF() override {}
};

class MtoKm : public Convert {
public:
    MtoKm(double val) : Convert(val) {}
    void compute() override {
        after = before * 1.609344;
    }
    void display() override {
        cout << before << " 마일 = " << after << " 킬로미터" << endl;
    }
    ~MtoKm() override {}
};

int main() {
    Convert* p[3] = { 
        new MtoKm(23.2), new LtoG(10.3), new CtoF(24.5)
    };
    
    for (int i = 0; i < 3; i++) {
        // 기본 클래스 포인터 p를 사용하여 단위 변환을 수행하시오.
        p[i]->compute();

        // 기본 클래스 포인터 p를 사용하여 변환 결과를 출력하시오.
        p[i]->display();
    }

    // delete 연산자를 사용해 동적할당 영역에 대한 반환 코드를 작성하시오.
    for (int i = 0; i < 3; i++) {
        delete p[i];
    }
    
    return 0;
}
```

**실행 결과:** 실행하면 다음과 같이 출력된다. 연습 문제 1과 같이 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 개발 환경(Xcode)이 표시하는 메시지이다.

```text
23.2 마일 = 37.3368 킬로미터
10.3 리터 = 2.72098 갤런
섭씨 24.5도 = 화씨 76.1도
```

---

<br>

## 연습 문제 3: 가상함수를 재정의하는 것의 이름

**교재 문항:** 9장 이론 문제 2번

**문제:** 기본 클래스의 가상함수와 같은 타입의 함수를 파생 클래스에서 재정의하는 것의 이름을 고르라. 보기는 [연습 문제 3](Exercises.ko.md#연습-문제-3-가상함수를-재정의하는-것의-이름)에 있다.

### 풀이

**정답:** ②

**해설:** 기본 클래스의 가상함수와 이름, 매개변수 목록, 리턴 타입이 같은 함수를 파생 클래스에서 다시 정의하는 것을 **오버라이딩(overriding)** 이라 한다([9장 2절](Concepts_Lecture.ko.md#2-가상함수)). 함수 중복은 같은 범위에서 이름이 같고 매개변수가 다른 함수를 여러 개 두는 것이고, `virtual`은 가상함수를 선언하는 키워드이며, 동적 바인딩은 오버라이딩된 가상함수 중 실행할 함수를 실행 시간에 정하는 과정이다.

---

<br>

## 연습 문제 4: 함수 중복, 오버라이딩, 연산자 중복의 객체 지향 특성

**교재 문항:** 9장 이론 문제 4번

**문제:** 함수 중복, 오버라이딩, 연산자 중복이 보여 주는 객체 지향 특성을 쓰라. 문제 전문은 [연습 문제 4](Exercises.ko.md#연습-문제-4-함수-중복-오버라이딩-연산자-중복의-객체-지향-특성)에 있다.

### 풀이

**정답:** 다형성

**해설:** 세 기능은 모두 같은 이름이나 기호가 상황에 따라 서로 다르게 동작하게 한다. 함수 중복과 연산자 중복은 인자의 타입과 개수에 따라 컴파일 시간에 호출할 함수가 정해지는 정적 다형성이고, 가상함수의 오버라이딩은 객체의 실제 타입에 따라 실행 시간에 함수가 정해지는 동적 다형성이다.

---

<br>

## 연습 문제 5: 가상함수의 판별과 호출 결과

**교재 문항:** 9장 이론 문제 6번

**문제:** `A`, `B`, `C`의 `f()`와 `A::func()` 중 가상함수를 고르고, 객체와 포인터로 `f()`와 `func()`를 호출한 결과를 쓰라. 문제 전문과 코드는 [연습 문제 5](Exercises.ko.md#연습-문제-5-가상함수의-판별과-호출-결과)에 있다.

### 풀이

**정답:**

- (1) ①, ②, ③
- (2)~(5) 네 문장 모두 `C::f() called`를 출력한다.

`A`의 `f()`는 `virtual`로 선언되었고, 기본 클래스에서 가상으로 선언된 함수를 같은 타입으로 재정의한 함수는 `virtual`을 쓰지 않아도 자동으로 가상함수가 된다. 따라서 `B`와 `C`의 `f()`도 가상함수이다. `func()`는 `virtual`이 없는 일반 멤버 함수이다.

- **(2) `c.f();`:** 객체 `c`의 타입이 `C`이므로 `C::f()`가 호출된다.
- **(3) `pb->f();`, (4) `pa->f();`:** `pa`와 `pb`는 각각 `A*`, `B*`이지만 실제로 가리키는 객체가 `C`이고 `f()`가 가상함수이므로, 동적 바인딩으로 `C::f()`가 호출된다([9장 2.4절](Concepts_Lecture.ko.md#24-정적-바인딩-vs-동적-바인딩)).
- **(5) `pa->func();`:** `func()`는 `A`의 일반 함수이므로 `A::func()`가 실행되지만, 그 안의 `f()` 호출은 `this->f()`로서 가상함수 호출이다. `this`가 가리키는 객체가 `C`이므로 `C::f()`가 호출된다.

---

<br>

## 연습 문제 6: `g()`에서 여러 `f()` 호출하기

**교재 문항:** 9장 이론 문제 8번

**문제:** `B::g()` 안에서 외부 함수 `f()`, `A`의 `f()`, 자신의 `f()`를 각각 호출하는 코드를 쓰라. 문제 전문과 코드는 [연습 문제 6](Exercises.ko.md#연습-문제-6-g에서-여러-f-호출하기)에 있다.

### 풀이

**정답:** (1) `::f();` (2) `A::f();` (3) `f();`

**해설:** `B`의 멤버 함수 안에서 이름 `f`는 먼저 자기 클래스의 멤버 `B::f()`를 찾으므로, `f()`라고만 쓰면 (3)처럼 자신의 멤버 함수가 호출된다. 전역 범위의 함수를 부르려면 범위 지정 연산자 앞에 아무것도 쓰지 않은 `::f()`를 쓴다. 기본 클래스의 버전을 부르려면 `A::f()`처럼 클래스 이름으로 범위를 지정하며, 이렇게 범위를 지정한 호출은 가상함수라도 동적 바인딩 없이 `A::f()`를 직접 호출한다([9장 2.10절](Concepts_Lecture.ko.md#210-범위-지정-연산자를-이용한-기본-클래스-가상함수-호출)).

---

<br>

## 연습 문제 7: 가상 소멸자가 없는 기본 클래스

**교재 문항:** 9장 이론 문제 10번

**문제:** 기본 클래스 포인터로 파생 클래스 객체를 `delete`할 때의 실행 결과와 문제를 쓰고, 문제를 해결하라. 문제 전문과 코드는 [연습 문제 7](Exercises.ko.md#연습-문제-7-가상-소멸자가-없는-기본-클래스)에 있다.

### 풀이

**정답:**

(1) 실행 결과는 `id=10` 한 줄이다. `Student`의 소멸자가 실행되지 않으므로 생성자에서 동적으로 할당한 `name`이 반환되지 않아 메모리 누수가 생긴다.

(2) `Person`의 소멸자를 가상함수로 선언한다.

```cpp
virtual ~Person() { cout << "id=" << id << endl; }
```

`delete p;`에서 `p`의 타입이 `Person*`이고 소멸자가 가상이 아니면, 컴파일러는 포인터의 타입만 보고 `~Person()`만 호출한다(정적 바인딩). 엄밀히는 가상 소멸자가 없는 기본 클래스의 포인터로 파생 클래스 객체를 `delete`하는 것은 정의되지 않은 동작이며, 이 환경에서는 `~Person()`만 실행되었다. 소멸자를 `virtual`로 선언하면 동적 바인딩으로 실제 객체의 소멸자 `~Student()`가 먼저 실행되고, 이어서 기본 클래스의 소멸자 `~Person()`이 실행된다. 그래서 출력은 `name=손연재`, `id=10`의 두 줄이 되고 `name`의 메모리도 반환된다([9장 3절](Concepts_Lecture.ko.md#3-가상-소멸자)). 다른 클래스의 기본 클래스로 쓰일 클래스는 소멸자를 가상으로 선언하는 것이 원칙이다.

---

<br>

## 연습 문제 8: 순수 가상함수에 대한 설명

**교재 문항:** 9장 이론 문제 12번

**문제:** 순수 가상함수에 관한 설명 중 틀린 것을 고르라. 보기는 [연습 문제 8](Exercises.ko.md#연습-문제-8-순수-가상함수에-대한-설명)에 있다.

### 풀이

**정답:** ③

**해설:** 순수 가상함수는 클래스의 멤버 함수를 `virtual void draw() = 0;`처럼 선언한 것이므로, 클래스에 속하지 않는 외부 함수는 순수 가상함수가 될 수 없다. 나머지는 옳다. 순수 가상함수를 하나라도 가진 클래스는 추상 클래스이고(①), 순수 가상함수는 몸체 없이 파생 클래스가 구현해야 할 함수의 형식을 정해 두며(②), 기본 클래스의 포인터나 참조로 호출하면 동적 바인딩으로 파생 클래스가 구현한 함수가 실행된다(④)([9장 4절](Concepts_Lecture.ko.md#4-추상-클래스)).

---

<br>

## 연습 문제 9: 순수 가상함수를 구현하지 않은 파생 클래스

**교재 문항:** 9장 이론 문제 14번

**문제:** 순수 가상함수 `draw()`를 가진 `Shape`과 `draw()`를 구현하지 않은 `Circle`에 대해, 오류가 나는 선언을 고르고 `반지름=10인 원`이 출력되도록 고치라. 문제 전문과 코드는 [연습 문제 9](Exercises.ko.md#연습-문제-9-순수-가상함수를-구현하지-않은-파생-클래스)에 있다.

### 풀이

**정답:**

(1) ①, ③. 추상 클래스는 객체를 생성할 수 없다. `Shape`은 순수 가상함수를 가진 추상 클래스이고, `Circle`도 상속받은 순수 가상함수 `draw()`를 구현하지 않았으므로 추상 클래스이다. 포인터를 선언하는 ②와 ④는 객체를 만들지 않으므로 오류가 아니다.

(2) `Circle`에 `draw()`를 구현한다.

```cpp
class Circle : public Shape {
    int radius;
public:
    Circle(int radius = 1) { this->radius = radius; }
    double getArea() { return 3.14 * radius * radius; }
    void draw() { cout << "반지름=" << radius << "인 원" << endl; }
};
```

`Circle`이 `draw()`를 구현하면 더 이상 추상 클래스가 아니므로 객체를 만들 수 있다. `p->paint();`는 `Shape`의 `paint()`를 실행하고, 그 안의 `draw()` 호출이 동적 바인딩으로 `Circle::draw()`를 실행하여 `반지름=10인 원`을 출력한다.
