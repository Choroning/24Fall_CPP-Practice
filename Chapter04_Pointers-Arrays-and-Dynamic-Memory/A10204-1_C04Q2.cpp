/**
 * @file    A10204-1_C04Q2.cpp
 * @brief   Q2. 객체의 동적 생성 및 소멸
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
#define PI 3.14
using namespace std;

class Circle {
   int radius;    // 원의 반지름
   string name;   // 원의 이름
public:
   void setCircle(string name, int radius);  // 이름과 반지름 설정
   double getArea();
   string getName();
};

void Circle::setCircle(string name, int radius) {
    this->radius = radius;
    this->name = name;
}
double Circle::getArea() {
    return PI * radius * radius;
}
string Circle::getName() {
    return name;
}


class CircleManager {
   Circle *p;     // Circle 배열에 대한 포인터
   int size;      // 배열의 크기
public:
   CircleManager(int size);   // 사용자로부터 입력받은 size 값 크기의 배열을 동적 생성
   ~CircleManager();
   void searchByName(); // 사용자로부터 원의 이름을 입력받아 면적 출력
   void searchByArea(); // 사용자로부터 면적을 입력받아 면적보다 큰 원의 이름 출력
};

CircleManager::CircleManager(int size) {
    this->size = size;
    
    if (this->size <= 0) {
        throw 1;
    } else {
        p = new Circle [this->size];
        
        for (int i = 0; i < this->size; i++) {
            string temp1 = "";
            int temp2;
            
            cout << "원 " << i + 1 << "의 이름과 반지름 >> ";
            cin >> temp1 >> temp2;
            
            p[i].setCircle(temp1, temp2);
        }
    }
}
CircleManager::~CircleManager() {
    delete [] p;
    p = nullptr;
}
void CircleManager::searchByName() {
    string temp3 = "";
    cout << "검색하고자 하는 원의 이름 >> ";
    cin >> temp3;
    
    for (int i = 0; i < size; i++) {
        if (p[i].getName() == temp3) {
            cout << temp3 << "의 면적은 " << p[i].getArea() << endl;
        }
    }
}
void CircleManager::searchByArea() {
    int temp4;
    cout << "최소 면적을 정수로 입력하세요 >> ";
    cin >> temp4;
    cout << temp4 << "보다 큰 원을 검색합니다.\n";
    for (int i = 0; i < size; i++) {
        if (p[i].getArea() > temp4) {
            cout << p[i].getName() << "의 면적은 " << p[i].getArea() << ", ";
        }
    }
    cout << endl;
}


int main(void)
{
    try {
        int val;
        cout << "원의 개수 >> ";
        cin >> val;
        CircleManager c(val);
        c.searchByName();
        c.searchByArea();
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