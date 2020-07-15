/////////////////////////////////////////////////////////////////////////////
// 立体属性設定コマンド部
// Attribute.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"

/***************************/
OBJTYPE* SetAttribute(void)												//<<< 属性No設定ｺﾏﾝﾄﾞ
/***************************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;
	int      atr;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	if((atr=GetParaInt(cmp, 0))!=30) {									// 旧JEWEL以外:旧属性設定
		PutObjAtr(op, atr);
	} else {															// 旧JEWEL:DIRECT[GEM]属性
		PutObjAtr(op, 1);
	}
	FreeCmd(op, cmp, 0);												// 属性Noｺﾏﾝﾄﾞ削除
	return op;
}

/**********************************************/
void WriteAttributeFile(FILE* fp, OBJTYPE* op)							//<<< 属性Noﾌｧｲﾙ保存
/**********************************************/
{
	fprintf(fp, "C%02d 1 0\n", ATTRIBUTE);								// ﾌｧｲﾙに書込
	fprintf(fp, "%d\n", GetObjAtr(op));
}

/***********************************************************/
void WriteAttributeFileJgd(FILE* fp, OBJTYPE* op, int obno)				//<<< 属性Noﾌｧｲﾙ保存(JGD用)
/***********************************************************/
{
	fprintf(fp, "C%02d %d %d 1 0\n", ATTRIBUTE, obno, -1);				// ﾌｧｲﾙに書込
	fprintf(fp, "%d\n", GetObjAtr(op));
}

/***********************/
OBJTYPE* SetColor(void)													//<<< ｶﾗｰNo設定ｺﾏﾝﾄﾞ
/***********************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	PutObjCol(op, GetParaInt(cmp, 0));									// ｶﾗｰNo設定
	FreeCmd(op, cmp, 0);												// ｶﾗｰNoｺﾏﾝﾄﾞ削除
	return op;
}

/******************************************/
void WriteColorFile(FILE* fp, OBJTYPE* op)								//<<< ｶﾗｰNoﾌｧｲﾙ保存
/******************************************/
{
	fprintf(fp, "C%02d 1 0\n", COLOR);									// ﾌｧｲﾙに書込
	fprintf(fp, "%d\n", GetObjCol(op));
}

/*******************************************************/
void WriteColorFileJgd(FILE* fp, OBJTYPE* op, int obno)					//<<< ｶﾗｰNoﾌｧｲﾙ保存(JGD用)
/*******************************************************/
{
	fprintf(fp, "C%02d %d %d 1 0\n", COLOR, obno, -1);					// ﾌｧｲﾙに書込
	fprintf(fp, "%d\n", GetObjCol(op));
}

/***********************/
OBJTYPE* SetGroup(void)													//<<< ｸﾞﾙｰﾌﾟ設定ｺﾏﾝﾄﾞ
/***********************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;
	int      i, prm, gno[32];

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	for(i=0; i<8; i++) {
		prm = GetParaInt(cmp, i);										// ﾊﾟﾗﾒｰﾀ取得
		gno[i] = (prm%2==1) ? 1 : 0;									// ｸﾞﾙｰﾌﾟ1～8設定
		prm = (prm%2) ? prm-1 : prm;
		prm = prm/2;
		gno[i+8] = (prm%2==1) ? 1 : 0;									// ｸﾞﾙｰﾌﾟ9～16設定
		prm = (prm%2) ? prm-1 : prm;
		prm = prm/2;
		gno[i+16] = (prm%2==1) ? 1 : 0;									// ｸﾞﾙｰﾌﾟ17～24設定
		gno[i+24] = (prm/2==1) ? 1 : 0;									// ｸﾞﾙｰﾌﾟ25～32設定
	}
	PutGroup(op, gno);													// ｸﾞﾙｰﾌﾟ情報設定
	FreeCmd(op, cmp, 0);												// ｸﾞﾙｰﾌﾟｺﾏﾝﾄﾞ削除
	return op;
}

/******************************************/
void WriteGroupFile(FILE* fp, OBJTYPE* op)								//<<< ｸﾞﾙｰﾌﾟ情報ﾌｧｲﾙ保存
/******************************************/
{
	int	i, gno[32], gno2[8];

	GetGroup(op, gno);													// ｸﾞﾙｰﾌﾟ情報取得
	for(i=0; i<8; i++) {
		gno2[i] = gno[i]+(gno[i+8]*2)+(gno[i+16]*4)+(gno[i+24]*8);		// 1-8:+1,9-16:+2,17-24:+4,25-32:+8
	}
	fprintf(fp, "C%02d 8 0\n", GROUP);									// ﾌｧｲﾙに書込
	fprintf(fp, "%d %d %d %d ",  gno2[0], gno2[1], gno2[2], gno2[3]);
	fprintf(fp, "%d %d %d %d\n", gno2[4], gno2[5], gno2[6], gno2[7]);
}

/*******************************************************/
void WriteGroupFileJgd(FILE* fp, OBJTYPE* op, int obno)					//<<< ｸﾞﾙｰﾌﾟ情報ﾌｧｲﾙ保存(JGD用)
/*******************************************************/
{
	int	i, gno[32], gno2[8];

	GetGroup(op, gno);													// ｸﾞﾙｰﾌﾟ情報取得
	for(i=0; i<8; i++) {
		gno2[i] = gno[i]+(gno[i+8]*2)+(gno[i+16]*4)+(gno[i+24]*8);		// 1-8:+1,9-16:+2,17-24:+4,25-32:+8
	}
	fprintf(fp, "C%02d %d %d 8 0\n", GROUP, obno, -1);					// ﾌｧｲﾙに書込
	fprintf(fp, "%d %d %d %d ",  gno2[0], gno2[1], gno2[2], gno2[3]);
	fprintf(fp, "%d %d %d %d\n", gno2[4], gno2[5], gno2[6], gno2[7]);
}

/***********************/
OBJTYPE* SetParts(void)													//<<< ﾊﾟｰﾂNo設定ｺﾏﾝﾄﾞ
/***********************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;

	GetCmdPtr(&cmp);													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
	GetObjPtr(&op);														// 対象立体取得
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	PutObjPrt(op, GetParaInt(cmp, 0), 1);								// ﾊﾟｰﾂNo設定
	FreeCmd(op, cmp, 0);												// ﾊﾟｰﾂNoｺﾏﾝﾄﾞ削除
	return op;
}

/******************************************/
void WritePartsFile(FILE* fp, OBJTYPE* op)								//<<< ﾊﾟｰﾂNoﾌｧｲﾙ保存
/******************************************/
{
	fprintf(fp, "C%02d 1 0\n", PARTS);									// ﾌｧｲﾙに書込
	fprintf(fp, "%d\n", GetObjPrt(op));
}

/*******************************************************/
void WritePartsFileJgd(FILE* fp, OBJTYPE* op, int obno)					//<<< ﾊﾟｰﾂNoﾌｧｲﾙ保存(JGD用)
/*******************************************************/
{
	fprintf(fp, "C%02d %d %d 1 0\n", PARTS, obno, -1);					// ﾌｧｲﾙに書込
	fprintf(fp, "%d\n", GetObjPrt(op));
}
