#include <stdio.h>
#include <math.h>
#include <float.h>
#include <altivec.h>

#define PRINT_VECTOR(v0)  printf("%08x %08x %08x %08x\n", ((vec_int4)v0)[0], ((vec_int4)v0)[1], ((vec_int4)v0)[2], ((vec_int4)v0)[3]);
#define PRINT_MEMORY(p,N) for(int i=0; i<N; i++){ printf("%02hhx", ((char*)(p))[i] & 0xff); } printf("\n");

int testInts[] = {
	0, 1, 2, 3, 4, -1, -2, -3, -4,
	0x00033333, 0x00fffff0, 0x10000000, 0x12345678,
	0x1fffffff, 0x42385722, 0x70000000, 0x72233411,
	0x7fffffff, 0x80000000, 0x80000001, 0x8fffffff,
};

float testFloats[] = {
	0.0, -0.0,  1.0, -1.0,  1.5, -1.5,
	1.6, -1.6,  1.4, -1.4,  2.0, -2.0,
	4.0, -10000000.4, 20000000.0, -20000.5, 20000.6,
	FLT_MIN, FLT_MAX, INFINITY, -INFINITY, NAN,
};

int main(void)
{
	/////////////////////////////////
	// VECTOR INTEGER INSTRUCTIONS //
	/////////////////////////////////

	// Input arguments: 2 vec_(u)int4 / vec_(u)short8 / vec_(u)char16
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++)
	{
		for (unsigned int j=0; j<sizeof(testInts)/sizeof(int); j++)
		{
			vec_int4 v1sw = (vec_int4)(testInts[i]);
			vec_int4 v2sw = (vec_int4)(testInts[j], testInts[j], 0.0, 0.0);
			vec_short8 v1sh = *(vec_short8 *)&v1sw;
			vec_short8 v2sh = *(vec_short8 *)&v2sw;
			vec_char16 v1sb = *(vec_char16 *)&v1sw;
			vec_char16 v2sb = *(vec_char16 *)&v2sw;

			vec_uint4 v1uw = (vec_uint4)(testInts[i]);
			vec_uint4 v2uw = (vec_uint4)(testInts[j], testInts[j], 0.0, 0.0);
			vec_ushort8 v1uh = *(vec_ushort8 *)&v1uw;
			vec_ushort8 v2uh = *(vec_ushort8 *)&v2uw;
			vec_uchar16 v1ub = *(vec_uchar16 *)&v1uw;
			vec_uchar16 v2ub = *(vec_uchar16 *)&v2uw;

			// Vector Integer Arithmetic Instructions
			printf("vec_vaddsbs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vaddsbs(v1sb,v2sb));
			printf("vec_vaddshs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vaddshs(v1sh,v2sh));
			printf("vec_vaddsws  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vaddsws(v1sw,v2sw));
			printf("vec_vaddubm  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vaddubm(v1ub,v2ub));
			printf("vec_vadduhm  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vadduhm(v1uh,v2uh));
			printf("vec_vadduwm  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vadduwm(v1uw,v2uw));
			printf("vec_vaddubs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vaddubs(v1ub,v2ub));
			printf("vec_vadduhs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vadduhs(v1uh,v2uh));
			printf("vec_vadduws  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vadduws(v1uw,v2uw));
			printf("vec_vaddcuw  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vaddcuw(v1uw,v2uw));
			printf("vec_vsubsbs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubsbs(v1sb,v2sb));
			printf("vec_vsubshs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubshs(v1sh,v2sh));
			printf("vec_vsubsws  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubsws(v1sw,v2sw));
			printf("vec_vsububm  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsububm(v1ub,v2ub));
			printf("vec_vsubuhm  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubuhm(v1uh,v2uh));
			printf("vec_vsubuwm  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubuwm(v1uw,v2uw));
			printf("vec_vsububs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsububs(v1ub,v2ub));
			printf("vec_vsubuhs  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubuhs(v1uh,v2uh));
			printf("vec_vsubuws  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubuws(v1uw,v2uw));
			printf("vec_vsubcuw  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubcuw(v1uw,v2uw));
			printf("vec_vmuloub  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmuloub(v1ub,v2ub));
			printf("vec_vmulouh  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmulouh(v1uh,v2uh));
			printf("vec_vmulosb  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmulosb(v1sb,v2sb));
			printf("vec_vmulosh  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmulosh(v1sh,v2sh));
			printf("vec_vmuleub  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmuleub(v1ub,v2ub));
			printf("vec_vmuleuh  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmuleuh(v1uh,v2uh));
			printf("vec_vmulesb  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmulesb(v1sb,v2sb));
			printf("vec_vmulesh  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmulesh(v1sh,v2sh));
			printf("vec_vsumsws  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsumsws(v1sw,v2sw));
			printf("vec_vsum2sws ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsum2sws(v1sw,v2sw));
			printf("vec_vsum4ubs ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsum4ubs(v1ub,v2uw));
			printf("vec_vsum4sbs ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsum4sbs(v1sb,v2sw));
			printf("vec_vsum4shs ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsum4shs(v1sh,v2sw));
			printf("vec_vavgub   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vavgub(v1ub,v2ub));
			printf("vec_vavguh   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vavguh(v1uh,v2uh));
			printf("vec_vavguw   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vavguw(v1uw,v2uw));
			printf("vec_vavgsb   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vavgsb(v1sb,v2sb));
			printf("vec_vavgsh   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vavgsh(v1sh,v2sh));
			printf("vec_vavgsw   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vavgsw(v1sw,v2sw));
			printf("vec_vmaxub   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmaxub(v1ub,v2ub));
			printf("vec_vmaxuh   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmaxuh(v1uh,v2uh));
			printf("vec_vmaxuw   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmaxuw(v1uw,v2uw));
			printf("vec_vmaxsb   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmaxsb(v1sb,v2sb));
			printf("vec_vmaxsh   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmaxsh(v1sh,v2sh));
			printf("vec_vmaxsw   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmaxsw(v1sw,v2sw));
			printf("vec_vminub   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vminub(v1ub,v2ub));
			printf("vec_vminuh   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vminuh(v1uh,v2uh));
			printf("vec_vminuw   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vminuw(v1uw,v2uw));
			printf("vec_vminsb   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vminsb(v1sb,v2sb));
			printf("vec_vminsh   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vminsh(v1sh,v2sh));
			printf("vec_vminsw   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vminsw(v1sw,v2sw));

			// Vector Integer Compare Instructions
			printf("vec_vcmpgtub ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgtub(v1ub,v2ub));
			printf("vec_vcmpgtuh ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgtuh(v1uh,v2uh));
			printf("vec_vcmpgtuw ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgtuw(v1uw,v2uw));
			printf("vec_vcmpgtsb ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgtsb(v1sb,v2sb));
			printf("vec_vcmpgtsh ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgtsh(v1sh,v2sh));
			printf("vec_vcmpgtsw ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgtsw(v1sw,v2sw));
			printf("vec_vcmpequb ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpequb(v1ub,v2ub));
			printf("vec_vcmpequh ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpequh(v1uh,v2uh));
			printf("vec_vcmpequw ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpequw(v1uw,v2uw));

			// Vector Integer Logical Instructions
			printf("vec_vand     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vand(v1uw,v2uw));
			printf("vec_vor      ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vor(v1uw,v2uw));
			printf("vec_vxor     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vxor(v1uw,v2uw));
			printf("vec_vandc    ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vandc(v1uw,v2uw));
			printf("vec_vnor     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vnor(v1uw,v2uw));

			// Vector Integer Rotate and Shift Instructions
			printf("vec_vrlb     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vrlb(v1ub,v2ub));
			printf("vec_vrlh     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vrlh(v1uh,v2uh));
			printf("vec_vrlw     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vrlw(v1uw,v2uw));
			printf("vec_vslb     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vslb(v1ub,v2ub));
			printf("vec_vslh     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vslh(v1uh,v2uh));
			printf("vec_vslw     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vslw(v1uw,v2uw));
			printf("vec_vsrb     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsrb(v1ub,v2ub));
			printf("vec_vsrh     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsrh(v1uh,v2uh));
			printf("vec_vsrw     ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsrw(v1uw,v2uw));
			printf("vec_vsrab    ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsrab(v1ub,v2ub));
			printf("vec_vsrah    ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsrah(v1uh,v2uh));
			printf("vec_vsraw    ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsraw(v1uw,v2uw));
		}
	}

	// Input arguments: 3 vec_(u)int4 / vec_(u)short8 / vec_(u)char16
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++)
	{
		for (unsigned int j=0; j<sizeof(testInts)/sizeof(int); j++)
		{
			for (unsigned int k=0; k<sizeof(testInts)/sizeof(int); k++)
			{
				vec_int4 v1sw = (vec_int4)(testInts[i]);
				vec_int4 v2sw = (vec_int4)(testInts[j], testInts[j], 0.0, 0.0);
				vec_int4 v3sw = (vec_int4)(testInts[k], 0.0, testInts[k], 0.0);
				vec_short8 v1sh = *(vec_short8 *)&v1sw;
				vec_short8 v2sh = *(vec_short8 *)&v2sw;
				vec_short8 v3sh = *(vec_short8 *)&v3sw;
				vec_char16 v1sb = *(vec_char16 *)&v1sw;
				vec_char16 v2sb = *(vec_char16 *)&v2sw;
				vec_char16 v3sb = *(vec_char16 *)&v3sw;

				vec_uint4 v1uw = (vec_uint4)(testInts[i]);
				vec_uint4 v2uw = (vec_uint4)(testInts[j], testInts[j], 0.0, 0.0);
				vec_uint4 v3uw = (vec_uint4)(testFloats[k], 0.0, testFloats[k], 0.0);
				vec_ushort8 v1uh = *(vec_ushort8 *)&v1uw;
				vec_ushort8 v2uh = *(vec_ushort8 *)&v2uw;
				vec_ushort8 v3uh = *(vec_ushort8 *)&v3uw;
				vec_uchar16 v1ub = *(vec_uchar16 *)&v1uw;
				vec_uchar16 v2ub = *(vec_uchar16 *)&v2uw;
				vec_uchar16 v3ub = *(vec_uchar16 *)&v3uw;

				// Vector Integer Arithmetic Instructions
				printf("vec_vmhaddshs  ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmhaddshs(v1sh,v2sh,v3sh));
				printf("vec_vmhraddshs ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmhraddshs(v1sh,v2sh,v3sh));
				printf("vec_vmladduhm  ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmladduhm(v1uh,v2uh,v3uh));
				printf("vec_vmsumubm   ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmsumubm(v1ub,v1ub,v1uw));
				printf("vec_vmsumuhm   ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmsumuhm(v1uh,v2uh,v3uw));
				printf("vec_vmsumshs   ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmsumshs(v1sh,v2sh,v3sw));
				printf("vec_vmsumuhs   ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmsumuhs(v1uh,v2uh,v3uw));
				printf("vec_vmsummbm   ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmsummbm(v1sb,v2ub,v3sw));
				printf("vec_vmsumshm   ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmsumshm(v1sh,v2sh,v3sw));
			}
		}
	}
	

	////////////////////////////////////////
	// VECTOR FLOATING-POINT INSTRUCTIONS //
	////////////////////////////////////////

	// Input arguments: 1 vec_float4, (1 UIMM)
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++)
	{
		vec_float4 v1 = (vec_float4)(testFloats[i]);

		// Vector Floating-Point Rounding and Conversion Instructions
		printf("vec_vrfin     ([%02d])    -> ", i); PRINT_VECTOR(vec_vrfin(v1));
		printf("vec_vrfiz     ([%02d])    -> ", i); PRINT_VECTOR(vec_vrfiz(v1));
		printf("vec_vrfip     ([%02d])    -> ", i); PRINT_VECTOR(vec_vrfip(v1));
		printf("vec_vrfim     ([%02d])    -> ", i); PRINT_VECTOR(vec_vrfim(v1));
		printf("vec_vctuxs    ([%02d], 0) -> ", i); PRINT_VECTOR(vec_vctuxs(v1, 0));
		printf("vec_vctsxs    ([%02d], 0) -> ", i); PRINT_VECTOR(vec_vctsxs(v1, 0));
		printf("vec_vctuxs    ([%02d], 1) -> ", i); PRINT_VECTOR(vec_vctuxs(v1, 1));
		printf("vec_vctsxs    ([%02d], 1) -> ", i); PRINT_VECTOR(vec_vctsxs(v1, 1));
		printf("vec_vctuxs    ([%02d], 2) -> ", i); PRINT_VECTOR(vec_vctuxs(v1, 2));
		printf("vec_vctsxs    ([%02d], 2) -> ", i); PRINT_VECTOR(vec_vctsxs(v1, 2));
		printf("vec_vctuxs    ([%02d],31) -> ", i); PRINT_VECTOR(vec_vctuxs(v1, 31));
		printf("vec_vctsxs    ([%02d],31) -> ", i); PRINT_VECTOR(vec_vctsxs(v1, 31));

		// Vector Floating-Point Estimate Instructions
		printf("vec_vrefp     ([%02d])    -> ", i); PRINT_VECTOR(vec_vrefp(v1));
		printf("vec_vrsqrtefp ([%02d])    -> ", i); PRINT_VECTOR(vec_vrsqrtefp(v1));
		printf("vec_vlogefp   ([%02d])    -> ", i); PRINT_VECTOR(vec_vlogefp(v1));
		printf("vec_vexptefp  ([%02d])    -> ", i); PRINT_VECTOR(vec_vexptefp(v1));
	}

	// Input arguments: 1 vec_(u)int4, 1 UIMM
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++)
	{
		vec_uint4 v1 = (vec_uint4)(testInts[i]);
		vec_int4 v2 = (vec_int4)(testInts[i]);

		// Vector Floating-Point Rounding and Conversion Instructions
		printf("vec_vcfux  ([%02d], 0) -> ", i); PRINT_VECTOR(vec_vcfux(v1, 0));
		printf("vec_vcfsx  ([%02d], 0) -> ", i); PRINT_VECTOR(vec_vcfsx(v2, 0));
		printf("vec_vcfux  ([%02d], 1) -> ", i); PRINT_VECTOR(vec_vcfux(v1, 1));
		printf("vec_vcfsx  ([%02d], 1) -> ", i); PRINT_VECTOR(vec_vcfsx(v2, 1));
		printf("vec_vcfux  ([%02d], 2) -> ", i); PRINT_VECTOR(vec_vcfux(v1, 2));
		printf("vec_vcfsx  ([%02d], 2) -> ", i); PRINT_VECTOR(vec_vcfsx(v2, 2));
		printf("vec_vcfux  ([%02d],31) -> ", i); PRINT_VECTOR(vec_vcfux(v1, 31));
		printf("vec_vcfsx  ([%02d],31) -> ", i); PRINT_VECTOR(vec_vcfsx(v2, 31));	
	}

	// Input arguments: 2 vec_float4
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++)
	{
		for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++)
		{
			vec_float4 v1 = (vec_float4)(testFloats[i]);
			vec_float4 v2 = (vec_float4)(testFloats[j], testFloats[j], 0.0, 0.0);

			// Floating-Point Arithmetic Instructions
			printf("vec_vaddfp   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vaddfp(v1,v2));
			printf("vec_vsubfp   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vsubfp(v1,v2));
			printf("vec_vminfp   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vminfp(v1,v2));
			printf("vec_vmaxfp   ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vmaxfp(v1,v2));

			// Vector Floating-Point Compare Instructions
			printf("vec_vcmpbfp  ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpbfp(v1,v2));
			printf("vec_vcmpeqfp ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpeqfp(v1,v2));
			printf("vec_vcmpgefp ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgefp(v1,v2));
			printf("vec_vcmpgefp ([%02d],[%02d]) -> ", i, j); PRINT_VECTOR(vec_vcmpgtfp(v1,v2));
		}
	}

	// Input arguments: 3 vec_float4
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++)
	{
		for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++)
		{
			for (unsigned int k=0; k<sizeof(testFloats)/sizeof(float); k++)
			{
				vec_float4 v1 = (vec_float4)(testFloats[i]);
				vec_float4 v2 = (vec_float4)(testFloats[j], testFloats[j], 0.0, 0.0);
				vec_float4 v3 = (vec_float4)(testFloats[k], 0.0, testFloats[k], 0.0);

				// Vector Floating-Point Multiply-Add Instructions
				printf("vec_vmaddfp  ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vmaddfp(v1,v2,v3));
				printf("vec_vnmsubfp ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vnmsubfp(v1,v2,v3));
			}
		}
	}


	//////////////////////////////////////
	// VECTOR LOAD & STORE INSTRUCTIONS //
	//////////////////////////////////////

	unsigned char srcUChars[16]   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	unsigned short srcUShorts[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	unsigned int srcUInts[16]     = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	vec_uchar16 srcVecChar  = (vec_uchar16)(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	vec_ushort8 srcVecShort = (vec_ushort8)(0, 1, 2, 3, 4, 5, 6, 7);
	vec_uint4 srcVecInt     = (vec_uint4)(0, 1, 2, 3);

	unsigned char dstUChars[16]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned short dstUShorts[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned int dstUInts[16]     = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Vector Load Instructions
	printf("vec_lvebx  (0, testUChars)  -> "); PRINT_VECTOR(vec_lvebx(0,srcUChars));
	printf("vec_lvehx  (5, testUShorts) -> "); PRINT_VECTOR(vec_lvehx(5,srcUShorts));
	printf("vec_lvewx  (16,testUInts)   -> "); PRINT_VECTOR(vec_lvewx(16,srcUInts));
	printf("vec_lvx    (16,testUInts)   -> "); PRINT_VECTOR(vec_lvx(32,srcUInts));
	printf("vec_lvxl   (16,testUInts)   -> "); PRINT_VECTOR(vec_lvxl(32,srcUInts));
	printf("vec_lvlx   (16,testUInts)   -> "); PRINT_VECTOR(vec_lvlx(32,srcUInts));
	printf("vec_lvlxl  (16,testUInts)   -> "); PRINT_VECTOR(vec_lvlxl(32,srcUInts));
	printf("vec_lvrx   (16,testUInts)   -> "); PRINT_VECTOR(vec_lvrx(32,srcUInts));
	printf("vec_lvrxl  (16,testUInts)   -> "); PRINT_VECTOR(vec_lvrxl(32,srcUInts));
	printf("vec_lvsl   (16,testUInts)   -> "); PRINT_VECTOR(vec_lvsl(32,srcUInts));
	printf("vec_lvsr   (16,testUInts)   -> "); PRINT_VECTOR(vec_lvsr(32,srcUInts));

	// Vector Store Instructions (TODO: I am not sure if this makes any sense)
	printf("vec_stvebx (srcVecChar,  0, dstUChars)  -> "); vec_stvebx(srcVecChar, 0, dstUChars);   PRINT_MEMORY(dstUChars, 16);
	printf("vec_stvehx (srcVecShort, 5, dstUShorts) -> "); vec_stvehx(srcVecShort, 5, dstUShorts); PRINT_MEMORY(dstUShorts, 32);
	printf("vec_stvewx (srcVecInt,  16, dstUInts)   -> "); vec_stvewx(srcVecInt, 16, dstUInts);    PRINT_MEMORY(dstUInts, 64);

	srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
	printf("vec_stvlx  (srcVecInt,  32, dstUInts)   -> "); vec_stvlx(srcVecInt, 32, dstUInts);     PRINT_MEMORY(dstUInts, 64);
	srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
	printf("vec_stvlxl (srcVecInt,  32, dstUInts)   -> "); vec_stvlxl(srcVecInt, 32, dstUInts);    PRINT_MEMORY(dstUInts, 64);
	srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
	printf("vec_stvrx  (srcVecInt,  32, dstUInts)   -> "); vec_stvrx(srcVecInt, 32, dstUInts);     PRINT_MEMORY(dstUInts, 64);
	srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
	printf("vec_stvrxl (srcVecInt,  32, dstUInts)   -> "); vec_stvrxl(srcVecInt, 32, dstUInts);    PRINT_MEMORY(dstUInts, 64);
	srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
	printf("vec_stvx   (srcVecInt,  32, dstUInts)   -> "); vec_stvx(srcVecInt, 32, dstUInts);      PRINT_MEMORY(dstUInts, 64);
	srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
	printf("vec_stvxl  (srcVecInt,  32, dstUInts)   -> "); vec_stvxl(srcVecInt, 32, dstUInts);     PRINT_MEMORY(dstUInts, 64);

	
	//////////////////////////////////////////////////
	// VECTOR PERMUTATION & FORMATTING INSTRUCTIONS //
	//////////////////////////////////////////////////

	// Input arguments: 1 SIMM
	// Vector Splat Instructions
	printf("vec_vspltisb (-1) -> "); PRINT_VECTOR(vec_vspltisb(-1));
	printf("vec_vspltisb ( 0) -> "); PRINT_VECTOR(vec_vspltisb(0));
	printf("vec_vspltisb ( 1) -> "); PRINT_VECTOR(vec_vspltisb(1));
	printf("vec_vspltisb ( 2) -> "); PRINT_VECTOR(vec_vspltisb(2));
	printf("vec_vspltish (-1) -> "); PRINT_VECTOR(vec_vspltish(-1));
	printf("vec_vspltish ( 0) -> "); PRINT_VECTOR(vec_vspltish(0));
	printf("vec_vspltish ( 1) -> "); PRINT_VECTOR(vec_vspltish(1));
	printf("vec_vspltish ( 2) -> "); PRINT_VECTOR(vec_vspltish(2));
	printf("vec_vspltisw (-1) -> "); PRINT_VECTOR(vec_vspltisw(-1));
	printf("vec_vspltisw ( 0) -> "); PRINT_VECTOR(vec_vspltisw(0));
	printf("vec_vspltisw ( 1) -> "); PRINT_VECTOR(vec_vspltisw(1));
	printf("vec_vspltisw ( 2) -> "); PRINT_VECTOR(vec_vspltisw(2));


	// Input arguments: 1 vec_(u)int4 / vec_(u)short8 / vec_(u)char16, (1 UIMM)
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++)
	{
		vec_int4 v1sw = (vec_int4)(testInts[i]);
		vec_short8 v1sh = *(vec_short8 *)&v1sw;
		vec_char16 v1sb = *(vec_char16 *)&v1sw;

		vec_uint4 v1uw = (vec_uint4)(testInts[i]);
		vec_ushort8 v1uh = *(vec_ushort8 *)&v1uw;
		vec_uchar16 v1ub = *(vec_uchar16 *)&v1uw;

		// Vector Unpack Instructions
		printf("vec_vupkhsb ([%02d]) -> ", i); PRINT_VECTOR(vec_vupkhsb(v1sb));
		printf("vec_vupkhsh ([%02d]) -> ", i); PRINT_VECTOR(vec_vupkhsh(v1sh));
		printf("vec_vupkhpx ([%02d]) -> ", i); PRINT_VECTOR(vec_vupkhpx(v1uh));
		printf("vec_vupklsb ([%02d]) -> ", i); PRINT_VECTOR(vec_vupklsb(v1sb));
		printf("vec_vupklsh ([%02d]) -> ", i); PRINT_VECTOR(vec_vupklsh(v1sh));
		printf("vec_vupklpx ([%02d]) -> ", i); PRINT_VECTOR(vec_vupklpx(v1uh));

		// Vector Splat Instructions
		printf("vec_vspltb ([%02d],-1) -> ", i); PRINT_VECTOR(vec_vspltb(v1ub,-1));
		printf("vec_vspltb ([%02d], 0) -> ", i); PRINT_VECTOR(vec_vspltb(v1ub,0));
		printf("vec_vspltb ([%02d], 1) -> ", i); PRINT_VECTOR(vec_vspltb(v1ub,1));
		printf("vec_vspltb ([%02d], 2) -> ", i); PRINT_VECTOR(vec_vspltb(v1ub,2));
		printf("vec_vsplth ([%02d],-1) -> ", i); PRINT_VECTOR(vec_vsplth(v1uh,-1));
		printf("vec_vsplth ([%02d], 0) -> ", i); PRINT_VECTOR(vec_vsplth(v1uh,0));
		printf("vec_vsplth ([%02d], 1) -> ", i); PRINT_VECTOR(vec_vsplth(v1uh,1));
		printf("vec_vsplth ([%02d], 2) -> ", i); PRINT_VECTOR(vec_vsplth(v1uh,2));
		printf("vec_vspltw ([%02d],-1) -> ", i); PRINT_VECTOR(vec_vspltw(v1uw,-1));
		printf("vec_vspltw ([%02d], 0) -> ", i); PRINT_VECTOR(vec_vspltw(v1uw,0));
		printf("vec_vspltw ([%02d], 1) -> ", i); PRINT_VECTOR(vec_vspltw(v1uw,1));
		printf("vec_vspltw ([%02d], 2) -> ", i); PRINT_VECTOR(vec_vspltw(v1uw,2));
	}
	
	// Input arguments: 2 vec_(u)int4 / vec_(u)short8 / vec_(u)char16, (1 UIMM)
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++)
	{
		for (unsigned int j=0; j<sizeof(testInts)/sizeof(int); j++)
		{
			vec_int4 v1sw = (vec_int4)(testInts[i]);
			vec_int4 v2sw = (vec_int4)(testInts[j], testInts[j], 0.0, 0.0);
			vec_short8 v1sh = *(vec_short8 *)&v1sw;
			vec_short8 v2sh = *(vec_short8 *)&v2sw;
			vec_char16 v1sb = *(vec_char16 *)&v1sw;
			vec_char16 v2sb = *(vec_char16 *)&v2sw;

			vec_uint4 v1uw = (vec_uint4)(testInts[i]);
			vec_uint4 v2uw = (vec_uint4)(testInts[j], testInts[j], 0.0, 0.0);
			vec_ushort8 v1uh = *(vec_ushort8 *)&v1uw;
			vec_ushort8 v2uh = *(vec_ushort8 *)&v2uw;
			vec_uchar16 v1ub = *(vec_uchar16 *)&v1uw;
			vec_uchar16 v2ub = *(vec_uchar16 *)&v2uw;

			// Vector Pack Instructions
			printf("vec_vpkuhum ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkuhum(v1uh,v2uh));
			printf("vec_vpkuwum ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkuwum(v1uw,v2uw));
			printf("vec_vpkuhus ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkuhus(v1uh,v2uh));
			printf("vec_vpkuwus ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkuwus(v1uw,v2uw));
			printf("vec_vpkshus ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkshus(v1sh,v2sh));
			printf("vec_vpkswus ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkswus(v1sw,v2sw));
			printf("vec_vpkshss ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkshss(v1sh,v2sh));
			printf("vec_vpkswss ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkswss(v1sw,v2sw));
			printf("vec_vpkpx   ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vpkpx(v1uw,v2uw));

			// Vector Merge Instructions
			printf("vec_vmrghb  ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vmrghb(v1ub,v2ub));
			printf("vec_vmrghh  ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vmrghh(v1uh,v2uh));
			printf("vec_vmrghw  ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vmrghw(v1uw,v2uw));
			printf("vec_vmrglb  ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vmrglb(v1ub,v2ub));
			printf("vec_vmrglh  ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vmrglh(v1uh,v2uh));
			printf("vec_vmrglw  ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vmrglw(v1uw,v2uw));

			// Vector Shift Instructions
			printf("vec_vsl     ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vsl(v1uw,v2uw));
			printf("vec_vsldoi  ([%02d],[%02d], 0) -> ", i, j); PRINT_VECTOR(vec_vsldoi(v1uw,v2uw, 0));
			printf("vec_vsldoi  ([%02d],[%02d], 1) -> ", i, j); PRINT_VECTOR(vec_vsldoi(v1uw,v2uw, 1));
			printf("vec_vsldoi  ([%02d],[%02d], 2) -> ", i, j); PRINT_VECTOR(vec_vsldoi(v1uw,v2uw, 2));
			printf("vec_vsldoi  ([%02d],[%02d],15) -> ", i, j); PRINT_VECTOR(vec_vsldoi(v1uw,v2uw,15));
			printf("vec_vslo    ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vslo(v1ub,v2ub));
			printf("vec_vsr     ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vsr(v1uw,v2uw));
			printf("vec_vsro    ([%02d],[%02d])    -> ", i, j); PRINT_VECTOR(vec_vsro(v1ub,v2ub));
		}
	}
	
	// Input arguments: 3 vec_uint4 / vec_uchar16
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++)
	{
		for (unsigned int j=0; j<sizeof(testInts)/sizeof(int); j++)
		{
			for (unsigned int k=0; k<sizeof(testInts)/sizeof(int); k++)
			{
				vec_uint4 v1uw = (vec_uint4)(testInts[i]);
				vec_uint4 v2uw = (vec_uint4)(testInts[j], testInts[j], 0.0, 0.0);
				vec_uint4 v3uw = (vec_uint4)(testFloats[k], 0.0, testFloats[k], 0.0);
				vec_uchar16 v1ub = *(vec_uchar16 *)&v1uw;
				vec_uchar16 v2ub = *(vec_uchar16 *)&v2uw;
				vec_uchar16 v3ub = *(vec_uchar16 *)&v3uw;

				// Vector Permute Instruction
				printf("vec_vperm ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vperm(v1ub,v2ub,v3ub));

				// Vector Select Instruction
				printf("vec_vsel  ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VECTOR(vec_vsel(v1uw,v2uw,v3uw));
			}
		}
	}

	return 0;
}
