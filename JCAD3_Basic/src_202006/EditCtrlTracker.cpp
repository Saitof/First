/////////////////////////////////////////////////////////////////////////////
// 曲面編集トラッカー操作部
// EditCtrlTracker.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "NumFunc.h"

/*******************************************************/
void GetMoveValue(int plane, PNTTYPE bas, PNTTYPE* val)					//<<< ﾄﾗｯｶｰ移動量取得
/*******************************************************/
{
	int i1, i2, i3;

	i1 = plane;															// 更新対象座標
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	val->p[i1] = val->p[i1]-bas.p[i1];									// 移動量算出
	val->p[i2] = val->p[i2]-bas.p[i2];
	val->p[i3] = 0.0;
}

/*******************************************************************/
void SetParsTracker(int ps, int* md, int fg, int* pk, PNTTYPE* pa,
                    int* ia, int an, PNTTYPE* pb, int* ib, int* bn)		//<<< 透視図ﾄﾗｯｶｰ設定
/*******************************************************************/
{
	int* cl = new int[View->m_MaxWCtl];
	int  i, j, ct=0;

	if(*md) {															// <<透視図選択有>>
		if(!fg) {														// <曲面の場合>
			for(i=0; i<an; i++) {
				if(pk[i]) {												// 指示済みを抽出
					pb[ct] = pa[i], ib[ct] = i, cl[ct++] = 4;
				}
			}
		} else {														// <曲線の場合>
			j = (an+1)/2;
			for(i=0; i<j; i++) {
				if(pk[i]) {												// 指示済みを抽出
					pb[ct] = pa[i*2], ib[ct] = i*2, cl[ct++] = 4;
				}
			}
		}
		if(ct>0) {
			View->Tracker1.Setup(pa, ia, NULL, an, 3, ps);				// ﾄﾗｯｶｰ1設定
			View->Tracker2.Setup(pb, cl, NULL, ct, 5, ps);				// ﾄﾗｯｶｰ2設定
		} else {
			View->Tracker1.Setup(pa, ia, NULL, an, 4, ps);				// ﾄﾗｯｶｰ1設定
			View->Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
			*md = 0;
		}
	} else {															// <<透視図選択無>>
		View->Tracker1.Setup(pa, ia, NULL, an, 4, ps);					// ﾄﾗｯｶｰ1設定
		View->Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
	}
	*bn = ct;
	delete[] cl;
}

/**********************************************************/
void DispParsTracker(int ps, int md, PNTTYPE* pa, int* ia,
                     int an, PNTTYPE* pb, int* ib, int bn)				//<<< 透視図ﾄﾗｯｶｰ表示
/**********************************************************/
{
	int* cl = new int[View->m_MaxSCtA];
	int  i;

	if(md) {															// <透視図選択有>
		View->Tracker1.Setup(pa, ia, NULL, an, 3, ps);					// ﾄﾗｯｶｰ1設定
		for(i=0; i<bn; i++) {
			pb[i] = pa[ib[i]], cl[i] = 4;								// 最新のﾃﾞｰﾀに更新
		}
		View->Tracker2.Setup(pb, cl, NULL, bn, 5, ps);					// ﾄﾗｯｶｰ2設定
	} else {															// <透視図選択無>
		View->Tracker1.Setup(pa, ia, NULL, an, 4, ps);					// ﾄﾗｯｶｰ1設定
		View->Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
	}
	delete[] cl;
}

/**********************************************************/
void HitCtrlTracker1(UINT fg, const CPoint& po, int pl,
                     int ms, PNTTYPE pt, PNTTYPE* p1,
                     int n1, PNTTYPE* p2, int* i2, int n2,
                     int* f1, int* f2, int* pn, int* pa)				//<<< 全ﾄﾗｯｶｰﾁｪｯｸ1
/**********************************************************/
{
	int ht;

	*pn = 0;
	if(n2==0||pl==3) {
		if(pl==3) {														// <透視図選択>
			if((ht=View->Tracker1.HitTracker3D(0, po, p1, n1))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				*f1 = 1, *pn = 1, pa[0] = ht;							// 透視図選択設定
			} else {
				View->EyeChange(fg, po, pl, ms);						// 視点変更
				*f2 = 1;												// 視点変更ﾓｰﾄﾞ
			}
		} else {														// <3面図選択>
			*pn = View->Tracker1.HitTrackerS(0, pl, pt, p1, n1, pa);	// ﾄﾗｯｶｰ1ﾁｪｯｸ
			if(*pn>1) {
				*f1 = 1;
			}
		}
	} else {
		if((ht=View->Tracker2.HitTracker(0, pl, pt, p2, n2))!=-1) {		// ﾄﾗｯｶｰ2ﾁｪｯｸ
			*pn = 1, pa[0] = i2[ht];									// 選択ﾄﾗｯｶｰ番号
		}
	}
}

/*************************************************************/
void HitCtrlTracker2(UINT fg, const CPoint& po, int pl,
                     int ms, PNTTYPE pt, PNTTYPE* p1, int n1,
                     int m1, int* f2, int* pn, int* pa)					//<<< 全ﾄﾗｯｶｰﾁｪｯｸ2
/*************************************************************/
{
	int ht;

	*pn = 0;
	if(pl==3) {															// <透視図選択>
		if((ht=View->Tracker1.HitTracker3D(0, po, p1, n1))!=-1) {		// 前半部ﾄﾗｯｶｰ1ﾁｪｯｸ
			*pn = 1, pa[0] = ht;
		} else {
			View->EyeChange(fg, po, pl, ms);							// 視点変更
			*f2 = 1;													// 視点変更ﾓｰﾄﾞ
		}
	} else {															// <3面図選択>
		if((ht=View->Tracker1.HitTracker(0, pl, pt, &p1[n1], m1))!=-1) {// 後半部ﾄﾗｯｶｰ1ﾁｪｯｸ
			*pn = 1, pa[0] = ht+n1;
		} else {
			*pn = View->Tracker1.HitTrackerS(0, pl, pt, p1, n1, pa);	// 前半部ﾄﾗｯｶｰ1ﾁｪｯｸ
		}
	}
}

/********************************************************/
void DragCtrlTracker1(int pl, BOXTYPE bx, PNTTYPE* p1,
                      int n1, PNTTYPE* p2, int* i2,
                      int n2, int* f1, int* pn, int* pa)				//<<< ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
/********************************************************/
{
	BOXTYPE box;
	int     i, j, k, cnt=0;
	double  xmin, xmax, ymin, ymax, zmin, zmax;

	View->GetTRLimit(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);			// ﾄﾗｯｶｰ範囲限定取得
	for(i=0; i<DIM; i++) {
		if(bx.bp[0].p[i]<bx.bp[1].p[i]) {								// ﾎﾞｯｸｽｻｲｽﾞ位置補正
			box.bp[0].p[i] = bx.bp[0].p[i], box.bp[1].p[i] = bx.bp[1].p[i];
		} else {
			box.bp[0].p[i] = bx.bp[1].p[i], box.bp[1].p[i] = bx.bp[0].p[i];
		}
	}
	if(n2==0) {
		for(i=0; i<n1; i++) {
			if(xmin<p1[i].p[0]&&p1[i].p[0]<xmax&&
               ymin<p1[i].p[1]&&p1[i].p[1]<ymax&&
               zmin<p1[i].p[2]&&p1[i].p[2]<zmax) {						// <限定範囲内>
				if(BoxCheck(pl, box, p1[i])) {							// BOX内は選択
					pa[cnt++] = i;
				}
			}
		}
		for(i=0; i<cnt; i++) {
			for(j=i+1; j<cnt; j++) {									// 同一頂点分ｶｯﾄ
				if(fabs(p1[pa[i]].p[0]-p1[pa[j]].p[0])<EPSILON&&
                   fabs(p1[pa[i]].p[1]-p1[pa[j]].p[1])<EPSILON&&
                   fabs(p1[pa[i]].p[2]-p1[pa[j]].p[2])<EPSILON) {
					for(k=j; k<cnt-1; k++) {
						pa[k] = pa[k+1];
					}
					cnt--, j--;
				}
			}
		}
	} else {
		for(i=0; i<n2; i++) {
			if(xmin<p2[i].p[0]&&p2[i].p[0]<xmax&&
               ymin<p2[i].p[1]&&p2[i].p[1]<ymax&&
               zmin<p2[i].p[2]&&p2[i].p[2]<zmax) {						// <限定範囲内>
				if(BoxCheck(pl, box, p2[i])) {							// BOX内は選択
					pa[cnt++] = i;
				}
			}
		}
		for(i=0; i<cnt; i++) {
			for(j=i+1; j<cnt; j++) {									// 同一頂点分ｶｯﾄ
				if(fabs(p2[pa[i]].p[0]-p2[pa[j]].p[0])<EPSILON&&
                   fabs(p2[pa[i]].p[1]-p2[pa[j]].p[1])<EPSILON&&
                   fabs(p2[pa[i]].p[2]-p2[pa[j]].p[2])<EPSILON) {
					for(k=j; k<cnt-1; k++) {
						pa[k] = pa[k+1];
					}
					cnt--, j--;
				}
			}
		}
		for(i=0; i<cnt; i++) {
			pa[i] = i2[pa[i]];
		}
	}
	if(cnt>1) {
		*f1 = 1;
	}
	*pn = cnt;
}

/******************************************************/
void DragCtrlTracker2(int pl, BOXTYPE bx, PNTTYPE* p1,
                      int n1, int* pn, int* pa)							//<<< ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
/******************************************************/
{
	BOXTYPE box;
	int     i, j, k, cnt=0;
	double  xmin, xmax, ymin, ymax, zmin, zmax;

	View->GetTRLimit(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);			// ﾄﾗｯｶｰ範囲限定取得
	for(i=0; i<DIM; i++) {
		if(bx.bp[0].p[i]<bx.bp[1].p[i]) {								// ﾎﾞｯｸｽｻｲｽﾞ位置補正
			box.bp[0].p[i] = bx.bp[0].p[i], box.bp[1].p[i] = bx.bp[1].p[i];
		} else {
			box.bp[0].p[i] = bx.bp[1].p[i], box.bp[1].p[i] = bx.bp[0].p[i];
		}
	}
	for(i=0; i<n1; i++) {
		if(xmin<p1[i].p[0]&&p1[i].p[0]<xmax&&
           ymin<p1[i].p[1]&&p1[i].p[1]<ymax&&
           zmin<p1[i].p[2]&&p1[i].p[2]<zmax) {							// <限定範囲内>
			if(BoxCheck(pl, box, p1[i])) {								// BOX内は選択
				pa[cnt++] = i;
			}
		}
	}
	for(i=0; i<cnt; i++) {
		for(j=i+1; j<cnt; j++) {										// 同一頂点分ｶｯﾄ
			if(fabs(p1[pa[i]].p[0]-p1[pa[j]].p[0])<EPSILON&&
               fabs(p1[pa[i]].p[1]-p1[pa[j]].p[1])<EPSILON&&
               fabs(p1[pa[i]].p[2]-p1[pa[j]].p[2])<EPSILON) {
				for(k=j; k<cnt-1; k++) {
					pa[k] = pa[k+1];
				}
				cnt--, j--;
			}
		}
	}
	*pn = cnt;
}

/********************************************************/
void SetLineNo1(int un, int vn, int f1, int f2, int* ib)				//<<< ﾗｲﾝ番号設定1
/********************************************************/
{
	int* la = new int[View->m_MaxSCtA];
	int  i, j, cnt, chk, wu1, wu2, wv1, wv2;

	wv1 = (f1%2==1) ? 1 : 0;											// 有効範囲設定
	wv2 = (f1>=2) ? 1 : 0;
	wu1 = (f2%2==1) ? 1 : 0;
	wu2 = (f2>=2) ? 1 : 0;
	for(i=wu1, cnt=0; i<un-wu2; i++) {									// 指定U断面検索
		for(j=wv1, chk=0; j<vn-wv2; j++) {
			if(ib[j*un+i]==1) {
				chk = 1; break;
			}
		}
		if(chk==1) {
			la[cnt++] = i;
		}
	}
	View->SetLineNo(0, cnt, la);										// ﾗｲﾝ番号設定
	for(i=wv1, cnt=0; i<vn-wv2; i++) {									// 指定V断面検索
		for(j=wu1, chk=0; j<un-wu2; j++) {
			if(ib[i*un+j]==1) {
				chk = 1; break;
			}
		}
		if(chk==1) {
			la[cnt++] = i;
		}
	}
	View->SetLineNo(1, cnt, la);										// ﾗｲﾝ番号設定
	delete[] la;
}

/********************************************************/
void SetLineNo2(int vn, int tn, int f1, int f2, int* iw)				//<<< ﾗｲﾝ番号設定2
/********************************************************/
{
	int* la = new int[View->m_MaxSCtA];
	int  i, cnt, wuv, wua, wva;

	wva = (f1%2==1) ? 1 : 0;											// U/V境界と補正数取得
	wua = (f2%2==1) ? 1 : 0;
	wuv = (f1%2==1) ? vn-1 : vn;
	wuv = (f1>=2) ? wuv-1 : wuv;
	for(i=0, cnt=0; i<wuv; i++) {
		if(iw[i]==1) {													// Vﾗｲﾝ確認
			la[cnt++] = i+wva;
		}
	}
	View->SetLineNo(1, cnt, la);										// ﾗｲﾝ番号設定
	for(i=wuv, cnt=0; i<tn; i++) {
		if(iw[i]==1) {													// Uﾗｲﾝ確認
			la[cnt++] = i-wuv+wua;
		}
	}
	View->SetLineNo(0, cnt, la);										// ﾗｲﾝ番号設定
	delete[] la;
}

/*****************************************************/
void SetLineNo3(int tn, int fg, int* ib, OBJTYPE* op)					//<<< ﾗｲﾝ番号設定3
/*****************************************************/
{
	int* la = new int[View->m_MaxSCtA];
	int  i, cnt=0;

	for(i=0; i<tn; i++) {
		if(ib[i]==1) {													// V始点同一は+1
			la[cnt++] = (fg%2) ? i+1 : i;
		}
	}
	View->SetLineNo(0, cnt, la);										// ﾗｲﾝ番号設定
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ登録
	delete[] la;
}

/*********************************************************/
void PickCPoint(PNTTYPE pnt, int ix, int pn, PNTTYPE* pb,
                int ub, int vb, int un, int vn,
                int uf, int vf, int fg, int* ia, int* ib)				//<<< 対象制御点選択
/*********************************************************/
{
	int i, ch, md=0;

	md = ((uf==1||uf==3)&&ix>=0&&ix<un) ? 1 : 0;						// 該当位置確認
	md = ((uf==2||uf==3)&&ix>=un*(vn-1)&&ix<un*(vn-1)+un) ? 2 : md;
	md = ((vf==1||vf==3)&&ix%un==0) ? 3 : md;
	md = ((vf==2||vf==3)&&(ix+1)%un==0) ? 4 : md;
	if(fg) {															// <曲線ﾓｰﾄﾞｼﾌﾄ>
		ch = 0;
		ia[ix] = (ia[ix]+1>3) ? 0 : ia[ix]+1;
		if((!ub)&&(ix%un==0||ix%un==un-1)) {							// <U開の場合>
			ia[ix] = (ia[ix]%2==0) ? ia[ix]+1 : ia[ix];					// U両端は折線
			ch++;
		}
		if((!vb)&&(ix<un||ix>=un*(vn-1))) {								// <V開の場合>
			ia[ix] = (ia[ix]==0) ? 2 : ia[ix];							// V両端は折線
			ch++;
		}
		ia[ix] = (ch==2) ? 3 : ia[ix];									// U/V開の四隅
	}
	ib[ix] = (ib[ix]) ? 0 : 1;											// 選択状況反転
	if(md==0) {															// 収束点位置以外終了
		return;
	}
	for(i=0; i<pn; i++) {												// <<指定点同一座標>>
		if(fabs(pnt.p[0]-pb[i].p[0])<EPSILON&&
           fabs(pnt.p[1]-pb[i].p[1])<EPSILON&&
           fabs(pnt.p[2]-pb[i].p[2])<EPSILON&&i!=ix) {
			if(md==1&&(i<0||i>=un)) {									// 対象外ﾃﾞｰﾀ
				continue;
			}
			if(md==2&&(i<un*(vn-1)||i>=un*(vn-1)+un)) {
				continue;
			}
			if(md==3&&i%un!=0) {
				continue;
			}
			if(md==4&&(i+1)%un!=0) {
				continue;
			}
			if(fg) {													// <曲線ﾓｰﾄﾞｼﾌﾄ>
				ch = 0;
				ia[i] = (ia[i]+1>3) ? 0 : ia[i]+1;
				if((!ub)&&(i%un==0||i%un==un-1)) {						// <U開の場合>
					ia[i] = (ia[i]%2==0) ? ia[i]+1 : ia[i];				// U両端は折線
					ch++;
				}
				if((!vb)&&(i<un||i>=un*(vn-1))) {						// <V開の場合>
					ia[i] = (ia[i]==0) ? 2 : ia[i];						// V両端は折線
					ch++;
				}
				ia[i] = (ch==2) ? 3 : ia[i];							// U/V開の四隅
			}
			ib[i] = (ib[i]) ? 0 : 1;									// 選択状況反転
		}
	}
}

/**************************************************/
void CheckCLine(int ub, int vb, int un, int vn,
                PNTTYPE* pa, int* uflg, int* vflg)						//<<< 制御点列確認
/**************************************************/
{
	int i, j, chk;

	*uflg = 0, chk = 0;
	if(ub) {															// <<U始点確認>>
		for(i=1; i<un; i++) {
			for(j=0; j<DIM; j++) {
				if(fabs(pa[0].p[j]-pa[i].p[j])>EPSILON) {				// U始点同一点?
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// 同一点+1
		*uflg += 1;
	}
	chk = 0;
	if(ub) {															// <<U終点確認>>
		for(i=1; i<un; i++) {
			for(j=0; j<DIM; j++) {										// U終点同一点?
				if(fabs(pa[un*(vn-1)].p[j]-pa[un*(vn-1)+i].p[j])>EPSILON) {
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// 同一点+2
		*uflg += 2;
	}
	*vflg = 0, chk = 0;
	if(vb) {															// <<V始点確認>>
		for(i=1; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				if(fabs(pa[0].p[j]-pa[i*un].p[j])>EPSILON) {			// V始点同一点?
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// 同一点+1
		*vflg += 1;
	}
	chk = 0;
	if(vb) {															// <<V終点確認>>
		for(i=1; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				if(fabs(pa[un-1].p[j]-pa[un-1+i*un].p[j])>EPSILON) {	// V終点同一点?
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// 同一点+2
		*vflg += 2;
	}
}

/****************************************************************/
void TrackerCLine(int md, int sw, int un, int vn, PNTTYPE* pa,
                  int uf, int vf, int *wn, PNTTYPE* pw, int* iw)		//<<< 制御点列ﾄﾗｯｶｰ取得
/****************************************************************/
{
	int i, j, uu, vv, us, ue, vs, ve, uv, pc=0;

	us = 0, vs = 0, uu = un, ue = un, vv = vn, ve = vn;					// 変数初期化
	if(uf%2==1) {														// <U始点同一の場合>
		vs++, vv--;
	}
	if(uf>=2) {															// <U終点同一の場合>
		ve--, vv--;
	}
	if(vf%2==1) {														// <V始点同一の場合>
		us++, uu--;
	}
	if(vf>=2) {															// <V終点同一の場合>
		ue--, uu--;
	}
	uv = us+(uu-1)/2;
	for(i=vs; i<ve; i++) {												// <<V方向ﾄﾗｯｶｰ>>
		if(sw) {														// 通常曲面は中間点
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[i*un+uv].p[j]+pa[i*un+uv+1].p[j])/2.0;
			}
		} else {														// 掃引曲面は開始断面
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = pa[i*un+us].p[j];
			}
		}
		iw[pc++] = 0;
	}
	uv = vs+(vv-1)/2;
	for(i=us; i<ue; i++) {												// <<U方向ﾄﾗｯｶｰ>>
		if(md) {														// 通過点は断面点上
			pw[pc] = pa[i+uv*un];
		} else {														// 制御点は断面点中間
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[i+uv*un].p[j]+pa[i+(uv+1)*un].p[j])/2.0;
			}
		}
		iw[pc++] = 2;
	}
	*wn = pc;
}

/****************************************************************/
static void CheckCPoint(PNTTYPE pnt, int pn, PNTTYPE* pp,
                        int ub, int vb, int un, int vn, int uvf,
                        int flg, int* ia, int* ib, int* iw)				//<<< 対象制御点確認
/****************************************************************/
{
	int i, f1, f2;

	for(i=0; i<pn; i++) {												// <<指定点と同一座標>>
		if(iw[i]==0) {													// <未処理の場合>
			if(fabs(pnt.p[0]-pp[i].p[0])<EPSILON&&
               fabs(pnt.p[1]-pp[i].p[1])<EPSILON&&
               fabs(pnt.p[2]-pp[i].p[2])<EPSILON) {
				f1 = ia[i]%2, f2 = ia[i]/2;								// 曲線ﾓｰﾄﾞ設定
				if(!uvf) {
					if(flg) {											// 曲線ﾓｰﾄﾞ反転
						f1 = (f1) ? 0 : 1;
					}
				} else {
					if(flg) {											// 曲線ﾓｰﾄﾞ反転
						f2 = (f2) ? 0 : 1;
					}
				}
				f1 = ((!ub)&&(i%un==0||i%un==un-1)) ? 1 : f1;			// U開はU両端折線
				f2 = ((!vb)&&(i<un||i>=un*(vn-1))) ? 1 : f2;			// V開はV両端折線
				ia[i] = f1+(f2*2);										// 曲線ﾓｰﾄﾞ
				ib[i] = (ib[i]) ? 0 : 1;								// 選択状況反転
				iw[i] = 1;												// 処理済
			}
		}
	}
}

/****************************************************************/
void PickCLine(int ix, int ub, int vb, int un, int vn, int uflg,
               int vflg, int flg, PNTTYPE* pa, int* ia, int* ib)		//<<< 対象制御点列選択
/****************************************************************/
{
	int*    iw = new int[View->m_MaxSCtA];
	PNTTYPE pnt;
	int     i, pn, uu, vv, ust, ued, vst, ved;

	ust = 0, vst = 0, uu = un, ued = un, vv = vn, ved = vn;				// 変数初期化
	if(uflg%2==1) {														// <U始点同一の場合>
		vst++, vv--;
	}
	if(uflg>=2) {														// <U終点同一の場合>
		ved--, vv--;
	}
	if(vflg%2==1) {														// <V始点同一の場合>
		ust++, uu--;
	}
	if(vflg>=2) {														// <V終点同一の場合>
		ued--, uu--;
	}
	pn = un*vn;															// 総制御点数
	for(i=0; i<pn; i++) {
		iw[i] = 0;														// 実行済ﾌﾗｸﾞ初期化
	}
	if(ix<vv) {															// <<V方向指定>>
		if(uflg%2==1&&ix==0) {											// <U始点同一&端指定>
			for(i=ust; i<ued; i++) {
				pnt = pa[i];											// 対象制御点確認
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 1, flg, ia, ib, iw);
			}
		}
		if(uflg>=2&&ix==vv-1) {											// <U終点同一&端指定>
			for(i=ust; i<ued; i++) {
				pnt = pa[i+(vn-1)*un];									// 対象制御点確認
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 1, flg, ia, ib, iw);
			}
		}
		if(uflg%2==1) {													// U始点同一+1
			ix++;
		}
		for(i=ust; i<ued; i++) {
			pnt = pa[i+ix*un];											// 対象制御点確認
			CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 1, flg, ia, ib, iw);
		}
	} else {															// <<U方向指定>>
		ix = ix-vv;
		if(vflg%2==1&&ix==0) {											// <V始点同一&端指定>
			for(i=vst; i<ved; i++) {
				pnt = pa[i*un];											// 対象制御点確認
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 0, flg, ia, ib, iw);
			}
		}
		if(vflg>=2&&ix==uu-1) {											// <V終点同一&端指定>
			for(i=vst; i<ved; i++) {
				pnt = pa[(i+1)*un-1];									// 対象制御点確認
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 0, flg, ia, ib, iw);
			}
		}
		if(vflg%2==1) {													// V始点同一+1
			ix++;
		}
		for(i=vst; i<ved; i++) {
			pnt = pa[ix+i*un];											// 対象制御点確認
			CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 0, flg, ia, ib, iw);
		}
	}
	delete[] iw;
}

/*******************************************************************/
void TrackerCInsert(int md, int ub, int vb, int un, int vn, int cn,
                    PNTTYPE* pa, int* wn, PNTTYPE* pw, int* iw)			//<<< 制御点列ﾄﾗｯｶｰ取得
/*******************************************************************/
{
	int i, j, k, l, mx, pc=0;

	k = (vn-1)/2, l = (un-1)/2;
	for(i=0; i<un-1; i++) {												// <<U方向ﾄﾗｯｶｰ>>
		if(md) {														// 通常曲面のみ断面置換用
			pw[pc] = pa[k*un+i], iw[pc++] = 2;
		}
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[k*un+i].p[j]+pa[k*un+i+1].p[j])/2.0;
		}
		iw[pc++] = 2;
	}
	if(md) {															// 通常曲面のみ断面置換用
		pw[pc] = pa[(k+1)*un-1], iw[pc++] = 2;
	}
	if(ub) {															// <U閉の場合>
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[(k+1)*un-1].p[j]+pa[k*un].p[j])/2.0;
		}
		iw[pc++] = 2;
	}
	for(i=0; i<vn-1; i++) {												// <<V方向ﾄﾗｯｶｰ>>
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[l+i*un].p[j]+pa[l+(i+1)*un].p[j])/2.0;
		}
		iw[pc++] = 0;
	}
	if(vb) {															// <V閉の場合>
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[l+(vn-1)*un].p[j]+pa[l].p[j])/2.0;
		}
		iw[pc++] = 0;
	}
	if(md) {															// 通常曲面はｺﾋﾟｰ用ﾄﾗｯｶｰ
		for(i=0; i<un; i++) {											// <<U方向ﾄﾗｯｶｰ>>
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[k*un+i].p[j]*3.0+pa[(k+1)*un+i].p[j])/4.0;
			}
			iw[pc++] = 3;
		}
	}
	if(cn>-1) {															// <複写断面指定時>
		mx = 2*un+vn;													// 最大挿入位置No
		if(!ub) {														// Uが開は減算
			mx--;
		}
		if(!vb) {														// Vが開は減算
			mx--;
		}
		iw[mx+cn] = 5;													// 指定断面ｶﾗｰ変更
	}
	*wn = pc;
}

/********************************************************************/
void TrackerULine(int md, int un, int vn, PNTTYPE* pa,
                  int uflg, int vflg, int* wn, PNTTYPE* pw, int* iw)	//<<< 制御点列ﾄﾗｯｶｰ取得
/********************************************************************/
{
	int i, j, vv, us, ue, vs, uv, pc=0;

	us = 0, vs = 0, ue = un, vv = vn;									// 変数初期化
	if(uflg%2==1) {														// <U始点同一の場合>
		vs++, vv--;
	}
	if(uflg>=2) {														// <U終点同一の場合>
		vv--;
	}
	if(vflg%2==1) {														// <V始点同一の場合>
		us++;
	}
	if(vflg>=2) {														// <V終点同一の場合>
		ue--;
	}
	uv = vs+(vv-1)/2;
	for(i=us; i<ue; i++) {												// <<U方向ﾄﾗｯｶｰ>>
		if(md) {														// 通過点は断面点上
			pw[pc] = pa[i+uv*un];
		} else {														// 制御点は断面点中間
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[i+uv*un].p[j]+pa[i+(uv+1)*un].p[j])/2.0;
			}
		}
		iw[pc++] = 2;
	}
	*wn = pc;
}

/****************************************************************/
void TrackerVLine(int un, int vn, PNTTYPE* pa, int vflg,
                  int ix, int tn, int* wn, PNTTYPE* pw, int* iw)		//<<< 制御点列ﾄﾗｯｶｰ取得
/****************************************************************/
{
	int i, st;

	st = tn;
	if(vflg%2==1) {														// <V始点同一の場合>
		ix++;
	}
	for(i=0; i<vn; i++) {												// <<V方向ﾄﾗｯｶｰ>>
		pw[st] = pa[ix+i*un], iw[st++] = 0;
	}
	*wn = vn;
}

/*******************************************************/
void SelAreaCtrl(int plane, BOXTYPE area,
                 int pn, PNTTYPE* pa, int* ia, int flg)					//<<< 制御点選択(ｴﾘｱ)
/*******************************************************/
{
	BOXTYPE box;
	int     i;
	double  xmin, xmax, ymin, ymax, zmin, zmax;

	View->GetTRLimit(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);			// ﾄﾗｯｶｰ範囲限定取得
	for(i=0; i<DIM; i++) {												// ﾎﾞｯｸｽｻｲｽﾞ位置補正
		if(area.bp[0].p[i]<area.bp[1].p[i]) {
			box.bp[0].p[i] = area.bp[0].p[i], box.bp[1].p[i] = area.bp[1].p[i];
		} else {
			box.bp[0].p[i] = area.bp[1].p[i], box.bp[1].p[i] = area.bp[0].p[i];
		}
	}
	for(i=0; i<pn; i++) {
		ia[i] = (!flg) ? 0 : ia[i];										// 選択全解除
		if(xmin<pa[i].p[0]&&pa[i].p[0]<xmax&&
           ymin<pa[i].p[1]&&pa[i].p[1]<ymax&&
           zmin<pa[i].p[2]&&pa[i].p[2]<zmax) {							// <限定範囲内>
			if(BoxCheck(plane, box, pa[i])) {
				ia[i] = (ia[i]) ? 0 : 1;
			}
		}
	}
}

/*****************************************************/
void GetBasePoint(int un, int vn, PNTTYPE* pa,
                  int ix, PNTTYPE* cpt, VECTYPE* vec)					//<<< 基準点取得
/*****************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	int      i, j, cnt;

	for(i=0, cnt=0; i<vn; i++) {
		pt[i] = pa[ix+i*un];											// V制御点抽出
		for(j=0; j<DIM; j++) {
			if(fabs(pt[0].p[j]-pt[i].p[j])>EPSILON) {					// 異点ｶｳﾝﾄ
				cnt++; break;
			}
		}
	}
	CalcCent(pt, vn, cpt);												// V断面中心取得
	CalcNVec(pt, vn, vec);												// V断面法線ﾍﾞｸﾄﾙ取得
	if(cnt==0) {														// <異点無の場合>
		vec->p[0] = 0.0, vec->p[1] = 0.0, vec->p[2] = 0.0;				// 法線ﾍﾞｸﾄﾙ無
	}
	delete[] pt;
}
