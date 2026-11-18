/**
 * @file    Example_CMath.h
 * @brief   Declares C functions so that both C and C++ translation units can include the header
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#ifndef EXAMPLE_CMATH_H
#define EXAMPLE_CMATH_H

#ifdef __cplusplus
extern "C" {                 /* seen only by a C++ compiler: give the names C language linkage */
#endif

int add(int a, int b);
double average(const int *values, int count);

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_CMATH_H */
