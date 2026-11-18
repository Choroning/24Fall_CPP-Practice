/**
 * @file    Example_Rethrow.cpp
 * @brief   Compares throw; with throw e; and catching by value with catching by reference
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <stdexcept>
using namespace std;

class FileError : public runtime_error {
public:
    explicit FileError(const string& what) : runtime_error(what) {}
};

void open() { throw FileError("config.txt not found"); }

void rethrowSame() {
    try { open(); }
    catch (const exception& e) {
        cout << "  log: " << e.what() << endl;
        throw;                                   // rethrows the original FileError object
    }
}

void rethrowCopy() {
    try { open(); }
    catch (const exception& e) {
        throw e;                                 // copies only the exception part: slicing
    }
}

int main() {
    try { rethrowSame(); }
    catch (const FileError& e) { cout << "throw;  -> FileError: " << e.what() << endl; }
    catch (const exception& e) { cout << "throw;  -> exception: " << e.what() << endl; }

    try { rethrowCopy(); }
    catch (const FileError& e) { cout << "throw e -> FileError: " << e.what() << endl; }
    catch (const exception& e) { cout << "throw e -> exception: " << e.what() << endl; }

    try { open(); }
    catch (exception e) {                        // by value: the copy is a plain exception
        cout << "by value  -> " << e.what() << endl;
    }
    return 0;
}
