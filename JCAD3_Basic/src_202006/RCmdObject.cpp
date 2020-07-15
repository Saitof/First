/////////////////////////////////////////////////////////////////////////////
// コマンド関連 立体複写/削除部
// RCmdObject.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "NumFunc.h"

/*************************/
OBJTYPE* CopyObject(void)												//<<< 立体&ｺﾏﾝﾄﾞ複写
/*************************/
{
	OBJTYPE* op, * optr;
	OIFTYPE* oif;
	CMDTYPE* cmp, * cptr;
	PNTTYPE  pd;
	int      i, num1, num2, num3, id1, id2, cno, gno[32];
	char     name[24];

	GetObjPtr(&op);														// 処理対象立体取得
	if(!CheckObjPtr(0, op)) {											// 立体ﾁｪｯｸ
		return NULL;
	}
	num1 = GetVtxNum(op);												// 頂点数取得
	num2 = GetFlpNum1(op);												// 面ﾙｰﾌﾟ数取得
	num3 = GetFlpNum2(op);
	CreatObj(num1, num2, num3, &optr);									// 立体領域確保
	LevelObj(0, optr);													// 同一ﾚﾍﾞﾙ生成立体接続
	for(i=0; i<num1; ++i) {
		GetVtx(op, i, &pd);												// 頂点取得
		PutVtx(optr, i, pd);											// 頂点設定
	}
	for(i=0; i<num2; ++i) {												// 面ﾙｰﾌﾟ1
		GetFlp1(op, i, &id1, &id2);										// 面ﾙｰﾌﾟ情報取得
		PutFlp1(optr, i, id1, id2);										// 面ﾙｰﾌﾟ情報設定
	}
	for(i=0; i<num3; ++i) {												// 面ﾙｰﾌﾟ2
		GetFlp2(op, i, &id1, &id2);										// 面ﾙｰﾌﾟ情報取得
		PutFlp2(optr, i, id1, id2);										// 面ﾙｰﾌﾟ情報設定
	}
	PutObjAtr(optr, GetObjAtr(op));										// 属性No設定
	PutObjPrt(optr, GetObjPrt(op), 0);									// ﾊﾟｰﾂNo設定
	PutObjCol(optr, GetObjCol(op));										// ｶﾗｰNo設定
	GetGroup(op, gno);													// ｸﾞﾙｰﾌﾟ情報取得
	PutGroup(optr, gno);												// ｸﾞﾙｰﾌﾟ情報設定
	SetObjBox(optr);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	GetObjOif(op, &oif);												// 立体構成情報ﾎﾟｲﾝﾀ取得
	GetOifName(oif, name);												// 立体構成名取得
	GetObjOif(optr, &oif);												// 立体構成情報ﾎﾟｲﾝﾀ取得
	SetOifName(0, oif, name);											// 立体構成名設定
	BaseCmdPtr(1, op, &cmp);
	while(cmp!=NULL) {
		cno = GetCmdNo(cmp);											// ｺﾏﾝﾄﾞNo取得
		if(cno==CHGSURF) {												// ｺﾏﾝﾄﾞNo変更
			cno = CRTSURF;
		}
		if(cno==CHGCURVE) {
			cno = CRTCURVE;
		}
		if(cno==CHGSWEEP) {
			cno = CRTSWEEP;
		}
		num1 = GetIntNum(cmp);											// 整数ﾊﾟﾗﾒｰﾀ数取得
		num2 = GetPntNum(cmp);											// 座標ﾊﾟﾗﾒｰﾀ数取得
		CreatCmd(cno, num1, num2, &cptr);								// ｺﾏﾝﾄﾞ領域確保
		for(i=0; i<num1; ++i) {
			PutParaInt(cptr, i, GetParaInt(cmp, i));					// 整数ﾊﾟﾗﾒｰﾀ設定
		}
		for(i=0; i<num2; ++i) {
			GetParaPnt(cmp, i, &pd);									// 座標ﾊﾟﾗﾒｰﾀ取得
			PutParaPnt(cptr, i, pd);									// 座標ﾊﾟﾗﾒｰﾀ設定
		}
		AppendCmd(optr, cptr);											// 立体&ｺﾏﾝﾄﾞ接続
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	return optr;
}

/***************/
void Copy(void)															//<<< 立体複写
/***************/
{
	OBJTYPE* op, * wop;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		SetObjPtr(op);													// 複写対象立体設定
		wop = CopyObject();												// 立体複写
		SetObjPtr(wop);													// 立体ﾎﾟｲﾝﾀ設定
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
}

/*****************/
void Delete(void)														//<<< 立体削除
/*****************/
{
	OBJTYPE* op, * dop;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		dop = op;														// 削除立体ﾎﾟｲﾝﾀ保存
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
		LevelObj(1, dop);												// 同一ﾚﾍﾞﾙ削除立体接続
		TempObj(dop);													// 立体一時削除
	}
}

/*******************************/
void InitPasteObject(int* clvl)											//<<< 立体ﾍﾟｰｽﾄ初期処理
/*******************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	CPCTYPE* cpc;
	PNTTYPE  pp, zer, vec;
	BOXTYPE  box;
	int      i, cno, ino, pno, pi, lvl;

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;						// 作業領域初期化
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = -1.0;
	InitLevel();														// Undo/Redo初期化
	BaseCopyCmdPtr(&cpc);												// 先頭ｺﾋﾟｰｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	while(cpc!=NULL) {
		cno = GetCopyCmdNo(cpc);										// ｺﾋﾟｰｺﾏﾝﾄﾞNo取得
		ino = GetCopyIntNum(cpc);										// ｺﾋﾟｰｺﾏﾝﾄﾞ整数ﾊﾟﾗ数取得
		pno = GetCopyPntNum(cpc);										// ｺﾋﾟｰｺﾏﾝﾄﾞ座標ﾊﾟﾗ数取得
		CreatCmd(cno, ino, pno, &cmp);									// 通常ｺﾏﾝﾄﾞ領域確保
		for(i=0; i<ino; i++) {
			GetCopyParaInt(cpc, i, &pi);								// ｺﾋﾟｰ整数ﾊﾟﾗﾒｰﾀ取得
			PutParaInt(cmp, i, pi);										// 通常整数ﾊﾟﾗﾒｰﾀ設定
		}
		for(i=0; i<pno; i++) {
			GetCopyParaPnt(cpc, i, &pp);								// ｺﾋﾟｰ座標ﾊﾟﾗﾒｰﾀ取得
			PutParaPnt(cmp, i, pp);										// 通常座標ﾊﾟﾗﾒｰﾀ設定
		}
		SetCmdPtr(cmp);													// 処理対象ｺﾏﾝﾄﾞ設定
		ExecCommand(FALSE);												// ｺﾏﾝﾄﾞ実行
		if(cno==ENDCMD) {												// <最終ｺﾏﾝﾄﾞの場合>
			GetObjPtr(&op);												// 立体のﾎﾟｲﾝﾀ取得
			GetObjOif(op, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			SetOifName(0, oif, OBJ_NAME34);								// 立体構成名設定
			if(op->part==2&&op->oset!=NULL) {							// <ﾕﾆｯﾄ･子の場合>
				GetObjBox(op->oset, &box);								// 立体BOX取得
			} else {													// <ﾕﾆｯﾄ･子以外の場合>
				GetObjBox(op, &box);									// 立体BOX取得
			}
			BoxCenter(box, &pp);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			BaseCmdPtr(0, op, &cmp);									// 生成ｺﾏﾝﾄﾞ取得
			lvl = GetCmdLvl(cmp);										// 現在ｺﾏﾝﾄﾞﾚﾍﾞﾙNo保存
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保[立体Z軸回転用]
			PutParaInt(cmp, 0, 0);										// 回転角度設定
			PutParaPnt(cmp, 0, pp);										// 回転軸座標設定
			PutParaPnt(cmp, 1, vec);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetCmdLvl(cmp, lvl+1);										// 仮ｺﾏﾝﾄﾞﾚﾍﾞﾙNo変更(+1)
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
			CreatCmd(MOVE, 3, 0, &cmp);									// ｺﾏﾝﾄﾞ領域確保[移動調整用]
			PutParaInt(cmp, 0, 0);										// 移動量設定
			PutParaInt(cmp, 1, 0);
			PutParaInt(cmp, 2, 0);
			SetCmdLvl(cmp, lvl+2);										// 仮ｺﾏﾝﾄﾞﾚﾍﾞﾙNo変更(+2)
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 移動ｺﾏﾝﾄﾞ実行
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保[回転調整用]
			PutParaInt(cmp, 0, 0);										// 回転角度設定
			PutParaPnt(cmp, 0, zer);									// 回転軸座標設定
			PutParaPnt(cmp, 1, vec);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetCmdLvl(cmp, lvl+3);										// 仮ｺﾏﾝﾄﾞﾚﾍﾞﾙNo変更(+3)
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
			OnEdtFlg(1, op);											// 立体edtflg:ON
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		NextCopyCmdPtr(&cpc);											// 次ｺﾋﾟｰｺﾏﾝﾄﾞへ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	MainWnd->SetGroupObj();												// ｸﾞﾙｰﾌﾟ立体設定
	*clvl = lvl;
}

/*****************************/
void EndPasteObject(int clvl)											//<<< 立体ﾍﾟｰｽﾄ終了処理
/*****************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp, * dcmp;
	int      del, dx, dy, dz, th;

	LevelBaseObjPtr(0, &op);											// 生成立体検索
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// 先頭ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			del = 0;
			if(GetCmdLvl(cmp)==clvl+1) {								// <立体Z軸回転用ｺﾏﾝﾄﾞ>
				th = GetParaInt(cmp, 0);								// 回転角度取得
				if(th==0) {
					del = 1;
				}
				SetCmdLvl(cmp, clvl);
			} else if(GetCmdLvl(cmp)==clvl+2) {							// <移動調整用ｺﾏﾝﾄﾞ>
				dx = GetParaInt(cmp, 0);								// 移動量取得
				dy = GetParaInt(cmp, 1);
				dz = GetParaInt(cmp, 2);
				if(dx==0&&dy==0&&dz==0) {
					del = 1;
				}
				SetCmdLvl(cmp, clvl);
			} else if(GetCmdLvl(cmp)==clvl+3) {							// <回転調整用ｺﾏﾝﾄﾞ>
				th = GetParaInt(cmp, 0);								// 回転角度取得
				if(th==0) {
					del = 1;
				}
				SetCmdLvl(cmp, clvl);
			}
			if(del) {
				dcmp = cmp;
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
			if(del) {
				FreeCmd(op, dcmp, 1);									// 無効ｺﾏﾝﾄﾞ削除
			}
		}
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*******************************************************/
void EditPasteObject1(int clvl, PNTTYPE mov, double th,
                      PNTTYPE cpt, PNTTYPE vec)							//<<< 立体ﾍﾟｰｽﾄ配置(移動･回転)
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// 生成立体の検索
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// 先頭ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdLvl(cmp)==clvl+2) {								// <移動調整用ｺﾏﾝﾄﾞ>
				PutParaInt(cmp, 0, (int)(mov.p[0]*1000.0));				// 移動量設定
				PutParaInt(cmp, 1, (int)(mov.p[1]*1000.0));
				PutParaInt(cmp, 2, (int)(mov.p[2]*1000.0));
			} else if(GetCmdLvl(cmp)==clvl+3) {							// <回転調整用ｺﾏﾝﾄﾞ>
				PutParaInt(cmp, 0, (int)(th*1000.0));					// 回転角度設定
				PutParaPnt(cmp, 0, cpt);								// 回転軸座標設定
				PutParaPnt(cmp, 1, vec);								// 回転軸方向ﾍﾞｸﾄﾙ設定
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextObjPtr(0, &op);										// 次生成立体検索
    }
}

/******************************************/
void EditPasteObject2(int clvl, double th)								//<<< 立体ﾍﾟｰｽZ軸回転
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// 生成立体の検索
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// 先頭ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			if(GetCmdLvl(cmp)==clvl+1) {								// <Z軸回転用ｺﾏﾝﾄﾞ>
				PutParaInt(cmp, 0, (int)(th*1000.0));					// 回転角度設定
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextObjPtr(0, &op);										// 次生成立体検索
    }
}
