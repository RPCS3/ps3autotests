#include <stdio.h>
#include <math.h>
#include <float.h>
#include <altivec.h>

#define SHOW_RESULTS(v0) printf("%08x %08x %08x %08x\n", *(int*)&v0[0], *(int*)&v0[1], *(int*)&v0[2], *(int*)&v0[3]);

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

	// TODO

	////////////////////////////////////////
	// VECTOR FLOATING-POINT INSTRUCTIONS //
	////////////////////////////////////////

	// Input arguments: 1 vec_float4, (1 UIMM)
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) {
		vec_float4 v1 = (vec_float4)(testFloats[i]);

		// Vector Floating-Point Rounding and Conversion Instructions
		printf("vec_vrfin     ([%02d])    -> ", i); SHOW_RESULTS(vec_vrfin(v1));
		printf("vec_vrfiz     ([%02d])    -> ", i); SHOW_RESULTS(vec_vrfiz(v1));
		printf("vec_vrfip     ([%02d])    -> ", i); SHOW_RESULTS(vec_vrfip(v1));
		printf("vec_vrfim     ([%02d])    -> ", i); SHOW_RESULTS(vec_vrfim(v1));
		printf("vec_vctuxs    ([%02d], 0) -> ", i); SHOW_RESULTS(vec_vctuxs(v1, 0));
		printf("vec_vctsxs    ([%02d], 0) -> ", i); SHOW_RESULTS(vec_vctsxs(v1, 0));
		printf("vec_vctuxs    ([%02d], 1) -> ", i); SHOW_RESULTS(vec_vctuxs(v1, 1));
		printf("vec_vctsxs    ([%02d], 1) -> ", i); SHOW_RESULTS(vec_vctsxs(v1, 1));
		printf("vec_vctuxs    ([%02d], 2) -> ", i); SHOW_RESULTS(vec_vctuxs(v1, 2));
		printf("vec_vctsxs    ([%02d], 2) -> ", i); SHOW_RESULTS(vec_vctsxs(v1, 2));
		printf("vec_vctuxs    ([%02d],31) -> ", i); SHOW_RESULTS(vec_vctuxs(v1, 31));
		printf("vec_vctsxs    ([%02d],31) -> ", i); SHOW_RESULTS(vec_vctsxs(v1, 31));

		// Vector Floating-Point Estimate Instructions
		printf("vec_vrefp     ([%02d])    -> ", i); SHOW_RESULTS(vec_vrefp(v1));
		printf("vec_vrsqrtefp ([%02d])    -> ", i); SHOW_RESULTS(vec_vrsqrtefp(v1));
		printf("vec_vlogefp   ([%02d])    -> ", i); SHOW_RESULTS(vec_vlogefp(v1));
		printf("vec_vexptefp  ([%02d])    -> ", i); SHOW_RESULTS(vec_vexptefp(v1));
	}

	// Input arguments: 1 vec_(u)int4, 1 UIMM
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(float); i++) {
		vec_uint4 v1 = (vec_uint4)(testInts[i]);
		vec_int4 v2 = (vec_int4)(testInts[i]);

		// Vector Floating-Point Rounding and Conversion Instructions
		printf("vec_vcfux  ([%02d], 0) -> ", i); SHOW_RESULTS(vec_vcfux(v1, 0));
		printf("vec_vcfsx  ([%02d], 0) -> ", i); SHOW_RESULTS(vec_vcfsx(v2, 0));
		printf("vec_vcfux  ([%02d], 1) -> ", i); SHOW_RESULTS(vec_vcfux(v1, 1));
		printf("vec_vcfsx  ([%02d], 1) -> ", i); SHOW_RESULTS(vec_vcfsx(v2, 1));
		printf("vec_vcfux  ([%02d], 2) -> ", i); SHOW_RESULTS(vec_vcfux(v1, 2));
		printf("vec_vcfsx  ([%02d], 2) -> ", i); SHOW_RESULTS(vec_vcfsx(v2, 2));
		printf("vec_vcfux  ([%02d],31) -> ", i); SHOW_RESULTS(vec_vcfux(v1, 31));
		printf("vec_vcfsx  ([%02d],31) -> ", i); SHOW_RESULTS(vec_vcfsx(v2, 31));	
	}

	// Input arguments: 2 vec_float4
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) {
		for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++) {
			vec_float4 v1 = (vec_float4)(testFloats[i]);
			vec_float4 v2 = (vec_float4)(testFloats[j], testFloats[j], 0.0, 0.0);

			// Floating-Point Arithmetic Instructions
			printf("vec_vaddfp   ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vaddfp(v1,v2));
			printf("vec_vsubfp   ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vsubfp(v1,v2));
			printf("vec_vminfp   ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vminfp(v1,v2));
			printf("vec_vmaxfp   ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vmaxfp(v1,v2));

			// Vector Floating-Point Compare Instructions
			printf("vec_vcmpbfp  ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vcmpbfp(v1,v2));
			printf("vec_vcmpeqfp ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vcmpeqfp(v1,v2));
			printf("vec_vcmpgefp ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vcmpgefp(v1,v2));
			printf("vec_vcmpgefp ([%02d],[%02d]) -> ", i, j); SHOW_RESULTS(vec_vcmpgtfp(v1,v2));
		}
	}

	// Input arguments: 3 vec_float4
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) {
		for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++) {
			for (unsigned int k=0; k<sizeof(testFloats)/sizeof(float); k++) {
				vec_float4 v1 = (vec_float4)(testFloats[i]);
				vec_float4 v2 = (vec_float4)(testFloats[j], testFloats[j], 0.0, 0.0);
				vec_float4 v3 = (vec_float4)(testFloats[k], 0.0, testFloats[k], 0.0);

				// Vector Floating-Point Multiply-Add Instructions
				printf("vec_vmaddfp  ([%02d],[%02d],[%02d]) -> ", i, j, k); SHOW_RESULTS(vec_vmaddfp(v1,v2,v3));
				printf("vec_vnmsubfp ([%02d],[%02d],[%02d]) -> ", i, j, k); SHOW_RESULTS(vec_vnmsubfp(v1,v2,v3));
			}
		}
	}

	//////////////////////////////////////
	// VECTOR LOAD & STORE INSTRUCTIONS //
	//////////////////////////////////////

	// TODO

	//////////////////////////////////////////////////
	// VECTOR PERMUTATION & FORMATTING INSTRUCTIONS //
	//////////////////////////////////////////////////

	// TODO

	return 0;
}
