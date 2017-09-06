/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include <stdio.h>

#include "../../../common/output.h"

// Show results
#define PRINT1(name, i, GPR) \
    fprintf(handler, name"([%02d]) -> %016llX [%016llX : %08X]\n", i, *(uint64_t*)&GPR, getXER(), getCR());
#define PRINT2(name, i, j, GPR) \
    fprintf(handler, name"([%02d],[%02d]) -> %016llX [%016llX : %08X]\n", i, j, *(uint64_t*)&GPR, getXER(), getCR());
#define PRINT3(name, i, j, k, GPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d]) -> %016llX [%016llX : %08X]\n", i, j, k, *(uint64_t*)&GPR, getXER(), getCR());
#define PRINT4(name, i, j, k, l, GPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d],[%02d]) -> %016llX [%016llX : %08X]\n", i, j, k, l, *(uint64_t*)&GPR, getXER(), getCR());

// Iterate
#define ITERATE1(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        {uint64_t r0, r1=testInts64[i]; clearCR(); clearXER(); x;}

#define ITERATE2(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        for (uint32_t j=0; j<sizeof(testInts64)/sizeof(testInts64[0]); j++) \
            {uint64_t r0, r1=testInts64[i], r2=testInts64[j]; clearCR(); clearXER(); x;}

// Get contents of the CR register
uint32_t getCR()
{
    uint32_t CR;
    __asm__ ("mfcr %0" : "=r"(CR));
    return CR;
};

// Get contents of the XER register
uint64_t getXER()
{
    uint64_t XER;
    __asm__ ("mfxer %0" : "=r"(XER));
    return XER;
};

// Clear CR register
void clearCR()
{
    uint32_t CR = 0;
    __asm__ ("mtcr %0" : "=r"(CR));
};

// Clear XER register
void clearXER()
{
    uint64_t XER = 0;
    __asm__ ("mtxer %0" : "=r"(XER));
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

    // Integer Arithmetic Instructions
    ITERATE2(__asm__ ("add     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("add    ",i,j,r0));
    ITERATE2(__asm__ ("add.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("add.   ",i,j,r0));
    ITERATE2(__asm__ ("addc    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("addc   ",i,j,r0));
    ITERATE2(__asm__ ("addc.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("addc.  ",i,j,r0));
    ITERATE2(__asm__ ("adde    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("adde   ",i,j,r0));
    ITERATE2(__asm__ ("adde.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("adde.  ",i,j,r0));
    ITERATE1(__asm__ ("addi    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("addi   ",i,0,r0));
    ITERATE1(__asm__ ("addi    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("addi   ",i,1,r0));
    ITERATE1(__asm__ ("addi    %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(-1));  PRINT2("addi   ",i,-1,r0));
    ITERATE1(__asm__ ("addic   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("addic  ",i,0,r0));
    ITERATE1(__asm__ ("addic   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("addic  ",i,1,r0));
    ITERATE1(__asm__ ("addic   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(-1));  PRINT2("addic  ",i,-1,r0));
    ITERATE1(__asm__ ("addic.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("addic. ",i,0,r0));
    ITERATE1(__asm__ ("addic.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("addic. ",i,1,r0));
    ITERATE1(__asm__ ("addic.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(-1));  PRINT2("addic. ",i,-1,r0));
    ITERATE1(__asm__ ("addis   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("addis  ",i,0,r0));
    ITERATE1(__asm__ ("addis   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("addis  ",i,1,r0));
    ITERATE1(__asm__ ("addis   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(-1));  PRINT2("addis  ",i,-1,r0));
    ITERATE1(__asm__ ("addme   %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("addme  ",i,r0));
    ITERATE1(__asm__ ("addme.  %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("addme. ",i,r0));
    ITERATE1(__asm__ ("addze   %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("addze  ",i,r0));
    ITERATE1(__asm__ ("addze.  %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("addze. ",i,r0));
    ITERATE2(__asm__ ("divd    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divd   ",i,j,r0));
    ITERATE2(__asm__ ("divd.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divd.  ",i,j,r0));
    ITERATE2(__asm__ ("divdu   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divdu  ",i,j,r0));
    ITERATE2(__asm__ ("divdu.  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divdu. ",i,j,r0));
    ITERATE2(__asm__ ("divw    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divw   ",i,j,r0));
    ITERATE2(__asm__ ("divw.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divw.  ",i,j,r0));
    ITERATE2(__asm__ ("divwu   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divwu  ",i,j,r0));
    ITERATE2(__asm__ ("divwu.  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("divwu. ",i,j,r0));
    ITERATE2(__asm__ ("mulhd   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhd  ",i,j,r0));
    ITERATE2(__asm__ ("mulhd.  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhd. ",i,j,r0));
    ITERATE2(__asm__ ("mulhdu  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhdu ",i,j,r0));
    ITERATE2(__asm__ ("mulhdu. %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhdu.",i,j,r0));
    ITERATE2(__asm__ ("mulhw   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhw  ",i,j,r0));
    ITERATE2(__asm__ ("mulhw.  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhw. ",i,j,r0));
    ITERATE2(__asm__ ("mulhwu  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhwu ",i,j,r0));
    ITERATE2(__asm__ ("mulhwu. %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulhwu.",i,j,r0));
    ITERATE2(__asm__ ("mulld   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mulld  ",i,j,r0));
    ITERATE1(__asm__ ("mulli   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("mulli  ",i,0,r0));
    ITERATE1(__asm__ ("mulli   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("mulli  ",i,1,r0));
    ITERATE1(__asm__ ("mulli   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(-1));  PRINT2("mulli  ",i,-1,r0));
    ITERATE2(__asm__ ("mullw   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mullw  ",i,j,r0));
    ITERATE2(__asm__ ("mullw.  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("mullw. ",i,j,r0));
    ITERATE1(__asm__ ("neg     %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("neg    ",i,r0));
    ITERATE1(__asm__ ("neg.    %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("neg.   ",i,r0));
    ITERATE2(__asm__ ("subf    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("subf   ",i,j,r0));
    ITERATE2(__asm__ ("subf.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("subf.  ",i,j,r0));
    ITERATE2(__asm__ ("subfc   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("subfc  ",i,j,r0));
    ITERATE2(__asm__ ("subfc.  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("subfc. ",i,j,r0));
    ITERATE1(__asm__ ("subfic  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("subfic ",i,0,r0));
    ITERATE1(__asm__ ("subfic  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("subfic ",i,1,r0));
    ITERATE1(__asm__ ("subfic  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(-1));  PRINT2("subfic ",i,-1,r0));
    ITERATE2(__asm__ ("subfe   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("subfe  ",i,j,r0));
    ITERATE2(__asm__ ("subfe.  %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("subfe. ",i,j,r0));
    ITERATE1(__asm__ ("subfme  %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("subfme ",i,r0));
    ITERATE1(__asm__ ("subfme. %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("subfme.",i,r0));
    ITERATE1(__asm__ ("subfze  %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("subfze ",i,r0));
    ITERATE1(__asm__ ("subfze. %0,%1"    : "=r"(r0) : "r"(r1));           PRINT1("subfze.",i,r0));

    return 0;
}
