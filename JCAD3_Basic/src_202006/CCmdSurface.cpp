/////////////////////////////////////////////////////////////////////////////
// 立体生成 曲面生成コマンド部
// CCmdSurface.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"

/******************************************************************/
static void SetSectionPoint(int flg, int un, int vn,
                            PNTTYPE* pa, PNTTYPE* ps, PNTTYPE* pe)		//<<< 断面頂点保存
/******************************************************************/
{
	int i, j=0;

	if(!flg) {															// <U閉&V開の場合>
		for(i=0; i<un-1; i++) {
			ps[i] = pa[i];												// U開始断面保存
		}
		for(i=un-2; i>=0; i--) {
			pe[j++] = pa[i+un*(vn-1)];									// U終了断面保存
		}
	} else {															// <U開&V閉の場合>
		for(i=0; i<vn-1; i++) {
			ps[i] = pa[i*un];											// V開始断面保存
		}
		for(i=vn-2; i>=0; i--) {
			pe[j++] = pa[(i+1)*un-1];									// V終了断面保存
		}
	}
}

/*******************************************************/
static int CheckFacePoint(int pn, PNTTYPE* pa,
                          int* f1, int* f2, double* th)					//<<< 断面分割確認
/*******************************************************/
{
	PNTTYPE pt[3];
	VECTYPE vc1, vc2;
	int     i, j, j1, d, ch1, ch2, cnt=0;
	double  th1, th2, ln1, ln2;

	CalcNVec(pa, pn, &vc1);												// 法線ﾍﾞｸﾄﾙ取得
	for(i=0, d=0, th1=0.0; i<DIM; i++) {
		if(th1<fabs(vc1.p[i])) {										// 最大値座標方向取得
			th1 = fabs(vc1.p[i]), d = i;
		}
	}
	for(i=0; i<pn; i++) {												// 連続3点を抽出
		if(i==0) {
			pt[0] = pa[pn-1], pt[1] = pa[0],    pt[2] = pa[1];
		} else if(i==pn-1) {
			pt[0] = pa[pn-2], pt[1] = pa[pn-1], pt[2] = pa[0];
		} else {
			pt[0] = pa[i-1],  pt[1] = pa[i],    pt[2] = pa[i+1];
		}
		CalcRotAngleVec(pt, &th1, &vc2);								// 3点間角度取得
		ch1 = 1;														// ﾌﾗｸﾞ初期化(反対向)
		ch1 = (vc1.p[d]<0.0&&vc2.p[d]<0.0) ? 0 : ch1;					// 面法線ﾍﾞｸﾄﾙと同一方向の場合は0
		ch1 = (vc1.p[d]>0.0&&vc2.p[d]>0.0) ? 0 : ch1;
		ch1 = (th1<0.139) ? 0 : ch1;									// 角度8度未満は0
		if(ch1) {														// <<角度が逆方向点>>
			th1 = ((PI+th1)/2.0)-th1;									// 基準角度
			ln1 = DBL_MAX;
			ch2 = 0;
			for(j=0, j1=0; j<pn; j++) {
				if(j!=i) {
					pt[2] = pa[j];										// 対象点設定
					CalcRotAngleVec(pt, &th2, &vc2);					// 対象点との角度取得
					if((vc1.p[d]<0.0&&vc2.p[d]<0.0)||
						(vc1.p[d]>0.0&&vc2.p[d]>0.0)) {					// <法線方向一致>
						if(fabs(th2-th1)<0.785) {						// <基準角度差45度未満>
							ln2 = PntD(&pt[1], &pa[j]);					// 基準点との距離算出
							if(ln2<ln1) {								// 最短距離ﾃﾞｰﾀを保存
								j1 = j, ln1 = ln2, ch2 = 1;
							}
						}
					}
				}
			}
			if(ch2) {													// 分割線番号を設定
				f1[cnt] = i, f2[cnt] = j1, th[cnt++] = th1;
			}
		}
	}
	return cnt;
}

/*************************************************/
static int CheckSectionPoint(int pn, PNTTYPE* pa)						//<<< 断面頂点分割確認
/*************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCPnt];
	int*     f1 = new int[View->m_MaxCPnt];
	int*     f2 = new int[View->m_MaxCPnt];
	double*  th = new double[View->m_MaxCPnt];
	int      i, i1, i2, j, n1, n2, n3, st, ed, c0, c1, c2, c3, e1, e2, sp, cnt=0;
	double   sth;

	for(i=0; i<pn; i++) {												// 頂点情報複写
		p1[i] = pa[i];
	}
	n1 = pn;
	while(TRUE) {
		c0 = CheckFacePoint(n1, p1, f1, f2, th);						// 断面分割確認
		if(c0==0) {														// <断面分割無>
			break;
		} else {														// <断面分割有>
			n2 = 0, i1 = 0, i2 = 0, sp = 1, c3 = INT_MAX;
			for(i=0; i<c0; i++) {
				if(f1[i]<f2[i]) {										// 最端最少交線分割線検索
					st = f1[i], ed = f2[i];
				} else {
					st = f2[i], ed = f1[i];
				}
				for(j=0, c1=0, c2=0, e1=0, e2=0; j<c0; j++) {
					if(j!=i) {
						if(f1[j]<st||f1[j]>ed) {
							e1 = 1;
						}
						if(f2[j]<st||f2[j]>ed) {
							c1++;
						}
						if(f1[j]>st&&f1[j]<ed) {
							e2 = 1;
						}
						if(f2[j]>st&&f2[j]<ed) {
							c2++;
						}
						if(e1&&e2) {
							break;
						}
					}
				}
				if((!e1)&&c1<c3) {										// 検索分割線設定
					i1 = i, i2 = 0, c3 = c1, sp = 0;
				}
				if((!e2)&&c2<c3) {
					i1 = i, i2 = 1, c3 = c2, sp = 0;
				}
			}
			if(sp) {													// <検索ﾃﾞｰﾀ無>
				for(sth=PI, i=0, i1=0; i<c0; i++) {
					if(sth>th[i]) {										// 基準角度最小検索
						i1 = i, sth = th[i];
					}
				}
				if(f1[i1]<f2[i1]) {										// 最小位置に分割線
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				for(n2=0, i=0; i<=st; i++) {							// 分割後面頂点数確認
					n2++;
				}
				for(i=ed; i<n1; i++) {
					n2++;
				}
				for(n3=0, i=st; i<=ed; i++) {
					n3++;
				}
				if(n2>2&&n3>2) {										// 2面とも正常分割数UP
					cnt++;
				}
				break;													// 処理終了
			} else {													// <検索ﾃﾞｰﾀ有>
				if(f1[i1]<f2[i1]) {										// 検索位置に分割線
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				if(!i2) {												// 今回分割面設定
					for(i=0; i<=st; i++) {
						p2[n2++] = p1[i];
					}
					for(i=ed; i<n1; i++) {
						p2[n2++] = p1[i];
					}
				} else {
					for(i=st; i<=ed; i++) {
						p2[n2++] = p1[i];
					}
				}
				if(n2>2) {												// <有効分割面の場合>
					cnt++;												// 分割数UP
					if(!i2) {											// 次回分割面の設定
						for(n2=0, i=st; i<=ed; i++) {
							p2[n2++] = p1[i];
						}
					} else {
						for(n2=0, i=0; i<=st; i++) {
							p2[n2++] = p1[i];
						}
						for(i=ed; i<n1; i++) {
							p2[n2++] = p1[i];
						}
					}
					for(i=0; i<n2; i++) {
						p1[i] = p2[i];									// 次確認ﾃﾞｰﾀｼﾌﾄ
					}
					n1 = n2;
				} else {												// <無効分割面の場合>
					for(sth=PI, i=0, i1=0; i<c0; i++) {
						if(sth>th[i]) {									// 基準角度最小検索
							i1 = i, sth = th[i];
						}
					}
					if(f1[i1]<f2[i1]) {									// 最小位置に分割線
						st = f1[i1], ed = f2[i1];
					} else {
						st = f2[i1], ed = f1[i1];
					}
					for(n2=0, i=0; i<=st; i++) {						// 分割後の面頂点数確認
						n2++;
					}
					for(i=ed; i<n1; i++) {
						n2++;
					}
					for(n3=0, i=st; i<=ed; i++) {
						n3++;
					}
					if(n2>2&&n3>2) {									// 2面とも正常分割数UP
						cnt++;
					}
					break;												// 処理終了
				}
			}
		}
	}
	delete[] p1;
	delete[] p2;
	delete[] f1;
	delete[] f2;
	delete[] th;
	return cnt;
}

/***************************************************/
static int UPointCheck(int un, int vn, PNTTYPE* pa)						//<<< U制御点同一確認
/***************************************************/
{
	PNTTYPE pnt;
	int     i, j, chk, ret=0;

	pnt = pa[0];
	for(i=1, chk=0; i<un; i++) {										// <先頭Uﾙｰﾌﾟ確認>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i].p[j])>EPSILON) {						// 同一点でない
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
			if(fabs(pnt.p[j]-pa[i].p[j])>EPSILON) {						// 同一点でない
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
			if(fabs(pnt.p[j]-pa[i*un].p[j])>EPSILON) {					// 同一点でない
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
			if(fabs(pnt.p[j]-pa[i*un+un-1].p[j])>EPSILON) {				// 同一点でない
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// 同一点は2加算
		ret += 2;
	}
	return ret;
}

/**************************************************/
static void SetVtx(OBJTYPE* op, PNTTYPE* pa,
                   int ub, int vb, int un, int vn)						//<<< 曲面頂点登録
/**************************************************/
{
	int i, j, uu, vno=0;

	uu = un;															// U制御点数保存
	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	for(j=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i+uu*j]);								// 頂点登録
		}
	}
}

/********************************************************/
static void SetVtxU(OBJTYPE* op, int flg,
                    PNTTYPE* pa, int ub, int un, int vn)				//<<< 頂点登録(U同一点)
/********************************************************/
{
	int i, j, uu, vno=0;

	uu = un;															// U制御点数保存
	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(flg==1||flg==3) {												// <先頭Uﾙｰﾌﾟ同一>
		PutVtx(op, vno++, pa[0]);										// 頂点登録
	} else {															// <先頭Uﾙｰﾌﾟ通常>
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i]);
		}
	}
	for(j=1; j<vn-1; j++) {
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i+uu*j]);
		}
	}
	if(flg==2||flg==3) {												// <最終Uﾙｰﾌﾟ同一>
		PutVtx(op, vno++, pa[uu*(vn-1)]);
	} else {															// <最終Uﾙｰﾌﾟ通常>
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i+uu*(vn-1)]);
		}
	}
}

/********************************************************/
static void SetVtxV(OBJTYPE* op, int flg,
                    PNTTYPE* pa, int vb, int un, int vn)				//<<< 頂点登録(V同一点)
/********************************************************/
{
	int i, j, vno=0;

	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	for(i=0; i<un; i++) {
		PutVtx(op, vno++, pa[i]);										// 頂点登録
	}
	if(flg==1) {														// <先頭Vﾙｰﾌﾟ同一>
		for(j=1; j<vn; j++) {
			for(i=1; i<un; i++) {
				PutVtx(op, vno++, pa[i+un*j]);
			}
		}
	} else if(flg==2) {													// <最終Vﾙｰﾌﾟ同一>
		for(j=1; j<vn; j++) {
			for(i=0; i<un-1; i++) {
				PutVtx(op, vno++, pa[i+un*j]);
			}
		}
	} else {															// <先頭/最終同一>
		for(j=1; j<vn; j++) {
			for(i=1; i<un-1; i++) {
				PutVtx(op, vno++, pa[i+un*j]);
			}
		}
	}
}

/**************************************************/
static void SetFac(int* lnum, OBJTYPE* op,
                   int ub, int vb, int un, int vn)						//<<< 曲面面ﾙｰﾌﾟ1登録
/**************************************************/
{
	int i, j, lno;

	lno = *lnum;
	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	if(ub&&vb) {														// <U閉&V閉の場合>
		for(j=0; j<vn; j++) {
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*j+i, 0);							// 面ﾙｰﾌﾟ登録
				if(i==un-1) {
					PutFlp1(op, lno++, un*j, 0);
				} else {
					PutFlp1(op, lno++, un*j+i+1, 0);
				}
				if(j==vn-1) {
					if(i==un-1) {
						PutFlp1(op, lno++, 0, 0);
					} else {
						PutFlp1(op, lno++, i+1, 0);
					}
					PutFlp1(op, lno++, i, 1);
				} else {
					if(i==un-1) {
						PutFlp1(op, lno++, un*(j+1), 0);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					}
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		}
	} else if(ub&&(!vb)) {												// <U閉&V開の場合>
		for(j=0; j<vn-1; j++) {
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*j+i, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, un*j, 0);
					PutFlp1(op, lno++, un*(j+1), 0);
				} else {
					PutFlp1(op, lno++, un*j+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
				}
				PutFlp1(op, lno++, un*(j+1)+i, 1);
			}
		}
	} else if((!ub)&&vb) {												// <U開&V閉の場合>
		for(j=0; j<vn; j++) {
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*j+i, 0);
				PutFlp1(op, lno++, un*j+i+1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, i+1, 0);
					PutFlp1(op, lno++, i, 1);
				} else {
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		}
	} else {															// <U開&V開の場合>
		for(j=0; j<vn-1; j++) {
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*j+i, 0);
				PutFlp1(op, lno++, un*j+i+1, 0);
				PutFlp1(op, lno++, un*(j+1)+i+1, 0);
				PutFlp1(op, lno++, un*(j+1)+i, 1);
			}
		}
	}
	*lnum = lno;
}

/****************************************************/
static void SetFacU(int* lnum, OBJTYPE* op,
                    int flg, int ub, int un, int vn)					//<<< 面ﾙｰﾌﾟ1登録(U同一点)
/****************************************************/
{
	int i, j, lno;

	lno = *lnum;
	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(flg==1) {														// <<先頭Uﾙｰﾌﾟ同一>>
		if(ub) {														// <U閉の場合>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, 0, 0);								// 面ﾙｰﾌﾟ登録
				if(i==un-1) {
					PutFlp1(op, lno++, 1, 0);
				} else {
					PutFlp1(op, lno++, i+2, 0);
				}
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-1; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					if(i==un-1) {
						PutFlp1(op, lno++, un*(j-1)+1, 0);
						PutFlp1(op, lno++, un*j+1, 0);
					} else {
						PutFlp1(op, lno++, un*(j-1)+i+2, 0);
						PutFlp1(op, lno++, un*j+i+2, 0);
					}
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
		} else {														// <U開の場合>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, i+2, 0);
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-1; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					PutFlp1(op, lno++, un*(j-1)+i+2, 0);
					PutFlp1(op, lno++, un*j+i+2, 0);
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
		}
	} else if(flg==2) {													// <<最終Uﾙｰﾌﾟ同一>>
		if(ub) {														// <U閉の場合>
			for(j=0; j<vn-2; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, (un*j)+i, 0);
					if(i==un-1) {
						PutFlp1(op, lno++, un*j, 0);
						PutFlp1(op, lno++, un*(j+1), 0);
					} else {
						PutFlp1(op, lno++, un*j+i+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					}
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*(vn-2)+i, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, un*(vn-2), 0);
				} else {
					PutFlp1(op, lno++, un*(vn-2)+i+1, 0);
				}
				PutFlp1(op, lno++, un*(vn-1), 1);
			}
		} else {														// <U開の場合>
			for(j=0; j<vn-2; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*j+i, 0);
					PutFlp1(op, lno++, un*j+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*(vn-2)+i, 0);
				PutFlp1(op, lno++, un*(vn-2)+i+1, 0);
				PutFlp1(op, lno++, un*(vn-1), 1);
			}
		}
	} else {															// <<先頭/最終同一>>
		if(ub) {														// <U閉の場合>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, 0, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, 1, 0);
				} else {
					PutFlp1(op, lno++, i+2, 0);
				}
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-2; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					if(i==un-1) {
						PutFlp1(op, lno++, un*(j-1)+1, 0);
						PutFlp1(op, lno++, un*j+1, 0);
					} else {
						PutFlp1(op, lno++, un*(j-1)+i+2, 0);
						PutFlp1(op, lno++, un*j+i+2, 0);
					}
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*(vn-3)+i+1, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, un*(vn-3)+1, 0);
				} else {
					PutFlp1(op, lno++, un*(vn-3)+i+2, 0);
				}
				PutFlp1(op, lno++, un*(vn-2)+1, 1);
			}
		} else {														// <U開の場合>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, i+2, 0);
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-2; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					PutFlp1(op, lno++, un*(j-1)+i+2, 0);
					PutFlp1(op, lno++, un*j+i+2, 0);
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*(vn-3)+i+1, 0);
				PutFlp1(op, lno++, un*(vn-3)+i+2, 0);
				PutFlp1(op, lno++, un*(vn-2)+1, 1);
			}
		}
	}
	*lnum = lno;
}

/****************************************************/
static void SetFacV(int* lnum, OBJTYPE* op,
                    int flg, int vb, int un, int vn)					//<<< 面ﾙｰﾌﾟ1登録(V同一点)
/****************************************************/
{
	int i, j, ms1, ms2, lno;

	lno = *lnum;
	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	if(flg==1) {														// <<先頭Vﾙｰﾌﾟ同一>>
		if(vb) {														// <V閉の場合>
			for(j=0; j<vn; j++) {
				PutFlp1(op, lno++, 0, 0);								// 面ﾙｰﾌﾟ登録
				PutFlp1(op, lno++, (un-1)*j+1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, 1, 1);
				} else {
					PutFlp1(op, lno++, (un-1)*(j+1)+1, 1);
				}
				for(i=1; i<un-1; i++) {
					PutFlp1(op, lno++, (un-1)*j+i, 0);
					PutFlp1(op, lno++, (un-1)*j+i+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, (un-1)*(j+1)+i+1, 0);
						PutFlp1(op, lno++, (un-1)*(j+1)+i, 1);
					}
				}
			}
		} else {														// <V開の場合>
			for(j=0; j<vn-1; j++) {
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, (un-1)*j+1, 0);
				PutFlp1(op, lno++, (un-1)*(j+1)+1, 1);
				for(i=1; i<un-1; i++) {
					PutFlp1(op, lno++, (un-1)*j+i, 0);
					PutFlp1(op, lno++, (un-1)*j+i+1, 0);
					PutFlp1(op, lno++, (un-1)*(j+1)+i+1, 0);
					PutFlp1(op, lno++, (un-1)*(j+1)+i, 1);
				}
			}
		}
	} else if(flg==2) {													// <<最終Vﾙｰﾌﾟ同一>>
		if(vb) {														// <V閉の場合>
			for(j=0; j<vn; j++) {
				if(j==0) {												// 補正(-)数設定
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = j-1, ms2 = j;
				}
				for(i=0; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
					}
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, un-2, 1);
				} else {
					PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
				}
			}
		} else {														// <V開の場合>
			for(j=0; j<vn-1; j++) {
				if(j==0) {
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = j-1, ms2 = j;
				}
				for(i=0; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
			}
		}
	} else {															// <<先頭/最終同一>>
		if(vb) {														// <V閉の場合>
			for(j=0; j<vn; j++) {
				if(j==0) {												// 補正(-)数設定
					ms1 = 0, ms2 = 1;
				} else if(j==1) {
					ms1 = 1, ms2 = 3;
				} else {
					ms1 = 2*j-1, ms2 = 2*j+1;
				}
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, un*j-ms1+1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, 1, 1);
				} else {
					PutFlp1(op, lno++, un*(j+1)-ms2+1, 1);
				}
				for(i=1; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
					}
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, un-2, 1);
				} else {
					PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
				}
			}
		} else {														// <V開の場合>
			for(j=0; j<vn-1; j++) {
				if(j==0) {
					ms1 = 0, ms2 = 1;
				} else if(j==1) {
					ms1 = 1, ms2 = 3;
				} else {
					ms1 = 2*j-1, ms2 = 2*j+1;
				}
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, un*j-ms1+1, 0);
				PutFlp1(op, lno++, un*(j+1)-ms2+1, 1);
				for(i=1; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
			}
		}
	}
	*lnum = lno;
}

/********************************************************/
static void SetSectionFace(int* num, OBJTYPE* op,
                           int pn, PNTTYPE* pa, int* va)				//<<< 断面頂点ﾙｰﾌﾟ設定
/********************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCPnt];
	int*     vv = new int[View->m_MaxCPnt];
	int*     f1 = new int[View->m_MaxCPnt];
	int*     f2 = new int[View->m_MaxCPnt];
	double*  th = new double[View->m_MaxCPnt];
	int      i, i1, i2, j, wn, st, ed, c0, c1, c2, c3, e1, e2, sp, lno;
	double   sth;

	lno = *num;
	while(TRUE) {
		c0 = CheckFacePoint(pn, pa, f1, f2, th);						// 断面分割確認
		if(c0==0) {														// <<<断面分割無>>>
			for(i=0; i<pn-1; i++) {
				PutFlp1(op, lno++, va[i], 0);							// 面ﾙｰﾌﾟ登録
			}
			PutFlp1(op, lno++, va[i], 1);
			break;														// 設定完了
		} else {														// <<<断面分割有>>>
			wn = 0, i1 = 0, i2 = 0, sp = 1, c3 = INT_MAX;
			for(i=0; i<c0; i++) {
				if(f1[i]<f2[i]) {										// 最端最少交線分割線検索
					st = f1[i], ed = f2[i];
				} else {
					st = f2[i], ed = f1[i];
				}
				for(j=0, c1=0, c2=0, e1=0, e2=0; j<c0; j++) {
					if(j!=i) {
						if(f1[j]<st||f1[j]>ed) {
							e1 = 1;
						}
						if(f2[j]<st||f2[j]>ed) {
							c1++;
						}
						if(f1[j]>st&&f1[j]<ed) {
							e2 = 1;
						}
						if(f2[j]>st&&f2[j]<ed) {
							c2++;
						}
						if(e1&&e2) {
							break;
						}
					}
				}
				if((!e1)&&c1<c3) {										// 検索分割線設定
					i1 = i, i2 = 0, c3 = c1, sp = 0;
				}
				if((!e2)&&c2<c3) {
					i1 = i, i2 = 1, c3 = c2, sp = 0;
				}
			}
			if(sp) {													// <<検索ﾃﾞｰﾀ無>>
				for(sth=PI, i=0, i1=0; i<c0; i++) {
					if(sth>th[i]) {										// 基準角度最小検索
						i1 = i, sth = th[i];
					}
				}
				if(f1[i1]<f2[i1]) {										// 最小位置分割線
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				for(wn=0, i=0; i<=st; i++) {							// 分割した面ﾙｰﾌﾟ抽出
					vv[wn++] = va[i];
				}
				for(i=ed; i<pn; i++) {
					vv[wn++] = va[i];
				}
				if(wn>2) {												// <有効ﾙｰﾌﾟの場合>
					for(i=0; i<wn-1; i++) {
						PutFlp1(op, lno++, vv[i], 0);
					}
					PutFlp1(op, lno++, vv[i], 1);
				}
				for(wn=0, i=st; i<=ed; i++) {							// 分割した面ﾙｰﾌﾟ抽出
					vv[wn++] = va[i];
				}
				if(wn>2) {												// <有効ﾙｰﾌﾟの場合>
					for(i=0; i<wn-1; i++) {
						PutFlp1(op, lno++, vv[i], 0);
					}
					PutFlp1(op, lno++, vv[i], 1);
				}
				break;													// 設定完了
			} else {													// <<検索ﾃﾞｰﾀ有>>
				if(f1[i1]<f2[i1]) {										// 検索位置分割線
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				if(!i2) {												// 今回分割面設定
					for(i=0; i<=st; i++) {
						vv[wn++] = va[i];
					}
					for(i=ed; i<pn; i++) {
						vv[wn++] = va[i];
					}
				} else {
					for(i=st; i<=ed; i++) {
						vv[wn++] = va[i];
					}
				}
				if(wn>2) {												// <有効ﾙｰﾌﾟの場合>
					for(i=0; i<wn-1; i++) {
						PutFlp1(op, lno++, vv[i], 0);
					}
					PutFlp1(op, lno++, vv[i], 1);
					if(!i2) {											// 次回分割面設定
						for(wn=0, i=st; i<=ed; i++) {
							pp[wn] = pa[i], vv[wn++] = va[i];
						}
					} else {
						for(wn=0, i=0; i<=st; i++) {
							pp[wn] = pa[i], vv[wn++] = va[i];
						}
						for(i=ed; i<pn; i++) {
							pp[wn] = pa[i], vv[wn++] = va[i];
						}
					}
					for(i=0; i<wn; i++) {								// 次ﾃﾞｰﾀｼﾌﾄ
						pa[i] = pp[i], va[i] = vv[i];
					}
					pn = wn;
				} else {												// <無効分割面の場合>
					for(sth=PI, i=0, i1=0; i<c0; i++) {
						if(sth>th[i]) {									// 基準角度最小検索
							i1 = i, sth = th[i];
						}
					}
					if(f1[i1]<f2[i1]) {									// 最小位置分割線
						st = f1[i1], ed = f2[i1];
					} else {
						st = f2[i1], ed = f1[i1];
					}
					for(wn=0, i=0; i<=st; i++) {						// 分割した面ﾙｰﾌﾟ抽出
						vv[wn++] = va[i];
					}
					for(i=ed; i<pn; i++) {
						vv[wn++] = va[i];
					}
					if(wn>2) {											// <有効ﾙｰﾌﾟの場合>
						for(i=0; i<wn-1; i++) {
							PutFlp1(op, lno++, vv[i], 0);
						}
						PutFlp1(op, lno++, vv[i], 1);
					}
					for(wn=0, i=st; i<=ed; i++) {						// 分割した面ﾙｰﾌﾟ抽出
						vv[wn++] = va[i];
					}
					if(wn>2) {											// <有効ﾙｰﾌﾟの場合>
						for(i=0; i<wn-1; i++) {
							PutFlp1(op, lno++, vv[i], 0);
						}
						PutFlp1(op, lno++, vv[i], 1);
					}
					break;												// 設定完了
				}
			}
		}
	}
	*num = lno;
	delete[] pp;
	delete[] vv;
	delete[] f1;
	delete[] f2;
	delete[] th;
}

/*************************************************************/
static void SetFacS(int* lnum, OBJTYPE* op, int ub, int vb,
                    int un, int vn, PNTTYPE* ps, PNTTYPE* pe)			//<<< 断面面ﾙｰﾌﾟ登録
/*************************************************************/
{
	int* va = new int[View->m_MaxCPnt];
	int  i, j=0;

	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	if(ub&&(!vb)) {														// <<<U閉&V開の場合>>>
		for(i=un-1; i>=0; i--) {										// U開始断面頂点番号抽出
			va[j++] = i;
		}
		SetSectionFace(lnum, op, un, ps, va);							// U開始断面頂点ﾙｰﾌﾟ設定
		for(i=0; i<un; i++) {											// U終了断面頂点番号抽出
			va[i] = i+un*(vn-1);
		}
		SetSectionFace(lnum, op, un, pe, va);							// U終了断面頂点ﾙｰﾌﾟ設定
	} else if((!ub)&&vb) {												// <<<U開&V閉の場合>>>
		for(i=0; i<vn; i++) {											// V開始断面頂点番号抽出
			va[i] = i*un;
		}
		SetSectionFace(lnum, op, vn, ps, va);							// V開始断面頂点ﾙｰﾌﾟ設定
		for(i=vn-1; i>=0; i--) {										// V終了断面頂点番号抽出
			va[j++] = i*un+un-1;
		}
		SetSectionFace(lnum, op, vn, pe, va);							// V終了断面頂点ﾙｰﾌﾟ設定
	}
	delete[] va;
}

/**************************************************************/
static void SetFacUS(int* lnum, OBJTYPE* op, int ufg,
                     int un, int vn, PNTTYPE* ps, PNTTYPE* pe)			//<<< 断面面ﾙｰﾌﾟ登録(U)
/**************************************************************/
{
	int* va = new int[View->m_MaxCPnt];
	int  i, j=0;

	un--;																// U閉は始点/終点同一
	if(ufg==1) {														// <<<先頭Uﾙｰﾌﾟ同一>>>
		for(i=un-1; i>=0; i--) {										// U終了断面頂点番号抽出
			va[j++] = un*(vn-2)+i+1;
		}
		SetSectionFace(lnum, op, un, pe, va);							// U終了断面頂点ﾙｰﾌﾟ設定
	} else {															// <<<最終Uﾙｰﾌﾟ同一>>>
		for(i=0; i<un; i++) {											// U開始断面頂点番号抽出
			va[i] = i;
		}
		SetSectionFace(lnum, op, un, ps, va);							// U開始断面頂点ﾙｰﾌﾟ設定
	}
	delete[] va;
}

/**************************************************************/
static void SetFacVS(int* lnum, OBJTYPE* op, int vfg,
                     int un, int vn, PNTTYPE* ps, PNTTYPE* pe)			//<<< 断面面ﾙｰﾌﾟ登録(V)
/**************************************************************/
{
	int* va = new int[View->m_MaxCPnt];
	int  i, j=0, ms;

	vn--;																// V閉は始点/終点同一
	if(vfg==1) {														// <<<先頭Vﾙｰﾌﾟ同一>>>
		for(i=vn-1; i>=0; i--) {										// V終了断面頂点番号抽出
			va[j++] = (un-1)*(i+1);
		}
		SetSectionFace(lnum, op, vn, pe, va);							// V終了断面頂点ﾙｰﾌﾟ設定
	} else {															// <<<最終Vﾙｰﾌﾟ同一>>>
		for(i=0; i<vn; i++) {
			ms = (i<2) ? 0 : i-1;
			va[i] = i*un-ms;											// V開始断面頂点番号抽出
		}
		SetSectionFace(lnum, op, vn, ps, va);							// V開始断面頂点ﾙｰﾌﾟ設定
	}
	delete[] va;
}

/***********************************************************/
static void SetFac2(OBJTYPE* op, int ub, int vb,
                    int un, int vn, int uo, int vo, int mx)				//<<< 曲面面ﾙｰﾌﾟ2登録
/***********************************************************/
{
	int i, j, div, lno=0;

	for(i=0, div=1; i<mx; i++) {										// 分割挿入数計算
		div = div*2;
	}
	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	if(ub) {															// <<U閉の場合>>
		for(i=0; i<vo; i++) {
			for(j=0; j<un; j++) {
				PutFlp2(op, lno++, div*un*i+j, 0);
			}
			PutFlp2(op, lno++, div*un*i, 1);
		}
	} else {															// <<U開の場合>>
		for(i=0; i<vo; i++) {
			for(j=0; j<un-1; j++) {
				PutFlp2(op, lno++, div*un*i+j, 0);
			}
			PutFlp2(op, lno++, div*un*i+j, 1);
		}
	}
	if(vb) {															// <<V閉の場合>>
		for(i=0; i<uo; i++) {
			for(j=0; j<vn; j++) {
				PutFlp2(op, lno++, i*div+un*j, 0);
			}
			PutFlp2(op, lno++, i*div, 1);
		}
	} else {															// <<V開の場合>>
		for(i=0; i<uo; i++) {
			for(j=0; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*j, 0);
			}
			PutFlp2(op, lno++, i*div+un*j, 1);
		}
	}
}

/************************************************************/
static void SetFacU2(OBJTYPE* op, int flg, int ub,
                     int un, int vn, int uo, int vo, int mx)			//<<< 面ﾙｰﾌﾟ2登録(U同一)
/************************************************************/
{
	int i, j, div, lno=0;

	for(i=0, div=1; i<mx; i++) {										// 分割挿入数計算
		div = div*2;
	}
	if(ub) {															// U閉は始点/終点同一
		un--;
	}
	if(flg==1) {														// <<先頭Uﾙｰﾌﾟ同一>>
		if(ub) {														// <U閉の場合>
			for(i=1; i<vo; i++) {
				for(j=0; j<un; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+1, 1);
			}
		} else {														// <U開の場合>
			for(i=1; i<vo; i++) {
				for(j=0; j<un-1; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+j+1, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno++, 0, 0);
			for(j=1; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*(j-1)+1, 0);
			}
			PutFlp2(op, lno++, i*div+un*(j-1)+1, 1);
		}
	} else if(flg==2) {													// <<最終Uﾙｰﾌﾟ同一>>
		if(ub) {														// <U閉の場合>
			for(i=0; i<vo-1; i++) {
				for(j=0; j<un; j++) {
					PutFlp2(op, lno++, un*div*i+j, 0);
				}
				PutFlp2(op, lno++, un*div*i, 1);
			}
		} else {														// <U開の場合>
			for(i=0; i<vo-1; i++) {
				for(j=0; j<un-1; j++) {
					PutFlp2(op, lno++, un*div*i+j, 0);
				}
				PutFlp2(op, lno++, un*div*i+j, 1);
			}
		}
		for(i=0; i<uo; i++) {
			for(j=0; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*j, 0);
			}
			PutFlp2(op, lno++, un*(vn-1), 1);
		}
	} else {															// <<先頭/最終同一>>
		if(ub) {														// <U閉の場合>
			for(i=1; i<vo-1; i++) {
				for(j=0; j<un; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+1, 1);
			}
		} else {														// <U開の場合>
			for(i=1; i<vo-1; i++) {
				for(j=0; j<un-1; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+j+1, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno++, 0, 0);
			for(j=1; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*(j-1)+1, 0);
			}
			PutFlp2(op, lno++, un*(vn-2)+1, 1);
		}
	}
}

/************************************************************/
static void SetFacV2(OBJTYPE* op, int flg, int vb,
                     int un, int vn, int uo, int vo, int mx)			//<<< 面ﾙｰﾌﾟ2登録(V同一)
/************************************************************/
{
	int i, j, div, ms, lno=0;

	for(i=0, div=1; i<mx; i++) {										// 分割挿入数計算
		div = div*2;
	}
	if(vb) {															// V閉は始点/終点同一
		vn--;
	}
	if(flg==1) {														// <先頭Vﾙｰﾌﾟ同一>
		for(i=0; i<vo; i++) {
			PutFlp2(op, lno++, 0, 0);									// 面ﾙｰﾌﾟ登録
			for(j=1; j<un-1; j++) {
				PutFlp2(op, lno++, (un-1)*div*i+j, 0);
			}
			PutFlp2(op, lno++, (un-1)*div*i+j, 1);
		}
		if(vb) {														// <V閉の場合>
			for(i=1; i<uo; i++) {
				for(j=0; j<vn; j++) {
					PutFlp2(op, lno++, i*div+(un-1)*j, 0);
				}
				PutFlp2(op, lno++, i*div, 1);
			}
		} else {														// <V開の場合>
			for(i=1; i<uo; i++) {
				for(j=0; j<vn-1; j++) {
					PutFlp2(op, lno++, i*div+(un-1)*j, 0);
				}
				PutFlp2(op, lno++, i*div+(un-1)*j, 1);
			}
		}
	} else if(flg==2) {													// <最終Vﾙｰﾌﾟ同一>
		for(i=0; i<vo; i++) {
			ms = (div*i<2) ? 0 : div*i-1;								// 補正(-)数設定
			for(j=0; j<un-1; j++) {
				PutFlp2(op, lno++, un*div*i+j-ms, 0);
			}
			PutFlp2(op, lno++, un-1, 1);
		}
		if(vb) {														// <V閉の場合>
			for(i=0; i<uo-1; i++) {
				for(j=0; j<vn; j++) {
					ms = (j<2) ? 0 : j-1;
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				PutFlp2(op, lno++, i*div, 1);
			}
		} else {														// <V開の場合>
			for(i=0; i<uo-1; i++) {
				for(j=0; j<vn-1; j++) {
					ms = (j<2) ? 0 : j-1;
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				ms = (j<2) ? 0 : j-1;
				PutFlp2(op, lno++, i*div+un*j-ms, 1);
			}
		}
	} else {															// <先頭/最終同一>
		for(i=0; i<vo; i++) {
			if(div*i==0) {												// 補正(-)数設定
				ms = 0;
			} else if(div*i==1) {
				ms = 1;
			} else {
				ms = 2*div*i-1;
			}
			PutFlp2(op, lno++, 0, 0);
			for(j=1; j<un-1; j++) {
				PutFlp2(op, lno++, un*div*i+j-ms, 0);
			}
			PutFlp2(op, lno++, un-1, 1);
		}
		if(vb) {														// <V閉の場合>
			for(i=1; i<uo-1; i++) {
				for(j=0; j<vn; j++) {
					if(j==0) {
						ms = 0;
					} else if(j==1) {
						ms = 1;
					} else {
						ms = 2*j-1;
					}
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				PutFlp2(op, lno++, i*div, 1);
			}
		} else {														// <V開の場合>
			for(i=1; i<uo-1; i++) {
				for(j=0; j<vn-1; j++) {
					if(j==0) {
						ms = 0;
					} else if(j==1) {
						ms = 1;
					} else {
						ms = 2*j-1;
					}
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				if(j==0) {
					ms = 0;
				} else if(j==1) {
					ms = 1;
				} else {
					ms = 2*j-1;
				}
				PutFlp2(op, lno++, i*div+un*j-ms, 1);
			}
		}
	}
}

/***********************************************************/
static void SCurve(int* pn, int bc, double wt, PNTTYPE* pa)				//<<< 連続曲線生成
/***********************************************************/
{
	PNTTYPE* rp = new PNTTYPE[View->m_MaxCPnt];
	int      rn, i, j;

	if(*pn<2) {															// 制御点数2未満は中止
		delete[] rp;
		return;
	}
	rn = *pn;
	if(rn>2) {															// <制御点数3以上>
		for(j=0; j<DIM; j++) {
			rp[1].p[j] = (pa[0].p[j]+pa[1].p[j])/2.0;					// 中間点先頭曲線計算
		}
		for(i=0; i<rn-2; i++) {
			for(j=0; j<DIM; j++) {										// 先頭以外曲線計算
				rp[2*i+3].p[j] = (pa[i+1].p[j]+pa[i+2].p[j])/2.0;
				rp[2*i+2].p[j] = (rp[2*i+1].p[j]+rp[2*i+3].p[j]+
                                  wt*pa[i+1].p[j])/(wt+2.0);
			}
		}
		if(bc) {														// <境界が閉の場合>
			for(j=0; j<DIM; j++) {										// 始点曲線計算
				rp[0].p[j] = (rp[1].p[j]+rp[2*(rn-1)-1].p[j]+
                              wt*pa[0].p[j])/(wt+2.0);
			}
			rp[2*(rn-1)] = rp[0];										// 終点は始点と同一
		} else {														// <境界が開の場合>
			rp[0] = pa[0];												// 始点/終点変更無
			rp[2*(rn-1)] = pa[rn-1];
		}
	} else {															// <制御点数2以下>
		for(i=0; i<rn-1; i++) {
			rp[2*i] = pa[i];
			for(j=0; j<DIM; j++) {										// 中間点計算
				rp[2*i+1].p[j] = (pa[i].p[j]+pa[i+1].p[j])/2.0;
			}
		}
		rp[2*(rn-1)] = pa[rn-1];
	}
	*pn = 2*rn-1;														// 新制御点置換
	for(i=0; i<*pn; i++) {
		pa[i] = rp[i];
	}
	delete[] rp;
}

/****************************************************/
static void SCurves(int flg, int* pn, int bc,
                    double wt, PNTTYPE* pa, int* ea)					//<<< S曲線の生成
/****************************************************/
{
	PNTTYPE* wpa = new PNTTYPE[View->m_MaxCPnt];
	int*     wea = new int[View->m_MaxCPnt];
	PNTTYPE  spa;
	int      sea, cn, en, wn, sn, nn, i, flg1, flg2;

	if(*pn<2) {															// 制御点数2未満中止
		delete[] wpa;
		delete[] wea;
		return;
	}
	for(i=0, en=0; i<*pn; i++) {
		if(ea[i]==flg||ea[i]==3) {										// 折線点数確認
			en++;
		}
	}
	cn = *pn;
	if(en==0) {															// <<折線点無の場合>>
		SCurve(&cn, bc, wt, pa);										// 連続曲線生成
		wea[0] = ea[0];													// 先頭曲線ﾓｰﾄﾞ設定
		for(i=1; i<*pn; i++) {
			wea[2*i-1] = (ea[i-1]>ea[i]) ? ea[i] : ea[i-1];				// 挿入点は前後の小を設定
			wea[2*i] = ea[i];											// 設定済点不変更
		}
		for(i=0; i<cn; i++) {											// 作業領域より正規複写
			ea[i] = wea[i];
		}
	} else {															// <<折線点有の場合>>
		wn = 0, sn = 0, en = 0;
		if(bc&&ea[0]!=flg&&ea[0]!=3) {									// <境界が閉&折線点無>
			cn--;
			while(1) {													// 先頭が折線点まで繰返す
				if(ea[0]==flg||ea[0]==3) {
					break;
				}
				spa = pa[0], sea = ea[0];								// 先頭ﾃﾞｰﾀ保存
				for(i=0; i<cn-1; i++) {									// 1つ前にｼﾌﾄ
					pa[i] = pa[i+1], ea[i] = ea[i+1];
				}
				pa[cn-1] = spa,	ea[cn-1] = sea;							// 最後に旧先頭設定
				en++;
			}
			pa[cn] = pa[0], ea[cn++] = ea[0];							// 先頭追加
			en = 2*en;
		}
		while(1) {
			for(i=sn, nn=0; i<cn; i++) {								// 先頭から順次行う
				wpa[wn+(nn++)] = pa[i];
				if(i!=sn&&(ea[i]==flg||ea[i]==3)) {						// 2点の折線点間制御点抽出
					sn = i; break;
				}
			}
			if(i==cn) {
				break;
			}
			SCurve(&nn, 0, wt, &wpa[wn]);								// 連続曲線生成
			wn = wn+nn-1;												// 折線点は前後で共通使用
		}
		cn = wn+1;
		wea[0] = ea[0];													// 先頭曲線ﾓｰﾄﾞ設定
		flg1 = (ea[0]==flg||ea[0]==3) ? ea[0]-flg : ea[0];				// 折線は該当方向ﾓｰﾄﾞ減
		for(i=1; i<*pn; i++) {
			flg2 = (ea[i]==flg||ea[i]==3) ? ea[i]-flg : ea[i];			// 折線は該当方向ﾓｰﾄﾞ減
			wea[2*i-1] = (flg1>flg2) ? flg2 : flg1;						// 挿入点は前後の小を設定
			wea[2*i] = ea[i];											// 設定済点不変更
			flg1 = flg2;												// 今回後点を次回前点
		}
		if(en==0) {
			for(i=0; i<cn; i++) {										// 作業領域より正規複写
				pa[i] = wpa[i], ea[i] = wea[i];
			}
		} else {
			for(i=0; i<cn-1; i++) {
				if(i+en<cn) {
					pa[i+en] = wpa[i], ea[i+en] = wea[i];
				} else {
					pa[i+en-cn+1] = wpa[i], ea[i+en-cn+1] = wea[i];
				}
			}
			pa[0] = pa[cn-1], ea[0] = ea[cn-1];
		}
	}
	*pn = cn;
	delete[] wpa;
	delete[] wea;
}

/*****************************************************************/
static void MakeSSurface(int* un, int* vn, int ub, int vb,
                         int mx, double wt, PNTTYPE* pa, int* ea)		//<<< S曲面生成
/*****************************************************************/
{
	PNTTYPE* t1 = new PNTTYPE[View->m_MaxSPnt];
	int*     e1 = new int[View->m_MaxSPnt];
	int      uu, vv, pn, i, j, k, m, u, v, s, uv;
	double   w1, w2;

	uu = *un, vv = *vn;													// U/V制御点数
	for(i=0; i<mx; i++) {												// 分割数分繰返す
		for(j=0, s=0; j<vv; j++) {										// <<U方向S曲線化>>
			for(m=0; m<uu; m++) {										// U1行分ﾃﾞｰﾀ抽出
				t1[s+m] = pa[uu*j+m], e1[s+m] = ea[uu*j+m];
			}
			u = uu;
			SCurves(1, &u, ub, wt, &t1[s], &e1[s]);						// S曲線生成
			s = u*(j+1);
		}
		for(j=0; j<vv; j++) {
			for(m=0; m<u; m++) {										// U方向S曲線化結果記憶
				pa[2*u*j+m] = t1[u*j+m], ea[2*u*j+m] = e1[u*j+m];
			}
		}
		for(j=0, s=0; j<u; j++) {										// <<V方向S曲線化>>
			for(m=0; m<vv; m++) {										// V1行分ﾃﾞｰﾀ抽出
				t1[s+m] = pa[2*u*m+j], e1[s+m] = ea[2*u*m+j];
			}
			v = vv;
			SCurves(2, &v, vb, wt, &t1[s], &e1[s]);						// S曲線生成
			s = v*(j+1);
		}
		for(j=0; j<u; j++) {
			for(m=0; m<v; m++) {										// V方向S曲線化結果記憶
				pa[u*m+j] = t1[v*j+m], ea[u*m+j] = e1[v*j+m];
			}
		}
		uu = u, vv = v;													// 新U/V制御点数
	}
	pn = uu*vv;
	for(j=0; j<pn; j++) {												// 作業ﾜｰｸ複写
		t1[j] = pa[j], e1[j] = ea[j];
	}
	for(i=1; i<vv-1; i++) {												// <<中間点収束計算>>
		for(j=1; j<uu-1; j++) {
			uv = i*uu+j;
			if(e1[uv]==0) {												// <U&V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu-1].p[k]+t1[uv-uu+1].p[k]+
                         t1[uv+uu-1].p[k]+t1[uv+uu+1].p[k];
					w2 = t1[uv-uu].p[k]+t1[uv-1].p[k]+
                         t1[uv+1].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                                   t1[uv].p[k])/((wt+4.0)*(wt+4.0));
				}
			} else if(e1[uv]==1) {										// <V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			} else if(e1[uv]==2) {										// <U曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
		}
	}
	if(ub) {															// <<U両端収束計算>>
		for(i=1; i<vv-1; i++) {											// <U閉の場合>
			uv = i*uu;
			if(e1[uv]==0) {												// <U&V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-2].p[k]+t1[uv-uu+1].p[k]+
                         t1[2*uu+uv-2].p[k]+t1[uv+uu+1].p[k];
					w2 = t1[uv-uu].p[k]+t1[uv+uu-2].p[k]+
                         t1[uv+1].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                                   t1[uv].p[k])/((wt+4.0)*(wt+4.0));
					pa[uv+uu-1].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==1) {										// <V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[uv+uu-1].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==2) {										// <U曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv+uu-2].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[uv+uu-1].p[k] = pa[uv].p[k];
				}
			}
		}
	} else {															// <U開の場合>
		for(i=1; i<vv-1; i++) {
			uv = i*uu;
			if(e1[uv]==0||e1[uv]==1) {									// <V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
			uv = (i+1)*uu-1;
			if(e1[uv]==0||e1[uv]==1) {									// <V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
		}
		if(vb) {														// <V閉の場合>
			uv = 0;
			if(e1[uv]==0||e1[uv]==1) {									// <V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-2)*uu].p[k]+t1[uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv-1)*uu].p[k] = pa[uv].p[k];
				}
			}
			uv = uu-1;
			if(e1[uv]==0||e1[uv]==1) {									// <V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-1)*uu-1].p[k]+t1[2*uu-1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv*uu)-1].p[k] = pa[uv].p[k];
				}
			}
		}
	}
	if(vb) {															// <<V両端収束計算>>
		for(i=1; i<uu-1; i++) {											// <V閉の場合>
			uv = i;
			if(e1[uv]==0) {												// <U&V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-2)*uu+uv-1].p[k]+t1[(vv-2)*uu+uv+1].p[k]+
                         t1[uu+uv-1].p[k]+t1[uu+uv+1].p[k];
					w2 = t1[(vv-2)*uu+uv].p[k]+t1[uv-1].p[k]+
                         t1[uv+1].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                                   t1[uv].p[k])/((wt+4.0)*(wt+4.0));
					pa[(vv-1)*uu+uv].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==1) {										// <V曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-2)*uu+uv].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv-1)*uu+uv].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==2) {										// <U曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv-1)*uu+uv].p[k] = pa[uv].p[k];
				}
			}
		}
	} else {															// <V開の場合>
		for(i=1; i<uu-1; i++) {
			uv = i;
			if(e1[uv]==0||e1[uv]==2) {									// <U曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
			uv = (vv-1)*uu+i;
			if(e1[uv]==0||e1[uv]==2) {									// <U曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
		}
		if(ub) {														// <U閉の場合>
			uv = 0;
			if(e1[uv]==0||e1[uv]==2) {									// <U曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[uu-2].p[k]+t1[1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[uu-1].p[k] = pa[uv].p[k];
				}
			}
			uv = (vv-1)*uu;
			if(e1[uv]==0||e1[uv]==2) {									// <U曲線点>
				for(k=0; k<DIM; k++) {
					w1 = t1[vv*uu-2].p[k]+t1[(vv-1)*uu+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[vv*uu-1].p[k] = pa[uv].p[k];
				}
			}
		}
	}
	if(ub&&vb) {														// <U閉V閉の場合>
		if(e1[0]==0) {													// <U&V曲線点>
			for(k=0; k<DIM; k++) {
				w1 = t1[(vv-1)*uu-2].p[k]+t1[(vv-2)*uu+1].p[k]+
                     t1[2*uu-2].p[k]+t1[uu+1].p[k];
				w2 = t1[(vv-2)*uu].p[k]+t1[uu-2].p[k]+t1[1].p[k]+t1[uu].p[k];
				pa[0].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                              t1[0].p[k])/((wt+4.0)*(wt+4.0));
				pa[uu-1].p[k] = pa[0].p[k];
				pa[(vv-1)*uu].p[k] = pa[0].p[k];
				pa[vv*uu-1].p[k] = pa[0].p[k];
			}
		} else if(e1[0]==1) {											// <V曲線点>
			for(k=0; k<DIM; k++) {
				w1 = t1[(vv-2)*uu].p[k]+t1[uu].p[k];
				pa[0].p[k] = (w1+(wt+2.0)*t1[0].p[k])/(wt+4.0);
				pa[uu-1].p[k] = pa[0].p[k];
				pa[(vv-1)*uu].p[k] = pa[0].p[k];
				pa[vv*uu-1].p[k] = pa[0].p[k];
			}
		} else if(e1[0]==2) {											// <U曲線点>
			for(k=0; k<DIM; k++) {
				w1 = t1[uu-2].p[k]+t1[1].p[k];
				pa[0].p[k] = (w1+(wt+2.0)*t1[0].p[k])/(wt+4.0);
				pa[uu-1].p[k] = pa[0].p[k];
				pa[(vv-1)*uu].p[k] = pa[0].p[k];
				pa[vv*uu-1].p[k] = pa[0].p[k];
			}
		}
	}
	*un = uu;
	*vn = vv;
	delete[] t1;
	delete[] e1;
}

/**************************/
OBJTYPE* MakeSurface(void)												//<<< 曲面生成ｺﾏﾝﾄﾞ
/**************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc, un, vn, uo, vo, ch, uu, vv, ub, vb, mx, wt;
	int      vnum=0, lnum=0, lnum2=0, sflg, uflg=0, vflg=0;
	double   dwt;

	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	un = GetParaInt(cmp, 0);											// U制御点数取得
	vn = GetParaInt(cmp, 1);											// V制御点数取得
	ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	mx = GetParaInt(cmp, 4);											// 曲線化繰返数
	if(GetCurveReptSW()) {												// <曲面細分化オン>
		mx = GetCurveReptNum();											// 曲面分割数取得
	}
	wt = GetParaInt(cmp, 5);											// 曲線重み係数
	sflg = GetParaInt(cmp, 6);											// ｿﾘｯﾄﾞﾓｰﾄﾞ
	dwt = wt/1000.0;													// 重み係数実数変換
	uo = un, vo = vn;													// U/V制御点数保存
	for(j=0, pc=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			ea[pc] = GetParaInt(cmp, un*j+i+7);							// ﾓｰﾄﾞ取得
			if(ea[pc]>3) {												// ﾓｰﾄﾞ異常の場合
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U両端は折線
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V両端は折線
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V開の四隅
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+i+7, ea[pc]);						// ﾓｰﾄﾞ変更
			}
			GetParaPnt(cmp, un*j+i, &pa[pc++]);							// 制御点取得
		}
		if(ub) {														// <U閉の場合>
			ea[pc] = GetParaInt(cmp, un*j+7);							// ﾓｰﾄﾞ取得
			if(ea[pc]>3) {												// ﾓｰﾄﾞ異常の場合
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U両端は折線
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V両端は折線
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V開の四隅
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+7, ea[pc]);						// ﾓｰﾄﾞ変更
			}
			GetParaPnt(cmp, un*j, &pa[pc++]);							// 制御点取得
		}
	}
	if(ub) {															// U閉は制御点数UP
		un++;
	}
	if(vb) {															// <V閉の場合>
		for(i=0; i<un; i++) {											// ﾓｰﾄﾞ,制御点取得
			ea[pc] = ea[i], pa[pc++] = pa[i];
		}
		vn++;															// 制御点数UP
	}
	if(un>1&&vn>1) {
		MakeSSurface(&un, &vn, ub, vb, mx, dwt, pa, ea);				// S曲面生成
		if(!vb) {
			uflg = UPointCheck(un, vn, pa);								// U同一制御点確認
		}
		if(!ub) {
			vflg = VPointCheck(un, vn, pa);								// V同一制御点確認
		}
		if(ub&&(!vb)) {													// <U閉&V開の場合>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U断面頂点保存
		} else if((!ub)&&vb) {											// <U開&V閉の場合>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V断面頂点保存
		}
		uu = (ub) ? un-1 : un;											// U閉は頂点数減
		vv = (vb) ? vn-1 : vn;											// V閉は頂点数減
		vnum  = uu*vv;													// 頂点数取得
		lnum  = (un-1)*(vn-1)*4;										// 面ﾙｰﾌﾟ1数取得
		lnum2 = un*vo+vn*uo;											// 面ﾙｰﾌﾟ2数取得
		if(uflg==1||uflg==2) {											// <<U一端同一点収束>>
			vnum -= uu-1, lnum -= un-1, lnum2 -= un;
			if(ub&&sflg) {												// <断面有の場合>
				if(uflg==1) {
					i = CheckSectionPoint(un-1, pe);					// 終了断面頂点分割確認
				} else {
					i = CheckSectionPoint(un-1, ps);					// 開始断面頂点分割確認
				}
				lnum += 2*i+un-1;
			}
		} else if(uflg==3) {											// <<U両端同一点収束>>
			vnum -= 2*(uu-1), lnum -= 2*(un-1), lnum2 -= 2*un;
		} else if(vflg==1||vflg==2) {									// <<V一端同一点収束>>
			vnum -= vv-1, lnum -= vn-1, lnum2 -= vn;
			if(vb&&sflg) {												// <断面有の場合>
				if(vflg==1) {
					i = CheckSectionPoint(vn-1, pe);					// 終了断面頂点分割確認
				} else {
					i = CheckSectionPoint(vn-1, ps);					// 開始断面頂点分割確認
				}
				lnum += 2*i+vn-1;
			}
		} else if(vflg==3) {											// <<V両端同一点収束>>
			vnum -= 2*(vv-1), lnum -= 2*(vn-1), lnum2 -= 2*vn;
		} else {														// <<同一点収束無>>
			if(ub&&(!vb)&&sflg) {										// <U断面有の場合>
				i = CheckSectionPoint(un-1, ps);						// 開始断面頂点分割確認
				j = CheckSectionPoint(un-1, pe);						// 終了断面頂点分割確認
				lnum += 2*(un-1)+2*(i+j);
			} else if((!ub)&&vb&&sflg) {								// <V断面有の場合>
				i = CheckSectionPoint(vn-1, ps);						// 開始断面頂点分割確認
				j = CheckSectionPoint(vn-1, pe);						// 終了断面頂点分割確認
				lnum += 2*(vn-1)+2*(i+j);
			}
		}
	}
	CreatObj(vnum, lnum, lnum2, &op);									// 立体領域確保
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	LevelObj(0, op);													// 同一ﾚﾍﾞﾙ生成立体接続
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		lnum = 0;
		if(uflg==1||uflg==2||uflg==3) {									// <<U端同一点収束>>
			SetVtxU(op, uflg, pa, ub, un, vn);							// 頂点座標登録(U同一)
			SetFacU(&lnum, op, uflg, ub, un, vn);						// 面ﾙｰﾌﾟ1登録(U同一)
			if(ub&&sflg&&uflg!=3) {										// <ｿﾘｯﾄﾞで一端不同一>
				SetFacUS(&lnum, op, uflg, un, vn, ps, pe);				// 一端断面登録
			}
			SetFacU2(op, uflg, ub, un, vn, uo, vo, mx);					// 面ﾙｰﾌﾟ2登録(U同一)
		} else if(vflg==1||vflg==2||vflg==3) {							// <<V端同一点収束>>
			SetVtxV(op, vflg, pa, vb, un, vn);							// 頂点座標登録(V同一)
			SetFacV(&lnum, op, vflg, vb, un, vn);						// 面ﾙｰﾌﾟ1登録(V同一)
			if(vb&&sflg&&vflg!=3) {										// <ｿﾘｯﾄﾞで一端不同一>
				SetFacVS(&lnum, op, vflg, un, vn, ps, pe);				// 一端断面登録
			}
			SetFacV2(op, vflg, vb, un, vn, uo, vo, mx);					// 面ﾙｰﾌﾟ2登録(V同一)
		} else {														// <<同一点収束無>>
			SetVtx(op, pa, ub, vb, un, vn);								// 頂点座標登録
			SetFac(&lnum, op, ub, vb, un, vn);							// 面ﾙｰﾌﾟ1登録
			if(sflg) {													// <ｿﾘｯﾄﾞ>
				SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);				// 断面登録
			}
			SetFac2(op, ub, vb, un, vn, uo, vo, mx);					// 面ﾙｰﾌﾟ2登録
		}
	}
	if((ub&&vb)||(vb&&uflg==3)||(ub&&vflg==3)||(sflg&&(ub||vb))) {		// 属性No設定(SURFACE)
		PutObjAtr(op, 20);
	} else {
		PutObjAtr(op, 21);
	}
	PutObjPrt(op, 0, 0);												// ﾊﾟｰﾂNo設定(通常)
	PutObjCol(op, 0);													// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}

/****************************/
OBJTYPE* ChangeSurface(void)											//<<< 曲面編集ｺﾏﾝﾄﾞ
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc, un, vn, uo, vo, ch, uu, vv, ub, vb, mx, wt;
	int      vnum=0, lnum=0, lnum2=0, sflg, uflg=0, vflg=0;
	double   dwt;

	GetObjPtr(&op);														// 処理対象立体取得
	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	un = GetParaInt(cmp, 0);											// U制御点数取得
	vn = GetParaInt(cmp, 1);											// V制御点数取得
	ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	mx = GetParaInt(cmp, 4);										// 曲線化繰返数
	if(GetCurveReptSW()) {												// <曲面細分化オン>
		mx = GetCurveReptNum();											// 曲面分割数取得
	}
	wt = GetParaInt(cmp, 5);											// 曲線重み係数
	sflg = GetParaInt(cmp, 6);											// ｿﾘｯﾄﾞﾓｰﾄﾞ
	dwt = wt/1000.0;													// 重み係数実数変換
	uo = un, vo = vn;													// U/V制御点数保存
	for(j=0, pc=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			ea[pc] = GetParaInt(cmp, un*j+i+7);							// ﾓｰﾄﾞ取得
			if(ea[pc]>3) {												// ﾓｰﾄﾞ異常の場合
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U両端は折線
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V両端は折線
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V開の四隅
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+i+7, ea[pc]);						// ﾓｰﾄﾞ変更
			}
			GetParaPnt(cmp, un*j+i, &pa[pc++]);							// 制御点取得
		}
		if(ub) {														// <U閉の場合>
			ea[pc] = GetParaInt(cmp, un*j+7);							// ﾓｰﾄﾞ取得
			if(ea[pc]>3) {												// ﾓｰﾄﾞ異常の場合
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U両端は折線
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V両端は折線
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V開の四隅
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+7, ea[pc]);						// ﾓｰﾄﾞ変更
			}
			GetParaPnt(cmp, un*j, &pa[pc++]);							// 制御点取得
		}
	}
	if(ub) {															// U閉は制御点数UP
		un++;
	}
	if(vb) {															// <V閉の場合>
		for(i=0; i<un; i++) {											// ﾓｰﾄﾞ,制御点取得
			ea[pc] = ea[i], pa[pc++] = pa[i];
		}
		vn++;															// 制御点数UP
	}
	if(un>1&&vn>1) {
		MakeSSurface(&un, &vn, ub, vb, mx, dwt, pa, ea);				// S曲面生成
		if(!vb) {
			uflg = UPointCheck(un, vn, pa);								// U同一制御点確認
		}
		if(!ub) {
			vflg = VPointCheck(un, vn, pa);								// V同一制御点確認
		}
		if(ub&&(!vb)) {													// <U閉&V開の場合>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U断面頂点保存
		} else if((!ub)&&vb) {											// <U開&V閉の場合>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V断面頂点保存
		}
		uu = (ub) ? un-1 : un;											// U閉は頂点数減
		vv = (vb) ? vn-1 : vn;											// V閉は頂点数減
		vnum  = uu*vv;													// 頂点数取得
		lnum  = (un-1)*(vn-1)*4;										// 面ﾙｰﾌﾟ1数取得
		lnum2 = un*vo+vn*uo;											// 面ﾙｰﾌﾟ2数取得
		if(uflg==1||uflg==2) {											// <<U一端同一点収束>>
			vnum -= uu-1, lnum -= un-1, lnum2 -= un;
			if(ub&&sflg) {												// <断面有の場合>
				if(uflg==1) {
					i = CheckSectionPoint(un-1, pe);					// 終了断面頂点分割確認
				} else {
					i = CheckSectionPoint(un-1, ps);					// 開始断面頂点分割確認
				}
				lnum += 2*i+un-1;
			}
		} else if(uflg==3) {											// <<U両端同一点収束>>
			vnum -= 2*(uu-1), lnum -= 2*(un-1), lnum2 -= 2*un;
		} else if(vflg==1||vflg==2) {									// <<V一端同一点収束>>
			vnum -= vv-1, lnum -= vn-1, lnum2 -= vn;
			if(vb&&sflg) {												// <断面有の場合>
				if(vflg==1) {
					i = CheckSectionPoint(vn-1, pe);					// 終了断面頂点分割確認
				} else {
					i = CheckSectionPoint(vn-1, ps);					// 開始断面頂点分割確認
				}
				lnum += 2*i+vn-1;
			}
		} else if(vflg==3) {											// <<V両端同一点収束>>
			vnum -= 2*(vv-1), lnum -= 2*(vn-1), lnum2 -= 2*vn;
		} else {														// <<同一点収束無>>
			if(ub&&(!vb)&&sflg) {										// <U断面有の場合>
				i = CheckSectionPoint(un-1, ps);						// 開始断面頂点分割確認
				j = CheckSectionPoint(un-1, pe);						// 終了断面頂点分割確認
				lnum += 2*(un-1)+2*(i+j);
			} else if((!ub)&&vb&&sflg) {								// <V断面有の場合>
				i = CheckSectionPoint(vn-1, ps);						// 開始断面頂点分割確認
				j = CheckSectionPoint(vn-1, pe);						// 終了断面頂点分割確認
				lnum += 2*(vn-1)+2*(i+j);
			}
		}
	}
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	ChangeObj(op, vnum, lnum, lnum2);									// 立体領域変更
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		lnum = 0;
		if(uflg==1||uflg==2||uflg==3) {									// <<U端同一点収束>>
			SetVtxU(op, uflg, pa, ub, un, vn);							// 頂点座標登録(U同一)
			SetFacU(&lnum, op, uflg, ub, un, vn);						// 面ﾙｰﾌﾟ1登録(U同一)
			if(ub&&sflg&&uflg!=3) {										// <ｿﾘｯﾄﾞで一端不同一>
				SetFacUS(&lnum, op, uflg, un, vn, ps, pe);				// 一端断面登録
			}
			SetFacU2(op, uflg, ub, un, vn, uo, vo, mx);					// 面ﾙｰﾌﾟ2登録(U同一)
		} else if(vflg==1||vflg==2||vflg==3) {							// <<V端同一点収束>>
			SetVtxV(op, vflg, pa, vb, un, vn);							// 頂点座標登録(V同一)
			SetFacV(&lnum, op, vflg, vb, un, vn);						// 面ﾙｰﾌﾟ1登録(V同一)
			if(vb&&sflg&&vflg!=3) {										// <ｿﾘｯﾄﾞで一端不同一>
				SetFacVS(&lnum, op, vflg, un, vn, ps, pe);				// 一端断面登録
			}
			SetFacV2(op, vflg, vb, un, vn, uo, vo, mx);					// 面ﾙｰﾌﾟ2登録(V同一)
		} else {														// <<同一点収束無>>
			SetVtx(op, pa, ub, vb, un, vn);								// 頂点座標登録
			SetFac(&lnum, op, ub, vb, un, vn);							// 面ﾙｰﾌﾟ1登録
			if(sflg) {													// <ｿﾘｯﾄﾞ>
				SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);				// 断面登録
			}
			SetFac2(op, ub, vb, un, vn, uo, vo, mx);					// 面ﾙｰﾌﾟ2登録
		}
	}
	if((ub&&vb)||(vb&&uflg==3)||(ub&&vflg==3)||(sflg&&(ub||vb))) {		// 属性No設定(SURFACE)
		PutObjAtr(op, 20);
	} else {
		PutObjAtr(op, 21);
	}
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}

/*************************/
OBJTYPE* SetSurface(void)												//<<< 曲面制御点生成ｺﾏﾝﾄﾞ
/*************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, un, vn, ub, vb, vnum=0, lnum=0, vno=0, lno=0;

	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	un = GetParaInt(cmp, 0);											// U制御点数取得
	vn = GetParaInt(cmp, 1);											// V制御点数取得
	ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	for(j=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			GetParaPnt(cmp, un*j+i, &pa[un*j+i]);						// 制御点取得
		}
	}
	if(un>1&&vn>1) {
		vnum = un*vn;													// 頂点数取得
		if(ub&&vb) {													// 面ﾙｰﾌﾟ数取得
			lnum = 4*un*vn;
		} else if(ub&&(!vb)) {
			lnum = 4*un*(vn-1);
		} else if((!ub)&&vb) {
			lnum = 4*(un-1)*vn;
		} else {
			lnum = 4*(un-1)*(vn-1);
		}
	}
	CreatObj(vnum, lnum, 0, &op);										// 立体領域確保
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	LevelObj(0, op);													// 同一ﾚﾍﾞﾙ生成立体接続
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		for(j=0; j<vn; j++) {
			for(i=0; i<un; i++) {
				PutVtx(op, vno++, pa[un*j+i]);							// 頂点座標設定
			}
		}
		if(ub&&vb) {													// <U閉&V閉の場合>
			for(j=0; j<vn; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// 面ﾙｰﾌﾟ設定
					if(i==un-1) {
						PutFlp1(op, lno++, un*j, 0);
					} else {
						PutFlp1(op, lno++, un*j+i+1, 0);
					}
					if(j==vn-1) {
						if(i==un-1) {
							PutFlp1(op, lno++, 0, 0);
						} else {
							PutFlp1(op, lno++, i+1, 0);
						}
						PutFlp1(op, lno++, i, 1);
					} else {
						if(i==un-1) {
							PutFlp1(op, lno++, un*(j+1), 0);
						} else {
							PutFlp1(op, lno++, un*(j+1)+i+1, 0);
						}
						PutFlp1(op, lno++, un*(j+1)+i, 1);
					}
				}
			}
		} else if(ub&&(!vb)) {											// <U閉&V開の場合>
			for(j=0; j<vn-1; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// 面ﾙｰﾌﾟ設定
					if(i==un-1) {
						PutFlp1(op, lno++, un*j, 0);
						PutFlp1(op, lno++, un*(j+1), 0);
					} else {
						PutFlp1(op, lno++, un*j+i+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					}
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		} else if((!ub)&&vb) {											// <U開&V閉の場合>
			for(j=0; j<vn; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// 面ﾙｰﾌﾟ設定
					PutFlp1(op, lno++, un*j+i+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i, 1);
					}
				}
			}
		} else {														// <U開&V開の場合>
			for(j=0; j<vn-1; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// 面ﾙｰﾌﾟ設定
					PutFlp1(op, lno++, un*j+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		}
	}
	if(ub||vb) {
		PutObjAtr(op, 20);												// 属性No設定(SURFACE)
	} else {
		PutObjAtr(op, 21);
	}
	PutObjPrt(op, 0, 0);												// ﾊﾟｰﾂNo設定(通常)
	PutObjCol(op, 0);													// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
		OnEdtFlg(1, op);												// edtflg:ON
	}
	delete[] pa;
	return op;
}

/***********************************************************/
static void ScaleSection(PNTTYPE* pa, int pn, VECTYPE vec,
                         double th, int d1, int d2, int d3)				//<<< 断面拡大
/***********************************************************/
{
	VECTYPE rv1, rv2, fvc;
	int     i, r1, r2;
	double  rot[3][3], t1, t2, sc;

	r1 = 0, r2 = 0;
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
	sc = fabs(1.0/cos(th/2));											// 断面拡大倍率
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

/********************************************************************/
static void MakeSSweepSurf(int pl, int* un, int* vn, int ub, int vb,
                           int mx, double wt, PNTTYPE* pa, int* ea)		//<<< S掃引曲面生成
/********************************************************************/
{
	PNTTYPE* up = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* vp = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* wup = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* wvp = new PNTTYPE[View->m_MaxCPnt];
	int*     ue = new int[View->m_MaxCPnt];
	int*     ve = new int[View->m_MaxCPnt];
	PNTTYPE  wp[3], pt;
	int      i, j, k, d1, d2, d3, uu, vv, uc, vc;
	VECTYPE  vec, rvc;
	double   rota[3][3], rotb[3][3], th;

	uu = *un, vv = *vn;													// U/V制御点数
	for(uc=0, i=0; i<uu; i++) {
		up[i] = pa[i], ue[i] = ea[i];									// U制御点抽出
		if(ue[i]!=0) {													// U折線点ｶｳﾝﾄ
			uc++;
		}
	}
	wp[0] = up[0], wp[1] = up[1];										// U制御点1,2点目保存
	for(vc=0, i=0; i<vv; i++) {
		vp[i] = pa[i+uu], ve[i] = ea[i+uu];								// V制御点抽出
		if(ve[i]!=0) {													// V折線点ｶｳﾝﾄ
			vc++;
		}
	}
	for(i=0; i<mx; i++) {												// 分割数分繰返す
		SCurves(1, &uu, ub, wt, up, ue);								// U方向S曲線生成
		SCurves(1, &vv, vb, wt, vp, ve);								// V方向S曲線生成
	}
	for(i=0; i<uu; i++) {												// U作業領域複写
		wup[i] = up[i];
	}
	for(i=1; i<uu-1; i++) {
		if(ue[i]!=1) {
			for(j=0; j<DIM; j++) {										// U曲線点収束
				up[i].p[j] = (wup[i-1].p[j]+wup[i+1].p[j]+
                              (wt+2.0)*wup[i].p[j])/(wt+4.0);
			}
		}
	}
	if(ub&&uc==0) {														// <U境界:閉&全曲線の場合>
		for(j=0; j<DIM; j++) {											// 2点目&最終-1点目収束
			up[0].p[j] = (wup[uu-2].p[j]+wup[1].p[j]+
                          (wt+2.0)*wup[0].p[j])/(wt+4.0);
		}
		up[uu-1] = up[0];												// 始点と終点は同一
	}
	for(i=0; i<vv; i++) {
		wvp[i] = vp[i];													// V作業領域複写
	}
	for(i=1; i<vv-1; i++) {
		if(ve[i]!=1) {
			for(j=0; j<DIM; j++) {										// V曲線点収束
				vp[i].p[j] = (wvp[i-1].p[j]+wvp[i+1].p[j]+
                              (wt+2.0)*wvp[i].p[j])/(wt+4.0);
			}
		}
	}
	if(vb&&vc==0) {														// <V境界:閉&全曲線の場合>
		for(j=0; j<DIM; j++) {											// 2点目&最終-1点目収束
			vp[0].p[j] = (wvp[vv-2].p[j]+wvp[1].p[j]+
                          (wt+2.0)*wvp[0].p[j])/(wt+4.0);
		}
		vp[vv-1] = vp[0];												// 始点と終点は同一
	}
	if(pl==-1) {														// <平面指定無に場合(新版)>
		for(i=0; i<DIM; i++) {
			vec.p[i] = wp[1].p[i]-wp[0].p[i];							// ﾍﾞｰｽ線制御点ﾍﾞｸﾄﾙ取得
		}
		VecN(&vec);														// 単位ﾍﾞｸﾄﾙ化
		d1 = 0, d2 = 1, d3 = 2;											// 座標軸設定
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// 基準軸角度分回転
		th = atan2(vec.p[1], vec.p[0]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[2])>EPSILON) {									// <基準平面上でない場合>
			for(i=0; i<vv; i++) {
				VecRotate(&vp[i], rota);
			}
			th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
			if(th<EPSILON) {
				rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
				th = (vec.p[2]<0.0) ? -PI/2.0 : PI/2.0;
			} else {
				rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
				VecN(&rvc);
				th = atan2(vec.p[2], th);
				th = (th<-10||th>10) ? 0.0 : th;
			}
			SetRotateMatrix(rvc, -th, rota);
		}
	} else {															// <平面指定有の場合(旧版)>
		for(i=0; i<DIM; i++) {
			vec.p[i] = up[1].p[i]-up[0].p[i];							// ﾍﾞｰｽ線頂点ﾍﾞｸﾄﾙ取得
		}
		VecN(&vec);														// 単位ﾍﾞｸﾄﾙ化
		d1 = pl;														// 座標軸設定
		d2 = (d1+1>2) ? 0 : d1+1;
		d3 = (d2+1>2) ? 0 : d2+1;
		rvc.p[d1] = 0.0, rvc.p[d2] = 0.0, rvc.p[d3] = -1.0;				// 基準軸(d1)角度分回転
		th = atan2(vec.p[d1], vec.p[d2]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[d3])>EPSILON) {									// <基準平面上でない場合>
			for(i=0; i<vv; i++) {
				VecRotate(&vp[i], rota);
			}
			th = sqrt(vec.p[d1]*vec.p[d1]+vec.p[d2]*vec.p[d2]);
			if(th<EPSILON) {
				rvc.p[d1] = 0.0, rvc.p[d2] = 1.0, rvc.p[d3] = 0.0;
				th = (vec.p[d3]<0.0) ? PI/2.0 : 0.0;
			} else {
				rvc.p[d1] = -vec.p[d2], rvc.p[d2] = vec.p[d1], rvc.p[d3] = 0.0;
				VecN(&rvc);
				th = atan2(vec.p[d3], th);
				th = (th<-10||th>10) ? 0.0 : th;
			}
			SetRotateMatrix(rvc, -th, rota);
		}
	}
	for(i=0; i<vv; i++) {												// <<ﾍﾞｰｽ線始点処理>>
		pt = vp[i];
		VecRotateMove(&pt, rota, up[0]);								// ﾍﾞｰｽ線始点垂直に回転移動
		pa[i*uu] = pt;
		VecRotate(&vp[i], rota);										// 始点垂直に回転
	}
	for(i=1; i<uu-1; i++) {												// <<ﾍﾞｰｽ線中間点処理>>
		CalcRotAngleVec(&up[i-1], &th, &vec);							// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		SetRotateMatrix(vec, th, rotb);									// 次回点用ﾏﾄﾘｯｸｽ
		if(!ue[i]) {													// <掃引方向が曲線点>
			for(j=0; j<vv; j++) {
				pt = vp[j];
				VecRotateMove(&pt, rota, up[i]);						// 前後点中間角度に回転移動
				pa[j*uu+i] = pt;
				VecRotate(&vp[j], rotb);								// 次点垂直に回転
			}
		} else {														// <掃引方向が折線点>
			for(j=0; j<vv; j++) {
				wvp[j] = vp[j];
				VecRotate(&wvp[j], rota);								// 前後点中間角度に回転
				VecRotate(&vp[j], rotb);								// 次点垂直に回転
			}
			ScaleSection(wvp, vv, vec, th, d1, d2, d3);					// 断面拡大
			for(j=0; j<vv; j++) {										// 掃引点に移動
				for(k=0; k<DIM; k++) {
					pa[j*uu+i].p[k] = wvp[j].p[k]+up[i].p[k];
				}
			}
		}
	}
	if(!ub) {															// <<<境界:開の場合>>>
		for(i=0; i<vv; i++) {											// <<ﾍﾞｰｽ線終点処理>>
			pt = vp[i];
			for(j=0; j<DIM; j++) {
				pt.p[j] += up[uu-1].p[j];								// 終点移動
			}
			pa[i*uu+uu-1] = pt;
		}
	} else {															// <<<境界:閉の場合>>>
		wp[0] = up[uu-2], wp[1] = up[0], wp[2] = up[1];					// 終点と始点を連結
		CalcRotAngleVec(wp, &th, &vec);									// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		if(!ue[0]) {													// <掃引方向が曲線点>
			for(i=0; i<vv; i++) {
				pt = vp[i];
				VecRotateMove(&pt, rota, up[0]);
				pa[i*uu] = pt;											// 開始/終了断面
				pa[i*uu+uu-1] = pt;
			}
		} else {														// <掃引方向が折線点>
			for(i=0; i<vv; i++) {
				VecRotate(&vp[i], rota);
			}
			ScaleSection(vp, vv, vec, th, d1, d2, d3);					// 断面拡大
			for(i=0; i<vv; i++) {										// 掃引点に移動
				for(j=0; j<DIM; j++) {									// 開始/終了断面
					pa[i*uu].p[j] = vp[i].p[j]+up[0].p[j];
					pa[i*uu+uu-1].p[j] = vp[i].p[j]+up[0].p[j];
				}
			}
		}
	}
	*un = uu, *vn = vv;
	delete[] up;
	delete[] vp;
	delete[] wup;
	delete[] wvp;
	delete[] ue;
	delete[] ve;
}

/****************************/
OBJTYPE* MakeSweepSurf(void)											//<<< 掃引曲面生成ｺﾏﾝﾄﾞ
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc=0, un, vn, uo, vo, uu, vv;
	int      pl, ub, vb, mx, wt, vnum=0, lnum=0, lnum2=0, sflg;
	double   dwt;

	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	un = GetParaInt(cmp, 0);											// U制御点数取得
	vn = GetParaInt(cmp, 1);											// V制御点数取得
	ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	mx = GetParaInt(cmp, 4);											// 曲線化繰返数
	if(GetCurveReptSW()) {												// <曲面細分化オン>
		mx = GetCurveReptNum();											// 曲面分割数取得
	}
	wt = GetParaInt(cmp, 5);											// 曲線化重み係数
	sflg = GetParaInt(cmp, 6);											// ｿﾘｯﾄﾞﾓｰﾄﾞ
	pl = GetParaInt(cmp, 7);											// U平面取得
	dwt = wt/1000.0;													// 重み係数実数変換
	uo = un, vo = vn;													// U/V制御点数保存
	for(i=0; i<un; i++) {												// <U方向>
		ea[pc] = GetParaInt(cmp, i+8);									// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, i, &pa[pc++]);									// 制御点取得
	}
	if(ub) {															// <U閉の場合>
		ea[pc] = GetParaInt(cmp, 8);									// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, 0, &pa[pc++]);									// 制御点取得
		un++;															// 制御点数UP
	}
	for(i=0; i<vn; i++) {
		ea[pc] = GetParaInt(cmp, i+uo+8);								// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, i+uo, &pa[pc++]);								// 制御点取得
	}
	if(vb) {															// <V閉の場合>
		ea[pc] = GetParaInt(cmp, uo+8);									// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, uo, &pa[pc++]);									// 制御点取得
		vn++;															// 制御点数UP
	}
	if(un>1&&vn>1) {
		MakeSSweepSurf(pl, &un, &vn, ub, vb, mx, dwt, pa, ea);			// S掃引曲面生成
		if(ub&&(!vb)) {													// <U閉&V開の場合>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U断面頂点保存
		} else if((!ub)&&vb) {											// <U開&V閉の場合>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V断面頂点保存
		}
		uu = (ub) ? un-1 : un;											// U閉は頂点数減
		vv = (vb) ? vn-1 : vn;											// V閉は頂点数減
		vnum  = uu*vv;													// 頂点数取得
		lnum  = (un-1)*(vn-1)*4;										// 面ﾙｰﾌﾟ1数取得
		lnum2 = un*vo+vn*uo;											// 面ﾙｰﾌﾟ2数取得
		if(ub&&(!vb)&&sflg) {											// <U断面有の場合>
			i = CheckSectionPoint(un-1, ps);							// 開始断面頂点分割確認
			j = CheckSectionPoint(un-1, pe);							// 終了断面頂点分割確認
			lnum += 2*(un-1)+2*(i+j);
		} else if((!ub)&&vb&&sflg) {									// <V断面有の場合>
			i = CheckSectionPoint(vn-1, ps);							// 開始断面頂点分割確認
			j = CheckSectionPoint(vn-1, pe);							// 終了断面頂点分割確認
			lnum += 2*(vn-1)+2*(i+j);
		}
	}
	CreatObj(vnum, lnum, lnum2, &op);									// 立体領域確保
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	LevelObj(0, op);													// 同一ﾚﾍﾞﾙ生成立体接続
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		lnum = 0;
		SetVtx(op, pa, ub, vb, un, vn);									// 頂点座標登録
		SetFac(&lnum, op, ub, vb, un, vn);								// 面ﾙｰﾌﾟ1登録
		if(sflg) {
			SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);					// 断面登録
		}
		SetFac2(op, ub, vb, un, vn, uo, vo, mx);						// 面ﾙｰﾌﾟ2登録
	}
	if((ub&&vb)||(sflg&&(ub||vb))) {									// 属性No設定(SWEEP)
		PutObjAtr(op, 22);
	} else {
		PutObjAtr(op, 23);
	}
	PutObjPrt(op, 0, 0);												// ﾊﾟｰﾂNo設定(通常)
	PutObjCol(op, 0);													// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}

/******************************/
OBJTYPE* ChangeSweepSurf(void)											//<<< 掃引曲面編集ｺﾏﾝﾄﾞ
/******************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc=0, un, vn, uo, vo, uu, vv, pl;
	int      ub, vb, mx, wt, vnum=0, lnum=0, lnum2=0, sflg;
	double   dwt;

	GetObjPtr(&op);														// 処理対象立体取得
	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	un = GetParaInt(cmp, 0);											// U制御点数取得
	vn = GetParaInt(cmp, 1);											// V制御点数取得
	ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	mx = GetParaInt(cmp, 4);											// 曲線化繰返数
	if(GetCurveReptSW()) {												// <曲面細分化オン>
		mx = GetCurveReptNum();											// 曲面分割数取得
	}
	wt = GetParaInt(cmp, 5);											// 曲線化重み係数
	sflg = GetParaInt(cmp, 6);											// ｿﾘｯﾄﾞﾓｰﾄﾞ
	pl = GetParaInt(cmp, 7);											// U平面取得
	dwt = wt/1000.0;													// 重み係数実数変換
	uo = un, vo = vn;													// U/V制御点数保存
	for(i=0; i<un; i++) {												// <U方向>
		ea[pc] = GetParaInt(cmp, i+8);									// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, i, &pa[pc++]);									// 制御点取得
	}
	if(ub) {															// <U閉の場合>
		ea[pc] = GetParaInt(cmp, 8);									// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, 0, &pa[pc++]);									// 制御点取得
		un++;															// 制御点数UP
	}
	for(i=0; i<vn; i++) {
		ea[pc] = GetParaInt(cmp, i+uo+8);								// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, i+uo, &pa[pc++]);								// 制御点取得
	}
	if(vb) {															// <V閉の場合>
		ea[pc] = GetParaInt(cmp, uo+8);									// ﾓｰﾄﾞ取得
		GetParaPnt(cmp, uo, &pa[pc++]);									// 制御点取得
		vn++;															// 制御点数UP
	}
	if(un>1&&vn>1) {
		MakeSSweepSurf(pl, &un, &vn, ub, vb, mx, dwt, pa, ea);			// S掃引曲面生成
		if(ub&&(!vb)) {													// <U閉&V開の場合>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U断面頂点保存
		} else if((!ub)&&vb) {											// <U開&V閉の場合>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V断面頂点保存
		}
		uu = (ub) ? un-1 : un;											// U閉は頂点数減
		vv = (vb) ? vn-1 : vn;											// V閉は頂点数減
		vnum  = uu*vv;													// 頂点数取得
		lnum  = (un-1)*(vn-1)*4;										// 面ﾙｰﾌﾟ1数取得
		lnum2 = un*vo+vn*uo;											// 面ﾙｰﾌﾟ2数取得
		if(ub&&(!vb)&&sflg) {											// <U断面有の場合>
			i = CheckSectionPoint(un-1, ps);							// 開始断面頂点分割確認
			j = CheckSectionPoint(un-1, pe);							// 終了断面頂点分割確認
			lnum += 2*(un-1)+2*(i+j);
		} else if((!ub)&&vb&&sflg) {									// <V断面有の場合>
			i = CheckSectionPoint(vn-1, ps);							// 開始断面頂点分割確認
			j = CheckSectionPoint(vn-1, pe);							// 終了断面頂点分割確認
			lnum += 2*(vn-1)+2*(i+j);
		}
	}
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	ChangeObj(op, vnum, lnum, lnum2);									// 立体領域変更
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		lnum = 0;
		SetVtx(op, pa, ub, vb, un, vn);									// 頂点座標登録
		SetFac(&lnum, op, ub, vb, un, vn);								// 面ﾙｰﾌﾟ1登録
		if(sflg) {
			SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);					// 断面登録
		}
		SetFac2(op, ub, vb, un, vn, uo, vo, mx);						// 面ﾙｰﾌﾟ2登録
	}
	if((ub&&vb)||(sflg&&(ub||vb))) {									// 属性No設定(SWEEP)
		PutObjAtr(op, 22);
	} else {
		PutObjAtr(op, 23);
	}
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}
