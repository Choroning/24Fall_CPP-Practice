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
    friend ostream& operator <<(ostream& stream, Fraction f) {
        stream << f.num << "/" << f.denom;
        return stream;
    }
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
// 연산자 함수 구현

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
