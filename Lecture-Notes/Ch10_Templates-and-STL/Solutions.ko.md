# 10장 연습 문제 풀이 — 템플릿과 표준 라이브러리 (STL)

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-11-19

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

연습 문제 1과 2는 위 과목의 실습 문제이고, 연습 문제 3\~9는 10장의 개념을 확인하도록 따로 만든 문제이다.

---

<br>

## 목차

- [연습 문제 1: 제네릭 함수](#연습-문제-1-제네릭-함수)
- [연습 문제 2: 제네릭 클래스와 구체화 디버깅](#연습-문제-2-제네릭-클래스와-구체화-디버깅)
- [연습 문제 3: 템플릿 인자의 추론](#연습-문제-3-템플릿-인자의-추론)
- [연습 문제 4: 템플릿 함수와 일반 함수의 선택](#연습-문제-4-템플릿-함수와-일반-함수의-선택)
- [연습 문제 5: 제네릭 원형 큐](#연습-문제-5-제네릭-원형-큐)
- [연습 문제 6: vector의 변경 연산 추적](#연습-문제-6-vector의-변경-연산-추적)
- [연습 문제 7: 원소를 지우는 반복문의 오류](#연습-문제-7-원소를-지우는-반복문의-오류)
- [연습 문제 8: 단어 빈도의 정렬 출력](#연습-문제-8-단어-빈도의-정렬-출력)
- [연습 문제 9: 람다 표현식의 캡처](#연습-문제-9-람다-표현식의-캡처)

---

<br>

## 연습 문제 1: 제네릭 함수

**문제:** 배열의 원소를 역순으로 재정렬하는 `reverseArray()`와 배열의 내용을 출력하는 `display()`를 템플릿을 활용한 제네릭 함수로 작성하고, `char` 배열 `str`에 저장된 문자열 `"C++ is powerful"`을 뒤집는 호출문으로 `main()`의 빈칸을 채운다. 주어진 예시 코드는 바꿀 수 없다. 예시 코드와 실행 예시는 [연습 문제 1](Exercises.ko.md#연습-문제-1-제네릭-함수)에 있다.

### 풀이

**`reverseArray()` 템플릿:** `template <typename T> void reverseArray(T arr[], int size)`로 선언하면 `reverseArray(x, 4)`에서는 `T`가 `int`로, `reverseArray(y, 6)`에서는 `double`로 추론되어 두 함수가 구체화된다([10장 1.3절](Concepts_Lecture.ko.md#13-템플릿-구체화)). 본문은 앞에서 `i`번째 원소와 뒤에서 `i`번째 원소(`arr[size - 1 - i]`)를 `T` 타입 임시 변수 `tmp`로 교환하며, 교환은 배열의 앞쪽 절반인 `i < size/2` 동안만 한다. 끝까지 교환하면 앞쪽 절반에서 바꾼 원소를 뒤쪽 절반에서 다시 바꾸어 원래 순서로 돌아가기 때문이다. 원소가 4개이면 (0, 3), (1, 2) 두 쌍을, 6개이면 (0, 5), (1, 4), (2, 3) 세 쌍을 교환하고, 원소 수가 홀수이면 가운데 원소는 제자리에 남는다.

**`display()` 템플릿:** 배열을 읽기만 하므로 매개변수를 `const T arr[]`로 선언하고, 각 원소 뒤에 공백 두 칸을 붙여 출력한 다음 줄을 바꾼다.

**문자열을 뒤집는 호출문:** `str`은 크기가 20인 `char` 배열이지만 실제 문자열 `"C++ is powerful"`은 15글자이고 그 뒤에 널 문자 `'\0'`이 있다. 템플릿을 `reverseArray(str, 20)`으로 호출하면 뒤쪽의 널 문자들까지 뒤집혀 `str[0]`이 `'\0'`이 되므로 `str`은 빈 문자열로 출력된다. 그래서 풀이는 템플릿과 별도로 `char` 배열만 받는 일반 함수 `void reverseArray(char arr[])`를 중복 정의하였다. 이 함수는 널 문자가 나올 때까지 글자 수 `size`를 센 다음 앞의 15글자만 같은 방법으로 교환하며, 빈칸에는 `reverseArray(str);`을 쓴다. 인자가 하나인 호출에는 매개변수가 둘인 템플릿이 맞지 않으므로 일반 함수가 호출된다([10장 1.5절](Concepts_Lecture.ko.md#15-템플릿-함수와-일반-함수의-중복)). 일반 함수를 따로 만들지 않고 `<cstring>`을 포함하여 템플릿을 `reverseArray(str, strlen(str));`로 호출해도 같은 결과를 얻는다.

**코드:** [`Lab_Q1.cpp`](../../Coursework/Lab10_Templates-and-STL/Lab_Q1.cpp)

```cpp
/**
 * @file    A10204-1_C10Q1.cpp
 * @brief   Q1. 제네릭 함수
 * @author  박철원 (Cheolwon Park)
 * @date    2024-12-04
 */

#include <iostream>
using namespace std;

// reverseArray() 정의
template <typename T>
void reverseArray(T arr [], int size) {
    T tmp;
    for (int i = 0; i < size/2; i++) {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

void reverseArray(char arr []) {
    int size = 0;
    while(arr[size])
        size++;
    
    char tmp;
    for (int i = 0; i < size/2; i++) {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

// display() 정의
template <typename T>
void display(const T arr [], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << "  ";
    }
    cout << endl;
}

int main() {
   int x[] = { 1, 10, 100, 1000 };

   reverseArray(x, 4);
   cout << "x = ";
   display(x, 4);

   double y[] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6 };
   reverseArray(y, 6);
   cout << "y = ";
   display(y, 6);

   char str[20] = "C++ is powerful";
   // str을 뒤집는 reverseArray() 호출문
   reverseArray(str);
   cout << "str = " << str << endl;

   return 0;
}
```

**실행 결과:** 실행하면 다음과 같이 출력된다. `display()`가 마지막 원소 뒤에도 공백 두 칸을 출력하므로 `x`와 `y` 줄의 끝에는 화면에 보이지 않는 공백이 붙는다. 실행 예시의 마지막 줄 `Program ended with exit code: 0`은 프로그램이 출력한 것이 아니라 개발 환경(Xcode)이 프로그램 종료 후 표시하는 메시지이다.

```text
x = 1000  100  10  1  
y = 6.6  5.5  4.4  3.3  2.2  1.1  
str = lufrewop si ++C
```

---

<br>

## 연습 문제 2: 제네릭 클래스와 구체화 디버깅

**문제:** 클래스 템플릿 `Rectangle`의 멤버 함수 `getArea()`를 클래스 밖에서 정의하고, `double`형 `Rectangle` 객체 `rect1`(10.2, 23.5), `rect2`(20.4, 13.7), `rect3`을 선언하여 `main()`의 빈칸을 채운다. 그러면 `rect3 = bigger(rect1, rect2);`에서 컴파일 오류가 나므로, 그 원인을 찾아 두 가지 방법으로 고친다. 예시 코드와 목표 실행 결과는 [연습 문제 2](Exercises.ko.md#연습-문제-2-제네릭-클래스와-구체화-디버깅)에 있다.

### 풀이

**빈칸 채우기:** 클래스 템플릿의 멤버 함수를 클래스 밖에서 정의할 때는 템플릿 선언을 다시 쓰고 `Rectangle<T>::`로 소속을 밝힌다([10장 2.1절](Concepts_Lecture.ko.md#21-문법)). `getArea()`는 `width * height`를 리턴한다. 객체는 `Rectangle<double> rect1(10.2, 23.5), rect2(20.4, 13.7), rect3;`으로 선언하며, `rect3`은 생성자의 기본값에 따라 너비와 높이가 모두 1로 초기화된다.

**오류 원인:** `bigger(a, b)`는 `T = int`로 구체화되고 `int`에는 `>` 연산자가 있으므로 문제가 없다. 그러나 `bigger(rect1, rect2)`는 `T = Rectangle<double>`로 구체화되며, 구체화된 본문의 `if (a > b)`는 두 `Rectangle<double>` 객체를 `>`로 비교한다. `Rectangle` 클래스에는 `operator>`가 없으므로 이 구체화가 실패하여 컴파일 오류가 난다. 템플릿 자체의 문법에는 잘못이 없고, 특정 타입으로 구체화할 때 그 타입이 템플릿 본문이 요구하는 연산을 지원하지 않아서 생기는 오류이다([10장 2.3절](Concepts_Lecture.ko.md#23-템플릿에서의-연산자-중복), [10장 10.2절](Concepts_Lecture.ko.md#102-흔한-오류)).

**방법 1, 연산자 중복:** `Rectangle` 클래스에 두 객체의 면적을 비교하는 멤버 함수 `bool operator>(Rectangle op2)`를 추가한다. 그러면 `bigger<Rectangle<double>>`의 `a > b`가 `a.operator>(b)`로 해석되어 구체화가 성공한다. 이 연산자는 클래스 템플릿의 멤버이므로 `Rectangle<double>`, `Rectangle<int>`처럼 구체화되는 클래스마다 함께 생성된다.

**방법 2, 더 특수화된 `bigger()` 템플릿:** `Rectangle<T>` 객체 두 개를 받아 면적이 큰 쪽을 조건 연산자로 리턴하는 함수 템플릿 `Rectangle<T> bigger(Rectangle<T> a, Rectangle<T> b)`를 추가한다. `bigger(rect1, rect2)`에는 원래 템플릿(`T = Rectangle<double>`)과 이 템플릿(`T = double`)이 모두 똑같이 정확히 맞지만, 컴파일러는 더 특수화된 이 템플릿을 고르므로 `>`로 객체를 비교하는 원래 본문은 구체화되지 않는다([10장 1.5절](Concepts_Lecture.ko.md#15-템플릿-함수와-일반-함수의-중복)).

풀이 코드에는 두 방법이 모두 들어 있다. 이때 `bigger(rect1, rect2)`는 방법 2의 템플릿을 호출하며, 방법 1의 `operator>`는 이 호출에 쓰이지 않는다. 두 방법 중 하나만 남겨도 각각 컴파일되고 같은 결과를 출력한다.

**결과 계산:** `rect1`의 면적은 10.2 × 23.5 = 239.7이고 `rect2`의 면적은 20.4 × 13.7 = 279.48이므로 `bigger()`는 `rect2`를 리턴하여 `rect3`에 대입한다. `double`로 계산한 면적은 279.47999...이지만 `cout`의 기본 정밀도(유효 숫자 6자리)로 출력하면 279.48이 된다.

**코드:** [`Lab_Q2.cpp`](../../Coursework/Lab10_Templates-and-STL/Lab_Q2.cpp)

```cpp
/**
 * @file    A10204-1_C10Q2.cpp
 * @brief   Q2. 제네릭 클래스와 구체화 디버깅
 * @author  박철원 (Cheolwon Park)
 * @date    2024-12-10
 */

#include <iostream>
using namespace std;

template <typename T>
class Rectangle {
    T width;
    T height;
public:
    Rectangle(T w = 1, T h = 1) { width = w; height = h; }
    T getArea();
    
    /*
     * [방법1] 연산자 중복을 통한 해결
     * 이때, 제네릭 함수는 되도록 멤버함수로 중복하는 것이 더 좋은 방법이다.
     * (불가능한 케이스 제외)
     */
    bool operator > (Rectangle op2) {       
        if (getArea() > op2.getArea())
            return true;
        else
            return false;
    }
};

// 멤버함수 getArea() 정의
template <typename T>
T Rectangle<T>::getArea() {
    return width * height;
}

template <typename T>
T bigger(T a, T b) {
    if (a > b) return a;
    else return b;
}

/*
 * [방법2] 제네릭 함수 bigger() 정의
 */
template <typename T>
Rectangle<T> bigger(Rectangle<T> a, Rectangle<T> b) {
    return (a.getArea() > b.getArea()) ? a : b;
}

int main() {
    int a = 20, b = 50, c;

    c = bigger(a, b);
    cout << "20과 50 중 더 큰 값은 " << c << endl;
    
    // double형 Rectangle 객체 rect1, rect2, rect3 선언
    // 단, rect1의 width와 height은 각각 10.2와 23.5, rect2는 20.4와 13.7로 초기화
    Rectangle<double> rect1(10.2, 23.5), rect2(20.4, 13.7), rect3;
    
    rect3 = bigger(rect1, rect2);
    cout << "rect1과 rect2 중 더 큰 것의 면적은 " << rect3.getArea() << endl;

    return 0;
}
```

**실행 결과:** 실행하면 다음과 같이 출력된다. 연습 문제 1과 같이 마지막 줄 `Program ended with exit code: 0`은 개발 환경(Xcode)이 표시하는 메시지이다.

```text
20과 50 중 더 큰 값은 50
rect1과 rect2 중 더 큰 것의 면적은 279.48
```

---

<br>

## 연습 문제 3: 템플릿 인자의 추론

**문제:** `template <class T> T bigger(T a, T b)`에 대한 네 호출 `bigger(3, 7)`, `bigger(3, 7.5)`, `bigger<double>(3, 7.5)`, `bigger('a', 'b')`가 컴파일되는지 판단하고, 컴파일되는 호출의 `T`와 출력, 컴파일되지 않는 호출의 이유를 쓴다. 프로그램은 [연습 문제 3](Exercises.ko.md#연습-문제-3-템플릿-인자의-추론)에 있다.

### 풀이

컴파일러는 템플릿 함수를 호출하는 인자의 타입을 보고 `T`를 정한다. 이 과정을 템플릿 인자 추론이라고 하며, 두 매개변수가 모두 `T`이면 두 인자에서 추론한 타입이 정확히 같아야 한다. 추론할 때는 `int`를 `double`로 바꾸는 것 같은 형 변환을 고려하지 않는다.

| 호출 | 추론 결과 | 결과 |
|:----|:----|:----|
| (1) `bigger(3, 7)` | 두 인자가 모두 `int`이므로 `T = int` | 7 출력 |
| (2) `bigger(3, 7.5)` | 첫 인자에서 `int`, 둘째 인자에서 `double`이 나와 서로 다름 | 컴파일 오류 |
| (3) `bigger<double>(3, 7.5)` | `T`를 `double`로 명시했으므로 추론하지 않음. 3은 `double`로 변환됨 | 7.5 출력 |
| (4) `bigger('a', 'b')` | 두 인자가 모두 `char`이므로 `T = char` | b 출력 |

(2)가 있으면 Apple clang은 "no matching function for call to 'bigger'"라는 오류와 함께 "candidate template ignored: deduced conflicting types for parameter 'T' ('int' vs. 'double')"라는 설명을 낸다. 추론에 실패한 템플릿은 후보에서 빠지고, 다른 후보도 없으므로 호출할 함수가 없다. (3)처럼 `<double>`로 템플릿 인자를 명시하면 `bigger<double>(double, double)`이라는 보통 함수를 호출하는 것과 같으므로, 인자 3은 일반적인 형 변환으로 3.0이 된다. (4)의 `T = char`에서 `'a' > 'b'`는 문자 코드 97과 98의 비교이므로 `'b'`가 리턴되고, `char`는 문자로 출력된다.

(2)를 지우고 실행한 결과는 다음과 같다.

```text
7
7.5
b
```

---

<br>

## 연습 문제 4: 템플릿 함수와 일반 함수의 선택

**문제:** 함수 템플릿 `print(T)`와 일반 함수 `print(int)`가 함께 있을 때 `print(5)`, `print(5.0)`, `print('c')`, `print<>(5)`의 출력과 선택되는 함수를 설명한다. 프로그램은 [연습 문제 4](Exercises.ko.md#연습-문제-4-템플릿-함수와-일반-함수의-선택)에 있다.

### 풀이

```text
int 5
template 5
template c
template 5
```

컴파일러는 템플릿에서 추론한 함수와 일반 함수를 모두 후보로 놓고 6장의 함수 중복 해석 규칙으로 고른다. 변환 순위가 같으면 일반 함수가 템플릿 함수보다 우선한다([10장 1.5절](Concepts_Lecture.ko.md#15-템플릿-함수와-일반-함수의-중복)).

- **(1) `print(5)`:** 템플릿에서 `T = int`인 `print<int>(int)`가, 일반 함수로 `print(int)`가 후보가 된다. 둘 다 정확한 일치이므로 일반 함수가 우선하여 `int 5`가 출력된다.
- **(2) `print(5.0)`:** 템플릿은 `T = double`로 정확한 일치이고, 일반 함수 `print(int)`는 `double`을 `int`로 바꾸는 표준 형 변환이 필요하다. 템플릿의 순위가 더 좋으므로 `template 5`가 출력된다. `double` 값 5.0은 `cout`의 기본 형식으로 `5`로 출력된다.
- **(3) `print('c')`:** 템플릿은 `T = char`로 정확한 일치이고, 일반 함수는 `char`를 `int`로 바꾸는 형 승격이 필요하다. 정확한 일치가 형 승격보다 순위가 좋으므로 템플릿이 선택되어 `template c`가 출력된다. 일반 함수가 우선하는 것은 순위가 같을 때뿐이다.
- **(4) `print<>(5)`:** 빈 꺾쇠 `<>`는 템플릿 함수만 후보로 삼으라는 뜻이다. 일반 함수는 후보에서 빠지고 `T = int`로 추론된 템플릿이 호출되어 `template 5`가 출력된다.

---

<br>

## 연습 문제 5: 제네릭 원형 큐

**문제:** 원소를 최대 4개까지 담는 원형 큐 `MyQueue<T>`를 클래스 템플릿으로 작성하여 `push()`, `pop()`, `size()`를 제공한다. 요구사항, `main()`, 실행 예시는 [연습 문제 5](Exercises.ko.md#연습-문제-5-제네릭-원형-큐)에 있다.

### 풀이

**원형 배열의 관리:** `front`는 다음에 꺼낼 원소의 위치, `rear`는 다음에 넣을 위치, `count`는 현재 원소 수이다. 넣을 때는 `data[rear]`에 저장하고 `rear`를 `(rear + 1) % 4`로 옮기며, 꺼낼 때는 `data[front]`를 읽고 `front`를 `(front + 1) % 4`로 옮긴다. 나머지 연산 `% 4` 덕분에 위치가 3 다음에 0으로 돌아가므로, 앞에서 꺼내 비운 칸을 다시 쓸 수 있다. 큐가 가득 찼는지와 비었는지는 `front`와 `rear`만으로는 구별할 수 없으므로(둘 다 `front == rear`이다) `count`로 판단한다. 8장 실습의 배열 기반 원형 큐와 같은 원리이며, 원소 타입만 `T`로 일반화했다.

**템플릿으로 만든 효과:** `MyQueue<int>`와 `MyQueue<string>`은 같은 코드에서 구체화된 서로 다른 클래스이다. `pop()`은 꺼낸 원소를 리턴값이 아니라 참조 매개변수 `T &x`로 돌려주므로, 리턴값 `bool`로 성공 여부를 함께 알릴 수 있다(5장 3.2절의 참조 매개변수로 결과 돌려주기).

```cpp
#include <iostream>
#include <string>
using namespace std;

template <class T>
class MyQueue {
    T data[4];                          // 원소를 담는 원형 배열
    int front = 0;                      // 다음에 꺼낼 원소의 위치
    int rear = 0;                       // 다음에 넣을 위치
    int count = 0;                      // 현재 원소 수
public:
    bool push(T x) {
        if (count == 4) return false;   // 가득 참
        data[rear] = x;
        rear = (rear + 1) % 4;          // 끝에 닿으면 0으로 돌아간다
        count++;
        return true;
    }
    bool pop(T &x) {                    // 꺼낸 원소를 참조 매개변수로 돌려준다
        if (count == 0) return false;   // 비어 있음
        x = data[front];
        front = (front + 1) % 4;
        count--;
        return true;
    }
    int size() const { return count; }
};

int main() {
    MyQueue<int> qi;
    for (int i = 1; i <= 5; i++)
        cout << "push " << i << ": " << (qi.push(i * 10) ? "ok" : "full") << endl;
    int v;
    qi.pop(v); cout << "pop " << v << endl;
    qi.push(60);
    while (qi.pop(v)) cout << v << ' ';
    cout << endl;

    MyQueue<string> qs;
    qs.push("C++");
    qs.push("STL");
    string s;
    while (qs.pop(s)) cout << s << ' ';
    cout << endl;
    return 0;
}
```

**실행 결과:**

```text
push 1: ok
push 2: ok
push 3: ok
push 4: ok
push 5: full
pop 10
20 30 40 60 
C++ STL 
```

| 시점 | `front` | `rear` | `count` | `data` |
|:----|:----|:----|:----|:----|
| 10, 20, 30, 40을 넣은 뒤 | 0 | 0 | 4 | 10, 20, 30, 40 |
| 50을 넣으려 할 때 | 0 | 0 | 4 | 변화 없음 (`full`) |
| 10을 꺼낸 뒤 | 1 | 0 | 3 | (10), 20, 30, 40 |
| 60을 넣은 뒤 | 1 | 1 | 4 | 60, 20, 30, 40 |

*표 1. `MyQueue<int>`의 상태 변화*

60은 앞에서 10을 꺼내 비운 `data[0]`에 들어간다. 그다음 반복문은 `front` 1부터 20, 30, 40을 꺼내고, `front`가 0으로 돌아와 60을 꺼낸다.

---

<br>

## 연습 문제 6: vector의 변경 연산 추적

**문제:** `push_back()`, `insert()`, `erase()`, `pop_back()`, `resize()`를 차례로 적용한 `vector<int>`의 크기와 원소를 (1)\~(5)에서 쓴다. 프로그램은 [연습 문제 6](Exercises.ko.md#연습-문제-6-vector의-변경-연산-추적)에 있다.

### 풀이

```text
size=3 : 10 20 30
size=4 : 10 15 20 30
size=3 : 15 20 30
size=2 : 15 20
size=4 : 15 20 0 0
```

- **(2)** `v.insert(v.begin() + 1, 15)`는 인덱스 1의 위치, 곧 10과 20 사이에 15를 넣는다. 그 뒤의 원소 20, 30은 한 칸씩 뒤로 밀린다.
- **(3)** `v.erase(v.begin())`은 첫 원소 10을 지우고 나머지를 한 칸씩 앞으로 당긴다.
- **(4)** `pop_back()`은 마지막 원소 30을 지운다.
- **(5)** `resize(4)`는 크기를 4로 늘리며, 새로 생긴 두 원소는 `int`의 값 초기화에 따라 0이 된다.

`insert()`와 `erase()`는 위치를 인덱스가 아니라 반복자로 받는다. 또 앞쪽에 넣거나 지우면 그 뒤의 원소를 모두 옮겨야 하므로, 원소가 n개일 때 앞쪽에서의 삽입과 삭제에는 원소 수에 비례하는 시간이 든다. 출력에 용량 `capacity()`를 넣지 않은 것은 용량이 늘어나는 방식이 구현마다 다르기 때문이다([10장 4.4절](Concepts_Lecture.ko.md#44-크기와-용량)).

---

<br>

## 연습 문제 7: 원소를 지우는 반복문의 오류

**문제:** `vector<int> v = {1, 2, 3, 4, 5, 6};`에서 짝수를 지우려고 `for (auto it = v.begin(); it != v.end(); it++)` 안에서 `v.erase(it);`를 호출하는 코드의 오류를 설명하고 고친다. 프로그램은 [연습 문제 7](Exercises.ko.md#연습-문제-7-원소를-지우는-반복문의-오류)에 있다.

### 풀이

**오류의 원인:** `v.erase(it)`를 실행하면 `it`가 가리키던 원소와 그 뒤의 모든 원소를 가리키던 반복자가 무효가 된다([10장 11.1절](Concepts_Lecture.ko.md#111-inserterase-후-반복자-무효화)). 그런데 반복문은 무효가 된 `it`에 `it++`를 실행하고 다시 역참조하므로 정의되지 않은 동작이다. 원소를 지운 뒤 뒤의 원소가 앞으로 당겨지므로, 설령 `it`가 같은 자리를 가리킨다고 해도 `it++`가 당겨진 원소 하나를 건너뛴다. 마지막 원소 6을 지우면 `it`가 `end()` 자리를 가리킨 상태에서 다시 `it++`가 실행되어 `end()`를 지나치므로, `it != v.end()` 검사로 반복이 끝나지 않고 배열 밖의 메모리를 읽는다.

macOS의 Apple clang으로 컴파일한 이 프로그램은 아무것도 출력하지 못하고 비정상 종료될 수 있으며, AddressSanitizer(`-fsanitize=address`)를 켜고 실행하면 `container-overflow` 오류가 보고된다. 정의되지 않은 동작이므로 결과는 실행 환경마다 다를 수 있다.

**고친 코드:** `erase()`는 지운 원소의 바로 다음 원소를 가리키는 유효한 반복자를 리턴한다. 원소를 지웠으면 그 리턴값을 `it`에 받고, 지우지 않았을 때만 `++it`로 전진한다.

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5, 6};
    for (auto it = v.begin(); it != v.end(); ) {
        if (*it % 2 == 0)
            it = v.erase(it);         // 지운 원소의 다음 위치를 받는다
        else
            ++it;                     // 지우지 않았을 때만 전진
    }
    for (int x : v) cout << x << ' ';
    cout << endl;
    return 0;
}
```

```text
1 3 5 
```

`for`의 세 번째 칸을 비워 두는 것이 핵심이다. 반복자를 옮기는 일을 `if`와 `else`가 나누어 맡으므로, 지운 경우에는 `erase()`가 알려 준 다음 원소에서 바로 검사를 이어 가고, 지우지 않은 경우에만 한 칸 전진한다.

---

<br>

## 연습 문제 8: 단어 빈도의 정렬 출력

**문제:** 단어 배열의 빈도를 `map<string, int>`로 센 뒤, 빈도 내림차순, 빈도가 같으면 사전 순서로 `sort()`와 람다 표현식을 사용하여 출력한다. 단어 배열과 실행 예시는 [연습 문제 8](Exercises.ko.md#연습-문제-8-단어-빈도의-정렬-출력)에 있다.

### 풀이

**빈도 세기:** `freq[w]++`는 `w`가 `map`에 없으면 값이 0인 항목을 먼저 만들고 1을 더하며, 있으면 그 값에 1을 더한다([10장 11.2절](Concepts_Lecture.ko.md#112-map의-는-기본-항목을-생성한다)). 이 성질 덕분에 처음 나온 단어를 따로 처리하지 않아도 된다.

**정렬:** `map`은 키(단어)의 순서로 항목을 유지하며 값(빈도)으로는 정렬할 수 없다. 그래서 `map`의 모든 항목을 `vector<pair<string, int>>`로 복사한 뒤 `sort()`로 정렬한다. 비교 람다는 `a`가 `b`보다 앞에 와야 하면 `true`를 리턴한다. 빈도가 다르면 빈도가 큰 쪽이 앞이고(`a.second > b.second`), 같으면 단어가 사전에서 앞서는 쪽이 앞이다(`a.first < b.first`).

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    string words[] = {"cat", "dog", "cat", "bird", "dog", "cat", "ant"};
    map<string, int> freq;
    for (const string &w : words)
        freq[w]++;                                    // 없는 단어는 0으로 만든 뒤 1 증가

    vector<pair<string, int>> items(freq.begin(), freq.end());
    sort(items.begin(), items.end(),
         [](const pair<string, int> &a, const pair<string, int> &b) {
             if (a.second != b.second) return a.second > b.second;   // 빈도 내림차순
             return a.first < b.first;                               // 같으면 사전순
         });
    for (const auto &p : items)
        cout << p.first << ' ' << p.second << endl;
    return 0;
}
```

```text
cat 3
dog 2
ant 1
bird 1
```

`map`에 들어간 순서는 ant(1), bird(1), cat(3), dog(2)이다. 정렬 뒤에는 빈도 3인 cat, 2인 dog가 앞에 오고, 빈도가 1로 같은 ant와 bird는 사전 순서로 놓인다.

---

<br>

## 연습 문제 9: 람다 표현식의 캡처

**문제:** 값으로 캡처한 람다 `byValue`, 참조로 캡처한 람다 `byRef`, 캡처한 변수를 바꾸는 람다 `addTo`를 실행한 결과를 쓰고 이유를 설명한다. 프로그램은 [연습 문제 9](Exercises.ko.md#연습-문제-9-람다-표현식의-캡처)에 있다.

### 풀이

```text
10 50
8 10 80
```

- `[x]`는 람다를 **만드는 시점**의 `x` 값 1을 람다 안에 복사해 둔다. 그 뒤 `x`를 5나 8로 바꾸어도 `byValue`가 가진 복사본은 1이므로 `byValue()`는 언제나 10을 리턴한다.
- `[&x]`는 `x` 자체를 참조하므로 `byRef()`는 **호출하는 시점**의 `x`를 읽는다. 첫 줄에서는 `x`가 5이므로 50, 둘째 줄에서는 8이므로 80을 리턴한다.
- `addTo`도 `x`를 참조로 캡처하므로 `addTo(3)`은 `main()`의 `x`를 5에서 8로 바꾼다. 값으로 캡처한 변수는 람다 안의 복사본이어서 기본적으로 바꿀 수 없고, 바꿀 수 있게 하더라도 바깥 변수에는 영향이 없으므로, 람다가 바깥 변수를 바꾸려면 참조로 캡처해야 한다([10장 8.3절](Concepts_Lecture.ko.md#83-람다-표현식-c11)).
