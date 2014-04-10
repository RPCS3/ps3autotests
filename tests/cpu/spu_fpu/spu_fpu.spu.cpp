#include <cell.h>
#include <spu_printf.h>
#include <spu_internals.h>
#include <math.h>
#include <float.h>


// Loops with floats
#define ITERATE1f(name, instruction) \
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) { \
		volatile vec_float4 v1 = (vec_float4)(testFloats[i]); \
		volatile vec_float4 r  = (vec_float4)instruction((qword)v1); \
		spu_printf(name"([%02d]) -> %08x %08x %08x %08x\n",i,r[0],r[1],r[2],r[3]); \
	}

#define ITERATE2f(name, instruction) \
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) \
		for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++) { \
			volatile vec_float4 v1 = (vec_float4)(testFloats[i]); \
			volatile vec_float4 v2 = (vec_float4)(testFloats[j],testFloats[j],0.0,0.0); \
			volatile vec_float4 r  = (vec_float4)instruction((qword)v1,(qword)v2); \
			spu_printf(name"([%02d],[%02d]) -> %08x %08x %08x %08x\n",i,j,r[0],r[1],r[2],r[3]); \
		}

#define ITERATE3f(name, instruction) \
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) \
		for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++) \
			for (unsigned int k=0; k<sizeof(testFloats)/sizeof(float); k++) { \
				volatile vec_float4 v1 = (vec_float4)(testFloats[i]); \
				volatile vec_float4 v2 = (vec_float4)(testFloats[j],testFloats[j],0.0,0.0); \
				volatile vec_float4 v3 = (vec_float4)(testFloats[k],0.0,testFloats[k],0.0); \
				volatile vec_float4 r  = (vec_float4)instruction((qword)v1,(qword)v2,(qword)v3); \
				spu_printf(name"([%02d],[%02d],[%02d]) -> %08x %08x %08x %08x\n",i,j,k,r[0],r[1],r[2],r[3]); \
			}

// Loops with doubles
#define ITERATE1d(name, instruction) \
	for (unsigned int i=0; i<sizeof(testDoubles)/sizeof(double); i++) { \
		volatile vec_double2 v1 = (vec_double2)(testDoubles[i]); \
		volatile vec_double2 r  = (vec_double2)instruction((qword)v1); \
		spu_printf(name"([%02d]) -> %016llx %016llx\n",i,r[0],r[1]); \
	}

#define ITERATE2d(name, instruction) \
	for (unsigned int i=0; i<sizeof(testDoubles)/sizeof(double); i++) \
		for (unsigned int j=0; j<sizeof(testDoubles)/sizeof(double); j++) { \
			volatile vec_double2 v1 = (vec_double2)(testDoubles[i]); \
			volatile vec_double2 v2 = (vec_double2)(testDoubles[j],0.0); \
			volatile vec_double2 r  = (vec_double2)instruction((qword)v1,(qword)v2); \
			spu_printf(name"([%02d],[%02d]) -> %016llx %016llx\n",i,j,r[0],r[1]); \
		}

#define ITERATE3d(name, instruction) \
	for (unsigned int i=0; i<sizeof(testDoubles)/sizeof(double); i++) \
		for (unsigned int j=0; j<sizeof(testDoubles)/sizeof(double); j++) \
			for (unsigned int k=0; k<sizeof(testDoubles)/sizeof(double); k++) { \
				volatile vec_double2 v1 = (vec_double2)(testDoubles[i]); \
				volatile vec_double2 v2 = (vec_double2)(testDoubles[j]); \
				volatile vec_double2 v3 = (vec_double2)(testDoubles[k],0.0); \
				volatile vec_double2 r  = (vec_double2)instruction((qword)v1,(qword)v2,(qword)v3); \
				spu_printf(name"([%02d],[%02d],[%02d]) -> %016llx %016llx\n",i,j,k,r[0],r[1]); \
			}

// Conversion loops
#define ITERATE1iimm(name, instruction, imm) \
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++) { \
		volatile vec_int4 v1 = (vec_int4)(testInts[i]); \
		volatile vec_float4 r  = (vec_float4)instruction((qword)v1, imm); \
		spu_printf(name"([%02d],%d) -> %08x %08x %08x %08x\n",i,imm,r[0],r[1],r[2],r[3]); \
	}

#define ITERATE1fimm(name, instruction, imm) \
	for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) { \
		volatile vec_float4 v1 = (vec_float4)(testFloats[i]); \
		volatile vec_int4 r  = (vec_int4)instruction((qword)v1, imm); \
		spu_printf(name"([%02d],%d) -> %08x %08x %08x %08x\n",i,imm,r[0],r[1],r[2],r[3]); \
	}


// Test values
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

double testDoubles[] = {
	0.0, -0.0,  1.0, -1.0,  1.5, -1.5,
	1.6, -1.6,  1.4, -1.4,  2.0, -2.0,
	4.0, -10000000.4, 20000000.0, -20000.5, 20000.6,
	DBL_MIN, DBL_MAX, INFINITY, -INFINITY, NAN,
};


int main()
{
	/////////////////////////////////
	// FLOATING-POINT INSTRUCTIONS //
	/////////////////////////////////

	// Single Precision instructions
	ITERATE2f("fa      ", si_fa);
	ITERATE2f("fs      ", si_fs);
	ITERATE2f("fm      ", si_fm);
	ITERATE3f("fma     ", si_fma);
	ITERATE3f("fnms    ", si_fnms);
	ITERATE3f("fms     ", si_fms);
	ITERATE1f("frest   ", si_frest);
	ITERATE1f("frsqest ", si_frsqest);
	ITERATE2f("fi      ", si_fi);
	ITERATE1f("fesd    ", si_fesd);
	ITERATE2f("fceq    ", si_fceq);
	ITERATE2f("fcmeq   ", si_fcmeq);
	ITERATE2f("fcgt    ", si_fcgt);
	ITERATE2f("fcmgt   ", si_fcmgt);

	// Double Precision instructions (without eDP instructions: dfceq, dfcmeq, dfcgt, dfcmgt, dftsv)
	ITERATE2d("dfa     ", si_dfa);
	ITERATE2d("dfs     ", si_dfs);
	ITERATE2d("dfm     ", si_dfm);
	ITERATE3d("dfma    ", si_dfma);
	ITERATE3d("dfnms   ", si_dfnms);
	ITERATE3d("dfms    ", si_dfms);
	ITERATE3d("dfnma   ", si_dfnma);
	ITERATE1d("frds    ", si_frds);

	// Conversion instructions
	ITERATE1iimm("csflt   ", si_csflt, 0x00);
	ITERATE1iimm("csflt   ", si_csflt, 0x01);
	ITERATE1iimm("csflt   ", si_csflt, 0x04);
	ITERATE1iimm("csflt   ", si_csflt, 0x7f);
	ITERATE1fimm("cflts   ", si_cflts, 0x00);
	ITERATE1fimm("cflts   ", si_cflts, 0x01);
	ITERATE1fimm("cflts   ", si_cflts, 0x04);
	ITERATE1fimm("cflts   ", si_cflts, 0x7f);
	ITERATE1iimm("cuflt   ", si_cuflt, 0x00);
	ITERATE1iimm("cuflt   ", si_cuflt, 0x01);
	ITERATE1iimm("cuflt   ", si_cuflt, 0x04);
	ITERATE1iimm("cuflt   ", si_cuflt, 0x7f);
	ITERATE1fimm("cfltu   ", si_cfltu, 0x00);
	ITERATE1fimm("cfltu   ", si_cfltu, 0x01);
	ITERATE1fimm("cfltu   ", si_cfltu, 0x04);
	ITERATE1fimm("cfltu   ", si_cfltu, 0x7f);

	// TODO: fscrwr, fscrrd
}
