/////////////////////////////////////////////////////////////////////////////
// 曲面編集制御点操作部
// EditCtrlPoint2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/06/26 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "NumFunc.h"

/*********************************************************************/
void DeleteCLine(int wn, int* iw, int ub, int vb, int uflg, int vflg,
				 int* un, int* vn, PNTTYPE* pa, int* ia, int* ta)		//<<< 対象制御点列削除
/*********************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCCtA];
	int*     pi = new int[View->m_MaxSCtA];
	int*     pt = new int[View->m_MaxSCtA];
	int*     df = new int[View->m_MaxSCtA];
	PNTTYPE  cpt;
	int      i, j, ix, pn, uu, vv, uc, vc, vl, ua, va;
	int      uw1, uw2, vw1, vw2, us1, us2, vs1, vs2, f1, f2;

	ua = 0, va = 0, uu = *un, uc = *un, vv = *vn, vc = *vn, vl = *vn;	// 初期値設定
	pn = uu*vv;															// 総制御点数
	for(i=0; i<pn; i++) {
		df[i] = 0;														// 削除ﾌﾗｸﾞ初期化
	}
	if(uflg%2==1) {														// <U始点同一の場合>
		va++, vl--;
	}
	if(uflg>=2) {														// <U終点同一の場合>
		vl--;
	}
	if(vflg%2==1) {														// <V始点同一の場合>
		ua++;
	}
	for(i=0; i<wn; i++) {
		if(iw[i]) {														// <<削除対象の場合>>
			if(i<vl) {													// <V方向指定>
				for(j=0; j<uu; j++) {
					df[j+(i+va)*uu] = 1;								// 削除ﾌﾗｸﾞON
				}
				vc--;													// V列数減算
			} else {													// <U方向指定>
				for(j=0; j<vv; j++) {
					df[j*uu+i-vl+ua] = 1;								// 削除ﾌﾗｸﾞON
				}
				uc--;													// U列数減算
			}
		}
	}
	for(i=0, ix=0; i<pn; i++) {											// <削除対象外のみ>
		if(!df[i]) {													// 制御点,曲線ﾓｰﾄﾞ,張力複写
			pp[ix] = pa[i], pi[ix] = ia[i], pt[ix++] = ta[i];
		}
	}
	uw1 = 0, uw2 = 0, vw1 = 0, vw2 = 0, us1 = 0, us2 = 0, vs1 = 0, vs2 = 0;
	uw1 = ((!vb)&&iw[0]==1) ? 1 : uw1;									// 両端面指定&同一点収束確認
	uw2 = ((!vb)&&iw[vl-1]==1) ? 1 : uw2;
	vw1 = ((!ub)&&iw[vl]==1) ? 1 : vw1;
	vw2 = ((!ub)&&iw[wn-1]==1) ? 1 : vw2;
	us1 = (uflg%2==1) ? 1 : us1;
	us2 = (uflg>=2) ? 1 : us2;
	vs1 = (vflg%2==1) ? 1 : vs1;
	vs2 = (vflg>=2) ? 1 : vs2;
	if(uw1) {															// <<U開始断面削除>>
		if(us1) {														// <U開始断面同一点>
			for(i=0; i<uc; i++) {
				f1 = pi[i+uc]%2, f2 = 1;								// 曲線ﾓｰﾄﾞ変更
				pi[i+uc] = f1+2*f2;
				ps[i] = pp[i+uc];										// 開始断面抽出
			}
			CalcCent(ps, uc, &cpt);										// ｾﾝﾀｰ取得
			for(i=0; i<uc; i++) {
				pp[i] = cpt;											// 同一点をｾﾝﾀｰへ
			}
		} else {														// <U開始断面不同一>
			for(i=0; i<uc; i++) {
				f1 = pi[i]%2, f2 = 1;									// 曲線ﾓｰﾄﾞ変更
				pi[i] = f1+2*f2;
			}
		}
	}
	if(uw2) {															// <<U終了断面削除>>
		if(us2) {														// <U終了断面同一点>
			for(i=0; i<uc; i++) {
				f1 = pi[i+uc*(vc-2)]%2, f2 = 1;							// 曲線ﾓｰﾄﾞ変更
				pi[i+uc*(vc-2)] = f1+2*f2;
				ps[i] = pp[i+uc*(vc-2)];								// 終了断面抽出
			}
			CalcCent(ps, uc, &cpt);										// ｾﾝﾀｰ取得
			for(i=0; i<uc; i++) {
				pp[i+uc*(vc-1)] = cpt;									// 同一点をｾﾝﾀｰへ
			}
		} else {														// <U終了断面不同一>
			for(i=0; i<uc; i++) {
				f1 = pi[i+uc*(vc-1)]%2, f2 = 1;							// 曲線ﾓｰﾄﾞ変更
				pi[i+uc*(vc-1)] = f1+2*f2;
			}
		}
	}
	if(vw1) {															// <<V開始断面削除>>
		if(vs1) {														// <V開始断面同一点>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[i*uc+1]/2;								// 曲線ﾓｰﾄﾞ変更
				pi[i*uc+1] = f1+2*f2;
				ps[i] = pp[i*uc+1];										// 開始断面抽出
			}
			CalcCent(ps, vc, &cpt);										// ｾﾝﾀｰ取得
			for(i=0; i<vc; i++) {
				pp[i*uc] = cpt;											// 同一点をｾﾝﾀｰへ
			}
		} else {														// <V開始断面不同一>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[i*uc]/2;								// 曲線ﾓｰﾄﾞ変更
				pi[i*uc] = f1+2*f2;
			}
		}
	}
	if(vw2) {															// <<V終了断面削除>>
		if(vs2) {														// <V終了断面同一点>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[(i+1)*uc-2]/2;							// 曲線ﾓｰﾄﾞ変更
				pi[(i+1)*uc-2] = f1+2*f2;
				ps[i] = pp[(i+1)*uc-2];									// 終了断面抽出
			}
			CalcCent(ps, vc, &cpt);										// ｾﾝﾀｰ取得
			for(i=0; i<vc; i++) {
				pp[(i+1)*uc-1] = cpt;									// 同一点をｾﾝﾀｰへ
			}
		} else {														// <V終了断面不同一>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[(i+1)*uc-1]/2;							// 曲線ﾓｰﾄﾞ変更
				pi[(i+1)*uc-1] = f1+2*f2;
			}
		}
	}
	for(i=0; i<ix; i++) {
		pa[i] = pp[i], ia[i] = pi[i], ta[i] = pt[i];					// 制御点,曲線ﾓｰﾄﾞ,張力復元
	}
	*un = uc, *vn = vc;													// 削除後U,V制御点数
	delete[] pp;
	delete[] ps;
	delete[] pi;
	delete[] pt;
	delete[] df;
}

/*************************************************************/
static void NearCenterPoint(PNTTYPE cp, BOXTYPE bx,
							PNTTYPE* pa, int pn, PNTTYPE* pt)			//<<< 中心最短点取得
/*************************************************************/
{
	PNTTYPE minp, maxp;
	int     i, j;
	double  x, y, z, min1, max1;

	x = bx.bp[1].p[0]-bx.bp[0].p[0];									// BOX Xｻｲｽﾞ
	y = bx.bp[1].p[1]-bx.bp[0].p[1];									// BOX Yｻｲｽﾞ
	z = (bx.bp[0].p[2]+bx.bp[1].p[2])/2.0;								// BOX Z中心位置
	j = (x>y) ? 0 : 1;													// 最大ｻｲｽﾞ方向
	minp = pa[0], min1 = pa[0].p[j];									// 最小･最大初期値
	maxp = pa[0], max1 = pa[0].p[j];
	for(i=1; i<pn; i++) {
		if(pa[i].p[j]<min1) {											// 最小検索
			minp = pa[i], min1 = pa[i].p[j];
		}
		if(pa[i].p[j]>max1) {											// 最大検索
			maxp = pa[i], max1 = pa[i].p[j];
		}
	}
	minp.p[2] = z, maxp.p[2] = z;										// Z値は中心位置
    *pt = (PntD(&cp, &minp)<PntD(&cp, &maxp)) ? minp : maxp;			// 中心点に近い方
}

/********************************************************************/
void InsertCLine(int no, int flg, int ub, int* un, int* vn, int uf,
                 int* cn, int bn, PNTTYPE cpt, PNTTYPE* pa, int* ia)	//<<< 対象制御点列挿入
/********************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pd = new PNTTYPE[View->m_MaxCCtA];
	int*     ib = new int[View->m_MaxSCtA];
	int*     ic = new int[View->m_MaxCCtA];
	PNTTYPE  pw[4], cpc, cpd, mov;
	VECTYPE  vc1, vc2, rvc;
	BOXTYPE  bxc, bxd;
	int      i, j, k, uu, vv, ul, cc, i1, i2, f1, f2, ct1=0, ct2=0;
	double   rot[3][3], th;

	uu = *un, vv = *vn, ul = 2*uu, cc = *cn;							// 始点加算値初期化
	ul = (!ub) ? ul-1 : ul;												// U開は挿入点=断面数-1
	if(no<ul) {															// <<U方向指定>>
		if(cc>-1) {														// <複写断面が有効な場合>
			for(i=0; i<vv; i++) {
				pc[i] = pa[cc+i*uu], ic[i] = ia[cc+i*uu];				// 複写断面抽出
			}
		}
		for(i=0; i<vv; i++) {
			for(j=0; j<uu; j++) {
				pb[ct1] = pa[j+i*uu], ib[ct1++] = ia[j+i*uu];			// 制御点･曲線ﾓｰﾄﾞ複写
				if(no%2==1&&j==no/2) {									// <<挿入位置の場合>>
					if(j==uu-1) {										// <U閉の始終点>
						i1 = (i+1)*uu-1, i2 = i*uu;
					} else {											// <上記以外>
						i1 = j+i*uu, i2 = j+i*uu+1;
					}
					for(k=0; k<DIM; k++) {
						pb[ct1].p[k] = (pa[i1].p[k]+pa[i2].p[k])/2.0;	// 中間座標
					}
					f1 = ia[i1]/2, f2 = ia[i2]/2;						// V曲線ﾓｰﾄﾞ
					f1 = (f1&&f2) ? 1 : 0;								// 両点折線は折線
					ib[ct1++] = flg+f1*2;								// 曲線ﾓｰﾄﾞ設定
					pd[ct2++] = pb[ct1-1];								// 挿入断面抽出
				} else if(no%2==0&&j==no/2) {							// <<置換位置の場合>>
					pd[ct2++] = pb[ct1-1];								// 置換断面抽出
				}
			}
		}
		if(no%2==1) {													// <挿入の場合>
			uu++;														// U列数加算
			cc = (cc>-1&&cc>no/2) ? cc+1 : cc;							// 複写位置より前の挿入
			no = no/2 + 1;												// 挿入断面No
		} else {														// <置換の場合>
			no = no/2;													// 置換断面No
		}
		if(cc>-1) {														// <<複写断面が有効な場合>>
			CalcNVec(pc, vv, &vc1);										// 複写断面法線ﾍﾞｸﾄﾙ取得
			CalcNVec(pd, vv, &vc2);										// 挿入断面法線ﾍﾞｸﾄﾙ取得
			if(fabs(vc1.p[0]-vc2.p[0])>EPSILON||
               fabs(vc1.p[1]-vc2.p[1])>EPSILON||
               fabs(vc1.p[2]-vc2.p[2])>EPSILON) {						// 2ﾍﾞｸﾄﾙ間の角度
				th = acos(vc1.p[0]*vc2.p[0]+vc1.p[1]*vc2.p[1]+vc1.p[2]*vc2.p[2]);
				th = (th<-10||th>10) ? 0.0 : th;
			} else {
				th = 0.0;
			}
			for(i=0, j=1; i<DIM; i++) {
				j = (fabs(vc1.p[i])>EPSILON) ? 0 : j;					// vc1ﾁｪｯｸ
			}
			if(j) {
				th = 0.0;
			}
			for(i=0, j=1; i<DIM; i++) {
				j = (fabs(vc2.p[i])>EPSILON) ? 0 : j;					// vc2ﾁｪｯｸ
			}
			if(j) {
				th = 0.0;
			}
			if(th<EPSILON) {											// <有効角度未満の場合>
				rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = -1.0;
			} else if(th>PI/90&&th<PI*89/90) {							// <2～88度の場合>
				for(i=0; i<DIM; i++) {
					pw[0].p[i] = 0.0;									// 原点を2法線ﾍﾞｸﾄﾙの
					pw[1].p[i] = vc1.p[i]*1000.0;						// なす3点
					pw[2].p[i] = vc2.p[i]*1000.0;
				}
				CalcNVec(pw, 3, &rvc);									// 2ﾍﾞｸﾄﾙ間の法線ﾍﾞｸﾄﾙ
			} else {													// <以外の場合>
				CalcCent(pc, vv, &pw[0]);								// 断面の中心点
				CalcCent(pd, vv, &pw[2]);
				for(i=0; i<DIM; i++) {
					pw[1].p[i] = pw[0].p[i]+vc1.p[i]*1000.0;			// 断面上の中心点の
					pw[3].p[i] = pw[2].p[i]+vc2.p[i]*1000.0;			// 法線ﾍﾞｸﾄﾙ方向の点
				}
				CalcNVec(pw, 4, &rvc);									// 2ﾍﾞｸﾄﾙ間の法線ﾍﾞｸﾄﾙ
			}
			SetRotateMatrix(rvc, th, rot);								// 回転ﾏﾄﾘｯｸｽ作成
			CalcBox(pc, vv, &bxc);										// 複写断面BOXｻｲｽﾞ取得
			if(ub) {													// 複写断面中心最短点取得
				NearCenterPoint(cpt, bxc, pc, vv, &cpc);
			} else {													// 複写断面中心点取得
				BoxCenter(bxc, &cpc);
			}
			CalcBox(pd, vv, &bxd);										// 挿入断面BOXｻｲｽﾞ取得
			if(ub) {													// 挿入断面中心最短点取得
				NearCenterPoint(cpt, bxd, pd, vv, &cpd);
			} else {													// 挿入断面中心点取得
				BoxCenter(bxd, &cpd);
			}
			for(i=0; i<vv; i++) {
				for(j=0; j<DIM; j++) {
					pc[i].p[j] -= cpc.p[j];								// 回転中心点を原点
				}
				VecRotateMove(&pc[i], rot, cpc);						// 回転後元位置に戻る
			}
			if(bn==0) {
				cpc.p[2] = bxc.bp[1].p[2], cpd.p[2] = bxd.bp[1].p[2];	// 複写基準位置変更
			} else if(bn==2) {
				cpc.p[2] = bxc.bp[0].p[2], cpd.p[2] = bxd.bp[0].p[2];
			}
			for(i=0; i<DIM; i++) {
				mov.p[i] = cpd.p[i]-cpc.p[i];							// 移動量算出
			}
			if(!ub&&(no==0||no==uu-1)) {								// U:開で両端の場合は
				for(i=0; i<vv; i++) {									// 必ず、U曲線ﾓｰﾄﾞ=1
					j = ic[i]/2, ic[i] = j*2+1;
				}
			}
			for(i=0; i<vv; i++) {
				for(j=0; j<DIM; j++) {
					pc[i].p[j] += mov.p[j];								// 複写断面移動
				}
				pb[no+i*uu] = pc[i], ib[no+i*uu] = ic[i];				// 複写断面設定
			}
			if(uf%2==1) {
				pb[no] = pd[0];											// 始点収束は挿入断面始点採用
			}
			if(uf>1) {
				pb[no+(vv-1)*uu] = pd[vv-1];							// 終点収束は挿入断面終点採用
			}
		}
	} else {															// <<V方向指定>>
		for(i=0, no=no-ul; i<vv; i++) {
			for(j=0; j<uu; j++) {
				pb[ct1] = pa[j+i*uu], ib[ct1++] = ia[j+i*uu];			// 制御点･曲線ﾓｰﾄﾞ複写
			}
			if(i==no) {													// <<挿入位置の場合>>
				for(j=0; j<uu; j++) {
					if(i==vv-1) {										// <V閉の始終点>
						i1 = j+(vv-1)*uu, i2 = j;
					} else {											// <上記以外>
						i1 = j+i*uu, i2 = j+(i+1)*uu;
					}
					for(k=0; k<DIM; k++) {
						pb[ct1].p[k] = (pa[i1].p[k]+pa[i2].p[k])/2.0;	// 中間点座標
					}
					pd[ct2++] = pb[ct1];								// 挿入断面抽出
					f1 = ia[i1]%2, f2 = ia[i2]%2;						// U曲線ﾓｰﾄﾞ
					f1 = (f1&&f2) ? 1 : 0;								// 両点折線は折線
					ib[ct1++] = f1+flg*2;								// 曲線ﾓｰﾄﾞ設定
				}
			}
		}
		vv++;															// V列数加算
	}
	j = uu*vv;															// 挿入後制御点数
	*un = uu, *vn = vv, *cn = cc;
	for(i=0; i<j; i++) {
		pa[i] = pb[i], ia[i] = ib[i];									// 制御点･曲線ﾓｰﾄﾞ復元
	}
	delete[] pb;
	delete[] pc;
	delete[] pd;
	delete[] ib;
	delete[] ic;
}

/**************************************************************/
void PutSPoint(int fg, int ix, int un, int vn, int vflg,
			   PNTTYPE scp, PNTTYPE* pb, int* ib, PNTTYPE cpt,
			   VECTYPE vec, PNTTYPE* pa, PNTTYPE* pc, int* ia)			//<<< 断面制御点情報更新
/**************************************************************/
{
	PNTTYPE pnt;
	VECTYPE rvc;
	int     i, j, i1, i2, i3, vd;
	double  rot[3][3], rth, dd;

	for(i=0, vd=0, dd=0.0; i<DIM; i++) {
		if(dd<fabs(vec.p[i])) {											// 基準断面ﾍﾞｸﾄﾙ最大方向検索
			dd = fabs(vec.p[i]), vd = i;
		}
	}
    i1 = (vd+1>2) ? 0 : vd+1;											// 座標軸設定
    i2 = (i1+1>2) ? 0 : i1+1;
    i3 = (i2+1>2) ? 0 : i2+1;
	if(fg) {															// <立体消去>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] -= scp.p[j];									// 断面編集位置移動
			}
		}
	} else {															// <立体非消去>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] -= cpt.p[j];									// 断面編集位置移動
			}
		}
	}
	rth = sqrt(vec.p[i1]*vec.p[i1]+vec.p[i2]*vec.p[i2]);				// 基準面方向長さ
	if(rth>EPSILON) {													// <基準面にない場合>
		rvc.p[i1] = -vec.p[i2], rvc.p[i2] = vec.p[i1], rvc.p[i3] = 0.0;
		VecN(&rvc);
		rth = atan2(rth, vec.p[i3]);									// 回転角度算出
		rth = (rth<-10||rth>10) ? 0.0 : rth;
		rth = (rth>PI/2) ? rth+PI : rth;
		SetRotateMatrix(rvc, rth, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(i=0; i<vn; i++) {
			VecRotate(&pb[i], rot);										// 基準面に回転
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pb[i].p[j] += cpt.p[j];										// 断面中心位置移動
		}
	}
	pnt.p[0] = 0.0, pnt.p[1] = 0.0, pnt.p[2] = 0.0;
	i1 = un*vn;
	for(i=0; i<i1; i++) {
		pc[i] = pnt;													// 移動量配列全ｸﾘｱ
	}
	ix = (vflg%2==1) ? ix+1 : ix;										// V始点同一の場合
	for(i=0; i<vn; i++) {												// <<V方向ﾄﾗｯｶｰ>>
		for(j=0; j<DIM; j++) {
			pc[ix+i*un].p[j] = pb[i].p[j]-pa[ix+i*un].p[j];				// 制御点移動量算出
		}
		i1 = ia[ix+i*un]%2;												// V曲線ﾓｰﾄﾞ保存
		ia[ix+i*un] = i1+2*ib[i];										// 曲線ﾓｰﾄﾞ設定
	}
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
	sc = fabs(1.0/cos(th/2.0));											// 断面拡大倍率
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

/*************************************************************/
void SetSweepCtrl(int pl, int un, int vn, int ub,
				  PNTTYPE* pa, int* ia, PNTTYPE* pb, int* ib)			//<<< 掃引曲面制御点変換
/*************************************************************/
{
	PNTTYPE* up = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* vp = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* wvp = new PNTTYPE[View->m_MaxCPnt];
	int*     ui = new int[View->m_MaxCPnt];
	int*     vi = new int[View->m_MaxCPnt];
	PNTTYPE  wp[3], pt;
	VECTYPE  vec, rvc;
	int      i, j, k, d1, d2, d3;
	double   rota[3][3], rotb[3][3], th;

	for(i=0; i<un; i++) {
		up[i] = pa[i], ui[i] = ia[i];									// U制御点情報抽出
	}
	for(i=0; i<vn; i++) {
		vp[i] = pa[i+un], vi[i] = ia[i+un]*2;							// V制御点情報抽出
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] = up[1].p[i]-up[0].p[i];								// ﾍﾞｰｽ線ﾍﾞｸﾄﾙ取得
	}
	VecN(&vec);															// 単位ﾍﾞｸﾄﾙ化
	if(pl==-1) {														// <平面指定無に場合(新版)>
		d1 = 0, d2 = 1, d3 = 2;											// 座標軸設定
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// 基準軸角度分回転
		th = atan2(vec.p[1], vec.p[0]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[2])>EPSILON) {									// <基準平面上でない場合>
			for(i=0; i<vn; i++) {
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
		d1 = pl;														// 座標軸設定
        d2 = (d1+1>2) ? 0 : d1+1;
        d3 = (d2+1>2) ? 0 : d2+1;
		rvc.p[d1] = 0.0, rvc.p[d2] = 0.0, rvc.p[d3] = -1.0;				// 基準軸(d1)角度分回転
		th = atan2(vec.p[d1], vec.p[d2]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[d3])>EPSILON) {									// <基準平面上でない場合>
			for(i=0; i<vn; i++) {
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
	for(i=0; i<vn; i++) {												// <<ﾍﾞｰｽ線始点処理>>
		pt = vp[i];
		VecRotateMove(&pt, rota, up[0]);								// ﾍﾞｰｽ線始点垂直に回転移動
		pb[i*un] = pt, ib[i*un] = vi[i]+ui[0];
		VecRotate(&vp[i], rota);										// 始点垂直に回転
	}
	for(i=1; i<un-1; i++) {												// <<ﾍﾞｰｽ線中間点処理>>
		CalcRotAngleVec(&up[i-1], &th, &vec);							// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		SetRotateMatrix(vec, th, rotb);									// 次回点用ﾏﾄﾘｯｸｽ
		if(!ui[i]) {													// <掃引方向が曲線点>
			for(j=0; j<vn; j++) {
				pt = vp[j];
				VecRotateMove(&pt, rota, up[i]);						// 前後点中間角度に回転移動
				pb[j*un+i] = pt, ib[j*un+i] = vi[j]+ui[i];
				VecRotate(&vp[j], rotb);								// 次点垂直に回転
			}
		} else {														// <掃引方向が折線点>
			for(j=0; j<vn; j++) {
				wvp[j] = vp[j];
				VecRotate(&wvp[j], rota);								// 前後点中間角度に回転
				VecRotate(&vp[j], rotb);								// 次点垂直に回転
			}
			ScaleSection(wvp, vn, vec, th, d1, d2, d3);					// 断面拡大
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pb[j*un+i].p[k] = wvp[j].p[k]+up[i].p[k];			// 掃引点に移動
				}
				ib[j*un+i] = vi[j]+ui[i];
			}
		}
	}
	if(!ub) {															// <<<境界:開の場合>>>
		for(i=0; i<vn; i++) {											// <<ﾍﾞｰｽ線終点処理>>
			pt = vp[i];
			for(j=0; j<DIM; j++) {
				pt.p[j] += up[un-1].p[j];								// 終点移動
			}
			pb[i*un+un-1] = pt, ib[i*un+un-1] = vi[i]+ui[un-1];
		}
	} else {															// <<<境界:閉の場合>>>
		wp[0] = up[un-2], wp[1] = up[un-1], wp[2] = up[0];				// 終点と始点を連結
		CalcRotAngleVec(wp, &th, &vec);									// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		SetRotateMatrix(vec, th, rotb);									// 次回点用ﾏﾄﾘｯｸｽ
		if(!ui[un-1]) {													// <掃引方向が曲線点>
			for(i=0; i<vn; i++) {
				pt = vp[i];
				VecRotateMove(&pt, rota, up[un-1]);						// 終了断面
				pb[i*un+un-1] = pt, ib[i*un+un-1] = vi[i]+ui[un-1];
				VecRotate(&vp[i], rotb);								// 次点垂直に回転
			}
		} else {														// <掃引方向が折線点>
			for(i=0; i<vn; i++) {
				wvp[i] = vp[i];
				VecRotate(&wvp[i], rota);								// 前後点中間角度に回転
				VecRotate(&vp[i], rotb);								// 次点垂直に回転
			}
			ScaleSection(wvp, vn, vec, th, d1, d2, d3);					// 断面拡大
			for(i=0; i<vn; i++) {										// 終了断面
				for(j=0; j<DIM; j++) {
					pb[i*un+un-1].p[j] = wvp[i].p[j]+up[un-1].p[j];
				}
				ib[i*un+un-1] = vi[i]+ui[un-1];
			}
		}
		wp[0] = up[un-1], wp[1] = up[0], wp[2] = up[1];					// 終点と始点を連結
		CalcRotAngleVec(wp, &th, &vec);									// 前後点の角度取得
		SetRotateMatrix(vec, th/2.0, rota);								// 今回点用ﾏﾄﾘｯｸｽ
		if(!ui[0]) {													// <掃引方向が曲線点>
			for(i=0; i<vn; i++) {
				pt = vp[i];
				VecRotateMove(&pt, rota, up[0]);						// 開始断面
				pb[i*un] = pt;
			}
		} else {														// <掃引方向が折線点>
			for(i=0; i<vn; i++) {
				VecRotate(&vp[i], rota);								// 前後点中間角度に回転
			}
			ScaleSection(vp, vn, vec, th, d1, d2, d3);					// 断面拡大
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pb[i*un].p[j] = vp[i].p[j]+up[0].p[j];				// 開始断面
				}
			}
		}
	}
	delete[] up;
	delete[] vp;
	delete[] wvp;
	delete[] ui;
	delete[] vi;
}

/********************************************************/
void DeleteSweep(int wn, int* iw, int ub, int vb,
				 int* un, int* vn, PNTTYPE* pa, int* ia)				//<<< 掃引制御点列削除
/********************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCCtA*2];
	int*     pi = new int[View->m_MaxCCtA*2];
	int*     df = new int[View->m_MaxCCtA*2];
	int      i, ix, pn, uu, vv, uc, vc;

	uu = *un, uc = *un, vv = *vn, vc = *vn;
	pn = uu+vv;															// 総制御点数
	for(i=0; i<pn; i++) {
		df[i] = 0;														// 削除ﾌﾗｸﾞ初期化
	}
	for(i=0; i<wn; i++) {
		if(iw[i]) {														// <削除対象の場合>
			if(i<vv) {													// V方向指定
				df[uu+i] = 1, vc--;
			} else {													// U方向指定
				df[i-vv] = 1, uc--;
			}
		}
	}
	for(i=0, ix=0; i<pn; i++) {											// <削除対象外のみ>
		if(!df[i]) {													// 制御点,曲線ﾓｰﾄﾞ複写
			pp[ix] = pa[i], pi[ix++] = ia[i];
		}
	}
	for(i=0; i<ix; i++) {
		pa[i] = pp[i], ia[i] = pi[i];									// 制御点,曲線ﾓｰﾄﾞ復元
	}
	if(!ub) {
		ia[0] = 1, ia[uc-1] = 1;
	}
	if(!vb) {
		ia[uc] = 1, ia[uc+vc-1] = 1;
	}
	*un = uc, *vn = vc;													// 削除後U,V制御点数
	delete[] pp;
	delete[] pi;
	delete[] df;
}

/*********************************************************/
void MoveSweep(PNTTYPE* ma, int pl, int un, int vn,
			   PNTTYPE* pa, int wn, int* iw, PNTTYPE* pb)				//<<< 掃引制御点移動
/*********************************************************/
{
	PNTTYPE pt;
	VECTYPE vec, rvc;
	int     i, j, d1, d2, d3;
	double  rota[3][3], rotb[3][3], th;

	for(i=0; i<DIM; i++) {
		vec.p[i] = pa[1].p[i]-pa[0].p[i];								// ﾍﾞｰｽ線ﾍﾞｸﾄﾙ取得
	}
	VecN(&vec);															// 単位ﾍﾞｸﾄﾙ化
	if(pl==-1) {														// <平面指定無に場合(新版)>
		d1 = 1, d2 = 0, d3 = 2;											// 座標軸設定
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// 基準軸角度分回転
		th = atan2(vec.p[1], vec.p[0]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, -th, rota);
		if(fabs(vec.p[2])>EPSILON) {									// <基準平面上でない場合>
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
			SetRotateMatrix(rvc, th, rotb);
		}
	} else {															// <平面指定有の場合(旧版)>
		d1 = pl;														// 座標軸設定
        d2 = (d1+1>2) ? 0 : d1+1;
        d3 = (d2+1>2) ? 0 : d2+1;
		rvc.p[d1] = 0.0, rvc.p[d2] = 0.0, rvc.p[d3] = -1.0;				// 基準軸(d1)角度分回転
		th = atan2(vec.p[d1], vec.p[d2]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, -th, rota);
		if(fabs(vec.p[d3])>EPSILON) {									// <基準平面上でない場合>
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
			SetRotateMatrix(rvc, th, rotb);
		}
	}
	for(i=0; i<wn; i++) {
		pb[i] = pa[i];													// 制御点複写
	}
	for(i=0; i<wn; i++) {
		if(iw[i]) {														// <<移動対象の場合>>
			if(i<vn) {													// <V方向指定>
				pt = ma[i];												// 移動量を断面上に変換
				VecRotate(&pt, rota);									// ﾍﾞｰｽ線始点垂直から戻す
				if(fabs(vec.p[d3])>EPSILON) {							// 基準平面上でない場合
					VecRotate(&pt, rotb);
				}
				pt.p[d2] = 0;
				for(j=0; j<DIM; j++) {
					pb[un+i].p[j] += pt.p[j];							// 移動量分加算
				}
			} else {													// <U方向指定>
				for(j=0; j<DIM; j++) {
					pb[i-vn].p[j] += ma[i].p[j];						// 移動量分加算
				}
			}
		}
	}
}

/********************************************************/
void InsertSweep(int ix, int flg, int ub,
				 int* un, int* vn, PNTTYPE* pa, int* ia)				//<<< 掃引制御点列挿入
/********************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCCtA*2];
	int*     pi = new int[View->m_MaxCCtA*2];
	int      i, j, uu, vv, ul, i1, i2, pc=0;

	uu = *un, vv = *vn, ul = uu;
	ul = (!ub) ? ul-1 : ul;												// U開の場合
	if(ix<ul) {															// <<U方向指定>>
		for(i=0; i<uu; i++) {
			pp[pc] = pa[i], pi[pc++] = ia[i];							// 制御点･曲線ﾓｰﾄﾞ複写
			if(i==ix) {													// <<挿入位置>>
				if(i==uu-1) {											// <U閉の始終点>
					i1 = uu-1, i2 = 0;
				} else {												// <上記以外>
					i1 = i, i2 = i+1;
				}
				for(j=0; j<DIM; j++) {
					pp[pc].p[j] = (pa[i1].p[j]+pa[i2].p[j])/2.0;		// 中間点座標
				}
				pi[pc++] = flg;											// 曲線ﾓｰﾄﾞ設定
			}
		}
		for(i=0; i<vv; i++) {
			pp[pc] = pa[i+uu], pi[pc++] = ia[i+uu];						// 制御点･曲線ﾓｰﾄﾞ複写
		}
		uu++;															// U列数加算
	} else {															// <<V方向指定>>
		for(i=0; i<uu; i++) {
			pp[pc] = pa[i], pi[pc++] = ia[i];							// 制御点･曲線ﾓｰﾄﾞ複写
		}
		ix = ix-ul;
		for(i=0; i<vv; i++) {
			pp[pc] = pa[i+uu], pi[pc++] = ia[i+uu];						// 制御点･曲線ﾓｰﾄﾞ複写
			if(i==ix) {													// <<挿入位置>>
				if(i==vv-1) {											// <V閉の始終点>
					i1 = vv-1+uu, i2 = uu;
				} else {												// <上記以外>
					i1 = i+uu, i2 = i+1+uu;
				}
				for(j=0; j<DIM; j++) {
					pp[pc].p[j] = (pa[i1].p[j]+pa[i2].p[j])/2.0;		// 中間点座標
				}
				pi[pc++] = flg;											// 曲線ﾓｰﾄﾞ設定
			}
		}
		vv++;															// V列数加算
	}
	pc = uu+vv;															// 総制御点数
	for(i=0; i<pc; i++) {
		pa[i] = pp[i], ia[i] = pi[i];									// 制御点･曲線ﾓｰﾄﾞ復元
	}
	*un = uu, *vn = vv;													// 挿入後U/V制御点数
	delete[] pp;
	delete[] pi;
}

/*****************************************************************/
void CtrlPointSpace(PNTTYPE* pa, PNTTYPE* pb, int* ia, int an,
                    PNTTYPE rcp, PNTTYPE scp, int flg, int plane,
                    double dx, double dy, double dz, double rx,
                    double ry, double rz, double sx, double sy,
                    double sz, double mv)								//<<< 制御点ｽﾍﾟｰｽ(CPS)
/*****************************************************************/
{
	PNTTYPE pnt, spt, ept, dis, dsx, dsy, dsz, zer;
	VECTYPE vec;
	int     i, j, i1, i2, i3;
	double  wk, tx, ty, tz, mat[3][3], matx[3][3], maty[3][3], matz[3][3];

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;
	if(flg==0) {														// <ｻｲｽﾞ変更の場合>
		tx = (sx<0.01) ? 0.01 : sx;										// 有効倍率ﾁｪｯｸ
		ty = (sy<0.01) ? 0.01 : sy;
		tz = (sz<0.01) ? 0.01 : sz;
		mat[0][0] = tx,  mat[0][1] = 0.0, mat[0][2] = 0.0;				// ｻｲｽﾞ変更ﾏﾄﾘｯｸｽ設定
		mat[1][0] = 0.0, mat[1][1] = ty,  mat[1][2] = 0.0;
		mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = tz;
		for(i=0; i<DIM; i++) {											// 移動量計算
			for(wk=0.0, j=0; j<DIM; j++) {
				wk += mat[i][j]*scp.p[j];
			}
			dis.p[i] = scp.p[i]-wk;
		}
	} else {															// <放射状移動の場合>
		i1 = plane;														// 座標軸設定
		i2 = (i1+1>2) ? 0 : i1+1;
		i3 = (i2+1>2) ? 0 : i2+1;
		spt = scp, spt.p[i3] = 0.0;										// ｻｲｽﾞ変更基準点が始点
	}
	tx = rx*PI/180.0, ty = ry*PI/180.0, tz = rz*PI/180.0;				// 回転角度変換
	vec.p[0] = -1.0, vec.p[1] = 0.0, vec.p[2] = 0.0;					// X軸用回転情報
	SetRotateMatrix(vec, tx, matx);										// 回転ﾏﾄﾘｯｸｽ設定
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {									// 移動量計算
			wk += matx[i][j]*rcp.p[j];
		}
		dsx.p[i] = rcp.p[i]-wk;
	}
	vec.p[0] = 0.0, vec.p[1] = -1.0, vec.p[2] = 0.0;					// Y軸用回転情報
	SetRotateMatrix(vec, ty, maty);										// 回転ﾏﾄﾘｯｸｽ設定
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {									// 移動量計算
			wk += maty[i][j]*rcp.p[j];
		}
		dsy.p[i] = rcp.p[i]-wk;
	}
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = -1.0;					// Z軸用回転情報
	SetRotateMatrix(vec, tz, matz);										// 回転ﾏﾄﾘｯｸｽ設定
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {									// 移動量計算
			wk += matz[i][j]*rcp.p[j];
		}
		dsz.p[i] = rcp.p[i]-wk;
	}
	for(i=0; i<an; i++) {
		if(ia[i]==1) {													// <<移動対象の制御点>>
			pnt = pa[i];
			if(flg==0) {												// <ｻｲｽﾞ変更の場合>
				VecRotateMove(&pnt, mat, dis);							// 頂点座標ｻｲｽﾞ変更
			} else {													// <放射状移動の場合>
				ept = pnt, ept.p[i3] = 0.0;								// 終点
				VecV(&spt, &ept, &vec);									// 始点->終点ﾍﾞｸﾄﾙ
				VecN(&vec);												// 単位ﾍﾞｸﾄﾙ化(3D)
				for(j=0; j<DIM; j++) {
					pnt.p[j] += mv*vec.p[j];							// 基準点から放射状に移動
				}
			}
			VecRotateMove(&pnt, matx, dsx);								// 頂点座標回転移動(X軸)
			VecRotateMove(&pnt, maty, dsy);								// 頂点座標回転移動(Y軸)
			VecRotateMove(&pnt, matz, dsz);								// 頂点座標回転移動(Z軸)
			pnt.p[0] += dx, pnt.p[1] += dy, pnt.p[2] += dz;				// 移動量分補正
			for(j=0; j<DIM; j++) {
				pb[i].p[j] = pnt.p[j]-pa[i].p[j];						// 移動前制御点との差
			}
		} else {
			pb[i] = zer;												// 対象外はゼロ
		}
	}
}
