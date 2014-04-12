#ifndef SPU_TEST_H
#define SPU_TEST_H

#include <stdio.h>
#include <spu_printf.h>
#include <spu_intrinsics.h>
#include <float.h>

#define __STDC_LIMIT_MACROS

#include <stdint.h>

typedef vec_uchar16 vec_uchar16_t;
typedef vec_char16  vec_char16_t; 
typedef vec_ushort8 vec_ushort8_t;
typedef vec_short8  vec_short8_t; 
typedef vec_uint4   vec_uint4_t; 
typedef vec_int4    vec_int4_t;   
typedef vec_ullong2 vec_ullong2_t;
typedef vec_llong2  vec_llong2_t; 
typedef vec_float4  vec_float4_t; 
typedef vec_double2 vec_double2_t;

#define LIT_0 0
#define LIT_1 1
#define LIT_2 7
#define LIT_3 13
#define LIT_4 60
#define LIT_5 127
#define LIT_6 128
#define LIT_7 500
#define LIT_8 1023
#define LIT_9 1024
#define LIT_10 31022
#define LIT_11 65535
#define LIT_12 65536
#define LIT_13 155541
#define LIT_14 262143
#define LIT_15 262144


// every array is padded with 4 zeros
float float_values[] = {0.0f, -0.0f, 1.0f, -1.0f, 2.001f, -2.001f, 15.0f, -15.0f, 1.0f+FLT_EPSILON, -1.0f-FLT_EPSILON, FLT_MAX, -FLT_MAX, FLT_MIN, -FLT_MIN, 20000.1f, -20000.1f, 0, 0, 0, 0};
double double_values[] = {0.0, -0.0, 1.0, -1.0, 2.001, -2.001, 15.0, -15.0, 1.0+DBL_EPSILON, -1.0-DBL_EPSILON, DBL_MAX, -DBL_MAX, DBL_MIN, -DBL_MIN, 20000.1, -20000.1, 0, 0, 0, 0};
char char_values[] = {0, 1, 2, 15, 85, 127, 128, 170, 240, UINT8_MAX, 0, 0, 0, 0};
uint8_t uint8_t_values[] = {0, 1, 2, 15, 85, 127, 128, 170, 240, UINT8_MAX, 0, 0, 0, 0};
int8_t int8_t_values[] = {0, 1, -2, INT8_MAX, INT8_MIN, 15, -16, 0xAA, 0x55, 127, -128, 0, 0, 0, 0};
uint16_t uint16_t_values[] = {0, 1, 2, 0x00ff, 0xff00, UINT16_MAX, 0xAAAA, 0x5555, 1023, 0, 0, 0, 0};
int16_t int16_t_values[] = {0, 1, -2, 255, -256, INT16_MAX, INT16_MIN, 0xAAAA, 0x5555, 1023, 0, 0, 0, 0};
uint32_t uint32_t_values[] = {0, 1, 0xff, 0xff00, 0xff0000, 0xff000000, UINT32_MAX, 0xAAAAAAAA, 0x55555555, 0xffff, 0xffff0000, 0, 0, 0, 0};
int32_t int32_t_values[] = {0, 1, INT32_MAX, INT32_MIN, 0xAAAAAAAA, 0x55555555, 0xffff, 0xffff0000, 0, 0, 0, 0 };
uint64_t uint64_t_values[] = {0, 1, 0xffffULL, 0xffff0000ULL, 0xffff00000000ULL, 0xffff000000000000ULL, UINT64_MAX, 0xAAAAAAAAAAAAAAAAULL, 0x5555555555555555ULL, 0xffffffffULL, 0xffffffff00000000ULL, 0, 0, 0, 0};
int64_t int64_t_values[] = {0, 1, INT64_MAX, INT64_MIN, 0xAAAAAAAAAAAAAAAAULL, 0x5555555555555555ULL, 0xffffffffULL, 0xffffffff00000000ULL, 0, 0, 0, 0};

static inline uint32_t float_to_int(float f)
{
	union {float f; uint32_t i;} u;
	u.f = f;
	return u.i;
}

static inline uint64_t double_to_int(double f)
{
	union {double f; uint64_t i;} u;
	u.f = f;
	return u.i;
}

#define double_size int(sizeof(double_values)/sizeof(double)-4)
#define float_size int(sizeof(float_values)/sizeof(float)-4)
#define char_size int(sizeof(char_values)/sizeof(char)-4)
#define int8_t_size int(sizeof(int8_t_values)/sizeof(int8_t)-4)
#define uint8_t_size int(sizeof(uint8_t_values)/sizeof(uint8_t)-4)
#define int16_t_size int(sizeof(int16_t_values)/sizeof(int16_t)-4)
#define uint16_t_size int(sizeof(uint16_t_values)/sizeof(uint16_t)-4)
#define int32_t_size int(sizeof(int32_t_values)/sizeof(int32_t)-4)
#define uint32_t_size int(sizeof(uint32_t_values)/sizeof(uint32_t)-4)
#define int64_t_size int(sizeof(int64_t_values)/sizeof(int64_t)-4)
#define uint64_t_size int(sizeof(uint64_t_values)/sizeof(uint64_t)-4)

#define vec_uchar16_t_size uint8_t_size
#define vec_char16_t_size int8_t_size
#define vec_ushort8_t_size uint16_t_size
#define vec_short8_t_size int16_t_size
#define vec_uint4_t_size uint32_t_size
#define vec_int4_t_size int32_t_size
#define vec_ullong2_t_size uint64_t_size
#define vec_llong2_t_size int64_t_size
#define vec_float4_t_size float_size
#define vec_double2_t_size double_size

//#define FORMAT_vec_char16_t "%04hx%04hx%04hx%04hx%04hx%04hx%04hx%04hx"
//#define FORMAT_vec_uchar16_t "%04hx%04hx%04hx%04hx%04hx%04hx%04hx%04hx"
#define FORMAT_vec_char16_t "%016llx%016llx"
#define FORMAT_vec_uchar16_t "%016llx%016llx"

#define FORMAT_vec_short8_t "(%04hx, %04hx, %04hx, %04hx, %04hx, %04hx, %04hx, %04hx)"
#define FORMAT_vec_ushort8_t "(%04hx, %04hx, %04hx, %04hx, %04hx, %04hx, %04hx, %04hx)"
#define FORMAT_vec_int4_t "(%08x, %08x, %08x, %08x)"
#define FORMAT_vec_uint4_t "(%08x, %08x, %08x, %08x)"
#define FORMAT_vec_llong2_t "(%016llx, %016llx)"
#define FORMAT_vec_ullong2_t "(%016llx, %016llx)"
#define FORMAT_vec_float4_t "(%gf, %gf, %gf, %gf)[%08x.%08x.%08x.%08x]"
#define FORMAT_vec_double2_t "(%g, %g)[%016llx.%016llx]"

#define FORMAT_char "0x%02hhx"
#define FORMAT_int8_t "0x%02hhx"
#define FORMAT_uint8_t "0x%02hhx"
#define FORMAT_int16_t "0x%04hx"
#define FORMAT_uint16_t "0x%04hx"
#define FORMAT_int32_t "0x%08x"
#define FORMAT_uint32_t "0x%08x"
#define FORMAT_int64_t "0x%016llx"
#define FORMAT_uint64_t "0x%016llx"
#define FORMAT_float "%gf[%08x]"
#define FORMAT_double "%g[%016llx]"

#define INIT_vec_char16_t(i) {int8_t_values[i+0], int8_t_values[i+1], int8_t_values[i+2], int8_t_values[i+3], int8_t_values[i], int8_t_values[i+0], int8_t_values[i+1], int8_t_values[i+2], int8_t_values[i+3], int8_t_values[i], int8_t_values[i], int8_t_values[i+0], int8_t_values[i+1], int8_t_values[i+2], int8_t_values[i+3], int8_t_values[i]}
#define INIT_vec_uchar16_t(i) {uint8_t_values[i+0], uint8_t_values[i+1], uint8_t_values[i+2], uint8_t_values[i+3], uint8_t_values[i], uint8_t_values[i+0], uint8_t_values[i+1], uint8_t_values[i+2], uint8_t_values[i+3], uint8_t_values[i], uint8_t_values[i], uint8_t_values[i+0], uint8_t_values[i+1], uint8_t_values[i+2], uint8_t_values[i+3], uint8_t_values[i]}
#define INIT_vec_short8_t(i) {int16_t_values[i+0], int16_t_values[i+1], int16_t_values[i+2], int16_t_values[i+3], int16_t_values[i+2], int16_t_values[i], int16_t_values[i+1], int16_t_values[i+3]}
#define INIT_vec_ushort8_t(i) {uint16_t_values[i+0], uint16_t_values[i+1], uint16_t_values[i+2], uint16_t_values[i+3], uint16_t_values[i+2], uint16_t_values[i], uint16_t_values[i+1], uint16_t_values[i+3]}
#define INIT_vec_int4_t(i) {int32_t_values[i], int32_t_values[i+1], int32_t_values[i+2], int32_t_values[i+3]}
#define INIT_vec_uint4_t(i) {uint32_t_values[i], uint32_t_values[i+1], uint32_t_values[i+2], uint32_t_values[i+3]}
#define INIT_vec_llong2_t(i) {int64_t_values[i], int64_t_values[i+1]}
#define INIT_vec_ullong2_t(i) {uint64_t_values[i], uint64_t_values[i+1]}
#define INIT_vec_float4_t(i) {float_values[i], float_values[i+1], float_values[i+2], float_values[i+3]}
#define INIT_vec_double2_t(i) {double_values[i], double_values[i+1]}

#define INIT_char(i) char_values[i]
#define INIT_int8_t(i) int8_t_values[i]
#define INIT_uint8_t(i) uint8_t_values[i]
#define INIT_int16_t(i) int16_t_values[i]
#define INIT_uint16_t(i) uint16_t_values[i]
#define INIT_int32_t(i) int32_t_values[i]
#define INIT_uint32_t(i) uint32_t_values[i]
#define INIT_int64_t(i) int64_t_values[i]
#define INIT_uint64_t(i) uint64_t_values[i]
#define INIT_float(i) float_values[i]
#define INIT_double(i) double_values[i]

//#define ARG_vec_char16_t(x) ((vec_ushort8_t)x)[0], ((vec_ushort8_t)x)[1], ((vec_ushort8_t)x)[2], ((vec_ushort8_t)x)[3], ((vec_ushort8_t)x)[4], ((vec_ushort8_t)x)[5], ((vec_ushort8_t)x)[6], ((vec_ushort8_t)x)[7]
//#define ARG_vec_uchar16_t(x) ((vec_ushort8_t)x)[0], ((vec_ushort8_t)x)[1], ((vec_ushort8_t)x)[2], ((vec_ushort8_t)x)[3], ((vec_ushort8_t)x)[4], ((vec_ushort8_t)x)[5], ((vec_ushort8_t)x)[6], ((vec_ushort8_t)x)[7]
#define ARG_vec_char16_t(x) ((vec_ullong2_t)x)[0], ((vec_ullong2_t)x)[1]
#define ARG_vec_uchar16_t(x) ((vec_ullong2_t)x)[0], ((vec_ullong2_t)x)[1]
#define ARG_vec_short8_t(x) x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]
#define ARG_vec_ushort8_t(x) x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]
#define ARG_vec_int4_t(x) x[0], x[1], x[2], x[3]
#define ARG_vec_uint4_t(x) x[0], x[1], x[2], x[3]
#define ARG_vec_llong2_t(x) x[0], x[1]
#define ARG_vec_ullong2_t(x) x[0], x[1]
#define ARG_vec_float4_t(x) x[0], x[1], x[2], x[3], ((vec_uint4_t)x)[0], ((vec_uint4_t)x)[1], ((vec_uint4_t)x)[2], ((vec_uint4_t)x)[3]
#define ARG_vec_double2_t(x) x[0], x[1], ((vec_ullong2_t)x)[0], ((vec_ullong2_t)x)[1]

#define ARG_char(x) x
#define ARG_int8_t(x) x
#define ARG_uint8_t(x) x
#define ARG_int16_t(x) x
#define ARG_uint16_t(x) x
#define ARG_int32_t(x) x
#define ARG_uint32_t(x) x
#define ARG_int64_t(x) x
#define ARG_uint64_t(x) x
#define ARG_float(x) x, float_to_int(x)
#define ARG_double(x) x, double_to_int(x)

#define TEST_1(name, type_a, type_d)\
{\
	for(int ia = 0; ia < type_a##_size; ia++)\
	{\
		type_a a = INIT_##type_a(ia);\
		type_d d = name(a);\
		spu_printf( "%s( (%s)", #name, #type_a );\
		spu_printf( FORMAT_##type_a ") = ", ARG_##type_a(a) );\
		spu_printf( "(%s)" , #type_d);\
		spu_printf( FORMAT_##type_d "\n", ARG_##type_d(d) );\
	}\
}

#define TEST_2(name, type_a, type_b, type_d)\
{\
	for(int ia = 0; ia < type_a##_size; ia++)\
	{\
		for(int ib = 0; ib < type_b##_size; ib++)\
		{\
			type_a a = INIT_##type_a(ia);\
			type_b b = INIT_##type_b(ib);\
			type_d d = name(a, b);\
			spu_printf( "%s( (%s)" , #name, #type_a);\
			spu_printf( FORMAT_##type_a ", " , ARG_##type_a(a));\
			spu_printf( "(%s)" , #type_b);\
			spu_printf( FORMAT_##type_b , ARG_##type_b(b));\
			spu_printf( ") = (%s)" , #type_d);\
			spu_printf( FORMAT_##type_d "\n" , ARG_##type_d(d));\
		}\
	}\
}

#define TEST_3(name, type_a, type_b, type_c, type_d)\
{\
	for(int ia = 0; ia < type_a##_size; ia++)\
	{\
		for(int ib = 0; ib < type_b##_size; ib++)\
		{\
			for(int ic = 0; ic < type_c##_size; ic++)\
			{\
				type_a a = INIT_##type_a(ia);\
				type_b b = INIT_##type_b(ib);\
				type_c c = INIT_##type_c(ic);\
				type_d d = name(a, b, c);\
				spu_printf( "%s( (%s)" , #name, #type_a);\
				spu_printf( FORMAT_##type_a ", " , ARG_##type_a(a));\
				spu_printf( "(%s)" , #type_b);\
				spu_printf( FORMAT_##type_b ", ", ARG_##type_b(b));\
				spu_printf( "(%s)" , #type_c);\
				spu_printf( FORMAT_##type_c , ARG_##type_c(c));\
				spu_printf( ") = (%s)" , #type_d);\
				spu_printf( FORMAT_##type_d "\n" , ARG_##type_d(d));\
			}\
		}\
	}\
}

#define TEST_1_run_l(name, type_a, a, type_d)\
{\
		type_d d = name((type_a)a);\
		spu_printf( "%s( ", #name );\
		spu_printf( "(%s)%d ) = ", #type_a, a );\
		spu_printf( "(%s)" , #type_d);\
		spu_printf( FORMAT_##type_d "\n", ARG_##type_d(d) );\
}

#define TEST_1_l(name, type_a, type_d)\
{\
	TEST_1_run_l(name, type_a, LIT_0, type_d)\
	TEST_1_run_l(name, type_a, LIT_1, type_d)\
	TEST_1_run_l(name, type_a, LIT_2, type_d)\
	TEST_1_run_l(name, type_a, LIT_3, type_d)\
	TEST_1_run_l(name, type_a, LIT_4, type_d)\
	TEST_1_run_l(name, type_a, LIT_5, type_d)\
	TEST_1_run_l(name, type_a, LIT_6, type_d)\
	TEST_1_run_l(name, type_a, LIT_7, type_d)\
	TEST_1_run_l(name, type_a, LIT_8, type_d)\
	TEST_1_run_l(name, type_a, LIT_9, type_d)\
	TEST_1_run_l(name, type_a, LIT_10, type_d)\
	TEST_1_run_l(name, type_a, LIT_11, type_d)\
	TEST_1_run_l(name, type_a, LIT_12, type_d)\
	TEST_1_run_l(name, type_a, LIT_13, type_d)\
	TEST_1_run_l(name, type_a, LIT_14, type_d)\
	TEST_1_run_l(name, type_a, LIT_15, type_d)\
}

#define TEST_2_run_vl(name, type_a, type_b, b, type_d)\
{\
	for(int ia = 0; ia < type_a##_size; ia++)\
	{\
		type_a a = INIT_##type_a(ia);\
		type_d d = name(a, (type_b)b);\
		spu_printf( "%s( (%s)" , #name, #type_a);\
		spu_printf( FORMAT_##type_a ", " , ARG_##type_a(a));\
		spu_printf( "(%s)%d", #type_b, b);\
		spu_printf( " ) = (%s)" , #type_d);\
		spu_printf( FORMAT_##type_d "\n" , ARG_##type_d(d));\
	}\
}

#define TEST_2_run_lv(name, type_a, a, type_b, type_d)\
{\
	for(int ib = 0; ib < type_b##_size; ib++)\
	{\
		type_b b = INIT_##type_b(ib);\
		type_d d = name((type_a)a, b);\
		spu_printf( "%s( (%s)%d, ", #name, #type_a, a);\
		spu_printf( "(%s)" , #type_b);\
		spu_printf( FORMAT_##type_b , ARG_##type_b(b));\
		spu_printf( ") = (%s)" , #type_d);\
		spu_printf( FORMAT_##type_d "\n" , ARG_##type_d(d));\
	}\
}

#define TEST_2_vl(name, type_a, type_b, type_d)\
{\
	TEST_2_run_vl(name, type_a, type_b, LIT_0, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_1, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_2, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_3, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_4, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_5, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_6, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_7, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_8, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_9, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_10, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_11, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_12, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_13, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_14, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_15, type_d)\
}

#define TEST_2_vl_6(name, type_a, type_b, type_d)\
{\
	TEST_2_run_vl(name, type_a, type_b, LIT_0, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_1, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_2, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_3, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_4, type_d)\
	TEST_2_run_vl(name, type_a, type_b, LIT_5, type_d)\
}

#define TEST_2_lv(name, type_a, type_b, type_d)\
{\
	TEST_2_run_lv(name, type_a, LIT_0, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_1, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_2, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_3, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_4, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_5, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_6, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_7, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_8, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_9, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_10, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_11, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_12, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_13, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_14, type_b, type_d)\
	TEST_2_run_lv(name, type_a, LIT_15, type_b, type_d)\
}

#define TEST_3_run_vvl(name, type_a, type_b, type_c, c, type_d)\
{\
	for(int ia = 0; ia < type_a##_size; ia++)\
	{\
		for(int ib = 0; ib < type_b##_size; ib++)\
		{\
			type_a a = INIT_##type_a(ia);\
			type_b b = INIT_##type_b(ib);\
			type_d d = name(a, b, (type_c)c);\
			spu_printf( "%s( (%s)" , #name, #type_a);\
			spu_printf( FORMAT_##type_a ", " , ARG_##type_a(a));\
			spu_printf( "(%s)" , #type_b);\
			spu_printf( FORMAT_##type_b ", ", ARG_##type_b(b));\
			spu_printf( "(%s)%d" , #type_c, c);\
			spu_printf( " ) = (%s)" , #type_d);\
			spu_printf( FORMAT_##type_d "\n" , ARG_##type_d(d));\
		}\
	}\
}

#define TEST_3_vvl(name, type_a, type_b, type_c, type_d)\
{\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_0, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_1, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_2, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_3, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_4, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_5, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_6, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_7, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_8, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_9, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_10, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_11, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_12, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_13, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_14, type_d)\
	TEST_3_run_vvl(name, type_a, type_b, type_c, LIT_15, type_d)\
}


#endif