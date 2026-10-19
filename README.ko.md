# [2024학년도 가을학기] C++ 프로그래밍 연습

![Last Commit](https://img.shields.io/github/last-commit/Choroning/24Fall_CPP-Practice)
![Languages](https://img.shields.io/github/languages/top/Choroning/24Fall_CPP-Practice)

이 레포지토리는 컴퓨터언어Ⅱ 과목의 이중 언어 강의 노트와 수업 결과물을 정리한 아카이브입니다.

*작성자: 박철원 (고려대학교(세종), 컴퓨터소프트웨어학과) - 2024년 기준 1학년*
<br><br>

## 📑 목차

- [레포지토리 소개](#about-this-repository)
- [강의 정보](#course-information)
- [사전 요구사항](#prerequisites)
- [레포지토리 구조](#repository-structure)
- [라이선스](#license)

---


<br><a name="about-this-repository"></a>
## 📝 레포지토리 소개

이 레포지토리에는 대학 수준의 C++ 프로그래밍 과목을 위해 작성한 강의 노트, 연습 문제, C++ 코드가 담겨 있습니다.

- `Lecture-Notes/`에는 장마다 핵심 개념을 정리한 강의 노트(`Concepts_Lecture`)가 있고, 3장\~10장에는 교수자가 낸 장별 실습 문제를 옮긴 연습 문제(`Exercises`)와 그 풀이(`Solutions`)가 함께 있습니다. 교재인 명품 C++ Programming의 장 번호(1장\~10장)를 따라 단원을 `Ch01`\~`Ch10`으로 나눴습니다.
- 모든 강의 노트, 연습 문제, 풀이는 영어 파일(`.md`)과 한국어 파일(`.ko.md`)이 같은 구성으로 짝을 이룹니다.
- `Coursework/`에는 3장\~10장의 실습 문제를 직접 푼 C++ 코드(`Lab03`\~`Lab10`)가 있으며, 각 단원의 풀이(`Solutions`)가 이 코드를 해설합니다.
- 실습 코드 파일에는 문제 제목, 작성자, 작성일을 적은 **Doxygen 스타일 헤더**가 있습니다.
- 모든 실습 코드는 C++11과 C++17 표준으로 컴파일하여 확인했습니다.

> **🤖 AI 에이전트 활용**
> 강의 노트와 연습 문제의 정리와 번역, 풀이의 해설 작성, 레포지토리 구조 정리에 [Claude Code](https://claude.ai/download)를 활용했습니다. 실습 코드는 직접 작성했습니다.

<br><a name="course-information"></a>
## 📚 강의 정보

- **학기:** 2024학년도 가을학기 (9월 - 12월)
- **소속:** 고려대학교(세종)

|학수번호      |강의명    |이수구분|교수자|개설학과|
|:----------:|:-------|:----:|:------:|:----------------|
|`DCCS102-04`|컴퓨터언어Ⅱ|교양|임영희 교수|컴퓨터소프트웨어학과|
|`DCCS104-03`|컴퓨터언어실습Ⅱ|교양|이종욱 교수|컴퓨터소프트웨어학과|

### 과목 개요

객체 지향 프로그래밍 언어인 C++의 기초와 프로그래밍 방법을 배우는 교양 과목이다. C++ 언어와 C 언어의 차이점에서 출발하여 객체 지향 프로그래밍의 핵심인 클래스와 상속의 개념을 공부하며, 강의는 교재를 중심으로 객체 지향 언어의 세 가지 기본 개념인 캡슐화, 상속, 다형성을 이해하는 것을 목표로 한다. 교재의 1장~10장을 따라 C++의 시작과 프로그래밍의 기본, 클래스와 객체, 객체 포인터와 객체 배열과 동적 생성, 함수와 참조와 복사 생성자, 함수 중복과 static 멤버, 프렌드와 연산자 중복, 상속, 가상 함수와 추상 클래스, 템플릿과 STL을 다룬다. 수업은 이론 강의, 실습, 퀴즈로 이루어지며, 장별 이론 과제(교재 연습문제 풀이)와 실습 과제를 수행한다. 컴퓨터언어실습Ⅱ는 같은 학기에 수강한 C++ 실습 과목이며, 이 레포지토리의 강의 노트와 실습 코드는 컴퓨터언어Ⅱ의 강의자료와 실습 과제를 바탕으로 한다. 교수자, 수업 일정과 형식, 평가, 수업 운영 규정, 주차별 계획 및 진도, 참고 자료는 [강의 정보 문서](Course-Information.ko.md)에 정리했다.

<br><a name="prerequisites"></a>
## ✅ 사전 요구사항

- 프로그래밍 개념에 대한 기본적인 이해가 있어야 합니다. 강의계획서는 컴퓨터언어 1과 컴퓨터언어 1 실습을 선수 필수 과목으로 지정했습니다.
- C++ 컴파일러(예: GCC, Clang, MSVC)가 설치되어 있어야 합니다. 이 레포지토리의 실습 코드를 컴파일하려면 C++11 이상을 지원하는 컴파일러가 필요합니다.
- 커맨드 라인 툴과 IDE(Visual Studio, VS Code 등) 사용에 익숙해야 합니다.

- **💻 개발 환경**

| 도구 | 회사 |  운영체제  | 비고 |
|:-----|:-------:|:----:|:------|
|CLion|JetBrains s.r.o.|macOS|    |
|Xcode|Apple Inc.|macOS|가장 많이 사용한 IDE|

<br><a name="repository-structure"></a>
## 🗂 레포지토리 구조

```plaintext
24Fall_CPP-Practice
├── Lecture-Notes               장별 강의 노트, 연습 문제, 풀이
├── Coursework                  장별 C++ 실습 코드
├── images                      연습 문제의 그림
├── Course-Information.ko.md    수업 운영 정보
├── Course-Information.md
├── LICENSE                     문서 라이선스 (CC BY-NC-SA 4.0)
├── LICENSE-CODE                코드 라이선스 (PolyForm Noncommercial 1.0.0)
├── README.ko.md
└── README.md
```

단원 목록은 [`Lecture-Notes` 색인](Lecture-Notes/README.ko.md)에, 실습 코드는 [`Coursework` 색인](Coursework/README.ko.md)에 있습니다.

<br><a name="license"></a>
## 🤝 라이선스

이 레포지토리의 문서(강의 노트, 해설, 요약지)는 [CC BY-NC-SA 4.0](LICENSE), 코드는 [PolyForm Noncommercial 1.0.0](LICENSE-CODE)에 따라 배포됩니다. 강의자료와 교재에서 가져온 그림과 예제 코드, 강의가 배포한 과제 원본과 템플릿 코드는 원저작자에게 권리가 있으며 이 라이선스의 대상이 아닙니다.

---
