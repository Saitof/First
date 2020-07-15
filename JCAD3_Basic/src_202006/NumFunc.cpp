/////////////////////////////////////////////////////////////////////////////
// 数値関数部
// NumFunc.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/12/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "DataAccess2.h"

/***********************/
double VecL(VECTYPE* v)													//<<< ﾍﾞｸﾄﾙ長
/***********************/
{
	int    i;
	double ln=0.0;

	for(i=0; i<DIM; i++) {
		ln += v->p[i]*v->p[i];
	}
	return sqrt(ln);
}

/***********************************************/
void VecO(VECTYPE* v1, VECTYPE* v2, VECTYPE* v)							//<<< 3Dﾍﾞｸﾄﾙ外積
/***********************************************/
{
	v->p[0] = v1->p[1]*v2->p[2]-v1->p[2]*v2->p[1];
	v->p[1] = v1->p[2]*v2->p[0]-v1->p[0]*v2->p[2];
	v->p[2] = v1->p[0]*v2->p[1]-v1->p[1]*v2->p[0];
}

/*********************/
void VecN(VECTYPE* v)													//<<< 単位ﾍﾞｸﾄﾙ化(3D)
/*********************/
{
	double vl;
	int    i;

	vl = VecL(v);
	if(vl>EPSILON) {
		for(i=0; i<DIM; i++) {
			v->p[i] = v->p[i]/vl;
		}
	}
}

/***********************************************/
void VecV(PNTTYPE* vs, PNTTYPE* ve, VECTYPE* v)							//<<< 始点->終点ﾍﾞｸﾄﾙ
/***********************************************/
{
	int i;

	for(i=0; i<DIM; i++) {
		v->p[i] = ve->p[i] - vs->p[i];
	}
}

/*************************************/
double PntD(PNTTYPE* v1, PNTTYPE* v2)									//<<< 2点間距離(3D)
/*************************************/
{
	int    i;
	double dd, ln=0.0;

	for(i=0; i<DIM; i++) {
		dd = v1->p[i] - v2->p[i];
		ln += dd*dd;
	}
	return sqrt(ln);
}

/*********************************************/
double CalcArea(double a, double b, double c)							//<<< 三角形面積算出
/*********************************************/
{
	double s, ret=0.0;

	s = (a+b+c)/2.0;													// ﾍﾛﾝの公式により
	ret = sqrt(s*(s-a)*(s-b)*(s-c));									// 面積を算出
	return ret;
}

/**************************************************************/
void SetRotateMatrix(VECTYPE axis, double th, double rot[][3])			//<<< 回転ﾏﾄﾘｯｸｽｾｯﾄ
/**************************************************************/
{
	double cx, sx, dx, r1, r2, r3, r12, r13, r23;

	cx = cos(th),   sx = sin(th),   dx = 1.0 - cx;
	r1 = axis.p[0], r2 = axis.p[1], r3 = axis.p[2];
	r12 = r1*r2,    r13 = r1*r3,    r23 = r2*r3;
	rot[0][0] = cx+dx*r1*r1;
	rot[0][1] = -sx*r3+dx*r12;
	rot[0][2] = sx*r2+dx*r13;
	rot[1][0] = sx*r3+dx*r12;
	rot[1][1] = cx+dx*r2*r2;
	rot[1][2] = -sx*r1+dx*r23;
	rot[2][0] = -sx*r2+dx*r13;
	rot[2][1] = sx*r1+dx*r23;
	rot[2][2] = cx+dx*r3*r3;
}

/*********************************************/
void VecRotate(PNTTYPE* pos, double rot[][3])							//<<< 頂点座標回転
/*********************************************/
{
	PNTTYPE pnt;
	int     i, j;
	double  wk;

	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += rot[i][j]*pos->p[j];
		}
		pnt.p[i] = wk;
	}
	for(i=0; i<DIM; i++) {
		pos->p[i] = pnt.p[i];
	}
}

/**************************************************************/
void VecRotateMove(PNTTYPE* pos, double rot[][3], PNTTYPE dis)			//<<< 頂点座標回転&平行移動
/**************************************************************/
{
	int i;

	VecRotate(pos, rot);
	for(i=0; i<DIM; i++) {
		pos->p[i] += dis.p[i];
	}
}

/******************************************************/
void Affine(OBJTYPE* op, PNTTYPE cp, double mat[3][3])					//<<< 頂点座標ｱﾌｨﾝ変換
/******************************************************/
{
	PNTTYPE dis, pnt;
	int     i, j, vnum;
	double  wk;

	for(i=0; i<DIM; i++) {												// 移動量計算
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += mat[i][j]*cp.p[j];
		}
		dis.p[i] = cp.p[i]-wk;
	}
	vnum = GetVtxNum(op);												// 頂点座標数取得
	for(i=0; i<vnum; i++) {
		GetVtx(op, i, &pnt);											// 頂点座標取得
		VecRotateMove(&pnt, mat, dis);									// 頂点座標を回転移動
		PutVtx(op, i, pnt);												// 頂点座標更新
	}
}

/*****************************************************************/
void AffineVtx(int pn, PNTTYPE* pa, PNTTYPE cp, double mat[3][3])		//<<< 頂点座標ｱﾌｨﾝ変換
/*****************************************************************/
{
	PNTTYPE dis;
	int     i, j;
	double  wk;

	for(i=0; i<DIM; i++) {												// 移動量計算
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += mat[i][j]*cp.p[j];
		}
		dis.p[i] = cp.p[i]-wk;
	}
	for(i=0; i<pn; i++) {
		VecRotateMove(&pa[i], mat, dis);								// 頂点座標を回転移動
	}
}

/***********************************************/
void CalcCent(PNTTYPE* pa, int pn, PNTTYPE* cp)							//<<< 図形重心取得
/***********************************************/
{
	int    i, j;
	double tg;

	for(i=0; i<DIM; i++) {
		for(tg=0.0, j=0; j<pn; j++) {
			tg += pa[j].p[i];
		}
		cp->p[i] = tg/pn;
	}
}

/***********************************************/
void CalcBox(PNTTYPE* pa, int pn, BOXTYPE* box)							//<<< 図形BOXｻｲｽﾞ取得
/***********************************************/
{
	PNTTYPE mip, map;
	int     i, j;

	mip = pa[0], map = pa[0];
	for(i=1; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mip.p[j] = (mip.p[j]>pa[i].p[j]) ? pa[i].p[j] : mip.p[j];	// 最小値検索
			map.p[j] = (map.p[j]<pa[i].p[j]) ? pa[i].p[j] : map.p[j];	// 最大値検索
		}
	}
	box->bp[0] = mip, box->bp[1] = map;
}

/************************************************/
void CalcNVec(PNTTYPE* pa, int pn, VECTYPE* vec)						//<<< 図形法線ﾍﾞｸﾄﾙ取得
/************************************************/
{
	PNTTYPE p1, p2;
	VECTYPE v;
	int     i, j, j1, j2;
	double  d1, d2;

	v.p[0] = 0.0, v.p[1] = 0.0, v.p[2] = 0.0;
	for(i=0; i<pn; i++) {
		p1 = pa[i];
		p2 = (i==pn-1) ? pa[0] : pa[i+1];
		for(j=0; j<DIM; j++) {
			j1 = (j==2) ? 0 : j+1;
			j2 = (j1==2) ? 0 : j1+1;
			d1 = p1.p[j1] - p2.p[j1];
			d2 = p1.p[j2] + p2.p[j2];
			v.p[j] += d1*d2;
		}
	}
	VecN(&v);															// 単位ﾍﾞｸﾄﾙ化
	*vec = v;
}

/***********************************************************/
void CalcRotAngleVec(PNTTYPE* pa, double* th, VECTYPE* vec)				//<<< 図形法線ﾍﾞｸﾄﾙ&角度取得
/***********************************************************/
{
	VECTYPE svc, rvc;
	double  dt;

	VecV(&pa[0], &pa[1], &svc);											// 1->2点間ﾍﾞｸﾄﾙ取得
	VecV(&pa[1], &pa[2], &rvc);											// 2->3点間ﾍﾞｸﾄﾙ取得
	VecN(&svc);															// 単位ﾍﾞｸﾄﾙ化
	VecN(&rvc);															// 単位ﾍﾞｸﾄﾙ化
	if(fabs(svc.p[0]-rvc.p[0])>EPSILON||
       fabs(svc.p[1]-rvc.p[1])>EPSILON||
       fabs(svc.p[2]-rvc.p[2])>EPSILON) {								// 2ﾍﾞｸﾄﾙ間の角度
		dt = svc.p[0]*rvc.p[0]+svc.p[1]*rvc.p[1]+svc.p[2]*rvc.p[2];
		dt = (dt>1.0) ? 1.0 : ((dt<-1.0) ? -1.0 : dt);
		*th = acos(dt);
		*th = (*th<-10||*th>10) ? 0.0 : *th;
	} else {
		*th = 0.0;
	}
	if(*th>EPSILON) {
		CalcNVec(pa, 3, vec);											// 2ﾍﾞｸﾄﾙ間の法線ﾍﾞｸﾄﾙ
	} else {
		vec->p[0] = 0.0, vec->p[1] = 0.0, vec->p[2] = 0.0;
	}
}

/********************************/
double CalcRotAngle(PNTTYPE* pa)										//<<< 図形角度取得
/********************************/
{
	VECTYPE svc, rvc;
	double  th=0.0, dt;

	VecV(&pa[0], &pa[1], &svc);											// 1->2点間ﾍﾞｸﾄﾙ取得
	VecV(&pa[1], &pa[2], &rvc);											// 2->3点間ﾍﾞｸﾄﾙ取得
	VecN(&svc);															// 単位ﾍﾞｸﾄﾙ化
	VecN(&rvc);															// 単位ﾍﾞｸﾄﾙ化
	if(fabs(svc.p[0]-rvc.p[0])>EPSILON||
       fabs(svc.p[1]-rvc.p[1])>EPSILON||
       fabs(svc.p[2]-rvc.p[2])>EPSILON) {								// 2ﾍﾞｸﾄﾙ間の角度
		dt = svc.p[0]*rvc.p[0]+svc.p[1]*rvc.p[1]+svc.p[2]*rvc.p[2];
		dt = (dt>1.0) ? 1.0 : ((dt<-1.0) ? -1.0 : dt);
		th = acos(dt);
		th = (th<-10||th>10) ? 0.0 : th;
	}
	return th;
}

/*********************************************/
double CalcAngle(PNTTYPE* pa, int ix, int pm)							//<<< 3点のなす角度取得
/*********************************************/
{
	VECTYPE sv, rv, vc;
	double  th=0.0, dt;

	VecV(&pa[0], &pa[1], &sv);											// 1->2点間ﾍﾞｸﾄﾙ取得
	VecN(&sv);															// 単位ﾍﾞｸﾄﾙ化
	VecV(&pa[1], &pa[2], &rv);											// 2->3点間ﾍﾞｸﾄﾙ取得
	VecN(&rv);															// 単位ﾍﾞｸﾄﾙ化
	if(fabs(sv.p[0]-rv.p[0])>EPSILON||
       fabs(sv.p[1]-rv.p[1])>EPSILON||
       fabs(sv.p[2]-rv.p[2])>EPSILON) {
		dt = sv.p[0]*rv.p[0]+sv.p[1]*rv.p[1]+sv.p[2]*rv.p[2];
		dt = (dt>1.0) ? 1.0 : ((dt<-1.0) ? -1.0 : dt);
		th = acos(dt);													// 2ﾍﾞｸﾄﾙ間の角度
		th = (th<-10||th>10) ? 0.0 : th;
	} else {
		th = 0.0;
	}
	if(th>EPSILON) {
		CalcNVec(pa, 3, &vc);											// 2ﾍﾞｸﾄﾙ間の法線ﾍﾞｸﾄﾙ
	} else {
		vc.p[0] = 0.0, vc.p[1] = 0.0, vc.p[2] = 0.0;
	}
	th = (pm==0&&vc.p[ix]<0.0) ? -th : ((pm==1&&vc.p[ix]>0.0)?-th:th);	// 法線ﾍﾞｸﾄﾙと逆向きはﾏｲﾅｽ
	return th;
}

/*********************************************************/
double CalcBaseAngle(PNTTYPE pt1, PNTTYPE pt2, int plane)				//<<< 基準線角度取得
/*********************************************************/
{
	int    i1, i2;
	double d1, d2, th;

	i1 = plane;															// 座標軸設定
	i2 = (plane+1>2) ? 0 : plane+1;
	d1 = pt2.p[i1] - pt1.p[i1];
	d2 = pt2.p[i2] - pt1.p[i2];
	th = atan2(d2, d1);													// 基準線角度
	th = (th<-10||th>10) ? 0.0 : th;
	return th;
}

/******************************************************/
void CalcLinePoint(int plane, int flg, double a,
				   double b, PNTTYPE* ip, PNTTYPE* op)					//<<< 線分交点算出
/******************************************************/
{
	int    i1, i2;
	double c;

	i1 = plane;															// 座標軸設定
	i2 = (i1+1>2) ? 0 : i1+1;
	if(flg==1) {														// <i1軸平行線分>
		op->p[i1] = a;													// i1軸座標
		op->p[i2] = ip->p[i2];											// i2軸座標
	} else if(flg==2) {													// <i2軸平行線分>
		op->p[i1] = ip->p[i1];											// i1軸座標
		op->p[i2] = b;													// i2軸座標
	} else {															// <i1,i2軸非平行線分>
		c = ip->p[i1]/a+ip->p[i2];										// 指定点を通る垂線定数
		op->p[i1] = (c-b)*a/(a*a+1);									// i1軸座標
		op->p[i2] = a*(c-b)*a/(a*a+1)+b;								// i2軸座標
	}
}

/***********************************************************/
double DistPointLine(PNTTYPE* sp, PNTTYPE* ep, PNTTYPE* pt,
					 int* fg, double* sl, PNTTYPE* op)					//<<< 点<->線分最短距離取得
/***********************************************************/
{
	int    i;
	double a, b, c, c1, c2, d, s, ln;

	a = PntD(sp, pt);													// 点<->始点距離
	b = PntD(ep, pt);													// 点<->終点距離
	c = PntD(sp, ep);													// 始点<->終点距離
	d = (a+b+c)/2;
	s = sqrt(d*(d-a)*(d-b)*(d-c));										// 三角形面積
	if(s<EPSILON) {														// <面積=0の場合>
		if(fabs(a+c-b)<EPSILON) {
			ln = a, *fg = 1, *sl = 0, *op = *sp;
		} else if(fabs(b+c-a)<EPSILON) {
			ln = b, *fg = 2, *sl = c, *op = *ep;
		} else {
			ln = DBL_MAX, *fg = -1, *sl = 0, *op = *sp;
		}
	} else {															// <面積>0の場合>
		ln = 2*s/c;														// 点からの垂線長(高さ)
		c1 = (a*a-ln*ln<0) ? 0 : a*a-ln*ln;								// 垂線で分割される底辺1
		c2 = (b*b-ln*ln<0) ? 0 : b*b-ln*ln;								// 垂線で分割される底辺2
		c1 = sqrt(c1), c2 = sqrt(c2);
		if(c<c1||c<c2) {												// <線分<底辺合計>
			if(c1<c2) {													// 底辺短採用
				ln = a, *fg = 1, *sl = 0, *op = *sp;
			} else {
				ln = b, *fg = 2, *sl = c, *op = *ep;
			}
		} else {
			for(i=0; i<DIM; i++) {
				a = (ep->p[i]-sp->p[i])*c1/c;							// 垂線交点位置取得
				op->p[i] = sp->p[i]+a;
			}
			*fg = 0, *sl = c1;
		}
	}
	return ln;
}

/*******************************************/
double BoxCenter(BOXTYPE box, PNTTYPE* pnt)								//<<< ﾎﾞｯｸｽｾﾝﾀｰ
/*******************************************/
{
	double x, y, z, ln=0;

	x = box.bp[1].p[0]-box.bp[0].p[0];									// X方向の長さ
	y = box.bp[1].p[1]-box.bp[0].p[1];									// Y方向の長さ
	z = box.bp[1].p[2]-box.bp[0].p[2];									// Z方向の長さ
	pnt->p[0] = box.bp[0].p[0]+x/2.0;									// 中心点算出
	pnt->p[1] = box.bp[0].p[1]+y/2.0;
	pnt->p[2] = box.bp[0].p[2]+z/2.0;
	ln = (x<y) ? y : x;													// 最大長さ算出
	ln = (ln<z) ? z : ln;
	return ln;
}

/**************************************************/
BOOL BoxCheck(int plane, BOXTYPE box, PNTTYPE pnt)						//<<< ﾎﾞｯｸｽ内存在確認
/**************************************************/
{
	int i1, i2;

	i1 = plane;															// 座標軸設定
	i2 = (plane+1>2) ? 0 : plane+1;
	if(pnt.p[i1]<box.bp[0].p[i1]) {
		return FALSE;
	}
	if(pnt.p[i1]>box.bp[1].p[i1]) {
		return FALSE;
	}
	if(pnt.p[i2]<box.bp[0].p[i2]) {
		return FALSE;
	}
	if(pnt.p[i2]>box.bp[1].p[i2]) {
		return FALSE;
	}
	return TRUE;
}

/**********************************************************************/
void GetTensionFltValue(int mode, double aln, double bln, double* dat)	//<<< 張力設定値変換(実数)
/**********************************************************************/
{
	double dd;

	if(mode==0) {														// <ﾓｰﾄﾞ:0>
		*dat = 0.0;
	} else if(mode==1) {												// <ﾓｰﾄﾞ:1>
		dd = bln/aln*3.0;												// ﾊﾟｰｾﾝﾄ算出
		dd = (4.0/(dd+1.0))-1.0;										// 張力係数計算
		*dat = (*dat)*dd/3.0;											// 張力計算後数値
	} else if(mode==2) {												// <ﾓｰﾄﾞ:2>
		dd = (aln-bln)/aln;												// ﾊﾟｰｾﾝﾄ算出
		*dat = (*dat)*dd;												// 張力計算後数値
	} else if(mode==3) {												// <ﾓｰﾄﾞ:3>
		dd = (aln-bln)/aln*3.0;											// ﾊﾟｰｾﾝﾄ算出
		dd = 4.0-(4.0/(dd+1.0));										// 張力係数計算
		*dat = (*dat)*dd/3.0;											// 張力計算後数値
	}
}

/***********************************************************************/
void GetTensionPntValue(int mode, double aln, double bln, PNTTYPE* dat)	//<<< 張力設定値変換(頂点)
/***********************************************************************/
{
	double dd;

	if(mode==0) {														// <ﾓｰﾄﾞ:0>
		dat->p[0] = 0.0;
		dat->p[1] = 0.0;
		dat->p[2] = 0.0;
	} else if(mode==1) {												// <ﾓｰﾄﾞ:1>
		dd = bln/aln*3.0;												// ﾊﾟｰｾﾝﾄ算出
		dd = (4.0/(dd+1.0))-1.0;										// 張力係数計算
		dat->p[0] = dat->p[0]*dd/3.0;									// 張力計算後数値
		dat->p[1] = dat->p[1]*dd/3.0;
		dat->p[2] = dat->p[2]*dd/3.0;
	} else if(mode==2) {												// <ﾓｰﾄﾞ:2>
		dd = (aln-bln)/aln;												// ﾊﾟｰｾﾝﾄ算出
		dat->p[0] = dat->p[0]*dd;										// 張力計算後数値
		dat->p[1] = dat->p[1]*dd;
		dat->p[2] = dat->p[2]*dd;
	} else if(mode==3) {												// <ﾓｰﾄﾞ:3>
		dd = (aln-bln)/aln*3.0;											// ﾊﾟｰｾﾝﾄ算出
		dd = 4.0-(4.0/(dd+1.0));										// 張力係数計算
		dat->p[0] = dat->p[0]*dd/3.0;									// 張力計算後数値
		dat->p[1] = dat->p[1]*dd/3.0;
		dat->p[2] = dat->p[2]*dd/3.0;
	}
}

/*********************************************/
BOOL CheckNextPoint(PNTTYPE pt1, PNTTYPE pt2)							//<<< 次点有効ﾁｪｯｸ
/*********************************************/
{
	if(fabs(pt1.p[0]-pt2.p[0])>0.099999||
       fabs(pt1.p[1]-pt2.p[1])>0.099999||
       fabs(pt1.p[2]-pt2.p[2])>0.099999) {								// 2点間の距離
		return TRUE;													// 1軸が0.1㎜以上でOK
	} else {
		return FALSE;
	}
}

/**************************************************/
void ScrToPnt(int flg, SCRTYPE* scr, PNTTYPE* pnt)						//<<< SCRTYPE<=>PNTTYPE変換
/**************************************************/
{
	int i;

	if(flg==0) {														// <SCRTYPE=>PNTTYPE>
		for(i=0; i<DIM; i++) {
			pnt->p[i] = (double)scr->p[i]/1000.0;
		}
	} else {															// <PNTTYPE=>SCRTYPE>
		for(i=0; i<DIM; i++) {
  			scr->p[i] = (int)(pnt->p[i]*1000.0+(pnt->p[i]<0?-0.5:0.5));
		}
	}
}

/*********************************************/
double CalcVecAngle(VECTYPE* v1, VECTYPE* v2)							//<<< 2ﾍﾞｸﾄﾙ間角度算出
/*********************************************/
{
	double th=0.0;

	if(fabs(v1->p[0]-v2->p[0])>EPSILON||
       fabs(v1->p[1]-v2->p[1])>EPSILON||
       fabs(v1->p[2]-v2->p[2])>EPSILON) {
		th = v1->p[0]*v2->p[0]+v1->p[1]*v2->p[1]+v1->p[2]*v2->p[2];
		th = (th>1.0) ? 1.0 : ((th<-1.0) ? -1.0 : th);
		th = acos(th);													// 2ﾍﾞｸﾄﾙ間角度
		th = (th<-10||th>10) ? 0.0 : th;
	}
	return th;
}
