/**
 * @file    A6Q2.cpp
 * @brief   Q2. static 멤버로 회원 수를 세는 Person 클래스 (11주차 실습 2-3)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-15
 */

#include <iostream>
using namespace std;

class Person {
    int id;
    double weight;
    string name;
    static int numPerson;
public:
    Person();
    Person(int id, string name);
    Person(int id, string name, double weight);
    ~Person();
    void show() { cout << id << ' ' << weight << ' ' << name << ' ' << numPerson << endl; }
    static void nPerson();
};
Person::Person() {
    id = 1;
    weight = 20.5;
    name = "Grace";
    numPerson++;
}
Person::Person(int id, string name) {
    this->id = id;
    weight = 20.5;
    this->name = name;
    numPerson++;
}
Person::Person(int id, string name, double weight) {
    this->id = id;
    this->weight = weight;
    this->name = name;
    numPerson++;
}
Person::~Person() {
    numPerson--;
}
void Person::nPerson() {
    cout << numPerson << endl;
}

int Person::numPerson = 0;

int main() {
    Person grace;
    grace.show();
    
    Person helen(13, "Helen", 32.5);
    helen.show();
    
    Person *p = new Person[2] { {43, "Tera"}, {98, "Hera"} };
    p[0].show();
    p[1].show();
    Person::nPerson();
    
    delete [] p;
    Person::nPerson();
}
