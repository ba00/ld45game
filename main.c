#include "cmn.h"
#include "gobj.h"
#include "SDL.h"
#include "Windows.h"

_cam *plcam = NULL;
_imgw* scrb = NULL;
_atm *catm = NULL;
_ter *cter = NULL;

int prev_x = W2;
float pl_speed = 5.0;
unsigned long int mtms = 0;
unsigned short int mtmsdiff = 0;

void draw_atm()
{
	int i,j,ic = 0;
	int hl = H2 - (plcam->b)*HAH;
	_clr cclr0, cclr1,cclrd;

	cclr0.i = catm->skygr_clr[0]->clr;
	cclr1.i = catm->skygr_clr[1]->clr;

	for( i=hl, ic=0; i >= 0; i--,ic++ )
	{
		cclrd.c[CR] = LIP_FT( cclr0.c[CR], cclr1.c[CR],ic,H);
		cclrd.c[CG] = LIP_FT( cclr0.c[CG], cclr1.c[CG],ic,H);
		cclrd.c[CB] = LIP_FT( cclr0.c[CB], cclr1.c[CB],ic,H);
		
		for( j=0; j<W; j++ ) SPIX(scrb,j,i,cclrd.i);
	}
}

void stob_draw(_tob *ob)
{
	int i = 0;

	ob->sob->sh = pcScale[ob->dst]*ob->sob->h;
	ob->sob->sw = pcScale[ob->dst]*ob->sob->w;
	ob->sob->sx = ob->sx + ob->sw/2;
	ob->sob->sy = ob->sy;

	for( i=0; i< ob->sob->pntlc; i++ )
	{
		//tbd - continue if point is not needed
		ob->sob->pntl[i].cdx = LIP( ob->sob->sw, ob->sob->pntl[i].r, ob->sob->w )*MCOS( ANGN360( ob->sob->pntl[i].a + ob->da ) );
		ob->sob->pntl[i].cdy = LIP( ob->sob->sh, ob->sob->pntl[i].y, ob->sob->h );
	}

	switch( ob->sob->type )
	{
		case OBT_VFERM:
			draw_tex_plvp(scrb,ob->sob->texl[0],ob->sob->sx + ob->sob->pntl[0].cdx,ob->sy,ob->sob->pntl[1].cdx - ob->sob->pntl[0].cdx,-ob->sob->sh,ob->sob->h);
			draw_tex_plvp(scrb,ob->sob->texl[0],ob->sob->sx + ob->sob->pntl[1].cdx,ob->sy,ob->sob->pntl[2].cdx - ob->sob->pntl[1].cdx,-ob->sob->sh,ob->sob->h);
			draw_tex_plvp(scrb,ob->sob->texl[0],ob->sob->sx + ob->sob->pntl[2].cdx,ob->sy,ob->sob->pntl[3].cdx - ob->sob->pntl[2].cdx,-ob->sob->sh,ob->sob->h);
			draw_tex_plvp(scrb,ob->sob->texl[0],ob->sob->sx + ob->sob->pntl[3].cdx,ob->sy,ob->sob->pntl[0].cdx - ob->sob->pntl[3].cdx,-ob->sob->sh,ob->sob->h);
			break;

		case OBT_VPANEL:
			draw_tex_plvp(scrb,ob->sob->texl[0],ob->sob->sx + ob->sob->pntl[0].cdx,ob->sy, ob->sob->pntl[1].cdx - ob->sob->pntl[0].cdx, -ob->sob->sh,1);
			break;

		case OBT_TREE:
			draw_tex_plvp(scrb,ob->sob->texl[0],ob->sob->sx - ob->sob->sw/2,ob->sy, ob->sob->sw,-ob->sob->sh,1);
			for( i=0; i < ob->sob->pntlc; i++ )
			{
				draw_tex_plvp(scrb,ob->sob->texl[1],ob->sob->sx, ob->sy - ob->sob->sh, ob->sob->pntl[i].cdx, ob->sob->pntl[i].cdy,1);
			}
			break;

		case OBT_CIRC:
			draw_tex_plvp(scrb,ob->sob->texl[0],ob->sob->sx - ob->sob->sw/2,ob->sy, ob->sob->sw,-ob->sob->sh,1);
			break;
	}
}

void ter_draw(_tob *ob )
{
	int i,j,k = 0;
	int kstetp = 0;
	unsigned char clr;

	if( ob->linf > 0 )
	{
		for( j =0; j <gclr_lc; j++)
		{
			gclr_res[j].c[CR] = LIP_FT( gclr_env[j].c[CR], ob->lclr.c[CR], ob->linf, 100);
			gclr_res[j].c[CG] = LIP_FT( gclr_env[j].c[CG], ob->lclr.c[CG], ob->linf, 100);
			gclr_res[j].c[CB] = LIP_FT( gclr_env[j].c[CB], ob->lclr.c[CB], ob->linf, 100);
		}
	}

	//some sw check, to be sure of normal size
	if( ob->gtype.c[GT] == GT_WALL )
	{
		if( ob->gtype.c[TT] == TP_WBRICK ) clr = TYBR;
		else if( ob->gtype.c[TT] == TP_WCONCR ) clr = TGRL;
		else if( ob->gtype.c[TT] == TP_WSTONE ) clr = TGRD;
		else if( ob->gtype.c[TT] == TP_WSAND ) clr = TYD;
		else if( ob->gtype.c[TT] == TP_WCLY ) clr = TYR;
		else if( ob->gtype.c[TT] == TP_LIVE ) clr = TGND;

		ob->sh = pcScale[ob->dst]*ob->h;

		for( i = 0; i < ob->sw; i++ )
		for( j = 0; j < ob->sh; j++ )
		{
			SPIX( scrb, ob->sx + i, ob->sy-j, gclr_res[clr]);
		}

	}

	if( ob->gtype.c[TT] == TP_HGRASS ) clr = TGNY;
	else if( ob->gtype.c[TT] == TP_PLATE ) clr = TGRDD;
	else if( ob->gtype.c[TT] == TP_TSAND ) clr = TY;
	else if( ob->gtype.c[TT] == TP_TSTONE ) clr = TGR;
	else if( ob->gtype.c[TT] == TP_MARBLE ) clr = TGRL;
	else if( ob->gtype.c[TT] == TP_NONE ) clr = cter->dclr;

	//wow, it is actually quite cool (but quite useless)
	//ob->sw = pcScale[ob->dst]*cter->gscl;
	
	for( i = 0; i < ob->sw; i++ )
	for( j = 0; j < ob->sh; j++ )
	{
		SPIX( scrb, ob->sx + i, ob->sy+j, gclr_res[clr]);
	}		
	ob->dr_st = DRST_INVIS;
	//point is to draw object once, and terrain - as needed
	if( NULL != ob->sob && ob->odr_st == DRST_INVIS )
	{
		stob_draw(ob);
		ob->odr_st = DRST_DRAWED;
	}

	if( ob->linf > 0 ) for( j =0; j <gclr_lc; j++) gclr_res[j].i = gclr_env[j].i;

}

typedef struct
{
	_clr lclr;
	int dst;
	int frs;

	int ch_lvl;
}_plight;

void dterr_bt()
{
	int i,j,k;
	int lidx, lidx_c = -1;
	int atm_f, atm_fc = -1;
	int sx, sy;
	float dl, da, lx, ly, dlx, dly, olx, oly;

	//maybe no need
	for( i =0; i < cter->toblc; i++ )
	{
		cter->tobl[i]->dr_st = DRST_INVIS;
		cter->tobl[i]->odr_st = DRST_INVIS;
	}
	sy = H2 - plcam->b*HAH;


	for( i=PERDST-1; i >=0 ; i-- )
	{
		
		dl = plcam->a - AW2;
		ANGWR036(dl);
		sy += pcDepth[i];
		atm_f = ( i > catm->env_fmax_dst )? catm->env_fmax:LIP_FT(catm->env_fmin,catm->env_fmax,i,catm->env_fmax_dst);

		if( atm_f != atm_fc )
		{
			for( j =0; j <gclr_lc; j++)
			{
				gclr_res[j].c[CR] = gclr_env[j].c[CR] = LIP_FT( gclr_base[j].c[CR], catm->env_clr.c[CR], atm_f, 100);
				gclr_res[j].c[CG] = gclr_env[j].c[CG] = LIP_FT( gclr_base[j].c[CG], catm->env_clr.c[CG], atm_f, 100);
				gclr_res[j].c[CB] = gclr_env[j].c[CB] = LIP_FT( gclr_base[j].c[CB], catm->env_clr.c[CB], atm_f, 100);
			}
			atm_fc = atm_f;
		}

		for( j=0; j<W; j++ )
		{
			dl += 0.125;	
			if( dl >=360 ) dl -=360;

			lx = ((plcam->x - i*MCOS(dl) - cter->gsx)/cter->gscl);
			ly = ((plcam->y - i*MSIN(dl) - cter->gsy)/cter->gscl);
			

			if( lx >= 0 && lx < cter->w && ly >=0 && ly < cter->h )
			{
				lidx = (int)lx + ((int)ly)*cter->w;
				
				if( lidx_c != lidx && lidx_c > 0)
				{
					ter_draw( cter->tobl[lidx_c] );
				}

				if( cter->tobl[lidx]->dr_st == DRST_PASSBY ) cter->tobl[lidx]->sw++; //DRST_PASSBY
				else
				{
					cter->tobl[lidx]->sx = j;
					cter->tobl[lidx]->sy = sy;
					cter->tobl[lidx]->sw = 1;
					cter->tobl[lidx]->dst = i;
					cter->tobl[lidx]->sh = pcDepth[i-1];
					cter->tobl[lidx]->da = dl;
					cter->tobl[lidx]->dr_st = DRST_PASSBY; 
				}

				lidx_c = lidx;
			}
			else for( k =0; k<pcDepth[i]; k++ ) SPIX( scrb, j, sy-k, gclr_env[cter->dclr]);
		}
	}

}


float ter_gh( float x, float y)
{
	if( (x < cter->gsx) || (x > cter->gsx + cter->w*cter->gscl) || 
		(y < cter->gsy) || (y > cter->gsy + cter->h*cter->gscl) )
	{
		return cter->dh + 1.5;
	}
	else
	{
		int cidx =  (int)((y - cter->gsy)/cter->gscl)*cter->w + (int)((x - cter->gsx)/cter->gscl);
		return cter->tobl[cidx]->h;
	}

}

#define MVIMPOS 0
#define MVOK 1 
#define MVOBJ 2

int mv_isok( float nx, float ny )
{
	if( (nx < cter->gsx) || (nx > cter->gsx + cter->w*cter->gscl) || 
		(ny < cter->gsy) || (ny > cter->gsy + cter->h*cter->gscl) )
	{
		return MVOK;
	}
	else
	{
		int cidx =  (int)((ny - cter->gsy)/cter->gscl)*cter->w + (int)((nx - cter->gsx)/cter->gscl);

		if( cter->tobl[cidx]->gtype.c[GT] == GT_WALL ) return MVIMPOS;
		else return MVOK;
	}
}

void inputproc()
{
	SDL_Event evt;

	while(SDL_PollEvent(&evt))
	{
		if( evt.type == SDL_QUIT )
		{
			SDL_Quit();
			exit(0); 
		}
		else if( evt.type == SDL_MOUSEMOTION )
		{
			plcam->b = (evt.button.y - H2)/12; 
	
			plcam->a = ang_norm( plcam->a + (evt.button.x - prev_x) ); 		

			if( evt.button.x > W-10 )
			{
				SDL_WarpMouse( 10 ,evt.button.y);
				prev_x = 10;
			}
			else if( evt.button.x < 10 ) 
			{
				SDL_WarpMouse(W-10,evt.button.y);
				prev_x = W-10;
			}
			else prev_x = evt.button.x;
		}
		else if( evt.type == SDL_MOUSEBUTTONDOWN) //SDL_MOUSEBUTTONUP
		{
			if( evt.button.button == SDL_BUTTON_LEFT )
			{

			}
		}
		else if( evt.type == SDL_KEYDOWN )
		{
			if( evt.key.keysym.sym == SDLK_w ) pmvs |= Wprs;
			if( evt.key.keysym.sym == SDLK_s ) pmvs |= Sprs;
			if( evt.key.keysym.sym == SDLK_a ) pmvs |= Aprs;
			if( evt.key.keysym.sym == SDLK_d ) pmvs |= Dprs;
		}
		else if( evt.type == SDL_KEYUP )
		{
			if( evt.key.keysym.sym == SDLK_w ) pmvs &= ~Wprs;
			if( evt.key.keysym.sym == SDLK_s ) pmvs &= ~Sprs;
			if( evt.key.keysym.sym == SDLK_a ) pmvs &= ~Aprs;
			if( evt.key.keysym.sym == SDLK_d ) pmvs &= ~Dprs;
		}
	}
}


void pl_move()
{
	int ca = (int) plcam->a;
	float dx, dy;
	
	if( pmvs&Wprs )
	{
		ANGWR036( ca );
		dx = -MCOS( ca );
		dy = -MSIN( ca );
	}
	else if( pmvs&Sprs  )
	{
		ANGWR036( ca );
		dx = MCOS( ca );
		dy = MSIN( ca );
	}

	if( pmvs&Aprs )
	{
		ca = (int)plcam->a -90;
		ANGWR036( ca );
		dx = -MCOS( ca );
		dy = -MSIN( ca );
	}
	else if( pmvs&Dprs  )
	{
		ca = (int)plcam->a -90;
		ANGWR036( ca );
		dx = MCOS( ca );
		dy = MSIN( ca );
	}
	
	if( mtmsdiff > 0 )
	{
		dx = (dx*pl_speed)/(1000/mtmsdiff);
		dy = (dy*pl_speed)/(1000/mtmsdiff);

		if( MVOK == mv_isok( plcam->x +dx, plcam->y +dy ))
		{
			plcam->x += dx;
			plcam->y += dy;
			plcam->z = ter_gh( plcam->x, plcam->y);
		} 
	}
}


_tex* wep_tex1 = NULL;
_tex* ferm_tex1 = NULL;
_tex* wall_tex1 = NULL;
_tex* barrel_tex1 = NULL;
_tex* branch_tex1 = NULL;


void ter_setl( int x, int y, unsigned char gt, unsigned char tt, float h )
{
	int idx =(int)((y - cter->gsy)/cter->gscl)*cter->w + (int)((x - cter->gsx)/cter->gscl);

	cter->tobl[idx]->gtype.c[GT] = gt;
	cter->tobl[idx]->gtype.c[TT] = tt;
	cter->tobl[idx]->h = h;
}

void ter_adl_spot( int x, int y, int r, int lclr, int lvl_max )
{
	int i,j, jm = 45;
	int idx =(int)((y - cter->gsy)/cter->gscl)*cter->w + (int)((x - cter->gsx)/cter->gscl);


	cter->tobl[idx]->lclr.i = lclr;
	cter->tobl[idx]->linf = lvl_max;

	for(i=1;i<=r;i++)
	{
		for(j=0;j<360;j+=jm)
		{
			idx =(int)((y + i*MSIN(j) - cter->gsy)/cter->gscl)*cter->w + (int)((x + i*MCOS(j) - cter->gsx)/cter->gscl);
			cter->tobl[idx]->lclr.i = lclr;
			cter->tobl[idx]->linf = LIP(lvl_max,r-i,r);
		}
		jm = (jm>1)?jm/2:1;
	}


}

_stob* stob_init_ferm( float w, float h, float a, _tex* tex )
{
	_stob* ret = NULL;

	MALSET( ret, sizeof(_stob) );

	ret->type = OBT_VFERM;
	ret->w = w;
	ret->h = h;
	ret->a = a;

	ret->texlc = 1;
	MALSET( ret->texl, sizeof(_tex*)*ret->texlc );
	ret->texl[0] = tex;

	ret->pntlc = 4;
	MALSET( ret->pntl, sizeof(_pnta)*ret->pntlc );
	ret->pntl[0].a = ANGN360(ret->a); 
	ret->pntl[1].a = ANGN360(ret->a+90); 
	ret->pntl[2].a = ANGN360(ret->a+180); 
	ret->pntl[3].a = ANGN360(ret->a+270);
	ret->pntl[0].r = ret->pntl[1].r = ret->pntl[2].r = ret->pntl[3].r = 0.5*w;

	return ret;
}

_stob* stob_init_tree( float w, float h, float a, float brny, int brnc, _tex* tex_barr, _tex* tex_br )
{
	_stob* ret = NULL;
	int i=0;

	MALSET( ret, sizeof(_stob) );

	ret->type = OBT_TREE;
	ret->w = w;
	ret->h = h;
	ret->a = a;

	ret->texlc = 2;
	MALSET( ret->texl, sizeof(_tex*)*ret->texlc );
	ret->texl[0] = tex_barr;
	ret->texl[1] = tex_br;

	ret->pntlc = brnc;
	MALSET( ret->pntl, sizeof(_pnta)*ret->pntlc );

	for( i =0; i< ret->pntlc; i++ )
	{
		ret->pntl[i].a = ANGN360(ret->a + i*(360/brnc)); 
		ret->pntl[i].r = 0.5*w;
		ret->pntl[i].y = brny;
	}
	return ret;
}

_stob* stob_init_panel( float w, float h, float a, _tex* tex )
{
	_stob* ret = NULL;

	MALSET( ret, sizeof(_stob) );

	ret->type = OBT_VPANEL;
	ret->w = w;
	ret->h = h;
	ret->a = a;

	ret->texlc = 1;
	MALSET( ret->texl, sizeof(_tex*)*ret->texlc );
	ret->texl[0] = tex;

	ret->pntlc = 2;
	MALSET( ret->pntl, sizeof(_pnta)*ret->pntlc );
	ret->pntl[0].a = ANGN360(ret->a); 
	ret->pntl[1].a = ANGN360(ret->a+180); 
	ret->pntl[0].r = ret->pntl[1].r = 0.5*w;

	return ret;
}

void ter_addobj( int x, int y, _stob* inobj )
{
	int idx = (int)((y - cter->gsy)/cter->gscl)*cter->w + (int)((x - cter->gsx)/cter->gscl);

	cter->tobl[idx]->sob = inobj;
}


_stob* pferm_s = NULL;

void lg2( int sx, int sy, int l, int scl, int cx, int cy )
{
	int i,j,k,m;
	int lmin, lmax;
	int mi, mj;
	unsigned char ctex;
	float ch;

	//set floor
	for(i=0;i<l+4;i++)
	for(j=0;j<l+4;j++)
	for( m=1; m<scl-1; m++ )
	for( k=1; k<scl-1; k++ )
	{
		ctex = TP_MARBLE;
		if( m == 1 || k == 1 || k == scl -2 || m == scl-2) ctex = TP_PLATE;
		else if ( m == scl/2 || k == scl/2 ) ctex = TP_TSTONE;

		ter_setl( sx + cx*(i*scl+k), sy + cy*(scl*j+m),GT_PLAIN, ctex, 0);	
	}


	//outer border
	for( i=0; i<l+4; i++ )
	for( k=0; k<scl; k++ )
	{
		ter_setl( sx + cx*(i*scl+k), sy ,GT_WALL, TP_LIVE, 0.8);	
		ter_setl( sx + cx*(i*scl+k), sy + cy*(scl*(l+4)),GT_WALL, TP_LIVE, 0.8);

		ter_setl( sx , sy + cy*(i*scl+k),GT_WALL, TP_LIVE, 0.8);	
		ter_setl( sx + cx*(scl*(l+4)), sy + cy*(i*scl+k) ,GT_WALL, TP_LIVE, 0.8);
	}

	//outer gates
	//ter_setl( sx + cx*(i*scl+k), sy + cy*(scl*j+m),GT_PLAIN, ctex, 0);	
	for( k=0; k<scl; k++ )
	{
		ter_setl( sx + cx*(k), sy ,GT_PLAIN, TP_HGRASS, 0);	
		ter_setl( sx , sy + cy*(k),GT_PLAIN, TP_HGRASS, 0);	
	}

	ter_setl( sx + cx*scl, sy ,GT_WALL, TP_WSTONE, 5);	
	ter_setl( sx , sy + cy*scl,GT_WALL, TP_WSTONE, 5);	

	//inner sanctum
	for( i=(l+4)/2-1; i<(l+4)/2+1; i++ )
	for( k=0; k<scl; k++ )
	{
		int rside = rand()%4;

		if(rside !=0) ter_setl( sx + cx*(i*scl+k), sy + cy*(scl*((l+4)/2-1)),GT_WALL, TP_WCONCR, 1.5);	
		if(rside !=1)ter_setl( sx + cx*(i*scl+k), sy + cy*(scl*((l+4)/2+1)),GT_WALL, TP_WCONCR, 1.5);

		if(rside !=2)ter_setl( sx + cx*(scl*((l+4)/2-1)), sy + cy*(i*scl+k),GT_WALL, TP_WCONCR, 1.5);	
		if(rside !=3)ter_setl( sx + cx*(scl*((l+4)/2+1)), sy + cy*(i*scl+k) ,GT_WALL, TP_WCONCR, 1.5);
	}

	if( cx < 0 && cy < 0) ter_adl_spot( sx +cx*scl*(l+4)/2, sy+cy*scl*(l+4)/2, 40,CLR_BRED, 60);
	if( cx < 0 && cy > 0) ter_adl_spot( sx +cx*scl*(l+4)/2, sy+cy*scl*(l+4)/2, 30,CLR_FUCIA, 70);
	if( cx > 0 && cy > 0) ter_adl_spot( sx +cx*scl*(l+4)/2, sy+cy*scl*(l+4)/2, 30,CLR_BLUE, 70);
	if( cx > 0 && cy < 0) ter_adl_spot( sx +cx*scl*(l+4)/2, sy+cy*scl*(l+4)/2, 40,CLR_GREEN, 70);

	//draw squares
	for( j = 1; j <(l+4)/2-1;j++)
	for( i=j; i<(l+4)-j; i++ )
	for( k=0; k<scl; k++ )
	{
		ch = 1+j*0.5;
		ctex = TP_WSTONE;

		if( k==0 && ((i==j) || i == (l+4)-j -1) )
		{
			ch = 3+j*0.5;
			ctex = TP_WCONCR;
		}

		ter_setl( sx + cx*(i*scl+k), sy + cy*(scl*j),GT_WALL, ctex , ch);	
		ter_setl( sx + cx*(i*scl+k), sy + cy*(scl*((l+4)-j)),GT_WALL, ctex, ch);

		ter_setl( sx + cx*(scl*j), sy + cy*(i*scl+k),GT_WALL, ctex, ch);	
		ter_setl( sx + cx*(scl*((l+4) -j)), sy + cy*(i*scl+k) ,GT_WALL, ctex, ch);
	}

	//set holes
	for( j = 1; j <(l+4)/2-1;j++)
	{
		lmin = 1;
		lmax = l-1;

		for( i=0; i<3; i++ )
		{
			int rside = rand()%4;
			int ridx = lmin + rand()%lmax;
			int ctype = GT_PLAIN;
			

			for( k=0; k<scl; k++ )
			{
				if( k == 0 || k ==scl-1)
				{
					ctex = TP_WCONCR;
					ctype = GT_WALL;
				}
				else
				{
					ctex = TP_PLATE;
					ctype = GT_PLAIN;
				}
				if(rside == 0 ) ter_setl( sx + cx*(ridx*scl+k), sy + cy*(scl*j), ctype, ctex, 5);	
				if(rside == 1 ) ter_setl( sx + cx*(ridx*scl+k), sy + cy*(scl*((l+4)-j)),ctype, ctex, 5);
				if(rside == 2 ) ter_setl( sx + cx*(scl*j), sy + cy*(ridx*scl+k), ctype, ctex, 5);
				if(rside == 3 ) ter_setl( sx + cx*(scl*((l+4) -j)), sy + cy*(ridx*scl+k) ,ctype, ctex, 5);
			}
		}

		lmin++;
		lmax-=2;
	}
}

void lgen()
{
	int i,j,k,l=0;
	_stob* ptree_s = stob_init_tree(3.0,4,0,3,6,barrel_tex1, branch_tex1 ); 
	_stob* ppnl = stob_init_panel(2,4,90,ferm_tex1);

	pferm_s = stob_init_ferm(0.5,10.0, 0, ferm_tex1 );

	MALSET( cter, sizeof(_ter) );

	cter->gsx = 0;
	cter->gsy = 0;
	cter->gscl = 1;
	cter->w = 300;
	cter->h = 300;
	cter->dclr = TGRL;
	cter->toblc = cter->w*cter->h;

	MALSET( cter->tobl, sizeof(_stob*)*cter->toblc);


	for( i=0; i<cter->toblc;i++)
	{
		MALSET( cter->tobl[i], sizeof(_tob));
	}

	//first of all - start game room, (start with floor,)

	for( i=0; i<360; i++ )
	{

		//inner circle
		ter_setl(150 + 5*MCOS(i), 150 + 6*MSIN(i),GT_PLAIN, TP_HGRASS, 0);
		ter_setl(150 + 6*MCOS(i), 150 + 5*MSIN(i),GT_PLAIN, TP_HGRASS, 0);
	

		if( i%90 == 0)
		{
			for( j=-30;j<30;j++)
			{
				ter_setl(150 + 20*MCOS(ANGN360(i+j)), 150 + 20*MSIN(ANGN360(i+j)),GT_WALL, TP_LIVE, 1);
				ter_setl(150 + 21*MCOS(ANGN360(i+j)), 150 + 21*MSIN(ANGN360(i+j)),GT_WALL, TP_LIVE, 1);

				for( k =0; k<10; k++ )
				{
					l = 25 + rand()%124;
					ter_setl(150 + l*MCOS(ANGN360(i+j)), 150 + l*MSIN(ANGN360(i+j)),GT_WALL, TP_LIVE, 1);
				}

			}
		}


		if( i%40 == 0 )
		{
			ter_addobj( 150 + 10*MCOS(i), 150 + 15*MSIN(i), ptree_s );
			ter_addobj( 150 + 17*MCOS(i+3), 150 + 8*MSIN(i+3), ptree_s );

		}




		ter_setl(150 + 148*MCOS(i+0.5), 150 + 148*MSIN(i+0.5),GT_WALL, TP_LIVE, 7+rand()%2);
		ter_setl(150 + 145*MCOS(i+0.25), 150 + 145*MSIN(i+0.25),GT_WALL, TP_LIVE, 6+rand()%1);
		ter_setl(150 + 146*MCOS(i), 150 + 146*MSIN(i),GT_WALL, TP_LIVE, 5+rand()%2);

	}
	
	//ter_addobj( 150 + 30*MCOS(i), 150 + 30*MSIN(i), pferm_s );
	//ter_addobj( 105, 105, ppnl );
	//ter_addobj( 118, 98, ptree_s );
	//ter_addobj( 118, 102, ptree_s );
	ter_adl_spot( 150,150,20,CLR_BLUE,70);


	lg2( 140, 140, 8, 7, -1, -1 );	
	lg2( 160, 140, 12, 7, 1, -1 );
	lg2( 140, 160, 6, 7, -1, 1 );
	lg2( 160, 160, 8, 7, 1, 1 );

}

void tdm_init()
{
	scrb = imgw_init(W,H);

	MALSET(plcam, sizeof(_cam));

	plcam->x = 150;
	plcam->y = 150;
	plcam->z = 1.5;
	plcam->a = 180.0;
	plcam->b = 0.0;

	wep_tex1 = tex_init(4,32);
	tex_sline(wep_tex1,0,2,0,2,32,4,8);
	tex_sline(wep_tex1,1,22,0,22,32,4,8);
	tex_sline(wep_tex1,2,10,0,10,4,4,12);
	tex_sline(wep_tex1,3,10,12,10,16,4,12);

	ferm_tex1 = tex_init(5,32);
	tex_sline(ferm_tex1,0,0,0,0,32,TGRDD,4);
	tex_sline(ferm_tex1,1,32,0,32,32,TGRDD,4);
	tex_sline(ferm_tex1,2,0,0,32,0,TGRDD,4);
	tex_sline(ferm_tex1,3,0,0,32,32,TGRDD,4);
	tex_sline(ferm_tex1,4,32,0,0,32,TGRDD,4);

	wall_tex1 = tex_init(2,32);
	tex_sline(wall_tex1,1,0,0,0,24,TGR,32);
	tex_sline(wall_tex1,2,0,24,0,32,TGRL,32);

	barrel_tex1 = tex_init(1,32);
	tex_sline(barrel_tex1,0,15,0,15,32,TGRBR,2);

	branch_tex1 = tex_init(1,32);
	tex_sline(branch_tex1,0,24,8,0,24,TGND,2);


	MALSET(catm,sizeof(_atm));

	//it must be sunset
	catm->gll_a = 0;
	catm->gll_b = 65;
	catm->gll_clr.i = CLR_BRED;
	catm->gll_contrast = 10; 
	catm->gll_force = 25;

	catm->env_clr.i = CLR_DBLUE;
	catm->env_fmin = 10;
	catm->env_fmax = 80;
	catm->env_fmax_dst = 60;

	catm->skygr_clr = LADD( catm->skygr_clr, malloc(sizeof(_clgrd)),&catm->skygr_clrc );
	catm->skygr_clr[0]->clr = CLR_FUCIA;
	catm->skygr_clr[0]->proc = 0;

	catm->skygr_clr = LADD( catm->skygr_clr, malloc(sizeof(_clgrd)),&catm->skygr_clrc );
	catm->skygr_clr[1]->clr = CLR_BLUE;
	catm->skygr_clr[1]->proc = 100;

	

}


int main(int argc, char *argv[] )
{
	SDL_Surface *scr = NULL;
	DWORD t0;

	int i,j=0;
	int tidx = 0;
	int sflag = 0;

	if( 0 < SDL_Init( SDL_INIT_VIDEO ) ) return 0;

	scr = SDL_SetVideoMode(W,H,32, SDL_SWSURFACE );
	SDL_WM_SetCaption("SDL TEST", NULL );
	SDL_ShowCursor(SDL_DISABLE);

	clr_init();
	c_precalc();
	tdm_init();
	lgen();
	SDL_WarpMouse( W2, H2 );

	mtms = GetTickCount();

	while(1)
	{
		SDL_Delay(20);
		mtmsdiff = (GetTickCount() - mtms);


		inputproc();	
		if( pmvs != 0 ) pl_move();

		memset( scrb->data, 0xFFFFFFF, W*H*4);
		
		draw_atm();
		dterr_bt();

		//draw_tex_plx(scrb,wep_tex1,W2,H-H/4,H/4,H/4,W/4);

		memcpy( scr->pixels, scrb->data, W*H*4 );
		SDL_Flip(scr);
		

		catm->gll_a++;
		if( catm->gll_a > 180 ) catm->gll_a = -180;
		mtms += mtmsdiff;
	}

	return 0;
}