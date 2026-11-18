# Chapter 12 — C++ File I/O

[Repository Guide](../../README.md) | [English](Concepts_Lecture.md) | [한국어](Concepts_Lecture.ko.md)

> **Last Updated:** 2026-11-19

> **Learning Objectives:**
> 1. Explain the difference between text files and binary files and the newline translation of text mode
> 2. Open and close files with `ifstream`, `ofstream`, and `fstream`, and check for open failures
> 3. Read and write text files with `<<`, `>>`, `get()`, `put()`, and `getline()`
> 4. Combine file modes to distinguish overwriting, appending, and opening for both reading and writing
> 5. Access binary records randomly with `read()`, `write()` and `seekg()`, `seekp()`, `tellg()`, `tellp()`
> 6. Distinguish the end of file input from errors through the stream state, and work with files and directories through `std::filesystem`

---

## Table of Contents

- [1. Text Files and Binary Files](#1-text-files-and-binary-files)
  - [1.1 Files as Sequences of Bytes](#11-files-as-sequences-of-bytes)
  - [1.2 Text Mode and Binary Mode](#12-text-mode-and-binary-mode)
- [2. Overview of File I/O](#2-overview-of-file-io)
  - [2.1 File Stream Classes](#21-file-stream-classes)
  - [2.2 Opening and Closing Files](#22-opening-and-closing-files)
  - [2.3 File Paths](#23-file-paths)
  - [2.4 Causes of Open Failure](#24-causes-of-open-failure)
- [3. Text File I/O with the `<<` and `>>` Operators](#3-text-file-io-with-the--and--operators)
  - [3.1 Writing to a File](#31-writing-to-a-file)
  - [3.2 Reading from a File](#32-reading-from-a-file)
  - [3.3 Saving and Restoring Objects](#33-saving-and-restoring-objects)
- [4. File Modes](#4-file-modes)
  - [4.1 Mode Flags](#41-mode-flags)
  - [4.2 Flag Combinations and C `fopen` Modes](#42-flag-combinations-and-c-fopen-modes)
  - [4.3 Differences in Results by Mode](#43-differences-in-results-by-mode)
- [5. Text I/O with Member Functions](#5-text-io-with-member-functions)
  - [5.1 Copying Character by Character with `get()` and `put()`](#51-copying-character-by-character-with-get-and-put)
  - [5.2 Reading Line by Line with `getline()`](#52-reading-line-by-line-with-getline)
  - [5.3 Text Files Made on Other Operating Systems](#53-text-files-made-on-other-operating-systems)
  - [5.4 Counting Lines, Words, and Characters](#54-counting-lines-words-and-characters)
- [6. Binary I/O](#6-binary-io)
  - [6.1 `read()` and `write()`](#61-read-and-write)
  - [6.2 Block Copying](#62-block-copying)
  - [6.3 Storing Structure Records](#63-storing-structure-records)
- [7. Checking the Stream State](#7-checking-the-stream-state)
  - [7.1 Points to Check](#71-points-to-check)
  - [7.2 The Error of Using `eof()` as the Loop Condition](#72-the-error-of-using-eof-as-the-loop-condition)
  - [7.3 Telling Why a Loop Ended](#73-telling-why-a-loop-ended)
  - [7.4 Reporting Failure with Exceptions](#74-reporting-failure-with-exceptions)
- [8. Random Access](#8-random-access)
  - [8.1 File Position](#81-file-position)
  - [8.2 `seekg()`, `seekp()`, `tellg()`, `tellp()`](#82-seekg-seekp-tellg-tellp)
  - [8.3 Random Access by Record](#83-random-access-by-record)
  - [8.4 Positioning in Text Mode](#84-positioning-in-text-mode)
- [9. The Filesystem Library](#9-the-filesystem-library)
  - [9.1 Working with Paths](#91-working-with-paths)
  - [9.2 Operating on Files and Directories](#92-operating-on-files-and-directories)
- [Concept Applications](#concept-applications)
- [Glossary](#glossary)
- [Summary](#summary)
- [Self-Check Questions](#self-check-questions)
- [References](#references)

---

<br>

## 1. Text Files and Binary Files

### 1.1 Files as Sequences of Bytes

A file stored on disk, whatever its kind, is a sequence of bytes. The operating system gives no meaning to the bytes of a file; how to interpret them is decided by the program that reads the file. Dividing files into text files and binary files is a distinction by exactly this interpretation.

> **Definition:** A **text file** is a file whose bytes are all character codes (ASCII, UTF-8, and so on) and which is interpreted as a sequence of lines separated by newline characters.

> **Definition:** A **binary file** is a file whose bytes have meanings defined by the program that created it; images, executables, compressed files, and record files in which a program stored its in-memory data as it is belong here.

The text file `"Hi\nC++\n"` consists of 7 bytes of character codes. The newline character is also a single byte, with code value `0x0A`.

| Position | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|:-----|:-----|:-----|:-----|:-----|:-----|:-----|:-----|
| Character | `H` | `i` | `\n` | `C` | `+` | `+` | `\n` |
| Byte (hexadecimal) | `48` | `69` | `0A` | `43` | `2B` | `2B` | `0A` |

*Table 1. The bytes of the text `"Hi\nC++\n"`*

Even the same integer 1000 has different bytes depending on how it is stored. Stored as text, it becomes the codes `31 30 30 30` of the characters `'1'`, `'0'`, `'0'`, `'0'`, using as many bytes as there are digits, and a person can read it in an editor. Stored as binary, the in-memory `int` value 1000 (hexadecimal `0x000003E8`) is written as it is, so it always takes 4 bytes on an environment where `int` is 4 bytes, and on environments that put the low-order byte first, such as x86-64 and arm64, it becomes `E8 03 00 00`. Binary storage is fast because its size is fixed and there is no conversion cost, but it cannot be read in an editor and may not be interpreted as the same value on another environment (Section 6.3).

### 1.2 Text Mode and Binary Mode

Operating systems mark the end of a line differently. Unix-like systems (Linux, macOS) end a line with the single newline character `LF` (`0x0A`), and Windows ends it with the two bytes carriage return and line feed, `CR LF` (`0x0D 0x0A`). Inside a C++ program, the end of a line is handled as the single `'\n'` on every operating system, and the **mode** in which a file is opened bridges this difference.

- A file opened in **text mode** (the default) may translate between the operating system's line-ending convention and `'\n'`. In text mode, the Windows C runtime turns `LF` into `CR LF` on writing and `CR LF` into `LF` on reading, and treats `Ctrl+Z` (`0x1A`) met while reading as the end of the file[1].
- A file opened in **binary mode** (`ios::binary`) reads and writes bytes as they are, without translation.
- On POSIX systems, the two modes are the same. POSIX specifies that the `b` in an `fopen` mode has no effect[2].

[Example_HexDump.cpp](Example_HexDump.cpp) writes the same string in text mode and in binary mode, then reopens both files in binary mode and prints their bytes. The result on macOS is as follows.

```text
text.txt: 48 69 0a 43 2b 2b 0a  (7 bytes)
binary.txt: 48 69 0a 43 2b 2b 0a  (7 bytes)
```

On macOS the two files are the same. By the text-mode rules of Windows[1], when the same program runs on Windows, `text.txt`, written in text mode, gains a `0D` before every newline and becomes the 9 bytes `48 69 0D 0A 43 2B 2B 0D 0A`. This is also why the `dump` function, which prints the bytes, opens the file in binary mode: if it opened the file in text mode, on Windows `0D 0A` would be read as a single `0A` and the stored bytes could not be seen as they are.

Reading and writing binary data in text mode changes the data on Windows as follows.

| Byte in the data | Written in text mode | Read in text mode |
|:-----|:-----|:-----|
| `0A` | Becomes `0D 0A` (one byte longer) | Unchanged |
| `0D 0A` | Becomes `0D 0D 0A` | Becomes a single `0A` (one byte shorter) |
| `1A` | Unchanged | The file is treated as ending at that position |

*Table 2. Effect of Windows text mode on binary data*

> **Key Point:** Data that does not consist of characters and lines is opened with `ios::binary` on every operating system. On POSIX it has no effect, but it prevents the data from changing when the same code runs on Windows.

---

<br>

## 2. Overview of File I/O

### 2.1 File Stream Classes

`<fstream>` provides three stream classes connected to files. As in Figure 2 of Chapter 11, `ifstream` inherits `istream`, `ofstream` inherits `ostream`, and `fstream` inherits `iostream`, so `>>`, `<<`, `get()`, `getline()`, the manipulators, and the state bits covered in Chapter 11 are used on files as they are.

| Class | Inherits | Use | Mode always added when opening |
|:-----|:-----|:-----|:-----|
| `ifstream` | `istream` | Reading from a file | `ios::in` |
| `ofstream` | `ostream` | Writing to a file | `ios::out` |
| `fstream` | `iostream` | Reading and writing one file | None (`ios::in \| ios::out` if none is given) |

*Table 3. File stream classes*

`ifstream` adds `ios::in` to whatever mode it is given, and `ofstream` adds `ios::out`[3]. So `ofstream f("log.txt", ios::app)` is opened with `ios::out | ios::app`. `fstream` uses only the mode it is given, so reading and writing are specified explicitly as needed.

File streams are used in the following order.

1. Open the file while creating the file stream object, or with `open()` after creating it.
2. Check that the file was opened.
3. Read or write with `<<`, `>>`, and member functions.
4. Close it with `close()`, or let it close automatically when the object is destroyed.

### 2.2 Opening and Closing Files

There are two ways to open a file.

```cpp
ofstream fout("scores.txt");          // 1. open in the constructor
ifstream fin;
fin.open("scores.txt");               // 2. create an empty stream, then open() it
```

**Checking the open.** If the file does not exist, there is no permission, or the path is wrong, opening fails and `failbit` is set on the stream[3]. I/O on a failed stream does nothing, so without a check right after opening, the program produces empty results without any error. The check is `if (!fin)` or `if (!fin.is_open())`. `!fin` looks at the state bits, and `is_open()` looks at whether a file is attached.

```cpp
ifstream fin("scores.txt");
if (!fin) {                           // failbit is set when opening fails
    cerr << "cannot open scores.txt" << endl;
    return 1;
}
```

**Closing.** `close()` flushes the output buffer and closes the file. When a file stream is destroyed, the destructor of its file buffer (`filebuf`) calls `close()`[3], so a file stream that is a local variable closes automatically when control leaves its block. This way of tying a resource to the lifetime of an object is RAII from Chapter 13. `close()` must be called explicitly in the following cases.

- When a file just written is reopened for reading in the same function. Without closing, the buffered contents may not yet be in the file.
- When the same stream object opens another file. Calling `open()` on a stream that is already open fails[3].
- When the file must not stay open for long (for example, when another program needs to use it).

> **Note:** **State when reopening.** Since C++11, the `open()` of a file stream calls `clear()` when opening succeeds[3]. Under earlier standards, closing a stream that had read to the end with `eofbit` set and opening another file left the state as it was, so the first read failed; older code therefore has a line that calls `clear()` explicitly after `open()`.

### 2.3 File Paths

If a file name has no path, the file is looked up in the program's **working directory**. The working directory is not the location of the source file or the executable but the location from which the program was run. In a terminal it is the current directory, and in Visual Studio or Xcode it is the directory set in the project settings. It is the first thing to check when a program cannot find a file in an IDE. The example programs of this chapter also create files under names without a path, so running them creates files such as `scores.txt` and `records.bin` in the working directory.

The `\` of a Windows path is an escape character in a C++ string, so it is written twice, as in `"C:\\data\\scores.txt"`, or a raw string literal `R"(C:\data\scores.txt)"` is used. Windows file APIs also accept `/` as a path separator, so `"C:/data/scores.txt"` can be used too. Since C++17, the constructors and `open()` of the file streams also accept the `std::filesystem::path` of Section 9[3].

### 2.4 Causes of Open Failure

A file stream reports only the fact that opening failed, through `failbit`, and not its cause. To report the cause to the user, it must be found out by other means. [Example_OpenFailure.cpp](Example_OpenFailure.cpp) uses two methods together.

```cpp
errno = 0;
ifstream fin(p);
if (fin) {
    fin.get();                               // opening is not the same as being readable
    cout << p.string() << ": opened, first read "
         << (fin ? "succeeded" : "failed") << " (bad=" << fin.bad() << ")" << endl;
    return;
}
int saved = errno;                           // POSIX open() sets errno; ISO C++ does not promise it
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

- **`errno`.** On POSIX systems, the system call that opens a file leaves the number of the cause in `errno` when it fails, and `strerror()` turns that number into a sentence. ISO C++ and ISO C do not guarantee that opening a file sets `errno`, so this method is used only to show diagnostic messages on POSIX environments.
- **`std::filesystem`.** The functions of Section 9 can check directly whether a file exists and whether there is permission. This method works independently of the operating system.
- **Directories.** On macOS, opening the directory `folder` with `ifstream` was reported as a success and the first read failed. A successful open does not guarantee a successful read, so the result of each read operation must also be checked (Section 7). What happens when a directory is opened differs between operating systems.

---

<br>

## 3. Text File I/O with the `<<` and `>>` Operators

### 3.1 Writing to a File

Using `<<` on an `ofstream` records the same text in the file as output to `cout` would produce. [Example_ScoreFile.cpp](Example_ScoreFile.cpp) writes the names and scores of three students, one per line, and then reads them back.

```cpp
ofstream fout("scores.txt");                 // creates the file, or empties an existing one
if (!fout) {
    cerr << "cannot create scores.txt" << endl;
    return 1;
}
fout << "Kim 90\n" << "Lee 85\n" << "Park 77\n";
fout.close();                                // flushes the buffer and closes the file
```

Values written as text must be separated by spaces or newlines so that `>>` can read them back. Writing them without separation, as in `fout << 90 << 85`, records `9085` in the file, which is read as a single integer. Likewise, a name containing a space, such as `Kim Minsu`, written with `<<` is read by `>>` as two words, so such values are stored one per line and read with `getline()` (Section 5.2).

### 3.2 Reading from a File

```cpp
ifstream fin("scores.txt");
if (!fin.is_open()) {
    cerr << "cannot open scores.txt" << endl;
    return 1;
}
string name;
int score, total = 0, count = 0;
while (fin >> name >> score) {               // the loop ends when a read fails
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

Tracing how the loop condition `fin >> name >> score` is judged gives the following.

| Iteration | `name` read | `score` read | Condition | State |
|:-----|:-----|:-----|:-----|:-----|
| 1 | `Kim` | 90 | True | Normal |
| 2 | `Lee` | 85 | True | Normal |
| 3 | `Park` | 77 | True | Normal (stops before the `'\n'` after `77`) |
| 4 | None | None | False | Reached the end while skipping `'\n'`, `eofbit` and `failbit` |

*Table 4. Trace of `while (fin >> name >> score)`*

The third iteration reads `77` and stops before the `'\n'` at the end of the line, so the end has not yet been reached. On the fourth attempt, `>> name` meets the end of the file while skipping whitespace and reads nothing, so `eofbit` and `failbit` are set together, the condition becomes false, and the loop ends. The state after the loop, `eof=1, fail=1`, means "it stopped because it read to the end" (Section 7.3).

### 3.3 Saving and Restoring Objects

The insertion and extraction operators of Chapter 11, Section 6 take `ostream&` and `istream&` parameters, so the same functions work on file streams as they are. [Example_PointFile.cpp](Example_PointFile.cpp) saves a list of points to a file and reads it back.

```cpp
ostream& operator<<(ostream& out, const Point& p) {     // works for cout and for ofstream
    return out << p.x << ' ' << p.y;
}

istream& operator>>(istream& in, Point& p) {            // works for cin and for ifstream
    Point t;
    if (in >> t.x >> t.y)
        p = t;
    return in;
}

ofstream fout("shape.txt");
fout << shape.size() << '\n';                // a count first, then one point per line
for (const Point& p : shape)
    fout << p << '\n';
```

```text
3
0 0
4 0
4 3
```

The saved `shape.txt` is shown above, and the program reads this file and outputs `loaded 3 of 3 points: (0 0) (4 0) (4 3)`. The reading side reads the count `n` on the first line and then reads at most `n` points with `for (size_t i = 0; i < n && fin >> p; i++)`. Because `fin >> p` is part of the condition, the loop ends early if the file was cut off in the middle and points are missing, and comparing the number read with `n` reveals the damage. When choosing a storage format, record a count or separators like this so that the reading side can determine the end of the data and detect damage.

---

<br>

## 4. File Modes

### 4.1 Mode Flags

How a file is opened is decided by combining flags of type `ios_base::openmode` with `|`.

| Flag | Meaning |
|:-----|:-----|
| `ios::in` | Open for reading |
| `ios::out` | Open for writing |
| `ios::app` | Move to the end of the file before every write (append) |
| `ios::ate` | Move the position to the end of the file right after opening (at end) |
| `ios::trunc` | Erase the existing contents when opening (truncate) |
| `ios::binary` | Open in binary mode (Section 1.2) |
| `ios::noreplace` (C++23) | Fail to open if the file already exists (exclusive creation) |

*Table 5. File mode flags*

The first six flags are defined in the C++17 standard[3], and `noreplace` was added in C++23[4]. `app` and `ate` have similar names but differ. `app` performs every write at the end of the file, so even if the position is moved with `seekp()`, writes still go to the end. `ate` only moves the initial position to the end, so afterward the program can move to any position to read or write.

### 4.2 Flag Combinations and C `fopen` Modes

Internally, a file stream opens a file the same way as C's `fopen`. For each flag combination (excluding `ate`), the standard specifies the equivalent `fopen` mode string, and it specifies that **opening fails for a combination not in the table**[3].

| Flag combination (excluding `binary`) | `fopen` mode | If the file exists | If the file does not exist |
|:-----|:-----|:-----|:-----|
| `out`, `out \| trunc` | `"w"` | Erases the contents and writes | Creates it |
| `out \| app`, `app` | `"a"` | Appends at the end | Creates it |
| `in` | `"r"` | Reads from the beginning | Open fails |
| `in \| out` | `"r+"` | Keeps the contents, reads and writes | Open fails |
| `in \| out \| trunc` | `"w+"` | Erases the contents, reads and writes | Creates it |
| `in \| out \| app`, `in \| app` | `"a+"` | Reads; writes append at the end | Creates it |

*Table 6. File mode combinations and the equivalent `fopen` modes*

Adding `binary` to each combination adds `b` to the `fopen` mode (`"wb"`, `"r+b"`, and so on). Combinations not in the table, such as `in | trunc`, `trunc` alone, and `out | app | trunc`, fail to open, and when run, `is_open()` is false in all three cases. The table yields the following facts.

- Giving only `out` erases the existing contents even without `trunc`. This is why the default behavior of `ofstream` is overwriting.
- To modify a file without erasing its contents, open it with `in | out`. This combination equals `"r+"`, so it fails if the file does not exist.
- `ate` is a position option added to a combination in the table. `out | ate` still equals `"w"`, so the contents are erased. To start at the end of the existing contents, use `in | out | ate` or `app`.

### 4.3 Differences in Results by Mode

[Example_OpenModes.cpp](Example_OpenModes.cpp) opens the same file `mode.txt` with different modes and, each time, prints the lines of the file joined with `/`.

```cpp
{ ofstream f("mode.txt"); f << "one\ntwo\n"; }             // out: truncates, then writes
show("out        ");
{ ofstream f("mode.txt", ios::app); f << "three\n"; }      // app: every write goes to the end
show("app        ");
{ fstream f("mode.txt", ios::in | ios::out); f << "ONE"; } // in | out: keeps the contents
show("in | out   ");
{ ofstream f("mode.txt", ios::out | ios::ate); f << "X\n"; } // out | ate: still truncates
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

At the end of each brace block the stream is destroyed and the file is closed, so the `show()` on the next line reads a file whose writing is finished. In the third result, the position of the file opened with `in | out` is at the beginning, so `ONE` overwrites `one` and the rest of the contents remain. The fourth result shows that `out` erases the contents even when `ate` is given. The last line shows that `in | out`, which equals `"r+"`, does not create a missing file.

> **Caution:** Writing `ofstream f("log.txt");` with the intent of appending to an existing file erases the previous records on every run. To append, specify `ios::app`.

---

<br>

## 5. Text I/O with Member Functions

### 5.1 Copying Character by Character with `get()` and `put()`

`>>` skips whitespace, so it cannot be used to copy a file as it is. To move every character including spaces and newlines, use the unformatted functions `get()` and `put()` from Chapter 11. The first part of [Example_LineNumbers.cpp](Example_LineNumbers.cpp) is as follows.

```cpp
ifstream fin(src);
ofstream fout("copy.txt");
if (!fin || !fout) {
    cerr << "cannot open the files" << endl;
    return 1;
}
int ch, chars = 0;
while ((ch = fin.get()) != EOF) {            // get() returns int so that EOF stays distinct
    fout.put(static_cast<char>(ch));
    chars++;
}
```

The input file is `"roses are red\n\nviolets are blue"`. The first line has 13 characters, there are 2 newlines, and the third line has 16 characters, so 13 + 2 + 16 = 31 characters are copied, and the program prints `copied 31 characters`. The last line has no newline, but `get()` reads characters regardless of lines, so it is copied as it is.

### 5.2 Reading Line by Line with `getline()`

Text that must be processed line by line (configuration files, logs, CSV) is read with the global function `getline(fin, line)`. The second part of the same example numbers the lines of the copied file and finds the length of the longest line.

```cpp
ifstream again("copy.txt");
string line;
int number = 0;
size_t longest = 0;
while (getline(again, line)) {               // the last line counts even without '\n'
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

- The second line is empty. `getline()` meets `'\n'` at once and stores an empty string, but because it took out the delimiter, it is not a failure, and the line is counted.
- The third line, `violets are blue`, stops at the end of the file without a newline. `eofbit` is set, but characters were read, so the condition is true and the line is counted.
- On the fourth attempt there are no characters to read, so `failbit` is set and the loop ends.

So `while (getline(fin, line))` counts lines correctly whether or not the last line has a newline. The array-based member function `fin.getline(buf, n)`, which limits the line length, sets `failbit` on long lines (Chapter 11, Section 3.3), so the global function with a `string` suits files whose line lengths are unknown.

### 5.3 Text Files Made on Other Operating Systems

The translation of text mode handles only the conventions of the operating system on which the program runs. When a file with `CR LF` line endings made on Windows is read on macOS or Linux, no translation occurs, so `getline()` stops before `'\n'` and `'\r'` (`0x0D`) remains at the end of the string. Reading `"one\r\ntwo\r\n"` makes the first line `"one\r"` of length 4 instead of `"one"`, and comparisons such as `line == "one"` fail. A program that accepts files from several operating systems removes a `'\r'` at the end of each line it reads.

```cpp
while (getline(fin, line)) {
    if (!line.empty() && line.back() == '\r')
        line.pop_back();                     // removes the CR of a file made on Windows
    // ... process line
}
```

### 5.4 Counting Lines, Words, and Characters

Reading lines with `getline()` and splitting each line with an `istringstream` (Chapter 11, Section 8) allows line processing and word processing together. [Example_WordCount.cpp](Example_WordCount.cpp) counts lines, words, and characters, like the Unix `wc` command.

```cpp
long lines = 0, words = 0, chars = 0;
string line, longest;
while (getline(fin, line)) {
    lines++;
    chars += line.size() + 1;                // + 1 for the '\n' that getline() discarded
    istringstream in(line);
    string w;
    int inLine = 0;
    while (in >> w) {                        // >> splits the line at whitespace
        inLine++;
        if (w.size() > longest.size())
            longest = w;
    }
    words += inLine;
    cout << "line " << lines << ": " << inLine << " words" << endl;
}
if (fin.eof() && !line.empty())              // the last line had no '\n'
    chars--;
```

When the input file is `"the quick brown fox\n  jumps over\n\nthe lazy dog\n"`, the result is as follows, and it matches `4 9 47`, the output of `wc` for the same file.

```text
line 1: 4 words
line 2: 2 words
line 3: 0 words
line 4: 3 words
4 9 47 words.txt
longest word: quick
```

| Line | Contents | `line.size() + 1` | Words |
|:-----|:-----|:-----|:-----|
| 1 | `the quick brown fox` | 19 + 1 = 20 | 4 |
| 2 | `  jumps over` | 12 + 1 = 13 | 2 |
| 3 | (empty line) | 0 + 1 = 1 | 0 |
| 4 | `the lazy dog` | 12 + 1 = 13 | 3 |
| Total | | 47 | 9 |

*Table 7. Characters and words counted per line*

- The two spaces at the start of the second line are skipped by `>>`, so they are not counted as words, but they are counted as characters.
- An empty line is also counted as a line because `getline()` succeeds, and it has 0 words.
- The character count adds 1 per line for the `'\n'` that `getline()` discarded. In a file whose last line has no `'\n'`, this counts one more than the actual number, so 1 is subtracted after the loop. The final `getline()` that ends the loop does not change `line` if it cannot start reading, so a non-empty `line` means that the last line read ended at the end of the file without a `'\n'`.
- The longest word is `quick`, the first of `quick`, `brown`, and `jumps`, which all have length 5. Because the comparison is `>`, a later word of the same length does not replace it.

---

<br>

## 6. Binary I/O

### 6.1 `read()` and `write()`

```cpp
istream& read(char* s, streamsize n);
ostream& write(const char* s, streamsize n);
```

`read()` takes `n` bytes from the stream and stores them in `s`, and `write()` writes `n` bytes starting at `s` as they are. Neither function interprets character codes, so they are used for binary data. The parameter is a `char*`, so data of other types is passed after converting its address to a byte address with `reinterpret_cast<char*>(&value)`.

```cpp
int value = 1000;
fout.write(reinterpret_cast<const char*>(&value), sizeof(value));   // 4 bytes E8 03 00 00
int back;
fin.read(reinterpret_cast<char*>(&back), sizeof(back));
```

If `read()` reaches the end of the file before filling all `n` bytes, it stores only what it read and sets both `eofbit` and `failbit`. The number of bytes actually read is given by `gcount()`. Therefore, a failed `read()` must not be taken to mean that no data was read.

### 6.2 Block Copying

The byte-by-byte copying of Section 5.1 is easy to understand, but it makes as many function calls as there are bytes. Large files are read and written a buffer at a time.

```cpp
ifstream fin("big.bin", ios::binary);
ofstream fout("big2.bin", ios::binary);
char buf[4096];
while (fin.read(buf, sizeof buf) || fin.gcount() > 0)   // also handles the last piece
    fout.write(buf, fin.gcount());
```

Copying a 10,000-byte file with this code reads it in three pieces of 4,096, 4,096, and 1,808 bytes (4096 + 4096 + 1808 = 10000). The third `read()` cannot fill 4,096 bytes and fails, but `gcount()` is 1,808, so the second part of the condition is true and that piece is written. The fourth `read()` reads nothing, `gcount()` becomes 0, and the loop ends. If the condition were only `while (fin.read(buf, sizeof buf))`, the last 1,808 bytes would be lost. Passing `fin.gcount()` rather than `sizeof buf` to `write()` is for the same reason.

> **Caution:** If a block copy always writes the full buffer, as in `write(buf, sizeof buf)`, then when the last piece is written, the rest of the previously read data is recorded in the file too, and the copy becomes larger than the original.

### 6.3 Storing Structure Records

Storing records of the same size in binary allows a record to be read directly by computing its position from the record number (Section 8). [Example_BinaryRecords.cpp](Example_BinaryRecords.cpp) uses the following structure.

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

The sizes of the members add up to 4 + 10 + 8 = 22 bytes, but in this environment (macOS, arm64) `sizeof(Record)` is 24. In this environment a `double` must be placed at an address that is a multiple of 8, so the compiler inserts 2 bytes of **padding** after `name` and places `score` at offset 16. The bytes stored in the file for the first record, `{101, "Kim", 93.5}`, are as follows.

```text
Offset   Contents                            Bytes (hexadecimal, from the low address)
 0-3     id = 101                            65 00 00 00
 4-13    name = "Kim" and 7 null characters  4B 69 6D 00 00 00 00 00 00 00
14-15    padding (unspecified value)         00 00
16-23    score = 93.5 (IEEE 754 double)      00 00 00 00 00 60 57 40
```

*Figure 1. The 24-byte layout of `Record {101, "Kim", 93.5}`*

The `id` 101 is `0x65` in hexadecimal, so it is `65 00 00 00`, and the `score` 93.5 is the IEEE 754 double-precision value `0x4057600000000000` written from the low-order byte, `00 00 00 00 00 60 57 40`.

> **[Computer Architecture]** Data alignment and byte order (endianness), covered in Computer Architecture, determine the format of a binary file. A processor reads n-byte data most efficiently at an address that is a multiple of n, so the compiler inserts padding into structures, and x86-64 and arm64 are little-endian, placing the low-order byte at the low address.

Storing a whole structure with `write()` is simple, but the following conditions must hold.

- **Every member must hold its value itself.** Members of type `std::string`, `std::vector`, or pointer hold not the actual data but an address on the heap, so when the stored bytes are read back, they become an address that has already been freed or that means nothing in another process. Such members must have their length and contents stored separately. The standard calls a type that may be stored by copying its bytes a **trivially copyable type**, which can be checked with `std::is_trivially_copyable<T>::value`[3].
- **Read it only on the same environment.** Padding, byte order, and the sizes of `int` and `long` can differ between compilers and processors, so a file to be read on another environment is stored by **serialization**, writing the members one by one with fixed sizes and a fixed byte order.
- **The values of padding are not specified.** Comparing structures as bytes or hashing them can give different results for records with the same values.

---

<br>

## 7. Checking the Stream State

### 7.1 Points to Check

The state bits of file I/O are the same as in Chapter 11, Section 4. The points at which failures occur with files, and the states at those points, are as follows.

| Point | Cause of failure | Bits set | How to check |
|:-----|:-----|:-----|:-----|
| Opening | No file, no permission, wrong path, a mode combination not in the table | `failbit` | `if (!f)`, `is_open()` |
| Formatted reading | Not the expected format | `failbit` | Use the result of the read as the condition |
| Reading | End of the file | `eofbit` (also `failbit` if more was attempted) | `eof()` after the loop |
| Reading, writing | Device error, disk full | `badbit` | `bad()` |
| Closing | Failure to flush the buffer | `failbit` | `fail()` after `close()` |

*Table 8. Failure points of file I/O and how to check them*

A write failure shows up late because of the buffer. Even if `<<` succeeds, the bytes may still be in the buffer, and the fact that the disk is full is reported when the buffer is flushed. A program that writes important data calls `close()` explicitly and checks `fail()` afterward.

### 7.2 The Error of Using `eof()` as the Loop Condition

`while (!fin.eof())` reads as "read while not at the end of the file" and looks natural, but `eof()` becomes true only **after** a read has reached the end, so the last value is processed twice. [Example_EofLoop.cpp](Example_EofLoop.cpp) shows this error with a file containing `10\n20\n30\n`.

```cpp
while (!bad.eof()) {                         // wrong: eof() is false until a read hits the end
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

| Iteration | `eof()` at the start | Result of `bad >> value` | `value` | `sum` |
|:-----|:-----|:-----|:-----|:-----|
| 1 | 0 | Reads `10`, stops before `'\n'` | 10 | 10 |
| 2 | 0 | Reads `20` | 20 | 30 |
| 3 | 0 | Reads `30`, stops before the last `'\n'` | 30 | 60 |
| 4 | 0 | Reaches the end while skipping `'\n'`, fails | 30 (unchanged) | 90 |

*Table 9. Trace of `while (!fin.eof())`*

When the third iteration ends, the last `'\n'` is still there, so `eof()` is false. In the fourth iteration, `>>` reaches the end while skipping that `'\n'` and fails; because it ended before finding a digit, `value` stays 30 (Chapter 11, Section 4.2), and the body, which does not check for failure, adds 30 once more. If the last line of the file has no newline, `eofbit` is set by the third read and the result comes out right, so this error appears with some files and hides with others. The correct loop uses the read operation itself as the condition.

```cpp
while (good >> value)                        // right: test the result of the read itself
    sum += value;
```

> **Key Point:** Use a read operation (`fin >> x`, `getline(fin, line)`, `fin.get(c)`, `fin.read(...)`) as the loop condition, not `eof()`. `eof()` is used after the loop ends, to tell its cause.

### 7.3 Telling Why a Loop Ended

`while (fin >> x)` ends not only at the end of the file but also at data with the wrong format and at device errors. After the loop, tell the cause in the following order and report errors.

```cpp
if (good.bad())
    cout << "stopped by an I/O error" << endl;
else if (good.eof())
    cout << "stopped at the end of the file" << endl;
else
    cout << "stopped at data that is not an integer" << endl;
```

`bad()` is checked first because a device error is the most serious and can be set together with other bits. Next, if `eof()` is true, the file was read normally to the end; if neither, only `failbit` is set, which means there is non-integer data in the middle of the file. For example, a file containing `10 20 abc 30` stops at the total 30 and prints the third message.

### 7.4 Reporting Failure with Exceptions

State bits pass silently if they are not checked. Calling the member function `exceptions(mask)` makes the stream throw an `ios_base::failure` exception the moment a bit in `mask` is set[3]. The syntax of exceptions is covered in Chapter 13. [Example_StreamExceptions.cpp](Example_StreamExceptions.cpp) receives an open failure as an exception.

```cpp
ifstream fin;
fin.exceptions(ios::failbit | ios::badbit);  // these bits now raise an exception
try {
    fin.open("no_such_file.txt");            // open failure sets failbit, so it throws
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

> **Caution:** If `while (fin >> x)` is used on a stream with exceptions set for `failbit`, an exception is thrown at the end of the file the moment the last read sets `failbit`. Even the normal end is then handled as an exception, so on streams read in a loop, set only `badbit`.

The second part of the example reads three times from `7 8` on a stream with only `badbit` set. The third read fails at the end, but `failbit` is not an exception target, so `c` stays -1 without an exception.

---

<br>

## 8. Random Access

### 8.1 File Position

A stream remembers the position of the next byte to read or write. This position is the number of bytes counted from the beginning of the file, and it moves forward by as much as is read or written. Processing in order from the beginning is called **sequential access**, and processing after moving the position to a desired place is called **random access**.

> **Definition:** **Random access** is a way of reading or writing directly at a desired byte by moving the file position there, instead of reading the file in order from the beginning.

`istream` has functions for the read position (the get position), and `ostream` has functions for the write position (the put position). However, the buffer of a file stream uses a single file position for both reading and writing[3], so in an `fstream`, moving the read position with `seekg()` moves the write position too. In fact, in an `fstream` opened with `in | out`, `tellp()` after `seekg(2)` is 2, and `tellg()` after `seekp(4)` is 4. A `stringstream`, in contrast, keeps the two positions separately, so `tellp()` is still 0 after `seekg(2)`.

### 8.2 `seekg()`, `seekp()`, `tellg()`, `tellp()`

| Function | Behavior |
|:-----|:-----|
| `seekg(pos)` | Moves the read position to `pos` bytes from the beginning |
| `seekg(off, dir)` | Moves the read position to `off` bytes from the reference point `dir` |
| `tellg()` | Returns the current read position (-1 on failure) |
| `seekp(pos)`, `seekp(off, dir)` | Do the same for the write position |
| `tellp()` | Returns the current write position |

*Table 10. Member functions that work with the file position*

The reference point `dir` is `ios::beg` (the beginning of the file), `ios::cur` (the current position), or `ios::end` (the end of the file)[3]. `off` has the signed integer type `streamoff`, so `seekg(-4, ios::end)` moves 4 bytes before the end, and `seekg(8, ios::cur)` moves 8 bytes past the current position. `seekg()` clears `eofbit` and moves if only `eofbit` is set, but it fails without moving if `failbit` is set[3]. To move the position of a stream that tried to read past the end and has `failbit` set as well, call `clear()` first.

**File size.** Moving the position to the end and calling `tellg()` gives the number of bytes in the file.

```cpp
file.seekg(0, ios::end);
cout << "file size = " << file.tellg() << " bytes" << endl;   // 72
```

This method is used in binary mode. The C standard says that a binary stream need not meaningfully support seeking relative to `SEEK_END`[5], and text mode has the restrictions of Section 8.4, so when portability matters, use `std::filesystem::file_size()` from Section 9.

### 8.3 Random Access by Record

When records have a fixed size, the record with index `i` starts at byte `i × sizeof(Record)`. The `Record` of Section 6.3 is 24 bytes, so records 0, 1, and 2 start at bytes 0, 24, and 48, and the file size is 3 × 24 = 72 bytes. [Example_BinaryRecords.cpp](Example_BinaryRecords.cpp) reads only record 2, corrects its score, and writes it back in place.

```cpp
fstream file("records.bin", ios::in | ios::out | ios::binary);   // keeps the contents, reads and writes
Record r;
file.seekg(2 * sizeof(Record), ios::beg);    // jumps to record index 2
file.read(reinterpret_cast<char*>(&r), sizeof(r));

r.score = 88.0;                              // changes the score of record 2
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

Tracing the changes in position gives the following.

| Operation | Position before | Position after | Result |
|:-----|:-----|:-----|:-----|
| `seekg(0, ios::end)`, `tellg()` | 0 | 72 | File size 72 |
| `seekg(48, ios::beg)` | 72 | 48 | Start of record 2 |
| `read(&r, 24)` | 48 | 72 | `{103, "Park", 72.25}`, `gcount()` is 24 |
| `seekp(48, ios::beg)` | 72 | 48 | Back to the same record |
| `write(&r, 24)` | 48 | 72 | Overwritten with the record whose score is 88.0 |
| `read` loop after `seekg(0, ios::beg)` | 0 | 72 | Reads three records and fails on the fourth |

*Table 11. File positions while reading and correcting record 2*

Opening the file with `in | out | binary` is essential. Giving only `out | binary` equals `"wb"` and erases all existing records. Also, when switching between reading and writing, call a positioning function in between. The standard places the same restrictions on reading and writing file streams as on C `FILE`s[3], and C requires, on a stream opened in update mode, a call to `fflush` or a positioning function between writing and a following read, and a call to a positioning function between reading and a following write[5]. The last loop reads three records, and the fourth `read()` fails at 0 bytes, so `gcount()` is 0.

### 8.4 Positioning in Text Mode

In text mode, on environments that translate line endings, such as Windows, the number of bytes in the file does not match the number of characters the program reads. The C standard therefore allows, for text streams, only a positioning offset of 0 or a value previously returned by the position query function[5]. File streams move the position the same way as C's `fseek`, so they follow the same restriction. In text files, use a position obtained with `tellg()` only to return to it later with `seekg(pos)`, and do positioning by computed byte counts in binary mode.

---

<br>

## 9. The Filesystem Library

File streams read and write the contents of files, but they have no features to check whether a file exists or to create and remove directories. C++17's `<filesystem>` provides such operations independently of the operating system[3]. The name is long, so it is customary to use an alias such as `namespace fs = std::filesystem;`.

### 9.1 Working with Paths

`fs::path` is the class that represents paths. The `/` operator joins paths with the separator suited to the operating system, and member functions split a path into its parts.

```cpp
fs::path p = dir / "logs" / "b.log";         // fs_demo/logs/b.log
p.parent_path();                             // fs_demo/logs
p.filename();                                // b.log
p.stem();                                    // b
p.extension();                               // .log
```

`generic_string()` returns a string whose separators are unified to `/`, and `string()` returns a string in the operating system's native format (`\` on Windows). File streams accept `fs::path` as it is, so `ofstream(dir / "a.txt")` can be written.

### 9.2 Operating on Files and Directories

| Function | Behavior |
|:-----|:-----|
| `fs::exists(p)` | Whether the file or directory `p` refers to exists |
| `fs::is_regular_file(p)`, `fs::is_directory(p)` | Whether it is a regular file, a directory |
| `fs::file_size(p)` | The number of bytes of a regular file |
| `fs::create_directory(p)`, `fs::create_directories(p)` | Creates one directory, every intermediate directory as well |
| `fs::copy_file(from, to)`, `fs::rename(from, to)` | Copies a file, renames or moves it |
| `fs::remove(p)`, `fs::remove_all(p)` | Removes one file or empty directory, a directory and everything in it |
| `fs::directory_iterator(p)`, `fs::recursive_directory_iterator(p)` | Iterates over the entries of a directory, including subdirectories |

*Table 12. Frequently used `std::filesystem` functions*

> **[Operating Systems]** The file systems covered in Operating Systems organize files into a tree of directories and manage metadata for each file, such as size, type, permissions, and times. The functions of `std::filesystem` wrap the operating system calls that query this metadata or change the directory tree, while file streams handle the contents of those files.

[Example_Filesystem.cpp](Example_Filesystem.cpp) creates directories, writes two files, iterates over the tree printing sizes, and then removes everything.

```cpp
fs::path dir = "fs_demo";
fs::create_directories(dir / "logs");        // creates both levels if missing
{ ofstream(dir / "a.txt") << "alpha\n"; }    // a temporary ofstream closes at the ';'
{ ofstream(dir / "logs" / "b.log") << "beta beta\n"; }

for (const auto& entry : fs::recursive_directory_iterator(dir)) {
    cout << entry.path().generic_string();
    if (entry.is_regular_file())
        cout << "  " << entry.file_size() << " bytes";
    cout << endl;
}

error_code ec;
auto size = fs::file_size(dir / "none.txt", ec);   // the error_code version does not throw
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

- `a.txt` is 6 bytes, `alpha` and a newline, and `b.log` is 10 bytes, `beta beta` and a newline.
- The standard does not specify the order of iteration[3], so it differs with the operating system and the file system. If a sorted list is needed, collect the paths in a `vector` and sort it.
- Most functions come in two forms: one that throws an `fs::filesystem_error` exception on failure, and one that takes an `error_code&` as the last argument and records the error there. Where an error is expected (querying the size of a file that may not exist), use the `error_code` form. In that case, `file_size()` returns `static_cast<uintmax_t>(-1)`[3].
- `remove_all()` returns the number of entries removed. With the directories `fs_demo` and `logs` and the two files, it is 4.

---

<br>

## Concept Applications

**True/False Check:**

| Statement | Judgment and Reason |
|:-----|:-----|
| On macOS, writing `'\n'` to a file opened without `ios::binary` records `0D 0A`. | **F.** On POSIX, text mode and binary mode are the same, so a single `0A` is recorded. The translation happens in Windows text mode (Section 1.2). |
| `ofstream f("a.txt", ios::out \| ios::ate);` appends after the existing contents. | **F.** `out \| ate` equals `"w"`, so it erases the contents. Use `app` or `in \| out \| ate` (Section 4.2). |
| `fstream f("new.txt", ios::in \| ios::out);` creates the file if it does not exist. | **F.** It equals `"r+"`, so opening fails if the file does not exist (Section 4.2). |
| A `while (!fin.eof())` loop can process the last value of the file twice. | **T.** `eof()` becomes true only after a read has reached the end, so the body runs once more after the failed read (Section 7.2). |
| When `read()` fails, no data has been stored in the buffer. | **F.** Reading only part of the data at the end of the file also counts as a failure, and the number of bytes read is given by `gcount()` (Section 6.1). |
| A structure with a `std::string` member can also be stored with `write()` and restored with `read()`. | **F.** A `string` holds an address on the heap, so storing its bytes does not store its contents (Section 6.3). |
| In an `fstream`, moving the position with `seekg()` changes `tellp()` as well. | **T.** The buffer of a file stream has only one file position (Section 8.1). |
| `recursive_directory_iterator` visits entries in name order. | **F.** The order of iteration is not specified (Section 9.2). |

---

<br>

## Glossary

| English | Korean | One-Line Definition |
|:-----|:-----|:-----|
| text mode / binary mode | 텍스트 모드 / 바이너리 모드 | A way of opening that may translate line endings, and a way of opening that handles bytes as they are |
| line ending (LF, CR LF) | 줄 끝 표기 | The bytes an operating system uses to mark the end of a line in a text file (`0A` on Unix, `0D 0A` on Windows) |
| working directory | 작업 디렉터리 | The directory in which file names without a path are looked up |
| open mode | 파일 모드 | The combination of `ios::in`, `out`, `app`, `ate`, `trunc`, and `binary` that decides how a file is opened |
| truncate | 내용 지우기 | Removing the existing contents of a file when opening it, making its length 0 |
| padding | 패딩 | Bytes the compiler inserts between or after structure members to meet alignment requirements |
| trivially copyable type | 트리비얼하게 복사 가능한 타입 | A type whose value is copied completely by copying its bytes |
| serialization | 직렬화 | Turning data into a byte sequence with fixed sizes and byte order for storage or transmission |
| sequential access | 순차 접근 | Reading or writing a file in order from the beginning |
| file position | 파일 위치 | The position of the next byte to read or write, counted from the beginning |
| seek direction | 위치 기준 | The reference points of positioning, `ios::beg`, `ios::cur`, `ios::end` |
| path | 경로 | A name that indicates the location of a file or directory in the file system |

*Table 13. Glossary*

---

<br>

## Summary

| Concept | Key Summary |
|:-----|:-----|
| Text files and binary files | Every file is a sequence of bytes; interpreted as character codes and lines it is a text file, and in a format defined by a program it is a binary file. |
| Text mode | On Windows it translates `LF` and `CR LF` and treats `Ctrl+Z` as the end; on POSIX it is the same as binary mode. |
| File stream classes | `ifstream`, `ofstream`, and `fstream` inherit `istream`, `ostream`, and `iostream`, and the first two always add `in` and `out` when opening. |
| Opening and closing | Open with the constructor or `open()`, check with `if (!f)`, and the file closes when the stream is destroyed, so it closes when control leaves the block. |
| Working directory | Relative paths are interpreted relative to the directory from which the program was run, not to the source file. |
| Causes of open failure | The stream reports only failure; the cause is found with `std::filesystem`, or with `errno` on POSIX, and a successful open still requires checking each read. |
| `<<` and `>>` | Write and read values as text; when writing, separate them with spaces or newlines so that they can be read back. |
| File modes | `out` erases the contents, `app` appends at the end, `in \| out` keeps the contents but fails if the file does not exist, and combinations not in the table fail to open. |
| `get()`, `put()`, `getline()` | Used for character-by-character copying including spaces and for line processing; a `getline()` loop counts lines correctly whether or not the last line has a newline. |
| Files from other operating systems | Reading a file made on Windows on POSIX leaves `'\r'` at the end of each line, which must be removed. |
| `read()`, `write()` | Read and write bytes as they are, and even when `read()` fails, `gcount()` bytes were read. |
| Structure records | Because of padding, `sizeof` can exceed the sum of the member sizes, only trivially copyable types are stored as bytes, and other environments require serialization. |
| `eof()` loop error | `eof()` becomes true after the end is reached, so using it as the loop condition processes the last value twice; use the read operation as the condition. |
| Why a loop ended | Check `bad()`, then `eof()`; if neither, it is a format error. |
| `exceptions()` | Makes the stream throw `ios_base::failure` when the given state bits are set; for streams read in a loop, give only `badbit`. |
| Random access | `seekg` and `seekp` move to positions relative to `beg`, `cur`, `end`, `tellg` and `tellp` query them, and a file stream has one position for reading and writing. |
| Record position | Fixed-size record `i` starts at `i × sizeof(Record)`, and in-place modification opens with `in \| out \| binary`. |
| `std::filesystem` | Provides path manipulation, existence checks, size queries, directory creation and removal, and iteration independently of the operating system; the order of iteration is not specified. |

---

<br>

## Self-Check Questions

1. **Byte counts:** Find the size of the file written by `fout << "A\nB\n";` in text mode on Windows, and of the file written in binary mode. What about macOS?

   <details>
   <summary>Answer</summary>

   In Windows text mode each of the two `'\n'` becomes `0D 0A`, giving the 6 bytes `41 0D 0A 42 0D 0A`; in binary mode it is the 4 bytes `41 0A 42 0A`. On macOS the two modes are the same, so both are 4 bytes.

   </details>

2. **Choosing a mode:** Write the file mode suited to each purpose. (1) Append a log at the end on every run. (2) Correct one record of an existing binary file in place. (3) Create the result file anew every time.

   <details>
   <summary>Answer</summary>

   (1) `ofstream f("log.txt", ios::app);`. (2) `fstream f("data.bin", ios::in | ios::out | ios::binary);`; giving only `out | binary` erases the contents. (3) `ofstream f("result.txt");`; the default mode `out` erases the existing contents.

   </details>

3. **Tracing a loop condition:** When the file contains `5 7` (no newline at the end), find `sum` for `while (!fin.eof()) { fin >> x; sum += x; }`, and explain what changes if the contents are `5 7\n`. `sum` and `x` were initialized to 0.

   <details>
   <summary>Answer</summary>

   With `5 7`, the first iteration reads 5, and the second iteration reads 7 and reaches the end, setting `eofbit`, so the loop ends. `sum` is 12, correct by chance. With `5 7\n`, after the second iteration reads 7, the `'\n'` remains and `eof()` is false, so a third iteration runs; the failed read does not change `x`, so 7 is added once more and `sum` becomes 19. Because the result depends on the final newline of the file, `while (fin >> x)` must be used.

   </details>

4. **Block copying:** With a buffer size of 1,000 bytes, how many times is `read()` called when copying a 2,500-byte file with the code of Section 6.2, and what is `gcount()` after each call?

   <details>
   <summary>Answer</summary>

   It is called four times. `gcount()` is 1,000, 1,000, 500, and 0. The third call fills only 500 bytes and fails, but `gcount() > 0`, so the 500 bytes are written; the fourth call reads 0 bytes, the whole condition becomes false, and the loop ends.

   </details>

5. **Record position:** On an environment where `sizeof` of `struct Item { char code; int qty; double price; };` is 16, find the start position of the record with index 5, and explain why 16 differs from the sum of the member sizes (1 + 4 + 8 = 13).

   <details>
   <summary>Answer</summary>

   It is 5 × 16 = 80 bytes. An `int` must be placed at a multiple of 4 and a `double` at a multiple of 8, so 3 bytes of padding go after `code`, `qty` is placed at offset 4 and `price` at offset 8, and the whole is 16 bytes.

   </details>

6. **String members:** What problem arises if `struct Person { std::string name; int age; };` is stored with `write(reinterpret_cast<const char*>(&p), sizeof p)` and read back with `read()` in another run? Write a correct way to store it.

   <details>
   <summary>Answer</summary>

   Unless the string is short, `std::string` keeps its characters on the heap and holds their address inside the object. The stored bytes contain a heap address from the previous run, so the object read back points to memory that does not exist, and using or destroying it is undefined behavior. `std::is_trivially_copyable<Person>::value` is also false. Store the members one by one, for example by writing the length of the name as an integer, then that many characters, and then `age`.

   </details>

7. **Telling the cause:** After `while (fin >> x)` ended, `fin.eof()` was false and `fin.bad()` was also false. What ended the loop, and how can the data after that point be read?

   <details>
   <summary>Answer</summary>

   Only `failbit` is set, so the loop stopped at data in the middle of the file that cannot be interpreted as an integer. After clearing the state with `fin.clear()`, discard the invalid token with `fin.ignore()` or `fin >> word`, and reading can continue (Chapter 11, Section 4.3).

   </details>

8. **File size:** Write why `tellg()` after `ifstream f("a.txt"); f.seekg(0, ios::end);` can differ from the number of characters in the file in Windows text mode, and how to obtain the file size portably.

   <details>
   <summary>Answer</summary>

   Windows text mode reads the line ending `0D 0A` as a single `'\n'`, so the number of bytes in the file differs from the number of characters the program reads, and the C standard does not guarantee that the position values of a text stream can be interpreted as byte counts. When the byte count is needed, use `std::filesystem::file_size("a.txt")` or open the file in binary mode and query the position.

   </details>

---

<br>

## References

1. Microsoft, "fopen, _wfopen," *Microsoft Learn*. https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen
2. IEEE and The Open Group, "fopen," in *The Open Group Base Specifications Issue 8*, IEEE Std 1003.1-2024. https://pubs.opengroup.org/onlinepubs/9799919799/functions/fopen.html
3. ISO/IEC JTC 1/SC 22/WG 21, "Working Draft, Standard for Programming Language C++," Document N4659, Mar. 2017. https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
4. cppreference.com, "std::ios_base::openmode." https://en.cppreference.com/w/cpp/io/ios_base/openmode
5. ISO/IEC JTC 1/SC 22/WG 14, "Programming languages: C," Committee Draft N1570, Apr. 2011. https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
