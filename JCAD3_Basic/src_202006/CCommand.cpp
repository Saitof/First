/////////////////////////////////////////////////////////////////////////////
// 立体生成コマンド部
// CCommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"

/**********************************************************************/
void CrtCurve(int pn, int bc, int mx, double wt, PNTTYPE* pa, int* ea)	//<<< 曲線生成
/**********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	CreatCmd(CRTCURVE, 4+pn, pn, &cmp);									// ｺﾏﾝﾄﾞ領域確保
	PutParaInt(cmp, 0, pn);												// 制御点数設定
	PutParaInt(cmp, 1, bc);												// 境界ﾓｰﾄﾞ設定
	PutParaInt(cmp, 2, mx);												// 曲線化繰返数
	PutParaInt(cmp, 3, (int)(wt*1000.0));								// 曲線重み係数
	for(i=0; i<pn; i++) {
		PutParaInt(cmp, 4+i, ea[i]);									// ﾗｲﾝﾓｰﾄﾞ設定
		PutParaPnt(cmp, i, pa[i]);										// 制御点設定
	}
	SetCmdPtr(cmp);														// ｺﾏﾝﾄﾞ設定
	ExecCommand(FALSE);													// ｺﾏﾝﾄﾞ実行
	GetObjPtr(&op);														// 対象立体取得
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*******************************************************/
void RedoCrtCurve(int pn, int bc, PNTTYPE* pa, int* ea)					//<<< 曲線生成
/*******************************************************/				//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn, sbc;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTCURVE) {									// <曲線生成>
			ChangeCmd(cmp, 4+pn, pn);									// ｺﾏﾝﾄﾞ領域変更
			spn = GetParaInt(cmp, 0);									// 制御点数取得
			PutParaInt(cmp, 0, pn);										// 制御点数設定
			sbc = GetParaInt(cmp, 1);									// 境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 1, bc);										// 境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 4+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(pn!=spn||bc!=sbc) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/*************************************************************/
void ChgCurve(OBJTYPE* so, int pn, int bc, int mx, double wt,
              PNTTYPE* pa, int* ea)										//<<< 曲線変更
/*************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, i;

	if(so==NULL) {														// <立体指定なし>
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
	} else {															// <立体指定あり>
		op = so;														// 指定立体
	}
	if(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// 先頭ｺﾏﾝﾄﾞ取得
		if(GetCmdNo(cmp)==CRTCURVE) {									// <曲線生成>
			CreatCmd(CHGCURVE, 4+pn, pn, &cmp);							// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, pn);										// 制御点数設定
			PutParaInt(cmp, 1, bc);										// 境界ﾓｰﾄﾞ設定
			PutParaInt(cmp, 2, mx);										// 曲線化繰返数
			PutParaInt(cmp, 3, (int)(wt*1000.0));						// 曲線重み係数
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 4+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			SetObjPtr(op);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(TRUE);											// ｺﾏﾝﾄﾞ実行
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*******************************************************/
void RedoChgCurve(int pn, int bc, PNTTYPE* pa, int* ea)					//<<< 曲線変更
/*******************************************************/				//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn, sbc;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CHGCURVE) {									// <曲線変更>
			ChangeCmd(cmp, 4+pn, pn);									// ｺﾏﾝﾄﾞ領域変更
			spn = GetParaInt(cmp, 0);									// 制御点数取得
			PutParaInt(cmp, 0, pn);										// 制御点数設定
			sbc = GetParaInt(cmp, 1);									// 境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 1, bc);										// 境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 4+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(pn!=spn||bc!=sbc) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/******************************************/
void SetCurve(int pn, int bc, PNTTYPE* pa)								//<<< 曲線制御点生成
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	CreatCmd(SETCURVE, 2, pn, &cmp);									// ｺﾏﾝﾄﾞ領域確保
	PutParaInt(cmp, 0, pn);												// 制御点数設定
	PutParaInt(cmp, 1, bc);												// 境界ﾓｰﾄﾞ設定
	for(i=0; i<pn; i++) {
		PutParaPnt(cmp, i, pa[i]);										// 制御点設定
	}
	SetCmdPtr(cmp);														// ｺﾏﾝﾄﾞ設定
	ExecCommand(FALSE);													// ｺﾏﾝﾄﾞ実行
	GetObjPtr(&op);														// 対象立体取得
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/**********************************************/
void RedoSetCurve(int pn, int bc, PNTTYPE* pa)							//<<< 曲線制御点生成
/**********************************************/						//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn, sbc;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SETCURVE) {									// <曲線制御点生成>
			ChangeCmd(cmp, 2, pn);										// ｺﾏﾝﾄﾞ領域変更
			spn = GetParaInt(cmp, 0);									// 制御点数取得
			PutParaInt(cmp, 0, pn);										// 制御点数設定
			sbc = GetParaInt(cmp, 1);									// 境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 1, bc);										// 境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(pn!=spn||bc!=sbc) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/********************************************************/
void CrtSurface(int un, int vn, int ub, int vb, int mx,
                double wt, int fg, PNTTYPE* pa, int* ea)				//<<< 曲面生成
/********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn;

	pn = un*vn;															// 総頂点数
	CreatCmd(CRTSURF, 7+pn, pn, &cmp);									// ｺﾏﾝﾄﾞ領域確保
	PutParaInt(cmp, 0, un);												// U制御点数設定
	PutParaInt(cmp, 1, vn);												// V制御点数設定
	PutParaInt(cmp, 2, ub);												// U境界ﾓｰﾄﾞ設定
	PutParaInt(cmp, 3, vb);												// V境界ﾓｰﾄﾞ設定
	PutParaInt(cmp, 4, mx);												// 曲線化繰返数
	PutParaInt(cmp, 5, (int)(wt*1000.0));								// 曲線重み係数
	PutParaInt(cmp, 6, fg);												// ｿﾘｯﾄﾞﾓｰﾄﾞ
	for(i=0; i<pn; i++) {
		PutParaInt(cmp, 7+i, ea[i]);									// ﾗｲﾝﾓｰﾄﾞ設定
		PutParaPnt(cmp, i, pa[i]);										// 制御点設定
	}
	SetCmdPtr(cmp);														// ｺﾏﾝﾄﾞ設定
	ExecCommand(FALSE);													// ｺﾏﾝﾄﾞ実行
	GetObjPtr(&op);														// 対象立体取得
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***************************************************/
void RedoCrtSurface(int un, int vn, int ub, int vb,
                    PNTTYPE* pa, int* ea)								//<<< 曲面生成
/***************************************************/					//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTSURF) {									// <曲面生成>
			pn = un*vn;													// 総頂点数
			ChangeCmd(cmp, 7+pn, pn);									// ｺﾏﾝﾄﾞ領域変更
			sun = GetParaInt(cmp, 0);									// U制御点数取得
			PutParaInt(cmp, 0, un);										// U制御点数設定
			svn = GetParaInt(cmp, 1);									// V制御点数取得
			PutParaInt(cmp, 1, vn);										// V制御点数設定
			sub = GetParaInt(cmp, 2);									// U境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 2, ub);										// U境界ﾓｰﾄﾞ設定
			svb = GetParaInt(cmp, 3);									// V境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 3, vb);										// V境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 7+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/************************************************************/
void RedoCrtSurface2(int no, int un, int vn, int ub, int vb,
                     PNTTYPE* pa, int* ea)								//<<< 曲面生成2
/************************************************************/			//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb, cnt=0;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTSURF) {									// <曲面生成>
			if(no==cnt) {
				pn = un*vn;												// 総頂点数
				ChangeCmd(cmp, 7+pn, pn);								// ｺﾏﾝﾄﾞ領域変更
				sun = GetParaInt(cmp, 0);								// U制御点数取得
				PutParaInt(cmp, 0, un);									// U制御点数設定
				svn = GetParaInt(cmp, 1);								// V制御点数取得
				PutParaInt(cmp, 1, vn);									// V制御点数設定
				sub = GetParaInt(cmp, 2);								// U境界ﾓｰﾄﾞ取得
				PutParaInt(cmp, 2, ub);									// U境界ﾓｰﾄﾞ設定
				svb = GetParaInt(cmp, 3);								// V境界ﾓｰﾄﾞ取得
				PutParaInt(cmp, 3, vb);									// V境界ﾓｰﾄﾞ設定
				for(i=0; i<pn; i++) {
					PutParaInt(cmp, 7+i, ea[i]);						// ﾗｲﾝﾓｰﾄﾞ設定
					PutParaPnt(cmp, i, pa[i]);							// 制御点設定
				}
				GetCmdObj(cmp, &op);									// 編集対象立体取得
				RedoCommand(op);										// ｺﾏﾝﾄﾞ再実行
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
				break;
			}
			cnt++;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/************************************************************/
void ChgSurface(OBJTYPE* so, int un, int vn, int ub, int vb,
                int mx, double wt, PNTTYPE* pa, int* ea)				//<<< 曲面変更
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, pn, sm, i;

	if(so==NULL) {														// <立体指定なし>
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
	} else {															// <立体指定あり>
		op = so;														// 指定立体
	}
	if(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// 先頭ｺﾏﾝﾄﾞ取得
		if(GetCmdNo(cmp)==CRTSURF) {									// <曲面生成>
			sm = GetParaInt(cmp, 6);									// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
			pn = un*vn;													// 総頂点数
			CreatCmd(CHGSURF, 7+pn, pn, &cmp);							// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, un);										// U制御点数設定
			PutParaInt(cmp, 1, vn);										// V制御点数設定
			PutParaInt(cmp, 2, ub);										// U境界ﾓｰﾄﾞ設定
			PutParaInt(cmp, 3, vb);										// V境界ﾓｰﾄﾞ設定
			PutParaInt(cmp, 4, mx);										// 曲線化繰返数
			PutParaInt(cmp, 5, (int)(wt*1000.0));						// 曲線重み係数
			PutParaInt(cmp, 6, sm);										// ｿﾘｯﾄﾞﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 7+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			SetObjPtr(op);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(TRUE);											// ｺﾏﾝﾄﾞ実行
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***************************************************/
void RedoChgSurface(int un, int vn, int ub, int vb,
                    PNTTYPE* pa, int* ea)								//<<< 曲面変更
/***************************************************/					//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CHGSURF) {									// <曲面変更>
			pn = un*vn;													// 総頂点数
			ChangeCmd(cmp, 7+pn, pn);									// ｺﾏﾝﾄﾞ領域変更
			sun = GetParaInt(cmp, 0);									// U制御点数取得
			PutParaInt(cmp, 0, un);										// U制御点数設定
			svn = GetParaInt(cmp, 1);									// V制御点数取得
			PutParaInt(cmp, 1, vn);										// V制御点数設定
			sub = GetParaInt(cmp, 2);									// U境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 2, ub);										// U境界ﾓｰﾄﾞ設定
			svb = GetParaInt(cmp, 3);									// V境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 3, vb);										// V境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 7+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/************************************************************/
void SetSurface(int un, int vn, int ub, int vb, PNTTYPE* pa)			//<<< 曲面制御点生成
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn;

	pn = un*vn;															// 総頂点数
	CreatCmd(SETSURF, 4, pn, &cmp);										// ｺﾏﾝﾄﾞ領域確保
	PutParaInt(cmp, 0, un);												// U制御点数設定
	PutParaInt(cmp, 1, vn);												// V制御点数設定
	PutParaInt(cmp, 2, ub);												// U境界ﾓｰﾄﾞ設定
	PutParaInt(cmp, 3, vb);												// V境界ﾓｰﾄﾞ設定
	for(i=0; i<pn; i++) {
		PutParaPnt(cmp, i, pa[i]);										// 制御点設定
	}
	SetCmdPtr(cmp);														// ｺﾏﾝﾄﾞ設定
	ExecCommand(FALSE);													// ｺﾏﾝﾄﾞ実行
	GetObjPtr(&op);														// 対象立体取得
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/****************************************************************/
void RedoSetSurface(int un, int vn, int ub, int vb, PNTTYPE* pa)		//<<< 曲面制御点生成
/****************************************************************/		//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SETSURF) {									// <曲面制御点生成>
			pn = un*vn;													// 総頂点数
			ChangeCmd(cmp, 4, pn);										// ｺﾏﾝﾄﾞ領域変更
			sun = GetParaInt(cmp, 0);									// U制御点数取得
			PutParaInt(cmp, 0, un);										// U制御点数設定
			svn = GetParaInt(cmp, 1);									// V制御点数取得
			PutParaInt(cmp, 1, vn);										// V制御点数設定
			sub = GetParaInt(cmp, 2);									// U境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 2, ub);										// U境界ﾓｰﾄﾞ設定
			svb = GetParaInt(cmp, 3);									// V境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 3, vb);										// V境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/***************************************/
void Thickness(OBJTYPE* op, double len)									//<<< 曲面厚みづけ
/***************************************/
{
	CMDTYPE* cmp;

	CreatCmd(THICKNESS, 1, 0, &cmp);									// ｺﾏﾝﾄﾞ領域確保
	PutParaInt(cmp, 0, (int)(len*1000.0));								// 厚み幅設定
	SetObjPtr(op);														// 対象立体設定
	SetCmdPtr(cmp);														// ｺﾏﾝﾄﾞ設定
	ExecCommand(FALSE);													// ｺﾏﾝﾄﾞ実行
	GetObjPtr(&op);														// 対象立体取得
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***********************************/
void MakeDirectCommand(OBJTYPE* op)										//<<< 直接生成ｺﾏﾝﾄﾞ作成
/***********************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      vnum, lnum, vno, flg, i;

	vnum = GetVtxNum(op);												// 頂点数取得
	lnum = GetFlpNum1(op);												// 面ﾙｰﾌﾟ数取得
	CreatCmd(DIRECT, lnum+2, vnum, &cmp);								// ｺﾏﾝﾄﾞ領域取得
	for(i=0; i<vnum; i++) {
		GetVtx(op, i, &pnt);											// 頂点取得
		PutParaPnt(cmp, i, pnt);										// 座標ﾊﾟﾗﾒｰﾀ設定
	}
	PutParaInt(cmp, 0, vnum);											// 頂点数設定
	PutParaInt(cmp, 1, lnum);											// 面ﾙｰﾌﾟ数設定
	for(i=0; i<lnum; i++) {
		GetFlp1(op, i, &vno, &flg);										// 面ﾙｰﾌﾟ情報取得
		if(flg) {
			PutParaInt(cmp, i+2, -(vno+1));								// 整数ﾊﾟﾗﾒｰﾀ設定
		} else {
			PutParaInt(cmp, i+2, vno+1);
		}
	}
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	SetObjPtr(op);														// 処理対象立体設定
	GetObjOif(op, &oif);												// 立体構成情報ﾎﾟｲﾝﾀ取得
	SetOifName(0, oif, OBJ_NAME33);										// 構成名(直接生成立体)
	OnCmdFlg();															// ｺﾏﾝﾄﾞ未保存ON
}

/************************************/
void RedoDirect(int pn, PNTTYPE* pa)									//<<< 直接生成
/************************************/									//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	GetObjPtr(&op);														// 立体ﾎﾟｲﾝﾀ取得
	BaseCmdPtr(0, op, &cmp);											// 先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	if(cmp!=NULL) {
		if(GetCmdNo(cmp)==DIRECT&&GetPntNum(cmp)==pn) {					// <直接ｺﾏﾝﾄﾞ&座標ﾊﾟﾗﾒｰﾀ=>
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i, pa[i]);								// 座標ﾊﾟﾗﾒｰﾀ更新
			}
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
	}
}

/******************************************************************/
void CrtSweepSurf(int un, int vn, int ub, int vb, int mx,
                  double wt, int sm, int pl, PNTTYPE* pa, int* ea)		//<<< 掃引曲面生成
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn;

	pn = un+vn;															// 総頂点数
	CreatCmd(CRTSWEEP, 8+pn, pn, &cmp);									// ｺﾏﾝﾄﾞ領域確保
	PutParaInt(cmp, 0, un);												// U制御点数設定
	PutParaInt(cmp, 1, vn);												// V制御点数設定
	PutParaInt(cmp, 2, ub);												// U境界ﾓｰﾄﾞ設定
	PutParaInt(cmp, 3, vb);												// V境界ﾓｰﾄﾞ設定
	PutParaInt(cmp, 4, mx);												// 曲線化繰返数
	PutParaInt(cmp, 5, (int)(wt*1000.0));								// 曲線重み係数
	PutParaInt(cmp, 6, sm);												// ｿﾘｯﾄﾞﾓｰﾄﾞ
	PutParaInt(cmp, 7, pl);												// U平面設定
	for(i=0; i<pn; i++) {
		PutParaInt(cmp, 8+i, ea[i]);									// ﾗｲﾝﾓｰﾄﾞ設定
		PutParaPnt(cmp, i, pa[i]);										// 制御点設定
	}
	SetCmdPtr(cmp);														// ｺﾏﾝﾄﾞ設定
	ExecCommand(FALSE);													// ｺﾏﾝﾄﾞ実行
	GetObjPtr(&op);														// 対象立体取得
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*****************************************************/
void RedoCrtSweepSurf(int un, int vn, int ub, int vb,
                      PNTTYPE* pa, int* ea)								//<<< 掃引曲面生成
/*****************************************************/					//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTSWEEP) {									// <掃引曲面生成>
			pn = un+vn;													// 総頂点数
			ChangeCmd(cmp, 8+pn, pn);									// ｺﾏﾝﾄﾞ領域変更
			sun = GetParaInt(cmp, 0);									// U制御点数取得
			PutParaInt(cmp, 0, un);										// U制御点数設定
			svn = GetParaInt(cmp, 1);									// V制御点数取得
			PutParaInt(cmp, 1, vn);										// V制御点数設定
			sub = GetParaInt(cmp, 2);									// U境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 2, ub);										// U境界ﾓｰﾄﾞ設定
			svb = GetParaInt(cmp, 3);									// V境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 3, vb);										// V境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 8+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}

/**************************************************************/
void ChgSweepSurf(OBJTYPE* so, int un, int vn, int ub, int vb,
                  int mx, double wt, PNTTYPE* pa, int* ea)				//<<< 掃引曲面変更
/**************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, pn, sm, pl, i;

	if(so==NULL) {														// <立体指定なし>
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
	} else {															// <立体指定あり>
		op = so;														// 指定立体
	}
	if(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// 先頭ｺﾏﾝﾄﾞ取得
		if(GetCmdNo(cmp)==CRTSWEEP) {									// <掃引曲面生成>
			sm = GetParaInt(cmp, 6);									// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
			pl = GetParaInt(cmp, 7);									// U平面取得
			pn = un+vn;													// 総頂点数
			CreatCmd(CHGSWEEP, 8+pn, pn, &cmp);							// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, un);										// U制御点数設定
			PutParaInt(cmp, 1, vn);										// V制御点数設定
			PutParaInt(cmp, 2, ub);										// U境界ﾓｰﾄﾞ設定
			PutParaInt(cmp, 3, vb);										// V境界ﾓｰﾄﾞ設定
			PutParaInt(cmp, 4, mx);										// 曲線化繰返数
			PutParaInt(cmp, 5, (int)(wt*1000.0));						// 曲線重み係数
			PutParaInt(cmp, 6, sm);										// ｿﾘｯﾄﾞﾓｰﾄﾞ設定
			PutParaInt(cmp, 7, pl);										// U平面設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 8+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			SetObjPtr(op);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(TRUE);											// ｺﾏﾝﾄﾞ実行
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*****************************************************/
void RedoChgSweepSurf(int un, int vn, int ub, int vb,
                      PNTTYPE* pa, int* ea)								//<<< 掃引曲面変更
/*****************************************************/					//    (再実行)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CHGSWEEP) {									// <掃引曲面変更>
			pn = un+vn;													// 総頂点数
			ChangeCmd(cmp, 8+pn, pn);									// ｺﾏﾝﾄﾞ領域変更
			sun = GetParaInt(cmp, 0);									// U制御点数取得
			PutParaInt(cmp, 0, un);										// U制御点数設定
			svn = GetParaInt(cmp, 1);									// V制御点数取得
			PutParaInt(cmp, 1, vn);										// V制御点数設定
			sub = GetParaInt(cmp, 2);									// U境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 2, ub);										// U境界ﾓｰﾄﾞ設定
			svb = GetParaInt(cmp, 3);									// V境界ﾓｰﾄﾞ取得
			PutParaInt(cmp, 3, vb);										// V境界ﾓｰﾄﾞ設定
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 8+i, ea[i]);							// ﾗｲﾝﾓｰﾄﾞ設定
				PutParaPnt(cmp, i, pa[i]);								// 制御点設定
			}
			GetCmdObj(cmp, &op);										// 編集対象立体取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// ﾃﾞｰﾀ使用率設定
	}
}
