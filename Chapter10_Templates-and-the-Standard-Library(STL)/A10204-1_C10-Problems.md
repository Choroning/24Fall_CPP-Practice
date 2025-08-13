# Chapter 10. 템플릿

**문항 출처:** *임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04) 교수자* <br>
**Source:** *Prof. Younghee&nbsp;Im, Instructor of COMPUTER LANGUAGE Ⅱ (DCCS102-04)*

---

## Q1. 제네릭 함수

예시 코드와 실행 예시를 참고하여 `main()`함수를 완성하고, 정상적으로 실행되도록 `reverseArray()`, `display()`함수를 작성하시오.


<br>**프로그램 요구사항**

1. 주어진 예시 코드는 변경할 수 없다.
2. 아래 함수를 템플릿을 활용한 제네릭 함수로 작성하시오.
   - `reverseArray()`함수는 배열에 저장된 원소를 역순으로 재정렬하는 기능을 수행한다.
   - `display()`함수는 배열의 내용을 출력하는 기능을 수행한다.


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

// reverseArray() 정의
[ 빈칸 ]

// display() 정의
[ 빈칸 ]

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
   [ 빈칸 ]
   cout << "str = " << str << endl;

   return 0;
}
```


<br>**실행 예시**

```text
x = 1000  100  10  1
y = 6.6  5.5  4.4  3.3  2.2  1.1
str = lufrewop si ++C

Program ended with exit code: 0
```



---

## Q2. 제네릭 클래스와 구체화 디버깅

주어진 예시 코드는 다음과 같이 실행되는 것을 목표로 작성되었다.

```text
20과 50 중 더 큰 값은 50
rect1과 rect2 중 더 큰 것의 면적은 279.48

Program ended with exit code: 0
```

그러나, 알 수 없는 원인으로 컴파일 오류가 발생하여 정상적으로 실행되지 않는다. 프로그램이 정상적으로 실행되도록 완성하고, 오류 원인을 찾아 디버깅하시오.

아래의 예시 코드에서 발생하는 오류는 대표적으로 두 가지 방법을 통해 해결할 수 있다.


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

template <typename T>
class Rectangle {
   T width;
   T height;
public:
   Rectangle(T w = 1, T h = 1) { width = w; height = h; }
   T getArea();
};

// 멤버함수 getArea() 정의
[ 빈칸 ]

template <typename T>
T bigger(T a, T b) {
   if (a > b) return a;
   else return b;
}

int main() {
   int a = 20, b = 50, c;

   c = bigger(a, b);
   cout << "20과 50 중 더 큰 값은 " << c << endl;

   // double형 Rectangle 객체 rect1, rect2, rect3 선언
   // 단, rect1의 width와 height은 각각 10.2와 23.5, rect2는 20.4와 13.7로 초기화
   [ 빈칸 ]

   rect3 = bigger(rect1, rect2);
   cout << "rect1과 rect2 중 더 큰 것의 면적은 " << rect3.getArea() << endl;

   return 0;
}
```



---
