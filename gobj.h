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
#define TP_BOULDER 0x02
#define TP_PLATE 0x03
#define TP_MOSAIC1 0x04

//texture detail levels
//full detail - for near objects
#define TL_FULLD 1
//half detail - for most object in view
#define TL_HALFD 2
//no detail - color (or form) only
#define TL_NOD 3

typedef struct
{
	float h;
	unsigned int ttype;

	//grid scale (size of 1 grid array element)
	float gscl;
	float gsx;
	float gsy;
	
	//grid type array
	_imgw *gtype;
	//grid height array
	_imgw *gh;
}_ter;


#endif