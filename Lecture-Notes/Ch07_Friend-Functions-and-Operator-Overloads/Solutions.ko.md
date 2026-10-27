# 7장 연습 문제 풀이 — 프렌드 함수와 연산자 중복

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 멤버 함수를 이용한 분수 클래스의 연산자 중복](#연습-문제-1-멤버-함수를-이용한-분수-클래스의-연산자-중복)
- [연습 문제 2: 프렌드 함수를 이용한 복소수 클래스의 연산자 중복](#연습-문제-2-프렌드-함수를-이용한-복소수-클래스의-연산자-중복)
- [연습 문제 3: 시간 클래스의 연산자 중복](#연습-문제-3-시간-클래스의-연산자-중복)
- [교재 이론 문제](#교재-이론-문제)

---

<br>

## 연습 문제 1: 멤버 함수를 이용한 분수 클래스의 연산자 중복

**문제:** 분자 `num`과 분모 `denom`을 가진 `Fraction` 클래스에 이항 연산자 `+`, `==`, `^`(정수 제곱), `*=`, `/`(정수로 나누기)와 단항 연산자 `~`(역수), 후위 `++`, 전위 `--`를 모두 멤버 함수로 작성하여 주어진 `main()`이 실행되게 한다. 전체 문제와 실행 예시는 [연습 문제 1](Exercises.ko.md#연습-문제-1-멤버-함수를-이용한-분수-클래스의-연산자-중복)에 있다.

### 풀이

모든 연산자가 멤버 함수이므로 좌측 피연산자는 `*this`이고, 이항 연산자의 우측 피연산자는 매개변수 `op`로 받는다. 각 연산자는 분수의 연산 규칙을 그대로 옮긴다.

| 연산자 | 구현 방법 | 실행 예시의 계산 |
|:----|:----|:----|
| `+` | $\frac{a}{b} + \frac{c}{d} = \frac{ad + bc}{bd}$로 새 객체 `tmp`를 만들어 반환 | 3/4 + 2/5 = (15 + 8)/20 = 23/20 |
| `==` | 교차 곱 $a \times d$와 $b \times c$가 같으면 `true` | 2/5와 6/15는 30 = 30이므로 같고, 2/5와 7/15는 30 ≠ 35이므로 다름 |
| `*=` | 자신의 분자와 분모에 각각 상대의 분자와 분모를 곱하고 `*this`를 반환 | 3/4 × 2/5 = 6/20 |
| `^` | 기본 생성자로 만든 1/1에 분자와 분모를 `op`번 곱함 | $(2/5)^3 = 8/125$ |
| `/` | 분모에 정수를 곱한 새 객체를 반환 | 2/5 ÷ 5 = 2/25 |
| `~` | 분자와 분모를 바꾼 새 객체를 반환하므로 원본은 그대로 | `~(2/5)` = 5/2 |
| 후위 `++` | 수정 전 상태를 `tmp`에 저장하고, 분자에 분모를 더해 1을 더한 뒤 `tmp`를 반환 | 6/20 + 1 = 26/20, 반환값은 6/20 |
| 전위 `--` | 분자에서 분모를 빼 1을 뺀 뒤 `*this`를 반환 | 2/5 - 1 = -3/5, 반환값도 -3/5 |

*표 1. `Fraction` 연산자 함수의 구현 방법과 계산*

후위 `++`는 전위와 구분하기 위해 사용하지 않는 `int` 매개변수 `notUsed`를 받는다. `*=`와 전위 `--`는 `Fraction&`가 아닌 `Fraction`을 값으로 반환하므로 수정된 객체의 복사본이 반환된다. 이 `main()`은 반환값을 연쇄하지 않으므로 결과는 같지만, [7장 3.3절](Concepts_Lecture.ko.md#33-복합-대입-연산자)과 [7장 4.3절](Concepts_Lecture.ko.md#43-증가--감소-연산자)처럼 참조로 반환하는 것이 일반적인 형태이다.

작성한 코드는 다음과 같다([`Lab_Q1.cpp`](../../Coursework/Lab07_Friend-Functions-and-Operator-Overloads/Lab_Q1.cpp)).

```cpp
/**
 * @file    A10204-1_C07Q1.cpp
 * @brief   Q1. 멤버함수를 이용한 분수 클래스의 연산자 중복
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-12
 */

#include <iostream>
using namespace std;

class Fraction {
    int num;    // 분자
    int denom;  // 분모
public:
    Fraction() { num = denom = 1; }
    Fraction(int n, int d) { num = n; denom = d; }
    friend ostream& operator<<(ostream& stream, Fraction f) {
        stream << f.num << "/" << f.denom;
        return stream;
    }
    // 연산자 함수 선언
    // 이항 연산자 함수
    Fraction operator+(Fraction op);
    bool operator==(Fraction op);
    Fraction operator^(int op);
    Fraction operator*=(Fraction op);
    Fraction operator/(int op);
    
    // 단항 연산자 함수
    Fraction operator~();
    Fraction operator++(int notUsed);
    Fraction operator--();
};

// 연산자 함수 정의
// 이항 연산자 함수
Fraction Fraction::operator+(Fraction op) {
    Fraction tmp;
    tmp.num = num * op.denom + denom * op.num;
    tmp.denom = denom * op.denom;
    return tmp;
}
bool Fraction::operator==(Fraction op) {
    if (num * op.denom == denom * op.num)
        return true;
    else
        return false;
}
Fraction Fraction::operator^(int op) {
    Fraction tmp;
    for (int i = 0; i < op; i++) {
        tmp.num = tmp.num * num;
        tmp.denom = tmp.denom * denom;
    }
    return tmp;
}
Fraction Fraction::operator*=(Fraction op) {
    num = num * op.num;
    denom = denom * op.denom;
    return *this;
}
Fraction Fraction::operator/(int op) {
    Fraction tmp;
    tmp.num = num;
    tmp.denom = denom * op;
    return tmp;
}

// 단항 연산자 함수
Fraction Fraction::operator~() {
    Fraction tmp;
    tmp.num = denom;
    tmp.denom = num;
    return tmp;
}
Fraction Fraction::operator++(int notUsed) {
    Fraction tmp;
    tmp.num = num;
    tmp.denom = denom;
    num = num + denom;
    return tmp;
}
Fraction Fraction::operator--() {
    num = num - denom;
    return *this;
}

int main() {
    Fraction f1(3, 4), f2(2, 5), f3, f4(7, 15), f5(6, 15);
    
    f3 = f1 + f2;   // 분수 + 분수
    cout << f1 << " + " << f2 << " = " << f3 << "\n\n";
    
    if (f2 == f5) cout << f2 << " == " << f5 << "\n\n";
    if (f2 == f4) cout << f2 << " == " << f4 << "\n\n";
    
    f1 *= f2;       // 분수 *= 분수
    cout << "f1 *= f2 " << "===> f1 = " << f1 << "\n\n";
    
    f3 = f2 ^ 3;    // 분수 ^ 정수 (분수의 정수 제곱승 구현)
    cout << "( " << f2 << " )^3 = " << f3 << "\n\n";
    
    f3 = f2 / 5;    // 분수 / 정수
    cout << f2 << " / 5 = " << f3 << "\n\n";
    
    f3 = ~f2;       // ~ 분수 (분수의 역수 계산)
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

실행 결과는 다음과 같다.

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
```

실행 예시의 마지막 줄 `Program ended with exit code: 0`은 프로그램이 끝난 뒤 Xcode가 출력하는 메시지이며 프로그램의 출력이 아니다.

---

<br>

## 연습 문제 2: 프렌드 함수를 이용한 복소수 클래스의 연산자 중복

**문제:** 실수부 `real`과 허수부 `imaginary`를 가진 `Complex` 클래스에 이항 연산자 `정수 + 복소수`, `*`, `-=`, `!=`와 단항 연산자 `~`(켤레복소수), 후위 `--`, 전위 `++`를 모두 프렌드 함수로 작성하여 주어진 `main()`이 실행되게 한다. 전체 문제와 실행 예시는 [연습 문제 2](Exercises.ko.md#연습-문제-2-프렌드-함수를-이용한-복소수-클래스의-연산자-중복)에 있다.

### 풀이

프렌드 함수는 멤버가 아니므로 모든 피연산자를 매개변수로 받으며, 클래스 안의 `friend` 선언 덕분에 private 멤버 `real`과 `imaginary`에 접근한다. 피연산자를 수정하는 연산자(`-=`, 후위 `--`, 전위 `++`)는 그 피연산자를 참조(`Complex&`)로 받아야 원본이 바뀐다.

| 연산자 | 구현 방법 | 실행 예시의 계산 |
|:----|:----|:----|
| `정수 + 복소수` | 좌측 피연산자가 `int`이므로 멤버로 만들 수 없고, 정수를 실수부에만 더함 | 10 + (2-3i) = 12-3i |
| `*` | $(a + bi)(c + di) = (ac - bd) + (ad + bc)i$ | (-5+7i)(2-3i) = (-10 + 21) + (15 + 14)i = 11+29i |
| `-=` | 첫 번째 매개변수를 참조로 받아 실수부와 허수부를 각각 빼고, 그 참조를 반환 | (-5+7i) - (2-3i) = -7+10i |
| `!=` | 실수부와 허수부가 모두 같으면 `false`, 아니면 `true` | `c = 2-3i`, `b = 1-3i`이므로 `true` |
| `~` | 매개변수를 값으로 받아 그 복사본의 허수부 부호를 바꿔 반환하므로 원본은 그대로 | `~(2-3i)` = 2+3i |
| 후위 `--` | 수정 전 상태를 `tmp`에 저장하고 실수부를 1 줄인 뒤 `tmp`를 반환 | `b`는 1-3i, 반환값은 2-3i |
| 전위 `++` | 실수부를 1 늘리고 그 참조를 반환 | `b`와 `c` 모두 2-3i |

*표 2. `Complex` 연산자 함수의 구현 방법과 계산*

증가와 감소는 복소수에 실수 1을 더하거나 빼는 연산으로 보아 실수부만 바꾼다. 실행 예시에서 `b--`가 2-3i를 1-3i로 바꾸는 것도 이와 같다. `~`의 주석 처리된 줄 `op.imaginary *= (-1);`은 바로 아래 줄과 같은 결과를 내는 다른 표현이다.

작성한 코드는 다음과 같다([`Lab_Q2.cpp`](../../Coursework/Lab07_Friend-Functions-and-Operator-Overloads/Lab_Q2.cpp)).

```cpp
/**
 * @file    A10204-1_C07Q2.cpp
 * @brief   Q2. 프렌드함수를 이용한 복소수 클래스의 연산자 중복
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-12
 */

#include <iostream>
using namespace std;

class Complex {
    int real;           // 실수부
    int imaginary;      // 허수부
public:
    Complex(int r = 0, int i = 0) { real = r; imaginary= i; }
    friend ostream& operator<<(ostream& stream, Complex op2);
    friend Complex operator+(int op1, Complex op2);
    friend Complex operator*(Complex op1, Complex op2);
    friend Complex& operator-=(Complex& op1, Complex op2);
    friend bool operator!=(Complex op1, Complex op2);
    friend Complex operator~(Complex op);
    friend Complex operator--(Complex& op, int notUsed);
    friend Complex& operator++(Complex& op);
};

ostream& operator<<(ostream& stream, Complex op2) {
    stream << op2.real;
    if (op2.imaginary >= 0)
            stream << "+";
    stream << op2.imaginary << "i";
    return stream;
}

Complex operator+(int op1, Complex op2) {
    Complex tmp;
    tmp.real = op1 + op2.real;
    tmp.imaginary = op2.imaginary;
    return tmp;
}
Complex operator*(Complex op1, Complex op2) {
    Complex tmp;
    tmp.real = (op1.real * op2.real - op1.imaginary * op2.imaginary);
    tmp.imaginary = (op1.real * op2.imaginary + op1.imaginary * op2.real);
    return tmp;
}
Complex& operator-=(Complex& op1, Complex op2) {
    op1.real = op1.real - op2.real;
    op1.imaginary = op1.imaginary - op2.imaginary;
    return op1;
}
bool operator!=(Complex op1, Complex op2) {
    if ((op1.real == op2.real) && (op1.imaginary == op2.imaginary))
        return false;
    else
        return true;
}
Complex operator~(Complex op) {
    //op.imaginary *= (-1);
    op.imaginary = (-op.imaginary);
    return op;
}
Complex operator--(Complex& op, int notUsed) {
    Complex tmp = op;
    op.real--;
    return tmp;
}
Complex& operator++(Complex& op) {
    op.real++;
    return op;
}

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
    
    if (c != b) cout << "[c != b]\n\n";
    
    c = ++b;
    cout << "[c = ++b 실행 후]\n";
    cout << "c = " << c << "\t b = " << b << "\n\n";
    
    return 0;
}
```

실행 결과는 다음과 같다.

```text
[c = 10 + b 실행 후]
10 + b = 10 + (2-3i) = 12-3i

[c = a * b 실행 후]
a * b = (-5+7i) * (2-3i) = 11+29i

[a -= b 실행 후]
a = -7+10i       b = 2-3i

[c = ~b 실행 후]
c = 2+3i         b = 2-3i

[c = b-- 실행 후]
c = 2-3i         b = 1-3i

[c != b]

[c = ++b 실행 후]
c = 2-3i         b = 2-3i
```

두 값 사이의 간격은 탭 문자와 공백(`"\t "`)으로 출력되므로 위 결과는 탭 너비를 8로 펼친 것이며, 터미널의 탭 설정에 따라 실행 예시와 간격이 다르게 보일 수 있다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 출력하는 메시지이다.

---

<br>

## 연습 문제 3: 시간 클래스의 연산자 중복

**문제:** 시, 분, 초(`hour`, `min`, `sec`)로 시간의 길이를 나타내는 `Time` 클래스에 `normalize()` 함수와 멤버 연산자 `객체 - 객체`(시간 차), `<=`, `[]`, 프렌드 연산자 `cout << 객체`, `+=`를 작성하여 주어진 `main()`이 실행되게 한다. 모든 객체는 분과 초가 0~59 사이인 상태를 유지해야 한다. 전체 문제와 실행 예시는 [연습 문제 3](Exercises.ko.md#연습-문제-3-시간-클래스의-연산자-중복)에 있다.

### 풀이

`normalize()`는 초가 60 이상이면 60을 빼고 분을 1 늘리며, 음수이면 60을 더하고 분을 1 줄인다. 분도 같은 방식으로 시에 반영한다. 생성자와 `+=`, `-`가 결과를 만든 뒤 `normalize()`를 호출하므로 객체가 항상 정규화된 상태로 유지된다. 예를 들어 `t1(20, 75, 63)`은 63초가 3초와 1분으로, 76분이 16분과 1시간으로 바뀌어 (21:16:3)이 된다.

| 연산자 | 구현 방법 | 실행 예시의 계산 |
|:----|:----|:----|
| `+=` (프렌드) | 시, 분, 초를 각각 더하고 `normalize()`한 뒤 첫 번째 매개변수의 참조를 반환 | (21:16:3) + (2:50:59) = 23시 66분 62초 → (24:7:2) |
| `-` (멤버) | `<=`로 크기를 비교해 큰 시간에서 작은 시간을 빼고 `normalize()`하므로 빼는 순서와 관계없이 같은 결과 | 22시 -43분 -57초 → (21:16:3) |
| `<=` (멤버) | 두 시간을 초 단위(`hour * 3600 + min * 60 + sec`)로 바꿔 비교 | 40707초 > 40700초이므로 (11:18:27) > (11:18:20) |
| `[]` (멤버) | 첨자 0, 1, 그 밖의 값에 따라 `hour`, `min`, `sec`의 참조(`int&`)를 반환하므로 대입할 수 있음 | `t3[0] = 3` 등으로 (3:9:27) |
| `<<` (프렌드) | `(시:분:초)` 형식으로 출력하고 스트림을 반환 | (2:50:59) |

*표 3. `Time` 연산자 함수의 구현 방법과 계산*

시 부분은 24를 넘을 때만 24를 빼므로, 합이 정확히 24시간인 (24:7:2)는 그대로 출력된다. `[]`로 대입한 값은 `normalize()`를 거치지 않지만, 이 `main()`이 대입하는 3, 9, 27은 이미 범위 안에 있다.

> **참고:** **한 번만 조정하는 `normalize()`.** 이 `normalize()`는 초와 분을 각각 최대 한 번만 60만큼 조정하므로, 정규화 전의 값이 -60보다 작거나 120 이상이면 정규화가 끝나지 않는다(예: `Time(0, 0, 130)`은 70초가 남는다). 이 `main()`에서는 생성자 인자가 120보다 작고, 정규화된 두 값의 합은 받아올림을 더해도 최대 119, 차는 받아내림을 빼도 최소 -60이므로 한 번의 조정으로 충분하다. 임의의 값을 받으려면 `while` 문으로 반복하거나 총 초를 계산해 나눗셈과 나머지로 다시 나누면 된다.

작성한 코드는 다음과 같다([`Lab_Q3.cpp`](../../Coursework/Lab07_Friend-Functions-and-Operator-Overloads/Lab_Q3.cpp)).

```cpp
/**
 * @file    A10204-1_C07Q3.cpp
 * @brief   Q3. 시간 클래스의 연산자 중복
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Time {
    int hour;
    int min;
    int sec;
    void normalize();    // 분, 초를 0~59 사이로 조정
public:
    Time(int h = 0, int m = 0, int s = 0);
    Time operator-(const Time& op);
    bool operator<=(const Time& op);
    int& operator[](const int op);
    friend ostream& operator<<(ostream& stream, Time op2);
    friend Time& operator+=(Time& op1, Time& op2);
};
Time::Time(int h, int m, int s) {
    hour = h; min = m; sec = s;
    normalize();
}
void Time::normalize() {
    if (sec >= 60) {
        sec -= 60;
        min++;
    } else if (sec < 0) {
        sec += 60;
        min--;
    }
    
    if (min >= 60) {
        min -= 60;
        hour++;
    } else if (min < 0) {
        min += 60;
        hour--;
    }
    
    if (hour > 24) {
        hour -= 24;
    } else if (hour < 0) {
        hour += 24;
    }
}

Time Time::operator-(const Time& op) {
    Time tmp;
    if (*this <= op) {
        tmp.hour = op.hour - hour;
        tmp.min = op.min - min;
        tmp.sec = op.sec - sec;
    } else {
        tmp.hour = hour - op.hour;
        tmp.min = min - op.min;
        tmp.sec = sec - op.sec;
    }
    tmp.normalize();
    return tmp;
}
bool Time::operator<=(const Time& op) {
    int tmp1, tmp2;
    tmp1 = hour * 3600 + min * 60 + sec;
    tmp2 = op.hour * 3600 + op.min * 60 + op.sec;
    return (tmp1 <= tmp2) ? true : false;
}
int& Time::operator[](const int op) {
    if (op == 0)
        return hour;
    else if (op == 1)
        return min;
    else
        return sec;
}
ostream& operator<<(ostream& stream, Time op2) {
    stream << '(' << op2.hour << ':' << op2.min << ':' << op2.sec << ')';
    return stream;
}
Time& operator+=(Time& op1, Time& op2) {
    op1.hour += op2.hour;
    op1.min += op2.min;
    op1.sec += op2.sec;
    op1.normalize();
    return op1;
}

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
    
    t3[0] = 3;      // t3의 시간을 3으로 변환
    t3[1] = 9;      // t3의 분을 9로 변환
    t3[2] = 27;     // t3의 초를 27로 변환
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

실행 결과는 다음과 같다.

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
```

실행 예시의 마지막 줄 `Program ended with exit code: 0`은 Xcode가 출력하는 메시지이다.

---

<br>

## 교재 이론 문제

**교재:** 7장 연습문제의 이론 문제 중 짝수 번 문항

### 이론 문제 2

**문제:** 프렌드 함수가 필요한 경우가 아닌 것을 고르라.

**정답:** ④

프렌드 함수는 클래스의 멤버가 아닌 함수에 그 클래스의 `private` 멤버에 대한 접근 권한을 주기 위해 쓴다([7장 1.1절](Concepts_Lecture.ko.md#11-프렌드-함수)). 함수 중복은 같은 이름의 함수를 매개변수만 달리하여 여러 개 정의하는 것으로, `private` 멤버에 접근하는 일과 관계가 없으므로 프렌드가 필요하지 않다. 반면 멤버가 아닌 함수가 `private` 멤버에 접근해야 할 때(①), 한 함수가 두 클래스의 `private` 멤버를 함께 다룰 때(②), 왼쪽 피연산자가 그 클래스의 객체가 아닌 연산자를 중복할 때(③)에는 프렌드 함수가 필요하다.

### 이론 문제 4

**문제:** `SampleManager`의 멤버 함수 `compare()`를 `Sample`의 프렌드로 선언하라.

**정답:**

```cpp
class Sample {
    ...
    friend bool SampleManager::compare(Sample &a, Sample &b);
};
```

다른 클래스의 멤버 함수를 프렌드로 초대할 때는 함수 이름 앞에 그 함수가 속한 클래스 이름과 범위 지정 연산자를 붙여 `SampleManager::compare`로 쓴다([7장 1.3절](Concepts_Lecture.ko.md#13-프렌드-선언의-세-가지-형태)). `SampleManager::`를 빼고 `friend bool compare(Sample &a, Sample &b);`라고 쓰면 같은 이름의 전역 함수를 프렌드로 선언한 것이 되어, `SampleManager`의 멤버 함수는 여전히 `Sample`의 `private` 멤버에 접근할 수 없다. 또 이 선언이 컴파일되려면 `SampleManager` 클래스가 `Sample`보다 앞에 완전히 선언되어 있어야 하고, `SampleManager` 안에서 `Sample`을 쓰기 위해 그 앞에 `class Sample;`이라는 전방 선언이 필요하다.

### 이론 문제 6

**문제:** 전역 함수 `show()`가 `Student`와 `Professor`의 `private` 멤버에 접근하여 생기는 컴파일 오류의 원인을 쓰고 수정하라.

**정답:** `Student`의 `id`와 `Professor`의 `name`은 `private` 멤버이므로 멤버가 아닌 `show()`에서 직접 접근할 수 없다. 두 클래스에서 `show()`를 프렌드로 선언하여 고친다.

```cpp
class Student;
class Professor;

class Student {
    int id;
public:
    Student(int id) { this->id = id; }
    friend void show(Student s, Professor p);
};
class Professor {
    string name;
public:
    Professor(string name) { this->name = name; }
    friend void show(Student s, Professor p);
};
void show(Student s, Professor p) {
    cout << s.id << p.name;
}
```

`show()`는 두 클래스의 `private` 멤버를 함께 읽어야 하므로 두 클래스 모두에 프렌드로 선언한다. `Student` 안의 프렌드 선언이 아직 정의되지 않은 `Professor`를 매개변수 타입으로 쓰므로, 힌트대로 맨 앞에 두 클래스의 전방 선언을 둔다. 다른 방법으로, 두 클래스에 `int getId() const`, `string getName() const` 같은 `public` getter를 두고 `show()`가 getter를 호출하게 해도 된다. 이때 `show()`가 매개변수를 `const Student&`처럼 `const` 참조로 받는다면 getter를 `const` 멤버 함수로 선언해야 한다. `const` 객체에서는 `const` 멤버 함수만 호출할 수 있기 때문이다.

### 이론 문제 8

**문제:** 프렌드 선언의 위치에 관한 설명 중 옳은 것을 고르라.

**정답:** ④

프렌드 선언은 클래스의 멤버를 선언하는 것이 아니라 클래스 밖의 함수나 클래스에 접근 권한을 주는 선언이므로, 접근 지정자의 영향을 받지 않는다. 따라서 `private`, `protected`, `public` 중 어느 영역에 두어도 효과가 같다([7장 1.4절](Concepts_Lecture.ko.md#14-사용-지침)). 관례적으로는 클래스의 인터페이스를 읽기 쉽도록 `public` 영역이나 클래스의 처음 또는 끝에 모아 둔다.

### 이론 문제 10

**문제:** 모든 멤버가 `public`인 `Sample`에서 `isZero()`의 프렌드 선언이 필요한지 설명하라.

**정답:** 필요하지 않다.

`Sample`의 멤버 `x`와 생성자는 모두 `public`이므로, 전역 함수 `isZero()`는 프렌드가 아니어도 `a.x`에 접근할 수 있다. 프렌드 선언은 `private`이나 `protected` 멤버에 접근해야 할 때만 의미가 있다. 프렌드 선언을 지워도 프로그램은 똑같이 컴파일되고 동작한다.

### 이론 문제 12

**문제:** 서로 다른 대상에 같은 `+`를 쓰면서 대상에 따라 결과가 달라지는 다형성의 다른 사례를 하나 들라.

**정답:** `"I love C" + "++" = "I love C++"`

같은 `+` 기호가 수에서는 덧셈을, 색에서는 섞기를, 문자열에서는 이어 붙이기를 뜻한다. 이처럼 같은 연산자가 피연산자의 종류에 따라 다른 의미로 동작하는 것이 연산자 중복으로 구현하는 다형성이다([7장 2.1절](Concepts_Lecture.ko.md#21-왜-연산자-중복이-필요한가)). C++에서는 `string` 클래스가 `+`를 문자열 연결로 중복하므로 `string("I love C") + "++"`가 `"I love C++"`가 된다. 두 문자열 리터럴을 바로 더하는 `"I love C" + "++"`는 포인터끼리의 덧셈이 되어 컴파일되지 않으므로, 적어도 한쪽은 `string` 객체여야 한다.

### 이론 문제 14

**문제:** `Power` 객체 `a`, `b`에 대해 `+` 연산자 함수를 멤버 함수로 작성할 수 없는 식을 고르라.

**정답:** ④

멤버 연산자 함수는 왼쪽 피연산자의 멤버로 호출되므로 `a + b`는 `a.operator+(b)`, `a + 3`은 `a.operator+(3)`이 된다. `3 + a`는 왼쪽 피연산자가 정수 3이어서 `3.operator+(a)`가 되어야 하지만, `int`는 클래스가 아니므로 멤버 함수를 둘 수 없다. 따라서 `3 + a`는 `Power operator+(int op1, Power op2)` 같은 외부 함수로, 보통 `Power`의 프렌드로 구현한다([7장 3.2절](Concepts_Lecture.ko.md#32-프렌드-함수로-구현)). ③의 `b = a += 3`은 `+=` 연산자를 쓰며, `a += 3`은 `a.operator+=(3)`으로 멤버 함수로 구현할 수 있다.

### 이론 문제 16

**문제:** 프렌드 연산자 함수의 선언과 연산의 짝 중 잘못된 것을 고르라.

**정답:** ④

대입 연산자 `=`는 멤버 함수로만 중복할 수 있다. `Power& operator = (Power &a, Power b);`처럼 매개변수 두 개를 받는 외부 함수로 선언하면 `overloaded 'operator=' must be a non-static member function` 오류가 난다. 멤버 함수로는 `Power& operator=(const Power& b);`처럼 오른쪽 피연산자 하나만 받는다. `=`와 함께 `()`, `[]`, `->`도 멤버 함수로만 중복할 수 있다([7장 2.4절](Concepts_Lecture.ko.md#24-멤버-함수-vs-프렌드-함수-판단-가이드)).

나머지 선언은 문법에 맞다. 다만 ③의 `Power operator ++ (Power a, int b);`는 피연산자를 값으로 받으므로, 함수 안에서 증가시키는 대상이 `a`의 복사본이 되어 원래 객체가 바뀌지 않는다. 후위 `++`를 프렌드로 구현할 때는 `Power operator++(Power& a, int b);`처럼 참조로 받아야 원래 객체가 증가한다([7장 4.2절](Concepts_Lecture.ko.md#42-프렌드-함수로-구현)).
