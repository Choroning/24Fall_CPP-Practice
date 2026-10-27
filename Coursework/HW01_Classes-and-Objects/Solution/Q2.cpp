/**
 * @file    A1Q2.cpp
 * @brief   Q2. 랜덤 정수를 발생시키는 Random 클래스 (교재 3장 실습 문제 5번)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Random{
public:
    Random();
    int next();
    int nextInRange(int, int);
};

Random::Random(){
    srand((unsigned)time(0));
}

int Random::next(){
    return rand();
}

int Random::nextInRange(int s, int f){
    return (rand() % (f - s + 1)) + s;
}


int main()
{
    Random r;
    cout << "-- 0에서 " << RAND_MAX << "까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++){
        int n = r.next();
        cout << n << ' ';
    }
    cout << endl << endl << "-- 2에서 " << "4까지의 랜덤 정수 10개 --" << endl;
    for (int i = 0; i < 10; i++){
        int n = r.nextInRange(2, 4);
        cout << n << ' ';
    }
    cout << endl;
}
