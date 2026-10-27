# 4장 연습 문제 풀이 — 객체 활용

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-27
>
> 명품 C++ Programming, 황기태, Ch 4

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 동적 배열을 가진 클래스](#연습-문제-1-동적-배열을-가진-클래스)
- [연습 문제 2: 객체의 동적 생성 및 소멸](#연습-문제-2-객체의-동적-생성-및-소멸)
- [연습 문제 3: `Rect` 객체의 동적 생성과 반환](#연습-문제-3-rect-객체의-동적-생성과-반환)
- [연습 문제 4: `Rect` 객체 배열의 동적 생성](#연습-문제-4-rect-객체-배열의-동적-생성)
- [연습 문제 5: 객체 포인터로 멤버 함수 호출하기](#연습-문제-5-객체-포인터로-멤버-함수-호출하기)
- [연습 문제 6: `Palette` 객체의 동적 생성과 반환](#연습-문제-6-palette-객체의-동적-생성과-반환)
- [연습 문제 7: 객체 배열을 `delete`로 반환하는 코드](#연습-문제-7-객체-배열을-delete로-반환하는-코드)
- [연습 문제 8: `this`의 활용](#연습-문제-8-this의-활용)
- [연습 문제 9: 메모리 누수가 발생하는 상황](#연습-문제-9-메모리-누수가-발생하는-상황)
- [연습 문제 10: `string` 클래스의 헤더 파일](#연습-문제-10-string-클래스의-헤더-파일)
- [연습 문제 11: C-스트링과 `string` 클래스](#연습-문제-11-c-스트링과-string-클래스)

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

**실행 결과:** 터미널에서 실행하면 사용자가 입력한 숫자 줄 다음에 `write()`가 출력한 같은 숫자 줄이 한 번 더 나타나는데, 실행 예시에는 이 줄이 한 번만 보인다. `write()`는 각 수 뒤에 공백을 출력하므로 출력한 줄 끝에는 공백이 하나 남는다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 프로그램의 출력이 아니라 Xcode 콘솔이 덧붙이는 종료 메시지이다.

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

**실행 결과:** 반지름 10, 2, 1, 15인 원의 면적은 각각 314, 12.56, 3.14, 706.5이므로 최소 면적 10보다 큰 빈대떡, 도넛, 피자가 출력된다. 마지막 원 뒤에도 `", "`를 출력하므로 줄 끝에 쉼표가 남는다.

> **참고:** 원의 개수가 0 이하일 때 생성자는 `p`를 초기화하기 전에 예외를 던진다. 생성이 끝나지 않은 객체의 소멸자는 호출되지 않으므로 초기화되지 않은 `p`에 대해 `delete [] p`가 실행되는 일은 없다.

---

<br>

## 연습 문제 3: `Rect` 객체의 동적 생성과 반환

**교재 문항:** 4장 이론 문제 2번

**문제:** 입력받은 폭과 높이로 `Rect` 객체를 동적으로 생성하고, 면적을 출력한 뒤 객체를 반환하는 세 줄을 쓰라. 문제 전문과 코드는 [연습 문제 3](Exercises.ko.md#연습-문제-3-rect-객체의-동적-생성과-반환)에 있다.

### 풀이

**정답:**

```cpp
q = new Rect(w, h);              // (1)
cout << q->getArea() << endl;    // (2)
delete q;                        // (3)
```

**해설:** (1)의 `new Rect(w, h)`는 힙에 `Rect` 객체를 만들면서 매개변수 2개 생성자를 호출하고, 만든 객체의 주소를 리턴한다([4장 2.2절](Concepts_Lecture.ko.md#22-동적-객체)). (2)는 포인터로 멤버 함수를 호출하므로 `->`를 쓰며, `(*q).getArea()`도 같은 호출이다. 문제가 면적의 출력을 요구하므로 `q->getArea()`의 리턴값을 `cout`으로 출력해야 한다. (3)의 `delete q;`는 객체의 소멸자를 호출한 뒤 메모리를 반환한다. 반환한 뒤에는 `q = nullptr;`를 더해 댕글링 포인터로 남지 않게 할 수 있다.

---

<br>

## 연습 문제 4: `Rect` 객체 배열의 동적 생성

**교재 문항:** 4장 이론 문제 4번

**문제:** `new Rect[10]`이 컴파일 오류가 나지 않도록 `Rect` 클래스를 수정하라. 문제 전문과 코드는 [연습 문제 4](Exercises.ko.md#연습-문제-4-rect-객체-배열의-동적-생성)에 있다.

### 풀이

**정답:** `Rect` 클래스의 `public` 부분에 매개변수 없는 기본 생성자를 추가한다.

```cpp
Rect() { width = 1; height = 1; }
```

**해설:** `new Rect[10]`처럼 객체 배열을 만들면 원소마다 기본 생성자가 호출된다([4장 3.3절](Concepts_Lecture.ko.md#33-동적-객체-배열)). `Rect`에는 매개변수 2개 생성자만 있고, 생성자를 하나라도 선언한 클래스에는 컴파일러가 기본 생성자를 만들어 주지 않으므로 원소를 만들 방법이 없어 오류가 난다. 기본 생성자를 추가하면 열 개의 원소가 모두 1×1 사각형으로 만들어진다. 기본 생성자 대신 `Rect(int w = 1, int h = 1)`처럼 기존 생성자에 디폴트 매개변수를 주어도 된다.

---

<br>

## 연습 문제 5: 객체 포인터로 멤버 함수 호출하기

**교재 문항:** 4장 이론 문제 6번

**문제:** `Cube c; Cube *p = &c;` 뒤의 네 문장 중 컴파일 오류가 나는 것을 고르라. 문제 전문과 코드는 [연습 문제 5](Exercises.ko.md#연습-문제-5-객체-포인터로-멤버-함수-호출하기)에 있다.

### 풀이

**정답:** ④

**해설:** `c`는 포인터가 아니라 객체이므로 화살표 연산자 `->`를 쓸 수 없다. 객체는 `c.getVolume()`(①)처럼 점 연산자로, 포인터는 `p->getVolume()`(②)이나 역참조한 뒤 점 연산자를 쓰는 `(*p).getVolume()`(③)으로 멤버를 호출한다([4장 1.1절](Concepts_Lecture.ko.md#11-객체에-대한-포인터)).

---

<br>

## 연습 문제 6: `Palette` 객체의 동적 생성과 반환

**교재 문항:** 4장 이론 문제 8번

**문제:** `Palette` 객체를 동적으로 생성하고 반환할 때 출력되는 내용을 쓰라. 문제 전문과 코드는 [연습 문제 6](Exercises.ko.md#연습-문제-6-palette-객체의-동적-생성과-반환)에 있다.

### 풀이

**정답:**

```text
기본생성자
기본생성자
기본생성자
소멸자
소멸자
소멸자
```

**해설:** `new Palette()`가 `Palette`의 생성자를 호출하고, 생성자는 `new Color[3]`으로 `Color` 객체 세 개의 배열을 만들며 원소마다 기본 생성자를 호출한다. 그래서 "기본생성자"가 세 번 출력된다. `delete p;`는 `Palette`의 소멸자를 호출하고, 소멸자의 `delete [] p;`가 배열의 세 원소에 대해 `Color`의 소멸자를 호출하므로 "소멸자"가 세 번 출력된다. 배열 원소의 소멸자는 생성의 역순, 즉 `p[2]`, `p[1]`, `p[0]`의 순서로 호출된다. 매개변수 생성자는 호출되지 않는다.

---

<br>

## 연습 문제 7: 객체 배열을 `delete`로 반환하는 코드

**교재 문항:** 4장 이론 문제 10번

**문제:** `new Cube [4]`로 할당한 배열을 `delete p;`로 반환하는 코드의 문제점을 쓰라. 문제 전문과 코드는 [연습 문제 7](Exercises.ko.md#연습-문제-7-객체-배열을-delete로-반환하는-코드)에 있다.

### 풀이

**정답:** `new []`로 할당한 배열은 `delete [] p;`로 반환해야 하는데 `delete p;`로 반환하였다.

**해설:** `new`와 `delete`, `new []`와 `delete []`는 짝을 맞추어 써야 하며, 짝이 맞지 않으면 결과는 정의되지 않은 동작이다([4장 2.3절](Concepts_Lecture.ko.md#23-동적-배열)). `delete []`는 원소 네 개의 소멸자를 모두 호출한 뒤 배열 전체를 반환하지만, `delete`는 객체 하나를 반환하는 연산이므로 나머지 원소의 소멸자가 호출되지 않고 메모리 반환도 올바르게 이루어지지 않는다. 소멸자를 가진 `Cube`로 시험하면 Clang은 `'delete' applied to a pointer that was allocated with 'new[]'` 경고(`-Wmismatched-new-delete`)를 내고, 실행하면 소멸자가 한 번만 호출된 뒤 메모리를 반환하는 단계에서 프로그램이 비정상 종료된다. 원소가 동적 메모리를 가진 객체라면 소멸자가 호출되지 않은 원소의 메모리는 누수로 남는다.

---

<br>

## 연습 문제 8: `this`의 활용

**교재 문항:** 4장 이론 문제 12번

**문제:** `this`의 활용에 관한 설명 중 틀린 것을 고르라. 보기는 [연습 문제 8](Exercises.ko.md#연습-문제-8-this의-활용)에 있다.

### 풀이

**정답:** ③

**해설:** `this`는 멤버 함수가 호출된 객체의 주소를 가진 포인터로, 생성자를 포함한 모든 non-static 멤버 함수 안에서 쓸 수 있다. 생성자에서는 매개변수 이름이 멤버 변수 이름과 같을 때 `this->width = width;`처럼 멤버를 가리키는 데 흔히 쓴다([4장 1.2절](Concepts_Lecture.ko.md#12-this-포인터)). 나머지는 옳다. `this`는 멤버 함수 안에서만 의미가 있고(①), static 멤버 함수는 특정 객체 없이 호출되므로 `this`가 없으며(②), 멤버 함수가 `this`나 `*this`를 리턴하여 호출을 연결하기도 한다(④).

---

<br>

## 연습 문제 9: 메모리 누수가 발생하는 상황

**교재 문항:** 4장 이론 문제 14번

**문제:** 메모리 누수가 발생하는 상황을 설명하라. 문제 전문은 [연습 문제 9](Exercises.ko.md#연습-문제-9-메모리-누수가-발생하는-상황)에 있다.

### 풀이

**정답:** 동적으로 할당받은 메모리를 다 쓴 뒤에도 반환하지 않거나, 반환할 수 없게 되었을 때 발생한다.

**해설:** **메모리 누수(memory leak)** 는 `new`로 할당받은 힙 메모리를 `delete`로 반환하지 않은 채 그 메모리를 가리키는 포인터를 모두 잃어, 프로그램이 끝날 때까지 쓸 수도 반환할 수도 없는 메모리가 남는 것이다([4장 4.1절](Concepts_Lecture.ko.md#41-메모리-누수)). 대표적인 경우는 다음과 같다.

- 사용이 끝난 메모리를 `delete`하지 않고 함수가 끝나, 주소를 저장한 지역 포인터가 사라진다.
- 동적 메모리의 주소를 가진 포인터에 다른 주소를 대입하여 원래 주소를 잃어버린다.
- 동적으로 할당한 배열을 `delete []`가 아니라 `delete`로 반환한다.
- 동적 메모리를 가진 객체의 소멸자가 그 메모리를 반환하지 않는다.

누수가 쌓이면 사용할 수 있는 메모리가 줄어 오래 실행되는 프로그램이 느려지거나 할당에 실패한다. 프로그램이 끝나면 운영체제가 메모리를 회수하지만, 그전까지는 회수되지 않는다.

---

<br>

## 연습 문제 10: `string` 클래스의 헤더 파일

**교재 문항:** 4장 이론 문제 16번

**문제:** `string` 클래스를 사용하기 위해 필요한 헤더 파일을 고르라. 보기는 [연습 문제 10](Exercises.ko.md#연습-문제-10-string-클래스의-헤더-파일)에 있다.

### 풀이

**정답:** ①

**해설:** `string` 클래스는 C++ 표준 라이브러리의 헤더 `<string>`에 선언되어 있다([4장 7절](Concepts_Lecture.ko.md#7-string-클래스)). `<string.h>`와 `<cstring>`은 `strcpy()`, `strlen()` 같은 C-스트링 함수를 선언하는 C 라이브러리의 헤더이다. `<iostream>`을 포함하면 구현에 따라 `string`을 쓸 수 있는 경우가 있지만 표준이 보장하지 않으므로, `string`을 쓰는 파일은 `<string>`을 직접 포함한다.

---

<br>

## 연습 문제 11: C-스트링과 `string` 클래스

**교재 문항:** 4장 이론 문제 18번

**문제:** C-스트링과 `string` 클래스에 관한 설명 중 틀린 것을 고르라. 보기는 [연습 문제 11](Exercises.ko.md#연습-문제-11-c-스트링과-string-클래스)에 있다.

### 풀이

**정답:** ③

**해설:** `string` 클래스는 C++ 표준 라이브러리에 속하는 표준 클래스이므로, 사용을 피할 이유가 없고 오히려 C-스트링보다 권장된다. 나머지 설명은 옳다. C-스트링은 널 문자(`'\0'`)로 끝나는 `char` 배열로 문자열을 표현하고(①), `string` 클래스는 크기 조절, 대입, 연결, 비교를 객체의 멤버 함수와 연산자로 제공하므로 쓰기 쉽다(②). `string` 클래스에는 대문자와 소문자를 바꾸는 멤버 함수가 없으므로(④), 문자를 변환하려면 `<cctype>`의 `toupper()`, `tolower()`를 문자마다 적용한다([4장 7.2절](Concepts_Lecture.ko.md#72-주요-멤버-함수와-연산자)).
