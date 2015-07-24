/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */
 
#include <stdio.h>

#include "../../../common/output.h"

#define TEST_BRANCH(branch) { \
    uint32_t branched = 0;                         \
    __asm__ (                                      \
        "   xor     %0, %0, %0;  "                 \
            branch          "1f; "                 \
        "   xori    %0, %0, 0x1; "                 \
        "1: xori    %0, %0, 0x1; "                 \
        : "=r"(branched));                         \
    fprintf(handler, branch " -> %d\n", branched); \
}

#define TEST_BRANCH_COND(branch, values) \
    for (uint32_t i = 0; i < sizeof(values)/sizeof(values[0]); i++) {   \
        uint32_t branched = 0;                                          \
        __asm__ (                                                       \
            "   mtcr    %1;          "                                  \
            "   xor     %0, %0, %0;  "                                  \
                branch  ", 1f;       "                                  \
            "   xori    %0, %0, 0x1; "                                  \
            "1: xori    %0, %0, 0x1; "                                  \
            : "=r"(branched) : "r"(values[i]));                         \
        fprintf(handler, branch "[%08X] -> %d\n", values[i], branched); \
    }

#define TEST_BRANCH_CONDS(suffix, reg, values)  \
    TEST_BRANCH_COND("beq" suffix " " reg, values);  \
    TEST_BRANCH_COND("bge" suffix " " reg, values);  \
    TEST_BRANCH_COND("bgt" suffix " " reg, values);  \
    TEST_BRANCH_COND("ble" suffix " " reg, values);  \
    TEST_BRANCH_COND("blt" suffix " " reg, values);  \
    TEST_BRANCH_COND("bnl" suffix " " reg, values);  \
    TEST_BRANCH_COND("bne" suffix " " reg, values);  \
    TEST_BRANCH_COND("bng" suffix " " reg, values);  \
    TEST_BRANCH_COND("bns" suffix " " reg, values);  \
    TEST_BRANCH_COND("bnu" suffix " " reg, values);  \
    TEST_BRANCH_COND("bso" suffix " " reg, values);  \
    TEST_BRANCH_COND("bun" suffix " " reg, values);

// CR0
uint32_t cr0Values[] = {
    0x00000000, 0x10000000, 0x20000000, 0x30000000,  //  0  1  2  3
    0x40000000, 0x50000000, 0x60000000, 0x70000000,  //  4  5  6  7
    0x80000000, 0x90000000, 0xA0000000, 0xB0000000,  //  8  9 10 11
    0xC0000000, 0xD0000000, 0xE0000000, 0xF0000000,  // 12 13 14 15
};

// CR6
uint32_t cr6Values[] = {
    0x00000000, 0x00000010, 0x00000020, 0x00000030,  //  0  1  2  3
    0x00000040, 0x00000050, 0x00000060, 0x00000070,  //  4  5  6  7
    0x00000080, 0x00000090, 0x000000A0, 0x000000B0,  //  8  9 10 11
    0x000000C0, 0x000000D0, 0x000000E0, 0x000000F0,  // 12 13 14 15
};

// CR7
uint32_t cr7Values[] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003,  //  0  1  2  3
    0x00000004, 0x00000005, 0x00000006, 0x00000007,  //  4  5  6  7
    0x00000008, 0x00000009, 0x0000000A, 0x0000000B,  //  8  9 10 11
    0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F,  // 12 13 14 15
};

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // Branch Instructions
    TEST_BRANCH("b  ");
    TEST_BRANCH("bl ");
    TEST_BRANCH_CONDS(" ", "cr0", cr0Values);
    TEST_BRANCH_CONDS(" ", "cr6", cr6Values);
    TEST_BRANCH_CONDS(" ", "cr7", cr7Values);
    TEST_BRANCH_CONDS("+", "cr0", cr0Values);
    TEST_BRANCH_CONDS("+", "cr6", cr6Values);
    TEST_BRANCH_CONDS("+", "cr7", cr7Values);
    TEST_BRANCH_CONDS("-", "cr0", cr0Values);
    TEST_BRANCH_CONDS("-", "cr6", cr6Values);
    TEST_BRANCH_CONDS("-", "cr7", cr7Values);

    return 0;
}
