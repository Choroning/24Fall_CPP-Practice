# Chapter 8 Exercises — Inheritance

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 8

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Exercise 1: Color and Position of a Point

**Problem:** Referring to the example code and the sample run, define the `ColorPoint` class so that `main()` runs correctly. `ColorPoint` is a derived class of `Point`. The problem is based on programming exercises 3 and 4 of Chapter 8 of the textbook.

**Program Requirements**

- Fill in the [ 빈칸 ] (blank) part; the given example code must not be changed.

**Example Code**

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
    // definition of the ColorPoint class
    [ 빈칸 ]
};

int main() {
    ColorPoint cp1(5, 5, "RED");  // color "RED", position (5, 5)
    cp1.show();
    cp1.setPoint(10, 20);         // sets position to (10, 20)
    cp1.setColor("BLUE");         // sets color to "BLUE"
    cp1.show();

    ColorPoint cp2(5, 5);         // color "BLACK", position (5, 5)
    cp2.show();
    cp2.setPoint(3, 7);           // sets position to (3, 7)
    cp2.setColor("GREEN");        // sets color to "GREEN"
    cp2.show();

    ColorPoint zeroPoint;         // color "BLACK", position (0, 0)
    zeroPoint.show();

    return 0;
}
```

**Sample Run**

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.

Program ended with exit code: 0
```

**[Solution](Solutions.md#exercise-1-color-and-position-of-a-point)**

---

<br>

## Exercise 2: Student Grade Management

**Problem:** Referring to the example code and the sample run, and considering the inheritance structure, complete the `Student` class and its derived classes so that `main()` runs correctly. The `Student` class holds the elements shared by the three derived classes `FirstGrade`, `SecondGrade`, and `ThirdGrade`, and the three derived classes are implemented by inheriting from `Student`.

![Figure 1. Member variables and member functions of the FirstGrade, SecondGrade, and ThirdGrade classes](../../images/Ch08_Grade-Classes.png)

*Figure 1. Member variables and member functions of the FirstGrade, SecondGrade, and ThirdGrade classes*

All three classes have the name (`name`) and the Korean (`kor`) and informatics (`pro`) scores as member variables; `FirstGrade` also has a math score (`mat`), `SecondGrade` science (`sci`) and ethics (`mor`) scores, and `ThirdGrade` English (`eng`) and Korean history (`his`) scores. The member functions of all three classes are a constructor, `getName()` returning the name, `getAverage()` returning the average score, and `display()` printing the scores.

In Figure 1, the element highlighted in red (the name) is `private`, and the elements highlighted in green (the subject scores) are `protected`.

**Program Requirements**

- The given example code must not be changed.

**Example Code**

```cpp
#include <iostream>
using namespace std;

int main() {
    // initializes name and Korean, informatics, and math scores
    FirstGrade f("홍길동", 90, 81, 88);

    // initializes name and Korean, informatics, science, and ethics scores
    SecondGrade s("장영실", 73, 91, 99, 68);
    
    // initializes name and Korean, informatics, English, and Korean history scores
    ThirdGrade t("정도전", 81, 53, 80, 96);
    
    f.display();
    s.display();
    t.display();
    
    return 0;
}
```

**Sample Run**

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

**[Solution](Solutions.md#exercise-2-student-grade-management)**
