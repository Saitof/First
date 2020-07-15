/////////////////////////////////////////////////////////////////////////////
// 立体編集コマンド部
// EditCommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/14 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "NumFunc.h"

/*************************/
OBJTYPE* MoveObject(void)												//<<< 平行移動ｺﾏﾝﾄﾞ
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, vnum, ix, iy, iz;
	double   dx, dy, dz;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		ix = GetParaInt(cmp, 0);										// 移動量取得
		iy = GetParaInt(cmp, 1);
		iz = GetParaInt(cmp, 2);
		if(ix==0&&iy==0&&iz==0) {										// 移動量無終了
			return op;
		}
		dx = ix/1000.0, dy = iy/1000.0, dz = iz/1000.0;					// 移動量実数変換
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// 頂点座標取得
			pnt.p[0] += dx, pnt.p[1] += dy, pnt.p[2] += dz;				// 移動量分補正
			PutVtx(op, i, pnt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽ設定
		}
	}
	return op;
}

/***************************/
OBJTYPE* RotateObject(void)												//<<< 回転移動ｺﾏﾝﾄﾞ
/***************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  cp, dis, pnt;
	VECTYPE  vec;
	int      i, j, vnum;
	double   th, rot[3][3], wk;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		if((i=GetParaInt(cmp, 0))==0) {									// 回転角度取得
			return op;
		}
		th = i/1000.0*PI/180.0;
		GetParaPnt(cmp, 0, &cp);										// 回転軸座標取得
		GetParaPnt(cmp, 1, &pnt);										// 回転軸方向ﾍﾞｸﾄﾙ取得
		vec.p[0] = pnt.p[0], vec.p[1] = pnt.p[1], vec.p[2] = pnt.p[2];
		VecN(&vec);														// 単位ﾍﾞｸﾄﾙ化
		SetRotateMatrix(vec, th, rot);									// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// 移動量計算
				wk += rot[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		vnum = GetVtxNum(op);											// 頂点座標数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// 頂点座標取得
			VecRotateMove(&pnt, rot, dis);								// 頂点座標回転移動
			PutVtx(op, i, pnt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/***************************/
OBJTYPE* MirrorObject(void)												//<<< 鏡像移動ｺﾏﾝﾄﾞ
/***************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt, spt, ept, pt;
	int      i, j, vnum, lnum, stp, bln, fln, vno1, vno2, flg1, flg2;
	double   as, bs, cs, ds, rs, x1, y1, z1, x2, y2, z2;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		i = (GetParaInt(cmp, 0)-1<0) ? 2 : GetParaInt(cmp, 0)-1;		// 面方程式正規化
		GetParaPnt(cmp, 0, &spt);
		GetParaPnt(cmp, 1, &ept);
		pt = spt, pt.p[i] += 1.0;
		x1 = ept.p[0]-spt.p[0], y1 = ept.p[1]-spt.p[1], z1 = ept.p[2]-spt.p[2];
		x2 = pt.p[0]-spt.p[0], y2 = pt.p[1]-spt.p[1], z2 = pt.p[2]-spt.p[2];
		as = (y1*z2)-(z1*y2), bs = (z1*x2)-(x1*z2), cs = (x1*y2)-(y1*x2);
		ds = -(ept.p[0]*as+ept.p[1]*bs+ept.p[2]*cs);
		rs = sqrt(as*as+bs*bs+cs*cs);
		as /= rs, bs /= rs, cs /= rs, ds /= rs;
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// 頂点座標取得
			rs = (as*pnt.p[0]+bs*pnt.p[1]+cs*pnt.p[2]+ds)*2.0;
			pnt.p[0] -= rs*as, pnt.p[1] -= rs*bs, pnt.p[2] -= rs*cs;	// 座標値変転
			PutVtx(op, i, pnt);											// 頂点座標更新
		}
		lnum = GetFlpNum1(op);											// 面ﾙｰﾌﾟ1数取得
		for(stp=-1, i=0; i<lnum; i++) {
			GetFlp1(op, i, &vno1, &flg1);								// 面ﾙｰﾌﾟ1情報取得
			if(flg1) {													// <面ﾙｰﾌﾟ終了の場合>
				bln = i, fln = (i-stp)/2;								// 該当ﾙｰﾌﾟ最後尾
				for(j=stp+1; j<stp+1+fln; j++) {						// 該当ﾙｰﾌﾟ最前列より
					GetFlp1(op, j, &vno1, &flg1);						// 最後尾と頂点番号入替
					GetFlp1(op, bln, &vno2, &flg2);
					PutFlp1(op, j, vno2, flg1);
					PutFlp1(op, bln, vno1, flg2);
					bln--;
				}
				stp = i;												// 次面ﾙｰﾌﾟ開始位置(+1)
			}
		}
		lnum = GetFlpNum2(op);											// 面ﾙｰﾌﾟ数2取得
		for(stp=-1, i=0; i<lnum; i++) {
			GetFlp2(op, i, &vno1, &flg1);								// 面ﾙｰﾌﾟ2情報取得
			if(flg1) {													// <面ﾙｰﾌﾟ終了の場合>
				bln = i, fln = (i-stp)/2;								// 該当ﾙｰﾌﾟ最後尾
				for(j=stp+1; j<stp+1+fln; j++) {						// 該当ﾙｰﾌﾟ最前列より
					GetFlp2(op, j, &vno1, &flg1);						// 最後尾と頂点番号入替
					GetFlp2(op, bln, &vno2, &flg2);
					PutFlp2(op, j, vno2, flg1);
					PutFlp2(op, bln, vno1, flg2);
					bln--;
				}
				stp = i;												// 次面ﾙｰﾌﾟ開始位置(+1)
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/**************************/
OBJTYPE* ScaleObject(void)												//<<< ｻｲｽﾞ変更ｺﾏﾝﾄﾞ
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  cp;
	int      ix, iy, iz;
	double   sx, sy, sz, mat[3][3];

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		ix = GetParaInt(cmp, 0);										// ｻｲｽﾞ変更倍率取得
		iy = GetParaInt(cmp, 1);
		iz = GetParaInt(cmp, 2);
		if(ix==1000&&iy==1000&&iz==1000) {								// 等倍は終了
			return op;
		}
		GetParaPnt(cmp, 0, &cp);										// 変換中心座標取得
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// 倍率実数変換
		mat[0][0] = sx,  mat[0][1] = 0.0, mat[0][2] = 0.0;				// ﾏﾄﾘｯｸｽ設定
		mat[1][0] = 0.0, mat[1][1] = sy,  mat[1][2] = 0.0;
		mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = sz;
		Affine(op, cp, mat);											// ｱﾌｨﾝ変換(ｻｲｽﾞ変更)
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/*************************/
OBJTYPE* BendObject(void)												//<<< 円筒曲げｺﾏﾝﾄﾞ
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  sp, ep, cp, rp, pnt, wsp[2], wep[2];
	VECTYPE  vec;
	int      i, vnum, fc, vc, dc, ith;
	double   dth, ln, dd, rr, wth, rots[3][3], rote[3][3];

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		fc = GetParaInt(cmp, 0);										// 曲げ水平方向取得
		vc = GetParaInt(cmp, 1);										// 曲げ垂直方向取得
		ith = GetParaInt(cmp, 2);										// 曲げ角度取得
		GetParaPnt(cmp, 0, &sp);										// 基準線始点取得
		GetParaPnt(cmp, 1, &ep);										// 基準線終点取得
		ln = ep.p[fc]-sp.p[fc];											// 曲げ弧長さ
		if(ith==0||fabs(ln)<EPSILON) {									// 角度/弧の長さ=0は中止
			return op;
		}
		dc = (fc+vc==1) ? 2 : ((fc+vc==2) ? 1 : 0);						// 曲げ奥行方向取得
		dth = ith/1000.0;												// 実数角度算出
		dd = (ln*360.0)/(dth*PI*2.0);									// 曲げ弧の半径
		dth = (dth*PI)/360.0;											// 回転する角度(ﾗｼﾞｱﾝ)算出
		vec.p[fc] = 0.0, vec.p[vc] = 0.0;								// 回転ﾍﾞｸﾄﾙ設定
		vec.p[dc] = ((fc==0&&vc==2)||(fc==1&&vc==0)||(fc==2&&vc==1))
                    ? -1.0 : 1.0;
		for(i=0; i<DIM; i++) {
			cp.p[i] = (sp.p[i]+ep.p[i])/2;								// 基準点1(曲げ前)算出
		}
		rp = cp, rp.p[vc] = cp.p[vc]-dd;								// 基準点2(曲げ後)算出
		SetRotateMatrix(vec, dth, rots);								// 回転ﾏﾄﾘｯｸｽ作成
		wsp[0] = sp;
		VecRotate(&wsp[0], rots);										// 始点回転後位置算出
		pnt = sp;
		rr = dd+pnt.p[vc]-cp.p[vc];										// 始点曲げ後位置算出
		wth = (pnt.p[fc]-cp.p[fc])/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wsp[1] = pnt;
		for(i=0; i<DIM; i++) {
			wsp[0].p[i] = wsp[1].p[i]-wsp[0].p[i];						// 回転と曲げ誤差算出
		}
		SetRotateMatrix(vec, -dth, rote);								// 回転ﾏﾄﾘｯｸｽ作成
		wep[0] = ep;
		VecRotate(&wep[0], rote);										// 終点回転後位置算出
		pnt = ep;
		rr = dd+pnt.p[vc]-cp.p[vc];										// 終点曲げ後位置算出
		wth = (pnt.p[fc]-cp.p[fc])/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wep[1] = pnt;
		for(i=0; i<DIM; i++) {
			wep[0].p[i] = wep[1].p[i]-wep[0].p[i];						// 回転と曲げ誤差算出
		}
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// 頂点座標取得
			if(sp.p[fc]>pnt.p[fc]) {									// <回転(左側)の場合>
				VecRotateMove(&pnt, rots, wsp[0]);						// 回転移動(左側)
			} else if(ep.p[fc]<pnt.p[fc]) {								// <回転(右側)の場合>
				VecRotateMove(&pnt, rote, wep[0]);						// 回転移動(右側)
			} else {													// <曲げ(中央)の場合>
				rr = dd+pnt.p[vc]-cp.p[vc];								// 半径算出(基準点1->)
				wth = (pnt.p[fc]-cp.p[fc])/dd;							// 角度算出(基準点2->)
				pnt.p[fc] = rr*sin(wth)+rp.p[fc];						// 曲げ後位置算出後に
				pnt.p[vc] = rr*cos(wth)+rp.p[vc];						// 基準点2に移動
			}
			PutVtx(op, i, pnt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/**************************/
OBJTYPE* TwistObject(void)												//<<< ひねりｺﾏﾝﾄﾞ
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt, cpt, dis;
	VECTYPE  vec;
	int      i, j, k, dir, ith, iln, num;
	double   dth, dln, wth, wk, rot[3][3];

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		dir = GetParaInt(cmp, 0);										// ひねり軸方向取得
		ith = GetParaInt(cmp, 1);										// ひねり角度取得
		iln = GetParaInt(cmp, 2);										// ひねり軸幅取得
		if(ith==0||iln==0) {											// 角度/軸幅無中止
			return op;
		}
		GetParaPnt(cmp, 0, &cpt);										// ひねり基準点取得
		dth = ith/1000.0;												// 実数角度算出
		dln = iln/1000.0;												// 実数軸幅算出
		vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 0.0;
		vec.p[dir] = -1.0;
		num = GetVtxNum(op);											// 頂点座標数取得
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pnt);										// 頂点座標取得
			wk = pnt.p[dir]-cpt.p[dir];									// 該当位置距離
			wth = PI*dth*wk/(180.0*dln);								// 該当位置角度
			SetRotateMatrix(vec, wth, rot);								// 回転ﾏﾄﾘｯｸｽ設定
			for(j=0; j<DIM; j++) {										// 移動量計算
				for(wk=0.0, k=0; k<DIM; k++) {
					wk += rot[j][k]*cpt.p[k];
				}
				dis.p[j] = cpt.p[j]-wk;
			}
			VecRotateMove(&pnt, rot, dis);								// 頂点座標回転移動
			PutVtx(op, i, pnt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject1(void)											//<<< 直線指定ﾘｻｲｽﾞ1ｺﾏﾝﾄﾞ
/****************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pt, p1, p2, p3;
	int      i, j=0, pln, mpf, s1, s2, s3, i1, i2, sf1, ef1, ef2, num;
	double   sp1, sp2, ep1, ep2, ln1, ln2, bp, wln, ss1, ss2, es1, es2, wsc;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		pln = GetParaInt(cmp, 0);										// 指定平面取得
		mpf = GetParaInt(cmp, 1);										// 正負ﾌﾗｸﾞ取得
		s1 = GetParaInt(cmp, 2);										// 倍率1取得
		s2 = GetParaInt(cmp, 3);										// 倍率2取得
		if(s2==-1000) {													// <バグ対応 07.01.09>
			s2 = -1;													// -1000では、Ver2.0.2以下
			PutParaInt(cmp, 3, s2);										// 未対応のため、-1 に変更
		}
		s3 = GetParaInt(cmp, 4);										// 倍率3取得
		GetParaPnt(cmp, 0, &p1);										// 基準点1取得
		GetParaPnt(cmp, 1, &p2);										// 基準点2取得
		GetParaPnt(cmp, 2, &p3);										// 基準点3取得
		for(i=0; i<DIM; i++) {
			if(fabs(p1.p[i]-p3.p[i])>EPSILON) {							// 基準線座標軸確認
				j++; i1 = i;
			}
		}
		if(j!=1) {														// 基準線無中止
			return op;
		}
		s1 = (s1>=0&&s1<10) ? 10 : s1;
		s2 = (s2>=0&&s2<10) ? 10 : s2;
		s3 = (s3>=0&&s3<10) ? 10 : s3;
		sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
		sf1 = 0,   ef1 = 0,   ef2 = 0;
		ss2 = 1.0, es2 = 1.0;
		if(s2<0) {														// <2点ﾓｰﾄﾞ>
			if(s1==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// 基準点位置
				sp1 = p1.p[i1], ep1 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep1 = p1.p[i1];
			}
			ln1 = ep1-sp1;												// 基準点間距離
			ss1 = s1/1000.0, es1 = s3/1000.0;							// 基準倍率
			sf1 = (s1!=1000) ? 1 : sf1;									// 倍率!=1.0
			ef1 = (s3!=1000) ? 1 : ef1;
		} else {														// <3点ﾓｰﾄﾞ>
			if(s1==1000&&s2==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// 基準点位置
				sp1 = p1.p[i1], ep2 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep2 = p1.p[i1];
			}
			ep1 = p2.p[i1],  sp2 = p2.p[i1];
			ln1 = ep1-sp1,   ln2 = ep2-sp2;								// 基準点間距離
			ss1 = s1/1000.0, es1 = s2/1000.0;							// 基準倍率
			ss2 = s2/1000.0, es2 = s3/1000.0;
			sf1 = (s1!=1000) ? 1 : sf1;									// 倍率!=1.0
			ef2 = (s3!=1000) ? 1 : ef2;
		}
		i2 = (pln==i1) ? ((pln+1>2) ? 0 : pln+1) : pln;					// ﾘｻｲｽﾞ座標軸取得
		bp = p1.p[i2];													// ﾘｻｲｽﾞ基準点
		num = GetVtxNum(op);											// 頂点座標数取得
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pt);											// 頂点座標取得
			if(mpf==2||(mpf==0&&pt.p[i2]<bp)||(mpf==1&&pt.p[i2]>bp)) {	// <<正負方向一致>>
				if(pt.p[i1]>=sp1&&pt.p[i1]<=ep1) {						// <基準1範囲内>
					wln = pt.p[i1]-sp1;									// 始点からの距離
					wsc = ss1+(es1-ss1)*wln/ln1;						// 該当位置の倍率
					pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
				} else if(pt.p[i1]>sp2&&pt.p[i1]<=ep2) {				// <基準2範囲内>
					wln = pt.p[i1]-sp2;									// 始点からの距離
					wsc = ss2+(es2-ss2)*wln/ln2;						// 該当位置の倍率
					pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
				} else if(sf1==1&&pt.p[i1]<sp1) {						// <倍率!=1.0の基準1前>
					pt.p[i2] = ss1*(pt.p[i2]-bp)+bp;
				} else if(ef1==1&&pt.p[i1]>ep1) {						// <倍率!=1.0の基準1後>
					pt.p[i2] = es1*(pt.p[i2]-bp)+bp;
				} else if(ef2==1&&pt.p[i1]>ep2) {						// <倍率!=1.0の基準2後>
					pt.p[i2] = es2*(pt.p[i2]-bp)+bp;
				}
			}
			PutVtx(op, i, pt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject2(void)											//<<< 曲線指定ﾘｻｲｽﾞ1ｺﾏﾝﾄﾞ
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  bs, pt;
	int      i, j, f1, f2, d1, d2, ln, pn, vnum;
	double   dln, pr, lw, dd;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		f1 = GetParaInt(cmp, 0);										// 正負ﾌﾗｸﾞ取得
		f2 = GetParaInt(cmp, 1);										// 両側ﾌﾗｸﾞ取得
		d1 = GetParaInt(cmp, 2);										// 長さ方向取得
		d2 = GetParaInt(cmp, 3);										// ｻｲｽﾞ指定方向取得
		ln = GetParaInt(cmp, 4);										// 基準長さ取得
		pn = GetParaInt(cmp, 5);										// 制御点数取得
		if(ln==0||pn==0) {												// 基準長さ無/制御点無は中止
			delete[] pa;
			delete[] ia;
			return op;
		}
		dln = ln/1000.0;												// 基準長さ実数化
		for(i=0; i<pn; i++) {
			ia[i] = GetParaInt(cmp, i+6);								// 曲線ﾓｰﾄﾞ取得
		}
		GetParaPnt(cmp, 0, &bs);										// 基準点取得
		for(i=0; i<pn; i++) {
			GetParaPnt(cmp, i+1, &pa[i]);								// 制御点取得
		}
		MakeSCurve(&pn, 0, 3, 2.0, pa, ia);								// S曲線生成
		if(f1) {														// <正側指定の場合>
			for(i=0; i<pn; i++) {
				dd = pa[i].p[d2]-bs.p[d2];								// 基準点とのｻｲｽﾞ指定方向差
				pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// 負の場合は0
			}
		} else {														// <負側指定の場合>
			for(i=0; i<pn; i++) {
				dd = bs.p[d2]-pa[i].p[d2];								// 基準点とのｻｲｽﾞ指定方向差
	            pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// 負の場合は0
			}
		}
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// 頂点座標取得
			if(f2||(f1&&pt.p[d2]>bs.p[d2])||(!f1&&pt.p[d2]<bs.p[d2])) {	// <頂点が対象領域内>
				for(j=1; j<pn-1; j++) {									// 頂点の長さ方向位置が含
					if(pt.p[d1]<pa[j].p[d1]) {							// まれる曲線の線分を検索
						break;
					}
				}
				pr=(pt.p[d1]-pa[j-1].p[d1])/(pa[j].p[d1]-pa[j-1].p[d1]);// 線分内位置の比率
				pr = (pr<0.0) ? 0.0 : ((pr>1.0) ? 1.0 : pr);			// 比率が範囲外
				lw = (pa[j].p[d2]-pa[j-1].p[d2])*pr;					// 線分内のｻｲｽﾞ指定方向位置
				lw += pa[j-1].p[d2];									// 頂点位置のｻｲｽﾞ指定方向距離
				pr = lw/dln;											// ｻｲｽﾞ変更比率
				lw = pt.p[d2]-bs.p[d2];									// 基準点とのｻｲｽﾞ指定方向差
				pt.p[d2] = bs.p[d2]+lw*pr;								// ｻｲｽﾞ指定方向位置変更
				PutVtx(op, i, pt);										// 頂点座標更新
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽ設定
		}
	}
	delete[] pa;
	delete[] ia;
	return op;
}

/**************************/
OBJTYPE* ShearObject(void)												//<<< せん断ｺﾏﾝﾄﾞ
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt, bas;
	int      i, vnum, d1, d2;
	double   inc, ln;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		d1 = GetParaInt(cmp, 0);										// せん断垂直方向取得
		d2 = GetParaInt(cmp, 1);										// せん断水平方向取得
		if((i=GetParaInt(cmp, 2))==0) {									// 傾き取得
			return op;
		}
		inc = i/1000.0;
		GetParaPnt(cmp, 0, &bas);										// せん断基準点取得
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// 頂点座標取得
			ln = pnt.p[d1]-bas.p[d1];									// 基準点からの垂直方向距離
			ln = ln*inc;												// せん断水平方向移動量算出
			pnt.p[d2] += ln;											// 移動量分補正
			PutVtx(op, i, pnt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽ設定
		}
	}
	return op;
}

/*************************/
OBJTYPE* RingObject(void)												//<<< ﾘﾝｸﾞｺﾏﾝﾄﾞ
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  cp, sp, ep, rp, pt, ws[2], we[2];
	VECTYPE  vec;
	int      i, ix, iy, iz, fc, vc, it, dc, vnum;
	double   sx, sy, sz, ln, dt, dd, rr, wt, mat1[3][3], mat2[3][3];

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		ix = GetParaInt(cmp, 0);										// ｻｲｽﾞ変更倍率取得
		iy = GetParaInt(cmp, 1);
		iz = GetParaInt(cmp, 2);
		fc = GetParaInt(cmp, 3);										// 曲げ水平方向取得
		vc = GetParaInt(cmp, 4);										// 曲げ垂直方向取得
		it = GetParaInt(cmp, 5);										// 曲げ角度取得
		GetParaPnt(cmp, 0, &cp);										// 変換中心座標取得
		GetParaPnt(cmp, 1, &sp);										// 基準線始点取得
		GetParaPnt(cmp, 2, &ep);										// 基準線終点取得
		ln = ep.p[fc] - sp.p[fc];										// 曲げ弧長さ
		if(ix==1000&&iy==1000&&iz==1000&&(it==0||fabs(ln)<EPSILON)) {	// 等倍で角度/弧の長さ=0は中止
			return op;
		}
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// 倍率実数変換
		mat1[0][0] = sx,  mat1[0][1] = 0.0, mat1[0][2] = 0.0;			// ﾏﾄﾘｯｸｽ設定
		mat1[1][0] = 0.0, mat1[1][1] = sy,  mat1[1][2] = 0.0;
		mat1[2][0] = 0.0, mat1[2][1] = 0.0, mat1[2][2] = sz;
		Affine(op, cp, mat1);											// ｱﾌｨﾝ変換
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
		dc = (fc+vc==1) ? 2 : ((fc+vc==2) ? 1 : 0);						// 曲げ奥行方向取得
		dt = it/1000.0;													// 実数角度算出
		dd = (ln*360.0)/(dt*PI*2.0);									// 曲げ弧の半径
		dt = (dt*PI)/360.0;												// 回転する角度(ﾗｼﾞｱﾝ)算出
		vec.p[fc] = 0.0, vec.p[vc] = 0.0;								// 回転ﾍﾞｸﾄﾙ設定
		vec.p[dc] = ((fc==0&&vc==2)||(fc==1&&vc==0)||(fc==2&&vc==1))
                    ? -1.0 : 1.0;
		for(i=0; i<DIM; i++) {
			cp.p[i] = (sp.p[i]+ep.p[i])/2;								// 基準点1(曲げ前)算出
		}
		rp = cp, rp.p[vc] = cp.p[vc]-dd;								// 基準点2(曲げ後)算出
		SetRotateMatrix(vec, dt, mat1);									// 回転ﾏﾄﾘｯｸｽ作成
		ws[0] = sp;
		VecRotate(&ws[0], mat1);										// 始点回転後位置算出
		pt = sp;
		rr = dd+pt.p[vc]-cp.p[vc];										// 始点曲げ後位置算出
		wt = (pt.p[fc]-cp.p[fc])/dd;
		pt.p[fc] = rr*sin(wt)+rp.p[fc];
		pt.p[vc] = rr*cos(wt)+rp.p[vc];
		ws[1] = pt;
		for(i=0; i<DIM; i++) {
			ws[0].p[i] = ws[1].p[i]-ws[0].p[i];							// 回転と曲げ誤差算出
		}
		SetRotateMatrix(vec, -dt, mat2);								// 回転ﾏﾄﾘｯｸｽ作成
		we[0] = ep;
		VecRotate(&we[0], mat2);										// 終点回転後位置算出
		pt = ep;
		rr = dd+pt.p[vc]-cp.p[vc];										// 終点曲げ後位置算出
		wt = (pt.p[fc]-cp.p[fc])/dd;
		pt.p[fc] = rr*sin(wt)+rp.p[fc];
		pt.p[vc] = rr*cos(wt)+rp.p[vc];
		we[1] = pt;
		for(i=0; i<DIM; i++) {
			we[0].p[i] = we[1].p[i]-we[0].p[i];							// 回転と曲げ誤差算出
		}
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// 頂点座標取得
			if(sp.p[fc]>pt.p[fc]) {										// <回転(左側)の場合>
				VecRotateMove(&pt, mat1, ws[0]);						// 回転移動(左側)
			} else if(ep.p[fc]<pt.p[fc]) {								// <回転(右側)の場合>
				VecRotateMove(&pt, mat2, we[0]);						// 回転移動(右側)
			} else {													// <曲げ(中央)の場合>
				rr = dd+pt.p[vc]-cp.p[vc];								// 半径算出(基準点1->)
				wt = (pt.p[fc]-cp.p[fc])/dd;							// 角度算出(基準点2->)
				pt.p[fc] = rr*sin(wt)+rp.p[fc];							// 曲げ後位置算出後に
				pt.p[vc] = rr*cos(wt)+rp.p[vc];							// 基準点2に移動
			}
			PutVtx(op, i, pt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/**************************/
OBJTYPE* EarthObject(void)												//<<< ｱｰｽｺﾏﾝﾄﾞ
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rp, cp, pt, dsx, dsy, dsz;
	VECTYPE  rv;
	int      i, j, mx, my, mz, rx, ry, rz, ix, iy, iz, vnum;
	double   dx, dy, dz, tx, ty, tz, sx, sy, sz, wk;
	double   matx[3][3], maty[3][3], matz[3][3];

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		mx = GetParaInt(cmp, 0);										// 移動量取得
		my = GetParaInt(cmp, 1);
		mz = GetParaInt(cmp, 2);
		rx = GetParaInt(cmp, 3);										// 回転角度取得
		ry = GetParaInt(cmp, 4);
		rz = GetParaInt(cmp, 5);
		ix = GetParaInt(cmp, 6);										// ｻｲｽﾞ変更倍率取得
		iy = GetParaInt(cmp, 7);
		iz = GetParaInt(cmp, 8);
		if(mx==0&&my==0&&mz==0&&rx==0&&ry==0&&rz==0&&
           ix==1000&&iy==1000&&iz==1000) {								// 移動量無+角度無+等倍は終了
			return op;
		}
		dx = mx/1000.0, dy = my/1000.0, dz = mz/1000.0;					// 移動量実数変換
		tx = rx/1000.0*PI/180.0;										// 回転角度変換
		ty = ry/1000.0*PI/180.0;
		tz = rz/1000.0*PI/180.0;
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// 倍率実数変換
		GetParaPnt(cmp, 0, &rp);										// 回転軸座標取得
		GetParaPnt(cmp, 1, &cp);										// 変換中心座標取得
		matx[0][0] = sx,  matx[0][1] = 0.0, matx[0][2] = 0.0;			// ﾏﾄﾘｯｸｽ設定
		matx[1][0] = 0.0, matx[1][1] = sy,  matx[1][2] = 0.0;
		matx[2][0] = 0.0, matx[2][1] = 0.0, matx[2][2] = sz;
		Affine(op, cp, matx);											// ｱﾌｨﾝ変換(ｻｲｽﾞ変更)
		rv.p[0] = -1.0, rv.p[1] = 0.0, rv.p[2] = 0.0;					// X軸用回転情報
		SetRotateMatrix(rv, tx, matx);									// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// 移動量計算
				wk += matx[i][j]*rp.p[j];
			}
			dsx.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = -1.0, rv.p[2] = 0.0;					// Y軸用回転情報
		SetRotateMatrix(rv, ty, maty);									// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// 移動量計算
				wk += maty[i][j]*rp.p[j];
			}
			dsy.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = 0.0, rv.p[2] = -1.0;					// Z軸用回転情報
		SetRotateMatrix(rv, tz, matz);									// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// 移動量計算
				wk += matz[i][j]*rp.p[j];
			}
			dsz.p[i] = rp.p[i]-wk;
		}
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// 頂点座標取得
			VecRotateMove(&pt, matx, dsx);								// 頂点座標回転移動(X軸)
			VecRotateMove(&pt, maty, dsy);								// 頂点座標回転移動(X軸)
			VecRotateMove(&pt, matz, dsz);								// 頂点座標回転移動(X軸)
			pt.p[0] += dx, pt.p[1] += dy, pt.p[2] += dz;				// 移動量分補正
			PutVtx(op, i, pt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽ設定
		}
	}
	return op;
}

/**************************/
OBJTYPE* SpaceObject(void)												//<<< ｽﾍﾟｰｽｺﾏﾝﾄﾞ
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rp, cp, pt, dsx, dsy, dsz;
	VECTYPE  rv;
	int      i, j, mx, my, mz, rx, ry, rz, ix, iy, iz, vnum;
	double   dx, dy, dz, tx, ty, tz, sx, sy, sz, wk;
	double   matx[3][3], maty[3][3], matz[3][3];

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		mx = GetParaInt(cmp, 0);										// 移動量取得
		my = GetParaInt(cmp, 1);
		mz = GetParaInt(cmp, 2);
		rx = GetParaInt(cmp, 3);										// 回転角度取得
		ry = GetParaInt(cmp, 4);
		rz = GetParaInt(cmp, 5);
		ix = GetParaInt(cmp, 6);										// ｻｲｽﾞ変更倍率取得
		iy = GetParaInt(cmp, 7);
		iz = GetParaInt(cmp, 8);
		if(mx==0&&my==0&&mz==0&&rx==0&&ry==0&&rz==0&&
           ix==1000&&iy==1000&&iz==1000) {								// 移動量無+角度無+等倍は終了
			return op;
		}
		dx = mx/1000.0, dy = my/1000.0, dz = mz/1000.0;					// 移動量実数変換
		tx = rx/1000.0*PI/180.0;										// 回転角度変換
		ty = ry/1000.0*PI/180.0;
		tz = rz/1000.0*PI/180.0;
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// 倍率実数変換
		GetParaPnt(cmp, 0, &rp);										// 回転軸座標取得
		GetParaPnt(cmp, 1, &cp);										// 変換中心座標取得
		matx[0][0] = sx,  matx[0][1] = 0.0, matx[0][2] = 0.0;			// ﾏﾄﾘｯｸｽ設定
		matx[1][0] = 0.0, matx[1][1] = sy,  matx[1][2] = 0.0;
		matx[2][0] = 0.0, matx[2][1] = 0.0, matx[2][2] = sz;
		Affine(op, cp, matx);											// ｱﾌｨﾝ変換(ｻｲｽﾞ変更)
		rv.p[0] = -1.0, rv.p[1] = 0.0, rv.p[2] = 0.0;					// X軸用回転情報
		SetRotateMatrix(rv, tx, matx);									// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// 移動量計算
				wk += matx[i][j]*rp.p[j];
			}
			dsx.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = -1.0, rv.p[2] = 0.0;					// Y軸用回転情報
		SetRotateMatrix(rv, ty, maty);									// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// 移動量計算
				wk += maty[i][j]*rp.p[j];
			}
			dsy.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = 0.0, rv.p[2] = -1.0;					// Z軸用回転情報
		SetRotateMatrix(rv, tz, matz);									// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// 移動量計算
				wk += matz[i][j]*rp.p[j];
			}
			dsz.p[i] = rp.p[i]-wk;
		}
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// 頂点座標取得
			VecRotateMove(&pt, matx, dsx);								// 頂点座標回転移動(X軸)
			VecRotateMove(&pt, maty, dsy);								// 頂点座標回転移動(X軸)
			VecRotateMove(&pt, matz, dsz);								// 頂点座標回転移動(X軸)
			pt.p[0] += dx, pt.p[1] += dy, pt.p[2] += dz;				// 移動量分補正
			PutVtx(op, i, pt);											// 頂点座標更新
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽ設定
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject3(void)											//<<< 直線指定ﾘｻｲｽﾞ2ｺﾏﾝﾄﾞ
/****************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pt, p1, p2, p3;
	int      i, j=0, pln, mpf, s1, s2, s3, vflg, v1, v2;
	int      i1, i2, i3, sf1, ef1, ef2, num;
	double   sp1, sp2, ep1, ep2, ln1, ln2, bp;
	double   wln, ss1, ss2, es1, es2, wsc, vr1, vr2;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		pln = GetParaInt(cmp, 0);										// 指定平面取得
		mpf = GetParaInt(cmp, 1);										// 正負ﾌﾗｸﾞ取得
		s1 = GetParaInt(cmp, 2);										// 倍率1取得
		s2 = GetParaInt(cmp, 3);										// 倍率2取得
		s3 = GetParaInt(cmp, 4);										// 倍率3取得
		vflg = GetParaInt(cmp, 5);										// 垂直限定ﾌﾗｸﾞ取得
		v1 = GetParaInt(cmp, 6);										// 垂直範囲1取得
		v2 = GetParaInt(cmp, 7);										// 垂直範囲2取得
		GetParaPnt(cmp, 0, &p1);										// 基準点1取得
		GetParaPnt(cmp, 1, &p2);										// 基準点2取得
		GetParaPnt(cmp, 2, &p3);										// 基準点3取得
		for(i=0; i<DIM; i++) {
			if(fabs(p1.p[i]-p3.p[i])>EPSILON) {							// 基準線座標軸確認
				j++; i1 = i;
			}
		}
		if(j!=1) {														// 基準線無中止
			return op;
		}
		vr1 = (double)v1/1000.0;										// 垂直限定範囲実数変換
		vr2 = (double)v2/1000.0;
		i3 = (pln+1>2) ? 0 : pln+1;										// 垂直方向算出
		i3 = (i3+1>2) ? 0 : i3+1;
		s1 = (s1>=0&&s1<10) ? 10 : s1;
		s2 = (s2>=0&&s2<10) ? 10 : s2;
		s3 = (s3>=0&&s3<10) ? 10 : s3;
		sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
		sf1 = 0,   ef1 = 0,   ef2 = 0;
		ss2 = 1.0, es2 = 1.0;
		if(s2<0) {														// <2点ﾓｰﾄﾞ>
			if(s1==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// 基準点位置
				sp1 = p1.p[i1], ep1 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep1 = p1.p[i1];
			}
			ln1 = ep1-sp1;												// 基準点間距離
			ss1 = s1/1000.0, es1 = s3/1000.0;							// 基準倍率
			sf1 = (s1!=1000) ? 1 : sf1;									// 倍率!=1.0
			ef1 = (s3!=1000) ? 1 : ef1;
		} else {														// <3点ﾓｰﾄﾞ>
			if(s1==1000&&s2==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// 基準点位置
				sp1 = p1.p[i1], ep2 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep2 = p1.p[i1];
			}
			ep1 = p2.p[i1],  sp2 = p2.p[i1];
			ln1 = ep1-sp1,   ln2 = ep2-sp2;								// 基準点間距離
			ss1 = s1/1000.0, es1 = s2/1000.0;							// 基準倍率
			ss2 = s2/1000.0, es2 = s3/1000.0;
			sf1 = (s1!=1000) ? 1 : sf1;									// 倍率!=1.0
			ef2 = (s3!=1000) ? 1 : ef2;
		}
		i2 = (pln==i1) ? ((pln+1>2) ? 0 : pln+1) : pln;					// ﾘｻｲｽﾞ座標軸取得
		bp = p1.p[i2];													// ﾘｻｲｽﾞ基準点
		num = GetVtxNum(op);											// 頂点座標数取得
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pt);											// 頂点座標取得
			if((vflg==0)||												// <<<垂直方向限定なし/
               (vflg==1&&pt.p[i3]>vr1-EPSILON)||						//    下限指定で範囲内/
               (vflg==2&&pt.p[i3]<vr2+EPSILON)||						//    上限指定で範囲内
               (vflg==3&&pt.p[i3]>vr1-EPSILON&&pt.p[i3]<vr2+EPSILON)) {	//    上下指定で範囲内>>>
				if(mpf==2||(mpf==0&&pt.p[i2]<bp)||
                           (mpf==1&&pt.p[i2]>bp)) {						// <<正負方向一致>>
					if(pt.p[i1]>=sp1&&pt.p[i1]<=ep1) {					// <基準1範囲内>
						wln = pt.p[i1]-sp1;								// 始点からの距離
						wsc = ss1+(es1-ss1)*wln/ln1;					// 該当位置の倍率
						pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
					} else if(pt.p[i1]>sp2&&pt.p[i1]<=ep2) {			// <基準2範囲内>
						wln = pt.p[i1]-sp2;								// 始点からの距離
						wsc = ss2+(es2-ss2)*wln/ln2;					// 該当位置の倍率
						pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
					} else if(sf1==1&&pt.p[i1]<sp1) {					// <倍率!=1.0の基準1前>
						pt.p[i2] = ss1*(pt.p[i2]-bp)+bp;
					} else if(ef1==1&&pt.p[i1]>ep1) {					// <倍率!=1.0の基準1後>
						pt.p[i2] = es1*(pt.p[i2]-bp)+bp;
					} else if(ef2==1&&pt.p[i1]>ep2) {					// <倍率!=1.0の基準2後>
						pt.p[i2] = es2*(pt.p[i2]-bp)+bp;
					}
				}
				PutVtx(op, i, pt);										// 頂点座標更新
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject4(void)											//<<< 曲線指定ﾘｻｲｽﾞ2ｺﾏﾝﾄﾞ
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  bs, pt;
	int      i, j, f1, f2, d1, d2, d3, ln, vflg, v1, v2, pn, vnum;
	double   dln, pr, lw, dd, vr1, vr2;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		f1 = GetParaInt(cmp, 0);										// 正負ﾌﾗｸﾞ取得
		f2 = GetParaInt(cmp, 1);										// 両側ﾌﾗｸﾞ取得
		d1 = GetParaInt(cmp, 2);										// 長さ方向取得
		d2 = GetParaInt(cmp, 3);										// ｻｲｽﾞ指定方向取得
		d3 = GetParaInt(cmp, 4);										// 垂直方向取得
		ln = GetParaInt(cmp, 5);										// 基準長さ取得
		vflg = GetParaInt(cmp, 6);										// 垂直限定ﾌﾗｸﾞ取得
		v1 = GetParaInt(cmp, 7);										// 垂直範囲1取得
		v2 = GetParaInt(cmp, 8);										// 垂直範囲2取得
		pn = GetParaInt(cmp, 9);										// 制御点数取得
		if(ln==0||pn==0) {												// 基準長さ無/制御点無は中止
			delete[] pa;
			delete[] ia;
			return op;
		}
		dln = ln/1000.0;												// 基準長さ実数化
		vr1 = (double)v1/1000.0;										// 垂直限定範囲実数変換
		vr2 = (double)v2/1000.0;
		for(i=0; i<pn; i++) {
			ia[i] = GetParaInt(cmp, i+10);								// 曲線ﾓｰﾄﾞ取得
		}
		GetParaPnt(cmp, 0, &bs);										// 基準点取得
		for(i=0; i<pn; i++) {
			GetParaPnt(cmp, i+1, &pa[i]);								// 制御点取得
		}
		MakeSCurve(&pn, 0, 3, 2.0, pa, ia);								// S曲線生成
		if(f1) {														// <正側指定の場合>
			for(i=0; i<pn; i++) {
				dd = pa[i].p[d2]-bs.p[d2];								// 基準点とのｻｲｽﾞ指定方向差
				pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// 負の場合は0
			}
		} else {														// <負側指定の場合>
			for(i=0; i<pn; i++) {
				dd = bs.p[d2]-pa[i].p[d2];								// 基準点とのｻｲｽﾞ指定方向差
	            pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// 負の場合は0
			}
		}
		vnum = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// 頂点座標取得
			if((vflg==0)||												// <<<垂直方向限定なし/
               (vflg==1&&pt.p[d3]>vr1-EPSILON)||						//    下限指定で範囲内/
               (vflg==2&&pt.p[d3]<vr2+EPSILON)||						//    上限指定で範囲内
               (vflg==3&&pt.p[d3]>vr1-EPSILON&&pt.p[d3]<vr2+EPSILON)) {	//    上下指定で範囲内>>>
				if(f2||(f1&&pt.p[d2]>bs.p[d2])||
                       (!f1&&pt.p[d2]<bs.p[d2])) {						// <頂点が対象領域内>
					for(j=1; j<pn-1; j++) {								// 頂点の長さ方向位置が含
						if(pt.p[d1]<pa[j].p[d1]) {						// まれる曲線の線分を検索
							break;
						}
					}
					pr = (pt.p[d1]-pa[j-1].p[d1])/
                         (pa[j].p[d1]-pa[j-1].p[d1]);					// 線分内位置の比率
					pr = (pr<0.0) ? 0.0 : ((pr>1.0) ? 1.0 : pr);		// 比率が範囲外
					lw = (pa[j].p[d2]-pa[j-1].p[d2])*pr;				// 線分内のｻｲｽﾞ指定方向位置
					lw += pa[j-1].p[d2];								// 頂点位置のｻｲｽﾞ指定方向距離
					pr = lw/dln;										// ｻｲｽﾞ変更比率
					lw = pt.p[d2]-bs.p[d2];								// 基準点とのｻｲｽﾞ指定方向差
					pt.p[d2] = bs.p[d2]+lw*pr;							// ｻｲｽﾞ指定方向位置変更
					PutVtx(op, i, pt);									// 頂点座標更新
				}
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽ設定
		}
	}
	delete[] pa;
	delete[] ia;
	return op;
}
