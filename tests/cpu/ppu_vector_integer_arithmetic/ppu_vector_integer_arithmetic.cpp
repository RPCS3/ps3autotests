/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <altivec.h>

#include "../../../common/output.h"

// Show results
#define PRINT1(name, i, VPR) \
    fprintf(handler, name"([%02d]) -> %08X %08X %08X %08X [%08X]\n", i, \
        ((vec_int4)VPR)[0], ((vec_int4)VPR)[1], ((vec_int4)VPR)[2], ((vec_int4)VPR)[3], getVSCR());
#define PRINT2(name, i, j, VPR) \
    fprintf(handler, name"([%02d],[%02d]) -> %08X %08X %08X %08X [%08X]\n", i, j, \
        ((vec_int4)VPR)[0], ((vec_int4)VPR)[1], ((vec_int4)VPR)[2], ((vec_int4)VPR)[3], getVSCR());
#define PRINT3(name, i, j, k, GPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d]) -> %08X %08X %08X %08X [%08X]\n", i, j, k, \
        ((vec_int4)VPR)[0], ((vec_int4)VPR)[1], ((vec_int4)VPR)[2], ((vec_int4)VPR)[3], getVSCR());
#define PRINT4(name, i, j, k, l, GPR) \
    fprintf(handler, name"([%02d],[%02d],[%02d],[%02d]) -> %08X %08X %08X %08X [%08X]\n", i, j, k, l, \
        ((vec_int4)VPR)[0], ((vec_int4)VPR)[1], ((vec_int4)VPR)[2], ((vec_int4)VPR)[3], getVSCR());

// Iterate
#define ITERATE2I(x) \
    for (uint32_t i=0; i<sizeof(testInts)/sizeof(testInts[0]); i++) \
        for (uint32_t j=0; j<sizeof(testInts)/sizeof(testInts[0]); j++) \
            {vec_int4 v1sw = (vec_int4)(testInts[i]); \
            vec_int4 v2sw = (vec_int4)(testInts[j], testInts[j], 0.0, 0.0); \
            vec_short8 v1sh = *(vec_short8 *)&v1sw; \
            vec_short8 v2sh = *(vec_short8 *)&v2sw; \
            vec_char16 v1sb = *(vec_char16 *)&v1sw; \
            vec_char16 v2sb = *(vec_char16 *)&v2sw; \
            vec_uint4 v1uw = (vec_uint4)(testInts[i]); \
            vec_uint4 v2uw = (vec_uint4)(testInts[j], testInts[j], 0.0, 0.0); \
            vec_ushort8 v1uh = *(vec_ushort8 *)&v1uw; \
            vec_ushort8 v2uh = *(vec_ushort8 *)&v2uw; \
            vec_uchar16 v1ub = *(vec_uchar16 *)&v1uw; \
            vec_uchar16 v2ub = *(vec_uchar16 *)&v2uw; \
            clearVSCR(); x;}

// Get contents of the VSCR register
uint32_t getVSCR()
{
    return ((vec_int4)vec_mfvscr())[3];
};

// Clear VSCR register
void clearVSCR()
{
    vec_ushort8 VSCR = (vec_ushort8)();
    vec_mtvscr(VSCR);
};

const uint32_t testInts[] = {
    0x00000000, 0x00000001, 0x00000002, 0xffffffff, //  0  1  2  3
    0xfffffffe, 0xfffffffd, 0x00033333, 0x00fffff0, //  4  5  6  7
    0x10000000, 0x12345678, 0x1fffffff, 0x42385722, //  8  9 10 11
    0x7fffffff, 0x80000000, 0x80000001, 0x8fffffff, // 12 13 14 15
};

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // Vector Integer Arithmetic Instructions
    ITERATE2I(PRINT2("vaddsbs  ", i, j, vec_vaddsbs(v1sb,v2sb)));
    ITERATE2I(PRINT2("vaddshs  ", i, j, vec_vaddshs(v1sh,v2sh)));
    ITERATE2I(PRINT2("vaddsws  ", i, j, vec_vaddsws(v1sw,v2sw)));
    ITERATE2I(PRINT2("vaddubm  ", i, j, vec_vaddubm(v1ub,v2ub)));
    ITERATE2I(PRINT2("vadduhm  ", i, j, vec_vadduhm(v1uh,v2uh)));
    ITERATE2I(PRINT2("vadduwm  ", i, j, vec_vadduwm(v1uw,v2uw)));
    ITERATE2I(PRINT2("vaddubs  ", i, j, vec_vaddubs(v1ub,v2ub)));
    ITERATE2I(PRINT2("vadduhs  ", i, j, vec_vadduhs(v1uh,v2uh)));
    ITERATE2I(PRINT2("vadduws  ", i, j, vec_vadduws(v1uw,v2uw)));
    ITERATE2I(PRINT2("vaddcuw  ", i, j, vec_vaddcuw(v1uw,v2uw)));
    ITERATE2I(PRINT2("vsubsbs  ", i, j, vec_vsubsbs(v1sb,v2sb)));
    ITERATE2I(PRINT2("vsubshs  ", i, j, vec_vsubshs(v1sh,v2sh)));
    ITERATE2I(PRINT2("vsubsws  ", i, j, vec_vsubsws(v1sw,v2sw)));
    ITERATE2I(PRINT2("vsububm  ", i, j, vec_vsububm(v1ub,v2ub)));
    ITERATE2I(PRINT2("vsubuhm  ", i, j, vec_vsubuhm(v1uh,v2uh)));
    ITERATE2I(PRINT2("vsubuwm  ", i, j, vec_vsubuwm(v1uw,v2uw)));
    ITERATE2I(PRINT2("vsububs  ", i, j, vec_vsububs(v1ub,v2ub)));
    ITERATE2I(PRINT2("vsubuhs  ", i, j, vec_vsubuhs(v1uh,v2uh)));
    ITERATE2I(PRINT2("vsubuws  ", i, j, vec_vsubuws(v1uw,v2uw)));
    ITERATE2I(PRINT2("vsubcuw  ", i, j, vec_vsubcuw(v1uw,v2uw)));
    ITERATE2I(PRINT2("vmuloub  ", i, j, vec_vmuloub(v1ub,v2ub)));
    ITERATE2I(PRINT2("vmulouh  ", i, j, vec_vmulouh(v1uh,v2uh)));
    ITERATE2I(PRINT2("vmulosb  ", i, j, vec_vmulosb(v1sb,v2sb)));
    ITERATE2I(PRINT2("vmulosh  ", i, j, vec_vmulosh(v1sh,v2sh)));
    ITERATE2I(PRINT2("vmuleub  ", i, j, vec_vmuleub(v1ub,v2ub)));
    ITERATE2I(PRINT2("vmuleuh  ", i, j, vec_vmuleuh(v1uh,v2uh)));
    ITERATE2I(PRINT2("vmulesb  ", i, j, vec_vmulesb(v1sb,v2sb)));
    ITERATE2I(PRINT2("vmulesh  ", i, j, vec_vmulesh(v1sh,v2sh)));
    ITERATE2I(PRINT2("vsumsws  ", i, j, vec_vsumsws(v1sw,v2sw)));
    ITERATE2I(PRINT2("vsum2sws ", i, j, vec_vsum2sws(v1sw,v2sw)));
    ITERATE2I(PRINT2("vsum4ubs ", i, j, vec_vsum4ubs(v1ub,v2uw)));
    ITERATE2I(PRINT2("vsum4sbs ", i, j, vec_vsum4sbs(v1sb,v2sw)));
    ITERATE2I(PRINT2("vsum4shs ", i, j, vec_vsum4shs(v1sh,v2sw)));
    ITERATE2I(PRINT2("vavgub   ", i, j, vec_vavgub(v1ub,v2ub)));
    ITERATE2I(PRINT2("vavguh   ", i, j, vec_vavguh(v1uh,v2uh)));
    ITERATE2I(PRINT2("vavguw   ", i, j, vec_vavguw(v1uw,v2uw)));
    ITERATE2I(PRINT2("vavgsb   ", i, j, vec_vavgsb(v1sb,v2sb)));
    ITERATE2I(PRINT2("vavgsh   ", i, j, vec_vavgsh(v1sh,v2sh)));
    ITERATE2I(PRINT2("vavgsw   ", i, j, vec_vavgsw(v1sw,v2sw)));
    ITERATE2I(PRINT2("vmaxub   ", i, j, vec_vmaxub(v1ub,v2ub)));
    ITERATE2I(PRINT2("vmaxuh   ", i, j, vec_vmaxuh(v1uh,v2uh)));
    ITERATE2I(PRINT2("vmaxuw   ", i, j, vec_vmaxuw(v1uw,v2uw)));
    ITERATE2I(PRINT2("vmaxsb   ", i, j, vec_vmaxsb(v1sb,v2sb)));
    ITERATE2I(PRINT2("vmaxsh   ", i, j, vec_vmaxsh(v1sh,v2sh)));
    ITERATE2I(PRINT2("vmaxsw   ", i, j, vec_vmaxsw(v1sw,v2sw)));
    ITERATE2I(PRINT2("vminub   ", i, j, vec_vminub(v1ub,v2ub)));
    ITERATE2I(PRINT2("vminuh   ", i, j, vec_vminuh(v1uh,v2uh)));
    ITERATE2I(PRINT2("vminuw   ", i, j, vec_vminuw(v1uw,v2uw)));
    ITERATE2I(PRINT2("vminsb   ", i, j, vec_vminsb(v1sb,v2sb)));
    ITERATE2I(PRINT2("vminsh   ", i, j, vec_vminsh(v1sh,v2sh)));
    ITERATE2I(PRINT2("vminsw   ", i, j, vec_vminsw(v1sw,v2sw)));

    return 0;
}
