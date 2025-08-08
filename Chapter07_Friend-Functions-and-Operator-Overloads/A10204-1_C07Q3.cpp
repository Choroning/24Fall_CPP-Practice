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
    Time operator - (const Time& op);
    bool operator <= (const Time& op);
    int& operator [] (const int op);
    friend ostream& operator << (ostream& stream, Time op2);
    friend Time& operator += (Time& op1, Time& op2);
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

Time Time::operator - (const Time& op) {
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
bool Time::operator <= (const Time& op) {
    int tmp1, tmp2;
    tmp1 = hour * 3600 + min * 60 + sec;
    tmp2 = op.hour * 3600 + op.min * 60 + op.sec;
    return (tmp1 <= tmp2) ? true : false;
}
int& Time::operator [] (const int op) {
    if (op == 0)
        return hour;
    else if (op == 1)
        return min;
    else
        return sec;
}
ostream& operator << (ostream& stream, Time op2) {
    stream << '(' << op2.hour << ':' << op2.min << ':' << op2.sec << ')';
    return stream;
}
Time& operator += (Time& op1, Time& op2) {
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
}
