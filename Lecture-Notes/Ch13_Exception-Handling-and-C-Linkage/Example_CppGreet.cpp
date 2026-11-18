/**
 * @file    Example_CppGreet.cpp
 * @brief   A C++ function with C language linkage that C code can call; exceptions are caught before they reach C
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <cstdio>
#include <string>

extern "C" int greet_length(const char* name) {   // callable from C
    try {
        std::string s = std::string("Hello, ") + name;
        std::printf("%s\n", s.c_str());
        return static_cast<int>(s.size());
    } catch (...) {                                // keeps exceptions from crossing into C
        return -1;
    }
}
