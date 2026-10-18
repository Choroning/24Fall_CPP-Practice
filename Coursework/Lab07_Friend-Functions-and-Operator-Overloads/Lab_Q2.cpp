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
