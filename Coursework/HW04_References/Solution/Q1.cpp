/**
 * @file    A4Q1.cpp
 * @brief   Q1. 참조 매개변수로 값을 꺼내는 MyIntStack 클래스 (9주차 문제 2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
#define MAX 2
using namespace std;

class MyIntStack {
    int p[MAX];
    int tos; // 스택의 꼭대기를 가리키는 인덱스
public:
    MyIntStack();
    bool push(int n);
    // 정수n푸시, 꽉차있으면 false, 아니면 true 리턴
    bool pop(int &n);
    // 팝하여 n에 저장, 스택이 비어 있으면 false, 아니면 true 리턴
};

MyIntStack::MyIntStack() {
    tos = 0;
}

bool MyIntStack::push(int n) {
    if (tos < MAX) {
        p[tos] = n;
        tos++;
        return true;
    }
    else
        return false;
}
bool MyIntStack::pop(int& n) {
    if (tos > 0) {
        tos--;
        n = p[tos];
        return true;
    }
    else
        return false;
}

int main() {
    MyIntStack a;
    for (int i = 0; i < MAX + 1; i++) {
        if(a.push(i))
            cout << i << ' '; // 푸시된 값 에코
        else
            cout << endl << i + 1 << "번째 시도 stack full" << endl;
    }
    cout << "------------------------" << endl;
    
    int n;
    for (int i = 0; i < MAX + 1; i++) {
        if(a.pop(n))
            cout << n << ' ';
        else
            cout << endl << i + 1 << "번째 시도 stack empty";
    }
    
    cout << endl;
    return 0;
}
