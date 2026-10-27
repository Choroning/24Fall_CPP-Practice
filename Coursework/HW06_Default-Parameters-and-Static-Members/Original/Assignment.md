# HW6 Assignment Handout — Default Parameters and Static Members

[Repository Guide](../../../README.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **Last Updated:** 2026-10-27

This is the sixth assignment of section 3 of Computer Language and Training Ⅱ. It consists of four lab problems from the Week 11 lecture slides.

- **Deadline:** 2024-11-18 (Mon) 13:00, before the class starts. After the deadline the assignment is hidden automatically.
- **Submission:** Write the answers in the assignment template (.hwp) distributed by the course and submit it in the assignment area of Blackboard.
- **Contents:** For a coding problem, an analysis of the code must be written together with the code.

---

<br>

## Problem 1. Week 11 Lecture Slides [Lab 2-2]

The definition of the `Person` class and the `main()` function are as follows. Complete the code using a constructor with default parameters.

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

**Sample Run**

```text
1 20.5 Grace
2 20.5 Ashley
3 32.5 Helen
```

The slides first have the same class and `main()` completed with three overloaded constructors in the preceding [Lab 2-1], and in [Lab 2-2] they have the same behavior implemented with a constructor with default parameters.

---

<br>

## Problem 2. Week 11 Lecture Slides [Lab 2-3]

To keep track of how many members there are, add a variable `numPerson` under `private` to the `Person` class of the previous problem. Also modify the related functions and the `show()` function so that this variable can be printed. That is, add a static member variable, and modify or add `show()`, the constructors, and the destructor. The modified `main()`, marked by the comment "modified main" in Korean, is as follows.

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

**Sample Run**

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

## Problem 3. Week 11 Lecture Slides [Lab 3]

Complete the `Random` class with static members as follows, and also complete the `main()` code so that it prints the output as in the sample run. The comments say that `seed()` sets the seed so that different random numbers are always generated, that `nextInt()` returns a random integer between `min` and `max`, and that `nextDouble()` returns a real number greater than or equal to 0 and less than 1. The three guidance lines of `main()` say in Korean that 10 random integers from 1 to 100, 10 random letters, and 10 random real numbers are printed.

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

**Sample Run** (the values change with every run)

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

## Problem 4. Week 11 Lecture Slides [Lab 4]

The goal is to check how many `Circle` objects currently exist whenever a `Circle` object is created.

1. Declare the static variable `count` needed by the `Circle` class under `private`, and add the code that initializes it.
2. Write the code so that the count increases when an object is created and decreases when an object is cleaned up (released).

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

**Sample Run**

```text
Radius: 5, Count: 1
Radius: 0, Count: 3
Radius: 0, Count: 3
Radius: 5, Count: 3
Radius: 5, Count: 1
```

`show()` is completed so that it prints the current number of objects after the radius, as in the sample run. The comment on `p = NULL;` in `main()` says that adding the line is recommended.
