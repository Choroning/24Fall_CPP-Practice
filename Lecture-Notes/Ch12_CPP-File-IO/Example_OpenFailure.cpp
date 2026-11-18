/**
 * @file    Example_OpenFailure.cpp
 * @brief   Finds out why a file could not be opened, with std::filesystem and, on POSIX systems, errno
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;
using namespace std;

void tryOpen(const fs::path& p) {
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
}

int main() {
    { ofstream make("present.txt"); make << "ok\n"; }
    { ofstream make("locked.txt"); make << "secret\n"; }
    fs::permissions("locked.txt", fs::perms::none);   // removes every permission bit
    fs::create_directory("folder");
    tryOpen("present.txt");
    tryOpen("absent.txt");
    tryOpen("locked.txt");
    tryOpen("folder");
    fs::permissions("locked.txt", fs::perms::owner_all);
    fs::remove("locked.txt");
    fs::remove("folder");
    return 0;
}
