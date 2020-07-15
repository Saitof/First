/////////////////////////////////////////////////////////////////////////////
// Ｓ曲線 制御点<->通過点 変換部
// ConvSCurve.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/05/27 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"

/****************************************************************/
static void MakeSCurveMatrix(double wt, int bc, int pn, int* ea,
                             double* ma, double* mb)					//<<< S曲線変換ﾏﾄﾘｯｸｽ作成
/****************************************************************/
{
	int* ew = new int[View->m_MaxCPnt+1];
	int  i, i1;

	if(pn<1) {															// 制御点数1未満は中止
		delete[] ew;
		return;
	}
	i1 = pn*pn;
	for(i=0; i<i1; i++) {												// ﾏﾄﾘｯｸｽ初期化
		ma[i] = 0.0, mb[i] = 0.0;
	}
	for(i=0; i<pn; i++) {
		ew[i] = ea[i];													// ﾗｲﾝﾓｰﾄﾞ複写
	}
	if(!bc) {															// 境界開は始点終点を折線点
		ew[0] = 1, ew[pn-1] = 1;
	}
	for(i=0; i<pn; i++) {
		if(ew[i]==1) {													// <折線点の場合>
			ma[i*pn+i] = 1.0, mb[i*pn+i] = 1.0;
		} else {														// <曲線点の場合>
			ma[i*pn+i] = wt+2.0, mb[i*pn+i] = wt+4.0;
			if(i==0) {
				ma[1] = 1.0, ma[pn-1] = 1.0;
			} else if(i==pn-1) {
				ma[pn*pn-2] = 1.0, ma[pn*pn-pn] = 1.0;
			} else {
				ma[i*pn+i-1] = 1.0, ma[i*pn+i+1] = 1.0;
			}
		}
	}
	delete[] ew;
}

/************************************************************/
static BOOL CalcSCurveMatrix(int pn, double* ma, double* mb)			//<<< S曲線変換ﾏﾄﾘｯｸｽ計算
/************************************************************/
{
	double* wa = new double[View->m_MaxCCtA];
	double* wb = new double[View->m_MaxCCtA];
	int     i, j, k, l, er;
	double  d;

	for(i=0; i<pn; i++) {
		for(er=1, j=i; j<pn; j++) {										// 該当(n行n列)位置が有効
			d = ma[i*pn+i];												// な行を検索する
			if(fabs(d)>DEPSILON) {										// 0でなければ次処理へ
				er = 0; break;
			}
			for(l=0; l<pn; l++) {
				wa[l] = ma[i*pn+l], wb[l] = mb[i*pn+l];					// 無効ﾃﾞｰﾀ行を保存
			}
			for(k=i; k<pn-1; k++) {
				for(l=0; l<pn; l++) {
					ma[i*pn+l] = ma[(i+1)*pn+l];						// 未検索行を前にｼﾌﾄ
					mb[i*pn+l] = mb[(i+1)*pn+l];
				}
			}
			for(l=0; l<pn; l++) {
				ma[(pn-1)*pn+l] = wa[l], mb[(pn-1)*pn+l] = wb[l];		// 無効ﾃﾞｰﾀ行を末尾行に移動
			}
		}
		if(er) {														// 有効ﾃﾞｰﾀ無はｴﾗｰﾒｯｾｰｼﾞ後終了
			AfxMessageBox(IDS_ERR_CONV);
			delete[] wa;
			delete[] wb;
			return FALSE;
		}
		for(l=0; l<pn; l++) {											// 該当(n行n列)位置の係数で
			ma[i*pn+l] /= d;											// 該当(n)行のﾃﾞｰﾀを割る
			mb[i*pn+l] /= d;											// [該当(n行n列)係数=1.0]
		}
		for(j=0; j<pn; j++) {
			d = ma[j*pn+i];												// 該当(n)行以外の行の
			if((j!=i)&&(fabs(d)>DEPSILON)) {							// 該当(n)列の係数が有効な場合
				for(l=0; l<pn; l++) {									// 該当(n)行以外の行のﾃﾞｰﾀから
					ma[j*pn+l] -= (d*ma[i*pn+l]);						// 係数×該当(n)行のﾃﾞｰﾀを減算
					mb[j*pn+l] -= (d*mb[i*pn+l]);						// [n列のn行以外の行の係数=0.0]
				}
			}
		}
	}
	delete[] wa;
	delete[] wb;
	return TRUE;
}

/*******************************************************/
void SetCurveMatrix1(int md, double wt, int bc, int pn,					//<<< S曲面変換ﾏﾄﾘｯｸｽ設定1
					 int* ea, double* ma, double* mb)					//    (通過点=>制御点)
/*******************************************************/
{
	if(md) {															// <通過点指定の場合>
		MakeSCurveMatrix(wt, bc, pn, ea, ma, mb);						// S曲線変換ﾏﾄﾘｯｸｽ作成
		CalcSCurveMatrix(pn, ma, mb);									// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
	}
}

/*******************************************************/
void SetCurveMatrix2(int md, double wt, int bc, int pn,					//<<< S曲面変換ﾏﾄﾘｯｸｽ設定2
					 int* ea, double* ma, double* mb)					//    (制御点=>通過点)
/*******************************************************/
{
	if(md) {															// <通過点指定の場合>
		MakeSCurveMatrix(wt, bc, pn, ea, ma, mb);						// S曲線変換ﾏﾄﾘｯｸｽ作成
		CalcSCurveMatrix(pn, mb, ma);									// 制御点=>通過点変換ﾏﾄﾘｯｸｽ
	}
}

/*******************************************************/
void SetCurveMatrix3(int md, double wt, int bc, int pn,					//<<< S曲面変換ﾏﾄﾘｯｸｽ設定3
					 int* ea, double* ma, double* mb)					//    (制御点<=>通過点)
/*******************************************************/
{
	double* wa = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double* wb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	int     i, mx;

	if(md) {															// <通過点指定の場合>
		MakeSCurveMatrix(wt, bc, pn, ea, ma, mb);						// S曲線変換ﾏﾄﾘｯｸｽ作成
		mx = pn*pn;
		for(i=0; i<mx; i++) {											// 変換ﾏﾄﾘｯｸｽ複写
			wa[i] = ma[i], wb[i] = mb[i];
		}
		CalcSCurveMatrix(pn, wb, ma);									// 制御点=>通過点変換ﾏﾄﾘｯｸｽ計算
		CalcSCurveMatrix(pn, wa, mb);									// 通過点=>制御点変換ﾏﾄﾘｯｸｽ計算
	}
	delete[] wa;
	delete[] wb;
}

/***********************************************************************/
static void CalcCtrlPoint(int pn, PNTTYPE* pa, PNTTYPE* pb, double* ma)	//<<< 制御点<=>通過点計算ｻﾌﾞ
/***********************************************************************/
{
	int    i, j, k;
	double d[3];

	for(i=0; i<pn; i++) {
		d[0] = 0.0, d[1] = 0.0, d[2] = 0.0;
		for(j=0; j<pn; j++) {
			for(k=0; k<3; k++) {
				d[k] += pa[j].p[k]*ma[i*pn+j];
			}
		}
		pb[i].p[0] = d[0], pb[i].p[1] = d[1], pb[i].p[2] = d[2];
	}
}

/************************************************/
void CalcCurvePoint(int md, int pn, PNTTYPE* pa,						//<<< 制御点<=>通過点計算
					PNTTYPE* pb, double* ma)							//    (曲線変換)
/************************************************/
{
	int i;

	if(md) {															// <通過点指定の場合>
		CalcCtrlPoint(pn, pa, pb, ma);									// 制御点<=>通過点計算
	} else {															// <制御点指定の場合>
		for(i=0; i<pn; i++) {
			pb[i] = pa[i];												// 制御点複写
		}
	}
}

/*************************************************************/
void CalcSurfacePoint(int md, int f1, int f2, int un, int vn,
                      int ub, int vb, double wt, int* ea,				//<<< 制御点<=>通過点計算
                      PNTTYPE* pa, PNTTYPE* pb)							//    (通常曲面変換)
/*************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	int      i, j;

	if(f1==0) {															// <<<通過点=>制御点変換>>>
		for(i=0; i<un; i++) {											// <<V方向の変換>>
			if(i==0||f2==0) {											// <先頭列/編集時変換の場合>
				for(j=0; j<vn; j++) {
					i1[j] = (ea[i+un*j]>1)?1:0;							// V方向曲線ﾓｰﾄﾞ抽出
				}
				SetCurveMatrix1(md, wt, vb, vn, i1, ma, mb);			// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			}
			for(j=0; j<vn; j++) {
				p1[j] = pa[i+un*j];										// V方向制御点抽出
			}
			CalcCurvePoint(md, vn, p1, p2, mb);							// 通過点=>制御点変換
			for(j=0; j<vn; j++) {
				pb[i+un*j] = p2[j];										// V方向制御点設定
			}
		}
		for(i=0; i<vn; i++) {											// <<U方向の変換>>
			if(i==0||f2==0) {											// <先頭列/編集時変換の場合>
				for(j=0; j<un; j++) {
					i1[j] = (ea[i*un+j]%2==1)?1:0;						// U方向曲線ﾓｰﾄﾞ抽出
				}
				SetCurveMatrix1(md, wt, ub, un, i1, ma, mb);			// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			}
			for(j=0; j<un; j++) {
				p1[j] = pb[i*un+j];										// U方向制御点抽出
			}
			CalcCurvePoint(md, un, p1, p2, mb);							// 通過点=>制御点変換
			for(j=0; j<un; j++) {
				pb[i*un+j] = p2[j];										// U方向制御点設定
			}
		}
	} else {															// <<<制御点=>通過点変換>>>
		for(i=0; i<vn; i++) {											// <<U方向の変換>>
			if(i==0||f2==0) {											// <先頭列/編集時変換の場合>
				for(j=0; j<un; j++) {
					i1[j] = (ea[i*un+j]%2==1)?1:0;						// U方向曲線ﾓｰﾄﾞ抽出
				}
				SetCurveMatrix2(md, wt, ub, un, i1, ma, mb);			// 制御点=>通過点変換ﾏﾄﾘｯｸｽ
			}
			for(j=0; j<un; j++) {
				p1[j] = pa[i*un+j];										// U方向制御点抽出
			}
			CalcCurvePoint(md, un, p1, p2, ma);							// 制御点=>通過点変換
			for(j=0; j<un; j++) {
				pb[i*un+j] = p2[j];										// U方向制御点設定
			}
		}
		for(i=0; i<un; i++) {											// <<V方向の変換>>
			if(i==0||f2==0) {											// <先頭列/編集時変換の場合>
				for(j=0; j<vn; j++) {
					i1[j] = (ea[i+un*j]>1)?1:0;							// V方向曲線ﾓｰﾄﾞ抽出
				}
				SetCurveMatrix2(md, wt, vb, vn, i1, ma, mb);			// 制御点=>通過点変換ﾏﾄﾘｯｸｽ
			}
			for(j=0; j<vn; j++) {
				p1[j] = pb[i+un*j];										// V方向制御点抽出
			}
			CalcCurvePoint(md, vn, p1, p2, ma);							// 制御点=>通過点変換
			for(j=0; j<vn; j++) {
				pb[i+un*j] = p2[j];										// V方向制御点設定
			}
		}
	}
	delete[] p1;
	delete[] p2;
	delete[] i1;
	delete[] ma;
	delete[] mb;
}
