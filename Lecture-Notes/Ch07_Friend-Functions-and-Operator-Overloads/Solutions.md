# Chapter 7 Solutions — Friend Functions and Operator Overloads

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-19

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Operator Overloading for a Fraction Class with Member Functions](#exercise-1-operator-overloading-for-a-fraction-class-with-member-functions)
- [Exercise 2: Operator Overloading for a Complex Number Class with Friend Functions](#exercise-2-operator-overloading-for-a-complex-number-class-with-friend-functions)
- [Exercise 3: Operator Overloading for a Time Class](#exercise-3-operator-overloading-for-a-time-class)

---

<br>

## Exercise 1: Operator Overloading for a Fraction Class with Member Functions

**Problem:** For a `Fraction` class with numerator `num` and denominator `denom`, write the binary operators `+`, `==`, `^` (integer power), `*=`, `/` (division by an integer) and the unary operators `~` (reciprocal), postfix `++`, and prefix `--`, all as member functions, so that the given `main()` runs. The full problem and sample run are in [Exercise 1](Exercises.md#exercise-1-operator-overloading-for-a-fraction-class-with-member-functions).

### Solution

Since every operator is a member function, the left operand is `*this`, and the right operand of each binary operator is received as the parameter `op`. Each operator transcribes the arithmetic rule for fractions.

| Operator | Implementation | Calculation in the sample run |
|:-----|:-----|:-----|
| `+` | Builds and returns a new object `tmp` with $\frac{a}{b} + \frac{c}{d} = \frac{ad + bc}{bd}$ | 3/4 + 2/5 = (15 + 8)/20 = 23/20 |
| `==` | `true` if the cross products $a \times d$ and $b \times c$ are equal | 2/5 and 6/15 are equal since 30 = 30, while 2/5 and 7/15 differ since 30 ≠ 35 |
| `*=` | Multiplies its own numerator and denominator by the other's and returns `*this` | 3/4 × 2/5 = 6/20 |
| `^` | Multiplies the numerator and denominator of a default-constructed 1/1 `op` times | $(2/5)^3 = 8/125$ |
| `/` | Returns a new object whose denominator is multiplied by the integer | 2/5 ÷ 5 = 2/25 |
| `~` | Returns a new object with numerator and denominator swapped, so the original is unchanged | `~(2/5)` = 5/2 |
| Postfix `++` | Saves the old state in `tmp`, adds 1 by adding the denominator to the numerator, and returns `tmp` | 6/20 + 1 = 26/20, returning 6/20 |
| Prefix `--` | Subtracts 1 by subtracting the denominator from the numerator and returns `*this` | 2/5 - 1 = -3/5, also returning -3/5 |

*Table 1. Implementation and calculations of the `Fraction` operator functions*

Postfix `++` takes an unused `int` parameter `notUsed` to distinguish it from prefix. `*=` and prefix `--` return `Fraction` by value rather than `Fraction&`, so a copy of the modified object is returned. This `main()` does not chain the results, so the output is the same, but returning a reference, as in Sections 3.3 and 4.3 of the [Chapter 7 notes](Concepts_Lecture.md), is the usual form.

The code is as follows ([`Lab_Q1.cpp`](../../Coursework/Lab07_Friend-Functions-and-Operator-Overloads/Lab_Q1.cpp)).

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

It compiles without warnings with `g++ -std=c++17 -Wall`, and the output is as follows.

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

The output matches the sample run. The last line of the sample run, `Program ended with exit code: 0`, is a message Xcode prints after the program ends and is not output of the program.

---

<br>

## Exercise 2: Operator Overloading for a Complex Number Class with Friend Functions

**Problem:** For a `Complex` class with real part `real` and imaginary part `imaginary`, write the binary operators `integer + complex`, `*`, `-=`, `!=` and the unary operators `~` (complex conjugate), postfix `--`, and prefix `++`, all as friend functions, so that the given `main()` runs. The full problem and sample run are in [Exercise 2](Exercises.md#exercise-2-operator-overloading-for-a-complex-number-class-with-friend-functions).

### Solution

A friend function is not a member, so it receives every operand as a parameter, and the `friend` declaration inside the class lets it access the private members `real` and `imaginary`. Operators that modify an operand (`-=`, postfix `--`, prefix `++`) must receive that operand by reference (`Complex&`) so that the original changes.

| Operator | Implementation | Calculation in the sample run |
|:-----|:-----|:-----|
| `integer + complex` | Cannot be a member because the left operand is `int`; adds the integer to the real part only | 10 + (2-3i) = 12-3i |
| `*` | $(a + bi)(c + di) = (ac - bd) + (ad + bc)i$ | (-5+7i)(2-3i) = (-10 + 21) + (15 + 14)i = 11+29i |
| `-=` | Receives the first parameter by reference, subtracts the real and imaginary parts, and returns that reference | (-5+7i) - (2-3i) = -7+10i |
| `!=` | `false` if both the real and imaginary parts are equal, `true` otherwise | `true` since `c = 2-3i` and `b = 1-3i` |
| `~` | Receives the parameter by value and returns the copy with its imaginary sign flipped, so the original is unchanged | `~(2-3i)` = 2+3i |
| Postfix `--` | Saves the old state in `tmp`, decreases the real part by 1, and returns `tmp` | `b` becomes 1-3i, returning 2-3i |
| Prefix `++` | Increases the real part by 1 and returns that reference | Both `b` and `c` are 2-3i |

*Table 2. Implementation and calculations of the `Complex` operator functions*

Increment and decrement are treated as adding or subtracting the real number 1, so only the real part changes. This matches the sample run, where `b--` turns 2-3i into 1-3i. The commented-out line `op.imaginary *= (-1);` in `~` is another way to write the line below it with the same result.

The code is as follows ([`Lab_Q2.cpp`](../../Coursework/Lab07_Friend-Functions-and-Operator-Overloads/Lab_Q2.cpp)).

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

It compiles without warnings with `g++ -std=c++17 -Wall`, and the output is as follows.

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

The output matches the sample run. The gap between the two values is printed as a tab character and a space (`"\t "`), so the output above expands tabs to a width of 8, and the gap may look different from the sample run depending on the terminal's tab setting. The last line of the sample run, `Program ended with exit code: 0`, is a message printed by Xcode.

---

<br>

## Exercise 3: Operator Overloading for a Time Class

**Problem:** For a `Time` class that represents a length of time in hours, minutes, and seconds (`hour`, `min`, `sec`), write the function `normalize()`, the member operators `object - object` (time difference), `<=`, and `[]`, and the friend operators `cout << object` and `+=`, so that the given `main()` runs. Every object must keep its minutes and seconds between 0 and 59. The full problem and sample run are in [Exercise 3](Exercises.md#exercise-3-operator-overloading-for-a-time-class).

### Solution

`normalize()` subtracts 60 from the seconds and adds 1 to the minutes when the seconds are 60 or more, and adds 60 and subtracts 1 from the minutes when they are negative. The minutes carry into the hours the same way. The constructor, `+=`, and `-` call `normalize()` after computing their result, so objects always stay normalized. For example, in `t1(20, 75, 63)`, 63 seconds become 3 seconds and 1 minute, and 76 minutes become 16 minutes and 1 hour, giving (21:16:3).

| Operator | Implementation | Calculation in the sample run |
|:-----|:-----|:-----|
| `+=` (friend) | Adds hours, minutes, and seconds, calls `normalize()`, and returns a reference to the first parameter | (21:16:3) + (2:50:59) = 23 h 66 min 62 s → (24:7:2) |
| `-` (member) | Compares the times with `<=`, subtracts the smaller from the larger, and calls `normalize()`, so the result does not depend on the order | 22 h -43 min -57 s → (21:16:3) |
| `<=` (member) | Converts both times to seconds (`hour * 3600 + min * 60 + sec`) and compares them | 40707 s > 40700 s, so (11:18:27) > (11:18:20) |
| `[]` (member) | Returns a reference (`int&`) to `hour`, `min`, or `sec` for index 0, 1, or any other value, so it can be assigned to | `t3[0] = 3` and so on give (3:9:27) |
| `<<` (friend) | Prints in the form `(h:m:s)` and returns the stream | (2:50:59) |

*Table 3. Implementation and calculations of the `Time` operator functions*

The hours are reduced by 24 only when they exceed 24, so (24:7:2), whose total is exactly 24 hours, is printed as is. Values assigned through `[]` do not pass through `normalize()`, but the values 3, 9, and 27 assigned in this `main()` are already in range.

> **Note:** **A `normalize()` that adjusts only once.** This `normalize()` adjusts the seconds and the minutes by 60 at most once each, so normalization is incomplete if a value before normalization is less than -60 or 120 or more (for example, `Time(0, 0, 130)` keeps 70 seconds). In this `main()`, the constructor arguments are below 120, the sum of two normalized values is at most 119 even with a carry, and their difference is at least -60 even with a borrow, so one adjustment is enough. To accept arbitrary values, repeat with a `while` statement or compute the total seconds and split them again with division and remainder.

The code is as follows ([`Lab_Q3.cpp`](../../Coursework/Lab07_Friend-Functions-and-Operator-Overloads/Lab_Q3.cpp)).

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

It compiles without warnings with `g++ -std=c++17 -Wall`, and the output is as follows.

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

The output matches the sample run. The last line of the sample run, `Program ended with exit code: 0`, is a message printed by Xcode.
