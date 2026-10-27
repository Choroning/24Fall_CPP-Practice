/**
 * @file    A7Q3.cpp
 * @brief   Q3. 연산자 중복 관련 자유 코드 (MyString 클래스)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-11-24
 */

#include <iostream>
#include <cstring>
using namespace std;

class MyString {
    char* str;
    int len;
public:
    // 생성자
    MyString(const char* s = "") {
        len = strlen(s);
        str = new char[len + 1];
        strcpy(str, s);
    }
    
    // 복사 생성자
    MyString(const MyString& s) {
        len = s.len;
        str = new char[len + 1];
        strcpy(str, s.str);
    }
    
    // 소멸자
    ~MyString() {
        if(str) delete[] str;
    }
    
    // = 연산자 중복
    MyString& operator=(const MyString& s) {
        if(this == &s) return *this;
        if(str) delete[] str;
        len = s.len;
        str = new char[len + 1];
        strcpy(str, s.str);
        return *this;
    }
    
    // + 연산자 중복 (문자열 연결)
    MyString operator+(const MyString& s) {
        char* temp = new char[len + s.len + 1];
        strcpy(temp, str);
        strcat(temp, s.str);
        MyString result(temp);
        delete[] temp;
        return result;
    }
    
    // < 연산자 중복 (사전식 비교)
    bool operator<(const MyString& s) {
        return strcmp(str, s.str) < 0;
    }
    
    // == 연산자 중복
    bool operator==(const MyString& s) {
        return strcmp(str, s.str) == 0;
    }

    // += 연산자 중복
    MyString& operator+=(const MyString& s) {
        char* temp = new char[len + s.len + 1];
        strcpy(temp, str);
        strcat(temp, s.str);
        if(str) delete[] str;
        str = temp;
        len = len + s.len;
        return *this;
    }
    
    // [] 연산자 중복 (인덱스로 문자 접근)
    char& operator[](int index) {
        if(index < 0 || index >= len) {
            cout << "Index out of range" << endl;
            exit(1);
        }
        return str[index];
    }
    
    string show() {
        return str;
    }
};

int main() {
    MyString s1("Hello ");
    MyString s2("World!");
    MyString s3;
    
    cout << "[초기 상태]" << endl;
    cout << "s1: " << s1.show() << endl;
    cout << "s2: " << s2.show() << endl;
    cout << "s3: " << s3.show() << endl << endl;
    
    cout << "[연산 수행]" << endl;
    // + 연산자 사용
    s3 = s1 + s2;
    cout << "s1 + s2 = " << s3.show() << endl;
    
    // += 연산자 사용
    s1 += "C++";
    cout << "s1 += 'C++' = " << s1.show() << endl;
    
    // < 연산자 사용 (사전식 비교)
    MyString a("Apple");
    MyString b("Banana");
    if(a < b)
        cout << a.show() << "은 " << b.show() << "보다 사전순으로 앞에 있습니다." << endl;
    
    // [] 연산자 사용
    cout << "s2의 첫 번째 문자: " << s2[0] << endl;
    
    // == 연산자 사용
    MyString str1("Test");
    MyString str2("Test");
    if(str1 == str2)
        cout << "두 문자열이 같습니다." << endl;
    
    return 0;
}
