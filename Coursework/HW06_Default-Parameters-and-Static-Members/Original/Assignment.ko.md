# HW6 과제 안내문 — 디폴트 매개변수와 static 멤버

[레포지토리 안내](../../../README.ko.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **최종 수정일:** 2026-10-27

컴퓨터언어실습Ⅱ 3분반의 6차 과제이다. 11주차 강의자료의 실습 문제 네 개로 이루어진다.

- **제출 기한:** 2024-11-18 (월) 13:00, 수업 시작 전까지이다. 기한이 지나면 과제가 자동으로 비공개된다.
- **제출 방법:** 과목이 배포한 과제 제출 양식(.hwp)에 작성하여 블랙보드의 과제란에 제출한다.
- **작성 내용:** 코드 문제는 코드와 함께 반드시 코드 분석 내용을 작성해야 한다.

---

<br>

## 문제 1. 11주차 강의자료 [실습 2-2]

`Person` 클래스의 정의와 `main()` 함수는 다음과 같다. 디폴트 매개변수를 갖는 생성자를 이용하여 코드를 완성하라.

```cpp
class Person {
    int id;
    double weight;
    string name;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
```

**실행 예시**

```text
1 20.5 Grace
2 20.5 Ashley
3 32.5 Helen
```

강의자료는 앞의 [실습 2-1]에서 같은 클래스와 `main()`을 세 생성자의 중복으로 완성하게 하고, [실습 2-2]에서는 같은 동작을 디폴트 매개변수를 갖는 생성자로 구현하게 한다.

---

<br>

## 문제 2. 11주차 강의자료 [실습 2-3]

앞 문제의 `Person` 클래스에 회원이 몇 명인지 관리하기 위해 `private`에 `numPerson` 변수를 추가한다. 또한 이 변수를 출력할 수 있도록 관련된 함수와 `show()` 함수를 수정하라. 즉 static 멤버 변수를 추가하고, `show()`, 생성자, 소멸자를 수정하거나 추가한다. 수정된 `main()`은 다음과 같다.

```cpp
// 수정된 main 문
int main() {
    Person grace;
    grace.show();

    Person helen(13, "Helen", 32.5);
    helen.show();

    Person *p = new Person[2]{ {43, "Tera"}, {98, "Hera"} };
    p[0].show();
    p[1].show();
    Person::nPerson();

    delete[] p;
    Person::nPerson();
}
```

**실행 예시**

```text
1 20.5 Grace 1
13 32.5 Helen 2
43 20.5 Tera 4
98 20.5 Hera 4
4
2
```

---

<br>

## 문제 3. 11주차 강의자료 [실습 3]

다음과 같이 static 멤버를 가진 `Random` 클래스를 완성하고, 실행 예시와 같이 출력되도록 `main()` 코드도 완성하라.

```cpp
class Random {
public:
    // 항상 다른 랜덤수를 발생시키기
    // 위한 seed를 설정하는 함수
    static void seed() {
        srand((unsigned)time(0));
    }

    //min과 max 사이의 랜덤 정수 리턴
    static int nextInt(int min = 0, int max = 32767);
    static char nextAlphabet();

    // 0보다 크거나 같고 1보다 적은 실수 리턴
    static double nextDouble();
};

int main() {
    Random::seed();
    cout << "1에서 100까지 랜덤한 정수 10개를 출력합니다" << endl;
    for (int i = 0; i<10; i++)
        cout << Random::nextInt(1, 100) << ' ';
    cout << endl;

    cout << "알파벳을 랜덤하게 10개를 출력합니다" << endl;
    for (int i = 0; i<10; i++)
        cout << Random::nextAlphabet() << ' ';
    cout << endl;

    cout << "랜덤한 실수를 10개를 출력합니다" << endl;
    for (int i = 0; i<5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
    for (int i = 0; i<5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
}
```

**실행 예시** (값은 실행할 때마다 다르다)

```text
1에서 100까지 랜덤한 정수 10개를 출력합니다
2 27 57 16 53 2 30 25 94 7
알파벳을 랜덤하게 10개를 출력합니다
a R C I o u S K U K
랜덤한 실수를 10개를 출력합니다
0.885067 0.230293 0.232643 0.0912503 0.4785
0.247414 0.189245 0.996429 0.0416272 0.91174
```

---

<br>

## 문제 4. 11주차 강의자료 [실습 4]

`Circle` 객체가 생성될 때 현재 몇 개의 `Circle` 객체가 생성되어 있는지 확인하려고 한다.

1. `Circle` 클래스에 필요한 static 변수 `count`를 `private`에 선언하고, 초기화 코드를 추가한다.
2. 객체가 생성될 때에는 개수를 증가시키고, 객체가 정리(해제)될 때에는 감소시키도록 코드를 작성한다.

```cpp
class Circle {
    int radius;
public:
    Circle() {
        radius = 0;
    }

    Circle(int r) {
        radius = r;
    }

    ~Circle();

    void show() {
        cout << "Radius: " << radius;
    }
};

int main() {
    Circle a(5);
    a.show();

    Circle *p = new Circle[2];

    for (int i = 0; i < 2; i++)
        p[i].show();

    a.show();

    delete[] p;
    p = NULL; // 추가 권장
    a.show();
}
```

**실행 예시**

```text
Radius: 5, Count: 1
Radius: 0, Count: 3
Radius: 0, Count: 3
Radius: 5, Count: 3
Radius: 5, Count: 1
```

`show()`는 실행 예시처럼 반지름 뒤에 현재 객체 수를 함께 출력하도록 완성한다.
