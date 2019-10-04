#ifndef CMHH

#define CMHH
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"


#define M_PI       3.14159265358979323846

//screen dimensions
#define W 1024
#define W2 512
#define H 640
#define H2 320

//view angles
#define AW 128
#define AH 64
#define AW2 80
#define AH2 40
//screen pixels per degree
#define HAH 8
#define WAW 8

#define LIP(x, a, b) (((a)*(x))/(b)) 

#define LIP_FT(x, y, a, b) ( ((x)*((b)-(a)) + (y)*(a))/(b) ) 

typedef union
{
	unsigned int i;
	unsigned char c[4];
}_clr;

#define CA 3
#define CR 2
#define CG 1
#define CB 0

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

#define FREEIF( p ) if( NULL != p ) free(p); p=NULL;
#define MALSET( p, s ) p = malloc(s); memset( p, 0, s);


int ang_norm( int a )
{
	while( a >= 180 ) a -=360;
	while( a < -180 ) a +=360;

	return a;
}



//scale & distance coeff
#define PERDST 150

//precalculated cos, sin, depth, scale
float pcCOS[360*8];
float pcSIN[360*8];
int pcDepth[PERDST+1];
float pcScale[PERDST+1];

#define ANGWR180( a ) while( (a) >= 180 ) (a)-=360; while( (a) < -180 ) (a)+=360;
#define ANGWR036( a ) while( (a) < 0 ) (a)+=360; while( (a) >= 360 ) (a)-=360;

#define MSIN( ang ) ( pcSIN[(int)((ang)*8)] )
#define MCOS( ang ) ( pcCOS[(int)((ang)*8)] )

#define DSNEAR 4
#define DSMED 15

void c_precalc()
{
	int i=0;
	int h_med = 10;
	int h_near = 200;
	int h_sum = 0;
	float a = 0.0;

	for( i=0; i<=90*8; i++ )
	{
		pcCOS[i] = cos( (a*M_PI)/180.0);
		pcCOS[180*8 - i] = -pcCOS[i];
		pcCOS[360*8 - i] = pcCOS[i];
		pcCOS[180*8 + i] = -pcCOS[i];

		pcSIN[i] = sin( (a*M_PI)/180.0);
		pcSIN[180*8 - i] = pcSIN[i];
		pcSIN[360*8 - i] = -pcSIN[i];
		pcSIN[180*8 + i] = -pcSIN[i];

		a += 0.125;
	}

	for( i=0; i<=PERDST; i++ )
	{

		if( i < DSNEAR )
		{
			pcDepth[i] = LIP_FT( h_near, h_med, i, DSNEAR );
			pcScale[i] = LIP_FT( 160.0, 100.0, i, DSNEAR );
		}
		else if( i < DSMED )
		{
			pcDepth[i] = LIP_FT( h_med, 1, i- DSNEAR, DSMED - DSNEAR );
			pcScale[i] = LIP_FT( 100.0, 80.0, i - DSNEAR, DSMED - DSNEAR);
		}
		else 
		{
			pcDepth[i] = 1;
			pcScale[i] = LIP_FT( 80.0, 2.0, i, PERDST );
		}

		h_sum += pcDepth[i];
	}

	return;
}


_clr *gclr_base = NULL;
_clr *gclr_env = NULL;
_clr *gclr_res = NULL;

int gclr_lc = 0;



typedef struct
{
	unsigned short w;
	unsigned short h;
	void* data;
}_imgw;


_imgw* imgw_init(unsigned short  w, unsigned short  h)
{
	_imgw *ret = NULL;
	
	MALSET(ret,sizeof(_imgw));
	MALSET(ret->data,w*h*4);
	ret->w = w;
	ret->h = h;
	return ret;
}


#define SPIX( IM, X, Y, C ) if( (X)>0 && (X)<(IM)->w && (Y)>0 && (Y)<(IM)->h ) memcpy( &((int*)(IM)->data)[(IM)->w*(Y) + (X)],&(C), 4 )


void SWAP(int *a, int *b)
{
	*a = *a + *b; 
	*b = *a - *b; 
	*a = *a - *b;
}

int dls_i;
int dls_dx;
int dls_dy;
int dls_dxabs;
int dls_dyabs;
int dls_sdx;
int dls_sdy;
int dls_x;
int dls_y;
int dls_px;
int dls_py;

int draw_line( _imgw *img, int xfr, int yfr, int xto, int yto, unsigned int clr )
{

	dls_i = 0;
	dls_dx = xto - xfr;
	dls_dy = yto - yfr;
	dls_dxabs = abs(dls_dx);
	dls_dyabs = abs(dls_dy);
	dls_sdx = (dls_dx>0)?1:-1;
	dls_sdy = (dls_dy>0)?1:-1;
	dls_x = dls_dxabs>>1;
	dls_y = dls_dyabs>>1;
	dls_px = xfr;
	dls_py = yfr;

	SPIX( img, xfr, yfr, clr );

	if( dls_dxabs >= dls_dyabs )
	{
		for( dls_i=0; dls_i<dls_dxabs; dls_i++ )
		{
			dls_y += dls_dyabs;
			if( dls_y >= dls_dxabs )
			{
				dls_y -= dls_dxabs;
				dls_py+=dls_sdy;
			}
			dls_px+=dls_sdx;
			SPIX( img, dls_px, dls_py, clr );
		}
	}
	else
	{
		for( dls_i=0; dls_i<dls_dyabs; dls_i++ )
		{
			dls_x += dls_dxabs;
			if( dls_x >= dls_dyabs )
			{
				dls_x -= dls_dyabs;
				dls_px+=dls_sdx;
			}
			dls_py+=dls_sdy;
			SPIX( img, dls_px, dls_py, clr );
		}
	}
}

char *cstr(char *in)
{
	char *ret = NULL;
	int len = 0;
	
	if( NULL == in ) return NULL;
	if( (len = strlen(in)) <= 0 ) return NULL;
	
	ret = (char*)malloc( len + 1);
	strncpy(ret,in,len);
	ret[len] = '\0';
	
	return ret;
}

void** LADD( void** lold, void* lel, int *lcnt )
{
	int i=0;
	void **lnew = NULL;
	
	MALSET( lnew, ( (*lcnt) + 1)*sizeof(void*) );
	
	for( i=0; i<(*lcnt); i++ ) lnew[i] = lold[i];
	
	FREEIF(lold);
	
	lnew[(*lcnt)] = lel;	
	(*lcnt)++;
	return lnew;
}




#define Wprs 0x0001
#define Sprs 0x0002
#define Aprs 0x0004
#define Dprs 0x0008

unsigned int pmvs = 0;

typedef struct
{
	unsigned int clr;
	unsigned int proc;
	unsigned int brtype;
}_clgrd;

typedef struct
{
	_clr env_clr;
	unsigned int env_fmin;
	unsigned int env_fmax_dst;
	unsigned int env_fmax;

	_clr gll_clr;
	int gll_force;
	int gll_contrast;
	int gll_a;
	int gll_b;

	_clgrd **skygr_clr;
	int skygr_clrc;
}_atm;

typedef struct
{
	float x;
	float y;
	float z;
	int a;
	int b;
}_cam;


#endif