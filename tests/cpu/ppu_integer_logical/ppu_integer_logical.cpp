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

    // Integer Logical Instructions
    ITERATE2(__asm__ ("and     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("and    ",i,j,r0));
    ITERATE2(__asm__ ("and.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("and.   ",i,j,r0));
    ITERATE2(__asm__ ("andc    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("andc   ",i,j,r0));
    ITERATE2(__asm__ ("andc.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("andc.  ",i,j,r0));
    ITERATE1(__asm__ ("andi.   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0000));  PRINT2("andi.  ",i,0,r0));
    ITERATE1(__asm__ ("andi.   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0001));  PRINT2("andi.  ",i,1,r0));
    ITERATE1(__asm__ ("andi.   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0xFFFF));  PRINT2("andi.  ",i,0xFFFF,r0));
    ITERATE1(__asm__ ("andis.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0000));  PRINT2("andis. ",i,0,r0));
    ITERATE1(__asm__ ("andis.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0001));  PRINT2("andis. ",i,1,r0));
    ITERATE1(__asm__ ("andis.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0xFFFF));  PRINT2("andis. ",i,0xFFFF,r0));
    ITERATE1(__asm__ ("cntlzd  %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("cntlzd ",i,r0));
    ITERATE1(__asm__ ("cntlzd. %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("cntlzd.",i,r0));
    ITERATE1(__asm__ ("cntlzw  %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("cntlzw ",i,r0));
    ITERATE1(__asm__ ("cntlzw. %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("cntlzw.",i,r0));
    ITERATE2(__asm__ ("eqv     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("eqv    ",i,j,r0));
    ITERATE2(__asm__ ("eqv.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("eqv.   ",i,j,r0));
    ITERATE1(__asm__ ("extsb   %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("extsb  ",i,r0));
    ITERATE1(__asm__ ("extsb.  %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("extsb. ",i,r0));
    ITERATE1(__asm__ ("extsh   %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("extsh  ",i,r0));
    ITERATE1(__asm__ ("extsh.  %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("extsh. ",i,r0));
    ITERATE1(__asm__ ("extsw   %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("extsw  ",i,r0));
    ITERATE1(__asm__ ("extsw.  %0,%1" : "=r"(r0) : "r"(r1));                  PRINT1("extsw. ",i,r0));
    ITERATE2(__asm__ ("nand    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("nand   ",i,j,r0));
    ITERATE2(__asm__ ("nand.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("nand.  ",i,j,r0));
    ITERATE2(__asm__ ("nor     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("nor    ",i,j,r0));
    ITERATE2(__asm__ ("nor.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("nor.   ",i,j,r0));
    ITERATE2(__asm__ ("or      %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("or     ",i,j,r0));
    ITERATE2(__asm__ ("or.     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("or.    ",i,j,r0));
    ITERATE2(__asm__ ("orc     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("orc    ",i,j,r0));
    ITERATE2(__asm__ ("orc.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("orc.   ",i,j,r0));
    ITERATE1(__asm__ ("ori     %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0000));  PRINT2("ori    ",i,0,r0));
    ITERATE1(__asm__ ("ori     %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0001));  PRINT2("ori    ",i,1,r0));
    ITERATE1(__asm__ ("ori     %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0xFFFF));  PRINT2("ori    ",i,0xFFFF,r0));
    ITERATE1(__asm__ ("oris    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0000));  PRINT2("oris   ",i,0,r0));
    ITERATE1(__asm__ ("oris    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0001));  PRINT2("oris   ",i,1,r0));
    ITERATE1(__asm__ ("oris    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0xFFFF));  PRINT2("oris   ",i,0xFFFF,r0));
    ITERATE2(__asm__ ("xor     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("xor    ",i,j,r0));
    ITERATE2(__asm__ ("xor.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));      PRINT2("xor.   ",i,j,r0));
    ITERATE1(__asm__ ("xori    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0000));  PRINT2("xori   ",i,0,r0));
    ITERATE1(__asm__ ("xori    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0001));  PRINT2("xori   ",i,1,r0));
    ITERATE1(__asm__ ("xori    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0xFFFF));  PRINT2("xori   ",i,0xFFFF,r0));
    ITERATE1(__asm__ ("xoris   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0000));  PRINT2("xoris  ",i,0,r0));
    ITERATE1(__asm__ ("xoris   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0x0001));  PRINT2("xoris  ",i,1,r0));
    ITERATE1(__asm__ ("xoris   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0xFFFF));  PRINT2("xoris  ",i,0xFFFF,r0));

    return 0;
}
