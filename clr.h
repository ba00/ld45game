#ifndef CLRH
#define CLRH

#include "util.h"

//typical colors
unsigned int CLR_ORNG = 0xFFFF8800;
unsigned int CLR_YEL1 = 0xFFAA9960;
unsigned int CLR_BRED = 0xFFFF0000;
unsigned int CLR_GRAY = 0xFF808080;
unsigned int CLR_LGRAY = 0xFFA0A0A0;

unsigned int CLR_BLUE = 0xFF0080FF;
unsigned int CLR_DBLUE = 0xFF000770;

unsigned int CLR_FUCIA = 0xFFAA00CC;

unsigned int CLR_GREEN = 0xFF22BB44;
unsigned int CLR_DGREEN = 0xFF004020;

unsigned int CLR_LBROWN = 0xFF503020;
unsigned int CLR_DBROWN = 0xFF281611;

unsigned int CLR_YELBR = 0xFF885020;
unsigned int CLR_YELGR = 0xFF669920;
unsigned int CLR_YELBL = 0xFF6699FF;

typedef union
{
	unsigned int i;
	unsigned char c[4];
}_clr;

_clr *gclr_base = NULL;
_clr *gclr_env = NULL;
_clr *gclr_res = NULL;

int gclr_lc = 0;

//green
#define TGND 0
#define TGN 1
#define TGNY 2
//gray shades
#define TGRL 3
#define TGR 4
#define TGRD 5
#define TGRY 6
#define TGRDD 7
#define TGRBR 8
//yellow
#define TY 9
#define TYBR 10
#define TYD 11
#define TYL 12
#define TYR 13


void clr_init()
{
	gclr_lc = 14;
	MALSET( gclr_base, gclr_lc*sizeof(_clr));
	MALSET( gclr_env, gclr_lc*sizeof(_clr));
	MALSET( gclr_res, gclr_lc*sizeof(_clr));

	gclr_base[TGND].i = CLR_DGREEN;
	gclr_base[TGN].i = CLR_GREEN;
	gclr_base[TGNY].i = CLR_YELGR;
	gclr_base[TGRL].i = CLR_LGRAY;
	gclr_base[TGR].i = CLR_GRAY;
	gclr_base[TGRD].i = 0xFF606060;
	gclr_base[TGRY].i = 0xFF907040;
	gclr_base[TGRDD].i = 0xFF404040;
	gclr_base[TGRBR].i = 0xFF404020;

	gclr_base[TY].i = 0xFFAA9960;
	gclr_base[TYBR].i = 0xFF885020;
	gclr_base[TYD].i = 0xFF885520;
	gclr_base[TYL].i = 0xFFCCAA80;
	gclr_base[TYR].i = 0xFFFF8030;
}

#define CA 3
#define CR 2
#define CG 1
#define CB 0





#endif