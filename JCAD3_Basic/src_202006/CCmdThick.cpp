/////////////////////////////////////////////////////////////////////////////
// 立体生成 曲面厚みづけコマンド部
// CCmdThick.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/11/03 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"

/***************************************************/
static int UPointCheck(int un, int vn, PNTTYPE* pa)						//<<< U制御点同一確認
/***************************************************/
{
	PNTTYPE pnt;
	int     i, j, chk, ret=0;

	pnt = pa[0];
	for(i=1, chk=0; i<un; i++) {										// <先頭Uﾙｰﾌﾟ確認>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i].p[j])>EPSILON) {						// 同一点でなし
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// 同一点は1加算
		ret += 1;
	}
	pnt = pa[un*(vn-1)];
	for(i=un*(vn-1)+1, chk=0; i<un*vn; i++) {							// <最終Uﾙｰﾌﾟ確認>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i].p[j])>EPSILON) {						// 同一点でなし
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// 同一点は2加算
		ret += 2;
	}
	return ret;
}

/***************************************************/
static int VPointCheck(int un, int vn, PNTTYPE* pa)						//<<< V制御点同一確認
/***************************************************/
{
	PNTTYPE pnt;
	int     i, j, chk, ret=0;

	pnt = pa[0];
	for(i=1, chk=0; i<vn; i++) {										// <先頭Vﾙｰﾌﾟ確認>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i*un].p[j])>EPSILON) {					// 同一点でなし
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// 同一点は1加算
		ret += 1;
	}
	pnt = pa[un-1];
	for(i=1, chk=0; i<vn; i++) {										// <最終Vﾙｰﾌﾟ確認>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i*un+un-1].p[j])>EPSILON) {				// 同一点でなし
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// 同一点は2加算
		ret += 2;
	}
	return ret;
}

/*********************************************************/
static BOOL GetThickFVec(OBJTYPE* op, double ln,
						 int vnum, int lnum, PNTTYPE* pa)				//<<< 面法線ﾍﾞｸﾄﾙ取得
/*********************************************************/
{
	VECTYPE* pv = new VECTYPE[View->m_MaxSPnt];
	VECTYPE* av = new VECTYPE[View->m_MaxSPnt];
	VECTYPE* wv = new VECTYPE[View->m_MaxCPnt];
	int*     fg = new int[View->m_MaxSPnt];
	int*     nx = new int[View->m_MaxSPnt];
	int*     wf = new int[View->m_MaxCPnt];
	PNTTYPE  pt[5], wpt;
	VECTYPE  v1, v2, v3, v4;
	int      pn[5], i, j, k, l, cnt, an, vno, flg, stp;
	double   th;

	for(i=0; i<vnum; i++) {
		fg[i] = -2;														// ﾌﾗｸﾞ初期化
	}
	for(i=0, cnt=0, an=0, stp=0; i<lnum; i++) {
		GetFlp1(op, i, &vno, &flg);										// 面ﾙｰﾌﾟ番号取得
		GetVtx(op, vno, &wpt);											// 頂点座標取得
		pt[cnt] = wpt;													// 頂点座標保存
		pn[cnt++] = vno;												// 頂点番号保存
		if(flg==1) {													// <<<ﾙｰﾌﾟ最終の場合>>>
			CalcNVec(pt, cnt, &v1);										// 法線ﾍﾞｸﾄﾙ取得
			for(j=0; j<cnt; j++) {
				if(fg[pn[j]]==-2) {										// <<初期の場合>>
					pv[pn[j]] = v1;										// 対象頂点ﾍﾞｸﾄﾙ設定
					fg[pn[j]] = -1;										// ﾌﾗｸﾞ更新
				} else {												// <<初期以外の場合>>
					v2 = pv[pn[j]];										// 登録済みﾍﾞｸﾄﾙ
					VecN(&v2);											// 単位ﾍﾞｸﾄﾙ化
					if(fabs(v1.p[0]-v2.p[0])>EPSILON||
						fabs(v1.p[1]-v2.p[1])>EPSILON||
						fabs(v1.p[2]-v2.p[2])>EPSILON) {				// 2ﾍﾞｸﾄﾙ間角度
						th = acos(v1.p[0]*v2.p[0]+v1.p[1]*v2.p[1]+v1.p[2]*v2.p[2]);
						th = (th<-10||th>10) ? 0.0 : th;
					} else {
						th = 0.0;
					}
					if(fabs(th)<2*PI/9) {								// <40度未満(曲線)>
						for(k=0; k<DIM; k++) {
							pv[pn[j]].p[k] += v1.p[k];					// 対象頂点ﾍﾞｸﾄﾙ加算
						}
					} else {											// <45度以上(折線)>
						if(fg[pn[j]]==-1) {								// <最初の折線>
							if(an>=View->m_MaxSPnt) {					// ﾃﾞｰﾀﾌﾙ中止
								stp = 1; break;
							}
							fg[pn[j]] = an;								// 折線配列番号設定
							av[an] = v1;								// 対象頂点ﾍﾞｸﾄﾙ設定
							nx[an++]  = -1;								// 次ﾃﾞｰﾀ番号初期化
						} else {										// <2件目以降の折線>
							if(an>=View->m_MaxSPnt) {					// ﾃﾞｰﾀﾌﾙ中止
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
							av[an] = v1;								// 対象頂点ﾍﾞｸﾄﾙ設定
							nx[an++] = -1;								// 次ﾃﾞｰﾀ番号初期化
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
		delete[] pv;
		delete[] av;
		delete[] wv;
		delete[] fg;
		delete[] nx;
		delete[] wf;
		return FALSE;
	} else {															// <ﾃﾞｰﾀOKの場合>
		for(i=0; i<vnum; i++) {
			VecN(&pv[i]);												// 単位ﾍﾞｸﾄﾙ化
			v1 = pv[i];
			if(fg[i]>=0) {												// <折線有の場合>
				cnt = 0;
				j = fg[i];
				while(1) {
					wv[cnt] = av[j];									// 折線ﾍﾞｸﾄﾙ複写
					wf[cnt++] = 0;										// 処理ﾌﾗｸﾞ初期化
					if(nx[j]==-1) {
						break;
					}
					j = nx[j];
				}
				for(j=0; j<cnt; j++) {
					if(wf[j]==0) {										// <未処理ﾍﾞｸﾄﾙ>
						v2 = wv[j];
						for(k=j+1; k<cnt; k++) {
							v3 = v2;
							VecN(&v3);									// 単位ﾍﾞｸﾄﾙ化
							v4 = wv[k];
							if(fabs(v3.p[0]-v4.p[0])>EPSILON||
                               fabs(v3.p[1]-v4.p[1])>EPSILON||
                               fabs(v3.p[2]-v4.p[2])>EPSILON) {			// 2ﾍﾞｸﾄﾙ間角度
								th = acos(v3.p[0]*v4.p[0]+v3.p[1]*v4.p[1]+v3.p[2]*v4.p[2]);
								th = (th<-10||th>10) ? 0.0 : th;
							} else {
								th = 0.0;
							}
							if(fabs(th)<2*PI/9) {						// <40度未満(曲線)>
								for(l=0; l<DIM; l++) {
									v2.p[l] += v4.p[l];					// 対象頂点ﾍﾞｸﾄﾙ加算
								}
								wf[k] = 1;								// 処理済み
							}
						}
						VecN(&v2);										// 単位ﾍﾞｸﾄﾙ化
						wv[j] = v2;										// 許容範囲内集計済
					}
				}
				for(an=1, j=0; j<cnt; j++) {
					if(wf[j]==0) {
						for(k=0; k<DIM; k++) {
							v1.p[k] += wv[j].p[k];						// 集計済加算
						}
						an++;
					}
				}
				v2 = v1;
				for(j=0; j<DIM; j++) {
					v2.p[j] /= an;										// 加算個数で除算
				}
				th = VecL(&v2);											// ﾍﾞｸﾄﾙ長さ
				th = (2.0/th)/2.0;										// 求めるﾍﾞｸﾄﾙ長
				VecN(&v1);												// 単位ﾍﾞｸﾄﾙ化
				for(j=0; j<DIM; j++) {
					v1.p[j] *= th;										// 長さ分乗算
				}
			}
			for(j=0; j<DIM; j++) {
				pa[i].p[j] += v1.p[j]*ln;								// 法線ﾍﾞｸﾄﾙ分移動
			}
		}
		delete[] pv;
		delete[] av;
		delete[] wv;
		delete[] fg;
		delete[] nx;
		delete[] wf;
		return TRUE;
	}
}

/***********************************************************/
static void PutThickVtx(OBJTYPE* op, int vnum, PNTTYPE* pa)				//<<< 頂点座標設定
/***********************************************************/
{
	int i;

	for(i=0; i<vnum; i++) {
		PutVtx(op, vnum+i, pa[i]);										// 頂点座標設定
	}
}

/**************************************************************/
static void PutThickFlp(OBJTYPE* op, int vn, int ln1, int ln2)			//<<< 面ﾙｰﾌﾟ設定
/**************************************************************/
{
	int i, vno, flg;

	for(i=0; i<ln2; i++) {
		GetFlp1(op, i+ln1, &vno, &flg);									// 面ﾙｰﾌﾟ2取得
		PutFlp2(op, i, vno, flg);										// 面ﾙｰﾌﾟ2ｼﾌﾄ
		PutFlp2(op, i+ln2, vno+vn, flg);								// 面ﾙｰﾌﾟ2設定
	}
	for(i=0; i<ln1; i++) {
		GetFlp1(op, i, &vno, &flg);										// 面ﾙｰﾌﾟ1取得
		PutFlp1(op, i+ln1, vno+vn, flg);								// 面ﾙｰﾌﾟ1設定
	}
}

/*****************************************************************/
static void ReverseFlp(OBJTYPE* op, int ln, int lnum1, int lnum2)		//<<< 面ﾙｰﾌﾟ反転
/*****************************************************************/
{
	int i, j, st, ed, sp, bl, fl, vn1, vn2, fg1, fg2;

	if(ln>0) {															// 反転範囲指定
		st = 0, ed = lnum1;
	} else {
		st = lnum1, ed = 2*lnum1;
	}
	for(sp=st-1, i=st; i<ed; i++) {
		GetFlp1(op, i, &vn1, &fg1);										// 面ﾙｰﾌﾟ1情報取得
		if(fg1) {														// <面ﾙｰﾌﾟ終了の場合>
			bl = i;														// 該当ﾙｰﾌﾟ最後尾
			fl = (i-sp)/2;
			for(j=sp+1; j<sp+1+fl; j++) {								// 該当ﾙｰﾌﾟ最前列より
				GetFlp1(op, j, &vn1, &fg1);								// 最後尾と頂点番号入替
				GetFlp1(op, bl, &vn2, &fg2);
				PutFlp1(op, j, vn2, fg1);
				PutFlp1(op, bl, vn1, fg2);
				bl--;
			}
			sp = i;														// 次ﾙｰﾌﾟ開始位置(+1)
		}
	}
	if(ln>0) {															// 反転範囲指定
		st = 0, ed = lnum2;
	} else {
		st = lnum2, ed = 2*lnum2;
	}
	for(sp=st-1, i=st; i<ed; i++) {
		GetFlp2(op, i, &vn1, &fg1);										// 面ﾙｰﾌﾟ2情報取得
		if(fg1) {														// <面ﾙｰﾌﾟ終了の場合>
			bl = i;														// 該当ﾙｰﾌﾟ最後尾
			fl = (i-sp)/2;
			for(j=sp+1; j<sp+1+fl; j++) {								// 該当ﾙｰﾌﾟ最前列より
				GetFlp2(op,  j, &vn1, &fg1);							// 最後尾と頂点番号入替
				GetFlp2(op, bl, &vn2, &fg2);
				PutFlp2(op,  j, vn2, fg1);
				PutFlp2(op, bl, vn1, fg2);
				bl--;
			}
			sp = i;														// 次ﾙｰﾌﾟ開始位置(+1)
		}
	}
}

/****************************************************************/
static void PutThickFlpU(OBJTYPE* op, int uflg, int vflg,
						 int vnum, int* ln1, int* ln2, int ln,
						 int ub, int un, int vn, int uo, int mx)		//<<< U断面ﾙｰﾌﾟ設定
/****************************************************************/
{
	int i, lno1, lno2, ad1, ad2, div;

	lno1 = *ln1;														// 現在の面ﾙｰﾌﾟ1数
	lno2 = *ln2;														// 現在の面ﾙｰﾌﾟ2数
	if(ln>0) {
		ad1 = vnum, ad2 = 0;
	} else {
		ad1 = 0, ad2 = vnum;
	}
	for(i=0, div=1; i<mx; i++) {										// 分割挿入数計算
		div = div*2;
	}
	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(uflg==0) {														// <<Uﾙｰﾌﾟ不同一>>
		if(ub) {														// <U閉&V開の場合>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno1++, i+ad1, 0);							// 面ﾙｰﾌﾟ1登録
				PutFlp1(op, lno1++, i+ad2, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, i+ad2+1, 0);
					PutFlp1(op, lno1++, i+ad1+1, 1);
				}
				PutFlp1(op, lno1++, un*(vn-1)+i+ad2, 0);
				PutFlp1(op, lno1++, un*(vn-1)+i+ad1, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, un*(vn-1)+ad1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+ad2, 1);
				} else {
					PutFlp1(op, lno1++, un*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad2+1, 1);
				}
			}
		} else {														// <U開&V開の場合>
			for(i=0; i<un-1; i++) {										// <共通先頭ﾙｰﾌﾟ>
				PutFlp1(op, lno1++, i+ad1, 0);
				PutFlp1(op, lno1++, i+ad2, 0);
				PutFlp1(op, lno1++, i+ad2+1, 0);
				PutFlp1(op, lno1++, i+ad1+1, 1);
			}
			if(vflg==0) {												// <Vﾙｰﾌﾟ不同一>
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno1++, un*(vn-1)+i+ad2, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad2+1, 1);
				}
			} else if(vflg==1) {										// <先頭Vﾙｰﾌﾟ同一>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, (un-1)*(vn-1)+ad1+1, 0);
				PutFlp1(op, lno1++, (un-1)*(vn-1)+ad2+1, 1);
				for(i=1; i<un-1; i++) {
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2+1, 1);
				}
			} else if(vflg==2) {										// <最終Vﾙｰﾌﾟ同一>
				for(i=0; i<un-2; i++) {
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2+1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1+2, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2+2, 1);
				}
				PutFlp1(op, lno1++, (un-1)*vn+ad2, 0);
				PutFlp1(op, lno1++, (un-1)*vn+ad1, 0);
				PutFlp1(op, lno1++, un-1+ad1, 0);
				PutFlp1(op, lno1++, un-1+ad2, 1);
			} else {													// <先頭/最終同一>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, (un-2)*(vn-1)+ad1+2, 0);
				PutFlp1(op, lno1++, (un-2)*(vn-1)+ad2+2, 1);
				for(i=1; i<un-2; i++) {
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad2+1, 0);
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad1+2, 0);
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad2+2, 1);
				}
				PutFlp1(op, lno1++, (un-2)*vn+ad2+1, 0);
				PutFlp1(op, lno1++, (un-2)*vn+ad1+1, 0);
				PutFlp1(op, lno1++, un-1+ad1, 0);
				PutFlp1(op, lno1++, un-1+ad2, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno2++, i*div+ad1, 0);							// 面ﾙｰﾌﾟ2登録
			PutFlp2(op, lno2++, i*div+ad2, 1);
		}
		if(vflg==0) {													// <Vﾙｰﾌﾟ不同一>
			for(i=0; i<uo; i++) {
				PutFlp2(op, lno2++, un*(vn-1)+i*div+ad2, 0);
				PutFlp2(op, lno2++, un*(vn-1)+i*div+ad1, 1);
			}
		} else if(vflg==1) {											// <先頭Vﾙｰﾌﾟ同一>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<uo; i++) {
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad2, 0);
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad1, 1);
			}
		} else if(vflg==2) {											// <最終Vﾙｰﾌﾟ同一>
			for(i=0; i<uo-1; i++) {
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad2+1, 0);
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad1+1, 1);
			}
			PutFlp2(op, lno2++, un-1+ad2, 0);
			PutFlp2(op, lno2++, un-1+ad1, 1);
		} else {														// <先頭/最終同一>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<uo-1; i++) {
				PutFlp2(op, lno2++, (un-2)*(vn-1)+i*div+ad2+1, 0);
				PutFlp2(op, lno2++, (un-2)*(vn-1)+i*div+ad1+1, 1);
			}
			PutFlp2(op, lno2++, un-1+ad2, 0);
			PutFlp2(op, lno2++, un-1+ad1, 1);
		}
	} else if(uflg==1) {												// <<先頭Uﾙｰﾌﾟ同一>>
		if(ub) {														// <U閉&V開の場合>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno1++, un*(vn-2)+i+ad2+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad1+1, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, un*(vn-2)+ad1+1, 0);
					PutFlp1(op, lno1++, un*(vn-2)+ad2+1, 1);
				} else {
					PutFlp1(op, lno1++, un*(vn-2)+i+ad1+2, 0);
					PutFlp1(op, lno1++, un*(vn-2)+i+ad2+2, 1);
				}
			}
		} else {														// <U開&V開の場合>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno1++, un*(vn-2)+i+ad2+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad1+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad1+2, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad2+2, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno2++, un*(vn-2)+i*div+ad2+1, 0);
			PutFlp2(op, lno2++, un*(vn-2)+i*div+ad1+1, 1);
		}
	} else if(uflg==2) {												// <<最終Uﾙｰﾌﾟ同一>>
		if(ub) {														// <U閉&V開の場合>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno1++, i+ad1, 0);
				PutFlp1(op, lno1++, i+ad2, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, i+ad2+1, 0);
					PutFlp1(op, lno1++, i+ad1+1, 1);
				}
			}
		} else {														// <U開&V開の場合>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno1++, i+ad1, 0);
				PutFlp1(op, lno1++, i+ad2, 0);
				PutFlp1(op, lno1++, i+ad2+1, 0);
				PutFlp1(op, lno1++, i+ad1+1, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno2++, i*div+ad1, 0);
			PutFlp2(op, lno2++, i*div+ad2, 1);
		}
	}
	*ln1 = lno1;														// 更新後面ﾙｰﾌﾟ1数
	*ln2 = lno2;														// 更新後面ﾙｰﾌﾟ2数
}

/****************************************************************/
static void PutThickFlpV(OBJTYPE* op, int uflg, int vflg,
						 int vnum, int* ln1, int* ln2, int ln,
						 int vb, int un, int vn, int vo, int mx)		//<<< V断面ﾙｰﾌﾟ設定
/****************************************************************/
{
	int i, lno1, lno2, ad1, ad2, div, ms1, ms2;

	lno1 = *ln1;														// 現在面ﾙｰﾌﾟ1数
	lno2 = *ln2;														// 現在面ﾙｰﾌﾟ2数
	if(ln<0) {
		ad1 = vnum, ad2 = 0;
	} else {
		ad1 = 0, ad2 = vnum;
	}
	for(i=0, div=1; i<mx; i++) {										// 分割挿入数計算
		div = div*2;
	}
	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	if(vflg==0) {														// <<Vﾙｰﾌﾟ不同一>>
		if(vb) {														// <U開&V閉の場合>
			for(i=0; i<vn; i++) {
				PutFlp1(op, lno1++, un*i+ad1, 0);						// 面ﾙｰﾌﾟ1登録
				PutFlp1(op, lno1++, un*i+ad2, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, un*(i+1)+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 1);
				}
				PutFlp1(op, lno1++, un-1+un*i+ad2, 0);
				PutFlp1(op, lno1++, un-1+un*i+ad1, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, un-1+ad1, 0);
					PutFlp1(op, lno1++, un-1+ad2, 1);
				} else {
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad2, 1);
				}
			}
		} else {														// <U開&V開の場合>
			if(uflg==0||uflg==2) {										// <共通先頭ﾙｰﾌﾟ>
				for(i=0; i<vn-1; i++) {
					PutFlp1(op, lno1++, un*i+ad1, 0);
					PutFlp1(op, lno1++, un*i+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 1);
				}
			} else {
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, 1+ad2, 0);
				PutFlp1(op, lno1++, 1+ad1, 1);
				for(i=1; i<vn-1; i++) {
					PutFlp1(op, lno1++, un*(i-1)+ad1+1, 0);
					PutFlp1(op, lno1++, un*(i-1)+ad2+1, 0);
					PutFlp1(op, lno1++, un*i+ad2+1, 0);
					PutFlp1(op, lno1++, un*i+ad1+1, 1);
				}
			}
			if(uflg==0) {												// <Uﾙｰﾌﾟ不同一>
				for(i=0; i<vn-1; i++) {
					PutFlp1(op, lno1++, un-1+un*i+ad2, 0);
					PutFlp1(op, lno1++, un-1+un*i+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad2, 1);
				}
			} else if(uflg==1) {										// <先頭Uﾙｰﾌﾟ同一>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, un+ad1, 0);
				PutFlp1(op, lno1++, un+ad2, 1);
				for(i=1; i<vn-1; i++) {
					PutFlp1(op, lno1++, un*i+ad2, 0);
					PutFlp1(op, lno1++, un*i+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad2, 1);
				}
			} else if(uflg==2) {										// <最終Uﾙｰﾌﾟ同一>
				for(i=0; i<vn-2; i++) {
					PutFlp1(op, lno1++, un-1+un*i+ad2, 0);
					PutFlp1(op, lno1++, un-1+un*i+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad2, 1);
				}
				PutFlp1(op, lno1++, un*(vn-1)+ad2-1, 0);
				PutFlp1(op, lno1++, un*(vn-1)+ad1-1, 0);
				PutFlp1(op, lno1++, un*(vn-1)+ad1, 0);
				PutFlp1(op, lno1++, un*(vn-1)+ad2, 1);
			} else {													// <先頭/最終同一>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, un+ad1, 0);
				PutFlp1(op, lno1++, un+ad2, 1);
				for(i=1; i<vn-2; i++) {
					PutFlp1(op, lno1++, un*i+ad2, 0);
					PutFlp1(op, lno1++, un*i+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad2, 1);
				}
				PutFlp1(op, lno1++, un*(vn-2)+ad2, 0);
				PutFlp1(op, lno1++, un*(vn-2)+ad1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+ad1+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+ad2+1, 1);
			}
		}
		if(uflg==0||uflg==2) {
			for(i=0; i<vo; i++) {
				PutFlp2(op, lno2++, div*un*i+ad1, 0);					// 面ﾙｰﾌﾟ2登録
				PutFlp2(op, lno2++, div*un*i+ad2, 1);
			}
		} else {
			PutFlp2(op, lno2++, ad1, 0);
			PutFlp2(op, lno2++, ad2, 1);
			for(i=1; i<vo; i++) {
				PutFlp2(op, lno2++, un*(div*i-1)+ad1+1, 0);
				PutFlp2(op, lno2++, un*(div*i-1)+ad2+1, 1);
			}
		}
		if(uflg==0) {													// <Uﾙｰﾌﾟ不同一>
			for(i=0; i<vo; i++) {
				PutFlp2(op, lno2++, un-1+div*un*i+ad2, 0);
				PutFlp2(op, lno2++, un-1+div*un*i+ad1, 1);
			}
		} else if(uflg==1) {											// <先頭Uﾙｰﾌﾟ同一>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<vo; i++) {
				PutFlp2(op, lno2++, div*un*i+ad2, 0);
				PutFlp2(op, lno2++, div*un*i+ad1, 1);
			}
		} else if(uflg==2) {											// <最終Uﾙｰﾌﾟ同一>
			for(i=0; i<vo-1; i++) {
				PutFlp2(op, lno2++, un-1+div*un*i+ad2, 0);
				PutFlp2(op, lno2++, un-1+div*un*i+ad1, 1);
			}
			PutFlp2(op, lno2++, un*(vn-1)+ad2, 0);
			PutFlp2(op, lno2++, un*(vn-1)+ad1, 1);
		} else {														// <先頭/最終同一>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<vo-1; i++) {
				PutFlp2(op, lno2++, div*un*i+ad2, 0);
				PutFlp2(op, lno2++, div*un*i+ad1, 1);
			}
			PutFlp2(op, lno2++, un*(vn-2)+ad2+1, 0);
			PutFlp2(op, lno2++, un*(vn-2)+ad1+1, 1);
		}
	} else if(vflg==1) {												// <<先頭Vﾙｰﾌﾟ同一>>
		if(vb) {														// <U開&V閉の場合>
			for(i=0; i<vn; i++) {
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad2, 0);
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad1, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, un-1+ad1, 0);
					PutFlp1(op, lno1++, un-1+ad2, 1);
				} else {
					PutFlp1(op, lno1++, (un-1)*(i+2)+ad1, 0);
					PutFlp1(op, lno1++, (un-1)*(i+2)+ad2, 1);
				}
			}
		} else {														// <U開&V開の場合>
			for(i=0; i<vn-1; i++) {
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad2, 0);
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad1, 0);
				PutFlp1(op, lno1++, (un-1)*(i+2)+ad1, 0);
				PutFlp1(op, lno1++, (un-1)*(i+2)+ad2, 1);
			}
		}
		for(i=0; i<vo; i++) {
			PutFlp2(op, lno2++, (un-1)*(div*i+1)+ad2, 0);
			PutFlp2(op, lno2++, (un-1)*(div*i+1)+ad1, 1);
		}
	} else if(vflg==2) {												// <<最終Vﾙｰﾌﾟ同一>>
		if(vb) {														// <U開&V閉の場合>
			for(i=0; i<vn; i++) {
				if(i==0) {												// 補正(-)数設定
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = i-1, ms2 = i;
				}
				PutFlp1(op, lno1++, un*i-ms1+ad1, 0);
				PutFlp1(op, lno1++, un*i-ms1+ad2, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, un*(i+1)-ms2+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)-ms2+ad1, 1);
				}
			}
		} else {														// <U開&V開の場合>
			for(i=0; i<vn-1; i++) {
				if(i==0) {
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = i-1, ms2 = i;
				}
				PutFlp1(op, lno1++, un*i-ms1+ad1, 0);
				PutFlp1(op, lno1++, un*i-ms1+ad2, 0);
				PutFlp1(op, lno1++, un*(i+1)-ms2+ad2, 0);
				PutFlp1(op, lno1++, un*(i+1)-ms2+ad1, 1);
			}
		}
		for(i=0; i<vo; i++) {
			ms1 = (div*i<2) ? 0 : div*i-1;
			PutFlp2(op, lno2++, un*div*i-ms1+ad1, 0);
			PutFlp2(op, lno2++, un*div*i-ms1+ad2, 1);
		}
	}
	*ln1 = lno1;														// 更新後面ﾙｰﾌﾟ1数
	*ln2 = lno2;														// 更新後面ﾙｰﾌﾟ2数
}

/******************************/
OBJTYPE* ThicknessObject(void)											//<<< 曲面厚みづけｺﾏﾝﾄﾞ
/******************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp, * bcp;
	int      i, j, ln, un, vn, uo, vo, ub, vb, mx, pc, atr;
	int      ovn, oln1, oln2, nvn, nln1, nln2, uflg=0, vflg=0;
	double   dln;

	GetObjPtr(&op);														// 処理対象立体取得
	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	ln = -GetParaInt(cmp, 0);											// 厚み幅取得
	BaseCmdPtr(1, op, &bcp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	un = GetParaInt(bcp, 0);											// U制御点数取得
	vn = GetParaInt(bcp, 1);											// V制御点数取得
	ub = GetParaInt(bcp, 2);											// U境界ﾓｰﾄﾞ取得
	vb = GetParaInt(bcp, 3);											// V境界ﾓｰﾄﾞ取得
	mx = GetParaInt(bcp, 4);											// 曲線化繰返数
	if(GetCurveReptSW()) {												// <曲面細分化オン>
		mx = GetCurveReptNum();											// 曲面分割数取得
	}
	uo = un, vo = vn;
	if((atr=GetObjAtr(op))==21) {										// <曲面ｺﾏﾝﾄﾞの場合>
		for(j=0, pc=0; j<vn; j++) {										// 制御点取得
			for(i=0; i<un; i++) {
				GetParaPnt(bcp, un*j+i, &pa[pc++]);
			}
			if(ub) {													// <U閉の場合>
				GetParaPnt(bcp, un*j, &pa[pc++]);
			}
		}
		if(ub) {														// U閉制御点数UP
			un++;
		}
		if(vb) {														// <V閉の場合>
			for(i=0; i<un; i++) {
				pa[pc++] = pa[i];
			}
			vn++;														// V閉制御点数UP
		}
		if(!vb) {
			uflg = UPointCheck(un, vn, pa);								// U同一点収束確認
		}
		if(!ub) {
			vflg = VPointCheck(un, vn, pa);								// V同一点収束確認
		}
	} else {															// <掃引曲面ｺﾏﾝﾄﾞの場合>
		if(ub) {
			un++;														// U閉制御点数UP
		}
		if(vb) {
			vn++;														// V閉制御点数UP
		}
	}
	for(i=0, pc=1; i<mx; i++) {											// 分割挿入数計算
		pc = 2*pc;
	}
	dln = ln/1000.0;													// 厚み幅実数化
	un = (un-1)*pc+1;													// U曲線化後頂点数
	vn = (vn-1)*pc+1;													// V曲線化後頂点数
	ovn  = GetVtxNum(op);												// 現頂点座標数取得
	oln1 = GetFlpNum1(op);												// 現面ﾙｰﾌﾟ1数取得
	oln2 = GetFlpNum2(op);												// 現面ﾙｰﾌﾟ2数取得
	nvn  = 2*ovn;														// 新頂点座標数
	nln1 = 2*oln1;														// 新面ﾙｰﾌﾟ1数
	nln2 = 2*oln2;														// 新面ﾙｰﾌﾟ2数
	if(!vb) {															// V開の断面
		if(uflg==0) {
			nln1 += 2*(un-1)*4, nln2 += 2*uo*2;
		} else if(uflg==1||uflg==2) {
			nln1 += (un-1)*4, nln2 += uo*2;
		}
	}
	if(!ub) {															// U開の断面
		if(vflg==0) {
			nln1 += 2*(vn-1)*4, nln2 += 2*vo*2;
		} else if(vflg==1||vflg==2) {
			nln1 += (vn-1)*4, nln2 += vo*2;
		}
	}
	for(i=0; i<ovn; i++) {
		GetVtx(op, i, &pa[i]);											// 頂点座標取得
	}
	if(GetThickFVec(op, dln, ovn, oln1, pa)) {							// 法線ﾍﾞｸﾄﾙ移動
		AppendCmd(op, cmp);												// 立体&ｺﾏﾝﾄﾞ接続
		ChangeObj(op, nvn, nln1, nln2);									// 立体領域変更
		LevelCmd(cmp);													// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
		if(!GetCmdExeStp()) {											// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
			PutThickVtx(op, ovn, pa);									// 頂点座標設定
			PutThickFlp(op, ovn, oln1, oln2);							// 面ﾙｰﾌﾟ設定
			ReverseFlp(op, ln, oln1, oln2);								// 面ﾙｰﾌﾟ反転
			nln1 = 2*oln1, nln2 = 2*oln2;
			if(!vb) {
				PutThickFlpU(op, uflg, vflg, ovn, &nln1, &nln2, ln,
			                 ub, un, vn, uo, mx);						// U断面ﾙｰﾌﾟ設定
	        }
		    if(!ub) {
			    PutThickFlpV(op, uflg, vflg, ovn, &nln1, &nln2, ln,
				             vb, un, vn, vo, mx);						// V断面ﾙｰﾌﾟ設定
			}
		}
		if(atr==21) {
			PutObjAtr(op, 20);											// 属性No設定(SURFACE)
		} else {
			PutObjAtr(op, 22);											// 属性No設定(SWEEP)
		}
		if(!GetCmdExeStp()&&!GetResetObjMode()) {						// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	delete[] pa;
	return op;
}
