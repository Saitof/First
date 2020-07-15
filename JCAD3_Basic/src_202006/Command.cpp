/////////////////////////////////////////////////////////////////////////////
// コマンド起動制御部
// Command.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"

typedef  struct  cmdst {
	OBJTYPE* (*comfunc)();
} CTBTYPE;																// ｺﾏﾝﾄﾞﾃｰﾌﾞﾙ

CTBTYPE Cmdtbl[] = {
	{ MakeSurface },													// 曲面生成
	{ MakeDirect },														// 直接生成
	{ MakeCurve },														// 曲線生成
	{ ChangeSurface },													// 曲面編集
	{ ChangeCurve },													// 曲線編集
	{ SetSurface },														// 曲面制御点生成
	{ SetCurve },														// 曲線制御点生成
	{ ThicknessObject },												// 曲面厚みづけ
	{ MoveObject },														// 平行移動
	{ RotateObject },													// 回転移動
	{ MirrorObject },													// 鏡像移動
	{ ScaleObject },													// ｻｲｽﾞ変更
	{ BendObject },														// 円筒曲げ
	{ SetGroup },														// ｸﾞﾙｰﾌﾟ設定
	{ SetColor },														// ｶﾗｰNo設定
	{ SetAttribute },													// 属性No設定
	{ MakeSweepSurf },													// 掃引曲面生成
	{ ChangeSweepSurf },												// 掃引曲面編集
	{ TwistObject },													// ひねり
	{ ResizeObject1 },													// 直線指定ﾘｻｲｽﾞ1
	{ SetParts },														// ﾊﾟｰﾂNo設定
	{ ResizeObject2 },													// 曲線指定ﾘｻｲｽﾞ1
	{ ShearObject },													// せん断
	{ RingObject },														// ﾘﾝｸﾞ
	{ EarthObject },													// ｱｰｽ
	{ SpaceObject },													// ｽﾍﾟｰｽ
	{ ResizeObject3 },													// 直線指定ﾘｻｲｽﾞ2
	{ ResizeObject4 },													// 曲線指定ﾘｻｲｽﾞ2
	{ CommandError }
};

/**************************/
void ExecCommand(BOOL stp)												//<<< ｺﾏﾝﾄﾞ実行
/**************************/
{
	CMDTYPE* cmp;
	CTBTYPE* pc;
	OBJTYPE* op;
	int      cno;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	cno = GetCmdNo(cmp);												// ｺﾏﾝﾄﾞNo取得
	cno = (cno>MAXCMD) ? MAXCMD+1 : cno;
	pc = &Cmdtbl[cno];
	if(stp) {															// <ｺﾏﾝﾄﾞ実行停止あり>
		OnCmdExeStp();													// ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞON
		op = (*pc->comfunc)();											// ｺﾏﾝﾄﾞ起動
		OffCmdExeStp();													// ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞOFF
	} else {															// <ｺﾏﾝﾄﾞ実行停止なし>
		op = (*pc->comfunc)();											// ｺﾏﾝﾄﾞ起動
	}
	if(op!=NULL) {
		SetObjPtr(op);													// ﾎﾟｲﾝﾀｾｯﾄ
		OnCmdFlg();														// ｺﾏﾝﾄﾞ未保存ON
	}
}

/***************************/
OBJTYPE* CommandError(void)												//<<< ｺﾏﾝﾄﾞｴﾗｰ処理
/***************************/
{
	AfxMessageBox(IDS_ERR_COMMAND);										// ｴﾗｰﾒｯｾｰｼﾞ出力
	return NULL;
}

/*****************************/
void RedoCommand(OBJTYPE* op)											//<<< ｺﾏﾝﾄﾞ再実行
/*****************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ﾘｾｯﾄ立体登録
	BaseCmdPtr(1, op, &cmp);											// 有効ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		SetCmdPtr(cmp);													// 処理対象ｺﾏﾝﾄﾞ設定
		SetObjPtr(op);													// 処理対象立体登録
		ExecCommand(FALSE);												// ｺﾏﾝﾄﾞ起動
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞ取得
	}
	OffResetObj();														// ﾘｾｯﾄ立体解除
	SetObjBox(op);														// ﾎﾞｯｸｽｻｲｽﾞ設定
}

/**********************************/
void RedoStartCommand(OBJTYPE* op)										//<<< ﾒﾆｭｰ開始時ｺﾏﾝﾄﾞ再実行
/**********************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ﾘｾｯﾄ立体登録
	BaseCmdPtr(1, op, &cmp);											// 有効ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(cmp->onxt!=NULL) {											// <最終(現在実行中)ｺﾏﾝﾄﾞ以外>
			SetCmdPtr(cmp);												// 処理対象ｺﾏﾝﾄﾞ設定
			SetObjPtr(op);												// 処理対象立体登録
			ExecCommand(FALSE);											// ｺﾏﾝﾄﾞ起動
		}
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞ取得
	}
	OffResetObj();														// ﾘｾｯﾄ立体解除
	SetObjBox(op);														// ﾎﾞｯｸｽｻｲｽﾞ設定
}

/***********************************/
void RedoCreateCommand(OBJTYPE* op)										//<<< 生成ｺﾏﾝﾄﾞ再実行
/***********************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ﾘｾｯﾄ立体登録
	BaseCmdPtr(1, op, &cmp);											// 有効ｺﾏﾝﾄﾞ取得
	if(cmp!=NULL) {
		SetCmdPtr(cmp);													// 処理対象ｺﾏﾝﾄﾞ設定
		SetObjPtr(op);													// 処理対象立体登録
		ExecCommand(FALSE);												// ｺﾏﾝﾄﾞ起動
	}
	OffResetObj();														// ﾘｾｯﾄ立体解除
	SetObjBox(op);														// ﾎﾞｯｸｽｻｲｽﾞ設定
}

/***************************************/
void RedoRingBeforeCommand(OBJTYPE* op)									//<<< ﾘﾝｸﾞｻｲｽﾞ前ｺﾏﾝﾄﾞ再実行
/***************************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ﾘｾｯﾄ立体登録
	BaseCmdPtr(1, op, &cmp);											// 有効ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		if(cmp==op->rccmp) {											// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞは処理中止
			break;
		}
		SetCmdPtr(cmp);													// 処理対象ｺﾏﾝﾄﾞ設定
		SetObjPtr(op);													// 処理対象立体登録
		ExecCommand(FALSE);												// ｺﾏﾝﾄﾞ起動
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞ取得
	}
	OffResetObj();														// ﾘｾｯﾄ立体解除
	SetObjBox(op);														// ﾎﾞｯｸｽｻｲｽﾞ設定
}

/***********************************************/
void RedoCommandCopy(OBJTYPE* op, CMDTYPE* cmp)							//<<< ｺﾏﾝﾄﾞ複写実行
/***********************************************/
{
	CMDTYPE* cptr;
	PNTTYPE  pd;
	int      cno, i, inum, pnum;

	cno = GetCmdNo(cmp);												// ｺﾏﾝﾄﾞNo取得
	inum = GetIntNum(cmp);												// 整数ﾊﾟﾗﾒｰﾀ数取得
	pnum = GetPntNum(cmp);												// 座標ﾊﾟﾗﾒｰﾀ数取得
	CreatCmd(cno, inum, pnum, &cptr);									// ｺﾏﾝﾄﾞ領域確保
	for(i=0; i<inum; i++) {
		PutParaInt(cptr, i, GetParaInt(cmp, i));						// 整数ﾊﾟﾗﾒｰﾀ設定
	}
	for(i=0; i<pnum; i++) {
		GetParaPnt(cmp, i, &pd);										// 座標ﾊﾟﾗﾒｰﾀ取得
		PutParaPnt(cptr, i, pd);										// 座標ﾊﾟﾗﾒｰﾀ設定
	}
	SetObjPtr(op);														// 対象立体設定
	SetCmdPtr(cptr);													// ｺﾏﾝﾄﾞ設定
	ExecCommand(FALSE);													// ｺﾏﾝﾄﾞ起動
}
