# Chapter 4 Solutions — Using Objects

[Repository Guide](../../README.md) | [English](Solutions.md) | [한국어](Solutions.ko.md)

> **Last Updated:** 2026-10-19
>
> 명품 C++ Programming, 황기태, Ch 4

**Problem Source:** Prof. Younghee Im, Computer Language Ⅱ (DCCS102-04)

---

<br>

## Table of Contents

- [Exercise 1: Class with a Dynamic Array](#exercise-1-class-with-a-dynamic-array)
- [Exercise 2: Dynamic Creation and Destruction of Objects](#exercise-2-dynamic-creation-and-destruction-of-objects)

---

<br>

## Exercise 1: Class with a Dynamic Array

**Textbook Problem:** Chapter 4 programming exercise 4

**Problem:** Complete `read()`, `write()`, `big()`, and the destructor of the `Sample` class, whose constructor dynamically allocates an integer array of the size entered by the user, and handle allocation failure and an array size of 0 or less with `try-catch`. The full problem and sample runs are in [Exercise 1](Exercises.md#exercise-1-class-with-a-dynamic-array).

### Solution

**Approach:** Separate where an error is detected from where it is handled. The constructor detects a size error and throws an exception, and `new` reports an allocation failure with a `bad_alloc` exception. `main()` catches both, prints an error message, and exits normally.

- **Constructor:** It checks the size first and throws the `int` value 1 if the size is 0 or less; only otherwise does it allocate the array with `new int [size]`. The size must be checked before `new` for two reasons. Calling `new` with a negative size throws an exception of the `bad_alloc` family, so a size error would be reported as an allocation failure, and with size 0 the allocation succeeds but yields an array with no elements, so `p[0]` in `big()` reads outside the array.
- **`read()` and `write()`:** `read()` reads `size` integers in turn with `cin >>`, and `write()` prints each number followed by one space and then ends the line.
- **`big()`:** It takes the first element `p[0]` as the initial maximum and compares it with the remaining elements using the conditional operator. Because it starts from the first element, it finds the correct maximum even if every number is negative.
- **Destructor:** The array was allocated with `new []`, so it is freed with `delete [] p;`, and `p` is set to `nullptr`.
- **`main()`:** Everything from creating the object to printing goes in the `try` block; `catch (bad_alloc)` handles allocation failure and `catch (int e)` handles the size error. When a constructor throws, the object is considered never constructed and its destructor is not called, so an unallocated array is never passed to `delete []`.

**Code:** [Lab_Q1.cpp](../../Coursework/Lab04_Pointers-Arrays-and-Dynamic-Memory/Lab_Q1.cpp)

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

**Output:** Entering the same values as in the two sample runs gives the same results. When run in a terminal, however, the line of numbers typed by the user is followed by the same numbers printed by `write()`, whereas the sample run shows this line only once. Because `write()` prints a space after each number, the printed line ends with one trailing space. The last line of the sample runs, `Program ended with exit code: 0`, is not program output but the exit message added by the Xcode console.

```text
생성할 정수 배열 사이즈를 입력하세요: 10
100 4 -2 9 55 300 44 38 99 -500
100 4 -2 9 55 300 44 38 99 -500 
가장 큰 수는 300
```

> **Note:** `bad_alloc` is declared in the `<new>` header, and this code compiles on implementations where `<iostream>` includes `<new>` internally. For portability, add `#include <new>`. Also, catching an exception by value as in `catch (bad_alloc)` copies the exception object, so the convention is to catch standard exceptions by reference, as in `catch (const bad_alloc &e)`, and check the message with `e.what()` when needed.

---

<br>

## Exercise 2: Dynamic Creation and Destruction of Objects

**Textbook Problem:** Chapter 4 programming exercise 12

**Problem:** Complete the `Circle` and `CircleManager` classes without changing the given declarations. The constructor of `CircleManager` dynamically creates a `Circle` array with as many elements as the number of circles and reads the name and radius of each circle; the class provides `searchByName()`, which looks up the area by name, and `searchByArea()`, which prints the circles larger than an entered minimum area. Handle a circle count of 0 or less and dynamic allocation failure with `try-catch`. The full problem and sample runs are in [Exercise 2](Exercises.md#exercise-2-dynamic-creation-and-destruction-of-objects).

### Solution

**Approach:** `CircleManager` takes charge of creating, using, and freeing the array of `Circle` objects. When a dynamic object array is created with `new Circle [size]` and no initializer list, each element is created with the default constructor ([Chapter 4, Section 3.3](Concepts_Lecture.md#33-dynamic-object-arrays)), so the array is created first and the entered values are then set on each element with `setCircle()`.

- **`Circle`:** It declares no constructor, so the compiler provides a default constructor, which is what lets `new Circle [size]` compile. `setCircle()` has parameters with the same names as the members, so it uses `this->radius` and `this->name` to refer to the members ([Chapter 4, Section 1.2](Concepts_Lecture.md#12-this-pointer)). `getArea()` computes $\pi r^2$ with the macro `PI` (3.14).
- **Constructor:** It checks the number of circles and throws the `int` value 1 if it is 0 or less; otherwise it allocates the array and reads each circle's name and radius with `cin >>`. Because names are read with `cin >>`, each name must be a single word without spaces.
- **Destructor:** `delete [] p;` frees the array, running the destructor of each element in the reverse order of creation.
- **`searchByName()`:** It finds every circle whose name matches the entered name and prints its area. If no name matches, it prints nothing.
- **`searchByArea()`:** It prints, on one line separated by `", "`, the name and area of each circle whose area is greater than the minimum area entered as an integer.

**Code:** [Lab_Q2.cpp](../../Coursework/Lab04_Pointers-Arrays-and-Dynamic-Memory/Lab_Q2.cpp)

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

**Output:** Identical to the two sample runs. The areas of the circles with radii 10, 2, 1, and 15 are 314, 12.56, 3.14, and 706.5, so 빈대떡, 도넛, and 피자, whose areas exceed the minimum area 10, are printed. The code also prints `", "` after the last circle, so the line ends with a comma, as in the sample run.

> **Note:** When the number of circles is 0 or less, the constructor throws before initializing `p`. The destructor of an object whose construction did not finish is not called, so `delete [] p` is never executed on the uninitialized `p`.
