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

- `Lecture-Notes/`에는 장별 강의 노트가 있고, 1장\~10장에는 실습 문제나 교재 이론 문제를 옮긴 연습 문제와 풀이가 함께 있습니다.
- `Coursework/`에는 컴퓨터언어실습Ⅱ 과제(`HW01`\~`HW08`)의 풀이와 해설, 3장\~10장의 실습 문제를 푼 C++ 코드(`Lab03`\~`Lab10`)가 있습니다.
- 단원은 교재의 장 번호(`Ch01`\~`Ch10`)를 따르며, 모든 문서는 영어(`.md`)와 한국어(`.ko.md`) 파일이 짝을 이룹니다.
- 모든 코드는 C++11과 C++17로 컴파일하여 확인했습니다.

> **🤖 AI 에이전트 활용**
> [Claude Code](https://claude.ai/download)를 노트 정리와 번역, 레포지토리 정리에 활용했습니다.
> 실습 코드는 직접 작성했습니다.

<br><a name="course-information"></a>
## 📚 강의 정보

- **학기:** 2024학년도 가을학기 (9월 - 12월)
- **소속:** 고려대학교(세종)

|학수번호      |강의명    |이수구분|교수자|개설학과|
|:----------:|:-------|:----:|:------:|:----------------|
|`DCCS102-04`|컴퓨터언어Ⅱ|교양|임영희 교수|컴퓨터소프트웨어학과|
|`DCCS104-03`|컴퓨터언어실습Ⅱ|교양|이종욱 교수|컴퓨터소프트웨어학과|

### 과목 개요

C++의 기초와 객체 지향 프로그래밍(캡슐화, 상속, 다형성)을 배우는 교양 과목이며, 컴퓨터언어실습Ⅱ는 같은 학기에 연계된 실습 과목이다. 교수자, 수업 일정, 평가, 수업 운영 규정, 진도는 [강의 정보 문서](Course-Information.ko.md)에 정리했다.

<br><a name="prerequisites"></a>
## ✅ 사전 요구사항

- 프로그래밍 개념에 대한 기본적인 이해
- C++ 컴파일러 설치(예: GCC, Clang, MSVC)
- 커맨드 라인 툴 사용에 익숙함

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
├── Coursework                  과제와 장별 C++ 실습 코드
├── images                      연습 문제의 그림
├── Course-Information.ko.md    수업 운영 정보
├── Course-Information.md
├── LICENSE                     문서 라이선스 (CC BY-NC-SA 4.0)
├── LICENSE-CODE                코드 라이선스 (PolyForm Noncommercial 1.0.0)
├── README.ko.md
└── README.md
```

단원 목록은 [`Lecture-Notes` 색인](Lecture-Notes/README.ko.md)에, 과제와 실습 코드는 [`Coursework` 색인](Coursework/README.ko.md)에 있습니다.

<br><a name="license"></a>
## 🤝 라이선스

이 레포지토리의 문서(강의 노트, 해설, 요약지)는 [CC BY-NC-SA 4.0](LICENSE), 코드는 [PolyForm Noncommercial 1.0.0](LICENSE-CODE)에 따라 배포됩니다. 강의자료와 교재에서 가져온 그림과 예제 코드, 강의가 배포한 과제 원본과 템플릿 코드는 원저작자에게 권리가 있으며 이 라이선스의 대상이 아닙니다.

---
