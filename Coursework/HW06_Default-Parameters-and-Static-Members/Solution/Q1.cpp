/**
 * @file    A6Q1.cpp
 * @brief   Q1. 디폴트 매개변수를 갖는 Person 생성자 (11주차 실습 2-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
public:
    Person(int id = 1, string name = "Grace", double weight = 20.5);
    void show() { cout << id << ' ' << weight << ' ' << name << endl; }
};

Person::Person(int id, string name, double weight) {
    this->id = id;
    this->name = name;
    this->weight = weight;
}

int main() {
    Person grace, ashley(2, "Ashley"), helen(3, "Helen", 32.5);
    grace.show();
    ashley.show();
    helen.show();
}
