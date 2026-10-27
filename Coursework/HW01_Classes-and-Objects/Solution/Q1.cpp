/**
 * @file    A1Q1.cpp
 * @brief   Q1. 은행 계좌 Account 클래스 (교재 3장 실습 문제 3번)
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-28
 */

#include <iostream>
#include <string>
using namespace std;

class Account{
public:
    string name;
    int id;
    int balance;
    
    Account();
    Account(string, int, int);
    string getOwner();
    void deposit(int);
    int withdraw(int);
    int inquiry();
};

Account::Account(string n, int i, int b){
    name = n;
    id = i;
    balance = b;
}

string Account::getOwner(){
    return name;
}

void Account::deposit(int m){
    balance += m;
}

int Account::withdraw(int m){
    balance -= m;
    return m;
}

int Account::inquiry(){
    return balance;
}


int main()
{
    Account a("kitae", 1, 5000);
    a.deposit(50000);
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
    int money = a.withdraw(20000);
    cout << a.getOwner() << "의 잔액은 " << a.inquiry() << endl;
}
