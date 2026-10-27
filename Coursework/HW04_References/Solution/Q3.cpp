/**
 * @file    A4Q3.cpp
 * @brief   Q3. 참조를 클래스와 연계한 자유 문제 2
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
public:
    Person(const string& n = "", int a = 0) : name(n), age(a) {}
    
    string& getName() { return name; }
    int& getAge() { return age; }
    const string& getName() const { return name; }
    const int& getAge() const { return age; }
    void setName(const string& n) { name = n; }
    void setAge(const int& a) { age = a; }
    bool isOlderThan(const Person& other) const {
        return this->age > other.age;
    }
    void printInfo() const {
        cout << "Name: " << name << ", Age: " << age << endl;
    }
};

void celebrateBirthday(Person& person) {
    person.getAge()++;
}

int main() {
    Person person1("Alice", 25);
    Person person2("Bob", 30);
    
    person1.getName() = "Alice Smith";
    
    person1.setName("Alice Johnson");
    person1.setAge(26);
    
    cout << "Person 1: ";
    person1.printInfo();
    cout << "Person 2: ";
    person2.printInfo();
    
    if (person1.isOlderThan(person2)) {
        cout << person1.getName() << " is older" << endl;
    } else {
        cout << person2.getName() << " is older or same age" << endl;
    }
    
    celebrateBirthday(person1);
    cout << "\nAfter birthday celebration:" << endl;
    person1.printInfo();
    
    return 0;
}
