/////////////////////////////////////////////////////////////////////////////
// ライン描画クラス(CLine)定義
// Line.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLine

/******************/
CLine::CLine(void)														//<<< ｸﾗｽの構築
/******************/
{
	SetFlg = FALSE;
	Lsno = 0;
	Mode = 0;
}

/********************************************************************/
void CLine::Setup(PNTTYPE* ps, PNTTYPE* pe, int* cl, int pn, int md)	//<<< ﾗｲﾝ設定
/********************************************************************/
{
	int    i;
	double rc, gc, bc;

	if(pn<1) {
		return;
	}
	Lsno = glGenLists(1);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	glNewList(Lsno, GL_COMPILE);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成
		for(i=0; i<pn; i++) {
			if(!cl[i]) {
				MainWnd->GetColor(MainWnd->LineColor, &rc, &gc, &bc);
			} else {
				MainWnd->GetColor(MainWnd->BaseColor, &rc, &gc, &bc);
			}
			glColor3d(rc, gc, bc);										// 表示色設定
			glBegin(GL_LINES);
				glVertex3d(ps[i].p[0], ps[i].p[1], ps[i].p[2]);			// 開始点の登録
				glVertex3d(pe[i].p[0], pe[i].p[1], pe[i].p[2]);			// 終了点の登録
			glEnd();
		}
	glEndList();
	Mode = md;
	SetFlg = TRUE;
}

/***************************/
void CLine::Draw(int plane)												//<<< ﾗｲﾝ描画
/***************************/
{
	if(Mode==plane||Mode==4||(Mode==5&&plane!=PRS)) {
		glDisable(GL_DEPTH_TEST);
		glCallList(Lsno);
		glEnable(GL_DEPTH_TEST);
	}
}

/***********************/
void CLine::Reset(void)													//<<< ﾘｾｯﾄ
/***********************/
{
	SetFlg = FALSE;
	if(Lsno>0) {
		glDeleteLists(Lsno, 1);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
	}
	Lsno = 0, Mode = 0;
}
