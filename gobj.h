#ifndef GOBJH
#define GOBJH

#include "cmn.h"

//geometry type
#define GT 3
//direction type
#define GD 2
//detail type
#define DT 1
//texture type
#define TT 0

//types of grid geometry (connect medium points of grid)
#define GT_PLAIN 0x00
#define GT_GRAD 0x01
#define GT_WALL 0x02
#define GT_LADD 0x03

//grid diretcion type
#define GD_NODIR 0x00
#define GD_N 0x01
#define GD_NE 0x02
#define GD_E 0x03
#define GD_SE 0x04
#define GD_S 0x05
#define GD_SW 0x06
#define GD_W 0x07
#define GD_NW 0x08

//terrain textures
#define TP_NONE 0x00
#define TP_HGRASS 0x01
#define TP_TSAND 0x02
#define TP_PLATE 0x03
#define TP_TSTONE 0x04
#define TP_MARBLE 0x05

//wall textures
#define TP_WBRICK 0x06
#define TP_WCONCR 0x07
#define TP_WSTONE 0x08
#define TP_WSAND 0x09
#define TP_WCLY 0x0A
#define TP_LIVE 0x0B

typedef struct
{
	int lcnt;
	unsigned char scl;

	unsigned char *lclr;
	unsigned char *lxf;
	unsigned char *lyf;
	unsigned char *lxt;
	unsigned char *lyt;
	unsigned char *lw;
}_tex;

_tex* tex_init(int lcnt, unsigned char scl )
{
	_tex* ret = NULL;
	MALSET(ret, sizeof(_tex));

	ret->lcnt = lcnt;
	ret->scl = scl;

	MALSET(ret->lclr, ret->lcnt );
	MALSET(ret->lxf, ret->lcnt );
	MALSET(ret->lyf, ret->lcnt );
	MALSET(ret->lxt, ret->lcnt );
	MALSET(ret->lyt, ret->lcnt );
	MALSET(ret->lw, ret->lcnt );

	return ret;
}
typedef unsigned char _uch;
void tex_sline( _tex* in, int lid, _uch xf, _uch yf, _uch xt, _uch yt, _uch cl, _uch lw )
{
	in->lclr[lid] = cl;
	in->lxf[lid] = xf;
	in->lyf[lid] = yf;
	in->lxt[lid] = xt;
	in->lyt[lid] = yt;
	in->lw[lid] = lw;
}

//void draw_tex_plvp(_imgw *pimg, _tex *tex, int sxf, int syf, int sw, int sh, float ds, int ry)


void draw_tex_plvp(_imgw *pimg, _tex *tex, int sxf, int syf, int sw, int sh, int ry )
{
	int i,j,k,l =0;
	int pxt,pyt,pxf,pyf,lw=1;


	for( k=0;k<ry;k++)
	{
		for( i=0;i<tex->lcnt;i++)
		{
			//(pc*tex->lyt[i])/tex->scl

			pxf = sxf + (sw*tex->lxf[i])/tex->scl;
			pxt = sxf + (sw*tex->lxt[i])/tex->scl;

			pyf = syf + (k*sh)/ry+ ((sh/ry)*tex->lyf[i])/tex->scl;
			pyt = syf + (k*sh)/ry+ ((sh/ry)*tex->lyt[i])/tex->scl;
			lw = (abs(sw)*tex->lw[i])/tex->scl;

			for(j=0;j<lw;j++) draw_line(pimg,pxf+j,pyf,pxt+j,pyt, gclr_res[tex->lclr[i]].i);
		}
	}
}

void draw_tex_plx( _imgw *pimg, _tex *tex, int sxf, int syf, int sw, int sh, int pc )
{
	int i,j =0;
	int pxt,pyt,pxf,pyf,lw;

	for( i=0;i<tex->lcnt;i++)
	{
		pxf = sxf + (sw*tex->lxf[i])/tex->scl + (pc*tex->lyf[i])/tex->scl;
		pxt = sxf + (sw*tex->lxt[i])/tex->scl + (pc*tex->lyt[i])/tex->scl;
		pyf = syf + (sw*tex->lyf[i])/tex->scl;
		pyt = syf + (sw*tex->lyt[i])/tex->scl;
		lw = (sw*tex->lw[i])/tex->scl;

		for(j=0;j<lw;j++) draw_line(pimg,pxf+j,pyf,pxt+j,pyt, gclr_res[tex->lclr[i]].i);
	}
}


#define OBT_VNO 0x00
#define OBT_VFERM 0x01
#define OBT_VPANEL 0x02
#define OBT_TREE 0x08 
#define OBT_CIRC 0x10



typedef struct
{
	float a;
	float r;
	float y;

	//store calculated values for further use
	int cdx;
	int cdy;
}_pnta;

#define DRST_INVIS 0 
#define DRST_PASSBY 1 
#define DRST_DRAWED 1 

typedef struct
{
	unsigned char type;
	int sx,sy,sh,sw;
	float h,w,a;

	_tex **texl;
	int texlc;

	_pnta* pntl;
	int pntlc;
}_stob;



typedef struct
{
	int sx,sy,sh,sw;
	float h,da;
	int dst;
	_clr gtype;

	//draw state
	unsigned char dr_st;
	//attached game_obj
	_stob *sob;
	unsigned char odr_st;

	//light force
	unsigned char linf;
	//light color
	_clr lclr;
}_tob;

typedef struct
{
	float dh;
	unsigned char dclr;

	//grid scale (size of 1 grid array element)
	float gscl;
	float gsx;
	float gsy;

	int w;
	int h;
	_tob **tobl;
	int toblc;
}_ter;


#endif