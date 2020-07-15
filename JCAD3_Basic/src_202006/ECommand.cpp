/////////////////////////////////////////////////////////////////////////////
// 立体編集コマンド部
// ECommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "RCommand.h"
#include "Select.h"
#include "NumFunc.h"

/******************************************/
void Move(double dx, double dy, double dz)								//<<< 平行移動
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(MOVE, 3, 0, &cmp);										// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// 移動量設定
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 移動ｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*******************************************************/
static void CopyMove(double dx, double dy,
                     double dz, PNTTYPE cp, PNTTYPE sc)					//<<< 平行移動(複写立体)
/*******************************************************/
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		SetObjPtr(op);													// 複写対象立体設定
		wop = CopyObject();												// 立体複写
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <倍率変更有の場合>
			CreatCmd(SCALE, 3, 1, &cmp);								// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(sc.p[0]*1000.0));					// 各方向倍率設定
			PutParaInt(cmp, 1, (int)(sc.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(sc.p[2]*1000.0));
			PutParaPnt(cmp, 0, cp);										// 変換中心座標設定
			SetObjPtr(wop);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// ｻｲｽﾞ変更ｺﾏﾝﾄﾞ実行
		}
		CreatCmd(MOVE, 3, 0, &cmp);										// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// 移動量設定
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		SetObjPtr(wop);													// 立体ﾎﾟｲﾝﾀ設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 移動ｺﾏﾝﾄﾞ実行
		View->SetDispList(wop);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*******************************************************************/
static void GetLnBase(double dx, double dy, double dz, PNTTYPE* pa)		//<<< 距離測定基準点取得
/*******************************************************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt, cpt, pt1, pt2, pp1, pp2;
	VECTYPE  vec;
	BOXTYPE  box;
	int      i, dsp, edt, num;
	double   ln, min1=DBL_MAX, min2=DBL_MAX;

	GetAreaBox(2, &box);												// ﾎﾞｯｸｽｻｲｽﾞ取得
	ln = BoxCenter(box, &cpt);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
	vec.p[0] = dx, vec.p[1] = dy, vec.p[2] = dz;						// 移動方向ﾍﾞｸﾄﾙ
	VecN(&vec);															// 単位ﾍﾞｸﾄﾙ化
	for(i=0; i<DIM; i++) {
		pt1.p[i] = cpt.p[i]+vec.p[i]*ln;								// 移動方向上の1点(A)
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] *= -1.0;												// 逆移動方向ﾍﾞｸﾄﾙ
	}
	for(i=0; i<DIM; i++) {
		pt2.p[i] = cpt.p[i]+vec.p[i]*ln;								// 移動方向上の1点(B)
	}
	for(i=0; i<DIM; i++) {
		pp1.p[i] = 0.0, pp2.p[i] = 0.0;
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		num = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pnt);										// 頂点取得
			ln = PntD(&pnt, &pt1);										// 2点間距離(3D)
			if(ln<min1) {												// A点に最も近い点
				min1 = ln, pp1 = pnt;
			}
			ln = PntD(&pnt, &pt2);										// 2点間距離(3D)
			if(ln<min2) {												// B点に最も近い点
				min2 = ln, pp2 = pnt;
			}
		}
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	pa[0] = pp1, pa[1] = pp2;
}

/********************************************************************/
static void CalcLn(PNTTYPE cp, PNTTYPE ws, PNTTYPE* pa, PNTTYPE* ln)	//<<< ｻｲｽﾞ変更後距離算出
/********************************************************************/
{
	PNTTYPE pt[2];
	int     i, j;
	double  dd;

	for(i=0; i<2; i++) {
		for(j=0; j<DIM; j++) {
			dd = pa[i].p[j]-cp.p[j];									// 基準点->対象点ﾍﾞｸﾄﾙ
			dd = dd*ws.p[j];											// ﾍﾞｸﾄﾙ倍率積算
			pt[i].p[j] = dd+cp.p[j];									// ｻｲｽﾞ変更後対象点
		}
	}
	for(i=0; i<DIM; i++) {
		ln->p[i] = fabs(pt[0].p[i]-pt[1].p[i]);							// 対象点間距離
	}
}

/****************************************************************/
static void CopyMoveCopy(double dx, double dy, double dz,
                         int cn, PNTTYPE cp, PNTTYPE sc, int sf)		//<<< 平行移動複写
/****************************************************************/		//    (複写立体)
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  pa[2], ln1, ln2, ss, is, ws;
	int      i, j, dsp, edt;
	double   ix, iy, iz, wx, wy, wz;

	if((fabs(sc.p[0])>EPSILON||
        fabs(sc.p[1])>EPSILON||
        fabs(sc.p[2])>EPSILON)&&sf!=0) {
		GetLnBase(dx, dy, dz, pa);										// 距離測定基準点取得
		for(i=0; i<DIM; i++) {
			ss.p[i] = (1.0+sc.p[i]<0.01) ? 0.01 : 1.0+sc.p[i];			// 倍率計算
		}
		CalcLn(cp, ss, pa, &ln1);										// 1個目の距離算出
	}
	ix = dx, iy = dy, iz = dz, is = sc;									// 基準移動量															// 開始倍率
	for(i=0; i<cn; i++) {												// 複写数分行う
		wx = dx, wy = dy, wz = dz;
		if(fabs(sc.p[0])>EPSILON||
           fabs(sc.p[1])>EPSILON||
           fabs(sc.p[2])>EPSILON) {										// <倍率変更有の場合>
			for(j=0; j<DIM; j++) {
				is.p[j] += sc.p[j];										// 倍率加算
				ws.p[j] = (1.0+is.p[j]<0.01) ? 0.01 : 1.0+is.p[j];		// 倍率計算
			}
			if(sf==1) {													// <等間隔連動の場合>
				CalcLn(cp, ws, pa, &ln2);								// n+1個目の距離算出
				if(dx>0) {												// 1個目とn+1個目の差加算
					wx = dx-(ln1.p[0]-ln2.p[0]);
				} else if(dx<0) {
					wx = dx+(ln1.p[0]-ln2.p[0]);
				}
				if(dy>0) {
					wy = dy-(ln1.p[1]-ln2.p[1]);
				} else if(dy<0) {
					wy = dy+(ln1.p[1]-ln2.p[1]);
				}
				if(dz>0) {
					wz = dz-(ln1.p[2]-ln2.p[2]);
				} else if(dz<0) {
					wz = dz+(ln1.p[2]-ln2.p[2]);
				}
			} else if(sf==2) {											// <不等間隔連動の場合>
				wx = dx*ws.p[0]/ss.p[0];								// 1個目とn+1個目の比率
				wy = dy*ws.p[1]/ss.p[1];
				wz = dz*ws.p[2]/ss.p[2];
			}
		}
		ix += wx, iy += wy, iz += wz;									// 移動量加算
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
		while(op!=NULL) {
			SetObjPtr(op);												// 複写対象立体設定
			wop = CopyObject();											// 立体複写
			if(fabs(sc.p[0])>EPSILON||
               fabs(sc.p[1])>EPSILON||
               fabs(sc.p[2])>EPSILON) {									// <倍率変更有の場合>
				CreatCmd(SCALE, 3, 1, &cmp);							// ｺﾏﾝﾄﾞ領域確保
				PutParaInt(cmp, 0, (int)(ws.p[0]*1000.0));				// 各方向倍率設定
				PutParaInt(cmp, 1, (int)(ws.p[1]*1000.0));
				PutParaInt(cmp, 2, (int)(ws.p[2]*1000.0));
				PutParaPnt(cmp, 0, cp);									// 変換中心座標設定
				SetObjPtr(wop);											// 対象立体設定
				SetCmdPtr(cmp);											// ｺﾏﾝﾄﾞ設定
				ExecCommand(FALSE);										// ｻｲｽﾞ変更ｺﾏﾝﾄﾞ実行
			}
			CreatCmd(MOVE, 3, 0, &cmp);									// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(ix*1000.0));						// 移動量設定
			PutParaInt(cmp, 1, (int)(iy*1000.0));
			PutParaInt(cmp, 2, (int)(iz*1000.0));
			SetObjPtr(wop);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 移動ｺﾏﾝﾄﾞ実行
			View->SetDispList(wop);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			NextObjPtr(2, &op, &dsp, &edt);								// 次立体へ
		}
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*****************************************************/
static void RedoMove(double dx, double dy, double dz)					//<<< 平行移動(再実行)
/*****************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==MOVE) {										// <移動ｺﾏﾝﾄﾞの場合>
			PutParaInt(cmp, 0, (int)(dx*1000.0));						// 移動量設定
			PutParaInt(cmp, 1, (int)(dy*1000.0));
			PutParaInt(cmp, 2, (int)(dz*1000.0));
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/**********************************************************/
void ObjMove(OBJTYPE* op, double dx, double dy, double dz)				//<<< 指定立体の移動
/**********************************************************/
{
	CMDTYPE* cmp;

	if(op!=NULL) {
		CreatCmd(MOVE, 3, 0, &cmp);										// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// 移動量設定
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 移動ｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/************************************************************/
void ObjectMove(int flg, int cn, PNTTYPE sc,
                SCRTYPE bp, double dx, double dy, double dz)			//<<< 立体平行移動
/************************************************************/
{
	BOXTYPE box;
	PNTTYPE cpt, wsc;
	int     i;

	if(flg==0) {														// <初回のみ>
		InitLevel();													// Undo/Redo初期化
		if(cn>0) {														// <移動複写の場合>
			for(i=0; i<DIM; i++) {
				wsc.p[i] = 1.0;
			}
			if(fabs(sc.p[0]-1.0)>EPSILON||
               fabs(sc.p[1]-1.0)>EPSILON||
               fabs(sc.p[2]-1.0)>EPSILON) {								// <倍率変更有>
				GetAreaBox(2, &box);									// ﾎﾞｯｸｽｻｲｽﾞ取得
				for(i=0; i<DIM; i++) {
					if(bp.p[i]==0) {									// 基準点算出
						cpt.p[i] = box.bp[0].p[i];
					} else if(bp.p[i]==2) {
						cpt.p[i] = box.bp[1].p[i];
					} else {
						cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
					}
					wsc.p[i] = (sc.p[i]<0.01) ? 0.01 : sc.p[i];			// 倍率計算
				}
			}
			CopyMove(dx, dy, dz, cpt, wsc);								// 複写立体移動
		} else {														// <移動の場合>
			Move(dx, dy, dz);											// 平行移動
		}
	} else {															// <2回目以降>
		RedoMove(dx, dy, dz);											// 平行移動再実行
	}
}

/*********************************************************/
void EndMove(int cn, PNTTYPE sc, int sf,
             SCRTYPE bp, double dx, double dy, double dz)				//<<< 平行移動終了処理
/*********************************************************/
{
	OBJTYPE* op;
	BOXTYPE  box;
	PNTTYPE  cpt, wsc;
	int      i;

	if(cn>1) {															// <2個以上複写の場合>
		for(i=0; i<DIM; i++) {
			wsc.p[i] = 0.0;
		}
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <倍率変更有>
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			for(i=0; i<DIM; i++) {
				wsc.p[i] = sc.p[i]-1.0;									// 倍率増減分
				if(bp.p[i]==0) {										// 基準点算出
					cpt.p[i] = box.bp[0].p[i];
				} else if(bp.p[i]==2) {
					cpt.p[i] = box.bp[1].p[i];
				} else {
					cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
				}
			}
		}
		CopyMoveCopy(dx, dy, dz, cn-1, cpt, wsc, sf);					// 立体移動複写(2個以降)
	}
	if(cn>0) {															// <複写の場合>
		OffEdtAllObject();												// 編集立体全解除
		LevelBaseObjPtr(0, &op);										// 生成立体取得
		while(op) {
			OnEdtFlg(1, op);											// edtflg:ON
			LevelNextObjPtr(0, &op);
		}
	}
}

/***********************************************/
void Rotate(PNTTYPE cp, VECTYPE vec, double th)							//<<< 回転移動
/***********************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      dsp, edt, i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(ROTATE, 1, 2, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(th*1000.0));							// 回転角度設定
		PutParaPnt(cmp, 0, cp);											// 回転軸座標設定
		PutParaPnt(cmp, 1, pnt);										// 回転軸方向ﾍﾞｸﾄﾙ設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 回転ｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***********************************************************/
static void CopyRotate(PNTTYPE cp, VECTYPE vec, double th,
                       PNTTYPE mov, PNTTYPE bp, PNTTYPE sc)				//<<< 回転移動(立体複写)
/***********************************************************/
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, dsp, edt;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		SetObjPtr(op);													// 複写対象立体設定
		wop = CopyObject();												// 立体複写
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <倍率変更有の場合>
			CreatCmd(SCALE, 3, 1, &cmp);								// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(sc.p[0]*1000.0));					// 各方向倍率設定
			PutParaInt(cmp, 1, (int)(sc.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(sc.p[2]*1000.0));
			PutParaPnt(cmp, 0, bp);										// 変換中心座標設定
			SetObjPtr(wop);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// ｻｲｽﾞ変更ｺﾏﾝﾄﾞ実行
		}
		CreatCmd(ROTATE, 1, 2, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(th*1000.0));							// 回転角度設定
		PutParaPnt(cmp, 0, cp);											// 回転軸座標設定
		PutParaPnt(cmp, 1, pnt);										// 回転軸方向ﾍﾞｸﾄﾙ設定
		SetObjPtr(wop);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 回転ｺﾏﾝﾄﾞ実行
		CreatCmd(MOVE, 3, 0, &cmp);										// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(mov.p[0]*1000.0));						// 移動量設定
		PutParaInt(cmp, 1, (int)(mov.p[1]*1000.0));
		PutParaInt(cmp, 2, (int)(mov.p[2]*1000.0));
		SetObjPtr(wop);													// 立体ﾎﾟｲﾝﾀ設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 移動ｺﾏﾝﾄﾞ実行
		View->SetDispList(wop);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*********************************************************/
static void GetThBase(PNTTYPE cp, PNTTYPE* pa, int plane)				//<<< 角度測定基準点取得
/*********************************************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt, minp1, maxp1, minp2, maxp2;
	VECTYPE  vec;
	int      i, i1, i2, i3, dsp, edt, num;
	double   th, t1, t2, min1=100.0, max1=-100.0, min2=100.0, max2=-100.0;

	for(i=0; i<DIM; i++) {
		minp1.p[i] = 0.0, maxp1.p[i] = 0.0;
		minp2.p[i] = 0.0, maxp2.p[i] = 0.0;
	}
	i1 = plane;
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		num = GetVtxNum(op);											// 頂点数取得
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pnt);										// 頂点取得
			vec.p[i1] = pnt.p[i1]-cp.p[i1];								// 中心点からのﾍﾞｸﾄﾙ
			vec.p[i2] = pnt.p[i2]-cp.p[i2];
			vec.p[i3] = 0.0;
			VecN(&vec);													// 単位ﾍﾞｸﾄﾙ化
			th = atan2(vec.p[i2], vec.p[i1]);							// i1軸からの角度
			th = (th<-10||th>10) ? 0.0 : th;
			th = (th<0) ? th+(2.0*PI) : th;
			if(min1>th) {												// i1最小保存
				min1 = th, pnt.p[i3] = 0.0, minp1 = pnt;
			}
			if(max1<th) {												// i1最大保存
				max1 = th, pnt.p[i3] = 0.0, maxp1 = pnt;
			}
			th = atan2(vec.p[i1], vec.p[i2]);							// i2軸からの角度
			th = (th<-10||th>10) ? 0.0 : th;
			th = (th<0) ? th+(2.0*PI) : th;
			if(min2>th) {												// i2最小保存
				min2 = th, pnt.p[i3] = 0.0, minp2 = pnt;
			}
			if(max2<th) {												// i2最大保存
				max2 = th, pnt.p[i3] = 0.0, maxp2 = pnt;
			}
		}
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	t1 = (max1-min1>PI) ? 2.0*PI-(max1-min1) : max1-min1;				// 180度未満変換
	t2 = (max2-min2>PI) ? 2.0*PI-(max2-min2) : max2-min2;
	if(t1>t2) {															// 角度大採用
		pa[0] = minp1, pa[1] = maxp1;
	} else {
		pa[0] = minp2, pa[1] = maxp2;
	}
}

/*************************************************************/
static double CalcTh(PNTTYPE pc, PNTTYPE pb,
                     PNTTYPE* pa, PNTTYPE sc, int i1, int i2)			//<<< ｻｲｽﾞ変更後角度算出
/*************************************************************/
{
	PNTTYPE pt[2];
	VECTYPE svc, rvc;
	int     i, j;
	double  th=0.0, wd;

	for(i=0; i<2; i++) {
		for(j=0; j<DIM; j++) {
			wd = pa[i].p[j]-pb.p[j];									// 基準点->対象点ﾍﾞｸﾄﾙ
			wd = wd*sc.p[j];											// ﾍﾞｸﾄﾙ倍率積算
			pt[i].p[j] = wd+pb.p[j];									// ｻｲｽﾞ変更後対象点
		}
	}
	VecV(&pt[0], &pc, &svc);											// 1->2点間ﾍﾞｸﾄﾙ取得
	VecV(&pc, &pt[1], &rvc);											// 2->3点間ﾍﾞｸﾄﾙ取得
	VecN(&svc); VecN(&rvc);												// 単位ﾍﾞｸﾄﾙ化
	if(fabs(svc.p[i1]-rvc.p[i1])>EPSILON||
       fabs(svc.p[i2]-rvc.p[i2])>EPSILON) {								// 2ﾍﾞｸﾄﾙ間角度
		wd = acos(svc.p[i1]*rvc.p[i1]+svc.p[i2]*rvc.p[i2]);
		wd = (wd<-10||wd>10) ? 0.0 : wd;
		wd = PI - fabs(wd);
		th = (th<wd) ? wd : th;
	}
	return 180.0*th/PI;													// 角度
}

/******************************************************************/
static void CopyRotateCopy(PNTTYPE cp, VECTYPE vec,
                           double th, int pl, PNTTYPE mov,
                           int cn, PNTTYPE bp, PNTTYPE sc, int sf)		//<<< 回転移動複写
/******************************************************************/	//   (複写立体)
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  pnt, smv, pc, pb, pa[2], ssc, isc, wsc;
	int      i, j, i1, i2, dsp, edt;
	double   ith, wth, it1, it2;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	if((fabs(sc.p[0])>EPSILON||
        fabs(sc.p[1])>EPSILON||
        fabs(sc.p[2])>EPSILON)&&sf!=0) {
		GetThBase(cp, pa, pl);											// 角度測定基準点取得
		i1 = pl;
		i2 = (pl+1>2) ? 0 : pl+1;
		for(i=0; i<DIM; i++) {
			ssc.p[i] = (1.0+sc.p[i]<0.01) ? 0.01 : 1.0+sc.p[i];			// 倍率計算
			pc.p[i] = 0.0, pb.p[i] = 0.0;								// ﾃﾞｰﾀ初期化
		}
		pc.p[i1] = cp.p[i1], pc.p[i2] = cp.p[i2];						// 回転中心点
		pb.p[i1] = bp.p[i1], pb.p[i2] = bp.p[i2];						// ｻｲｽﾞ変更基準点
		it1 = CalcTh(pc, pb, pa, ssc, i1, i2);							// 1個目角度
	}
	smv = mov;															// 基準変位量
	isc = sc;															// 開始倍率
	ith = th;															// 基準回転角度
	for(i=0; i<cn; i++) {												// 複写数分行う
		wth = th;
		if(fabs(sc.p[0])>EPSILON||
           fabs(sc.p[1])>EPSILON||
           fabs(sc.p[2])>EPSILON) {										// <倍率変更有の場合>
			for(j=0; j<DIM; j++) {
				isc.p[j] += sc.p[j];									// 倍率加算
				wsc.p[j] = (1.0+isc.p[j]<0.01) ? 0.01 : 1.0+isc.p[j];	// 倍率計算
			}
			if(sf==1) {													// <等間隔連動の場合>
				it2 = CalcTh(pc, pb, pa, wsc, i1, i2);					// n+1個目の角度
				it2 = it1-it2;											// 1個目とn+1個目の差
				if(th>0) {
					wth = th-it2;
				} else if(th<0) {
					wth = th+it2;
				}
			} else if(sf==2) {											// <不等間隔連動の場合>
				it2 = CalcTh(pc, pb, pa, wsc, i1, i2);					// n+1個目の角度
				wth = th*it2/it1;										// 1個目とn+1個目の比率
			}
		}
		ith += wth;														// 回転角度加算
		for(j=0; j<DIM; j++) {
			smv.p[j] += mov.p[j];										// 変位量加算
		}
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
		while(op!=NULL) {
			SetObjPtr(op);												// 複写対象立体設定
			wop = CopyObject();											// 立体複写
			if(fabs(sc.p[0])>EPSILON||
               fabs(sc.p[1])>EPSILON||
               fabs(sc.p[2])>EPSILON) {									// <倍率変更有の場合>
				CreatCmd(SCALE, 3, 1, &cmp);							// ｺﾏﾝﾄﾞ領域確保
				PutParaInt(cmp, 0, (int)(wsc.p[0]*1000.0));				// 各方向倍率設定
				PutParaInt(cmp, 1, (int)(wsc.p[1]*1000.0));
				PutParaInt(cmp, 2, (int)(wsc.p[2]*1000.0));
				PutParaPnt(cmp, 0, bp);									// 変換中心座標設定
				SetObjPtr(wop);											// 対象立体設定
				SetCmdPtr(cmp);											// ｺﾏﾝﾄﾞ設定
				ExecCommand(FALSE);										// ｻｲｽﾞ変更ｺﾏﾝﾄﾞ実行
			}
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(ith*1000.0));						// 回転角度設定
			PutParaPnt(cmp, 0, cp);										// 回転軸座標設定
			PutParaPnt(cmp, 1, pnt);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetObjPtr(wop);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
			CreatCmd(MOVE, 3, 0, &cmp);									// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(smv.p[0]*1000.0));					// 移動量設定
			PutParaInt(cmp, 1, (int)(smv.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(smv.p[2]*1000.0));
			SetObjPtr(wop);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 移動ｺﾏﾝﾄﾞ実行
			View->SetDispList(wop);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			NextObjPtr(2, &op, &dsp, &edt);								// 次立体へ
		}
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/**********************************************************/
static void RedoRotate(PNTTYPE cp, VECTYPE vec, double th)				//<<< 回転移動(再実行)
/**********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==ROTATE) {										// <回転ｺﾏﾝﾄﾞの場合>
			PutParaInt(cmp, 0, (int)(th*1000.0));						// 回転角度設定
			PutParaPnt(cmp, 0, cp);										// 回転軸座標設定
			PutParaPnt(cmp, 1, pnt);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/***************************************************************/
void ObjRotate(OBJTYPE* op, PNTTYPE cp, VECTYPE vec, double th)			//<<< 指定立体の回転
/***************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	if(op!=NULL) {
		CreatCmd(ROTATE, 1, 2, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(th*1000.0));							// 回転角度設定
		PutParaPnt(cmp, 0, cp);											// 回転軸座標設定
		PutParaPnt(cmp, 1, pnt);										// 回転軸方向ﾍﾞｸﾄﾙ設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 回転ｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/***********************************************************/
void ObjectRotate(int flg, int cn, PNTTYPE sc, SCRTYPE bp,
                  PNTTYPE cp, int pl, double th, double mv)				//<<< 立体回転移動
/***********************************************************/
{
	VECTYPE vec;
	BOXTYPE box;
	PNTTYPE cpt, mov, wsc;
	int     i;

	for(i=0; i<DIM; i++) {
		vec.p[i] = 0.0, mov.p[i] = 0.0, wsc.p[i] = 1.0;
	}
	i = (pl-1<0) ? 2 : pl-1;
	vec.p[i] = -1.0;													// 回転軸方向ﾍﾞｸﾄﾙ算出
	mov.p[i] = mv;														// 変位量設定
	if(flg==0) {														// <初回のみ>
		InitLevel();													// Undo/Redo初期化
		if(cn>0) {														// <移動複写の場合>
			if(fabs(sc.p[0]-1.0)>EPSILON||
               fabs(sc.p[1]-1.0)>EPSILON||
               fabs(sc.p[2]-1.0)>EPSILON) {								// <倍率変更有>
				GetAreaBox(2, &box);									// ﾎﾞｯｸｽｻｲｽﾞ取得
				for(i=0; i<DIM; i++) {
					if(bp.p[i]==0) {									// 基準点算出
						cpt.p[i] = box.bp[0].p[i];
					} else if(bp.p[i]==2) {
						cpt.p[i] = box.bp[1].p[i];
					} else {
						cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
					}
					wsc.p[i] = (sc.p[i]<0.01) ? 0.01 : sc.p[i];			// 倍率計算
				}
			}
			CopyRotate(cp, vec, th, mov, cpt, wsc);						// 複写立体回転
		} else {														// <回転の場合>
			Rotate(cp, vec, th);										// 回転移動
			Move(mov.p[0], mov.p[1], mov.p[2]);							// 平行移動
		}
	} else {															// <2回目以降>
		RedoRotate(cp, vec, th);										// 回転移動再実行
		RedoMove(mov.p[0], mov.p[1], mov.p[2]);							// 平行移動再実行
	}
}

/********************************************************/
void EndRotate(int cn, PNTTYPE sc, int sf, SCRTYPE bp,
               PNTTYPE cp, int pl, double th, double mv)				//<<< 回転移動終了処理
/********************************************************/
{
	OBJTYPE* op;
	VECTYPE  vec;
	PNTTYPE  mov, cpt, wsc;
	BOXTYPE  box;
	int      i;

	for(i=0; i<DIM; i++) {
		vec.p[i] = 0.0, mov.p[i] = 0.0, wsc.p[i] = 0.0;
	}
	i = (pl-1<0) ? 2 : pl-1;
	vec.p[i] = -1.0;													// 回転軸方向ﾍﾞｸﾄﾙ算出
	mov.p[i] = mv;														// 変位量設定
	if(cn>1) {															// <2個以上複写の場合>
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <倍率変更有>
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			for(i=0; i<DIM; i++) {
				wsc.p[i] = sc.p[i]-1.0;									// 倍率増減分
				if(bp.p[i]==0) {										// 基準点算出
					cpt.p[i] = box.bp[0].p[i];
				} else if(bp.p[i]==2) {
					cpt.p[i] = box.bp[1].p[i];
				} else {
					cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
				}
			}
		}
		CopyRotateCopy(cp, vec, th, pl, mov, cn-1, cpt, wsc, sf);		// 立体回転複写(2個以降)
	}
	if(cn>0) {															// <複写の場合>
		OffEdtAllObject();												// 編集立体全解除
		LevelBaseObjPtr(0, &op);										// 生成立体取得
		while(op) {
			OnEdtFlg(1, op);											// edtflg:ON
			LevelNextObjPtr(0, &op);
		}
	}
}

/*******************************************************/
static void Mirror(PNTTYPE spt, PNTTYPE ept, int plane)					//<<< 鏡像移動
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(MIRROR, 1, 2, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, plane);										// ﾊﾟﾗﾒｰﾀ設定
		PutParaPnt(cmp, 0, spt);
		PutParaPnt(cmp, 1, ept);
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 鏡像ｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***********************************************************/
static void RedoMirror(PNTTYPE spt, PNTTYPE ept, int plane)				//<<< 鏡像移動(再実行)
/***********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==MIRROR) {										// <鏡像ｺﾏﾝﾄﾞの場合>
			PutParaInt(cmp, 0, plane);									// ﾊﾟﾗﾒｰﾀ設定
			PutParaPnt(cmp, 0, spt);
			PutParaPnt(cmp, 1, ept);
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/******************************************************************/
void ObjectMirror(int flg, int cf, PNTTYPE sp, PNTTYPE ep, int pl)		// 立体鏡像移動
/******************************************************************/
{
	OBJTYPE* op;

	if(flg==0) {														// <初回のみ>
		InitLevel();													// Undo/Redo初期化
		if(cf!=0) {														// <複写の場合>
			Copy();														// 立体複写
			OffEdtAllObject();											// 編集立体全解除
			LevelBaseObjPtr(0, &op);									// 生成立体取得
			while(op) {
				OnEdtFlg(1, op);										// edtflg:ON
				LevelNextObjPtr(0, &op);
			}
		}
		Mirror(sp, ep, pl);												// 鏡像移動
	} else {															// <2回目以降>
		RedoMirror(sp, ep, pl);											// 鏡像移動再実行
	}
}

/*******************************************************/
void Scale(PNTTYPE cp, double sx, double sy, double sz)					//<<< ｻｲｽﾞ変更
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(SCALE, 3, 1, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(sx*1000.0));							// 各方向倍率設定
		PutParaInt(cmp, 1, (int)(sy*1000.0));
		PutParaInt(cmp, 2, (int)(sz*1000.0));
		PutParaPnt(cmp, 0, cp);											// 変換中心座標設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// ｻｲｽﾞ変更ｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/******************************************************************/
static void RedoScale(PNTTYPE cp, double sx, double sy, double sz)		//<<< ｻｲｽﾞ変更(再実行)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SCALE) {										// <ｻｲｽﾞ変更ｺﾏﾝﾄﾞの場合>
			PutParaInt(cmp, 0, (int)(sx*1000.0));						// 各方向倍率設定
			PutParaInt(cmp, 1, (int)(sy*1000.0));
			PutParaInt(cmp, 2, (int)(sz*1000.0));
			PutParaPnt(cmp, 0, cp);										// 変換中心座標設定
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/**********************************************************************/
void ObjectScale(int flg, PNTTYPE cp, double sx, double sy, double sz)	//<<< 立体ｻｲｽﾞ変更
/**********************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Scale(cp, sx, sy, sz);											// ｻｲｽﾞ変更
	} else {															// <2回目以降>
		RedoScale(cp, sx, sy, sz);										// ｻｲｽﾞ変更再実行
	}
}

/***********************************************************/
void Resize1(int pln, int mpf, double s1, double s2,
             double s3, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)				//<<< 直線指定ﾘｻｲｽﾞ1
/***********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(RESIZE1, 5, 3, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, pln);										// 指定平面設定
		PutParaInt(cmp, 1, mpf);										// 正負ﾌﾗｸﾞ設定
		PutParaInt(cmp, 2, (int)(s1*1000.0));							// 倍率1設定
		PutParaInt(cmp, 3, (int)(s2*1000.0));							// 倍率2設定
		PutParaInt(cmp, 4, (int)(s3*1000.0));							// 倍率3設定
		PutParaPnt(cmp, 0, p1);											// 基準点1設定
		PutParaPnt(cmp, 1, p2);											// 基準点2設定
		PutParaPnt(cmp, 2, p3);											// 基準点3設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 直線指定ﾘｻｲｽﾞｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/**********************************************************************/
static void RedoResize1(int pln, int mpf, double s1, double s2,
                        double s3, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)	//<<< 直線指定ﾘｻｲｽﾞ1(再実行)
/**********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE1) {									// <直線指定ﾘｻｲｽﾞ1の場合>
			PutParaInt(cmp, 0, pln);									// 指定平面設定
			PutParaInt(cmp, 1, mpf);									// 正負ﾌﾗｸﾞ設定
			PutParaInt(cmp, 2, (int)(s1*1000.0));						// 倍率1設定
			PutParaInt(cmp, 3, (int)(s2*1000.0));						// 倍率2設定
			PutParaInt(cmp, 4, (int)(s3*1000.0));						// 倍率3設定
			PutParaPnt(cmp, 0, p1);										// 基準点1設定
			PutParaPnt(cmp, 1, p2);										// 基準点2設定
			PutParaPnt(cmp, 2, p3);										// 基準点3設定
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/******************************************************/
void ObjectResize1(int flg, int pln, int mpf,
                   double s1, double s2, double s3,
                   PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)					//<<< 立体直線指定ﾘｻｲｽﾞ1
/******************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Resize1(pln, mpf, s1, s2, s3, p1, p2, p3);						// 直線指定ﾘｻｲｽﾞ1
	} else {															// <2回目以降>
		RedoResize1(pln, mpf, s1, s2, s3, p1, p2, p3);					// 直線指定ﾘｻｲｽﾞ1再実行
	}
}

/*****************************************************************/
void Resize2(int f1, int f2, int d1, int d2,
             double ln, PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)		//<<< 曲線指定ﾘｻｲｽﾞ1
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(RESIZE2, 6+pn, 1+pn, &cmp);							// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, f1);											// 正負ﾌﾗｸﾞ設定
		PutParaInt(cmp, 1, f2);											// 両側ﾌﾗｸﾞ設定
		PutParaInt(cmp, 2, d1);											// 長さ方向設定
		PutParaInt(cmp, 3, d2);											// ｻｲｽﾞ指定方向設定
		PutParaInt(cmp, 4, (int)(ln*1000.0));							// 基準長さ設定
		PutParaInt(cmp, 5, pn);											// 制御点数設定
		for(i=0; i<pn; i++) {
			PutParaInt(cmp, i+6, ia[i]);								// 曲線ﾓｰﾄﾞ設定
		}
		PutParaPnt(cmp, 0, bs);											// 基準点設定
		for(i=0; i<pn; i++) {
			PutParaPnt(cmp, i+1, pa[i]);								// 制御点設定
		}
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 曲線指定ﾘｻｲｽﾞｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/******************************************************************/
static void RedoResize2(int f1, int f2, int d1, int d2, double ln,
                        PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)		//<<< 曲線指定ﾘｻｲｽﾞ1(再実行)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE2) {									// <<曲線指定ﾘｻｲｽﾞ1の場合>>
			spn = GetParaInt(cmp, 5);									// 制御点数取得
			if(pn!=spn) {												// <制御点数が異なる場合>
				ChangeCmd(cmp, 6+pn, 1+pn);								// ｺﾏﾝﾄﾞ領域変更
			}
			PutParaInt(cmp, 0, f1);										// 正負ﾌﾗｸﾞ設定
			PutParaInt(cmp, 1, f2);										// 両側ﾌﾗｸﾞ設定
			PutParaInt(cmp, 2, d1);										// 長さ方向設定
			PutParaInt(cmp, 3, d2);										// ｻｲｽﾞ指定方向設定
			PutParaInt(cmp, 4, (int)(ln*1000.0));						// 基準長さ設定
			PutParaInt(cmp, 5, pn);										// 制御点数設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, i+6, ia[i]);							// 曲線ﾓｰﾄﾞ設定
			}
			PutParaPnt(cmp, 0, bs);										// 基準点設定
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i+1, pa[i]);							// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(pn!=spn) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/***********************************************************************/
void ObjectResize2(int flg, int f1, int f2, int d1, int d2,
                   double ln, PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)	//<<< 立体曲線指定ﾘｻｲｽﾞ1
/***********************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Resize2(f1, f2, d1, d2, ln, bs, pn, pa, ia);					// 曲線指定ﾘｻｲｽﾞ1
	} else {															// <2回目以降>
		RedoResize2(f1, f2, d1, d2, ln, bs, pn, pa, ia);				// 曲線指定ﾘｻｲｽﾞ1再実行
	}
}

/************************************************************/
void Bend(int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)			//<<< 円筒曲げ
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(BEND, 3, 2, &cmp);										// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, fc);											// 曲げる水平方向設定
		PutParaInt(cmp, 1, vc);											// 曲げる垂直方向設定
		PutParaInt(cmp, 2, (int)(th*1000.0));							// 曲げる角度設定
		PutParaPnt(cmp, 0, sp);											// 基準線始点設定
		PutParaPnt(cmp, 1, ep);											// 基準線終点設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 円筒曲げｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***********************************************************************/
static void RedoBend(int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)	//<<< 円筒曲げ(再実行)
/***********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==BEND) {										// <円筒曲げｺﾏﾝﾄﾞの場合>
			PutParaInt(cmp, 0, fc);										// 曲げる水平方向設定
			PutParaInt(cmp, 1, vc);										// 曲げる垂直方向設定
			PutParaInt(cmp, 2, (int)(th*1000.0));						// 曲げる角度設定
			PutParaPnt(cmp, 0, sp);										// 基準線始点設定
			PutParaPnt(cmp, 1, ep);										// 基準線終点設定
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/**************************************************/
void ObjectBend(int flg, int fc, int vc,
                double th, PNTTYPE sp, PNTTYPE ep)						//<<< 立体円筒曲げ
/**************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Bend(fc, vc, th, sp, ep);										// 円筒曲げ
	} else {															// <2回目以降>
		RedoBend(fc, vc, th, sp, ep);									// 円筒曲げ再実行
	}
}

/****************************************************/
void Twist(int dr, double th, double ln, PNTTYPE cp)					//<<< ひねり
/****************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(TWIST, 3, 1, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, dr);											// ひねり軸方向設定
		PutParaInt(cmp, 1, (int)(th*1000.0));							// ひねり角度設定
		PutParaInt(cmp, 2, (int)(ln*1000.0));							// ひねり軸幅設定
		PutParaPnt(cmp, 0, cp);											// ひねり基準点設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// ひねりｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***************************************************************/
static void RedoTwist(int dr, double th, double ln, PNTTYPE cp)			//<<< ひねり(再実行)
/***************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==TWIST) {										// <ひねりｺﾏﾝﾄﾞの場合>
			PutParaInt(cmp, 0, dr);										// ひねり軸方向設定
			PutParaInt(cmp, 1, (int)(th*1000.0));						// ひねり角度設定
			PutParaInt(cmp, 2, (int)(ln*1000.0));						// ひねり軸幅設定
			PutParaPnt(cmp, 0, cp);										// ひねり基準点設定
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/*******************************************************************/
void ObjectTwist(int flg, int dr, double th, double ln, PNTTYPE cp)		//<<< 立体ひねり
/*******************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Twist(dr, th, ln, cp);											// ひねり
	} else {															// <2回目以降>
		RedoTwist(dr, th, ln, cp);										// ひねり再実行
	}
}

/*****************************************************/
void Shear(int dr1, int dr2, double inc, PNTTYPE bas)					//<<< せん断
/*****************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(SHEAR, 3, 1, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, dr1);										// せん断垂直方向設定
		PutParaInt(cmp, 1, dr2);										// せん断水平方向設定
		PutParaInt(cmp, 2, (int)(inc*1000.0));							// せん断傾き設定
		PutParaPnt(cmp, 0, bas);										// せん断基準点設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// せん断ｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/****************************************************************/
static void RedoShear(int dr1, int dr2, double inc, PNTTYPE bas)		//<<< せん断(再実行)
/****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SHEAR) {										// <せん断ｺﾏﾝﾄﾞの場合>
			PutParaInt(cmp, 0, dr1);									// せん断垂直方向設定
			PutParaInt(cmp, 1, dr2);									// せん断水平方向設定
			PutParaInt(cmp, 2, (int)(inc*1000.0));						// せん断傾き設定
			PutParaPnt(cmp, 0, bas);									// せん断基準点設定
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
    }
}

/********************************************************************/
void ObjectShear(int flg, int dr1, int dr2, double inc, PNTTYPE bas)	//<<< 立体せん断
/********************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Shear(dr1, dr2, inc, bas);										// せん断
	} else {															// <2回目以降>
		RedoShear(dr1, dr2, inc, bas);									// せん断再実行
	}
}

/************************************************************/
void Ring(PNTTYPE cp, double sx, double sy, double sz,
          int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)			//<<< ﾘﾝｸﾞｻｲｽﾞ合わせ
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(RING, 6, 3, &cmp);										// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(sx*1000.0));							// 各方向倍率設定
		PutParaInt(cmp, 1, (int)(sy*1000.0));
		PutParaInt(cmp, 2, (int)(sz*1000.0));
		PutParaInt(cmp, 3, fc);											// 曲げる水平方向設定
		PutParaInt(cmp, 4, vc);											// 曲げる垂直方向設定
		PutParaInt(cmp, 5, (int)(th*1000.0));							// 曲げる角度設定
		PutParaPnt(cmp, 0, cp);											// 変換中心座標設定
		PutParaPnt(cmp, 1, sp);											// 基準線始点設定
		PutParaPnt(cmp, 2, ep);											// 基準線終点設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(TRUE);												// ｺﾏﾝﾄﾞ実行
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/******************************************************/
void Earth(int rc, int sc, int sp, double dx,
           double dy, double dz, double tx, double ty,
           double tz, double sx, double sy, double sz)					//<<< ｱｰｽ
/******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {													// 生成時のﾎﾞｯｸｽｻｲｽﾞ取得へ
		RedoCreateCommand(op);											// 生成ｺﾏﾝﾄﾞ再実行
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	GetAreaBox(2, &ebx);												// ｴﾘｱﾎﾞｯｸｽ取得(処理対象立体)
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		GetObjBox(op, &obx);											// 立体ﾎﾞｯｸｽ取得
		GetUnitBox(op, &ubx);											// ﾕﾆｯﾄ立体ﾎﾞｯｸｽ取得
		box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);					// 立体/ﾕﾆｯﾄ/ｴﾘｱ
		BoxCenter(box, &rcp);											// BOX中心取得
		box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);					// 立体/ﾕﾆｯﾄ/ｴﾘｱ
		BoxCenter(box, &scp);											// BOX中心取得
		scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];					// Z方向位置設定
		scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
		CreatCmd(EARTH, 9, 2, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// 移動量設定
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		PutParaInt(cmp, 3, (int)(tx*1000.0));							// 回転角度設定
		PutParaInt(cmp, 4, (int)(ty*1000.0));
		PutParaInt(cmp, 5, (int)(tz*1000.0));
		PutParaInt(cmp, 6, (int)(sx*1000.0));							// 各方向倍率設定
		PutParaInt(cmp, 7, (int)(sy*1000.0));
		PutParaInt(cmp, 8, (int)(sz*1000.0));
		PutParaPnt(cmp, 0, rcp);										// 回転軸座標設定
		PutParaPnt(cmp, 1, scp);										// 変換中心座標設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(TRUE);												// ｺﾏﾝﾄﾞ実行
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*****************************************************************/
static void RedoEarth(int rc, int sc, int sp, int ch, double dx,
                      double dy, double dz, double tx, double ty,
                      double tz, double sx, double sy, double sz)		//<<< ｱｰｽ(再実行)
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	if(ch) {															// <中心点変更有の場合>
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
		while(op!=NULL) {												// 生成時のﾎﾞｯｸｽｻｲｽﾞ取得へ
			RedoCreateCommand(op);										// 生成ｺﾏﾝﾄﾞ再実行
			NextObjPtr(2, &op, &dsp, &edt);								// 次立体へ
		}
		GetAreaBox(2, &ebx);											// ｴﾘｱﾎﾞｯｸｽ取得(処理対象立体)
		LevelBaseCmdPtr(&cmp);											// 処理対象ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==EARTH) {									// <ｱｰｽｺﾏﾝﾄﾞの場合>
				GetCmdObj(cmp, &op);									// 編集対象立体取得
				GetObjBox(op, &obx);									// 立体ﾎﾞｯｸｽ取得
				GetUnitBox(op, &ubx);									// ﾕﾆｯﾄ立体ﾎﾞｯｸｽ取得
				box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);			// 立体/ﾕﾆｯﾄ/ｴﾘｱ
				BoxCenter(box, &rcp);									// BOX中心取得
				box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);			// 立体/ﾕﾆｯﾄ/ｴﾘｱ
				BoxCenter(box, &scp);									// BOX中心取得
				scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];			// Z方向位置設定
				scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// 移動量設定
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// 回転角度設定
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// 各方向倍率設定
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				PutParaPnt(cmp, 0, rcp);								// 回転軸座標設定
				PutParaPnt(cmp, 1, scp);								// 変換中心座標設定
			}
			LevelNextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
		LevelBaseCmdPtr(&cmp);											// 処理対象ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==EARTH) {									// <ｱｰｽｺﾏﾝﾄﾞの場合>
				GetCmdObj(cmp, &op);									// 編集対象立体取得
				RedoCommand(op);										// ｺﾏﾝﾄﾞ再実行
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			}
			LevelNextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
	} else {															// <中心点変更無の場合>
		LevelBaseCmdPtr(&cmp);											// 処理対象ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==EARTH) {									// <ｱｰｽｺﾏﾝﾄﾞの場合>
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// 移動量設定
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// 回転角度設定
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// 各方向倍率設定
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				GetCmdObj(cmp, &op);									// 編集対象立体取得
				RedoCommand(op);										// ｺﾏﾝﾄﾞ再実行
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			}
			LevelNextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
	}
}

/********************************************************************/
void ObjectEarth(int flg, int rc, int sc, int sp, int ch, double dx,
                 double dy, double dz, double tx, double ty,
                 double tz, double sx, double sy, double sz)			//<<< 立体ｱｰｽ
/********************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Earth(rc, sc, sp, dx, dy, dz, tx, ty, tz, sx, sy, sz);			// ｱｰｽ
	} else {															// <2回目以降>
		RedoEarth(rc, sc, sp, ch, dx, dy, dz, tx, ty, tz, sx, sy, sz);	// ｱｰｽ再実行
	}
}

/******************************************************/
void Space(int rc, int sc, int sp, double dx,
           double dy, double dz, double tx, double ty,
           double tz, double sx, double sy, double sz)					//<<< ｽﾍﾟｰｽ
/******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	GetAreaBox(2, &ebx);												// ｴﾘｱﾎﾞｯｸｽ取得(処理対象立体)
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		GetObjBox(op, &obx);											// 立体ﾎﾞｯｸｽ取得
		GetUnitBox(op, &ubx);											// ﾕﾆｯﾄ立体ﾎﾞｯｸｽ取得
		box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);					// 立体/ﾕﾆｯﾄ/ｴﾘｱ
		BoxCenter(box, &rcp);											// BOX中心取得
		box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);					// 立体/ﾕﾆｯﾄ/ｴﾘｱ
		BoxCenter(box, &scp);											// BOX中心取得
		scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];					// Z方向位置設定
		scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
		CreatCmd(SPACE, 9, 2, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// 移動量設定
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		PutParaInt(cmp, 3, (int)(tx*1000.0));							// 回転角度設定
		PutParaInt(cmp, 4, (int)(ty*1000.0));
		PutParaInt(cmp, 5, (int)(tz*1000.0));
		PutParaInt(cmp, 6, (int)(sx*1000.0));							// 各方向倍率設定
		PutParaInt(cmp, 7, (int)(sy*1000.0));
		PutParaInt(cmp, 8, (int)(sz*1000.0));
		PutParaPnt(cmp, 0, rcp);										// 回転軸座標設定
		PutParaPnt(cmp, 1, scp);										// 変換中心座標設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(TRUE);												// ｺﾏﾝﾄﾞ実行
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*****************************************************************/
static void RedoSpace(int rc, int sc, int sp, int ch, double dx,
                      double dy, double dz, double tx, double ty,
                      double tz, double sx, double sy, double sz)		//<<< ｽﾍﾟｰｽ(再実行)
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	if(ch) {															// <中心点変更有の場合>
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
		while(op!=NULL) {												// 生成時のﾎﾞｯｸｽｻｲｽﾞ取得へ
			RedoStartCommand(op);										// ﾒﾆｭｰ開始時ｺﾏﾝﾄﾞ再実行
			NextObjPtr(2, &op, &dsp, &edt);								// 次立体へ
		}
		GetAreaBox(2, &ebx);											// ｴﾘｱﾎﾞｯｸｽ取得(処理対象立体)
		LevelBaseCmdPtr(&cmp);											// 処理対象ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==SPACE) {									// <ｽﾍﾟｰｽｺﾏﾝﾄﾞの場合>
				GetCmdObj(cmp, &op);									// 編集対象立体取得
				GetObjBox(op, &obx);									// 立体ﾎﾞｯｸｽ取得
				GetUnitBox(op, &ubx);									// ﾕﾆｯﾄ立体ﾎﾞｯｸｽ取得
				box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);			// 立体/ﾕﾆｯﾄ/ｴﾘｱ
				BoxCenter(box, &rcp);									// BOX中心取得
				box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);			// 立体/ﾕﾆｯﾄ/ｴﾘｱ
				BoxCenter(box, &scp);									// BOX中心取得
				scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];			// Z方向位置設定
				scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// 移動量設定
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// 回転角度設定
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// 各方向倍率設定
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				PutParaPnt(cmp, 0, rcp);								// 回転軸座標設定
				PutParaPnt(cmp, 1, scp);								// 変換中心座標設定
			}
			LevelNextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
		LevelBaseCmdPtr(&cmp);											// 処理対象ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==SPACE) {									// <ｽﾍﾟｰｽｺﾏﾝﾄﾞの場合>
				GetCmdObj(cmp, &op);									// 編集対象立体取得
				RedoCommand(op);										// ｺﾏﾝﾄﾞ再実行
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			}
			LevelNextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
	} else {															// <中心点変更無の場合>
		LevelBaseCmdPtr(&cmp);											// 処理対象ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==SPACE) {									// <ｽﾍﾟｰｽｺﾏﾝﾄﾞの場合>
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// 移動量設定
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// 回転角度設定
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// 各方向倍率設定
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				GetCmdObj(cmp, &op);									// 編集対象立体取得
				RedoCommand(op);										// ｺﾏﾝﾄﾞ再実行
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			}
			LevelNextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
	}
}

/********************************************************************/
void ObjectSpace(int flg, int rc, int sc, int sp, int ch, double dx,
                 double dy, double dz, double tx, double ty,
                 double tz, double sx, double sy, double sz)			//<<< 立体ｽﾍﾟｰｽ
/********************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Space(rc, sc, sp, dx, dy, dz, tx, ty, tz, sx, sy, sz);			// ｽﾍﾟｰｽ
	} else {															// <2回目以降>
		RedoSpace(rc, sc, sp, ch, dx, dy, dz, tx, ty, tz, sx, sy, sz);	// ｽﾍﾟｰｽ再実行
	}
}

/***************************************************************/
void Resize3(int pln, int mpf, double s1, double s2, double s3,
             int vflg, double v1, double v2,
             PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)						//<<< 直線指定ﾘｻｲｽﾞ2
/***************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(RESIZE3, 8, 3, &cmp);									// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, pln);										// 指定平面設定
		PutParaInt(cmp, 1, mpf);										// 正負ﾌﾗｸﾞ設定
		PutParaInt(cmp, 2, (int)(s1*1000.0));							// 倍率1設定
		PutParaInt(cmp, 3, (int)(s2*1000.0));							// 倍率2設定
		PutParaInt(cmp, 4, (int)(s3*1000.0));							// 倍率3設定
		PutParaInt(cmp, 5, vflg);										// 垂直限定ﾌﾗｸﾞ設定
		PutParaInt(cmp, 6, (int)(v1*1000.0));							// 垂直範囲1設定
		PutParaInt(cmp, 7, (int)(v2*1000.0));							// 垂直範囲2設定
		PutParaPnt(cmp, 0, p1);											// 基準点1設定
		PutParaPnt(cmp, 1, p2);											// 基準点2設定
		PutParaPnt(cmp, 2, p3);											// 基準点3設定
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 直線指定ﾘｻｲｽﾞｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/******************************************************************/
static void RedoResize3(int pln, int mpf, double s1, double s2,
                        double s3, int vflg, double v1, double v2,
                        PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)				//<<< 直線指定ﾘｻｲｽﾞ2(再実行)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE3) {									// <直線指定ﾘｻｲｽﾞ2の場合>
			PutParaInt(cmp, 0, pln);									// 指定平面設定
			PutParaInt(cmp, 1, mpf);									// 正負ﾌﾗｸﾞ設定
			PutParaInt(cmp, 2, (int)(s1*1000.0));						// 倍率1設定
			PutParaInt(cmp, 3, (int)(s2*1000.0));						// 倍率2設定
			PutParaInt(cmp, 4, (int)(s3*1000.0));						// 倍率3設定
			PutParaInt(cmp, 5, vflg);									// 垂直限定ﾌﾗｸﾞ設定
			PutParaInt(cmp, 6, (int)(v1*1000.0));						// 垂直範囲1設定
			PutParaInt(cmp, 7, (int)(v2*1000.0));						// 垂直範囲2設定
			PutParaPnt(cmp, 0, p1);										// 基準点1設定
			PutParaPnt(cmp, 1, p2);										// 基準点2設定
			PutParaPnt(cmp, 2, p3);										// 基準点3設定
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/*****************************************************************/
void ObjectResize3(int flg, int pln, int mpf, double s1,
                   double s2, double s3, int vflg, double v1,
                   double v2, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)		//<<< 立体直線指定ﾘｻｲｽﾞ2
/*****************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Resize3(pln, mpf, s1, s2, s3, vflg, v1, v2, p1, p2, p3);		// 直線指定ﾘｻｲｽﾞ2
	} else {															// <2回目以降>
		RedoResize3(pln, mpf, s1, s2, s3, vflg, v1, v2, p1, p2, p3);	// 直線指定ﾘｻｲｽﾞ2再実行
	}
}

/*******************************************************/
void Resize4(int f1, int f2, int d1, int d2, int d3,
             double ln, int vflg, double v1, double v2,
             PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)					//<<< 曲線指定ﾘｻｲｽﾞ2
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		CreatCmd(RESIZE4, 10+pn, 1+pn, &cmp);							// ｺﾏﾝﾄﾞ領域確保
		PutParaInt(cmp, 0, f1);											// 正負ﾌﾗｸﾞ設定
		PutParaInt(cmp, 1, f2);											// 両側ﾌﾗｸﾞ設定
		PutParaInt(cmp, 2, d1);											// 長さ方向設定
		PutParaInt(cmp, 3, d2);											// ｻｲｽﾞ指定方向設定
		PutParaInt(cmp, 4, d3);											// 垂直方向設定
		PutParaInt(cmp, 5, (int)(ln*1000.0));							// 基準長さ設定
		PutParaInt(cmp, 6, vflg);										// 垂直限定ﾌﾗｸﾞ設定
		PutParaInt(cmp, 7, (int)(v1*1000.0));							// 垂直範囲1設定
		PutParaInt(cmp, 8, (int)(v2*1000.0));							// 垂直範囲2設定
		PutParaInt(cmp, 9, pn);											// 制御点数設定
		for(i=0; i<pn; i++) {
			PutParaInt(cmp, i+10, ia[i]);								// 曲線ﾓｰﾄﾞ設定
		}
		PutParaPnt(cmp, 0, bs);											// 基準点設定
		for(i=0; i<pn; i++) {
			PutParaPnt(cmp, i+1, pa[i]);								// 制御点設定
		}
		SetObjPtr(op);													// 対象立体設定
		SetCmdPtr(cmp);													// ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// 曲線指定ﾘｻｲｽﾞｺﾏﾝﾄﾞ実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/******************************************************************/
static void RedoResize4(int f1, int f2, int d1, int d2, int d3,
                        double ln, int vflg, double v1, double v2,
                        PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)		//<<< 曲線指定ﾘｻｲｽﾞ2(再実行)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE4) {									// <<曲線指定ﾘｻｲｽﾞの場合>>
			spn = GetParaInt(cmp, 9);									// 制御点数取得
			if(pn!=spn) {												// <制御点数が異なる場合>
				ChangeCmd(cmp, 10+pn, 1+pn);								// ｺﾏﾝﾄﾞ領域変更
			}
			PutParaInt(cmp, 0, f1);										// 正負ﾌﾗｸﾞ設定
			PutParaInt(cmp, 1, f2);										// 両側ﾌﾗｸﾞ設定
			PutParaInt(cmp, 2, d1);										// 長さ方向設定
			PutParaInt(cmp, 3, d2);										// ｻｲｽﾞ指定方向設定
			PutParaInt(cmp, 4, d3);										// 垂直方向設定
			PutParaInt(cmp, 5, (int)(ln*1000.0));						// 基準長さ設定
			PutParaInt(cmp, 6, vflg);									// 垂直限定ﾌﾗｸﾞ設定
			PutParaInt(cmp, 7, (int)(v1*1000.0));						// 垂直範囲1設定
			PutParaInt(cmp, 8, (int)(v2*1000.0));						// 垂直範囲2設定
			PutParaInt(cmp, 9, pn);										// 制御点数設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, i+10, ia[i]);							// 曲線ﾓｰﾄﾞ設定
			}
			PutParaPnt(cmp, 0, bs);										// 基準点設定
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i+1, pa[i]);							// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(pn!=spn) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/*******************************************************************/
void ObjectResize4(int flg, int f1, int f2, int d1, int d2, int d3,
                   double ln, int vflg, double v1, double v2,
                   PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)			//<<< 立体曲線指定ﾘｻｲｽﾞ2
/*******************************************************************/
{
	if(flg==0) {														// <1回目>
		InitLevel();													// Undo/Redo初期化
		Resize4(f1, f2, d1, d2, d3, ln, vflg, v1, v2, bs, pn, pa, ia);	// 曲線指定ﾘｻｲｽﾞ2
	} else {															// <2回目以降>
		RedoResize4(f1, f2, d1, d2, d3, ln,
                    vflg, v1, v2, bs, pn, pa, ia);						// 曲線指定ﾘｻｲｽﾞ2再実行
	}
}
