#include <cell.h>
#include <spu_printf.h>
#include <spu_internals.h>
#include <math.h>
#include <float.h>


// Loops with ints
#define ITERATE1i(name, instruction) \
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++) { \
		volatile vec_int4 v1 = (vec_int4)(testInts[i]); \
		volatile vec_int4 r  = (vec_int4)instruction((qword)v1); \
		spu_printf(name"([%02d]) -> %08x %08x %08x %08x\n",i,r[0],r[1],r[2],r[3]); \
	}

#define ITERATE2i(name, instruction) \
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++) \
		for (unsigned int j=0; j<sizeof(testInts)/sizeof(int); j++) { \
			volatile vec_int4 v1 = (vec_int4)(testInts[i]); \
			volatile vec_int4 v2 = (vec_int4)(testInts[j],testInts[j],0.0,0.0); \
			volatile vec_int4 r  = (vec_int4)instruction((qword)v1,(qword)v2); \
			spu_printf(name"([%02d],[%02d]) -> %08x %08x %08x %08x\n",i,j,r[0],r[1],r[2],r[3]); \
		}

#define ITERATE3i(name, instruction) \
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++) \
		for (unsigned int j=0; j<sizeof(testInts)/sizeof(int); j++) \
			for (unsigned int k=0; k<sizeof(testInts)/sizeof(int); k++) { \
				volatile vec_int4 v1 = (vec_int4)(testInts[i]); \
				volatile vec_int4 v2 = (vec_int4)(testInts[j],testInts[j],0.0,0.0); \
				volatile vec_int4 v3 = (vec_int4)(testInts[k],0.0,testInts[k],0.0); \
				volatile vec_int4 r  = (vec_int4)instruction((qword)v1,(qword)v2,(qword)v3); \
				spu_printf(name"([%02d],[%02d],[%02d]) -> %08x %08x %08x %08x\n",i,j,k,r[0],r[1],r[2],r[3]); \
			}

// Loops with ints and immediate values
#define ITERATE1iimm(name, instruction, imm) \
	for (unsigned int i=0; i<sizeof(testInts)/sizeof(int); i++) { \
		volatile vec_int4 v1 = (vec_int4)(testInts[i]); \
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


int main()
{
	//////////////////////////////////////
	// INTEGER AND LOGICAL INSTRUCTIONS //
	//////////////////////////////////////

	// Arithmetic instructions
	ITERATE2i("ah      ", si_ah);
	ITERATE2i("a       ", si_a);
	ITERATE3i("addx    ", si_addx);
	ITERATE2i("sfh     ", si_sfh);
	ITERATE2i("sf      ", si_sf);
	ITERATE3i("sfx     ", si_sfx);
	ITERATE2i("mpy     ", si_mpy);
	ITERATE2i("mpyu    ", si_mpyu);
	ITERATE3i("mpya    ", si_mpya);
	ITERATE2i("mpyh    ", si_mpyh);
	ITERATE2i("mpys    ", si_mpys);
	ITERATE2i("mpyhh   ", si_mpyhh);
	ITERATE3i("mpyhha  ", si_mpyhha);
	ITERATE2i("mpyhhu  ", si_mpyhhu);
	ITERATE3i("mpyhhau ", si_mpyhhau);
	ITERATE1iimm("ahi     ", si_ahi, 0);
	ITERATE1iimm("ahi     ", si_ahi, 1);
	ITERATE1iimm("ahi     ", si_ahi, 511);
	ITERATE1iimm("ahi     ", si_ahi, -512);
	ITERATE1iimm("ai      ", si_ai, 0);
	ITERATE1iimm("ai      ", si_ai, 1);
	ITERATE1iimm("ai      ", si_ai, 511);
	ITERATE1iimm("ai      ", si_ai, -512);
	ITERATE1iimm("sfhi    ", si_sfhi, 0);
	ITERATE1iimm("sfhi    ", si_sfhi, 1);
	ITERATE1iimm("sfhi    ", si_sfhi, 511);
	ITERATE1iimm("sfhi    ", si_sfhi, -512);
	ITERATE1iimm("sfi     ", si_sfi, 0);
	ITERATE1iimm("sfi     ", si_sfi, 1);
	ITERATE1iimm("sfi     ", si_sfi, 511);
	ITERATE1iimm("sfi     ", si_sfi, -512);
	ITERATE1iimm("mpyi    ", si_mpyi, 0);
	ITERATE1iimm("mpyi    ", si_mpyi, 1);
	ITERATE1iimm("mpyi    ", si_mpyi, 511);
	ITERATE1iimm("mpyi    ", si_mpyi, -512);
	ITERATE1iimm("mpyui   ", si_mpyui, 0);
	ITERATE1iimm("mpyui   ", si_mpyui, 1);
	ITERATE1iimm("mpyui   ", si_mpyui, 511);
	ITERATE1iimm("mpyui   ", si_mpyui, -512);

	// Logic instructions
	ITERATE2i("and     ", si_and);
	ITERATE2i("andc    ", si_andc);
	ITERATE2i("or      ", si_or);
	ITERATE2i("orc     ", si_orc);
	ITERATE1i("orx     ", si_orx);
	ITERATE2i("xor     ", si_xor);
	ITERATE2i("nand    ", si_nand);
	ITERATE2i("nor     ", si_nor);
	ITERATE2i("eqv     ", si_eqv);
	ITERATE1iimm("andbi   ", si_andbi, 0);
	ITERATE1iimm("andbi   ", si_andbi, 1);
	ITERATE1iimm("andbi   ", si_andbi, 511);
	ITERATE1iimm("andbi   ", si_andbi, -512);
	ITERATE1iimm("andhi   ", si_andhi, 0);
	ITERATE1iimm("andhi   ", si_andhi, 1);
	ITERATE1iimm("andhi   ", si_andhi, 511);
	ITERATE1iimm("andhi   ", si_andhi, -512);
	ITERATE1iimm("andi    ", si_andi, 0);
	ITERATE1iimm("andi    ", si_andi, 1);
	ITERATE1iimm("andi    ", si_andi, 511);
	ITERATE1iimm("andi    ", si_andi, -512);
	ITERATE1iimm("orbi    ", si_orbi, 0);
	ITERATE1iimm("orbi    ", si_orbi, 1);
	ITERATE1iimm("orbi    ", si_orbi, 511);
	ITERATE1iimm("orbi    ", si_orbi, -512);
	ITERATE1iimm("orhi    ", si_orhi, 0);
	ITERATE1iimm("orhi    ", si_orhi, 1);
	ITERATE1iimm("orhi    ", si_orhi, 511);
	ITERATE1iimm("orhi    ", si_orhi, -512);
	ITERATE1iimm("ori     ", si_ori, 0);
	ITERATE1iimm("ori     ", si_ori, 1);
	ITERATE1iimm("ori     ", si_ori, 511);
	ITERATE1iimm("ori     ", si_ori, -512);
	ITERATE1iimm("xorbi   ", si_xorbi, 0);
	ITERATE1iimm("xorbi   ", si_xorbi, 1);
	ITERATE1iimm("xorbi   ", si_xorbi, 511);
	ITERATE1iimm("xorbi   ", si_xorbi, -512);
	ITERATE1iimm("xorhi   ", si_xorhi, 0);
	ITERATE1iimm("xorhi   ", si_xorhi, 1);
	ITERATE1iimm("xorhi   ", si_xorhi, 511);
	ITERATE1iimm("xorhi   ", si_xorhi, -512);
	ITERATE1iimm("xori    ", si_xori, 0);
	ITERATE1iimm("xori    ", si_xori, 1);
	ITERATE1iimm("xori    ", si_xori, 511);
	ITERATE1iimm("xori    ", si_xori, -512);

	// Other
	ITERATE2i("cg      ", si_cg);
	ITERATE3i("cgx     ", si_cgx);
	ITERATE2i("bg      ", si_bg);
	ITERATE3i("bgx     ", si_bgx);
	ITERATE1i("clz     ", si_clz);
	ITERATE1i("cntb    ", si_cntb);
	ITERATE1i("fsmb    ", si_fsmb);
	ITERATE1i("fsmh    ", si_fsmh);
	ITERATE1i("fsm     ", si_fsm);
	ITERATE1i("gbb     ", si_gbb);
	ITERATE1i("gbh     ", si_gbh);
	ITERATE1i("gb      ", si_gb);
	ITERATE2i("avgb    ", si_avgb);
	ITERATE2i("absdb   ", si_absdb);
	ITERATE2i("sumb    ", si_sumb);
	ITERATE1i("xsbh    ", si_xsbh);
	ITERATE1i("xshw    ", si_xshw);
	ITERATE1i("xswd    ", si_xswd);
	ITERATE3i("selb    ", si_selb);
	ITERATE3i("shufb   ", si_shufb);


	///////////////////////////////////
	// SHIFT AND ROTATE INSTRUCTIONS //
	///////////////////////////////////

	// Shift
	ITERATE2i("shlh     ", si_shlh);
	ITERATE2i("shl      ", si_shl);
	ITERATE2i("shlqbi   ", si_shlqbi);
	ITERATE2i("shlqby   ", si_shlqby);
	ITERATE2i("shlqbybi ", si_shlqbybi);
	ITERATE1iimm("shlhi    ", si_shlhi, 0);
	ITERATE1iimm("shlhi    ", si_shlhi, 1);
	ITERATE1iimm("shlhi    ", si_shlhi, 127);
	ITERATE1iimm("shlhi    ", si_shlhi, -64);
	ITERATE1iimm("shli     ", si_shli, 0);
	ITERATE1iimm("shli     ", si_shli, 1);
	ITERATE1iimm("shli     ", si_shli, 127);
	ITERATE1iimm("shli     ", si_shli, -64);
	ITERATE1iimm("shlqbii  ", si_shlqbii, 0);
	ITERATE1iimm("shlqbii  ", si_shlqbii, 1);
	ITERATE1iimm("shlqbii  ", si_shlqbii, 127);
	ITERATE1iimm("shlqbii  ", si_shlqbii, -64);
	ITERATE1iimm("shlqbyi  ", si_shlqbyi, 0);
	ITERATE1iimm("shlqbyi  ", si_shlqbyi, 1);
	ITERATE1iimm("shlqbyi  ", si_shlqbyi, 127);
	ITERATE1iimm("shlqbyi  ", si_shlqbyi, -64);

	// Rotate
	ITERATE2i("roth     ", si_roth);
	ITERATE2i("rot      ", si_rot);
	ITERATE2i("rotqbi   ", si_rotqbi);
	ITERATE2i("rotqby   ", si_rotqby);
	ITERATE2i("rotqbybi ", si_rotqbybi);
	ITERATE1iimm("rothi    ", si_rothi, 0);
	ITERATE1iimm("rothi    ", si_rothi, 1);
	ITERATE1iimm("rothi    ", si_rothi, 127);
	ITERATE1iimm("rothi    ", si_rothi, -64);
	ITERATE1iimm("roti     ", si_roti, 0);
	ITERATE1iimm("roti     ", si_roti, 1);
	ITERATE1iimm("roti     ", si_roti, 127);
	ITERATE1iimm("roti     ", si_roti, -64);
	ITERATE1iimm("rotqbii  ", si_rotqbii, 0);
	ITERATE1iimm("rotqbii  ", si_rotqbii, 1);
	ITERATE1iimm("rotqbii  ", si_rotqbii, 127);
	ITERATE1iimm("rotqbii  ", si_rotqbii, -64);
	ITERATE1iimm("rotqbyi  ", si_rotqbyi, 0);
	ITERATE1iimm("rotqbyi  ", si_rotqbyi, 1);
	ITERATE1iimm("rotqbyi  ", si_rotqbyi, 127);
	ITERATE1iimm("rotqbyi  ", si_rotqbyi, -64);

	// Rotate and Mask
	ITERATE2i("rothm     ", si_rothm);
	ITERATE2i("rotm      ", si_rotm);
	ITERATE2i("rotqmbi   ", si_rotqmbi);
	ITERATE2i("rotqmby   ", si_rotqmby);
	ITERATE2i("rotqmbybi ", si_rotqmbybi);
	ITERATE1iimm("rothmi   ", si_rothmi, 0);
	ITERATE1iimm("rothmi   ", si_rothmi, 1);
	ITERATE1iimm("rothmi   ", si_rothmi, 127);
	ITERATE1iimm("rothmi   ", si_rothmi, -64);
	ITERATE1iimm("rotmi    ", si_roti, 0);
	ITERATE1iimm("rotmi    ", si_roti, 1);
	ITERATE1iimm("rotmi    ", si_roti, 127);
	ITERATE1iimm("rotmi    ", si_roti, -64);
	ITERATE1iimm("rotqmbii ", si_rotqmbii, 0);
	ITERATE1iimm("rotqmbii ", si_rotqmbii, 1);
	ITERATE1iimm("rotqmbii ", si_rotqmbii, 127);
	ITERATE1iimm("rotqmbii ", si_rotqmbii, -64);
	ITERATE1iimm("rotqmbyi ", si_rotqmbyi, 0);
	ITERATE1iimm("rotqmbyi ", si_rotqmbyi, 1);
	ITERATE1iimm("rotqmbyi ", si_rotqmbyi, 127);
	ITERATE1iimm("rotqmbyi ", si_rotqmbyi, -64);

	// Rotate and Mask Algebraic
	ITERATE2i("rotmah    ", si_rotmah);
	ITERATE2i("rotma     ", si_rotma);
	ITERATE1iimm("rotmahi  ", si_rotmahi, 0);
	ITERATE1iimm("rotmahi  ", si_rotmahi, 1);
	ITERATE1iimm("rotmahi  ", si_rotmahi, 127);
	ITERATE1iimm("rotmahi  ", si_rotmahi, -64);
	ITERATE1iimm("rotmai   ", si_rotmai, 0);
	ITERATE1iimm("rotmai   ", si_rotmai, 1);
	ITERATE1iimm("rotmai   ", si_rotmai, 127);
	ITERATE1iimm("rotmai   ", si_rotmai, -64);


	//////////////////////////
	// COMPARE INSTRUCTIONS //
	//////////////////////////

	// Compare Equal
	ITERATE2i("ceqb    ", si_ceqb);
	ITERATE2i("ceqh    ", si_ceqh);
	ITERATE2i("ceq     ", si_ceq);
	ITERATE1iimm("ceqbi   ", si_ceqbi, 0);
	ITERATE1iimm("ceqbi   ", si_ceqbi, 1);
	ITERATE1iimm("ceqbi   ", si_ceqbi, 255);
	ITERATE1iimm("ceqbi   ", si_ceqbi, -128);
	ITERATE1iimm("ceqhi   ", si_ceqhi, 0);
	ITERATE1iimm("ceqhi   ", si_ceqhi, 1);
	ITERATE1iimm("ceqhi   ", si_ceqhi, 511);
	ITERATE1iimm("ceqhi   ", si_ceqhi, -512);
	ITERATE1iimm("ceqi    ", si_ceqi, 0);
	ITERATE1iimm("ceqi    ", si_ceqi, 1);
	ITERATE1iimm("ceqi    ", si_ceqi, 511);
	ITERATE1iimm("ceqi    ", si_ceqi, -512);

	// Compare Greater
	ITERATE2i("cgtb    ", si_cgtb);
	ITERATE2i("cgth    ", si_cgth);
	ITERATE2i("cgt     ", si_cgt);
	ITERATE1iimm("cgtbi   ", si_cgtbi, 0);
	ITERATE1iimm("cgtbi   ", si_cgtbi, 1);
	ITERATE1iimm("cgtbi   ", si_cgtbi, 255);
	ITERATE1iimm("cgtbi   ", si_cgtbi, -128);
	ITERATE1iimm("cgthi   ", si_cgthi, 0);
	ITERATE1iimm("cgthi   ", si_cgthi, 1);
	ITERATE1iimm("cgthi   ", si_cgthi, 511);
	ITERATE1iimm("cgthi   ", si_cgthi, -512);
	ITERATE1iimm("cgti    ", si_cgti, 0);
	ITERATE1iimm("cgti    ", si_cgti, 1);
	ITERATE1iimm("cgti    ", si_cgti, 511);
	ITERATE1iimm("cgti    ", si_cgti, -512);

	// Compare Logical Greater
	ITERATE2i("clgtb   ", si_clgtb);
	ITERATE2i("clgth   ", si_clgth);
	ITERATE2i("clgt    ", si_clgt);
	ITERATE1iimm("clgtbi  ", si_clgtbi, 0);
	ITERATE1iimm("clgtbi  ", si_clgtbi, 1);
	ITERATE1iimm("clgtbi  ", si_clgtbi, 255);
	ITERATE1iimm("clgtbi  ", si_clgtbi, -128);
	ITERATE1iimm("clgthi  ", si_clgthi, 0);
	ITERATE1iimm("clgthi  ", si_clgthi, 1);
	ITERATE1iimm("clgthi  ", si_clgthi, 511);
	ITERATE1iimm("clgthi  ", si_clgthi, -512);
	ITERATE1iimm("clgti   ", si_clgti, 0);
	ITERATE1iimm("clgti   ", si_clgti, 1);
	ITERATE1iimm("clgti   ", si_clgti, 511);
	ITERATE1iimm("clgti   ", si_clgti, -512);
}
