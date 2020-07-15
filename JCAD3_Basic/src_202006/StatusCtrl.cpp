/////////////////////////////////////////////////////////////////////////////
// ステータスバークラス(CStatusCtrl)定義
// StatusCtrl.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CStatusCtrl

BEGIN_MESSAGE_MAP(CStatusCtrl, CStatusBar)
END_MESSAGE_MAP()

/******************************/
CStatusCtrl::CStatusCtrl(void)											// ｸﾗｽの構築
/******************************/
{
	m_sMess = "Ready";
}

/////////////////////////////////////////////////////////////////////////////
// CStatusCtrl メッセージ ハンドラ

/**************************************/
void CStatusCtrl::MessageOut(UINT nID)									// ﾒｯｾｰｼﾞ設定
/**************************************/
{
	m_sMess.LoadString(nID);											// ﾒｯｾｰｼﾞ文字列読込
}

/**********************************/
void CStatusCtrl::MessageOut(void)										// ﾒｯｾｰｼﾞ表示
/**********************************/
{
	POINT point;

	GetCursorPos(&point);
	if(MainWnd->GetActiveView()==MainWnd->WindowFromPoint(point)) {		// ﾏｳｽがView上にある場合
		GetStatusBarCtrl().SetText(m_sMess, 0, 0);						// ﾒｯｾｰｼﾞ表示
	}
}
