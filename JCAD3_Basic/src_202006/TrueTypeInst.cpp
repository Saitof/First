/////////////////////////////////////////////////////////////////////////////
// TrueTypeフォントインストラクション部
// TrueTypeInst.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "DataAccess1.h"
#include "TrueTypeInstData.h"
#include "TrueTypeData.h"

#define PACK(x, y)	((x<<4)|y)
static const BYTE  PopPushCount[256] = {
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// SVTCA y,   SVTCA x,   SPvTCA y,  SPvTCA x,
	PACK(0, 0), PACK(0, 0), PACK(2, 0), PACK(2, 0),		// SFvTCA y,  SFvTCA x,  SPvTL,     SPvTL +,
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// SFvTL,     SFvTL +,   SPvFS,     SFvFS,
	PACK(0, 2), PACK(0, 2), PACK(0, 0), PACK(5, 0),		// GPV,       GFV,       SFvTPv,    ISECT,
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// SRP0,      SRP1,      SRP2,      SZP0,
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// SZP1,      SZP2,      SZPS,      SLOOP,
	PACK(0, 0), PACK(0, 0), PACK(1, 0), PACK(0, 0),		// RTG,       RTHG,      SMD,       ELSE,
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// JMPR,      SCvTCi,    SSwCi,     SSW,
	PACK(1, 2), PACK(1, 0), PACK(0, 0), PACK(2, 2),		// DUP,       POP,       CLEAR,     SWAP,
	PACK(0, 1), PACK(1, 1), PACK(1, 0), PACK(2, 0),		// DEPTH,     CINDEX,    MINDEX,    AlignPTS,
	PACK(0, 0), PACK(1, 0), PACK(2, 0), PACK(1, 0),		// INS_$28,   UTP,       LOOPCALL,  CALL,
	PACK(1, 0), PACK(0, 0), PACK(1, 0), PACK(1, 0),		// FDEF,      ENDF,      MDAP[0],   MDAP[1],
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// IUP[0],    IUP[1],    SHP[0],    SHP[1],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// SHC[0],    SHC[1],    SHZ[0],    SHZ[1],
	PACK(1, 0), PACK(0, 0), PACK(2, 0), PACK(2, 0),		// SHPIX,     IP,        MSIRP[0],  MSIRP[1],
	PACK(0, 0), PACK(0, 0), PACK(2, 0), PACK(2, 0),		// AlignRP,   RTDG,      MIAP[0],   MIAP[1],
	PACK(0, 0), PACK(0, 0), PACK(2, 0), PACK(1, 1),		// NPushB,    NPushW,    WS,        RS,
	PACK(2, 0), PACK(1, 1), PACK(1, 1), PACK(1, 1),		// WCvtP,     RCvt,      GC[0],     GC[1],
	PACK(2, 0), PACK(2, 1), PACK(2, 1), PACK(0, 1),		// SCFS,      MD[0],     MD[1],     MPPEM,
	PACK(0, 1), PACK(0, 0), PACK(0, 0), PACK(1, 0),		// MPS,       FlipON,    FlipOFF,   DEBUG,
	PACK(2, 1), PACK(2, 1), PACK(2, 1), PACK(2, 1),		// LT,        LTEQ,      GT,        GTEQ,
	PACK(2, 1), PACK(2, 1), PACK(1, 1), PACK(1, 1),		// EQ,        NEQ,       ODD,       EVEN,
	PACK(1, 0), PACK(0, 0), PACK(2, 1), PACK(2, 1),		// IF,        EIF,       AND,       OR,
	PACK(1, 1), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// NOT,       DeltaP1,   SDB,       SDS,
	PACK(2, 1), PACK(2, 1), PACK(2, 1), PACK(2, 1),		// ADD,       SUB,       DIV,       MUL,
	PACK(1, 1), PACK(1, 1), PACK(1, 1), PACK(1, 1),		// ABS,       NEG,       FLOOR,     CEILING,
	PACK(1, 1), PACK(1, 1), PACK(1, 1), PACK(1, 1),		// ROUND[0],  ROUND[1],  ROUND[2],  ROUND[3],
	PACK(1, 1), PACK(1, 1), PACK(1, 1), PACK(1, 1),		// NROUND[0], NROUND[1], NROUND[2], NROUND[3],
	PACK(2, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// WCvtF,     DeltaP2,   DeltaP3,   DeltaCn[0],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// DeltaCn[1],DeltaCn[2],SROUND,    S45Round,
	PACK(2, 0), PACK(2, 0), PACK(0, 0), PACK(0, 0),		// JROT,      JROF,      ROFF,      INS_$7B,
	PACK(0, 0), PACK(0, 0), PACK(1, 0), PACK(1, 0),		// RUTG,      RDTG,      SANGW,     AA,
	PACK(0, 0), PACK(2, 0), PACK(2, 0), PACK(0, 0),		// FlipPT,    FlipRgON,  FlipRgOFF, INS_$83,
	PACK(0, 0), PACK(1, 0), PACK(2, 0), PACK(2, 0),		// INS_$84,   ScanCTRL,  SDVPTL[0], SDVPTL[1],
	PACK(1, 1), PACK(1, 0), PACK(3, 3), PACK(2, 1),		// GetINFO,   IDEF,      ROLL,      MAX,
	PACK(2, 1), PACK(1, 0), PACK(2, 0), PACK(0, 0),		// MIN,       ScanTYPE,  InstCTRL,  INS_$8F,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$90,   INS_$91,   INS_$92,   INS_$93,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$94,   INS_$95,   INS_$96,   INS_$97,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$98,   INS_$99,   INS_$9A,   INS_$9B,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$9C,   INS_$9D,   INS_$9E,   INS_$9F,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$A0,   INS_$A1,   INS_$A2,   INS_$A3,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$A4,   INS_$A5,   INS_$A6,   INS_$A7,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$A8,   INS_$A9,   INS_$AA,   INS_$AB,
	PACK(0, 0), PACK(0, 0), PACK(0, 0), PACK(0, 0),		// INS_$AC,   INS_$AD,   INS_$AE,   INS_$AF,
	PACK(0, 1), PACK(0, 2), PACK(0, 3), PACK(0, 4),		// PushB[0],  PushB[1],  PushB[2],  PushB[3],
	PACK(0, 5), PACK(0, 6), PACK(0, 7), PACK(0, 8),		// PushB[4],  PushB[5],  PushB[6],  PushB[7],
	PACK(0, 1), PACK(0, 2), PACK(0, 3), PACK(0, 4),		// PushW[0],  PushW[1],  PushW[2],  PushW[3],
	PACK(0, 5), PACK(0, 6), PACK(0, 7), PACK(0, 8),		// PushW[4],  PushW[5],  PushW[6],  PushW[7],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[00],  MDRP[01],  MDRP[02],  MDRP[03],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[04],  MDRP[05],  MDRP[06],  MDRP[07],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[08],  MDRP[09],  MDRP[10],  MDRP[11],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[12],  MDRP[13],  MDRP[14],  MDRP[15],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[16],  MDRP[17],  MDRP[18],  MDRP[19],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[20],  MDRP[21],  MDRP[22],  MDRP[23],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[24],  MDRP[25],  MDRP[26],  MDRP[27],
	PACK(1, 0), PACK(1, 0), PACK(1, 0), PACK(1, 0),		// MDRP[28],  MDRP[29],  MDRP[30],  MDRP[31],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// MIRP[00],  MIRP[01],  MIRP[02],  MIRP[03],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// MIRP[04],  MIRP[05],  MIRP[06],  MIRP[07],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// MIRP[08],  MIRP[09],  MIRP[10],  MIRP[11],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// MIRP[12],  MIRP[13],  MIRP[14],  MIRP[15],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// MIRP[16],  MIRP[17],  MIRP[18],  MIRP[19],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// MIRP[20],  MIRP[21],  MIRP[22],  MIRP[23],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0),		// MIRP[24],  MIRP[25],  MIRP[26],  MIRP[27],
	PACK(2, 0), PACK(2, 0), PACK(2, 0), PACK(2, 0)		// MIRP[28],  MIRP[29],  MIRP[30],  MIRP[31]
};

static const CHAR OpcodeLength[256] = {
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
   -1,-1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	2, 3, 4, 5,  6, 7, 8, 9,  3, 5, 7, 9, 11,13,15,17,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1
};

static const LONG TrigArctanTable[24] = {
	4157273L, 2949120L, 1740967L, 919879L, 466945L, 234379L, 117304L,
	58666L, 29335L, 14668L, 7334L, 3667L, 1833L, 917L, 458L, 229L, 115L,
	57L, 29L, 14L, 7L, 4L, 2L, 1L
};

static const Vector NullVec = {0,0};
#define NullVector  (Vector*)&NullVec

typedef struct  INT64bit_ {
	UINT  lo;
	UINT  hi;
} INT64bit;

/************************************************/
static void* TCalloc(size_t nitems, size_t size)						//<<< ﾒﾓﾘ確保
/************************************************/
{
	void*  block;
	size_t actsize;

	actsize=nitems*size;
	block = (void *)malloc(actsize);
	if(block==NULL) {
		MemErr(IDS_MEMERR5);
	}
	memset(block, 0, actsize);
	return (void *)((char *)block);
}

/****************************/
static void TFree(void* ptr)											//<<< ﾒﾓﾘ解放
/****************************/
{
	void* block;

	if(ptr!=NULL) {
		block = (void *)((char *)ptr);
		free(block);
	}
}

/**********************************************/
static void multo64(INT x, INT y, INT64bit* z)
/**********************************************/
{
	INT lo1, hi1, lo2, hi2, lo, hi, i1, i2;

	lo1 = x&0x0000FFFFU;
	hi1 = x>>16;
	lo2 = y&0x0000FFFFU;
	hi2 = y>>16;
	lo = lo1*lo2;
	i1 = lo1*hi2;
	i2 = lo2*hi1;
	hi = hi1*hi2;
	i1 += i2;
	hi += (UINT)(i1<i2)<<16;
	hi += i1>>16;
	i1 = i1<<16;
	lo += i1;
	hi += (lo<i1);
	z->lo = lo;
	z->hi = hi;
}

/***********************************************/
static UINT div64by32(UINT hi, UINT lo, UINT y)
/***********************************************/
{
	UINT r=hi, q=0;
	INT  i=32;

	if(r>=y) {
		return (UINT)0x7FFFFFFFL;
	}
	do {
		r <<= 1;
		q <<= 1;
		r |= lo>>31;
		if(r>=(UINT)y) {
			r -= y;
			q |= 1;
		}
		lo <<= 1;
	} while (--i);
	return q;
}

/********************************************************/
static void Add64(INT64bit* x, INT64bit* y, INT64bit* z)
/********************************************************/
{
	register UINT lo, hi, mx;

	mx = x->lo>y->lo ? x->lo : y->lo;
	lo = x->lo+y->lo;
	hi = x->hi+y->hi+(lo<mx);
	z->lo = lo;
	z->hi = hi;
}

/******************************************/
static LONG MulDiv(LONG a, LONG b, LONG c)
/******************************************/
{
	LONG     s;
	INT64bit temp, temp2;

	if(a==0||b==c) {
		return a;
	}
	s = a;
	a = abs(a);
	s ^= b;
	b = abs(b);
	s ^= c;
	c = abs(c);
	if(a<=46340L&&b<=46340L&&c<=176095L&&c>0) {
		a = (a*b+(c>>1))/c;
	} else if(c>0) {
		multo64(a, b, &temp);
		temp2.hi = 0;
		temp2.lo = (UINT)(c>>1);
		Add64(&temp, &temp2, &temp);
		a = div64by32(temp.hi, temp.lo, c);
	} else {
		a = 0x7FFFFFFFL;
	}
	return (s<0 ? -a : a);
}

/**********************************/
static LONG MulFix(LONG a, LONG b)
/**********************************/
{
	LONG  s;
	ULONG ua, ub, al;

	if(a==0||b==0x10000L) {
		return a;
	}
	s = a;
	a = abs(a);
	s ^= b;
	b = abs(b);
	ua = (ULONG)a;
	ub = (ULONG)b;
	if(ua<=2048&&ub<=1048576L) {
		ua = (ua*ub+0x8000)>>16;
	} else {
		al = ua&0xFFFF;
		ua = (ua>>16)*ub+al*(ub>>16)+((al*(ub&0xFFFF)+0x8000)>>16);
	}
	return (s<0 ? -(LONG)ua : (LONG)ua);
}

/**********************************/
static LONG DivFix(LONG a, LONG b)
/**********************************/
{
	INT      s;
	UINT     q;
	INT64bit temp, temp2;

	s = a;
	a = abs(a);
	s ^= b;
	b = abs(b);
	if(b==0) {
		q = 0x7FFFFFFFL;
	} else if((a>>16)==0) {
		q = (UINT)((a<<16)+(b>>1))/(UINT)b;
	} else {
		temp.hi = (INT)(a>>16);
		temp.lo = (UINT)(a<<16);
		temp2.hi = 0;
		temp2.lo = (UINT)(b>>1);
		Add64(&temp, &temp2, &temp);
		q = div64by32(temp.hi, temp.lo, b);
	}
	return (s<0 ? -(LONG)q : (LONG)q);
}

/***********************************/
static INT TrigPrenorm(Vector* vec)
/***********************************/
{
	LONG x=vec->x, y=vec->y, z;
	INT  shift=0;

	z = ((x>=0) ? x : - x)|((y>=0) ? y : -y );
	if(z<(1L<<27)) {
		do {
			shift++;
			z <<= 1;
		} while (z<(1L<<27));
		vec->x = x<<shift;
		vec->y = y<<shift;
	} else if(z>(1L<<28)) {
		do {
			shift++;
			z >>= 1;
		} while (z>(1L<<28));
		vec->x = x>>shift;
		vec->y = y>>shift;
		shift = -shift;
	}
	return shift;
}

/*******************************************/
static void TrigPseudoPolarize(Vector* vec)
/*******************************************/
{
	LONG        theta=0, i=0, x=vec->x, y=vec->y, yi;
	const LONG* arctanptr;

	if(x<0) {
		x = -x;
		y = -y;
		theta = (180L<<16);
	}
	if(y>0) {
		theta = - theta;
	}
	arctanptr = TrigArctanTable;
	if(y<0) {
		yi = y+(x<<1);
		x = x-(y<<1);
		y = yi;
		theta -= *arctanptr++;
	} else {
		yi = y-(x<<1);
		x = x+(y<<1);
		y = yi;
		theta += *arctanptr++;
	}
	do {
		if(y<0) {
			yi = y+(x>>i);
			x = x-(y>>i);
			y = yi;
			theta -= *arctanptr++;
		} else {
			yi = y-(x>>i);
			x = x+(y>>i);
			y = yi;
			theta += *arctanptr++;
		}
	} while(++i<23);
	if(theta>=0) {
		theta = (theta+16)&-32;
	} else {
		theta = -((-theta+16)&-32);
	}
	vec->x = x;
	vec->y = theta;
}

/***********************************/
static LONG TrigDownscale(LONG val)
/***********************************/
{
	LONG s=val;
	UINT v1, v2, k1, k2, hi, lo1, lo2, lo3;

	val = (val>=0) ? val : -val;
	v1 = (UINT)val>>16;
	v2 = (UINT)val&0xFFFF;
	k1 = (1166391785UL)>>16;
	k2 = (1166391785UL)&0xFFFF;
	hi = k1*v1; lo1 = k1*v2 + k2*v1; lo2 = (k2*v2)>>16;
	lo3 = (lo1>=lo2) ? lo1 : lo2;
	lo1 += lo2;
	hi += lo1>>16;
	if(lo1<lo3) {
		hi += 0x10000UL;
	}
	val = (LONG)hi;
	return (s>=0) ? val : -val;
}

/*****************************************************/
static void TrigPseudoRotate(Vector* vec, LONG theta)
/*****************************************************/
{
	INT         i=0;
	LONG        x=vec->x, y=vec->y, xtemp;
	const LONG* arctanptr;

	while(theta<=-((180L<<16)/2)) {
		x = -x;
		y = -y;
		theta += (180L<<16);
	}
	while(theta>(180L<<16)/2) {
		x = -x;
		y = -y;
		theta -= (180L<<16);
	}
	arctanptr = TrigArctanTable;
	if(theta<0) {
		xtemp = x+(y<<1);
		y = y-(x<<1);
		x = xtemp;
		theta += *arctanptr++;
	} else {
		xtemp = x-(y<<1);
		y = y+(x<<1);
		x = xtemp;
		theta -= *arctanptr++;
	}
	do {
		if(theta<0) {
			xtemp = x+(y>>i);
			y = y-(x>>i);
			x = xtemp;
			theta += *arctanptr++;
		} else {
			xtemp = x-(y>>i);
			y = y+(x>>i);
			x = xtemp;
			theta -= *arctanptr++;
		}
	} while(++i<23);
	vec->x = x;
	vec->y = y;
}

/*************************************/
static LONG VectorLength(Vector* vec)
/*************************************/
{
	INT    shift;
	Vector v=*vec;

	if(v.x==0) {
		return (v.y>=0) ? v.y : -v.y;
	} else if(v.y==0) {
		return (v.x>=0) ? v.x : -v.x;
	}
	shift = TrigPrenorm(&v);
	TrigPseudoPolarize(&v);
	v.x = TrigDownscale(v.x);
	return (shift>=0) ? (v.x>>shift) : (v.x<<-shift);
}

/***********************************/
static LONG Atan2(LONG dx, LONG dy)
/***********************************/
{
	Vector v;

	if(dx==0&&dy==0) {
		return 0;
	}
	v.x = dx;
	v.y = dy;
	TrigPrenorm(&v);
	TrigPseudoPolarize(&v);
	return v.y;
}

/***********************************************/
static void VectorUnit(Vector* vec, LONG angle)
/***********************************************/
{
	vec->x = (0x11616E8EUL)>>2;
	vec->y = 0;
	TrigPseudoRotate(vec, angle);
	vec->x >>= 12;
	vec->y >>= 12;
}

/*****************************************/
static SHORT GetShortIns(ExecContext exc)
/*****************************************/
{
	exc->IP += 2;
	return (SHORT)((exc->Code[exc->IP-2]<<8)+exc->Code[exc->IP-1]);
}

/***********************************************************/
static INT GotoCodeRange(ExecContext exc, INT rno, LONG ip)
/***********************************************************/
{
	CodeRange* crng;

	crng = &exc->CodeRTable[rno-1];
	exc->Code = crng->Base;
	exc->CdSize = crng->Size;
	exc->IP = ip;
	exc->cRange = rno;
	return SUCCESS;
}

/***************************************************************/
static BYTE InsGotoCodeRange(ExecContext exc, INT rno, LONG ip)
/***************************************************************/
{
	CodeRange* crng;

	crng = &exc->CodeRTable[rno-1];
	if(crng->Base==NULL) {												// invalid coderange
		exc->Error = ERR_INVALID_CODERANGE;
		return FAILURE;
	}
	if(ip>(LONG)crng->Size) {
		exc->Error = ERR_CODE_OVERFLOW;
		return FAILURE;
	}
	exc->Code = crng->Base;
	exc->CdSize = crng->Size;
	exc->IP = ip;
	exc->cRange = rno;
	return SUCCESS;
}

/*****************************************/
static LONG CurrentRatio(ExecContext exc)
/*****************************************/
{
	Vector v;

	if(exc->TMetric.Ratio) {
		return exc->TMetric.Ratio;
	}
	if(exc->GS.pVector.y==0) {
		exc->TMetric.Ratio = exc->TMetric.xRatio;
	} else if(exc->GS.pVector.x==0) {
		exc->TMetric.Ratio = exc->TMetric.yRatio;
	} else {
		v.x = MulDiv((LONG)exc->GS.pVector.x, exc->TMetric.xRatio, 0x4000L);
		v.y = MulDiv((LONG)exc->GS.pVector.y, exc->TMetric.yRatio, 0x4000L);
		exc->TMetric.Ratio = VectorLength(&v);
	}
	return exc->TMetric.Ratio;
}

/****************************************/
static LONG CurrentPpem(ExecContext exc)
/****************************************/
{
	return MulFix(exc->TMetric.Ppem, CurrentRatio(exc));
}

/**********************************************/
static LONG ReadCVT(ExecContext exc, LONG idx)
/**********************************************/
{
	return exc->Cvt[idx];
}

/*********************************************************/
static void WriteCVT(ExecContext exc, LONG idx, LONG val)
/*********************************************************/
{
	exc->Cvt[idx] = val;
}

/********************************************************/
static void MoveCVT(ExecContext exc, LONG idx, LONG val)
/********************************************************/
{
	exc->Cvt[idx] += val;
}

/*******************************************************/
static void DirectMove(ExecContext exc, GlyphZone* zon,
                       USHORT pnt, LONG dst)
/*******************************************************/
{
	LONG v;

	if((v=exc->GS.fVector.x)!=0) {
		if(abs(exc->FdotP)>0x4000000L) {
			zon->Cur[pnt].x += dst;
		}
		zon->Tag[pnt] |= CURVE_TAG_TOUCH_X;
	}
	if((v=exc->GS.fVector.y)!=0) {
		if(abs(exc->FdotP)>0x4000000L) {
			zon->Cur[pnt].y += dst;
		}
		zon->Tag[pnt] |= CURVE_TAG_TOUCH_Y;
	}
}

/********************************************************/
static void DirectMoveX(ExecContext exc, GlyphZone* zon,
                        USHORT pnt, LONG dst)
/********************************************************/
{
	(*exc) = (*exc);
	zon->Cur[pnt].x += dst;
	zon->Tag[pnt] |= CURVE_TAG_TOUCH_X;
}

/********************************************************/
static void DirectMoveY(ExecContext exc, GlyphZone* zon,
                        USHORT pnt, LONG dst)
/********************************************************/
{
	(*exc) = (*exc);
	zon->Cur[pnt].y += dst;
	zon->Tag[pnt] |= CURVE_TAG_TOUCH_Y;
}

/**********************************************************/
static LONG RoundNone(ExecContext exc, LONG dst, LONG cps)
/**********************************************************/
{
	LONG val;

	(*exc) = (*exc);
	if(dst>=0) {
		if((val=dst+cps)<0) {
			val = 0;
		}
	} else {
		if((val=dst-cps)>0) {
			val = 0;
		}
	}
	return val;
}

/************************************************************/
static LONG RoundToGrid(ExecContext exc, LONG dst, LONG cps)
/************************************************************/
{
	LONG val;

	(*exc) = (*exc);
	if(dst>=0) {
		val = dst+cps+32;
		if(val>0) {
			val &= ~63;
		} else {
			val = 0;
		}
	} else {
		if((val=-((cps-dst+32)&(-64)))>0) {
			val = 0;
		}
	}
	return  val;
}

/****************************************************************/
static LONG RoundToHalfGrid(ExecContext exc, LONG dst, LONG cps)
/****************************************************************/
{
	LONG val;

	(*exc) = (*exc);
	if(dst>=0) {
		if((val=((dst+cps)&(-64))+32)<0) {
			val = 0;
		}
	} else {
		if((val=-(((cps-dst)&(-64))+32))>0) {
			val = 0;
		}
	}
	return val;
}

/****************************************************************/
static LONG RoundDownToGrid(ExecContext exc, LONG dst, LONG cps)
/****************************************************************/
{
	LONG val;

	(*exc) = (*exc);
	if(dst>=0) {
		val = dst+cps;
		if(val>0) {
			val &= ~63;
		} else {
			val = 0;
		}
	} else {
		if((val=-((cps-dst)&(-64)))>0) {
			val = 0;
		}
	}
	return val;
}

/**************************************************************/
static LONG RoundUpToGrid(ExecContext exc, LONG dst, LONG cps)
/**************************************************************/
{
	LONG val;

	(*exc) = (*exc);
	if(dst>=0) {
		val = dst+cps+63;
		if(val>0) {
			val &= ~63;
		} else {
			val = 0;
		}
	} else {
		if((val=-((cps-dst+63)&(-64)))>0) {
			val = 0;
		}
	}
	return val;
}

/******************************************************************/
static LONG RoundToDoubleGrid(ExecContext exc, LONG dst, LONG cps)
/******************************************************************/
{
	LONG val;

	(*exc) = (*exc);
	if(dst>=0) {
		val = dst+cps+16;
		if(val>0) {
			val &= ~31;
		} else {
			val = 0;
		}
	} else {
		if((val=-((cps-dst+16)&(-32)))>0) {
			val = 0;
		}
	}
	return val;
}

/***********************************************************/
static LONG RoundSuper(ExecContext exc, LONG dst, LONG cps)
/***********************************************************/
{
	LONG val;

	if(dst>=0) {
		if((val=(dst-exc->Phase+exc->Thrhld+cps)&(-exc->Period))<0) {
			val = 0;
		}
		val += exc->Phase;
	} else {
		if((val=-((exc->Thrhld-exc->Phase-dst+cps)&(-exc->Period)))>0) {
			val = 0;
		}
		val -= exc->Phase;
	}
	return val;
}

/*************************************************************/
static LONG RoundSuper45(ExecContext exc, LONG dst, LONG cps)
/*************************************************************/
{
	LONG val;

	if(dst>=0) {
		if((val=((dst-exc->Phase+exc->Thrhld+cps)/exc->Period)*exc->Period)<0) {
			val = 0;
		}
		val += exc->Phase;
	} else {
		if((val=-(((exc->Thrhld-exc->Phase-dst+cps)/exc->Period)*exc->Period))>0) {
			val = 0;
		}
		val -= exc->Phase;
	}
	return val;
}

/***************************************************/
static void ComputeRound(ExecContext exc, BYTE rmd)
/***************************************************/
{
	if(rmd==ROUND_OFF) {
		exc->FuncRound = (Round_Func)RoundNone;
	} else if(rmd==ROUND_TO_GRID) {
		exc->FuncRound = (Round_Func)RoundToGrid;
	} else if(rmd==ROUND_UP_TO_GRID) {
		exc->FuncRound = (Round_Func)RoundUpToGrid;
	} else if(rmd==ROUND_DOWN_TO_GRID) {
		exc->FuncRound = (Round_Func)RoundDownToGrid;
	} else if(rmd==ROUND_TO_HALF_GRID) {
		exc->FuncRound = (Round_Func)RoundToHalfGrid;
	} else if(rmd==ROUND_TO_DOUBLE_GRID) {
		exc->FuncRound = (Round_Func)RoundToDoubleGrid;
	} else if(rmd==ROUND_SUPER) {
		exc->FuncRound = (Round_Func)RoundSuper;
	} else if(rmd==ROUND_SUPER_45) {
		exc->FuncRound = (Round_Func)RoundSuper45;
	}
}

/**************************************************************/
static void SetSuperRound(ExecContext exc, LONG gpd, LONG slt)
/**************************************************************/
{
	if((INT)(slt&0xC0)==0) {
		exc->Period = gpd/2;
	} else if((INT)(slt&0xC0)==0x40) {
		exc->Period = gpd;
	} else if((INT)(slt&0xC0)==0x80) {
		exc->Period = gpd*2;
	} else if((INT)(slt&0xC0)==0xC0) {
		exc->Period = gpd;
	}
	if((INT)(slt&0x30)==0) {
		exc->Phase = 0;
	} else if((INT)(slt&0x30)==0x10) {
		exc->Phase = exc->Period/4;
	} else if((INT)(slt&0x30)==0x20) {
		exc->Phase = exc->Period/2;
	} else if((INT)(slt&0x30)==0x30) {
		exc->Phase = gpd*3/4;
	}
	if((INT)(slt&0x0F)==0) {
		exc->Thrhld = exc->Period-1;
	} else {
		exc->Thrhld = ((INT)(slt&0x0F)-4)*exc->Period/8;
	}
	exc->Period /= 256;
	exc->Phase /= 256;
	exc->Thrhld /= 256;
}

/************************************************************/
static LONG Project(ExecContext exc, Vector* v1, Vector* v2)
/************************************************************/
{
	return MulDiv(v1->x-v2->x, (LONG)exc->GS.pVector.x, 0x4000L)+
           MulDiv(v1->y-v2->y, (LONG)exc->GS.pVector.y, 0x4000L);
}

/****************************************************************/
static LONG DualProject(ExecContext exc, Vector* v1, Vector* v2)
/****************************************************************/
{
	return MulDiv(v1->x-v2->x, (LONG)exc->GS.dVector.x, 0x4000L)+
           MulDiv(v1->y-v2->y, (LONG)exc->GS.dVector.y, 0x4000L);
}

/****************************************************************/
static LONG FreeProject(ExecContext exc, Vector* v1, Vector* v2)
/****************************************************************/
{
	return MulDiv(v1->x-v2->x, (LONG)exc->GS.fVector.x, 0x4000L)+
           MulDiv(v1->y-v2->y, (LONG)exc->GS.fVector.y, 0x4000L);
}

/*************************************************************/
static LONG ProjectX(ExecContext exc, Vector* v1, Vector* v2)
/*************************************************************/
{
	(*exc) = (*exc);
	return (v1->x-v2->x);
}

/*************************************************************/
static LONG ProjectY(ExecContext exc, Vector* v1, Vector* v2)
/*************************************************************/
{
	(*exc) = (*exc);
	return (v1->y-v2->y);
}

/*****************************************/
static void ComputeFuncs(ExecContext exc)
/*****************************************/
{
	if(exc->GS.fVector.x==0x4000) {
		exc->FuncFProj = (Project_Func)ProjectX;
		exc->FdotP = exc->GS.pVector.x*0x10000L;
	} else {
		if(exc->GS.fVector.y==0x4000) {
			exc->FuncFProj = (Project_Func)ProjectY;
			exc->FdotP = exc->GS.pVector.y*0x10000L;
		} else {
			exc->FuncFProj = (Project_Func)FreeProject;
			exc->FdotP = (LONG)exc->GS.pVector.x*exc->GS.fVector.x*4+
							(LONG)exc->GS.pVector.y*exc->GS.fVector.y*4;
		}
	}
	if(exc->GS.pVector.x==0x4000) {
		exc->FuncProj = (Project_Func)ProjectX;
	} else {
		if(exc->GS.pVector.y==0x4000) {
			exc->FuncProj = (Project_Func)ProjectY;
		} else {
			exc->FuncProj = (Project_Func)Project;
		}
	}
	if(exc->GS.dVector.x==0x4000) {
		exc->FuncDProj = (Project_Func)ProjectX;
	} else {
		if(exc->GS.dVector.y==0x4000) {
			exc->FuncDProj = (Project_Func)ProjectY;
		} else {
			exc->FuncDProj = (Project_Func)DualProject;
		}
	}
	exc->FuncMove = (Move_Func)DirectMove;
	if(exc->FdotP==0x40000000L) {
		if(exc->GS.fVector.x==0x4000) {
			exc->FuncMove = (Move_Func)DirectMoveX;
		} else {
			if(exc->GS.fVector.y==0x4000) {
				exc->FuncMove = (Move_Func)DirectMoveY;
			}
		}
	}
	if(abs(exc->FdotP)<0x4000000L) {
		exc->FdotP = 0x40000000L;
	}
	exc->TMetric.Ratio = 0;
}

/******************************************************/
static BYTE Normalize(LONG vx, LONG vy, UnitVector* r)
/******************************************************/
{
	Vector v;
	LONG   ang;

	ang = Atan2(vx, vy);
	VectorUnit(&v, ang);
	r->x = (SHORT)(v.x>>2);
	r->y = (SHORT)(v.y>>2);
	return SUCCESS;
}

/*************************************************************/
static BYTE InsSxVTL(ExecContext exc, USHORT id1, USHORT id2,
									INT pc, UnitVector* vec)
/*************************************************************/
{
	LONG    a, b, c;
	Vector* p1, * p2;

	if((UINT)id1>=(UINT)exc->Zp2.nPoint||(UINT)id2>=(UINT)exc->Zp1.nPoint) {
		return FAILURE;
	}
	p1 = exc->Zp1.Cur+id2;
	p2 = exc->Zp2.Cur+id1;
	a = p1->x-p2->x;
	b = p1->y-p2->y;
	if((pc&1)!=0) {
		c = b;
		b = a;
		a = -c;
	}
	Normalize(a, b, vec);
	return SUCCESS;
}

/*************************************************/
static void InsMINDEX(ExecContext exc, LONG* arg)
/*************************************************/
{
	LONG l=arg[0], k;

	if(l<=0||l>exc->Arg) {
		exc->Error=ERR_INVALID_REFERENCE;
		return;
	}
	k = exc->Stack[exc->Arg-l];
	memmove(&exc->Stack[exc->Arg-l], &exc->Stack[exc->Arg-l+1],
											(l-1)*sizeof(LONG));
	exc->Stack[exc->Arg-1] = k;
}

/******************************/
static void InsROLL(LONG* arg)
/******************************/
{
	LONG a=arg[0], b=arg[1], c=arg[2];

	arg[0] = b;
	arg[1] = c;
	arg[2] = a;
}

/*************************************/
static BYTE SkipCode(ExecContext exc)
/*************************************/
{
	exc->IP += exc->Length;
	if(exc->IP<exc->CdSize) {
		exc->OpCd = exc->Code[exc->IP];
		exc->Length = OpcodeLength[exc->OpCd];
		if(exc->Length<0) {
			if(exc->IP+1>exc->CdSize) {
				goto Fail_Overflow;
			}
			exc->Length = exc->Code[exc->IP+1]+2;
		}
		if(exc->IP+exc->Length<=exc->CdSize) {
			return SUCCESS;
		}
	}
Fail_Overflow:
	exc->Error = ERR_CODE_OVERFLOW;
	return FAILURE;
}

/*********************************************/
static void InsIF(ExecContext exc, LONG* arg)
/*********************************************/
{
	INT  nIfs=1;
	BYTE Out=0;

	if(arg[0]!=0) {
		return;
	}
	do {
		if(SkipCode(exc)==FAILURE) {
			return;
		}
		if(exc->OpCd==0x58) {											// IF
			nIfs++;
		} else if(exc->OpCd==0x1B) {									// ELSE
			Out = (BYTE)(nIfs==1);
		} else if(exc->OpCd==0x59) {									// EIF
			nIfs--;
			Out = (BYTE)(nIfs==0);
		}
	} while(Out==0);
}

/************************************/
static void InsELSE(ExecContext exc)
/************************************/
{
	INT nIfs=1;

	do {
		if(SkipCode(exc)==FAILURE) {
			return;
		}
		if(exc->OpCd==0x58) {											// IF
			nIfs++;
		} else if(exc->OpCd==0x59) {									// EIF
			nIfs--;
		}
	} while(nIfs!=0);
}

/***********************************************/
static void InsFDEF(ExecContext exc, LONG* arg)
/***********************************************/
{
	DefRecord* def=exc->FDef, * lmt=def+exc->numFDef;
	ULONG      n=arg[0];

	for(; def<lmt; def++) {
		if(def->Opc==n) {
			break;
		}
	}
	if(def==lmt) {
		if(exc->numFDef>=exc->maxFDef) {
			exc->Error = ERR_TOO_MANY_FUNC_DEF;
			return;
		}
		exc->numFDef++;
	}
	def->Range = exc->cRange;
	def->Opc = n;
	def->Start = exc->IP+1;
	def->Active = TRUE;
	if(n>exc->maxFunc) {
		exc->maxFunc = n;
	}
	while(SkipCode(exc)==SUCCESS) {
		if(exc->OpCd==0x89||exc->OpCd==0x2C) {							// IDEF/FDEF
			exc->Error = ERR_NESTED_DEF;
			return;
		} else if(exc->OpCd==0x2D) {									// ENDF
			return;
		}
	}
}

/************************************/
static void InsENDF(ExecContext exc)
/************************************/
{
	CalRecord* cal;

	if(exc->ClTop<=0) {
		exc->Error = ERR_ENDF_IN_EXC_STRM;
		return;
	}
	exc->ClTop--;
	cal = exc->ClStack+exc->ClTop;
	cal->CurCount--;
	exc->StepIns = FALSE;
	if(cal->CurCount>0) {
		exc->ClTop++;
		exc->IP = cal->CurRestart;
	} else {
		InsGotoCodeRange(exc, cal->CalRange, cal->CalIP);
	}
}

/***********************************************/
static void InsCALL(ExecContext exc, LONG* arg)
/***********************************************/
{
	ULONG      f=arg[0];
	DefRecord* def=exc->FDef+f, * lmt;
	CalRecord* cal;

	if((UINT)f>=(UINT)(exc->maxFunc+1)) {
		goto Fail;
	}
	if(exc->maxFunc+1!=exc->numFDef||def->Opc!=f) {
		def = exc->FDef;
		lmt = def+exc->numFDef;
		while(def<lmt&&def->Opc!=f) {
			def++;
		}
		if(def==lmt) {
			goto Fail;
		}
	}
	if(!def->Active) {
		goto Fail;
	}
	if(exc->ClTop>=exc->ClSize) {
		exc->Error = ERR_STACK_OVERFLOW;
		return;
	}
	cal = exc->ClStack+exc->ClTop++;
	cal->CalRange = exc->cRange;
	cal->CalIP = exc->IP+1;
	cal->CurCount = 1;
	cal->CurRestart = def->Start;
	InsGotoCodeRange(exc, def->Range, def->Start);
	exc->StepIns = FALSE;
	return;
Fail:
	exc->Error = ERR_INVALID_REFERENCE;
}

/***************************************************/
static void InsLOOPCALL(ExecContext exc, LONG* arg)
/***************************************************/
{
	ULONG      f=arg[1];
	DefRecord* def=exc->FDef+f, * lmt;
	CalRecord* cal;

	if((UINT)f>=(UINT)(exc->maxFunc+1)) {
		goto Fail;
	}
	if(exc->maxFunc+1!=exc->numFDef||def->Opc!=f) {
		def = exc->FDef;
		lmt = def+exc->numFDef;
		while(def<lmt&&def->Opc!=f) {
			def++;
		}
		if(def==lmt) {
			goto Fail;
		}
	}
	if(!def->Active) {
		goto Fail;
	}
	if(exc->ClTop>=exc->ClSize) {
		exc->Error = ERR_STACK_OVERFLOW;
		return;
	}
	if(arg[0]>0) {
		cal = exc->ClStack+exc->ClTop++;
		cal->CalRange = exc->cRange;
		cal->CalIP = exc->IP+1;
		cal->CurCount = (INT)arg[0];
		cal->CurRestart = def->Start;
		InsGotoCodeRange(exc, def->Range, def->Start);
		exc->StepIns = FALSE;
	}
	return;
Fail:
	exc->Error = ERR_INVALID_REFERENCE;
}

/***********************************************/
static void InsIDEF(ExecContext exc, LONG* arg)
/***********************************************/
{
	DefRecord* def=exc->IDef, * lmt=def+exc->numIDef;

	for(; def<lmt; def++) {
		if(def->Opc==(ULONG)arg[0]) {
			break;
		}
	}
	if(def==lmt) {
		if(exc->numIDef>=exc->maxIDef) {
			exc->Error = ERR_TOO_MANY_INST_DEF;
			return;
		}
		exc->numIDef++;
	}
	def->Range = exc->cRange;
	def->Opc = arg[0];
	def->Start = exc->IP+1;
	def->Active = TRUE;
	if((ULONG)arg[0]>exc->maxIns) {
		exc->maxIns = arg[0];
	}
	while(SkipCode(exc)==SUCCESS) {
		if(exc->OpCd==0x89||exc->OpCd==0x2C) {							// IDEF/FDEF
			exc->Error = ERR_NESTED_DEF;
			return;
		} else if(exc->OpCd==0x2D) {									// ENDF
			return;
		}
	}
}

/*************************************************/
static void InsNPUSHB(ExecContext exc, LONG* arg)
/*************************************************/
{
	USHORT k, l=(USHORT)exc->Code[exc->IP+1];

	if((UINT)l>=(UINT)(exc->StackSize+1-exc->Top)) {
		exc->Error = ERR_STACK_OVERFLOW;
		return;
	}
	for(k=1; k<=l; k++) {
		arg[k-1] = exc->Code[exc->IP+k+1];
	}
	exc->NewTop += l;
}

/*************************************************/
static void InsNPUSHW(ExecContext exc, LONG* arg)
/*************************************************/
{
	USHORT k, l=(USHORT)exc->Code[exc->IP+1];

	if((UINT)l>=(UINT)(exc->StackSize+1-exc->Top)) {
		exc->Error = ERR_STACK_OVERFLOW;
		return;
	}
	exc->IP += 2;
	for(k=0; k<l; k++) {
		arg[k] = GetShortIns(exc);
	}
	exc->StepIns = FALSE;
	exc->NewTop += l;
}

/************************************************/
static void InsPUSHB(ExecContext exc, LONG* arg)
/************************************************/
{
	USHORT k, l=(USHORT)(exc->OpCd-0xB0+1);

	if((UINT)l>=(UINT)(exc->StackSize+1-exc->Top)) {
		exc->Error = ERR_STACK_OVERFLOW;
		return;
	}
	for(k=1; k<=l; k++) {
		arg[k-1] = exc->Code[exc->IP+k];
	}
}

/************************************************/
static void InsPUSHW(ExecContext exc, LONG* arg)
/************************************************/
{
	USHORT k, l=(USHORT)(exc->OpCd-0xB8+1);

	if((UINT)l>=(UINT)(exc->StackSize+1-exc->Top)) {
		exc->Error = ERR_STACK_OVERFLOW;
		return;
	}
	exc->IP++;
	for(k=0; k<l; k++) {
		arg[k] = GetShortIns(exc);
	}
	exc->StepIns = FALSE;
}

/*********************************************/
static void InsGC(ExecContext exc, LONG* arg)
/*********************************************/
{
	ULONG l=(ULONG)arg[0];
	LONG  r;

	if((UINT)l>=(UINT)exc->Zp2.nPoint) {
		r = 0;
	} else {
		if(exc->OpCd&1) {
			r = exc->FuncDProj(exc, exc->Zp2.Org+l, NullVector);
		} else {
			r = exc->FuncProj(exc, exc->Zp2.Cur+l, NullVector);
		}
	}
	arg[0] = r;
}

/***********************************************/
static void InsSCFS(ExecContext exc, LONG* arg)
/***********************************************/
{
	USHORT l=(USHORT)arg[0];
	LONG   k;

	if((UINT)l>=(UINT)exc->Zp2.nPoint) {
		return;
	}
	k = exc->FuncProj(exc, exc->Zp2.Cur+l, NullVector);
	exc->FuncMove(exc, &exc->Zp2, l, arg[1]-k);
	if(exc->GS.Gep2==0) {
		exc->Zp2.Org[l] = exc->Zp2.Cur[l];
	}
}

/*********************************************/
static void InsMD(ExecContext exc, LONG* arg)
/*********************************************/
{
	USHORT k=(USHORT)arg[1], l=(USHORT)arg[0];
	LONG   d=0;

	if((UINT)l<(UINT)exc->Zp0.nPoint&&(UINT)k<(UINT)exc->Zp1.nPoint) {
		if(exc->OpCd&1) {
			d = exc->FuncProj(exc, exc->Zp0.Cur+l, exc->Zp1.Cur+k);
		} else {
			d = exc->FuncDProj(exc, exc->Zp0.Org+l, exc->Zp1.Org+k);
		}
	}
	arg[0] = d;
}

/*************************************************/
static void InsSDPVTL(ExecContext exc, LONG* arg)
/*************************************************/
{
	LONG    a, b, c;
	USHORT  p1=(USHORT)arg[1], p2=(USHORT)arg[0];
	Vector* v1, * v2;

	if((UINT)p2>=(UINT)exc->Zp1.nPoint||(UINT)p1>=(UINT)exc->Zp2.nPoint) {
		return;
	}
	v1 = exc->Zp1.Org+p2;
	v2 = exc->Zp2.Org+p1;
	a = v1->x-v2->x;
	b = v1->y-v2->y;
	if((exc->OpCd&1)!=0) {
		c = b;
		b = a;
		a = -c;
	}
	Normalize(a, b, &exc->GS.dVector);
	v1 = exc->Zp1.Cur+p2;
	v2 = exc->Zp2.Cur+p1;
	a = v1->x-v2->x;
	b = v1->y-v2->y;
	if((exc->OpCd&1)!=0) {
		c = b;
		b = a;
		a = -c;
	}
	Normalize(a, b, &exc->GS.pVector);
	ComputeFuncs(exc);
}

/***********************************************/
static void InsSZP0(ExecContext exc, LONG* arg)
/***********************************************/
{
	if((INT)arg[0]==0) {
		exc->Zp0 = exc->Twilight;
	} else if((INT)arg[0]==1) {
		exc->Zp0 = exc->Pts;
	} else {
		return;
	}
	exc->GS.Gep0 = (USHORT)arg[0];
}

/***********************************************/
static void InsSZP1(ExecContext exc, LONG* arg)
/***********************************************/
{
	if((INT)arg[0]==0) {
		exc->Zp1 = exc->Twilight;
	} else if((INT)arg[0]==1) {
		exc->Zp1 = exc->Pts;
	} else {
		return;
	}
	exc->GS.Gep1 = (USHORT)arg[0];
}

/***********************************************/
static void InsSZP2(ExecContext exc, LONG* arg)
/***********************************************/
{
	if((INT)arg[0]==0) {
		exc->Zp2 = exc->Twilight;
	} else if((INT)arg[0]==1) {
		exc->Zp2 = exc->Pts;
	} else {
		return;
	}
	exc->GS.Gep2 = (USHORT)arg[0];
}

/***********************************************/
static void InsSZPS(ExecContext exc, LONG* arg)
/***********************************************/
{
	if((INT)arg[0]==0) {
		exc->Zp0 = exc->Twilight;
	} else if((INT)arg[0]==1) {
		exc->Zp0 = exc->Pts;
	} else {
		return;
	}
	exc->Zp1 = exc->Zp0;
	exc->Zp2 = exc->Zp0;
	exc->GS.Gep0 = (USHORT)arg[0];
	exc->GS.Gep1 = (USHORT)arg[0];
	exc->GS.Gep2 = (USHORT)arg[0];
}

/***************************************************/
static void InsINSTCTRL(ExecContext exc, LONG* arg)
/***************************************************/
{
	LONG k=arg[1], l=arg[0];

	if(k<1||k>2) {
		return;
	}
	if(l!=0) {
		l = k;
	}
	exc->GS.InstCtrl = (BYTE)(((BYTE)exc->GS.InstCtrl&~(BYTE)k)|(BYTE)l);
}

/***************************************************/
static void InsSCANCTRL(ExecContext exc, LONG* arg)
/***************************************************/
{
	INT a=(INT)(arg[0]&0xFF);

	if(a==0xFF) {
		exc->GS.ScanCtrl = TRUE;
		return;
	} else if(a==0) {
		exc->GS.ScanCtrl = FALSE;
		return;
	}
	a *= 64;
	if((arg[0]&0x200)!=0&&exc->TMetric.Rotate) {
		exc->GS.ScanCtrl = TRUE;
	}
	if((arg[0]&0x400)!=0&&exc->TMetric.Stretch) {
		exc->GS.ScanCtrl = TRUE;
	}
	if((arg[0]&0x1000)!=0&&exc->TMetric.Rotate) {
		exc->GS.ScanCtrl = FALSE;
	}
	if((arg[0]&0x2000)!=0&&exc->TMetric.Stretch) {
		exc->GS.ScanCtrl = FALSE;
	}
}

/***************************************************/
static void InsSCANTYPE(ExecContext exc, LONG* arg)
/***************************************************/
{
	if(arg[0]>=0&&arg[0]<=5) {
		if(arg[0]==3) {
			arg[0] = 2;
		}
		exc->GS.ScanType = (INT)arg[0];
	}
}

/**************************************/
static void InsFLIPPT(ExecContext exc)
/**************************************/
{
	USHORT pnt;

	if(exc->Top<exc->GS.Loop) {
		exc->Error = ERR_TOO_FEW_ARGUMENT;
		return;
	}
	while(exc->GS.Loop>0) {
		exc->Arg--;
		pnt = (USHORT)exc->Stack[exc->Arg];
		if((UINT)pnt<(UINT)exc->Pts.nPoint) {
			exc->Pts.Tag[pnt] ^= CURVE_TAG_ON;
		}
		exc->GS.Loop--;
	}
	exc->GS.Loop = 1;
	exc->NewTop = exc->Arg;
}

/***************************************************/
static void InsFLIPRGON(ExecContext exc, LONG* arg)
/***************************************************/
{
	USHORT i, k=(USHORT)arg[1], l=(USHORT)arg[0];

	if((UINT)k>=(UINT)exc->Pts.nPoint||
		(UINT)l>=(UINT)exc->Pts.nPoint) {
		return;
	}
	for(i=l; i<=k; i++) {
		exc->Pts.Tag[i] |= CURVE_TAG_ON;
	}
}

/****************************************************/
static void InsFLIPRGOFF(ExecContext exc, LONG* arg)
/****************************************************/
{
	USHORT i, k=(USHORT)arg[1], l=(USHORT)arg[0];

	if((UINT)k>=(UINT)exc->Pts.nPoint||(UINT)l>=(UINT)exc->Pts.nPoint) {
		return;
	}
	for(i=l; i<=k; i++) {
		exc->Pts.Tag[i] &= ~CURVE_TAG_ON;
	}
}

/*************************************************************/
static INT ComputePointDisplacement(ExecContext exc, LONG* x,
						LONG* y, GlyphZone* zon, USHORT* ref)
/*************************************************************/
{
	GlyphZone zp;
	USHORT    p;
	LONG      d;

	if(exc->OpCd&1) {
		zp = exc->Zp0;
		p = exc->GS.Rp1;
	} else {
		zp = exc->Zp1;
		p = exc->GS.Rp2;
	}
	if((UINT)p>=(UINT)zp.nPoint) {
		return FAILURE;
	}
	*zon = zp;
	*ref = p;
	d = exc->FuncProj(exc, zp.Cur+p, zp.Org+p);
	*x = MulDiv(d, (LONG)exc->GS.fVector.x, 0x4000L);
	*y = MulDiv(d, (LONG)exc->GS.fVector.y, 0x4000L);
	return SUCCESS;
}

/**************************************************************/
static void MoveZp2Point(ExecContext exc, USHORT pnt, LONG dx,
											LONG dy, BYTE tch)
/**************************************************************/
{
	if(exc->GS.fVector.x!=0) {
		exc->Zp2.Cur[pnt].x += dx;
		if(tch) {
			exc->Zp2.Tag[pnt] |= CURVE_TAG_TOUCH_X;
		}
	}
	if(exc->GS.fVector.y != 0) {
		exc->Zp2.Cur[pnt].y += dy;
		if(tch) {
			exc->Zp2.Tag[pnt] |= CURVE_TAG_TOUCH_Y;
		}
	}
}

/***********************************/
static void InsSHP(ExecContext exc)
/***********************************/
{
	GlyphZone zp;
	USHORT    ref, pnt;
	LONG      dx, dy;

	if(exc->Top<exc->GS.Loop) {
		exc->Error = ERR_INVALID_REFERENCE;
		return;
	}
	if(ComputePointDisplacement(exc, &dx, &dy, &zp, &ref)) {
		return;
	}
	while(exc->GS.Loop>0) {
		exc->Arg--;
		pnt = (USHORT)exc->Stack[exc->Arg];
		if((UINT)pnt<(UINT)exc->Zp2.nPoint) {
			MoveZp2Point(exc, pnt, dx, dy, TRUE);
		}
		exc->GS.Loop--;
	}
	exc->GS.Loop = 1;
	exc->NewTop = exc->Arg;
}

/**********************************************/
static void InsSHC(ExecContext exc, LONG* arg)
/**********************************************/
{
	GlyphZone zp;
	USHORT    ref, fpnt, lpnt, i;
	LONG      dx, dy;
	USHORT    ctr=(USHORT)arg[0];

	if((UINT)ctr>=(UINT)exc->Pts.nContour) {
		return;
	}
	if(ComputePointDisplacement(exc, &dx, &dy, &zp, &ref)) {
		return;
	}
	if(ctr==0) {
		fpnt = 0;
	} else {
		fpnt = (USHORT)(exc->Pts.Contour[ctr-1]+1);
	}
	lpnt = exc->Pts.Contour[ctr];
	if(lpnt>exc->Zp2.nPoint) {
		if(exc->Zp2.nPoint>0) {
			lpnt = (USHORT)(exc->Zp2.nPoint-1);
		} else {
			lpnt = 0;
		}
	}
	for(i=fpnt; i<=lpnt; i++) {
		if(zp.Cur!=exc->Zp2.Cur||ref!=i) {
			MoveZp2Point(exc, i, dx, dy, FALSE);
		}
	}
}

/**********************************************/
static void InsSHZ(ExecContext exc, LONG* arg)
/**********************************************/
{
	GlyphZone zp;
	USHORT    ref, lpnt, i;
	LONG      dx, dy;

	if((UINT)arg[0]>=2) {
		return;
	}
	if(ComputePointDisplacement(exc, &dx, &dy, &zp, &ref)) {
		return;
	}
	if(exc->Zp2.nPoint>0) {
		lpnt = (USHORT)(exc->Zp2.nPoint-1);
	} else {
		lpnt = 0;
	}
	for(i=0; i<=lpnt; i++) {
		if(zp.Cur!=exc->Zp2.Cur||ref!=i) {
			MoveZp2Point(exc, i, dx, dy, FALSE);
		}
	}
}

/************************************************/
static void InsSHPIX(ExecContext exc, LONG* arg)
/************************************************/
{
	USHORT pnt;
	LONG   dx, dy;

	if(exc->Top<exc->GS.Loop+1) {
		exc->Error = ERR_INVALID_REFERENCE;
		return;
	}
	dx = MulDiv(arg[0], (LONG)exc->GS.fVector.x, 0x4000L);
	dy = MulDiv(arg[0], (LONG)exc->GS.fVector.y, 0x4000L);
	while(exc->GS.Loop>0) {
		exc->Arg--;
		pnt = (USHORT)exc->Stack[exc->Arg];
		if((UINT)pnt<(UINT)exc->Zp2.nPoint) {
			MoveZp2Point(exc, pnt, dx, dy, TRUE);
		}
		exc->GS.Loop--;
	}
	exc->GS.Loop = 1;
	exc->NewTop = exc->Arg;
}

/************************************************/
static void InsMSIRP(ExecContext exc, LONG* arg)
/************************************************/
{
	USHORT pnt=(USHORT)arg[0];
	LONG   dst;

	if((UINT)pnt>=(UINT)exc->Zp1.nPoint||
		(UINT)exc->GS.Rp0>=(UINT)exc->Zp0.nPoint) {
		return;
	}
	if(exc->GS.Gep0==0) {
		exc->Zp1.Org[pnt] = exc->Zp0.Org[exc->GS.Rp0];
		exc->Zp1.Cur[pnt] = exc->Zp1.Org[pnt];
	}
	dst = exc->FuncProj(exc, exc->Zp1.Cur+pnt, exc->Zp0.Cur+exc->GS.Rp0);
	exc->FuncMove(exc, &exc->Zp1, pnt, arg[1]-dst);
	exc->GS.Rp1 = exc->GS.Rp0;
	exc->GS.Rp2 = pnt;
	if((exc->OpCd&1)!=0) {
		exc->GS.Rp0 = pnt;
	}
}

/***********************************************/
static void InsMDAP(ExecContext exc, LONG* arg)
/***********************************************/
{
	USHORT pnt=(USHORT)arg[0];
	LONG   cdst, dst;

	if((UINT)pnt>=(UINT)exc->Zp0.nPoint) {
		return;
	}
	if((exc->OpCd&1)!=0) {
		cdst = exc->FuncProj(exc, exc->Zp0.Cur+pnt, NullVector);
		dst = exc->FuncRound(exc, cdst, exc->TMetric.Compens[0])-cdst;
	} else {
		dst = 0;
	}
	exc->FuncMove(exc, &exc->Zp0, pnt, dst);
	exc->GS.Rp0 = pnt;
	exc->GS.Rp1 = pnt;
}

/***********************************************/
static void InsMIAP(ExecContext exc, LONG* arg)
/***********************************************/
{
	ULONG  ent=(ULONG)arg[1];
	USHORT pnt=(USHORT)arg[0];
	LONG   dst, odst;

	if((UINT)pnt>=(UINT)exc->Zp0.nPoint||(UINT)ent>=(UINT)exc->CvtSize) {
		return;
	}
	dst = exc->FuncReadCvt(exc, ent);
	if(exc->GS.Gep0 == 0) {
		exc->Zp0.Org[pnt].x = MulDiv((LONG)exc->GS.fVector.x, dst, 0x4000L);
		exc->Zp0.Org[pnt].y = MulDiv((LONG)exc->GS.fVector.y, dst, 0x4000L);
		exc->Zp0.Cur[pnt] = exc->Zp0.Org[pnt];
	}
	odst = exc->FuncProj(exc, exc->Zp0.Cur+pnt, NullVector);
	if((exc->OpCd&1)!=0) {
		if(abs(dst-odst)>exc->GS.CtrlValCI) {
			dst = odst;
		}
		dst = exc->FuncRound(exc, dst, exc->TMetric.Compens[0]);
	}
	exc->FuncMove(exc, &exc->Zp0, pnt, dst-odst);
	exc->GS.Rp0 = pnt;
	exc->GS.Rp1 = pnt;
}

/***********************************************/
static void InsMDRP(ExecContext exc, LONG* arg)
/***********************************************/
{
	USHORT pnt=(USHORT)arg[0];
	LONG   odst, dst;

	if((UINT)pnt>=(UINT)exc->Zp1.nPoint||
		(UINT)exc->GS.Rp0>=(UINT)exc->Zp0.nPoint) {
		return;
	}
	odst=exc->FuncDProj(exc, exc->Zp1.Org+pnt, exc->Zp0.Org+exc->GS.Rp0);
	if(abs(odst)<exc->GS.SglWidthCI) {
		if(odst>=0) {
			odst = exc->GS.SglWidthVal;
		} else {
			odst = -exc->GS.SglWidthVal;
		}
	}
	if((exc->OpCd&4)!=0) {
		dst = exc->FuncRound(exc, odst, exc->TMetric.Compens[exc->OpCd&3]);
	} else {
		dst = RoundNone(exc, odst, exc->TMetric.Compens[exc->OpCd&3]);
	}
	if((exc->OpCd&8)!=0) {
		if(odst>=0) {
			if(dst<exc->GS.MinDist) {
				dst = exc->GS.MinDist;
			}
		} else {
			if(dst>-exc->GS.MinDist) {
				dst = -exc->GS.MinDist;
			}
		}
	}
	odst = exc->FuncProj(exc, exc->Zp1.Cur+pnt, exc->Zp0.Cur+exc->GS.Rp0);
	exc->FuncMove(exc, &exc->Zp1, pnt, dst-odst);
	exc->GS.Rp1 = exc->GS.Rp0;
	exc->GS.Rp2 = pnt;
	if((exc->OpCd&16)!=0) {
		exc->GS.Rp0 = pnt;
	}
}

/***********************************************/
static void InsMIRP(ExecContext exc, LONG* arg)
/***********************************************/
{
	USHORT pnt=(USHORT)arg[0];
	ULONG  ent=(ULONG)(arg[1]+1);
	LONG   cdst, dst, udst, odst;

	if((UINT)pnt>=(UINT)exc->Zp1.nPoint||
		(UINT)ent>=(UINT)(exc->CvtSize+1)||
		(UINT)exc->GS.Rp0>=(UINT)exc->Zp0.nPoint) {
		return;
	}
	if(!ent) {
		cdst = 0;
	} else {
		cdst = exc->FuncReadCvt(exc, ent-1);
	}
	if(abs(cdst)<exc->GS.SglWidthCI) {
		if(cdst>=0) {
			cdst = exc->GS.SglWidthVal;
		} else {
			cdst = -exc->GS.SglWidthVal;
		}
	}
	if(exc->GS.Gep1==0) {
		exc->Zp1.Org[pnt].x = exc->Zp0.Org[exc->GS.Rp0].x+
				MulDiv(cdst, (LONG)exc->GS.fVector.x, 0x4000L);
		exc->Zp1.Org[pnt].y = exc->Zp0.Org[exc->GS.Rp0].y+
				MulDiv(cdst, (LONG)exc->GS.fVector.y, 0x4000L);
		exc->Zp1.Cur[pnt] = exc->Zp1.Org[pnt];
	}
	odst=exc->FuncDProj(exc, exc->Zp1.Org+pnt, exc->Zp0.Org+exc->GS.Rp0);
	udst=exc->FuncProj(exc, exc->Zp1.Cur+pnt, exc->Zp0.Cur+exc->GS.Rp0);
	if(exc->GS.AutoFlip&&(odst^cdst)<0) {
		cdst = -cdst;
	}
	if((exc->OpCd&4)!=0) {
		if(exc->GS.Gep0==exc->GS.Gep1) {
			if(abs(cdst-odst)>=exc->GS.CtrlValCI) {
				cdst = odst;
			}
		}
		dst=exc->FuncRound(exc, cdst, exc->TMetric.Compens[exc->OpCd&3]);
	} else {
		dst=RoundNone(exc, cdst, exc->TMetric.Compens[exc->OpCd&3]);
	}
	if((exc->OpCd&8)!=0) {
		if(odst>=0) {
			if(dst<exc->GS.MinDist) {
				dst = exc->GS.MinDist;
			}
		} else {
			if(dst>-exc->GS.MinDist) {
				dst = -exc->GS.MinDist;
			}
		}
	}
	exc->FuncMove(exc, &exc->Zp1, pnt, dst-udst);
	exc->GS.Rp1 = exc->GS.Rp0;
	if((exc->OpCd&16)!=0) {
		exc->GS.Rp0 = pnt;
	}
	exc->GS.Rp2 = pnt;
}

/***************************************/
static void InsALIGNRP(ExecContext exc)
/***************************************/
{
	USHORT pnt;
	LONG   dst;

	if(exc->Top<exc->GS.Loop||(UINT)exc->GS.Rp0>=(UINT)exc->Zp0.nPoint) {
		return;
	}
	while(exc->GS.Loop>0) {
		exc->Arg--;
		pnt = (USHORT)exc->Stack[exc->Arg];
		if((UINT)pnt<(UINT)exc->Zp1.nPoint) {
			dst=exc->FuncProj(exc, exc->Zp1.Cur+pnt, exc->Zp0.Cur+exc->GS.Rp0);
			exc->FuncMove(exc, &exc->Zp1, pnt, -dst);
		}
		exc->GS.Loop--;
	}
	exc->GS.Loop = 1;
	exc->NewTop = exc->Arg;
}

/************************************************/
static void InsISECT(ExecContext exc, LONG* arg)
/************************************************/
{
	USHORT pnt, a0, a1, b0, b1;
	LONG   dsc, dx, dy, dax, day, dbx, dby, val;
	Vector r;

	pnt = (USHORT)arg[0];
	a0 = (USHORT)arg[1];
	a1 = (USHORT)arg[2];
	b0 = (USHORT)arg[3];
	b1 = (USHORT)arg[4];
	if((UINT)b0>=(UINT)exc->Zp0.nPoint||
       (UINT)b1>=(UINT)exc->Zp0.nPoint||
       (UINT)a0>=(UINT)exc->Zp1.nPoint||
       (UINT)a1>=(UINT)exc->Zp1.nPoint||
       (UINT)pnt>=(UINT)exc->Zp2.nPoint) {
		return;
	}
	dbx = exc->Zp0.Cur[b1].x-exc->Zp0.Cur[b0].x;
	dby = exc->Zp0.Cur[b1].y-exc->Zp0.Cur[b0].y;
	dax = exc->Zp1.Cur[a1].x-exc->Zp1.Cur[a0].x;
	day = exc->Zp1.Cur[a1].y-exc->Zp1.Cur[a0].y;
	dx = exc->Zp0.Cur[b0].x-exc->Zp1.Cur[a0].x;
	dy = exc->Zp0.Cur[b0].y-exc->Zp1.Cur[a0].y;
	exc->Zp2.Tag[pnt] |= CURVE_TAG_TOUCH_BOTH;
	dsc = MulDiv(dax, -dby, 0x40L)+MulDiv(day, dbx, 0x40L);
	if(abs(dsc)>=0x40) {
		val = MulDiv(dx, -dby, 0x40L)+MulDiv(dy, dbx, 0x40L);
		r.x = MulDiv(val, dax, dsc);
		r.y = MulDiv(val, day, dsc);
		exc->Zp2.Cur[pnt].x = exc->Zp1.Cur[a0].x+r.x;
		exc->Zp2.Cur[pnt].y = exc->Zp1.Cur[a0].y+r.y;
	} else {
		exc->Zp2.Cur[pnt].x = (exc->Zp1.Cur[a0].x+exc->Zp1.Cur[a1].x+
								exc->Zp0.Cur[b0].x+exc->Zp0.Cur[b1].x)/4;
		exc->Zp2.Cur[pnt].y = (exc->Zp1.Cur[a0].y+exc->Zp1.Cur[a1].y+
								exc->Zp0.Cur[b0].y+exc->Zp0.Cur[b1].y)/4;
	}
}

/***************************************************/
static void InsALIGNPTS(ExecContext exc, LONG* arg)
/***************************************************/
{
	USHORT p1=(USHORT)arg[0], p2=(USHORT)arg[1];
	LONG   dst;

	if((UINT)arg[0]>=(UINT)exc->Zp1.nPoint||(UINT)arg[1]>=(UINT)exc->Zp0.nPoint) {
		return;
	}
	dst = exc->FuncProj(exc, exc->Zp0.Cur+p2, exc->Zp1.Cur+p1)/2;
	exc->FuncMove(exc, &exc->Zp1, p1, dst);
	exc->FuncMove(exc, &exc->Zp0, p2, -dst);
}

/**********************************/
static void InsIP(ExecContext exc)
/**********************************/
{
	LONG   oga, ogb, ogx, cra, crb, crx, dst;
	USHORT pnt;

	if(exc->Top<exc->GS.Loop) {
		exc->Error = ERR_INVALID_REFERENCE;
		return;
	}
	if((UINT)exc->GS.Rp1>=(UINT)exc->Zp0.nPoint||
		(UINT)exc->GS.Rp2>=(UINT)exc->Zp1.nPoint) {
		oga = 0;
		cra = 0;
		ogb = 0;
		crb = 0;
	} else {
		oga=exc->FuncDProj(exc, exc->Zp0.Org+exc->GS.Rp1, NullVector);
		ogb=exc->FuncDProj(exc, exc->Zp1.Org+exc->GS.Rp2, NullVector);
		cra=exc->FuncProj(exc, exc->Zp0.Cur+exc->GS.Rp1, NullVector);
		crb=exc->FuncProj(exc, exc->Zp1.Cur+exc->GS.Rp2, NullVector);
	}
	while(exc->GS.Loop>0) {
		exc->Arg--;
		pnt = (USHORT)exc->Stack[exc->Arg];
		if((UINT)pnt<(UINT)exc->Zp2.nPoint) {
			ogx = exc->FuncDProj(exc, exc->Zp2.Org+pnt, NullVector);
			crx = exc->FuncProj(exc, exc->Zp2.Cur+pnt, NullVector);
			if((oga<=ogb&&ogx<=oga)||(oga>ogb&&ogx>=oga)) {
				dst = (cra-oga)+(ogx-crx);
			} else if((oga<=ogb&&ogx>=ogb)||(oga>ogb&&ogx<ogb)) {
				dst = (crb-ogb)+(ogx-crx);
			} else {
				dst = MulDiv(crb-cra, ogx-oga, ogb-oga)+(cra-crx);
			}
			exc->FuncMove(exc, &exc->Zp2, pnt, dst);
		}
		exc->GS.Loop--;
	}
	exc->GS.Loop = 1;
	exc->NewTop = exc->Arg;
}

/**********************************************/
static void InsUTP(ExecContext exc, LONG* arg)
/**********************************************/
{
	USHORT pnt=(USHORT)arg[0];
	BYTE   mak;

	if((UINT)pnt>=(UINT)exc->Zp0.nPoint) {
		return;
	}
	mak = 0xFF;
	if(exc->GS.fVector.x!=0) {
		mak &= ~CURVE_TAG_TOUCH_X;
	}
	if(exc->GS.fVector.y!=0) {
		mak &= ~CURVE_TAG_TOUCH_Y;
	}
	exc->Zp0.Tag[pnt] &= mak;
}

struct LOC_IUP {
	Vector* orgs;		// original and current coordinate
	Vector* curs;		// arrays
};

/****************************************************************/
static void Shift(UINT p1, UINT p2, UINT p, struct LOC_IUP* lnk)
/****************************************************************/
{
	UINT i;
	LONG x;

	x = lnk->curs[p].x-lnk->orgs[p].x;
	for(i=p1; i<p; i++) {
		lnk->curs[i].x += x;
	}
	for(i=p+1; i<=p2; i++) {
		lnk->curs[i].x += x;
	}
}

/********************************************************/
static void Interp(UINT p1, UINT p2, UINT rf1, UINT rf2,
                   struct LOC_IUP* lnk)
/********************************************************/
{
	UINT i;
	LONG x, x1, x2, d1, d2;

	if(p1>p2) {
		return;
	}
	x1 = lnk->orgs[rf1].x;
	d1 = lnk->curs[rf1].x-lnk->orgs[rf1].x;
	x2 = lnk->orgs[rf2].x;
	d2 = lnk->curs[rf2].x-lnk->orgs[rf2].x;
	if(x1==x2) {
		for(i=p1; i<=p2; i++) {
			x = lnk->orgs[i].x;
			x += (x<=x1) ? d1 : d2;
			lnk->curs[i].x = x;
		}
		return;
	}
	if(x1<x2) {
		for(i=p1; i<=p2; i++) {
			x = lnk->orgs[i].x;
			if(x<=x1) {
				x += d1;
			} else {
				if(x>=x2) {
					x += d2;
				} else {
					x = lnk->curs[rf1].x +
						MulDiv(x-x1, lnk->curs[rf2].x-lnk->curs[rf1].x, x2-x1);
				}
			}
			lnk->curs[i].x = x;
		}
		return;
	}
	for(i=p1; i<=p2; i++) {
		x = lnk->orgs[i].x;
		if(x<=x2) {
			x += d2;
		} else {
			if(x>=x1) {
				x += d1;
			} else {
				x = lnk->curs[rf1].x +
					MulDiv(x-x1, lnk->curs[rf2].x-lnk->curs[rf1].x, x2-x1);
			}
		}
		lnk->curs[i].x = x;
	}
}

/***********************************/
static void InsIUP(ExecContext exc)
/***********************************/
{
	struct LOC_IUP v;
	BYTE  mak;
	UINT  fpnt, epnt, ftch, ctch, pnt=0;
	SHORT ctr=0;

	if(exc->OpCd&1) {
		mak = CURVE_TAG_TOUCH_X;
		v.orgs = exc->Pts.Org;
		v.curs = exc->Pts.Cur;
	} else {
		mak = CURVE_TAG_TOUCH_Y;
		v.orgs = (Vector*)((LONG*)exc->Pts.Org+1);
		v.curs = (Vector*)((LONG*)exc->Pts.Cur+1);
	}
	do {
		epnt = exc->Pts.Contour[ctr];
		fpnt = pnt;
		while (pnt<=epnt&&(exc->Pts.Tag[pnt]&mak)==0) {
			pnt++;
		}
		if(pnt<=epnt) {
			ftch = pnt;
			ctch = pnt;
			pnt++;
			while(pnt<=epnt) {
				if((exc->Pts.Tag[pnt]&mak)!=0) {
					if(pnt>0) {
						Interp(ctch+1, pnt-1, ctch, pnt, &v);
					}
					ctch = pnt;
				}
				pnt++;
			}
			if(ctch==ftch) {
				Shift(fpnt, epnt, ctch, &v);
			} else {
				Interp((USHORT)(ctch+1), epnt, ctch, ftch, &v);
				if(ftch>0) {
					Interp(fpnt, ftch-1, ctch, ftch, &v);
				}
			}
		}
		ctr++;
	} while(ctr<exc->Pts.nContour);
}

/*************************************************/
static void InsDELTAP(ExecContext exc, LONG* arg)
/*************************************************/
{
	USHORT a;
	LONG   b;
	ULONG  c, k, num=(ULONG)arg[0];

	for(k=1; k<=num; k++) {
		if(exc->Arg<2) {
			exc->Error = ERR_TOO_FEW_ARGUMENT;
			return;
		}
		exc->Arg -= 2;
		a = (USHORT)exc->Stack[exc->Arg+1];
		b = exc->Stack[exc->Arg];
		if((UINT)a<(UINT)exc->Zp0.nPoint) {
			c = ((ULONG)b&0xF0)>>4;
			if(exc->OpCd==0x71) {
				c += 16;
			} else if(exc->OpCd==0x72) {
				c += 32;
			}
			c += exc->GS.DeltaBase;
			if(CurrentPpem(exc)==(LONG)c) {
				if((b=((ULONG)b&0xF)-8)>=0) {
					b++;
				}
				b = b*64/(1L<<exc->GS.DeltaShift);
				exc->FuncMove(exc, &exc->Zp0, a, b);
			}
		}
	}
	exc->NewTop = exc->Arg;
}

/*************************************************/
static void InsDELTAC(ExecContext exc, LONG* arg)
/*************************************************/
{
	ULONG a, c, k, num=(ULONG)arg[0];
	LONG  b;

	for(k=1; k<=num; k++) {
		if(exc->Arg<2) {
			exc->Error = ERR_TOO_FEW_ARGUMENT;
			return;
		}
		exc->Arg -= 2;
		a = (ULONG)exc->Stack[exc->Arg+1];
		b = exc->Stack[exc->Arg];
		if((UINT)a<(UINT)exc->CvtSize) {
			c = ((ULONG)b&0xF0)>>4;
			if(exc->OpCd==0x74) {
				c += 16;
			} else if(exc->OpCd==0x75) {
				c += 32;
			}
			c += exc->GS.DeltaBase;
			if(CurrentPpem(exc)==(LONG)c) {
				if((b=((ULONG)b&0xF)-8)>=0) {
					b++;
				}
				b = b*64/(1L<<exc->GS.DeltaShift);
				exc->FuncMoveCvt(exc, a, b);
			}
		}
	}
	exc->NewTop = exc->Arg;
}

/**************************************************/
static void InsGETINFO(ExecContext exc, LONG* arg)
/**************************************************/
{
	LONG k=0;

	if((arg[0]&1)!=0) {
		k = 3;
	}
	if(exc->TMetric.Rotate) {
		k |= 0x80;
	}
	if(exc->TMetric.Stretch) {
		k |= 0x100;
	}
	arg[0] = k;
}

/***************************************/
static void InsUNKNOWN(ExecContext exc)
/***************************************/
{
	DefRecord* def=exc->IDef, * lmt=def+exc->numIDef;
	CalRecord* cal;

	for(; def<lmt; def++) {
		if((BYTE)def->Opc==exc->OpCd&&def->Active) {
			if(exc->ClTop>=exc->ClSize) {
				exc->Error = ERR_STACK_OVERFLOW;
				return;
			}
			cal = exc->ClStack+exc->ClTop++;
			cal->CalRange = exc->cRange;
			cal->CalIP = exc->IP+1;
			cal->CurCount = 1;
			cal->CurRestart = def->Start;
			InsGotoCodeRange(exc, def->Range, def->Start);
			exc->StepIns = FALSE;
			return;
		}
	}
	exc->Error = ERR_INVALID_OPCODE;
}

/**********************************/
static INT RunIns(ExecContext exc)
/**********************************/
{
	DefRecord* def, * lmt;
	CalRecord* cal;
	LONG  cnt=0, x, y, l;
	ULONG i;
	SHORT a, b, s;
	BYTE  opc;
	LONG* arg;

	exc->TMetric.Ratio = 0;
	exc->FuncReadCvt = ReadCVT;
	exc->FuncWriteCvt = WriteCVT;
	exc->FuncMoveCvt = MoveCVT;
	ComputeFuncs(exc);
	ComputeRound(exc, (BYTE)exc->GS.RoundStat);
	while(1) {
		exc->OpCd = exc->Code[exc->IP];
		if((exc->Length=OpcodeLength[exc->OpCd])<0) {
			if(exc->IP+1>exc->CdSize) {
				goto LErrorCodeOverflow_;
			}
			exc->Length = exc->Code[exc->IP+1]+2;
		}
		if(exc->IP+exc->Length>exc->CdSize) {
			goto LErrorCodeOverflow_;
		}
		exc->Arg = exc->Top-(PopPushCount[exc->OpCd]>>4);
		if(exc->Arg<0) {
			exc->Error = ERR_TOO_FEW_ARGUMENT;
			goto LErrorLabel_;
		}
		exc->NewTop = exc->Arg+(PopPushCount[exc->OpCd]&15);
		if(exc->NewTop>exc->StackSize) {
			exc->Error = ERR_STACK_OVERFLOW;
			goto LErrorLabel_;
		}
		exc->StepIns = TRUE;
		exc->Error = ERR_OK;
		arg = exc->Stack+exc->Arg;
		opc = exc->OpCd;
		if(opc==0x00||opc==0x01||opc==0x02||							// [SVTCA y][SVTCA x][SPvTCA y]
			opc==0x03||opc==0x04||opc==0x05) {							// [SPvTCA x][SFvTCA y][SFvTCA x]
			a = (SHORT)((opc&1)<<14);
			b = (SHORT)(a^0x4000);
			if(opc<4) {
				exc->GS.pVector.x = a;
				exc->GS.pVector.y = b;
				exc->GS.dVector.x = a;
				exc->GS.dVector.y = b;
			}
			if((opc&2)==0) {
				exc->GS.fVector.x = a;
				exc->GS.fVector.y = b;
			}
			ComputeFuncs(exc);
		} else if(opc==0x06||opc==0x07) {								// [SPvTL][SPvTL +]
			if(InsSxVTL(exc, (USHORT)arg[1], (USHORT)arg[0],
								exc->OpCd, &exc->GS.pVector)==SUCCESS) {
				exc->GS.dVector = exc->GS.pVector;
				ComputeFuncs(exc);
			}
		} else if(opc==0x08||opc==0x09) {								// [SFvTL][SFvTL +]
			if(InsSxVTL(exc, (USHORT)arg[1], (USHORT)arg[0],
								exc->OpCd, &exc->GS.fVector)==SUCCESS) {
				ComputeFuncs(exc);
			}
		} else if(opc==0x0A) {											// [SPvFS]
			s = (SHORT)arg[1];
			y = (LONG)s;
			s = (SHORT)arg[0];
			x = (LONG)s;
			Normalize(x, y, &exc->GS.pVector);
			exc->GS.dVector = exc->GS.pVector;
			ComputeFuncs(exc);
		} else if(opc==0x0B) {											// [SFvFS]
			s = (SHORT)arg[1];
			y = (LONG)s;
			s = (SHORT)arg[0];
			x = (LONG)s;
			Normalize(x, y, &exc->GS.fVector);
			ComputeFuncs(exc);
		} else if(opc==0x0C) {											// [GPV]
			arg[0] = exc->GS.pVector.x;
			arg[1] = exc->GS.pVector.y;
		} else if(opc==0x0D) {											// [GFV]
			arg[0] = exc->GS.fVector.x;
			arg[1] = exc->GS.fVector.y;
		} else if(opc==0x0E) {											// [SFvTPv]
			exc->GS.fVector = exc->GS.pVector;
			ComputeFuncs(exc);
		} else if(opc==0x0F) {											// [ISECT]
			InsISECT(exc, arg);
		} else if(opc==0x10) {											// [SRP0]
			exc->GS.Rp0 = (SHORT)arg[0];
		} else if(opc==0x11) {											// [SRP1]
			exc->GS.Rp1 = (USHORT)arg[0];
		} else if(opc==0x12) {											// [SRP2]
			exc->GS.Rp2 = (USHORT)arg[0];
		} else if(opc==0x13) {											// [SZP0]
			InsSZP0(exc, arg);
		} else if(opc==0x14) {											// [SZP1]
			InsSZP1(exc, arg);
		} else if(opc==0x15) {											// [SZP2]
			InsSZP2(exc, arg);
		} else if(opc==0x16) {											// [SZPS]
			InsSZPS(exc, arg);
		} else if(opc==0x17) {											// [SLOOP]
			if(arg[0]<0) {
				exc->Error = ERR_BAD_ARGUMENT;
			} else {
				exc->GS.Loop = arg[0];
			}
		} else if(opc==0x18) {											// [RTG]
//			exc->GS.RoundStat = ROUND_TO_GRID;
//			exc->FuncRound = (Round_Func)RoundToGrid;
		} else if(opc==0x19) {											// [RTHG]
//			exc->GS.RoundStat = ROUND_TO_HALF_GRID;
//			exc->FuncRound = (Round_Func)RoundToHalfGrid;
		} else if(opc==0x1A) {											// [SMD]
			exc->GS.MinDist = arg[0];
		} else if(opc==0x1B) {											// [ELSE]
			InsELSE(exc);
		} else if(opc==0x1C) {											// [JMPR]
			exc->IP += arg[0];
			exc->StepIns = FALSE;
		} else if(opc==0x1D) {											// [SCVTCI]
			exc->GS.CtrlValCI = (LONG)arg[0];
		} else if(opc==0x1E) {											// [SSWCI]
			exc->GS.SglWidthCI = (LONG)arg[0];
		} else if(opc==0x1F) {											// [SSW]
			exc->GS.SglWidthVal = (LONG)(arg[0]>>10);
		} else if(opc==0x20) {											// [DUP]
			arg[1] = arg[0];
		} else if(opc==0x21) {											// [POP]
			;
		} else if(opc==0x22) {											// [CLEAR]
			exc->NewTop = 0;
		} else if(opc==0x23) {											// [SWAP]
			l = arg[0];
			arg[0] = arg[1];
			arg[1] = l;
		} else if(opc==0x24) {											// [DEPTH]
			arg[0] = exc->Top;
		} else if(opc==0x25) {											// [CINDEX]
			l = arg[0];
			if(l<=0||l>exc->Arg) {
				exc->Error = ERR_INVALID_REFERENCE;
			} else {
				arg[0] = exc->Stack[exc->Arg-l];
			}
		} else if(opc==0x26) {											// [MINDEX]
			InsMINDEX(exc, arg);
		} else if(opc==0x27) {											// [ALIGNPTS]
			InsALIGNPTS(exc, arg);
		} else if(opc==0x28) {											// [????]
			InsUNKNOWN(exc);
		} else if(opc==0x29) {											// [UTP]
			InsUTP(exc, arg);
		} else if(opc==0x2A) {											// [LOOPCALL]
			InsLOOPCALL(exc, arg);
		} else if(opc==0x2B) {											// [CALL]
			InsCALL(exc, arg);
		} else if(opc==0x2C) {											// [FDEF]
			InsFDEF(exc, arg);
		} else if(opc==0x2D) {											// [ENDF]
			InsENDF(exc);
		} else if(opc==0x2E||opc==0x2F) {								// [MDAP][MDAP]
			InsMDAP(exc, arg);
		} else if(opc==0x30||opc==0x31) {								// [IUP][IUP]
			InsIUP(exc);
		} else if(opc==0x32||opc==0x33) {								// [SHP][SHP]
			InsSHP(exc);
		} else if(opc==0x34||opc==0x35) {								// [SHC][SHC]
			InsSHC(exc, arg);
		} else if(opc==0x36||opc==0x37) {								// [SHZ][SHZ]
			InsSHZ(exc, arg);
		} else if(opc==0x38) {											// [SHPIX]
			InsSHPIX(exc, arg);
		} else if(opc==0x39) {											// [IP]
			InsIP(exc);
		} else if(opc==0x3A||opc==0x3B) {								// [MSIRP][MSIRP]
			InsMSIRP(exc, arg);
		} else if(opc==0x3C) {											// [AlignRP]
			InsALIGNRP(exc);
		} else if(opc==0x3D) {											// [RTDG]
//			exc->GS.RoundStat = ROUND_TO_DOUBLE_GRID;
//			exc->FuncRound = (Round_Func)RoundToDoubleGrid;
		} else if(opc==0x3E||opc==0x3F) {								// [MIAP][MIAP]
			InsMIAP(exc, arg);
		} else if(opc==0x40) {											// [NPUSHB]
			InsNPUSHB(exc, arg);
		} else if(opc==0x41) {											// [NPUSHW]
			InsNPUSHW(exc, arg);
		} else if(opc==0x42) {											// [WS]
			i = (ULONG)arg[0];
			if((UINT)i<(UINT)exc->StrSize) {
				exc->Storage[i] = arg[1];
			}
		} else if(opc==0x43) {											// [RS]
			i = (ULONG)arg[0];
			if((UINT)i>=(UINT)exc->StrSize) {
				arg[0] = 0;
			} else {
				arg[0] = exc->Storage[i];
			}
		} else if(opc==0x44) {											// [WCVTP]
			i = (ULONG)arg[0];
			if((UINT)i<(UINT)exc->CvtSize) {
				exc->FuncWriteCvt(exc, i, arg[1]);
			}
		} else if(opc==0x45) {											// [RCVT]
			i = (ULONG)arg[0];
			if((UINT)i>=(UINT)exc->CvtSize) {
				arg[0] = 0;
			} else {
				arg[0] = exc->FuncReadCvt(exc, i);
			}
		} else if(opc==0x46||opc==0x47) {								// [GC][GC]
			InsGC(exc, arg);
		} else if(opc==0x48) {											// [SCFS]
			InsSCFS(exc, arg);
		} else if(opc==0x49||opc==0x4A) {								// [MD][MD]
			InsMD(exc, arg);
		} else if(opc==0x4B) {											// [MPPEM]
			arg[0] = CurrentPpem(exc);
		} else if(opc==0x4C) {											// [MPS]
			arg[0] = CurrentPpem(exc);
		} else if(opc==0x4D) {											// [FLIPON]
			exc->GS.AutoFlip = TRUE;
		} else if(opc==0x4E) {											// [FLIPOFF]
			exc->GS.AutoFlip = FALSE;
		} else if(opc==0x4F) {											// [DEBUG]
			exc->Error = ERR_DEBUG_OPCODE;
		} else if(opc==0x50) {											// [LT]
			arg[0] = (arg[0]<arg[1]);
		} else if(opc==0x51) {											// [LTEQ]
			arg[0] = (arg[0]<=arg[1]);
		} else if(opc==0x52) {											// [GT]
			arg[0] = (arg[0]>arg[1]);
		} else if(opc==0x53) {											// [GTEQ]
			arg[0] = (arg[0]>=arg[1]);
		} else if(opc==0x54) {											// [EQ]
			arg[0] = (arg[0]==arg[1]);
		} else if(opc==0x55) {											// [NEQ]
			arg[0] = (arg[0]!=arg[1]);
		} else if(opc==0x56) {											// [ODD]
			arg[0] = ((exc->FuncRound(exc, arg[0], 0)&127)==64);
		} else if(opc==0x57) {											// [EVEN]
			arg[0] = ((exc->FuncRound(exc, arg[0], 0)&127)==0);
		} else if(opc==0x58) {											// [IF]
			InsIF(exc, arg);
		} else if(opc==0x59) {											// [EIF]
			;
		} else if(opc==0x5A) {											// [AND]
			arg[0] = (arg[0]&&arg[1]);
		} else if(opc==0x5B) {											// [OR]
			arg[0] = (arg[0]||arg[1]);
		} else if(opc==0x5C) {											// [NOT]
			arg[0] = !arg[0];
		} else if(opc==0x5D) {											// [DELTAP1]
			InsDELTAP(exc, arg);
		} else if(opc==0x5E) {											// [SDB]
			exc->GS.DeltaBase = (SHORT)arg[0];
		} else if(opc==0x5F) {											// [SDS]
			exc->GS.DeltaShift = (SHORT)arg[0];
		} else if(opc==0x60) {											// [ADD]
			arg[0] += arg[1];
		} else if(opc==0x61) {											// [SUB]
			arg[0] -= arg[1];
		} else if(opc==0x62) {											// [DIV]
			if(arg[1]==0) {
				exc->Error = ERR_DIVIDE_BY_ZERO;
			} else {
				arg[0] = MulDiv(arg[0], 64L, arg[1]);
			}
		} else if(opc==0x63) {											// [MUL]
			arg[0] = MulDiv(arg[0], arg[1], 64L);
		} else if(opc==0x64) {											// [ABS]
			arg[0] = abs(arg[0]);
		} else if(opc==0x65) {											// [NEG]
			arg[0] = -arg[0];
		} else if(opc==0x66) {											// [FLOOR]
			arg[0] &= -64;
		} else if(opc==0x67) {											// [CEILING]
			arg[0] = (arg[0]+63)&(-64);
		} else if(opc==0x68||opc==0x69||opc==0x6A||opc==0x6B) {			// [ROUND][ROUND][ROUND][ROUND]
			arg[0] = exc->FuncRound(exc, arg[0],
								exc->TMetric.Compens[exc->OpCd-0x68]);
		} else if(opc==0x6C||opc==0x6D||opc==0x6E||opc==0x6F) {			// [NROUND][NROUND][NRRUND][NROUND]
			arg[0] = RoundNone(exc, arg[0],
								exc->TMetric.Compens[exc->OpCd-0x6C]);
		} else if(opc==0x70) {											// [WCVTF]
			i = (ULONG)arg[0];
			if((UINT)i<(UINT)exc->CvtSize) {
				exc->Cvt[i] = MulFix(arg[1], exc->TMetric.Scale);
			}
		} else if(opc==0x71||opc==0x72) {								// [DELTAP2][DELTAP3]
			InsDELTAP(exc, arg);
		} else if(opc==0x73||opc==0x74||opc==0x75) {					// [DELTAC0][DELTAC1][DELTAC2]
			InsDELTAC(exc, arg);
		} else if(opc==0x76) {											// [SROUND]
//			SetSuperRound(exc, 0x4000, arg[0]);
//			exc->GS.RoundStat = ROUND_SUPER;
//			exc->FuncRound = (Round_Func)RoundSuper;
		} else if(opc==0x77) {											// [S45Round]
//			SetSuperRound(exc, 0x2D41, arg[0]);
//			exc->GS.RoundStat = ROUND_SUPER_45;
//			exc->FuncRound = (Round_Func)RoundSuper45;
		} else if(opc==0x78) {											// [JROT]
			if(arg[1]!=0) {
				exc->IP += arg[0];
				exc->StepIns = FALSE;
			}
		} else if(opc==0x79) {											// [JROF]
			if(arg[1]==0) {
				exc->IP += arg[0];
				exc->StepIns = FALSE;
			}
		} else if(opc==0x7A) {											// [ROFF]
//			exc->GS.RoundStat = ROUND_OFF;
//			exc->FuncRound = (Round_Func)RoundNone;
		} else if(opc==0x7B) {											// [????]
			InsUNKNOWN(exc);
		} else if(opc==0x7C) {											// [RUTG]
//			exc->GS.RoundStat = ROUND_UP_TO_GRID;
//			exc->FuncRound = (Round_Func)RoundUpToGrid;
		} else if(opc==0x7D) {											// [RDTG]
//			exc->GS.RoundStat = ROUND_DOWN_TO_GRID;
//			exc->FuncRound = (Round_Func)RoundDownToGrid;
		} else if(opc==0x7E||opc==0x7F) {								// [SANGW][AA]
			;
		} else if(opc==0x80) {											// [FLIPPT]
			InsFLIPPT(exc);
		} else if(opc==0x81) {											// [FLIPRGON]
			InsFLIPRGON(exc, arg);
		} else if(opc==0x82) {											// [FLIPRGOFF]
			InsFLIPRGOFF(exc, arg);
		} else if(opc==0x83||opc==0x84) {								// [UNKNOWN][UNKNOWN]
			InsUNKNOWN(exc);
		} else if(opc==0x85) {											// [SCANCTRL]
			InsSCANCTRL(exc, arg);
		} else if(opc==0x86||opc==0x87) {								// [SDPVTL][SDPVTL]
			InsSDPVTL(exc, arg);
		} else if(opc==0x88) {											// [GETINFO]
			InsGETINFO(exc, arg);
		} else if(opc==0x89) {											// [IDEF]
			InsIDEF(exc, arg);
		} else if(opc==0x8A) {											// [ROLL]
			InsROLL(arg);
		} else if(opc==0x8B) {											// [MAX]
			if(arg[1]>arg[0]) {
				arg[0] = arg[1];
			}
		} else if(opc==0x8C) {											// [MIN]
			if(arg[1]<arg[0]) {
				arg[0] = arg[1];
			}
		} else if(opc==0x8D) {											// [SCANTYPE]
			InsSCANTYPE(exc, arg);
		} else if(opc==0x8E) {											// [INSTCTRL]
			InsINSTCTRL(exc, arg);
		} else if(opc==0x8F) {
			InsUNKNOWN(exc);
		} else {
			if(opc>=0xE0) {
				InsMIRP(exc, arg);
			} else if(opc>=0xC0) {
				InsMDRP(exc, arg);
			} else if(opc>=0xB8) {
				InsPUSHW(exc, arg);
			} else if(opc>=0xB0) {
				InsPUSHB(exc, arg);
			} else {
				InsUNKNOWN(exc);
			}
		}
		if(exc->Error!=ERR_OK) {
			if(exc->Error==ERR_INVALID_OPCODE) {						// looking for redefined instructions
				def = exc->IDef; lmt = def+exc->numIDef;
				for(; def<lmt; def++) {
					if(def->Active&&exc->OpCd==(BYTE)def->Opc) {
						if(exc->ClTop>=exc->ClSize) {
							exc->Error = ERR_INVALID_REFERENCE;
							goto LErrorLabel_;
						}
						cal = exc->ClStack+exc->ClTop;
						cal->CalRange = exc->cRange;
						cal->CalIP = exc->IP+1;
						cal->CurCount = 1;
						cal->CurRestart = def->Start;
						if(InsGotoCodeRange(exc, def->Range, def->Start)==FAILURE) {
							goto LErrorLabel_;
						}
						goto LSuiteLabel_;
					}
				}
				exc->Error = ERR_INVALID_OPCODE;
				goto LErrorLabel_;
			} else {
				goto LErrorLabel_;
			}
		}
		exc->Top = exc->NewTop;
		if(exc->StepIns) {
			exc->IP += exc->Length;
		}
		if(++cnt>1000000L) {
			return ERR_EXECUTION_TOO_LONG;
		}
	LSuiteLabel_:
		if(exc->IP>=exc->CdSize) {
			if(exc->ClTop>0) {
				exc->Error = ERR_CODE_OVERFLOW;
				goto LErrorLabel_;
			} else {
				goto LNo_Error_;
			}
		}
	}
LNo_Error_:
	return ERR_OK;
LErrorCodeOverflow_:
	exc->Error = ERR_CODE_OVERFLOW;
LErrorLabel_:
	return exc->Error;
}

/****************************************************************/
void SetCodeRange(ExecContext exc, INT rno, void* bas, LONG len)
/****************************************************************/
{
	exc->CodeRTable[rno-1].Base = (BYTE*)bas;
	exc->CodeRTable[rno-1].Size = len;
}

/*******************************************/
ExecContext NewContext(FontFileInfo *ffile)
/*******************************************/
{
	ExecContext exc=NULL;

	exc = (ExecContext)TCalloc(1, sizeof(ExecContextRec));				// 領域確保
	exc->Top = 0;
	exc->StackSize = ffile->maxStackElements;
	exc->Stack = (LONG*)TCalloc(exc->StackSize, sizeof(LONG));
	exc->Arg = 0;
	exc->NewTop = 0;
	exc->FMetric.xPpem = 0;
	exc->FMetric.yPpem = 0;
	exc->FMetric.xScale = 0;
	exc->FMetric.yScale = 0;
	exc->TMetric.Ppem = 0;
	exc->TMetric.Ratio = 0x10000L;
	exc->TMetric.Scale = 0;
	exc->GS.Rp0 = 0;
	exc->GS.Rp1 = 0;
	exc->GS.Rp2 = 0;
	exc->GS.dVector.x = 0x4000;
	exc->GS.dVector.y = 0x0000;
	exc->GS.pVector = exc->GS.dVector;
	exc->GS.fVector = exc->GS.dVector;
	exc->GS.Loop = 1;
	exc->GS.MinDist = 64;
	exc->GS.RoundStat = ROUND_OFF;
	exc->GS.AutoFlip = TRUE;
	exc->GS.CtrlValCI = 68;
	exc->GS.SglWidthCI = 1;
	exc->GS.SglWidthVal = 24;
	exc->GS.DeltaBase = 9;
	exc->GS.DeltaShift = 3;
	exc->GS.InstCtrl = 0;
	exc->GS.ScanCtrl = FALSE;
	exc->GS.ScanType = 2;
	exc->GS.Gep0 = 1;
	exc->GS.Gep1 = 1;
	exc->GS.Gep2 = 1;
	exc->CvtSize = ffile->cvt_length;
	exc->Cvt = ffile->cvt_table;
	exc->GlyphIns =(BYTE*)TCalloc(ffile->maxSizeOfInstructions, sizeof(BYTE));
	exc->numFDef = 0;
	exc->maxFDef = ffile->maxFunctionDefs;
	exc->FDef = (DefArray)TCalloc(exc->maxFDef, sizeof(DefRecord));
	exc->numIDef = 0;
	exc->maxIDef = ffile->maxInstructionDefs;
	exc->IDef = (DefArray)TCalloc(exc->maxIDef, sizeof(DefRecord));
	exc->ClTop = 0;
	exc->ClSize = 32;
	exc->ClStack = (CalArray)TCalloc(exc->ClSize, sizeof(CalRecord));
	exc->StrSize = ffile->maxStorage;
	exc->Storage = (LONG*)TCalloc(exc->StrSize, sizeof(LONG));
	exc->Period = 64;
	exc->Phase = 0;
	exc->Thrhld = 0;
	exc->FdotP = 0x10000L;
	SetCodeRange(exc, 1, ffile->fpgm_table, ffile->fpgm_length);
	if(GotoCodeRange(exc, 1, 0)==SUCCESS) {
		RunIns(exc);
	}
	return exc;
}

/*******************************/
INT RunContext(ExecContext exc)
/*******************************/
{
	INT err;

	err = GotoCodeRange(exc, 2, 0);
	if(err!=SUCCESS) {
		return err;
	}
	exc->Zp0 = exc->Pts;
	exc->Zp1 = exc->Pts;
	exc->Zp2 = exc->Pts;
	exc->GS.Gep0 = 1;
	exc->GS.Gep1 = 1;
	exc->GS.Gep2 = 1;
	exc->GS.pVector.x = 0x4000;
	exc->GS.pVector.y = 0x0000;
	exc->GS.fVector = exc->GS.pVector;
	exc->GS.dVector = exc->GS.pVector;
	exc->GS.RoundStat = ROUND_OFF;		//ROUND_TO_GRID;
	exc->GS.Loop = 1;
	exc->Top = 0;
	exc->ClTop = 0;
	return RunIns(exc);
}

/********************************/
void DelContext(ExecContext exc)
/********************************/
{
	if(exc==NULL) {
		return;
	}
	if(exc->Storage!=NULL) {
		TFree(exc->Storage);
	}
	if(exc->ClStack!=NULL) {
		TFree(exc->ClStack);
	}
	if(exc->IDef!=NULL) {
		TFree(exc->IDef);
	}
	if(exc->FDef!=NULL) {
		TFree(exc->FDef);
	}
	if(exc->GlyphIns!=NULL) {
		TFree(exc->GlyphIns);
	}
	if(exc->Stack!=NULL) {
		TFree(exc->Stack);
	}
	TFree(exc);
}
