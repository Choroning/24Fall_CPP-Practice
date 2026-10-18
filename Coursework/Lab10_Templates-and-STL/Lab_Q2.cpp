/**
 * @file    A10204-1_C10Q2.cpp
 * @brief   Q2. 제네릭 클래스와 구체화 디버깅
 * @author  박철원 (Cheolwon Park)
 * @date    2024-12-10
 */

#include <iostream>
using namespace std;

template <typename T>
class Rectangle {
    T width;
    T height;
public:
    Rectangle(T w = 1, T h = 1) { width = w; height = h; }
    T getArea();
    
    /*
     * [방법1] 연산자 중복을 통한 해결
     * 이때, 제네릭 함수는 되도록 멤버함수로 중복하는 것이 더 좋은 방법이다.
     * (불가능한 케이스 제외)
     */
    bool operator > (Rectangle op2) {       
        if (getArea() > op2.getArea())
            return true;
        else
            return false;
    }
};

// 멤버함수 getArea() 정의
template <typename T>
T Rectangle<T>::getArea() {
    return width * height;
}

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

/*
 * [방법2] 제네릭 함수 bigger() 정의
 */
template <typename T>
Rectangle<T> bigger(Rectangle<T> a, Rectangle<T> b) {
    return (a.getArea() > b.getArea()) ? a : b;
}

int main() {
    int a = 20, b = 50, c;

    c = bigger(a, b);
    cout << "20과 50 중 더 큰 값은 " << c << endl;
    
    // double형 Rectangle 객체 rect1, rect2, rect3 선언
    // 단, rect1의 width와 height은 각각 10.2와 23.5, rect2는 20.4와 13.7로 초기화
    Rectangle<double> rect1(10.2, 23.5), rect2(20.4, 13.7), rect3;
    
    rect3 = bigger(rect1, rect2);
    cout << "rect1과 rect2 중 더 큰 것의 면적은 " << rect3.getArea() << endl;

    return 0;
}
