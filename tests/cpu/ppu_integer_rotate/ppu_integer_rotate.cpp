/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include <stdio.h>

#include "../../../common/output.h"

// Show results
#define PRINT1(name, i, GPR) \
    fprintf(handler, name"([%02d]) -> %016llX [%08X]\n", i, *(uint64_t*)&GPR, getCR());
#define PRINT2(name, i, j, GPR) \
    fprintf(handler, name"([%02d],[%02d]) -> %016llX [%08X]\n", i, j, *(uint64_t*)&GPR, getCR());
#define PRINT3(name, i, j, k, GPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d]) -> %016llX [%08X]\n", i, j, k, *(uint64_t*)&GPR, getCR());
#define PRINT4(name, i, j, k, l, GPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d],[%02d]) -> %016llX [%08X]\n", i, j, k, l, *(uint64_t*)&GPR, getCR());

// Pair-wise coverage for 3 arguments
#define PAIRWISE3(name, a, b, c) \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(a), "i"(a), "i"(a));  PRINT4(name, i, a, a, a, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(b), "i"(b), "i"(a));  PRINT4(name, i, b, b, a, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(c), "i"(c), "i"(a));  PRINT4(name, i, c, c, a, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(b), "i"(a), "i"(b));  PRINT4(name, i, b, a, b, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(c), "i"(b), "i"(b));  PRINT4(name, i, c, b, b, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(a), "i"(c), "i"(b));  PRINT4(name, i, a, c, b, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(c), "i"(a), "i"(c));  PRINT4(name, i, c, a, c, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(a), "i"(b), "i"(c));  PRINT4(name, i, a, b, c, r0)); \
    ITERATE1(__asm__ (name " %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "i"(b), "i"(c), "i"(c));  PRINT4(name, i, b, c, c, r0));

// Iterate
#define ITERATE1(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        {uint64_t r0, r1=testInts64[i]; clearCR(); x;}

#define ITERATE2(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        for (uint32_t j=0; j<sizeof(testInts64)/sizeof(testInts64[0]); j++) \
            {uint64_t r0, r1=testInts64[i], r2=testInts64[j]; clearCR(); x;}

// Get contents of the CR register
uint32_t getCR()
{
    uint32_t CR;
    __asm__ ("mfcr %0" : "=r"(CR));
    return CR;
};

// Clear CR register
void clearCR()
{
    uint32_t CR = 0;
    __asm__ ("mtcr %0" : "=r"(CR));
};

uint64_t testInts64[] = {
    0x0000000000000000LL, 0x0000000000000001LL, 0x0000000000000002LL, 0xFFFFFFFFFFFFFFFFLL,  //  0  1  2  3
    0xFFFFFFFFFFFFFFFELL, 0x0003333300330033LL, 0x000000FFFFF00000LL, 0x1000000000000000LL,  //  4  5  6  7
    0x1FFFFFFFFFFFFFFFLL, 0x4238572200000000LL, 0x7000000000000000LL, 0x0000000072233411LL,  //  8  9 10 11
    0x7FFFFFFFFFFFFFFFLL, 0x8000000000000000LL, 0x8000000000000001LL, 0x0123456789ABCDEFLL,  // 12 13 14 15
};

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // Integer Rotate Instructions
    ITERATE2(__asm__ ("rldcl   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(0));  PRINT3("rldcl  ",i,j,0,r0));
    ITERATE2(__asm__ ("rldcl   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(1));  PRINT3("rldcl  ",i,j,1,r0));
    ITERATE2(__asm__ ("rldcl   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(63)); PRINT3("rldcl  ",i,j,63,r0));
    ITERATE2(__asm__ ("rldcl.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(0));  PRINT3("rldcl. ",i,j,0,r0));
    ITERATE2(__asm__ ("rldcl.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(1));  PRINT3("rldcl. ",i,j,1,r0));
    ITERATE2(__asm__ ("rldcl.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(63)); PRINT3("rldcl. ",i,j,63,r0));
    ITERATE2(__asm__ ("rldcr   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(0));  PRINT3("rldcr  ",i,j,0,r0));
    ITERATE2(__asm__ ("rldcr   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(1));  PRINT3("rldcr  ",i,j,1,r0));
    ITERATE2(__asm__ ("rldcr   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(63)); PRINT3("rldcr  ",i,j,63,r0));
    ITERATE2(__asm__ ("rldcr.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(0));  PRINT3("rldcr. ",i,j,0,r0));
    ITERATE2(__asm__ ("rldcr.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(1));  PRINT3("rldcr. ",i,j,1,r0));
    ITERATE2(__asm__ ("rldcr.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "r"(r2), "i"(63)); PRINT3("rldcr. ",i,j,63,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldic  ",i,0,0,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldic  ",i,0,1,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldic  ",i,0,63,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldic  ",i,1,0,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldic  ",i,1,1,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldic  ",i,1,63,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldic  ",i,63,0,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldic  ",i,63,1,r0));
    ITERATE1(__asm__ ("rldic   %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldic  ",i,63,63,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldic. ",i,0,0,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldic. ",i,0,1,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldic. ",i,0,63,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldic. ",i,1,0,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldic. ",i,1,1,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldic. ",i,1,63,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldic. ",i,63,0,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldic. ",i,63,1,r0));
    ITERATE1(__asm__ ("rldic.  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldic. ",i,63,63,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldicl ",i,0,0,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldicl ",i,0,1,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldicl ",i,0,63,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldicl ",i,1,0,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldicl ",i,1,1,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldicl ",i,1,63,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldicl ",i,63,0,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldicl ",i,63,1,r0));
    ITERATE1(__asm__ ("rldicl  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldicl ",i,63,63,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldicl.",i,0,0,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldicl.",i,0,1,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldicl.",i,0,63,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldicl.",i,1,0,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldicl.",i,1,1,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldicl.",i,1,63,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldicl.",i,63,0,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldicl.",i,63,1,r0));
    ITERATE1(__asm__ ("rldicl. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldicl.",i,63,63,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldicr ",i,0,0,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldicr ",i,0,1,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldicr ",i,0,63,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldicr ",i,1,0,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldicr ",i,1,1,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldicr ",i,1,63,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldicr ",i,63,0,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldicr ",i,63,1,r0));
    ITERATE1(__asm__ ("rldicr  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldicr ",i,63,63,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldicr.",i,0,0,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldicr.",i,0,1,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldicr.",i,0,63,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldicr.",i,1,0,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldicr.",i,1,1,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldicr.",i,1,63,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldicr.",i,63,0,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldicr.",i,63,1,r0));
    ITERATE1(__asm__ ("rldicr. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldicr.",i,63,63,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldimi ",i,0,0,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldimi ",i,0,1,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldimi ",i,0,63,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldimi ",i,1,0,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldimi ",i,1,1,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldimi ",i,1,63,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldimi ",i,63,0,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldimi ",i,63,1,r0));
    ITERATE1(__asm__ ("rldimi  %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldimi ",i,63,63,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(0));   PRINT3("rldimi.",i,0,0,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(1));   PRINT3("rldimi.",i,0,1,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(0), "i"(63));  PRINT3("rldimi.",i,0,63,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(0));   PRINT3("rldimi.",i,1,0,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(1));   PRINT3("rldimi.",i,1,1,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(1), "i"(63));  PRINT3("rldimi.",i,1,63,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(0));  PRINT3("rldimi.",i,63,0,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(1));  PRINT3("rldimi.",i,63,1,r0));
    ITERATE1(__asm__ ("rldimi. %0,%1,%2,%3" : "=r"(r0) : "r"(r1), "i"(63), "i"(63)); PRINT3("rldimi.",i,63,63,r0));
    PAIRWISE3("rlwimi ", 0, 1, 31);
    PAIRWISE3("rlwimi.", 0, 1, 31);
    PAIRWISE3("rlwinm ", 0, 1, 31);
    PAIRWISE3("rlwinm.", 0, 1, 31);
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 0), "i"( 0)); PRINT4("rlwnm  ",i,j, 0, 0, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 0), "i"( 1)); PRINT4("rlwnm  ",i,j, 0, 1, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 0), "i"(31)); PRINT4("rlwnm  ",i,j, 0,31, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 1), "i"( 0)); PRINT4("rlwnm  ",i,j, 1, 0, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 1), "i"( 1)); PRINT4("rlwnm  ",i,j, 1, 1, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 1), "i"(31)); PRINT4("rlwnm  ",i,j, 1,31, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"(31), "i"( 0)); PRINT4("rlwnm  ",i,j,31, 0, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"(31), "i"( 1)); PRINT4("rlwnm  ",i,j,31, 1, r0));
    ITERATE2(__asm__ ("rlwnm   %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"(31), "i"(31)); PRINT4("rlwnm  ",i,j,31,31, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 0), "i"( 0)); PRINT4("rlwnm. ",i,j, 0, 0, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 0), "i"( 1)); PRINT4("rlwnm. ",i,j, 0, 1, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 0), "i"(31)); PRINT4("rlwnm. ",i,j, 0,31, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 1), "i"( 0)); PRINT4("rlwnm. ",i,j, 1, 0, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 1), "i"( 1)); PRINT4("rlwnm. ",i,j, 1, 1, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"( 1), "i"(31)); PRINT4("rlwnm. ",i,j, 1,31, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"(31), "i"( 0)); PRINT4("rlwnm. ",i,j,31, 0, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"(31), "i"( 1)); PRINT4("rlwnm. ",i,j,31, 1, r0));
    ITERATE2(__asm__ ("rlwnm.  %0,%1,%2,%3,%4" : "=r"(r0) : "r"(r1), "r"(r2), "i"(31), "i"(31)); PRINT4("rlwnm. ",i,j,31,31, r0));

    return 0;
}
