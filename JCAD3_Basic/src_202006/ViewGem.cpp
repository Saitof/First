/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewGem.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "DataFile1.h"
#include "Select.h"
#include "NumFunc.h"

/************************************************/
static BOOL InputGemSize(double* xd, double* hd)                        //<<< 石の直径,高さ入力
/************************************************/
{
	CInputDlg dlg(8);
	dlg.SetValue(*xd, *hd);												// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 数値入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(xd, hd);											// ｻｲｽﾞ取得
		return TRUE;
	} else {
		return FALSE;
	}
}

/************************************************************/
static BOOL InputGemSize(double* xd, double* yd, double* hd)            //<<< 石のX,Y,高さ入力
/************************************************************/
{
	CInputDlg dlg(7);
	dlg.SetValue(*xd, *yd, *hd);										// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 数値入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(xd, yd, hd);										// ｻｲｽﾞ取得
		return TRUE;
	} else {
		return FALSE;
	}
}

/****************************************************/
static void SetBrilliantVtx(PNTTYPE* pa, double xd,
                            double hd, PNTTYPE cent)                    //<<< ﾌﾞﾘﾘｱﾝﾄ頂点設定
/****************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, d3, d4, z0, z1, z2, z3, z4;

	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		z0 = -xd*0.431, z1 = -xd*0.33, z2 = 0.0;
		z4 = xd*0.146, z3 = z4*2.0/3.0;
	} else {															// 高さ指定有の場合
		z0 = -hd*0.431/0.577, z1 = -hd*0.33/0.577, z2 = 0.0;
		z4 = hd*0.146/0.577, z3 = z4*2.0/3.0;
	}
	dth = PI/8.0;
	d2 = xd/2.0;
	d1 = (d2-d2*z1/z0)/cos(dth);
	d4 = xd*0.575/2.0;
	d3 = ((d2-d4)/3.0+d4)/cos(dth);
	pa[vn].p[0] = 0, pa[vn].p[1] = 0, pa[vn++].p[2] = z0;				// 頂点の設定
	for(th=(2.0*PI)+dth, i=0; i<16; th-=dth, i++) {
		sp = sin(th), cp = cos(th);
		if(i%2) {
			pa[vn].p[0] = d4*cp, pa[vn].p[1] = d4*sp, pa[vn++].p[2] = z4;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
		} else {
			pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
			pa[vn].p[0] = d3*cp, pa[vn].p[1] = d3*sp, pa[vn++].p[2] = z3;
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/*************************************************************/
static void SetMarquiseVtx(PNTTYPE* pa, double xd, double yd,
                           double hd, PNTTYPE cent)                     //<<< ﾏｰｷｰｽﾞ頂点設定
/*************************************************************/
{
	PNTTYPE pt, p0, p1;
	VECTYPE vc;
	int     i, j, sw, vn=0;
	double  dx, dy, l0, l1, l2, l3, th, t0, t1, z0, z1, z3, z4, dd;

	if(xd>yd) {
		dx = yd, dy = xd, sw = 1;
	} else {
		dx = xd, dy = yd, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		z0 = -dx*0.431, z1 = -dx*0.33, z4 = dx*0.146, z3 = z4*2.0/3.0;
	} else {															// 高さ指定有の場合
		z0 = -hd*0.431/0.577, z1 = -hd*0.33/0.577;
		z4 = hd*0.146/0.577, z3 = z4*2.0/3.0;
	}
	for(i=0; i<DIM; i++) {
		p0.p[i] = 0.0, p1.p[i] = 0.0;									// 基準点初期化
	}
	l0 = sqrt(dx*dx+dy*dy)/2.0;											// 基準点算出計算
	t0 = acos(dx/(l0*2.0));
	t0 = (t0<-10||t0>10) ? 0.0 : t0;
	t1 = PI-t0*2.0;
	l0 = l0/(cos(t0)*2.0);
	l1 = dx/2.0-((dx/2.0-dx*0.575/2.0)/3.0+dx*0.575/2.0)/cos(PI/8.0);
	l2 = dx/2.0-dx*0.575/2.0;
	p1.p[0] = l0-dx/2.0;												// 基準点設定
	t0 = t1/4.0;
	pa[vn] = p0, pa[vn++].p[2] = z0;									// 頂点の設定
	for(th=PI+t1-t0, i=0; i<8; th-=t0, i++) {
		pt.p[0] = l0*cos(th)+p1.p[0], pt.p[1] = l0*sin(th), pt.p[2] = 0.0;
		if(i!=7) {
			if(i%2) {
				VecV(&pt, &p1, &vc);									// ﾍﾞｸﾄﾙ取得
				VecN(&vc);												// 単位ﾍﾞｸﾄﾙ化
				pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
				pa[vn++].p[2] = z4;
				pa[vn++] = pt;
			} else {
				VecV(&pt, &pa[0], &vc);									// ﾍﾞｸﾄﾙ取得
				VecN(&vc);												// 単位ﾍﾞｸﾄﾙ化
				l3 = PntD(&pt, &pa[0]);									// 2点間の距離
				l3 = l3*z1/z0;											// 有効距離
				pa[vn].p[0] = pt.p[0]+l3*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l3*vc.p[1];
				pa[vn++].p[2] = pt.p[2]+l3*vc.p[2];
				pa[vn++] = pt;
				VecV(&pt, &p1, &vc);									// ﾍﾞｸﾄﾙ取得
				VecN(&vc);												// 単位ﾍﾞｸﾄﾙ化
				pa[vn].p[0] = pt.p[0]+l1*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l1*vc.p[1];
				pa[vn++].p[2] = z3;
			}
		} else {
			VecV(&pt, &p0, &vc);										// ﾍﾞｸﾄﾙ取得
			VecN(&vc);													// 単位ﾍﾞｸﾄﾙ化
			l2 *= dy/dx;
			pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
			pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
			pa[vn++].p[2] = z4;
			pa[vn++] = pt;
		}
	}
	for(i=1; i<21; i++) {
		pa[i+20].p[0] = -pa[i].p[0];									// 180度反転複写
		pa[i+20].p[1] = -pa[i].p[1];
		pa[i+20].p[2] = pa[i].p[2];
	}
	if(sw) {															// <XYを入れ替えた場合>
		for(i=0; i<41; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90度回転
		}
	}
	for(i=0; i<41; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/**************************************************************/
static void SetPearShapeVtx(PNTTYPE* pa, double xd, double yd,
                            double hd, PNTTYPE cent)                    //<<< ﾍﾟｱｼｪｰﾌﾟ頂点設定
/**************************************************************/
{
	PNTTYPE pt, p0, p1;
	VECTYPE vc;
	int     i, j, sw, vn=0;
	double  dx, dy, md, l0, l1, l2, l3, th, t0, t1, sp, cp, d1, d2, d3, d4, z0, z1, z2, z3, z4;

	if(xd>yd) {
		dx = yd, dy = xd, sw = 1;
	} else {
		dx = xd, dy = yd, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		z0 = -dx*0.431, z1 = -dx*0.33, z2 = 0.0;
		z4 = dx*0.146, z3 = z4*2.0/3.0;
	} else {															// 高さ指定有の場合
		z0 = -hd*0.431/0.577, z1 = -hd*0.33/0.577, z2 = 0.0;
		z4 = hd*0.146/0.577, z3 = z4*2.0/3.0;
	}
	md = (dy-dx)/2.0;
	dy = (dy-dx/2.0)*2.0;
	d2 = dx/2.0;
	d1 = (d2-d2*z1/z0)/cos(PI/8.0);
	d4 = dx*0.575/2.0;
	d3 = ((d2-d4)/3.0+d4)/cos(PI/8.0);
	for(i=0; i<DIM; i++) {
		p0.p[i] = 0.0, p1.p[i] = 0.0;									// 基準点初期化
	}
	l0 = sqrt(dx*dx+dy*dy)/2.0;											// 基準点算出計算
	t0 = acos(dx/(l0*2.0));
	t0 = (t0<-10||t0>10) ? 0.0 : t0;
	t1 = PI-(t0*2.0);
	l0 = l0/(cos(t0)*2.0);
	l1 = d2-d3, l2 = d2-d4;
	p1.p[0] = l0-dx/2.0;												// 基準点設定
	t0 = t1/4.0;
	pa[vn] = p0, pa[vn++].p[2] = z0;									// 頂点の設定
	for(th=PI+t1-t0, i=0; i<8; th-=t0, i++) {
		pt.p[0] = l0*cos(th)+p1.p[0], pt.p[1] = l0*sin(th), pt.p[2] = 0;
		if(i!=7) {
			if(i%2) {
				VecV(&pt, &p1, &vc);									// ﾍﾞｸﾄﾙ取得
				VecN(&vc);												// 単位ﾍﾞｸﾄﾙ化
				pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
				pa[vn++].p[2] = z4;
				pa[vn++] = pt;
			} else {
				VecV(&pt, &pa[0], &vc);									// ﾍﾞｸﾄﾙ取得
				VecN(&vc);												// 単位ﾍﾞｸﾄﾙ化
				l3 = PntD(&pt, &pa[0]);									// 2点間の距離
				l3 = l3*z1/z0;											// 有効距離
				pa[vn].p[0] = pt.p[0]+l3*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l3*vc.p[1];
				pa[vn++].p[2] = pt.p[2]+l3*vc.p[2];
				pa[vn++] = pt;
				VecV(&pt, &p1, &vc);									// ﾍﾞｸﾄﾙ取得
				VecN(&vc);												// 単位ﾍﾞｸﾄﾙ化
				pa[vn].p[0] = pt.p[0]+l1*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l1*vc.p[1];
				pa[vn++].p[2] = z3;
			}
		} else {
			VecV(&pt, &p0, &vc);										// ﾍﾞｸﾄﾙ取得
			VecN(&vc);													// 単位ﾍﾞｸﾄﾙ化
			l2 *= dy/dx;
			pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
			pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
			pa[vn++].p[2] = z4;
			pa[vn++] = pt;
		}
	}
	for(i=1; i<21; i++) {
		pa[i+20].p[0] = -pa[i].p[0];									// 180度反転複写
		pa[i+20].p[1] = -pa[i].p[1];
		pa[i+20].p[2] = pa[i].p[2];
	}
	t0 = PI/8.0;
	for(th=PI, vn=9, i=0; i<9; th-=t0, i++) {							// 半分をﾗｳﾝﾄﾞに変形
		sp = sin(th), cp = cos(th);
		if(i%2) {
			pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
			pa[vn].p[0] = d3*cp, pa[vn].p[1] = d3*sp, pa[vn++].p[2] = z3;
		} else {
			pa[vn].p[0] = d4*cp, pa[vn].p[1] = d4*sp, pa[vn++].p[2] = z4;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
		}
	}
	for(i=0; i<41; i++) {
		pa[i].p[1] += md;												// 立体の中心を原点にする
	}
	if(sw) {															// <XYを入れ替えた場合>
		for(i=0; i<41; i++) {
			md = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -md;	// 90度回転
		}
	}
	for(i=0; i<41; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/***************************************************************/
static void SetHeartShapeVtx(PNTTYPE* pa, double xd, double yd,
                             double hd, PNTTYPE cent)                   //<<< ﾊｰﾄｼｪｰﾌﾟ頂点設定
/***************************************************************/
{
	PNTTYPE cp;
	int     i, j;
	double  minx, miny, minz, maxx, maxy, maxz;
	double  dx, dy, dh, gx, gy, gz, mat[3][3], sx, sy, sz;

	dx = xd, dy = yd;													// 指定X/Y長さ
	dh = (dx<dy) ? dx : dy;												// 短辺長を採用
	dh = (fabs(hd)<EPSILON) ? dh*0.577 : hd;							// 指定高さ
	minx = DBL_MAX, miny = DBL_MAX, minz = DBL_MAX;
	maxx = 1.0-DBL_MAX, maxy = 1.0-DBL_MAX, maxz = 1.0-DBL_MAX;
	for(i=0; i<41; i++) {
		minx = (minx>pa[i].p[0]) ? pa[i].p[0] : minx;					// 最小/最大取得
		miny = (miny>pa[i].p[1]) ? pa[i].p[1] : miny;
		minz = (minz>pa[i].p[2]) ? pa[i].p[2] : minz;
		maxx = (maxx<pa[i].p[0]) ? pa[i].p[0] : maxx;
		maxy = (maxy<pa[i].p[1]) ? pa[i].p[1] : maxy;
		maxz = (maxz<pa[i].p[2]) ? pa[i].p[2] : maxz;
	}
	gx = maxx-minx, gy = maxy-miny, gz = maxz-minz;						// 現状X長さ/Y長さ/高さ
	cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;						// ｱﾌｨﾝ変換の基準点
	sx = dx/gx, sy = dy/gy, sz = dh/gz;									// X/Y/Z方向の倍率
	mat[0][0] = sx,  mat[0][1] = 0.0, mat[0][2] = 0.0;					// ｱﾌｨﾝ変換ﾏﾄﾘｯｸｽ
	mat[1][0] = 0.0, mat[1][1] = sy,  mat[1][2] = 0.0;
	mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = sz;
	AffineVtx(41, pa, cp, mat);											// 頂点のｱﾌｨﾝ変換
	for(i=0; i<41; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/*************************************************/
static void SetSingleVtx(PNTTYPE* pa, double xd,
                         double hd, PNTTYPE cent)                       //<<< ｼﾝｸﾞﾙ頂点設定
/*************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, z0, z1, z2;

	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		z0 = -xd*0.431, z1 = 0.0, z2 = xd*0.146;
	} else {															// 高さ指定有の場合
		z0 = -hd*0.431/0.577, z1 = 0.0, z2 = hd*0.146/0.577;
	}
	dth = PI/4.0;
	d1 = xd/2.0;
	d2 = xd*0.575/2.0;
	pa[vn].p[0] = 0.0, pa[vn].p[1] = 0.0, pa[vn++].p[2] = z0;			// 頂点の設定
	for(th=2.0*PI, i=0; i<8; th-=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
		pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

double zpa[7] = { 0.447, 0.402, 0.246, 0.0, -0.56, -0.94, -1.34 };
double xpa[6] = { 0.434, 0.346, 0.173, 0.0, 0.29, 0.58 };
double tan60  = 2.4142;

/************************************************************/
static void SetEmeraldVtx(PNTTYPE* pa, double xd, double yd,
                          double hd, PNTTYPE cent)                      //<<< ｴﾒﾗﾙﾄﾞ頂点設定
/************************************************************/
{
	int    i, j, sw, vn=0;
	double dx, dy, dd, wzpa[7];

	if(xd>yd) {
		dx = yd/2.0, dy = xd/2.0, sw = 1;
	} else {
		dx = xd/2.0, dy = yd/2.0, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		for(i=0; i<7; i++) {
			wzpa[i] = dx*zpa[i];
		}
	} else {															// 高さ指定有の場合
		for(i=0; i<7; i++) {
			wzpa[i] = hd*zpa[i]/1.787;
		}
	}
	for(i=0; i<6; i++) {
		dd = (xpa[i]/tan60+0.4)*dx;										// 頂点の設定
		pa[vn].p[0] = dx-dx*xpa[i], pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = dy-dx*xpa[i], pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = dy-dx*xpa[i], pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dx*xpa[i]), pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dx*xpa[i]), pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = -(dy-dx*xpa[i]), pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = -(dy-dx*xpa[i]), pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = dx-dx*xpa[i], pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wzpa[i];
	}
	pa[vn].p[0] = 0.0, pa[vn].p[1] = dy-dx, pa[vn++].p[2] = wzpa[6];
	pa[vn].p[0] = 0.0, pa[vn].p[1] = -(dy-dx), pa[vn++].p[2] = wzpa[6];
	if(sw) {															// <XYを入れ替えた場合>
		for(i=0; i<vn; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90度回転
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

double bzpa[4] = { 0.45, 0.0, -0.8, -1.2 };
double bxpa[3] = { 0.4, 0.0, 0.5 };

/*************************************************************/
static void SetBaguetteVtx(PNTTYPE* pa, double xd, double yd,
                           double hd, PNTTYPE cent)                     //<<< ﾊﾞｹﾞｯﾄ頂点設定
/*************************************************************/
{
	int    i, j, sw, vn=0;
	double dx, dy, dd, wbzpa[4];

	if(xd>yd) {
		dx = yd/2.0, dy = xd/2.0, sw = 1;
	} else {
		dx = xd/2.0, dy = yd/2.0, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		for(i=0; i<4; i++) {
			wbzpa[i] = dx*bzpa[i];
		}
    } else {															// 高さ指定有の場合
		for(i=0; i<4; i++) {
			wbzpa[i] = hd*bzpa[i]/1.65;
		}
	}
	for(i=0; i<3; i++) {
		dd = bxpa[i]*dx;												// 頂点の設定
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
	}
	pa[vn].p[0] = 0.0, pa[vn].p[1] = dy-dx, pa[vn++].p[2] = wbzpa[3];
	pa[vn].p[0] = 0.0, pa[vn].p[1] = -(dy-dx), pa[vn++].p[2] = wbzpa[3];
	if(sw) {															// <XYを入れ替えた場合>
		for(i=0; i<vn; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90度回転
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/***********************************************************************/
static void SetTaperedVtx(PNTTYPE* pa, double xd, double yd, double hd,
                          double xh, double yh, int fg, PNTTYPE cent)   //<<< ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ頂点設定
/***********************************************************************/
{
	VECTYPE v1, v2;
	int     i, j, i1, i2, sw, ix, vn=0, uf=0;
	double  dx, dy, hx, hy, dd, xx, ln, wbzpa[4];

	if(xd>yd) {
		dx = yd/2.0, dy = xd/2.0, hx = yh, hy = xh, sw = 1;
	} else {
		dx = xd/2.0, dy = yd/2.0, hx = xh, hy = yh, sw = 0;
	}
	if(xd>yd&&xh>0) {
		fg = (!fg) ? 1 : 0;
	}
	xx = (dx-hx>dy-hy) ? dy-hy : dx-hx;
	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		for(i=0; i<4; i++) {
			wbzpa[i] = xx*bzpa[i];
		}
	} else {															// 高さ指定有の場合
		for(i=0; i<4; i++) {
			wbzpa[i] = hd*bzpa[i]/1.65;
		}
	}
	for(i=0; i<3; i++) {
		dd = bxpa[i]*xx;												// 頂点の設定
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
	}
	pa[vn].p[0] = 0.0, pa[vn].p[1] = dy-dx, pa[vn++].p[2] = wbzpa[3];
	pa[vn].p[0] = 0.0, pa[vn].p[1] = -(dy-dx), pa[vn++].p[2] = wbzpa[3];
	if(hx>0) {															// 補正値がある場合は
		if(!fg) {
			pa[6].p[0] += hx, pa[7].p[0] -= hx;							// 補正値分の座標移動
		} else {
			pa[5].p[0] += hx, pa[4].p[0] -= hx;
		}
		uf = 1, ix = 1;
	} else if(hy>0) {
		if(!fg) {
			pa[6].p[1] += hy, pa[5].p[1] -= hy;
		} else {
			pa[7].p[1] += hy, pa[4].p[1] -= hy;
		}
		uf = 1, ix = 0;
	}
	if(uf) {															// <補正有>
		for(i=0; i<4; i++) {
			i1 = (i-1<0) ? 3 : i-1;
			i2 = (i+1>3) ? 0 : i+1;
			VecV(&pa[i+4], &pa[i1+4], &v1);								// 隣接する2点への
			VecN(&v1);													// 単位ﾍﾞｸﾄﾙを算出
			VecV(&pa[i+4], &pa[i2+4], &v2);
			VecN(&v2);
			for(j=0; j<DIM; j++) {										// ﾍﾞｸﾄﾙの和
				v1.p[j] += v2.p[j];
			}
			VecN(&v1);													// 単位ﾍﾞｸﾄﾙ化
			ln = fabs((pa[i+4].p[ix]-pa[i].p[ix])/v1.p[ix]);			// 新しい点に移動
			for(j=0; j<2; j++) {
				pa[i].p[j] = pa[i+4].p[j]+v1.p[j]*ln;
			}
			ln = fabs((pa[i+4].p[ix]-pa[i+8].p[ix])/v1.p[ix]);
			for(j=0; j<2; j++) {
				pa[i+8].p[j] = pa[i+4].p[j]+v1.p[j]*ln;
			}
		}
		VecV(&pa[5], &pa[4], &v1);										// 隣接する2点への
		VecN(&v1);														// 単位ﾍﾞｸﾄﾙを算出
		VecV(&pa[5], &pa[6], &v2);
		VecN(&v2);
		for(j=0; j<DIM; j++) {
			v1.p[j] += v2.p[j];											// ﾍﾞｸﾄﾙの和
		}
		VecN(&v1);														// 単位ﾍﾞｸﾄﾙ化
		ln = fabs(pa[5].p[0]/v1.p[0]);									// 新しい点に移動
		for(j=0; j<2; j++) {
			pa[12].p[j] = pa[5].p[j]+v1.p[j]*ln;
		}
		VecV(&pa[6], &pa[5], &v1);										// 隣接する2点への
		VecN(&v1);														// 単位ﾍﾞｸﾄﾙを算出
		VecV(&pa[6], &pa[7], &v2);
		VecN(&v2);
		for(j=0; j<DIM; j++) {
			v1.p[j] += v2.p[j];											// ﾍﾞｸﾄﾙの和
		}
		VecN(&v1);														// 単位ﾍﾞｸﾄﾙ化
		ln = fabs(pa[6].p[0]/v1.p[0]);									// 新しい点に移動
		for(j=0; j<2; j++) {
			pa[13].p[j] = pa[6].p[j]+v1.p[j]*ln;
		}
	}
	if(sw) {															// <XYを入れ替えた場合>
		for(i=0; i<vn; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90度回転
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/***********************************************************************/
static void SetRoseVtx(PNTTYPE* pa, double xd, double hd, PNTTYPE cent) //<<< ﾛｰｽﾞ頂点設定
/***********************************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, z0, z1, z2;

	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		z0 = xd*0.2, z1 = xd*0.12, z2 = 0.0;
	} else {															// 高さ指定有の場合
		z0 = hd, z1 = hd*0.12/0.2, z2 = 0.0;
	}
	d2 = xd/2.0;
	d1 = d2*0.76;
	pa[vn].p[0] = 0.0, pa[vn].p[1] = 0.0, pa[vn++].p[2] = z0;			// 頂点の設定
	dth = PI/3.0;
	for(th=0, i=0; i<6; th+=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
	}
	dth = PI/6;
	for(th=0, i=0; i<12; th+=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/***************************************************/
static void SetCabochonVtx(PNTTYPE* pa, double xd,
                           double hd, PNTTYPE cent)                     //<<< ｶﾎﾞｼｮﾝ頂点設定
/***************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, d3, d4, d5, d6, z0, z1, z2, z3, z4, z5, z6;

	if(fabs(hd)<EPSILON) {												// 高さ指定無の場合
		z0 = xd*0.3, z1 = xd*0.29, z2 = xd*0.26, z3 = xd*0.21;
		z4 = xd*0.14, z5 = xd*0.07, z6 = 0.0;
	} else {															// 高さ指定有の場合
		z0 = hd, z1 = hd*0.29/0.3, z2 = hd*0.26/0.3, z3 = hd*0.21/0.3;
		z4 = hd*0.14/0.3, z5 = hd*0.07/0.3, z6 = 0.0;
	}
	d1 = xd*0.155, d2 = xd*0.266, d3 = xd*0.357;
	d4 = xd*0.431, d5 = xd*0.474, d6 = xd*0.5;
	pa[vn].p[0] = 0.0, pa[vn].p[1] = 0.0, pa[vn++].p[2] = z0;			// 頂点の設定
	dth = PI/12.0;
	for(th=0, i=0; i<24; th+=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
		pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
		pa[vn].p[0] = d3*cp, pa[vn].p[1] = d3*sp, pa[vn++].p[2] = z3;
		pa[vn].p[0] = d4*cp, pa[vn].p[1] = d4*sp, pa[vn++].p[2] = z4;
		pa[vn].p[0] = d5*cp, pa[vn].p[1] = d5*sp, pa[vn++].p[2] = z5;
		pa[vn].p[0] = d6*cp, pa[vn].p[1] = d6*sp, pa[vn++].p[2] = z6;
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// 画面ｾﾝﾀｰ移動
		}
	}
}

/*************************************************************/
BOOL CJcad3GlbView::ReadGem(int no, PNTTYPE* cpt, double* dh)           //<<< 宝石ｶｯﾄ入力
/*************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[150];
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  cent, cp;
	CString  fname;
	BOOL     bOK=TRUE;
	int      i, j, vn;
	double   mat[3][3], xd, yd, hd=0.0, dd, wx, wy;

	xd = GetDispHeight()/2.0, yd = GetDispHeight()/2.0;					// 画面の長さの半分取得
	if(GetNumInp()!=0) {												// <数値入力あり>
		if(no==0||no==5||no==9) {
			if((bOK=InputGemSize(&m_GemRndSize,
                                 &m_GemRndHeigt))==TRUE) {				// 石の直径,高さ入力
				xd = m_GemRndSize, hd = m_GemRndHeigt;
			}
		} else {
			if((bOK=InputGemSize(&m_GemSqrSize1, &m_GemSqrSize2,
                                 &m_GemSqrHeigt))==TRUE) {				// 石のX,Y,高さ入力
				xd = m_GemSqrSize1, yd = m_GemSqrSize2, hd = m_GemSqrHeigt;
			}
		}
	}
	if(bOK) {															// <指定ｷｬﾝｾﾙ以外>
		if(no==0) {														// ﾌｧｲﾙ名(ﾗｳﾝﾄﾞﾌﾞﾘﾘｱﾝﾄ)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==1) {												//       (ｵｰﾊﾞﾙﾌﾞﾘﾘｱﾝﾄ)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==2) {												//       (ﾏｰｷｰｽﾞﾌﾞﾘﾘｱﾝﾄ)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==3) {												//       (ﾍﾟｱｼｪｰﾌﾟﾌﾞﾘﾘｱﾝﾄ)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==4) {												//       (ﾊｰﾄｼｪｰﾌﾟﾌﾞﾘﾘｱﾝﾄ)
			fname = GetAppPath(0)+HEA_NAME;
		} else if(no==5) {												//       (ｼﾝｸﾞﾙ)
			fname = GetAppPath(0)+SNG_NAME;
		} else if(no==6) {												//       (ｴﾒﾗﾙﾄﾞ)
			fname = GetAppPath(0)+EMR_NAME;
		} else if(no==7) {												//       (ﾊﾞｹﾞｯﾄ)
			fname = GetAppPath(0)+BGE_NAME;
		} else if(no==8) {												//       (ﾃｰﾊﾟｰﾄﾞﾊﾞｹﾞｯﾄ)
			fname = GetAppPath(0)+BGE_NAME;
		} else if(no==9) {												//       (ﾛｰｽﾞ)
			fname = GetAppPath(0)+ROS_NAME;
		} else {														//       (ｶﾎﾞｼｮﾝ)
			fname = GetAppPath(0)+CAB_NAME;
		}
		InitLevel();													// Undo/Redo初期化
		if(ReadJsdFile(fname, 0, 0)) {									// 宝石ｶｯﾄ入力
			AfxMessageBox(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
			BackLevel();												// Undo/Redo戻り
			delete[] pa;
			return FALSE;												// 入力不能は中止
		}
		OffEdtAllObject();												// 編集立体全解除
		GetObjPtr(&op);													// 立体のﾎﾟｲﾝﾄ取得
		GetObjOif(op, &oif);											// 立体構成情報ﾎﾟｲﾝﾀ取得
		if(no==0) {
			SetOifName(1, oif, OBJ_NAME19);								// 構成名(ﾗｳﾝﾄﾞﾌﾞﾘﾘｱﾝﾄ)
		} else if(no==1) {
			SetOifName(1, oif, OBJ_NAME20);								//       (ｵｰﾊﾞﾙﾌﾞﾘﾘｱﾝﾄ)
		} else if(no==2) {
			SetOifName(1, oif, OBJ_NAME21);								//       (ﾏｰｷｰｽﾞﾌﾞﾘﾘｱﾝﾄ)
		} else if(no==3) {
			SetOifName(1, oif, OBJ_NAME22);								//       (ﾍﾟｱｼｪｰﾌﾟﾌﾞﾘﾘｱﾝﾄ)
		} else if(no==4) {
			SetOifName(1, oif, OBJ_NAME23);								//       (ﾊｰﾄｼｪｰﾌﾟﾌﾞﾘﾘｱﾝﾄ)
		} else if(no==5) {
			SetOifName(1, oif, OBJ_NAME24);								//       (ｼﾝｸﾞﾙ)
		} else if(no==6) {
			SetOifName(1, oif, OBJ_NAME25);								//       (ｴﾒﾗﾙﾄﾞ)
		} else if(no==7) {
			SetOifName(1, oif, OBJ_NAME26);								//       (ﾊﾞｹﾞｯﾄ)
		} else if(no==8) {
			SetOifName(1, oif, OBJ_NAME27);								//       (ﾃｰﾊﾟｰﾄﾞﾊﾞｹﾞｯﾄ)
		} else if(no==9) {
			SetOifName(1, oif, OBJ_NAME28);								//       (ﾛｰｽﾞ)
		} else {
			SetOifName(1, oif, OBJ_NAME29);								//       (ｶﾎﾞｼｮﾝ)
		}
		OnEdtFlg(1, op);												// 編集立体設定
		GetCentPnt(&cent);												// 画面の中心取得
		if(no==0) {														// <ﾗｳﾝﾄﾞ>
			SetBrilliantVtx(pa, xd, hd, cent);							// ﾌﾞﾘﾘｱﾝﾄ頂点設定
			vn = 41;													// 頂点数
		} else if(no==1) {												// <ｵｰﾊﾞﾙ>
			cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
			if(xd>yd) {													// X軸の方が長い場合は
				dd = xd, xd = yd, yd = dd;								// XとYを入替
				wx = yd/xd, wy = 1.0;									// XとYの比率
			} else {
				wx = 1.0, wy = yd/xd;									// XとYの比率
			}
			SetBrilliantVtx(pa, xd, hd, cp);							// ﾌﾞﾘﾘｱﾝﾄ頂点設定
			vn = 41;													// 頂点数
			mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
			mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
			mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
			AffineVtx(vn, pa, cp, mat);									// 頂点のｱﾌｨﾝ変換
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pa[i].p[j] += cent.p[j];							// 画面ｾﾝﾀｰ移動
				}
			}
		} else if(no==2) {												// <ﾏｰｷｰｽﾞ>
			SetMarquiseVtx(pa, xd, yd, hd, cent);						// ﾏｰｷｰｽﾞ頂点設定
			vn = 41;													// 頂点数
		} else if(no==3) {												// <ﾍﾟｱｼｪｰﾌﾟ>
			SetPearShapeVtx(pa, xd, yd, hd, cent);						// ﾍﾟｱｼｪｰﾌﾟ頂点設定
			vn = 41;													// 頂点数
		} else if(no==4) {												// <ﾊｰﾄｼｪｰﾌﾟ>
			for(i=0; i<41; i++) {
				GetVtx(op, i, &pa[i]);									// 頂点取得
			}
			SetHeartShapeVtx(pa, xd, yd, hd, cent);						// ﾊｰﾄｼｪｰﾌﾟ頂点設定
			vn = 41;													// 頂点数
		} else if(no==5) {												// <ｼﾝｸﾞﾙ>
			SetSingleVtx(pa, xd, hd, cent);								// ｼﾝｸﾞﾙ頂点設定
			vn = 17;													// 頂点数
		} else if(no==6) {												// <ｴﾒﾗﾙﾄﾞ>
			SetEmeraldVtx(pa, xd, yd, hd, cent);						// ｴﾒﾗﾙﾄﾞ頂点設定
			vn = 50;													// 頂点数
		} else if(no==7) {												// <ﾊﾞｹﾞｯﾄ>
			SetBaguetteVtx(pa, xd, yd, hd, cent);						// ﾊﾞｹﾞｯﾄ頂点設定
			vn = 14;													// 頂点数
		} else if(no==8) {												// <ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ>
			SetTaperedVtx(pa, xd, yd, hd, 0.0, 0.0, 0, cent);			// ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ頂点設定
			vn = 14;													// 頂点数
		} else if(no==9) {												// <ﾛｰｽﾞ>
			SetRoseVtx(pa, xd, hd, cent);								// ﾛｰｽﾞ頂点設定
			vn = 19;													// 頂点数
		} else {														// <ｶﾎﾞｼｮﾝ>
			cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
			if(xd>yd) {													// X軸の方が長い場合は
				dd = xd, xd = yd, yd = dd;								// XとYを入替
				wx = yd/xd, wy = 1.0;									// XとYの比率
			} else {
				wx = 1.0, wy = yd/xd;									// XとYの比率
			}
			SetCabochonVtx(pa, xd, hd, cp);								// ｶﾎﾞｼｮﾝ頂点設定
			vn = 145;													// 頂点数
			mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
			mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
			mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
			AffineVtx(vn, pa, cp, mat);									// 頂点のｱﾌｨﾝ変換
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pa[i].p[j] += cent.p[j];							// 画面ｾﾝﾀｰ移動
				}
			}
		}
		RedoDirect(vn, pa);												// 直接生成ｺﾏﾝﾄﾞ再実行
		*cpt = cent;													// 画面ｾﾝﾀｰ座標
		*dh  = hd;														// 指定高さ
	}
	delete[] pa;
	return bOK;
}

/***************************************************************/
static void TrackerGem1(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa)         //<<< ﾘｻｲｽﾞ用ﾄﾗｯｶｰ取得
/***************************************************************/       //<<< ﾗｳﾝﾄﾞ系宝石ｶｯﾄ
{
	pa[0].p[0] = box.bp[0].p[0];										// ﾄﾗｯｶｰ座標設定
	pa[0].p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
	pa[0].p[2] = cent.p[2];
	pa[1].p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
	pa[1].p[1] = box.bp[0].p[1];
	pa[1].p[2] = cent.p[2];
	pa[2].p[0] = box.bp[1].p[0];
	pa[2].p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
	pa[2].p[2] = cent.p[2];
	pa[3].p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
	pa[3].p[1] = box.bp[1].p[1];
	pa[3].p[2] = cent.p[2];
}

/***************************************************************/
static void TrackerGem2(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa)         //<<< ﾘｻｲｽﾞ用ﾄﾗｯｶｰ取得
/***************************************************************/       //<<< ﾚｸﾄ系宝石ｶｯﾄ
{
	pa[0] = box.bp[0], pa[0].p[2] = cent.p[2];							// ﾄﾗｯｶｰ座標設定
	pa[1].p[0] = box.bp[1].p[0];
	pa[1].p[1] = box.bp[0].p[1];
	pa[1].p[2] = cent.p[2];
	pa[2] = box.bp[1], pa[2].p[2] = cent.p[2];
	pa[3].p[0] = box.bp[0].p[0];
	pa[3].p[1] = box.bp[1].p[1];
	pa[3].p[2] = cent.p[2];
}

/**********************************************************/
static void TrackerGem3(BOXTYPE box, PNTTYPE cent,
                        double xh, double yh, PNTTYPE* pa)              //<<< ﾘｻｲｽﾞ用ﾄﾗｯｶｰ取得
/**********************************************************/            //<<< ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ
{
	int    i;
	double wx, wy;

	pa[0] = box.bp[0];													// ﾄﾗｯｶｰ座標設定
	pa[1].p[0] = box.bp[1].p[0], pa[1].p[1] = box.bp[0].p[1];
	pa[2] = box.bp[1];
	pa[3].p[0] = box.bp[0].p[0], pa[3].p[1] = box.bp[1].p[1];
	for(i=0; i<4; i++) {
		pa[i].p[2] = cent.p[2];
	}
	wx = box.bp[1].p[0]-cent.p[0], wy = box.bp[1].p[1]-cent.p[1];
	xh = (fabs(xh)<EPSILON) ? wx/2.0 : xh;
	yh = (fabs(yh)<EPSILON) ? wy/2.0 : yh;
	for(i=0; i<4; i++) {
		pa[2*i+4] = pa[i], pa[2*i+5] = pa[i];
	}
	pa[4].p[1] += yh, pa[5].p[0] += xh;
	pa[6].p[0] -= xh, pa[7].p[1] += yh;
	pa[8].p[1] -= yh, pa[9].p[0] -= xh;
	pa[10].p[0] += xh, pa[11].p[1] -= yh;
}

/**************************************************************/
static void TrackerGemH(BOXTYPE box, PNTTYPE gct, PNTTYPE* pa)          //<<< 高さ用ﾄﾗｯｶｰ取得
/**************************************************************/
{
	int    gno;
	double z, hd;

	pa[0] = gct;
	z = box.bp[1].p[2]-box.bp[0].p[2];									// 宝石ｶｯﾄの高さ
	gno = View->GetGemNo();												// 宝石ｶｯﾄNo取得
	if(gno==5) {														// ｼﾝｸﾞﾙ･ｶｯﾄ
		hd = z*0.146/0.577;
	} else if(gno==6) {													// ｴﾒﾗﾙﾄﾞ･ｶｯﾄ
		hd = z*0.447/1.787;
	} else if(gno==7) {													// ﾊﾞｹﾞｯﾄ･ｶｯﾄ
		hd = z*0.45/1.65;
	} else if(gno==8) {													// ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ･ｶｯﾄ
		hd = z*0.45/1.65;
	} else if(gno==9) {													// ﾛｰｽﾞ･ｶｯﾄ
		hd = z;
	} else if(gno==10) {												// ｶﾎﾞｼｮﾝ･ｶｯﾄ
		hd = z;
	} else {															// ﾌﾞﾘﾘｱﾝﾄ･ｶｯﾄ
		hd = z*0.146/0.577;
	}
	pa[0].p[2] += hd;													// 中心にZ分加算
}

/********************************/
static double CalcGemH(double z)                                        //<<< 宝石ｶｯﾄ高さ算出
/********************************/
{
	int    gno;
	double hd;

	gno = View->GetGemNo();												// 宝石ｶｯﾄNo取得
	if(gno==5) {														// ｼﾝｸﾞﾙ･ｶｯﾄ
		hd = z*0.577/0.146;
	} else if(gno==6) {													// ｴﾒﾗﾙﾄﾞ･ｶｯﾄ
		hd = z*1.787/0.447;
	} else if(gno==7) {													// ﾊﾞｹﾞｯﾄ･ｶｯﾄ
		hd = z*1.65/0.45;
	} else if(gno==8) {													// ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ･ｶｯﾄ
		hd = z*1.65/0.45;
	} else if(gno==9) {													// ﾛｰｽﾞ･ｶｯﾄ
		hd = z;
	} else if(gno==10) {												// ｶﾎﾞｼｮﾝ･ｶｯﾄ
		hd = z;
	} else {															// ﾌﾞﾘﾘｱﾝﾄ･ｶｯﾄ
		hd = z*0.577/0.146;
	}
	return hd;
}

/*****************************************************************/
void CJcad3GlbView::MouseGemSquare(UINT flg, const CPoint& point,
                                   int plane, int ms)                   //<<< 宝石ｶｯﾄ1
/*****************************************************************/     // ﾗｳﾝﾄﾞ,ｼﾝｸﾞﾙ,ﾛｰｽﾞ
{
	PNTTYPE pnt;
	BOXTYPE box;
	int     ht, i, esw=0, col=0;
	double  dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// 透視図の場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// ﾏｳｽ移動
		if(flg&MK_LBUTTON) {											// ﾏｳｽの左釦押下中のみ有効
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(Sphase!=0) {												// 初期以外は入力ﾁｪｯｸ
				if(Sphase==1) {											// 直径変更の場合
					if(Si3==-1) {
						esw = (pnt.p[0]>=Sbase.p[0]) ? 1 : esw;
					} else if(Si3==1) {
						esw = (pnt.p[0]<=Sbase.p[0]) ? 1 : esw;
					}
					if(Si4==-1)	{
						esw = (pnt.p[1]>=Sbase.p[1]) ? 1 : esw;
					} else if(Si4==1) {
						esw = (pnt.p[1]<=Sbase.p[1]) ? 1 : esw;
					}
				} else {												// 高さ変更の場合
					esw = (pnt.p[2]<=Spnt1.p[2]) ? 1 : esw;
				}
				if(esw==0) {											// <<<ｴﾗｰ無の場合>>>
					if(Sphase==1) {
						Sd1 = fabs(pnt.p[Si2]-Sbase.p[Si2])*2.0;		// 直径変更
						dd = 0.0;										// 高さ未指定
					} else {
						dd = fabs(pnt.p[2]-Spnt1.p[2]);					// 画面中心から上部長
						dd = CalcGemH(dd);								// 宝石ｶｯﾄの高さ
					}
					if(m_GemNo==0) {									// <ﾗｳﾝﾄﾞ>
						SetBrilliantVtx(Spd, Sd1, dd, Spnt1);			// ﾌﾞﾘﾘｱﾝﾄ頂点設定
						i = 41;											// 頂点数
					} else if(m_GemNo==5) {								// <ｼﾝｸﾞﾙ>
						SetSingleVtx(Spd, Sd1, dd, Spnt1);				// ｼﾝｸﾞﾙ頂点設定
						i = 17;											// 頂点数
					} else {											// <ﾛｰｽﾞ>
						SetRoseVtx(Spd, Sd1, dd, Spnt1);				// ﾛｰｽﾞ頂点設定
						i = 19;											// 頂点数
					}
					RedoDirect(i, Spd);									// 直接生成ｺﾏﾝﾄﾞ再実行
					GetAreaBox(2, &box);								// ﾎﾞｯｸｽｻｲｽﾞ取得
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1],
                                         box.bp[1].p[2]-box.bp[0].p[2]);
					Display(0);											// 再描画
				}
			}
		}
	} else if(ms==1) {													// ﾏｳｽ左釦押下
		if(Sphase==0) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spa, 4))!=-1) {		// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Si2 = ht%2;
				Si3 = (fabs(Spa[ht].p[0]-Sbase.p[0])<EPSILON) ? 0		// 基準(中心)点と
                                    : ((Spa[ht].p[0]<Sbase.p[0])?-1:1);	// 更新点位置関係
				Si4 = (fabs(Spa[ht].p[1]-Sbase.p[1])<EPSILON) ? 0
                                    : ((Spa[ht].p[1]<Sbase.p[1])?-1:1);
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 1;
			} else if(Tracker2.HitTracker(1, 1, pnt, Spb, 1)!=-1) {		// ﾄﾗｯｶｰ2ﾁｪｯｸ
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// ﾏｳｽ左釦解放
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		TrackerGem1(box, Sbase, Spa);									// ﾄﾗｯｶｰ1取得
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// ﾄﾗｯｶｰ1設定
		TrackerGemH(box, Spnt1, Spb);									// ﾄﾗｯｶｰ2取得
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// ﾄﾗｯｶｰ2設定
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==0) {													// 初期処理
		StartMenu();													// ﾒﾆｭｰ開始処理
		CheckCtrlKey1();												// 制御ｷｰ確認(起動時)
		if(!ReadGem(m_GemNo, &Spnt1, &dd)) {							// 宝石ｶｯﾄ入力
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
		} else {
			SetNumDisp(2);												// 数値表示初期処理
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			if(m_NumInp!=0) {											// 数値入力の場合
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
				EndJMenu1();											// ﾒﾆｭｰ終了処理
			} else {													// 数値入力無の場合
				GetAreaBox(2, &box);									// 選択立体のﾎﾞｯｸｽｻｲｽﾞ取得
				BoxCenter(box, &Sbase);									// ﾎﾞｯｸｽｾﾝﾀｰ取得
				Sd1 = box.bp[1].p[0]-box.bp[0].p[0];					// 宝石ｶｯﾄの直径
				dd = box.bp[1].p[2]-box.bp[0].p[2];
				m_pNumDisp->SetParam(Sd1, Sd1, dd);
				TrackerGem1(box, Sbase, Spa);							// ﾄﾗｯｶｰ1取得
				Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;			// ﾄﾗｯｶｰ1色設定
				Tens[0] = 5, Tens[1] = 6, Tens[2] = 5, Tens[3] = 6;		// ﾄﾗｯｶｰ1ｵﾌﾟｼｮﾝ設定
				Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);				// ﾄﾗｯｶｰ1設定
				TrackerGemH(box, Spnt1, Spb);							// ﾄﾗｯｶｰ2取得
				Sib[0] = 0, TensC[0] = 6;								// ﾄﾗｯｶｰ2色/ｵﾌﾟｼｮﾝ設定
				Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);				// ﾄﾗｯｶｰ2表示
			}
		}
		Display(0);														// 再描画
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// ﾏｳｽ右釦押下/終了
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/****************************************************************/      //<<< 宝石ｶｯﾄ2
void CJcad3GlbView::MouseGemRect(UINT flg, const CPoint& point,         // ｵｰﾊﾞﾙ,ﾏｰｷｰｽﾞ,ﾍﾟｱｼｪｰﾌﾟ,
                                 int plane, int ms)                     // ﾊｰﾄｼｪｰﾌﾟ,ｴﾒﾗﾙﾄﾞ,ﾊﾞｹﾞｯﾄ,
/****************************************************************/      // ｶﾎﾞｼｮﾝ
{
	OBJTYPE* op;
	PNTTYPE  pnt, cp;
	BOXTYPE  box;
	int      ht, esw=0, col=0, smd=0, i, j, k;
	double   mat[3][3], d1, d2, wx, wy;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// 透視図の場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// ﾏｳｽ移動
		if(flg&MK_LBUTTON) {											// ﾏｳｽの左釦押下中のみ有効
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(Sphase!=0) {												// 初期以外は入力ﾁｪｯｸ
				if(Sphase==1) {											// 直径変更の場合
					if(flg&m_LimKey) {
						smd = LimitPoint(plane, &pnt, &Spnt2);			// ﾏｳｽ入力制限座標
					}
					if(Si3==-1) {
						esw = (pnt.p[0]>=Sbase.p[0]) ? 1 : esw;
					} else if(Si3==1) {
						esw = (pnt.p[0]<=Sbase.p[0]) ? 1 : esw;
					}
					if(Si4==-1) {
						esw = (pnt.p[1]>=Sbase.p[1]) ? 1 : esw;
					} else if(Si4==1) {
						esw = (pnt.p[1]<=Sbase.p[1]) ? 1 : esw;
					}
				} else {												// 高さ変更の場合
					esw = (pnt.p[2]<=Spnt1.p[2]) ? 1 : esw;
				}
				if(esw==0) {											// <<<ｴﾗｰ無の場合>>>
					if(Sphase==1) {
						Sd1 = fabs(pnt.p[0]-Sbase.p[0])*2.0;			// Xｻｲｽﾞ変更
						Sd2 = fabs(pnt.p[1]-Sbase.p[1])*2.0;			// Yｻｲｽﾞ変更
						if(smd==1) {									// ﾓｰﾄﾞ:2D均等
							if(Sd1<Sd2) {								// 小さい方の倍率採用
								Sd2 = Sd1;
							} else {
								Sd1 = Sd2;
							}
						}
						d2 = 0.0;										// 高さ未指定
					} else {
						d2 = fabs(pnt.p[2]-Spnt1.p[2]);					// 画面中心から上部長
						d2 = CalcGemH(d2);								// 宝石ｶｯﾄの高さ
					}
					if(m_GemNo==1) {									// <ｵｰﾊﾞﾙ>
						cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
						if(Sd1>Sd2) {									// X軸の方が長い場合は
							d1 = Sd2;									// Yを採用
							wx = Sd1/Sd2, wy = 1.0;						// XとYの比率
						} else {
							d1 = Sd1;									// Xを採用
							wx = 1.0, wy = Sd2/Sd1;						// XとYの比率
						}
						SetBrilliantVtx(Spd, d1, d2, cp);				// ﾌﾞﾘﾘｱﾝﾄ頂点設定
						k = 41;											// 頂点数
						mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
						mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
						mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
						AffineVtx(k, Spd, cp, mat);						// 頂点のｱﾌｨﾝ変換
						for(i=0; i<k; i++) {
							for(j=0; j<DIM; j++) {
								Spd[i].p[j] += Spnt1.p[j];				// 画面ｾﾝﾀｰ移動
							}
						}
					} else if(m_GemNo==2) {								// <ﾏｰｷｰｽﾞ>
						SetMarquiseVtx(Spd, Sd1, Sd2, d2, Spnt1);		// ﾏｰｷｰｽﾞ頂点設定
						k = 41;											// 頂点数
					} else if(m_GemNo==3) {								// <ﾍﾟｱｼｪｰﾌﾟ>
						SetPearShapeVtx(Spd, Sd1, Sd2, d2, Spnt1);		// ﾍﾟｱｼｪｰﾌﾟ頂点設定
						k = 41;											// 頂点数
					} else if(m_GemNo==4) {								// <ﾊｰﾄｼｪｰﾌﾟ>
						for(i=0; i<41; i++) {
							Spd[i] = Spc[i];
						}
						SetHeartShapeVtx(Spd, Sd1, Sd2, d2, Spnt1);		// ﾊｰﾄｼｪｰﾌﾟ頂点設定
						k = 41;											// 頂点数
					} else if(m_GemNo==6) {								// <ｴﾒﾗﾙﾄﾞ>
						SetEmeraldVtx(Spd, Sd1, Sd2, d2, Spnt1);		// ｴﾒﾗﾙﾄﾞ頂点設定
						k = 50;											// 頂点数
					} else if(m_GemNo==7) {								// <ﾊﾞｹﾞｯﾄ>
						SetBaguetteVtx(Spd, Sd1, Sd2, d2, Spnt1);		// ﾊﾞｹﾞｯﾄ頂点設定
						k = 14;											// 頂点数
					} else {											// <ｶﾎﾞｼｮﾝ>
						cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
						if(Sd1>Sd2) {									// X軸の方が長い場合は
							d1 = Sd2;									// Yを採用
							wx = Sd1/Sd2, wy = 1.0;						// XとYの比率
							
						} else {
							d1 = Sd1;									// Xを採用
							wx = 1.0, wy = Sd2/Sd1;						// XとYの比率
						}
						SetCabochonVtx(Spd, d1, d2, cp);				// ｶﾎﾞｼｮﾝ頂点設定
						k = 145;										// 頂点数
						mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
						mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
						mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
						AffineVtx(k, Spd, cp, mat);						// 頂点のｱﾌｨﾝ変換
						for(i=0; i<k; i++) {
							for(j=0; j<DIM; j++) {
								Spd[i].p[j] += Spnt1.p[j];				// 画面ｾﾝﾀｰ移動
							}
						}
					}
					RedoDirect(k, Spd);									// 直接生成ｺﾏﾝﾄﾞ再実行
					GetAreaBox(2, &box);								// ﾎﾞｯｸｽｻｲｽﾞ取得
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1],
                                         box.bp[1].p[2]-box.bp[0].p[2]);
					Display(0);											// 再描画
				}
			}
		}
	} else if(ms==1) {													// ﾏｳｽ左釦押下
		if(Sphase==0) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spa, 4))!=-1) {		// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Spnt2 = Spa[ht];										// ﾘｻｲｽﾞ更新点設定
				Si3 = (Spnt2.p[0]<Sbase.p[0]) ? -1						// 基準(中心)点と
                                       : ((Spnt2.p[0]>Sbase.p[0])?1:0);	// 更新点位置関係
				Si4 = (Spnt2.p[1]<Sbase.p[1]) ? -1
                                       : ((Spnt2.p[1]>Sbase.p[1])?1:0);
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 1;
			} else if(Tracker2.HitTracker(1, 1, pnt, Spb, 1)!=-1) {		// ﾄﾗｯｶｰ2ﾁｪｯｸ
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// ﾏｳｽ左釦解放
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		TrackerGem2(box, Sbase, Spa);									// ﾄﾗｯｶｰ1取得
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// ﾄﾗｯｶｰ1設定
		TrackerGemH(box, Spnt1, Spb);									// ﾄﾗｯｶｰ2取得
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// ﾄﾗｯｶｰ2設定
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==0) {													// 初期処理
		StartMenu();													// ﾒﾆｭｰ開始処理
		CheckCtrlKey1();												// 制御ｷｰ確認(起動時)
		if(!ReadGem(m_GemNo, &Spnt1, &d1)) {							// 宝石ｶｯﾄ入力
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
		} else {
			SetNumDisp(2);												// 数値表示初期処理
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			if(m_NumInp!=0) {											// 数値入力の場合
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
				EndJMenu1();											// ﾒﾆｭｰ終了処理
			} else {													// 数値入力無の場合
				GetAreaBox(2, &box);									// 選択立体のﾎﾞｯｸｽｻｲｽﾞ取得
				BoxCenter(box, &Sbase);									// ﾎﾞｯｸｽｾﾝﾀｰ取得
				Sd1 = box.bp[1].p[0]-box.bp[0].p[0];					// 宝石ｶｯﾄのXｻｲｽﾞ
				Sd2 = box.bp[1].p[1]-box.bp[0].p[1];					// 宝石ｶｯﾄのYｻｲｽﾞ
				d1 = box.bp[1].p[2]-box.bp[0].p[2];
				m_pNumDisp->SetParam(Sd1, Sd2, d1);
				TrackerGem2(box, Sbase, Spa);							// ﾄﾗｯｶｰ1取得
				Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;			// ﾄﾗｯｶｰ1色設定
				Tens[0] = 7, Tens[1] = 7, Tens[2] = 7, Tens[3] = 7;		// ﾄﾗｯｶｰ1ｵﾌﾟｼｮﾝ設定
				Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);				// ﾄﾗｯｶｰ1設定
				TrackerGemH(box, Spnt1, Spb);							// ﾄﾗｯｶｰ2取得
				Sib[0] = 0, TensC[0] = 6;								// ﾄﾗｯｶｰ2色/ｵﾌﾟｼｮﾝ設定
				Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);				// ﾄﾗｯｶｰ2表示
				if(m_GemNo==4) {										// <ﾊｰﾄｼｪｰﾌﾟ>
					GetObjPtr(&op);										// 立体ﾎﾟｲﾝﾄ取得
					for(i=0; i<41; i++) {
						GetVtx(op, i, &Spc[i]);							// 頂点保存
						for(j=0; j<DIM; j++) {
							Spc[i].p[j] -= Spnt1.p[j];
						}
					}
				}
			}
		}
		Display(0);														// 再描画
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// ﾏｳｽ右釦押下/終了
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/****************************************************************/
void CJcad3GlbView::MouseGemRect2(UINT flg, const CPoint& point,
                                  int plane, int ms)                    //<<< 宝石ｶｯﾄ3
/****************************************************************/      // ﾃｰﾊﾟｰﾄﾞﾊﾞｹﾞｯﾄ
{
	PNTTYPE pnt, cpt, mip, map;
	BOXTYPE box;
	int     ht, esw=0, col=0, smd=0, i, i1, i2;
	double  dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// 透視図の場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// ﾏｳｽ移動
		if(flg&MK_LBUTTON) {											// ﾏｳｽの左釦押下中のみ有効
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(Sphase!=0) {												// 初期以外は入力ﾁｪｯｸ
				if(Sphase==1) {											// 直径変更の場合
					if(flg&m_LimKey) {
						smd = LimitPoint(plane, &pnt, &Spnt2);			// ﾏｳｽ入力制限座標
					}
					if(!Si6) {											// <ｻｲｽﾞ変更>
						if(Si7==-1) {
							esw = (pnt.p[0]>=Sbase.p[0]) ? 1 : esw;
						} else if(Si7==1) {
							esw = (pnt.p[0]<=Sbase.p[0]) ? 1 : esw;
						}
						if(Si8==-1) {
							esw = (pnt.p[1]>=Sbase.p[1]) ? 1 : esw;
						} else if(Si8==1) {
							esw = (pnt.p[1]<=Sbase.p[1]) ? 1 : esw;
						}
					} else {											// <四隅変更>
						if(!Si8) {										// 変更可能範囲取得
							if(Si7==0||Si7==1) {
								i1 = 0, i2 = 1;
							} else {
								i1 = 3, i2 = 2;
							}
						} else {
							if(Si7==0||Si7==3) {
								i1 = 0, i2 = 3;
							} else {
								i1 = 1, i2 = 2;
							}
						}
						for(i=0; i<DIM; i++) {
							cpt.p[i] = (Spa[i1].p[i]+Spa[i2].p[i])/2.0;	// 中間点取得
						}
						if(!Si8) {
							if(Si7==0||Si7==3) {
								mip = Spa[Si7], map = cpt;
							} else {
								mip = cpt, map = Spa[Si7];
							}
						} else {
							if(Si7==0||Si7==1) {
								mip = Spa[Si7], map = cpt;
							} else {
								mip = cpt, map = Spa[Si7];
							}
						}
						esw = (pnt.p[Si8]<=mip.p[Si8]) ? 1 : esw;
						esw = (pnt.p[Si8]>=map.p[Si8]) ? 1 : esw;
					}
				} else {												// 高さ変更の場合
					esw = (pnt.p[2]<=Spnt1.p[2]) ? 1 : esw;
				}
				if(esw==0) {											// <<<ｴﾗｰ無の場合>>>
					if(Sphase==1) {
						if(!Si6) {										// 変更後のｻｲｽﾞ
							Sd1 = fabs(pnt.p[0]-Sbase.p[0])*2.0;		// Xｻｲｽﾞ変更
							Sd2 = fabs(pnt.p[1]-Sbase.p[1])*2.0;		// Yｻｲｽﾞ変更
							if(smd==1) {								// ﾓｰﾄﾞ:2D均等
								if(Sd1<Sd2) {							// 小さい方の倍率採用
									Sd2 = Sd1;
								} else {
									Sd1 = Sd2;
								}
							}
							Sd3 = 0.0, Sd4 = 0.0, Sd5 = 0.0, Si9 = 0;	// 高さ/四隅情報ｸﾘｱ
						} else {										// 四隅の変更
							if(!Si8) {
								Sd4 = fabs(pnt.p[0]-Spa[Si7].p[0]);
								Sd5 = 0.0;
								Si9 = (Si7==0||Si7==1) ? 0 : 1;
							} else {
								Sd4 = 0.0;
								Sd5 = fabs(pnt.p[1]-Spa[Si7].p[1]);
								Si9 = (Si7==0||Si7==3) ? 0 : 1;
							}
						}
					} else {
						Sd3 = fabs(pnt.p[2]-Spnt1.p[2]);				// 画面中心から上部長
						Sd3 = CalcGemH(Sd3);							// 宝石ｶｯﾄの高さ
					}
					SetTaperedVtx(Spd, Sd1, Sd2, Sd3, Sd4, Sd5,
                                  Si9, Spnt1);							// ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ頂点設定
					RedoDirect(14, Spd);								// 直接生成ｺﾏﾝﾄﾞ再実行
					GetAreaBox(2, &box);								// ﾎﾞｯｸｽｻｲｽﾞ取得
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1],
                                         box.bp[1].p[2]-box.bp[0].p[2]);
					Display(0);											// 再描画
				}
			}
		}
	} else if(ms==1) {													// ﾏｳｽ左釦押下
		if(Sphase==0) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spa, 12))!=-1) {		// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Spnt2 = Spa[ht];										// ﾘｻｲｽﾞ更新点設定
				if(ht<4) {												// <<ｻｲｽﾞ変更の場合>>
					Si6 = 0;											// ｻｲｽﾞ変更ﾓｰﾄﾞ
					Si7 = (Spnt2.p[0]<Sbase.p[0]) ? -1					// 基準(中心)点と
                                       : ((Spnt2.p[0]>Sbase.p[0])?1:0);	// 更新点位置関係
					Si8 = (Spnt2.p[1]<Sbase.p[1]) ? -1
                                       : ((Spnt2.p[1]>Sbase.p[1])?1:0);
				} else {												// <<四隅変更の場合>>
					Si6 = 1;											// 四隅変更ﾓｰﾄﾞ
					Si7 = (ht-4)/2;										// 編集情報保存
					Si8 = (ht==5||ht==6||ht==9||ht==10) ? 0 : 1;
				}
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 1;
			} else if(Tracker2.HitTracker(1, 1, pnt, Spb, 1)!=-1) {		// ﾄﾗｯｶｰ2ﾁｪｯｸ
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// ﾏｳｽ左釦解放
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		TrackerGem3(box, Sbase, Sd4, Sd5, Spa);							// ﾄﾗｯｶｰ1取得
		Tracker1.Setup(Spa, Sia, Tens, 12, 0, 7);						// ﾄﾗｯｶｰ1設定
		TrackerGemH(box, Spnt1, Spb);									// ﾄﾗｯｶｰ2取得
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// ﾄﾗｯｶｰ2設定
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==11) {													// <<<DEL>>>
		Sd4 = 0.0, Sd5 = 0.0, Si9 = 0;
		SetTaperedVtx(Spd, Sd1, Sd2, Sd3, Sd4, Sd5, Si9, Spnt1);		// ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ頂点設定
		RedoDirect(14, Spd);											// 直接生成ｺﾏﾝﾄﾞ再実行
		GetAreaBox(2, &box);											// 選択立体のﾎﾞｯｸｽｻｲｽﾞ取得
		TrackerGem3(box, Sbase, Sd4, Sd5, Spa);							// ﾄﾗｯｶｰ1取得
		Tracker1.Setup(Spa, Sia, Tens, 12, 0, 7);						// ﾄﾗｯｶｰ1設定
		TrackerGemH(box, Spnt1, Spb);									// ﾄﾗｯｶｰ2取得
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// ﾄﾗｯｶｰ2設定
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==0) {													// 初期処理
		Sd1 = 0.0, Sd2 = 0.0, Sd4 = 0.0, Sd5 = 0.0;
		Si6 = 0, Si7 = 0, Si8 = 0, Si9 = 0;
		StartMenu();													// ﾒﾆｭｰ開始処理
		CheckCtrlKey1();												// 制御ｷｰ確認(起動時)
		if(!ReadGem(m_GemNo, &Spnt1, &Sd3)) {							// 宝石ｶｯﾄ入力
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
		} else {
			SetNumDisp(2);												// 数値表示初期処理
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			GetAreaBox(2, &box);										// 選択立体のﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &Sbase);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			Sd1 = box.bp[1].p[0]-box.bp[0].p[0];						// 宝石ｶｯﾄのXｻｲｽﾞ
			Sd2 = box.bp[1].p[1]-box.bp[0].p[1];						// 宝石ｶｯﾄのYｻｲｽﾞ
			dd = box.bp[1].p[2]-box.bp[0].p[2];
			m_pNumDisp->SetParam(Sd1, Sd2, dd);
			TrackerGem3(box, Sbase, Sd4, Sd5, Spa);						// ﾄﾗｯｶｰ1取得
			Sia[0]=0, Sia[1]=0, Sia[2]=0, Sia[3]=0, Sia[4]=2, Sia[5]=2;	// ﾄﾗｯｶｰ1色設定
			Sia[6]=2, Sia[7]=2, Sia[8]=2, Sia[9]=2, Sia[10]=2, Sia[11]=2;
			Tens[0] = 7, Tens[1] = 7, Tens[2] = 7, Tens[3] = 7;			// ﾄﾗｯｶｰ1ｵﾌﾟｼｮﾝ設定
			Tens[4] = 6, Tens[5] = 5, Tens[6] = 5, Tens[7] = 6;
			Tens[8] = 6, Tens[9] = 5, Tens[10] = 5, Tens[11] = 6;
			Tracker1.Setup(Spa, Sia, Tens, 12, 0, 7);					// ﾄﾗｯｶｰ1設定
			TrackerGemH(box, Spnt1, Spb);								// ﾄﾗｯｶｰ2取得
			Sib[0] = 0, TensC[0] = 6;									// ﾄﾗｯｶｰ2色/ｵﾌﾟｼｮﾝ設定
			Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);					// ﾄﾗｯｶｰ2表示
		}
		Display(0);														// 再描画
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// ﾏｳｽ右釦押下/終了
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}
