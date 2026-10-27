# 7장 연습 문제 — 프렌드 함수와 연산자 중복

[레포지토리 안내](../../README.ko.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **최종 수정일:** 2026-10-27

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 연습 문제 1: 멤버 함수를 이용한 분수 클래스의 연산자 중복

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `Fraction` 클래스의 연산자 함수를 완성하라.

**프로그램 요구사항**

1. [ 빈칸 ]을 완성하되, 주어진 예시 코드는 변경할 수 없다.
2. 분모는 `0`이 될 수 없으며, 최종 출력된 분수가 기약분수의 형태가 아니어도 상관없다.
3. 다음 조건을 만족하는 각 연산자 함수를 작성한다.
   - 멤버 함수로 작성해야 하는 이항 연산자 함수: `분수 + 분수`, `분수 == 분수`, `분수 ^ 정수` (분수의 정수 제곱), `분수 *= 분수`, `분수 / 정수`
   - 멤버 함수로 작성해야 하는 단항 연산자 함수: `~분수` (분수의 역수), `분수++` (후위 연산), `--분수` (전위 연산)

**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Fraction {
    int num;    // 분자(numerator)
    int denom;  // 분모(denominator)
public:
    Fraction() { num = denom = 1; }
    Fraction(int n, int d) { num = n; denom = d; }
    friend ostream& operator<<(ostream& stream, Fraction f) {
        stream << f.num << "/" << f.denom;
        return stream;
    }
    // 연산자 함수 선언
    [ 빈칸 ]
};

// 연산자 함수 정의
[ 빈칸 ]

int main() {
    Fraction f1(3, 4), f2(2, 5), f3, f4(7, 15), f5(6, 15);
    
    f3 = f1 + f2;   // 분수 + 분수
    cout << f1 << " + " << f2 << " = " << f3 << "\n\n";
    
    if (f2 == f5) cout << f2 << " == " << f5 << "\n\n";
    if (f2 == f4) cout << f2 << " == " << f4 << "\n\n";
    
    f1 *= f2;       // 분수 *= 분수
    cout << "f1 *= f2 " << "===> f1 = " << f1 << "\n\n";
    
    f3 = f2 ^ 3;    // 분수 ^ 정수 (분수의 정수 제곱)
    cout << "( " << f2 << " )^3 = " << f3 << "\n\n";
    
    f3 = f2 / 5;    // 분수 / 정수
    cout << f2 << " / 5 = " << f3 << "\n\n";
    
    f3 = ~f2;       // ~ 분수 (분수의 역수)
    cout << "~( " << f2 << " ) = " << f3 << "\n";
    cout << "f2는 변하지 않음! f2 = " << f2 << "\n\n";
    
    cout << "(" << f1 << ")++  = ";
    f4 = f1++;
    cout << f1 << "\n";
    cout << "후위연산 수행 후, f4 = " << f4 << "\n\n";
    
    cout << "--(" << f2 << ")  = ";
    f4 = --f2;
    cout << f2 << "\n";
    cout << "전위연산 수행 후, f4 = " << f4 << "\n\n";
    
    return 0;
}
```

**실행 예시**

```text
3/4 + 2/5 = 23/20

2/5 == 6/15

f1 *= f2 ===> f1 = 6/20

( 2/5 )^3 = 8/125

2/5 / 5 = 2/25

~( 2/5 ) = 5/2
f2는 변하지 않음! f2 = 2/5

(6/20)++  = 26/20
후위연산 수행 후, f4 = 6/20

--(2/5)  = -3/5
전위연산 수행 후, f4 = -3/5

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-1-멤버-함수를-이용한-분수-클래스의-연산자-중복)**

---

<br>

## 연습 문제 2: 프렌드 함수를 이용한 복소수 클래스의 연산자 중복

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `Complex` 클래스의 연산자 함수를 완성하라.

**프로그램 요구사항**

1. [ 빈칸 ]을 완성하되, 주어진 예시 코드는 변경할 수 없다.
2. 다음 조건을 만족하는 각 연산자 함수를 작성한다.
   - 프렌드 함수로 작성해야 하는 이항 연산자 함수: `정수 + 복소수`, `복소수 * 복소수`, `복소수 -= 복소수`, `복소수 != 복소수`
   - 프렌드 함수로 작성해야 하는 단항 연산자 함수: `~복소수` (켤레복소수), `복소수--` (후위 연산), `++복소수` (전위 연산)

**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Complex {
    int real;         // 실수부
    int imaginary;    // 허수부
public:
    Complex(int r = 0, int i = 0) { real = r; imaginary = i; }
    friend ostream& operator<<(ostream& stream, Complex op2);
    // 연산자 함수 선언
    [ 빈칸 ]
};
ostream& operator<<(ostream& stream, Complex op2) {
    stream << op2.real;
    if (op2.imaginary >= 0)
        stream << "+";
    stream << op2.imaginary << "i";

    return stream;
}
// 연산자 함수 정의
[ 빈칸 ]

int main() {
    Complex a(-5, 7), b(2, -3), c;

    c = 10 + b;
    cout << "[c = 10 + b 실행 후]\n";
    cout << "10 + b = 10 + (" << b << ") = " << c << "\n\n";

    c = a * b;
    cout << "[c = a * b 실행 후]\n";
    cout << "a * b = (" << a << ") * (" << b << ") = " << c << "\n\n";
   
    a -= b;
    cout << "[a -= b 실행 후]\n";
    cout << "a = " << a << "\t b = " << b << "\n\n";
   
    c = ~ b;
    cout << "[c = ~b 실행 후]\n";
    cout << "c = " << c << "\t b = " << b << "\n\n";
   
    c = b--;
    cout << "[c = b-- 실행 후]\n";
    cout << "c = " << c << "\t b = " << b << "\n\n";
   
    if (c != b) 
        cout << "[c != b]\n\n";
   
    c = ++b;
    cout << "[c = ++b 실행 후]\n";
    cout << "c = " << c << "\t b = " << b << "\n\n";
   
    return 0;
}
```

예시 코드의 `operator<<`에 있는 `if`문은 허수부가 0 이상일 때 `+`를 출력한다. 실수부가 1이고 허수부가 -2이면 음수 부호가 함께 출력되어 `1-2i`가 되지만, 허수부가 3이면 `if`문이 없을 때 `13i`처럼 `+` 없이 출력되므로 이 경우를 처리하기 위한 부분이다.

**실행 예시**

```text
[c = 10 + b 실행 후]
10 + b = 10 + (2-3i) = 12-3i

[c = a * b 실행 후]
a * b = (-5+7i) * (2-3i) = 11+29i

[a -= b 실행 후]
a = -7+10i     b = 2-3i

[c = ~b 실행 후]
c = 2+3i       b = 2-3i

[c = b-- 실행 후]
c = 2-3i       b = 1-3i

[c != b]

[c = ++b 실행 후]
c = 2-3i       b = 2-3i

Program ended with exit code: 0
```

**[풀이](Solutions.ko.md#연습-문제-2-프렌드-함수를-이용한-복소수-클래스의-연산자-중복)**

---

<br>

## 연습 문제 3: 시간 클래스의 연산자 중복

**문제:** 예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록, 시간의 길이를 나타내는 `Time` 클래스의 연산자 함수를 완성하라.

**프로그램 요구사항**

1. [ 빈칸 ]을 완성하되, 주어진 예시 코드는 변경할 수 없다.
2. 모든 `Time` 객체는 정규화(normalize)된 상태, 곧 분과 초가 0~59 사이인 상태를 유지해야 한다(실행 예시의 첫 번째 줄 참고).
3. 다음 조건을 만족하는 각 연산자 함수를 작성한다.
   - 멤버 함수로 작성해야 하는 연산자 함수: `객체 - 객체` (두 객체의 시간 차), `객체 <= 객체`, `객체[정수]`
   - 프렌드 함수로 작성해야 하는 연산자 함수: `cout << 객체`, `객체 += 객체`
4. `객체 - 객체`는 빼는 순서와 관계없이 두 시간의 차를 구한다. 따라서 실행 예시에서 `t1 - t2`와 `t2 - t1`의 결과가 같다.

**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Time {
    int hour;
    int min;
    int sec;
    void normalize();    // 분, 초를 0~59 사이로 조정
public:
    Time(int h = 0, int m = 0, int s = 0);
    // 연산자 함수 선언
    [ 빈칸 ]
};
Time::Time(int h, int m, int s) {
    hour = h; min = m; sec = s;
    normalize();
}
// normalize() 함수 및 연산자 함수 정의
[ 빈칸 ]

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

**실행 예시**

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

**[풀이](Solutions.ko.md#연습-문제-3-시간-클래스의-연산자-중복)**

---

<br>

## 교재 이론 문제

**교재:** 7장 연습문제의 이론 문제 중 짝수 번 문항

### 이론 문제 2

프렌드 함수가 필요한 경우가 아닌 것은?

- ① 멤버는 아니지만 클래스의 `private` 멤버에 접근해야만 하는 함수를 작성하는 경우
- ② 두 개 이상의 클래스에 대해 `private` 멤버를 동시에 접근하는 함수를 작성하는 경우
- ③ 연산자 중복 시에
- ④ 함수 중복 시에

**[풀이](Solutions.ko.md#이론-문제-2)**

### 이론 문제 4

클래스 `SampleManager`에는 다음 멤버 함수가 있다. 이 멤버 함수를 클래스 `Sample`에 프렌드로 초대하도록 선언하라.

```cpp
bool compare(Sample &a, Sample &b);
```

```cpp
class Sample {
    ...
};
```

**[풀이](Solutions.ko.md#이론-문제-4)**

### 이론 문제 6

다음 프로그램은 컴파일 오류가 발생한다. 오류의 원인은 무엇인가? 오류를 바람직하게 수정하라.

```cpp
class Student {
    int id;
public:
    Student(int id) { this->id = id; }
};
class Professor {
    string name;
public:
    Professor(string name) { this->name = name; }
};
void show(Student s, Professor p) {
    cout << s.id << p.name;
}
```

교재는 풀이 중에 생기는 전방 참조 문제를 해결하기 위해 코드의 맨 앞에 `class Student;`와 `class Professor;`의 두 줄을 전방 선언으로 넣으라는 힌트를 준다.

**[풀이](Solutions.ko.md#이론-문제-6)**

### 이론 문제 8

프렌드 선언의 위치에 대한 설명 중 옳은 것은?

- ① 클래스 내의 `private` 영역에 선언되어야 한다.
- ② 클래스 내의 `protected` 영역에 선언되어야 한다.
- ③ 클래스 내의 `public` 영역에 선언되어야 한다.
- ④ 클래스 내의 아무 영역에 선언되어도 상관없다.

**[풀이](Solutions.ko.md#이론-문제-8)**

### 이론 문제 10

다음 코드에서 프렌드 선언이 필요한지 설명하라.

```cpp
class Sample {
public:
    int x;
    Sample(int x) { this->x = x; }
    friend bool isZero(Sample &a);   // 이곳에 주목
};
bool isZero(Sample& a) {
    if (a.x == 0) return true;
    else return false;
}
```

**[풀이](Solutions.ko.md#이론-문제-10)**

### 이론 문제 12

다음에 보이는 다형성에 일치하는 다른 사례를 하나만 들어라.

```text
2 + 3 = 5
빨강 + 파랑 = 보라
남자 + 여자 = 결혼
```

**[풀이](Solutions.ko.md#이론-문제-12)**

### 이론 문제 14

`Power` 클래스의 객체 `a`, `b`에 대해, 다음 중 멤버 함수로 `+` 연산자 함수를 작성할 수 없는 경우는?

```cpp
Power a, b;   // Power 클래스에 대해
```

- ① `a = a + b`
- ② `b = a + 3`
- ③ `b = a += 3`
- ④ `b = 3 + a`

**[풀이](Solutions.ko.md#이론-문제-14)**

### 이론 문제 16

`a`, `b`는 `Power` 클래스의 객체이다. 연산자 함수를 `Power` 클래스의 프렌드 함수로 작성한다고 할 때, 왼쪽의 연산과 오른쪽의 연산자 함수 선언이 잘못 짝지어진 것은?

- ① `a + b`: `Power operator + (Power& a, Power &b);`
- ② `a == b`: `bool operator == (Power a, Power b);`
- ③ `a++`: `Power operator ++ (Power a, int b);`
- ④ `a = b`: `Power& operator = (Power &a, Power b);`

**[풀이](Solutions.ko.md#이론-문제-16)**
