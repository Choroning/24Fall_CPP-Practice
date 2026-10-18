# Chapter 4. 객체 활용

**문항 출처:** *임영희 교수, 컴퓨터언어Ⅱ (DCCS102-04) 교수자* <br>
**Source:** *Prof. Younghee&nbsp;Im, Instructor of COMPUTER LANGUAGE Ⅱ (DCCS102-04)*

---

## Q1. 동적 배열을 가진 클래스

예시 코드와 실행 예시를 참고하여 `main()` 함수가 정상적으로 실행되도록 `Sample` 클래스를 완성하시오.


<br>**프로그램 요구사항**

- `main()` 함수와 `Sample` 클래스에 `try-catch`문을 이용하여 아래 상황에도 오류가 발생하지 않도록 예외 처리하시오.
   - 동적 할당에 실패하는 경우
   - 사용자로부터 입력받은 정수 배열 크기가 0보다 작거나 같은 경우


<br>**예시 코드**

```cpp
#include <iostream>
using namespace std;

class Sample {
   int *p = nullptr;
   int size;
public:
   Sample(int n) {     // 생성자
      size = n;
      p = new int [size];
   }
   void read();        // 동적 할당한 정수 배열 p에 사용자로부터 정수 입력
   void write();       // 정수 배열의 내용 출력
   int big();          // 정수 배열에서 가장 큰 수를 반환
   ~Sample();          // 소멸자
};

int main() {
   int input;
    
   cout << "생성할 정수 배열 사이즈를 입력하세요: ";
   cin >> input;
   Sample s(input);     // 크기 input의 정수 배열을 가진 Sample 객체 생성
   s.read();            // 사용자로부터 정수 배열을 입력받아 저장
   s.write();           // 정수 배열의 내용 출력
   cout << "가장 큰 수는 " << s.big() << endl;
    
   return 0;
}
```


<br>**실행 예시**

```text
생성할 정수 배열 사이즈를 입력하세요: 10
100 4 -2 9 55 300 44 38 99 -500
가장 큰 수는 300

Program ended with exit code: 0
```

```text
생성할 정수 배열 사이즈를 입력하세요: 0
[오류] 사이즈가 너무 작습니다!

Program ended with exit code: 0
```


---

## Q2. 객체의 동적 생성 및 소멸

예시 코드를 참고하여 실행 예시와 동일하게 실행되도록 `Circle`, `CircleManager` 클래스를 완성하고 `main()` 함수를 작성하시오.


<br>**프로그램 요구사항**

- 예시 코드에 주어진 `Circle`, `CircleManager` 클래스 선언부의 내용은 변경할 수 없다.
- `main()`함수와 `Circle`, `CircleManager` 클래스 선언부에 `try-catch`문을 이용하여 아래 상황에도 오류가 발생하지 않도록 예외 처리하시오.
   - 동적 할당에 실패하는 경우
   - 사용자로부터 입력받은 원의 개수가 0보다 작거나 같은 경우


<br>**예시 코드**

```cpp
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

class CircleManager {
   Circle *p;     // Circle 배열에 대한 포인터
   int size;      // 배열의 크기
public:
   CircleManager(int size);   // 사용자로부터 입력받은 size 값 크기의 배열을 동적 생성
   ~CircleManager();
   void searchByName(); // 사용자로부터 원의 이름을 입력받아 면적 출력
   void searchByArea(); // 사용자로부터 면적을 입력받아 면적보다 큰 원의 이름 출력
};
```


<br>**실행 예시**

```text
원의 개수 >> 4
원 1의 이름과 반지름 >> 빈대떡 10
원 2의 이름과 반지름 >> 도넛 2
원 3의 이름과 반지름 >> 초코파이 1
원 4의 이름과 반지름 >> 피자 15
검색하고자 하는 원의 이름 >> 도넛
도넛의 면적은 12.56
최소 면적을 정수로 입력하세요 >> 10
10보다 큰 원을 검색합니다.
빈대떡의 면적은 314, 도넛의 면적은 12.56, 피자의 면적은 706.5,

Program ended with exit code: 0
```

```text
원의 개수 >> -1
[오류] 사이즈가 너무 작습니다!

Program ended with exit code: 0
```



---
