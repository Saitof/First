/////////////////////////////////////////////////////////////////////////////
// 曲面制御点生成部
// CreatCtrlPoint1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/08 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "CreatCtrlPoint2.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlPoint2.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/**************************************************************/
void SetBasePoint(int pln, int pos, BOXTYPE box, PNTTYPE* bas)			//<<< 断面生成座標設定
/**************************************************************/
{
	bas->p[0] = 0.0, bas->p[1] = 0.0, bas->p[2] = 0.0;
	if(pln==0) {
		if(pos<3) {
			bas->p[0] = box.bp[0].p[0];
		} else if(pos>5) {
			bas->p[0] = box.bp[1].p[0];
		} else {
			bas->p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
		}
		if(pos%3==0) {
			bas->p[1] = box.bp[0].p[1];
		} else if(pos%3==2) {
			bas->p[1] = box.bp[1].p[1];
		} else {
			bas->p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
		}
	} else if(pln==1) {
		if(pos<3) {
			bas->p[2] = box.bp[1].p[2];
		} else if(pos>5) {
			bas->p[2] = box.bp[0].p[2];
		} else {
			bas->p[2] = (box.bp[0].p[2]+box.bp[1].p[2])/2.0;
		}
		if(pos%3==0) {
			bas->p[1] = box.bp[0].p[1];
		} else if(pos%3==2) {
			bas->p[1] = box.bp[1].p[1];
		} else {
			bas->p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
		}
	} else {
		if(pos<3) {
			bas->p[2] = box.bp[1].p[2];
		} else if(pos>5) {
			bas->p[2] = box.bp[0].p[2];
		} else {
			bas->p[2] = (box.bp[0].p[2]+box.bp[1].p[2])/2.0;
		}
		if(pos%3==0) {
			bas->p[0] = box.bp[1].p[0];
		} else if(pos%3==2) {
			bas->p[0] = box.bp[0].p[0];
		} else {
			bas->p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
		}
	}
}

/***********************************************************************/
BOOL InitCreat(int flg, int bc, int an, PNTTYPE* pa,int* ia,int* plane)	//<<< 生成初期処理
/***********************************************************************/
{
	PNTTYPE spa;
	VECTYPE vec;
	int     sia, cnt, i, ix, x=0, y=0, z=0, ret;
	double  mad=0.0;

	for(i=1; i<an; i++) {
		x = (fabs(pa[0].p[0]-pa[i].p[0])>EPSILON) ? 1 : x;
		y = (fabs(pa[0].p[1]-pa[i].p[1])>EPSILON) ? 1 : y;
		z = (fabs(pa[0].p[2]-pa[i].p[2])>EPSILON) ? 1 : z;
	}
	CalcNVec(pa, an, &vec);												// 法線ﾍﾞｸﾄﾙ取得
	ret = (x&&y&&z) ? FALSE : TRUE;										// 3面図上になしはｴﾗｰ
	if(!x&&y&&z) {														// 2D法線/平面取得
		ix = 0;
		*plane = 1;
	} else if(!y&&z&&x) {
		ix = 1;
		*plane = 2;
	} else if(!z&&x&&y) {
		ix = 2;
		*plane = 0;
	} else if(!x&&!y&&z) {
		ix = 0;
		*plane = 1;
	} else if(!z&&!x&&y) {
		ix = 0;
		*plane = 1;
	} else if(!y&&!z&&x) {
		ix = 2;
		*plane = 0;
	} else {
		for(i=0; i<DIM; i++) {
			if(fabs(vec.p[i])>mad) {									// 法線ﾍﾞｸﾄﾙ最大方向取得
				mad = fabs(vec.p[i]), ix = i;
			}
		}
		*plane = (ix+1>2) ? 0 : ix+1;
	}
	if(flg&&vec.p[ix]>0.0) {											// ﾍﾞｸﾄﾙ正は面ﾙｰﾌﾟ反転
		ChangeLoop(an, pa, ia);
	}
	if(bc) {															// <<境界閉の場合>>
		for(i=0, cnt=0; i<an; i++) {
			cnt = (ia[i]==1) ? cnt+1 : cnt;								// 折線点数確認
		}
		if(cnt>0) {														// <折線点有の場合>
			while(1) {													// 先頭が折線点まで繰返
				if(ia[0]==1) {
					break;
				}
				spa = pa[0], sia = ia[0];								// 先頭ﾃﾞｰﾀ保存
				for(i=0; i<an-1; i++) {
					pa[i] = pa[i+1], ia[i] = ia[i+1];					// 1つ前にｼﾌﾄ
				}
				pa[an-1] = spa, ia[an-1] = sia;							// 最後に旧先頭設定
			}
		}
	} else {															// <<境界が開の場合>>
		ia[0] = 1, ia[an-1] = 1;										// 先頭&最後は折線点
	}
	for(i=0; i<an; i++) {
		ia[i] = ia[i]*2;												// ﾓｰﾄﾞ2倍
	}
	return ret;
}

/**************************************************************/
void InitCreat2(int flg, int bc, int an, PNTTYPE* pa, int* ia)			//<<< 生成初期処理2
/**************************************************************/
{
	PNTTYPE spa;
	int     sia, cnt, i;

	if(flg) {
		ChangeLoop(an, pa, ia);											// 面ﾙｰﾌﾟ反転
	}
	if(bc) {															// <<境界閉の場合>>
		for(i=0, cnt=0; i<an; i++) {
			cnt = (ia[i]==1) ? cnt+1 : cnt;								// 折線点数確認
		}
		if(cnt>0) {														// <折線点有の場合>
			while(1) {													// 先頭が折線点まで
				if(ia[0]==1) {
					break;
				}
				spa = pa[0], sia = ia[0];								// 先頭ﾃﾞｰﾀ保存
				for(i=0; i<an-1; i++) {
					pa[i] = pa[i+1], ia[i] = ia[i+1];					// 一つ前にｼﾌﾄ
				}
				pa[an-1] = spa, ia[an-1] = sia;							// 最後に旧先頭設定
			}
		}
	} else {															// <<境界開の場合>>
		ia[0] = 1, ia[an-1] = 1;										// 先頭&最後は折線点
	}
	for(i=0; i<an; i++) {
		ia[i] = ia[i]*2;												// ﾓｰﾄﾞ2倍
	}
}

/************************************************************************/
int GetCurveCenter(int plane, int bc, int an, PNTTYPE* pa, PNTTYPE* pnt)//<<< 2D図形中心取得
/************************************************************************/
{
	PNTTYPE cnt, wpa[3];
	VECTYPE vec;
	int     i, d, flg, ret=0;

	an = (bc) ? an-1 : an;												// 境界閉は減算
	CalcCent(pa, an, &cnt);												// 2D中心点取得
	*pnt = cnt;
	d = (plane-1<0) ? 2 : plane-1;										// 2D法線方向取得
	if(bc==1) {															// <<境界閉の場合>>
		ret = 1;														// 初期は中心が2D中
		wpa[0] = pa[0], wpa[1] = pa[1], wpa[2] = cnt;					// <1点目の確認>
		CalcNVec(wpa, 3, &vec);											// 法線ﾍﾞｸﾄﾙ取得
		flg = (vec.p[d]>0) ? 1 : 0;										// ﾍﾞｸﾄﾙ正負
		for(i=1; i<an-1; i++) {
			wpa[0] = pa[i], wpa[1] = pa[i+1], wpa[2] = cnt;				// <2点目以降の確認>
			CalcNVec(wpa, 3, &vec);										// 法線ﾍﾞｸﾄﾙ取得
			if(vec.p[d]>0) {											// <ﾍﾞｸﾄﾙ正>
				if(flg!=1) {											// !=1 ==> 2D外
					ret = 0; break;
				}
			} else {													// <ﾍﾞｸﾄﾙ負>
				if(flg!=0) {											// !=0 ==> 2D外
					ret = 0; break;
				}
			}
		}
		wpa[0] = pa[an-1], wpa[1] = pa[0], wpa[2] = cnt;				// <最終点確認>
		CalcNVec(wpa, 3, &vec);											// 法線ﾍﾞｸﾄﾙ取得
		if(vec.p[d]>0) {												// <ﾍﾞｸﾄﾙ正>
			ret = (flg!=1) ? 0 : ret;									// !=1 ==> 2D外
		} else {														// <ﾍﾞｸﾄﾙ負>
			ret = (flg!=0) ? 0 : ret;									// !=0 ==> 2D外
		}
	}
	return ret;
}

/*********************************************************/
void ChangeRotate2D(int tn, int flg, int an, PNTTYPE* pa)				//<<< 2D断面回転更新
/*********************************************************/
{
	VECTYPE vec;
	int     i, j, wan;
	double  rot[3][3], th=0.0;

	wan = an+flg;														// 実配列数
	CalcNVec(pa, an, &vec);												// 法線ﾍﾞｸﾄﾙ取得
	for(i=0; i<DIM; i++) {
		if(fabs(vec.p[i])>th) {											// ﾍﾞｸﾄﾙ最大方向取得
			th = fabs(vec.p[i]), j = i;
		}
	}
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 0.0, vec.p[j] = -1.0;	// 回転ﾍﾞｸﾄﾙ設定
	th = PI*tn/2.0;														// 回転角度
	SetRotateMatrix(vec, th, rot);										// 回転ﾏﾄﾘｯｸｽ作成
	for(i=0; i<wan; i++) {
		VecRotate(&pa[i], rot);											// 2D断面制御点回転
	}
}

/******************************************************************/
void InitSweep(PNTTYPE pos, int flg, int pln, int an, PNTTYPE* pa)		//<<< 掃引初期処理
/******************************************************************/
{
	VECTYPE vec;
	int     i, wan;
	double  rot[3][3];

	wan = an+flg;														// 実配列数
	if(pln==0) {														// <2D平面:XY>
		for(i=0; i<wan; i++) {											// Z値補正
			pa[i].p[2] = 0.0;
		}
		pos.p[2] = 0.0;
		vec.p[0] = 0.0, vec.p[1] = 1.0, vec.p[2] = 0.0;					// Y軸で90度回転
		SetRotateMatrix(vec, PI/2.0, rot);
		for(i=0; i<wan; i++) {
			VecRotate(&pa[i], rot);
		}
		VecRotate(&pos, rot);
	} else if(pln==2) {													// <2D平面:ZX>
		for(i=0; i<wan; i++) {											// Y値補正
			pa[i].p[1] = 0.0;
		}
		pos.p[1] = 0.0;
		vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 1.0;					// Z軸で-90度回転
		SetRotateMatrix(vec, -PI/2.0, rot);
		for(i=0; i<wan; i++) {
			VecRotate(&pa[i], rot);
		}
		VecRotate(&pos, rot);
	}
	pos.p[0] = 0.0;														// X値補正
	for(i=0; i<wan; i++) {												// 2Dを基準点位置へ移動
		pa[i].p[0] = 0.0, pa[i].p[1] -= pos.p[1], pa[i].p[2] -= pos.p[2];
	}
}

/***********************************************************/
static void ScaleSection(PNTTYPE* pa, int pn, VECTYPE vec,
                         double th, int d1, int d2, int d3)				//<<< 断面拡大
/***********************************************************/
{
	VECTYPE rv1, rv2, fvc;
	int     i, r1=0, r2=0;
	double  rot[3][3], t1, t2, sc;

	t1 = sqrt(vec.p[d1]*vec.p[d1]+vec.p[d2]*vec.p[d2]);
	if(t1>EPSILON) {													// <回転軸がd3でない場合>
		rv1.p[d1] = -vec.p[d2], rv1.p[d2] = vec.p[d1], rv1.p[d3] = 0.0;
		VecN(&rv1);
		t1 = atan2(t1, vec.p[d3]);
		t1 = (t1<-10||t1>10) ? 0.0 : t1;
		SetRotateMatrix(rv1, -t1, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d3方向へ断面回転
		}
		r1 = 1;
	}
	CalcNVec(pa, pn, &fvc);												// 断面法線ﾍﾞｸﾄﾙ取得
	if(fabs(fvc.p[d1])>EPSILON) {										// <断面がd1線上でない場合>
		rv2.p[d1] = 0.0, rv2.p[d2] = 0.0, rv2.p[d3] = -1.0;
		t2 = atan2(fvc.p[d1], fvc.p[d2]);
		t2 = (t2<-10||t2>10) ? 0.0 : t2;
		SetRotateMatrix(rv2, -t2, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d1線上へ断面回転
		}
		r2 = 1;
	}
	sc = fabs(1.0/cos(th/2.0));											// 断面拡大倍率
	for(i=0; i<pn; i++) {
		pa[i].p[d1] = sc*pa[i].p[d1];									// 断面拡大
	}
	if(r2) {															// <d1線上へ断面回転>
		SetRotateMatrix(rv2, t2, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d1線上から断面逆回転
		}
	}
	if(r1) {															// <d3方向へ断面回転>
		SetRotateMatrix(rv1, t1, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d3方向から断面逆回転
		}
	}
}

/************************************************************/
static void Sweep(int md, int flg, int an, PNTTYPE* pa,
                  int* ia, int bc, int bn, PNTTYPE* pb,
                  int* ib, PNTTYPE* pc, int* ic, double* db)			//<<< 掃引制御点生成
/************************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w3 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  wp[3], cpt, pt, wpt;
	VECTYPE  vec, rvc;
	double   rota[3][3], rotb[3][3], th;
	int      i, j, k, wbn;

	if(bn==0) {															// <ﾍﾞｰｽ線指定無>
		for(i=0; i<an; i++) {
			pc[i] = pa[i], ic[i] = ia[i];								// 2D図形使用
		}
		delete[] w1;
		delete[] w2;
		delete[] w3;
		return;
	} else if(bn==1) {													// <ﾍﾞｰｽ線指定1点>
		for(i=0; i<an; i++) {
			for(j=0; j<DIM; j++){
				pc[i].p[j] = pa[i].p[j]+pb[0].p[j];						// 2D図形を1点目に移動
			}
			ic[i] = ia[i];
		}
		delete[] w1;
		delete[] w2;
		delete[] w3;
		return;
	}
	for(i=0; i<bn; i++) {
		w3[i] = pb[i];
	}
	CalcCurvePoint(md, bn, pb, w1, db);									// 通過点=>制御点変換
	for(i=0; i<bn; i++) {
		pb[i] = w1[i];
	}
	for(i=0; i<an; i++) {												// 2D図形複写
		w1[i] = pa[i];
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] = pb[1].p[i]-pb[0].p[i];								// ﾍﾞｰｽ線ﾍﾞｸﾄﾙ取得
	}
	VecN(&vec);															// 単位ﾍﾞｸﾄﾙ化
	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;						// XY平面上の回転軸
	th = atan2(vec.p[1], vec.p[0]);										// XY平面上の回転角度
	th = (th<-10||th>10) ? 0.0 : th;
	if(flg) {
		cpt = pa[an];													// 2D中心有は中心点も処理
	}
	SetRotateMatrix(rvc, th, rota);										// 回転ﾏﾄﾘｯｸｽ作成
	if(fabs(vec.p[2])>EPSILON) {										// <Z方向の傾きあり>
		for(i=0; i<an; i++) {
			VecRotate(&w1[i], rota);									// XY平面上の回転
		}
		if(flg) {
			VecRotate(&cpt, rota);
		}
		th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
		if(th<EPSILON) {
			rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
			th = (vec.p[2]<0.0) ? -PI/2.0 : PI/2.0;
		} else {
			rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;	// Z軸方向の回転軸
			VecN(&rvc);
			th = atan2(vec.p[2], th);									// Z軸方向の回転角度
			th = (th<-10||th>10) ? 0.0 : th;
		}
		SetRotateMatrix(rvc, -th, rota);								// 回転ﾏﾄﾘｯｸｽ作成
	}
	if(flg) {															// <<<2D中心有の場合>>>
		wbn = bn+2;														// 両端点追加
		wpt = cpt;
		VecRotateMove(&wpt, rota, pb[0]);								// ﾍﾞｰｽ線始点垂直に回転移動
		for(i=0; i<an; i++) {											// <<ﾍﾞｰｽ線始点処理>>
			pc[i*wbn] = wpt, ic[i*wbn] = ia[i]+1;						// 始点中心点
			pt = w1[i];
			VecRotateMove(&pt, rota, pb[0]);							// ﾍﾞｰｽ線始点垂直に回転移動
			pc[i*wbn+1] = pt, ic[i*wbn+1] = ia[i]+1;
			VecRotate(&w1[i], rota);									// 始点垂直に回転
		}
		VecRotate(&cpt, rota);											// 始点垂直に回転
		for(i=1; i<bn-1; i++) {											// <<ﾍﾞｰｽ線中間点処理>>
			CalcRotAngleVec(&pb[i-1], &th, &vec);						// 前後点の角度取得
			SetRotateMatrix(vec, th/2.0, rota);							// 今回点用ﾏﾄﾘｯｸｽ
			SetRotateMatrix(vec, th, rotb);								// 次回点用ﾏﾄﾘｯｸｽ
			if(!ib[i]) {												// <掃引方向が曲線点>
				for(j=0; j<an; j++) {
					pt = w1[j];
					VecRotateMove(&pt, rota, pb[i]);					// 前後点中間角度に回転移動
					pc[j*wbn+i+1] = pt, ic[j*wbn+i+1] = ia[j]+ib[i];
					VecRotate(&w1[j], rotb);							// 次点垂直に回転
				}
			} else {													// <掃引方向が折線点>
				for(j=0; j<an; j++) {
					w2[j] = w1[j];
					VecRotate(&w2[j], rota);							// 前後点中間角度に回転
					VecRotate(&w1[j], rotb);							// 次点垂直に回転
				}
				ScaleSection(w2, an, vec, th, 0, 1, 2);					// 断面拡大
				for(j=0; j<an; j++) {									// 掃引点に移動
					for(k=0; k<DIM; k++) {
						pc[j*wbn+i+1].p[k] = w2[j].p[k]+pb[i].p[k];
					}
					ic[j*wbn+i+1] = ia[j]+ib[i];
				}
			}
			VecRotate(&cpt, rotb);										// 次点垂直に回転
		}
		for(j=0; j<DIM; j++) {
			cpt.p[j] += pb[bn-1].p[j];									// 終点移動
		}
		for(i=0; i<an; i++) {											// <<ﾍﾞｰｽ線終点処理>>
			pt = w1[i];
			for(j=0; j<DIM; j++) {
				pt.p[j] += pb[bn-1].p[j];								// 終点移動
			}
			pc[i*wbn+bn] = pt, ic[i*wbn+bn] = ia[i]+1;					// 終点中心点
			pc[i*wbn+bn+1] = cpt, ic[i*wbn+bn+1] = ia[i]+1;
		}
	} else {															// <<<2D中心無の場合>>>
		for(i=0; i<an; i++) {											// <<ﾍﾞｰｽ線始点処理>>
			pt = w1[i];
			VecRotateMove(&pt, rota, pb[0]);							// ﾍﾞｰｽ線始点垂直に回転移動
			pc[i*bn] = pt, ic[i*bn] = ia[i]+1;
			VecRotate(&w1[i], rota);									// 始点垂直に回転
		}
		for(i=1; i<bn-1; i++) {											// <<ﾍﾞｰｽ線中間点処理>>
			CalcRotAngleVec(&pb[i-1], &th, &vec);						// 前後点の角度取得
			SetRotateMatrix(vec, th/2.0, rota);							// 今回点用ﾏﾄﾘｯｸｽ
			SetRotateMatrix(vec, th, rotb);								// 次回点用ﾏﾄﾘｯｸｽ
			if(!ib[i]) {												// <掃引方向が曲線点>
				for(j=0; j<an; j++) {
					pt = w1[j];
					VecRotateMove(&pt, rota, pb[i]);					// 前後点中間角度に回転移動
					pc[j*bn+i] = pt, ic[j*bn+i] = ia[j]+ib[i];
					VecRotate(&w1[j], rotb);							// 次点垂直に回転
				}
			} else {													// <掃引方向が折線点>
				for(j=0; j<an; j++) {
					w2[j] = w1[j];
					VecRotate(&w2[j], rota);							// 前後点中間角度に回転
					VecRotate(&w1[j], rotb);							// 次点垂直に回転
				}
				ScaleSection(w2, an, vec, th, 0, 1, 2);					// 断面拡大
				for(j=0; j<an; j++) {									// 掃引点に移動
					for(k=0; k<DIM; k++) {
						pc[j*bn+i].p[k] = w2[j].p[k]+pb[i].p[k];
					}
					ic[j*bn+i] = ia[j]+ib[i];
				}
			}
		}
		if(!bc) {														// <<<境界:開の場合>>>
			for(i=0; i<an; i++) {										// <<ﾍﾞｰｽ線終点処理>>
				pt = w1[i];
				for(j=0; j<DIM; j++) {
					pt.p[j] += pb[bn-1].p[j];							// 終点移動
				}
				pc[i*bn+bn-1] = pt, ic[i*bn+bn-1] = ia[i]+1;
			}
		} else {														// <<<境界:閉の場合>>>
			wp[0] = pb[bn-2], wp[1] = pb[bn-1], wp[2] = pb[0];			// 終点と始点を連結
			CalcRotAngleVec(wp, &th, &vec);								// 前後点の角度取得
			SetRotateMatrix(vec, th/2.0, rota);							// 今回点用ﾏﾄﾘｯｸｽ
			SetRotateMatrix(vec, th, rotb);								// 次回点用ﾏﾄﾘｯｸｽ
			if(!ib[bn-1]) {												// <掃引方向が曲線点>
				for(i=0; i<an; i++) {
					pt = w1[i];
					VecRotateMove(&pt, rota, pb[bn-1]);					// 終了断面
					pc[i*bn+bn-1] = pt, ic[i*bn+bn-1] = ia[i]+ib[bn-1];
					VecRotate(&w1[i], rotb);							// 次点垂直に回転
				}
			} else {													// <掃引方向が折線点>
				for(i=0; i<an; i++) {
					w2[i] = w1[i];
					VecRotate(&w2[i], rota);							// 前後点中間角度に回転
					VecRotate(&w1[i], rotb);							// 次点垂直に回転
				}
				ScaleSection(w2, an, vec, th, 0, 1, 2);					// 断面拡大
				for(i=0; i<an; i++) {									// 終了断面
					for(j=0; j<DIM; j++) {
						pc[i*bn+bn-1].p[j] = w2[i].p[j]+pb[bn-1].p[j];
					}
					ic[i*bn+bn-1] = ia[i]+ib[bn-1];
				}
			}
			wp[0] = pb[bn-1], wp[1] = pb[0], wp[2] = pb[1];				// 終点と始点を連結
			CalcRotAngleVec(wp, &th, &vec);								// 前後点の角度取得
			SetRotateMatrix(vec, th/2.0, rota);							// 今回点用ﾏﾄﾘｯｸｽ
			if(!ib[0]) {												// <掃引方向が曲線点>
				for(i=0; i<an; i++) {
					pt = w1[i];
					VecRotateMove(&pt, rota, pb[0]);					// 開始断面
					pc[i*bn] = pt, ic[i*bn] = ia[i]+ib[0];
				}
			} else {													// <掃引方向が折線点>
				for(i=0; i<an; i++) {
					VecRotate(&w1[i], rota);							// 前後点中間角度に回転
				}
				ScaleSection(w1, an, vec, th, 0, 1, 2);					// 断面拡大
				for(i=0; i<an; i++) {									// 開始断面
					for(j=0; j<DIM; j++) {
						pc[i*bn].p[j] = w1[i].p[j]+pb[0].p[j];
					}
					ic[i*bn] = ia[i]+ib[0];
				}
			}
		}
	}
	for(i=0; i<bn; i++) {
		pb[i] = w3[i];													// 通過点はﾍﾞｰｽ線を戻す
	}
	delete[] w1;
	delete[] w2;
	delete[] w3;
}

/*******************************************************************/
void CalcSweepPoint(int fg, int sw, int md, int sf, int an, int ac,
                    int bn, PNTTYPE* pa, int* ia, PNTTYPE* pb,
                    int* ib, PNTTYPE* pc, int* ic, double* db)			//<<< 掃引体制御点計算
/*******************************************************************/
{
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCCtA*2];
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int*     is = new int[View->m_MaxCCtA*2];
	int      i, rp, sm;
	double   wt;

	if(!sw) {															// <通常曲面>
		Sweep(md, sf, an, pa, ia, 0, bn, pb, ib, pw, ic, db);			// 掃引制御点生成
		if(!fg) {
			rp = View->GetCurveRepeat();								// 曲線分割数取得
			wt = View->GetCurveWeight();								// 曲線重み係数取得
			sm = View->GetSolidMode();									// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
			CrtSurface(bn, an, 0, ac, rp, wt, sm, pw, ic);				// 曲面生成(初回)
		} else {
			RedoCrtSurface(2*sf+bn, an, 0, ac, pw, ic);					// 曲面生成(2回目以降)
		}
		CalcSurfacePoint(md, 1, 1, bn, an, 0, ac, wt, ic, pw, pc);		// 制御点=>通過点変換
	} else {															// <掃引曲面>
		for(i=0; i<bn; i++) {
			ps[i] = pb[i], is[i] = ib[i];								// 掃引ﾍﾞｰｽ線設定
		}
		is[0] = 1, is[bn-1] = 1;										// 始点終点は折線
		for(i=0; i<an; i++) {
			ps[bn+i] = pa[i], is[bn+i] = ia[i];							// 断面曲線設定
		}
		if(!fg) {
			rp = View->GetCurveRepeat();								// 曲線分割数取得
			wt = View->GetCurveWeight();								// 曲線重み係数取得
			sm = View->GetSolidMode();									// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
			CrtSweepSurf(bn, an, 0, ac, rp, wt, sm, -1, ps, is);		// 掃引曲面生成(初回)
		} else {
			RedoCrtSweepSurf(bn, an, 0, ac, ps, is);					// 掃引曲面生成(2回目以降)
		}
		SetSweepCtrl(-1, bn, an, 0, ps, is, pc, ic);					// 掃引曲面制御点変換
	}
	delete[] ps;
	delete[] pw;
	delete[] is;
}

/*************************************************************/
BOOL InsertSweepPoint(int fg, int n1, int n2,
                      int* dv, int* pn, PNTTYPE* pa, int* ea)			//<<< 掃引体中間制御点挿入
/*************************************************************/
{
	PNTTYPE pnt;
	BOOL    ret=TRUE;
	int     i, j, dn;

	if(!fg) {															// ﾀﾞｲｱﾛｸﾞ省略無の場合
		dn = *dv;
		CInputDlg dlg(10);
		dlg.SetValue(dn);												// 既存値表示
		if(dlg.DoModal()==IDOK) {										// ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&dn);											// 掃引分割数取得
			*dv = dn;
		} else {														// ｷｬﾝｾﾙは中止
			ret = FALSE;
		}
	} else {															// ﾀﾞｲｱﾛｸﾞ省略は0
		dn = 0;
	}
	if(ret) {
		if(n1*(2*n2+2+dn)>View->m_MaxSCtl) {							// <<制御点上限より大>>
			AfxMessageBox(IDS_MANY_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
		} else {														// <<制御点上限以下>>
			if(dn>0) {													// <掃引分割数>0>
				for(i=0; i<DIM; i++) {
					pnt.p[i] = pa[1].p[i]-pa[0].p[i];					// 始終点間変位量
					pnt.p[i] = pnt.p[i]/(dn+1);							// 挿入数分分割
				}
				pa[dn+1] = pa[1], ea[dn+1] = ea[1];						// 終点を後にｼﾌﾄ
				*pn = 2+dn;												// 新制御点数
				for(i=1; i<=dn; i++) {
					for(j=0; j<DIM; j++) {
						pa[i].p[j] = pa[0].p[j]+pnt.p[j]*i;				// 挿入分制御点追加
					}
					ea[i] = 0;
				}
			}
		}
	}
	return ret;
}

/*********************************************************************/
void SetCircleSection(int sn, int md, double ln, int un, int vn,
                      PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ic)	//<<< 円断面割付
/*********************************************************************/
{
	PNTTYPE* pd = new PNTTYPE[View->m_MaxCCtA];
	int*     id = new int[View->m_MaxCCtA];
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, j, ct=0;
	double   d1, d2, rd;

	if(sn==1&&md==0) {													// <<掃引線1本の掃引体>>
		d1 = 2.0*PI/vn, d2 = PI;										// 開始角度,増分角度
		for(i=0; i<vn; i++) {
			pd[i].p[0] = 0.0;											// 断面制御点(YZ)
			pd[i].p[1] = ln*cos(d2);
			pd[i].p[2] = ln*sin(d2);
			id[i] = 0, d2 -= d1;
		}
		for(i=0; i<un; i++) {
			for(j=0; j<vn; j++) {
				pc[ct] = pd[j], ic[ct++] = id[j];						// 掃引点数分断面追加
			}
		}
	} else if(sn==1&&md==1) {											// <<掃引線1本のｿﾘｯﾄﾞ>>
		CalcBox(pa, un, &box);											// 掃引線BOXｻｲｽﾞ取得
		BoxCenter(box, &cpt);											// ﾎﾞｯｸｽｾﾝﾀｰ
		for(i=0, rd=DBL_MAX; i<un; i++) {
			if((d1=PntD(&cpt, &pa[i]))<rd) {							// 中心点からの最短距離
				rd = d1;
			}
		}
		d1 = PI/(vn-1), d2 = PI/2.0;									// 開始角度,増分角度
		for(i=0; i<vn; i++) {
			pd[i].p[0] = 0.0;											// 断面制御点(YZ)
			pd[i].p[1] = rd*cos(d2);
			pd[i].p[2] = rd*sin(d2);
			id[i] = 0, d2 -= d1;
		}
		id[0] = 1, id[vn-1] = 1;										// 始点･終点は折線点
		for(i=0; i<un; i++) {
			d1 = PntD(&cpt, &pa[i])/rd;									// 最短距離との倍率
			for(j=0; j<vn; j++) {
				pc[ct] = pd[j];											// 掃引点数分断面追加
				pc[ct].p[1] = d1*pc[ct].p[1];
				ic[ct++]=id[j];
			}
		}
	} else {															// <<掃引線2本の掃引体>>
		for(i=0; i<un; i++) {
			rd = PntD(&pa[i], &pb[i])/2.0;								// 2点間の距離/2
			d1 = 2.0*PI/vn, d2 = PI;									// 開始角度,増分角度
			for(j=0; j<vn; j++) {
				pc[ct].p[0] = 0.0;										// 断面制御点(YZ)
				pc[ct].p[1] = rd*cos(d2);
				pc[ct].p[2] = rd*sin(d2);
				ic[ct++] = 0, d2 -= d1;
			}
		}
	}
	delete[] pd;
	delete[] id;
}

/********************************************************************/
void SetCurveSection(int sn, int md, int no, int b1, int b2, int b3,
                     BOXTYPE bx, int vn, int vb, PNTTYPE* pa,
                     int* ia, PNTTYPE* pb, int* ib)						//<<< 曲線断面割付
/********************************************************************/
{
	PNTTYPE bas;
	VECTYPE vec;
	int     i, j, bn, pln;
	double  rot[3][3], th=0.0, d1, d2;

	InitCreat(1, vb, vn, pa, ia, &pln);									// 生成初期処理
	for(i=0; i<vn; i++) {												// 曲線ﾓｰﾄﾞを初期に戻す
		ia[i] = ia[i]/2;
	}
	if(sn==1&&md==0) {													// <<掃引線1本の掃引体>>
		bn = b1;														// 基準位置No
	} else if(sn==1&&md==1) {											// <<掃引線1本のｿﾘｯﾄﾞ>>
		bn = 4;															// 基準位置No(仮に中心)
	} else {															// <<掃引線2本の掃引体>>
		if(b3==1) {
			bn = 4;														// 基準位置No
		} else {
			if(b2==0) {
				bn = (b3==0) ? 7 : 1;
			} else if(b2==1) {
				bn = (b3==0) ? 5 : 3;
			} else if(b2==2) {
				bn = (b3==0) ? 3 : 5;
			} else {
				bn = (b3==0) ? 1 : 7;
			}
		}
	}
	SetBasePoint(pln, bn, bx, &bas);									// 断面生成基準点設定
	InitSweep(bas, 0, pln, vn, pa);										// 掃引初期処理
	if((sn==1&&md==1)||sn==2) {											// <<ｿﾘｯﾄﾞ/掃引線2本の場合>>
		vec.p[0] = -1.0, vec.p[1] = 0.0, vec.p[2] = 0.0;				// 回転軸
		if(sn==1&&md==1) {												// <ｿﾘｯﾄﾞの場合>
			d1 = pa[vn-1].p[1]-pa[0].p[1];
			d2 = pa[vn-1].p[2]-pa[0].p[2];
			th = atan2(d1, -d2);										// 回転角度
			th = (th<-10||th>10) ? 0.0 : th;
		} else {														// <掃引線2本の場合>
			if(b2==0) {													// 回転角度
				th = PI;
			} else if(b2==1) {
				th = -PI/2.0;
			} else if(b2==2) {
				th = PI/2.0;
			}
		}
		SetRotateMatrix(vec, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(i=0; i<vn; i++) {
			VecRotate(&pa[i], rot);										// 制御点の回転
		}
		if(sn==1&&md==1) {												// <ｿﾘｯﾄﾞの場合>
			for(i=0; i<DIM; i++) {
				bas.p[i] = (pa[0].p[i]+pa[vn-1].p[i])/2.0;				// 始点-終点の中間点
			}
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pa[i].p[j] -= bas.p[j];								// 中間点が原点へ移動
				}
			}
		}
	}
	for(i=0; i<vn; i++) {
		pb[vn*no+i] = pa[i], ib[vn*no+i] = ia[i];						// 該当位置に断面を設定
	}
}

/***************************************************************/
void CalcCurveSection(int sn, int md, int un, int ub,
                      int vn, int vb, PNTTYPE* pa, PNTTYPE* pb,
                      int* ia, PNTTYPE* pc, int* ic)					//<<< 曲線割付断面計算
/***************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	int*     fg = new int[View->m_MaxCCtA];
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, j, k, ct=0, st, ed, ss;
	double   d1, d2, ln1, ln2;

	if(sn==2) {															// <<掃引線数:2の場合>>
		for(i=0; i<un; i++) {
			if(ia[i]>0) {												// <断面割付済の場合>
				for(j=0, ln1=DBL_MAX, ln2=1.0-DBL_MAX; j<vn; j++) {
					if(pc[i*vn+j].p[1]<ln1) {							// 該当断面の最小Y値
						ln1 = pc[i*vn+j].p[1];
					}
					if(pc[i*vn+j].p[1]>ln2) {							// 該当断面の最大Y値
						ln2 = pc[i*vn+j].p[1];
					}
				}
				ln1 = ln2-ln1;											// 該当断面のY幅
				ln2 = PntD(&pa[i], &pb[i]);								// 掃引線制御点間距離
				d1 = ln2/ln1;											// Y方向の倍率
				d2 = (ia[i]>1) ? 1.0 : d1;								// Z方向の倍率
				for(j=0; j<vn; j++) {
					pc[i*vn+j].p[1] = d1*pc[i*vn+j].p[1];				// Y方向ｻｲｽﾞ調整
					pc[i*vn+j].p[2] = d2*pc[i*vn+j].p[2];				// Z方向ｻｲｽﾞ調整
				}
			}
		}
	} else if(sn==1&&md==1) {											// <<ｿﾘｯﾄﾞの場合>>
		for(i=0, ln1=1.0-DBL_MAX; i<un; i++) {
			if(ia[i]>0) {												// <断面割付済の場合>
				ln2 = PntD(&pc[i*vn], &pc[(i+1)*vn-1]);					// 始点-終点距離
				ln1 = (ln1<ln2) ? ln2 : ln1;							// 始点-終点最大距離
			}
		}
		for(i=0; i<un; i++) {
			if(ia[i]>0) {
				pc[i*vn].p[1] = 0.0;									// 最大距離に始点を調整
				pc[i*vn].p[2] = ln1/2.0;
				pc[(i+1)*vn-1].p[1] = 0.0;								// 最大距離に終点を調整
				pc[(i+1)*vn-1].p[2] = -ln1/2.0;
			}
		}
	}
	if(sn>1) {															// <掃引線2本の場合>
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				p1[i].p[j] = (pa[i].p[j]+pb[i].p[j])/2.0;				// 2本中間点を掃引線とする
			}
		}
	} else {
		for(i=0; i<un; i++) {											// 1本はそのまま掃引線
			p1[i] = pa[i];
		}
	}
	for(i=0, st=-1; i<un; i++) {
		if(ia[i]>0) {
			ct++;														// 割付済断面数ｶｳﾝﾄ
			st = (st==-1) ? i : st;										// 先頭割付済断面保存
		}
	}
	if(ub==0||ct==1) {													// <掃引:開/割付数:1の場合>
		for(i=0; i<st; i++) {
			for(j=0; j<vn; j++) {
				pc[i*vn+j] = pc[st*vn+j], ic[i*vn+j] = ic[st*vn+j];		// 先頭割付済断面を複写
			}
			fg[i] = ia[st];												// ｻｲｽﾞ調整ﾌﾗｸﾞ
		}
	} else {															// 以外は先頭割付済断面保存
		ss = st;
	}
	while(TRUE) {
		for(i=st+1, ed=-1, ln1=0.0; i<un; i++) {
			ln1 += PntD(&p1[i-1], &p1[i]);								// 掃引線間距離合計
			if(ia[i]>0) {												// 次割付済断面保存
				ed = i; break;
			}
		}
		if(ed==-1) {													// 次割付済断面無は中止
			ed = st; break;
		}
		if(st+1<ed) {
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					p2[i].p[j] = pc[ed*vn+i].p[j]-pc[st*vn+i].p[j];		// 前後の割付済断面の変位量
				}
			}
		}
		for(i=st+1, ln2=0.0; i<ed; i++) {
			ln2 += PntD(&p1[i-1], &p1[i]);								// 掃引線間距離累積
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pc[i*vn+j].p[k] = pc[st*vn+j].p[k]+					// 前の割付済断面に
                                      p2[j].p[k]*ln2/ln1;				// 変位量の距離分を加算
				}
				ic[i*vn+j] = (ic[st*vn+j]&&ic[ed*vn+j]) ? 1 : 0;		// 前後の曲線ﾓｰﾄﾞ:1は1
			}
			fg[i] = (ia[st]==2&&ia[ed]==2) ? 2 : 1;						// ｻｲｽﾞ調整ﾌﾗｸﾞ
		}
		st = ed;														// 割付済断面をｼﾌﾄ
	}
	if(ub==0||ct==1) {													// <掃引:開/割付数:1の場合>
		for(i=ed+1; i<un; i++) {
			for(j=0; j<vn; j++) {
				pc[i*vn+j] = pc[ed*vn+j], ic[i*vn+j] = ic[ed*vn+j];		// 末尾割付済断面を複写
			}
			fg[i] = ia[ed];												// ｻｲｽﾞ調整ﾌﾗｸﾞ
		}
	} else {															// <掃引:閉&割付数>2の場合>
		for(i=ed+1, ct=0, ln1=0.0; i<un; i++) {
			ln1 += PntD(&p1[i-1], &p1[i]);								// 掃引線間距離合計
			ct++;
		}
		for(i=0; i<=ss; i++) {											// 掃引線間距離合計
			ln1+=(i==0)?PntD(&p1[un-1], &p1[0]):PntD(&p1[i-1], &p1[i]);
			ct++;
		}
		if(ct>1) {
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					p2[i].p[j] = pc[ss*vn+i].p[j]-pc[ed*vn+i].p[j];		// 前後の割付済断面の変位量
				}
			}
		}
		for(i=ed+1, ln2=0.0; i<un; i++) {
			ln2 += PntD(&p1[i-1], &p1[i]);								// 掃引線間距離累積
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pc[i*vn+j].p[k] = pc[ed*vn+j].p[k]+					// 前の割付済断面に
                                      p2[j].p[k]*ln2/ln1;				// 変位量の距離分を加算
				}
				ic[i*vn+j] = (ic[ed*vn+j]&&ic[ss*vn+j]) ? 1 : 0;		// 前後の曲線ﾓｰﾄﾞ:1は1
			}
			fg[i] = (ia[ed]==2&&ia[ss]==2) ? 2 : 1;						// ｻｲｽﾞ調整ﾌﾗｸﾞ
		}
		for(i=0; i<ss; i++) {											// 掃引線間距離合計
			ln2+=(i==0)?PntD(&p1[un-1], &p1[0]):PntD(&p1[i-1], &p1[i]);
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pc[i*vn+j].p[k] = pc[ed*vn+j].p[k]+					// 前の割付済断面に
                                      p2[j].p[k]*ln2/ln1;				// 変位量の距離分を加算
				}
				ic[i*vn+j] = (ic[ed*vn+j]&&ic[ss*vn+j]) ? 1 : 0;		// 前後の曲線ﾓｰﾄﾞ:1は1
			}
			fg[i] = (ia[ed]==2&&ia[ss]==2) ? 2 : 1;						// ｻｲｽﾞ調整ﾌﾗｸﾞ
		}
	}
	if(sn==2) {															// <<掃引線数:2の場合>>
		for(i=0; i<un; i++) {
			if(ia[i]==0) {												// <今回割付断面の場合>
				for(j=0, ln1=DBL_MAX, ln2=1.0-DBL_MAX; j<vn; j++) {
					if(pc[i*vn+j].p[1]<ln1) {							// 該当断面の最小Y値
						ln1 = pc[i*vn+j].p[1];
					}
					if(pc[i*vn+j].p[1]>ln2) {							// 該当断面の最大Y値
						ln2 = pc[i*vn+j].p[1];
					}
				}
				ln1 = ln2-ln1;											// 該当断面のY幅
				ln2 = PntD(&pa[i], &pb[i]);								// 掃引線制御点間距離
				d1 = ln2/ln1;											// Y方向の倍率
				d2 = (fg[i]>1) ? 1.0 : d1;								// Z方向の倍率
				for(j=0; j<vn; j++) {
					pc[i*vn+j].p[1] = d1*pc[i*vn+j].p[1];				// Y方向ｻｲｽﾞ調整
					pc[i*vn+j].p[2] = d2*pc[i*vn+j].p[2];				// Z方向ｻｲｽﾞ調整
				}
			}
		}
	} else if(sn==1&&md==1) {											// <<ｿﾘｯﾄﾞの場合>>
		CalcBox(pa, un, &box);											// 掃引線BOXｻｲｽﾞ取得
		BoxCenter(box, &cpt);											// ﾎﾞｯｸｽｾﾝﾀｰ
		for(i=0; i<un; i++) {
			for(j=0, ln1=DBL_MAX, ln2=1.0-DBL_MAX; j<vn; j++) {
				ln1 = (pc[i*vn+j].p[1]<ln1) ? pc[i*vn+j].p[1] : ln1;	// 該当断面の最小Y値
				ln2 = (pc[i*vn+j].p[1]>ln2) ? pc[i*vn+j].p[1] : ln2;	// 該当断面の最大Y値
			}
			ln1 = ln2-ln1;												// 該当断面のY幅
			ln2 = PntD(&cpt, &pa[i]);									// 中心点からの距離
			d1 = ln2/ln1;												// Y方向の倍率
			for(j=0; j<vn; j++) {
				pc[i*vn+j].p[1] = d1*pc[i*vn+j].p[1];					// Y方向ｻｲｽﾞ調整
			}
		}
	}
	delete[] p1;
	delete[] p2;
	delete[] fg;
}

/***********************************************************/
static void SweepSection(int no, int un, int ub, int vn,
                         PNTTYPE* pa, int* ia, PNTTYPE* pb,
                         int* ib, PNTTYPE* pc, int* ic)					//<<< 割付断面掃引体生成
/***********************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  wp[3], pt;
	VECTYPE  vec, rvc;
	double   rota[3][3], rotb[3][3], th;
	int      i, j, k, stp;

	for(i=0; i<vn; i++) {
		w1[i] = pb[i];													// 2D図形複写
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] = pa[1].p[i]-pa[0].p[i];								// ﾍﾞｰｽ線ﾍﾞｸﾄﾙ取得
	}
	VecN(&vec);															// 単位ﾍﾞｸﾄﾙ化
	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;						// XY平面上の回転軸
	th = atan2(vec.p[1], vec.p[0]);										// XY平面上の回転角度
	th = (th<-10||th>10) ? 0.0 : th;
	SetRotateMatrix(rvc, th, rota);										// 回転ﾏﾄﾘｯｸｽ作成
	if(fabs(vec.p[2])>EPSILON) {										// <Z方向の傾きあり>
		for(i=0; i<vn; i++) {
			VecRotate(&w1[i], rota);									// XY平面上の回転
		}
		th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
		if(th<EPSILON) {
			rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
			th = (vec.p[2]<0.0) ? -PI/2.0 : PI/2.0;
		} else {
			rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;	// Z軸方向の回転軸
			VecN(&rvc);
			th = atan2(vec.p[2], th);									// Z軸方向の回転角度
			th = (th<-10||th>10) ? 0.0 : th;
		}
		SetRotateMatrix(rvc, -th, rota);								// 回転ﾏﾄﾘｯｸｽ作成
	}
	for(i=0, stp=0; i<vn; i++) {										// <<ﾍﾞｰｽ線始点処理>>
		pt = w1[i];
		VecRotateMove(&pt, rota, pa[0]);								// ﾍﾞｰｽ線始点垂直に回転移動
		if(no==0) {
			pc[i*un] = pt, ic[i*un] = ib[i]*2+1, stp = 1;
		}
		VecRotate(&w1[i], rota);										// 始点垂直に回転
	}
	if(!ub&&stp) {
		delete[] w1;
		delete[] w2;
		return;
	}
	for(i=1, stp=0; i<un-1; i++) {										// <<ﾍﾞｰｽ線中間点処理>>
		CalcRotAngleVec(&pa[i-1], &th, &vec);							// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		SetRotateMatrix(vec, th, rotb);									// 次回点用ﾏﾄﾘｯｸｽ
		if(!ia[i]) {													// <掃引方向が曲線点>
			for(j=0; j<vn; j++) {
				pt = w1[j];
				VecRotateMove(&pt, rota, pa[i]);						// 前後点中間角度に回転移動
				if(no==i) {
					pc[j*un+i] = pt, ic[j*un+i] = ia[i]+ib[j]*2, stp = 1;
				}
				VecRotate(&w1[j], rotb);								// 次点垂直に回転
			}
		} else {														// <掃引方向が折線点>
			for(j=0; j<vn; j++) {
				w2[j] = w1[j];
				VecRotate(&w2[j], rota);								// 前後点中間角度に回転
				VecRotate(&w1[j], rotb);								// 次点垂直に回転
			}
			if(no==i) {
				ScaleSection(w2, vn, vec, th, 0, 1, 2);					// 断面拡大
				for(j=0; j<vn; j++) {									// 掃引点に移動
					for(k=0; k<DIM; k++) {
						pc[j*un+i].p[k] = w2[j].p[k]+pa[i].p[k];
					}
					ic[j*un+i] = ia[i]+ib[j]*2;
				}
				stp = 1;
			}
		}
		if(stp) {
			delete[] w1;
			delete[] w2;
			return;
		}
	}
	if(!ub) {															// <<<境界:開の場合>>>
		for(i=0; i<vn; i++) {											// <<ﾍﾞｰｽ線終点処理>>
			pt = w1[i];
			for(j=0; j<DIM; j++) {										// 終点移動
				pt.p[j] += pa[un-1].p[j];
			}
			pc[(i+1)*un-1] = pt, ic[(i+1)*un-1] = ib[i]*2+1;
		}
	} else {															// <<<境界:閉の場合>>>
		wp[0] = pa[un-2], wp[1] = pa[un-1], wp[2] = pa[0];				// 終点と始点を連結
		CalcRotAngleVec(wp, &th, &vec);									// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		SetRotateMatrix(vec, th, rotb);									// 次回点用ﾏﾄﾘｯｸｽ
		if(!ia[un-1]) {													// <掃引方向が曲線点>
			for(i=0; i<vn; i++) {
				pt = w1[i];
				VecRotateMove(&pt, rota, pa[un-1]);						// 終了断面
				pc[(i+1)*un-1] = pt;
				ic[(i+1)*un-1] = ia[un-1]+ib[i]*2;
				VecRotate(&w1[i], rotb);								// 次点垂直に回転
			}
		} else {														// <掃引方向が折線点>
			for(i=0; i<vn; i++) {
				w2[i] = w1[i];
				VecRotate(&w2[i], rota);								// 前後点中間角度に回転
				VecRotate(&w1[i], rotb);								// 次点垂直に回転
			}
			ScaleSection(w2, vn, vec, th, 0, 1, 2);						// 断面拡大
			for(i=0; i<vn; i++) {										// 終了断面
				for(j=0; j<DIM; j++) {
					pc[(i+1)*un-1].p[j] = w2[i].p[j]+pa[un-1].p[j];
				}
				ic[(i+1)*un-1] = ia[un-1]+ib[i]*2;
			}
		}
		if(no!=0) {
			delete[] w1;
			delete[] w2;
			return;
		}
		wp[0] = pa[un-1], wp[1] = pa[0], wp[2] = pa[1];					// 終点と始点を連結
		CalcRotAngleVec(wp, &th, &vec);									// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		if(!ia[0]) {													// <掃引方向が曲線点>
			for(i=0; i<vn; i++) {
				pt = w1[i];
				VecRotateMove(&pt, rota, pa[0]);						// 開始断面
				pc[i*un] = pt;
				ic[i*un] = ia[0]+ib[i]*2;
			}
		} else {														// <掃引方向が折線点>
			for(i=0; i<vn; i++) {
				VecRotate(&w1[i], rota);								// 前後点中間角度に回転
			}
			ScaleSection(w1, vn, vec, th, 0, 1, 2);						// 断面拡大
			for(i=0; i<vn; i++) {										// 開始断面
				for(j=0; j<DIM; j++) {
					pc[i*un].p[j] = w1[i].p[j]+pa[0].p[j];
				}
				ic[i*un] = ia[0]+ib[i]*2;
			}
		}
	}
	delete[] w1;
	delete[] w2;
}

/**********************************************************************/
static void TwoLineSection(int no, int un, int ub, int vn, int* fg,
                           PNTTYPE* pa, PNTTYPE* pb, int* ia,
                           PNTTYPE* pc, int* ic, PNTTYPE* pd, int* id)	//<<< 2掃引線断面割付
/**********************************************************************/
{
	PNTTYPE wk[3], cpt, pt1, pt2, ptw;
	VECTYPE vc1, vc2, vc3, vc4, nvc, rvc;
	int     i, j, k, st, ed;
	double  rot[3][3], th, d1, d2;

	if(!ub) {															// 開は,0=>1
		st = 0, ed = 1;
	} else {															// 閉は,un-1=>1
		st = un-1, ed = 1;
	}
	for(i=0; i<DIM; i++) {
		ptw.p[i] = (pa[ed].p[i]-pa[st].p[i])+
                   (pb[ed].p[i]-pb[st].p[i])+pa[0].p[i];				// 合算掃引方向位置
	}
	VecV(&pa[0], &pb[0], &vc1);											// 掃引1→掃引2ﾍﾞｸﾄﾙ(vc1)
	VecN(&vc1);
	VecV(&pa[0], &ptw, &vc2);											// 合算掃引方向ﾍﾞｸﾄﾙ(vc2)
	VecN(&vc2);
	VecO(&vc1, &vc2, &vc3);												// ﾍﾞｸﾄﾙの外積(vc1,vc2直交ﾍﾞｸﾄﾙ)
	VecN(&vc3);
	VecO(&vc1, &vc3, &nvc);												// ﾍﾞｸﾄﾙの外積(断面法線ﾍﾞｸﾄﾙ)
	VecN(&nvc);
	for(i=0, j=0, d1=0.0; i<DIM; i++) {
		if((d2=fabs(vc2.p[i]))>d1) {									// 合算掃引方向ﾍﾞｸﾄﾙ最大方向
			d1 = d2, j = i;
		}
	}
	if((vc2.p[j]>0&&nvc.p[j]<0)||(vc2.p[j]<0&&nvc.p[j]>0)) {			// 合算掃引方向ﾍﾞｸﾄﾙと異なる
		for(i=0; i<DIM; i++) {											// 場合は断面法線ﾍﾞｸﾄﾙ反転
			nvc.p[i] *= -1.0;
		}
	}
	if(no==0) {															// <先頭断面の場合>
		for(i=0, j=0, d1=0.0; i<DIM; i++) {
			cpt.p[i] = (pa[0].p[i]+pb[0].p[i])/2.0;						// 掃引1と2の中心点
			if((d2=fabs(vc3.p[i]))>d1) {								// 直交ﾍﾞｸﾄﾙ最大方向
				d1 = d2, j = i;
			}
		}
		d2 = PntD(&pa[0], &pb[0]);										// 2点間距離
		for(i=0; i<DIM; i++) {
			ptw.p[i] = cpt.p[i]+nvc.p[i]*d2;							// 断面法線ﾍﾞｸﾄﾙ方向の点
		}
		wk[0] = pa[0], wk[1] = pb[0], wk[2] = ptw;						// 掃引1+掃引2+法線方向点
		CalcNVec(wk, 3, &vc1);											// 法線ﾍﾞｸﾄﾙ取得
		*fg = (vc1.p[j]<0) ? 0 : 1;										// 基準点ﾌﾗｸﾞ
	}
	pt1.p[0] = 0.0, pt1.p[1] = -1.0, pt1.p[2] = 0.0;					// 断面左側基準点
	pt2.p[0] = 0.0, pt2.p[1] = 1.0,  pt2.p[2] = 0.0;					// 断面右側基準点
	if(fabs(nvc.p[0])>EPSILON||fabs(nvc.p[1])>EPSILON) {				// <<法線ﾍﾞｸﾄﾙがZ軸以外>>
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// 回転軸:Z軸
		th = atan2(nvc.p[1], nvc.p[0]);									// 回転角度
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(i=0; i<vn; i++) {
			VecRotate(&pc[i], rot);										// XY平面上の回転
		}
		VecRotate(&pt1, rot);											// 断面左側基準点の回転
		VecRotate(&pt2, rot);											// 断面右側基準点の回転
		if(fabs(nvc.p[2])>EPSILON) {									// <法線ﾍﾞｸﾄﾙのZ値あり>
			rvc.p[0] = -nvc.p[1], rvc.p[1] = nvc.p[0], rvc.p[2] = 0.0;	// 回転軸:nvcに直交
			VecN(&rvc);													// 単位ﾍﾞｸﾄﾙ化
			th = sqrt(nvc.p[0]*nvc.p[0]+nvc.p[1]*nvc.p[1]);				// XY平面上の長さ
			th = atan2(nvc.p[2], th);									// 回転角度
			th = (th<-10||th>10) ? 0.0 : th;
			SetRotateMatrix(rvc, -th, rot);								// 回転ﾏﾄﾘｯｸｽ作成
			for(i=0; i<vn; i++) {
				VecRotate(&pc[i], rot);									// Z軸方向の回転
			}
			VecRotate(&pt1, rot);										// 断面左側基準点の回転
			VecRotate(&pt2, rot);										// 断面右側基準点の回転
		}
	} else {															// <<法線ﾍﾞｸﾄﾙがZ軸>>
		rvc.p[0] = 0.0, rvc.p[1] = -1.0, rvc.p[2] = 0.0;				// 回転軸:Y軸
		th = (nvc.p[2]<0.0) ? PI/2.0 : -PI/2.0;							// 回転角度
		SetRotateMatrix(rvc, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(i=0; i<vn; i++) {
			VecRotate(&pc[i], rot);										// Z軸方向の回転
		}
		VecRotate(&pt1, rot);											// 断面左側基準点の回転
		VecRotate(&pt2, rot);											// 断面右側基準点の回転
	}
	for(i=1; i<=no; i++) {
		if(i==un-1&&!ub) {												// 終点で開は,un-2=>un-1
			st = un-2, ed = un-1;
		} else if(i==un-1&&ub) {										// 終点で閉は,un-2=>0
			st = un-2, ed = 0;
		} else {														// 以外は,i-1=>i+1
			st = i-1, ed = i+1;
		}
		for(j=0; j<DIM; j++) {
			ptw.p[j] = (pa[ed].p[j]-pa[st].p[j])+
                       (pb[ed].p[j]-pb[st].p[j])+pa[i].p[j];			// 合算掃引方向位置
		}
		VecV(&pa[i], &pb[i], &vc1);										// 掃引1→掃引2ﾍﾞｸﾄﾙ(vc1)
		VecN(&vc1);
		VecV(&pa[i], &ptw, &vc2);										// 合算掃引方向ﾍﾞｸﾄﾙ(vc2)
		VecN(&vc2);
		VecO(&vc1, &vc2, &vc3);											// ﾍﾞｸﾄﾙの外積(vc1,vc2直交ﾍﾞｸﾄﾙ)
		VecN(&vc3);
		VecO(&vc1, &vc3, &vc4);											// ﾍﾞｸﾄﾙの外積(断面法線ﾍﾞｸﾄﾙ)
		VecN(&vc4);
		for(j=0, k=0, d1=0.0; j<DIM; j++) {
			if((d2=fabs(vc2.p[j]))>d1) {								// 合算掃引方向ﾍﾞｸﾄﾙ最大方向
				d1 = d2, k = j;
			}
		}
		if((vc2.p[k]>0&&vc4.p[k]<0)||(vc2.p[k]<0&&vc4.p[k]>0)) {		// 合算掃引方向ﾍﾞｸﾄﾙと異なる
			for(j=0; j<DIM; j++) {										// 場合は断面法線ﾍﾞｸﾄﾙ反転
				vc4.p[j] *= -1.0;
			}
		}
		if(fabs(nvc.p[0]-vc4.p[0])>EPSILON||
           fabs(nvc.p[1]-vc4.p[1])>EPSILON||
           fabs(nvc.p[2]-vc4.p[2])>EPSILON) {
			th = nvc.p[0]*vc4.p[0]+nvc.p[1]*vc4.p[1]+nvc.p[2]*vc4.p[2];
			th = acos(th);												// 回転角度
			th = (th<-10||th>10) ? 0.0 : th;
		} else {
			th = 0.0;
		}
		if(th>EPSILON) {												// <回転角度が0より大>
			for(j=0; j<DIM; j++) {										// 2ﾍﾞｸﾄﾙの成す三角形
				wk[0].p[j] = 0.0;
				wk[1].p[j] = nvc.p[j]*1000.0;
				wk[2].p[j] = vc4.p[j]*1000.0;
			}
			CalcNVec(wk, 3, &rvc);										// 法線ﾍﾞｸﾄﾙ取得
		} else {
			rvc = vc3;
		}
		SetRotateMatrix(rvc, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(j=0; j<vn; j++) {
			VecRotate(&pc[j], rot);										// vc4軸の回転
		}
		VecRotate(&pt1, rot);											// 断面左側基準点の回転
		VecRotate(&pt2, rot);											// 断面右側基準点の回転
		nvc = vc4;														// 今回法線ﾍﾞｸﾄﾙ保存
	}
	if(*fg==0) {														// 掃引線上の基準線ﾍﾞｸﾄﾙ
		VecV(&pa[no], &pb[no], &vc1);
	} else {
		VecV(&pb[no], &pa[no], &vc1);
	}
	VecN(&vc1);															// 単位ﾍﾞｸﾄﾙ化
	VecV(&pt1, &pt2, &vc2);												// 断面上の基準線ﾍﾞｸﾄﾙ
	VecN(&vc2);															// 単位ﾍﾞｸﾄﾙ化
	if(fabs(vc1.p[0]-vc2.p[0])>EPSILON||
       fabs(vc1.p[1]-vc2.p[1])>EPSILON||
       fabs(vc1.p[2]-vc2.p[2])>EPSILON) {
		th = vc1.p[0]*vc2.p[0]+vc1.p[1]*vc2.p[1]+vc1.p[2]*vc2.p[2];
		th = acos(th);													// 回転角度
		th = (th<-10||th>10) ? 0.0 : th;
	} else {
		th = 0.0;
	}
	if(th>EPSILON) {													// <回転角度が0より大>
		for(i=0; i<DIM; i++) {											// 2ﾍﾞｸﾄﾙの成す三角形
			wk[0].p[i] = 0.0;
			wk[1].p[i] = vc2.p[i]*1000.0;
			wk[2].p[i] = vc1.p[i]*1000.0;
		}
		CalcNVec(wk, 3, &rvc);											// 法線ﾍﾞｸﾄﾙ取得
	} else {
		rvc = nvc;
	}
	SetRotateMatrix(rvc, th, rot);										// 回転ﾏﾄﾘｯｸｽ作成
	for(i=0; i<DIM; i++) {
		cpt.p[i] = (pa[no].p[i]+pb[no].p[i])/2.0;						// 掃引1と2の中心点
	}
	for(i=0; i<vn; i++) {
		VecRotateMove(&pc[i], rot, cpt);								// vc4軸の回転
		pd[i*un+no] = pc[i];											// 掃引体制御点情報設定
		id[i*un+no] = ia[no]+ic[i]*2;
	}
}

/****************************************************************/
void CrtSectionSweep(int sn, int md, int un, int ub, int vn,
                     int vb, PNTTYPE* pa, PNTTYPE* pb, int* ia,
                     PNTTYPE* pc, int* ic, PNTTYPE* pd, int* id)		//<<< 割付断面掃引体生成
/****************************************************************/
{
	PNTTYPE cpt, pnt;
	VECTYPE vec;
	BOXTYPE box;
	int     i, j, pln=0, i1, i2, i3;
	double  rot[3][3], th;

	if(sn==1&&md==0) {													// <<掃引線1本の掃引体>>
		for(i=0; i<un; i++) {
			SweepSection(i, un, ub, vn, pa, ia, &pc[i*vn],
                         &ic[i*vn], pd, id);							// 割付断面掃引体生成
		}
	} else if(sn==1&&md==1) {											// <<掃引線1本のｿﾘｯﾄﾞ>>
		CalcBox(pa, un, &box);											// 掃引線BOXｻｲｽﾞ取得
		BoxCenter(box, &cpt);											// ﾎﾞｯｸｽｾﾝﾀｰ
		if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {				// ｿﾘｯﾄﾞ掃引線指定平面
			pln = 1;
		} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
			pln = 2;
		} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
			pln = 0;
		}
		i1 = pln;														// 作業用座標軸設定
		i2 = (i1+1>2) ? 0 : i1+1;
		i3 = (i2+1>2) ? 0 : i2+1;
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				pa[i].p[j] -= cpt.p[j];									// 掃引線中心を原点に移動
			}
		}
		if(pln!=0) {													// <XY平面以外の場合>
			vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = -1.0;			// 回転軸:Z軸
			th = (pln==1) ? -PI/2.0 : PI/2.0;							// YZは左90度/ZXは右90度
			SetRotateMatrix(vec, th, rot);								// 回転ﾏﾄﾘｯｸｽ作成
			for(i=0; i<un*vn; i++) {
				VecRotate(&pc[i], rot);									// 断面制御点回転
			}
			vec.p[0] = (pln==1) ? 0.0 : -1.0;							// YZ平面は回転軸:Y軸
			vec.p[1] = (pln==1) ? -1.0 : 0.0;							// ZX平面は回転軸:X軸
			vec.p[2] = 0.0;
			SetRotateMatrix(vec, th, rot);								// 回転ﾏﾄﾘｯｸｽ作成
			for(i=0; i<un*vn; i++) {
				VecRotate(&pc[i], rot);									// 断面制御点回転
			}
		}
		pnt = pc[0], pnt.p[i1] = 0.0, pnt.p[i2] = 0.0;					// 断面始点座標補正
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				pd[i].p[j] = pnt.p[j]+cpt.p[j];							// 断面始点制御点設定
			}
			id[i] = ic[0]*2;											// 断面始点曲線ﾓｰﾄﾞ設定
		}
		pnt = pc[vn-1], pnt.p[i1] = 0.0, pnt.p[i2] = 0.0;				// 断面終点座標補正
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				pd[(vn-1)*un+i].p[j] = pnt.p[j]+cpt.p[j];				// 断面終点制御点設定
			}
			id[(vn-1)*un+i] = ic[vn-1]*2;								// 断面終点曲線ﾓｰﾄﾞ設定
		}
		vec.p[i1] = 0.0, vec.p[i2] = 0.0, vec.p[i3] = -1.0;
		for(i=0; i<un; i++) {
			th = atan2(pa[i].p[i1], pa[i].p[i2]);						// 回転角度
			th = (th<-10||th>10) ? 0.0 : th;
			SetRotateMatrix(vec, th, rot);								// 回転ﾏﾄﾘｯｸｽ作成
			for(j=1; j<vn-1; j++) {
				pnt = pc[i*vn+j];
				VecRotateMove(&pnt, rot, cpt);							// 断面制御点回転移動
				pd[j*un+i] = pnt;										// 断面制御点･曲線ﾓｰﾄﾞ設定
				id[j*un+i] = ic[i*vn+j]*2;
			}
		}
	} else {															// <<掃引線2本の掃引体>>
		if(!ub) {														// 掃引線:開は両端折線点
			ia[0] = 1, ia[un-1] = 1;
		}
		for(i=0; i<un; i++) {
			TwoLineSection(i, un, ub, vn, &i1, pa, pb, ia,
                           &pc[i*vn], &ic[i*vn], pd, id);				// 2掃引線断面割付
		}
	}
}

/******************************************************************/
void AddCenterPoint(int* un, int vn, int vb, PNTTYPE* pa, int* ia)		//<<< 端面中心点追加
/******************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxCCtA];
	int*     ib = new int[View->m_MaxCPnt];
	int*     ic = new int[View->m_MaxCCtA];
	OBJTYPE* op;
	PNTTYPE  pnt;
	VECTYPE  vec;
	int      uu, mx, i, j, i1, i2, i3, i4;
	double   d1, d2, wt;

	uu = *un;															// U制御点数
	for(i=0,i1=0; i<vn; i++) {											// 開始断面ﾃﾞｰﾀ抽出
		pc[i1] = pa[i*uu], ic[i1++] = ia[i*uu]/2;
	}
	for(i=1, i1=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			if(fabs(pc[0].p[j]-pc[i].p[j])>EPSILON) {					// 開始断面同一点ﾁｪｯｸ
				i1 = 1; break;
			}
		}
		if(i1) {
			break;
		}
	}
	if(i1) {															// <開始断面が同一点でない>
		mx = View->GetCurveRepeat();									// 曲線化繰返数取得
		wt = View->GetCurveWeight();									// 曲線重み係数取得
		CrtCurve(vn, vb, mx, wt, pc, ic);								// 開始断面曲線生成
		GetObjPtr(&op);													// 開始断面曲線OP取得
		i2 = GetVtxNum(op);												// 開始断面頂点数取得
		for(i=0; i<i2; i++) {
			GetVtx(op, i, &pb[i]);										// 開始断面曲線頂点取得
		}
		FreeObj(op, 1);													// 開始断面曲線削除
		CalcNVec(pb, i2, &vec);											// 開始断面法線ﾍﾞｸﾄﾙ取得
		for(i=0, i3=0, d1=0.0; i<DIM; i++) {
			if((d2=fabs(vec.p[i]))>d1) {								// 法線ﾍﾞｸﾄﾙの最大方向
				d1 = d2, i3 = i;
			}
		}
		i3 = (i3+1>2) ? 0 : i3+1;										// 開始断面の近似平面No
		if(GetCurveCenter(i3, vb, i2, pb, &pnt)) {						// <開始断面中心点あり>
			for(i=0, i4=0; i<vn; i++) {
				pb[i4] = pnt;											// 開始断面中心点を追加
				ib[i4++] = ic[i]*2+1;
				for(j=0; j<uu; j++) {
					pb[i4] = pa[i*uu+j], ib[i4++] = ia[i*uu+j];			// 制御点情報を再設定
				}
			}
			uu += 1;													// U制御点数を加算
			for(i=0; i<uu*vn; i++) {									// 曲面制御点を復元
				pa[i] = pb[i], ia[i] = ib[i];
			}
		}
	}
	for(i=0, i1=0; i<vn; i++) {											// 終了断面ﾃﾞｰﾀ抽出
		pc[i1] = pa[(i+1)*uu-1], ic[i1++] = ia[(i+1)*uu-1]/2;
	}
	for(i=1, i1=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			if(fabs(pc[0].p[j]-pc[i].p[j])>EPSILON) {					// 終了断面同一点ﾁｪｯｸ
				i1 = 1; break;
			}
		}
		if(i1) {
			break;
		}
	}
	if(i1) {															// <終了断面が同一点でない>
		mx = View->GetCurveRepeat();									// 曲線化繰返数取得
		wt = View->GetCurveWeight();									// 曲線重み係数取得
		CrtCurve(vn, vb, mx, wt, pc, ic);								// 終了断面曲線生成
		GetObjPtr(&op);													// 終了断面曲線OP取得
		i2 = GetVtxNum(op);												// 終了断面頂点数取得
		for(i=0; i<i2; i++) {
			GetVtx(op, i, &pb[i]);										// 終了断面曲線頂点取得
		}
		FreeObj(op, 1);													// 終了断面曲線削除
		CalcNVec(pb, i2, &vec);											// 終了断面法線ﾍﾞｸﾄﾙ取得
		for(i=0, i3=0, d1=0.0; i<DIM; i++) {
			if((d2=fabs(vec.p[i]))>d1) {								// 法線ﾍﾞｸﾄﾙの最大方向
				d1 = d2, i3 = i;
			}
		}
		i3 = (i3+1>2) ? 0 : i3+1;										// 終了断面の近似平面No
		if(GetCurveCenter(i3, vb, i2, pb, &pnt)) {						// <終了断面中心点あり>
			for(i=0, i4=0; i<vn; i++) {
				for(j=0; j<uu; j++) {
					pb[i4] = pa[i*uu+j], ib[i4++] = ia[i*uu+j];			// 制御点情報を再設定
				}
				pb[i4] = pnt, ib[i4++] = ic[i]*2+1;						// 終了断面中心点を追加
			}
			uu += 1;													// U制御点数を加算
			for(i=0; i<uu*vn; i++) {									// 曲面制御点を復元
				pa[i] = pb[i], ia[i] = ib[i];
			}
		}
	}
	*un = uu;															// U制御点数
	delete[] pb;
	delete[] pc;
	delete[] ib;
	delete[] ic;
}

/*******************************************************************/
void CreatSetCurve(int tn, int sd, int sd1, int sw, int pl, int ps)		//<<< 曲線組合せ
/*******************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pb = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* blp = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* cpt = new PNTTYPE[100];
	PNTTYPE* cpw = new PNTTYPE[100];
	int*     ia = new int[View->m_MaxCCtA];
	int*     ib = new int[View->m_MaxCCtA];
	int*     ic = new int[View->m_MaxSCtA];
	int*     bli = new int[View->m_MaxSCtA];
	int*     cpi = new int[100];
	OBJTYPE* op1, * op2, * op3;
	OIFTYPE* oif;
	CMDTYPE* cmp1, * cmp2;
	PNTTYPE  pnt, bas;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, mx, sm, un, vn, ub, vb, vpl, ad=0, ct1=0, ct1w, ct2;
	double   wk[10], wt, mi1, mi2, mi3, mi4, ma1, ma2, ma3, ma4;

	i1 = pl;															// 断面平面設定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	mi1 = DBL_MAX, mi2 = DBL_MAX, mi3 = DBL_MAX, mi4 = DBL_MAX;
	ma1 = 1.0-DBL_MAX, ma2 = 1.0-DBL_MAX;
	ma3 = 1.0-DBL_MAX, ma4 = 1.0-DBL_MAX;
	SelBaseObjPtr(1, &op1);												// 断面曲線先頭ﾎﾟｲﾝﾀ取得
	while(op1!=NULL) {
		GetObjBox(op1, &box);											// 立体BOXｻｲｽﾞ取得
		pnt.p[i1] = (box.bp[0].p[i1]+box.bp[1].p[i1])/2.0;				// 立体の中心点
		pnt.p[i2] = (box.bp[0].p[i2]+box.bp[1].p[i2])/2.0;
		pnt.p[i3] = 0.0;
		cpt[ct1] = pnt, cpi[ct1++] = 0;
		mi1 = (box.bp[0].p[i1]<mi1) ? box.bp[0].p[i1] : mi1;			// 全断面のBOXｻｲｽﾞ取得
		ma1 = (box.bp[1].p[i1]>ma1) ? box.bp[1].p[i1] : ma1;
		mi2 = (box.bp[0].p[i2]<mi2) ? box.bp[0].p[i2] : mi2;
		ma2 = (box.bp[1].p[i2]>ma2) ? box.bp[1].p[i2] : ma2;
		mi3 = (pnt.p[i1]<mi3) ? pnt.p[i1] : mi3;						// 断面中心点のBOXｻｲｽﾞ取得
		ma3 = (pnt.p[i1]>ma3) ? pnt.p[i1] : ma3;
		mi4 = (pnt.p[i2]<mi4) ? pnt.p[i2] : mi4;
		ma4 = (pnt.p[i2]>ma4) ? pnt.p[i2] : ma4;
		SelNextObjPtr(1, &op1);											// 次断面曲線
	}
	if(ct1==2||(ct1>2&&(fabs(mi3-ma3)<EPSILON||fabs(mi4-ma4)<EPSILON))){// <断面が2個/縦横一列の場合>
		cpt[ct1].p[i1] = ma1+1.0;										// 仮にもう1点を追加
		cpt[ct1].p[i2] = ma2+1.0;
		cpt[ct1].p[i3] = 0.0;
		ct1w = ct1+1;
	} else {
		ct1w = ct1;
	}
	box.bp[0].p[i1] = mi1, box.bp[1].p[i1] = ma1;						// 全断面のBOXｻｲｽﾞ
	box.bp[0].p[i2] = mi2, box.bp[1].p[i2] = ma2;
	SetBasePoint(pl, ps, box, &bas);									// 断面生成座標設定
	SelBaseObjPtr(2, &op2);												// 掃引曲線先頭ﾎﾟｲﾝﾀ取得
	while(op2!=NULL) {
		BaseCmdPtr(1, op2, &cmp2);										// 掃引先頭ｺﾏﾝﾄﾞ取得
		un = GetParaInt(cmp2, 0);										// 制御点数取得
		ub = GetParaInt(cmp2, 1);										// 境界ﾓｰﾄﾞ取得
		for(i=0; i<un; i++) {
			ib[i] = GetParaInt(cmp2, 4+i);								// ﾗｲﾝﾓｰﾄﾞ取得
			GetParaPnt(cmp2, i, &pb[i]);								// 曲線制御点取得
		}
		SetCurve(un, ub, pb);											// 曲線制御点生成
		GetObjPtr(&op3);												// 制御点OP保存
		BaseCmdPtr(1, op2, &cmp2);										// 有効先頭ｺﾏﾝﾄﾞ取得
		NextCmdPtr(&cmp2);												// 曲線編集後ｺﾏﾝﾄﾞ
		while(cmp2!=NULL) {
			RedoCommandCopy(op3, cmp2);									// ｺﾏﾝﾄﾞ複写実行
			NextCmdPtr(&cmp2);											// 次ｺﾏﾝﾄﾞへ
		}
		for(i=0; i<un; i++) {
			GetVtx(op3, i, &pb[i]);										// 立体編集後制御点取得
		}
		FreeObj(op3, 1);												// 編集後立体削除
		if(ct1==1) {													// <<断面:1個の場合>>
			SelBaseObjPtr(1, &op1);										// 断面曲線先頭ﾎﾟｲﾝﾀ取得
			if(op1!=NULL) {
				BaseCmdPtr(1, op1, &cmp1);								// 断面先頭ｺﾏﾝﾄﾞ取得
				vn = GetParaInt(cmp1, 0);								// 制御点数取得
				vb = GetParaInt(cmp1, 1);								// 境界ﾓｰﾄﾞ取得
				for(i=0; i<vn; i++) {
					ia[i] = GetParaInt(cmp1, 4+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp1, i, &pa[i]);						// 曲線制御点取得
				}
				SetCurve(vn, vb, pa);									// 曲線制御点生成
				GetObjPtr(&op3);										// 制御点OP保存
				BaseCmdPtr(1, op1, &cmp1);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp1);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp1!=NULL) {
					RedoCommandCopy(op3, cmp1);							// ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp1);									// 次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<vn; i++) {
					GetVtx(op3, i, &pa[i]);								// 立体編集後制御点取得
				}
				FreeObj(op3, 1);										// 編集後立体削除
				InitCreat(1, vb, vn, pa, ia, &vpl);						// 生成初期処理
				if(!sw) {												// <通常曲面>
					j = GetVtxNum(op1);									// 曲面頂点数取得
					for(i=0; i<j; i++) {
						GetVtx(op1, i, &pc[i]);							// 曲面頂点座標取得
					}
					ad = GetCurveCenter(vpl, vb, j, pc, &pnt);			// 2D図形中心取得
					ad = ((!sd)||ub||sd1) ? 0 : ad;						// OFFは断面無
					if(ad) {											// 2D中心追加
						pa[vn] = pnt;
					}
				} else {												// <掃引曲面>
					for(i=0; i<vn; i++) {
						ia[i] /= 2;										// 曲線ﾓｰﾄﾞ変換(0/1)
					}
				}
				if(vn*(2*ad+un)<=View->m_MaxSCtl) {						// <制御点上限以下>
					InitSweep(bas, ad, vpl, vn, pa);					// 掃引初期処理
					if(tn>0) {
						ChangeRotate2D(tn, ad, vn, pa);					// 2D断面回転更新
					}
					if(!sw) {											// <通常曲面>
						Sweep(0, ad, vn, pa, ia, ub, un, pb, ib,
                              pc, ic, wk);								// 掃引制御点生成
						mx = View->GetCurveRepeat();					// 曲線化繰返数取得
						wt = View->GetCurveWeight();					// 曲線重み係数取得
						sm = View->GetSolidMode();						// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
						CrtSurface(2*ad+un, vn, ub, vb, mx, wt, sm,
                                   pc, ic);								// 曲面生成
					} else {											// <掃引曲面>
						for(i=0; i<vn; i++) {
							pb[un+i] = pa[i], ib[un+i] = ia[i];			// 断面曲線分追加
						}
						mx = View->GetCurveRepeat();					// 曲線化繰返数取得
						wt = View->GetCurveWeight();					// 曲線重み係数取得
						sm = View->GetSolidMode();						// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
						CrtSweepSurf(un, vn, ub, vb, mx, wt, sm,
                                     -1, pb, ib);						// 掃引曲面生成(初回)
					}
					GetObjPtr(&op3);									// 曲面立体OP取得
					i = GetObjAtr(op3);									// 立体属性取得
					GetObjOif(op3, &oif);								// 立体構成情報ﾎﾟｲﾝﾀ取得
					if(i==20) {
						SetOifName(0, oif, OBJ_NAME03);					// 体構成名(通常掃引体)
					} else if(i==21) {
						SetOifName(0, oif, OBJ_NAME04);					//         (通常掃引体[面体])
					} else if(i==22) {
						SetOifName(0, oif, OBJ_NAME05);					//         (掃引掃引体)
					} else {
						SetOifName(0, oif, OBJ_NAME06);					//         (掃引掃引体[面体])
					}
					OnEdtFlg(1, op3);									// 曲面edtflg:ON
				} else {
					AfxMessageBox(IDS_MANY_CTRLP);						// 制御点上限以上,ｴﾗｰﾒｯｾｰｼﾞ
				}
			}
		} else {														// <<断面:複数の場合>>
			for(i=0; i<ct1w; i++) {
				cpw[i] = cpt[i];
			}
			InitSweep(bas, 0, pl, ct1w, cpw);							// 複数断面の場合は
			if(tn>0) {
				ChangeRotate2D(tn, 0, ct1w, cpw);						// 2D断面回転更新
			}
			Sweep(0, 0, ct1w, cpw, cpi, ub, un, pb, ib, blp, bli, wk);	// 各断面の掃引線取得
			ct2 = 0;
			SelBaseObjPtr(1, &op1);										// 断面曲線先頭ﾎﾟｲﾝﾀ取得
			while(op1!=NULL) {
				BaseCmdPtr(1, op1, &cmp1);								// 断面先頭ｺﾏﾝﾄﾞ取得
				vn = GetParaInt(cmp1, 0);								// 制御点数取得
				vb = GetParaInt(cmp1, 1);								// 境界ﾓｰﾄﾞ取得
				for(i=0; i<vn; i++) {
					ia[i] = GetParaInt(cmp1, 4+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp1, i, &pa[i]);						// 曲線制御点取得
				}
				SetCurve(vn, vb, pa);									// 曲線制御点生成
				GetObjPtr(&op3);										// 制御点OP保存
				BaseCmdPtr(1, op1, &cmp1);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp1);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp1!=NULL) {
					RedoCommandCopy(op3, cmp1);							// ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp1);									// 次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<vn; i++) {
					GetVtx(op3, i, &pa[i]);								// 立体編集後制御点取得
				}
				FreeObj(op3, 1);										// 編集後立体削除
				InitCreat(1, vb, vn, pa, ia, &vpl);						// 生成初期処理
				if(!sw) {												// <通常曲面>
					j = GetVtxNum(op1);									// 曲面頂点数取得
					for(i=0; i<j; i++) {
						GetVtx(op1, i, &pc[i]);							// 曲面頂点座標取得
					}
					ad = GetCurveCenter(vpl, vb, j, pc, &pnt);			// 2D図形中心取得
					ad = ((!sd)||ub||sd1) ? 0 : ad;						// OFFは断面無
					if(ad) {											// 2D中心追加
						pa[vn] = pnt;
					}
				} else {												// <掃引曲面>
					for(i=0; i<vn; i++) {
						ia[i] /= 2;										// 曲線ﾓｰﾄﾞ変換(0/1)
					}
				}
				if(vn*(2*ad+un)<=View->m_MaxSCtl) {						// <制御点上限以下>
					for(i=0; i<un; i++) {
						pb[i] = blp[un*ct2+i], ib[i] = bli[un*ct2+i];	// 掃引情報を抽出
					}
					InitSweep(cpt[ct2], ad, vpl, vn, pa);				// 掃引初期処理
					if(tn>0) {
						ChangeRotate2D(tn, ad, vn, pa);					// 2D断面回転更新
					}
					if(!sw) {											// <通常曲面>
						Sweep(0, ad, vn, pa, ia, ub, un, pb, ib,
                              pc, ic, wk);								// 掃引制御点生成
						mx = View->GetCurveRepeat();					// 曲線化繰返数取得
						wt = View->GetCurveWeight();					// 曲線重み係数取得
						sm = View->GetSolidMode();						// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
						CrtSurface(2*ad+un, vn, ub, vb, mx, wt, sm,
                                   pc, ic);								// 曲面生成
					} else {											// <掃引曲面>
						for(i=0; i<vn; i++) {
							pb[un+i] = pa[i], ib[un+i] = ia[i];			// 断面曲線分追加
						}
						mx = View->GetCurveRepeat();					// 曲線化繰返数取得
						wt = View->GetCurveWeight();					// 曲線重み係数取得
						sm = View->GetSolidMode();						// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
						CrtSweepSurf(un, vn, ub, vb, mx, wt, sm,
                                     -1, pb, ib);						// 掃引曲面生成
					}
					GetObjPtr(&op3);									// 曲面立体OP取得
					i = GetObjAtr(op3);									// 立体属性取得
					GetObjOif(op3, &oif);								// 立体構成情報ﾎﾟｲﾝﾀ取得
					if(i==20) {
						SetOifName(0, oif, OBJ_NAME03);					// 体構成名(通常掃引体)
					} else if(i==21) {
						SetOifName(0, oif, OBJ_NAME04);					//         (通常掃引体[面体])
					} else if(i==22) {
						SetOifName(0, oif, OBJ_NAME05);					//         (掃引掃引体)
					} else {
						SetOifName(0, oif, OBJ_NAME06);					//         (掃引掃引体[面体])
					}
					OnEdtFlg(1, op3);									// 曲面edtflg:ON
				} else {
					AfxMessageBox(IDS_MANY_CTRLP);						// 制御点上限以上,ｴﾗｰﾒｯｾｰｼﾞ
				}
				SelNextObjPtr(1, &op1);									// 次断面曲線
				ct2++;
			}
		}
		SelNextObjPtr(2, &op2);											// 次掃引曲線
	}
	delete[] pa;
	delete[] pb;
	delete[] pc;
	delete[] blp;
	delete[] cpt;
	delete[] cpw;
	delete[] ia;
	delete[] ib;
	delete[] ic;
	delete[] bli;
	delete[] cpi;
}
