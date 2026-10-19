# [Fall 2024] C++ Programming Practice

![Last Commit](https://img.shields.io/github/last-commit/Choroning/24Fall_CPP-Practice)
![Languages](https://img.shields.io/github/languages/top/Choroning/24Fall_CPP-Practice)

This repository archives bilingual lecture notes and coursework for the Computer Language Ⅱ course.

*Author: Cheolwon Park (Korea University Sejong, CSE) - Year 1 (Freshman) as of 2024*
<br><br>

## 📑 Table of Contents

- [About This Repository](#about-this-repository)
- [Course Information](#course-information)
- [Prerequisites](#prerequisites)
- [Repository Structure](#repository-structure)
- [License](#license)

---


<br><a name="about-this-repository"></a>
## 📝 About This Repository

This repository contains the lecture notes, exercises, and C++ code written for a university-level C++ programming course.

- `Lecture-Notes/` holds, for each chapter, lecture notes that organize the key concepts (`Concepts_Lecture`), and Chapters 3-10 also hold exercises that transcribe the instructor's lab problems for each chapter (`Exercises`) and their solutions (`Solutions`). The units follow the chapter numbers (Chapters 1-10) of the textbook, 명품 C++ Programming, and are numbered `Ch01`-`Ch10`.
- Every lecture note, exercise set, and solution set comes as an English file (`.md`) and a Korean file (`.ko.md`) with the same structure.
- `Coursework/` holds the author's own C++ code that solves the lab problems of Chapters 3-10 (`Lab03`-`Lab10`), and the solutions (`Solutions`) of each unit explain this code.
- The lab code files carry **Doxygen-style headers** that record the problem title, author, and date.
- All lab code was compiled under the C++11 and C++17 standards.

> **🤖 AI Agent Usage**
> [Claude Code](https://claude.ai/download) was used to organize and translate the lecture notes and exercises, to write the explanations in the solutions, and to restructure the repository. The lab code was written by the author.

<br><a name="course-information"></a>
## 📚 Course Information

- **Semester:** Fall 2024 (September - December)
- **Affiliation:** Korea University Sejong

| Course&nbsp;Code| Course            | Type          | Instructor      | Department                              |
|:----------:|:------------------|:-------------:|:---------------:|:----------------------------------------|
|`DCCS102-04`|COMPUTER LANGUAGE Ⅱ|General Studies|Prof. Younghee&nbsp;Im|Department of Computer Science and Software Engineering|
|`DCCS104-03`|COMPUTER LANGUAGE AND TRAINING Ⅱ|General Studies|Prof. Jonguk&nbsp;Lee|Department of Computer Science and Software Engineering|

### Course Overview

A general studies course on the fundamentals of C++, an object-oriented programming language, and on how to program in it. Starting from the differences between C++ and C, it studies the concepts of classes and inheritance at the core of object-oriented programming, and the lectures, centered on the textbook, aim at an understanding of the three basic concepts of object-oriented languages: encapsulation, inheritance, and polymorphism. Following Chapters 1-10 of the textbook, it covers getting started with C++ and the basics of C++ programming, classes and objects, object pointers, object arrays, and dynamic creation, functions, references, and copy constructors, function overloading and static members, friends and operator overloading, inheritance, virtual functions and abstract classes, and templates and the STL. Classes consist of theory lectures, labs, and quizzes, and students complete theory homework (solving the textbook exercises) and lab assignments for each chapter. Computer Language and Training Ⅱ is a C++ lab course taken in the same semester, and the lecture notes and lab code in this repository are based on the lecture slides and lab assignments of Computer Language Ⅱ. The instructor, schedule and class format, assessment, course policies, weekly plan and progress, and references are in the [course information document](Course-Information.md).

<br><a name="prerequisites"></a>
## ✅ Prerequisites

- A basic understanding of programming concepts is required. The syllabus designates Computer Language 1 and Computer Language 1 Lab as required prerequisites.
- A C++ compiler (e.g., GCC, Clang, MSVC) must be installed. Compiling the lab code in this repository requires a compiler that supports C++11 or later.
- Familiarity with command-line tools and an IDE (Visual Studio, VS Code, etc.) is required.

- **💻 Development Environment**

| Tool | Company |  OS  | Notes |
|:-----|:-------:|:----:|:------|
|CLion|JetBrains s.r.o.|macOS|    |
|Xcode|Apple Inc.|macOS|Most used IDE|

<br><a name="repository-structure"></a>
## 🗂 Repository Structure

```plaintext
24Fall_CPP-Practice
├── Lecture-Notes               Lecture notes, exercises, and solutions by chapter
├── Coursework                  C++ lab code by chapter
├── images                      Figures in the exercises
├── Course-Information.ko.md    Course operations
├── Course-Information.md
├── LICENSE                     Documentation license (CC BY-NC-SA 4.0)
├── LICENSE-CODE                Code license (PolyForm Noncommercial 1.0.0)
├── README.ko.md
└── README.md
```

The units are listed in the [`Lecture-Notes` index](Lecture-Notes/README.md), and the lab code in the [`Coursework` index](Coursework/README.md).

<br><a name="license"></a>
## 🤝 License

The documents in this repository (lecture notes, explanations, and summary sheets) are licensed under [CC BY-NC-SA 4.0](LICENSE), and the code under [PolyForm Noncommercial 1.0.0](LICENSE-CODE). Figures and example code taken from lecture slides and textbooks, as well as assignment handouts and template code distributed by the course, belong to their original authors and are not covered by these licenses.

---
