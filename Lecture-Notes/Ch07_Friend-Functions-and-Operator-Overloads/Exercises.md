# Chapter 7 Exercises — Friend Functions and Operator Overloads

[Repository Guide](../../README.md) | [English](Exercises.md) | [한국어](Exercises.ko.md)

> **Last Updated:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 7

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

Exercises 1-3 are lab problems, and Exercises 4-11 are the even-numbered theory problems (2-16) of Chapter 7 of the textbook.

---

<br>

## Table of Contents

- [Exercise 1: Operator Overloading for a Fraction Class with Member Functions](#exercise-1-operator-overloading-for-a-fraction-class-with-member-functions)
- [Exercise 2: Operator Overloading for a Complex Number Class with Friend Functions](#exercise-2-operator-overloading-for-a-complex-number-class-with-friend-functions)
- [Exercise 3: Operator Overloading for a Time Class](#exercise-3-operator-overloading-for-a-time-class)
- [Exercise 4: When a Friend Function Is Needed](#exercise-4-when-a-friend-function-is-needed)
- [Exercise 5: Declaring a Member Function of Another Class as a Friend](#exercise-5-declaring-a-member-function-of-another-class-as-a-friend)
- [Exercise 6: A Function That Accesses the `private` Members of Two Classes](#exercise-6-a-function-that-accesses-the-private-members-of-two-classes)
- [Exercise 7: The Position of a Friend Declaration](#exercise-7-the-position-of-a-friend-declaration)
- [Exercise 8: Whether a Friend Declaration Is Needed](#exercise-8-whether-a-friend-declaration-is-needed)
- [Exercise 9: Polymorphism of the Plus Operator](#exercise-9-polymorphism-of-the-plus-operator)
- [Exercise 10: An Operator Function That Cannot Be a Member Function](#exercise-10-an-operator-function-that-cannot-be-a-member-function)
- [Exercise 11: Declarations of Friend Operator Functions](#exercise-11-declarations-of-friend-operator-functions)

---

<br>

## Exercise 1: Operator Overloading for a Fraction Class with Member Functions

**Problem:** Referring to the example code and the sample run, complete the operator functions of the `Fraction` class so that `main()` runs correctly.

**Program Requirements**

1. Fill in the [ 빈칸 ] (blank) parts; the given example code must not be changed.
2. The denominator can never be `0`, and the printed fractions do not have to be in lowest terms.
3. Write each operator function so that it meets the following conditions.
   - Binary operator functions to be written as member functions: `fraction + fraction`, `fraction == fraction`, `fraction ^ integer` (integer power of a fraction), `fraction *= fraction`, `fraction / integer`
   - Unary operator functions to be written as member functions: `~fraction` (reciprocal), `fraction++` (postfix), `--fraction` (prefix)

**Example Code**

```cpp
#include <iostream>
using namespace std;

class Fraction {
    int num;    // numerator
    int denom;  // denominator
public:
    Fraction() { num = denom = 1; }
    Fraction(int n, int d) { num = n; denom = d; }
    friend ostream& operator<<(ostream& stream, Fraction f) {
        stream << f.num << "/" << f.denom;
        return stream;
    }
    // operator function declarations
    [ 빈칸 ]
};

// operator function definitions
[ 빈칸 ]

int main() {
    Fraction f1(3, 4), f2(2, 5), f3, f4(7, 15), f5(6, 15);
    
    f3 = f1 + f2;   // fraction + fraction
    cout << f1 << " + " << f2 << " = " << f3 << "\n\n";
    
    if (f2 == f5) cout << f2 << " == " << f5 << "\n\n";
    if (f2 == f4) cout << f2 << " == " << f4 << "\n\n";
    
    f1 *= f2;       // fraction *= fraction
    cout << "f1 *= f2 " << "===> f1 = " << f1 << "\n\n";
    
    f3 = f2 ^ 3;    // fraction ^ integer (integer power of a fraction)
    cout << "( " << f2 << " )^3 = " << f3 << "\n\n";
    
    f3 = f2 / 5;    // fraction / integer
    cout << f2 << " / 5 = " << f3 << "\n\n";
    
    f3 = ~f2;       // ~ fraction (reciprocal)
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

**Sample Run**

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

**[Solution](Solutions.md#exercise-1-operator-overloading-for-a-fraction-class-with-member-functions)**

---

<br>

## Exercise 2: Operator Overloading for a Complex Number Class with Friend Functions

**Problem:** Referring to the example code and the sample run, complete the operator functions of the `Complex` class so that `main()` runs correctly.

**Program Requirements**

1. Fill in the [ 빈칸 ] (blank) parts; the given example code must not be changed.
2. Write each operator function so that it meets the following conditions.
   - Binary operator functions to be written as friend functions: `integer + complex`, `complex * complex`, `complex -= complex`, `complex != complex`
   - Unary operator functions to be written as friend functions: `~complex` (complex conjugate), `complex--` (postfix), `++complex` (prefix)

**Example Code**

```cpp
#include <iostream>
using namespace std;

class Complex {
    int real;         // real part
    int imaginary;    // imaginary part
public:
    Complex(int r = 0, int i = 0) { real = r; imaginary = i; }
    friend ostream& operator<<(ostream& stream, Complex op2);
    // operator function declarations
    [ 빈칸 ]
};
ostream& operator<<(ostream& stream, Complex op2) {
    stream << op2.real;
    if (op2.imaginary >= 0)
        stream << "+";
    stream << op2.imaginary << "i";

    return stream;
}
// operator function definitions
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

The `if` statement in `operator<<` of the example code prints `+` when the imaginary part is 0 or greater. With real part 1 and imaginary part -2, the minus sign is printed with the number, giving `1-2i`, but with imaginary part 3 the output would be `13i` without a `+` if the `if` statement were missing, so this part handles that case.

**Sample Run**

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

**[Solution](Solutions.md#exercise-2-operator-overloading-for-a-complex-number-class-with-friend-functions)**

---

<br>

## Exercise 3: Operator Overloading for a Time Class

**Problem:** Referring to the example code and the sample run, complete the operator functions of the `Time` class, which represents a length of time, so that `main()` runs correctly.

**Program Requirements**

1. Fill in the [ 빈칸 ] (blank) parts; the given example code must not be changed.
2. Every `Time` object must stay normalized, that is, its minutes and seconds must stay between 0 and 59 (see the first line of the sample run).
3. Write each operator function so that it meets the following conditions.
   - Operator functions to be written as member functions: `object - object` (time difference of two objects), `object <= object`, `object[integer]`
   - Operator functions to be written as friend functions: `cout << object`, `object += object`
4. `object - object` computes the difference of the two times regardless of the order of subtraction, so `t1 - t2` and `t2 - t1` give the same result in the sample run.

**Example Code**

```cpp
#include <iostream>
using namespace std;

class Time {
    int hour;
    int min;
    int sec;
    void normalize();    // adjusts minutes and seconds to 0-59
public:
    Time(int h = 0, int m = 0, int s = 0);
    // operator function declarations
    [ 빈칸 ]
};
Time::Time(int h, int m, int s) {
    hour = h; min = m; sec = s;
    normalize();
}
// definitions of normalize() and the operator functions
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

    t3[0] = 3;     // sets the hours of t3 to 3
    t3[1] = 9;     // sets the minutes of t3 to 9
    t3[2] = 27;    // sets the seconds of t3 to 27
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

**Sample Run**

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

**[Solution](Solutions.md#exercise-3-operator-overloading-for-a-time-class)**

---

<br>

## Exercise 4: When a Friend Function Is Needed

**Textbook Problem:** Chapter 7 theory problem 2

**Problem:** In which case is a friend function not needed?

- ① When writing a function that is not a member but must access the `private` members of a class
- ② When writing a function that accesses the `private` members of two or more classes at the same time
- ③ When overloading operators
- ④ When overloading functions

**[Solution](Solutions.md#exercise-4-when-a-friend-function-is-needed)**

---

<br>

## Exercise 5: Declaring a Member Function of Another Class as a Friend

**Textbook Problem:** Chapter 7 theory problem 4

**Problem:** The class `SampleManager` has the following member function. Declare it so that this member function is invited as a friend into the class `Sample`.

```cpp
bool compare(Sample &a, Sample &b);
```

```cpp
class Sample {
    ...
};
```

**[Solution](Solutions.md#exercise-5-declaring-a-member-function-of-another-class-as-a-friend)**

---

<br>

## Exercise 6: A Function That Accesses the `private` Members of Two Classes

**Textbook Problem:** Chapter 7 theory problem 6

**Problem:** The following program causes a compile error. What is the cause of the error? Fix the error appropriately.

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

As a hint, the textbook says to insert the two forward declarations `class Student;` and `class Professor;` at the very beginning of the code to solve the forward reference problem that arises while solving it.

**[Solution](Solutions.md#exercise-6-a-function-that-accesses-the-private-members-of-two-classes)**

---

<br>

## Exercise 7: The Position of a Friend Declaration

**Textbook Problem:** Chapter 7 theory problem 8

**Problem:** Which statement about the position of a friend declaration is correct?

- ① It must be declared in the `private` section of the class.
- ② It must be declared in the `protected` section of the class.
- ③ It must be declared in the `public` section of the class.
- ④ It may be declared in any section of the class.

**[Solution](Solutions.md#exercise-7-the-position-of-a-friend-declaration)**

---

<br>

## Exercise 8: Whether a Friend Declaration Is Needed

**Textbook Problem:** Chapter 7 theory problem 10

**Problem:** Explain whether the friend declaration is needed in the following code.

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

**[Solution](Solutions.md#exercise-8-whether-a-friend-declaration-is-needed)**

---

<br>

## Exercise 9: Polymorphism of the Plus Operator

**Textbook Problem:** Chapter 7 theory problem 12

**Problem:** Give just one other example that matches the polymorphism shown below.

```text
2 + 3 = 5
빨강 + 파랑 = 보라
남자 + 여자 = 결혼
```

**[Solution](Solutions.md#exercise-9-polymorphism-of-the-plus-operator)**

---

<br>

## Exercise 10: An Operator Function That Cannot Be a Member Function

**Textbook Problem:** Chapter 7 theory problem 14

**Problem:** For objects `a` and `b` of the `Power` class, in which case can the `+` operator function not be written as a member function?

```cpp
Power a, b;   // Power 클래스에 대해
```

- ① `a = a + b`
- ② `b = a + 3`
- ③ `b = a += 3`
- ④ `b = 3 + a`

**[Solution](Solutions.md#exercise-10-an-operator-function-that-cannot-be-a-member-function)**

---

<br>

## Exercise 11: Declarations of Friend Operator Functions

**Textbook Problem:** Chapter 7 theory problem 16

**Problem:** `a` and `b` are objects of the `Power` class. When the operator functions are written as friend functions of the `Power` class, which pair of an operation on the left and an operator function declaration on the right is wrong?

- ① `a + b`: `Power operator + (Power& a, Power &b);`
- ② `a == b`: `bool operator == (Power a, Power b);`
- ③ `a++`: `Power operator ++ (Power a, int b);`
- ④ `a = b`: `Power& operator = (Power &a, Power b);`

**[Solution](Solutions.md#exercise-11-declarations-of-friend-operator-functions)**
