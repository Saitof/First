/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�������C���N���X(CJcad3GlbApp)��`
// Jcad3Glb.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

// ************************ �ǉ� ************************
#define  _MAIN_IK7_PROGRAM_
// ******************************************************

#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "AboutDlg.h"
#include "Splash.h"
#include "DataAccess1.h"
#include "LCheck.h"

#define VERSION_NO  10000												// VersionNo:1.00.00
#define LANGUAGE_ID 1													// ����ID:1(���{��)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp

BEGIN_MESSAGE_MAP(CJcad3GlbApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CJcad3GlbApp::OnAppAbout)
	// �W���̃t�@�C����{�h�L�������g �R�}���h
//	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp �R���X�g���N�V����

/****************************/
CJcad3GlbApp::CJcad3GlbApp()
/****************************/
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CJcad3GlbApp �I�u�W�F�N�g�ł��B

CJcad3GlbApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp ������

/*********************************/
BOOL CJcad3GlbApp::InitInstance()
/*********************************/
{
	int ret;

	OutputDebugString("-Basic version-\n");

	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ͂����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	// OLE ���C�u���������������܂��B
	if(!AfxOleInit()) {
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

// ************************ �ǉ� ************************
	//�����ɂ́@dll�t�@�C��������p�X�����ĉ�����
	//��FLoadImageKitDll('c:\\Product\\ImageKit\\ImageKit7\\');
	//������NULL����ꂽ�ꍇ�ɂ́A�p�X���ʂ��Ă���t�H���_���������܂�
	LoadImageKitDll(NULL);
// ******************************************************
	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B

	SetRegistryKey(_T("GLB"));
	LoadStdProfileSettings(8);		// �W���� INI �t�@�C���̃I�v�V���������[�h���܂� (MRU ���܂�)
	ret = LicenseCheck();												// ײ�ݽ����

	InitVar();															// CAD���я�����
	if(ret!=0) {
		SetDemoFlg(1);													// �̌���
	}
	StartBackUp(1);														// �ޯ����ߊJ�n����
	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(								// �޷��������ڰč쐬
		IDR_MAINFRAME,													// ؿ��ID
		RUNTIME_CLASS(CJcad3GlbDoc),									// �޷����
		RUNTIME_CLASS(CMainFrame),										// Ҳ�SDI�ڰѳ���޳
		RUNTIME_CLASS(CJcad3GlbView));									// �ޭ�
	if(!pDocTemplate) {
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);
	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h ���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);				// ���ׯ����ذ�
	// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�AFalse ��Ԃ��܂��B

	if(!ProcessShellCommand(cmdInfo)) {
		return FALSE;
	}

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �ڔ��������݂���ꍇ�ɂ̂� DragAcceptFiles ���Ăяo���Ă��������B
	m_pMainWnd->DragAcceptFiles();
	//  SDI �A�v���P�[�V�����ł́AProcessShellCommand �̒���ɂ��̌Ăяo�����������Ȃ���΂Ȃ�܂���B
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp ���b�Z�[�W �n���h��

/**************************************/
BOOL CJcad3GlbApp::OnIdle(LONG lCount)									//<<< WM_IDLE ү���މ�������
/**************************************/								// ����َ�����
{
	MainWnd->MessageOut();
	return CWinApp::OnIdle(lCount);
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp �R�}���h

/********************************/
int CJcad3GlbApp::ExitInstance() 
/********************************/
{
// ************************ �ǉ� ************************
	UnloadImageKitDll();
// ******************************************************
	return CWinApp::ExitInstance();
}

/*******************************/
void CJcad3GlbApp::OnAppAbout()
/*******************************/
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/*************************************************/
BOOL CJcad3GlbApp::PreTranslateMessage(MSG* pMsg)
/*************************************************/
{
	if(CSplashWnd::PreTranslateAppMessage(pMsg)) {
		return TRUE;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

/***************************************************************/
CDocument* CJcad3GlbApp::OpenDocumentFile(LPCTSTR lpszFileName)
/***************************************************************/
{
	CString str(lpszFileName);
	MainWnd->DoOpenFile(str);
	return CWinApp::OpenDocumentFile(lpszFileName);
}

/****************************/
CString GetAppPath(int mode)											//<<< ���s�`��̧�ق��ިڸ�؎擾
/****************************/
{
	TCHAR buf[_MAX_PATH], sbuf[_MAX_PATH];

	::GetModuleFileName(AfxGetInstanceHandle(), buf, _MAX_PATH);		// ���s�`��̧�ٖ�(���߽)�擾
	LPTSTR lpszFile = _tcsrchr(buf, '\\');
	lpszFile++;
	*lpszFile = 0x00;													// ̧�ٖ��̐擪���ق����
	if(mode) {
		::GetShortPathName(buf, sbuf, _MAX_PATH);						// ����̧�ٖ�(���߽)�擾
		return CString(sbuf);											// ����̧�ق��ިڸ�؎擾
	} else {
		return CString(buf);											// ���s�`��̧�ق��ިڸ�؎擾
	}
}

/*****************************************/
BOOL CheckFilePath(int flg, CString path)								//<<< ̧���߽�̗L���m�F
/*****************************************/
{
	char*   volname = new char[_MAX_PATH];
	char*   sysname = new char[_MAX_PATH];
	WIN32_FIND_DATA wf;
	CString wpath;
	BOOL    ret=TRUE;
	int     idx;
	DWORD   serial, maxlen, flgs;

	if(!flg) {															// <̫��ނ̏ꍇ>
		idx = path.GetLength() - 1;										// �߽��(�ŏI\�ʂ�)
	} else {															// <̧�ق̏ꍇ>
		idx = path.GetLength();											// ̧�ْ�
	}
	wpath = path.Left(idx);
	if(idx>2) {															// <̫��ޗL>
		if(FindFirstFile(wpath, &wf)==INVALID_HANDLE_VALUE) {
			ret = FALSE;
		}
	} else {															// <̫��ޖ�>
		ret = GetVolumeInformation(wpath, volname, _MAX_PATH,
                                   &serial, &maxlen, &flgs,
                                   sysname, _MAX_PATH);					// VOLUME���擾
	}
	delete[] volname;
	delete[] sysname;
	return ret;
}
