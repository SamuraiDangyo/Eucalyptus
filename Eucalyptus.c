/**
*
* Eucalyptus, KPK Bitbases Generator
* Copyright (C) 2019 Toni Helminen ( kalleankka1@gmail.com )
*
* Eucalyptus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Eucalyptus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**/

/**
* INCLUDES
**/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>

/**
* EUCALYPTUS INCLUDES
**/

#include "Eucalyptus.h"

/**
* CONSTANTS
**/

#define NAME "Eucalyptus"
#define VERSION "1.04"
#define AUTHOR "Toni Helminen"

#define MCOUNT ((2 * 24 * 64 * 64) / 64)

/**
* MACROS
**/

#define FLOAT(a) ((float) (a))
#define LEGAL(i) ( ! ((i) & 0x88))
#define X(a) ((a) & 7)
#define Y(a) ((a) >> 3)
#define ERROR() P("Error: Line: %i", __LINE__);
#define EUCALYPTUS_ASSERT(test) if ( ! (test)) {ERROR() exit(0);}

/**
* GLOBALS
**/

static int KING_MOVES[64][8] = {{0}};
static int KING_MOVES_L[64] = {0};
static int PAWN_ATTACK[64][2] = {{0}};
static int PAWN_ATTACK_L[64] = {0};
static int WINS[2][64][64][64] = {{{{0}}}};
static int RESULT[100][2][64][64][64] = {{{{{0}}}}};
static int RESULT2[100][2][64][64][64] = {{{{{0}}}}};
static const int KING_MOVES_D[2][8] = {{17,15,-15,-17,16,-16,1,-1},{9,7,-7,-9,8,-8,1,-1}};
static unsigned long long EUCALYPTUS_KPK[MCOUNT] = {0};
static bool CPP = 0;

/**
* FUNCTIONS
**/

static float Time_diff(const clock_t time)
{
	return FLOAT(clock() - time) / FLOAT(CLOCKS_PER_SEC);
}

static int Max(const int a, const int b)
{
	return a > b ? a : b;
}

static int Min(const int a, const int b)
{
	return a < b ? a : b;
}

static void P(const char *f, ...)
{
	va_list a;
	va_start(a, f);
	vfprintf(stdout, f, a);
	va_end(a);
	fprintf(stdout, "\n");
	fflush(stdout);
}
	
static bool Attacks(const int k, const int i)
{
	int j;
	for (j = 0; j < KING_MOVES_L[k]; j++)
		if (KING_MOVES[k][j] == i)
			return 1;
	return 0;
}

static bool Attacks2(const int p, const int i)
{
	int j;
	for (j = 0; j < PAWN_ATTACK_L[p]; j++)
		if (PAWN_ATTACK[p][j] == i)
			return 1;
	return 0;
}

static bool Stalemate(const int wp, const int wk, const int bk)
{
	int i;
	for (i = 0; i < KING_MOVES_L[bk]; i++)
		if ( ! Attacks(wk, KING_MOVES[bk][i]) && ! Attacks2(wp, KING_MOVES[bk][i]))
			return 0;
	return 1;
}

static char Win(const int wp, const int wk, const int bk, const int wtm, const int d)
{
	int i;
	int r = 0;
	if (RESULT2[d][wtm][wp][wk][bk])
		return RESULT[d][wtm][wp][wk][bk];
	if ( ! wtm && Attacks(bk, wp) && ! Attacks(wk, wp))
		return -1;
	if (X(wp) > 3)
		return Win(8 * Y(wp) + 7 - X(wp), 8 * Y(wk) + 7 - X(wk), 8 * Y(bk) + 7 - X(bk), wtm, d);
	if (Y(wp) == 7)
		return (Attacks(wk, wp) || ! Attacks(bk, wp)) ? 1 : -1;
	if (wtm && Attacks2(wp, bk))
		return 1;
	if (wtm && Attacks(wk, bk))
		return 1;
	if ( ! wtm && Attacks(bk, wk))
		return -1;
	if (Stalemate(wp, wk, bk))
		return -1;
	if (WINS[wtm][wp][wk][bk])
		return WINS[wtm][wp][wk][bk];
	if (wk == wp || wp == bk || wk == bk || wp < 8)
		return 0;
	if (d <= 0)
		return 0;
	if (wtm) {
		if (wp + 8 != wk && wp + 8 != bk)
			r = Max(-1, Win(wp + 8, wk, bk, ! wtm, d - 1));
		if (r == 1)
			goto goto_exit;
		if (Y(wp) == 1 && wp + 8 != wk && wp + 8 != bk && wp + 16 != wk && wp + 16 != bk)
			r = Max(r, Win(wp + 16, wk, bk, ! wtm, d - 1));
		if (r == 1)
			goto goto_exit;
		for (i = 0; i < KING_MOVES_L[wk]; i++) {
			r = Max(r, Win(wp, KING_MOVES[wk][i], bk, ! wtm, d - 1));
			if (r == 1)
				goto goto_exit;
		}
	} else {
		r = 1;
		for (i = 0; i < KING_MOVES_L[bk]; i++) {
			r = Min(r, Win(wp, wk, KING_MOVES[bk][i], ! wtm, d - 1));
			if (r == -1)
				goto goto_exit;
		}
	}
goto_exit: // <- goto
	RESULT2[d][wtm][wp][wk][bk] = 1;
	RESULT[d][wtm][wp][wk][bk] = r;
	return r;
}

static void Pack(const int wtm)
{
	int i, j, k, q;
	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++) {
			for (k = 0; k < 64; k++) {
				if ((0x1ULL << i) & 0xFFF0F0F0F0F0F0FFULL)
					continue;
				q = wtm * 24 * 64 * 64 + (4 * (i >> 3) - 4 + (i & 3)) * 64 * 64 + j * 64 + k;
				EUCALYPTUS_KPK[q / 64] |= (WINS[wtm][i][j][k] == 1 ? 0x1ULL : 0) << (q & 63);
			}
		}
	}
}

static bool Probe_Eucalyptus(int white_pawn, int white_king, int black_king, const bool wtm)
{
	int i;
	if ((0x1ULL << white_pawn) & 0xF0F0F0F0F0F0F0F0ULL) {
		white_king = 8 * (white_king >> 3) + (7 - (white_king & 7));
		white_pawn = 8 * (white_pawn >> 3) + (7 - (white_pawn & 7));
		black_king = 8 * (black_king >> 3) + (7 - (black_king & 7));
	}
	i = (wtm ? 1 : 0) * 24 * 64 * 64 + (4 * (white_pawn >> 3) - 4 + (white_pawn & 3)) * 64 * 64 + white_king * 64 + black_king;
	return (EUCALYPTUS_KPK[i / 64] & (0x1ULL << (i & 63))) ? 1 : 0;
}

static void Tests()
{
	// draws
	assert(Probe_Eucalyptus(24, 16, 7, 0));
	assert(Probe_Eucalyptus(24, 16, 7, 1));
	assert(Probe_Eucalyptus(24, 16, 6, 0));
	assert(Probe_Eucalyptus(24, 16, 6, 1));
	assert(Probe_Eucalyptus(19, 18, 37, 1));
	// not draws
	assert( ! Probe_Eucalyptus(32, 16, 41, 0));
	assert( ! Probe_Eucalyptus(24, 16, 41, 1));
	assert( ! Probe_Eucalyptus(24, 16, 41, 0));
	assert( ! Probe_Eucalyptus(11, 3, 19, 1));
}

static void Build()
{
	int w, i, j, k;	
	for (w = 0; w < 2; w++) {
		for (i = 63; i > -1; i--) {
			for (j = 0; j < 64; j++) {
				for (k = 0; k < 64; k++) {
					WINS[w][i][j][k] = Win(i, j, k, w, 49) == 1 ? 1 : 0;
				}
			}
		}
		Pack(w);
	}
	Tests();
}

static void Write_info(FILE* f)
{
	fprintf(f, "/**\n*\n");
	fprintf(f, "* Generated by: Eucalyptus, KPK Bitbases Generator\n*\n");
	fprintf(f, "**/\n\n");
}

static void Write_header()
{
	int i;
	const char *fstr = CPP ? "Eucalyptus_KPK.hpp" : "Eucalyptus_KPK.h";
	FILE *f = fopen(fstr, "w");
	EUCALYPTUS_ASSERT(f != NULL)
	Write_info(f);
	fprintf(f, "#ifndef EUCALYPTUS_KPK_H\n");
	fprintf(f, "#define EUCALYPTUS_KPK_H\n\n");
	fprintf(f, "#include <assert.h>\n");
	fprintf(f, "#include <stdbool.h>\n\n");
	fprintf(f, "bool Probe_Eucalyptus(int white_pawn, int white_king, int black_king, const bool wtm);\n\n");
	fprintf(f, "const unsigned long long EUCALYPTUS_KPK[(2 * 24 * 64 * 64) / 64] = {\n");
	for (i = 0; i < MCOUNT; i++)
		fprintf(f, "\t0x%llxULL%s\n", EUCALYPTUS_KPK[i], i < MCOUNT - 1 ? "," : "");
	fprintf(f, "};\n\n");
	fprintf(f, "#endif /** END EUCALYPTUS_KPK_H **/");
	P("... %s", fstr);
}

static void Write_program()
{
	const char *fstr = CPP ? "Eucalyptus_KPK.cpp" : "Eucalyptus_KPK.c";
	FILE *f = fopen(fstr, "w");
	EUCALYPTUS_ASSERT(f != NULL)
	Write_info(f);
	fprintf(f, "bool Probe_Eucalyptus(int white_pawn, int white_king, int black_king, const bool wtm)\n{\n\
	int i;\n\
	if ((0x1ULL << white_pawn) & 0xF0F0F0F0F0F0F0F0ULL) {\n\
		white_king = 8 * (white_king >> 3) + (7 - (white_king & 7));\n\
		white_pawn = 8 * (white_pawn >> 3) + (7 - (white_pawn & 7));\n\
		black_king = 8 * (black_king >> 3) + (7 - (black_king & 7));\n\
	}\n\
	i = (wtm ? 1 : 0) * 24 * 64 * 64 + (4 * (white_pawn >> 3) - 4 \n\t\t+ (white_pawn & 3)) * 64 * 64 + white_king * 64 + black_king;\n\
	return (EUCALYPTUS_KPK[i / 64] & (0x1ULL << (i & 63))) ? 1 : 0;\n}");
	fclose(f);
	P("... %s", fstr);
}

static void Write()
{
	Write_header();
	Write_program();
}

static void Init()
{
	int i, j, h, l; 
	for (i = 0; i < 64; i++) {
		h = 16 * Y(i) + X(i);
		l = 0;
		for (j = 0; j < 8; j++) {
			if (LEGAL(h + KING_MOVES_D[0][j])) {
				KING_MOVES[i][l] = i + KING_MOVES_D[1][j];
				l++;
				KING_MOVES_L[i] = l;
			}
		}
		l = 0;
		if (LEGAL(h + 15)) {
			PAWN_ATTACK[i][l] = i + 7;
			l++;
		}
		if (LEGAL(h + 17)) {
			PAWN_ATTACK[i][l] = i + 9;
			l++;
		}
		PAWN_ATTACK_L[i] = l;
	}
}

static void Print_help()
{
	P("%s v%s by %s", NAME, VERSION, AUTHOR);
	P("Usage: Eucalyptus [OPTION]... ");
	P("Generate KPK Bitbases For Your Chess Engine\n~~~");
	P("Options:");
	P("--help: This help");
	P("--license: GPLv3");
	P("--cpp: Make c++ files");
	exit(0);
}

static void Print_license()
{
	P("GNU General Public License version 3; for details see LICENSE");
	exit(0);
}

static void Options(int argc, char** argv)
{
	int i;
	for (i = 0; i < argc; i++) {
		if ( ! strcmp(argv[i], "--help"))
			Print_help();
		else if ( ! strcmp(argv[i], "--cpp"))
			CPP = 1;
		else if ( ! strcmp(argv[i], "--license"))
			Print_license();
	}
}

static void Eucalyptus()
{
	clock_t time = clock();
	P("%s v%s by %s", NAME, VERSION, AUTHOR);
	P("Generating ...");
	Init();
	Build();
	Write();
	printf("Done ~ %.3fs\n", Time_diff(time));
}

int main(int argc, char** argv)
{
	Options(argc, argv);
	Eucalyptus();
	return EXIT_SUCCESS;
}
