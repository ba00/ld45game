#include "cmn.h"
#include "gobj.h"
#include "SDL.h"


_cam *plcam = NULL;
_imgw* scrb = NULL;
_atm *catm = NULL;
_ter *cter = NULL;
short *terbmask = NULL;
int prev_x = W2;
float pl_speed = 1.0;

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

void dterr_bt()
{
	int i,j,k,kmax = 0;
	int lidx;
	int atm_f, atm_fc = -1;
	int sx, sy, sh, sw;
	float dl, da, lx, ly;
	unsigned char hpx, hpy, hs, tc, tl, tr,dw;
	_clr lclr;

	for( j=0; j<W; j++ ) terbmask[j] = 1.10*H; 

	sy = 1.5*H - plcam->b*HAH;

	for( i=0; i < PERDST; i++ )
	{
		dl = plcam->a - AW2;
		da = plcam->a;

		sy -= pcDepth[i];

		if( i < 10 ) hs = TL_FULLD;	//10
		else if( i < 60 ) hs = TL_HALFD; //60
		else hs = TL_NOD;

		ANGWR036(dl);
		ANGWR036(da);

		atm_f = ( i > catm->env_fmax_dst )? catm->env_fmax:LIP_FT(catm->env_fmin,catm->env_fmax,i,catm->env_fmax_dst);

		if( atm_f != atm_fc )
		{
			for( j =0; j <gclr_lc; j++)
			{
				gclr_env[j].c[CR] = LIP_FT( gclr_base[j].c[CR], catm->env_clr.c[CR], atm_f, 100);
				gclr_env[j].c[CG] = LIP_FT( gclr_base[j].c[CG], catm->env_clr.c[CG], atm_f, 100);
				gclr_env[j].c[CB] = LIP_FT( gclr_base[j].c[CB], catm->env_clr.c[CB], atm_f, 100);
			}
			atm_fc = atm_f;
		}
		for( j=0; j<W; j++ )
		{
			dl += 0.125;	
			if( dl >=360 ) dl -=360;

			da = dl;
			dw = 0;

			lx = ((plcam->x - i*MCOS(dl) - cter->gsx)/cter->gscl);
			ly = ((plcam->y - i*MSIN(dl) - cter->gsy)/cter->gscl);

			if( lx >= 0 && lx < cter->gtype->w && ly >=0 && ly < cter->gtype->h )
			{
				lidx = (int)lx + ((int)ly)*cter->gtype->w;

				if( terbmask[j] < sy ) continue;

				hpx = (unsigned char)( (lx - (int)lx)*256);	
				hpy = (unsigned char)( (ly - (int)ly)*256);


				if( ((_clr*)cter->gtype->data)[lidx].c[GT] == GT_WALL )
				{
					dw = 1;
					tl = 0; 					
					tr = 255;

					lclr.i = gclr_env[4].i;

					switch( ((_clr*)cter->gtype->data)[lidx].c[GD] )
					{
						case GD_N:
							//270
							if( da <= 270 && da > 180 ) tl = LIP( (270-da), 255, 90 );
							else if( da > 270 ) tr = LIP( (360-da), 255, 90 );
							else dw = 0;
							if( hpy < tl || hpy > tr ) dw = 0;
							break;

						case GD_NE:
							//315
							if( da <= 315 && da > 225 ) tl = LIP( (315-da), 255, 90 );
							else if( da > 315 ) tr = LIP( (405-da), 255, 90 );
							else if( da < 45 ) tr = LIP( (45 - da), 255, 90 );
							else dw = 0;
							//if( ( hpy < tl && hpx < tl ) || ( hpx >tr && hpy > tr ) ) continue;
							break;

						case GD_E:
							//0
							if( da > 270 ) tl = LIP( (360-da), 255, 90 );
							else if( da >= 0 && da < 90 ) tr = LIP( (90-da), 255, 90 );
							else dw = 0;
							if( hpx < tl || hpx > tr ) dw = 0;
							break;

						case GD_SE:
							//45
							if( da <= 45 ) tl = LIP( (45-da), 255, 90 );
							else if( da > 315 ) tl = LIP( (405-da), 255, 90 );
							else if( da > 45 && da < 135 ) tr = LIP( (135-da), 255, 90 );
							else dw = 0;
							//if( ( hpy < tl && hpx < tl ) || ( hpx >tr && hpy > tr ) ) continue;
							break;

						case GD_S:
							//90
							if( da <= 90 && da > 0 ) tl = LIP( (90-da), 255, 90 );
							else if( da > 90 && da < 180 ) tr = LIP( (180-da), 255, 90 );
							else dw = 0;
							if( hpy < tl || hpy > tr ) dw = 0;

							break;

						case GD_SW:
							//135
							if( da <= 135 && da > 45 ) tl = LIP( (135-da), 255, 90 );
							else if( da > 135 && da < 225 ) tr = LIP( (225-da), 255, 90 );
							else dw = 0;
							//if( ( hpy < tl && hpx < tl ) || ( hpx >tr && hpy > tr ) ) continue;
							break;

						case GD_W:
							//180
							if( da <= 180 && da > 90 ) tl = LIP( (180-da), 255, 90 );
							else if( da > 180 && da < 270 ) tr = LIP( (270-da), 255, 90 );
							else dw = 0;
							if( hpx < tl || hpx > tr ) dw = 0;
							break;

						case GD_NW:
							//225
							if( da <= 225 && da > 135 ) tl = LIP( (225-da), 255, 90 );
							else if( da > 225 && da < 315 ) tr = LIP( (315-da), 255, 90 );
							else dw = 0;							
							//if( ( hpy < tl && hpx < tl ) || ( hpx >tr && hpy > tr ) ) continue;
							break;
					}
					
					//if( tc < tl + 8 || tc >= tr- 8 ) lclr.i = CLR_GRAY;
					//else lclr.i = CLR_LGRAY;
				}
				if( dw == 0) kmax = pcDepth[i+1];
				else kmax = pcScale[i+1]*((float*)cter->gh->data)[lidx];

				if( dw == 0 )
				{
					switch(hs)
					{
						case TL_NOD:
							if( ((_clr*)cter->gtype->data)[lidx].c[TT] == TP_PLATE ) lclr.i = gclr_env[3].i;
							if( ((_clr*)cter->gtype->data)[lidx].c[TT] == TP_HGRASS ) lclr.i = gclr_env[1].i;
							break;

						case TL_HALFD:
							if( ((_clr*)cter->gtype->data)[lidx].c[TT] == TP_PLATE ) lclr.i = gclr_env[3].i;
							if( ((_clr*)cter->gtype->data)[lidx].c[TT] == TP_HGRASS ) lclr.i = (hpx%4 == 0)?gclr_env[1].i:gclr_env[0].i;
							break;

						case TL_FULLD:
							if( ((_clr*)cter->gtype->data)[lidx].c[TT] == TP_PLATE ) lclr.i = gclr_env[3].i;
							if( ((_clr*)cter->gtype->data)[lidx].c[TT] == TP_HGRASS ) lclr.i = (hpx%2 == 0)?gclr_env[1].i:gclr_env[0].i;
							break;
					}
				}
				for( k = 0; k<kmax; k++)
				{
					SPIX( scrb, j, sy - k, lclr);
				}
				terbmask[j] = sy - kmax; 

			}
		}
	}
	

}


float ter_gh( float x, float y)
{
	if( (x < cter->gsx) || (x > cter->gsx + cter->gtype->w*cter->gscl) || 
		(y < cter->gsy) || (y > cter->gsy + cter->gtype->h*cter->gscl) )
	{
		return cter->h + 1.5;
	}
	else
	{
		int cidx =  (int)((y - cter->gsy)/cter->gscl)*cter->gtype->w + (int)((x - cter->gsx)/cter->gscl);
		float grdx = x - (cter->gsx + (cidx%cter->gtype->w)*cter->gscl);	
		float grdy = y - (cter->gsy + (cidx/cter->gtype->w)*cter->gscl);	

		if( ((_clr*)cter->gtype->data)[cidx].c[GT] == GT_PLAIN ) return ((float*)cter->gh->data)[cidx];
		else if( ((_clr*)cter->gtype->data)[cidx].c[GT] == GT_GRAD )
		{
			if( ((_clr*)cter->gtype->data)[cidx].c[GD] == GD_N ) return LIP_FT( ((float*)cter->gh->data-cter->gtype->w)[cidx],((float*)cter->gh->data)[cidx], grdy, cter->gscl ) + 1.5;  
			if( ((_clr*)cter->gtype->data)[cidx].c[GD] == GD_S ) return LIP_FT( ((float*)cter->gh->data)[cidx],((float*)cter->gh->data)[cidx+cter->gtype->w], grdy, cter->gscl ) + 1.5;  
		}
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
	
	if( pmvs&Wprs )
	{
		ANGWR036( ca );
		plcam->x -= MCOS( ca );
		plcam->y -= MSIN( ca );
	}
	else if( pmvs&Sprs  )
	{
		ANGWR036( ca );
		plcam->x += MCOS( ca );
		plcam->y += MSIN( ca );
	}

	if( pmvs&Aprs )
	{
		ca = (int)plcam->a -90;
		ANGWR036( ca );
		plcam->x -= MCOS( ca );
		plcam->y -= MSIN( ca );
	}
	else if( pmvs&Dprs  )
	{
		ca = (int)plcam->a -90;
		ANGWR036( ca );
		plcam->x += MCOS( ca );
		plcam->y += MSIN( ca );
	}


	plcam->z = ter_gh( plcam->x, plcam->y); 
}


void tdm_init()
{
	int i,j,k=0;
	float h = 0.0;
	int oang = 0;
	scrb = imgw_init(W,H);

	MALSET(terbmask, W*sizeof(short));

	MALSET(plcam, sizeof(_cam));

	plcam->x = 0.0;
	plcam->y = 0.0;
	plcam->z = 1.5;
	plcam->a = 0.0;
	plcam->b = 0.0;

	MALSET( cter, sizeof(_ter) );

	cter->gsx = -600;
	cter->gsy = -600;
	cter->gscl = 4;
	cter->gtype = imgw_init(400,400);
	cter->gh = imgw_init(400,400);

	for( i=0; i<400*400;i++)
	{
		if( i%2 == 0 )((_clr*)cter->gtype->data)[i].c[TT] = TP_HGRASS;
		if( i%2 == 1 )((_clr*)cter->gtype->data)[i].c[TT] = TP_PLATE;
	}

	for( k = 0; k< 1000; k++)
	{
		i = 10 + rand()%380;
		j = 10 + rand()%380;
		h = 2.0 + rand()%8;
	
		((_clr*)cter->gtype->data)[(i-1)*cter->gtype->w + j-1].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i-1)*cter->gtype->w + j-1].c[GD] = GD_NW;
		((float*)cter->gh->data)[(i-1)*cter->gh->w + j-1] = h;

		((_clr*)cter->gtype->data)[(i-1)*cter->gtype->w + j].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i-1)*cter->gtype->w + j].c[GD] = GD_N;
		((float*)cter->gh->data)[(i-1)*cter->gh->w + j] = h;
		
		((_clr*)cter->gtype->data)[(i-1)*cter->gtype->w + j+1].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i-1)*cter->gtype->w + j+1].c[GD] = GD_NE;
		((float*)cter->gh->data)[(i-1)*cter->gh->w + j+1] = h;

		((_clr*)cter->gtype->data)[(i)*cter->gtype->w + j+1].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i)*cter->gtype->w + j+1].c[GD] = GD_E;
		((float*)cter->gh->data)[(i)*cter->gh->w + j+1] = h;

		
		((_clr*)cter->gtype->data)[(i+1)*cter->gtype->w + j+1].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i+1)*cter->gtype->w + j+1].c[GD] = GD_SE;
		((float*)cter->gh->data)[(i+1)*cter->gh->w + j+1] = h;
		
		((_clr*)cter->gtype->data)[(i+1)*cter->gtype->w + j].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i+1)*cter->gtype->w + j].c[GD] = GD_S;
		((float*)cter->gh->data)[(i+1)*cter->gh->w + j] = h;

		((_clr*)cter->gtype->data)[(i+1)*cter->gtype->w + j-1].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i+1)*cter->gtype->w + j-1].c[GD] = GD_SW;
		((float*)cter->gh->data)[(i+1)*cter->gh->w + j-1] = h;

		((_clr*)cter->gtype->data)[(i)*cter->gtype->w + j-1].c[GT] = GT_WALL;
		((_clr*)cter->gtype->data)[(i)*cter->gtype->w + j-1].c[GD] = GD_W;
		((float*)cter->gh->data)[(i)*cter->gh->w + j-1] = h;


	}


	MALSET(catm,sizeof(_atm));

	//it must be sunset
	catm->gll_a = 0;
	catm->gll_b = 65;
	catm->gll_clr.i = CLR_ORNG;
	catm->gll_contrast = 10; 
	catm->gll_force = 25;

	catm->env_clr.i = CLR_ORNG;
	catm->env_fmin = 20;
	catm->env_fmax = 100;
	catm->env_fmax_dst = 140;

	catm->skygr_clr = LADD( catm->skygr_clr, malloc(sizeof(_clgrd)),&catm->skygr_clrc );
	catm->skygr_clr[0]->clr = CLR_ORNG;
	catm->skygr_clr[0]->proc = 0;

	catm->skygr_clr = LADD( catm->skygr_clr, malloc(sizeof(_clgrd)),&catm->skygr_clrc );
	catm->skygr_clr[1]->clr = CLR_YEL1;
	catm->skygr_clr[1]->proc = 100;

	
	gclr_lc = 5;
	MALSET( gclr_base, gclr_lc*sizeof(_clr));
	MALSET( gclr_env, gclr_lc*sizeof(_clr));
	MALSET( gclr_res, gclr_lc*sizeof(_clr));

	gclr_base[0].i = CLR_DGREEN;
	gclr_base[1].i = CLR_GREEN;
	gclr_base[2].i = CLR_DBROWN;
	gclr_base[3].i = CLR_LBROWN;
	gclr_base[4].i = CLR_GRAY;
}


int main(int argc, char *argv[] )
{
	SDL_Surface *scr = NULL;

	int i,j=0;
	int tidx = 0;
	int sflag = 0;

	if( 0 < SDL_Init( SDL_INIT_VIDEO ) ) return 0;

	scr = SDL_SetVideoMode(W,H,32, SDL_SWSURFACE );
	SDL_WM_SetCaption("SDL TEST", NULL );
	SDL_ShowCursor(SDL_DISABLE);

	c_precalc();
	tdm_init();
	SDL_WarpMouse( W2, H2 );

	while(1)
	{
		inputproc();	
		pl_move();

		memset( scrb->data, 0xFFFFFFF, W*H*4);
		
		draw_atm();
		dterr_bt();

		memcpy( scr->pixels, scrb->data, W*H*4 );
		SDL_Flip(scr);
		SDL_Delay(20);

		catm->gll_a++;
		if( catm->gll_a > 180 ) catm->gll_a = -180;
	}

	return 0;
}