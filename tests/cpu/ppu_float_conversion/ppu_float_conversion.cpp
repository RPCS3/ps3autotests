/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include <float.h>
#include <math.h>
#include <stdio.h>

#include "../../../common/output.h"

// Show results
#define PRINT1(name, i, FPR) \
    fprintf(handler, name"([%02d]) -> %016llX [%08X : %08X]\n", i, *(uint64_t*)&FPR, getFPSCR(), getCR());
#define PRINT2(name, i, j, FPR) \
    fprintf(handler, name"([%02d],[%02d]) -> %016llX [%08X : %08X]\n", i, j, *(uint64_t*)&FPR, getFPSCR(), getCR());
#define PRINT3(name, i, j, k, FPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d]) -> %016llX [%08X : %08X]\n", i, j, k, *(uint64_t*)&FPR, getFPSCR(), getCR());

// Array sizes
#define TESTSIZEF (sizeof(testFloats)/sizeof(float))
#define TESTSIZED (sizeof(testDoubles)/sizeof(double))

// Loops with floats
#define ITERATE1F(x) \
    for (uint32_t i = 0; i < TESTSIZEF; i++) \
        {float f0, f1=testFloats[i]; clearCR(); clearFPSCR(); x;}

#define ITERATE2F(x) \
    for (uint32_t i = 0; i < TESTSIZEF; i++) \
        for (uint32_t j = 0; j < TESTSIZEF; j++) \
            {float f0, f1=testFloats[i], f2=testFloats[j]; clearCR(); clearFPSCR(); x;}

// Loops with doubles
#define ITERATE1D(x) \
    for (uint32_t i = 0; i < TESTSIZED; i++) \
        {double d0, d1=testDoubles[i]; clearCR(); clearFPSCR(); x;}

#define ITERATE2D(x) \
    for (uint32_t i = 0; i < TESTSIZED; i++) \
        for (uint32_t j = 0; j < TESTSIZED; j++) \
            {double d0, d1=testDoubles[i], d2=testDoubles[j]; clearCR(); clearFPSCR(); x;}

// Get contents of the CR register
uint32_t getCR()
{
    uint32_t CR;
    __asm__ ("mfcr %0" : "=r"(CR));
    return CR;
};

// Get contents of the FPSCR register
uint32_t getFPSCR()
{
    double FPSCR = 0.0;
    __asm__ ("mffs %0" : "=f"(FPSCR));
    return ((uint32_t*)&FPSCR)[1];
};

// Clear CR register
void clearCR()
{
    uint32_t CR = 0;
    __asm__ ("mtcr %0" : "=r"(CR));
};

// Clear FPSCR register
void clearFPSCR()
{
    __asm__ ("mtfsfi 0, 0");
    __asm__ ("mtfsfi 1, 0");
    __asm__ ("mtfsfi 2, 0");
    __asm__ ("mtfsfi 3, 0");
    __asm__ ("mtfsfi 4, 0");
    __asm__ ("mtfsfi 5, 0");
    __asm__ ("mtfsfi 6, 0");
    __asm__ ("mtfsfi 7, 0");
};

const float testFloats[] = {
    0.0f, -0.0f,  1.0f, -1.0f,                      //  0  1  2  3
    1.5f, -1.5f,  1.6f, -1.6f,                      //  4  5  6  7
    1.4f, -1.4f,  2.0f,   NAN,                      //  8  9 10 11
    -10000000.4f, 20000000.0f, -20000.5f, 20000.6f, // 12 13 14 15
    FLT_MIN, FLT_MAX, INFINITY, -INFINITY,          // 16 17 18 19
};

const double testDoubles[] = {
    0.0, -0.0,  1.0, -1.0,                          //  0  1  2  3
    1.5, -1.5,  1.6, -1.6,                          //  4  5  6  7
    1.4, -1.4,  2.0,  NAN,                          //  8  9 10 11
    -10000000.4, 20000000.0, -20000.5, 20000.6,     // 12 13 14 15
    DBL_MIN, DBL_MAX, INFINITY, -INFINITY,          // 16 17 18 19
};

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // Integer Arithmetic Instructions
    ITERATE1D(__asm__ ("frsp    %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("frsp   ",i,d0));
    ITERATE1D(__asm__ ("frsp.   %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("frsp.  ",i,d0));
    ITERATE1F(__asm__ ("frsp    %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("frsp   ",i,f0));
    ITERATE1F(__asm__ ("frsp.   %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("frsp.  ",i,f0));
    ITERATE1D(__asm__ ("fcfid   %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fcfid  ",i,d0));
    ITERATE1D(__asm__ ("fcfid.  %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fcfid. ",i,d0));
    ITERATE1F(__asm__ ("fcfid   %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fcfid  ",i,f0));
    ITERATE1F(__asm__ ("fcfid.  %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fcfid. ",i,f0));
    ITERATE1D(__asm__ ("fctid   %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctid  ",i,d0));
    ITERATE1D(__asm__ ("fctid.  %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctid. ",i,d0));
    ITERATE1F(__asm__ ("fctid   %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctid  ",i,f0));
    ITERATE1F(__asm__ ("fctid.  %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctid. ",i,f0));
    ITERATE1D(__asm__ ("fctidz  %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctidz ",i,d0));
    ITERATE1D(__asm__ ("fctidz. %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctidz.",i,d0));
    ITERATE1F(__asm__ ("fctidz  %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctidz ",i,f0));
    ITERATE1F(__asm__ ("fctidz. %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctidz.",i,f0));
    ITERATE1D(__asm__ ("fctiw   %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctiw  ",i,d0));
    ITERATE1D(__asm__ ("fctiw.  %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctiw. ",i,d0));
    ITERATE1F(__asm__ ("fctiw   %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctiw  ",i,f0));
    ITERATE1F(__asm__ ("fctiw.  %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctiw. ",i,f0));
    ITERATE1D(__asm__ ("fctiwz  %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctiwz ",i,d0));
    ITERATE1D(__asm__ ("fctiwz. %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fctiwz.",i,d0));
    ITERATE1F(__asm__ ("fctiwz  %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctiwz ",i,f0));
    ITERATE1F(__asm__ ("fctiwz. %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fctiwz.",i,f0));

    return 0;
}
