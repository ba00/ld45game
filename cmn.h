#ifndef CMHH

#define CMHH
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "clr.h"

int ang_norm( int a )
{
	while( a >= 180 ) a -=360;
	while( a < -180 ) a +=360;

	return a;
}

//scale & distance coeff
#define PERDST 160

//precalculated cos, sin, depth, scale
float pcCOS[360*8];
float pcSIN[360*8];
int pcDepth[PERDST+1];
float pcScale[PERDST+1];

#define ANGWR180( a ) while( (a) >= 180 ) (a)-=360; while( (a) < -180 ) (a)+=360;
#define ANGWR036( a ) while( (a) < 0 ) (a)+=360; while( (a) >= 360 ) (a)-=360;

#define ANGN360( a ) ((a) <0)?((a)+360):(((a) >=360)?((a)-360):(a))

#define MSIN( ang ) ( pcSIN[(int)((ang)*8)] )
#define MCOS( ang ) ( pcCOS[(int)((ang)*8)] )

#define DSNEAR 5
#define DSMED 20

void c_precalc()
{
	int i=0;
	int h_med = 20;
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
			pcScale[i] = LIP_FT( 200.0, 80.0, i, DSNEAR );
		}
		else if( i < DSMED )
		{
			pcDepth[i] = LIP_FT( h_med, 1, i- DSNEAR, DSMED - DSNEAR );
			pcScale[i] = LIP_FT( 80.0, 40.0, i - DSNEAR, DSMED - DSNEAR);
		}
		else 
		{
			pcDepth[i] = 1;
			pcScale[i] = LIP_FT( 40.0, 4.0, i, PERDST );
		}

		h_sum += pcDepth[i];
	}

	return;
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