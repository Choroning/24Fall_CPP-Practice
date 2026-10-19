/**
 * @file    A10204-1_C09Q1.cpp
 * @brief   Q1. 가상함수를 사용한 학생 성적 관리
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-27
 */

#include <iostream>
#include <string>
using namespace std;

class Student {
    string name;
protected:
    int kor, pro;
public:
    Student(string name, int kor, int pro) {
        this->name = name;
        this->kor = kor;
        this->pro = pro;
    }
    virtual double getAverage()  = 0;
    virtual void displayGrade() {
        cout << "이름: " << name << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
    }
    virtual ~Student() {}
};

class FirstGrade : public Student {
    int mat;
public:
    FirstGrade(string name, int kor, int pro, int mat) : Student(name, kor, pro) {
        this->mat = mat;
    }
    double getAverage() override {
        return (kor + pro + mat) / 3.0;
    }
    void displayGrade() override {
        cout << "[1학년]" << endl;
        Student::displayGrade();
        cout << "수학: " << mat << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~FirstGrade() override {}
};

class SecondGrade : public Student {
    int sci, mor;
public:
    SecondGrade(string name, int kor, int pro, int sci, int mor) : Student(name, kor, pro) {
        this->sci = sci;
        this->mor = mor;
    }
    double getAverage() override {
        return (kor + pro + sci + mor) / 4.0;
    }
    void displayGrade() override {
        cout << "[2학년]" << endl;
        Student::displayGrade();
        cout << "과학: " << sci << endl;
        cout << "도덕: " << mor << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~SecondGrade() override {}
};

class ThirdGrade : public Student {
    int eng, his;
public:
    ThirdGrade(string name, int kor, int pro, int eng, int his) : Student(name, kor, pro) {
        this->eng = eng;
        this->his = his;
    }
    double getAverage() override {
        return (kor + pro + eng + his) / 4.0;
    }
    void displayGrade() override {
        cout << "[3학년]" << endl;
        Student::displayGrade();
        cout << "영어: " << eng << endl;
        cout << "한국사: " << his << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
    ~ThirdGrade() override {}
};

int main() {
    Student* p[4];
    p[0] = new FirstGrade("고길동", 80, 70, 70);
    p[1] = new SecondGrade("둘리", 70, 60, 90, 70);
    p[2] = new ThirdGrade("또치", 90, 100, 90, 100);
    p[3] = new SecondGrade("도우너", 80, 90, 60, 100);
    
    for (int i = 0; i < 4; i++) {
        // 기본 클래스 포인터 p를 사용하여 displayGrade()를 올바르게 구현하시오.
        p[i]->displayGrade();
    }

    // delete 연산자를 사용해 동적할당 영역에 대한 반환 코드를 작성하시오.
    for (int i = 0; i < 4; i++) {
        delete p[i];
    }
    
    return 0;
}