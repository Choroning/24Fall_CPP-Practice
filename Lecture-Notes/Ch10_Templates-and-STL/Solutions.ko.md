# 10장 연습 문제 풀이 — 템플릿

[레포지토리 안내](../../README.ko.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **최종 수정일:** 2026-10-27

**문항 출처:** 임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04)

---

<br>

## 목차

- [연습 문제 1: 제네릭 함수](#연습-문제-1-제네릭-함수)
- [연습 문제 2: 제네릭 클래스와 구체화 디버깅](#연습-문제-2-제네릭-클래스와-구체화-디버깅)

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
