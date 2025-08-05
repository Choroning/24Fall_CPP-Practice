/**
 * @file    A10204-1_C03Q2.cpp
 * @brief   Q2. 커피 머신
 * @author  박철원 (Cheolwon Park)
 * @date    2024-09-26
 */

#include <iostream>
using namespace std;

class CoffeeMachine{
private:
    int coffeeAmount;
    int waterAmount;
    int sugarAmount;
public:
    CoffeeMachine(int c, int w, int s);
    void drinkAmericano();
    void drinkSugarCoffee();
    void drinkEspresso();
    void show();
    void fill();
};

CoffeeMachine::CoffeeMachine(int c, int w, int s){
    coffeeAmount = c;
    waterAmount = w;
    sugarAmount = s;
}

void CoffeeMachine::drinkAmericano(){
    if (coffeeAmount >= 1 && waterAmount >= 2){
        coffeeAmount -= 1;
        waterAmount -= 2;
    }else{
        cout << "\n재료가 부족하여 아메리카노를 만들 수 없습니다.\n";
        show();
    }
}

void CoffeeMachine::drinkSugarCoffee(){
    if (coffeeAmount >= 1 && waterAmount >= 2 && sugarAmount >= 1){
        coffeeAmount -= 1;
        waterAmount -= 2;
        sugarAmount -= 1;
    }else{
        cout << "\n재료가 부족하여 설탕커피를 만들 수 없습니다.\n";
        show();
    }
}

void CoffeeMachine::drinkEspresso(){
    if (coffeeAmount >= 1 && waterAmount >= 1){
        coffeeAmount -= 1;
        waterAmount -= 1;
    }else{
        cout << "\n재료가 부족하여 에스프레소를 만들 수 없습니다.\n";
        show();
    }
}

void CoffeeMachine::show(){
    cout << "커피머신 상태: 커피 = " << coffeeAmount << "   물 = " << waterAmount << "   설탕 = " << sugarAmount << endl;
}

void CoffeeMachine::fill(){
    coffeeAmount = 10;
    waterAmount = 10;
    sugarAmount = 10;
}


int main()
{
    CoffeeMachine java(5, 8, 2);  // 커피 5, 물 8, 설탕 2로 초기화
    
    java.drinkAmericano();
    java.show();
    
    java.drinkSugarCoffee();
    java.show();
    
    java.drinkEspresso();
    java.show();
    
    java.drinkSugarCoffee();
    java.show();
    
    java.drinkSugarCoffee();
    
    java.drinkAmericano();
    
    java.fill();
    java.show();
    
    return 0;
}
