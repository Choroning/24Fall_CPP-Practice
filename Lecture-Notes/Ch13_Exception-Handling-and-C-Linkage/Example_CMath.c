/**
 * @file    Example_CMath.c
 * @brief   C implementations of the functions declared in Example_CMath.h
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include "Example_CMath.h"

int add(int a, int b)
{
    return a + b;
}

double average(const int *values, int count)
{
    int sum = 0;
    for (int i = 0; i < count; i++)
        sum += values[i];
    return count > 0 ? (double)sum / count : 0.0;
}
