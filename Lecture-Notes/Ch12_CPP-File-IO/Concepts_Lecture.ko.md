# 12장 — C++ 파일 입출력

[레포지토리 안내](../../README.ko.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **최종 수정일:** 2026-11-19

> **학습 목표:**
> 1. 텍스트 파일과 바이너리 파일의 차이와 텍스트 모드의 개행 변환을 설명할 수 있다
> 2. `ifstream`, `ofstream`, `fstream`으로 파일을 열고 닫으며 열기 실패를 검사할 수 있다
> 3. `<<`, `>>`, `get()`, `put()`, `getline()`으로 텍스트 파일을 읽고 쓸 수 있다
> 4. 파일 모드를 조합하여 덮어쓰기, 이어 쓰기, 읽기와 쓰기를 함께 하는 열기를 구별할 수 있다
> 5. `read()`, `write()`와 `seekg()`, `seekp()`, `tellg()`, `tellp()`로 바이너리 레코드를 임의 접근할 수 있다
> 6. 스트림 상태로 파일 입력의 끝과 오류를 구별하고 `std::filesystem`으로 파일과 디렉터리를 다룰 수 있다

---

## 목차

- [1. 텍스트 파일과 바이너리 파일](#1-텍스트-파일과-바이너리-파일)
  - [1.1 바이트의 나열로서의 파일](#11-바이트의-나열로서의-파일)
  - [1.2 텍스트 모드와 바이너리 모드](#12-텍스트-모드와-바이너리-모드)
- [2. 파일 입출력 개요](#2-파일-입출력-개요)
  - [2.1 파일 스트림 클래스](#21-파일-스트림-클래스)
  - [2.2 파일 열기와 닫기](#22-파일-열기와-닫기)
  - [2.3 파일 경로](#23-파일-경로)
  - [2.4 열기 실패의 원인](#24-열기-실패의-원인)
- [3. `<<`와 `>>` 연산자를 이용한 텍스트 파일 입출력](#3-와--연산자를-이용한-텍스트-파일-입출력)
  - [3.1 파일에 쓰기](#31-파일에-쓰기)
  - [3.2 파일에서 읽기](#32-파일에서-읽기)
  - [3.3 객체의 저장과 복원](#33-객체의-저장과-복원)
- [4. 파일 모드](#4-파일-모드)
  - [4.1 모드 플래그](#41-모드-플래그)
  - [4.2 플래그 조합과 C의 `fopen` 모드](#42-플래그-조합과-c의-fopen-모드)
  - [4.3 모드에 따른 결과의 차이](#43-모드에-따른-결과의-차이)
- [5. 멤버 함수를 이용한 텍스트 I/O](#5-멤버-함수를-이용한-텍스트-io)
  - [5.1 `get()`과 `put()`을 이용한 문자 단위 복사](#51-get과-put을-이용한-문자-단위-복사)
  - [5.2 `getline()`을 이용한 줄 단위 읽기](#52-getline을-이용한-줄-단위-읽기)
  - [5.3 다른 운영체제에서 만든 텍스트 파일](#53-다른-운영체제에서-만든-텍스트-파일)
  - [5.4 줄, 단어, 문자 수 세기](#54-줄-단어-문자-수-세기)
- [6. 바이너리 I/O](#6-바이너리-io)
  - [6.1 `read()`와 `write()`](#61-read와-write)
  - [6.2 블록 단위 복사](#62-블록-단위-복사)
  - [6.3 구조체 레코드의 저장](#63-구조체-레코드의-저장)
- [7. 스트림 상태 검사](#7-스트림-상태-검사)
  - [7.1 검사해야 하는 지점](#71-검사해야-하는-지점)
  - [7.2 `eof()`를 반복 조건으로 쓰는 오류](#72-eof를-반복-조건으로-쓰는-오류)
  - [7.3 반복이 끝난 원인의 구별](#73-반복이-끝난-원인의-구별)
  - [7.4 예외로 실패 알리기](#74-예외로-실패-알리기)
- [8. 임의 접근](#8-임의-접근)
  - [8.1 파일 위치](#81-파일-위치)
  - [8.2 `seekg()`, `seekp()`, `tellg()`, `tellp()`](#82-seekg-seekp-tellg-tellp)
  - [8.3 레코드 단위의 임의 접근](#83-레코드-단위의-임의-접근)
  - [8.4 텍스트 모드에서의 위치 이동](#84-텍스트-모드에서의-위치-이동)
- [9. 파일 시스템 라이브러리](#9-파일-시스템-라이브러리)
  - [9.1 경로 다루기](#91-경로-다루기)
  - [9.2 파일과 디렉터리 조작](#92-파일과-디렉터리-조작)
- [개념 적용](#개념-적용)
- [용어 사전](#용어-사전)
- [요약](#요약)
- [점검 문제](#점검-문제)
- [참고 문헌](#참고-문헌)

---

<br>

## 1. 텍스트 파일과 바이너리 파일

### 1.1 바이트의 나열로서의 파일

디스크에 저장된 파일은 어떤 종류이든 바이트의 나열이다. 운영체제는 파일의 바이트에 의미를 부여하지 않으며, 그 바이트를 어떻게 해석할지는 파일을 읽는 프로그램이 정한다. 파일을 텍스트 파일과 바이너리 파일로 나누는 것은 바로 이 해석 방법에 따른 구분이다.

> **용어 정의:** **텍스트 파일(text file)** 은 바이트가 모두 문자 코드(ASCII, UTF-8 등)이고, 개행 문자로 나뉜 줄의 나열로 해석되는 파일이다.

> **용어 정의:** **바이너리 파일(binary file)** 은 바이트의 의미를 그 파일을 만든 프로그램이 정하는 파일이며, 이미지, 실행 파일, 압축 파일, 프로그램이 메모리의 데이터를 그대로 저장한 레코드 파일이 여기에 속한다.

텍스트 파일 `"Hi\nC++\n"`은 문자 코드 7바이트로 이루어진다. 개행 문자도 코드 값 `0x0A`를 가진 바이트 하나이다.

| 위치 | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|:-----|:-----|:-----|:-----|:-----|:-----|:-----|:-----|
| 문자 | `H` | `i` | `\n` | `C` | `+` | `+` | `\n` |
| 바이트 (16진수) | `48` | `69` | `0A` | `43` | `2B` | `2B` | `0A` |

*표 1. 텍스트 `"Hi\nC++\n"`의 바이트*

같은 정수 1000도 저장하는 방식에 따라 바이트가 다르다. 텍스트로 저장하면 문자 `'1'`, `'0'`, `'0'`, `'0'`의 코드 `31 30 30 30`이 되어 자릿수만큼 바이트를 쓰고, 사람이 편집기로 읽을 수 있다. 바이너리로 저장하면 메모리의 `int` 값 1000(16진수 `0x000003E8`)을 그대로 쓰므로 `int`가 4바이트인 환경에서 항상 4바이트이고, x86-64나 arm64처럼 낮은 자리 바이트를 먼저 두는 환경에서는 `E8 03 00 00`이 된다. 바이너리 저장은 크기가 일정하고 변환 비용이 없어 빠르지만, 편집기로 읽을 수 없고 다른 환경에서는 같은 값으로 해석되지 않을 수 있다(6.3절).

### 1.2 텍스트 모드와 바이너리 모드

운영체제마다 줄 끝을 나타내는 방법이 다르다. Unix 계열(Linux, macOS)은 개행 문자 `LF`(`0x0A`) 하나로 줄을 끝내고, Windows는 캐리지 리턴과 개행의 두 바이트 `CR LF`(`0x0D 0x0A`)로 끝낸다. C++ 프로그램 안에서는 어느 운영체제에서나 줄 끝을 `'\n'` 하나로 다루므로, 이 차이를 메워 주는 것이 파일을 여는 **모드**이다.

- **텍스트 모드**(기본)로 연 파일은 운영체제의 줄 끝 표기와 `'\n'` 사이를 변환할 수 있다. Windows의 C 런타임은 텍스트 모드에서 쓸 때 `LF`를 `CR LF`로 바꾸고, 읽을 때 `CR LF`를 `LF`로 바꾸며, 읽는 중에 `Ctrl+Z`(`0x1A`)를 만나면 파일의 끝으로 처리한다[1].
- **바이너리 모드**(`ios::binary`)로 연 파일은 바이트를 변환하지 않고 그대로 읽고 쓴다.
- POSIX 시스템에서는 두 모드가 같다. POSIX는 `fopen` 모드의 `b`가 아무 효과가 없다고 정한다[2].

[Example_HexDump.cpp](Example_HexDump.cpp)는 같은 문자열을 텍스트 모드와 바이너리 모드로 쓴 뒤, 두 파일을 바이너리 모드로 다시 열어 바이트를 출력한다. macOS에서의 결과는 다음과 같다.

```text
text.txt: 48 69 0a 43 2b 2b 0a  (7 bytes)
binary.txt: 48 69 0a 43 2b 2b 0a  (7 bytes)
```

macOS에서는 두 파일이 같다. Windows의 텍스트 모드 규칙[1]에 따르면, 같은 프로그램을 Windows에서 실행할 때 텍스트 모드로 쓴 `text.txt`는 개행마다 `0D`가 더해져 `48 69 0D 0A 43 2B 2B 0D 0A`의 9바이트가 된다. 바이트를 출력하는 `dump` 함수가 파일을 바이너리 모드로 여는 이유도 여기에 있다. 텍스트 모드로 열면 Windows에서 `0D 0A`가 `0A` 하나로 읽혀 저장된 바이트를 그대로 볼 수 없다.

바이너리 데이터를 텍스트 모드로 읽고 쓰면 Windows에서 다음과 같이 데이터가 바뀐다.

| 데이터의 바이트 | 텍스트 모드로 쓸 때 | 텍스트 모드로 읽을 때 |
|:-----|:-----|:-----|
| `0A` | `0D 0A`로 바뀜 (1바이트 늘어남) | 그대로 |
| `0D 0A` | `0D 0D 0A`로 바뀜 | `0A` 하나로 바뀜 (1바이트 줄어듦) |
| `1A` | 그대로 | 그 위치에서 파일이 끝난 것으로 처리됨 |

*표 2. Windows의 텍스트 모드가 바이너리 데이터에 주는 영향*

> **핵심:** 문자와 줄로 이루어지지 않은 데이터는 어느 운영체제에서나 `ios::binary`로 연다. POSIX에서는 효과가 없지만, 같은 코드를 Windows에서 실행할 때 데이터가 바뀌는 것을 막는다.

---

<br>

## 2. 파일 입출력 개요

### 2.1 파일 스트림 클래스

`<fstream>`은 파일에 연결된 세 가지 스트림 클래스를 제공한다. 11장의 그림 2처럼 `ifstream`은 `istream`을, `ofstream`은 `ostream`을, `fstream`은 `iostream`을 상속하므로, 11장에서 다룬 `>>`, `<<`, `get()`, `getline()`, 조작자, 상태 비트를 파일에서도 그대로 쓴다.

| 클래스 | 상속 | 용도 | 열 때 항상 더해지는 모드 |
|:-----|:-----|:-----|:-----|
| `ifstream` | `istream` | 파일에서 읽기 | `ios::in` |
| `ofstream` | `ostream` | 파일에 쓰기 | `ios::out` |
| `fstream` | `iostream` | 한 파일을 읽고 쓰기 | 없음 (지정하지 않으면 `ios::in \| ios::out`) |

*표 3. 파일 스트림 클래스*

`ifstream`은 어떤 모드를 주든 `ios::in`을 더해서 열고, `ofstream`은 `ios::out`을 더해서 연다[3]. 그래서 `ofstream f("log.txt", ios::app)`은 `ios::out | ios::app`으로 열린다. `fstream`은 모드를 주면 그 모드만 쓰므로 읽기와 쓰기 중 필요한 것을 직접 지정한다.

파일 스트림을 쓰는 순서는 다음과 같다.

1. 파일 스트림 객체를 만들면서, 또는 만든 뒤 `open()`으로 파일을 연다.
2. 파일이 열렸는지 검사한다.
3. `<<`, `>>`, 멤버 함수로 읽거나 쓴다.
4. `close()`로 닫거나, 객체가 소멸할 때 자동으로 닫히게 한다.

### 2.2 파일 열기와 닫기

파일을 여는 방법은 두 가지이다.

```cpp
ofstream fout("scores.txt");          // 1. 생성자에서 연다
ifstream fin;
fin.open("scores.txt");               // 2. 빈 스트림을 만든 뒤 open()으로 연다
```

**열기 검사.** 파일이 없거나, 권한이 없거나, 경로가 틀리면 열기에 실패하고 스트림에 `failbit`가 켜진다[3]. 실패한 스트림에서의 입출력은 아무 일도 하지 않으므로, 열기 직후에 검사하지 않으면 프로그램은 오류 없이 빈 결과를 낸다. 검사는 `if (!fin)`이나 `if (!fin.is_open())`으로 한다. `!fin`은 상태 비트를, `is_open()`은 파일이 연결되어 있는지를 본다.

```cpp
ifstream fin("scores.txt");
if (!fin) {                           // 열기에 실패하면 failbit가 켜진다
    cerr << "cannot open scores.txt" << endl;
    return 1;
}
```

**닫기.** `close()`는 출력 버퍼를 플러시하고 파일을 닫는다. 파일 스트림이 소멸할 때 그 안의 파일 버퍼(`filebuf`)의 소멸자가 `close()`를 호출하므로[3], 지역 변수인 파일 스트림은 블록을 벗어날 때 자동으로 닫힌다. 자원을 객체의 수명에 묶는 이 방식은 13장의 RAII이다. 명시적으로 `close()`를 호출해야 하는 경우는 다음과 같다.

- 같은 함수 안에서 방금 쓴 파일을 다시 열어 읽을 때. 닫지 않으면 버퍼의 내용이 아직 파일에 없을 수 있다.
- 같은 스트림 객체로 다른 파일을 열 때. 이미 열린 스트림에서 `open()`을 호출하면 실패한다[3].
- 파일을 오래 열어 두지 않아야 할 때(다른 프로그램이 그 파일을 써야 하는 경우 등).

> **참고:** **다시 열 때의 상태.** C++11부터 파일 스트림의 `open()`은 열기에 성공하면 `clear()`를 호출한다[3]. 그 이전의 표준에서는 끝까지 읽어 `eofbit`가 켜진 스트림을 닫고 다른 파일을 열어도 상태가 남아 첫 읽기부터 실패했으므로, 오래된 코드에는 `open()` 뒤에 `clear()`를 직접 호출하는 줄이 있다.

### 2.3 파일 경로

파일 이름에 경로를 붙이지 않으면 프로그램의 **작업 디렉터리**에서 파일을 찾는다. 작업 디렉터리는 소스 파일이나 실행 파일의 위치가 아니라 프로그램을 실행한 위치이다. 터미널에서는 현재 디렉터리이고, Visual Studio나 Xcode에서는 프로젝트 설정에서 정한 디렉터리이다. 프로그램이 IDE에서 파일을 찾지 못할 때 가장 먼저 확인할 항목이다. 이 장의 예제 프로그램도 경로 없는 이름으로 파일을 만들므로, 실행하면 작업 디렉터리에 `scores.txt`, `records.bin` 같은 파일이 생긴다.

Windows 경로의 `\`는 C++ 문자열에서 이스케이프 문자이므로 `"C:\\data\\scores.txt"`처럼 두 번 쓰거나, 원시 문자열 리터럴 `R"(C:\data\scores.txt)"`을 쓴다. Windows의 파일 API는 `/`도 경로 구분자로 받으므로 `"C:/data/scores.txt"`도 쓸 수 있다. C++17부터 파일 스트림의 생성자와 `open()`은 9절의 `std::filesystem::path`도 받는다[3].

### 2.4 열기 실패의 원인

파일 스트림은 열기에 실패했다는 사실만 `failbit`로 알리고, 그 원인은 알려 주지 않는다. 원인을 사용자에게 보고하려면 다른 방법으로 알아내야 한다. [Example_OpenFailure.cpp](Example_OpenFailure.cpp)는 두 가지 방법을 함께 쓴다.

```cpp
errno = 0;
ifstream fin(p);
if (fin) {
    fin.get();                               // 열렸다고 해서 읽을 수 있는 것은 아니다
    cout << p.string() << ": opened, first read "
         << (fin ? "succeeded" : "failed") << " (bad=" << fin.bad() << ")" << endl;
    return;
}
int saved = errno;                           // POSIX의 open()은 errno를 설정하지만 ISO C++는 보장하지 않는다
cout << p.string() << ": failed";
if (!fs::exists(p))
    cout << " (does not exist)";
else if ((fs::status(p).permissions() & fs::perms::owner_read) == fs::perms::none)
    cout << " (no read permission)";
cout << ", errno says: " << strerror(saved) << endl;
```

```text
present.txt: opened, first read succeeded (bad=0)
absent.txt: failed (does not exist), errno says: No such file or directory
locked.txt: failed (no read permission), errno says: Permission denied
folder: opened, first read failed (bad=0)
```

- **`errno`.** POSIX 시스템에서 파일을 여는 시스템 콜은 실패하면 원인 번호를 `errno`에 남기고, `strerror()`가 그 번호를 문장으로 바꾼다. ISO C++와 ISO C는 파일 열기가 `errno`를 설정한다고 보장하지 않으므로, 이 방법은 POSIX 환경에서 진단 메시지를 보이는 용도로만 쓴다.
- **`std::filesystem`.** 9절의 함수로 파일이 있는지, 권한이 있는지를 직접 확인할 수 있다. 이 방법은 운영체제와 무관하게 동작한다.
- **디렉터리.** macOS에서는 디렉터리 `folder`를 `ifstream`으로 여는 것이 성공으로 보고되고 첫 읽기에서 실패했다. 열기의 성공이 읽기의 성공을 보장하지 않으므로, 읽기 연산의 결과도 따로 검사해야 한다(7절). 디렉터리를 열 때의 동작은 운영체제마다 다르다.

---

<br>

## 3. `<<`와 `>>` 연산자를 이용한 텍스트 파일 입출력

### 3.1 파일에 쓰기

`ofstream`에 `<<`를 쓰면 `cout`에 출력하는 것과 같은 텍스트가 파일에 기록된다. [Example_ScoreFile.cpp](Example_ScoreFile.cpp)는 학생 세 명의 이름과 점수를 한 줄씩 쓴 뒤 다시 읽는다.

```cpp
ofstream fout("scores.txt");                 // 파일을 만들거나, 이미 있으면 내용을 비운다
if (!fout) {
    cerr << "cannot create scores.txt" << endl;
    return 1;
}
fout << "Kim 90\n" << "Lee 85\n" << "Park 77\n";
fout.close();                                // 버퍼를 플러시하고 파일을 닫는다
```

텍스트로 쓴 값은 `>>`로 다시 읽을 수 있도록 공백이나 개행으로 구분해야 한다. `fout << 90 << 85`처럼 구분 없이 쓰면 파일에 `9085`가 기록되어 정수 하나로 읽힌다. 마찬가지로 `Kim Minsu`처럼 공백이 든 이름을 `<<`로 쓰면 `>>`로는 두 단어로 읽히므로, 그런 값은 줄 단위로 저장하고 `getline()`으로 읽는다(5.2절).

### 3.2 파일에서 읽기

```cpp
ifstream fin("scores.txt");
if (!fin.is_open()) {
    cerr << "cannot open scores.txt" << endl;
    return 1;
}
string name;
int score, total = 0, count = 0;
while (fin >> name >> score) {               // 읽기가 실패하면 반복이 끝난다
    cout << name << " -> " << score << endl;
    total += score;
    count++;
}
cout << "count=" << count << ", total=" << total << endl;
cout << "eof=" << fin.eof() << ", fail=" << fin.fail() << endl;
```

```text
Kim -> 90
Lee -> 85
Park -> 77
count=3, total=252
eof=1, fail=1
```

반복 조건 `fin >> name >> score`가 어떻게 판정되는지 추적하면 다음과 같다.

| 반복 | 읽은 `name` | 읽은 `score` | 조건 | 상태 |
|:-----|:-----|:-----|:-----|:-----|
| 1 | `Kim` | 90 | 참 | 정상 |
| 2 | `Lee` | 85 | 참 | 정상 |
| 3 | `Park` | 77 | 참 | 정상 (`77` 뒤의 `'\n'` 앞에서 멈춤) |
| 4 | 없음 | 없음 | 거짓 | `'\n'`을 건너뛰다 끝에 도달, `eofbit`와 `failbit` |

*표 4. `while (fin >> name >> score)`의 추적*

세 번째 반복은 `77`을 읽고 줄 끝의 `'\n'` 앞에서 멈추므로 아직 끝에 도달하지 않았다. 네 번째 시도에서 `>> name`이 공백 문자를 건너뛰다가 파일의 끝을 만나 아무것도 읽지 못하므로 `eofbit`와 `failbit`가 함께 켜지고, 조건이 거짓이 되어 반복이 끝난다. 끝난 뒤의 상태 `eof=1, fail=1`은 "끝까지 읽어서 멈추었다"는 뜻이다(7.3절).

### 3.3 객체의 저장과 복원

11장 6절의 삽입 연산자와 추출 연산자는 매개변수가 `ostream&`과 `istream&`이므로, 같은 함수가 파일 스트림에도 그대로 쓰인다. [Example_PointFile.cpp](Example_PointFile.cpp)는 점의 목록을 파일에 저장하고 다시 읽는다.

```cpp
ostream& operator<<(ostream& out, const Point& p) {     // cout에도 ofstream에도 쓰인다
    return out << p.x << ' ' << p.y;
}

istream& operator>>(istream& in, Point& p) {            // cin에도 ifstream에도 쓰인다
    Point t;
    if (in >> t.x >> t.y)
        p = t;
    return in;
}

ofstream fout("shape.txt");
fout << shape.size() << '\n';                // 개수를 먼저 쓰고, 점을 한 줄에 하나씩 쓴다
for (const Point& p : shape)
    fout << p << '\n';
```

```text
3
0 0
4 0
4 3
```

저장된 `shape.txt`는 위와 같고, 프로그램은 이 파일을 읽어 `loaded 3 of 3 points: (0 0) (4 0) (4 3)`을 출력한다. 읽는 쪽은 첫 줄의 개수 `n`을 읽은 뒤 `for (size_t i = 0; i < n && fin >> p; i++)`로 최대 `n`개를 읽는다. 조건에 `fin >> p`를 함께 두었으므로 파일이 중간에 잘려 점이 모자라면 반복이 일찍 끝나고, 읽은 개수와 `n`을 비교하여 손상을 알아낼 수 있다. 저장 형식을 정할 때는 이처럼 개수나 구분자를 함께 기록하여 읽는 쪽이 데이터의 끝과 손상을 판단할 수 있게 한다.

---

<br>

## 4. 파일 모드

### 4.1 모드 플래그

파일을 여는 방식은 `ios_base::openmode` 타입의 플래그를 `|`로 조합하여 정한다.

| 플래그 | 뜻 |
|:-----|:-----|
| `ios::in` | 읽기용으로 연다 |
| `ios::out` | 쓰기용으로 연다 |
| `ios::app` | 쓸 때마다 파일의 끝으로 이동한 뒤 쓴다 (append) |
| `ios::ate` | 연 직후 위치를 파일의 끝으로 옮긴다 (at end) |
| `ios::trunc` | 열 때 기존 내용을 지운다 (truncate) |
| `ios::binary` | 바이너리 모드로 연다 (1.2절) |
| `ios::noreplace` (C++23) | 파일이 이미 있으면 열기에 실패한다 (배타적 만들기) |

*표 5. 파일 모드 플래그*

앞의 여섯 플래그는 C++17 표준에 정의되어 있고[3], `noreplace`는 C++23에서 추가되었다[4]. `app`과 `ate`는 이름이 비슷하지만 다르다. `app`은 모든 쓰기를 파일의 끝에 하므로 `seekp()`로 위치를 옮겨도 쓰기는 끝에 붙는다. `ate`는 처음 위치만 끝으로 옮길 뿐이므로 그 뒤에는 원하는 위치로 이동하여 읽거나 쓸 수 있다.

### 4.2 플래그 조합과 C의 `fopen` 모드

파일 스트림은 내부적으로 C의 `fopen`과 같은 방식으로 파일을 연다. 표준은 플래그 조합(`ate`는 제외)마다 그와 같은 `fopen` 모드 문자열을 정하고, **표에 없는 조합이면 열기가 실패한다**고 규정한다[3].

| 플래그 조합 (`binary` 제외) | `fopen` 모드 | 파일이 있을 때 | 파일이 없을 때 |
|:-----|:-----|:-----|:-----|
| `out`, `out \| trunc` | `"w"` | 내용을 지우고 씀 | 새로 만듦 |
| `out \| app`, `app` | `"a"` | 끝에 이어 씀 | 새로 만듦 |
| `in` | `"r"` | 처음부터 읽음 | 열기 실패 |
| `in \| out` | `"r+"` | 내용을 유지하고 읽고 씀 | 열기 실패 |
| `in \| out \| trunc` | `"w+"` | 내용을 지우고 읽고 씀 | 새로 만듦 |
| `in \| out \| app`, `in \| app` | `"a+"` | 읽고, 쓰기는 끝에 이어 씀 | 새로 만듦 |

*표 6. 파일 모드 조합과 같은 `fopen` 모드*

각 조합에 `binary`를 더하면 `fopen` 모드에 `b`가 붙는다(`"wb"`, `"r+b"` 등). `in | trunc`, `trunc` 하나, `out | app | trunc`처럼 표에 없는 조합은 열기에 실패하며, 실행해 보면 세 경우 모두 `is_open()`이 거짓이다. 표에서 알 수 있는 사실을 정리하면 다음과 같다.

- `out`만 주면 `trunc`를 주지 않아도 기존 내용이 지워진다. `ofstream`의 기본 동작이 덮어쓰기인 이유이다.
- 기존 내용을 지우지 않고 수정하려면 `in | out`으로 연다. 이 조합은 `"r+"`와 같으므로 파일이 없으면 실패한다.
- `ate`는 표의 조합에 더해 쓰는 위치 옵션이다. `out | ate`는 여전히 `"w"`와 같으므로 내용이 지워진다. 기존 내용 끝에서 시작하려면 `in | out | ate`나 `app`을 쓴다.

### 4.3 모드에 따른 결과의 차이

[Example_OpenModes.cpp](Example_OpenModes.cpp)는 같은 파일 `mode.txt`를 모드를 바꿔 가며 열고, 매번 파일의 줄을 `/`로 이어 출력한다.

```cpp
{ ofstream f("mode.txt"); f << "one\ntwo\n"; }             // out: 지우고 쓴다
show("out        ");
{ ofstream f("mode.txt", ios::app); f << "three\n"; }      // app: 모든 쓰기가 끝에 붙는다
show("app        ");
{ fstream f("mode.txt", ios::in | ios::out); f << "ONE"; } // in | out: 내용을 유지한다
show("in | out   ");
{ ofstream f("mode.txt", ios::out | ios::ate); f << "X\n"; } // out | ate: 그래도 지운다
show("out | ate  ");
{ fstream f("missing.txt", ios::in | ios::out);
  cout << "in | out on a missing file opens: " << f.is_open() << endl; }
```

```text
out        : one/two/
app        : one/two/three/
in | out   : ONE/two/three/
out | ate  : X/
in | out on a missing file opens: 0
```

중괄호 블록마다 스트림이 소멸하며 파일이 닫히므로, 다음 줄의 `show()`는 쓰기가 끝난 파일을 읽는다. 세 번째 결과에서 `in | out`으로 연 파일의 위치는 처음이므로 `ONE`이 `one`을 덮어쓰고 나머지 내용은 그대로 남는다. 네 번째 결과는 `ate`를 주어도 `out`이 내용을 지운다는 것을 보인다. 마지막 줄은 `"r+"`와 같은 `in | out`이 없는 파일을 만들지 않는다는 것을 보인다.

> **주의:** 기존 파일에 기록을 덧붙일 생각으로 `ofstream f("log.txt");`라고 쓰면 실행할 때마다 이전 기록이 지워진다. 덧붙이려면 `ios::app`을 지정한다.

---

<br>

## 5. 멤버 함수를 이용한 텍스트 I/O

### 5.1 `get()`과 `put()`을 이용한 문자 단위 복사

`>>`는 공백 문자를 건너뛰므로 파일을 그대로 복사하는 데 쓸 수 없다. 공백과 개행을 포함한 모든 문자를 옮기려면 11장의 비형식 함수 `get()`과 `put()`을 쓴다. [Example_LineNumbers.cpp](Example_LineNumbers.cpp)의 앞부분은 다음과 같다.

```cpp
ifstream fin(src);
ofstream fout("copy.txt");
if (!fin || !fout) {
    cerr << "cannot open the files" << endl;
    return 1;
}
int ch, chars = 0;
while ((ch = fin.get()) != EOF) {            // get()은 EOF를 구별하기 위해 int를 리턴한다
    fout.put(static_cast<char>(ch));
    chars++;
}
```

입력 파일은 `"roses are red\n\nviolets are blue"`이다. 첫 줄 13글자, 개행 2개, 셋째 줄 16글자이므로 13 + 2 + 16 = 31글자가 복사되고, 프로그램은 `copied 31 characters`를 출력한다. 마지막 줄에는 개행이 없지만 `get()`은 줄과 무관하게 문자를 읽으므로 그대로 복사된다.

### 5.2 `getline()`을 이용한 줄 단위 읽기

줄마다 처리해야 하는 텍스트(설정 파일, 로그, CSV)는 전역 함수 `getline(fin, line)`으로 읽는다. 같은 예제의 뒷부분은 복사한 파일의 줄에 번호를 붙이고 가장 긴 줄의 길이를 구한다.

```cpp
ifstream again("copy.txt");
string line;
int number = 0;
size_t longest = 0;
while (getline(again, line)) {               // 마지막 줄은 '\n'이 없어도 센다
    number++;
    if (line.size() > longest)
        longest = line.size();
    cout << number << ": " << line << endl;
}
cout << number << " lines, longest " << longest << " characters" << endl;
```

```text
copied 31 characters
1: roses are red
2: 
3: violets are blue
3 lines, longest 16 characters
```

- 둘째 줄은 빈 줄이다. `getline()`은 곧바로 `'\n'`을 만나 빈 문자열을 저장하지만, 구분 문자를 꺼냈으므로 실패가 아니고 줄 수에 들어간다.
- 셋째 줄 `violets are blue`는 개행 없이 파일의 끝에서 멈춘다. `eofbit`는 켜지지만 문자를 읽었으므로 조건이 참이고 줄 수에 들어간다.
- 네 번째 시도에서 읽을 문자가 없으므로 `failbit`가 켜지고 반복이 끝난다.

이처럼 `while (getline(fin, line))`은 마지막 줄에 개행이 있든 없든 줄의 개수를 정확히 센다. 줄의 길이 제한이 있는 배열용 멤버 함수 `fin.getline(buf, n)`은 긴 줄에서 `failbit`를 켜므로(11장 3.3절), 길이를 모르는 파일에는 `string`을 쓰는 전역 함수가 알맞다.

### 5.3 다른 운영체제에서 만든 텍스트 파일

텍스트 모드의 변환은 프로그램을 실행하는 운영체제의 관례만 처리한다. Windows에서 만든 `CR LF` 줄 끝의 파일을 macOS나 Linux에서 읽으면 변환이 일어나지 않으므로, `getline()`이 `'\n'` 앞에서 멈추고 `'\r'`(`0x0D`)이 문자열 끝에 남는다. `"one\r\ntwo\r\n"`을 읽으면 첫 줄은 `"one"`이 아니라 길이 4인 `"one\r"`이 되어 `line == "one"` 같은 비교가 실패한다. 여러 운영체제의 파일을 받는 프로그램은 읽은 줄의 끝에 `'\r'`이 있으면 지운다.

```cpp
while (getline(fin, line)) {
    if (!line.empty() && line.back() == '\r')
        line.pop_back();                     // Windows에서 만든 파일의 CR을 지운다
    // ... line을 처리한다
}
```

### 5.4 줄, 단어, 문자 수 세기

`getline()`으로 줄을 읽고 그 줄을 `istringstream`(11장 8절)으로 나누면 줄 단위 처리와 단어 단위 처리를 함께 할 수 있다. [Example_WordCount.cpp](Example_WordCount.cpp)는 Unix의 `wc` 명령처럼 줄, 단어, 문자의 수를 센다.

```cpp
long lines = 0, words = 0, chars = 0;
string line, longest;
while (getline(fin, line)) {
    lines++;
    chars += line.size() + 1;                // getline()이 버린 '\n'을 위해 1을 더한다
    istringstream in(line);
    string w;
    int inLine = 0;
    while (in >> w) {                        // >>가 공백 문자에서 줄을 나눈다
        inLine++;
        if (w.size() > longest.size())
            longest = w;
    }
    words += inLine;
    cout << "line " << lines << ": " << inLine << " words" << endl;
}
if (fin.eof() && !line.empty())              // 마지막 줄에 '\n'이 없었다
    chars--;
```

입력 파일이 `"the quick brown fox\n  jumps over\n\nthe lazy dog\n"`일 때 결과는 다음과 같고, 같은 파일에 대한 `wc`의 출력 `4 9 47`과 일치한다.

```text
line 1: 4 words
line 2: 2 words
line 3: 0 words
line 4: 3 words
4 9 47 words.txt
longest word: quick
```

| 줄 | 내용 | `line.size() + 1` | 단어 수 |
|:-----|:-----|:-----|:-----|
| 1 | `the quick brown fox` | 19 + 1 = 20 | 4 |
| 2 | `  jumps over` | 12 + 1 = 13 | 2 |
| 3 | (빈 줄) | 0 + 1 = 1 | 0 |
| 4 | `the lazy dog` | 12 + 1 = 13 | 3 |
| 합계 | | 47 | 9 |

*표 7. 줄마다 센 문자 수와 단어 수*

- 둘째 줄 앞의 공백 두 개는 `>>`가 건너뛰므로 단어로 세지 않지만 문자 수에는 들어간다.
- 빈 줄도 `getline()`이 성공하므로 줄 수에 들어가고, 단어는 0개이다.
- 문자 수는 줄마다 `getline()`이 버린 `'\n'`을 1로 더한다. 마지막 줄에 `'\n'`이 없는 파일에서는 실제보다 하나를 더 세므로 반복 뒤에 1을 뺀다. 반복이 끝날 때의 마지막 `getline()`은 읽기를 시작하지 못하면 `line`을 바꾸지 않으므로, `line`이 비어 있지 않다는 것은 마지막으로 읽은 줄이 `'\n'` 없이 파일의 끝에서 끝났다는 뜻이다.
- 가장 긴 단어는 길이가 5인 `quick`, `brown`, `jumps` 가운데 처음 나온 `quick`이다. 비교가 `>`이므로 길이가 같은 뒤의 단어로 바뀌지 않는다.

---

<br>

## 6. 바이너리 I/O

### 6.1 `read()`와 `write()`

```cpp
istream& read(char* s, streamsize n);
ostream& write(const char* s, streamsize n);
```

`read()`는 스트림에서 `n`바이트를 꺼내 `s`에 저장하고, `write()`는 `s`부터 `n`바이트를 그대로 쓴다. 두 함수 모두 문자 코드를 해석하지 않으므로 바이너리 데이터에 쓴다. 매개변수가 `char*`이므로 다른 타입의 데이터는 `reinterpret_cast<char*>(&value)`로 바이트 단위의 주소로 바꾸어 넘긴다.

```cpp
int value = 1000;
fout.write(reinterpret_cast<const char*>(&value), sizeof(value));   // 4바이트 E8 03 00 00
int back;
fin.read(reinterpret_cast<char*>(&back), sizeof(back));
```

`read()`는 파일의 끝에 도달하여 `n`바이트를 다 채우지 못하면 읽은 만큼만 저장하고 `eofbit`와 `failbit`를 함께 켠다. 실제로 읽은 바이트 수는 `gcount()`로 알 수 있다. 따라서 `read()`가 실패했다고 해서 데이터를 하나도 읽지 않았다고 판단하면 안 된다.

### 6.2 블록 단위 복사

한 바이트씩 복사하는 5.1절의 방법은 이해하기 쉽지만 함수 호출이 바이트 수만큼 일어난다. 큰 파일은 버퍼 크기만큼 한꺼번에 읽고 쓴다.

```cpp
ifstream fin("big.bin", ios::binary);
ofstream fout("big2.bin", ios::binary);
char buf[4096];
while (fin.read(buf, sizeof buf) || fin.gcount() > 0)   // 마지막 조각도 처리한다
    fout.write(buf, fin.gcount());
```

10,000바이트 파일을 이 코드로 복사하면 4,096바이트, 4,096바이트, 1,808바이트의 세 조각으로 읽힌다(4096 + 4096 + 1808 = 10000). 세 번째 `read()`는 4,096바이트를 채우지 못해 실패하지만 `gcount()`가 1,808이므로 조건의 두 번째 부분이 참이 되어 그 조각을 쓴다. 네 번째 `read()`는 하나도 읽지 못해 `gcount()`가 0이 되고 반복이 끝난다. 조건을 `while (fin.read(buf, sizeof buf))`로만 쓰면 마지막 1,808바이트를 잃는다. `write()`에 `sizeof buf`가 아니라 `fin.gcount()`를 넘기는 것도 같은 이유이다.

> **주의:** 블록 복사에서 `write(buf, sizeof buf)`처럼 항상 버퍼 크기만큼 쓰면, 마지막 조각을 쓸 때 이전에 읽은 데이터의 나머지까지 파일에 기록되어 복사본이 원본보다 커진다.

### 6.3 구조체 레코드의 저장

같은 크기의 레코드를 바이너리로 저장하면 레코드 번호로 위치를 계산하여 바로 읽을 수 있다(8절). [Example_BinaryRecords.cpp](Example_BinaryRecords.cpp)는 다음 구조체를 사용한다.

```cpp
struct Record {
    int id;
    char name[10];
    double score;
};
```

```text
sizeof(Record) = 24, offsets: id 0, name 4, score 16
```

멤버의 크기를 더하면 4 + 10 + 8 = 22바이트이지만 이 실행 환경(macOS, arm64)에서 `sizeof(Record)`는 24이다. 이 환경에서 `double`은 8의 배수 주소에 놓여야 하므로 컴파일러가 `name` 뒤에 2바이트의 **패딩**을 넣어 `score`를 오프셋 16에 둔다. 첫 레코드 `{101, "Kim", 93.5}`가 파일에 저장된 바이트는 다음과 같다.

```text
오프셋   내용                                바이트 (16진수, 낮은 주소부터)
 0~3     id = 101                            65 00 00 00
 4~13    name = "Kim"과 널 문자 7개          4B 69 6D 00 00 00 00 00 00 00
14~15    패딩 (값은 정해지지 않음)           00 00
16~23    score = 93.5 (IEEE 754 배정밀도)    00 00 00 00 00 60 57 40
```

*그림 1. `Record {101, "Kim", 93.5}`의 24바이트 배치*

`id`의 101은 16진수 `0x65`이므로 `65 00 00 00`이고, `score`의 93.5는 IEEE 754 배정밀도 값 `0x4057600000000000`을 낮은 자리 바이트부터 쓴 `00 00 00 00 00 60 57 40`이다.

> **[컴퓨터구조]** 컴퓨터구조에서 배우는 데이터 정렬(alignment)과 바이트 순서(endianness)가 바이너리 파일의 형식을 정한다. 프로세서는 크기가 n바이트인 데이터를 n의 배수 주소에서 가장 효율적으로 읽기 때문에 컴파일러가 구조체에 패딩을 넣고, x86-64와 arm64는 낮은 자리 바이트를 낮은 주소에 두는 리틀 엔디언이다.

구조체를 `write()`로 통째로 저장하는 방법은 간단하지만, 다음 조건을 지켜야 한다.

- **멤버가 모두 값 자체를 담아야 한다.** `std::string`, `std::vector`, 포인터 멤버는 실제 데이터가 아니라 힙의 주소를 담고 있으므로, 저장한 바이트를 다시 읽으면 이미 해제되었거나 다른 프로세스의 의미 없는 주소가 된다. 이런 멤버는 길이와 내용을 따로 저장해야 한다. 바이트 복사로 저장해도 되는 타입을 표준은 **트리비얼하게 복사 가능한 타입(trivially copyable type)** 이라 하며, `std::is_trivially_copyable<T>::value`로 확인할 수 있다[3].
- **같은 환경에서만 읽는다.** 패딩, 바이트 순서, `int`와 `long`의 크기는 컴파일러와 프로세서마다 다를 수 있으므로, 다른 환경에서 읽을 파일은 멤버를 하나씩 정해진 크기와 바이트 순서로 쓰는 **직렬화(serialization)** 방식으로 저장한다.
- **패딩의 값은 정해져 있지 않다.** 구조체를 바이트로 비교하거나 해시를 구하면 같은 값의 레코드가 다르게 나올 수 있다.

---

<br>

## 7. 스트림 상태 검사

### 7.1 검사해야 하는 지점

파일 입출력의 상태 비트는 11장 4절과 같다. 파일에서 실패가 일어나는 지점과 그때의 상태는 다음과 같다.

| 지점 | 실패의 원인 | 켜지는 비트 | 검사 방법 |
|:-----|:-----|:-----|:-----|
| 열기 | 파일 없음, 권한 없음, 경로 오류, 표에 없는 모드 조합 | `failbit` | `if (!f)`, `is_open()` |
| 형식화 읽기 | 기대한 형식이 아님 | `failbit` | 읽기 연산의 결과를 조건으로 |
| 읽기 | 파일의 끝 | `eofbit` (더 읽으려 했으면 `failbit`도) | 반복이 끝난 뒤 `eof()` |
| 읽기, 쓰기 | 장치 오류, 디스크 가득 참 | `badbit` | `bad()` |
| 닫기 | 버퍼의 플러시 실패 | `failbit` | `close()` 뒤 `fail()` |

*표 8. 파일 입출력의 실패 지점과 검사 방법*

쓰기의 실패는 버퍼 때문에 늦게 드러난다. `<<`가 성공해도 바이트는 아직 버퍼에 있을 수 있고, 디스크가 가득 찬 사실은 플러시할 때 알려진다. 중요한 데이터를 쓰는 프로그램은 `close()`를 명시적으로 호출하고 그 뒤에 `fail()`을 검사한다.

### 7.2 `eof()`를 반복 조건으로 쓰는 오류

`while (!fin.eof())`는 "파일의 끝이 아닌 동안 읽는다"로 읽혀 자연스러워 보이지만, `eof()`는 읽기가 끝에 **도달한 뒤에야** 참이 되므로 마지막 값을 두 번 처리한다. [Example_EofLoop.cpp](Example_EofLoop.cpp)는 `10\n20\n30\n`이 든 파일로 이 오류를 보인다.

```cpp
while (!bad.eof()) {                         // 잘못됨: 읽기가 끝에 닿기 전에는 eof()가 거짓이다
    bad >> value;
    sum += value;
    cout << "read " << value << " (eof=" << bad.eof() << ", fail=" << bad.fail() << ")" << endl;
}
```

```text
read 10 (eof=0, fail=0)
read 20 (eof=0, fail=0)
read 30 (eof=0, fail=0)
read 30 (eof=1, fail=1)
wrong loop sum = 90
correct loop sum = 60
stopped at the end of the file
```

| 반복 | 시작할 때 `eof()` | `bad >> value`의 결과 | `value` | `sum` |
|:-----|:-----|:-----|:-----|:-----|
| 1 | 0 | `10` 읽음, `'\n'` 앞에서 멈춤 | 10 | 10 |
| 2 | 0 | `20` 읽음 | 20 | 30 |
| 3 | 0 | `30` 읽음, 마지막 `'\n'` 앞에서 멈춤 | 30 | 60 |
| 4 | 0 | `'\n'`을 건너뛰다 끝에 도달, 실패 | 30 (바뀌지 않음) | 90 |

*표 9. `while (!fin.eof())`의 추적*

세 번째 반복이 끝났을 때 마지막 `'\n'`이 아직 남아 있으므로 `eof()`는 거짓이다. 네 번째 반복에서 `>>`가 그 `'\n'`을 건너뛰다가 끝에 도달해 실패하는데, 숫자를 찾기 전에 끝났으므로 `value`는 30 그대로이고(11장 4.2절), 실패를 검사하지 않은 본문이 30을 한 번 더 더한다. 파일의 마지막 줄에 개행이 없으면 세 번째 읽기에서 `eofbit`가 켜져 결과가 맞게 나오므로, 이 오류는 파일에 따라 나타나기도 하고 숨기도 한다. 올바른 반복은 읽기 연산 자체를 조건으로 쓴다.

```cpp
while (good >> value)                        // 올바름: 읽기의 결과를 검사한다
    sum += value;
```

> **핵심:** 반복 조건에는 `eof()`가 아니라 읽기 연산(`fin >> x`, `getline(fin, line)`, `fin.get(c)`, `fin.read(...)`)을 쓴다. `eof()`는 반복이 끝난 뒤 그 원인을 구별할 때 쓴다.

### 7.3 반복이 끝난 원인의 구별

`while (fin >> x)`는 파일의 끝뿐 아니라 형식이 틀린 데이터와 장치 오류에서도 끝난다. 반복 뒤에 다음 순서로 원인을 구별하여 오류를 보고한다.

```cpp
if (good.bad())
    cout << "stopped by an I/O error" << endl;
else if (good.eof())
    cout << "stopped at the end of the file" << endl;
else
    cout << "stopped at data that is not an integer" << endl;
```

`bad()`를 먼저 검사하는 이유는 장치 오류가 가장 심각하고 다른 비트와 함께 켜질 수 있기 때문이다. 그다음 `eof()`가 참이면 정상적으로 끝까지 읽은 것이고, 둘 다 아니면 `failbit`만 켜진 것이므로 파일 중간에 정수가 아닌 데이터가 있다는 뜻이다. 예를 들어 `10 20 abc 30`이 든 파일은 합계 30에서 멈추고 세 번째 메시지를 출력한다.

### 7.4 예외로 실패 알리기

상태 비트는 검사하지 않으면 그대로 지나간다. 멤버 함수 `exceptions(mask)`를 호출하면 `mask`의 비트가 켜지는 순간 스트림이 `ios_base::failure` 예외를 던진다[3]. 예외의 문법은 13장에서 다룬다. [Example_StreamExceptions.cpp](Example_StreamExceptions.cpp)는 열기 실패를 예외로 받는다.

```cpp
ifstream fin;
fin.exceptions(ios::failbit | ios::badbit);  // 이 비트가 켜지면 예외를 던진다
try {
    fin.open("no_such_file.txt");            // 열기 실패로 failbit가 켜지므로 예외가 발생한다
    cout << "opened" << endl;
} catch (const ios_base::failure& e) {
    cout << "caught ios_base::failure" << endl;
}
```

```text
caught ios_base::failure
state: fail=1, is_open=0
7 8 -1 fail=1
```

> **주의:** `failbit`에 예외를 설정한 스트림에 `while (fin >> x)`를 쓰면, 파일의 끝에서 마지막 읽기가 `failbit`를 켜는 순간 예외가 발생한다. 정상적인 끝까지 예외로 처리되므로, 반복으로 읽는 스트림에는 `badbit`만 설정한다.

예제의 뒷부분은 `badbit`만 설정한 스트림에서 `7 8`을 세 번 읽는다. 세 번째 읽기는 끝에서 실패하지만 `failbit`는 예외 대상이 아니므로 예외 없이 `c`가 -1로 남는다.

---

<br>

## 8. 임의 접근

### 8.1 파일 위치

스트림은 다음에 읽거나 쓸 바이트의 위치를 기억한다. 이 위치는 파일의 처음부터 센 바이트 수이며, 읽거나 쓰면 그만큼 뒤로 이동한다. 처음부터 순서대로 처리하는 것을 **순차 접근**, 위치를 원하는 곳으로 옮겨 처리하는 것을 **임의 접근**이라 한다.

> **용어 정의:** **임의 접근(random access)** 은 파일의 처음부터 차례로 읽지 않고 파일 위치를 원하는 바이트로 옮겨 그곳에서 바로 읽거나 쓰는 방식이다.

`istream`은 읽을 위치(get 위치)를, `ostream`은 쓸 위치(put 위치)를 다루는 함수를 가진다. 다만 파일 스트림의 버퍼는 읽기와 쓰기에 하나의 파일 위치를 함께 쓰므로[3], `fstream`에서 `seekg()`로 읽을 위치를 옮기면 쓸 위치도 함께 옮겨진다. 실제로 `in | out`으로 연 `fstream`에서 `seekg(2)` 뒤의 `tellp()`는 2이고, `seekp(4)` 뒤의 `tellg()`는 4이다. 반면 `stringstream`은 두 위치를 따로 가지므로 `seekg(2)` 뒤에도 `tellp()`는 0이다.

### 8.2 `seekg()`, `seekp()`, `tellg()`, `tellp()`

| 함수 | 동작 |
|:-----|:-----|
| `seekg(pos)` | 읽을 위치를 처음부터 `pos`바이트인 곳으로 옮긴다 |
| `seekg(off, dir)` | 읽을 위치를 기준 `dir`에서 `off`바이트 떨어진 곳으로 옮긴다 |
| `tellg()` | 현재 읽을 위치를 리턴한다 (실패하면 -1) |
| `seekp(pos)`, `seekp(off, dir)` | 쓸 위치에 대해 같은 일을 한다 |
| `tellp()` | 현재 쓸 위치를 리턴한다 |

*표 10. 파일 위치를 다루는 멤버 함수*

기준 `dir`은 `ios::beg`(파일의 처음), `ios::cur`(현재 위치), `ios::end`(파일의 끝)이다[3]. `off`는 부호 있는 정수 타입 `streamoff`이므로 `seekg(-4, ios::end)`는 끝에서 4바이트 앞으로, `seekg(8, ios::cur)`는 현재 위치에서 8바이트 뒤로 옮긴다. `seekg()`는 `eofbit`만 켜져 있으면 그 비트를 지우고 이동하지만, `failbit`가 켜져 있으면 이동하지 않고 실패한다[3]. 끝에서 더 읽으려다 `failbit`까지 켜진 스트림의 위치를 옮기려면 먼저 `clear()`를 호출한다.

**파일 크기.** 위치를 끝으로 옮긴 뒤 `tellg()`를 호출하면 파일의 바이트 수를 얻는다.

```cpp
file.seekg(0, ios::end);
cout << "file size = " << file.tellg() << " bytes" << endl;   // 72
```

이 방법은 바이너리 모드에서 쓴다. C 표준은 바이너리 스트림이 `SEEK_END` 기준의 이동을 의미 있게 지원하지 않아도 된다고 하고[5], 텍스트 모드에서는 8.4절의 제약이 있으므로, 이식성이 중요하면 9절의 `std::filesystem::file_size()`를 쓴다.

### 8.3 레코드 단위의 임의 접근

레코드의 크기가 일정하면 인덱스 `i`인 레코드는 바이트 `i × sizeof(Record)`에서 시작한다. 6.3절의 `Record`는 24바이트이므로 레코드 0, 1, 2는 각각 바이트 0, 24, 48에서 시작하고 파일 크기는 3 × 24 = 72바이트이다. [Example_BinaryRecords.cpp](Example_BinaryRecords.cpp)는 레코드 2만 읽고, 그 점수를 고친 뒤 제자리에 다시 쓴다.

```cpp
fstream file("records.bin", ios::in | ios::out | ios::binary);   // 내용을 유지하고 읽고 쓴다
Record r;
file.seekg(2 * sizeof(Record), ios::beg);    // 인덱스 2인 레코드로 이동한다
file.read(reinterpret_cast<char*>(&r), sizeof(r));

r.score = 88.0;                              // 레코드 2의 점수를 고친다
file.seekp(2 * sizeof(Record), ios::beg);
file.write(reinterpret_cast<const char*>(&r), sizeof(r));
file.flush();

file.seekg(0, ios::beg);
while (file.read(reinterpret_cast<char*>(&r), sizeof(r)))
    cout << r.id << ' ' << r.name << ' ' << r.score << endl;
```

```text
sizeof(Record) = 24, offsets: id 0, name 4, score 16
file size = 72 bytes
record 2: 103 Park 72.25 (gcount 24, tellg 72)
101 Kim 93.5
102 Lee 81
103 Park 88
last gcount = 0
```

위치의 변화를 추적하면 다음과 같다.

| 연산 | 연산 전 위치 | 연산 후 위치 | 결과 |
|:-----|:-----|:-----|:-----|
| `seekg(0, ios::end)`, `tellg()` | 0 | 72 | 파일 크기 72 |
| `seekg(48, ios::beg)` | 72 | 48 | 레코드 2의 시작 |
| `read(&r, 24)` | 48 | 72 | `{103, "Park", 72.25}`, `gcount()`는 24 |
| `seekp(48, ios::beg)` | 72 | 48 | 같은 레코드로 되돌아감 |
| `write(&r, 24)` | 48 | 72 | 점수 88.0인 레코드로 덮어씀 |
| `seekg(0, ios::beg)` 뒤 `read` 반복 | 0 | 72 | 레코드 세 개를 읽고 네 번째에서 실패 |

*표 11. 레코드 2를 읽고 고칠 때의 파일 위치*

파일을 `in | out | binary`로 연 점이 중요하다. `out | binary`만 주면 `"wb"`와 같아서 기존 레코드가 모두 지워진다. 또한 읽기와 쓰기를 바꿀 때는 그 사이에 위치 이동 함수를 호출한다. 표준은 파일 스트림의 읽기와 쓰기에 C의 `FILE`과 같은 제약을 두며[3], C는 갱신 모드로 연 스트림에서 쓰기 바로 뒤에 읽으려면 `fflush`나 위치 이동 함수를, 읽기 바로 뒤에 쓰려면 위치 이동 함수를 그 사이에 호출하도록 요구한다[5]. 마지막 반복은 레코드 세 개를 읽고 네 번째 `read()`가 0바이트에서 실패하므로 `gcount()`가 0이다.

### 8.4 텍스트 모드에서의 위치 이동

텍스트 모드에서는 Windows처럼 줄 끝이 변환되는 환경에서 파일의 바이트 수와 프로그램이 읽은 문자 수가 일치하지 않는다. 그래서 C 표준은 텍스트 스트림에 대해 위치 이동의 오프셋을 0이거나 앞서 위치 조회 함수가 리턴한 값으로만 허용한다[5]. 파일 스트림은 C의 `fseek`과 같은 방식으로 위치를 옮기므로 같은 제약을 따른다. 텍스트 파일에서는 `tellg()`로 얻은 위치를 저장해 두었다가 `seekg(pos)`로 되돌아가는 용도로만 쓰고, 바이트 수를 계산하여 이동하는 작업은 바이너리 모드에서 한다.

---

<br>

## 9. 파일 시스템 라이브러리

파일 스트림은 파일의 내용을 읽고 쓰지만, 파일이 있는지 확인하거나 디렉터리를 만들고 지우는 기능은 없다. C++17의 `<filesystem>`은 이런 작업을 운영체제와 무관한 방법으로 제공한다[3]. 이름이 길어 `namespace fs = std::filesystem;`처럼 별칭을 두고 쓰는 것이 관례이다.

### 9.1 경로 다루기

`fs::path`는 경로를 나타내는 클래스이다. `/` 연산자는 운영체제에 맞는 구분자로 경로를 잇고, 멤버 함수가 경로의 부분을 나누어 준다.

```cpp
fs::path p = dir / "logs" / "b.log";         // fs_demo/logs/b.log
p.parent_path();                             // fs_demo/logs
p.filename();                                // b.log
p.stem();                                    // b
p.extension();                               // .log
```

`generic_string()`은 구분자를 `/`로 통일한 문자열을, `string()`은 운영체제 고유의 형식(Windows에서는 `\`)으로 된 문자열을 리턴한다. 파일 스트림은 `fs::path`를 그대로 받으므로 `ofstream(dir / "a.txt")`처럼 쓸 수 있다.

### 9.2 파일과 디렉터리 조작

| 함수 | 동작 |
|:-----|:-----|
| `fs::exists(p)` | `p`가 가리키는 파일이나 디렉터리가 있는지 |
| `fs::is_regular_file(p)`, `fs::is_directory(p)` | 일반 파일인지, 디렉터리인지 |
| `fs::file_size(p)` | 일반 파일의 바이트 수 |
| `fs::create_directory(p)`, `fs::create_directories(p)` | 디렉터리 하나를, 중간 디렉터리까지 모두 만든다 |
| `fs::copy_file(from, to)`, `fs::rename(from, to)` | 파일을 복사한다, 이름을 바꾸거나 옮긴다 |
| `fs::remove(p)`, `fs::remove_all(p)` | 파일이나 빈 디렉터리 하나를, 디렉터리와 그 안의 모든 것을 지운다 |
| `fs::directory_iterator(p)`, `fs::recursive_directory_iterator(p)` | 디렉터리의 항목을, 하위 디렉터리까지 차례로 순회한다 |

*표 12. 자주 쓰는 `std::filesystem` 함수*

> **[운영체제]** 운영체제에서 배우는 파일 시스템은 파일을 디렉터리의 트리로 조직하고, 파일마다 크기, 종류, 권한, 시각 같은 메타데이터를 관리한다. `std::filesystem`의 함수는 이 메타데이터를 조회하거나 디렉터리 트리를 바꾸는 운영체제 호출을 감싼 것이며, 파일 스트림은 그 파일의 내용을 다룬다.

[Example_Filesystem.cpp](Example_Filesystem.cpp)는 디렉터리를 만들고, 파일 두 개를 쓰고, 트리를 순회하며 크기를 출력한 뒤 모두 지운다.

```cpp
fs::path dir = "fs_demo";
fs::create_directories(dir / "logs");        // 두 단계를 모두 만든다
{ ofstream(dir / "a.txt") << "alpha\n"; }    // 임시 ofstream은 문장 끝에서 닫힌다
{ ofstream(dir / "logs" / "b.log") << "beta beta\n"; }

for (const auto& entry : fs::recursive_directory_iterator(dir)) {
    cout << entry.path().generic_string();
    if (entry.is_regular_file())
        cout << "  " << entry.file_size() << " bytes";
    cout << endl;
}

error_code ec;
auto size = fs::file_size(dir / "none.txt", ec);   // error_code를 받는 형태는 예외를 던지지 않는다
cout << "missing file: " << ec.message() << " (" << (size == static_cast<uintmax_t>(-1)) << ")" << endl;

cout << "removed " << fs::remove_all(dir) << " entries" << endl;
```

```text
fs_demo/logs
fs_demo/logs/b.log  10 bytes
fs_demo/a.txt  6 bytes
parent=fs_demo/logs stem=b extension=.log
missing file: No such file or directory (1)
removed 4 entries
exists after remove_all: 0
```

- `a.txt`는 `alpha`와 개행으로 6바이트, `b.log`는 `beta beta`와 개행으로 10바이트이다.
- 순회 순서는 표준이 정하지 않으므로[3] 운영체제와 파일 시스템에 따라 다르다. 정렬된 목록이 필요하면 경로를 `vector`에 모아 정렬한다.
- 대부분의 함수는 실패하면 `fs::filesystem_error` 예외를 던지는 형태와, 마지막 인자로 `error_code&`를 받아 오류를 그곳에 기록하는 형태를 모두 가진다. 오류가 예상되는 곳(없을 수 있는 파일의 크기 조회)에서는 `error_code` 형태를 쓴다. 이때 `file_size()`는 `static_cast<uintmax_t>(-1)`을 리턴한다[3].
- `remove_all()`은 지운 항목의 수를 리턴한다. 디렉터리 `fs_demo`, `logs`와 파일 두 개로 4이다.

---

<br>

## 개념 적용

**T/F 점검:**

| 문장 | 판단과 근거 |
|:-----|:-----|
| macOS에서 `ios::binary`를 빼고 연 파일에 `'\n'`을 쓰면 `0D 0A`가 기록된다. | **F.** POSIX에서는 텍스트 모드와 바이너리 모드가 같으므로 `0A` 하나가 기록된다. 변환은 Windows의 텍스트 모드에서 일어난다(1.2절). |
| `ofstream f("a.txt", ios::out \| ios::ate);`는 기존 내용 뒤에 이어 쓴다. | **F.** `out \| ate`는 `"w"`와 같으므로 내용을 지운다. `app`이나 `in \| out \| ate`를 쓴다(4.2절). |
| `fstream f("new.txt", ios::in \| ios::out);`는 파일이 없으면 새로 만든다. | **F.** `"r+"`와 같으므로 파일이 없으면 열기에 실패한다(4.2절). |
| `while (!fin.eof())` 반복은 파일의 마지막 값을 두 번 처리할 수 있다. | **T.** `eof()`는 읽기가 끝에 도달한 뒤에야 참이 되므로 실패한 읽기 뒤에도 본문이 한 번 더 실행된다(7.2절). |
| `read()`가 실패하면 버퍼에 저장된 데이터는 없다. | **F.** 파일의 끝에서 일부만 읽어도 실패로 처리되며, 읽은 바이트 수는 `gcount()`로 알 수 있다(6.1절). |
| `std::string` 멤버를 가진 구조체도 `write()`로 저장하고 `read()`로 복원할 수 있다. | **F.** `string`은 힙의 주소를 담고 있으므로 바이트를 저장해도 내용이 저장되지 않는다(6.3절). |
| `fstream`에서 `seekg()`로 위치를 옮기면 `tellp()`도 바뀐다. | **T.** 파일 스트림의 버퍼는 파일 위치를 하나만 가진다(8.1절). |
| `recursive_directory_iterator`는 항목을 이름순으로 순회한다. | **F.** 순회 순서는 정해져 있지 않다(9.2절). |

---

<br>

## 용어 사전

| 영문 | 한국어 | 한 줄 정의 |
|:-----|:-----|:-----|
| text mode / binary mode | 텍스트 모드 / 바이너리 모드 | 줄 끝 변환을 할 수 있는 열기 방식과 바이트를 그대로 다루는 열기 방식 |
| line ending (LF, CR LF) | 줄 끝 표기 | 운영체제가 텍스트 파일의 줄 끝을 나타내는 바이트 (Unix는 `0A`, Windows는 `0D 0A`) |
| working directory | 작업 디렉터리 | 경로 없는 파일 이름을 찾는 기준 디렉터리 |
| open mode | 파일 모드 | 파일을 여는 방식을 정하는 `ios::in`, `out`, `app`, `ate`, `trunc`, `binary`의 조합 |
| truncate | 내용 지우기 | 파일을 열 때 기존 내용을 없애고 길이를 0으로 만드는 일 |
| padding | 패딩 | 정렬 조건을 맞추려고 컴파일러가 구조체 멤버 사이나 끝에 넣는 바이트 |
| trivially copyable type | 트리비얼하게 복사 가능한 타입 | 바이트 복사만으로 값이 온전히 복사되는 타입 |
| serialization | 직렬화 | 데이터를 정해진 크기와 바이트 순서의 바이트열로 바꾸어 저장하거나 전송하는 일 |
| sequential access | 순차 접근 | 파일을 처음부터 차례로 읽거나 쓰는 방식 |
| file position | 파일 위치 | 다음에 읽거나 쓸 바이트의 위치를 처음부터 센 값 |
| seek direction | 위치 기준 | 위치 이동의 기준점 `ios::beg`, `ios::cur`, `ios::end` |
| path | 경로 | 파일 시스템에서 파일이나 디렉터리의 위치를 나타내는 이름 |

*표 13. 용어 사전*

---

<br>

## 요약

| 개념 | 핵심 요약 |
|:-----|:-----|
| 텍스트 파일과 바이너리 파일 | 모든 파일은 바이트의 나열이며, 문자 코드와 줄로 해석하면 텍스트 파일, 프로그램이 정한 형식이면 바이너리 파일이다. |
| 텍스트 모드 | Windows에서는 `LF`와 `CR LF`를 변환하고 `Ctrl+Z`를 끝으로 보며, POSIX에서는 바이너리 모드와 같다. |
| 파일 스트림 클래스 | `ifstream`, `ofstream`, `fstream`은 각각 `istream`, `ostream`, `iostream`을 상속하며, 앞의 둘은 `in`과 `out`을 항상 더해서 연다. |
| 열기와 닫기 | 생성자나 `open()`으로 열고 `if (!f)`로 검사하며, 스트림이 소멸할 때 파일이 닫히므로 블록을 벗어나면 닫힌다. |
| 작업 디렉터리 | 상대 경로는 소스 파일이 아니라 프로그램을 실행한 디렉터리를 기준으로 해석된다. |
| 열기 실패의 원인 | 스트림은 실패 사실만 알리므로 원인은 `std::filesystem`이나 POSIX의 `errno`로 알아내며, 열기에 성공해도 읽기마다 결과를 검사해야 한다. |
| `<<`와 `>>` | 값을 텍스트로 쓰고 읽으며, 쓸 때는 공백이나 개행으로 구분해야 다시 읽을 수 있다. |
| 파일 모드 | `out`은 내용을 지우고, `app`은 끝에 이어 쓰고, `in \| out`은 내용을 유지하되 파일이 없으면 실패하며, 표에 없는 조합은 열기에 실패한다. |
| `get()`, `put()`, `getline()` | 공백을 포함한 문자 단위 복사와 줄 단위 처리에 쓰며, `getline()` 반복은 마지막 줄의 개행 유무와 관계없이 줄을 정확히 센다. |
| 다른 운영체제의 파일 | Windows에서 만든 파일을 POSIX에서 읽으면 줄 끝에 `'\r'`이 남으므로 지워야 한다. |
| `read()`, `write()` | 바이트를 그대로 읽고 쓰며, `read()`가 실패해도 `gcount()`만큼은 읽은 것이다. |
| 구조체 레코드 | 패딩 때문에 `sizeof`가 멤버 크기의 합보다 클 수 있고, 트리비얼하게 복사 가능한 타입만 바이트로 저장하며, 다른 환경에서는 직렬화가 필요하다. |
| `eof()` 반복 오류 | `eof()`는 끝에 도달한 뒤에 참이 되므로 반복 조건으로 쓰면 마지막 값을 두 번 처리하며, 읽기 연산을 조건으로 써야 한다. |
| 반복이 끝난 원인 | `bad()`, `eof()` 순서로 검사하고 둘 다 아니면 형식 오류이다. |
| `exceptions()` | 지정한 상태 비트가 켜지면 `ios_base::failure`를 던지게 하며, 반복으로 읽는 스트림에는 `badbit`만 지정한다. |
| 임의 접근 | `seekg`, `seekp`로 `beg`, `cur`, `end` 기준의 위치로 옮기고 `tellg`, `tellp`로 조회하며, 파일 스트림의 읽기 위치와 쓰기 위치는 하나이다. |
| 레코드 위치 | 고정 크기 레코드 `i`는 `i × sizeof(Record)`에서 시작하고, 제자리 수정은 `in \| out \| binary`로 연다. |
| `std::filesystem` | 경로 조작, 존재 확인, 크기 조회, 디렉터리 생성과 삭제, 순회를 운영체제와 무관하게 제공하며, 순회 순서는 정해져 있지 않다. |

---

<br>

## 점검 문제

1. **바이트 수:** Windows에서 텍스트 모드로 `fout << "A\nB\n";`을 쓴 파일과 바이너리 모드로 쓴 파일의 크기를 각각 구하라. macOS에서는 어떤가?

   <details>
   <summary>정답</summary>

   Windows의 텍스트 모드에서는 `'\n'` 두 개가 각각 `0D 0A`가 되어 `41 0D 0A 42 0D 0A`의 6바이트이고, 바이너리 모드에서는 `41 0A 42 0A`의 4바이트이다. macOS에서는 두 모드가 같으므로 둘 다 4바이트이다.

   </details>

2. **모드 선택:** 다음 목적에 맞는 파일 모드를 쓰라. (1) 실행할 때마다 로그를 끝에 덧붙인다. (2) 기존 바이너리 파일의 레코드 하나를 제자리에서 고친다. (3) 결과 파일을 매번 새로 만든다.

   <details>
   <summary>정답</summary>

   (1) `ofstream f("log.txt", ios::app);`이다. (2) `fstream f("data.bin", ios::in | ios::out | ios::binary);`이며, `out | binary`만 주면 내용이 지워진다. (3) `ofstream f("result.txt");`로, 기본 모드 `out`이 기존 내용을 지운다.

   </details>

3. **반복 조건 추적:** 파일의 내용이 `5 7`(마지막에 개행 없음)일 때 `while (!fin.eof()) { fin >> x; sum += x; }`의 `sum`을 구하고, 내용이 `5 7\n`이면 어떻게 달라지는지 설명하라. `sum`과 `x`는 0으로 초기화했다.

   <details>
   <summary>정답</summary>

   `5 7`이면 첫 반복에서 5를 읽고, 둘째 반복에서 7을 읽으며 끝에 도달해 `eofbit`가 켜지므로 반복이 끝난다. `sum`은 12로 우연히 맞다. `5 7\n`이면 둘째 반복이 7을 읽은 뒤에도 `'\n'`이 남아 `eof()`가 거짓이므로 셋째 반복이 실행되고, 실패한 읽기는 `x`를 바꾸지 않으므로 7이 한 번 더 더해져 `sum`은 19가 된다. 결과가 파일의 마지막 개행에 따라 달라지므로 `while (fin >> x)`를 써야 한다.

   </details>

4. **블록 복사:** 버퍼 크기가 1,000바이트일 때 2,500바이트 파일을 6.2절의 코드로 복사하면 `read()`는 몇 번 호출되고, 각 호출 뒤 `gcount()`는 얼마인가?

   <details>
   <summary>정답</summary>

   네 번 호출된다. `gcount()`는 1,000, 1,000, 500, 0이다. 세 번째 호출은 500바이트만 채워 실패하지만 `gcount() > 0`이므로 500바이트를 쓰고, 네 번째 호출은 0바이트를 읽어 조건 전체가 거짓이 되므로 반복이 끝난다.

   </details>

5. **레코드 위치:** `struct Item { char code; int qty; double price; };`의 `sizeof`가 16인 환경에서 인덱스 5인 레코드의 시작 위치를 구하고, 멤버 크기의 합(1 + 4 + 8 = 13)과 16이 다른 이유를 설명하라.

   <details>
   <summary>정답</summary>

   5 × 16 = 80바이트이다. `int`는 4의 배수, `double`은 8의 배수 주소에 놓여야 하므로 `code` 뒤에 3바이트의 패딩이 들어가 `qty`가 오프셋 4, `price`가 오프셋 8에 놓이고 전체가 16바이트가 된다.

   </details>

6. **문자열 멤버:** `struct Person { std::string name; int age; };`를 `write(reinterpret_cast<const char*>(&p), sizeof p)`로 저장한 뒤 다른 실행에서 `read()`로 읽으면 어떤 문제가 생기는가? 올바른 저장 방법을 쓰라.

   <details>
   <summary>정답</summary>

   `std::string`은 짧은 문자열이 아니면 문자를 힙에 두고 그 주소를 객체 안에 담는다. 저장된 바이트에는 이전 실행의 힙 주소가 들어 있으므로, 읽은 객체는 존재하지 않는 메모리를 가리키게 되고 사용하거나 소멸시키면 정의되지 않은 동작이 된다. `std::is_trivially_copyable<Person>::value`도 거짓이다. 이름의 길이를 정수로 쓰고 그 길이만큼 문자를 쓴 뒤 `age`를 쓰는 식으로 멤버를 하나씩 저장한다.

   </details>

7. **원인 구별:** `while (fin >> x)`가 끝난 뒤 `fin.eof()`가 거짓이고 `fin.bad()`도 거짓이었다. 반복이 끝난 원인은 무엇이며, 그 지점 이후의 데이터를 계속 읽으려면 어떻게 하는가?

   <details>
   <summary>정답</summary>

   `failbit`만 켜진 상태이므로 파일 중간에 정수로 해석할 수 없는 데이터가 있어 멈춘 것이다. `fin.clear()`로 상태를 지운 뒤 `fin.ignore()`나 `fin >> word`로 잘못된 토큰을 버리면 다시 읽을 수 있다(11장 4.3절).

   </details>

8. **파일 크기:** `ifstream f("a.txt"); f.seekg(0, ios::end);` 뒤의 `tellg()`가 Windows의 텍스트 모드에서 파일의 문자 수와 다를 수 있는 이유와, 이식성 있게 파일 크기를 구하는 방법을 쓰라.

   <details>
   <summary>정답</summary>

   Windows의 텍스트 모드는 줄 끝 `0D 0A`를 `'\n'` 하나로 읽으므로 파일의 바이트 수와 프로그램이 읽는 문자 수가 다르고, C 표준은 텍스트 스트림의 위치 값을 바이트 수로 해석하는 것을 보장하지 않는다. 바이트 수가 필요하면 `std::filesystem::file_size("a.txt")`를 쓰거나 바이너리 모드로 열어 위치를 조회한다.

   </details>

---

<br>

## 참고 문헌

1. Microsoft, "fopen, _wfopen," *Microsoft Learn*. https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen
2. IEEE and The Open Group, "fopen," in *The Open Group Base Specifications Issue 8*, IEEE Std 1003.1-2024. https://pubs.opengroup.org/onlinepubs/9799919799/functions/fopen.html
3. ISO/IEC JTC 1/SC 22/WG 21, "Working Draft, Standard for Programming Language C++," Document N4659, Mar. 2017. https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
4. cppreference.com, "std::ios_base::openmode." https://en.cppreference.com/w/cpp/io/ios_base/openmode
5. ISO/IEC JTC 1/SC 22/WG 14, "Programming languages: C," Committee Draft N1570, Apr. 2011. https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
