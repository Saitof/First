/////////////////////////////////////////////////////////////////////////////
// アプリケーションメインクラス(CJcad3GlbApp)定義
// Jcad3Glb.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

// ************************ 追加 ************************
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
#define LANGUAGE_ID 1													// 言語ID:1(日本語)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp

BEGIN_MESSAGE_MAP(CJcad3GlbApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CJcad3GlbApp::OnAppAbout)
	// 標準のファイル基本ドキュメント コマンド
//	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp コンストラクション

/****************************/
CJcad3GlbApp::CJcad3GlbApp()
/****************************/
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CJcad3GlbApp オブジェクトです。

CJcad3GlbApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp 初期化

/*********************************/
BOOL CJcad3GlbApp::InitInstance()
/*********************************/
{
	int ret;

	OutputDebugString("-Basic version-\n");

	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるにはこれを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	// OLE ライブラリを初期化します。
	if(!AfxOleInit()) {
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

// ************************ 追加 ************************
	//引数には　dllファイルがあるパスを入れて下さい
	//例：LoadImageKitDll('c:\\Product\\ImageKit\\ImageKit7\\');
	//引数にNULLを入れた場合には、パスが通っているフォルダを検索します
	LoadImageKitDll(NULL);
// ******************************************************
	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。

	SetRegistryKey(_T("GLB"));
	LoadStdProfileSettings(8);		// 標準の INI ファイルのオプションをロードします (MRU を含む)
	ret = LicenseCheck();												// ﾗｲｾﾝｽﾁｪｯｸ

	InitVar();															// CADｼｽﾃﾑ初期化
	if(ret!=0) {
		SetDemoFlg(1);													// 体験版
	}
	StartBackUp(1);														// ﾊﾞｯｸｱｯﾌﾟ開始処理
	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(								// ﾄﾞｷｭﾒﾝﾄﾃﾝﾌﾟﾚｰﾄ作成
		IDR_MAINFRAME,													// ﾘｿｰｽID
		RUNTIME_CLASS(CJcad3GlbDoc),									// ﾄﾞｷｭﾒﾝﾄ
		RUNTIME_CLASS(CMainFrame),										// ﾒｲﾝSDIﾌﾚｰﾑｳｨﾝﾄﾞｳ
		RUNTIME_CLASS(CJcad3GlbView));									// ﾋﾞｭｰ
	if(!pDocTemplate) {
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);
	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);				// ｽﾌﾟﾗｯｼｭｽｸﾘｰﾝ
	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。

	if(!ProcessShellCommand(cmdInfo)) {
		return FALSE;
	}

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出してください。
	m_pMainWnd->DragAcceptFiles();
	//  SDI アプリケーションでは、ProcessShellCommand の直後にこの呼び出しが発生しなければなりません。
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp メッセージ ハンドラ

/**************************************/
BOOL CJcad3GlbApp::OnIdle(LONG lCount)									//<<< WM_IDLE ﾒｯｾｰｼﾞ応答処理
/**************************************/								// ｱｲﾄﾞﾙ時処理
{
	MainWnd->MessageOut();
	return CWinApp::OnIdle(lCount);
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp コマンド

/********************************/
int CJcad3GlbApp::ExitInstance() 
/********************************/
{
// ************************ 追加 ************************
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
CString GetAppPath(int mode)											//<<< 実行形式ﾌｧｲﾙのﾃﾞｨﾚｸﾄﾘ取得
/****************************/
{
	TCHAR buf[_MAX_PATH], sbuf[_MAX_PATH];

	::GetModuleFileName(AfxGetInstanceHandle(), buf, _MAX_PATH);		// 実行形式ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
	LPTSTR lpszFile = _tcsrchr(buf, '\\');
	lpszFile++;
	*lpszFile = 0x00;													// ﾌｧｲﾙ名の先頭にﾇﾙを入れ
	if(mode) {
		::GetShortPathName(buf, sbuf, _MAX_PATH);						// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		return CString(sbuf);											// ｼｮｰﾄﾌｧｲﾙのﾃﾞｨﾚｸﾄﾘ取得
	} else {
		return CString(buf);											// 実行形式ﾌｧｲﾙのﾃﾞｨﾚｸﾄﾘ取得
	}
}

/*****************************************/
BOOL CheckFilePath(int flg, CString path)								//<<< ﾌｧｲﾙﾊﾟｽの有効確認
/*****************************************/
{
	char*   volname = new char[_MAX_PATH];
	char*   sysname = new char[_MAX_PATH];
	WIN32_FIND_DATA wf;
	CString wpath;
	BOOL    ret=TRUE;
	int     idx;
	DWORD   serial, maxlen, flgs;

	if(!flg) {															// <ﾌｫﾙﾀﾞの場合>
		idx = path.GetLength() - 1;										// ﾊﾟｽ長(最終\ぬき)
	} else {															// <ﾌｧｲﾙの場合>
		idx = path.GetLength();											// ﾌｧｲﾙ長
	}
	wpath = path.Left(idx);
	if(idx>2) {															// <ﾌｫﾙﾀﾞ有>
		if(FindFirstFile(wpath, &wf)==INVALID_HANDLE_VALUE) {
			ret = FALSE;
		}
	} else {															// <ﾌｫﾙﾀﾞ無>
		ret = GetVolumeInformation(wpath, volname, _MAX_PATH,
                                   &serial, &maxlen, &flgs,
                                   sysname, _MAX_PATH);					// VOLUME情報取得
	}
	delete[] volname;
	delete[] sysname;
	return ret;
}
