/////////////////////////////////////////////////////////////////////////////
// アプリケーションメインクラス(CJcad3GlbApp)宣言
// Jcad3Glb.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"													// メイン シンボル
// ************************ 追加 ************************
#include "Imagekit7.h"
#define  _IK7FILE_
#define  _IK7PRINT_
#define  _IK7VECT_
#include "Imagekit7Def.h"
// ******************************************************

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp:
// このクラスの実装については、Jcad3Glb.cpp を参照してください。
//

class CJcad3GlbApp : public CWinApp
{
public:
	CJcad3GlbApp();

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

CString GetAppPath(int mode);
BOOL    CheckFilePath(int flg, CString path);
extern  CJcad3GlbApp theApp;											// CJcad3GlbAppｵﾌﾞｼﾞｪｸﾄ

//=============================<EOF:Jcad3Glb.h>=============================