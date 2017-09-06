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
#define PRINT4(name, i, j, k, l, FPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d],[%02d]) -> %016llX [%08X : %08X]\n", i, j, k, l, *(uint64_t*)&FPR, getFPSCR(), getCR());

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

#define ITERATE3F(x) \
    for (uint32_t i = 0; i < TESTSIZEF; i++) \
        for (uint32_t j = 0; j < TESTSIZEF; j++) \
            {float f0, f1=testFloats[i], f2=testFloats[j], f3=testFloats[(i+j)%TESTSIZEF]; clearCR(); clearFPSCR(); x;}

// Loops with doubles
#define ITERATE1D(x) \
    for (uint32_t i = 0; i < TESTSIZED; i++) \
        {double d0, d1=testDoubles[i]; clearCR(); clearFPSCR(); x;}

#define ITERATE2D(x) \
    for (uint32_t i = 0; i < TESTSIZED; i++) \
        for (uint32_t j = 0; j < TESTSIZED; j++) \
            {double d0, d1=testDoubles[i], d2=testDoubles[j]; clearCR(); clearFPSCR(); x;}

#define ITERATE3D(x) \
    for (uint32_t i = 0; i < TESTSIZED; i++) \
        for (uint32_t j = 0; j < TESTSIZED; j++) \
            {double d0, d1=testDoubles[i], d2=testDoubles[j], d3=testDoubles[(i+j)%TESTSIZED]; clearCR(); clearFPSCR(); x;}

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

    // Floating-Point Arithmetic Instructions
    ITERATE2D(__asm__ ("fadd     %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fadd  ",i,j,d0));
    ITERATE2D(__asm__ ("fadd.    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fadd. ",i,j,d0));
    ITERATE2F(__asm__ ("fadds    %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fadds ",i,j,f0));
    ITERATE2F(__asm__ ("fadds.   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fadds.",i,j,f0));
    ITERATE2D(__asm__ ("fsub     %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fsub  ",i,j,d0));
    ITERATE2D(__asm__ ("fsub.    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fsub. ",i,j,d0));
    ITERATE2F(__asm__ ("fsubs    %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fsubs ",i,j,f0));
    ITERATE2F(__asm__ ("fsubs.   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fsubs.",i,j,f0));
    ITERATE2D(__asm__ ("fmul     %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fmul  ",i,j,d0));
    ITERATE2D(__asm__ ("fmul.    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fmul. ",i,j,d0));
    ITERATE2F(__asm__ ("fmuls    %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fmuls ",i,j,f0));
    ITERATE2F(__asm__ ("fmuls.   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fmuls.",i,j,f0));
    ITERATE2D(__asm__ ("fdiv     %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fdiv  ",i,j,d0));
    ITERATE2D(__asm__ ("fdiv.    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT2("fdiv. ",i,j,d0));
    ITERATE2F(__asm__ ("fdivs    %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fdivs ",i,j,f0));
    ITERATE2F(__asm__ ("fdivs.   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT2("fdivs.",i,j,f0));
    ITERATE1D(__asm__ ("fsqrt    %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fsqrt   ",i,d0));
    ITERATE1D(__asm__ ("fsqrt.   %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("fsqrt.  ",i,d0));
    ITERATE1F(__asm__ ("fsqrts   %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fsqrts  ",i,f0));
    ITERATE1F(__asm__ ("fsqrts.  %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fsqrts. ",i,f0));
    ITERATE1F(__asm__ ("fres     %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fres    ",i,f0));
    ITERATE1F(__asm__ ("fres.    %0,%1" : "=f"(f0) : "f"(f1));  PRINT1("fres.   ",i,f0));
    ITERATE1D(__asm__ ("frsqrte  %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("frsqrte ",i,d0));
    ITERATE1D(__asm__ ("frsqrte. %0,%1" : "=f"(d0) : "f"(d1));  PRINT1("frsqrte.",i,d0));
    ITERATE3D(__asm__ ("fsel     %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fsel    ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3D(__asm__ ("fsel.    %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fsel.   ",i,j,(i+j)%TESTSIZED,d0));

    // Floating-Point Multiply-Add Instructions
    ITERATE3D(__asm__ ("fmadd    %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fmadd   ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3D(__asm__ ("fmadd.   %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fmadd.  ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3F(__asm__ ("fmadds   %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fmadds  ",i,j,(i+j)%TESTSIZED,f0));
    ITERATE3F(__asm__ ("fmadds.  %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fmadds. ",i,j,(i+j)%TESTSIZED,f0));
    ITERATE3D(__asm__ ("fmsub    %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fmsub   ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3D(__asm__ ("fmsub.   %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fmsub.  ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3F(__asm__ ("fmsubs   %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fmsubs  ",i,j,(i+j)%TESTSIZED,f0));
    ITERATE3F(__asm__ ("fmsubs.  %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fmsubs. ",i,j,(i+j)%TESTSIZED,f0));
    ITERATE3D(__asm__ ("fnmadd   %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fnmadd  ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3D(__asm__ ("fnmadd.  %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fnmadd. ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3F(__asm__ ("fnmadds  %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fnmadds ",i,j,(i+j)%TESTSIZED,f0));
    ITERATE3F(__asm__ ("fnmadds. %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fnmadds.",i,j,(i+j)%TESTSIZED,f0));
    ITERATE3D(__asm__ ("fnmsub   %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fnmsub  ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3D(__asm__ ("fnmsub.  %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT3("fnmsub. ",i,j,(i+j)%TESTSIZED,d0));
    ITERATE3F(__asm__ ("fnmsubs  %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fnmsubs ",i,j,(i+j)%TESTSIZED,f0));
    ITERATE3F(__asm__ ("fnmsubs. %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT3("fnmsubs.",i,j,(i+j)%TESTSIZED,f0));

    return 0;
}
