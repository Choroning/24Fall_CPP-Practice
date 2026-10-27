/**
 * @file    A2Q2.cpp
 * @brief   Q2. 포인터로 객체를 참조하는 자유 코드
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-03
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {}
    void introduce() {
        cout << "안녕하세요, 제 이름은 " << name << "이고 " << age << "살입니다." << endl;
    }
    void setAge(int newAge) {
        age = newAge;
    }
};

int main() {
    Person person1("김철수", 20);
    Person person2("엘리자베스", 25);
    Person* personPtr = &person2;
    
    person1.introduce();
    personPtr->introduce();

    (*personPtr).setAge(31);
    personPtr->introduce();

    return 0;
}
