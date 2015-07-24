/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include <stdio.h>

#include "../../../common/output.h"

// Instructions: XXX rS,d(rA) without update
#define TEST_STORE1(instr, addr) {                               \
    uint32_t v[3] = {};                                          \
    __asm__ (                                                    \
        "fmr  0, %0;   " /* fr0 = value */                       \
        "mr  11, %1;   " /* r11 = addr */                        \
        instr     ";   "                                         \
        : : "f"((double&)n), "r"(addr));                         \
    fprintf(handler, instr " -> %08X %08X %08X\n",               \
        v[0], v[1], v[2]);                                       \
}

// Instructions: XXX rS,d(rA) with update
#define TEST_STORE2(instr, addr) {                               \
    uint64_t res;                                                \
    uint32_t v[3] = {};                                          \
    __asm__ (                                                    \
        "fmr  0, %1;   " /* fr0 = value */                       \
        "mr  11, %2;   " /* r11 = addr */                        \
        instr     ";   "                                         \
        "mr  %0, 11;   " /* res = r11 */                         \
        : "=r"(res) : "f"((double&)n), "r"(addr));               \
    fprintf(handler, instr " -> %08X %08X %08X [%016llX]\n",     \
        v[0], v[1], v[2], (res - (uint64_t)addr));               \
}

// Instructions: XXX rS,rA,rB without update
#define TEST_STORE3(instr, rA, rB) {                             \
    uint32_t v[3] = {};                                          \
    __asm__ (                                                    \
        "fmr  0, %0;   " /* fr0 = value */                       \
        "mr  11, %1;   " /* r11 = rA */                          \
        "mr  12, %2;   " /* r12 = rB */                          \
        instr     ";   "                                         \
        : : "f"((double&)n), "r"(rA), "r"(rB));                  \
    fprintf(handler, instr " -> %08X %08X %08X\n",               \
        v[0], v[1], v[2]);                                       \
}

// Instructions: XXX rS,rA,rB with update
#define TEST_STORE4(instr, rA, rB) {                             \
    uint64_t res;                                                \
    uint32_t v[3] = {};                                          \
    __asm__ (                                                    \
        "fmr  0, %1;   " /* fr0 = value */                       \
        "mr  11, %2;   " /* r11 = rA */                          \
        "mr  12, %3;   " /* r12 = rB */                          \
        instr     ";   "                                         \
        "mr  %0, 11;   " /* res = r11 */                         \
        : "=r"(res) : "f"((double&)n), "r"(rA), "r"(rB));        \
    fprintf(handler, instr " -> %08X %08X %08X [%016llX]\n",     \
        v[0], v[1], v[2], (res - (uint64_t)rA - (uint64_t)rB));  \
}

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // Test double
    uint64_t n = 0x3F80123456789ABCULL;

    // Floating-Point Store Instructions (Single)
    TEST_STORE1("stfs    0,  0(11)", &v[0]);
    TEST_STORE1("stfs    0,  8(11)", &v[0]);
    TEST_STORE1("stfs    0, -4(11)", &v[2]);
    TEST_STORE2("stfsu   0,  0(11)", &v[0]);
    TEST_STORE2("stfsu   0,  8(11)", &v[0]);
    TEST_STORE2("stfsu   0, -4(11)", &v[2]);
    TEST_STORE3("stfsx   0, 11, 12", &v[0],  0);
    TEST_STORE3("stfsx   0, 11, 12", &v[0],  8);
    TEST_STORE3("stfsx   0, 11, 12", &v[2], -4);
    TEST_STORE3("stfsx   0, 11, 12", 0,  &v[0]);
    TEST_STORE4("stfsux  0, 11, 12", &v[0],  0);
    TEST_STORE4("stfsux  0, 11, 12", &v[0],  8);
    TEST_STORE4("stfsux  0, 11, 12", &v[2], -4);
    TEST_STORE4("stfsux  0, 11, 12", 0,  &v[0]);

    // Floating-Point Store Instructions (Double)
    TEST_STORE1("stfd    0,  0(11)", &v[0]);
    TEST_STORE1("stfd    0,  4(11)", &v[0]);
    TEST_STORE1("stfd    0, -4(11)", &v[1]);
    TEST_STORE2("stfdu   0,  0(11)", &v[0]);
    TEST_STORE2("stfdu   0,  4(11)", &v[0]);
    TEST_STORE2("stfdu   0, -4(11)", &v[1]);
    TEST_STORE3("stfdx   0, 11, 12", &v[0],  0);
    TEST_STORE3("stfdx   0, 11, 12", &v[0],  4);
    TEST_STORE3("stfdx   0, 11, 12", &v[2], -4);
    TEST_STORE3("stfdx   0, 11, 12", 0,  &v[0]);
    TEST_STORE4("stfdux  0, 11, 12", &v[0],  0);
    TEST_STORE4("stfdux  0, 11, 12", &v[0],  4);
    TEST_STORE4("stfdux  0, 11, 12", &v[2], -4);
    TEST_STORE4("stfdux  0, 11, 12", 0,  &v[0]);

    // Floating-Point Store Instructions (Other)
    TEST_STORE3("stfiwx  0, 11, 12", &v[0],  0);
    TEST_STORE3("stfiwx  0, 11, 12", &v[0],  4);
    TEST_STORE3("stfiwx  0, 11, 12", &v[2], -4);
    TEST_STORE3("stfiwx  0, 11, 12", 0,  &v[0]);

    return 0;
}
