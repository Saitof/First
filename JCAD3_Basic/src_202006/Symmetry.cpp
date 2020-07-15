/////////////////////////////////////////////////////////////////////////////
// 対称軸編集操作部
// Symmetry.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "NumFunc.h"

/******************************/
BOOL InputRevolveDivi(int* dn)											//<<< 回転対称分割数取得
/******************************/
{
	CInputDlg dlg(24);
	dlg.SetValue(*dn);													// 既存値表示
	while(TRUE) {
		if(dlg.DoModal()==IDOK) {										// ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(dn);											// 回転対称分割数取得
			if(*dn>View->m_MaxCCtl) {									// <制御点上限以上>
				AfxMessageBox(IDS_ERR_RDIVI);							// 上記以外はｴﾗｰﾒｯｾｰｼﾞ
			} else {
				return TRUE;
			}
		} else {
			return FALSE;
		}
	}
}

/************************************************************/
void SetSymmetryLine(int flg, int mod, PNTTYPE cpt, int pln,
                     int lin, int div, int* dr1, int* dr2)				//<<< 対称軸設定
/************************************************************/
{
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCCtA];
	int*     cl = new int[View->m_MaxCCtA];
	PNTTYPE  pt;
	VECTYPE  vec;
	double   th, rot[3][3];
	int      i, j;

	for(i=0; i<View->m_MaxCCtA; i++) {									// ｶﾗｰ情報設定(共通)
		cl[i] = 0;
	}
	if(mod==1) {														// <<<1軸対称の場合>>>
		if(!lin) {														// <縦方向の場合>
			if(pln==0) {
				*dr1 = 1, *dr2 = 0;
			} else if(pln==1) {
				*dr1 = 1, *dr2 = 2;
			} else {
				*dr1 = 0, *dr2 = 2;
			}
		} else {														// <横方向の場合>
			if(pln==0) {
				*dr1 = 0, *dr2 = 1;
			} else if(pln==1) {
				*dr1 = 2, *dr2 = 1;
			} else {
				*dr1 = 2, *dr2 = 0;
			}
		}
		ps[0] = cpt, ps[0].p[*dr2] -= 2000.0;							// ﾗｲﾝ1始点
		pe[0] = cpt, pe[0].p[*dr2] += 2000.0;							// ﾗｲﾝ1終点
		View->Line.Setup(ps, pe, cl, 1, pln);							// ﾗｲﾝ設定
	} else if(mod==2) {													// <<<2軸対称の場合>>>
		if(pln==0) {
			*dr1 = 0, *dr2 = 1;
		} else if(pln==1) {
			*dr1 = 1, *dr2 = 2;
		} else {
			*dr1 = 2, *dr2 = 0;
		}
		ps[0] = cpt, ps[0].p[*dr1] -= 2000.0;							// ﾗｲﾝ1始点
		pe[0] = cpt, pe[0].p[*dr1] += 2000.0;							// ﾗｲﾝ1終点
		ps[1] = cpt, ps[1].p[*dr2] -= 2000.0;							// ﾗｲﾝ2始点
		pe[1] = cpt, pe[1].p[*dr2] += 2000.0;							// ﾗｲﾝ2終点
		View->Line.Setup(ps, pe, cl, 2, pln);							// ﾗｲﾝ設定
	} else {															// <<<回転対称の場合>>>
		for(i=0; i<DIM; i++) {
			vec.p[i] = 0.0;
		}
		j = (!flg) ? 2 : div;											// ﾗｲﾝ数
		for(i=0; i<j; i++) {
			ps[i] = cpt;												// ﾗｲﾝ始点設定
		}
		if(pln==0) {
			*dr1 = 0, *dr2 = 1, j = 0, vec.p[2] = -1.0;
		} else if(pln==1) {
			*dr1 = 2, *dr2 = 1, j = 1, vec.p[0] = -1.0;
		} else {
			*dr1 = 2, *dr2 = 0, j = 1, vec.p[1] = -1.0;
		}
		th = PI/div;													// 1分割あたりの角度/2
		for(i=0; i<DIM; i++) {
			pe[0].p[i] = 0.0;
		}
		if(j==0) {
			pe[0].p[*dr1] -= 2000.0;
		} else {
			pe[0].p[*dr1] += 2000.0;
		}
		pe[1] = pe[0], pt = pe[0];
		SetRotateMatrix(vec, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		VecRotateMove(&pe[0], rot, cpt);								// ﾗｲﾝ1終点回転移動
		VecRotate(&pt, rot);											// 基準点回転
		SetRotateMatrix(vec, -th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		VecRotateMove(&pe[1], rot, cpt);								// ﾗｲﾝ2終点回転移動
		if(flg&&div>2) {												// <｢曲編｣で3分割以上>
			th = 2.0*PI/div;											// 1分割あたりの角度
			for(i=2; i<div; i++) {
				pe[i] = pt;
				SetRotateMatrix(vec, th*(i-1), rot);					// 回転ﾏﾄﾘｯｸｽ作成
				VecRotateMove(&pe[i], rot, cpt);						// ﾗｲﾝ3以降終点回転移動
			}
			i = div;													// ﾗｲﾝ数
		} else {
			i = 2;
		}
		View->Line.Setup(ps, pe, cl, i, pln);							// ﾗｲﾝ設定
	}
	delete[] ps;
	delete[] pe;
	delete[] cl;
}

/**************************************************************/
BOOL CheckSymmetryArea(int* chk, PNTTYPE pnt, int mod,
                       PNTTYPE cpt, int div, int dr1, int dr2)			//<<< 対称領域ﾁｪｯｸ
/**************************************************************/
{
	BOOL   ret=TRUE;
	int    wch=0, ch1=-1, ch2=-1;
	double th1, th2, l1, l2;

	if(mod==0) {														// 対称指定以外は無条件OK
		return ret;
	} else if(mod==1) {													// <<<1軸対称の場合>>>
		if(pnt.p[dr1]<cpt.p[dr1]) {										// 今回点が負領域
			wch = 1;
		} else if(pnt.p[dr1]>cpt.p[dr1]) {								// 今回点が正領域
			wch = 2;
		}
		if(*chk>0&&wch>0&&*chk!=wch) {									// 既に領域決定済で異領域ﾁｪｯｸ
			ret = FALSE;
		}
		if(*chk==0) {													// 領域未決定は更新
			*chk = wch;
		}
	} else if(mod==2) {													// <<<2軸対称の場合>>>
		if(pnt.p[dr1]<cpt.p[dr1]) {										// 今回点のdr1方向が負領域
			ch1 = 1;
		} else if(pnt.p[dr1]>cpt.p[dr1]) {								// 今回点のdr1方向が正領域
			ch1 = 2;
		}
		if(pnt.p[dr2]<cpt.p[dr2]) {										// 今回点のdr2方向が負領域
			ch2 = 0;
		} else if(pnt.p[dr2]>cpt.p[dr2]) {								// 今回点のdr2方向が正領域
			ch2 = 2;
		}
		if(ch1>0) {
			if(ch2>=0) {												// 対称領域No設定
				wch = ch1 + ch2;
			} else {
				if(ch1==1) {
					wch = -3;
				} else if(ch1==2) {
					wch = -4;
				}
			}
		} else {
			if(ch2==0) {
				wch = -1;
			} else if(ch2==2) {
				wch = -2;
			}
		}
		if(*chk==1) {													// 既に領域決定済で異領域ﾁｪｯｸ
			if(wch!=0&&wch!=1&&wch!=-1&&wch!=-3) {
				ret = FALSE;
			}
		} else if(*chk==2) {
			if(wch!=0&&wch!=2&&wch!=-1&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==3) {
			if(wch!=0&&wch!=3&&wch!=-2&&wch!=-3) {
				ret = FALSE;
			}
		} else if(*chk==4) {
			if(wch!=0&&wch!=4&&wch!=-2&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==-1) {
			if(wch!=0&&wch!=1&&wch!=2&&wch!=-1&&wch!=-3&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==-2) {
			if(wch!=0&&wch!=3&&wch!=4&&wch!=-2&&wch!=-3&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==-3) {
			if(wch!=0&&wch!=1&&wch!=3&&wch!=-1&&wch!=-2&&wch!=-3) {
				ret = FALSE;
			}
		} else if(*chk==-4) {
			if(wch!=0&&wch!=2&&wch!=4&&wch!=-1&&wch!=-2&&wch!=-4) {
				ret = FALSE;
			}
		}
		if((*chk==-1&&wch==-3)||(*chk==-3&&wch==-1)) {					// 両側の境界選択は領域決定
			wch = 1;
		} else if((*chk==-1&&wch==-4)||(*chk==-4&&wch==-1)) {
			wch = 2;
		} else if((*chk==-2&&wch==-3)||(*chk==-3&&wch==-2)) {
			wch = 3;
		} else if((*chk==-2&&wch==-4)||(*chk==-4&&wch==-2)) {
			wch = 4;
		}
		if(*chk==0) {													// 領域未決定は更新
			*chk = wch;
		} else if(*chk<0&&wch!=0&&ret==TRUE) {
			*chk = wch;
		}
	} else if(mod==3) {													// <<<回転対称の場合>>>
		if(fabs(pnt.p[dr1]-cpt.p[dr1])>EPSILON||
			fabs(pnt.p[dr2]-cpt.p[dr2])>EPSILON) {
			th1 = PI/div;												// 対称領域の最大角度/2
			l1 = pnt.p[dr1]-cpt.p[dr1];
			if(dr1==0) {
				l1 = -l1;
			}
			l2 = pnt.p[dr2]-cpt.p[dr2];
			if(dr2==0) {
				l2 = -l2;
			}
			th2 = atan2(l2, l1);										// 今回点の角度
			if(th1<fabs(th2)) {											// 対称領域内ﾁｪｯｸ
				ret = FALSE;
			}
		}
	}
	return ret;
}

/*******************************************************/
static int CopySymmetry1(PNTTYPE cpt, int dr1, int* pn,
                         PNTTYPE* pa, int* ia, int* bc)					//<<< 1軸対称領域複写
/*******************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	int*     i2 = new int[View->m_MaxCCtA];
	int      i, j, n, ct=0, ret=0;
	double   ps, ln1, ln2;

	n = *pn;															// 指定制御点数
	for(i=0; i<n; i++) {
		p1[i] = pa[i], i1[i] = ia[i];									// 制御点情報複写
	}
	ps = cpt.p[dr1];													// 対称軸の座標
	ln1 = fabs(p1[0].p[dr1]-ps);										// 始点と対称軸の距離
	ln2 = fabs(p1[n-1].p[dr1]-ps);										// 終点と対称軸の距離
	for(i=0; i<n; i++) {
		p2[i] = p1[i], p2[i].p[dr1] = ps-(p1[i].p[dr1]-ps);				// 鏡像に制御点を移動
		i2[i] = i1[i];
	}
	if(ln1<EPSILON&&ln2<EPSILON) {										// <始点･終点が対称軸に接する>
		*bc = 1;														// 境界は必ず閉
		for(i=0; i<n; i++) {
			pa[ct] = p1[i], ia[ct++] = i1[i];							// 指定制御点を昇順で設定
		}
		for(i=n-2; i>0; i--) {
			pa[ct] = p2[i], ia[ct++] = i2[i];							// 複写制御点の両端抜きを降順で設定
		}
	} else if(ln1<EPSILON||ln1<ln2) {									// <始点が接する/始点の方が近い>
		for(i=n-1; i>=0; i--) {
			pa[ct] = p1[i], ia[ct++] = i1[i];							// 指定制御点を降順で設定
		}
		j = (ln1==0) ? 1 : 0;											// 接する場合は複写側始点を対象外
		for(i=j; i<n; i++) {
			pa[ct] = p2[i], ia[ct++] = i2[i];							// 複写側制御点を昇順で設定
		}
	} else {															// <終点が接する/終点の方が近い>
		for(i=0; i<n; i++) {
			pa[ct] = p1[i], ia[ct++] = i1[i];							// 指定制御点を昇順で設定
		}
		j = (ln2==0) ? n-2 : n-1;										// 接する場合は複写側終点を対象外
		for(i=j; i>=0; i--) {
			pa[ct] = p2[i], ia[ct++] = i2[i];							// 複写制御点を降順で設定
		}
	}
	*pn = ct;															// 対称複写後の制御点
	if(ln1==0) {														// 始点終点が対称軸に接した数
		ret += 1;
	}
	if(ln2==0) {
		ret += 1;
	}
	delete[] p1;
	delete[] p2;
	delete[] i1;
	delete[] i2;
	return ret;
}

/*****************************************************************/
static void CopySymmetry2(PNTTYPE cpt, int div, int dr1,
                          int dr2, int* pn, PNTTYPE* pa, int* ia)		//<<< 回転対称領域複写
/*****************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	PNTTYPE  pt;
	VECTYPE  vec;
	int      i, j, k, n, fg1, fg2, ct=0;
	double   rot[3][3], th, th1, th2, l1, l2;

	n = *pn;															// 指定制御点数
	th = PI/div;														// 対称領域最大角度/2(th)
	if(fabs(pa[0].p[dr1]-cpt.p[dr1])>EPSILON||
       fabs(pa[0].p[dr2]-cpt.p[dr2])>EPSILON) {
		l1 = (dr1==0) ? -(pa[0].p[dr1]-cpt.p[dr1]) :
                        pa[0].p[dr1]-cpt.p[dr1];
		l2 = (dr2==0) ? -(pa[0].p[dr2]-cpt.p[dr2]) :
                        pa[0].p[dr2]-cpt.p[dr2];
		th1 = atan2(l2, l1);											// 始点の角度(th1)
	} else {
		th1 = 0.0;
	}
	if(fabs(pa[n-1].p[dr1]-cpt.p[dr1])>EPSILON||
       fabs(pa[n-1].p[dr2]-cpt.p[dr2])>EPSILON) {
		l1 = (dr1==0) ? -(pa[n-1].p[dr1]-cpt.p[dr1]) :
                        pa[n-1].p[dr1]-cpt.p[dr1];
		l2 = (dr2==0) ? -(pa[n-1].p[dr2]-cpt.p[dr2]) :
                        pa[n-1].p[dr2]-cpt.p[dr2];
		th2 = atan2(l2, l1);											// 終点の角度(th2)
	} else {
		th2 = 0.0;
	}
	if(th+th1<th-th1) {													// 始点角度が-th度に近い
		l1 = th+th1, fg1 = 1;
	} else {															// 始点角度が+th度に近い
		l1 = th-th1, fg1 = 2;
	}
	if(th+th2<th-th2) {													// 終点角度が-th度に近い
		l2 = th+th2, fg2 = 1;
	} else {															// 終点角度が+th度に近い
		l2 = th-th2, fg2 = 2;
	}
	if(l1<l2) {															// <始点の方が対称線に近い>
		for(i=n-1, j=0; i>=0; i--) {
			p1[j] = pa[i], i1[j++] = ia[i];								// 制御点情報を降順に複写
		}
	} else {															// <終点の方が対称線に近い>
		for(i=0; i<n; i++) {
			p1[i] = pa[i], i1[i] = ia[i];								// 制御点情報を昇順に複写
		}
		fg1 = fg2;
	}
	if(fg1==1) {														// -th度の対称線に近い場合(逆回転)
		for(i=0; i<DIM; i++) {
			vec.p[i] = 1.0;
		}
	} else {															// +th度の対称線に近い場合(正回転)
		for(i=0; i<DIM; i++) {
			vec.p[i] = -1.0;
		}
	}
	vec.p[dr1] = 0.0, vec.p[dr2] = 0.0;
	th = 2.0*PI/div;													// 対称領域最大角度(th)
	for(i=0; i<div; i++) {
		SetRotateMatrix(vec, th*i, rot);								// 回転ﾏﾄﾘｯｸｽ作成
		for(j=0; j<n; j++) {
			for(k=0; k<DIM; k++) {
				pt.p[k] = p1[j].p[k]-cpt.p[k];
			}
			VecRotateMove(&pt, rot, cpt);								// 回転移動
			pa[ct] = pt;												// 回転後の制御点を設定
			ia[ct++] = i1[j];
		}
	}
	*pn = ct;															// 対称複写後の制御点
	delete[] p1;
	delete[] i1;
}

/************************************************************/
int CopySymmetryArea(int mod, PNTTYPE cpt, int div, int dr1,
                     int dr2, int* pn, int* bc, PNTTYPE* pa,
                     int* ia, int* fg1, int* fg2)						//<<< 対称領域複写
/************************************************************/
{
	int    rt, df1, df2, ret=0;
	double l1, l2, ln1, ln2;

	*fg1 = -1, *fg2 = -1;												// 初期は接しない状態
	if(mod==1) {														// <<<1軸対称の場合>>>
		rt = CopySymmetry1(cpt, dr1, pn, pa, ia, bc);					// 1軸対称領域複写
		if(rt==1) {														// 接した軸方向を設定
			*fg2 = dr1;
		} else if(rt==2) {
			*fg1 = dr1, *fg2 = dr1;
		}
	} else if(mod==2) {													// <<<2軸対称の場合>>>
		l1 = fabs(pa[0].p[dr1]-cpt.p[dr1]);								// 始点とdr1対称軸の距離
		l2 = fabs(pa[0].p[dr2]-cpt.p[dr2]);								// 始点とdr2対称軸の距離
		if(l1<EPSILON&&l2<EPSILON) {									// 始点が対称軸中心と一致
			ln1 = 0.0, df1 = 0;
		} else if(l1<l2) {												// 始点がdr1対称軸に近い
			ln1 = l1, df1 = 1;
		} else {														// 始点がdr2対称軸に近い
			ln1 = l2, df1 = 2;
		}
		l1 = fabs(pa[*pn-1].p[dr1]-cpt.p[dr1]);							// 終点とdr1対称軸の距離
		l2 = fabs(pa[*pn-1].p[dr2]-cpt.p[dr2]);							// 終点とdr2対称軸の距離
		if(l1<EPSILON&&l2<EPSILON) {									// 終点が対称軸中心と一致
			ln2 = 0.0, df2 = 0;
		} else if(l1<l2) {												// 終点がdr1対称軸に近い
			ln2 = l1, df2 = 1;
		} else {														// 終点がdr2対称軸に近い
			ln2 = l2, df2 = 2;
		}
		if(ln1<EPSILON&&ln2<EPSILON) {									// <<2点が接する場合>>
			if((df1==1&&df2==2)||(df1==2&&df2==1)) {					// <2軸両方に接する>
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1軸の1軸対称領域複写
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2軸の1軸対称領域複写
				*fg1 = dr2, *fg2 = dr1;									// 接した軸方向を設定
			} else if(df1==1||df2==1) {									// <dr1対称軸に接する>
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1軸の1軸対称領域複写
				*fg1 = dr1, *fg2 = dr1, ret = 1;						// 接した軸方向を設定
			} else {													// <dr2対称軸に接する>
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2軸の1軸対称領域複写
				*fg1 = dr2, *fg2 = dr2, ret = 2;						// 接した軸方向を設定
			}
		} else if(df1==0||df2==0) {										// <<何れかが対称軸中心と一致>>
			if(df1==1||df2==1) {										// <残りがdr1対称軸に近い>
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1軸の1軸対称領域複写
				*fg2 = dr1, ret = 1;									// 接した軸方向を設定
			} else {													// <残りがdr2対称軸に近い>
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2軸の1軸対称領域複写
				*fg2 = dr2, ret = 2;									// 接した軸方向を設定
			}
		} else {														// <<その他の場合>>
			if(ln1>=ln2) {
				df1 = df2;
			}
			if(df1==1) {												// <<dr1対称軸の方が近い>>
				rt = CopySymmetry1(cpt, dr1, pn, pa, ia, bc);			// dr1軸の1軸対称領域複写
				if(rt==1) {												// 接した軸方向を設定
					*fg2 = dr1;
				}
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2軸の1軸対称領域複写
			} else {													// <<dr2対称軸の方が近い>>
				rt = CopySymmetry1(cpt, dr2, pn, pa, ia, bc);			// dr2軸の1軸対称領域複写
				if(rt==1) {												// 接した軸方向を設定
					*fg2 = dr2;
				}
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1軸の1軸対称領域複写
			}
		}
	} else if(mod==3) {													// <<<回転対称の場合>>>
		CopySymmetry2(cpt, div, dr1, dr2, pn, pa, ia);					// 回転対称領域複写
	}
	return ret;
}

/*********************************************************/
void PickSymmetryArea(int mod, int div, int dn, int tn,
                      int fg1, int fg2, int* pn, int* pk)				//<<< 対称領域ﾄﾗｯｶｰ選択
/*********************************************************/
{
	int i, j, no, mx, bn;

	no = *pn, mx = *pn;													// 設定済みﾄﾗｯｶｰNo数
	if(mod==1) {														// <<<1軸対称の場合>>>
		bn = (fg2>-1) ? dn-1 : dn;										// 基準No
		for(i=mx-1; i>=0; i--) {
			j = bn-pk[i];												// 基準Noとの差
			if(j>0&&bn+j<tn) {
				pk[no++] = bn+j;										// 対称領域ﾄﾗｯｶｰNo追加
			}
		}
	} else if(mod==2) {													// <<<2軸対称の場合>>>
		bn = (fg2>-1) ? dn-1 : dn;										// 基準No
		for(i=mx-1; i>=0; i--) {
			j = bn-pk[i];												// 基準Noとの差
			if(j>0) {
				pk[no++] = bn+j;										// 対称領域ﾄﾗｯｶｰNo追加
			}
		}
		mx = no;
		bn = (fg2>-1) ? dn*2-1 : dn*2+1;								// 基準No
		if(fg1>-1) {
			bn -= 1;
		}
		for(i=mx-1; i>=0; i--) {
			j = bn-pk[i];												// 基準Noとの差
			if(j>0&&bn+j<tn) {
				pk[no++] = bn+j;										// 対称領域ﾄﾗｯｶｰNo追加
			}
		}
	} else if(mod==3) {													// <<<回転対称の場合>>>
		for(i=1; i<div; i++) {
			for(j=0; j<mx; j++) {
				pk[no++] = (dn+1)*i+pk[j];								// 対称領域ﾄﾗｯｶｰNo追加
			}
		}
	}
	*pn = no;
}

/************************************************************/
static int CalcRAreaNo(PNTTYPE pos, PNTTYPE cpt, int div,
                       int dr1, int dr2, int tn, double* ta)			//<<< 回転領域No計算
/************************************************************/
{
	int    i, no=-1;
	double th, wth, l1, l2;

	if(fabs(pos.p[dr1]-cpt.p[dr1])>EPSILON||
       fabs(pos.p[dr2]-cpt.p[dr2])>EPSILON) {
		l1 = (dr1==0) ? -(pos.p[dr1]-cpt.p[dr1]):pos.p[dr1]-cpt.p[dr1];
		l2 = (dr2==0) ? -(pos.p[dr2]-cpt.p[dr2]):pos.p[dr2]-cpt.p[dr2];
		th = atan2(l2, l1);												// 今回移動点角度算出
	} else {
		th = 0.0;
	}
	wth = fabs(th);														// 移動点角度絶対値
	for(i=0; i<tn; i++) {
		if(wth>ta[i]) {													// 分割角度ｵｰﾊﾞｰはNo設定
			no = i;
		} else {														// 分割角度未満は終了
			break;
		}
	}
	if(no==-1) {														// 角度ta[0]以下は0
		no = 0;
	} else if(div%2==0&&no==tn-1) {										// div偶数&ta[tn-1]大はdiv-1
		no = div-1;
	} else {															// 以外の場合で
		if(th>0.0) {													// 角度正は2*no+1
			no = 2*no+1;
		} else {														// 角度負は2*no+2
			no = 2*no+2;
		}
	}
	if(no>1) {
		if(no%2==1) {													// 領域Noを右回り
			no = (no/2)+1;
		} else {														// 昇順に並び替え
			no = div-(no/2);
		}
	}
	return no;
}

/****************************************************************/
void MoveSymmetryArea(PNTTYPE pos, PNTTYPE mov, int mod,
                      PNTTYPE cpt, int div, int dr1, int dr2,
                      int fg1, int fg2, int rvf, int dn,
                      int pn, int *pk, PNTTYPE* pa, PNTTYPE* ma)		//<<< 対称領域移動量設定
/****************************************************************/
{
	PNTTYPE* mv = new PNTTYPE[View->m_MaxCCtA];
	double*  ta = new double[View->m_MaxCCtA/2+1];
	VECTYPE  vec;
	int      i, j, no, tn;
	double   th, rot[3][3];

	if(dn>0) {															// <継続ﾓｰﾄﾞの場合>
		if(pk[0]==1&&fg1>-1) {											// 始点の接軸交差方向は無効
			mov.p[fg1] = 0.0;
		}
		if(pk[dn-1]==1&&fg2>-1) {										// 終点の接軸交差方向は無効
			mov.p[fg2] = 0.0;
		}
	}
	if(mod==1) {														// <<<1軸対称の場合>>>
		for(i=0; i<3; i++) {
			mv[i] = mov;
		}
		if(pos.p[dr1]<cpt.p[dr1]) {
			mv[1].p[dr1] = -mov.p[dr1];									// 対称領域負側の移動量
		} else if(pos.p[dr1]>cpt.p[dr1]) {
			mv[0].p[dr1] = -mov.p[dr1];									// 対称領域正側の移動量
		}
		if(rvf) {														// <軸方向反転あり>
			if(pos.p[dr1]<cpt.p[dr1]) {
				mv[1].p[dr2] = -mov.p[dr2];								// 対称領域負側の移動量
			} else if(pos.p[dr1]>cpt.p[dr1]) {
				mv[0].p[dr2] = -mov.p[dr2];								// 対称領域正側の移動量
			}
		}
		for(i=0; i<pn; i++) {
			if(pa[i].p[dr1]<cpt.p[dr1]) {								// 対称領域負側
				j = 0;
			} else if(pa[i].p[dr1]>cpt.p[dr1]) {						// 対称領域正側
				j = 1;
			} else {													// 対称軸上
				j = 2;
			}
			ma[i] = mv[j];												// 移動量設定
		}
	} else if(mod==2) {													// <<<2軸対称の場合>>>
		for(i=0; i<9; i++) {
			mv[i] = mov;
		}
		if(pos.p[dr1]<cpt.p[dr1]) {										// dr1軸負側の移動量
			mv[2].p[dr1] = -mov.p[dr1];
			mv[5].p[dr1] = -mov.p[dr1];
			mv[8].p[dr1] = -mov.p[dr1];
		} else if(pos.p[dr1]>cpt.p[dr1]) {								// dr1軸正側の移動量
			mv[0].p[dr1] = -mov.p[dr1];
			mv[3].p[dr1] = -mov.p[dr1];
			mv[6].p[dr1] = -mov.p[dr1];
		}
		if(pos.p[dr2]<cpt.p[dr2]) {										// dr2軸負側の移動量
			mv[6].p[dr2] = -mov.p[dr2];
			mv[7].p[dr2] = -mov.p[dr2];
			mv[8].p[dr2] = -mov.p[dr2];
		} else if(pos.p[dr2]>cpt.p[dr2]) {								// dr2軸正側の移動量
			mv[0].p[dr2] = -mov.p[dr2];
			mv[1].p[dr2] = -mov.p[dr2];
			mv[2].p[dr2] = -mov.p[dr2];
		}
		for(i=0; i<pn; i++) {
			if(pa[i].p[dr1]<cpt.p[dr1]) {								// dr1軸負側
				j = 0;
			} else if(pa[i].p[dr1]>cpt.p[dr1]) {						// dr1軸正側
				j = 2;
			} else {													// dr1軸上
				j = 1;
			}
			if(pa[i].p[dr2]<cpt.p[dr2]) {								// dr2軸負側
				j += 0;
			} else if(pa[i].p[dr2]>cpt.p[dr2]) {						// dr2軸正側
				j += 6;
			} else {													// dr2軸上
				j += 3;
			}
			ma[i] = mv[j];												// 移動量設定
		}
	} else if(mod==3) {													// <<<回転対称の場合>>>
		ta[0] = PI/div;													// 対称領域の最大角度/2
		th = 2.0*PI/div;												// 対称領域の最大角度
		tn = div/2;														// 正方向領域分割数
		for(i=1; i<tn; i++) {
			ta[i] = ta[0]+i*th;											// 領域分割角度
		}
		no = CalcRAreaNo(pos, cpt, div, dr1, dr2, tn, ta);				// 回転領域No計算
		for(i=0; i<DIM; i++) {
			vec.p[i] = -1.0;											// 回転軸設定
		}
		vec.p[dr1] = 0.0, vec.p[dr2] = 0.0;
		for(i=0; i<div; i++) {											// 回転領域毎の移動量計算
			SetRotateMatrix(vec, th*i, rot);							// 回転ﾏﾄﾘｯｸｽ作成
			if((j=no+i)>div-1) {
				j -= div;
			}
			mv[j] = mov;												// 指定移動量
			VecRotate(&mv[j], rot);										// 移動量回転
		}
		for(i=0; i<pn; i++) {
			no = CalcRAreaNo(pa[i], cpt, div, dr1, dr2, tn, ta);		// 回転領域No計算
			ma[i] = mv[no];												// 移動量設定
		}
	} else {															// <<<通常ﾓｰﾄﾞの場合>>>
		for(i=0; i<pn; i++) {
			ma[i] = mov;												// 全て同一方向の移動量
		}
	}
	delete[] mv;
	delete[] ta;
}
