/////////////////////////////////////////////////////////////////////////////
// �X�v���b�V���X�N���[���N���X(CSplashWnd)�錾
// Splash.h
//---------------------------------------------------------------------------
// LastEdit : 2009/06/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

class CSplashWnd : public CWnd
{
protected:
	CSplashWnd(void) { ; }												// �׽�̍\�z
public:
	virtual ~CSplashWnd(void);											// �׽�̏���

// ����
public:
	CBitmap m_bitmap;

// �I�y���[�V����
public:
	static void EnableSplashScreen(BOOL bEnable=TRUE) { c_bShowSplashWnd = bEnable; }
	static void ShowSplashScreen(CWnd* pParentWnd=NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);

// �I�[�o�[���C�h
	virtual void PostNcDestroy(void);

// �C���v�������e�[�V����
public:

protected:
	BOOL Create(CWnd* pParentWnd=NULL);
	void HideSplashScreen(void);

	static BOOL        c_bShowSplashWnd;
	static CSplashWnd* c_pSplashWnd;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};

#endif _SPLASH_SCRN_

//==============================<EOF:Splash.h>===============================
