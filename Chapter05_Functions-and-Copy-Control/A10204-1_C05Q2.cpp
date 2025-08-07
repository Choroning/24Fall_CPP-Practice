/**
 * @file    A10204-1_C05Q2.cpp
 * @brief   Q2. 참조에 의한 호출과 참조 반환
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-17
 */

#include <iostream>
#include <cstring>
using namespace std;

char& find(char a[], char c, bool& success) {
    unsigned long size = strlen(a);
    for (int i = 0; i < size; i++) {
        if (a[i] == c) {
            success = true;
            return a[i];
        }
    }
    success = false;
    return a[0];
}

int main() {
    char s[] = "Mike";
    bool b;

    char& loc1 = find(s, 'k', b);
    if (b == false) {
       cout << "k를 발견할 수 없음" << endl;
       return 0;
    }
    loc1 = 'm';          // 'k' 위치에 'm' 기록
    cout << s << endl;

    char& loc2 = find(s, 'k', b);
    if (b == false) {
       cout << "k를 발견할 수 없음" << endl;
       return 0;
    }
    loc2 = 't';          // 'k' 위치에 't' 기록
    cout << s << endl;

    return 0;
}
