/////////////////////////////////////////////////////////////////////////////
// 曲線編集操作部
// CreatCurvePoint.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "BackCommand.h"
#include "InputDlg.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/***************************************************************/
void InsertCurvePoint(int* inst, int* pn, PNTTYPE* pa, int* ea)			//<<< S曲線中間制御点挿入
/***************************************************************/
{
	PNTTYPE pnt;
	int     i, j, in;

	CInputDlg dlg(23);
	dlg.SetValue(*inst);												// 既存値表示
	if(dlg.DoModal()==IDOK) {											// ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(inst);												// 挿入制御点数取得
		in = *inst;
	} else {
		in = 0;
	}
	if(in>0) {
		ea[0] = 1, ea[1] = 1;											// 始点/終点折線点ﾓｰﾄﾞ
		for(i=0; i<DIM; i++) {
			pnt.p[i] = pa[1].p[i]-pa[0].p[i];							// 始終点間変位量
			pnt.p[i] = pnt.p[i]/(in+1);									// 挿入数分分割
		}
		pa[in+1] = pa[1], ea[in+1] = ea[1];								// 終点を後にｼﾌﾄ
		*pn = 2+in;														// 新制御点数
		for(i=1; i<=in; i++) {
			for(j=0; j<DIM; j++) {
				pa[i].p[j] = pa[0].p[j]+pnt.p[j]*i;						// 挿入分制御点追加
			}
			ea[i] = 0;
		}
	}
}

/**************************************************/
void CalcRPoint(int md, int pl, int dv, double rd,
                int* pn, PNTTYPE* pa, int* ea)							//<<< R頂点位置算出
/**************************************************/
{
	PNTTYPE pw[10], pts, pte, pt1, pt2, cpt;
	VECTYPE vec, vcs, vce;
	int     iw[10], i, i1, i2, n, ret=FALSE;
	double  th, ts, te, t1, t2, dd, ds, de;

	n = *pn;
	if(md==0) {															// <中間点の場合>
		for(i=n-3; i<n; i++) {
			pw[i-n+3] = pa[i], iw[i-n+3] = ea[i];						// 中間ﾃﾞｰﾀ複写
		}
	} else if(md==1) {													// <始点の場合>
		pw[0] = pa[n-1], iw[0] = ea[n-1];								// 終点ﾃﾞｰﾀ複写
		for(i=0; i<2; i++) {
			pw[i+1] = pa[i], iw[i+1] = ea[i];							// 始点ﾃﾞｰﾀ複写
		}
	} else {															// <終点の場合>
		for(i=n-2; i<n; i++) {
			pw[i-n+2] = pa[i], iw[i-n+2] = ea[i];						// 終点ﾃﾞｰﾀ複写
		}
		pw[2] = pa[0], iw[2] = ea[0];									// 始点ﾃﾞｰﾀ複写
	}
	th = PI-CalcRotAngle(&pw[0]);										// 角度取得
	if(th>PI*20.0/180.0&&th<PI*160.0/180.0) {							// <R>20度&R<160度で有効>
		ds = PntD(&pw[1], &pw[0]);										// 対象点と前後2点の
		VecV(&pw[1], &pw[0], &vcs);										// 距離とﾍﾞｸﾄﾙ算出
		VecN(&vcs);
		de = PntD(&pw[1], &pw[2]);
		VecV(&pw[1], &pw[2], &vce);
		VecN(&vce);
		dd = rd/tan(th/2.0);											// 対象点からのR開始距離
		ds = (ds>de) ? de : ds;											// 前後制御点最短距離
		if(dd>ds) {
			AfxMessageBox(IDS_ERR_CTRLL);								// ｴﾗｰﾒｯｾｰｼﾞ
		} else {														// <<指定R分の距離有>>
			if((n+3)*dv<=View->m_MaxCCtl) {								// <制御点上限未満>
				ret = TRUE;
			} else {
				AfxMessageBox(IDS_MANY_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ
			}
		}
	} else {
		AfxMessageBox(IDS_ERR_ANGLE);									// ｴﾗｰﾒｯｾｰｼﾞ
	}
	if(ret) {															// <R指定OK>
		i1 = pl;														// 有効座標軸設定
		i2 = (pl+1>2) ? 0 : pl+1;
		for(i=0; i<DIM; i++) {
			vec.p[i] = (vcs.p[i]+vce.p[i])/2.0;							// 対象点からのR中心点方向
			pts.p[i] = pw[1].p[i]+vcs.p[i]*dd;							// R開始点
			pte.p[i] = pw[1].p[i]+vce.p[i]*dd;							// R終了点
		}
		VecN(&vec);
		ds = sqrt(rd*rd+dd*dd);											// 対象点からのR中心点距離
		for(i=0; i<DIM; i++) {
			cpt.p[i] = pw[1].p[i]+vec.p[i]*ds;							// R中心点
		}
		if((ts=CalcBaseAngle(cpt, pts, pl))<0) {						// 基準角度取得
			ts += 2.0*PI;
		}
		if((te=CalcBaseAngle(cpt, pte, pl))<0) {
			te += 2.0*PI;
		}
		if(fabs(ts-te)<PI) {
			t1 = (ts<te) ? PI-th : -(PI-th);							// R生成用分割角度
		} else {
			t1 = (ts<te) ? -(PI-th) : PI-th;
		}
		t2 = ds-rd;														// 対象点とRの距離差
		if(th>PI*80.0/180.0) {											// <対象点角度>80度>
			t1 = t1/3.5, t2 = (t2/6.0)*2.0*th/PI;
		} else if(th>PI*50.0/180.0) {									// <対象点角度>50度>
			t1 = t1/3.4, t2 = (t2/5.5)*2.0*th/PI;
		} else {														// <対象点角度<=50度>
			t1 = t1/3.3, t2 = (t2/5.0)*2.0*th/PI;
		}
		t2 = rd+t2;
		for(i=0; i<DIM; i++) {
			pt1.p[i] = cpt.p[i], pt2.p[i] = cpt.p[i];
		}
		pt1.p[i1] = cpt.p[i1]+t2*cos(ts+t1);
		pt1.p[i2] = cpt.p[i2]+t2*sin(ts+t1);
		pt2.p[i1] = cpt.p[i1]+t2*cos(te-t1);
		pt2.p[i2] = cpt.p[i2]+t2*sin(te-t1);
		iw[5] = iw[2], iw[4] = 1, iw[3] = 0, iw[2] = 0, iw[1] = 1;		// 曲線ﾓｰﾄﾞｼﾌﾄ&設定
		pw[5] = pw[2], pw[4] = pte, pw[3] = pt2;						// 制御点ｼﾌﾄ&設定
		pw[2] = pt1,   pw[1] = pts;
		if(md==0) {														// <中間点の場合>
			for(i=1; i<6; i++) {
				pa[i+n-3] = pw[i], ea[i+n-3] = iw[i];					// R頂点ﾃﾞｰﾀ設定
			}
		} else if(md==1) {												// <始点の場合>
			for(i=n-1; i>0; i--) {
				pa[i+3] = pa[i], ea[i+3] = ea[i];						// ﾃﾞｰﾀｼﾌﾄ
			}
			for(i=1; i<5; i++) {
				pa[i-1] = pw[i], ea[i-1] = iw[i];						// R頂点ﾃﾞｰﾀ設定
			}
		} else {														// <終点の場合>
			for(i=1; i<5; i++) {
				pa[i+n-2] = pw[i], ea[i+n-2] = iw[i];					// R頂点ﾃﾞｰﾀ設定
			}
		}
		*pn = n+3;														// 制御点数UP
	}
}

/******************************************************************/
void CalcSpiralPoint(PNTTYPE pnt, int pln, double dmt, double hgt,
                     int cno, int sno, PNTTYPE* pa, int* ia)			//<<< 螺旋制御点計算
/******************************************************************/
{
	int    i, d1, d2, d3, cn;
	double rd, hg, shg, th, sth=0.0;

	d1 = pln;															// 座標軸設定
	d2 = (d1+1>2) ? 0 : d1+1;
	d3 = (d2+1>2) ? 0 : d2+1;
	cn = cno*sno+1;														// 総制御点数
	rd = dmt/2.0;														// 半径
	hg = hgt/cno;														// 1制御点の高さ
	shg = pnt.p[d3]-(hgt*sno/2.0);										// 開始位置の高さ
	th = 2.0*PI/cno;													// 1制御点の回転角度
	for(i=0; i<cn; i++) {
		pa[i].p[d1] = pnt.p[d1]+rd*cos(sth);							// 制御点配列作成
		pa[i].p[d2] = pnt.p[d2]+rd*sin(sth);
		pa[i].p[d3] = shg;
		ia[i] = 0;														// 曲線ﾓｰﾄﾞ設定
		sth += th, shg += hg;											// 半径,高さ加算
	}
	ia[0] = 1, ia[cn-1] = 1;											// 始点･終点は折線点
}

/*********************************************************************/
static void TrackerCurvePoint(int bc, int an, PNTTYPE* pa, int* ia,
                              int* ic, int* bn, PNTTYPE* pb, int* ib)	//<<< 曲線編集ﾄﾗｯｶｰ取得ｻﾌﾞ
/*********************************************************************/
{
	int i, j;

	for(i=0; i<an; i++) {
		pb[i*2] = pa[i];												// 制御点ﾄﾗｯｶｰ設定
		ib[i*2] = (ic[i]==0) ? ia[i]*2 : 4;								// 色設定
	}
	for(i=1; i<an; i++) {
		for(j=0; j<DIM; j++) {
			pb[(i-1)*2+1].p[j] = (pa[i-1].p[j]+pa[i].p[j])/2.0;			// 中間点ﾄﾗｯｶｰ設定
		}
		ib[(i-1)*2+1] = 5;												// 色設定(中間点)
	}
	if(bc) {															// <曲線(閉)の場合>
		for(j=0; j<DIM; j++) {
			pb[an*2-1].p[j] = (pa[0].p[j]+pa[an-1].p[j])/2.0;			// 始点と終点の中間点追加
		}
		ib[an*2-1] = 5;													// 色設定(中間点)
		*bn = an*2;														// ﾄﾗｯｶｰ数
	} else {															// <曲線(開)の場合>
		*bn = an*2-1;													// ﾄﾗｯｶｰ数
	}
}

/*******************************************************************/
void TrackerECurve(int md, double* da, int bc, int an, PNTTYPE* pa,
                   int* ia, int* ic, int* bn, PNTTYPE* pb, int* ib)		//<<< 曲線編集ﾄﾗｯｶｰ取得
/*******************************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];

	CalcCurvePoint(md, an, pa, pw, da);									// 制御点=>通過点変換
	TrackerCurvePoint(bc, an, pw, ia, ic, bn, pb, ib);					// ﾄﾗｯｶｰ頂点取得
	delete[] pw;
}

/************************************************************************/
void DeleteCurvePoint(int md, double* da, double* db, int bc, double wt,
                      int* no, int an, PNTTYPE* pa, int* ia, int* bn)	//<<< 制御点･通過点削除
/************************************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int      i, ct;

	CalcCurvePoint(md, an, pa, pw, da);									// 制御点=>通過点変換
	for(i=0, ct=0; i<an; i++) {
		if(no[i]!=1) {													// 制御点/ﾓｰﾄﾞ再設定
			pw[ct] = pw[i], ia[ct] = ia[i], no[ct++] = 0;
		}
	}
	SetCurveMatrix3(md, wt, bc, ct, ia, da, db);						// 制御点<=>通過点変換ﾏﾄﾘｯｸｽ
	CalcCurvePoint(md, ct, pw, pa, db);									// 通過点=>制御点変換
	*bn = ct;
	delete[] pw;
}

/***********************************************************/
static void ChangeCurveMode(int md, double* da, double* db,
                            int bc, double wt, int no,
                            int an, PNTTYPE* pa, int* ia)				//<<< 曲線ﾓｰﾄﾞ反転
/***********************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];

	CalcCurvePoint(md, an, pa, pw, da);									// 制御点=>通過点変換
	ia[no] = (ia[no]) ? 0 : 1;											// 折線/曲線反転
	SetCurveMatrix3(md, wt, bc, an, ia, da, db);						// 制御点<=>通過点変換ﾏﾄﾘｯｸｽ
	CalcCurvePoint(md, an, pw, pa, db);									// 通過点=>制御点変換
	delete[] pw;
}

/*****************************************************************/
static void AddCurvePoint(int md, double* da, double* db, int bc,
                          int rp, double wt, int no, int* ic,
                          int sft, int an, PNTTYPE* pa, int* ia)		//<<< 制御点･通過点追加
/*****************************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* px = new PNTTYPE[View->m_MaxSCtA];
	int*     ix = new int[View->m_MaxSCtA];
	PNTTYPE  pnt;
	int      i, j, pn, ut, wk;

	if(md) {															// <通過点指定の場合>
		pn = an;														// 制御点数
		for(ut=1, i=0; i<rp-1; i++) {
			ut *= 2;													// 分割により増える頂点数
		}
		for(i=0; i<pn; i++) {
			px[i] = pa[i], ix[i] = ia[i];								// 制御点,曲線ﾓｰﾄﾞ複写
		}
		wk = MakeSCurve(&pn, bc, rp, wt, px, ix);						// 曲線化
		for(i=0; i<an; i++) {
			px[i] = px[2*ut*i+ut];										// 中間点だけを抽出
		}
		for(i=0; i<wk; i++) {											// ｼﾌﾄ数分戻す
			pnt = px[an-1];												// 最後尾のﾃﾞｰﾀ保存
			for(j=an-1; j>0; j--) {
				px[j] = px[j-1];										// 後へｼﾌﾄ
			}
			px[0] = pnt;												// 最後尾ﾃﾞｰﾀを先頭へ
		}
		CalcCurvePoint(md, an, pa, pw, da);								// 制御点=>通過点変換
		for(i=an-1; i>no; i--) {
			pw[i+1] = pw[i], ia[i+1] = ia[i], ic[i+1] = ic[i];			// 指定点より後をｼﾌﾄ
		}
		no++;															// 追加位置番号
		pw[no] = px[no-1];												// 中間の通過点追加
		ia[no] = (sft) ? 1 : 0;											// Shift折線点ﾓｰﾄﾞ
		SetCurveMatrix3(md, wt, bc, an+1, ia, da, db);					// 制御点<=>通過点変換ﾏﾄﾘｯｸｽ
		CalcCurvePoint(md, an+1, pw, pa, db);							// 通過点=>制御点変換
	} else {															// <制御点指定の場合>
		for(i=an-1; i>no; i--) {
			pa[i+1] = pa[i], ia[i+1] = ia[i], ic[i+1] = ic[i];			// 指定点より後をｼﾌﾄ
		}
		no++;															// 追加位置番号
		if(no==an) {													// <境界:閉の最後>
			for(i=0; i<DIM; i++) {
				pa[no].p[i] = (pa[no-1].p[i]+pa[0].p[i])/2.0;			// 制御点設定
			}
		} else {														// <中間の制御点>
			for(i=0; i<DIM; i++) {
				pa[no].p[i] = (pa[no-1].p[i]+pa[no+1].p[i])/2.0;		// 制御点設定
			}
		}
		ia[no] = (sft) ? 1 : 0;											// Shift折線点ﾓｰﾄﾞ
	}
	delete[] pw;
	delete[] px;
	delete[] ix;
}

/****************************************************************/
void PickECurve(int pn, int* pk, int md, double* da, double* db,
                int* an, int bc, int rp, double wt, int* tn,
                OBJTYPE* op, int sf, PNTTYPE* pa, PNTTYPE* pb,
                int* ia, int* ib, int* ic)								//<<< 曲線編集 点選択
/****************************************************************/
{
	int i, j, pf=0;

	for(i=0; i<pn; i++) {
		if(pk[i]%2==0) {												// <制御点選択の場合>
			j = pk[i]/2;												// 制御点番号取得
			ic[j] = (ic[j]) ? 0 : 1;									// 選択状態反転
			if(sf) {
				ChangeCurveMode(md, da, db, bc, wt, j, *an, pa, ia);	// 曲線ﾓｰﾄﾞ反転
			}
			pf = 1;														// 制御点選択済み
		}
	}
	if(!pf) {															// <<制御点未選択の場合>>
		for(i=pn-1; i>=0; i--) {
			if(pk[i]%2!=0) {											// <制御点追加の場合>
				j = (pk[i]-1)/2;										// 追加位置直前番号
				AddCurvePoint(md, da, db, bc, rp, wt, j,
                              ic, sf, *an, pa, ia);						// 制御点･通過点追加
				ic[++j] = 1;											// 対象制御点選択
				(*an)++;												// 制御点数加算
			}
		}
	}
	RedoChgCurve(*an, bc, pa, ia);										// 曲線生成
	TrackerECurve(md, da, bc, *an, pa, ia, ic, tn, pb, ib);				// 曲線編集ﾄﾗｯｶｰ取得
	RedoSetCurve(*tn, bc, pb);											// 曲線制御点生成
	for(i=0; i<*tn; i++) {
		GetVtx(op, i, &pb[i]);											// 立体編集後制御点取得
	}
}

/*****************************************************************/
static void MoveCurvePoint(OBJTYPE* op, PNTTYPE opt, PNTTYPE npt,
                           int mx, int fg, PNTTYPE* pc)					//<<< 制御点移動
/*****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	CMDTYPE* cmp;
	PNTTYPE  wpt, spt, p1, p2, p3;
	int      i, i1, i2, i3, i4, i5, i6, i7, wf, ct, cno;
	double   d1, d2, d3, d4, d5, d6, d7, d8, d9;

	if(fg==1) {															// <移動対象の場合>
		while(mx>0) {
			wpt = opt;													// 立体編集前制御点
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			NextCmdPtr(&cmp);											// 曲線編集後ｺﾏﾝﾄﾞ
			ct = 0;														// ｺﾏﾝﾄﾞｶｳﾝﾀ初期化
			while(cmp!=NULL) {
				if(ct>mx-1) {											// ｺﾏﾝﾄﾞ個数以上中止
					break;
				}
				wf = (ct==mx-1) ? 1 : 0;								// 指定個数目更新有
				cno = GetCmdNo(cmp);									// ｺﾏﾝﾄﾞNo取得
				if(cno==MOVE) {											// <平行移動の場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向移動量取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向移動量取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向移動量取得
					BackMove(wf, &wpt, &npt, d1, d2, d3);				// 逆平行移動実行
				} else if(cno==ROTATE) {								// <回転移動の場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// 回転角度取得
					GetParaPnt(cmp, 0, &p1);							// 回転中心点取得
					GetParaPnt(cmp, 1, &p2);							// 回転軸ﾍﾞｸﾄﾙ取得
					BackRotate(wf, &wpt, &npt, p1, p2, d1);				// 逆回転移動実行
				} else if(cno==MIRROR) {								// <鏡像移動の場合>
					i1 = GetParaInt(cmp, 0);							// 対象平面取得
					GetParaPnt(cmp, 0, &p1);							// 鏡像軸点取得
					GetParaPnt(cmp, 1, &p2);							// 鏡像軸点取得
					BackMirror(wf, &wpt, &npt, p1, p2, i1);				// 逆鏡像移動実行
				} else if(cno==SCALE) {									// <ｻｲｽﾞ変更の場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向倍率取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向倍率取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向倍率取得
					GetParaPnt(cmp, 0, &p1);							// ｻｲｽﾞ変更中心点取得
					BackScale(wf, &wpt, &npt, p1, d1, d2, d3);			// 逆ｻｲｽﾞ変更実行
				} else if(cno==RESIZE1) {								// <直線指定ﾘｻｲｽﾞ1の場合>
					i1 = GetParaInt(cmp, 0);							// 指定平面取得
					i2 = GetParaInt(cmp, 1);							// 正負ﾌﾗｸﾞ取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// 倍率1取得
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// 倍率2取得
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// 倍率3取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					GetParaPnt(cmp, 1, &p2);							// 基準点2取得
					GetParaPnt(cmp, 2, &p3);							// 基準点3取得
					BackResize1(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                p1, p2, p3);							// 逆直線指定ﾘｻｲｽﾞ1実行
				} else if(cno==RESIZE2) {								// <曲線指定ﾘｻｲｽﾞ1の場合>
					i1 = GetParaInt(cmp, 0);							// 正負ﾌﾗｸﾞ取得
					i2 = GetParaInt(cmp, 1);							// 両側ﾌﾗｸﾞ取得
					i3 = GetParaInt(cmp, 2);							// 長さ方向取得
					i4 = GetParaInt(cmp, 3);							// ｻｲｽﾞ指定方向取得
					d1 = (double)GetParaInt(cmp, 4)/1000.0;				// 基準長さ取得
					i5 = GetParaInt(cmp, 5);							// 制御点数取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					for(i=0; i<i5; i++) {
						ia[i] = GetParaInt(cmp, i+6);					// 曲線ﾓｰﾄﾞ取得
						GetParaPnt(cmp, i+1, &pa[i]);					// 曲線制御点取得
					}
					BackResize2(wf, &wpt, &npt, i1, i2, i3, i4, d1,
                                p1, i5, pa, ia);						// 逆曲線指定ﾘｻｲｽﾞ1実行
				} else if(cno==BEND) {									// <円筒曲げの場合>
					i1 = GetParaInt(cmp, 0);							// 曲げ水平方向取得
					i2 = GetParaInt(cmp, 1);							// 曲げ垂直方向取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// 曲げ角度取得
					GetParaPnt(cmp, 0, &p1);							// 基準線点取得
					GetParaPnt(cmp, 1, &p2);							// 基準線点取得
					BackBend(wf, &wpt, &npt, i1, i2, d1, p1, p2);		// 逆円筒曲げ実行
				} else if(cno==TWIST) {									// <ひねりの場合>
					i1 = GetParaInt(cmp, 0);							// ひねり軸方向取得
					d1 = (double)GetParaInt(cmp, 1)/1000.0;				// ひねり角度取得
					d2 = (double)GetParaInt(cmp, 2)/1000.0;				// ひねり軸幅取得
					GetParaPnt(cmp, 0, &p1);							// ひねり基準点取得
					BackTwist(wf, &wpt, &npt, i1, d1, d2, p1);			// 逆ひねり実行
				} else if(cno==SHEAR) {									// <せん断の場合>
					i1 = GetParaInt(cmp, 0);							// せん断垂直方向取得
					i2 = GetParaInt(cmp, 1);							// せん断水平方向取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// せん断傾き取得
					GetParaPnt(cmp, 0, &p1);							// 基準線点取得
					BackShear(wf, &wpt, &npt, i1, i2, d1, p1);			// 逆せん断実行
				} else if(cno==RING) {									// <ﾘﾝｸﾞの場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向倍率取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向倍率取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向倍率取得
					i1 = GetParaInt(cmp, 3);							// 曲げ水平方向取得
					i2 = GetParaInt(cmp, 4);							// 曲げ垂直方向取得
					d4 = (double)GetParaInt(cmp, 5)/1000.0;				// 曲げ角度取得
					GetParaPnt(cmp, 0, &p1);							// ｻｲｽﾞ変更中心点取得
					GetParaPnt(cmp, 1, &p2);							// 基準線点取得
					GetParaPnt(cmp, 2, &p3);							// 基準線点取得
					if(wf==0) {											// <更新対象外>
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// 逆ｻｲｽﾞ変更実行(更新無)
						BackBend(0, &wpt, &npt, i1, i2, d4, p2, p3);	// 逆円筒曲げ実行(更新無)
					} else {											// <更新対象>
						spt = wpt;										// 制御点保存
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// 逆ｻｲｽﾞ変更実行(更新無)
						BackBend(1, &wpt, &npt, i1, i2, d4, p2, p3);	// 逆円筒曲げ実行(更新有)
						wpt = spt;										// 制御点復元
						BackScale(1, &wpt, &npt, p1, d1, d2, d3);		// 逆ｻｲｽﾞ変更実行(更新有)
					}
				} else if(cno==EARTH) {									// <ｱｰｽの場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向移動量取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向移動量取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向移動量取得
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X軸回転角度取得
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y軸回転角度取得
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z軸回転角度取得
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X方向倍率取得
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y方向倍率取得
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z方向倍率取得
					GetParaPnt(cmp, 0, &p1);							// 回転中心点取得
					GetParaPnt(cmp, 1, &p2);							// ｻｲｽﾞ変更中心点取得
					if(wf==0) {											// <更新対象外>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新無)
					} else {											// <更新対象>
						spt = wpt;										// 制御点保存
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新有)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新有)
					}
				} else if(cno==SPACE) {									// <ｽﾍﾟｰｽの場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向移動量取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向移動量取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向移動量取得
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X軸回転角度取得
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y軸回転角度取得
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z軸回転角度取得
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X方向倍率取得
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y方向倍率取得
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z方向倍率取得
					GetParaPnt(cmp, 0, &p1);							// 回転中心点取得
					GetParaPnt(cmp, 1, &p2);							// ｻｲｽﾞ変更中心点取得
					if(wf==0) {											// <更新対象外>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新無)
					} else {											// <更新対象>
						spt = wpt;										// 制御点保存
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新有)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新有)
					}
				} else if(cno==RESIZE3) {								// <直線指定ﾘｻｲｽﾞ2の場合>
					i1 = GetParaInt(cmp, 0);							// 指定平面取得
					i2 = GetParaInt(cmp, 1);							// 正負ﾌﾗｸﾞ取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// 倍率1取得
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// 倍率2取得
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// 倍率3取得
					i3 = GetParaInt(cmp, 5);							// 垂直限定ﾌﾗｸﾞ取得
					d4 = (double)GetParaInt(cmp, 6)/1000.0;				// 垂直範囲1取得
					d5 = (double)GetParaInt(cmp, 7)/1000.0;				// 垂直範囲2取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					GetParaPnt(cmp, 1, &p2);							// 基準点2取得
					GetParaPnt(cmp, 2, &p3);							// 基準点3取得
					BackResize3(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                i3, d4, d5, p1, p2, p3);				// 逆直線指定ﾘｻｲｽﾞ2実行
				} else if(cno==RESIZE4) {								// <曲線指定ﾘｻｲｽﾞ2の場合>
					i1 = GetParaInt(cmp, 0);							// 正負ﾌﾗｸﾞ取得
					i2 = GetParaInt(cmp, 1);							// 両側ﾌﾗｸﾞ取得
					i3 = GetParaInt(cmp, 2);							// 長さ方向取得
					i4 = GetParaInt(cmp, 3);							// ｻｲｽﾞ指定方向取得
					i5 = GetParaInt(cmp, 4);							// 垂直方向取得
					d1 = (double)GetParaInt(cmp, 5)/1000.0;				// 基準長さ取得
					i6 = GetParaInt(cmp, 6);							// 垂直限定ﾌﾗｸﾞ取得
					d2 = (double)GetParaInt(cmp, 7)/1000.0;				// 垂直範囲1取得
					d3 = (double)GetParaInt(cmp, 8)/1000.0;				// 垂直範囲2取得
					i7 = GetParaInt(cmp, 9);							// 制御点数取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					for(i=0; i<i7; i++) {
						ia[i] = GetParaInt(cmp, i+10);					// 曲線ﾓｰﾄﾞ取得
						GetParaPnt(cmp, i+1, &pa[i]);					// 曲線制御点取得
					}
					BackResize4(wf, &wpt, &npt, i1, i2, i3, i4, i5,
                                d1, i6, d2, d3, p1, i7, pa, ia);		// 逆曲線指定ﾘｻｲｽﾞ2実行
				}
				ct++;													// ｺﾏﾝﾄﾞｶｳﾝﾀUP
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			mx--;														// ｺﾏﾝﾄﾞ個数減算
		}
		*pc = npt;
	} else {															// <移動対象外の場合>
		*pc = opt;
	}
	delete[] pa;
	delete[] ia;
}

/***********************************************************/
void MoveECurve(int md, double* db, PNTTYPE* ma,
                OBJTYPE* op, int pn, int bc, PNTTYPE* pa,
                PNTTYPE* pb, int* ia, PNTTYPE* pc, int* ta)				//<<< 制御点･通過点移動
/***********************************************************/
{
	PNTTYPE* mov = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxCCtA];
	int*     flg = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  opt, npt;
	int      i, j, k, ten, p1, p2, sct, ect, sno, eno, mx=0, lst, ok;
	double   sln, wln;

	ten = View->GetSurfaceTension();									// 張力設定番号取得
	BaseCmdPtr(1, op, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲線編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		mx++;															// ｺﾏﾝﾄﾞ個数ｶｳﾝﾄ
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	opt.p[0] = 0.0, opt.p[1] = 0.0, opt.p[2] = 0.0;						// 座標値:0配列作成
	for(i=0; i<pn; i++) {
		if(ia[i]) {
			mov[i] = ma[i], flg[i] = 1;									// 移動量/ﾌﾗｸﾞ配列複写
		} else {
			mov[i] = opt,   flg[i] = 0;
		}
	}
	if(ten>0) {															// <<張力設定有の場合>>
		for(i=0, j=0, p1=-1, p2=-1; i<pn-1; i++) {
			if(flg[i]!=flg[i+1]) {										// <前後ﾌﾗｸﾞ異>
				j++;													// 指定点境界ｶｳﾝﾄ
				if(flg[i]) {											// 境界1位置
					p1 = i+1;
				} else {												// 境界2位置
					p2 = i;
				}
			}
		}
		if(j<=2) {														// <指定点連続>
			for(i=0; i<pn; i++) {										// 制御点間距離測定
				len[i] = (i==pn-1) ? PntD(&pb[i*2], &pb[0]) :
                                     PntD(&pb[i*2], &pb[(i+1)*2]);
			}
			if(p1==-1) {												// 指定点前後位置&点数設定
				if(bc) {
					sct = (p2+1)/2, ect = (p2+1)/2, sno = p2, eno = pn-1;
				} else {
					sct = p2+1, ect = 0, sno = p2, eno = pn-1;
				}
			} else if(p2==-1) {
				if(bc) {
					sct = (pn-p1)/2, ect = (pn-p1)/2, sno = pn-1, eno = p1-1;
				} else {
					sct = 0, ect = pn-p1, sno = pn-1, eno = p1-1;
				}
			} else if(p1>p2) {
				if(bc) {
					sct = (p2+1+pn-p1)/2, ect = (p2+1+pn-p1)/2, sno = p2, eno = p1-1;
				} else {
					sct = p2+1, ect = pn-p1, sno = p2, eno = p1-1;
				}
			} else {
				sct = (p2-p1+1)/2, ect = (p2-p1+1)/2, sno = p2, eno = p1-1;
			}
			for(i=0, j=sno, lst=-1; i<sct; i++) {						// <指定点前制御点移動>
				lst = (ta[j]) ? j : lst;								// 最終張力有効点検索
				j = (j-1<0) ? pn-1 : j-1;
			}
			ok = (lst==-1) ? 0 : 1;										// 張力有効点有無
			lst = (lst-1<0) ? pn-1 : lst-1;								// 有効点前線分も含む
			for(i=0, sln=0.0, j=sno; i<sct; i++) {
				sln = (ok) ? sln+len[j] : sln;							// 制御点間距離集計
				ok = (j==lst) ? 0 : ok;									// 最終張力有効点以降は無視
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, wln=0.0, j=sno; i<sct; i++) {
				wln += len[j];											// 指定制御点からの距離
				if(ta[j]) {												// <張力有効点>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// 張力設定値変換
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// 指定ﾌﾗｸﾞON
					}
				}
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, j=eno, lst=-1; i<ect; i++) {						// <指定点後制御点移動>
				lst = (ta[j]) ? j : lst;								// 最終張力有効点検索
				j = (j+1>pn-1) ? 0 : j+1;
			}
			ok = (lst==-1) ? 0 : 1;										// 張力有効点有無
			for(i=0, sln=0.0, j=eno; i<ect; i++) {
				k = (j+1>pn-1) ? 0 : j+1;
				sln = (ok||ta[k]) ? sln+len[j] : sln;					// 制御点間距離集計
				ok = (j==lst) ? 0 : ok;									// 最終張力有効点以降は無視
				j = (j+1>pn-1) ? 0 : j+1;
			}
			for(i=0, wln=0.0, j=eno; i<ect; i++) {
				wln += len[j];											// 指定制御点からの距離
				j = (j+1>pn-1) ? 0 : j+1;
				if(ta[j]) {												// <張力有効点>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// 張力設定値変換
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// 指定ﾌﾗｸﾞON
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mov[i].p[j] += pb[i*2].p[j];								// 立体編集&移動後座標
		}
	}
	CalcCurvePoint(md, pn, mov, cpt, db);								// 通過点=>制御点変換
	if(md) {
		for(i=0; i<pn; i++) {
			flg[i] = 1;													// 通過点は全て移動済み
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i], npt = cpt[i];										// 移動前/移動後 制御点
		MoveCurvePoint(op, opt, npt, mx, flg[i], &pc[i]);				// 制御点移動
	}
	delete[] mov;
	delete[] cpt;
	delete[] flg;
	delete[] len;
}

/************************************************************/
BOOL ExtractionCurve(int un, int vn, int ub, int vb, int f1,
                     int f2, int no, PNTTYPE* pa, int* ia,
                     int* pn, int* bc, PNTTYPE* pb, int*ib)				//<<< 曲線抽出
/************************************************************/
{
	int i, wuv, wua, wva, flg, lno;
	BOOL ret=TRUE;

	wva = (f1%2==1) ? 1 : 0;											// U/V境界と補正数取得
	wua = (f2%2==1) ? 1 : 0;
	wuv = (f1%2==1) ? vn-1 : vn;
	wuv = (f1>=2) ? wuv-1 : wuv;
	if(no<wuv) {														// <Vﾗｲﾝ>
		flg = 1;
		lno = no+wva;
	} else {															// <Uﾗｲﾝ>
		flg = 0;
		lno = no-wuv+wua;
	}
	if(flg==0) {														// <<Uﾗｲﾝ指定時>>
		*pn = vn;														// 曲線制御点数
		*bc = vb;														// 曲線境界条件
		if(vn>View->m_MaxCCtl) {										// <制御点上限以上>
			AfxMessageBox(IDS_MANY_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
			ret = FALSE;
		} else if(vn<3) {												// <制御点下限以下>
			AfxMessageBox(IDS_FEW_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
			ret = FALSE;
		}
		if(ret) {
			for(i=0; i<vn; i++) {
				pb[i] = pa[lno+(i*un)];									// 制御点
				ib[i] = ia[lno+(i*un)]/2;								// 曲線ﾓｰﾄﾞ
			}
		}
	} else {															// <<Vﾗｲﾝ指定時>>
		*pn = un;														// 曲線制御点数
		*bc = ub;														// 曲線境界条件
		if(un>View->m_MaxCCtl) {										// <制御点上限以上>
			AfxMessageBox(IDS_MANY_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
			ret = FALSE;
		} else if(un<3) {												// <制御点下限以下>
			AfxMessageBox(IDS_FEW_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
			ret = FALSE;
		}
		if(ret) {
			for(i=0; i<un; i++) {
				pb[i] = pa[(lno*un)+i];									// 制御点
				ib[i] = ia[(lno*un)+i]%2;								// 曲線ﾓｰﾄﾞ
			}
		}
	}
	return ret;
}
