/////////////////////////////////////////////////////////////////////////////
// スプラッシュスクリーンクラス(CSplashWnd)宣言
// Splash.h
//---------------------------------------------------------------------------
// LastEdit : 2009/06/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

class CSplashWnd : public CWnd
{
protected:
	CSplashWnd(void) { ; }												// ｸﾗｽの構築
public:
	virtual ~CSplashWnd(void);											// ｸﾗｽの消滅

// 属性
public:
	CBitmap m_bitmap;

// オペレーション
public:
	static void EnableSplashScreen(BOOL bEnable=TRUE) { c_bShowSplashWnd = bEnable; }
	static void ShowSplashScreen(CWnd* pParentWnd=NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);

// オーバーライド
	virtual void PostNcDestroy(void);

// インプリメンテーション
public:

protected:
	BOOL Create(CWnd* pParentWnd=NULL);
	void HideSplashScreen(void);

	static BOOL        c_bShowSplashWnd;
	static CSplashWnd* c_pSplashWnd;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};

#endif _SPLASH_SCRN_

//==============================<EOF:Splash.h>===============================
