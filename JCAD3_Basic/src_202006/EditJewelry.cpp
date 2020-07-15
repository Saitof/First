/////////////////////////////////////////////////////////////////////////////
// 宝石関係編集部
// EditJewelry.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "ECommand.h"
#include "RCommand.h"
#include "DataFile1.h"
#include "Select.h"
#include "NumFunc.h"
#include "CreatCtrlPoint2.h"
#include "ConvSCurve.h"

/*********************************************************************/
BOOL KeyboardJwlRing(int kind, int mode, double* size, double* space)	//<<< ｻｲｽﾞ,接合間隔入力
/*********************************************************************/
{
	CInputDlg dlg(17+2*kind+mode);
	dlg.SetValue(*size, *space);										// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(size, space);										// ｻｲｽﾞ,接合間隔入力
		return TRUE;
	} else {
		return FALSE;
	}
}

/********************************/
BOOL KeyboardStoneForm(int* frm)										//<<< 石枠形状入力
/********************************/
{
	CGemsetDlg dlg;
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(frm);												// 形状入力
		return TRUE;
	} else {
		return FALSE;
	}
}

/**********************************/
BOOL KeyboardRoundSize(double* xd)										//<<< 石枠直径入力
/**********************************/
{
	CInputDlg dlg(4);
	dlg.SetValue(*xd);													// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(xd);												// ｻｲｽﾞ入力
		return TRUE;
	} else {
		return FALSE;
	}
}

/***********************************************/
BOOL KeyboardSquareSize(double* xd, double* yd)							//<<< 石枠X,Yｻｲｽﾞ入力
/***********************************************/
{
	CInputDlg dlg(3);
	dlg.SetValue(*xd, *yd);												// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(xd, yd);											// ｻｲｽﾞ入力
		return TRUE;
	} else {
		return FALSE;
	}
}

/***************************************************************/
BOOL KeyboardProng(int* nm, double* ln, double* dm, double* rv)			//<<< 爪情報入力
/***************************************************************/
{
	CSetProngDlg dlg;
	dlg.SetValue(*nm, *ln, *dm, *rv);									// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(nm, ln, dm, rv);									// 爪情報入力
		return TRUE;
	} else {
		return FALSE;
	}
}

/*******************************************/
BOOL KeyboardArrange(double* ath, int* ano)								//<<< 配置情報入力
/*******************************************/
{
	CInputDlg dlg(12);
	dlg.SetValue(*ath, *ano);											// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(ath, ano);											// 領域,個数入力
		return TRUE;
	} else {
		return FALSE;
	}
}

/************************************************************************/
void TrackerStone(int flg, PNTTYPE base, double xs, double ys, double xh,
                  double yh, int* cn, PNTTYPE* pc, int* ic, int* op)	//<<< 石枠ﾄﾗｯｶｰ作成
/************************************************************************/
{
	int    i;
	double ws;

	pc[0] = base, pc[1] = base, pc[2] = base, pc[3] = base;
	ic[0] = 0, ic[1] = 0, ic[2] = 0, ic[3] = 0;
	if(flg==0) {														// <ﾗｳﾝﾄﾞ石枠の場合>
		pc[0].p[0] -= xs, pc[1].p[1] -= ys;
		pc[2].p[0] += xs, pc[3].p[1] += ys;
		op[0] = 5, op[1] = 6, op[2] = 5, op[3] = 6;
		*cn = 4;
	} else if(flg==1) {													// <ｵｰﾊﾞﾙ石枠の場合>
		pc[0].p[0] -= xs, pc[0].p[1] -= ys;
		pc[1].p[0] += xs, pc[1].p[1] -= ys;
		pc[2].p[0] += xs, pc[2].p[1] += ys;
		pc[3].p[0] -= xs, pc[3].p[1] += ys;
		op[0] = 7, op[1] = 7, op[2] = 7, op[3] = 7;
		*cn = 4;
	} else {															// <ｽｸｴｱ石枠の場合>
		pc[0].p[0] -= xs, pc[0].p[1] -= ys;
		pc[1].p[0] += xs, pc[1].p[1] -= ys;
		pc[2].p[0] += xs, pc[2].p[1] += ys;
		pc[3].p[0] -= xs, pc[3].p[1] += ys;
		op[0] = 7, op[1] = 7, op[2] = 7, op[3] = 7;
		xh = (fabs(xh)<EPSILON&&yh>0.0) ? yh : xh;
		yh = (fabs(yh)<EPSILON&&xh>0.0) ? xh : yh;
		ws = (xs<ys) ? xs : ys;
		if(fabs(xh)<EPSILON&&fabs(yh)<EPSILON) {
			xh = ws/2.0, yh = ws/2.0;
		}
		for(i=0; i<4; i++) {
			pc[2*i+4] = pc[i], ic[2*i+4] = 2;
			pc[2*i+5] = pc[i], ic[2*i+5] = 2;
		}
		pc[4].p[1] += yh,  pc[5].p[0] += xh;
		pc[6].p[0] -= xh,  pc[7].p[1] += yh;
		pc[8].p[1] -= yh,  pc[9].p[0] -= xh;
		pc[10].p[0] += xh, pc[11].p[1] -= yh;
		op[4] = 6, op[5] = 5, op[6] = 5, op[7] = 6;
		op[8] = 6, op[9] = 5, op[10] = 5, op[11] = 6;
		*cn = 12;
	}
}

/**********************************************************************/
void RoundStone(PNTTYPE base, double mi, double xs, double ys, int dv,
                int an, PNTTYPE* pa, int* ia, PNTTYPE* pb, int* ib)		//<<< ﾗｳﾝﾄﾞ石枠生成
/**********************************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	int*     ea = new int[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	int      i, j, cnt=0;
	double   ath, dth, wt;

	dth = 2.0*PI/dv;													// 1分割回転角度
	for(i=0; i<an; i++) {
		w1[i].p[0] = 0.0;												// 指定平面外座標
		w1[i].p[1] = pa[i].p[1]-mi;										// 2D図形複写
		w1[i].p[2] = pa[i].p[2];
	}
	for(i=0; i<an; i++) {
		for(ath=0.0, j=0; j<dv; ath+=dth, j++) {						// 制御点配列設定
			pb[cnt].p[0] = (xs-w1[i].p[1])*cos(ath)+base.p[0];
			pb[cnt].p[1] = (ys-w1[i].p[1])*sin(ath)+base.p[1];
			pb[cnt].p[2] = w1[i].p[2];
			ib[cnt++]=ia[i];											// 曲線ﾓｰﾄﾞ設定
		}
	}
	for(i=0; i<dv; i++) {
		ea[i] = 0;														// 回転方向のﾗｲﾝﾓｰﾄﾞ
	}
	wt = View->GetCurveWeight();										// 曲線重み係数取得
	SetCurveMatrix1(1, wt, 1, dv, ea, ma, mb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
	for(i=0; i<an; i++) {
		for(j=0; j<dv; j++) {
			w1[j] = pb[j+dv*i];
		}
		CalcCurvePoint(1, dv, w1, w2, mb);								// 通過点=>制御点変換
		for(j=0; j<dv; j++) {
			pb[j+dv*i] = w2[j];
		}
	}
	delete[] w1;
	delete[] w2;
	delete[] ea;
	delete[] ma;
	delete[] mb;
}

/************************************************************/
void SquareStone(PNTTYPE base, double mi, double xs,
                 double ys, double xh, double yh, int an,
                 PNTTYPE* pa, int* ia, PNTTYPE* pb, int* ib)			//<<< ｽｸｴｱ石枠生成
/************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* wp = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  ov[8], bp[4];
	VECTYPE  vec, rvc, v1, v2;
	int      i, j;
	double   rot[3][3], th, dd;

	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = -1.0;					// 回転軸設定
	for(i=0; i<an; i++) {
		pp[i].p[0] = 0.0;												// 指定平面外座標
		pp[i].p[1] = pa[i].p[1]-mi;										// 2D図形複写
		pp[i].p[2] = pa[i].p[2];
	}
	for(i=0; i<4; i++) {
		bp[i] = base;													// 基準点算出
	}
	bp[0].p[0] -= xs, bp[0].p[1] -= ys;
	bp[1].p[0] += xs, bp[1].p[1] -= ys;
	bp[2].p[0] += xs, bp[2].p[1] += ys;
	bp[3].p[0] -= xs, bp[3].p[1] += ys;
	if(fabs(xh)<EPSILON&&fabs(yh)<EPSILON) {							// <<補正無の場合>>
		for(i=0; i<4; i++) {
			ov[2*i] = bp[i];											// 基準点設定
			if(i==3) {													// 基準点中間点設定
				for(j=0; j<DIM; j++) {
					ov[2*i+1].p[j] = (bp[3].p[j]+bp[0].p[j])/2.0;
				}
			} else {
				for(j=0; j<DIM; j++) {
					ov[2*i+1].p[j] = (bp[i].p[j]+bp[i+1].p[j])/2.0;
				}
			}
		}
	} else {															// <<補正有の場合>>
		xh = (fabs(xh)<EPSILON) ? yh : xh;								// 一方が0が他方代入
		yh = (fabs(yh)<EPSILON) ? xh : yh;
		for(i=0; i<4; i++) {											// 基準点設定
			ov[2*i] = bp[i], ov[2*i+1] = bp[i];
		}
		ov[0].p[1] += yh, ov[1].p[0] += xh;								// 基準点補正
		ov[2].p[0] -= xh, ov[3].p[1] += yh;
		ov[4].p[1] -= yh, ov[5].p[0] -= xh;
		ov[6].p[0] += xh, ov[7].p[1] -= yh;
	}
	for(i=0; i<8; i++) {												// 隣接頂点とのﾍﾞｸﾄﾙ取得
		if(i==0) {
			for(j=0; j<DIM; j++) {
				v1.p[j] = ov[7].p[j]-ov[0].p[j];
				v2.p[j] = ov[1].p[j]-ov[0].p[j];
			}
		} else if(i==7) {
			for(j=0; j<DIM; j++) {
				v1.p[j] = ov[6].p[j]-ov[7].p[j];
				v2.p[j] = ov[0].p[j]-ov[7].p[j];
			}
		} else {
			for(j=0; j<DIM; j++) {
				v1.p[j] = ov[i-1].p[j]-ov[i].p[j];
				v2.p[j] = ov[i+1].p[j]-ov[i].p[j];
			}
		}
		VecN(&v1);														// 単位ﾍﾞｸﾄﾙ化
		VecN(&v2);
		for(j=0; j<DIM; j++) {
			vec.p[j] = (v1.p[j]+v2.p[j])/2.0;							// 回転方向ﾍﾞｸﾄﾙ計算
		}
		VecN(&vec);														// 単位ﾍﾞｸﾄﾙ化
		for(j=0; j<an; j++) {
			wp[j] = pp[j];
		}
		if(fabs(vec.p[1])>EPSILON) {
			th = atan2(vec.p[0], vec.p[1]);								// 回転角度計算
			th = (th<-10||th>10) ? 0.0 : th;							// Yｻｲｽﾞ補正値
			dd = (i==0||i==3||i==4||i==7) ? fabs(1.0/sin(th)) : fabs(1.0/cos(th));
			for(j=0; j<an; j++) {
				wp[j].p[1] = pp[j].p[1]*dd;								// Y座標のみ補正
			}
		} else {
			if(v2.p[0]>0.5) {
				th = 0.0;
			} else if(v2.p[0]<-0.5) {
				th = PI;
			} else if(v2.p[1]<-0.5) {
				th = PI/2.0;
			} else {
				th = -PI/2.0;
			}
		}
		SetRotateMatrix(rvc, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(j=0; j<an; j++) {
			VecRotateMove(&wp[j], rot, ov[i]);							// 断面回転移動
			pb[8*j+i] = wp[j];											// 制御点配列設定
			ib[8*j+i] = ia[j]+1;										// 曲線ﾓｰﾄﾞ設定
		}
	}
	delete[] pp;
	delete[] wp;
}


/**********************************************************/
static void ResetSurfaceObject(OBJTYPE* op, OBJTYPE** rop)				//<<<  曲面立体再作成
/**********************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	int*     ia = new int[View->m_MaxSCtA];
	OBJTYPE* cop;
	CMDTYPE* cmp;
	int      un, vn, ub, vb, mx, wt, fg, ct, i, j, mir=0;
	double   dwt;

	if(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		un = GetParaInt(cmp, 0);										// U制御点数取得
		vn = GetParaInt(cmp, 1);										// V制御点数取得
		ub = GetParaInt(cmp, 2);										// U境界ﾓｰﾄﾞ取得
		vb = GetParaInt(cmp, 3);										// V境界ﾓｰﾄﾞ取得
		mx = GetParaInt(cmp, 4);										// 曲線化繰返数
		wt = GetParaInt(cmp, 5);										// 曲線重み係数
		dwt = wt/1000.0;
		fg = GetParaInt(cmp, 6);										// ｿﾘｯﾄﾞﾓｰﾄﾞ
		for(i=0, ct=0; i<vn; i++) {
			for(j=0; j<un; j++) {
				GetParaPnt(cmp, un*i+j, &pa[ct]);						// 制御点取得
				ia[ct++] = GetParaInt(cmp, un*i+j+7);					// ﾓｰﾄﾞ取得
			}
		}
		SetSurface(un, vn, ub, vb, pa);									// 曲面制御点生成
		GetObjPtr(&cop);												// 制御点OP保存
		NextCmdPtr(&cmp);												// 曲面編集後ｺﾏﾝﾄﾞ
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==MIRROR) {									// 鏡像ｺﾏﾝﾄﾞ数ｶｳﾝﾄ
				mir++;
			}
			if(GetCmdNo(cmp)!=THICKNESS) {								// 厚み付け以外の
				RedoCommandCopy(cop, cmp);								// ｺﾏﾝﾄﾞ複写実行
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		j = un*vn;
		for(i=0; i<j; i++) {
			GetVtx(cop, i, &pa[i]);										// 立体編集後制御点取得
		}
		FreeObj(cop, 1);												// 立体削除
		if(mir%2==1) {
			ReverseSurf(un, vn, pa, ia);								// 曲面制御点反転
		}
		CrtSurface(un, vn, ub, vb, mx, dwt, fg, pa, ia);				// 曲面生成
		GetObjPtr(&cop);												// 生成立体取得
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==THICKNESS) {								// 厚み付けの
				RedoCommandCopy(cop, cmp);								// ｺﾏﾝﾄﾞ複写実行
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		*rop = cop;
	}
	delete[] pa;
	delete[] ia;
}

/******************************************************/
void SetProng(int nm, double ln, double dm, double rv,
			  BOXTYPE obx, int an, PNTTYPE* pa)							//<<< 石枠爪設定
/******************************************************/
{
	OBJTYPE** sop = new OBJTYPE*[16];
	OBJTYPE** gop = new OBJTYPE*[5];
	OBJTYPE*  op, * wop;
	OIFTYPE*  oif;
	PNTTYPE   pt1, pt2, pt3, cpt;
	VECTYPE   rvc;
	BOXTYPE   box;
	CString   fname;
	int       i, j, cnt=0, gcnt=0;
	double    th, sln, wln, xd, yd, zd;

	BoxCenter(obx, &cpt);												// ﾎﾞｯｸｽ中心取得
	cpt.p[1] = obx.bp[0].p[1];											// 中心面YZ最小点
	cpt.p[2] = obx.bp[0].p[2];
	pt1 = pa[0];
	for(i=1; i<an; i++) {
		pt1.p[1] = (pa[i].p[1]<pt1.p[1]) ? pa[i].p[1] : pt1.p[1];		// 2D-Y最小点
		pt1.p[2] = (pa[i].p[2]<pt1.p[2]) ? pa[i].p[2] : pt1.p[2];		// 2D-Z最小点
	}
	for(i=0; i<DIM; i++) {
		pt1.p[i] = cpt.p[i]-pt1.p[i];									// 2D移動量算出
	}
	for(i=0; i<an; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += pt1.p[j];										// ﾎﾞｯｸｽ基準点移動
		}
	}
	pt1 = pa[0];
	for(i=1; i<an; i++) {
		pt1.p[1] = (pa[i].p[1]<pt1.p[1]) ? pa[i].p[1] : pt1.p[1];		// 移動後2D-Y最小点
		pt1.p[2] = (pa[i].p[2]<pt1.p[2]) ? pa[i].p[2] : pt1.p[2];		// 移動後2D-Z最小点
	}
	sln = PntD(&pt1, &pa[0]);
	for(i=1, j=0; i<an; i++) {
		if((wln=PntD(&pt1, &pa[i]))<sln) {								// 最小距離点検索
			sln = wln, j = i;
		}
	}
	pt1 = pa[j], pt2 = pa[0];											// 下部基準点
	for(i=1; i<an; i++) {
		pt2.p[1] = (pa[i].p[1]<pt2.p[1]) ? pa[i].p[1] : pt2.p[1];		// 2D-Y最小点
		pt2.p[2] = (pa[i].p[2]>pt2.p[2]) ? pa[i].p[2] : pt2.p[2];		// 2D-Z最大点
	}
	sln = PntD(&pt2, &pa[0]);
	for(i=1, j=0; i<an; i++) {
		if((wln=PntD(&pt2, &pa[i]))<sln) {								// 最小距離点検索
			sln = wln, j = i;
		}
	}
	pt2 = pa[j];														// 上部基準点
	BaseObjPtr(2, &op, &i, &j);											// 編集対象立体取得
	while(op!=NULL) {
		gop[gcnt++] = op;												// 石座立体保存
		NextObjPtr(2, &op, &i, &j);										// 次編集立体へ
	}
	OffEdtAllObject();													// 編集立体全解除
	fname = GetAppPath(0)+PRO_NAME;										// ﾌｧｲﾙ名取得
	if(ReadJsdFile(fname, 0, 0)!=0) {									// ﾌﾟﾘﾐﾃｨﾌﾞ入力
		AfxMessageBox(IDS_CANT_READ_FILE);								// ｴﾗｰﾒｯｾｰｼﾞ
		delete[] sop;
		delete[] gop;
		return;															// 処理中止
	}
	GetObjPtr(&op);														// 立体ﾎﾟｲﾝﾀ取得
	sop[cnt++] = op;													// 立体ﾎﾟｲﾝﾀ保存
	OnEdtFlg(1, op);													// 立体edtflg:ON
	GetObjBox(op, &box);												// ﾎﾞｯｸｽｻｲｽﾞ取得
	BoxCenter(box, &cpt);												// ﾎﾞｯｸｽ中心取得
	wln = PntD(&pt1, &pt2);												// 基準点間距離
	xd = box.bp[1].p[0]-box.bp[0].p[0];									// 読込ｻｲｽﾞ算出
	yd = box.bp[1].p[1]-box.bp[0].p[1];
	zd = box.bp[1].p[2]-box.bp[0].p[2];
	xd = dm/xd;															// X方向倍率算出
	yd = dm/yd;															// Y方向倍率算出
	zd = (ln+wln)/zd;													// Z方向倍率算出
	Scale(cpt, xd, yd, zd);												// 指定ｻｲｽﾞﾘｻｲｽﾞ
	GetObjBox(op, &box);												// ﾎﾞｯｸｽｻｲｽﾞ取得
	BoxCenter(box, &cpt);												// ﾎﾞｯｸｽ中心取得
	pt3.p[0]=cpt.p[0], pt3.p[1]=box.bp[1].p[1],	pt3.p[2]=box.bp[0].p[2];// 爪側基準点
	th = atan2(pt2.p[1]-pt1.p[1], pt2.p[2]-pt1.p[2]);					// 回転角度
	th = (th<-10||th>10) ? 0.0 : th;
	th = 180.0/PI*th;
	rvc.p[0] = -1.0, rvc.p[1] = 0.0, rvc.p[2] = 0.0;					// 回転軸
	for(i=0; i<DIM; i++) {
		pt3.p[i] = pt1.p[i]-pt3.p[i];									// 移動量
	}
	Move(pt3.p[0], pt3.p[1]+rv, pt3.p[2]);								// 爪移動
	Rotate(pt1, rvc, th);												// 爪回転
	BoxCenter(obx, &cpt);												// ﾎﾞｯｸｽ中心取得
	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = -1.0;					// 複写回転軸
	th = 360.0/nm;														// 複写角度
	if(nm>=4&&nm%2==0) {												// <爪数:4以上で偶数個>
		Rotate(cpt, rvc, th/2.0);										// 爪回転(回転角度:複写角度/2)
	}
	for(i=1; i<nm; i++) {												// <2個目以降を複写･回転>
		Copy();															// 立体複写
		GetObjPtr(&wop);												// 立体ﾎﾟｲﾝﾀ取得
		sop[cnt++] = wop;												// 立体ﾎﾟｲﾝﾀ保存
		OffEdtFlg(1, op);												// 立体edtflg:OFF
		OnEdtFlg(1, wop);												// 立体edtflg:ON
		Rotate(cpt, rvc, th*i);											// 爪回転
		OffEdtFlg(1, wop);												// 立体edtflg:OFF
		OnEdtFlg(1, op);												// 立体edtflg:ON
	}
	for(i=0; i<cnt; i++) {
		ResetSurfaceObject(sop[i], &wop);								// 曲面立体再作成
		FreeObj(sop[i], 1);												// 立体削除
		sop[i] = wop;
	}
	for(i=0; i<cnt; i++) {
		PutObjPrt(sop[i], 2, 1);										// ﾊﾟｰﾂNo設定(子)
		GetObjOif(sop[i], &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
		SetOifLevel(oif, 1);											// 構成ﾚﾍﾞﾙ(1)
		SetOifName(0, oif, OBJ_NAME31);									// 構成名(爪)
		OnEdtFlg(1, sop[i]);											// 複写爪edtflg:ON
	}
	for(i=0; i<gcnt; i++) {
		OnEdtFlg(1, gop[i]);											// 石座edtflg:ON
	}
	delete[] sop;
	delete[] gop;
}
