/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewRend.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/12/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "NumFunc.h"

/*********************************************/
BOOL CJcad3GlbView::GetJewelFVec(OBJTYPE* op)							//<<< 面法線ﾍﾞｸﾄﾙ取得
/*********************************************/
{
	VECTYPE vec;
	int     lnm, i, j, cnt, stn, vno, flg;

	if((lnm=GetFlpNum1(op))>m_MaxSPnt) {								// 面ﾙｰﾌﾟ数1取得
		return FALSE;
	}
	PNTTYPE* pt = new PNTTYPE[m_MaxCPnt];
	VECTYPE* lv = new VECTYPE[m_MaxSPnt];
	for(i=0, cnt=0, stn=0; i<lnm; i++) {
		GetFlp1(op, i, &vno, &flg);										// 面ﾙｰﾌﾟ番号取得
		GetVtx(op, vno, &pt[cnt++]);									// 頂点座標取得
		if(flg==1) {													// <<<ﾙｰﾌﾟ最終の場合>>>
			CalcNVec(pt, cnt, &vec);									// 法線ﾍﾞｸﾄﾙ取得
			for(j=0; j<cnt; j++) {
				lv[stn+j] = vec;										// 面ﾙｰﾌﾟ単位に設定
			}
			cnt = 0;													// 頂点ｶｳﾝﾀ初期化
			stn = i+1;													// 次ﾙｰﾌﾟ開始番号
		}
	}
	if(lnm>0) {
		SetRenderList(op, lv);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	}
	delete[] pt;
	delete[] lv;
	return TRUE;
}

/***********************************************/
BOOL CJcad3GlbView::SetShadingData(OBJTYPE* op)							//<<< ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ設定
/***********************************************/
{
	PNTTYPE wp;
	VECTYPE v1, v2, v3;
	int     vnm, lnm, i, j, k, l, cnt, an, vno, flg, stp;

	if((vnm=GetVtxNum(op))>m_MaxOPnt) {									// 頂点数取得
		return FALSE;
	}
	if((lnm=GetFlpNum1(op))>4*m_MaxOPnt) {								// 面ﾙｰﾌﾟ数1取得
		return FALSE;
	}
	PNTTYPE* pt = new PNTTYPE[m_MaxCPnt];
	VECTYPE* lv = new VECTYPE[4*m_MaxOPnt];
	VECTYPE* pv = new VECTYPE[m_MaxOPnt];
	VECTYPE* av = new VECTYPE[m_MaxOPnt];
	VECTYPE* wv = new VECTYPE[m_MaxCPnt];
	int*     fg = new int[m_MaxOPnt];
	int*     ln = new int[m_MaxOPnt];
	int*     nx = new int[m_MaxOPnt];
	int*     wf = new int[m_MaxCPnt];
	int*     pn = new int[m_MaxCPnt];
	int*     lp = new int[m_MaxCPnt];
	for(i=0; i<vnm; i++) {												// ﾌﾗｸﾞ初期化
		fg[i] = -2;
	}
	for(i=0, cnt=0, an=0, stp=0; i<lnm; i++) {
		GetFlp1(op, i, &vno, &flg);										// 面ﾙｰﾌﾟ番号取得
		GetVtx(op, vno, &wp);											// 頂点座標取得
		pt[cnt] = wp, lp[cnt] = i;										// 座標,ﾙｰﾌﾟ番号,頂点番号保存
		pn[cnt++] = vno;
		if(flg==1) {													// <ﾙｰﾌﾟ最終の場合>
			CalcNVec(pt, cnt, &v1);										// 法線ﾍﾞｸﾄﾙ取得
			for(j=0; j<cnt; j++) {
				if(fg[pn[j]]==-2) {										// <初期の場合>
					pv[pn[j]] = v1, fg[pn[j]] = -1;						// 対象頂点ﾍﾞｸﾄﾙ,ﾌﾗｸﾞ設定
				} else {												// <初期以外の場合>
					v2 = pv[pn[j]];										// 登録済ﾍﾞｸﾄﾙ
					VecN(&v2);											// 単位ﾍﾞｸﾄﾙ化
					if(fabs(CalcVecAngle(&v1, &v2))<PI/5.0) {			// <36度未満(曲線)>
						for(k=0; k<DIM; k++) { 
							pv[pn[j]].p[k] += v1.p[k];					// 対象頂点ﾍﾞｸﾄﾙ加算
						}
					} else {											// <36度以上(折線)>
						if(fg[pn[j]]==-1) {								// <最初の折線>
							if(an>=m_MaxOPnt) {							// ﾃﾞｰﾀﾌﾙ中止
								stp = 1; break;
							}
							fg[pn[j]] = an;								// 折線配列番号設定
							av[an] = v1, ln[an] = lp[j], nx[an++] = -1;	// 頂点ﾍﾞｸﾄﾙ,ﾙｰﾌﾟ番号設定
						} else {										// <2件目以降の折線>
							if(an>=m_MaxOPnt) {							// ﾃﾞｰﾀﾌﾙ中止
								stp = 1; break;
							}
							k = fg[pn[j]];
							while(1) {
								if(nx[k]==-1) {							// 最終折線配列番号検索
									break;
								}
								k = nx[k];
							}
							nx[k] = an;									// 折線配列番号設定
							av[an] = v1, ln[an] = lp[j], nx[an++] = -1;	// 頂点ﾍﾞｸﾄﾙ,ﾙｰﾌﾟ番号設定
						}
					}
				}
			}
			if(stp) {													// ﾃﾞｰﾀﾌﾙ中止
				break;
			}
			cnt = 0;													// 頂点ｶｳﾝﾀ初期化
		}
	}
	if(stp) {															// <ﾃﾞｰﾀﾌﾙの場合>
		delete[] pt;
		delete[] lv;
		delete[] pv;
		delete[] av;
		delete[] wv;
		delete[] fg;
		delete[] ln;
		delete[] nx;
		delete[] wf;
		delete[] pn;
		delete[] lp;
		return FALSE;
	}
	for(i=0; i<vnm; i++) {
		VecN(&pv[i]);													// 単位ﾍﾞｸﾄﾙ化
		if(fg[i]>=0) {													// <折線有の場合>
			cnt = 0, j = fg[i];
			while(1) {
				wv[cnt] = av[j], wf[cnt++] = 0;							// 折線ﾍﾞｸﾄﾙ複写
				if(nx[j]==-1) {
					break;
				}
				j = nx[j];
			}
			for(j=0; j<cnt; j++) {
				if(wf[j]==0) {											// <未処理ﾍﾞｸﾄﾙ>
					v1 = wv[j];
					for(k=j+1; k<cnt; k++) {
						v2 = v1;
						VecN(&v2);										// 単位ﾍﾞｸﾄﾙ化
						v3 = wv[k];
						if(fabs(CalcVecAngle(&v2, &v3))<PI/5.0) {		// <36度未満(曲線)>
							for(l=0; l<DIM; l++) {
								v1.p[l] += v3.p[l];						// 対象頂点ﾍﾞｸﾄﾙ加算
							}
							wf[k] = 1;									// 処理途中
						}
					}
					VecN(&v1);											// 単位ﾍﾞｸﾄﾙ化
					wv[j] = v1, wf[j] = 2;								// 許容範囲内集計済
					for(k=j+1; k<cnt; k++) {							// <処理途中の場合>
						if(wf[k]==1) {
							wv[k] = v1, wf[k] = 2;						// 許容範囲内集計済
						}
					}
				}
			}
			cnt = 0;
			j = fg[i];
			while(1) {
				av[j] = wv[cnt++];										// 折線ﾍﾞｸﾄﾙ復元
				if(nx[j]==-1) {
					break;
				}
				j = nx[j];
			}
		}
	}
	for(i=0; i<lnm; i++) {
		GetFlp1(op, i, &vno, &flg);										// 面ﾙｰﾌﾟ番号取得
		if(fg[vno]<0) {													// <全ﾙｰﾌﾟ集計>
			lv[i] = pv[vno];											// 面ﾙｰﾌﾟ法線ﾍﾞｸﾄﾙ
		} else {														// <上記以外>
			v1 = pv[vno], j = fg[vno];
			while(1) {
				if(i==ln[j]) {											// <ﾙｰﾌﾟ番号一致>
					v1 = av[j]; break;
				}
				if(nx[j]==-1) {											// ﾍﾞｸﾄﾙ置換
					break;
				}
				j = nx[j];
			}
			lv[i] = v1;													// 面ﾙｰﾌﾟ法線ﾍﾞｸﾄﾙ
		}
	}
	if(lnm>0) {
		SetRenderList(op, lv);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	}
	delete[] pt;
	delete[] lv;
	delete[] pv;
	delete[] av;
	delete[] wv;
	delete[] fg;
	delete[] ln;
	delete[] nx;
	delete[] wf;
	delete[] pn;
	delete[] lp;
	return TRUE;
}

/*****************************************/
BOOL CJcad3GlbView::ObjectRendering(void)                               //<<< ﾚﾝﾀﾞﾘﾝｸﾞ処理
/*****************************************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(1, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		if(GetListMode(op)!=3) {										// <<ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ未設定の場合>>
			if(GetObjAtr(op)!=1) {										// <宝石ｶｯﾄ以外>
				if(!SetShadingData(op)) {								// ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ設定
					return FALSE;
				}
			} else {													// <宝石ｶｯﾄ>
				if(!GetJewelFVec(op)) {									// 面法線ﾍﾞｸﾄﾙ取得
					return FALSE;
				}
			}
		}
		NextObjPtr(1, &op, &dsp, &edt);									// 次立体へ
	}
	return TRUE;
}

/**********************************************/
void CJcad3GlbView::ObjectShading(OBJTYPE* op)                          //<<< ｼｪｰﾃﾞｨﾝｸﾞ処理
/**********************************************/
{
	if(op!=NULL&&m_Shading) {											// <立体有でｼｪｰﾃﾞｨﾝｸﾞ表示有>
		if(GetListMode(op)!=3) {										// <<ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ未設定の場合>>
			CWaitCursor wait;											// ｳｪｲﾄ･ｶｰｿﾙを表示
			if(SetShadingData(op)) {									// ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ設定
				SetSelNo(op, 3);										// ｼｪｰﾃﾞｨﾝｸﾞ立体登録
				SetRenderMode(1);										// ｼｪｰﾃﾞｨﾝｸﾞﾓｰﾄﾞ
			}
		} else {														// <<ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ設定済の場合>>
			SetSelNo(op, 3);											// ｼｪｰﾃﾞｨﾝｸﾞ立体登録
			SetRenderMode(1);											// ｼｪｰﾃﾞｨﾝｸﾞﾓｰﾄﾞ
		}
	}
}

/**************************************************************/
void CJcad3GlbView::MenuAllShad(UINT flg, const CPoint& point,
                                int plane, int ms)                      //<<< ｼｪｰﾃﾞｨﾝｸﾞ
/**************************************************************/
{
	if(plane==PRS&&ms!=4) {												// 透視図上場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==0) {
		m_RendPlane = (m_FChar&0x1) ? 1 : 0;							// [F5]:3面図ﾚﾝﾀﾞﾘﾝｸﾞ
		if(ObjectRendering()) {                                         // ﾚﾝﾀﾞﾘﾝｸﾞ処理
			SetRenderMode(3);
		}
		Display(0);                                                     // 再描画
	} else if(ms==4||ms==9) {
		m_RendPlane = 0;
		SetRenderMode(0);
		Display(0);														// 再描画
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}

/*************************************************************/
void CJcad3GlbView::MenuRender(UINT flg, const CPoint& point,
                               int plane, int ms)                       //<<< ﾚﾝﾀﾞﾘﾝｸﾞ
/*************************************************************/
{
	if(plane==PRS&&ms!=4) {												// 透視図上場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==0) {
		m_RendPlane = (m_FChar&0x1) ? 1 : 0;							// [F5]:3面図ﾚﾝﾀﾞﾘﾝｸﾞ
		if(ObjectRendering()) {                                         // ﾚﾝﾀﾞﾘﾝｸﾞ処理
			SetRenderMode(2);
		}
		Display(0);                                                     // 再描画
	} else if(ms==4||ms==9) {
		m_RendPlane = 0;
		SetRenderMode(0);
		Display(0);														// 再描画
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}
