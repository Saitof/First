/////////////////////////////////////////////////////////////////////////////
// 曲面制御点生成部プ
// CreatCtrlPoint2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "EditCtrlTracker.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/*******************************/
BOOL InputBasePoint1(int* base)											//<<< 断面生成位置入力1
/*******************************/
{
	CSetBaseDlg2 dlg;
	dlg.SetValue(*base);												// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(base);												// 基準点取得
		return TRUE;
	} else {
		return FALSE;
	}
}

/*****************************************************/
BOOL InputBasePoint2(int* bas1, int* bas2, int* size)					//<<< 断面生成位置入力2
/*****************************************************/
{
	CSetBaseDlg3 dlg;
	dlg.SetValue(*bas1, *bas2, *size);									// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(bas1, bas2, size);									// 基準点取得
		return TRUE;
	} else {
		return FALSE;
	}
}

/*********************************************/
void ChangeLoop(int an, PNTTYPE* pa, int* ia)							//<<< 面ﾙｰﾌﾟ反転
/*********************************************/
{
	PNTTYPE spa;
	int     sia, st, ed;

	st = 0, ed = an-1;													// 最小/最大番号
	while(st<ed) {														// <最大<最小迄>
		spa = pa[st], sia = ia[st];										// 最小側保存
		pa[st] = pa[ed], ia[st] = ia[ed];								// 最大側->最小側
		pa[ed] = spa, ia[ed] = sia;										// 最小側->最大側
		ed--, st++;														// 最大減算/最小加算
	}
}

/********************************************************/
void InitRevolve(int bc, int an, PNTTYPE* pa,
                 int* ia, int* bn, PNTTYPE* pb, int* ib)				//<<< 回転初期処理
/********************************************************/
{
	PNTTYPE pnt;
	int     i, j, cnt=0;

	if(bc) {															// <<境界閉の場合>>
		for(i=0; i<an-1; i++) {
			if(ia[i]!=0&&ia[i+1]!=0) {									// <連続折線ﾓｰﾄﾞ>
				for(j=0; j<DIM; j++) {
					pnt.p[j] = (pa[i].p[j]+pa[i+1].p[j])/2.0;			// 中間点座標取得
				}
				pb[cnt] = pnt, ib[cnt++] = 0;							// ﾄﾗｯｶｰ配列設定
			}
		}
		if(ia[an-1]!=0&&ia[0]!=0) {										// <終点&始点が折線>
			if(ia[an-1]!=0&&ia[0]!=0) {
				for(j=0; j<DIM; j++) {
					pnt.p[j] = (pa[an-1].p[j]+pa[0].p[j])/2.0;			// 中間点座標取得
				}
				pb[cnt] = pnt, ib[cnt++] = 0;							// ﾄﾗｯｶｰ配列設定
			}
		}
	}
	*bn = cnt;															// ﾄﾗｯｶｰ配列数
}

/*********************************************************************/
static void Revolve(int flg, int plane, double wt, int an,
                    PNTTYPE* pa, int* ia, PNTTYPE p1, PNTTYPE p2,
                    double rt, int div, int sf, PNTTYPE* pc, int* ic)	//<<< 回転制御点生成
/*********************************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	int*     i2 = new int[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	PNTTYPE  cpt, scp, sc1, sc2;
	VECTYPE  vec, wvc;
	int      md, i, j, d1, d2, d3, st, ed, cnt=0;
	double   rot[3][3], drt, th, dth, ath;

	md = (fabs(rt-360.0)<EPSILON) ? 0 : 1;								// 境界:開閉
	drt = PI*rt/180.0;													// 回転立体生成角度
	d1 = plane;															// 座標軸設定
	d2 = (d1+1>2) ? 0 : d1+1;
	d3 = (d2+1>2) ? 0 : d2+1;
	for(i=0; i<an; i++) {
		w1[i].p[d1] = pa[i].p[d1]-p1.p[d1];								// 2D図形複写
		w1[i].p[d2] = pa[i].p[d2]-p1.p[d2];
		w1[i].p[d3] = 0.0;												// 指定平面外座標
		i1[i] = ia[i];													// 曲線ﾓｰﾄﾞ複写
	}
	th = atan2(p2.p[d2]-p1.p[d2], p2.p[d1]-p1.p[d1]);					// d2軸に対する角度
	th = (th<-10||th>10) ? 0.0 : th;
	th = th-PI/2.0;
	vec.p[d1] = 0.0, vec.p[d2] = 0.0, vec.p[d3] = -1.0;					// 回転軸設定
	SetRotateMatrix(vec, th, rot);										// 回転ﾏﾄﾘｯｸｽ作成
	dth = (!md) ? drt/div : drt/(div-1);								// 1分割分回転角度
	for(i=0; i<an; i++) {
		VecRotate(&w1[i], rot);											// 回転軸中心へ回転移動
	}
	if(md) {
		for(i=0; i<an; i++) {
			w2[i] = w1[i], i2[i] = i1[i];								// 2D情報保存
		}
	}
	if(flg>1) {															// <<2D境界が開>>
		w1[an].p[d1] = 0.0;												// 回転軸と水平線交点追加
		w1[an].p[d2] = w1[an-1].p[d2];
		w1[an].p[d3] = 0.0;
		w1[an+1].p[d1] = 0.0;
		w1[an+1].p[d2] = w1[0].p[d2];
		w1[an+1].p[d3] = 0.0;
		CalcNVec(w1, an+2, &wvc);										// 法線ﾍﾞｸﾄﾙ取得
		if(wvc.p[d3]>0.0) {
			ChangeLoop(an, w1, i1);										// ﾍﾞｸﾄﾙ負は面ﾙｰﾌﾟ反転
			if(md) {
				ChangeLoop(an, w2, i2);
			}
		}
	}
	if(flg>0) {															// <<2D境界が閉>>
		CalcCent(w1, an, &cpt);											// 2D図形中心取得
		if(cpt.p[d1]<0) {
			ChangeLoop(an, w1, i1);										// 中心負は面ﾙｰﾌﾟ反転
			if(md) {
				ChangeLoop(an, w2, i2);
			}
		}
	}
	if(flg==3) {														// <<回転中心点有の場合>>
		for(j=0; j<div; j++) {											// <開始点回転中心>
			pc[cnt].p[d1] = 0.0;										// 制御点配列作成
			pc[cnt].p[d2] = w1[0].p[d2];
			pc[cnt].p[d3] = 0.0;
			ic[cnt++] = i1[0]+1;										// 曲線ﾓｰﾄﾞ設定
		}
	}
	if(flg>0&&sf) {														// <断面中心有の場合>
		scp.p[d1] = pa[an].p[d1]-p1.p[d1];								// 断面中心複写
		scp.p[d2] = pa[an].p[d2]-p1.p[d2];
		scp.p[d3] = 0.0;
		VecRotate(&scp, rot);											// 回転軸中心へ回転移動
		sc1.p[d1] = scp.p[d1]*cos(0.0);									// 開始断面中心点
		sc1.p[d2] = scp.p[d2];
		sc1.p[d3] = scp.p[d1]*sin(0.0);
		sc2.p[d1] = scp.p[d1]*cos(drt);									// 終了断面中心点
		sc2.p[d2] = scp.p[d2];
		sc2.p[d3] = scp.p[d1]*sin(drt);
	}
	if(!md) {
		st = 0, ed = div;
	} else {
		st = 1, ed = div-1;
	}
	for(i=0; i<an; i++) {
		ath = 0.0;
		if(md) {														// <円弧開始断面>
			if(flg>0&&sf) {												// 断面中心有
				pc[cnt] = sc1, ic[cnt++] = i1[i]+1;
			}
			pc[cnt].p[d1] = w2[i].p[d1]*cos(0.0);
			pc[cnt].p[d2] = w2[i].p[d2];
			pc[cnt].p[d3] = w2[i].p[d1]*sin(0.0);
			ic[cnt++] = i1[i]+1;
			ath = dth;
		}
		for(j=st; j<ed; ath+=dth, j++) {								// <通常断面>
			pc[cnt].p[d1] = w1[i].p[d1]*cos(ath);						// 制御点配列作成
			pc[cnt].p[d2] = w1[i].p[d2];
			pc[cnt].p[d3] = w1[i].p[d1]*sin(ath);
			ic[cnt++] = i1[i];											// 曲線ﾓｰﾄﾞ設定
		}
		if(md) {														// <円弧終了断面>
			pc[cnt].p[d1] = w2[i].p[d1]*cos(ath);
			pc[cnt].p[d2] = w2[i].p[d2];
			pc[cnt].p[d3] = w2[i].p[d1]*sin(ath);
			ic[cnt++] = i1[i]+1;
			if(flg>0&&sf) {												// 断面中心有
				pc[cnt] = sc2, ic[cnt++] = i1[i]+1;
			}
		}
	}
	if(flg==3) {														// <<回転中心点有の場合>>
		for(j=0; j<div; j++) {											// <最終点回転中心>
			pc[cnt].p[d1] = 0.0;										// 制御点配列作成
			pc[cnt].p[d2] = w1[an-1].p[d2];
			pc[cnt].p[d3] = 0.0;
			ic[cnt++] = i1[an-1]+1;										// 曲線ﾓｰﾄﾞ設定
		}
	}
	SetRotateMatrix(vec, -th, rot);										// 回転ﾏﾄﾘｯｸｽ作成
	for(i=0; i<cnt; i++) {
		VecRotateMove(&pc[i], rot, p1);									// 元位置へ回転移動
	}
	d1 = (flg==3) ? an+2 : an;											// 断面方向制御点数
	d2 = (flg>0&&sf) ? div+2 : div;										// 回転方向制御点数
	for(i=0; i<d2; i++) {
		i1[i] = 0;														// 回転方向のﾗｲﾝﾓｰﾄﾞ
	}
	if(!md) {															// 環体(360度回転)の場合
		d3 = 1;															// 境界条件:閉
	} else {															// 扇型(360度未満回転)の場合
		d3 = 0;															// 境界条件:開
		i1[0] = 1;														// 両端のﾗｲﾝﾓｰﾄﾞ:折線
		i1[d2-1] = 1;
		if(flg>0&&sf) {													// 断面有は次も折線
			i1[1] = 1, i1[d2-2] = 1;
		}
	}
	SetCurveMatrix1(1, wt, d3, d2, i1, ma, mb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
	if(flg!=3) {														// 通過点変換範囲設定
		st = 0, ed = d1;
	} else {
		st = 1, ed = d1-1;
	}
	for(i=st; i<ed; i++) {
		for(j=0; j<d2; j++) {
			w1[j] = pc[j+d2*i];
		}
		CalcCurvePoint(1, d2, w1, w2, mb);								// 通過点=>制御点変換
		for(j=0; j<d2; j++) {
			pc[j+d2*i] = w2[j];
		}
	}
	delete[] w1;
	delete[] w2;
	delete[] i1;
	delete[] i2;
	delete[] ma;
	delete[] mb;
}

/******************************************************************/
static void SweepRev(PNTTYPE cpt, int plane, double wt, int an,
                     PNTTYPE* pa, int* ia, PNTTYPE p1, PNTTYPE p2,
                     double rt, int div, PNTTYPE* pb, int* ib)			//<<< 回転用掃引曲面生成
/******************************************************************/
{
	PNTTYPE* wk = new PNTTYPE[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	PNTTYPE  wpt;
	VECTYPE  vec;
	int      i, d1, d2, d3, bc, st, ed, md=0, cnt=0;
	double   rot[3][3], drt, dth, th, ath=0.0;

	drt = PI*rt/180.0;													// 回転立体生成角度
	if(fabs(rt-360.0)<EPSILON) {										// <360度回転体>
		dth = drt/div, st = 0, ed = div, bc = 1;						// 1分割分回転角度
	} else {															// <360度未満回転体>
		dth = drt/(div-1), st = 1, ed = div-1, md = 1, bc = 0;
	}
	d1 = plane;															// 座標軸設定
	d2 = (d1+1>2) ? 0 : d1+1;
	d3 = (d2+1>2) ? 0 : d2+1;
	wpt.p[d1] = cpt.p[d1]-p1.p[d1];										// 2D中心点複写
	wpt.p[d2] = cpt.p[d2]-p1.p[d2];
	wpt.p[d3] = 0.0;
	th = atan2(p2.p[d2]-p1.p[d2], p2.p[d1]-p1.p[d1]);					// d2軸に対する角度
	th = (th<-10||th>10) ? 0.0 : th;
	th = th-PI/2.0;
	vec.p[d1] = 0.0, vec.p[d2] = 0.0, vec.p[d3] = -1.0;					// 回転軸設定
	SetRotateMatrix(vec, th, rot);										// 回転ﾏﾄﾘｯｸｽ作成
	VecRotate(&wpt, rot);												// 回転軸中心へ回転移動
	dth = (wpt.p[d1]<0) ? -dth : dth;									// 回転軸の反対側は角度を逆
	if(md) {															// <円弧開始断面中心点>
		pb[cnt].p[d1] = wpt.p[d1]*cos(0.0);
		pb[cnt].p[d2] = wpt.p[d2];
		pb[cnt].p[d3] = wpt.p[d1]*sin(0.0);
		ib[cnt++] = 1;
		ath = dth;
	}
	for(i=st; i<ed; ath+=dth, i++) {									// <断面中心点>
		pb[cnt].p[d1] = wpt.p[d1]*cos(ath);								// 制御点配列作成
		pb[cnt].p[d2] = wpt.p[d2];
		pb[cnt].p[d3] = wpt.p[d1]*sin(ath);
		ib[cnt++] = 0;													// 曲線ﾓｰﾄﾞ設定
	}
	if(md) {															// <円弧終了断面中心点>
		pb[cnt].p[d1] = wpt.p[d1]*cos(ath);
		pb[cnt].p[d2] = wpt.p[d2];
		pb[cnt].p[d3] = wpt.p[d1]*sin(ath);
		ib[cnt++] = 1;
	}
	SetRotateMatrix(vec, -th, rot);										// 回転ﾏﾄﾘｯｸｽ作成
	for(i=0; i<cnt; i++) {
		VecRotateMove(&pb[i], rot, p1);									// 元位置へ回転移動
	}
	SetCurveMatrix1(1, wt, bc, cnt, ib, ma, mb);						// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
	CalcCurvePoint(1, cnt, pb, wk, mb);									// 通過点=>制御点変換
	for(i=0; i<cnt; i++) {
		pb[i] = wk[i];
	}
	for(i=0; i<an; i++) {
		pb[cnt+i] = pa[i], ib[cnt+i] = ia[i];							// 断面制御点情報追加
	}
	delete[] wk;
	delete[] ma;
	delete[] mb;
}

/*********************************************************************/
void CalcRevolvePoint(int fg, int sw, int af, int bf, int ap, int an,
                      int ac, int bn, int bc, double th, PNTTYPE p1,
                      PNTTYPE p2, PNTTYPE p3, PNTTYPE* pa, int* ia)		//<<< 回転体制御点計算
/*********************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	int*     ib = new int[View->m_MaxSCtA];
	int      f, n1, n2, rp, sm, bp;
	double   wt;

	if(!sw) {															// <<通常曲線>>
		if(fg==2) {														// <終了時>
			if(bf==1) {													// 回転中心点有:f=3
				f = 3;
			} else if(!ac) {											// 回転中心点無&境界開:f=2
				f = 2;
			} else {													// 回転中心点無&境界閉:f=1
				f = 1;
			}
			n1 = 2*af+bn, n2 = 2*bf+an;									// 制御点数
		} else {														// <終了時以外>
			f = 0;														// 終了時以外:f=0
			n1 = bn, n2 = an;											// 制御点数
		}
		wt = View->GetCurveWeight();									// 曲線重み係数取得
		Revolve(f, ap, wt, an, pa, ia, p1, p2, th, bn, af, pb, ib);		// 回転制御点生成
		if(!fg) {
			rp = View->GetCurveRepeat();								// 曲線分割数取得
			sm = View->GetSolidMode();									// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
			CrtSurface(bn, an, bc, ac, rp, wt, sm, pb, ib);				// 曲面生成(初回)
		} else {
			RedoCrtSurface(n1, n2, bc, ac, pb, ib);						// 曲面生成(2回目以降)
		}
	} else {															// <<掃引曲線>>
		bp = (ap+1>2) ? 0 : ap+1;										// 掃引平面
		wt = View->GetCurveWeight();									// 曲線重み係数取得
		SweepRev(p3, ap, wt, an, pa, ia, p1, p2, th, bn, pb, ib);		// 回転用掃引曲面生成
		if(!fg) {
			rp = View->GetCurveRepeat();								// 曲線分割数取得
			sm = View->GetSolidMode();									// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
			CrtSweepSurf(bn, an, bc, ac, rp, wt, sm, bp, pb, ib);		// 掃引曲面生成(初回)
		} else {
			RedoCrtSweepSurf(bn, an, bc, ac, pb, ib);					// 掃引曲面生成(2回目以降)
		}
	}
	delete[] pb;
	delete[] ib;
}

/****************************************************************/
static void SetSRevolve(int plane, int hit, int an, PNTTYPE* pa,
                        int* ia, PNTTYPE* p1, PNTTYPE* p2)				//<<< ｿﾘｯﾄﾞﾃﾞｰﾀ作成
/****************************************************************/
{
	PNTTYPE spa;
	int     sia, i, j, ix, cnt=-1;
	double  dd;

	ix = (plane-1<0) ? 2 : plane-1;										// 対象平面外の座標
	dd = p1->p[ix];														// 座標値保存
	for(i=0; i<an; i++) {
		if(ia[0]!=0&&ia[1]!=0) {										// 連続折線ｶｳﾝﾄUP
			cnt++;
		}
		spa = pa[0], sia = ia[0];										// 現在の確認位置が
		for(j=1; j<an; j++) {											// 切目の1つ前にｼﾌﾄ
			pa[j-1] = pa[j], ia[j-1] = ia[j];
		}
		pa[an-1] = spa, ia[an-1] = sia;
		if(hit==cnt) {													// <指定連続折線の場合>
			*p1 = pa[0];												// 始点->回転軸1点
			*p2 = pa[an-1];												// 終点->回転軸1点
			p1->p[ix] = dd, p2->p[ix] = dd;								// 平面外座標復元
			break;
		}
	}
}

/******************************************************************/
void SetSRevolvePoint(int af, int ap, int no, int an, int bn,
                      double th, PNTTYPE* pa, int* ia, PNTTYPE pt)		//<<< 回転体ｿﾘｯﾄﾞ制御点作成
/******************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	int*     ib = new int[View->m_MaxSCtA];
	PNTTYPE  p1, p2;
	int      rp, sm;
	double   wt;

	p1 = pt;
	rp = View->GetCurveRepeat();										// 曲線分割数取得
	wt = View->GetCurveWeight();										// 曲線重み係数取得
	sm = View->GetSolidMode();											// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
	SetSRevolve(ap, no, an, pa, ia, &p1, &p2);							// ｿﾘｯﾄﾞﾃﾞｰﾀ作成
	Revolve(1, ap, wt, an, pa, ia, p1, p2, th, bn, af, pb, ib);			// 回転制御点生成
	CrtSurface(bn, an, 1, 0, rp, wt, sm, pb, ib);						// 曲面生成
	delete[] pb;
	delete[] ib;
}

/********************************************************/
BOOL GetRevolveInfo(int f1, int f2, double* th, int* rd)				//<<< 回転体情報取得
/********************************************************/
{
	BOOL ret=TRUE;

	if(f1!=0) {															// <数値入力の場合>
		CInputDlg dlg(6);
		dlg.SetValue(*th);												// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(th);											// 生成角度
		} else {
			ret = FALSE;
		}
	}
	if(*th>0&&ret) {													// <有効生成角度>
		if(!f2) {														// ﾀﾞｲｱﾛｸﾞ省略無の場合
			CInputDlg dlg(11);
			dlg.SetValue(*rd);											// 既存値表示
			if(dlg.DoModal()==IDOK) {									// ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetValue(rd);										// 回転分割数
			} else {
				ret = FALSE;
			}
		}
	}
	return ret;
}

/**************************************************************/
void InitPyramid(int plane, int an, PNTTYPE* pa, PNTTYPE* pnt)			//<<< 錘初期処理
/**************************************************************/
{
	PNTTYPE ocnt, dcnt;
	int     i;

	CalcCent(pa, an, &ocnt);											// 2D中心点取得
	View->GetCentPnt(&dcnt);											// 画面中心取得
	i = (plane-1<0) ? 2 : plane-1;										// 2D平面法線方向
	dcnt.p[i] += View->GetDispHeight()/2.0;								// 画面正方向端点
	ocnt.p[i] = (ocnt.p[i]+dcnt.p[i])/2.0;								// 2D中心&端点中間
	*pnt = ocnt;
}

/**************************************************************/
void Pyramid(int flg, int plane, int an, PNTTYPE* pa, int* ia,
             PNTTYPE pnt, int div, PNTTYPE* pb, int* ib)				//<<< 錘制御点生成
/**************************************************************/
{
	PNTTYPE pt;
	int     un, ix, i, j, k;

	ix = (plane-1<0) ? 2 : plane-1;										// 2D平面法線方向
	if(flg>0) {															// <<最終実行の場合>>
		flg--;
		if(pa[0].p[ix]>pnt.p[ix]) {										// 負方向は面ﾙｰﾌﾟ反転
			ChangeLoop(an, pa, ia);
		}
	}
	un = div+flg+2;														// U方向制御点数
	for(i=0; i<an; i++) {												// <V制御点数分>
		for(j=0; j<DIM; j++) {
			pt.p[j] = pnt.p[j]-pa[i].p[j];								// 始終点間変位量
			pt.p[j] = pt.p[j]/(div+1);									// 挿入数分に分割
		}
		if(flg) {														// <<2D中心有の場合>>
			pb[un*i] = pa[an], ib[un*i] = ia[i]+1;						// 2D中心, U先頭ﾓｰﾄﾞ
		}
		pb[un*i+flg] = pa[i], ib[un*i+flg] = ia[i]+1;					// U先頭制御点, U先頭ﾓｰﾄﾞ
		for(j=1; j<=div; j++) {
			for(k=0; k<DIM; k++) {
				pb[un*i+j+flg].p[k] = pa[i].p[k]+pt.p[k]*j;				// 挿入分制御点
			}
			ib[un*i+j+flg] = ia[i];										// 挿入分ﾓｰﾄﾞ
		}
		pb[un*i+div+flg+1] = pnt, ib[un*i+div+flg+1] = ia[i]+1;			// U最終制御点, U最終ﾓｰﾄﾞ
	}
}

/*****************************************************************/
void CreatSpiralObject(int pl, int vn, int vb, int fg, double dm,
                       double hg, int cn, int rn, PNTTYPE cp,
                       PNTTYPE sp, PNTTYPE* pa, int* ia)				//<<< 螺旋曲面生成
/*****************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	int*     ib = new int[View->m_MaxSCtA];
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  bpt, pnt;
	VECTYPE  vec;
	int      i, j, k, i1, i2, i3, un, mx, sm;
	double   rot[3][3], th, sth=0.0, mi1, mi2, ma1, ma2, ct1, ct2, wt;

	i1 = pl;															// 断面平面設定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	un = 2*fg+cn*rn+1;													// U制御点数
	mi1 = DBL_MAX, mi2 = DBL_MAX;
	ma1 = 1.0-DBL_MAX, ma2 = 1.0-DBL_MAX;
	for(i=0; i<vn; i++) {
		mi1 = (pa[i].p[i1]<mi1) ? pa[i].p[i1] : mi1;					// 断面最小･最大取得
		ma1 = (pa[i].p[i1]>ma1) ? pa[i].p[i1] : ma1;
		mi2 = (pa[i].p[i2]<mi2) ? pa[i].p[i2] : mi2;
		ma2 = (pa[i].p[i2]>ma2) ? pa[i].p[i2] : ma2;
		pa[i].p[i3] = 0.0;												// 断面平面外座標ｸﾘｱ
	}
	cp.p[i3] = 0.0;
	ct1 = mi1-dm/2.0, ct2 = (mi2+ma2)/2.0;								// 断面基準点
	for(i=0; i<vn; i++) {
		pa[i].p[i1] -= ct1, pa[i].p[i2] -= ct2;							// 基準点を原点へ移動
	}
	cp.p[i1] -= ct1, cp.p[i2] -= ct2;
	bpt = sp;															// 開始中心位置
	bpt.p[i2] -= hg*rn/2.0;
	th = 2.0*PI/cn;														// 1制御点の回転角度
	hg /= cn;															// 1制御点の高さ
	vec.p[i1] = 0.0, vec.p[i2] = -1.0, vec.p[i3] = 0.0;					// 回転ﾍﾞｸﾄﾙ
	if(fg) {															// <断面中心あり>
		for(k=0; k<DIM; k++) {
			pnt.p[k] = bpt.p[k]+cp.p[k];
		}
		for(j=0; j<vn; j++) {
			pb[j*un] = pnt, ib[j*un] = 3;
		}
	}
	for(j=0; j<vn; j++) {												// <第1断面>
		for(k=0; k<DIM; k++) {
			pnt.p[k] = bpt.p[k]+pa[j].p[k];
		}
		pb[j*un+fg] = pnt, ib[j*un+fg] = ia[j]+1;
	}
	for(i=1; i<=cn*rn; i++) {											// <第2断面以降>
		sth += th;														// 回転角度･垂直距離加算
		bpt.p[i2] += hg;
		SetRotateMatrix(vec, sth, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(j=0; j<vn; j++) {
			pnt = pa[j];
			VecRotateMove(&pnt, rot, bpt);
			pb[j*un+fg+i] = pnt;
			ib[j*un+fg+i] = (i==cn*rn) ? ia[j]+1 : ia[j];
		}
	}
	if(fg) {															// <断面中心あり>
		VecRotateMove(&cp, rot, bpt);
		for(j=0; j<vn; j++) {
			pb[j*un+un-1] = cp, ib[j*un+un-1] = 3;
		}
	}
	mx = View->GetCurveRepeat();										// 曲線化繰返数取得
	wt = View->GetCurveWeight();										// 曲線重み係数取得
	sm = View->GetSolidMode();											// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
	CrtSurface(un, vn, 0, vb, mx, wt, sm, pb, ib);						// 曲面生成
	GetObjPtr(&op);														// 曲面立体OP取得
	i = GetObjAtr(op);													// 立体属性取得
	GetObjOif(op, &oif);												// 立体構成情報ﾎﾟｲﾝﾀ取得
	if(i==20) {
		SetOifName(0, oif, OBJ_NAME03);									// 体構成名(通常掃引体)
	} else {
		SetOifName(0, oif, OBJ_NAME04);									//         (通常掃引体[面体])
	}
	OnEdtFlg(1, op);													// 曲面edtflg:ON
	delete[] pb;
	delete[] ib;
}

/******************************************************/
void ReverseSurf(int un, int vn, PNTTYPE* pa, int* ea)					//<<< 曲面制御点反転
/******************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int*     ew = new int[View->m_MaxSCtA];
	int      i, j, cnt=0;

	for(i=0; i<vn; i++) {
		for(j=un-1; j>=0; j--) {
			pw[cnt] = pa[j+un*i], ew[cnt++] = ea[j+un*i];				// U方向の並びを反転
		}
	}
	j = un*vn;
	for(i=0; i<j; i++) {
		pa[i] = pw[i], ea[i] = ew[i];
	}
	delete[] pw;
	delete[] ew;
}

/****************************************************/
BOOL CheckSurface(int n, int *un, int *vn,
                  int* ub, int* vb, int* md, int* p)					//<<< 曲面連結ﾁｪｯｸ
/****************************************************/
{
	int i, err1=0, err2=0;

	for(i=0; i<n; i++) {
		if(ub[i]==1) {													// U境界ﾓｰﾄﾞ:閉をﾁｪｯｸ
			err1 = 1; break;
		}
		if(vb[i]==1) {													// V境界ﾓｰﾄﾞ:閉をﾁｪｯｸ
			err2 = 1; break;
		}
	}
	if(!err1) {
		for(i=1; i<n; i++) {											// U境界ﾓｰﾄﾞ:開の場合
			err1 = (vn[0]!=vn[i]||vb[0]!=vb[i]) ? 1 : err1;				// 制御点数とV境界ﾁｪｯｸ
		}
	}
	if(!err2) {
		for(i=1; i<n; i++) {											// V境界ﾓｰﾄﾞ:開の場合
			err2 = (un[0]!=un[i]||ub[0]!=ub[i]) ? 1 : err2;				// 制御点数とU境界ﾁｪｯｸ
		}
	}
	if(!err1) {															// 境界ﾓｰﾄﾞ
		*p = 0;
	} else if(!err2) {
		*p = 1, err1 = 0;
	} else {
		*p = 0;
	}
	return err1;
}

/*************************************************************/
void GetSEPoint(int m1, int m2, int n, OBJTYPE** op, int* un,
                int* vn, int* ub, int* vb, PNTTYPE* pa,
                PNTTYPE* pb, SCRTYPE* pc, int* c1, int* c2)				//<<< 始点終点制御点取得
/*************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	CMDTYPE* cmp;
	int      i, j, k, l, i1, i2, i3, i4, i5, i6, ct=0;

	if(m1==0) {															// <<曲線の場合>>
		for(i=0; i<n; i++) {
			BaseCmdPtr(1, op[i], &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			GetParaPnt(cmp, 0, &pa[i]);									// 始点制御点取得
			GetParaPnt(cmp, un[i]-1, &pb[i]);							// 終点制御点取得
			pc[i].p[0] = 0, pc[i].p[1] = 0;								// 始点･終点収束無
			ct += un[i];												// 連結方向制御点数
		}
		*c2 = 1;														// 反対方向制御点数
	} else {															// <<通常曲面の場合>>
		if(m2==0) {														// <U連結の場合>
			for(i=0; i<n; i++) {
				i1 = un[i], i2 = vn[i], i3 = ub[i], i4 = vb[i];
				BaseCmdPtr(1, op[i], &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				k = i1*i2;												// 総制御点数
				for(j=0; j<k; j++) {
					GetParaPnt(cmp, j, &p1[j]);							// 制御点取得
				}
				CheckCLine(i3, i4, i1, i2, p1, &i5, &i6);				// 制御点列収束ﾁｪｯｸ
				if(i6%2) {												// 始点収束
					pc[i].p[0] = 1, k = 1;
				} else {
					pc[i].p[0] = 0, k = 0;
				}
				if(i6>1) {												// 終点収束
					pc[i].p[1] = 1, l = i1-2;
				} else {
					pc[i].p[1] = 0, l = i1-1;
				}
				for(j=0; j<i2; j++) {
					p2[j] = p1[j*i1+k];
				}
				CalcCent(p2, i2, &pa[i]);								// 始点端面中心取得
				for(j=0; j<i2; j++) {
					p2[j] = p1[j*i1+l];
				}
				CalcCent(p2, i2, &pb[i]);								// 終点端面中心取得
				ct += i1;												// 連結制御点数
			}
			*c2 = i2;													// 反対方向制御点数
		} else {														// <V連結の場合>
			for(i=0; i<n; i++) {
				i1 = un[i], i2 = vn[i], i3 = ub[i], i4 = vb[i];
				BaseCmdPtr(1, op[i], &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				k = i1*i2;												// 総制御点数
				for(j=0; j<k; j++) {
					GetParaPnt(cmp, j, &p1[j]);							// 制御点取得
				}
				CheckCLine(i3, i4, i1, i2, p1, &i5, &i6);				// 制御点列収束ﾁｪｯｸ
				if(i5%2) {												// 始点収束
					pc[i].p[0] = 1, k = i1;
				} else {
					pc[i].p[0] = 0, k = 0;
				}
				if(i5>1) {												// 終点収束
					pc[i].p[1] = 1, l = i1*(i2-2);
				} else {
					pc[i].p[1] = 0, l = i1*(i2-1);
				}
				for(j=0; j<i1; j++) {
					p2[j] = p1[j+k];
				}
				CalcCent(p2, i1, &pa[i]);								// 始点端面中心取得
				for(j=0; j<i1; j++) {
					p2[j] = p1[j+l];
				}
				CalcCent(p2, i1, &pb[i]);								// 終点端面中心取得
				ct += i2;												// 連結方向制御点数
			}
			*c2 = i1;													// 反対方向制御点数
		}
	}
	*c1 = ct;
	delete[] p1;
	delete[] p2;
}

/***************************************************************/
BOOL ConctSEPoint(int n, PNTTYPE* pa, PNTTYPE* pb, SCRTYPE* pd)			//<<< 始点終点制御点連結
/***************************************************************/
{
	SCRTYPE* wd = new SCRTYPE[View->m_MaxCCtA];
	SCRTYPE* er = new SCRTYPE[View->m_MaxCCtA];
	PNTTYPE  pt1, pt2;
	int      i, j, k, l, i1, i2, fg, j1, j2, sw, en=0;
	double   lw, ln;

	for(i=0; i<n; i++) {												// 連結状況初期化
		pd[i].p[0] = -1, pd[i].p[1] = -1;
	}
	for(i=0; i<n-1; i++) {												// <<連結回数分>>
		ln = DBL_MAX, i1 = -1, i2 = -1;									// ﾃﾞｰﾀ初期化
		for(j=0; j<n*2; j++) {
			if(j<n) {
				pt1 = pa[j%n], fg = pd[j%n].p[0];
			} else {
				pt1 = pb[j%n], fg = pd[j%n].p[1];
			}
			if(fg<0) {													// <1点目が未連結>
				for(k=0; k<n*2; k++) {
					if(k<n) {
						pt2 = pa[k%n], fg = pd[k%n].p[0];
					} else {
						pt2 = pb[k%n], fg = pd[k%n].p[1];
					}
					if(j%n!=k%n&&fg<0) {								// <2点目が未連結>
						for(l=0, sw=1; l<en; l++) {
							if((j==er[l].p[0]&&k==er[l].p[1])||
                               (j==er[l].p[1]&&k==er[l].p[0])) {		// ﾘﾄﾗｲﾃﾞｰﾀと一致は
								sw = 0; break;							// 無効ﾃﾞｰﾀ
							}
						}
						if(sw) {										// ﾘﾄﾗｲﾃﾞｰﾀ以外
							lw = PntD(&pt1, &pt2);						// 2点間距離
							if(lw<ln) {									// 最小の2点間距離
								ln = lw, i1 = j, i2 = k;
							}
						}
					}
				}
			}
		}
		if(i1>=0) {
			for(j=0; j<n; j++) {
				wd[j] = pd[j];											// 作業領域にｺﾋﾟｰ
			}
			if(i1<n) {													// 検索No
				j1 = wd[i1%n].p[1], wd[i1%n].p[1] = -2;
			} else {
				j1 = wd[i1%n].p[0], wd[i1%n].p[0] = -2;
			}
			j2 = i2%n;													// ﾙｰﾌﾟ閉番地
			if(j1==-1) {
				sw = 1;
			} else {
				for(j=0, sw=0; j<n; j++) {
					for(k=0, l=-1; k<n; k++) {
						if(wd[k].p[0]==j1) {							// 始点側で次立体を検索
							j1 = wd[k].p[1], wd[k].p[1] = -2;			// 次検索No
							if(k==j2) {									// ﾙｰﾌﾟ閉番地到達はｴﾗｰ
								sw = -1;
							} else if(j1==-1) {							// 次検索No無は正常終了
								sw = 1;
							}
							l = 0; break;
						}
					}
					if(l==-1) {											// 始点側にない場合
						for(k=0; k<n; k++) {
							if(wd[k].p[1]==j1) {						// 終点側で次立体を検索
								j1 = wd[k].p[0], wd[k].p[0] = -2;		// 次検索No
								if(k==j2) {								// ﾙｰﾌﾟ閉番地到達はｴﾗｰ
									sw = -1;
								} else if(j1==-1) {						// 次検索No無は正常終了
									sw = 1;
								}
								break;
							}
						}
					}
					if(sw!=0) {
						break;
					}
				}
			}
			if(sw!=-1) {												// <正常な場合>
				if(i1<n) {												// 最短点と連結
					pd[i1%n].p[0] = i;
				} else {
					pd[i1%n].p[1] = i;
				}
				if(i2<n) {
					pd[i2%n].p[0] = i;
				} else {
					pd[i2%n].p[1] = i;
				}
				en = 0;
			} else {													// <連結するとﾙｰﾌﾟ閉の場合>
				i--;
				er[en].p[0] = i1, er[en].p[1] = i2;						// 該当ﾃﾞｰﾀ以外でﾘﾄﾗｲ
				if(++en>=n) {
					delete[] wd;
					delete[] er;
					return TRUE;
				}
			}
		}
	}
	delete[] wd;
	delete[] er;
	return FALSE;
}

/*************************************************/
BOOL GetConctObj(int n, SCRTYPE* pd, SCRTYPE* pw)						//<<< 連結立体順取得
/*************************************************/
{
	int i, j, k, l;

	for(i=0, k=-1; i<n; i++) {
		if(pd[i].p[0]==-1) {											// 始点側で先頭立体を検索
			pw[0].p[0] = i, pw[0].p[1] = 0;								// 先頭立体No登録
			k = pd[i].p[1], pd[i].p[1] = -2;							// 次検索No
			break;
		}
	}
	if(k==-1) {															// 始点側にない場合
		for(i=0; i<n; i++) {
			if(pd[i].p[1]==-1) {										// 終点側で先頭立体を検索
				pw[0].p[0] = i, pw[0].p[1] = 1;							// 先頭立体No登録
				k = pd[i].p[0], pd[i].p[0] = -2;						// 次検索No
				break;
			}
		}
	}
	if(k==-1) {															// 検索不能でｴﾗｰ
		return TRUE;
	}
	for(i=1; i<n; i++) {
		for(j=0, l=-1; j<n; j++) {
			if(pd[j].p[0]==k) {											// 始点側で次立体を検索
				pw[i].p[0] = j, pw[i].p[1] = 0;							// 次立体No登録
				k = pd[j].p[1], pd[j].p[1] = -2, l = 0;					// 次検索No
				break;
			}
		}
		if(l==-1) {														// 始点側にない場合
			for(j=0; j<n; j++) {
				if(pd[j].p[1]==k) {										// 終点側で次立体を検索
					pw[i].p[0] = j, pw[i].p[1] = 1;						// 次立体No登録
					k = pd[j].p[0], pd[j].p[0] = -2, l = 0;				// 次検索No
					break;
				}
			}
		}
		if(l==-1) {														// 検索不能でｴﾗｰ
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************/
void ConctCurve(int clf, int n, OBJTYPE** op, SCRTYPE* no)				//<<< 曲線連結
/**********************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCCtA];
	int*     ea = new int[View->m_MaxCCtA];
	CMDTYPE* cmp;
	int      i, j, ono, flg, wn, pn=0, bc, mx;
	double   wt;

	for(i=0; i<n; i++) {
		ono = no[i].p[0];												// 立体No
		flg = no[i].p[1];												// 始点･終点ﾌﾗｸﾞ
		BaseCmdPtr(1, op[ono], &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
		wn = GetParaInt(cmp, 0);										// 制御点数取得
		if(i==0) {														// <先頭曲線>
			bc = GetParaInt(cmp, 1);									// 境界ﾓｰﾄﾞ取得
			bc = (clf) ? 1 : bc;										// 閉連結の場合は境界:閉
			mx = GetParaInt(cmp, 2);									// 曲線化繰返数
			wt = GetParaInt(cmp, 3)/1000.0;								// 曲線重み係数
		}
		if(!flg) {														// <始点連結の場合>
			for(j=0; j<wn; j++) {										// 昇順に
				GetParaPnt(cmp, j, &pa[pn]);							// 制御点取得
				ea[pn] = GetParaInt(cmp, j+4);							// ﾗｲﾝﾓｰﾄﾞ取得
				pn++;
			}
		} else {														// <終点連結の場合>
			for(j=wn-1; j>=0; j--) {									// 降順
				GetParaPnt(cmp, j, &pa[pn]);							// 制御点取得
				ea[pn] = GetParaInt(cmp, j+4);							// ﾗｲﾝﾓｰﾄﾞ取得
				pn++;
			}
		}
	}
	CrtCurve(pn, bc, mx, wt, pa, ea);									// 曲線生成
	delete[] pa;
	delete[] ea;
}

/*********************************************************/
void ConctSurface(int clf, int md, int n,
                  OBJTYPE** op, SCRTYPE* no, SCRTYPE* fa)				//<<< 通常曲面連結
/*********************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int*     ea = new int[View->m_MaxSCtA];
	int*     eb = new int[View->m_MaxSCtA];
	int*     ec = new int[View->m_MaxSCtA];
	int*     ew = new int[View->m_MaxSCtA];
	CMDTYPE* cmp;
	int      i, j, k, l, ono, flg, sfg, efg, us, ue, vs, ve, ct;
	int      uw, vw, uu, un=0, vn=0, ub, vb, mx, fg, chg;
	double   wt, m, ln;

	for(i=0; i<n; i++) {
		ono = no[i].p[0];												// 立体No
		flg = no[i].p[1];												// 始点･終点ﾌﾗｸﾞ
		sfg = fa[i].p[0];												// 始点/終点収束解除ﾌﾗｸﾞ
		efg = fa[i].p[1];
		BaseCmdPtr(1, op[ono], &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
		uw = GetParaInt(cmp, 0);										// U制御点数取得
		vw = GetParaInt(cmp, 1);										// V制御点数取得
		if(i==0) {														// <先頭通常曲面>
			if(md==0) {													// U連結はV制御点数決定
				vn = vw;
			} else {													// V連結はU制御点数決定
				un = uw;
			}
			ub = GetParaInt(cmp, 2);									// U境界ﾓｰﾄﾞ取得
			vb = GetParaInt(cmp, 3);									// V境界ﾓｰﾄﾞ取得
			chg = (ub==0&&vb==0) ? 1 : 0;								// 両方が開はｿﾘｯﾄﾞﾓｰﾄﾞ変更
			if(clf) {													// <閉連結の場合>
				if(md==0) {												// U連結はU境界:閉
					ub = 1;
				} else {												// V連結はV境界:閉
					vb = 1;
				}
			}
			mx = GetParaInt(cmp, 4);									// 曲線化繰返数
			wt = GetParaInt(cmp, 5)/1000.0;								// 曲線化重み係数
			fg = GetParaInt(cmp, 6);									// ｿﾘｯﾄﾞﾓｰﾄﾞ
			if(!clf) {													// <閉連結でない場合>
				if(!flg) {												// 始点連結は始点収束有効
					sfg = 0;
				} else {												// 終点連結は終点収束有効
					efg = 0;
				}
			}
		} else if(i==n-1) {												// <最終通常曲面>
			if(!clf) {													// <閉連結でない場合>
				if(!flg) {												// 始点連結は終点収束有効
					efg = 0;
				} else {												// 終点連結は始点収束有効
					sfg = 0;
				}
			}
		}
		us = 0, ue = uw, vs = 0, ve = vw, uu = uw;
		if(md==0) {														// <U連結の場合>
			if(sfg) {													// 収束点を対象外
				us += 1, uw -= 1;
			}
			if(efg) {
				ue -= 1, uw -= 1;
			}
		} else {														// <V連結の場合>
			if(sfg) {													// 収束点を対象外
				vs += 1, vw -= 1;
			}
			if(efg) {
				ve -= 1, vw -= 1;
			}
		}
		for(j=vs, ct=0; j<ve; j++) {
			for(k=us; k<ue; k++) {
				GetParaPnt(cmp, uu*j+k, &pa[ct]);						// 制御点取得
				ea[ct++] = GetParaInt(cmp, uu*j+k+7);					// ﾓｰﾄﾞ取得
			}
		}
		if(flg) {														// <終点連結の場合>
			for(j=vw-1, ct=0; j>=0; j--) {
				for(k=uw-1; k>=0; k--) {
					pw[ct] = pa[uw*j+k], ew[ct++] = ea[uw*j+k];			// 降順に並び替え
				}
			}
			for(j=0; j<ct; j++) {
				pa[j] = pw[j], ea[j] = ew[j];
			}
		}
		if(i==0) {														// <<<先頭通常曲面>>>
			for(j=0; j<ct; j++) {
				pc[j] = pa[j], ec[j] = ea[j];
			}
		} else {														// <<<以降通常曲面>>>
			if(md==0) {													// <<U連結の場合>>
				if(vb) {												// <V:閉の場合>
					for(j=0, ln=DBL_MAX; j<vn; j++) {
						for(k=0, l=j, m=0.0; k<vn; k++) {
							m += PntD(&pb[un*(k+1)-1], &pa[uw*l]);		// 連結位置制御点距離合計
							l = (l+1>=vn) ? 0 : l+1;
						}
						if(m<ln) {										// 最短合計距離点
							ln = m, vs = j;
						}
					}
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// 登録済みﾃﾞｰﾀ
						}
						for(k=0; k<uw; k++) {
							pc[ct] = pa[uw*vs+k];						// 今回ﾃﾞｰﾀ追加
							ec[ct++] = ea[uw*vs+k];
						}
						vs = (vs+1>=vn) ? 0 : vs+1;
					}
				} else {												// <V:開の場合>
					if(PntD(&pb[un-1], &pa[0])<
                       PntD(&pb[un-1], &pa[uw*(vn-1)])) {				// 始点側が近い場合は
						vs = 0, l = 0;									// 0から昇順
					} else {											// 終点側が近い場合は
						vs = vn-1, l = 1;								// vn-1から降順
					}
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// 登録済みﾃﾞｰﾀ
						}
						for(k=0; k<uw; k++) {
							pc[ct] = pa[uw*vs+k];						// 今回ﾃﾞｰﾀ追加
							ec[ct++] = ea[uw*vs+k];
						}
						if(!l) {
							vs++;
						} else {
							vs--;
						}
					}
				}
			} else {													// <<V連結の場合>>
				if(ub) {												// <U:閉の場合>
					for(j=0, ln=DBL_MAX; j<un; j++) {
						for(k=0, l=j, m=0.0; k<un; k++) {
							m += PntD(&pb[un*(vn-1)+k], &pa[l]);		// 連結位置制御点距離合計
							l = (l+1>=un) ? 0 : l+1;
						}
						if(m<ln) {										// 最短合計距離点
							ln = m, us = j;
						}
					}
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// 登録済みﾃﾞｰﾀ
						}
					}
					for(j=0; j<vw; j++) {
						for(k=0, l=us; k<un; k++) {
							pc[ct] = pa[un*j+l], ec[ct++] = ea[un*j+l];	// 今回ﾃﾞｰﾀ追加
							l = (l+1>=un) ? 0 : l+1;
						}
					}
				} else {												// <U:開の場合>
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// 登録済みﾃﾞｰﾀ
						}
					}
					if(PntD(&pb[un*(vn-1)], &pa[0])<
                       PntD(&pb[un*(vn-1)], &pa[un-1])) {				// 始点側が近い場合は
						for(j=0; j<vw; j++) {
							for(k=0; k<un; k++) {						// 昇順に今回ﾃﾞｰﾀ追加
								pc[ct] = pa[un*j+k];
								ec[ct++] = ea[un*j+k];
							}
						}
					} else {											// 終点側が近い場合は
						for(j=0; j<vw; j++) {
							for(k=un-1; k>=0; k--) {					// 降順に今回ﾃﾞｰﾀ追加
								pc[ct] = pa[un*j+k];
								ec[ct++] = ea[un*j+k];
							}
						}
					}
				}
			}
		}
		if(md==0) {														// U連結の場合
			un += uw;
		} else {														// V連結の場合
			vn += vw;
		}
		ct = un*vn;
		for(j=0; j<ct; j++) {
			pb[j] = pc[j], eb[j] = ec[j];								// ﾊﾞｯﾌｧ入替え
		}
	}
	fg = (chg==1&&n==1) ? 0 : fg;										// 1立体で両方が開は必ずｿﾘｯﾄﾞ:OFF
	CrtSurface(un, vn, ub, vb, mx, wt, fg, pb, eb);						// 曲面生成
	delete[] pa;
	delete[] pb;
	delete[] pc;
	delete[] pw;
	delete[] ea;
	delete[] eb;
	delete[] ec;
	delete[] ew;
}
