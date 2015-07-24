/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include <stdio.h>

#include "../../../common/output.h"

// Instructions: XXX rS,d(rA) without update
#define TEST_LOAD1(instr, addr) {                                \
    double n;                                                    \
    __asm__ (                                                    \
        "mr  11, %1;   " /* r11 = addr */                        \
        instr     ";   "                                         \
        "fmr %0,  0;   " /* value = fr0 */                       \
        : "=f"((double&)n) : "r"(addr));                         \
    fprintf(handler, instr " -> %016llX\n", (uint64_t&)n);       \
}

// Instructions: XXX rS,d(rA) with update
#define TEST_LOAD2(instr, addr) {                                \
    uint64_t res;                                                \
    double n;                                                    \
    __asm__ (                                                    \
        "mr  11, %2;   " /* r11 = addr */                        \
        instr     ";   "                                         \
        "fmr %0,  0;   " /* value = fr0 */                       \
        "mr  %1, 11;   " /* res = r11 */                         \
        : "=f"((double&)n), "=r"(res) : "r"(addr));              \
    fprintf(handler, instr " -> %016llX [%016llX]\n",            \
        (uint64_t&)n, res - (uint64_t)addr);                     \
}

// Instructions: XXX rS,rA,rB without update
#define TEST_LOAD3(instr, rA, rB) {                              \
    double n;                                                    \
    __asm__ (                                                    \
        "mr  11, %1;   " /* r11 = rA */                          \
        "mr  12, %2;   " /* r12 = rB */                          \
        instr     ";   "                                         \
        "fmr %0,  0;   " /* value = fr0 */                       \
        : "=f"((double&)n) : "r"(rA), "r"(rB));                  \
    fprintf(handler, instr " -> %016llX\n", (uint64_t&)n);       \
}

// Instructions: XXX rS,rA,rB with update
#define TEST_LOAD4(instr, rA, rB) {                              \
    uint64_t res;                                                \
    double n;                                                    \
    __asm__ (                                                    \
        "mr  11, %2;   " /* r11 = rA */                          \
        "mr  12, %3;   " /* r12 = rB */                          \
        instr     ";   "                                         \
        "fmr %0,  0;   " /* value = fr0 */                       \
        "mr  %1, 11;   " /* res = r11 */                         \
        : "=f"((double&)n), "=r"(res) : "r"(rA), "r"(rB));       \
    fprintf(handler, instr " -> %016llX [%016llX]\n",            \
        (uint64_t&)n, (res - (uint64_t)rA - (uint64_t)rB));      \
}

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // Test double data
    uint32_t v[3] = {0x3F801234, 0x56789ABC, 0xDEF01234};

    // Floating-Point Load Instructions (Single)
    TEST_LOAD1("lfs    0,  0(11)", &v[0]);
    TEST_LOAD1("lfs    0,  8(11)", &v[0]);
    TEST_LOAD1("lfs    0, -4(11)", &v[2]);
    TEST_LOAD2("lfsu   0,  0(11)", &v[0]);
    TEST_LOAD2("lfsu   0,  8(11)", &v[0]);
    TEST_LOAD2("lfsu   0, -4(11)", &v[2]);
    TEST_LOAD3("lfsx   0, 11, 12", &v[0],  0);
    TEST_LOAD3("lfsx   0, 11, 12", &v[0],  8);
    TEST_LOAD3("lfsx   0, 11, 12", &v[2], -4);
    TEST_LOAD3("lfsx   0, 11, 12", 0,  &v[0]);
    TEST_LOAD4("lfsux  0, 11, 12", &v[0],  0);
    TEST_LOAD4("lfsux  0, 11, 12", &v[0],  8);
    TEST_LOAD4("lfsux  0, 11, 12", &v[2], -4);
    TEST_LOAD4("lfsux  0, 11, 12", 0,  &v[0]);

    // Floating-Point Load Instructions (Double)
    TEST_LOAD1("lfd    0,  0(11)", &v[0]);
    TEST_LOAD1("lfd    0,  4(11)", &v[0]);
    TEST_LOAD1("lfd    0, -4(11)", &v[1]);
    TEST_LOAD2("lfdu   0,  0(11)", &v[0]);
    TEST_LOAD2("lfdu   0,  4(11)", &v[0]);
    TEST_LOAD2("lfdu   0, -4(11)", &v[1]);
    TEST_LOAD3("lfdx   0, 11, 12", &v[0],  0);
    TEST_LOAD3("lfdx   0, 11, 12", &v[0],  4);
    TEST_LOAD3("lfdx   0, 11, 12", &v[2], -4);
    TEST_LOAD3("lfdx   0, 11, 12", 0,  &v[0]);
    TEST_LOAD4("lfdux  0, 11, 12", &v[0],  0);
    TEST_LOAD4("lfdux  0, 11, 12", &v[0],  4);
    TEST_LOAD4("lfdux  0, 11, 12", &v[2], -4);
    TEST_LOAD4("lfdux  0, 11, 12", 0,  &v[0]);

    return 0;
}
