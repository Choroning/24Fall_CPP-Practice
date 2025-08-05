/**
 * @file    A10204-1_C04Q1.cpp
 * @brief   Q1. 동적 배열을 가진 클래스
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Sample {
    int *p = nullptr;
    int size;
public:
    Sample(int n) {
        size = n;
        if (size <= 0){
            throw 1;
        } else {
            p = new int [size];
        }
    }
    void read();
    void write();
    int big();
    ~Sample();
};

void Sample::read() {
    for (int i = 0; i < size; i++) {
        cin >> p[i];
    }
}
void Sample::write() {
    for (int i = 0; i < size; i++) {
        cout << p[i] << ' ';
    }
    cout << endl;
}
int Sample::big() {
    int max = p[0];
    for (int i = 1; i < size; i++) {
        max = (p[i] > max) ? p[i] : max;
    }
    return max;
}
Sample::~Sample() {
    delete [] p;
    p = nullptr;
}

int main()
{
    int input;
    try {
        cout << "생성할 정수 배열 사이즈를 입력하세요: ";
        cin >> input;
        Sample s(input);
        s.read();
        s.write();
        cout << "가장 큰 수는 " << s.big() << endl;
    }
    catch (bad_alloc) {
        cout << "[오류] 동적할당에 실패했습니다!\n";
        return 0;
    }
    catch (int e) {
        cout << "[오류] 사이즈가 너무 작습니다!\n";
        return 0;
    }
    
    return 0;
}
