/////////////////////////////////////////////////////////////////////////////
// 立体生成 直接生成コマンド部
// CCmdDirect.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataAccess1.h"
#include "DataAccess2.h"

/*************************/
OBJTYPE* MakeDirect(void)												//<<< 直接生成ｺﾏﾝﾄﾞ
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, vnum, lnum, vno;

	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	vnum = GetParaInt(cmp, 0);											// 頂点数取得
	lnum = GetParaInt(cmp, 1);											// 面ﾙｰﾌﾟ数取得
	CreatObj(vnum, lnum, 0, &op);										// 立体領域確保
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	LevelObj(0, op);													// 同一ﾚﾍﾞﾙ生成立体接続
	if(!GetCmdExeStp()) {												// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		for(i=0; i<vnum; i++) {
			GetParaPnt(cmp, i, &pnt);									// 座標ﾊﾟﾗﾒｰﾀ取得
			PutVtx(op, i, pnt);											// 頂点設定
		}
		for(i=0; i<lnum; i++) {
			vno = GetParaInt(cmp, i+2);									// 整数ﾊﾟﾗﾒｰﾀ取得
			if(vno>0) {
				PutFlp1(op, i, vno-1, 0);								// 面ﾙｰﾌﾟ情報設定
			} else {
				PutFlp1(op, i, -vno-1, 1);
			}
		}
	}
	PutObjAtr(op, 0);													// 属性No設定(DIRECT)
	PutObjPrt(op, 0, 0);												// ﾊﾟｰﾂNo設定(通常)
	PutObjCol(op, 0);													// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	}
	return op;
}
