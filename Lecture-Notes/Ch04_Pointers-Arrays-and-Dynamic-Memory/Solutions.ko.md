# 4장 연습 문제 풀이 — 객체 활용

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 4

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 동적 배열을 가진 클래스](#연습-문제-1-동적-배열을-가진-클래스)
- [연습 문제 2: 객체의 동적 생성 및 소멸](#연습-문제-2-객체의-동적-생성-및-소멸)

---

<br>

## 연습 문제 1: 동적 배열을 가진 클래스

**교재 문항:** 4장 실습 문제 4번

**문제:** 생성자에서 입력받은 크기의 정수 배열을 동적으로 할당하는 `Sample` 클래스의 `read()`, `write()`, `big()`과 소멸자를 완성하고, 동적 할당 실패와 0 이하의 배열 크기를 `try-catch`로 처리하라. 문제 전문과 실행 예시는 [연습 문제 1](Exercises.ko.md#연습-문제-1-동적-배열을-가진-클래스)에 있다.

### 풀이

**접근 방법:** 오류를 발견하는 곳과 처리하는 곳을 나눈다. 크기 오류는 생성자가 발견하여 예외를 던지고, 할당 실패는 `new`가 `bad_alloc` 예외로 알린다. `main()`은 두 예외를 모두 받아 오류 메시지를 출력하고 정상 종료한다.

- **생성자:** 크기를 먼저 검사하여 0 이하이면 `int` 값 1을 `throw`하고, 그렇지 않을 때만 `new int [size]`로 배열을 할당한다. 크기를 `new`보다 먼저 검사해야 하는 이유는 두 가지이다. 음수 크기로 `new`를 호출하면 `bad_alloc` 계열의 예외가 발생하여 크기 오류가 할당 실패로 보고되고, 크기가 0이면 할당은 성공하지만 원소가 없는 배열이 되어 `big()`의 `p[0]` 접근이 배열 밖을 읽는다.
- **`read()`와 `write()`:** `read()`는 `cin >>`으로 `size`개의 정수를 차례로 읽고, `write()`는 각 수 뒤에 공백을 하나 붙여 출력한 뒤 줄을 바꾼다.
- **`big()`:** 첫 원소 `p[0]`을 최댓값의 초깃값으로 두고 나머지 원소와 조건 연산자로 비교한다. 첫 원소로 시작하므로 모든 수가 음수여도 올바른 최댓값을 구한다.
- **소멸자:** `new []`로 할당한 배열이므로 `delete [] p;`로 반환하고 `p`에 `nullptr`를 대입한다.
- **`main()`:** 객체 생성부터 출력까지를 `try` 블록에 넣고, `catch (bad_alloc)`에서 할당 실패를, `catch (int e)`에서 크기 오류를 처리한다. 생성자가 예외를 던지면 객체는 생성되지 않은 것으로 간주되어 소멸자가 호출되지 않으므로, 할당되지 않은 배열을 `delete []`하는 일은 생기지 않는다.

**코드:** [Lab_Q1.cpp](../../Coursework/Lab04_Pointers-Arrays-and-Dynamic-Memory/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C04Q1.cpp
 * @brief   Q1. 동적 배열을 가진 클래스
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
using namespace std;

class Sample {
    int *p = nullptr;
    int size;
public:
    Sample(int n) {
        size = n;
        if (size <= 0) {
            throw 1;
        } else {
            p = new int [size];
        }
    }
    void read();
    void write();
    int big();
    ~Sample();
};

void Sample::read() {
    for (int i = 0; i < size; i++) {
        cin >> p[i];
    }
}
void Sample::write() {
    for (int i = 0; i < size; i++) {
        cout << p[i] << ' ';
    }
    cout << endl;
}
int Sample::big() {
    int max = p[0];
    for (int i = 1; i < size; i++) {
        max = (p[i] > max) ? p[i] : max;
    }
    return max;
}
Sample::~Sample() {
    delete [] p;
    p = nullptr;
}

int main() {
    int input;
    try {
        cout << "생성할 정수 배열 사이즈를 입력하세요: ";
        cin >> input;
        Sample s(input);
        s.read();
        s.write();
        cout << "가장 큰 수는 " << s.big() << endl;
    }
    catch (bad_alloc) {
        cout << "[오류] 동적할당에 실패했습니다!\n";
        return 0;
    }
    catch (int e) {
        cout << "[오류] 사이즈가 너무 작습니다!\n";
        return 0;
    }
    
    return 0;
}
```

**실행 결과:** 두 실행 예시와 같은 값을 입력하면 같은 결과가 나온다. 다만 터미널에서 실행하면 사용자가 입력한 숫자 줄 다음에 `write()`가 출력한 같은 숫자 줄이 한 번 더 나타나는데, 실행 예시에는 이 줄이 한 번만 보인다. `write()`는 각 수 뒤에 공백을 출력하므로 출력한 줄 끝에는 공백이 하나 남는다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 프로그램의 출력이 아니라 Xcode 콘솔이 덧붙이는 종료 메시지이다.

```text
생성할 정수 배열 사이즈를 입력하세요: 10
100 4 -2 9 55 300 44 38 99 -500
100 4 -2 9 55 300 44 38 99 -500 
가장 큰 수는 300
```

> **참고:** `bad_alloc`은 `<new>` 헤더에 선언되어 있으며, 이 코드는 `<iostream>`이 내부적으로 `<new>`를 포함하는 구현에서 컴파일된다. 이식성을 높이려면 `#include <new>`를 추가한다. 또한 `catch (bad_alloc)`처럼 예외를 값으로 받으면 예외 객체가 복사되므로, 표준 예외는 `catch (const bad_alloc &e)`처럼 참조로 받고 필요하면 `e.what()`으로 메시지를 확인하는 것이 관례이다.

---

<br>

## 연습 문제 2: 객체의 동적 생성 및 소멸

**교재 문항:** 4장 실습 문제 12번

**문제:** 주어진 선언을 바꾸지 않고 `Circle`과 `CircleManager` 클래스를 완성하라. `CircleManager`는 생성자에서 원의 개수만큼 `Circle` 배열을 동적으로 생성하고 각 원의 이름과 반지름을 입력받으며, 이름으로 면적을 검색하는 `searchByName()`과 입력한 최소 면적보다 큰 원을 출력하는 `searchByArea()`를 제공한다. 원의 개수가 0 이하이거나 동적 할당에 실패하면 `try-catch`로 처리한다. 문제 전문과 실행 예시는 [연습 문제 2](Exercises.ko.md#연습-문제-2-객체의-동적-생성-및-소멸)에 있다.

### 풀이

**접근 방법:** `CircleManager`가 `Circle` 객체 배열의 생성, 사용, 반환을 모두 맡는다. 동적 객체 배열은 초기화 목록 없이 `new Circle [size]`로 만들면 각 원소가 기본 생성자로 생성되므로([4장 3.3절](Concepts_Lecture.ko.md#33-동적-객체-배열)), 먼저 배열을 만든 뒤 입력받은 값을 `setCircle()`로 각 원소에 설정한다.

- **`Circle`:** 생성자를 선언하지 않았으므로 컴파일러가 기본 생성자를 제공하고, 그 덕분에 `new Circle [size]`가 컴파일된다. `setCircle()`은 매개변수 이름이 멤버 이름과 같으므로 `this->radius`, `this->name`으로 멤버를 구분한다([4장 1.2절](Concepts_Lecture.ko.md#12-this-포인터)). `getArea()`는 매크로 `PI`(3.14)로 $\pi r^2$을 계산한다.
- **생성자:** 원의 개수를 검사하여 0 이하이면 `int` 값 1을 `throw`하고, 그렇지 않으면 배열을 할당한 뒤 원마다 이름과 반지름을 `cin >>`으로 읽는다. 이름은 `cin >>`으로 읽으므로 공백 없는 한 단어여야 한다.
- **소멸자:** `delete [] p;`로 배열을 반환하며, 이때 각 원소의 소멸자가 생성의 반대 순서로 실행된다.
- **`searchByName()`:** 입력한 이름과 같은 원을 모두 찾아 면적을 출력한다. 같은 이름이 없으면 아무것도 출력하지 않는다.
- **`searchByArea()`:** 정수로 입력받은 최소 면적보다 면적이 큰 원의 이름과 면적을 `", "`로 구분하여 한 줄에 출력한다.

**코드:** [Lab_Q2.cpp](../../Coursework/Lab04_Pointers-Arrays-and-Dynamic-Memory/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C04Q2.cpp
 * @brief   Q2. 객체의 동적 생성 및 소멸
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-11
 */

#include <iostream>
#define PI 3.14
using namespace std;

class Circle {
   int radius;    // 원의 반지름
   string name;   // 원의 이름
public:
   void setCircle(string name, int radius);  // 이름과 반지름 설정
   double getArea();
   string getName();
};

void Circle::setCircle(string name, int radius) {
    this->radius = radius;
    this->name = name;
}
double Circle::getArea() {
    return PI * radius * radius;
}
string Circle::getName() {
    return name;
}


class CircleManager {
   Circle *p;     // Circle 배열에 대한 포인터
   int size;      // 배열의 크기
public:
   CircleManager(int size);   // 사용자로부터 입력받은 size 값 크기의 배열을 동적 생성
   ~CircleManager();
   void searchByName(); // 사용자로부터 원의 이름을 입력받아 면적 출력
   void searchByArea(); // 사용자로부터 면적을 입력받아 면적보다 큰 원의 이름 출력
};

CircleManager::CircleManager(int size) {
    this->size = size;
    
    if (this->size <= 0) {
        throw 1;
    } else {
        p = new Circle [this->size];
        
        for (int i = 0; i < this->size; i++) {
            string temp1 = "";
            int temp2;
            
            cout << "원 " << i + 1 << "의 이름과 반지름 >> ";
            cin >> temp1 >> temp2;
            
            p[i].setCircle(temp1, temp2);
        }
    }
}
CircleManager::~CircleManager() {
    delete [] p;
    p = nullptr;
}
void CircleManager::searchByName() {
    string temp3 = "";
    cout << "검색하고자 하는 원의 이름 >> ";
    cin >> temp3;
    
    for (int i = 0; i < size; i++) {
        if (p[i].getName() == temp3) {
            cout << temp3 << "의 면적은 " << p[i].getArea() << endl;
        }
    }
}
void CircleManager::searchByArea() {
    int temp4;
    cout << "최소 면적을 정수로 입력하세요 >> ";
    cin >> temp4;
    cout << temp4 << "보다 큰 원을 검색합니다.\n";
    for (int i = 0; i < size; i++) {
        if (p[i].getArea() > temp4) {
            cout << p[i].getName() << "의 면적은 " << p[i].getArea() << ", ";
        }
    }
    cout << endl;
}


int main(void)
{
    try {
        int val;
        cout << "원의 개수 >> ";
        cin >> val;
        CircleManager c(val);
        c.searchByName();
        c.searchByArea();
    }
    catch (bad_alloc) {
        cout << "[오류] 동적할당에 실패했습니다!\n";
        return 0;
    }
    catch (int e) {
        cout << "[오류] 사이즈가 너무 작습니다!\n";
        return 0;
    }
    
    return 0;
}
```

**실행 결과:** 두 실행 예시와 같다. 반지름 10, 2, 1, 15인 원의 면적은 각각 314, 12.56, 3.14, 706.5이므로 최소 면적 10보다 큰 빈대떡, 도넛, 피자가 출력된다. 마지막 원 뒤에도 `", "`를 출력하므로 줄 끝에 쉼표가 남는 것도 실행 예시와 같다.

> **참고:** 원의 개수가 0 이하일 때 생성자는 `p`를 초기화하기 전에 예외를 던진다. 생성이 끝나지 않은 객체의 소멸자는 호출되지 않으므로 초기화되지 않은 `p`에 대해 `delete [] p`가 실행되는 일은 없다.
