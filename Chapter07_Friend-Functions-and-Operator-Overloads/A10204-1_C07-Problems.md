# Chapter 7. 연산자 중복

**문항 출처:** *임영희 교수, 컴퓨터언어Ⅱ 교수자 (DCCS102-04)* <br>
**Source:** *Prof. Younghee&nbsp;Im, Instructor of COMPUTER LANGUAGE Ⅱ (DCCS102-04)*

---

## Q1. 멤버함수를 이용한 분수 클래스의 연산자 중복

예시 코드와 실행 예시를 참고하여 `main()`함수가 정상적으로 실행되도록 `Fraction`클래스의 연산자 함수를 완성하시오.


<br>**프로그램 요구사항**

1. 주어진 예시 코드는 변경할 수 없습니다.
2. 분모는 `0`이 될 수 없으며, 기약분수의 형태가 아니어도 됩니다.
3. 아래의 조건을 만족하는 각 연산자 함수를 작성하시오.
      - 멤버함수로 작성해야 하는 이항 연산자 함수<br>
        : `분수 + 분수`, <br>
          `분수 == 분수`, <br>
          `분수 ^ 정수` (분수의 정수 제곱), <br>
          `분수 *= 분수`, <br>
          `분수 / 정수`
      - 멤버함수로 작성해야 하는 단항 연산자 함수<br>
        : `~분수` (분수의 역수), <br>
          `분수++` (후위연산), <br>
          `--분수` (전위연산)


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Fraction {
   int num;    // 분자(numerator)
   int denom;  // 분모(denominator)
public:
   Fraction() { num = denom = 1; }
   Fraction(int n, int d) { num = n; denom = d; }
   friend ostream& operator << (ostream& stream, Fraction f) {
      stream << f.num << "/" << f.denom;
      return stream;
   }
   // 연산자 함수 선언
};

// 연산자 함수 정의

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


<br>**실행 예시**

```text
3/4 + 2/5 = 23/20

2/5 == 6/15

f1 *= f2 ===> f1 = 6/20

( 2/5 )^3 = 8/125

2/5 / 5 = 2/25

~( 2/5 ) = 5/2
f2는 변하지 않음! f2 = 2/5

(6/20)++ = 26/20
후위연산 수행 후, f4 = 6/20

--(2/5) = -3/5
전위연산 수행 후, f4 = -3/5

Program ended with exit code: 0
```



---

## Q2. 프렌드함수를 이용한 복소수 클래스의 연산자 중복

예시 코드와 실행 예시를 참고하여 `main()`함수가 정상적으로 실행되도록 `Complex`클래스의 연산자 함수를 완성하시오.


<br>**프로그램 요구사항**

1. 주어진 예시 코드는 변경할 수 없습니다.
2. 아래의 조건을 만족하는 각 연산자 함수를 작성하시오.
      - 프렌드함수로 작성해야 하는 이항 연산자 함수<br>
        : `정수 + 복소수`, <br>
          `복소수 * 복소수`, <br>
          `복소수 -= 복소수`, <br>
          `복소수 != 복소수`
      - 프렌드함수로 작성해야 하는 단항 연산자 함수<br>
        : `~복소수` (켤레복소수), <br>
          `복소수--` (후위연산), <br>
          `++복소수` (전위연산)


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Complex {
   int real;         // 실수부
   int imaginary;    // 허수부
public:
   Complex (int r = 0, int i = 0) { real = r; imaginary = i; }
   friend ostream& operator << (ostream& stream, Complex op2);
   // 연산자 함수 선언
};
ostream& operator << (ostream& stream, Complex op2) {
   stream << op2.real;
   if (op2.imaginary >= 0)
      stream << "+";
   stream << op2.imaginary << "i";

   return stream;
}
// 연산자 함수 정의

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


<br>**실행 예시**

```text
[c = 10 + b 실행 후]
10 + b = 10 + (2-3i) = 12-3i

[ c = a * b 실행 후]
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



---

## Q3. 시간 클래스의 연산자 중복

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