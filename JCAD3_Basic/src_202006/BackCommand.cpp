/////////////////////////////////////////////////////////////////////////////
// 立体編集コマンド逆実行(戻し)部
// BackCommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/14 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "DataType.h"
#include "CCommand.h"
#include "NumFunc.h"

/**************************************************/
void BackMove(int flg, PNTTYPE* opt, PNTTYPE* npt,
              double dx, double dy, double dz)							//<<< 逆平行移動
/**************************************************/
{
	if(fabs(dx)>EPSILON||fabs(dy)>EPSILON||fabs(dz)>EPSILON) {
		opt->p[0] += dx, opt->p[1] += dy, opt->p[2] += dz;				// 旧ﾃﾞｰﾀ正平行移動
		if(flg==1) {													// <更新ﾌﾗｸﾞONの場合>
			npt->p[0] -= dx, npt->p[1] -= dy, npt->p[2] -= dz;			// 新ﾃﾞｰﾀ負平行移動
		}
	}
}

/****************************************************/
void BackRotate(int flg, PNTTYPE* opt, PNTTYPE* npt,
                PNTTYPE cp, PNTTYPE pnt, double th)						//<<< 逆回転移動
/****************************************************/
{
	PNTTYPE dis;
	VECTYPE vec;
	double  rth, wk, rot[3][3];
	int     i, j;

	if(fabs(th)<EPSILON) {												// 角度無終了
		return;
	}
	rth = th*PI/180.0;
	vec.p[0] = pnt.p[0], vec.p[1] = pnt.p[1], vec.p[2] = pnt.p[2];
	VecN(&vec);															// 単位ﾍﾞｸﾄﾙ化
	SetRotateMatrix(vec, rth, rot);										// 回転ﾏﾄﾘｯｸｽ設定
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += rot[i][j]*cp.p[j];									// 移動量計算
		}
		dis.p[i] = cp.p[i]-wk;
	}
	VecRotateMove(opt, rot, dis);										// 旧ﾃﾞｰﾀ正回転移動
	if(flg==1) {														// <更新ﾌﾗｸﾞONの場合>
		SetRotateMatrix(vec, -rth, rot);								// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {											// 移動量計算
			for(wk=0, j=0; j<DIM; j++) {
				wk += rot[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		VecRotateMove(npt, rot, dis);									// 新ﾃﾞｰﾀ逆回転移動
	}
}

/****************************************************/
void BackMirror(int flg, PNTTYPE* opt, PNTTYPE* npt,
                PNTTYPE spt, PNTTYPE ept, int plane)					//<<< 逆鏡像移動
/****************************************************/
{
	PNTTYPE pt;
	double  as, bs, cs, ds, rs, x1, y1, z1, x2, y2, z2;

	plane = (plane-1<0) ? 2 : plane-1;									// 面方程式正規化
	pt = spt;
	pt.p[plane] += 1.0;
	x1 = ept.p[0]-spt.p[0], y1 = ept.p[1]-spt.p[1], z1 = ept.p[2]-spt.p[2];
	x2 = pt.p[0]-spt.p[0],  y2 = pt.p[1]-spt.p[1],  z2 = pt.p[2]-spt.p[2];
	as = (y1*z2)-(z1*y2),   bs = (z1*x2)-(x1*z2),   cs = (x1*y2)-(y1*x2);
	ds = -(ept.p[0]*as+ept.p[1]*bs+ept.p[2]*cs);
	rs = sqrt(as*as+bs*bs+cs*cs);
	as /= rs, bs /= rs, cs /= rs, ds /= rs;
	rs = (as*opt->p[0]+bs*opt->p[1]+cs*opt->p[2]+ds)*2.0;
	opt->p[0] -= rs*as, opt->p[1] -= rs*bs, opt->p[2] -= rs*cs;			// 旧ﾃﾞｰﾀ正鏡像移動
	if(flg==1) {														// <更新ﾌﾗｸﾞONの場合>
		rs = (as*npt->p[0]+bs*npt->p[1]+cs*npt->p[2]+ds)*2.0;
		npt->p[0] -= rs*as, npt->p[1] -= rs*bs, npt->p[2] -= rs*cs;		// 新ﾃﾞｰﾀ逆鏡像移動
	}
}

/***********************************************************/
void BackScale(int flg, PNTTYPE* opt, PNTTYPE* npt,
               PNTTYPE cp, double sx, double sy, double sz)				//<<< 逆ｻｲｽﾞ変更
/***********************************************************/
{
	PNTTYPE dis;
	double  mat[3][3], wk;
	int     i, j;

	if(fabs(sx-1.0)<EPSILON&&fabs(sy-1.0)<EPSILON&&fabs(sz-1.0)<EPSILON) {
		return;															// 等倍終了
	}
	mat[0][0] = sx,  mat[0][1] = 0.0, mat[0][2] = 0.0;					// ﾏﾄﾘｯｸｽ設定
	mat[1][0] = 0.0, mat[1][1] = sy,  mat[1][2] = 0.0;
	mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = sz;
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += mat[i][j]*cp.p[j];									// 移動量計算
		}
		dis.p[i] = cp.p[i]-wk;
	}
	VecRotateMove(opt, mat, dis);										// 旧ﾃﾞｰﾀ正ｻｲｽﾞ変更
	if(flg==1) {														// <更新ﾌﾗｸﾞONの場合>
		mat[0][0] = 1.0/sx, mat[1][1] = 1.0/sy, mat[2][2] = 1.0/sz;		// ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {											// 移動量計算
			for(wk=0.0, j=0; j<DIM; j++) {
				wk += mat[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		VecRotateMove(npt, mat, dis);									// 新ﾃﾞｰﾀ逆ｻｲｽﾞ変更
	}
}

/**************************************************************/
void BackResize1(int flg, PNTTYPE* opt, PNTTYPE* npt, int pln,
                 int mpf, double s1, double s2, double s3,
                 PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)					//<<< 逆直線指定ﾘｻｲｽﾞ1
/**************************************************************/
{
	int    i, j=0, i1, i2, sf1, ef1, ef2;
	double sp1, sp2, ep1, ep2, bp, ss1, ss2, es1, es2, wsc, ln1, ln2, wln;

	for(i=0; i<DIM; i++) {
		if(fabs(p1.p[i]-p3.p[i])>EPSILON) {								// 基準線座標軸確認
			j++;
			i1 = i;
		}
	}
	if(j!=1) {															// 基準線無中止
		return;
	}
	s1 = (s1>=0.0&&s1<0.01) ? 0.01 : s1;
	s2 = (s2>=0.0&&s2<0.01) ? 0.01 : s2;
	s3 = (s3>=0.0&&s3<0.01) ? 0.01 : s3;
	sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
	sf1 = 0,   ef1 = 0,   ef2 = 0;
	ss2 = 1.0, es2 = 1.0;
	if(s2<0.0) {														// <2点ﾓｰﾄﾞ>
		if(fabs(s1-1.0)<EPSILON&&fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// 基準点位置
			sp1 = p1.p[i1], ep1 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep1 = p1.p[i1];
		}
		ln1 = ep1-sp1;													// 基準点間距離
		ss1 = s1, es1 = s3;												// 基準倍率
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// 倍率!=1.0
		ef1 = (fabs(s3-1.0)>EPSILON) ? 1 : ef1;
	} else {															// <3点ﾓｰﾄﾞ>
		if(fabs(s1-1.0)<EPSILON&&
           fabs(s2-1.0)<EPSILON&&
           fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// 基準点位置
			sp1 = p1.p[i1], ep2 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep2 = p1.p[i1];
		}
		ep1 = p2.p[i1], sp2 = p2.p[i1];
		ln1 = ep1-sp1,  ln2 = ep2-sp2;									// 基準点間距離
		ss1 = s1, es1 = s2, ss2 = s2, es2 = s3;							// 基準倍率
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// 倍率!=1.0
		ef2 = (fabs(s3-1.0)>EPSILON) ? 1 : ef2;
	}
	if(pln==i1) {														// ﾘｻｲｽﾞ座標軸取得
		i2 = (pln+1>2) ? 0 : pln+1;
	} else {
		i2 = pln;
	}
	bp = p1.p[i2];														// ﾘｻｲｽﾞ基準点
	if(mpf==2||(mpf==0&&opt->p[i2]<bp)||(mpf==1&&opt->p[i2]>bp)) {		// <<正負方向一致>>
		if(opt->p[i1]>=sp1&&opt->p[i1]<=ep1) {							// <基準1範囲内>
			wln = opt->p[i1]-sp1;										// 始点からの距離
			wsc = ss1+(es1-ss1)*wln/ln1;								// 該当位置の倍率
			opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
		} else if(opt->p[i1]>sp2&&opt->p[i1]<=ep2) {					// <基準2範囲内>
			wln = opt->p[i1]-sp2;										// 始点からの距離
			wsc = ss2+(es2-ss2)*wln/ln2;								// 該当位置の倍率
			opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
		} else if(sf1==1&&opt->p[i1]<sp1) {								// <倍率!=1.0の基準1前>
			opt->p[i2] = ss1*(opt->p[i2]-bp)+bp;
		} else if(ef1==1&&opt->p[i1]>ep1) {								// <倍率!=1.0の基準1後>
			opt->p[i2] = es1*(opt->p[i2]-bp)+bp;
		} else if(ef2==1&&opt->p[i1]>ep2) {								// <倍率!=1.0の基準2後>
			opt->p[i2] = es2*(opt->p[i2]-bp)+bp;
		}
	}
	if(flg==1) {														// <<<更新ﾌﾗｸﾞONの場合>>>
		if(mpf==2||(mpf==0&&npt->p[i2]<bp)||(mpf==1&&npt->p[i2]>bp)) {	// <<正負方向一致>>
			if(npt->p[i1]>=sp1&&npt->p[i1]<=ep1) {						// <基準1範囲内>
				wln = npt->p[i1]-sp1;									// 始点からの距離
				wsc = 1.0/(ss1+(es1-ss1)*wln/ln1);						// 該当位置の逆倍率
				npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;					// 逆直線指定ﾘｻｲｽﾞ
			} else if(npt->p[i1]>sp2&&npt->p[i1]<=ep2) {				// <基準2範囲内>
				wln = npt->p[i1]-sp2;									// 始点からの距離
				wsc = 1.0/(ss2+(es2-ss2)*wln/ln2);						// 該当位置の逆倍率
				npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;					// 逆直線指定ﾘｻｲｽﾞ
			} else if(sf1==1&&npt->p[i1]<sp1) {							// <倍率!=1.0の基準1前>
				npt->p[i2] = 1.0/ss1*(npt->p[i2]-bp)+bp;				// 逆直線指定ﾘｻｲｽﾞ
			} else if(ef1==1&&npt->p[i1]>ep1) {							// <倍率!=1.0の基準1後>
				npt->p[i2] = 1.0/es1*(npt->p[i2]-bp)+bp;				// 逆直線指定ﾘｻｲｽﾞ
			} else if(ef2==1&&npt->p[i1]>ep2) {							// <倍率!=1.0の基準2後>
				npt->p[i2] = 1.0/es2*(npt->p[i2]-bp)+bp;				// 逆直線指定ﾘｻｲｽﾞ
			}
		}
	}
}

/***********************************************************/
void BackResize2(int flg, PNTTYPE* opt, PNTTYPE* npt,
                 int f1, int f2, int d1, int d2, double ln,
                 PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)				//<<< 逆曲線指定ﾘｻｲｽﾞ1
/***********************************************************/
{
	int    i;
	double pr, lw, dd;

	if(fabs(ln)<EPSILON||pn==0) {										// 基準長さ無/制御点無は中止
		return;
	}
	MakeSCurve(&pn, 0, 3, 2.0, pa, ia);									// S曲線生成
	if(f1) {															// <正側指定の場合>
		for(i=0; i<pn; i++) {
			dd = pa[i].p[d2]-bs.p[d2];									// 基準点とのｻｲｽﾞ指定方向差
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// 負の場合は0
		}
	} else {															// <負側指定の場合>
		for(i=0; i<pn; i++) {
			dd = bs.p[d2]-pa[i].p[d2];									// 基準点とのｻｲｽﾞ指定方向差
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// 負の場合は0
		}
	}
	if(f2||(f1&&opt->p[d2]>bs.p[d2])||(!f1&&opt->p[d2]<bs.p[d2])) {		// <頂点が対象領域内>
		for(i=1; i<pn-1; i++) {											// 頂点の長さ方向位置が含
			if(opt->p[d1]<pa[i].p[d1]) {								// まれる曲線の線分を検索
				break;
			}
		}
		pr = (opt->p[d1]-pa[i-1].p[d1])/(pa[i].p[d1]-pa[i-1].p[d1]);	// 線分内位置の比率
		pr = (pr<0.0) ? 0.0 : pr;										// 比率が範囲外
		pr = (pr>1.0) ? 1.0 : pr;
		lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;							// 線分内のｻｲｽﾞ指定方向位置
		lw += pa[i-1].p[d2];											// 頂点位置のｻｲｽﾞ指定方向距離
		pr = lw/ln;														// ｻｲｽﾞ変更比率
		lw = opt->p[d2]-bs.p[d2];										// 基準点とのｻｲｽﾞ指定方向差
		opt->p[d2] = bs.p[d2]+lw*pr;									// ｻｲｽﾞ指定方向位置変更
	}
	if(flg==1) {														// <更新ﾌﾗｸﾞONの場合>
		if(f2||(f1&&npt->p[d2]>bs.p[d2])||(!f1&&npt->p[d2]<bs.p[d2])) {	// <頂点が対象領域内>
			for(i=1; i<pn-1; i++) {										// 頂点の長さ方向位置が含
				if(npt->p[d1]<pa[i].p[d1]) {							// まれる曲線の線分を検索
					break;
				}
			}
			pr = (npt->p[d1]-pa[i-1].p[d1])/(pa[i].p[d1]-pa[i-1].p[d1]);// 線分内位置の比率
			pr = (pr<0.0) ? 0.0 : pr;									// 比率が範囲外
			pr = (pr>1.0) ? 1.0 : pr;
			lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;						// 線分内のｻｲｽﾞ指定方向位置
			lw += pa[i-1].p[d2];										// 頂点位置のｻｲｽﾞ指定方向距離
			pr = lw/ln;													// ｻｲｽﾞ変更比率
			lw = npt->p[d2]-bs.p[d2];									// 基準点とのｻｲｽﾞ指定方向差
			npt->p[d2] = bs.p[d2]+lw/pr;								// ｻｲｽﾞ指定方向位置変更
		}
	}
}

/****************************************************************/
void BackBend(int flg, PNTTYPE* opt, PNTTYPE* npt,
              int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)		//<<< 逆円筒曲げ
/****************************************************************/
{
	PNTTYPE pnt, cp, rp, wsp[2], wep[2];
	VECTYPE vec;
	int     i, dc;
	double  rots[3][3], rote[3][3], rtbs[3][3], rtbe[3][3];
	double  ln, dd, rth, wth, bth, d1, d2, rr;

	dc = (fc+vc==1) ? 2 : ((fc+vc==2) ? 1 : 0);							// 曲げる奥行方向取得
	ln = ep.p[fc]-sp.p[fc];												// 曲げる弧長さ
	if(fabs(th)<EPSILON||fabs(ln)<EPSILON) {							// 角度/弧長さ=0は中止
		return;
	}
	dd = (ln*360.0)/(th*PI);											// 曲げる弧直径
	rth = (th*PI)/360.0;												// 回転角度(ﾗｼﾞｱﾝ)算出
	vec.p[fc] = 0.0;													// 回転ﾍﾞｸﾄﾙ設定
	vec.p[vc] = 0.0;
	vec.p[dc] = ((fc==0&&vc==2)||(fc==1&&vc==0)||(fc==2&&vc==1))
                ? -1.0 : 1.0;
	for(i=0; i<DIM; i++) {
		cp.p[i] = (sp.p[i]+ep.p[i])/2.0;								// 基準点1(曲げ前)算出
	}
	rp = cp;															// 基準点2(曲げ後)算出
	rp.p[vc] = cp.p[vc]-dd/2.0;
	if(sp.p[fc]>opt->p[fc]) {											// <回転(左側)の場合>
		SetRotateMatrix(vec, rth, rots);								// 回転ﾏﾄﾘｯｸｽ作成
		SetRotateMatrix(vec, -rth, rtbs);								// 回転ﾏﾄﾘｯｸｽ作成(逆)
		wsp[0] = sp;
		VecRotate(&wsp[0], rots);										// 始点回転後位置算出
		pnt = sp;
		rr = (dd/2.0)+pnt.p[vc]-cp.p[vc];								// 始点曲げ後位置算出
		wth = (pnt.p[fc]-cp.p[fc])*2.0/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wsp[1] = pnt;
		for(i=0; i<DIM; i++) {											// 回転と曲げ誤差算出
			wsp[0].p[i] = wsp[1].p[i]-wsp[0].p[i];
		}
		VecRotateMove(opt, rots, wsp[0]);								// 回転移動(左側)
		if(flg==1) {													// <更新ﾌﾗｸﾞONの場合>
			for(i=0; i<DIM; i++) {
				npt->p[i] = npt->p[i]-wsp[0].p[i];						// 移動(左側)[逆]
			}
			VecRotate(npt, rtbs);										// 回転(左側)[逆]
		}
	} else if(ep.p[fc]<opt->p[fc]) {									// <回転(右側)の場合>
		SetRotateMatrix(vec, -rth, rote);								// 回転ﾏﾄﾘｯｸｽ作成
		SetRotateMatrix(vec, rth, rtbe);								// 回転ﾏﾄﾘｯｸｽ作成(逆)
		wep[0] = ep;
		VecRotate(&wep[0], rote);										// 終点回転後位置算出
		pnt = ep;
		rr = (dd/2.0)+pnt.p[vc]-cp.p[vc];								// 終点曲げ後位置算出
		wth = (pnt.p[fc]-cp.p[fc])*2.0/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wep[1] = pnt;
		for(i=0; i<DIM; i++) {
			wep[0].p[i] = wep[1].p[i]-wep[0].p[i];						// 回転と曲げ誤差算出
		}
		VecRotateMove(opt, rote, wep[0]);								// 回転移動(右側)
		if(flg==1) {													// <更新ﾌﾗｸﾞONの場合>
			for(i=0; i<DIM; i++) {
				npt->p[i] = npt->p[i]-wep[0].p[i];						// 移動(右側)[逆]
			}
			VecRotate(npt, rtbe);										// 回転(右側)[逆]
		}
	} else {															// <曲げ(中央)の場合>
		rr = (dd/2.0)+opt->p[vc]-cp.p[vc];								// 半径算出(基準点1->)
		wth = (opt->p[fc]-cp.p[fc])*2.0/dd;								// 角度算出(基準点2->)
		opt->p[fc] = rr*sin(wth)+rp.p[fc];								// 曲げ後位置算出後に
		opt->p[vc] = rr*cos(wth)+rp.p[vc];								// 基準点2に移動
		if(flg==1) {													// <更新ﾌﾗｸﾞONの場合>
			d1 = npt->p[fc]-rp.p[fc];									// 新ﾃﾞｰﾀ半径
			d2 = npt->p[vc]-rp.p[vc];
			ln = sqrt(d1*d1+d2*d2);
			ln = (rr<0) ? -ln : ln;										// 新半径を負変換
			if(rth>0) {
				bth = atan2(npt->p[fc]-rp.p[fc], npt->p[vc]-rp.p[vc]);	// 新ﾃﾞｰﾀ基準点角度
			} else {
				bth = atan2(rp.p[fc]-npt->p[fc], rp.p[vc]-npt->p[vc]);	// 新ﾃﾞｰﾀ基準点角度
			}
			bth = (bth<-10||bth>10) ? 0.0 : bth;						// 反対側に出た場合の補正
			if(wth>0&&bth<0&&fabs(wth-(bth+2.0*PI))<PI) {
				bth += 2.0*PI;
			} else if(wth<0&&bth>0&&fabs(wth-(bth-2.0*PI))<PI) {
				bth -= 2.0*PI;
			}
			npt->p[fc] = cp.p[fc]+dd*bth/2.0;							// 新ﾃﾞｰﾀ曲げ前位置に
			npt->p[vc] = ln+cp.p[vc]-dd/2.0;							// 変換(逆変換)
		}
	}
}

/********************************************************/
void BackTwist(int flg, PNTTYPE* opt, PNTTYPE* npt,
               int dr, double th, double ln, PNTTYPE cp)				//<<< 逆ひねり
/********************************************************/
{
	PNTTYPE dis;
	VECTYPE vec;
	int     i, j;
	double  wth, wk, rot[3][3];

	if(fabs(th)<EPSILON||fabs(ln)<EPSILON) {							// 角度/軸幅無中止
		return;
	}
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 0.0, vec.p[dr] = -1.0;	// ひねり軸ﾍﾞｸﾄﾙ作成
	wk = opt->p[dr]-cp.p[dr];											// 該当位置距離
	wth = PI*th*wk/(180.0*ln);											// 該当位置角度
	SetRotateMatrix(vec, wth, rot);										// 回転ﾏﾄﾘｯｸｽ設定
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += rot[i][j]*cp.p[j];									// 移動量計算
		}
		dis.p[i] = cp.p[i]-wk;
	}
	VecRotateMove(opt, rot, dis);										// 旧ﾃﾞｰﾀ正ひねり
	if(flg==1) {														// <更新ﾌﾗｸﾞONの場合>
		SetRotateMatrix(vec, -wth, rot);								// 回転ﾏﾄﾘｯｸｽ設定
		for(i=0; i<DIM; i++) {											// 移動量計算
			for(wk=0.0, j=0; j<DIM; j++) {
				wk += rot[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		VecRotateMove(npt, rot, dis);									// 新ﾃﾞｰﾀ逆ひねり
	}
}

/*********************************************************/
void BackShear(int flg, PNTTYPE* opt, PNTTYPE* npt,
               int dr1, int dr2, double inc, PNTTYPE bas)				//<<< 逆せん断
/*********************************************************/
{
	double ln;

	if(fabs(inc)<EPSILON) {												// 傾き無中止
		return;
	}
	ln = opt->p[dr1]-bas.p[dr1];										// 基準点からの垂直方向距離
	ln = ln*inc;														// せん断水平方向移動量算出
	opt->p[dr2] += ln;													// 正移動量分補正
	if(flg==1) {														// <更新ﾌﾗｸﾞONの場合>
		ln = npt->p[dr1]-bas.p[dr1];									// 基準点からの垂直方向距離
		ln = ln*inc;													// せん断水平方向移動量算出
		npt->p[dr2] -= ln;												// 逆移動量分補正
	}
}

/***********************************************************************/
void BackResize3(int flg, PNTTYPE* opt, PNTTYPE* npt, int pln, int mpf,
                 double s1, double s2, double s3, int vflg, double v1,
                 double v2, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)			//<<< 逆直線指定ﾘｻｲｽﾞ2
/***********************************************************************/
{
	int    i, j=0, i1, i2, i3, sf1, ef1, ef2;
	double sp1, sp2, ep1, ep2, bp, ss1, ss2, es1, es2, wsc, ln1, ln2, wln;

	for(i=0; i<DIM; i++) {
		if(fabs(p1.p[i]-p3.p[i])>EPSILON) {								// 基準線座標軸確認
			j++;
			i1 = i;
		}
	}
	if(j!=1) {															// 基準線無中止
		return;
	}
	s1 = (s1>=0.0&&s1<0.01) ? 0.01 : s1;
	s2 = (s2>=0.0&&s2<0.01) ? 0.01 : s2;
	s3 = (s3>=0.0&&s3<0.01) ? 0.01 : s3;
	sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
	sf1 = 0,   ef1 = 0,   ef2 = 0;
	ss2 = 1.0, es2 = 1.0;
	if(s2<0.0) {														// <2点ﾓｰﾄﾞ>
		if(fabs(s1-1.0)<EPSILON&&fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// 基準点位置
			sp1 = p1.p[i1], ep1 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep1 = p1.p[i1];
		}
		ln1 = ep1-sp1;													// 基準点間距離
		ss1 = s1, es1 = s3;												// 基準倍率
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// 倍率!=1.0
		ef1 = (fabs(s3-1.0)>EPSILON) ? 1 : ef1;
	} else {															// <3点ﾓｰﾄﾞ>
		if(fabs(s1-1.0)<EPSILON&&
           fabs(s2-1.0)<EPSILON&&
           fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// 基準点位置
			sp1 = p1.p[i1], ep2 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep2 = p1.p[i1];
		}
		ep1 = p2.p[i1], sp2 = p2.p[i1];
		ln1 = ep1-sp1,  ln2 = ep2-sp2;									// 基準点間距離
		ss1 = s1, es1 = s2, ss2 = s2, es2 = s3;							// 基準倍率
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// 倍率!=1.0
		ef2 = (fabs(s3-1.0)>EPSILON) ? 1 : ef2;
	}
	if(pln==i1) {														// ﾘｻｲｽﾞ座標軸取得
		i2 = (pln+1>2) ? 0 : pln+1;
	} else {
		i2 = pln;
	}
	i3 = (pln+1>2) ? 0 : pln+1;											// 垂直方向算出
	i3 = (i3+1>2) ? 0 : i3+1;
	bp = p1.p[i2];														// ﾘｻｲｽﾞ基準点
	if((vflg==0)||														// <<<垂直方向限定なし/
       (vflg==1&&opt->p[i3]>v1-EPSILON)||								//    下限指定で範囲内/
       (vflg==2&&opt->p[i3]<v2+EPSILON)||								//    上限指定で範囲内
       (vflg==3&&opt->p[i3]>v1-EPSILON&&opt->p[i3]<v2+EPSILON)) {		//    上下指定で範囲内>>>
		if(mpf==2||(mpf==0&&opt->p[i2]<bp)||(mpf==1&&opt->p[i2]>bp)) {	// <<正負方向一致>>
			if(opt->p[i1]>=sp1&&opt->p[i1]<=ep1) {						// <基準1範囲内>
				wln = opt->p[i1]-sp1;									// 始点からの距離
				wsc = ss1+(es1-ss1)*wln/ln1;							// 該当位置の倍率
				opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
			} else if(opt->p[i1]>sp2&&opt->p[i1]<=ep2) {				// <基準2範囲内>
				wln = opt->p[i1]-sp2;									// 始点からの距離
				wsc = ss2+(es2-ss2)*wln/ln2;							// 該当位置の倍率
				opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
			} else if(sf1==1&&opt->p[i1]<sp1) {							// <倍率!=1.0の基準1前>
				opt->p[i2] = ss1*(opt->p[i2]-bp)+bp;
			} else if(ef1==1&&opt->p[i1]>ep1) {							// <倍率!=1.0の基準1後>
				opt->p[i2] = es1*(opt->p[i2]-bp)+bp;
			} else if(ef2==1&&opt->p[i1]>ep2) {							// <倍率!=1.0の基準2後>
				opt->p[i2] = es2*(opt->p[i2]-bp)+bp;
			}
		}
	}
	if(flg==1) {														// <<<<更新ﾌﾗｸﾞONの場合>>>>
		if((vflg==0)||													// <<<垂直方向限定なし/
           (vflg==1&&npt->p[i3]>v1-EPSILON)||							//    下限指定で範囲内/
           (vflg==2&&npt->p[i3]<v2+EPSILON)||							//    上限指定で範囲内
           (vflg==3&&npt->p[i3]>v1-EPSILON&&npt->p[i3]<v2+EPSILON)) {	//    上下指定で範囲内>>>
			if(mpf==2||(mpf==0&&npt->p[i2]<bp)||
                       (mpf==1&&npt->p[i2]>bp)) {						// <<正負方向一致>>
				if(npt->p[i1]>=sp1&&npt->p[i1]<=ep1) {					// <基準1範囲内>
					wln = npt->p[i1]-sp1;								// 始点からの距離
					wsc = 1.0/(ss1+(es1-ss1)*wln/ln1);					// 該当位置の逆倍率
					npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;				// 逆直線指定ﾘｻｲｽﾞ
				} else if(npt->p[i1]>sp2&&npt->p[i1]<=ep2) {			// <基準2範囲内>
					wln = npt->p[i1]-sp2;								// 始点からの距離
					wsc = 1.0/(ss2+(es2-ss2)*wln/ln2);					// 該当位置の逆倍率
					npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;				// 逆直線指定ﾘｻｲｽﾞ
				} else if(sf1==1&&npt->p[i1]<sp1) {						// <倍率!=1.0の基準1前>
					npt->p[i2] = 1.0/ss1*(npt->p[i2]-bp)+bp;			// 逆直線指定ﾘｻｲｽﾞ
				} else if(ef1==1&&npt->p[i1]>ep1) {						// <倍率!=1.0の基準1後>
					npt->p[i2] = 1.0/es1*(npt->p[i2]-bp)+bp;			// 逆直線指定ﾘｻｲｽﾞ
				} else if(ef2==1&&npt->p[i1]>ep2) {						// <倍率!=1.0の基準2後>
					npt->p[i2] = 1.0/es2*(npt->p[i2]-bp)+bp;			// 逆直線指定ﾘｻｲｽﾞ
				}
			}
		}
	}
}

/************************************************************************/
void BackResize4(int flg, PNTTYPE* opt, PNTTYPE* npt, int f1, int f2,
                 int d1, int d2, int d3, double ln, int vflg, double v1,
                 double v2, PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)	//<<< 逆曲線指定ﾘｻｲｽﾞ2
/************************************************************************/
{
	int    i;
	double pr, lw, dd;

	if(fabs(ln)<EPSILON||pn==0) {										// 基準長さ無/制御点無は中止
		return;
	}
	MakeSCurve(&pn, 0, 3, 2.0, pa, ia);									// S曲線生成
	if(f1) {															// <正側指定の場合>
		for(i=0; i<pn; i++) {
			dd = pa[i].p[d2]-bs.p[d2];									// 基準点とのｻｲｽﾞ指定方向差
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// 負の場合は0
		}
	} else {															// <負側指定の場合>
		for(i=0; i<pn; i++) {
			dd = bs.p[d2]-pa[i].p[d2];									// 基準点とのｻｲｽﾞ指定方向差
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// 負の場合は0
		}
	}
	if((vflg==0)||														// <<<垂直方向限定なし/
       (vflg==1&&opt->p[d3]>v1-EPSILON)||								//    下限指定で範囲内/
       (vflg==2&&opt->p[d3]<v2+EPSILON)||								//    上限指定で範囲内
       (vflg==3&&opt->p[d3]>v1-EPSILON&&opt->p[d3]<v2+EPSILON)) {		//    上下指定で範囲内>>>
		if(f2||(f1&&opt->p[d2]>bs.p[d2])||(!f1&&opt->p[d2]<bs.p[d2])) {	// <頂点が対象領域内>
			for(i=1; i<pn-1; i++) {										// 頂点の長さ方向位置が含
				if(opt->p[d1]<pa[i].p[d1]) {							// まれる曲線の線分を検索
					break;
				}
			}
			pr = (opt->p[d1]-pa[i-1].p[d1])/(pa[i].p[d1]-pa[i-1].p[d1]);// 線分内位置の比率
			pr = (pr<0.0) ? 0.0 : pr;									// 比率が範囲外
			pr = (pr>1.0) ? 1.0 : pr;
			lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;						// 線分内のｻｲｽﾞ指定方向位置
			lw += pa[i-1].p[d2];										// 頂点位置のｻｲｽﾞ指定方向距離
			pr = lw/ln;													// ｻｲｽﾞ変更比率
			lw = opt->p[d2]-bs.p[d2];									// 基準点とのｻｲｽﾞ指定方向差
			opt->p[d2] = bs.p[d2]+lw*pr;								// ｻｲｽﾞ指定方向位置変更
		}
	}
	if(flg==1) {														// <<<更新ﾌﾗｸﾞONの場合>>>
		if((vflg==0)||													// <<<垂直方向限定なし/
           (vflg==1&&npt->p[d3]>v1-EPSILON)||							//    下限指定で範囲内/
           (vflg==2&&npt->p[d3]<v2+EPSILON)||							//    上限指定で範囲内
           (vflg==3&&npt->p[d3]>v1-EPSILON&&npt->p[d3]<v2+EPSILON)) {	//    上下指定で範囲内>>>
			if(f2||(f1&&npt->p[d2]>bs.p[d2])||
                   (!f1&&npt->p[d2]<bs.p[d2])) {						// <頂点が対象領域内>
				for(i=1; i<pn-1; i++) {									// 頂点の長さ方向位置が含
					if(npt->p[d1]<pa[i].p[d1]) {						// まれる曲線の線分を検索
						break;
					}
				}
				pr = (npt->p[d1]-pa[i-1].p[d1])/
                     (pa[i].p[d1]-pa[i-1].p[d1]);						// 線分内位置の比率
				pr = (pr<0.0) ? 0.0 : pr;								// 比率が範囲外
				pr = (pr>1.0) ? 1.0 : pr;
				lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;					// 線分内のｻｲｽﾞ指定方向位置
				lw += pa[i-1].p[d2];									// 頂点位置のｻｲｽﾞ指定方向距離
				pr = lw/ln;												// ｻｲｽﾞ変更比率
				lw = npt->p[d2]-bs.p[d2];								// 基準点とのｻｲｽﾞ指定方向差
				npt->p[d2] = bs.p[d2]+lw/pr;							// ｻｲｽﾞ指定方向位置変更
			}
		}
	}
}
