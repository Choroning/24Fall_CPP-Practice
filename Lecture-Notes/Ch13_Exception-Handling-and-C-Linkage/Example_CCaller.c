/**
 * @file    Example_CCaller.c
 * @brief   C code that calls the C++ function greet_length() defined in Example_CppGreet.cpp
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <stdio.h>

int greet_length(const char *name);   /* defined in C++ with extern "C" */

int main(void)
{
    int n = greet_length("C");
    printf("length %d\n", n);
    return 0;
}
