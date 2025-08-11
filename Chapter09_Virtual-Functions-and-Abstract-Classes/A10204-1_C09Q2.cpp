/**
 * @file    A10204-1_C09Q2.cpp
 * @brief   Q2. 가상함수를 사용한 단위 변환
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-27
 */

#include <iostream>
using namespace std;

class Convert {
protected:
    double before, after;
public:
    Convert(double before) {
        this->before = before;
    }
    virtual void compute() = 0;
    virtual void display() = 0;
    virtual ~Convert() {}
};

class LtoG : public Convert {
public:
    LtoG(double val) : Convert(val) {}
    void compute() override {
        after = before / 3.7854;
    }
    void display() override {
        cout << before << " 리터 = " << after << " 갤런" << endl;
    }
    ~LtoG() override {}
};

class CtoF : public Convert {
public:
    CtoF(double val) : Convert(val) {}
    void compute() override {
        after = (9.0/5.0) * before + 32;
    }
    void display() override {
        cout << "섭씨 " << before << "도 = 화씨 " << after << "도" << endl;
    }
    ~CtoF() override {}
};

class MtoKm : public Convert {
public:
    MtoKm(double val) : Convert(val) {}
    void compute() override {
        after = before * 1.609344;
    }
    void display() override {
        cout << before << " 마일 = " << after << " 킬로미터" << endl;
    }
    ~MtoKm() override {}
};

int main() {
    Convert* p[3] = { 
        new MtoKm(23.2), new LtoG(10.3), new CtoF(24.5)
    };
    
    for (int i = 0; i < 3; i++) {
        // 기본 클래스 포인터 p를 사용하여 단위 변환을 수행하시오.
        p[i]->compute();

        // 기본 클래스 포인터 p를 사용하여 변환 결과를 출력하시오.
        p[i]->display();
    }

    // delete 연산자를 사용해 동적할당 영역에 대한 반환 코드를 작성하시오.
    for (int i = 0; i < 3; i++) {
        delete p[i];
    }
    
    return 0;
}
