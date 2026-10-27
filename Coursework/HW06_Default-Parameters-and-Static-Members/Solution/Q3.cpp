/**
 * @file    A6Q3.cpp
 * @brief   Q3. static 멤버로 이루어진 Random 클래스 (11주차 실습 3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Random {
public:
    // 항상 다른 랜덤수를 발생시키기
    // 위한 seed를 설정하는 함수
    static void seed() {
        srand((unsigned)time(0));
    }
    
    //min과 max 사이의 랜덤 정수 리턴
    static int nextInt(int min = 0, int max = 32767);
    static char nextAlphabet();

    // 0보다 크거나 같고 1보다 적은 실수 리턴
    static double nextDouble();
};

int Random::nextInt(int min, int max) {
    return min + rand() % (max - min + 1);
}
char Random::nextAlphabet() {
    if (rand() % 2)
        return 'a' + rand() % 26;
    else
        return 'A' + rand() % 26;
}
double Random::nextDouble() {
    return (double) rand() / RAND_MAX;
}

int main() {
    Random::seed();
    cout << "1에서 100까지 랜덤한 정수 10개를 출력합니다" << endl;
    for (int i = 0; i < 10; i++)
        cout << Random::nextInt(1, 100) << ' ';
    cout << endl;
    
    cout << "랜덤한 알파벳 10개를 출력합니다" << endl;
    for (int i = 0; i < 10; i++)
        cout << Random::nextAlphabet() << ' ';
    cout << endl;
    
    cout << "랜덤한 실수 10개를 출력합니다" << endl;
    for (int i = 0; i < 5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
    for (int i = 0; i < 5; i++)
        cout << Random::nextDouble() << ' ';
    cout << endl;
}
