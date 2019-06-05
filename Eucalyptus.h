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

static float Time_diff(const clock_t time);
static int Max(const int a, const int b);
static int Min(const int a, const int b);
static void P(const char *f, ...);
static bool Attacks(const int k, const int i);
static bool Attacks2(const int p, const int i);
static bool Stalemate(const int wp, const int wk, const int bk);
static char Win(const int wp, const int wk, const int bk, const int wtm, const int d);
static void Pack(const int wtm);
static bool Probe_Eucalyptus(int white_pawn, int white_king, int black_king, const int wtm);
static void Build();
static void Write_header();	
static void Write_program();
static void Tests();
static void Write();
static void Init();
static void Write_info(FILE* f);
static void Print_help();
static void Print_license();
static void Options(int argc, char** argv);
static void Eucalyptus();
