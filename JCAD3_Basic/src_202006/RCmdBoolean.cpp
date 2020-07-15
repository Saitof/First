/////////////////////////////////////////////////////////////////////////////
// コマンド関連 図形演算部
// RCmdBoolean.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include <limits.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "BooleanType.h"
#include "NumFunc.h"
CProgressDlg* m_pProgressB=NULL;

/******************************/
static void InitWorkArea(void)											//<<< 図形演算初期処理
/******************************/
{
	Aflist = NULL, Bflist = NULL, A0list = NULL;
	A1list = NULL, B0list = NULL, B1list = NULL;
	Afcnt = 0, Bfcnt = 0, A0cnt = 0;
	A1cnt = 0, B0cnt = 0, B1cnt = 0;
}

/********************************************************/
static BOOL InPointSub(int d, PNTTYPE cp, PNTTYPE* mi,
                       PNTTYPE* pa, int pn, PNTTYPE* ip)				//<<< 図形内存在点取得ｻﾌﾞ
/********************************************************/
{
	PNTTYPE* pt = new PNTTYPE[100];
	PNTTYPE* pp = new PNTTYPE[100];
	double*  ln = new double[100];
	PNTTYPE  p1, p2, la;
	int      i, j, k, l, ct=0, ret=FALSE;
	double   m, lb;

	for(i=0; i<pn; i++) {
		j = (i==pn-1) ? 0 : i+1;
		if(pa[i].p[d]<pa[j].p[d]) {										// d座標が小がp1,大がp2
			p1 = pa[i], p2 = pa[j];
		} else {
			p1 = pa[j], p2 = pa[i];
		}
		if(p1.p[d]<=cp.p[d]&&p2.p[d]>cp.p[d]&&ct<100) {					// <中間点を通る線分>
			for(k=0; k<DIM; k++) {
				la.p[k] = p2.p[k]-p1.p[k];								// 線分の長さ
			}
			lb = (cp.p[d]-p1.p[d])/la.p[d];								// 開始点から面中心点までの比率
			for(k=0; k<DIM; k++) {
				pt[ct].p[k] = p1.p[k]+lb*la.p[k];						// 線分の面中心点位置の座標
			}
			ct++;
		}
	}
	if(ct==2) {															// <有効数の場合>
		for(i=0; i<DIM; i++) {
			ip->p[i] = (pt[0].p[i]+pt[1].p[i])/2.0;						// 2点の中心点
		}
		ret = TRUE;
	} else if(ct%2==0) {												// <偶数個の場合>
		for(i=0; i<ct; i++) {
			ln[i] = PntD(mi, &pt[i]);									// 最小点からの距離
		}
		for(i=0, k=0; i<ct; i++) {
			for(j=0, l=0, lb=0.0; j<ct; j++) {
				if(lb<ln[j]) {											// 有効な最長距離点
					lb = ln[j], l = j;
				}
			}
			pp[k++] = pt[l], ln[l] = -10.0;								// 最長距離順に並換
		}
		for(i=0, l=0, lb=0.0; i<ct/2; i++) {
			m = PntD(&pp[2*i], &pp[2*i+1]);								// 2点毎の距離
			if(lb<m) {													// 最長距離点保存
				lb = m, l = i;
			}
		}
		for(i=0; i<DIM; i++) {
			ip->p[i] = (pp[2*l].p[i]+pp[2*l+1].p[i])/2.0;				// 2点の中心点
		}
		ret = TRUE;
	}
	delete[] pt;
	delete[] pp;
	delete[] ln;
	return ret;
}

/**************************************************************/
static void CalcInPoint(PNTTYPE* pa, int pn,
                        PNTTYPE* mi, PNTTYPE* ma, PNTTYPE* ip)			//<<< 図形内存在点取得
/**************************************************************/
{
	PNTTYPE size, cp;
	int     i, j, d1, d2, d3;
	double  dp;

	for(i=0; i<DIM; i++) {												// X,Y,Z初期値設定
		mi->p[i] = DBL_MAX, ma->p[i] = 1.0-DBL_MAX;
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			if(mi->p[j]>pa[i].p[j]) {									// 面最小値取得
				mi->p[j] = pa[i].p[j];
			}
			if(ma->p[j]<pa[i].p[j]) {									// 面最大値取得
				ma->p[j] = pa[i].p[j];
			}
		}
	}
	for(i=0; i<DIM; i++) {
		size.p[i] = ma->p[i]-mi->p[i];									// 面のｻｲｽﾞ
		cp.p[i] = (ma->p[i]+mi->p[i])/2.0;								// 面の中心点
	}
	for(i=0, d1=0, dp=0.0; i<DIM; i++) {
		if(dp<size.p[i]) {												// 最大ｻｲｽﾞ検索
			dp = size.p[i], d1 = i;
		}
	}
	size.p[d1] = -10.0;
	for(i=0, d2=0, dp=0.0; i<DIM; i++) {
		if(dp<size.p[i]) {												// 次ｻｲｽﾞ検索
			dp = size.p[i], d2 = i;
		}
	}
	if(d1!=0&&d2!=0) {													// 最小ｻｲｽﾞ検索
		d3 = 0;
	} else if(d1!=1&&d2!=1) {
		d3 = 1;
	} else {
		d3 = 2;
	}
	if(!InPointSub(d1, cp, mi, pa, pn, ip)) {							// 面内存在点取得ｻﾌﾞ(1st)
		if(!InPointSub(d2, cp, mi, pa, pn, ip)) {						// 面内存在点取得ｻﾌﾞ(2nd)
			if(!InPointSub(d3, cp, mi, pa, pn, ip)) {					// 面内存在点取得ｻﾌﾞ(3rd)
				*ip = pa[0];											// 3回ともNGは1点目を使用
				ErrFlg1 = 1;											// ｴﾗｰﾌﾗｸﾞON
			}
		}
	}
}

/***************************************************************/
static void SetFaData(int flg, double x1, double x2, double y1,
                      double y2, double z1, double z2,
                      VECTYPE vc, PNTTYPE ip, int st)					//<<< 面情報ﾃﾞｰﾀ登録
/***************************************************************/
{
	BFATYPE* tf;

	tf = (BFATYPE *)malloc(sizeof(BFATYPE));							// 面情報領域確保
	if(tf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tf->xmin = x1, tf->xmax = x2;
	tf->ymin = y1, tf->ymax = y2;
	tf->zmin = z1, tf->zmax = z2;
	tf->vec  = vc, tf->inpt = ip, tf->stfl = st;
	if(!flg) {															// <A側>
		tf->next = Aflist, Aflist = tf, Afcnt++;
	} else {															// <B側>
		tf->next = Bflist, Bflist = tf, Bfcnt++;
	}
}

/************************************************/
static BFATYPE* MergeF(BFATYPE* la, BFATYPE* lb)						//<<< X最小値ｿｰﾄ
/************************************************/
{
	BFATYPE* lc, * ld, * rc;

	lc = (BFATYPE *)malloc(sizeof(BFATYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->xmin<=lb->xmin) {										// X最小値の大小で比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/************************************************/
static BFATYPE* F_Mergsrt(BFATYPE* mlist, int n)						//<<< X最小値のｿｰﾄ
/************************************************/
{
	BFATYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BFATYPE *)MergeF(F_Mergsrt(la, n/2),
                               F_Mergsrt(lb, n-n/2));					// 面ﾙｰﾌﾟ番号ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/**********************************************/
static void MakeFaceList(int flg, OBJTYPE* op)							//<<< 面情報ﾘｽﾄ作成
/**********************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE  pt, mi, ma, ip;
	VECTYPE  vc;
	int      i, num, ffg, st, pn, ipr=0;
	double   dpr;

	num = GetFlpNum1(op);												// 面ﾙｰﾌﾟ数取得
	for(i=0, st=-1; i<num; i++) {
		GetFlpVtx1(op, i, &pt, &ffg);									// 面ﾙｰﾌﾟ頂点取得
		if(st==-1) {													// 面ﾙｰﾌﾟ開始処理
			pn = 0, st = i;
		}
		pa[pn++] = pt;													// 頂点座標保存
		if(ffg) {														// <<面ﾙｰﾌﾟ終了>>
			CalcNVec(pa, pn, &vc);										// 法線ﾍﾞｸﾄﾙ取得
			CalcInPoint(pa, pn, &mi, &ma, &ip);							// 面内存在点取得
			SetFaData(flg, mi.p[0], ma.p[0], mi.p[1], ma.p[1],
                      mi.p[2], ma.p[2], vc, ip, st);					// 面情報ﾃﾞｰﾀ登録
			st = -1;													// ﾙｰﾌﾟ開始ﾌﾗｸﾞ初期化
		}
		dpr = (Ppr/6.0)*((double)i/(double)num);						// 現在ﾊﾟｰｾﾝﾄ
		if(ipr<(int)(Spr+dpr)) {										// <ﾊﾟｰｾﾝﾄUP>
			ipr = (int)(Spr+dpr);
			m_pProgressB->SetProgress(ipr);								// 進捗表示
		}
	}
	Spr += Ppr/6.0;														// 累積ﾊﾟｰｾﾝﾄ
	if(!flg) {															// A側X最小値のｿｰﾄ
		Aflist = F_Mergsrt(Aflist, Afcnt);
	} else {															// B側X最小値のｿｰﾄ
		Bflist = F_Mergsrt(Bflist, Bfcnt);
	}
	delete[] pa;
}

/*************************************************************/
static void SetFcData(int flg, int sno, int eno, PNTTYPE pnt)			//<<< 面交点算出ﾃﾞｰﾀ登録
/*************************************************************/
{
	BFCTYPE* tf;
	int      wno;

	tf = (BFCTYPE *)malloc(sizeof(BFCTYPE));							// 面交点算出領域確保
	if(tf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tf->flg = flg;
	if(sno<eno) {
		tf->sno = sno, tf->eno = eno;
	} else {
		tf->sno = eno, tf->eno = sno;
	}
	if((!flg)&&tf->eno-tf->sno>1) {										// <面ﾙｰﾌﾟで始終点>
		wno = tf->sno, tf->sno = tf->eno, tf->eno = wno;				// 終点→始点に戻す
	}
	tf->pnt = pnt;
	tf->next = Fclist, Fclist = tf;
}

/*********************************************************/
static BFCTYPE* MergeFc(BFCTYPE* la, BFCTYPE* lb, int id)				//<<< 面交点算出ｿｰﾄ
/*********************************************************/
{
	BFCTYPE* lc, * ld, * rc;

	lc = (BFCTYPE *)malloc(sizeof(BFCTYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pnt.p[id]<=lb->pnt.p[id]) {								// 頂点座標大小で比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/*********************************************************/
static BFCTYPE* Fc_Mergsrt(BFCTYPE* mlist, int n, int id)				//<<< 面交点算出のｿｰﾄ
/*********************************************************/
{
	BFCTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BFCTYPE *)MergeFc(Fc_Mergsrt(la, n/2, id),
                                Fc_Mergsrt(lb, n-n/2, id), id);			// 頂点座標ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/*************************************************************************/
static void SetClDataSub(int sfg, int ssn, int sen, PNTTYPE spt, int efg,
                         int esn, int een, PNTTYPE ept, int pfg)		//<<< 面交差稜線ﾃﾞｰﾀ設定
/*************************************************************************/
{
	BCLTYPE* tc;

	tc = (BCLTYPE *)malloc(sizeof(BCLTYPE));							// 面交差稜線領域確保
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->sfg = sfg, tc->ssn = ssn, tc->sen = sen, tc->spt = spt;			// 始点側情報
	tc->efg = efg, tc->esn = esn, tc->een = een, tc->ept = ept;			// 終点側情報
	tc->pfg = pfg;														// 出力ﾌﾗｸﾞ
	tc->cfs = 0;														// 確認ﾌﾗｸﾞ
	tc->cfe = 0;
	tc->next = Cllist, Cllist = tc;
}

/***************************/
static BOOL SetClData(void)												//<<< 面交差稜線ﾃﾞｰﾀ設定
/***************************/
{
	BFCTYPE* wfc, * sf1=NULL, * sf2=NULL, * wf0=NULL, * wf1=NULL;
	BOOL     ret=FALSE;
	int      psw=0, pf0=0, pf1=0, wp0=0, wp1=0;

	wfc = Fclist;
	while(wfc!=NULL) {
		if(!wfc->flg) {													// 出力側面の場合
			wp0 = 1;
			pf0 = (!pf0) ? 1 : 0;
		} else {														// 比較側面の場合
			wp1 = 1;
			pf1 = (!pf1) ? 1 : 0;
		}
		if(psw==2) {													// <出力ﾃﾞｰﾀ有>
			SetClDataSub(sf1->flg, sf1->sno, sf1->eno, sf1->pnt,
                         sf2->flg, sf2->sno, sf2->eno, sf2->pnt, 0);	// 面交差稜線ﾃﾞｰﾀ設定
			ret = TRUE;													// RET=出力済み
			psw = 0;
			if(!wfc->flg&&
               fabs(wfc->pnt.p[0]-sf2->pnt.p[0])<EPSILON&&
               fabs(wfc->pnt.p[1]-sf2->pnt.p[1])<EPSILON&&
               fabs(wfc->pnt.p[2]-sf2->pnt.p[2])<EPSILON) {				// 出力終点と近似で出力側は
				SetClDataSub(sf2->flg, sf2->sno, sf2->eno, sf2->pnt,
                             wfc->flg, wfc->sno, wfc->eno, wfc->pnt, 1);// 面交差稜線ﾃﾞｰﾀ設定(仮)
			}
		}
		if(!pf0&&!pf1) {												// <両線分が終了>
			psw = 0;
			if(wp0&&!wp1) {												// 出力側ﾃﾞｰﾀ保存
				wf0 = wfc;
			}
			if(!wp0&&wp1) {												// 比較側ﾃﾞｰﾀ保存
				wf1 = wfc;
			}
		} else if(pf0&&pf1) {											// <両線分が開始>
			if(wfc->flg&&
               fabs(wfc->pnt.p[0]-sf1->pnt.p[0])<EPSILON&&
               fabs(wfc->pnt.p[1]-sf1->pnt.p[1])<EPSILON&&
               fabs(wfc->pnt.p[2]-sf1->pnt.p[2])<EPSILON) {				// 開始前点と近似で出力側は
				SetClDataSub(sf1->flg, sf1->sno, sf1->eno, sf1->pnt,
                             wfc->flg, wfc->sno, wfc->eno, wfc->pnt, 1);// 面交差稜線ﾃﾞｰﾀ設定(仮)
				ret = TRUE;												// RET=出力済み
			}
			sf1 = wfc;													// ﾃﾞｰﾀ入替
			psw = 1;													// 出力ﾌﾗｸﾞ:始点設定
		} else {														// <一方が開始>
			if(psw==0) {												// 始点ﾎﾟｲﾝﾀ保存
				sf1 = wfc;
			} else if(psw==1) {											// 終点ﾎﾟｲﾝﾀ保存
				sf2 = wfc;
				psw = 2;
			}
			if(pf0&&wf1!=NULL) {										// 出力側ﾃﾞｰﾀ保存
				wf0 = wfc;
			}
			if(pf1&&wf0!=NULL) {										// 比較側ﾃﾞｰﾀ保存
				wf1 = wfc;
			}
			if(wf0!=NULL&&wf1!=NULL) {
				if(abs(wf0->pnt.p[0]-wf1->pnt.p[0])<EPSILON&&
                   abs(wf0->pnt.p[1]-wf1->pnt.p[1])<EPSILON&&
                   abs(wf0->pnt.p[2]-wf1->pnt.p[2])<EPSILON) {			// 出力側と比較側が近似は
					SetClDataSub(wf0->flg, wf0->sno, wf0->eno,
                                 wf0->pnt, wf1->flg, wf1->sno,
                                 wf1->eno, wf1->pnt, 1);				// 面交差稜線ﾃﾞｰﾀ設定(仮)
					ret = TRUE;											// RET=出力済み
				}
				wp0 = 0, wp1 = 0, wf0 = NULL, wf1 = NULL;
			}
		}
		wfc = wfc->next;												// 次ﾎﾟｲﾝﾀへ
	}
	if(psw==2) {														// <出力ﾃﾞｰﾀ有>
		SetClDataSub(sf1->flg, sf1->sno, sf1->eno, sf1->pnt,
                     sf2->flg, sf2->sno, sf2->eno, sf2->pnt, 0);		// 面交差稜線ﾃﾞｰﾀ設定
		ret = TRUE;														// RET=出力済み
	}
	return ret;
}

/******************************************************/
static BOOL CheckCrossFace(BFATYPE* fp1, BFATYPE* fp2,
                           OBJTYPE* op1, OBJTYPE* op2)					//<<< 面交差ﾁｪｯｸ
/******************************************************/
{
	BFCTYPE* wfc, * dfc;
	PNTTYPE  zr, mi, ma, cp, sp, ep, pt, pp;
	VECTYPE  vc;
	int      i, ix, ix1, sv, ev, sf, ef, cnt1=0, cnt2=0;
	BOOL     ret=FALSE;
	double   d1, d2, n[3], t, ln;

	for(i=0; i<DIM; i++) {
		zr.p[i] = 0.0;													// 原点座標
	}
	Fclist = NULL;														// 作業領域初期化
	for(i=0; i<DIM; i++) {
		mi.p[i] = DBL_MAX, ma.p[i] = 1.0-DBL_MAX;						// X,Y,Z初期値設定
	}
	vc = fp2->vec;														// 平面:fp2法線ﾍﾞｸﾄﾙ
	cp = fp2->inpt;														// 平面:fp2上の点
	d1 = vc.p[0]*cp.p[0]+vc.p[1]*cp.p[1]+vc.p[2]*cp.p[2];				// 平面:fp2定数
	ix = fp1->stfl;														// 平面:fp1ﾙｰﾌﾟ開始番号
	while(TRUE) {
		GetFlpVtx1(op1, ix, &sp, &sf);									// 平面:fp1ﾙｰﾌﾟ頂点取得
		ix1 = (!sf) ? ix+1 : fp1->stfl;									// 平面:fp1次ﾙｰﾌﾟ頂点取得
		GetFlpVtx1(op1, ix1, &ep, &ef);
		if(PntD(&zr, &sp)>PntD(&zr, &ep)) {								// <終点の方が原点に近い>
			pt = sp, sp = ep, ep = pt;									// 始点,終点入替
		}
		for(i=0; i<DIM; i++) {
			n[i] = ep.p[i]-sp.p[i];										// 平面:fp1線分ﾍﾞｸﾄﾙ
		}
		d2 = vc.p[0]*sp.p[0]+vc.p[1]*sp.p[1]+vc.p[2]*sp.p[2];			// 交点算出計算
		t = vc.p[0]*n[0]+vc.p[1]*n[1]+vc.p[2]*n[2];
		if(fabs(t)>DEPSILON) {
			t = (d1-d2)/t;
			for(i=0; i<DIM; i++) {
				pp.p[i] = sp.p[i]+n[i]*t;								// 平面:fp2&線分交点
			}
			if(((sp.p[0]<=pp.p[0]&&pp.p[0]<=ep.p[0])||
                (ep.p[0]<=pp.p[0]&&pp.p[0]<=sp.p[0]))&&
               ((sp.p[1]<=pp.p[1]&&pp.p[1]<=ep.p[1])||
                (ep.p[1]<=pp.p[1]&&pp.p[1]<=sp.p[1]))&&
               ((sp.p[2]<=pp.p[2]&&pp.p[2]<=ep.p[2])||
                (ep.p[2]<=pp.p[2]&&pp.p[2]<=sp.p[2]))) {				// <線分範囲内>
				SetFcData(0, ix, ix1, pp);								// 面交点設定
				cnt1++;													// ｶｳﾝﾄ
				for(i=0; i<DIM; i++) {
					if(mi.p[i]>pp.p[i]) {								// 最小･最大値取得
						mi.p[i] = pp.p[i];
					}
					if(ma.p[i]<pp.p[i]) {
						ma.p[i] = pp.p[i];
					}
				}
			}
		}
		if(sf) {														// 面ﾙｰﾌﾟ終了=>処理終了
			break;
		}
		ix++;
	}
	vc = fp1->vec;														// 平面:fp1法線ﾍﾞｸﾄﾙ
	cp = fp1->inpt;														// 平面:fp1上の点
	d1 = vc.p[0]*cp.p[0]+vc.p[1]*cp.p[1]+vc.p[2]*cp.p[2];				// 平面:fp1定数
	ix = fp2->stfl;														// 平面:fp2ﾙｰﾌﾟ開始番号
	while(TRUE) {
		GetFlp1(op2, ix, &sv, &sf);										// 平面:fp2ﾙｰﾌﾟ頂点取得
		GetVtx(op2, sv, &sp);
		if(!sf) {														// 平面:fp2次ﾙｰﾌﾟ頂点取得
			GetFlp1(op2, ix+1, &ev, &ef);
		} else {
			GetFlp1(op2, fp2->stfl, &ev, &ef);
		}
		GetVtx(op2, ev, &ep);
		if(PntD(&zr, &sp)>PntD(&zr, &ep)) {								// <終点の方が原点に近い>
			pt = sp, sp = ep, ep = pt;									// 始点,終点入替
		}
		for(i=0; i<DIM; i++) {
			n[i] = ep.p[i]-sp.p[i];										// 平面:fp2線分ﾍﾞｸﾄﾙ
		}
		d2 = vc.p[0]*sp.p[0]+vc.p[1]*sp.p[1]+vc.p[2]*sp.p[2];			// 交点算出計算
		t = vc.p[0]*n[0]+vc.p[1]*n[1]+vc.p[2]*n[2];
		if(fabs(t)>DEPSILON) {
			t = (d1-d2)/t;
			for(i=0; i<DIM; i++) {
				pp.p[i] = sp.p[i]+n[i]*t;								// 平面:fp1&線分交点
			}
			if(((sp.p[0]<=pp.p[0]&&pp.p[0]<=ep.p[0])||
                (ep.p[0]<=pp.p[0]&&pp.p[0]<=sp.p[0]))&&
               ((sp.p[1]<=pp.p[1]&&pp.p[1]<=ep.p[1])||
                (ep.p[1]<=pp.p[1]&&pp.p[1]<=sp.p[1]))&&
               ((sp.p[2]<=pp.p[2]&&pp.p[2]<=ep.p[2])||
                (ep.p[2]<=pp.p[2]&&pp.p[2]<=sp.p[2]))) {				// <線分範囲内>
				SetFcData(1, sv, ev, pp);								// 面交点設定
				cnt2++;													// ｶｳﾝﾄ
				for(i=0; i<DIM; i++) {
					if(mi.p[i]>pp.p[i]) {								// 最小･最大値取得
						mi.p[i] = pp.p[i];
					}
					if(ma.p[i]<pp.p[i]) {
						ma.p[i] = pp.p[i];
					}
				}
			}
		}
		if(sf) {														// 面ﾙｰﾌﾟ終了=>処理終了
			break;
		}
		ix++;
	}
	if((cnt1!=0&&cnt2!=0)&&(cnt1==1||cnt2==1)) {						// <<奇数個の交点あり>>
		ErrFlg2 = 1;													// ｴﾗｰﾌﾗｸﾞON
	} else if(cnt1>1&&cnt2>1) {											// <<有効ﾃﾞｰﾀ有>>
		ln = ma.p[0]-mi.p[0];											// X,Y,Zの大きい方でｿｰﾄ
		ix = 0;
		if(ln<ma.p[1]-mi.p[1]) {
			ix = 1;
			ln = ma.p[1]-mi.p[1];
		}
		if(ln<ma.p[2]-mi.p[2]) {
			ix = 2;
		}
		Fclist = Fc_Mergsrt(Fclist, cnt1+cnt2, ix);						// 面交点算出ﾃﾞｰﾀｿｰﾄ
		ret = SetClData();												// 面交差稜線ﾃﾞｰﾀ設定
	}
	wfc = Fclist;
	while(wfc) {														// 面交点算出領域解放
		dfc = wfc, wfc = wfc->next;
		free(dfc);
	}
	return ret;
}

/************************************************************/
static int CheckInFace(PNTTYPE cp, BFATYPE* fp, OBJTYPE* op)			//<<< 面内存在ﾁｪｯｸ
/************************************************************/
{
	PNTTYPE p1, p2, sp, ep;
	int     ix, f1, f2, chx, chy, ucx, dcx, ucy, dcy, ret=0;
	double  d1, d2, sx, sy, ex, ey;

	while(TRUE) {
		chx = 0, ucx = 0, dcx = 0;
		ix = fp->stfl;													// 面ﾙｰﾌﾟ開始番号
		while(TRUE) {
			GetFlpVtx1(op, ix, &p1, &f1);								// 面ﾙｰﾌﾟ頂点取得
			if(!f1) {
				GetFlpVtx1(op, ix+1, &p2, &f2);							// 次ﾙｰﾌﾟ頂点取得
			} else {
				GetFlpVtx1(op, fp->stfl, &p2, &f2);
			}
			if(p1.p[0]<p2.p[0]) {										// 小さい方を始点
				sp = p1, ep = p2;
			} else if(p1.p[0]>p2.p[0]) {
				sp = p2, ep = p1;
			} else if(p1.p[1]<p2.p[1]) {
				sp = p1, ep = p2;
			} else {
				sp = p2, ep = p1;
			}
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			if(sx<=cp.p[0]&&cp.p[0]<ex) {								// <指定点Xが線分Xに含>
				d1 = ey-sy, d2 = ex-sx;
				d1 = d1/d2;												// 傾き
				d2 = sy-d1*sx;											// 定数
				d2 = d1*cp.p[0]+d2;										// Y値計算
				if(d2>cp.p[1]) {										// 線分上点が指定点より大
					ucx++;
				} else if(cp.p[1]>d2) {									// 線分上点が指定点より小
					dcx++;
				} else {												// 接する場合
					chx = 1; break;
				}
			}
			if(f1) {													// 面ﾙｰﾌﾟ終了=>処理終了
				break;
			}
			ix++;
		}
		chy = 0, ucy = 0, dcy = 0;
		ix = fp->stfl;													// 面ﾙｰﾌﾟ開始番号
		while(TRUE) {
			GetFlpVtx1(op, ix, &p1, &f1);								// 面ﾙｰﾌﾟ頂点取得
			if(!f1) {													// 次ﾙｰﾌﾟ頂点取得
				GetFlpVtx1(op, ix+1, &p2, &f2);
			} else {
				GetFlpVtx1(op, fp->stfl, &p2, &f2);
			}
			if(p1.p[1]<p2.p[1]) {										// 小さい方を始点
				sp = p1, ep = p2;
			} else if(p1.p[1]>p2.p[1]) {
				sp = p2, ep = p1;
			} else if(p1.p[0]<p2.p[0]) {
				sp = p1, ep = p2;
			} else {
				sp = p2, ep = p1;
			}
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			if(sy<=cp.p[1]&&cp.p[1]<ey) {								// <指定点Yが線分Yに含>
				d1 = ex-sx, d2 = ey-sy;
				d1 = d1/d2;												// 傾き
				d2 = sx-d1*sy;											// 定数
				d2 = d1*cp.p[1]+d2;										// X値計算
				if(d2>cp.p[0]) {										// 線分上点が指定点より大
					ucy++;
				} else if(cp.p[0]>d2) {									// 線分上点が指定点より小
					dcy++;
				} else {												// 接する場合
					chy = 1; break;
				}
			}
			if(f1) {													// 面ﾙｰﾌﾟ終了=>処理終了
				break;
			}
			ix++;
		}
		if((ucx+dcx)%2==0&&chx==0&&(ucy+dcy)%2==0&&chy==0) {			// X/Y方向でﾁｪｯｸOK
			break;
		}
		cp.p[0] += 0.0011;												// ﾘﾄﾗｲは面内点位置変更
		cp.p[1] += 0.0013;
	}
	ucx = ucx%2, dcx = dcx%2;											// 2個1ｾｯﾄ
	ucy = ucy%2, dcy = dcy%2;
	if((ucx&&dcx)&&(ucy&&dcy)) {										// <面の中に存在>
		d2 = (fp->vec.p[0]*fp->inpt.p[0]+fp->vec.p[1]*fp->inpt.p[1]+
              fp->vec.p[2]*fp->inpt.p[2]-fp->vec.p[0]*cp.p[0]-
              fp->vec.p[1]*cp.p[1])/fp->vec.p[2];						// 指定点(XY)位置面上のZ
		if(d2>cp.p[2]) {												// 指定点より面が大
			ret = 1;
		} else if(d2<cp.p[2]) {											// 指定点より面が小
			ret = -1;
		}
	}
	return ret;
}

/*******************************************************************/
static int SearchClData(int f1, BCLTYPE* c1, int* f2, BCLTYPE** c2)		//<<< 面交差稜線ﾃﾞｰﾀ検索
/*******************************************************************/
{
	BCLTYPE* wc;
	int      sn, en, ret=0;

	if(!f1) {															// 検索線分
		sn = c1->esn, en = c1->een;
	} else {
		sn = c1->ssn, en = c1->sen;
	}
	wc = Cllist;
	while(wc!=NULL) {
		if(c1!=wc&&wc->pfg==0) {										// <対象以外の正式ﾃﾞｰﾀ>
			if(wc->sfg&&sn==wc->ssn&&en==wc->sen&&wc->cfs==0) {			// 始点同一線分有
				ret = 1;												// 仮確認済み
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->efg) {											// 反対側がOUTは確認済み
					ret = 2;
				}
				*f2 = 0, *c2 = wc; break;
			}
			if(wc->efg&&sn==wc->esn&&en==wc->een&&wc->cfe==0) {			// 終点同一線分有
				ret = 1;												// 仮確認済み
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->sfg) {											// 反対側がOUTは確認済み
					ret = 2;
				}
				*f2 = 1, *c2 = wc; break;
			}
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
	if(ret!=0) {														// 検索済みは終了
		return ret;
	}
	wc = Cllist;
	while(wc!=NULL) {
		if(c1!=wc&&wc->pfg==1) {										// <対象以外の仮ﾃﾞｰﾀ>
			if(wc->sfg&&sn==wc->ssn&&en==wc->sen&&wc->cfs==0) {			// 始点同一線分有
				ret = 1;												// 仮確認済み
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->efg) {											// 反対側がOUTは確認済み
					ret = 2;
				}
				*f2 = 1, *c2 = wc; break;
			}
			if(wc->efg&&sn==wc->esn&&en==wc->een&&wc->cfe==0) {			// 終点同一線分有
				ret = 1;												// 仮確認済み
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->sfg) {											// 反対側がOUTは確認済み
					ret = 2;
				}
				*f2 = 1, *c2 = wc; break;
			}
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
	return ret;
}

/********************************/
static void CompleteClData(void)										//<<< 面交差稜線検索完了
/********************************/
{
	BCLTYPE* wc;

	wc = Cllist;
	while(wc!=NULL) {
		if(wc->cfs==1&&wc->cfe==1) {									// 仮確認済みは
			wc->pfg = 2, wc->cfs = 2, wc->cfe = 2;						// 正式確認済みへ
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
}

/**********************************/
static void IncompleteClData(void)										//<<< 面交差稜線検索未完了
/**********************************/
{
	BCLTYPE* wc;

	wc = Cllist;
	while(wc!=NULL) {
		if(wc->cfs==1&&wc->cfe==1) {									// 仮確認済みは
			wc->cfs = 0, wc->cfe = 0;									// 取消へ
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
}

/****************************/
static int CheckClData(void)											//<<< 面交差稜線ﾃﾞｰﾀ確認
/****************************/
{
	BCLTYPE* wc, * wc1, * wc2;
	int      fg1, fg2, ret, cnt=0;

	wc = Cllist;
	while(wc!=NULL) {													// <<正式ﾃﾞｰﾀ確認>>
		if(!wc->sfg&&!wc->efg&&wc->pfg==0) {							// 始点･終点がOUT側
			wc->pfg = 2, wc->cfs = 2, wc->cfe = 2;						// 確認済み
		} else if((!wc->sfg||!wc->efg)&&wc->pfg==0) {					// 始点重複頂点ﾃﾞｰﾀ確認
			wc->cfs = 1, wc->cfe = 1;									// 仮確認済み
			fg1 = (!wc->sfg) ? 0 : 1;									// 検索ﾃﾞｰﾀｾｯﾄ
			wc1 = wc;
			while(TRUE) {
				ret = SearchClData(fg1, wc1, &fg2, &wc2);				// 面交差稜線ﾃﾞｰﾀ検索
				if(ret!=1) {											// 次検索有以外は終了
					break;
				}
				fg1 = fg2, wc1 = wc2;									// 次検索ﾃﾞｰﾀｾｯﾄ
			}
			if(ret==2) {
				CompleteClData();										// 正式確認済み
			} else {
				IncompleteClData();										// 仮確認済み取消
				wc->pfg = -1;											// IN側に同一線分無は無効
			}
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
	wc = Cllist;
	while(wc!=NULL) {													// <<仮ﾃﾞｰﾀ確認>>
		if(!wc->sfg&&!wc->efg&&wc->pfg==1) {							// 始点･終点がOUT側
			wc->pfg = 2, wc->cfs = 2, wc->cfe = 2;						// 確認済み
		} else if((!wc->sfg||!wc->efg)&&wc->pfg==1) {					// 始点重複頂点ﾃﾞｰﾀ確認
			wc->cfs = 1, wc->cfe = 1;									// 仮確認済み
			fg1 = (!wc->sfg) ? 0 : 1;									// 検索ﾃﾞｰﾀｾｯﾄ
			wc1 = wc;
			while(TRUE) {
				ret = SearchClData(fg1, wc1, &fg2, &wc2);				// 面交差稜線ﾃﾞｰﾀ検索
				if(ret!=1) {											// 次検索有以外は終了
					break;
				}
				fg1 = fg2, wc1 = wc2;									// 次検索ﾃﾞｰﾀｾｯﾄ
			}
			if(ret==2) {
				CompleteClData();										// 正式確認済み
			} else {
				IncompleteClData();										// 仮確認済み取消
				wc->pfg = -1;											// IN側に同一線分無は無効
			}
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
	wc = Cllist;
	while(wc!=NULL) {
		if(wc->pfg==2) {												// 有効ﾃﾞｰﾀｶｳﾝﾄ
			wc->pfg = 0, wc->cfs = 0, wc->cfe = 0;
			cnt++;
		} else if(wc->pfg!=-1) {
			cnt++;
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
	return cnt;
}

/**********************************************************************/
static void SetCdData(int flg, int ssn, int sen, PNTTYPE spt, int sno)	//<<< 分割線先頭ﾃﾞｰﾀ設定
/**********************************************************************/
{
	BCDTYPE* tc;
	int      i;

	tc = (BCDTYPE *)malloc(sizeof(BCDTYPE));							// 分割線先頭領域確保
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->flg = flg, tc->ssn = ssn, tc->sen = sen, tc->spt = spt;
	tc->sln = 0.0, tc->sfg = 0,   tc->esn = 0,   tc->een = 0;
	for(i=0; i<DIM; i++) {
		tc->ept.p[i] = 0;
	}
	tc->eln = 0.0, tc->efg = 0, tc->sno = sno, tc->pfg = 0;
	tc->cpp = NULL;
	tc->next = Cdlist, Cdlist = tc;
	if(!flg) {
		Cocnt++;
	} else {
		Cicnt++;
	}
}

/***************************************************/
static void SetCdEln(int esn, int een, PNTTYPE ept)						//<<< 分割線先頭･終了番地設定
/***************************************************/
{
	Cdlist->esn = esn;
	Cdlist->een = een;
	Cdlist->ept = ept;
}

/***********************************/
static void SetCpData1(PNTTYPE pnt)										//<<< 分割線頂点座標設定1
/***********************************/
{
	BCPTYPE* tc, * wcp;

	tc = (BCPTYPE *)malloc(sizeof(BCPTYPE));							// 分割線頂点領域確保
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->next = NULL, tc->pnt = pnt;
	wcp = Cdlist->cpp;													// ﾃﾞｰﾀ末尾接続
	if(wcp==NULL) {
		Cdlist->cpp = tc;
	} else {
		while(wcp->next!=NULL) {
			wcp = wcp->next;
		}
		wcp->next = tc;
	}
}

/***********************************/
static void SetCpData2(PNTTYPE pnt)										//<<< 分割線頂点座標設定2
/***********************************/
{
	BCPTYPE* tc;

	tc = (BCPTYPE *)malloc(sizeof(BCPTYPE));							// 分割線頂点領域確保
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->next = Cdlist->cpp;
	tc->pnt = pnt;
	Cdlist->cpp = tc;													// ﾃﾞｰﾀ先頭接続
}

/********************************************************************/
static BOOL SearchNearCdData(int sn, int en, int* flg, BCDTYPE** cd)	//<<< 基準位置最短ﾃﾞｰﾀ検索
/********************************************************************/
{
	BCDTYPE* wc, * sc;
	BOOL     ret=FALSE;
	int      fg;
	double   ln;

	wc = Cdlist, sc = NULL, ln = DBL_MAX;
	while(wc!=NULL) {
		if(!wc->flg&&!wc->sfg) {										// <OUT側で開始が未検索>
			if(sn==wc->ssn&&en==wc->sen&&(sn!=wc->esn||en!=wc->een)) {	// 開始ﾙｰﾌﾟが一致
				if(ln>wc->sln) {										// 最短点を保存
					sc = wc, ln = wc->sln, fg = 0;
				}
			}
		}
		if(!wc->flg&&!wc->efg) {										// <OUT側で終了が未検索>
			if(sn==wc->esn&&en==wc->een&&(sn!=wc->ssn||en!=wc->sen)){	// 終了ﾙｰﾌﾟが一致
				if(ln>wc->eln) {										// 最短点を保存
					sc = wc, ln = wc->eln, fg = 1;
				}
			}
		}
		wc = wc->next;
	}
	if(sc!=NULL) {														// <最短点があった場合>
		ret = TRUE;
		if(!fg) {														// 検索済み
			sc->sfg = 1;
		} else {
			sc->efg = 1;
		}
		*flg = fg;														// 開始/終了ﾌﾗｸﾞとﾎﾟｲﾝﾀｾｯﾄ
		*cd = sc;
	}
	return ret;
}

/**************************************************/
static void ChangeCdData(BFATYPE* fp, OBJTYPE* op)						//<<< 分割線先頭位置変更
/**************************************************/
{
	double*  ln = new double[View->m_MaxOPnt];
	BCDTYPE* wcd, * wc1, * wc2;
	BCPTYPE* wcp;
	PNTTYPE  p1, p2, p3;
	int      i=0, j, k, f, stf, fg1, fg2;
	double   aln, l11, l12, l21, l22;

	stf = fp->stfl;														// 対象面の面ﾙｰﾌﾟ開始位置
	while(TRUE) {
		GetFlpVtx1(op, stf+i, &p1, &f);									// 対象平面頂点座標取得
		if(i==0) {														// 先頭は頂点保存+ﾊﾞｯﾌｧｸﾘｱ
			p3 = p1, ln[0] = 0.0, aln = 0.0;
		} else {
			aln += PntD(&p1, &p2);										// 今回点と前回点の距離を加算
			ln[i] = aln;												// 現在までの距離合計を設定
		}
		p2 = p1;														// 今回頂点を保存
		i++;
		if(f) {															// 面ﾙｰﾌﾟ終了で終了
			break;
		}
	}
	aln += PntD(&p3, &p2);												// 最終点と開始点の距離を加算
	wcd = Cdlist;
	while(wcd!=NULL) {
		if(!wcd->flg) {
			GetFlpVtx1(op, wcd->ssn, &p1, &j);							// 開始ﾙｰﾌﾟ基準位置の頂点
			wcd->sln = PntD(&p1, &wcd->spt);							// 開始基準点と交点の距離
			GetFlpVtx1(op, wcd->esn, &p1, &j);							// 終了ﾙｰﾌﾟ基準位置の頂点
			wcd->eln = PntD(&p1, &wcd->ept);							// 終了基準点と交点の距離
		}
		wcd = wcd->next;
	}
	i = 0;
	while(TRUE) {
		GetFlp1(op, stf+i, &j, &f);										// 対象平面頂点No取得
		j = (!f) ? i+1 : 0;
		if(SearchNearCdData(stf+i, stf+j, &fg1, &wc1)) {				// 基準位置最短ﾃﾞｰﾀ検索
			while(TRUE) {
				if(SearchNearCdData(stf+i, stf+j, &fg2, &wc2)) {		// 基準位置最短ﾃﾞｰﾀ検索
					l11 = (!fg1) ? wc1->sln : wc1->eln;					// wc1の基準点距離
					l12 = (!fg2) ? wc2->sln : wc2->eln;					// wc2の基準点距離
					if(fabs(l11-l12)<EPSILON) {							// <誤差範囲内の場合>
						l11 += ln[i], l12 += ln[i];						// ﾙｰﾌﾟ開始位置からの距離
						l21 = (!fg1) ? wc1->eln : wc1->sln;				// wc1反対側の基準点距離
						k = (!fg1) ? wc1->esn : wc1->ssn;				// wc1反対側の基準位置
						l21 += ln[k-stf];								// ﾙｰﾌﾟ開始位置からの距離
						l22 = (!fg2) ? wc2->eln : wc2->sln;				// wc2反対側の基準点距離
						k = (!fg2) ? wc2->esn : wc2->ssn;				// wc2反対側の基準位置
						l22 += ln[k-stf];								// ﾙｰﾌﾟ開始位置からの距離
						if((l11=l11-l21)<0.0) {							// 検索位置から反対側への距離
							l11 += aln;
						}
						if((l12=l12-l22)<0.0) {
							l12 += aln;
						}
						if(l11>l12) {									// <前後が逆の場合>
							p1 = (!fg1) ? wc1->spt : wc1->ept;			// wc1の頂点
							p2 = (!fg2) ? wc2->spt : wc2->ept;			// wc2の頂点
							l11 = (!fg1) ? wc1->sln : wc1->eln;			// wc1の基準点距離
							l12 = (!fg2) ? wc2->sln : wc2->eln;			// wc2の基準点距離
							if(!fg1) {									// <wc1が開始側の場合>
								wc1->spt = p2, wc1->sln = l12;			// wc2の頂点と置換
								wcp = wc1->cpp, wcp->pnt = p2;			// 分割線の頂点も置換
							} else {									// <wc1が終了側の場合>
								wc1->ept = p2, wc1->eln = l12;			// wc2の頂点と置換
								wcp = wc1->cpp;
								while(wcp!=NULL&&wcp->next!=NULL) {		// wcpの最終ﾃﾞｰﾀ
									wcp = wcp->next;
								}
								wcp->pnt = p2;							// 分割線の頂点も置換
							}
							if(!fg2) {									// <wc2が開始側の場合>
								wc2->spt = p1, wc2->sln = l11;			// wc1の頂点と置換
								wcp = wc2->cpp, wcp->pnt = p1;			// 分割線の頂点も置換
							} else {									// <wc2が終了側の場合>
								wc2->ept = p1, wc2->eln = l11;			// wc1の頂点と置換
								wcp = wc2->cpp;
								while(wcp!=NULL&&wcp->next!=NULL) {		// wcpの最終ﾃﾞｰﾀ
									wcp = wcp->next;
								}
								wcp->pnt = p1;							// 分割線の頂点も置換
							}
							fg2 = fg1, wc2 = wc1;						// 頂点を置換=>wc1が小
						}
					}
					fg1 = fg2, wc1 = wc2;								// ﾃﾞｰﾀｼﾌﾄ
				} else {
					break;
				}
			}
		}
		i++;
		if(f) {															// 面ﾙｰﾌﾟ終了で終了
			break;
		}
	}
	delete[] ln;
}

/******************************/
static int CheckInClData(void)											//<<< 面交差稜線ﾃﾞｰﾀ(IN)確認
/******************************/
{
	BCLTYPE* sc, * wc;
	int      cnt=0, ssn, sen, esn ,een, er1, er2;

	sc = Cllist;
	while(sc!=NULL) {
		if(!sc->pfg) {													// <有効ﾃﾞｰﾀ>
			ssn = sc->ssn, sen = sc->sen, er1 = 1;						// 始点側ﾃﾞｰﾀ
			esn = sc->esn, een = sc->een, er2 = 1;						// 終点側ﾃﾞｰﾀ
			wc = Cllist;
			while(wc!=NULL) {
				if(!wc->pfg&&sc!=wc) {									// <他の有効ﾃﾞｰﾀ>
					if((ssn==wc->ssn&&sen==wc->sen)||
                       (ssn==wc->esn&&sen==wc->een)) {					// 始点側連結ﾃﾞｰﾀあり
						er1 = 0;
					}
					if((esn==wc->ssn&&een==wc->sen)||
                       (esn==wc->esn&&een==wc->een)) {					// 終点側連結ﾃﾞｰﾀあり
						er2 = 0;
					}
					if(!er1&&!er2) {									// 始点･終点連結有で終了
						break;
					}
				}
				wc = wc->next;
			}
			if(er1||er2) {												// 始点･終点連結無は無効ﾃﾞｰﾀ
				sc->pfg = -1;
				cnt++;
			}
		}
		sc = sc->next;
	}
	return cnt;															// 無効ﾃﾞｰﾀ件数
}

/*************************************************/
static void MakeCutLine(BFATYPE* fp, OBJTYPE* op)						//<<< 交差面分割線作成
/*************************************************/
{
	BCLTYPE* wc, * sc;
	BCDTYPE* cd;
	BCPTYPE* cp;
	PNTTYPE  spt, ept;
	CString  str;
	int      cnt, ssn, sen, esn, een, efg, ct1, ct2;

	Cdlist = NULL, Cocnt = 0, Cicnt = 0, ct1 = 0, ct2 = 0;				// 作業領域初期化
	if((cnt=CheckClData())<1) {											// 面交差稜線ﾃﾞｰﾀ確認
		return;
	}
	while(TRUE) {
		wc = Cllist;													// <<<OUT側ﾙｰﾌﾟの作成>>>
		while(wc!=NULL) {												// 分割線先頭ﾃﾞｰﾀ検索(OUT)
			if(!wc->pfg&&(!wc->sfg||!wc->efg)) {						// <<有効ﾃﾞｰﾀ有>>
				if(!wc->sfg) {											// <始点側有効>
					ssn = wc->ssn, sen = wc->sen, spt = wc->spt;
					efg = wc->efg;
					esn = wc->esn, een = wc->een, ept = wc->ept;
				} else {												// <終点側有効>
					ssn = wc->esn, sen = wc->een, spt = wc->ept;
					efg = wc->sfg;
					esn = wc->ssn, een = wc->sen, ept = wc->spt;
				}
				wc->pfg = 1;											// 処理済み
				ct1++, ct2++;
				sc = wc;												// ﾎﾟｲﾝﾀ保存
				SetCdData(0, ssn, sen, spt, ct2);						// 分割線先頭ﾃﾞｰﾀ設定(OUT)
				SetCpData1(spt);										// 分割線頂点座標設定
				SetCpData1(ept);
				if(!efg) {												// <始点終点の両方が有効>
					SetCdEln(esn, een, ept);
				} else {												// <始点終点の一方が有効>
					break;
				}
			}
			wc = wc->next;												// 次ﾎﾟｲﾝﾀへ
		}
		if(wc==NULL) {													// 先頭ﾃﾞｰﾀ無は終了
			break;
		}
		wc = Cllist;
		while(wc!=NULL) {												// <<始点が一致する>>
			if(!wc->pfg&&wc->sfg&&esn==wc->ssn&&een==wc->sen&&wc!=sc) {
				wc->pfg = 1;											// 処理済み
				ct1++;
				SetCpData1(wc->ept);									// 分割線頂点座標設定
				if(wc->efg) {											// <次ﾃﾞｰﾀ有>
					esn = wc->esn, een = wc->een;						// 次検索番号保存
					sc = wc, wc = Cllist;								// ﾎﾟｲﾝﾀ保存
					continue;											// ﾎﾟｲﾝﾀを先頭に戻す
				} else {												// <次ﾃﾞｰﾀ無>
					SetCdEln(wc->esn, wc->een, wc->ept);				// 終了番地設定
					break;
				}
			}
			if(!wc->pfg&&wc->efg&&esn==wc->esn&&een==wc->een&&wc!=sc) {	// <<終点が一致する>>
				wc->pfg = 1;											// 処理済み
				ct1++;
				SetCpData1(wc->spt);									// 分割線頂点座標設定
				if(wc->sfg) {											// <次ﾃﾞｰﾀ有>
					esn = wc->ssn, een = wc->sen;						// 次検索番号保存
					sc = wc, wc = Cllist;								// ﾎﾟｲﾝﾀ保存
					continue;											// ﾎﾟｲﾝﾀを先頭に戻す
				} else {												// <次ﾃﾞｰﾀ無>
					SetCdEln(wc->ssn, wc->sen, wc->spt);				// 終了番地設定
					break;
				}
			}
			wc = wc->next;												// 次ﾎﾟｲﾝﾀへ
		}
	}
	ChangeCdData(fp, op);												// 分割線先頭位置変更
	cd = Cdlist;
	while(cd!=NULL) {													// <<<反対側へ分割線作成>>>
		SetCdData(0, cd->esn, cd->een, cd->ept, cd->sno);				// 分割線先頭ﾃﾞｰﾀ設定(OUT)
		cp = cd->cpp;
		while(cp!=NULL) {
			SetCpData2(cp->pnt);										// 分割線頂点座標設定
			cp = cp->next;												// 次ﾎﾟｲﾝﾀへ
		}
		SetCdEln(cd->ssn, cd->sen, cd->spt);							// 終了番地設定
		cd = cd->next;													// 次ﾎﾟｲﾝﾀへ
	}
	if(cnt-ct1<3) {														// IN側の可能性数3未満は終了
		return;
	}
	cnt -= CheckInClData();												// 面交差稜線ﾃﾞｰﾀ(IN)確認
	while(TRUE) {
		ct2 = CheckInClData();											// 面交差稜線ﾃﾞｰﾀ(IN)確認
		cnt -= ct2;														// 無効ﾃﾞｰﾀ数減算
		if(ct2==0) {													// 無効ﾃﾞｰﾀなしで確認中止
			break;
		}
	}
	if(cnt-ct1<3) {														// IN側の可能性数3未満は終了
		return;
	}
	while(TRUE) {
		if(cnt<=ct1) {													// 全数処理済み中止
			break;
		}
		wc = Cllist;													// <<<IN側ﾙｰﾌﾟ作成>>>
		while(wc!=NULL) {												// 分割線先頭ﾃﾞｰﾀ検索(IN)
			if(!wc->pfg) {												// <<有効ﾃﾞｰﾀ有>>
				esn = wc->esn, een = wc->een;							// 次検索番号保存
				wc->pfg = 1;											// 処理済み
				ct1++;
				SetCdData(1, 0, 0, wc->ept, 0);							// 分割線先頭ﾃﾞｰﾀ設定(IN)
				SetCpData1(wc->ept);									// 分割線頂点座標設定
				break;
			}
			wc = wc->next;												// 次ﾎﾟｲﾝﾀへ
		}
		if(wc==NULL) {													// 先頭ﾃﾞｰﾀ無は終了
			break;
		}
		wc = Cllist;
		while(wc!=NULL) {												// 分割線頂点を検索
			if(!wc->pfg&&esn==wc->ssn&&een==wc->sen) {					// <<始点が一致>>
				esn = wc->esn, een = wc->een;							// 次検索番号保存
				wc->pfg = 1;											// 処理済み
				ct1++;
				SetCpData1(wc->ept);									// 分割線頂点座標設定
				wc = Cllist;
				continue;												// ﾎﾟｲﾝﾀを先頭に戻す
			}
			if(!wc->pfg&&esn==wc->esn&&een==wc->een) {					// <<終点が一致>>
				esn = wc->ssn, een = wc->sen;							// 次検索番号保存
				wc->pfg = 1;											// 処理済み
				ct1++;
				SetCpData1(wc->spt);									// 分割線頂点座標設定
				wc = Cllist;
				continue;												// ﾎﾟｲﾝﾀを先頭に戻す
			}
			wc = wc->next;												// 次ﾎﾟｲﾝﾀへ
		}
	}
}

/******************************************************/
static BOOL CheckOutLoop(PNTTYPE pt, int sn, int en,
                         BCDTYPE** cd1, BCDTYPE** cd2)					//<<< OUTﾙｰﾌﾟﾃﾞｰﾀ確認
/******************************************************/
{
	BCDTYPE* wc, * w1, * w2;
	PNTTYPE  pnt;
	BOOL     ret=FALSE;
	double   ln, ln1, ln2;

	wc = Cdlist, w1 = NULL, ln1 = DBL_MAX;
	while(wc!=NULL) {
		if(wc->pfg==0) {												// <<未処理の場合>>
			if(sn==wc->esn&&en==wc->een) {								// <終点が指定線分と一致>
				pnt = wc->ept;
				ln = PntD(&pt, &pnt);									// 基準点からの距離
				if(ln<ln1) {											// 基準点に最近点保存
					w1 = wc, ln1 = ln;
				}
			}
		}
		wc = wc->next;													// 次ﾎﾟｲﾝﾀへ
	}
	if(w1!=NULL) {
		wc = Cdlist, w2 = NULL, ln2 = DBL_MAX;
		while(wc!=NULL) {
			if(wc->pfg==0) {											// <<未処理の場合>>
				if(sn==wc->ssn&&en==wc->sen) {							// <始点が指定線分と一致>
					pnt = wc->spt;
					ln = PntD(&pt, &pnt);								// 基準点からの距離
					if(ln>ln1&&ln<ln2) {								// 終点側に最近点保存
						w2 = wc, ln2 = ln;
					}
				}
			}
			wc = wc->next;												// 次ﾎﾟｲﾝﾀへ
		}
		if(w2!=NULL) {													// 確認ﾎﾟｲﾝﾀ設定
			*cd1 = w1, *cd2 = w2;
			ret = TRUE;
		}
	}
	return ret;
}

/*********************************************************/
static BOOL CheckCdData(int ssn, int* een, BCPTYPE** cpp)				//<<< 分割線先頭ﾃﾞｰﾀ確認
/*********************************************************/
{
	BCDTYPE* wcd;
	BOOL     ret=FALSE;

	if(Cocnt>0) {														// <<有効ﾃﾞｰﾀ有>>
		wcd = Cdlist;
		while(wcd!=NULL) {
			if((!wcd->flg)&&wcd->pfg==0&&wcd->ssn==ssn) {				// <OUTで開始ﾙｰﾌﾟ一致>
				*een = wcd->een;										// 終了ﾙｰﾌﾟ
				*cpp = wcd->cpp;										// 頂点座標ﾎﾟｲﾝﾀ
				wcd->pfg = 1;											// 処理済み
				Cocnt--;												// ｶｳﾝﾀDOWN
				ret = TRUE; break;
			}
			wcd = wcd->next;											// 次ﾎﾟｲﾝﾀへ
		}
	}
	return ret;
}

/***********************************************************/
static void CheckInLoop(int x, int y, PNTTYPE* pa, int* pn)				//<<< INﾙｰﾌﾟﾃﾞｰﾀ確認
/***********************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxOPnt];
	BCDTYPE* wcd;
	BCPTYPE* wcp;
	PNTTYPE  sp, ep, ip;
	int      an, bn, ap, bp, i, j, uc, dc, cnt=0;
	double   d, ln1, ln2;

	an = *pn, wcd = Cdlist;
	while(wcd!=NULL) {
		if(wcd->flg&&wcd->pfg==0) {										// <<<有効IN側ﾙｰﾌﾟ>>>
			ip = wcd->ept;
			for(i=0, uc=0, dc=0; i<an; i++) {
				j = (i==an-1) ? 0 : i+1;
				if(pa[i].p[x]<pa[j].p[x]) {								// x座標が小がp1,大がp2
					sp = pa[i], ep = pa[j];
				} else {
					sp = pa[j], ep = pa[i];
				}
				if((ep.p[x]-sp.p[x]<EPSILON&&
                   (sp.p[x]-EPSILON<ip.p[x]&&ip.p[x]<ep.p[x]+EPSILON))||
                   (sp.p[x]<=ip.p[x]&&ip.p[x]<=ep.p[x])) {				// <指定点が線分に含>
					d = (ep.p[y]-sp.p[y])*(ip.p[x]-sp.p[x])/
                        (ep.p[x]-sp.p[x]);								// 始点から線分上の距離(X)
					d = d+sp.p[y];										// 線分上X座標(mm)
					if(d>ip.p[y]) {										// 線分上点が指定点より大
						uc++;
					} else if(d<ip.p[y]) {								// 線分上点が指定点より小
						dc++;
					}
					if(ip.p[y]-EPSILON<d&&d<ip.p[y]+EPSILON) {			// <線分と接する>
						uc = 1, dc = 1; break;							// 面内存在
					}
				}
			}
			uc = uc%2, dc = dc%2;										// 2個1ｾｯﾄ
			if(uc&&dc) {												// <面の中に存在>
				wcd->pfg = 2;
				Cicnt--;
				cnt++;
			}
		}
		wcd = wcd->next;												// 次ﾎﾟｲﾝﾀへ
	}
	if(cnt>0) {
		wcd = Cdlist;
		while(wcd!=NULL) {
			if(wcd->flg&&wcd->pfg==2) {									// <<<有効IN側ﾙｰﾌﾟ>>>
				wcd->pfg = 1;											// 処理済み
				wcp = wcd->cpp, bn = 0;
				while(wcp!=NULL) {
					pb[bn++] = wcp->pnt;								// IN側ﾙｰﾌﾟ頂点抽出
					wcp = wcp->next;									// 次ﾎﾟｲﾝﾀへ
				}
				ap = 0, bp = 0, ln1 = DBL_MAX;							// pa,pb間最短距離点検索
				for(i=0; i<an; i++) {
					for(j=0; j<bn; j++) {
						ln2 = PntD(&pa[i], &pb[j]);
						if(ln2<ln1) {
							ap = i, bp = j, ln1 = ln2;
						}
					}
				}
				for(i=0; i<an; i++) {									// paの最短点が先頭に
					pc[i] = pa[ap++];									// なるように並替
					if(ap>an-1) {
						ap = 0;
					}
				}
				for(i=0; i<an; i++) {
					pa[i] = pc[i];
				}
				pa[an++] = pa[0];										// paの最後に始点を追加
				for(i=0; i<bn; i++) {									// pbの最短点が先頭に
					pc[i] = pb[bp++];									// なるように並替
					if(bp>bn-1) {
						bp = 0;
					}
				}
				for(i=0; i<bn; i++) {
					pb[i] = pc[i];
				}
				pb[bn++] = pb[0];										// pbの最後に始点追加
				for(i=0; i<bn; i++) {
					pa[an++] = pb[i];									// paにpb追加
				}
			}
			wcd = wcd->next;											// 次ﾎﾟｲﾝﾀへ
		}
		*pn = an;
	}
	delete[] pb;
	delete[] pc;
}

/*****************************************************************/
static int CheckCutFacePoint(int pn, PNTTYPE* pa, VECTYPE vv,
                             int d, int* f1, int* f2, double* th)		//<<< 分割面再分割確認
/*****************************************************************/
{
	PNTTYPE pt[3];
	VECTYPE v;
	int     i, j, j1, ch1, ch2, mod, cnt=0;
	double  th1, th2, th3, th4, ln1, ln2;

	for(i=0; i<pn; i++) {
		if(i==0) {														// 連続する3点抽出
			pt[0] = pa[pn-1], pt[1] = pa[i], pt[2] = pa[1];
		} else if(i==pn-1) {
			pt[0] = pa[pn-2], pt[1] = pa[i], pt[2] = pa[0];
		} else {
			pt[0] = pa[i-1], pt[1] = pa[i], pt[2] = pa[i+1];
		}
		CalcRotAngleVec(pt, &th1, &v);									// 3点間角度取得
		ch1 = 1;														// ﾌﾗｸﾞ初期化(反対向)
		if(vv.p[d]<0&&v.p[d]<0) {										// 面の法線ﾍﾞｸﾄﾙと
			ch1 = 0;													// 同一方向の場合は0
		}
		if(vv.p[d]>0&&v.p[d]>0) {
			ch1 = 0;
		}
		if(th1<PI/60.0) {												// 角度3度未満は0
			ch1 = 0;
		}
		if(ch1) {														// <<角度が逆方向点>>
			mod = 0;
			th2 = (PI+th1)/2.0-th1;										// 基準の角度
			ln1 = DBL_MAX;
			ch2 = 0;
RECHECK:
			for(j=0, j1=0; j<pn; j++) {
				if(j!=i) {
					pt[2] = pa[j];										// 対象点設定
					CalcRotAngleVec(pt, &th3, &v);						// 対象点との角度取得
					if((vv.p[d]<0&&v.p[d]<0)||(vv.p[d]>0&&v.p[d]>0)) {	// <法線方向一致>
						if(!mod) {										// ﾓｰﾄﾞ:0は基準～60度未満
							th4 = PI/3.0;
						} else {										// ﾓｰﾄﾞ:1は基準～対象稜線内-5度
							th4 = (PI+th1)/2.0-PI/36.0;
						}
						if(fabs(th3-th2)<th4) {							// <基準角度差が指定値未満>
							ln2 = PntD(&pt[1], &pa[j]);					// 基準点との距離算出
							if(ln2<ln1) {								// 最短距離ﾃﾞｰﾀ保存
								j1 = j, ln1 = ln2, ch2 = 1;
							}
						}
					}
				}
			}
			if(ch2) {													// 分割線番号設定
				f1[cnt] = i, f2[cnt] = j1;
				th[cnt++] = th2;
			} else if(!mod) {											// ﾃﾞｰﾀなしは再ﾁｪｯｸ
				mod = 1;
				goto RECHECK;
			}
		}
	}
	return cnt;
}

/***************************************************/
static void SetVlData(int fg1, int fg2, PNTTYPE pt)						//<<< 図形演算頂点設定
/***************************************************/
{
	BVLTYPE* tv;

	tv = (BVLTYPE *)malloc(sizeof(BVLTYPE));							// 図形演算頂点領域確保
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->pt = pt, tv->vt = -1, tv->lfg = 0, tv->pfg = 0;
	if(!fg1) {															// <<A側>>
		if(!fg2) {														// <外側>
			tv->next = A0list, tv->no = A0cnt++, A0list = tv;
		} else {														// <内側>
			tv->next = A1list, tv->no = A1cnt++, A1list = tv;
		}
	} else {															// <<B側>>
		if(!fg2) {														// <外側>
			tv->next = B0list, tv->no = B0cnt++, B0list = tv;
		} else {														// <内側>
			tv->next = B1list, tv->no = B1cnt++, B1list = tv;
		}
	}
}

/******************************************/
static void SetVlDataEnd(int fg1, int fg2)								//<<< 図形演算面ﾙｰﾌﾟ終了設定
/******************************************/
{
	if(!fg1) {															// <<A側>>
		if(!fg2) {														// <外側>
			A0list->lfg = 1;
		} else {														// <内側>
			A1list->lfg = 1;
		}
	} else {															// <<B側>>
		if(!fg2) {														// <外側>
			B0list->lfg = 1;
		} else {														// <内側>
			B1list->lfg = 1;
		}
	}
}

/*********************************************************************/
static double CalcTriangleArea(PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc)	//<<< 三角形の面積算出
/*********************************************************************/
{
	double a, b, c;

	a = PntD(pa, pb);													// 3辺の長さ
	b = PntD(pb, pc);
	c = PntD(pc, pa);
	return CalcArea(a, b, c);											// 三角形の面積
}

/*****************************************************************/
static int CalcTrianglePoint(PNTTYPE* pa, int pn, int ix, int pm)		//<<< 三角形抽出位置算出
/*****************************************************************/
{
	double* th = new double[View->m_MaxOPnt];
	double* sq = new double[View->m_MaxOPnt];
	PNTTYPE wp[3];
	int     i, j, st, ed, ch, ret=0;
	double  t1, t2, s1, s2, s3;

	for(i=0; i<pn; i++) {
		th[i] = 0.0, sq[i] = 0.0;										// 作業領域初期化
		st = (i==0) ? pn-1 : i-1;										// 対象点の前点
		ed = (i==pn-1) ? 0 : i+1;										// 対象点の後点
		wp[0] = pa[st], wp[1] = pa[i], wp[2] = pa[ed];					// 対象の三角形
		t1 = CalcAngle(wp, ix, pm);										// 対象点のなす角度
		if(t1>0.0) {													// <ｵｰﾊﾞｰﾊﾝｸﾞしていない点>
			s1 = CalcTriangleArea(&pa[st], &pa[i], &pa[ed]);			// 三角形の面積算出 
			for(ch=1, j=0; j<pn; j++) {
				if(j!=st&&j!=i&&j!=ed) {								// 対象の三角形以外の点
					wp[2] = pa[j];										// 前点と対象点と
					t2 = CalcAngle(wp, ix, pm);							// その他の点のなす角度
					if(t2>t1) {											// <対象点角度より大きい>
						s2 = CalcTriangleArea(&pa[st], &pa[i], &pa[j]);	// 前点-対象点-その他の点の面積
						s3 = CalcTriangleArea(&pa[i], &pa[ed], &pa[j]);	// 対象点-後点-その他の点の面積
						if(s1>s2+s3) {									// 対象三角形内にその他の点は無効
							ch = 0; break;
						}
					}
				}
			}
			if(ch) {													// 有効な三角形
				th[i] = t1, sq[i] = s1;
			}
		}
	}
	t1 = PI/6.0, t2 = PI*5.0/6.0;										// 有効範囲：30度～150度
	for(i=0; i<2; i++) {
		for(ch=0, s1=0.0, j=0; j<pn; j++) {
			if(th[j]>t1&&th[j]<t2) {									// <有効範囲内>
				if(sq[j]>s1) {											// 最大の面積を検索
					ret = j, s1 = sq[j], ch = 1;
				}
			}
		}
		if(ch) {														// 検索OKは終了
			break;
		}
		t1 = 0.0, t2 = PI;												// 2度目は有効範囲：0度～180度
	}
	delete[] th;
	delete[] sq;
	return ret;
}

/*********************************************************************/
static void SetCutFace(int fg, VECTYPE vv, int ix,
					   PNTTYPE* pa, int pn, BFATYPE* fp, OBJTYPE* op)	//<<< 分割面情報設定
/*********************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE  wp[3], pt, mi, ma, ip;
	VECTYPE  vc;
	BFATYPE* wfp;
	int      i, j, nn, st, ed, pm, tp, uct, dct, flg;

	for(j=0; j<DIM; j++) {
		pt.p[j] = DBL_MAX;												// 比較領域初期化
	}
	for(nn=0, i=0; i<pn; i++) {
		for(st=0, j=0; j<DIM; j++) {
			if(fabs(pa[i].p[j]-pt.p[j])>EPSILON) {						// 何れかが異なればOK
				st = 1; break;
			}
		}
		if(st) {														// 前点と異なれば出力
			pp[nn++] = pa[i];
		}
		pt = pa[i];														// 比較領域更新
	}
	for(st=0, j=0; j<DIM; j++) {										// 始点と終点の確認
		if(fabs(pp[0].p[j]-pp[nn-1].p[j])>EPSILON) {					// 何れかが異なればOK
			st = 1; break;
		}
	}
	if(!st) {															// 同じ場合は頂点数減
		nn--;
	}
	if(nn<3) {
		delete[] pp;
		return;
	}
	CalcNVec(pp, nn, &vc);												// 法線ﾍﾞｸﾄﾙ取得
	if((vv.p[ix]>0&&vc.p[ix]<0)||(vv.p[ix]<0&&vc.p[ix]>0)) {			// <ﾍﾞｸﾄﾙの向きが逆>
		st = 0;
		ed = nn-1;
		while(st<ed) {
			pt = pp[st], pp[st] = pp[ed], pp[ed] = pt, ed--, st++;		// 頂点反転
		}
	}
	pm = (vv.p[ix]>0.0) ? 0 : 1;										// 法線最大方向の正負ﾌﾗｸﾞ
	CalcInPoint(pp, nn, &mi, &ma, &ip);									// 面内存在点取得
	wfp = fp, uct = 0, dct = 0, flg = 0;
	while(wfp!=NULL) {
		if(ma.p[0]<wfp->xmin) {											// X最大より最小が大は終了
			break;
		}
		if(ip.p[0]>=wfp->xmin&&ip.p[0]<=wfp->xmax&&
           ip.p[1]>=wfp->ymin&&ip.p[1]<=wfp->ymax) {					// <面内に存在点有>
			i = CheckInFace(ip, wfp, op);								// 面内存在ﾁｪｯｸ
			if(i>0) {													// 面内の上に存在
				uct++;
			} else if(i<0) {											// 面内の下に存在
				dct++;
			}
		}
		wfp = wfp->next;												// 次ﾎﾟｲﾝﾀへ
	}
	uct = uct%2, dct = dct%2;											// 面は2個1ｾｯﾄ
	if(uct&&dct) {														// 立体内に存在
		flg = 1;
	}
	while(nn>=3) {
		tp = CalcTrianglePoint(pp, nn, ix, pm);							// 三角形抽出位置算出
		st = (tp==0) ? nn-1 : tp-1;										// 抽出位置の前点
		ed = (tp==nn-1) ? 0 : tp+1;										// 抽出位置の後点
		wp[0] = pp[st], wp[1] = pp[tp], wp[2] = pp[ed];					// 抽出対象の三角形
		for(i=0; i<3; i++) {
			SetVlData(fg, flg, wp[i]);									// 図形演算頂点設定
		}
		SetVlDataEnd(fg, flg);											// 図形演算面ﾙｰﾌﾟ終了設定
		for(i=tp; i<nn-1; i++) {
			pp[i] = pp[i+1];											// 抽出点以降を前詰め
		}
		nn--;															// 分割面頂点数減算
	}
	delete[] pp;
}

/****************************************************************/
static void SetInOutCF(int fg, VECTYPE vv, int ix, PNTTYPE* pa,
                       int pn, BFATYPE* fp, OBJTYPE* op, int lv)		//<<< 内外分割面の設定
/****************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxBPnt];
	int*     f1 = new int[View->m_MaxBPnt];
	int*     f2 = new int[View->m_MaxBPnt];
	double*  th = new double[View->m_MaxBPnt];
	int      i, i1, i2, j, cnt, wn, st, ed, ct1, ct2, ct3, er1, er2, stp;
	double   sth;

	while(TRUE) {
		cnt = CheckCutFacePoint(pn, pa, vv, ix, f1, f2, th);			// 分割面再分割確認
		if(cnt==0||lv>100) {											// <面の分割無/ﾚﾍﾞﾙ上限>
			SetCutFace(fg, vv, ix, pa, pn, fp, op);						// 分割面の設定
			break;														// 処理終了
		} else {														// <面の分割有>
			wn = 0, i1 = 0, i2 = 0, stp = 1;
			ct3 = INT_MAX;
			for(i=0; i<cnt; i++) {
				if(f1[i]<f2[i]) {										// 最も端で最も交線の
					st = f1[i], ed = f2[i];								// 少ない分割線検索
				} else {
					st = f2[i], ed = f1[i];
				}
				for(j=0, ct1=0, ct2=0, er1=0, er2=0; j<cnt; j++) {
					if(j!=i) {
						if(f1[j]<st||f1[j]>ed) {
							er1 = 1;
						}
						if(f2[j]<st||f2[j]>ed) {
							ct1++;
						}
						if(f1[j]>st&&f1[j]<ed) {
							er2 = 1;
						}
						if(f2[j]>st&&f2[j]<ed) {
							ct2++;
						}
						if(er1&&er2) {
							break;
						}
					}
				}
				if((!er1)&&ct1<ct3) {									// 検索分割線設定
					i1 = i, i2 = 0, ct3 = ct1, stp = 0;
				}
				if((!er2)&&ct2<ct3) {
					i1 = i, i2 = 1, ct3 = ct2, stp = 0;
				}
			}
			if(stp) {													// <検索分割線がない>
				for(sth=PI, i=0, i1=0; i<cnt; i++) {
					if(sth>th[i]) {										// 基準角度の最小検索
						i1 = i, sth = th[i];
					}
				}
				if(f1[i1]<f2[i1]) {										// 分割線
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				for(wn=0, i=0; i<=st; i++) {
					pp[wn++] = pa[i];
				}
				for(i=ed; i<pn; i++) {
					pp[wn++] = pa[i];
				}
				if(wn>2) {
					SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);		// 内外分割面の設定
				}
				for(wn=0, i=st; i<=ed; i++) {
					pp[wn++] = pa[i];
				}
				if(wn>2) {
					SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);		// 内外分割面の設定
				}
				break;													// 処理終了
			} else {													// <検索分割線有>
				if(f1[i1]<f2[i1]) {										// 分割線
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				if(!i2) {												// 今回分割面設定
					for(i=0; i<=st; i++) {
						pp[wn++] = pa[i];
					}
					for(i=ed; i<pn; i++) {
						pp[wn++] = pa[i];
					}
				} else {
					for(i=st; i<=ed; i++) {
						pp[wn++] = pa[i];
					}
				}
				if(wn>2) {												// <有効分割面の場合>
					SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);		// 内外分割面の設定
					if(!i2) {											// 次回分割面設定
						for(wn=0, i=st; i<=ed; i++) {
							pp[wn++] = pa[i];
						}
					} else {
						for(wn=0, i=0; i<=st; i++) {
							pp[wn++] = pa[i];
						}
						for(i=ed; i<pn; i++) {
							pp[wn++] = pa[i];
						}
					}
					for(i=0; i<wn; i++) {
						pa[i] = pp[i];
					}
					pn = wn;
				} else {												// <無効分割面の場合>
					for(sth=PI, i=0, i1=0; i<cnt; i++) {
						if(sth>th[i]) {									// 基準角度の最小検索
							i1 = i, sth = th[i];
						}
					}
					if(f1[i1]<f2[i1]) {									// 分割線
						st = f1[i1], ed = f2[i1];
					} else {
						st = f2[i1], ed = f1[i1];
					}
					for(wn=0, i=0; i<=st; i++) {
						pp[wn++] = pa[i];
					}
					for(i=ed; i<pn; i++) {
						pp[wn++] = pa[i];
					}
					if(wn>2) {
						SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);	// 内外分割面の設定
					}
					for(wn=0, i=st; i<=ed; i++) {
						pp[wn++] = pa[i];
					}
					if(wn>2) {
						SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);	// 内外分割面の設定
					}
					break;												// 処理終了
				}
			}
		}
	}
	delete[] pp;
	delete[] f1;
	delete[] f2;
	delete[] th;
}

/*******************************************************************/
static void SetInOut(int fg, VECTYPE vv, int ix,
                     PNTTYPE* pa, int pn, BFATYPE* fp, OBJTYPE* op)		//<<< 内外分割面の設定
/*******************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxBPnt];
	PNTTYPE  pt;
	VECTYPE  vc;
	int      i, st, ed, wn;

	CalcNVec(pa, pn, &vc);												// 法線ﾍﾞｸﾄﾙ取得
	if((vv.p[ix]>0&&vc.p[ix]<0)||(vv.p[ix]<0&&vc.p[ix]>0)) {			// <ﾍﾞｸﾄﾙの向きが逆>
		st = 0, ed = pn-1;
		while(st<ed) {
			pt = pa[st], pa[st] = pa[ed], pa[ed] = pt, ed--, st++;		// 頂点反転
		}
	}
	st = 0;
	while(TRUE) {
		ed = (st+View->m_MaxBPnt<pn) ? st+View->m_MaxBPnt : pn;			// 今回設定範囲(終了)
		for(wn=0, i=st; i<ed; i++) {
			pp[wn++] = pa[i];											// 今回設定ﾃﾞｰﾀ複写
		}
		SetInOutCF(fg, vv, ix, pp, wn, fp, op, 0);						// 内外分割面の設定
		if(ed==pn) {													// 終了
			break;
		}
		st = ed;														// 終了範囲保存(次回開始)
	}
	delete[] pp;
}

/*****************************************************************/
static void MakeCutFace(int fg, BFATYPE* fp1,
                        BFATYPE* fp2, OBJTYPE* op1, OBJTYPE* op2)		//<<< 交差面分割
/*****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	int*     va = new int[View->m_MaxOPnt];
	short*   sf = new short[View->m_MaxOPnt];
	BCDTYPE* wc1, * wc2;
	BCPTYPE* wcp;
	PNTTYPE  pt, mi, ma, ip;
	VECTYPE  v1, v2;
	int      ix, i, j, i1, i2, pn, ed, st, vn, flg, eln, stp, mx=0;
	double   d;

	v1 = fp1->vec;														// 指定面法線ﾍﾞｸﾄﾙ
	d = fabs(v1.p[0]);													// ﾍﾞｸﾄﾙ最大方向取得
	ix = 0, i1 = 1, i2 = 2;
	if(d<fabs(v1.p[1])) {
		d = fabs(v1.p[1]);
		ix = 1, i1 = 2, i2 = 0;
	}
	if(d<fabs(v1.p[2])) {
		ix = 2, i1 = 0, i2 = 1;
	}
	i = fp1->stfl;														// 対象平面ﾙｰﾌﾟ開始番号
	while(TRUE) {
		GetFlp1(op1, i, &vn, &flg);										// 対象平面頂点番号取得
		va[mx] = vn;													// 頂点番号保存
		sf[mx++] = 0;													// 処理ﾌﾗｸﾞ初期化
		if(flg) {														// ﾙｰﾌﾟ終了
			break;
		}
		i++;
	}
	if(Cicnt>0) {														// <<<IN側ﾙｰﾌﾟ有>>>
		wc1 = Cdlist;
		while(wc1!=NULL) {
			if(wc1->flg) {												// <<<IN側ﾙｰﾌﾟの場合>>>
				wcp = wc1->cpp, pn = 0;
				while(wcp!=NULL) {
					pa[pn++] = wcp->pnt;								// IN側ﾙｰﾌﾟ頂点抽出
					wcp = wcp->next;									// 次ﾎﾟｲﾝﾀへ
				}
				if(pn>2) {												// <<有効頂点数有>>
					CalcNVec(pa, pn, &v2);								// 法線ﾍﾞｸﾄﾙ取得
					if((v1.p[ix]>0&&v2.p[ix]>0)||
                       (v1.p[ix]<0&&v2.p[ix]<0)) {						// <ﾍﾞｸﾄﾙが同じ>
						SetInOut(fg, v1, ix, pa, pn, fp2, op2);			// 内外分割面の設定
					} else {											// <ﾍﾞｸﾄﾙが異なる>
						CalcInPoint(pa, pn, &mi, &ma, &ip);				// 面内存在点取得
						wc1->ept = ip;									// 取得点保存
					}
					st = 0, ed = pn-1;
					while(st<ed) {
						pt = pa[st], pa[st] = pa[ed], pa[ed] = pt;		// 頂点反転
						ed--, st++;
					}
					CalcNVec(pa, pn, &v2);								// 法線ﾍﾞｸﾄﾙ再取得
					if((v1.p[ix]>0&&v2.p[ix]>0)||
                       (v1.p[ix]<0&&v2.p[ix]<0)) {						// <ﾍﾞｸﾄﾙが同じ>
						SetInOut(fg, v1, ix, pa, pn, fp2, op2);			// 内外分割面の設定
					} else {											// <ﾍﾞｸﾄﾙが異なる>
						wcp = wc1->cpp, pn = 0;
						while(wcp!=NULL) {								// 元は同じ方向なので
							wcp->pnt = pa[pn++];						// IN側ﾙｰﾌﾟ頂点再設定
							wcp = wcp->next;							// 次ﾎﾟｲﾝﾀへ
						}
						CalcInPoint(pa, pn, &mi, &ma, &ip);				// 面内存在点取得
						wc1->ept = ip;									// 取得点保存
					}
				} else {												// <<有効頂点数無>>
					wc1->pfg = 1, Cicnt--;
				}
			}
			wc1 = wc1->next;											// 次ﾎﾟｲﾝﾀへ
		}
	}
	if(Cocnt>0) {														// <閉のﾙｰﾌﾟ,接続のﾙｰﾌﾟ検索>
		i = fp1->stfl;													// 対象平面ﾙｰﾌﾟ開始番号
		while(TRUE) {
			GetFlpVtx1(op1, i, &pt, &flg);								// 対象平面頂点座標取得
			j = (!flg) ? i+1 : fp1->stfl;
			while(TRUE) {
				if(CheckOutLoop(pt, i, j, &wc1, &wc2)) {				// OUTﾙｰﾌﾟﾃﾞｰﾀ確認
					if(wc1==wc2) {										// <閉ﾙｰﾌﾟの場合>
						wc1->pfg = 1, Cocnt--;							// 処理済み
						wcp = wc1->cpp, pn = 0;
						while(wcp!=NULL) {
							pa[pn++] = wcp->pnt;						// 閉ﾙｰﾌﾟ頂点抽出
							wcp = wcp->next;							// 次ﾎﾟｲﾝﾀへ
						}
						if(pn>2) {
							if(Cicnt>0) {
								CheckInLoop(i1, i2, pa, &pn);			// INﾙｰﾌﾟ確認
							}
							SetInOut(fg, v1, ix, pa, pn, fp2, op2);		// 内外分割面設定
						}
					} else {											// <接続ﾙｰﾌﾟの場合>
						wc1->esn = wc2->esn;							// wc1とwc2を接続するため
						wc1->een = wc2->een;							// wc2の終点情報wc1複写
						wc1->ept = wc2->ept;
						if(wc1->cpp==NULL) {							// wc2の分割頂点を
							wc1->cpp = wc2->cpp;						// wc1の末尾に移動
						} else {
							wcp = wc1->cpp;
							while(wcp->next!=NULL) {
								wcp=wcp->next;
							}
							wcp->next = wc2->cpp;
						}
						wc2->pfg = 1, Cocnt--;							// wc2は処理済み
						wc2->cpp = NULL;								// wc2の分割頂点は無
					}
				} else {
					break;
				}
			}
			if(flg) {													// ﾙｰﾌﾟ終了で終了する
				break;
			}
			i++;														// 次頂点へ
		}
	}
	while(TRUE) {
		eln = 0, stp = 0, pn = 0;										// 作業領域初期化
		while(TRUE) {
			wcp = NULL;
			for(i=eln; i<mx; i++) {
				if(!sf[i]) {											// <未処理の場合>
					sf[i] = 1;											// 処理済み
					GetVtx(op1, va[i], &pa[pn++]);						// 頂点座標取得
					j = fp1->stfl + i;									// 面ﾙｰﾌﾟ番号
					if(CheckCdData(j, &eln, &wcp)) {					// 分割線先頭ﾃﾞｰﾀ確認
						eln = eln-fp1->stfl; break;						// 次回の開始面ﾙｰﾌﾟ番号
					}
				} else {												// <処理済の場合>
					if(pn>0) {											// ﾃﾞｰﾀ設定済みは終了
						stp = 1; break;
					}
				}
			}
			if(i>=mx||stp) {
				break;
			}
			while(wcp!=NULL) {											// <分割線有の場合>
				pa[pn++] = wcp->pnt;									// 分割線頂点取得
				wcp = wcp->next;										// 次ﾎﾟｲﾝﾀへ
			}
		}
		if(pn>0) {														// <有効頂点有>
			if(Cicnt>0) {
				CheckInLoop(i1, i2, pa, &pn);							// INﾙｰﾌﾟ確認
			}
			SetInOut(fg, v1, ix, pa, pn, fp2, op2);						// 内外分割面の設定
		} else {														// <有効頂点無>
			break;
		}
	}
	delete[] pa;
	delete[] va;
	delete[] sf;
}

/****************************************************************/
static void SetInOutFace(int fg1, int fg2, OBJTYPE* op, int sno)		//<<< 内外面情報の設定
/****************************************************************/
{
	PNTTYPE pt;
	int     ix, ffg;

	ix = sno;															// 面ﾙｰﾌﾟ開始番号
	while(TRUE) {
		GetFlpVtx1(op, ix, &pt, &ffg);									// 面ﾙｰﾌﾟ頂点取得
		SetVlData(fg1, fg2, pt);										// 図形演算頂点設定
		if(ffg) {														// 面ﾙｰﾌﾟ終了は処理終了
			break;
		}
		ix++;
	}
	SetVlDataEnd(fg1, fg2);												// 図形演算面ﾙｰﾌﾟ終了設定
}

/***************************************************************/
static void MakeBooleanList(int fg, OBJTYPE* op1, OBJTYPE* op2)			//<<< 図形演算ﾘｽﾄ作成
/***************************************************************/
{
	BFATYPE* fl1, * fl2, * wfp;
	BCLTYPE* wcl, * dcl;
	BCDTYPE* wcd, * dcd;
	BCPTYPE* wcp, * dcp;
	PNTTYPE  pt;
	int      num, cnt=0, ipr=0, cflg, flg, uct, dct, ret;
	double   dpr;

	if(!fg) {															// <A側>
		fl1 = Aflist, fl2 = Bflist, num = Afcnt;
	} else {															// <B側>
		fl1 = Bflist, fl2 = Aflist, num = Bfcnt;
	}
	while(fl1!=NULL) {
		cflg = 0, flg = 0, uct = 0, dct = 0;							// 作業領域初期化
		Cllist = NULL, pt = fl1->inpt, wfp = fl2;
		while(wfp!=NULL) {
			if(fl1->xmax<wfp->xmin) {									// X最大より最小が大は終了
				break;
			}
			if(fl1->xmin<=wfp->xmax&&
               fl1->ymin<=wfp->ymax&&fl1->ymax>=wfp->ymin) {			// <<面のX-Y領域が交差>>
				if(fl1->zmin<=wfp->zmax&&fl1->zmax>=wfp->zmin) {		// <面のZ領域が交差>
					if(CheckCrossFace(fl1, wfp, op1, op2)) {			// 面交差ﾁｪｯｸ
						cflg = 1;										// 交差はﾌﾗｸﾞON
					}
				}
				if(!cflg&&
                   pt.p[0]>=wfp->xmin&&pt.p[0]<=wfp->xmax&&
                   pt.p[1]>=wfp->ymin&&pt.p[1]<=wfp->ymax) {			// <交差無で面内に点有>
					ret = CheckInFace(fl1->inpt, wfp, op2);				// 面内存在ﾁｪｯｸ
					if(ret>0) {											// 面内の上に存在
						uct++;
					} else if(ret<0) {									// 面内の下に存在
						dct++;
					}
				}
			}
			wfp = wfp->next;											// 次ﾎﾟｲﾝﾀへ
		}
		if(cflg) {														// <<交差面の場合>>
			MakeCutLine(fl1, op1);										// 交差面分割線作成
			wcl = Cllist;
			while(wcl) {												// 面交差稜線領域解放
				dcl = wcl, wcl = wcl->next;
				free(dcl);
			}
			MakeCutFace(fg, fl1, fl2, op1, op2);						// 交差面分割
			wcd = Cdlist;
			while(wcd) {
				wcp = wcd->cpp;
				while(wcp) {											// 分割線頂点領域解放
					dcp = wcp, wcp = wcp->next;
					free(dcp);
				}
				dcd = wcd, wcd = wcd->next;
				free(dcd);												// 分割線分先頭領域解放
			}
		} else {														// <<未交差面の場合>>
			uct = uct%2, dct = dct%2;									// 面は2個1ｾｯﾄ
			if(uct&&dct) {												// 立体内に存在
				flg = 1;
			}
			SetInOutFace(fg, flg, op1, fl1->stfl);						// 内外面情報設定
		}
		cnt++;															// 処理件数UP
		dpr = (Ppr/6.0)*((double)cnt/(double)num);						// 現在のﾊﾟｰｾﾝﾄ
		if(ipr<(int)(Spr+dpr)) {										// <<ﾊﾟｰｾﾝﾄUP>>
			ipr = (int)(Spr+dpr);
			m_pProgressB->SetProgress(ipr);								// 進捗表示
		}
		fl1 = fl1->next;												// 次ﾎﾟｲﾝﾀへ
	}
	Spr += Ppr/6.0;														// 累積ﾊﾟｰｾﾝﾄ
}

/***************************/
static void ReverseB1(void)												//<<< B1面ﾙｰﾌﾟ反転
/***************************/
{
	BVLTYPE* wvl1, * wvl2;
	int      cnt=0;

	wvl1 = B1list, wvl2 = NULL;
	while(wvl1!=NULL) {
		wvl1->no = cnt++;												// 面ﾙｰﾌﾟ番号逆設定
		if(wvl1->lfg) {													// <旧面ﾙｰﾌﾟ最終>
			wvl1->lfg = 0;												// 最終解除
			if(wvl2!=NULL) {											// 直前を最終へ
				wvl2->lfg = 1;
			}
		}
		wvl2 = wvl1;													// 直前として保存
		wvl1 = wvl1->next;												// 次ﾎﾟｲﾝﾀへ
	}
	if(wvl2!=NULL) {													// 最後を最終へ
		wvl2->lfg = 1;
	}
}

/*******************************************************************/
static void ConnectVlData(BVLTYPE** list1, BVLTYPE* list2, int cnt)		//<<< 面ﾙｰﾌﾟ接続
/*******************************************************************/
{
	BVLTYPE* wvl;

	wvl = *list1;
	if(wvl!=NULL) {														// <ﾘｽﾄ1が有効>
		while(wvl->next!=NULL) {										// 次ﾎﾟｲﾝﾀがNULL検索
			wvl = wvl->next;
		}
		wvl->next = list2;												// ﾘｽﾄ2接続
	} else {															// <ﾘｽﾄ1が無効>
		*list1 = list2;													// ﾘｽﾄ2ﾘｽﾄ1へ
	}
	wvl = list2;
	while(wvl!=NULL) {													// ﾘｽﾄ2の
		wvl->no = wvl->no+cnt;											// 面ﾙｰﾌﾟ番号加算
		wvl = wvl->next;												// 次ﾎﾟｲﾝﾀへ
	}
}

/********************************************************/
static BVLTYPE* MergeV(BVLTYPE* la, BVLTYPE* lb, int id)				//<<< 頂点座標ｿｰﾄ
/********************************************************/
{
	BVLTYPE* lc, * ld, * rc;

	lc = (BVLTYPE *)malloc(sizeof(BVLTYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[id]<=lb->pt.p[id]) {								// 頂点座標の大小で比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/********************************************************/
static BVLTYPE* V_Mergsrt(BVLTYPE* mlist, int n, int id)				//<<< 頂点座標ｿｰﾄ
/********************************************************/
{
	BVLTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BVLTYPE *)MergeV(V_Mergsrt(la, n/2, id),
                               V_Mergsrt(lb, n-n/2, id), id);			// 頂点座標ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/*************************************************/
static BVLTYPE* MergeVn(BVLTYPE* la, BVLTYPE* lb)						//<<< 面ﾙｰﾌﾟ番号ｿｰﾄ
/*************************************************/
{
	BVLTYPE* lc, * ld, * rc;

	lc = (BVLTYPE *)malloc(sizeof(BVLTYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// 面ﾙｰﾌﾟ番号の大小で比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/*************************************************/
static BVLTYPE* V_Mergsrtn(BVLTYPE* mlist, int n)						//<<< 面ﾙｰﾌﾟ番号ｿｰﾄ
/*************************************************/
{
	BVLTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BVLTYPE *)MergeVn(V_Mergsrtn(la, n/2),
                                V_Mergsrtn(lb, n-n/2));					// 面ﾙｰﾌﾟ番号ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/**************************************/
static BOOL CheckLoopVtx(BVLTYPE* bvl)									//<<< 面ﾙｰﾌﾟ頂点重複確認
/**************************************/
{
	BVLTYPE* wvp, * bv1, * bv2, * sv1, * sv2;
	int      s1, s2, n1, n2;

	wvp = bvl, s1 = -1, n1 = -1, sv1 =NULL;
	while(wvp!=NULL) {
		if(!wvp->pfg) {													// <有効ﾃﾞｰﾀ>
			if(s1==-1) {												// 先頭有効頂点No保存
				s1 = wvp->vt, sv1 = wvp;
			}
			if(wvp->vt==n1) {											// 直前点と同一頂点は無効
				wvp->pfg = 1;
				return TRUE;
			}
			n1 = wvp->vt;												// 今回頂点を直前点として保存
		}
		if(wvp->lfg) {													// 該当面の終了
			break;
		}
		wvp = wvp->next;
	}
	if(n1!=-1&&n1==s1) {												// 先頭と最終の確認
		sv1->pfg = 1;
		return TRUE;
	}
	wvp = bvl, s1 = -1, s2 = -1, n1 = -1, n2 = -1;
	sv1 = NULL, sv2 = NULL, bv1 = NULL, bv2 = NULL;
	while(wvp!=NULL) {
		if(!wvp->pfg) {													// <有効ﾃﾞｰﾀ>
			if(s2==-1) {												// 先頭2有効頂点No保存
				s2 = s1, s1 = wvp->vt, sv2 = sv1, sv1 = wvp;
			}
			if(wvp->vt==n2) {											// 2点前と同一頂点は無効
				wvp->pfg = 1, bv1->pfg = 1;
				return TRUE;
			}
			n2 = n1, n1 = wvp->vt, bv2 = bv1, bv1 = wvp;				// 今回頂点を直前点として保存
		}
		if(wvp->lfg) {													// 該当面の終了
			break;
		}
		wvp = wvp->next;
	}
	if(n2!=-1&&n2==s2) {												// 最終-1と先頭の確認
		bv1->pfg = 1, sv2->pfg = 1;
		return TRUE;
	}
	if(n1!=-1&&n1==s1) {												// 最終と先頭+1の確認
		sv1->pfg = 1, sv2->pfg = 1;
		return TRUE;
	}
	return FALSE;
}

/**************************************************************/
static void RemakeObject(int mode, int atr, int col, int* gno)			//<<< 図形演算立体作成
/**************************************************************/
{
	BVLTYPE* list, * wvp, * svp, * nvp, * evp;
	BVTTYPE* vtl=NULL, * bvt=NULL, * tvt;
	OBJTYPE* op;
	PNTTYPE  pt;
	int      i, cnt, vct=0, lct=0;

	if(mode==0) {														// <<A-Bの場合>>
		ReverseB1();													// B1面ﾙｰﾌﾟ反転
		ConnectVlData(&A0list, B1list, A0cnt);							// 面ﾙｰﾌﾟ接続(A0+B1)
		B1list = NULL;													// B1ﾎﾟｲﾝﾀｸﾘｱ
		list = A0list, cnt  = A0cnt + B1cnt;
	} else if(mode==1) {												// <<A+Bの場合>>
		ConnectVlData(&A0list, B0list, A0cnt);							// 面ﾙｰﾌﾟ接続(A0+B0)
		B0list = NULL;													// B0ﾎﾟｲﾝﾀｸﾘｱ
		list = A0list, cnt  = A0cnt + B0cnt;
	} else if(mode==2) {												// <<A×Bの場合>>
		ConnectVlData(&A1list, B1list, A1cnt);							// 面ﾙｰﾌﾟ接続(A1+B1)
		B1list = NULL;													// B1ﾎﾟｲﾝﾀｸﾘｱ
		list = A1list, cnt  = A1cnt + B1cnt;
	}
	if(cnt==0) {														// 有効ﾃﾞｰﾀなしは中止
		return;
	}
	wvp = list;
	while(wvp!=NULL) {
		for(i=0; i<DIM; i++) {
			wvp->pt.p[i] -= ReszB;										// 頂点ﾃﾞｰﾀ戻し
		}
		wvp = wvp->next;
	}
	list = V_Mergsrt(list, cnt, 2);										// 頂点座標:Zｿｰﾄ
	list = V_Mergsrt(list, cnt, 1);										// 頂点座標:Yｿｰﾄ
	list = V_Mergsrt(list, cnt, 0);										// 頂点座標:Xｿｰﾄ
	Spr += Ppr/6.0;														// 累積ﾊﾟｰｾﾝﾄ
	m_pProgressB->SetProgress((int)Spr);								// 進捗表示
	pt = list->pt;														// 初期値設定
	pt.p[0] -= 1.0;
	wvp = list;
	while(wvp!=NULL) {													// 正規頂点数ｶｳﾝﾄ
		if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
           fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
           fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {						// <設定済と異なる場合>
			wvp->vt = vct++;											// 頂点番号保存
			pt = wvp->pt;
			tvt = (BVTTYPE *)malloc(sizeof(BVTTYPE));					// 頂点ﾃﾞｰﾀ領域確保
			if(tvt==NULL) {
				MemErr(IDS_MEMERR5);
			}
			tvt->pt = wvp->pt, tvt->vt = wvp->vt, tvt->next = NULL;
			if(bvt==NULL) {												// 先頭ﾃﾞｰﾀはﾎﾟｲﾝﾀ保存
				vtl = tvt;
			} else {													// 以外は前ﾃﾞｰﾀに連結
				bvt->next = tvt;
			}
			bvt = tvt;													// 今回ﾎﾟｲﾝﾀを保存
		} else {														// 同は頂点番号保存
			wvp->vt = vct-1;
		}
		wvp = wvp->next;
	}
	list = V_Mergsrtn(list, cnt);										// 面ﾙｰﾌﾟ番号ｿｰﾄ
	Spr += Ppr/6.0;														// 累積ﾊﾟｰｾﾝﾄ
	m_pProgressB->SetProgress((int)Spr);								// 進捗表示
	wvp = list, svp = NULL;
	while(wvp!=NULL) {
		if(svp==NULL) {													// 面ﾙｰﾌﾟの開始位置保存
			svp = wvp;
		}
		if(wvp->lfg) {													// 面ﾙｰﾌﾟの終了位置
			while(TRUE) {
				if(!CheckLoopVtx(svp)) {								// 面ﾙｰﾌﾟ頂点重複確認
					break;
				}
			}
			nvp = svp, cnt = 0, evp = NULL;
			while(nvp!=NULL) {
				if(!nvp->pfg) {											// 有効ﾃﾞｰﾀｶｳﾝﾄ
					cnt++;
					evp = nvp;
				}
				if(nvp->lfg) {											// 該当面の終了
					break;
				}
				nvp = nvp->next;
			}
			if(cnt>0&&cnt<3) {											// 有効ﾃﾞｰﾀが3未満は
				nvp = svp;
				while(nvp!=NULL) {
					nvp->pfg = 1;										// 該当面を全て無効
					if(nvp->lfg) {										// 該当面の終了
						break;
					}
					nvp = nvp->next;
				}
			} else if(cnt>=3&&evp!=NULL) {								// 有効最終ﾃﾞｰﾀ
				evp->lfg = 1;
			}
			svp = NULL;
		}
		wvp = wvp->next;
	}
	wvp = list, cnt = 0;
	while(wvp!=NULL) {
		if(!wvp->pfg) {													// 有効ﾃﾞｰﾀｶｳﾝﾄ
			cnt++;
		}
		wvp = wvp->next;
	}
	CreatObj(vct, cnt, 0, &op);											// 立体領域確保
	LevelObj(0, op);													// 同一ﾚﾍﾞﾙ生成立体接続
	tvt = vtl;
	while(tvt!=NULL) {
		PutVtx(op, tvt->vt, tvt->pt);									// 頂点座標設定
		tvt = tvt->next;
	}
	wvp = list;
	while(wvp!=NULL) {
		if(!wvp->pfg) {
			PutFlp1(op, lct++, wvp->vt, wvp->lfg);						// 面ﾙｰﾌﾟ1設定
		}
		wvp = wvp->next;
	}
	if(atr==1) {														// <A側立体属性がJEWEL>
		PutObjAtr(op, 1);												// 属性No設定(DIRECT[GEM])
	} else {															// <A側立体属性がJEWEL以外>
		PutObjAtr(op, 0);												// 属性No設定(DIRECT)
	}
	PutObjPrt(op, 0, 1);												// ﾊﾟｰﾂNo設定(通常)
	PutObjCol(op, col);													// ｶﾗｰNo設定
	PutGroup(op, gno);													// ｸﾞﾙｰﾌﾟ情報設定
	SetObjBox(op);														// ﾎﾞｯｸｽｻｲｽﾞ設定
	MakeDirectCommand(op);												// 直接生成ｺﾏﾝﾄﾞ生成
	if(mode==0) {														// 先頭ﾎﾟｲﾝﾀを再保存
		A0list = list;
	} else if(mode==1) {
		A0list = list;
	} else if(mode==2) {
		A1list = list;
	}
	tvt = vtl;
	while(tvt) {														// 頂点ﾃﾞｰﾀ領域解放
		bvt = tvt, tvt = tvt->next;
		free(bvt);
	}
}

/******************************/
static void FreeWorkArea(void)											//<<< 作業領域解放
/******************************/
{
	BFATYPE* wfp, * dfp;
	BVLTYPE* wvp, * dvp;

	wfp = Aflist;
	while(wfp) {														// A側面情報領域解放
		dfp = wfp, wfp = wfp->next;
		free(dfp);
	}
	wfp = Bflist;
	while(wfp) {														// B側面情報領域解放
		dfp = wfp, wfp = wfp->next;
		free(dfp);
	}
	wvp = A0list;
	while(wvp) {														// A側作業領域解放
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = A1list;
	while(wvp) {														// A側作業領域解放
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = B0list;
	while(wvp) {														// B側作業領域解放
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = B1list;
	while(wvp) {														// B側作業領域解放
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
}

/***************************************************************/
static void CalcBoolean(OBJTYPE* opa, OBJTYPE* opb,
                        int mode, int flg1, int flg2, int delb)			//<<< 図形演算計算
/***************************************************************/
{
	int    gno[32], atr, col;
	double wkrs;

	InitWorkArea();														// 図形演算初期処理
	ReszB = EPSILON*1.1;												// 補正値(A側立体)
	wkrs = ReszB + EPSILON*1.1;											// 補正値(B側立体)
	ChangeObjVtx(0, opa, ReszB, ReszB, ReszB);							// A側立体頂点変更
	ChangeObjVtx(0, opb, wkrs, wkrs, wkrs);								// B側立体頂点変更
	atr = GetObjAtr(opa);												// 立体属性取得
	col = GetObjCol(opa);												// ｶﾗｰNo取得
	GetGroup(opa, gno);													// ｸﾞﾙｰﾌﾟ情報取得
	MakeFaceList(0, opa);												// A側立体面ﾘｽﾄ作成
	MakeFaceList(1, opb);												// B側立体面ﾘｽﾄ作成
	MakeBooleanList(0, opa, opb);										// A側図形演算ﾘｽﾄ作成
	MakeBooleanList(1, opb, opa);										// B側図形演算ﾘｽﾄ作成
	RemakeObject(mode, atr, col, gno);									// 図形演算立体作成
	FreeWorkArea();														// 作業領域解放
	ChangeObjVtx(1, opa, ReszB, ReszB, ReszB);							// A側立体頂点戻し
	ChangeObjVtx(1, opb, wkrs, wkrs, wkrs);								// B側立体頂点戻し
	if(flg1) {															// <<指定立体の場合>>
		if(delb==0||delb==2) {											// <削除あり>
			LevelObj(1, opa);											// 同一ﾚﾍﾞﾙ削除立体接続
			TempObj(opa);												// 立体一時削除
		}
	} else {															// <中間立体の場合>
		FreeObj(opa, 1);												// 作業立体削除
	}
	if(flg2) {															// <<指定立体の場合>>
		if(delb==0) {													// <削除あり>
			LevelObj(1, opb);											// 同一ﾚﾍﾞﾙ削除立体接続
			TempObj(opb);												// 立体一時削除
		}
	} else {															// <中間立体の場合>
		FreeObj(opb, 1);												// 作業立体削除
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/************************************************************************/
static void SetBopData(OBJTYPE* op, int flg, PNTTYPE cpt, BOPTYPE** bop)//<<< 演算対象立体ﾃﾞｰﾀ登録
/************************************************************************/
{
	BOPTYPE* top, * wop;

	top = (BOPTYPE *)malloc(sizeof(BOPTYPE));							// 演算対象立体領域確保
	if(top==NULL) {
		MemErr(IDS_MEMERR5);
	}
	top->op = op, top->flg = flg, top->cpt = cpt;
	top->leng = 0, top->next = NULL;
	wop = *bop;															// ﾃﾞｰﾀ末尾接続
	if(wop==NULL) {
		*bop = top;
	} else {
		while(wop->next!=NULL) {
			wop = wop->next;
		}
		wop->next = top;
	}
}

/************************************/
static void SetBopLeng(BOPTYPE* bop)									//<<< 演算対象立体基準距離登録
/************************************/
{
	BOPTYPE* wop;
	PNTTYPE  pnt;
	int      i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = DBL_MAX;
	}
	wop = bop;
	while(wop!=NULL) {
		for(i=0; i<DIM; i++) {
			if(pnt.p[i]>wop->cpt.p[i]) {
				pnt.p[i] = wop->cpt.p[i];								// 最小の中心点座標検索
			}
		}
		wop = wop->next;
	}
	wop = bop;
	while(wop!=NULL) {
		wop->leng = PntD(&pnt, &wop->cpt);								// 基準点からの距離
		wop = wop->next;
	}
}

/*************************************************/
static BOPTYPE* MergeLn(BOPTYPE* la, BOPTYPE* lb)						//<<< 基準距離ｿｰﾄ
/*************************************************/
{
	BOPTYPE* lc, * ld, * rc;

	lc = (BOPTYPE *)malloc(sizeof(BOPTYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->leng<=lb->leng) {										// 基準距離の大小で比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/*************************************************/
static BOPTYPE* Ln_Mergsrt(BOPTYPE* mlist, int n)						//<<< 基準距離ｿｰﾄ
/*************************************************/
{
	BOPTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BOPTYPE *)MergeLn(Ln_Mergsrt(la, n/2),
                                Ln_Mergsrt(lb, n-n/2));					// 基準距離ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/**************************************/
void BooleanObject(int mode, int delb)									//<<< 立体図形演算
/**************************************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙ表示
	OBJTYPE* op;
	BOXTYPE  box;
	PNTTYPE  pnt;
	BOPTYPE* aop=NULL, * bop=NULL, * sop, * wop, * dop;
	CString  str;
	int      i1, i2, act=0, bct=0;

	m_pProgressB = new CProgressDlg(1);
	m_pProgressB->Create();												// ﾀﾞｲｱﾛｸﾞ表示
	ErrFlg1 = 0, ErrFlg2 = 0;											// ｴﾗｰﾌﾗｸﾞ初期化
	SelBaseObjPtr(1, &op);												// A側立体取得
	while(op!=NULL) {
		GetObjBox(op, &box);											// BOXｻｲｽﾞ取得
		BoxCenter(box, &pnt);											// BOXｾﾝﾀｰ取得
		SetBopData(op, 1, pnt, &aop);									// 演算対象A立体ﾃﾞｰﾀ登録
		act++;															// A側立体ｶｳﾝﾄ
		SelNextObjPtr(1, &op);											// 次立体へ
	}
	BaseObjPtr(2, &op, &i1, &i2);										// B側立体取得
	while(op!=NULL) {
		if(GetSelNo(op)!=1) {
			GetObjBox(op, &box);										// BOXｻｲｽﾞ取得
			BoxCenter(box, &pnt);										// BOXｾﾝﾀｰ取得
			SetBopData(op, 1, pnt, &bop);								// 演算対象B立体ﾃﾞｰﾀ登録
			bct++;														// B側立体ｶｳﾝﾄ
			SetSelNo(op, 2);											// 選択No：2
		}
		NextObjPtr(2, &op, &i1, &i2);									// 次立体へ
	}
	if(act<1||bct<1) {													// 対象立体無は終了
		return;
	}
	if(act>1) {
		SetBopLeng(aop);												// 演算対象立体基準距離登録
		aop = Ln_Mergsrt(aop, act);										// 基準距離ｿｰﾄ
	}
	if(bct>1) {
		SetBopLeng(bop);												// 演算対象立体基準距離登録
		bop = Ln_Mergsrt(bop, bct);										// 基準距離ｿｰﾄ
	}
	Spr = 0.0, Ppr = 100.0/(act+bct-1);									// 1回の演算のﾊﾟｰｾﾝﾄ
	while(TRUE) {
		wop = aop, sop = NULL;
		while(wop!=NULL&&wop->next!=NULL) {
			op = wop->op;												// 奇数個目の立体
			i1 = wop->flg;
			wop = wop->next;											// 次(偶数個目)の立体へ
			CalcBoolean(op, wop->op, 1, i1, wop->flg, 0);				// 図形演算(奇数個目+偶数個目)
			GetObjPtr(&op);												// 演算後立体ﾎﾟｲﾝﾀ取得
			GetObjBox(op, &box);										// BOXｻｲｽﾞ取得
			BoxCenter(box, &pnt);										// BOXｾﾝﾀｰ取得
			SetBopData(op, 0, pnt, &sop);								// 演算後A立体ﾃﾞｰﾀ登録
			wop = wop->next;
		}
		if(wop!=NULL) {
			SetBopData(wop->op, wop->flg, wop->cpt, &sop);				// 演算後A立体ﾃﾞｰﾀ登録(残り)
		}
		wop = aop;
		while(wop) {													// 演算対象立体領域解放
			dop = wop, wop = wop->next;
			free(dop);
		}
		aop = sop;
		if(aop->next==NULL) {											// 演算対象1個で終了
			break;
		}
	}
	while(TRUE) {
		wop = bop, sop = NULL;
		while(wop!=NULL&&wop->next!=NULL) {
			op = wop->op;												// 奇数個目の立体
			i1 = wop->flg;
			wop = wop->next;											// 次(偶数個目)の立体へ
			CalcBoolean(op, wop->op, 1, i1, wop->flg, delb);			// 図形演算(奇数個目+偶数個目)
			GetObjPtr(&op);												// 演算後立体ﾎﾟｲﾝﾀ取得
			GetObjBox(op, &box);										// BOXｻｲｽﾞ取得
			BoxCenter(box, &pnt);										// BOXｾﾝﾀｰ取得
			SetBopData(op, 0, pnt, &sop);								// 演算後B立体ﾃﾞｰﾀ登録
			wop = wop->next;
		}
		if(wop!=NULL) {
			SetBopData(wop->op, wop->flg, wop->cpt, &sop);				// 演算後B立体ﾃﾞｰﾀ登録(残り)
		}
		wop = bop;
		while(wop) {													// 演算対象立体領域解放
			dop = wop, wop = wop->next;
			free(dop);
		}
		bop = sop;
		if(bop->next==NULL) {											// 演算対象1個で終了
			break;
		}
	}
	CalcBoolean(aop->op, bop->op, mode, aop->flg, bop->flg, delb*2);	// 図形演算(A立体,B立体)
	if(ErrFlg1) {
		str.Format(IDS_ERR_DATA, 1);									// 警告ﾒｯｾｰｼﾞ(1)
		AfxMessageBox(str);
	}
	if(ErrFlg2) {
		str.Format(IDS_ERR_DATA, 2);									// 警告ﾒｯｾｰｼﾞ(2)
		AfxMessageBox(str);
	}
	GetObjPtr(&op);														// 対象立体取得
	OnEdtFlg(1, op);													// 編集対象ﾌﾗｸﾞON
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	wop = aop;
	while(wop) {														// 演算対象A立体領域解放
		dop = wop, wop = wop->next;
		free(dop);
	}
	wop = bop;
	while(wop) {														// 演算対象B立体領域解放
		dop = wop, wop = wop->next;
		free(dop);
	}
	m_pProgressB->DestroyWindow();										// ﾀﾞｲｱﾛｸﾞ廃棄
	delete m_pProgressB;
}
