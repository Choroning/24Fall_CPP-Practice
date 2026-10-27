/**
 * @file    A3Q2.cpp
 * @brief   Q2. 회원 정보를 관리하는 Member 클래스 (6주차 문제 6-2)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Member {
    string name;
    string id;
    int age;
public:
    Member() {
        name = "noname";
        id = "none";
        age = 0;
    }
    Member(string name, string id, int age) {
        this->name = name;
        this->id = id;
        this->age = age;
    }
    void setName(string name) {
        this->name = name;
    }
    void setId(string id) {
        this->id = id;
    }
    void setAge(int age) {
        this->age = age;
    }
    
    string getName() {
        return name;
    }
    string getId() {
        return id;
    }
    int getAge() {
        return age;
    }
    void printM() {
        cout << name << ", " << id << ", " << age << endl;
    }
};

int main()
{
    Member Kim;
    Member Lee("Lee Hi", "2016001", 22);
    Kim.printM();
    Lee.printM();
    Kim.setName("Kim Haha");
    Kim.setId("2016020");
    Kim.setAge(29);
    Kim.printM();
}
