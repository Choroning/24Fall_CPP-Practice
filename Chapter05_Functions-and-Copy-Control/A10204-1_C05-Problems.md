# Chapter 5. 참조와 복사 생성자 연습

**문항 출처:** *임영희 교수, 컴퓨터언어Ⅱ 교수자 (DCCS102-04)* <br>
**Source:** *Prof. Younghee&nbsp;Im, Instructor of COMPUTER LANGUAGE Ⅱ (DCCS102-04)*

---

## Q1. 다양한 함수 호출법

예시 코드와 실행 예시를 참고하여 `main()`함수가 정상적으로 실행되도록 `mySwap()`의 매개변수를 각각 참조, 포인터를 활용한 두 버전으로 작성하고, `increaseBy()`함수를 작성하시오.


<br>**프로그램 요구사항**

- `main()`함수의 [ 빈칸 ]을 채우되, 다른 내용은 변경할 수 없습니다.
- 필요 시 `Circle` 객체의 생성자를 추가로 정의할 수 있습니다.


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

int main() {
   Circle x(10), y(5);  // 각각 반지름이 10, 5인 원 객체 생성

   [ 빈칸 ]              // 참조 버전의 mySwap() 호출
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   cout << endl;

   [ 빈칸 ]              // 포인터 버전의 mySwap() 호출
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
   cout << endl;

   increaseBy(x, y);    // x 객체를 y 객체만큼 증가
   cout << "x 객체: ";
   x.show();
   cout << "y 객체: ";
   y.show();
}
```


<br>**실행 예시**

```text
x 객체: 반지름이 5인 원
y 객체: 반지름이 10인 원

x 객체: 반지름이 10인 원
y 객체: 반지름이 5인 원

x 객체: 반지름이 15인 원
y 객체: 반지름이 5인 원

Program ended with exit code: 0
```



---

## Q2. 참조에 의한 호출과 참조 반환

예시 코드와 실행 예시를 참고하여 `main()`함수가 정상적으로 실행되도록 `find()`함수를 작성하시오. `find()`함수의 원형은 다음과 같다.

```cpp
char& find(char a[], char c, bool& success);
```

문자열 `a`에서 문자 `c`를 찾아, 문자 `c`가 있는 공간에 대한 참조를 반환하고 `success` 참조 매개변수에 `true`를 설정한다. 문자 `c`를 찾을 수 없는 경우 `success` 참조 매개변수에 `false`를 설정한다.



<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

int main() {
   char s[] = "Mike";
   bool b;

   char& loc1 = find(s, 'k', b);
   if (b == false) {
      cout << "k를 발견할 수 없음" << endl;
      return 0;
   }
   loc1 = 'm';          // 'k' 위치에 'm' 기록
   cout << s << endl;   // "Mime" 출력

   char& loc2 = find(s, 'k', b);
   if (b == false) {
      cout << "k를 발견할 수 없음" << endl;
      return 0;
   }
   loc2 = 't';          // 'k' 위치에 't' 기록
   cout << s << endl;
   
   return 0;
}
```


<br>**실행 예시**

```text
Mime
k를 발견할 수 없음

Program ended with exit code: 0
```



---
