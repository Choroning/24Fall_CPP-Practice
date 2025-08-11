/**
 * @file    A10204-1_C08Q2.cpp
 * @brief   Q2. 학생 성적 관리
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-20
 */

#include <iostream>
using namespace std;

class Student {
    string name;
protected:
    int kor;
    int pro;
    string getName() {
        return name;
    }
    Student(string s, int kors, int pros) {
        name = s;
        kor = kors;
        pro = pros;
    }
};

class FirstGrade : public Student {
    int mat;
public:
    FirstGrade(string s, int kors, int pros, int mats) : Student(s, kors, pros) {
        mat = mats;
    }
    double getAverage() {
        return (kor + pro + mat) / 3.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "수학: " << mat << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

class SecondGrade : public Student {
    int sci;
    int mor;
public:
    SecondGrade(string s, int kors, int pros, int scis, int mors) : Student(s, kors, pros) {
        sci = scis;
        mor = mors;
    }
    double getAverage() {
        return (kor + pro + sci + mor) / 4.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "과학: " << sci << endl;
        cout << "도덕: " << mor << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

class ThirdGrade : public Student {
    int eng;
    int his;
public:
    ThirdGrade(string s, int kors, int pros, int engs, int hiss) : Student(s, kors, pros) {
        eng = engs;
        his = hiss;
    }
    double getAverage() {
        return (kor + pro + eng + his) / 4.0;
    }
    void display() {
        cout << "이름: " << getName() << endl;
        cout << "국어: " << kor << endl;
        cout << "정보: " << pro << endl;
        cout << "영어: " << eng << endl;
        cout << "한국사: " << his << endl;
        cout << "---------------" << endl;
        cout << "평균: " << getAverage() << endl << endl;
    }
};

int main() {
    // 이름, 국어, 정보, 수학 점수 초기화
    FirstGrade f("홍길동", 90, 81, 88);

    // 이름, 국어, 정보, 과학, 도덕 점수 초기화
    SecondGrade s("장영실", 73, 91, 99, 68);

    // 이름, 국어, 정보, 영어, 한국사 점수 초기화
    ThirdGrade t("정도전", 81, 53, 80, 96);
    
    f.display();
    s.display();
    t.display();
    
    return 0;
}
