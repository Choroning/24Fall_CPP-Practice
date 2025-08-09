# Chapter 8. 상속

**문항 출처:** *임영희 교수, 컴퓨터언어Ⅱ 교수자 (DCCS102-04)* <br>
**Source:** *Prof. Younghee&nbsp;Im, Instructor of COMPUTER LANGUAGE Ⅱ (DCCS102-04)*

---

## Q1. 점의 색상과 위치 정보

예시 코드와 실행 예시를 참고하여 `main()`함수가 정상적으로 실행되도록 `ColorPoint`클래스를 정의하시오. `ColorPoint`클래스는 `Point`클래스의 파생 클래스이다.


<br>**프로그램 요구사항**

- 주어진 예시 코드는 변경할 수 없습니다.


<br>**예시 코드**

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


<br>**실행 예시**

```text
RED색으로 (5, 5)에 위치한 점입니다.
BLUE색으로 (10, 20)에 위치한 점입니다.
BLACK색으로 (5, 5)에 위치한 점입니다.
GREEN색으로 (3, 7)에 위치한 점입니다.
BLACK색으로 (0, 0)에 위치한 점입니다.

Program ended with exit code: 0
```



---

## Q2. 학생 성적 관리

예시 코드와 실행 예시를 참고하여 `main()`함수가 정상적으로 실행되도록 상속 구조를 고려하여 `Student`클래스를 완성하시오. `Student`클래스는 세 파생 클래스 `FirstGrade`, `SecondGrade`, `ThirdGrade`가 공통으로 가지고 있는 요소를 포함하며, 세 클래스는 `Student`클래스 상속을 통해 중복으로 정의되는 부분을 줄여 소스코드를 최적화한다.

![FirstGrade, SecondGrade, ThirdGrade 클래스에 관한 설명이 담긴 사진. 각 클래스는 멤버변수로 이름, 과목별 성적을 가지고 있으며 멤버함수로 이름과 성적 평균에 대한 get함수와 성적 출력 함수를 가지고 있다.](/src/images/C08_A10204-1_1.png)

테이블의 각 요소에 대해 빨간색 하이라이트는 `private` 접근 제어, 초록색 하이라이트는 `protected` 접근 제어한다.


<br>**프로그램 요구사항**

- 주어진 예시 코드는 변경할 수 없습니다.


<br>**예시 코드**

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


<br>**실행 예시**

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



---

## Q3. 

예시 코드와 실행 예시를 참고하여 `main()`함수가 정상적으로 실행되도록 `Time`클래스의 연산자 함수를 완성하시오.


<br>**프로그램 요구사항**

1. 주어진 예시 코드는 변경할 수 없습니다.
2. 모든 Time 객체는 normalized 상태를 유지해야 합니다. (`분`, `초`가 `0~59` 사이)
3. 아래의 조건을 만족하는 각 연산자 함수를 작성하시오.
      - 멤버함수로 작성해야 하는 연산자 함수<br>
        : `객체 - 객체` (두 객체의 시간 차), <br>
          `객체 <= 객체`, <br>
          `객체[정수]`
      - 프렌드함수로 작성해야 하는 연산자 함수<br>
        : `cout << 객체`, <br>
          `객체 += 객체`


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Time {
   int hour;
   int min;
   int sec;
   void normalize();    // 분, 초를 0~59 사이로 조정
public:
   Time(int h = 0; int m = 0; int s = 0);
   // 연산자 함수 선언
};
Time::Time(int h, int m, int s) {
   hour = h; min = m; sec = s;
   normalize();
}
// normalize() 함수 및 연산자 함수 정의

int main() {
   Time t1(20, 75, 63), t2(2, 50, 59), t3;

   cout << "t1 = " << t1 << "\nt2 = " << t2 << "\n\n";

   cout << t1 << " + " << t2 << " = ";
   t1 += t2;
   cout << t1 << "\n\n";

   t3 = t1 - t2;
   cout << t1 << " - " << t2 << " = " << t3 << "\n\n";

   t3 = t2 - t1;
   cout << t2 << " - " << t1 << " = " << t3 << "\n\n";

   cout << t2 << " - " << t2 << " = " << t2 - t2 << "\n\n";

   t3[0] = 3;     // t3의 시간을 3으로 변환
   t3[1] = 9;     // t3의 분을 9로 변환
   t3[2] = 27;    // t3의 초를 27로 변환
   cout << "변환 후 t3 = " << t3 << "\n\n";

   Time t4(11, 18, 27), t5(11, 18, 20);
   if (t4 <= t5)
      cout << t4 << " <= " << t5 << "\n\n";
   else
      cout << t4 << " > " << t5 << "\n\n";

   Time t6(5, 15, 10), t7(5, 17, 20);
   if (t6 <= t7)
      cout << t6 << " <= " << t7 << "\n\n";
   else
      cout << t6 << " > " << t7 << "\n\n";
   
   return 0;
}
```


<br>**실행 예시**

```text
t1 = (21:16:3)
t2 = (2:50:59)

(21:16:3) + (2:50:59) = (24:7:2)

(24:7:2) - (2:50:59) = (21:16:3)

(2:50:59) - (24:7:2) = (21:16:3)

(2:50:59) - (2:50:59) = (0:0:0)

변환 후 t3 = (3:9:27)

(11:18:27) > (11:18:20)

(5:15:10) <= (5:17:20)

Program ended with exit code: 0
```



---