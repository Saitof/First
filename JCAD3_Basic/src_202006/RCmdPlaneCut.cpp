/////////////////////////////////////////////////////////////////////////////
// コマンド関連 平面分割部
// RCmdPlaneCut.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include <limits.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "ECommand.h"
#include "RCommand.h"
#include "CutType.h"
#include "NumFunc.h"
CProgressDlg* m_pProgressC=NULL;

/***************************************/
static BOOL CheckCutObject(OBJTYPE* op)									//<<< 分割立体確認
/***************************************/
{
	BOXTYPE box;

	GetObjBox(op, &box);												// 立体ﾎﾞｯｸｽｻｲｽﾞ取得
	if(box.bp[0].p[2]<0.0&&box.bp[1].p[2]>0.0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/******************************/
static void InitWorkArea(void)											//<<< 分割初期処理
/******************************/
{
	Avlist = NULL, Bvlist = NULL, Cvlist = NULL;
	Avcnt = 0, Bvcnt = 0, Cvcnt = 0;
}

/******************************************/
static void SetCutVtx(PNTTYPE pt, int flg)								//<<< 分割頂点設定
/******************************************/
{
	CVLTYPE* tv;

	tv = (CVLTYPE *)malloc(sizeof(CVLTYPE));							// 分割頂点領域確保
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->pt = pt, tv->vt = -1, tv->flg = 0;
	if(!flg) {															// <A側>
		tv->next = Avlist, tv->no = Avcnt++, Avlist = tv;
	} else {															// <B側>
		tv->next = Bvlist, tv->no = Bvcnt++, Bvlist = tv;
	}
}

/*******************************/
static void SetFaceEnd(int flg)											//<<< 分割面ﾙｰﾌﾟ終了設定
/*******************************/
{
	if(!flg) {															// <A側>
		Avlist->flg = 1;
	} else {															// <B側>
		Bvlist->flg = 1;
	}
}

/****************************************************/
static void SetFaceCutLine(PNTTYPE spt, PNTTYPE ept)					//<<< 面分割線設定
/****************************************************/
{
	CCVTYPE* tc;

	tc = (CCVTYPE *)malloc(sizeof(CCVTYPE));							// 面分割線領域確保
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->spt = spt, tc->ept = ept;
	tc->next = Cvlist, Cvlist = tc, Cvcnt++;
}

/**************************************************************/
static void SetObjFace(OBJTYPE* op, int flg, int sno, int eno)			//<<< 面情報設定
/**************************************************************/
{
	PNTTYPE pt;
	int     i, lfg;

	for(i=sno; i<=eno; i++) {
		GetFlpVtx1(op, i, &pt, &lfg);									// 面ﾙｰﾌﾟ頂点取得
		SetCutVtx(pt, flg);												// 分割頂点設定
	}
	SetFaceEnd(flg);													// 分割面ﾙｰﾌﾟ終了設定
}

/*****************************/
static void InitCutWork(void)											//<<< 分割ﾘｽﾄ作成初期化
/*****************************/
{
	Aclist = NULL, Bclist = NULL, Cplist = NULL;
	Accnt = 0, Bccnt = 0, Cpcnt = 0;
}

/******************************************/
static void GetCLArea(PNTTYPE pt, int flg)								//<<< 面分割線分領域取得
/******************************************/
{
	CCLTYPE* ct;

	ct = (CCLTYPE *)malloc(sizeof(CCLTYPE));							// 面分割線分領域確保
	if(ct==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ct->spt = pt;
	ct->ept.p[0] = DBL_MAX, ct->ept.p[1] = DBL_MAX, ct->ept.p[2] = DBL_MAX;
	ct->cp = NULL;
	if(!flg) {															// <A側>
		ct->next = Aclist, Aclist = ct, Accnt++;
	} else {															// <B側>
		ct->next = Bclist, Bclist = ct, Bccnt++;
	}
}

/********************************************************/
static void GetCPArea(PNTTYPE spt, PNTTYPE ept, int flg)				//<<< 線分領域取得
/********************************************************/
{
	CCPTYPE* ct, * wc;

	ct = (CCPTYPE *)malloc(sizeof(CCPTYPE));							// 線分領域確保
	if(ct==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ct->next = NULL, ct->spt = spt, ct->ept = ept;
	if(!flg) {															// <A側>
		wc = Aclist->cp;
		if(wc==NULL) {
			Aclist->cp = ct;
		} else {
			while(wc->next!=NULL) {
				wc = wc->next;
			}
			wc->next = ct;
		}
		Aclist->ept = ept;
	} else {															// <B側>
		wc = Bclist->cp;
		if(wc==NULL) {
			Bclist->cp = ct;
		} else {
			while(wc->next!=NULL) {
				wc = wc->next;
			}
			wc->next = ct;
		}
		Bclist->ept = ept;
	}
}

/*************************************/
static void GetCrossArea(PNTTYPE spt)									//<<< 分割交点領域取得
/*************************************/
{
	CCPTYPE* ct;

	ct = (CCPTYPE *)malloc(sizeof(CCPTYPE));							// 分割交点領域確保
	if(ct==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ct->spt = spt;
	ct->ept.p[0] = DBL_MAX, ct->ept.p[1] = DBL_MAX, ct->ept.p[2] = DBL_MAX;
	ct->next = Cplist, Cplist = ct, Cpcnt++;
}

/****************************************************************/
static void GetCrossPoint(PNTTYPE pt1, PNTTYPE pt2, PNTTYPE* pt)		//<<< 交点取得
/****************************************************************/
{
	PNTTYPE wp1, wp2;
	double  a, b, c;

	if(pt1.p[2]>pt2.p[2]) {
		wp1 = pt1, wp2 = pt2;
	} else {
		wp1 = pt2, wp2 = pt1;
	}
	c = wp2.p[2]/(wp1.p[2]-wp2.p[2]);									// 始終点比率
	a = wp1.p[0]-wp2.p[0];												// X方向差
	b = wp1.p[1]-wp2.p[1];												// Y方向差
	pt->p[0] = wp2.p[0]-a*c;											// X方向算出
	pt->p[1] = wp2.p[1]-b*c;											// Y方向算出
	pt->p[2] = 0.0;
}

/*****************************************************/
static void SetCutFace(OBJTYPE* op, int sno, int eno)					//<<< 分割面情報設定
/*****************************************************/
{
	PNTTYPE spt, pt1, pt2, cpt;
	int     i, lf, sfg, fg1, fg2;

	GetFlpVtx1(op, sno, &pt1, &lf);										// 開始面ﾙｰﾌﾟ頂点取得
	fg1 = (pt1.p[2]>=0) ? 0 : 1;										// 位置判別
	spt = pt1, sfg = fg1;												// 開始点情報保存
	GetCLArea(pt1, fg1);												// 面分割線分領域取得
	for(i=sno+1; i<=eno; i++) {
		GetFlpVtx1(op, i, &pt2, &lf);									// 面ﾙｰﾌﾟ頂点取得
		fg2 = (pt2.p[2]>=0) ? 0 : 1;									// 位置判別
		if(fg1==fg2) {													// <<前点と同一位置>>
			GetCPArea(pt1, pt2, fg2);									// 線分領域取得
		} else {														// <<前点と異なる位置>>
			if(fabs(pt1.p[2])>EPSILON&&fabs(pt2.p[2])>EPSILON) {		// <2点のZが0以外>
				GetCrossPoint(pt1, pt2, &cpt);							// 交点取得
				GetCPArea(pt1, cpt, fg1);								// 線分領域取得
				GetCLArea(cpt, fg2);									// 面分割線分領域取得
				GetCPArea(cpt, pt2, fg2);								// 線分領域取得
				GetCrossArea(cpt);										// 分割交点領域取得
			} else if(fabs(pt1.p[2])<EPSILON) {							// <1点目のZが0>
				GetCLArea(pt1, fg2);									// 面分割線分領域取得
				GetCPArea(pt1, pt2, fg2);								// 線分領域取得
				GetCrossArea(pt1);										// 分割交点領域取得
			} else {													// <2点目のZが0>
				GetCPArea(pt1, pt2, fg1);								// 線分領域取得
				GetCLArea(pt2, fg2);									// 面分割線分領域取得
				GetCrossArea(pt2);										// 分割交点領域取得
			}
		}
		pt1 = pt2, fg1 = fg2;											// 2点目を1点目へ
	}
	if(fg1==sfg) {														// <<終了点と同一位置>>
		GetCPArea(pt1, spt, sfg);										// 線分領域取得
	} else {															// <<終了点と異の位置>>
		if(fabs(pt1.p[2])>EPSILON&&fabs(spt.p[2])>EPSILON) {			// <2点のZが0以外>
			GetCrossPoint(pt1, spt, &cpt);								// 交点取得
			GetCPArea(pt1, cpt, fg1);									// 線分領域取得
			GetCLArea(cpt, sfg);										// 面分割線分領域取得
			GetCPArea(cpt, spt, sfg);									// 線分領域取得
			GetCrossArea(cpt);											// 分割交点領域取得
		} else if(fabs(pt1.p[2])<EPSILON) {								// <終了点のZが0>
			GetCLArea(pt1, sfg);										// 面分割線分領域取得
			GetCPArea(pt1, spt, sfg);									// 線分領域取得
			GetCrossArea(pt1);											// 分割交点領域取得
		} else {														// <開始点のZが0>
			GetCPArea(pt1, spt, fg1);									// 線分領域取得
			GetCLArea(spt, sfg);										// 面分割線分領域取得
			GetCrossArea(spt);											// 分割交点領域取得
		}
	}
}

/*********************************************************/
static CCPTYPE* MergeCP(CCPTYPE* la, CCPTYPE* lb, int id)				//<<< 頂点座標ｿｰﾄ
/*********************************************************/
{
	CCPTYPE* lc, * ld, * rc;

	lc = (CCPTYPE *)malloc(sizeof(CCPTYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->spt.p[id]<=lb->spt.p[id]) {								// 頂点座標の大小比較
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
static CCPTYPE* CPMergsrt(CCPTYPE* mlist, int n, int id)				//<<< 頂点座標ｿｰﾄ
/********************************************************/
{
	CCPTYPE* la, * lb, * lm, * rc;
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
		lm = (CCPTYPE *)MergeCP(CPMergsrt(la, n/2, id),
                                CPMergsrt(lb, n-n/2, id), id);			// 頂点座標ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/****************************/
static void SetCutLine(void)											//<<< 分割線情報設定
/****************************/
{
	CCPTYPE* wcp, * scp;
	int      id;
	double   xmin, xmax, ymin, ymax;

	wcp = Cplist;														// X,Y最小/最大取得
	if(wcp!=NULL) {
		xmin = wcp->spt.p[0], xmax = wcp->spt.p[0];
		ymin = wcp->spt.p[1], ymax = wcp->spt.p[1];
	}
	while(wcp!=NULL) {
		if(xmin>wcp->spt.p[0]) {
			xmin = wcp->spt.p[0];
		}
		if(xmax<wcp->spt.p[0]) {
			xmax = wcp->spt.p[0];
		}
		if(ymin>wcp->spt.p[1]) {
			ymin = wcp->spt.p[1];
		}
		if(ymax<wcp->spt.p[1]) {
			ymax = wcp->spt.p[1];
		}
		wcp = wcp->next;
	}
	id = (xmax-xmin>ymax-ymin) ? 0 : 1;									// 確認方向
	Cplist = CPMergsrt(Cplist, Cpcnt, id);								// 頂点座標ｿｰﾄ
	wcp = Cplist;
	while(wcp!=NULL) {
		scp = wcp, wcp = wcp->next;
		if(wcp!=NULL) {
			scp->ept = wcp->spt, wcp->ept = scp->spt;					// 2点づつｾｯﾄ
			wcp = wcp->next;
		}
	}
}

/**********************************************/
static BOOL CheckPoint(PNTTYPE p1, PNTTYPE p2)							//<<< 座標点同一点確認
/**********************************************/
{
	if(fabs(p1.p[0]-p2.p[0])<EPSILON&&
       fabs(p1.p[1]-p2.p[1])<EPSILON&&
       fabs(p1.p[2]-p2.p[2])<EPSILON) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**************************************************/
static void SetNewFacePoint(CCLTYPE* slp, int flg)						//<<< 新面ﾙｰﾌﾟ頂点設定
/**************************************************/
{
	CCPTYPE* wpp, * dpp;

	wpp = slp->cp;
	while(wpp!=NULL) {
		dpp = wpp;														// ﾎﾟｲﾝﾀ保存
		wpp = wpp->next;												// 次ﾎﾟｲﾝﾀｾｯﾄ
		SetCutVtx(dpp->spt, flg);										// 分割頂点設定
		free(dpp);														// 線分配列領域解放
	}
}

/*************************************************************/
static BOOL SearchNextPoint(PNTTYPE p1, PNTTYPE* p2, int flg)			//<<< 次線分開始点検索
/*************************************************************/
{
	CCPTYPE* wpp, * bpp;
	BOOL     ret=TRUE;

	wpp = Cplist;
	while(wpp!=NULL) {
		if(CheckPoint(p1, wpp->spt)) {									// 座標点同一確認
			ret = FALSE;
			*p2 = wpp->ept;												// 次座標点ｾｯﾄ
			if(wpp==Cplist) {											// 対象領域ﾘｽﾄ外
				Cplist = wpp->next;
			} else {
				bpp->next = wpp->next;
			}
			SetCutVtx(wpp->spt, flg);									// 分割頂点設定
			if(!flg) {
				SetFaceCutLine(wpp->spt, wpp->ept);						// 面分割線設定
			}
			free(wpp);													// 領域解放
			Cpcnt--;
			break;
		}
		bpp = wpp, wpp = wpp->next;										// 現ﾎﾟｲﾝﾀを保存,次へ
	}
	return ret;
}

/************************************************************/
static BOOL SearchNextLine(PNTTYPE p1, PNTTYPE* p2, int flg)			//<<< 次線分検索
/************************************************************/
{
	CCLTYPE* wlp, * blp;
	BOOL     ret=TRUE;

	if(!flg) {
		wlp = Aclist;
	} else {
		wlp = Bclist;
	}
	while(wlp!=NULL) {
		if(CheckPoint(p1, wlp->spt)) {									// 座標点同一確認
			ret = FALSE;
			*p2 = wlp->ept;												// 次座標点ｾｯﾄ
			if(!flg) {
				if(wlp==Aclist) {										// 対象領域ﾘｽﾄ外
					Aclist = wlp->next;
				} else {
					blp->next = wlp->next;
				}
			} else {
				if(wlp==Bclist) {										// 対象領域ﾘｽﾄ外
					Bclist = wlp->next;
				} else {
					blp->next = wlp->next;
				}
			}
			SetNewFacePoint(wlp, flg);									// 新面ﾙｰﾌﾟ頂点設定
			if(!flg) {													// 領域解放
				free(wlp);
				Accnt--;
			} else {
				free(wlp);
				Bccnt--;
			}
			break;
		}
		blp = wlp, wlp = wlp->next;
	}
	return ret;
}

/****************************/
static void SetNewFace(void)											//<<< 新面情報設定
/****************************/
{
	CCLTYPE* slp;
	PNTTYPE  spt, ept, npt;

	while(TRUE) {														// <<<A側処理>>>
		if(Accnt<1) {													// 全数処理/ﾎﾟｲﾝﾀNULL終了
			break;
		}
		if(Aclist==NULL) {
			break;
		}
		slp = Aclist;													// 先頭処理対象
		Aclist = slp->next;												// 処理済ﾘｽﾄ外
		spt = slp->spt;													// 開始点保存
		ept = slp->ept;													// 終了点保存
		SetNewFacePoint(slp, 0);										// 新面ﾙｰﾌﾟ頂点設定
		free(slp);														// 領域解放
		Accnt--;
		if(fabs(ept.p[2])>EPSILON) {									// <終了点Z!=0>
			npt = ept;
			if(SearchNextLine(npt, &ept, 0)) {							// 次線分検索
				SetFaceEnd(0);
				continue;												// 該当無次面へ
			}
		}
		while(TRUE) {
			if(SearchNextPoint(ept, &npt, 0)) {							// 次線分開始点検索
				SetFaceEnd(0);
				break;													// 該当無次面へ
			}
			if(CheckPoint(spt, npt)) {									// 座標点の同一確認
				SetFaceEnd(0);
				break;													// 始点同一次面へ
			}
			if(SearchNextLine(npt, &ept, 0)) {							// 次線分検索
				SetFaceEnd(0);
				break;													// 該当無次面へ
			}
			if(CheckPoint(spt, ept)) {									// 座標点の同一確認
				SetFaceEnd(0);
				break;													// 始点同一次面へ
			}
		}
	}
	while(TRUE) {														// <<<B側処理>>>
		if(Bccnt<1) {													// 全数処理/ﾎﾟｲﾝﾀNULL終了
			break;
		}
		if(Bclist==NULL) {
			break;
		}
		slp = Bclist;													// 先頭処理対象
		Bclist = slp->next;												// 処理済ﾘｽﾄ外
		spt = slp->spt;													// 開始点保存
		ept = slp->ept;													// 終了点保存
		SetNewFacePoint(slp, 1);										// 新面ﾙｰﾌﾟ頂点設定
		free(slp);														// 領域解放
		Bccnt--;
		if(fabs(ept.p[2])>EPSILON) {									// <終了点Z!=0>
			npt = ept;
			if(SearchNextLine(npt, &ept, 1)) {							// 次線分検索
				SetFaceEnd(1);
				continue;												// 該当無次面へ
			}
		}
		while(TRUE) {
			if(SearchNextPoint(ept, &npt, 1)) {							// 次線分開始点検索
				SetFaceEnd(1);
				break;													// 該当無次面へ
			}
			if(CheckPoint(spt, npt)) {									// 座標点同一確認
				SetFaceEnd(1);
				break;													// 始点同一次面へ
			}
			if(SearchNextLine(npt, &ept, 1)) {							// 次線分検索
				SetFaceEnd(1);
				break;													// 該当無次面へ
			}
			if(CheckPoint(spt, ept)) {									// 座標点同一確認
				SetFaceEnd(1);
				break;													// 始点同一次面へ
			}
		}
	}
}

/*****************************/
static void FreeCutWork(void)											//<<< 分割作業領域解放
/*****************************/
{
	CCLTYPE* wlp, * dlp;
	CCPTYPE* wpp, * dpp;

	wlp = Aclist;														// <A側作業領域>
	while(wlp) {
		wpp = wlp->cp;
		while(wpp) {													// 線分配列領域解放
			dpp = wpp, wpp = wpp->next;
			free(dpp);
		}
		dlp = wlp, wlp = wlp->next;
		free(dlp);														// 面分割線分配列領域解放
	}
	wlp = Bclist;														// <B側作業領域>
	while(wlp) {
		wpp = wlp->cp;
		while(wpp) {													// 線分配列領域解放
			dpp = wpp, wpp = wpp->next;
			free(dpp);
		}
		dlp = wlp, wlp = wlp->next;
		free(dlp);														// 面分割線分配列領域解放
	}
	wpp = Cplist;														// <分割交点領域解放>
	while(wpp) {														// 線分配列領域解放
		dpp = wpp, wpp = wpp->next;
		free(dpp);
	}
}

/************************************/
static void MakeCutList(OBJTYPE* op)									//<<< 分割ﾘｽﾄ作成
/************************************/
{
	PNTTYPE pt;
	int     i, lnum, flg, st, iper=0;
	double  dper, doper, min1, max1;

	doper = Oper/2.0;													// 本処理ﾊﾟｰｾﾝﾄ
	lnum = GetFlpNum1(op);												// 面ﾙｰﾌﾟ数取得
	for(i=0, st=-1; i<lnum; i++) {
		GetFlpVtx1(op, i, &pt, &flg);									// 面ﾙｰﾌﾟ頂点取得
		if(st==-1) {													// 面ﾙｰﾌﾟ開始処理
			min1 = pt.p[2], max1 = pt.p[2], st = i;
		}
		if(min1>pt.p[2]) {												// 面最小Z値取得
			min1 = pt.p[2];
		}
		if(max1<pt.p[2]) {												// 面最大Z値取得
			max1 = pt.p[2];
		}
		if(flg) {														// <<面ﾙｰﾌﾟ終了>>
			if(fabs(min1)<EPSILON&&fabs(max1)<EPSILON) {				// <面がZ=0上に存在>
				SetObjFace(op, 0, st, i);								// 面情報設定(A側)
			} else if(min1>=0) {										// <面がZ正位置に存在>
				SetObjFace(op, 0, st, i);								// 面情報設定(A側)
			} else if(max1<=0) {										// <面がZ負位置に存在>
				SetObjFace(op, 1, st, i);								// 面情報設定(B側)
			} else {													// <面がZ正負に混在>
				InitCutWork();											// 分割ﾘｽﾄ作成初期化
				SetCutFace(op, st, i);									// 分割面情報設定(AB側)
				SetCutLine();											// 分割線情報設定
				SetNewFace();											// 新面情報設定
				FreeCutWork();											// 分割作業領域解放
			}
			st = -1;													// ﾙｰﾌﾟ開始ﾌﾗｸﾞ初期化
		}
		dper = doper*((double)i/(double)lnum);							// 現在ﾊﾟｰｾﾝﾄ
		if(iper<(int)(Sper+dper)) {										// <ﾊﾟｰｾﾝﾄUP>
			iper = (int)(Sper+dper);
			m_pProgressC->SetProgress(iper);							// 進捗表示
		}
	}
	Sper += doper;														// 累積ﾊﾟｰｾﾝﾄ
}

/*********************************/
static void Search(CCVTYPE** cvl)										//<<< 次線分検索
/*********************************/
{
	CCVTYPE* cvlo, * cvlt, * cvlb;

	cvlo = *cvl, cvlb = *cvl;											// 指定線分ﾃﾞｰﾀ
	cvlt = cvlo->next;													// 次線分ﾃﾞｰﾀ
	*cvl = cvlt;														// 次線分設定
	while(cvlt) {
		if(fabs(cvlt->spt.p[0]-cvlo->ept.p[0])<EPSILON&&				// <指定線分終点と対象線分
           fabs(cvlt->spt.p[1]-cvlo->ept.p[1])<EPSILON) {				//  始点が一致の場合>
			if(*cvl!=cvlt) {											// <指定線分の次と異の場合>
				*cvl = cvlt;											// 今回線分設定 
				cvlb->next = cvlt->next;								// 線分ﾘｽﾄ並替
				cvlt->next = cvlo->next;								// 今回線分先頭
			}
			return;
		}    
		cvlb = cvlt;													// 最終確認済ﾘｽﾄﾎﾟｲﾝﾀ
		cvlt = cvlt->next;												// 次線分へ
	}
}

/*********************************/
static void Chain(CCVTYPE** cvlo)										//<<< 線分並替
/*********************************/
{
	CCVTYPE* cvl, * cvlt;

	cvl = *cvlo;
	while(cvl) {
		cvlt = cvl;
		Search(&cvlt);													// 次線分検索
		cvl->next = cvlt;												// 次線分ﾎﾟｲﾝﾀ接続
		cvl = cvlt;														// 次線分へ
	}
}

/********************************/
static void MakeCutSection(void)										//<<< 分割断面作成
/********************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	CCVTYPE* cvl;
	PNTTYPE  pt;
	VECTYPE  vec;
	int      i, fg1, fg2, cnt=0;

	Chain(&Cvlist);														// 線分並替
	cvl = Cvlist;
	if(cvl!=NULL) {
		pt = cvl->spt;
	}
	while(cvl!=NULL) {
		if(!CheckPoint(pt, cvl->spt)) {									// 座標点同一点確認
			if(cnt>2) {													// <終了/開始異の有効ﾃﾞｰﾀ>
				CalcNVec(pa, cnt, &vec);								// 法線ﾍﾞｸﾄﾙ取得
				if(vec.p[2]<0.0) {										// 正ﾙｰﾌﾟ位置判別
					fg1 = 0, fg2 = 1;
				} else {
					fg1 = 1, fg2 = 0;
				}
				for(i=0; i<cnt; i++) {
					SetCutVtx(pa[i], fg1);								// 正ﾙｰﾌﾟ登録
				}
				SetFaceEnd(fg1);
				for(i=cnt-1; i>=0; i--) {
					SetCutVtx(pa[i], fg2);								// 逆ﾙｰﾌﾟ登録
				}
				SetFaceEnd(fg2);
			}
			cnt = 0;
		}
		pa[cnt++] = cvl->spt;											// 開始点保存
		pt = cvl->ept;													// 終了点保存
		cvl = cvl->next;												// 次線分へ
	}
	if(cnt>2) {															// <終了/開始異の有効ﾃﾞｰﾀ>
		CalcNVec(pa, cnt, &vec);										// 法線ﾍﾞｸﾄﾙ取得
		if(vec.p[2]<0.0) {												// 正ﾙｰﾌﾟ位置判別
			fg1 = 0, fg2 = 1;
		} else {
			fg1 = 1, fg2 = 0;
		}
		for(i=0; i<cnt; i++) {
			SetCutVtx(pa[i], fg1);										// 正ﾙｰﾌﾟ登録
		}
		SetFaceEnd(fg1);
		for(i=cnt-1; i>=0; i--) {
			SetCutVtx(pa[i], fg2);										// 逆ﾙｰﾌﾟ登録
		}
		SetFaceEnd(fg2);
	}
	delete[] pa;
}

/********************************************************/
static CVLTYPE* MergeV(CVLTYPE* la, CVLTYPE* lb, int id)				//<<< 頂点座標ｿｰﾄ
/********************************************************/
{
	CVLTYPE* lc, * ld, * rc;

	lc = (CVLTYPE *)malloc(sizeof(CVLTYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[id]<=lb->pt.p[id]) {								// 頂点座標の大小比較
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
static CVLTYPE* V_Mergsrt(CVLTYPE* mlist, int n, int id)				//<<< 頂点座標ｿｰﾄ
/********************************************************/
{
	CVLTYPE* la, * lb, * lm, * rc;
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
		lm = (CVLTYPE *)MergeV(V_Mergsrt(la, n/2, id),
                               V_Mergsrt(lb, n-n/2, id), id);			// 頂点座標ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/*************************************************/
static CVLTYPE* MergeVn(CVLTYPE* la, CVLTYPE* lb)						//<<< 面ﾙｰﾌﾟ番号ｿｰﾄ
/*************************************************/
{
	CVLTYPE* lc, * ld, * rc;

	lc = (CVLTYPE *)malloc(sizeof(CVLTYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// 面ﾙｰﾌﾟ番号の大小比較
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
static CVLTYPE* V_Mergsrtn(CVLTYPE* mlist, int n)						//<<< 面ﾙｰﾌﾟ番号ｿｰﾄ
/*************************************************/
{
	CVLTYPE* la, * lb, * lm, * rc;
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
		lm = (CVLTYPE *)MergeVn(V_Mergsrtn(la, n/2),
                                V_Mergsrtn(lb, n-n/2));					// 面ﾙｰﾌﾟ番号ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/****************************************************/
static void RemakeObject(int atr, int col, int* gno)					//<<< 分割立体作成
/****************************************************/
{
	CVLTYPE* wvp;
	OBJTYPE* op;
	PNTTYPE  pt;
	int      i, vct, lct, wct;
	double   dper;

	dper = Oper/20;														// 処理毎ﾊﾟｰｾﾝﾄ
	if(Avcnt>0) {														// <<<A側立体>>>
		wvp = Avlist;
		while(wvp!=NULL) {
			for(i=0; i<DIM; i++) {
				wvp->pt.p[i] -= ReszC;									// 頂点ﾃﾞｰﾀ戻し
			}
			wvp = wvp->next;
		}
		vct = 0, lct = 0;
		Avlist = V_Mergsrt(Avlist, Avcnt, 2);							// 頂点座標:Zｿｰﾄ
		Avlist = V_Mergsrt(Avlist, Avcnt, 1);							// 頂点座標:Yｿｰﾄ
		Avlist = V_Mergsrt(Avlist, Avcnt, 0);							// 頂点座標:Xｿｰﾄ
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		pt = Avlist->pt;
		pt.p[0] = pt.p[0]-1.0;											// 初期値設定
		wvp = Avlist;
		while(wvp!=NULL) {												// 正規頂点数ｶｳﾝﾄ
			if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
               fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
               fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {					// <設定済と異なる場合>
				wvp->vt = vct++;										// 頂点番号保存
				pt = wvp->pt;
			} else {													// 同は頂点番号保存
				wvp->vt = vct-1;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		CreatObj(vct, Avcnt, 0, &op);									// 立体領域確保
		LevelObj(0, op);												// 同一ﾚﾍﾞﾙ生成立体接続
		vct = 0, wct = -1, wvp = Avlist;
		while(wvp!=NULL) {
			if(wct!=wvp->vt) {											// <番号が異の場合>
				PutVtx(op, vct++, wvp->pt);								// 頂点座標設定
				wct = wvp->vt;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		Avlist = V_Mergsrtn(Avlist, Avcnt);								// 面ﾙｰﾌﾟ番号ｿｰﾄ
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		wvp = Avlist;
		while(wvp!=NULL) {
			PutFlp1(op, lct++, wvp->vt, wvp->flg);						// 面ﾙｰﾌﾟ1設定
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		if(atr==1) {													// <A側立体属性がJEWEL>
			PutObjAtr(op, 1);											// 属性No設定(DIRECT[GEM])
		} else {														// <A側立体属性がJEWEL以外>
			PutObjAtr(op, 0);											// 属性No設定(DIRECT)
		}
		PutObjPrt(op, 0, 1);											// ﾊﾟｰﾂNo設定(通常)
		PutObjCol(op, col);												// ｶﾗｰNo設定
		PutGroup(op, gno);												// ｸﾞﾙｰﾌﾟ情報設定
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
		OnEdtFlg(1, op);												// 編集対象ﾌﾗｸﾞON
		MakeDirectCommand(op);											// 直接生成ｺﾏﾝﾄﾞ生成
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	}
	if(Bvcnt>0) {														// <<<B側立体>>>
		wvp = Bvlist;
		while(wvp!=NULL) {
			for(i=0; i<DIM; i++) {
				wvp->pt.p[i] -= ReszC;									// 頂点ﾃﾞｰﾀ戻し
			}
			wvp = wvp->next;
		}
		vct = 0, lct = 0;
		Bvlist = V_Mergsrt(Bvlist, Bvcnt, 2);							// 頂点座標:Zｿｰﾄ
		Bvlist = V_Mergsrt(Bvlist, Bvcnt, 1);							// 頂点座標:Yｿｰﾄ
		Bvlist = V_Mergsrt(Bvlist, Bvcnt, 0);							// 頂点座標:Xｿｰﾄ
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		pt = Bvlist->pt;
		pt.p[0] = pt.p[0]-1.0;											// 初期値設定
		wvp = Bvlist;
		while(wvp!=NULL) {												// 正規頂点数ｶｳﾝﾄ
			if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
               fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
               fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {					// <設定済と異なる場合>
				wvp->vt = vct++;										// 頂点番号保存
				pt = wvp->pt;
			} else {													// 同は頂点番号保存
				wvp->vt = vct-1;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		CreatObj(vct, Bvcnt, 0, &op);									// 立体領域確保
		LevelObj(0, op);												// 同一ﾚﾍﾞﾙ生成立体接続
		vct = 0, wct = -1, wvp = Bvlist;
		while(wvp!=NULL) {
			if(wct!=wvp->vt) {											// <番号が異の場合>
				PutVtx(op, vct++, wvp->pt);								// 頂点座標設定
				wct = wvp->vt;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		Bvlist = V_Mergsrtn(Bvlist, Bvcnt);								// 面ﾙｰﾌﾟ番号ｿｰﾄ
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		wvp = Bvlist;
		while(wvp!=NULL) {
			PutFlp1(op, lct++, wvp->vt, wvp->flg);						// 面ﾙｰﾌﾟ1設定
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// 進捗表示
		if(atr==1) {													// <A側立体属性がJEWEL>
			PutObjAtr(op, 1);											// 属性No設定(DIRECT[GEM])
		} else {														// <A側立体属性がJEWEL以外>
			PutObjAtr(op, 0);											// 属性No設定(DIRECT)
		}
		PutObjPrt(op, 0, 1);											// ﾊﾟｰﾂNo設定(通常)
		PutObjCol(op, col);												// ｶﾗｰNo設定
		PutGroup(op, gno);												// ｸﾞﾙｰﾌﾟ情報設定
		OnEdtFlg(1, op);												// 編集対象ﾌﾗｸﾞON
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
		MakeDirectCommand(op);											// 直接生成ｺﾏﾝﾄﾞ生成
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	}
}

/******************************/
static void FreeWorkArea(void)											//<<< 作業領域解放
/******************************/
{
	CVLTYPE* wvp, * dvp;
	CCVTYPE* wcp, * dcp;

	wvp = Avlist;
	while(wvp) {														// A側作業領域解放
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = Bvlist;
	while(wvp) {														// B側作業領域解放
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wcp = Cvlist;
	while(wcp) {														// 断面作業領域解放
		dcp = wcp, wcp = wcp->next;
		free(dcp);
	}
}

/**********************************************************/
static void CutObject(double dz, double th, PNTTYPE pt,
                      VECTYPE vc1, VECTYPE vc2, int plane)				//<<< 立体平面(XY)分割
/**********************************************************/
{
	OBJTYPE* op, * wop;
	int      i, i1, i2, oct=0, wct=0, atr, col, gno[32];

	BaseObjPtr(2, &op, &i1, &i2);										// 処理対象立体取得
	while(op!=NULL) {
		oct++;															// 立体数ｶｳﾝﾄ
		NextObjPtr(2, &op, &i1, &i2);									// 次立体へ
	}
	Oper = 100.0/oct, Sper = 0.0;										// 1立体ﾊﾟｰｾﾝﾄ
	BaseObjPtr(2, &op, &i1, &i2);										// 処理対象立体取得
	while(op!=NULL&&oct>wct) {
		if(CheckCutObject(op)) {										// <<分割対象立体>>
			atr = GetObjAtr(op);										// 立体属性取得
			col = GetObjCol(op);										// ｶﾗｰNo取得
			GetGroup(op, gno);											// ｸﾞﾙｰﾌﾟ情報取得
			InitWorkArea();												// 初期処理
			ReszC = EPSILON*3.3;										// 補正値
			ChangeObjVtx(0, op, ReszC, ReszC, ReszC);					// 立体頂点変更
			MakeCutList(op);											// 分割ﾘｽﾄ作成
			i1 = GetObjAtr(op);											// 立体属性取得
			if(i1!=10&&i1!=11&&i1!=21&&i1!=23) {
				MakeCutSection();										// 分割断面作成
			}
			RemakeObject(atr, col, gno);								// 分割立体作成
			FreeWorkArea();												// 作業領域解放
			ChangeObjVtx(1, op, ReszC, ReszC, ReszC);					// 立体頂点戻し
		} else {														// <<分割対象外立体>>
			SetObjPtr(op);												// 複写対象立体設定
			wop = CopyObject();											// 立体の複写
			SetObjPtr(wop);												// 立体ﾎﾟｲﾝﾀ設定
			OnEdtFlg(1, wop);											// 編集対象ﾌﾗｸﾞON
			for(i=0; i<(int)Oper; i++) {
				m_pProgressC->SetProgress((int)Sper+i);					// 進捗表示
			}
			Sper += Oper;
		}
		wop = op;														// 削除立体ﾎﾟｲﾝﾀ保存
		NextObjPtr(2, &op, &i1, &i2);									// 次立体へ
		ObjMove(wop, 0, 0, dz);											// 元の位置へ戻す
		if(plane==0) {
			ObjRotate(wop, pt, vc2, -90.0);
		}
		ObjRotate(wop, pt, vc1, -th);
		LevelObj(1, wop);												// 同一ﾚﾍﾞﾙ削除立体接続
		TempObj(wop);													// 立体の一時削除
		wct++;
	}
}

/**************************************************/
void PCutObject(PNTTYPE p1, PNTTYPE p2, int plane)						//<<< 立体平面分割
/**************************************************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	VECTYPE vc1, vc2;
	double  th=0.0;

	m_pProgressC = new CProgressDlg(2);
	m_pProgressC->Create();												// ﾀﾞｲｱﾛｸﾞ表示
	if(plane==0) {														// <分割線XY平面>
		vc1.p[0] = 0.0, vc1.p[1] = 0.0, vc1.p[2] = 1.0;
		if(fabs(p2.p[1]-p1.p[1])<EPSILON) {
			th = PI/2.0;
		} else {
			th = atan2(p2.p[0]-p1.p[0], p2.p[1]-p1.p[1]);
		}
		vc2.p[0] = 0.0, vc2.p[2] = 0.0, vc2.p[1] = 1.0;
	} else if(plane==1) {												// <分割線YZ平面>
		vc1.p[0] = -1.0, vc1.p[1] = 0.0, vc1.p[2] = 0.0;
		if(fabs(p2.p[1]-p1.p[1])<EPSILON) {
			th = PI/2.0;
		} else {
			th = atan2(p2.p[2]-p1.p[2], p2.p[1]-p1.p[1]);
		}
	} else if(plane==2) {												// <分割線ZX平面>
		vc1.p[1] = 1.0, vc1.p[0] = 0.0, vc1.p[2] = 0.0;
		if(fabs(p2.p[0]-p1.p[0])<EPSILON) {
			th = PI/2.0;
		} else {
			th = atan2(p2.p[2]-p1.p[2], p2.p[0]-p1.p[0]);
		}
	}
	th = (th<-10||th>10) ? 0.0 : th;
	th *= 180.0/PI;														// RAD->DEG 変換
	th = (th>90.0) ? th-180.0 : th;
	Rotate(p1, vc1, th);												// XY平面へ回転移動
	if(plane==0) {
		Rotate(p1, vc2, 90.0);
	}
	Move(0.0, 0.0, -p1.p[2]);
	CutObject(p1.p[2], th, p1, vc1, vc2, plane);						// 平面分割処理
	Move(0, 0, p1.p[2]);												// 元位置へ戻す
	if(plane==0) {
		Rotate(p1, vc2, -90.0);
	}
	Rotate(p1, vc1, -th);
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	m_pProgressC->DestroyWindow();										// ﾀﾞｲｱﾛｸﾞ廃棄
	delete m_pProgressC;
}
