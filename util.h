#ifndef UTILH

#define UTILH


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


#define FREEIF( p ) if( NULL != p ) free(p); p=NULL;
#define MALSET( p, s ) p = malloc(s); memset( p, 0, s);


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


#endif