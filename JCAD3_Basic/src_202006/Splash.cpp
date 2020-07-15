/////////////////////////////////////////////////////////////////////////////
// �X�v���b�V���X�N���[���N���X(CSplashWnd)��`
// Splash.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/06/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "Splash.h"
#include "DataAccess1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   �X�v���b�V�� �X�N���[�� �N���X

BOOL		CSplashWnd::c_bShowSplashWnd;
CSplashWnd*	CSplashWnd::c_pSplashWnd;

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/************************************/
inline CSplashWnd::~CSplashWnd(void)									// �׽�̏���
/************************************/
{
	// �X�^�e�B�b�N�ȃE�B���h�E �|�C���^�̃N���A
	ASSERT(c_pSplashWnd==this);
	c_pSplashWnd = NULL;
}

/*******************************************************/
int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
/*******************************************************/
{
	if(CWnd::OnCreate(lpCreateStruct)==-1) {
		return -1;
	}
	CenterWindow();														// ����޳�𒆉��ɔz�u
	SetTimer(1, 2000, NULL);											// ���ׯ����ذ݂�j��
	return 0;															// ����܂ł���ς�ݒ�
}

/******************************/
void CSplashWnd::OnPaint(void)
/******************************/
{
	CPaintDC dc(this);
	CDC      dcImage;

	if(!dcImage.CreateCompatibleDC(&dc)) {
		return;
	}
	BITMAP	bm;
	m_bitmap.GetBitmap(&bm);
	CBitmap*  pOldBitmap = dcImage.SelectObject(&m_bitmap);				// �摜��`��
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}

/*******************************************/
inline void CSplashWnd::PostNcDestroy(void)
/*******************************************/
{
	delete this;														// C++ �׽�� Free
}

/*************************************/
inline void CSplashWnd::OnTimer(UINT_PTR)
/*************************************/
{
	HideSplashScreen();													// ���ׯ����ذݳ���޳��j��
}

/*****************************************/
BOOL CSplashWnd::Create(CWnd* pParentWnd)
/*****************************************/
{
	if(!m_bitmap.LoadBitmap(IDB_SPLASH)) {
		return FALSE;
	}
	BITMAP	bm;
	m_bitmap.GetBitmap(&bm);
	return CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), NULL,
			WS_POPUP|WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

/**************************************************/
BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
/**************************************************/
{
	if(c_pSplashWnd==NULL) {
		return FALSE;
	}
	// �L�[�{�[�h�܂��̓}�E�X ���b�Z�[�W���󂯎�����ꍇ�ɃX�v���b�V���X�N���[�����\���ɂ��܂��B
	if(pMsg->message==WM_KEYDOWN || pMsg->message==WM_SYSKEYDOWN ||
		pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_RBUTTONDOWN ||
		pMsg->message==WM_MBUTTONDOWN || pMsg->message==WM_NCLBUTTONDOWN ||
		pMsg->message==WM_NCRBUTTONDOWN || pMsg->message==WM_NCMBUTTONDOWN) {
		c_pSplashWnd->HideSplashScreen();
		return TRUE;
	}
	return FALSE;
}

/***************************************************/
void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd)
/***************************************************/
{
	if(!c_bShowSplashWnd || c_pSplashWnd!=NULL) {
		return;
	}
	// �X�v���b�V�� �X�N���[����V�K�ɃA���P�[�g���A�E�B���h�E���쐬���܂��B
	c_pSplashWnd = new CSplashWnd;
	if(!c_pSplashWnd->Create(pParentWnd)) {
		delete c_pSplashWnd;
	} else {
		c_pSplashWnd->UpdateWindow();
	}
}

/**********************************************/
inline void CSplashWnd::HideSplashScreen(void)							// ���ׯ����ذݳ���޳��j��
/**********************************************/
{
	KillTimer(1);
	DestroyWindow();													// ���ׯ����ذݳ���޳��j��
	AfxGetMainWnd()->UpdateWindow();									// Ҳݳ���޳���X�V
	OutSplashMsg();														// ���ׯ���\����ү���ޏo��
}
