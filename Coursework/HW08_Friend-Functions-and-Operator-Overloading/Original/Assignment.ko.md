# HW8 과제 안내문 — 프렌드 함수와 연산자 중복

[레포지토리 안내](../../../README.ko.md) | [English](Assignment.md) | [한국어](Assignment.ko.md)

> **최종 수정일:** 2026-10-27

컴퓨터언어실습Ⅱ 3분반의 8차 과제이다. 13주차 강의자료의 실습 문제 세 개로 이루어진다.

- **제출 기한:** 2024-12-02 (월) 13:00, 수업 시작 전까지이다. 기한이 지나면 과제가 자동으로 비공개된다.
- **제출 방법:** 과목이 배포한 과제 제출 양식(.hwp)에 작성하여 블랙보드의 과제란에 제출한다.
- **작성 내용:** 코드 문제는 코드와 함께 반드시 코드 분석 내용을 작성해야 한다.

---

<br>

## 문제 1. 13주차 강의자료 [실습 1]

다음 코드에서 `b = 1 + a;` 연산이 가능하도록 연산자를 구현하라. `b = 1 + a;`는 `b`의 반지름을 `a`의 반지름에 1을 더한 값으로 바꾼다.

```cpp
class Circle {
    int radius;
public:
    Circle(int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
};

int main() {
    Circle a(5), b(4);
    b = 1 + a; // b의 반지름을 a의 반지름에 1을 더한 것으로 변경
    a.show();
    b.show();
}
```

**실행 예시**

```text
radius = 5 인 원
radius = 6 인 원
```

---

<br>

## 문제 2. 13주차 강의자료 [실습 2]

문제 1과 같은 `Circle` 클래스에 대해, 다음 `main()` 함수의 `++` 연산자 기능을 프렌드 함수로 작성하라.

```cpp
class Circle {
    int radius;
public:
    Circle(int radius = 0) {
        this->radius = radius;
    }
    void show() {
        cout << "radius = " << radius << " 인 원" << endl;
    }
};
int main() {
    Circle a(5), b(4);
    ++a; // 반지름을 1 증가시킨다.
    b = a++; // 반지름을 1 증가시킨다.
    a.show();
    b.show();
}
```

**실행 예시**

```text
radius = 7 인 원
radius = 6 인 원
```

---

<br>

## 문제 3. 13주차 강의자료 [실습 3]

2차원 행렬을 추상화한 `Matrix` 클래스는 2×2 행렬의 네 원소를 배열 `ar`에 저장하며 다음과 같이 정의된다.

```cpp
class Matrix {
    int ar[4];
public:
    Matrix() { for (int i = 0; i < 4; i++) ar[i] = 0; }
    Matrix(int a1, int a2, int b1, int b2) {
        ar[0] = a1;
        ar[1] = a2;
        ar[2] = b1;
        ar[3] = b2;
    }
    void show() {
        cout << "Matrix = { ";
        for (int i = 0; i < 4; i++) cout << ar[i] << ' ';
        cout << "}" << endl;
    }
};
```

이 클래스를 활용하는 다음 코드가 있다. `a >> x`는 `a`의 각 원소를 배열 `x`에 복사하고, `b << y`는 배열 `y`의 원소 값을 `b`의 각 원소에 설정한다.

```cpp
int main() {
    Matrix a(4, 3, 2, 1), b;
    int x[4], y[4] = { 1, 2, 3, 4 }; // 2차원 행렬의 4 개의 원소 값
    a >> x; // a의 각 원소를 배열 x에 복사. x[]는 {4, 3, 2, 1}
    b << y; // 배열 y의 원소 값을 b의 각 원소에 설정

    for (int i = 0; i<4; i++)
        cout << x[i] << ' '; // x[] 출력
    cout << endl;
    b.show();
}
```

1. `<<`, `>>` 연산자 함수를 `Matrix`의 멤버 함수로 구현하라.
2. `<<`, `>>` 연산자 함수를 `Matrix`의 프렌드 함수로 구현하라.

**실행 예시**

```text
4 3 2 1 
Matrix = { 1 2 3 4 }
```
