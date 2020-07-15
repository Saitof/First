/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// Jcad3GlbView.cpp
//---------------------------------------------------------------------------
// LastEdit: 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbDoc.h"
#include "Jcad3GlbView.h"
#include "MainFrm.h"
#include "InputsDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HBMP          512
#define VBMP          256
#define WIDTHBYTES(i) ((i+31)/32*4)

GLubyte        TexImage[VBMP][HBMP][3];
CJcad3GlbView* View;

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView

IMPLEMENT_DYNCREATE(CJcad3GlbView, CView)

BEGIN_MESSAGE_MAP(CJcad3GlbView, CView)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	// 標準印刷コマンド
//	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView コンストラクション/デストラクション

/**********************************/
CJcad3GlbView::CJcad3GlbView(void)										//<<< ｸﾗｽ構築
/**********************************/
{
	int i, j;

	m_pMainDC = NULL, View = this;
	m_hcursor1 = AfxGetApp()->LoadCursor(IDC_CURSOR1);
	m_hcursor2 = AfxGetApp()->LoadCursor(IDC_CURSOR2);
	m_hcursor3 = AfxGetApp()->LoadCursor(IDC_CURSOR3);
	m_hcursor4 = AfxGetApp()->LoadCursor(IDC_CURSOR4);
	m_hcursor5 = AfxGetApp()->LoadCursor(IDC_CURSOR5);
	m_Char = 0, m_CChar = 0, m_FChar = 0, m_FPres = 0, m_DChar = 0;
	Plane = XY, m_MoveInit = 0, m_ScrChg = 0, m_DisplayFlg = 0;
	for(i=0; i<3; i++) {
		m_TEXTURE[i] = 0, m_GLB[i] = 0, m_TEMP[i] = 0;
	}
	m_BOX = 0, m_CIRCLE = 0, m_SCALE = 0, m_PROT = 0, m_BASE = 0, m_NCLIST = 0, m_STAGE = 0;
	m_BoxPlane = 0, m_CirclePlane = 0, m_ScalePlane = 0, m_ProtPlane = 0, m_BasePlane = 0;
	m_PrtKind = 0, m_OpenGLTest = 0, m_CurveAFlg = 0, m_CurveRFlg = 0;
	m_MapFile = _T("");
	for(i=0; i<3; i++) {
		m_RColor[i] = RGB(255, 255, 255);
	}
	for(i=0; i<18; i++) {
		for(j=0; j<3; j++) {
			m_CColor[i][j] = RGB(255, 255, 255);
		}
		m_Shin[i] = 0;
	}
	m_CColor[18][0] = RGB(63, 47, 47);									// 選択立体ｼｪｰﾃﾞｨﾝｸﾞ色
	m_CColor[18][1] = RGB(63, 63, 63);
	m_CColor[18][2] = RGB(31, 31, 31);
	m_Shin[18] = 8;
	m_CColor[19][0] = RGB(47, 63, 47);									// 選択解除ｼｪｰﾃﾞｨﾝｸﾞ色
	m_CColor[19][1] = RGB(63, 63, 63);
	m_CColor[19][2] = RGB(31, 31, 31);
	m_Shin[19] = 8;
	m_CColor[20][0] = RGB(47, 47, 63);									// 図形演算ｼｪｰﾃﾞｨﾝｸﾞ色
	m_CColor[20][1] = RGB(63, 63, 63);
	m_CColor[20][2] = RGB(31, 31, 31);
	m_Shin[20] = 8;
	for(i=0; i<3; i++) {
		m_TexImage[i] = NULL, m_TexWidth[i] = 0, m_TexHeight[i] = 0;
	}
	InitRenderFile(0, FALSE);											// 初期ﾚﾝﾀﾞﾘﾝｸﾞﾌｧｲﾙ読込
	InitRColorFile(FALSE);												// 初期ﾚﾝﾀﾞﾘﾝｸﾞｶﾗｰﾌｧｲﾙ読込
//202006	InitRayTraceEnvFile();												// 初期ﾚｲ･ﾄﾚｰｽ環境ﾌｧｲﾙ読込
//202006	InitRayTraceLightFile();											// 初期ﾚｲ･ﾄﾚｰｽ光源ﾌｧｲﾙ読込
//202006	InitRayTraceColorFile();											// 初期ﾚｲ･ﾄﾚｰｽｶﾗｰﾌｧｲﾙ読込
	InitWorkTable();													// 作業ﾃｰﾌﾞﾙ初期処理
	InitData();															// ﾃﾞｰﾀ初期処理
	InitDialog();														// 常駐ﾀﾞｲｱﾛｸﾞ初期化
	SetCtrlKey();														// 制御キー設定
}

/***********************************/
CJcad3GlbView::~CJcad3GlbView(void)										//<<< ｸﾗｽ消滅
/***********************************/
{
	int i;

	WriteRenderFile();													// ﾚﾝﾀﾞﾘﾝｸﾞ保存ﾌｧｲﾙ書込
	WriteSysDataFile();													// ｼｽﾃﾑﾃﾞｰﾀ保存ﾌｧｲﾙ書込
	WriteScrInfoFile();													// 画面制御情報保存ﾌｧｲﾙ書込
	for(i=0; i<3; i++) {
		if(m_TexDisp[i]) {
			ClearTexImage(i);											// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ消去
		}
	}
	WriteRayTraceFile();												// ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ書込
	WritePovRayFile();													// POV-Ray保存ﾌｧｲﾙ書込
	FreeWorkTable();													// 作業ﾃｰﾌﾞﾙ解放処理
	if(m_pMainDC) {
		delete m_pMainDC;
	}
	m_pMainDC = NULL;
}

/*****************************************************/
BOOL CJcad3GlbView::PreCreateWindow(CREATESTRUCT& cs)					//<<< ｳｨﾝﾄﾞｳ作成前処理
/*****************************************************/
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを修正してください。
	cs.lpszClass = AfxRegisterWndClass(0,								// ﾀﾞﾌﾞﾙｸﾘｯｸは受け取らない
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_APPWORKSPACE),
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

/**************************************************/
BOOL CJcad3GlbView::PreTranslateMessage(MSG* pMsg)						//<<< ﾒｯｾｰｼﾞ変換
/**************************************************/
{
	int flg;

	if(pMsg->wParam>=VK_F5&&pMsg->wParam<=VK_F12) {
		if(pMsg->message==WM_KEYDOWN||pMsg->message==WM_SYSKEYDOWN) {	// ｷｰﾀﾞｳﾝの場合
			if(pMsg->wParam==VK_F5) {									// [F5]ｷｰ
				if(!(m_FPres&0x1)) {									// 初回押下時
					m_FChar = (m_FChar&0x1) ? m_FChar&~(0x1) : m_FChar|0x1;
					m_FPres = m_FPres|0x1;
					flg = (m_FChar&0x1) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(0, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(0, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			} else if(pMsg->wParam==VK_F6) {							// [F6]ｷｰ
				if(!(m_FPres&0x2)) {									// 初回押下時
					m_FChar = (m_FChar&0x2) ? m_FChar&~(0x2) : m_FChar|0x2;
					m_FPres = m_FPres|0x2;
					flg = (m_FChar&0x2) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(1, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(1, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			} else if(pMsg->wParam==VK_F7) {							// [F7]ｷｰ
				if(!(m_FPres&0x4)) {									// 初回押下時
					m_FChar = (m_FChar&0x4) ? m_FChar&~(0x4) : m_FChar|0x4;
					m_FPres = m_FPres|0x4;
					flg = (m_FChar&0x4) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(2, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(2, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			} else if(pMsg->wParam==VK_F8) {							// [F8]ｷｰ
				if(!(m_FPres&0x8)) {									// 初回押下時
					m_FChar = (m_FChar&0x8) ? m_FChar&~(0x8) : m_FChar|0x8;
					m_FPres = m_FPres|0x8;
					flg = (m_FChar&0x8) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(3, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(3, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			} else if(pMsg->wParam==VK_F9) {							// [F9]ｷｰ
				if(!(m_FPres&0x10)) {									// 初回押下時
					m_FChar = (m_FChar&0x10) ? m_FChar&~(0x10) : m_FChar|0x10;
					m_FPres = m_FPres|0x10;
					flg = (m_FChar&0x10) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(4, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(4, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			} else if(pMsg->wParam==VK_F10) {							// [F10]ｷｰ
				if(!(m_FPres&0x20)) {									// 初回押下時
					m_FChar = (m_FChar&0x20) ? m_FChar&~(0x20) : m_FChar|0x20;
					m_FPres = m_FPres|0x20;
					flg = (m_FChar&0x20) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(5, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(5, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			} else if(pMsg->wParam==VK_F11) {							// [F11]ｷｰ
				if(!(m_FPres&0x40)) {									// 初回押下時
					m_FChar = (m_FChar&0x40) ? m_FChar&~(0x40) : m_FChar|0x40;
					m_FPres = m_FPres|0x40;
					flg = (m_FChar&0x40) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(6, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(6, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			} else if(pMsg->wParam==VK_F12) {							// [F12]ｷｰ
				if(!(m_FPres&0x80)) {									// 初回押下時
					m_FChar = (m_FChar&0x80) ? m_FChar&~(0x80) : m_FChar|0x80;
					m_FPres = m_FPres|0x80;
					flg = (m_FChar&0x80) ? 1 : 0;
					if(m_SFuncMode<3) {									// <詳細>
						m_pFunction->OnOffButton(7, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
					} else if(m_SFuncMode<6) {							// <簡易>
						m_pFunction2->OnOffButton(7, flg);				// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
					}
				}
			}
		} else if(pMsg->message==WM_KEYUP||pMsg->message==WM_SYSKEYUP) {// ｷｰｱｯﾌﾟの場合
			if(pMsg->wParam==VK_F5) {									// [F5]ｷｰ
				m_FPres=m_FPres&~(0x1);
			} else if(pMsg->wParam==VK_F6) {							// [F6]ｷｰ
				m_FPres=m_FPres&~(0x2);
			} else if(pMsg->wParam==VK_F7) {							// [F7]ｷｰ
				m_FPres=m_FPres&~(0x4);
			} else if(pMsg->wParam==VK_F8) {							// [F8]ｷｰ
				m_FPres=m_FPres&~(0x8);
			} else if(pMsg->wParam==VK_F9) {							// [F9]ｷｰ
				m_FPres=m_FPres&~(0x10);
			} else if(pMsg->wParam==VK_F10) {							// [F10]ｷｰ
				m_FPres=m_FPres&~(0x20);
			} else if(pMsg->wParam==VK_F11) {							// [F11]ｷｰ
				m_FPres=m_FPres&~(0x40);
			} else if(pMsg->wParam==VK_F12) {							// [F12]ｷｰ
				m_FPres=m_FPres&~(0x80);
			}
		}
		return TRUE;
	}
	return CView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView 描画

/****************************************/
void CJcad3GlbView::OnDraw(CDC* /*pDC*/)								//<<< Viewｳｨﾝﾄﾞｳ描画
/****************************************/
{
    Display(0);															// 画面描画
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView 印刷

/********************************************************/
BOOL CJcad3GlbView::OnPreparePrinting(CPrintInfo* pInfo)				//<<< 印刷前処理
/********************************************************/
{
	if(GetDemoFlg()!=0) {												// ｢体験版,起動NG｣
		AfxMessageBox(IDS_TRIAL);
		return 0;
	}
	pInfo->m_bDirect = TRUE;											// ﾀﾞｲｱﾛｸﾞなし
	pInfo->SetMaxPage(1);												// 最終ﾍﾟｰｼﾞ設定
	DoPreparePrinting(pInfo);											// ﾃﾞﾌｫﾙﾄ印刷準備
	LPDEVMODE prtmode = pInfo->m_pPD->GetDevMode();						// ﾌﾟﾘﾝﾀ情報取得
	int prtori = (int)prtmode->dmOrientation-1;							// 現在の印刷の向き
	if(!m_PrtKind) {													// <<4面図印刷>>
		CPrtParamDlg dlg;
		dlg.SetValue(m_PrtMode, m_PrtColor, m_PrtOut, prtori,
                     m_PrtPos, m_PrtSize, m_PrtPath);					// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&m_PrtMode, &m_PrtColor, &m_PrtOut, &prtori,
                         &m_PrtPos, &m_PrtSize, &m_PrtPath,
                         &m_PrtTCheck, &m_PrtSCheck, &m_PrtFile);		// 印刷範囲入力
			Display(0);													// 画面描画
			Sleep(100);
			Display(0);													// 画面描画
			if(!m_PrtOut) {												// <ﾌﾟﾘﾝﾀ出力の場合>
				m_PrtSCheck = (m_PrtTCheck) ? 0 : m_PrtSCheck;			// ｻｲｽﾞ変更
				prtmode->dmOrientation = (short)(prtori+1);				// 印刷の向き
				return 1;
			} else {													// <ﾌｧｲﾙ出力の場合>
				m_PrtTCheck = 0;										// 実寸印刷なし
				m_PrtSCheck = 0;										// ｻｲｽﾞ変更なし
				DDBtoDIB(NULL);											// DDB=>DIB変換
				OutputFile();											// ﾌｧｲﾙ出力
				return 0;												// 印刷処理中止
			}
		} else {
			return 0;													// 中止=返り値:0
		}
	} else {															// <<ﾚﾝﾀﾞﾘﾝｸﾞ印刷>>
		CPrtParam2Dlg dlg;
		dlg.SetValue(m_PrtOut, prtori, m_PrtPos, m_PrtSize, m_PrtPath);	// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&m_PrtOut, &prtori, &m_PrtPos, &m_PrtSize,
                         &m_PrtPath, &m_PrtSCheck, &m_PrtFile);			// 印刷設定入力
			if(!m_PrtOut) {												// <ﾌﾟﾘﾝﾀ出力の場合>
				m_PrtTCheck = 0;										// 実寸印刷なし
				prtmode->dmOrientation = (short)(prtori+1);				// 印刷の向き
				return 1;
			} else {													// <ﾌｧｲﾙ出力の場合>
				m_PrtTCheck = 0;										// 実寸印刷なし
				m_PrtSCheck = 0;										// ｻｲｽﾞ変更なし
				m_pRendDlg->DDBtoDIB(NULL, m_PrtOut,
                                     m_PrtSCheck, m_PrtSize);			// DDB=>DIB変換
				m_pRendDlg->OutputFile(m_PrtFile);						// ﾌｧｲﾙ出力
				m_PrtKind = 0;
				return 0;												// 印刷処理中止
			}
		} else {
			m_PrtKind = 0;
			return 0;													// 中止=返り値:0
		}
	}    
}

/****************************************************************/
void CJcad3GlbView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)		//<<< 印刷開始処理
/****************************************************************/
{
	pDC->ResetDC(pInfo->m_pPD->GetDevMode());							// ﾌﾟﾘﾝﾀ情報の更新
	if(!m_PrtKind) {													// <<4面図印刷>>
		DDBtoDIB(pDC);													// DDB=>DIB変換
	} else {															// <<ﾚﾝﾀﾞﾘﾝｸﾞ印刷>>
		m_pRendDlg->DDBtoDIB(pDC, m_PrtOut, m_PrtSCheck, m_PrtSize);	// DDB=>DIB変換
	}
}

/************************************************************/
void CJcad3GlbView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)			//<<< Viewｳｨﾝﾄﾞｳ印刷
/************************************************************/
{
	if(!m_PrtKind) {													// <<4面図印刷>>
		OutputPrinter(pDC);												// ﾌﾟﾘﾝﾀ出力
	} else {															// <<ﾚﾝﾀﾞﾘﾝｸﾞ印刷>>
		m_pRendDlg->OutputPrinter(pDC, m_PrtPos);						// ﾌﾟﾘﾝﾀ出力
	}
}

/**********************************************************************/
void CJcad3GlbView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)	//<<< 印刷終了処理
/**********************************************************************/
{
	if(!m_PrtKind) {													// <<4面図印刷>>
		::GlobalFree((HGLOBAL)m_hDIB);
	} else {															// <<ﾚﾝﾀﾞﾘﾝｸﾞ印刷>>
		m_pRendDlg->FreeDIB();
	}
	m_PrtKind = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView 診断

#ifdef _DEBUG
void CJcad3GlbView::AssertValid() const
{
	CView::AssertValid();
}

void CJcad3GlbView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView メッセージ ハンドラ

/**********************************************************/
int CJcad3GlbView::OnCreate(LPCREATESTRUCT lpCreateStruct)				//<<< WM_CREATEﾒｯｾｰｼﾞ応答処理
/**********************************************************/
{
	double div1, div2;

	if(CView::OnCreate(lpCreateStruct)==-1) {
		return -1;
	}
	if(!InitGL()) {														// 初期化
		return -1;
	}
	CreateDialogs();													// 常駐ﾀﾞｲｱﾛｸﾞ生成
	div1 = ScrDiv1;														// 画面比率保存
	div2 = ScrDiv2;
	ScrDiv1 = 0.5;														// 4面図用画面比率
	ScrDiv2 = 0.5;
	InitDLSetup = 0;													// OpenGL DL 初期化:未
	ViewSize = CSize(1024, 768);										// 仮Viewｻｲｽﾞ設定
	PlSize = (int)(768/2.0-1);											// 仮XY平面ｻｲｽﾞ設定
	InitScreen(1024, 768);												// 仮ｽｸﾘｰﾝ初期処理
	ScrDiv1 = div1;														// 画面比率復元
	ScrDiv2 = div2;
	Init();																// 初期処理
	return 0;
}

/***********************************/
void CJcad3GlbView::OnDestroy(void)										//<<< WM_DESTROYﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	EndDialogs();														// 常駐ﾀﾞｲｱﾛｸﾞ終了
	::wglMakeCurrent(NULL, NULL);
	::wglDeleteContext(m_hRC);
	CView::OnDestroy();
}

/******************************************************/
void CJcad3GlbView::OnSize(UINT nType, int cx, int cy)					//<<< WM_SIZEﾒｯｾｰｼﾞ応答処理
/******************************************************/
{
	CView::OnSize(nType, cx, cy);
	ViewSize = CSize(cx, cy);											// Viewｻｲｽﾞ設定
	PlSize = (int)(cy/2.0-1);											// XY平面ｻｲｽﾞ設定
	InitScreen(cx, cy);													// ｽｸﾘｰﾝ初期処理
}

/************************************************************************/
BOOL CJcad3GlbView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)//<<< WM_SETCURSORﾒｯｾｰｼﾞ応答処理
/************************************************************************/
{
	POINT point;
	int   ret;

	GetCursorPos(&point);												// ﾏｳｽｽｸﾘｰﾝ座標取得
	ScreenToClient(&point);												// ｸﾗｲｱﾝﾄ座標変換
	if(m_CursorNo!=0) {													// <標準ｶｰｿﾙ以外>
		if(PlRect[XY].PtInRect(point)||
		   PlRect[YZ].PtInRect(point)||
		   PlRect[ZX].PtInRect(point)) {								// <3面図内>
			if(m_CursorNo==1) {											// [十字ｶｰｿﾙ]
				SetCursor(m_hcursor1);
			} else if(m_CursorNo==2) {									// [手ｶｰｿﾙ]
				SetCursor(m_hcursor2);
			} else {													// [ｽﾞｰﾑｶｰｿﾙ]
				SetCursor(m_hcursor3);
			}
			return TRUE;
		}
	}
	ret = CheckScrChg(point);											// ｽｸﾘｰﾝ変更確認
	if(ret==1) {														// <XY-PRS,YZ-ZXの境界>
		SetCursor(m_hcursor4);											// [境界横移動ｶｰｿﾙ]
		return TRUE;
	} else if(ret==2) {													// <XY-YZ, PRS-ZXの境界>
		SetCursor(m_hcursor5);											// [境界縦移動ｶｰｿﾙ]
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

/**********************************************************/
void CJcad3GlbView::OnMouseMove(UINT nFlags, CPoint point)				//<<< WM_MOUSEMOVEﾒｯｾｰｼﾞ応答処理
/**********************************************************/
{
	if(m_ScrChg!=0) {													// <ｽｸﾘｰﾝ変更中>
		ChangeScreen(m_ScrChg, point);									// ｽｸﾘｰﾝ変更処理
	} else {															// <ｽｸﾘｰﾝ変更中以外>
		CurPos = point;
		if(!m_MoveInit) {
			MenuCall(m_ActMenu, nFlags, point, Plane, 2);				// ﾒﾆｭｰ呼び出し
			CalcScreenWorld(0, &Cursor, point, HitTestPlane(point));	// 座標点取得
			MainWnd->SetCurPos(Cursor.p[0], Cursor.p[1], Cursor.p[2]);
		}
		m_MoveInit = 0;
	}
	CView::OnMouseMove(nFlags, point);
}

/***********************************************************/
BOOL CJcad3GlbView::OnMouseWheel(UINT nFlags, short zDelta,
                                 CPoint point)							//<<< WM_MOUSEHWHEELﾒｯｾｰｼﾞ応答処理
/***********************************************************/
{
	OBJTYPE* op;
	PNTTYPE  ept, cpt;
	BOXTYPE  box;
	VECTYPE  vec;
	SCRTYPE  len, pos;
	int      i, ln, sa, plane, dir, dsp, edt;
	double   th;

	if(m_ScrChg==0) {													// <<<ｽｸﾘｰﾝ変更中以外>>>
		if(zDelta!=0) {
			plane = HitTestPlane(CurPos);								// ｶｰｿﾙ位置の平面
			if(plane==PRS) {											// <<透視図の場合>>
				if(nFlags&MK_CONTROL||nFlags&MK_SHIFT) {				// <Ctrl/Shiftｷｰ押下時>
					dir = (nFlags&MK_CONTROL) ? 0 : 1;					// 回転方向
					th = ((zDelta>0)?1:-1)*5.0*PI/180.0;				// 角度を算出
					EyeChange(dir, th);									// 視点変更
				} else {												// <ｷｰ押下なし>
					ScrToPnt(0, &m_EyeSv, &ept);						// SCRTYPE => PNTTYPE
					ScrToPnt(0, &m_CentPs, &cpt);						// SCRTYPE => PNTTYPE
					VecV(&ept, &cpt, &vec);								// 開始視線ﾍﾞｸﾄﾙ取得
					VecN(&vec);
					for(i=0; i<DIM; i++) {
						ln = (int)(vec.p[i]*m_TruckLen*500.0);			// 誤差の補正
						sa = ((vec.p[i]*m_TruckLen*500.0)-ln>=0.5) ? 1 : 0;
						sa = ((vec.p[i]*m_TruckLen*500.0)-ln<=-0.5) ? -1 : sa;
						len.p[i] = ln+sa;								// 視点の移動距離算出
					}
					if(zDelta>0) {										// <拡大表示>
						for(i=0; i<DIM; i++) {
							m_Eye.p[i] = m_Eye.p[i]+len.p[i];			// 視点を移動
						}
						if(m_Eye.p[0]==m_Cent3D.p[0]&&
                           m_Eye.p[1]==m_Cent3D.p[1]&&
                           m_Eye.p[2]==m_Cent3D.p[2]) {
							for(i=0; i<DIM; i++) {
								m_Eye.p[i] = m_Eye.p[i]+len.p[i];
							}
						}
					} else {											// <縮小表示>
						for(i=0; i<DIM; i++) {
							m_Eye.p[i] = m_Eye.p[i]-len.p[i];			// 視点を移動
						}
						if(m_Eye.p[0]==m_Cent3D.p[0]&&
                           m_Eye.p[1]==m_Cent3D.p[1]&&
                           m_Eye.p[2]==m_Cent3D.p[2]) {
							for(i=0; i<DIM; i++) {
								m_Eye.p[i] = m_Eye.p[i]-len.p[i];
							}
						}
					}
					ScrToPnt(0, &m_Eye, &ept);							// SCRTYPE => PNTTYPE
					m_pState->SetEye(ept);
					SetPerspective();									// 透視法射影行列の再構築
					SetXYZGuide();										// XYZｶﾞｲﾄﾞ設定
					Display(2);											// 再描画
				}
			} else {													// <<3面図の場合>>
				if(nFlags&MK_CONTROL||nFlags&MK_SHIFT) {				// <Ctrl/Shiftｷｰ押下時>
					plane = (nFlags&MK_SHIFT) ? plane+1 : plane;		// Shiftｷｰ押下は交差軸方向
					plane = (plane>2) ? 0 : plane;
					dir = (zDelta>0) ? 1 : -1;							// 正負の方向を算出
					Scroll(plane, dir);									// 画面ｽｸﾛｰﾙ
				} else {												// <ｷｰ押下なし>
					m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;			// 3面図表示状態保存
					if(m_Char=='Z') {									// <[Z]ｷｰ押下>
						BaseObjPtr(1, &op, &dsp, &edt);
						if(op) {										// <表示立体あり>
							GetAreaBox(1, &box);						// 全立体のBOXの最大値取得
							if(m_DAraFlg) {								// <表示範囲指定の場合>
								for(i=0; i<DIM; i++) {
									if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
										box.bp[0].p[i] = m_DAra.bp[0].p[i];
									}
									if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
										box.bp[1].p[i] = m_DAra.bp[1].p[i];
									}
								}
							}
							BoxCenter(box, &cpt);						// ﾎﾞｯｸｽｾﾝﾀｰを3面中心
							ScrToPnt(1, &m_Cent3D, &cpt);				// PNTTYPE => SCRTYPE
							ScrToPnt(0, &m_Cent3D, &cpt);				// SCRTYPE => PNTTYPE
							m_pState->SetCent3D(cpt);
						}
					} else if(m_Char=='C') {							// <[C]ｷｰ押下>
						ScrToPnt(1, &pos, &Cursor);						// PNTTYPE => SCRTYPE
						for(i=0; i<DIM; i++) {
							len.p[i] = pos.p[i]-m_Cent3D.p[i];			// ｶｰｿﾙ位置と中心点の差算出
						}
						if(zDelta>0) {									// <拡大表示>
							if(m_Fovy>200) {							// 表示高が規定値(μm)以上
								for(i=0; i<DIM; i++) {					// ｽﾞｰﾑ後を正式に算出
									ln = (len.p[i]>0) ? (int)(len.p[i]*0.95+0.5)
                                                      : (int)(len.p[i]*0.95-0.5);
									m_Cent3D.p[i] += (len.p[i]-ln);		// ｽﾞｰﾑ後の差を画面中心点に加算
								}
							}
						} else {										// <縮小表示>
							if(m_Fovy<1000000) {						// 表示高が規定値(μm)以下
								for(i=0; i<DIM; i++) {					// ｽﾞｰﾑ後を正式に算出
									ln = (len.p[i]>0) ? (int)(len.p[i]/0.95+0.5)
                                                      : (int)(len.p[i]/0.95-0.5);
									m_Cent3D.p[i] += (len.p[i]-ln);		// ｽﾞｰﾑ後の差を画面中心点に加算
								}
							}
						}
						ScrToPnt(0, &m_Cent3D, &cpt);					// SCRTYPE => PNTTYPE
						m_pState->SetCent3D(cpt);
					}
					if(zDelta>0) {										// <拡大表示>
						m_Fovy = (m_Fovy>200) ? (int)(m_Fovy*0.95+0.5)	// 表示高が規定値(μm)以上
                                              : m_Fovy;
					} else {											// <縮小表示>
						m_Fovy = (m_Fovy<1000000) ? (int)(m_Fovy/0.95+0.5)
                                                  : m_Fovy;
					}
					SetSeishaei(m_Cent3D, m_Fovy);						// 正射影行列の再構築
					ChangeTexPixels();									// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
					if(m_GridDisp) {
						SetGrid();										// ｸﾞﾘｯﾄﾞ設定
					}
					Display(1);											// 再描画
				}
			}
		}
	}
	return CView::OnMouseWheel(nFlags, zDelta, point);
}

/************************************************************/
void CJcad3GlbView::OnLButtonDown(UINT nFlags, CPoint point)			//<<< WM_LBUTTONDOWNﾒｯｾｰｼﾞ応答処理
/************************************************************/
{
	SetCapture();														// ﾏｳｽｷｬﾌﾟﾁｬ
	BasePoint = point;
	CurPos = point;
	Plane = HitTestPlane(point);
	CalcScreenWorld(0, &BaseCursor, point, Plane);						// 座標点取得
	if((m_ScrChg=CheckScrChg(point))==0) {								// ｽｸﾘｰﾝ変更確認
		if(m_Char==0||m_Char=='M') {
			m_MoveInit = 1;
			MenuCall(m_ActMenu, nFlags, point, Plane, 1);				// ﾒﾆｭｰ呼び出し
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

/**********************************************************/
void CJcad3GlbView::OnLButtonUp(UINT nFlags, CPoint point)				//<<< WM_LBUTTONUPﾒｯｾｰｼﾞ応答処理
/**********************************************************/
{
	OBJTYPE* op;
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, dsp, edt, pflg;

	ReleaseCapture();
	if(m_ScrChg!=0) {													// <ｽｸﾘｰﾝ変更中>
		ChangeScreen(m_ScrChg, point);									// ｽｸﾘｰﾝ変更処理
		m_ScrChg = 0;
	} else {															// <ｽｸﾘｰﾝ変更中以外>
		if(Plane==PRS) {												// <<透視図の場合>>
			if(m_Char=='C') {											// <[C]ｷｰ押下>
				CentPars();												// 参照点指定(透視図)
			} else if(m_Char=='S') {									// <[S]ｷｰ押下>
				SetScreenNo(1);											// 表示範囲設定(透視図)
				Display(0);												// 再表示
			} else if(m_Char=='Q') {									// <[Q]ｷｰ押下>
				SetScreenNo(0);											// 表示範囲設定(全面図)
				Display(0);												// 再表示
			} else if(m_Char=='X'||m_Char=='Y'||m_Char=='Z'||
                      m_Char=='U'||m_Char=='V'||m_Char=='W') {			// <[X,Y,Z,U,V,W]ｷｰ押下>
				if(m_Char=='X') {										// [X]ｷｰ押下はXY平面(表)
					pflg = 0;
				} else if(m_Char=='Y') {								// [Y]ｷｰ押下はYZ平面(表)
					pflg = 1;
				} else if(m_Char=='Z') {								// [Z]ｷｰ押下はZX平面(表)
					pflg = 2;
				} else if(m_Char=='U') {								// [U]ｷｰ押下はXY平面(裏)
					pflg = 3;
				} else if(m_Char=='V') {								// [V]ｷｰ押下はYZ平面(裏)
					pflg = 4;
				} else if(m_Char=='W') {								// [W]ｷｰ押下はZX平面(裏)
					pflg = 5;
				}
				EyeSet(pflg);											// 透視図の変更
			}
		} else {														// <<3面図の場合>>
			if(m_Char=='Z') {											// <[Z]ｷｰ押下>
				BaseObjPtr(1, &op, &dsp, &edt);
				if(op) {												// <表示立体あり>
					m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;			// 3面図表示状態保存
					GetAreaBox(1, &box);								// 全立体のBOXの最大値取得
					if(m_DAraFlg) {										// <表示範囲指定の場合>
						for(i=0; i<DIM; i++) {
							if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
								box.bp[0].p[i] = m_DAra.bp[0].p[i];
							}
							if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
								box.bp[1].p[i] = m_DAra.bp[1].p[i];
							}
						}
					}
					BoxCenter(box, &cpt);								// ﾎﾞｯｸｽｾﾝﾀｰを3面中心
					ScrToPnt(1, &m_Cent3D, &cpt);						// PNTTYPE => SCRTYPE
					ScrToPnt(0, &m_Cent3D, &cpt);						// SCRTYPE => PNTTYPE
					m_pState->SetCent3D(cpt);
					SetSeishaei(m_Cent3D, m_Fovy);						// 正射影行列の再構築
					ChangeTexPixels();									// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
					if(m_GridDisp) {
						SetGrid();										// ｸﾞﾘｯﾄﾞ設定
					}
					Display(1);											// 再描画
				}
			} else if(m_Char=='C') {									// <[C]ｷｰ押下>
				CalcScreenWorld(0, &Cursor, point, HitTestPlane(point));// 座標点取得
				MainWnd->SetCurPos(Cursor.p[0], Cursor.p[1], Cursor.p[2]);
				m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;				// 3面図表示状態保存
				ScrToPnt(1, &m_Cent3D, &Cursor);						// PNTTYPE => SCRTYPE
				ScrToPnt(0, &m_Cent3D, &Cursor);						// SCRTYPE => PNTTYPE
				m_pState->SetCent3D(Cursor);
				SetSeishaei(m_Cent3D, m_Fovy);							// 正射影行列の再構築
				ChangeTexPixels();										// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
				if(m_GridDisp) {
					SetGrid();											// ｸﾞﾘｯﾄﾞ設定
				}
				Display(1);												// 再描画
			} else if(m_Char=='A') {									// <[A]ｷｰ押下>
				AutoZoom();												// 自動倍率
			} else if(m_Char=='X') {									// <[X]ｷｰ押下>
				ExactZoom();											// 実寸表示
			} else if(m_Char=='S') {									// <[S]ｷｰ押下>
				pflg = Plane+2;
				SetScreenNo(pflg);										// 表示範囲設定(3面図)
				Display(0);												// 再表示
			} else if(m_Char=='Q') {									// <[Q]ｷｰ押下>
				SetScreenNo(0);											// 表示範囲設定(全面図)
				Display(0);												// 再表示
			} else if(m_Char=='0') {									// <[0]ｷｰ押下>
				SetDispMode(0);											// 表示ﾓｰﾄﾞ設定(設定なし)
				Display(0);												// 再表示
			} else if(m_Char=='1') {									// <[1]ｷｰ押下>
				SetDispMode(1);											// 表示ﾓｰﾄﾞ設定(ﾓｰﾄﾞ1)
				Display(0);												// 再表示
			} else if(m_Char=='2') {									// <[2]ｷｰ押下>
				SetDispMode(2);											// 表示ﾓｰﾄﾞ設定(ﾓｰﾄﾞ2)
				Display(0);												// 再表示
			} else if(m_Char=='3') {									// <[3]ｷｰ押下>
				SetDispMode(3);											// 表示ﾓｰﾄﾞ設定(ﾓｰﾄﾞ3)
				Display(0);												// 再表示
			} else if(m_Char=='4') {									// <[4]ｷｰ押下>
				SetDispMode(4);											// 表示ﾓｰﾄﾞ設定(透視図ｼｪｰﾃﾞｨﾝｸﾞ)
				Display(0);												// 再表示
			} else if(m_Char=='5') {									// <[5]ｷｰ押下>
				SetDispMode(5);											// 表示ﾓｰﾄﾞ設定(全面ｼｪｰﾃﾞｨﾝｸﾞ)
				Display(0);												// 再表示
			}
		}
		if(m_Char==0||m_Char=='M') {
			MenuCall(m_ActMenu, nFlags, point, Plane, 3);				// ﾒﾆｭｰ呼び出し
		}
	}
	CView::OnLButtonUp(nFlags, point);
}

/************************************************************/
void CJcad3GlbView::OnRButtonDown(UINT nFlags, CPoint point)			//<<< WM_RBUTTONDOWNﾒｯｾｰｼﾞ応答処理
/************************************************************/
{
	MenuCall(m_ActMenu, nFlags, point, HitTestPlane(point), 4);			// ﾒﾆｭｰ呼び出し
	CView::OnRButtonDown(nFlags, point);
}

/*****************************************************/
void CJcad3GlbView::OnKeyDown(UINT nChar, UINT, UINT)					//<<< WM_KEYDOWNﾒｯｾｰｼﾞ応答処理
/*****************************************************/
{
	int range, mode;

	if(nChar==VK_DELETE) {												// [Delete]ｷｰ押下
		MenuCall(m_ActMenu, 0, 0, 0, 11);
	} else if(nChar==VK_SHIFT) {										// [Shift]ｷｰ押下
		m_CChar = m_CChar|0x1;
		if(m_SFuncMode<3) {												// <詳細>
			m_pFunction->OnOffButton(10, 1);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
		} else if(m_SFuncMode<6) {										// <簡易>
			m_pFunction2->OnOffButton(10, 1);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
		}
	} else if(nChar==VK_CONTROL) {										// [Ctrl]ｷｰ押下
		m_CChar = m_CChar|0x2;
		if(m_SFuncMode<3) {												// <詳細>
			m_pFunction->OnOffButton(11, 1);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
		} else if(m_SFuncMode<6) {										// <簡易>
			m_pFunction2->OnOffButton(11, 1);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
		}
	} else if(nChar==VK_SPACE) {										// [SPACE]ｷｰ押下
		m_CChar = m_CChar|0x4;
	} else if(nChar==VK_LEFT) {											// [←]ｷｰ押下
		m_DChar = m_DChar|0x1;
	} else if(nChar==VK_UP) {											// [↑]ｷｰ押下
		m_DChar = m_DChar|0x2;
	} else if(nChar==VK_RIGHT) {										// [→]ｷｰ押下
		m_DChar = m_DChar|0x4;
	} else if(nChar==VK_DOWN) {											// [↓]ｷｰ押下
		m_DChar = m_DChar|0x8;
	} else if(nChar>='A'&&nChar<='Z') {									// [ｱﾙﾌｧﾍﾞｯﾄ]ｷｰ押下
		m_Char = nChar;
		MenuCall(m_ActMenu, nChar, 0, 0, 5);							// ﾒﾆｭｰ呼び出し
	} else if(nChar>='0'&&nChar<='9') {									// [数値]ｷｰ押下
		m_Char = nChar;
	} else if(nChar>=VK_NUMPAD0&&nChar<=VK_NUMPAD9) {					// [ﾃﾝ]ｷｰ押下
		m_Char = nChar - (VK_NUMPAD0-'0');
    }
	if((m_CChar&0x1)&&(m_CChar&0x2)) {									// [Shift]+[Ctrl]ｷｰ押下で
		if(m_Char=='A'||m_Char=='S'||m_Char=='Z'||						// [A,S,Z,X,C]ｷｰ押下時
           m_Char=='X'||m_Char=='C') {
			if(m_Char=='A') {											// [A]ｷｰ押下は全平面
				range = 0;
			} else if(m_Char=='S') {									// [S]ｷｰ押下は透視図
				range = 1;
			} else if(m_Char=='Z') {									// [Z]ｷｰ押下はXY平面
				range = 2;
			} else if(m_Char=='X') {									// [X]ｷｰ押下はYZ平面
				range = 3;
			} else if(m_Char=='C') {									// [C]ｷｰ押下はZX平面
				range = 4;
			}
			SetScreenNo(range);											// 表示範囲設定
			Display(0);													// 再表示
		} else if(m_Char=='0'||m_Char=='1'||m_Char=='2'||				// [Shift]+[Ctrl]ｷｰ押下で
                  m_Char=='3'||m_Char=='4'||m_Char=='5') {				// [0,1,2,3,4,5]ｷｰ押下時
			if(m_Char=='0') {											// [0]ｷｰ押下は設定なし
				mode = 0;
			} else if(m_Char=='1') {									// [1]ｷｰ押下はﾓｰﾄﾞ1
				mode = 1;
			} else if(m_Char=='2') {									// [2]ｷｰ押下はﾓｰﾄﾞ2
				mode = 2;
			} else if(m_Char=='3') {									// [3]ｷｰ押下はﾓｰﾄﾞ3
				mode = 3;
			} else if(m_Char=='4') {									// [4]ｷｰ押下は透視図ｼｪｰﾃﾞｨﾝｸﾞ
				mode = 4;
			} else if(m_Char=='5') {									// [5]ｷｰ押下は全面ｼｪｰﾃﾞｨﾝｸﾞ
				mode = 5;
			}
			SetDispMode(mode);											// 表示ﾓｰﾄﾞ設定
			Display(0);													// 再表示
		}
	}
}

/**************************************************/
void CJcad3GlbView::OnChar(UINT nChar, UINT, UINT)						//<<< WM_CHARﾒｯｾｰｼﾞ応答処理
/**************************************************/
{
	m_Char = nChar;
	if(m_Char>='a'&&m_Char<='z') {										// 英小文字=>英大文字
		m_Char += ('A'-'a');
	}
}

/***************************************************/
void CJcad3GlbView::OnKeyUp(UINT nChar, UINT, UINT)						//<<< WM_KEYUPﾒｯｾｰｼﾞ応答処理
/***************************************************/
{
	if(nChar==VK_SHIFT) {												// [Shift]ｷｰ
		m_CChar = m_CChar&~(0x1);
		if(m_SFuncMode<3) {												// <詳細>
			m_pFunction->OnOffButton(10, 0);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
		} else if(m_SFuncMode<6) {										// <簡易>
			m_pFunction2->OnOffButton(10, 0);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
		}
	} else if(nChar==VK_CONTROL) {										// [Ctrl]ｷｰ
		m_CChar = m_CChar&~(0x2);
		if(m_SFuncMode<3) {												// <詳細>
			m_pFunction->OnOffButton(11, 0);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
		} else if(m_SFuncMode<6) {										// <簡易>
			m_pFunction2->OnOffButton(11, 0);							// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
		}
	} else if(nChar==VK_SPACE) {										// [SPACE]ｷｰ
		m_CChar = m_CChar&~(0x4);
	} else if(nChar==VK_LEFT) {											// [←]ｷｰ押下
		m_DChar = m_DChar&~(0x1);
	} else if(nChar==VK_UP) {											// [↑]ｷｰ押下
		m_DChar = m_DChar&~(0x2);
	} else if(nChar==VK_RIGHT) {										// [→]ｷｰ押下
		m_DChar = m_DChar&~(0x4);
	} else if(nChar==VK_DOWN) {											// [↓]ｷｰ押下
		m_DChar = m_DChar&~(0x8);
	} else {															// 以外のｷｰ
		m_Char = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// インプリメンテーション

/********************************/
BOOL CJcad3GlbView::InitGL(void)										//<<< 初期化(OpenGL)
/********************************/
{
	HDC     hdc;
	CString ext;
	int     i, index, form;
	double  rc, gc, bc;
	BOOL    ret;

	m_pMainDC = new CClientDC(this);									// ﾃﾞﾊﾞｲｽ･ｺﾝﾃｷｽﾄ取得
	hdc = m_pMainDC->GetSafeHdc();
	if(!SetupPixelFormat(hdc, &m_PFD)) {
		return FALSE;
	}
	index = ::GetPixelFormat(hdc);
	form = ::DescribePixelFormat(hdc, index, sizeof(PIXELFORMATDESCRIPTOR), &m_PFD);
	if(form<1) {
		return FALSE;
	}
	m_hRC = ::wglCreateContext(hdc);									// ﾚﾝﾀﾞﾘﾝｸﾞ･ｺﾝﾃｷｽﾄ取得
	::wglMakeCurrent(hdc, m_hRC);
	MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);				// 色取得
	glClearColor((float)rc, (float)gc, (float)bc, 0.0f); // 1.0f);		// ｶﾗｰﾊﾞｯﾌｧ初期化値設定
	glClearDepth(1.0);													// ﾃﾞﾌﾟｽﾊﾞｯﾌｧ初期化値設定
	glClearStencil(0x0);												// ｽﾃﾝｼﾙﾊﾞｯﾌｧ初期値設定
	glDepthFunc(GL_LESS);
	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	for(i=0; i<21; i++) {												// OpenGLｶﾗｰ設定
		SetGLColor(i);
	}
	SetGLRender();														// OpenGLｶﾗｰ設定
	if(!SetGLRenderMap()) {												// OpenGLﾏｯﾌﾟ設定
		return FALSE;
	}
	SetRenderMode(0);
	for(i=0; i<3; i++) {
		if(m_TexDisp[i]) {
			ext = m_TexFile[i].Right(3);
			if(!ext.CompareNoCase("bmp")) {
				ret = SetTexImageBMP(i, 0);								// ﾃｸｽﾁｬ用BMP画像ﾃﾞｰﾀ設定
			} else {
				ret = SetTexImageJPG(i);								// ﾃｸｽﾁｬ用JPG画像ﾃﾞｰﾀ設定
			}
			if(ret) {													// <<ﾃｸｽﾁｬﾃﾞｰﾀ設定OK>>
				if(m_TexRotate[i]==1) {									// <右90度回転>
					RotateTexImage(1, i);								// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ回転
				} else if(m_TexRotate[i]==2) {							// <180度回転>
					RotateTexImage(1, i);								// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ回転
					RotateTexImage(1, i);								// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ回転
				} else if(m_TexRotate[i]==3) {							// <左90度回転>
					RotateTexImage(0, i);								// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ回転
				}
			}
		}
	}
	glNewList(DL_XY, GL_COMPILE);										// <XY平面ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ>
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0);
	glEndList();
	glNewList(DL_YZ, GL_COMPILE);										// <YZ平面ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ>
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glEndList();
	glNewList(DL_ZX, GL_COMPILE);										// <ZX平面ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ>
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glEndList();
	return TRUE;
}

/**************************************************************************/
BOOL CJcad3GlbView::SetupPixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR* pPFD)
/**************************************************************************/
{
	int pixelformat;

	pPFD->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pPFD->nVersion = 1;
	pPFD->dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;//|PFD_SWAP_EXCHANGE;
	pPFD->iPixelType = PFD_TYPE_RGBA;
	pPFD->cColorBits = 24;
	pPFD->cRedBits = 0;
	pPFD->cRedShift = 0;
	pPFD->cGreenBits = 0;
	pPFD->cGreenShift = 0;
	pPFD->cBlueBits = 0;
	pPFD->cBlueShift = 0;
	pPFD->cAlphaBits = 0;
	pPFD->cAlphaShift = 0;
	pPFD->cAccumBits = 0;
	pPFD->cAccumRedBits = 0;
	pPFD->cAccumGreenBits = 0;
	pPFD->cAccumBlueBits = 0;
	pPFD->cAccumAlphaBits = 0;
	pPFD->cDepthBits = 32;			//64;
	pPFD->cStencilBits = 2;			//0;
	pPFD->cAuxBuffers = 0;
	pPFD->iLayerType = PFD_MAIN_PLANE;
	pPFD->bReserved = 0;
	pPFD->dwLayerMask = 0;
	pPFD->dwVisibleMask = 0;
	pPFD->dwDamageMask = 0;
	if((pixelformat=::ChoosePixelFormat(hdc, pPFD))==0) {
		return FALSE;
	}
	if(!::SetPixelFormat(hdc, pixelformat, pPFD)) {
		return FALSE;
	}
	return TRUE;
}

/**************************************/
void CJcad3GlbView::SetGLColor(int ix)									//<<< OpenGLｶﾗｰ設定
/**************************************/
{
	UINT    id;
	GLfloat ambi[4], spec[4], emis[4];

	ambi[3] = 1.0f, spec[3] = 1.0f, emis[3] = 1.0f;
	ambi[0] = GetRValue(m_CColor[ix][0])/255.0f;
	ambi[1] = GetGValue(m_CColor[ix][0])/255.0f;
	ambi[2] = GetBValue(m_CColor[ix][0])/255.0f;
	spec[0] = GetRValue(m_CColor[ix][1])/255.0f;
	spec[1] = GetGValue(m_CColor[ix][1])/255.0f;
	spec[2] = GetBValue(m_CColor[ix][1])/255.0f;
	emis[0] = GetRValue(m_CColor[ix][2])/255.0f;
	emis[1] = GetGValue(m_CColor[ix][2])/255.0f;
	emis[2] = GetBValue(m_CColor[ix][2])/255.0f;
	id = DL_GOLD + ix;													// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄNo
	glNewList(id, GL_COMPILE);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambi);			// 環境光&拡散光
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);						// 鏡面光
		glMaterialf(GL_FRONT, GL_SHININESS, (float)m_Shin[ix]);			// 鏡面指数
		glMaterialfv(GL_FRONT, GL_EMISSION, emis);						// 放射光
	glEndList();
}

/*************************************/
void CJcad3GlbView::SetGLRender(void)									//<<< OpenGLｶﾗｰ設定
/*************************************/
{
	GLfloat ambi[4], diff[4], spec[4], ligh[4];

	ambi[0] = GetRValue(m_RColor[0])/255.0f;
	ambi[1] = GetGValue(m_RColor[0])/255.0f;
	ambi[2] = GetBValue(m_RColor[0])/255.0f;
	diff[0] = GetRValue(m_RColor[1])/255.0f;
	diff[1] = GetGValue(m_RColor[1])/255.0f;
	diff[2] = GetBValue(m_RColor[1])/255.0f;
	spec[0] = GetRValue(m_RColor[2])/255.0f;
	spec[1] = GetGValue(m_RColor[2])/255.0f;
	spec[2] = GetBValue(m_RColor[2])/255.0f;
	ambi[3] = 1.0f, diff[3] = 1.0f, spec[3] = 1.0f;
	ligh[0] = 1.0f, ligh[1] = 1.0f, ligh[2] = 1.0f, ligh[3] = 0.0f;
	glNewList(DL_LIGHT, GL_COMPILE_AND_EXECUTE);						// 光源ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);							// 環境光
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);							// 拡散光
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);						// 鏡面光
		glLightfv(GL_LIGHT0, GL_POSITION, ligh);						// 光源位置
		glEnable(GL_LIGHT0);											// 光源0使用可
	glEndList();
}

/****************************************/
BOOL CJcad3GlbView::SetGLRenderMap(void)								//<<< OpenGLﾏｯﾌﾟ設定
/****************************************/
{
	WIN32_FIND_DATA wfd;
	FILE* fp;
	int   i, j, w, h;
	BOOL  ret;

	if(FindFirstFile(m_MapFile, &wfd)==INVALID_HANDLE_VALUE) {			// ﾌｧｲﾙ無の場合
		m_MapFile = GetAppPath(0)+MAP_NAME;								// 初期ﾌｧｲﾙ名使用
	}
	if(fopen_s(&fp, m_MapFile, "rb")!=0) {
		SetSplashMsg(IDS_NO_MAPPING);									// 環境ﾏｯﾋﾟﾝｸﾞ無:中止
		return FALSE;
	}
	int* hd = new int[54];
	for(i=0; i<54; i++) {
		hd[i] = fgetc(fp);
	}
	w = hd[18]+hd[19]*256+hd[20]*256*256+hd[21]*256*256*256;			// 画像の横ﾋﾟｸｾﾙ数
	h = hd[22]+hd[23]*256+hd[24]*256*256+hd[25]*256*256*256;			// 画像の縦ﾋﾟｸｾﾙ数
	if(w!=HBMP||h!=VBMP) {												// <幅:512,高:256以外>
		AfxMessageBox(IDS_ERR_MAPFILE);									// 画像ﾌｧｲﾙｴﾗｰ
		for(i=0; i<VBMP; i++) {
			for(j=0; j<HBMP; j++) {
				TexImage[i][j][2] = 0xff;
				TexImage[i][j][1] = 0xff;
				TexImage[i][j][0] = 0xff;
			}
		}
		ret = FALSE;
	} else {															// <正常ﾃﾞｰﾀ>
		for(i=0; i<VBMP; i++) {
			for(j=0; j<HBMP; j++) {
				TexImage[i][j][2] = (UCHAR)fgetc(fp);
				TexImage[i][j][1] = (UCHAR)fgetc(fp);
				TexImage[i][j][0] = (UCHAR)fgetc(fp);
			}
		}
		ret = TRUE;
	}
	fclose(fp);
	delete[] hd;
	return ret;
}

/**************************************/
void CJcad3GlbView::DDBtoDIB(CDC* pDC)									//<<< DDB=>DIB変換
/**************************************/
{
	UINT*   col = new UINT[20];
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙ表示
	CWnd*	m_pwnd;
	CDC*    m_pcdc;
	CDC     m_scdc;
	CBitmap m_bitmap;
	CRect   drct, crct;
	int     fx, fy, fw, fh, tw, th, px, py, esw=0;
	double  rc, gc, bc;
	BITMAP  bm;
	DWORD   dwLen;
	WORD    biBits;
	HDC     hdc;
	BITMAPINFOHEADER   bi;
	LPBITMAPINFOHEADER lpbi;

	m_hDIB = NULL;														// DIBﾊﾝﾄﾞﾙ初期化
	Display(0);															// 画面描画
	if(m_PrtColor!=0) {													// <背景色変更有>
		MainWnd->SaveObjColor(col);										// 立体表示色保存
		if(m_PrtColor==1) {												// 背景色:黒の場合
			MainWnd->ChangeObjColor(MainWnd->WhiteColor);				// 立体表示色変更
			MainWnd->GetColor(MainWnd->BlackColor, &rc, &gc, &bc);
		} else {														// 背景色:白の場合
			MainWnd->ChangeObjColor(MainWnd->BlackColor);				// 立体表示色変更
			MainWnd->GetColor(MainWnd->WhiteColor, &rc, &gc, &bc);
		}
		glClearColor((float)rc, (float)gc, (float)bc, 0.0f); // 1.0f);	// ｶﾗｰﾊﾞｯﾌｧ初期化値設定
	}
	if(m_PrtTCheck) {													// <実寸印刷>
		SetScreenNo(0);													// 表示範囲設定
	}
	Display(0);															// OpenGL再表示
	Display(0);															// OpenGL再表示
	Display(0);															// OpenGL再表示
	Display(0);															// OpenGL再表示
	if(!m_PrtMode) {													// <<4面図指定>>
		fx = 0, fy = 0;													// 4面図転送元位置
		fw = ViewSize.cx, fh = ViewSize.cy;								// 4面図転送元ｻｲｽﾞ
	} else {															// <<透視図指定>>
		fx = PlRect[PRS].left, fy = PlRect[PRS].top;					// 透視図転送元位置
		fw = PlRect[PRS].Width(), fh = PlRect[PRS].Height();			// 透視図転送元ｻｲｽﾞ
		if(fw<10||fh<10) {
			fx = 0, fy = 0;												// 透視図が有効範囲分ない
			fw = ViewSize.cx, fh = ViewSize.cy;							// 場合は4面図転送
		}
	}
	GetActiveWindow()->GetClientRect(&crct);							// ｸﾗｲｱﾝﾄ領域の位置
	ClientToScreen(&crct);												// 画面座標へ変換
	m_pwnd = GetDesktopWindow();										// ﾃﾞｽｸﾄｯﾌﾟｳｨﾝﾄﾞｳ
	m_pcdc = m_pwnd->GetDC();											// ｱｸﾃｨﾌﾞｳｨﾝﾄﾞｳのｸﾗｲｱﾝﾄDC
	m_pwnd->GetWindowRect(&drct);										// ﾃﾞｽｸﾄｯﾌﾟｳｨﾝﾄﾞｳの位置
	fx += crct.left, fy += crct.top;									// ﾃﾞｽｸﾄｯﾌﾟ上のｸﾗｲｱﾝﾄ領域の位置
	if(fx>drct.right||fy>drct.bottom||
       fx+fw>drct.right||fy+fh>drct.bottom) {							// 領域がﾃﾞｽｸﾄｯﾌﾟｻｲｽﾞを超える場合
		fx = 0, fy = 0, fw = drct.Width(), fh = drct.Height();			// ﾃﾞｽｸﾄｯﾌﾟ領域のｺﾋﾟｰ
	}
	if(!m_PrtOut) {														// <<<ﾌﾟﾘﾝﾀ出力>>>
		px = pDC->GetDeviceCaps(LOGPIXELSX);							// X方向ﾌﾟﾘﾝﾀdpi
		py = pDC->GetDeviceCaps(LOGPIXELSY);							// Y方向ﾌﾟﾘﾝﾀdpi
		if(m_PrtTCheck) {												// <<実寸印刷>>
			if(fw>fh) {													// <幅が大>
				tw = (int)((px*PlDstW[0]*2.0)/25.4);					// ﾌﾟﾘﾝﾀ印刷幅(dpi)
				th = (int)(((double)fh*py*tw)/(
					fw*px));					// ﾌﾟﾘﾝﾀ印刷高(dpi)
			} else {													// <高が大>
				th = (int)((py*PlDstH[0]*2.0)/25.4);					// ﾌﾟﾘﾝﾀ印刷高(dpi)
				tw = (int)(((double)fw*px*th)/(fh*py));					// ﾌﾟﾘﾝﾀ印刷幅(dpi)
			}
		} else if(m_PrtSCheck&&m_PrtSize>0) {							// <<出力ｻｲｽﾞ指定>>
			if(fw>fh) {													// <幅が大>
				tw = (int)(px*m_PrtSize/25.4);							// ﾌﾟﾘﾝﾀ印刷幅(dpi)
				th = (int)(((double)fh*py*tw)/(fw*px));					// ﾌﾟﾘﾝﾀ印刷高(dpi)
			} else {													// <高が大>
				th = (int)(py*m_PrtSize/25.4);							// ﾌﾟﾘﾝﾀ印刷高(dpi)
				tw = (int)(((double)fw*px*th)/(fh*py));					// ﾌﾟﾘﾝﾀ印刷幅(dpi)
			}
		} else {														// <<ｻｲｽﾞ指定無>>
			tw = fw, th = fh;											// 複写元と同一ｻｲｽﾞ
		}
	} else {															// <<<ﾌｧｲﾙ出力>>>
		tw = fw, th = fh;												// 複写元と同一ｻｲｽﾞ
	}
	PrRect.SetRect(0, 0, tw, th);										// 印刷(転送先)ｻｲｽﾞ保存
	if(!m_scdc.CreateCompatibleDC(m_pcdc)) {							// ｽｸﾘｰﾝﾒﾓﾘDC生成
		esw = 1;
	}
	if(!m_bitmap.CreateCompatibleBitmap(m_pcdc, tw, th)) {				// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ生成
		esw = 1;
	}
	CBitmap*  pOldBitmap = m_scdc.SelectObject(&m_bitmap);				// ﾒﾓﾘDCへﾋﾞｯﾄﾏｯﾌﾟ設定
	int  pOldSMode = m_scdc.SetStretchBltMode(COLORONCOLOR);			// 伸縮モード設定
	if(!m_scdc.StretchBlt(0, 0, tw, th, m_pcdc,
                          fx, fy, fw, fh, SRCCOPY)) {					// ｽｸﾘｰﾝ->ﾒﾓﾘDC 伸縮ｺﾋﾟｰ
		esw = 1;
	}
	m_scdc.SetStretchBltMode(pOldSMode);								// 伸縮ﾓｰﾄﾞ戻し
	m_scdc.SelectObject(pOldBitmap);									// ﾒﾓﾘDC->ﾋﾞｯﾄﾏｯﾌﾟ 切離
	if(esw) {
		m_scdc.DeleteDC();												// ﾒﾓﾘDC削除
		m_bitmap.DeleteObject();										// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ削除
		if(m_PrtColor!=0) {												// <背景色変更有>
			MainWnd->RestoreObjColor(col);								// 立体表示色復元
			MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);
			glClearColor((float)rc, (float)gc, (float)bc, 0.0f);//1.0f);// ｶﾗｰﾊﾞｯﾌｧ初期化値設定
			Display(0);													// OpenGL再表示
			Display(0);													// OpenGL再表示
		}
		AfxMessageBox(IDS_PRTERR);										// ｴﾗｰﾒｯｾｰｼﾞ後中止
		delete[] col;
		return;
	}
	HBITMAP hbitmap = (HBITMAP)m_bitmap.GetSafeHandle();				// ﾋﾞｯﾄﾏｯﾌﾟﾊﾝﾄﾞﾙ取得
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	::GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);
	biBits = 24;			// (WORD)(bm.bmPlanes*bm.bmBitsPixel);
	bi.biSize = sizeof(BITMAPINFOHEADER);								// DIBﾍｯﾀﾞｰ作成
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = biBits;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth*biBits)*bm.bmHeight;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	if(biBits==1) {														// ﾊﾟﾚｯﾄｻｲｽﾞ計算
		PalSize = 2;
	} else if(biBits==4) {
		PalSize = 16;
	} else if(biBits==8) {
		PalSize = 256;
	} else {
		PalSize = 0;
	}
	PalSize = PalSize*sizeof(RGBQUAD);
	dwLen = bi.biSize+PalSize+bi.biSizeImage;
	hdc = m_scdc.m_hDC;
	hpal = ::SelectPalette(hdc, hpal, FALSE);
	m_hDIB = ::GlobalAlloc(GMEM_MOVEABLE, dwLen);
	lpbi = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)m_hDIB);
	::RealizePalette(hdc);
	*lpbi = bi;
	::GetDIBits(hdc, hbitmap, 0, (UINT)bi.biHeight,
                (LPSTR)lpbi+lpbi->biSize+PalSize,
				(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);					// DDB => DIB
	::GlobalUnlock((HGLOBAL)m_hDIB);
	::SelectPalette(hdc, hpal, FALSE);
	m_scdc.DeleteDC();													// ﾒﾓﾘDC削除
	m_bitmap.DeleteObject();											// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ削除
	if(m_PrtColor!=0) {													// <背景色変更有>
		MainWnd->RestoreObjColor(col);									// 立体表示色復元
		MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);
		glClearColor((float)rc, (float)gc, (float)bc, 0.0f); // 1.0f);	// ｶﾗｰﾊﾞｯﾌｧ初期化値設定
		Display(0);														// OpenGL再表示
		Display(0);														// OpenGL再表示
	}
	delete[] col;
}

/*******************************************/
void CJcad3GlbView::OutputPrinter(CDC* pDC)								//<<< ﾌﾟﾘﾝﾀへの印刷
/*******************************************/
{
	HDC   hdc;
	LPSTR lpDIBHdr;														// Pointer to BITMAPINFOHEADER
	LPSTR lpDIBBits;													// Pointer to DIB bits
	int   sx, sy, px=0, py=0, bSuccess=0;

	if(m_hDIB==NULL) {
		return;
	}
	if(!m_PrtPos) {														// <用紙中央印刷の場合>
		sx = pDC->GetDeviceCaps(HORZRES);								// X方向ﾌﾟﾘﾝﾀ幅
		sy = pDC->GetDeviceCaps(VERTRES);								// Y方向ﾌﾟﾘﾝﾀ高
		px = (sx>PrRect.Width()) ? (sx-PrRect.Width())/2 : 0;			// X方向印刷位置
		py = (sy>PrRect.Height()) ? (sy-PrRect.Height())/2 : 0;			// Y方向印刷位置
	}
	lpDIBHdr = (LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
	lpDIBBits = lpDIBHdr + *(LPDWORD)lpDIBHdr + PalSize;
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	hdc = pDC->m_hDC;
	hpal = ::SelectPalette(hdc, hpal, TRUE);							// ｼｽﾃﾑﾊﾟﾚｯﾄ変更
	bSuccess = ::SetDIBitsToDevice(hdc,									// hDC
								   px, py,								// DestX, DestY
								   PrRect.Width(), PrRect.Height(),		// nDestWidth, Height
								   PrRect.left, PrRect.top,				// SrcX, SrcY
								   0,									// nStartScan
								   (WORD)PrRect.Height(),				// nNumScans
								   lpDIBBits,							// lpBits
								   (LPBITMAPINFO)lpDIBHdr,				// lpBitsInfo
								   DIB_RGB_COLORS);						// wUsage
	::GlobalUnlock((HGLOBAL)m_hDIB);
	::SelectPalette(hdc, hpal, TRUE);
	if(bSuccess==0) {
		AfxMessageBox(IDS_PRTERR);										// 解像度を下げる
	}
}

/************************************/
void CJcad3GlbView::OutputFile(void)									//<<< ﾌｧｲﾙ印刷
/************************************/
{
	CString ext;
	BOOL    ret;

	if(m_hDIB!=NULL) {
		ext = m_PrtFile.Right(3);										// ﾌｧｲﾙの拡張子
		if(!ext.CompareNoCase("jpg")) {									// <JPEG形式>
			ret = IKJpegFileSave(m_PrtFile, m_hDIB, 0, m_CapJComp, 2,
                                 "JCAD3 GLB Output Picture",
                                 0, NULL, NULL, NULL);					// JPEGﾌｧｲﾙ出力
		} else if(!ext.CompareNoCase("bmp")) {							// <ﾋﾞｯﾄﾏｯﾌﾟ形式>
			ret = IKBmpFileSave(m_PrtFile, m_hDIB, FALSE,
                                0, NULL, NULL, NULL);					// ﾋﾞｯﾄﾏｯﾌﾟﾌｧｲﾙ出力
		}
		::GlobalFree((HGLOBAL)m_hDIB);
		if(!ret) {
			AfxMessageBox(IDS_ERR_FILEWRITE);							// ﾌｧｲﾙｴﾗｰ
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// オペレーション

/******************************/
void CJcad3GlbView::Init(void)											//<<< 初期化
/******************************/
{
	Sphase = 0;
	m_CursorNo = 0;
	m_ActMenu = NONE, m_SaveMenu = NONE;
	m_EdtCount = 0, m_DspCount = 0;
	m_DAraFlg = 0;
	m_CopyBtn = 0, m_NumBtn = 0, m_CopyNum = 0;
	m_GemNo = 0;
	m_Shading = 0, m_Rendering = 0, m_RendPlane = 0;
	m_Ncdflg = 0, m_Ncpflg = 0;
	m_ULineCnt = 0, m_VLineCnt = 0;
	m_SurfTension = 0;
	Cursor.p[0] = 0.0, Cursor.p[1] = 0.0, Cursor.p[2] = 0.0;
	Wi0 = 0, Wi1 = 0, Wi2 = 0, Wi3 = 0, Wi4 = 0, Wi5 = 0, Wi6 = 0;
	InitChar();															// ｷｰ入力初期化
	m_XLmtR = 0.0, m_YLmtR = 0.0, m_ZLmtR = 0.0;						// ﾄﾗｯｶｰ限定初期化
	Tracker1.Reset(1);													// ﾄﾗｯｶｰ1解除
	Tracker2.Reset(1);													// ﾄﾗｯｶｰ2解除
	Tracker3.Reset(1);													// ﾄﾗｯｶｰ3解除
	Tracker4.Reset(1);													// ﾄﾗｯｶｰ4解除
	RestoreHomePosition(0);												// 表示状態復帰
	InitOifItem();														// 立体構成情報ｱｲﾃﾑ初期化
	InitDLSetup = 1;													// OpenGL DL 初期化:済
}

/**********************************/
void CJcad3GlbView::InitChar(void)										//<<< ｷｰ入力初期化
/**********************************/
{
	m_Char = 0, m_CChar = 0, m_FChar = 0, m_FPres = 0, m_DChar = 0;
	if(m_SFuncMode<3) {													// <詳細>
		m_pFunction->OffAllButton();									// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ全釦OFF
	} else if(m_SFuncMode<6) {											// <簡易>
		m_pFunction2->OffAllButton();									// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2全釦OFF
	}
}

/***********************************************/
void CJcad3GlbView::SetCChar(int flg, UINT chr)							//<<< 制御ｷｰ設定
/***********************************************/
{
	m_CChar = (flg) ? m_CChar|chr : m_CChar&~(chr);
}

/***********************************************/
void CJcad3GlbView::SetFChar(int flg, UINT chr)							//<<< ﾌｧﾝｸｼｮﾝｷｰ設定
/***********************************************/
{
	m_FChar = (flg) ? m_FChar|chr : m_FChar&~(chr);
}

/***************************************/
void CJcad3GlbView::GetFChar(int* func)									//<<< ﾌｧﾝｸｼｮﾝｷｰ取得
/***************************************/
{
	func[0] = (m_FChar&0x1) ? 1 : 0;									// [F5]ｷｰ押下中
	func[1] = (m_FChar&0x2) ? 1 : 0;									// [F6]ｷｰ押下中
	func[2] = (m_FChar&0x4) ? 1 : 0;									// [F7]ｷｰ押下中
	func[3] = (m_FChar&0x8) ? 1 : 0;									// [F8]ｷｰ押下中
	func[4] = (m_FChar&0x10) ? 1 : 0;									// [F9]ｷｰ押下中
	func[5] = (m_FChar&0x20) ? 1 : 0;									// [F10]ｷｰ押下中
	func[6] = (m_FChar&0x40) ? 1 : 0;									// [F11]ｷｰ押下中
	func[7] = (m_FChar&0x80) ? 1 : 0;									// [F12]ｷｰ押下中
}

/***************************************/
void CJcad3GlbView::SetScreenNo(int no)									//<<< ｽｸﾘｰﾝNo設定
/***************************************/
{
	if(no==0) {															// 全平面
		ScrDiv1 = 0.5, ScrDiv2 = 0.5;
	} else if(no==1) {													// 透視図
		ScrDiv1 = 0.0, ScrDiv2 = 1.0;
	} else if(no==2) {													// XY平面
		ScrDiv1 = 1.0, ScrDiv2 = 1.0;
	} else if(no==3) {													// YZ平面
		ScrDiv1 = 1.0, ScrDiv2 = 0.0;
	} else if(no==4) {													// ZX平面
		ScrDiv1 = 0.0, ScrDiv2 = 0.0;
	}
	InitScreen(ViewSize.cx, ViewSize.cy);								// ｽｸﾘｰﾝ初期処理
}

/***********************************************/
void CJcad3GlbView::OutputCapData(CString path)							//<<< ｷｬﾌﾟﾁｬﾃﾞｰﾀ出力
/***********************************************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙ表示
	CWnd*	m_pwnd;
	CDC*    m_pcdc;
	CDC     m_scdc;
	CBitmap m_bitmap;
	CRect   drct, crct;
	int     fx, fy, fw, fh, sw, sh, esw=0;
	BITMAP  bm;
	DWORD   dwLen;
	WORD    biBits;
	HDC     hdc;
	BOOL    ret;
	BITMAPINFOHEADER   bi;
	LPBITMAPINFOHEADER lpbi;

	Display(0);															// OpenGL再表示
	Display(0);															// OpenGL再表示
	Display(0);															// OpenGL再表示
	Display(0);															// OpenGL再表示
	m_hDIB = NULL;														// DIBﾊﾝﾄﾞﾙ初期化
	Display(0);															// OpenGL再表示
	if(m_CapPlane==0) {													// <X-Y平面>
		fx = PlRect[XY].left, fy = PlRect[XY].top;						// X-Y平面転送元位置
		fw = PlRect[XY].Width(), fh = PlRect[XY].Height();				// X-Y平面転送元ｻｲｽﾞ
	} else if(m_CapPlane==1) {											// <Y-Z平面>
		fx = PlRect[YZ].left, fy = PlRect[YZ].top;						// Y-Z平面転送元位置
		fw = PlRect[YZ].Width(), fh = PlRect[YZ].Height();				// Y-Z平面転送元ｻｲｽﾞ
	} else if(m_CapPlane==2) {											// <Z-X平面>
		fx = PlRect[ZX].left, fy = PlRect[ZX].top;						// Z-X平面転送元位置
		fw = PlRect[ZX].Width(), fh = PlRect[ZX].Height();				// Z-X平面転送元ｻｲｽﾞ
	} else if(m_CapPlane==3) {											// <透視平面>
		fx = PlRect[PRS].left, fy = PlRect[PRS].top;					// 透視図転送元位置
		fw = PlRect[PRS].Width(), fh = PlRect[PRS].Height();			// 透視図転送元ｻｲｽﾞ
	} else {															// <全面(4面)>
		fx = 0, fy = 0;													// 4面図転送元位置
		fw = ViewSize.cx, fh = ViewSize.cy;								// 4面図転送元ｻｲｽﾞ
	}
	if(fw<10||fh<10) {													// 有効範囲分ない場合は
		fx = 0, fy = 0, fw = ViewSize.cx, fh = ViewSize.cy;				// 4面図転送
	}
	sw = fw, sh = fh;													// 基準画面ｻｲｽﾞを保存
	fw = (int)(((double)fw*(double)m_CapWidth/100.0)+0.5);				// 出力幅
	fw = (fw<10) ? 10 : fw;
	fh = (int)(((double)fh*(double)m_CapHeight/100.0)+0.5);				// 出力高さ
	fh = (fh<10) ? 10 : fh;
	if(m_CapPos==0) {													// 画面中心の場合は
		fx += (sw-fw)/2, fy += (sh-fh)/2;								// 縮小分/2を転送元位置に加算
	}
	GetActiveWindow()->GetClientRect(&crct);							// ｸﾗｲｱﾝﾄ領域の位置
	ClientToScreen(&crct);												// 画面座標へ変換
	m_pwnd = GetDesktopWindow();										// ﾃﾞｽｸﾄｯﾌﾟｳｨﾝﾄﾞｳ
	m_pcdc = m_pwnd->GetDC();											// ｱｸﾃｨﾌﾞｳｨﾝﾄﾞｳのｸﾗｲｱﾝﾄDC
	m_pwnd->GetWindowRect(&drct);										// ﾃﾞｽｸﾄｯﾌﾟｳｨﾝﾄﾞｳの位置
	fx += crct.left, fy += crct.top;									// ﾃﾞｽｸﾄｯﾌﾟ上のｸﾗｲｱﾝﾄ領域の位置
	if(fx>drct.right||fy>drct.bottom||
       fx+fw>drct.right||fy+fh>drct.bottom) {							// 領域がﾃﾞｽｸﾄｯﾌﾟｻｲｽﾞを超える場合
		fx = 0, fy = 0, fw = drct.Width(), fh = drct.Height();			// ﾃﾞｽｸﾄｯﾌﾟ領域のｺﾋﾟｰ
	}
	if(!m_scdc.CreateCompatibleDC(m_pcdc)) {							// ｽｸﾘｰﾝﾒﾓﾘDC生成
		esw = 1;
	}
	if(!m_bitmap.CreateCompatibleBitmap(m_pcdc, fw, fh)) {				// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ生成
		esw = 1;
	}
	CBitmap* pOldBitmap = m_scdc.SelectObject(&m_bitmap);				// ﾒﾓﾘDCへﾋﾞｯﾄﾏｯﾌﾟ設定
	if(!m_scdc.BitBlt(0, 0, fw, fh, m_pcdc, fx, fy, SRCCOPY)) {			// ｽｸﾘｰﾝ->ﾒﾓﾘDCへｺﾋﾟｰ
		esw = 1;
	}
	m_scdc.SelectObject(pOldBitmap);									// ﾒﾓﾘDC->ﾋﾞｯﾄﾏｯﾌﾟ 切離
	if(esw) {
		m_scdc.DeleteDC();												// ﾒﾓﾘDC削除
		m_bitmap.DeleteObject();										// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ削除
		m_pwnd->ReleaseDC(m_pcdc);										// ｸﾗｲｱﾝﾄDCの解放
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｴﾗｰﾒｯｾｰｼﾞ後中止
		return;
	}
	HBITMAP hbitmap = (HBITMAP)m_bitmap.GetSafeHandle();				// ﾋﾞｯﾄﾏｯﾌﾟﾊﾝﾄﾞﾙ取得
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	::GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);
	biBits = 24;			// (WORD)(bm.bmPlanes*bm.bmBitsPixel);
	bi.biSize = sizeof(BITMAPINFOHEADER);								// DIBﾍｯﾀﾞｰ作成
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = biBits;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth*biBits)*bm.bmHeight;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	if(biBits==1) {														// ﾊﾟﾚｯﾄｻｲｽﾞ計算
		PalSize = 2;
	} else if(biBits==4) {
		PalSize = 16;
	} else if(biBits==8) {
		PalSize = 256;
	} else {
		PalSize = 0;
	}
	PalSize = PalSize*sizeof(RGBQUAD);
	dwLen = bi.biSize+PalSize+bi.biSizeImage;
	hdc = m_scdc.m_hDC;
	hpal = ::SelectPalette(hdc, hpal, FALSE);
	m_hDIB = ::GlobalAlloc(GMEM_MOVEABLE, dwLen);
	lpbi = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)m_hDIB);
	::RealizePalette(hdc);
	*lpbi = bi;
	::GetDIBits(hdc, hbitmap, 0, (UINT)bi.biHeight,
                (LPSTR)lpbi+lpbi->biSize+PalSize,
				(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);					// DDB => DIB
	::GlobalUnlock((HGLOBAL)m_hDIB);
	::SelectPalette(hdc, hpal, FALSE);
	m_scdc.DeleteDC();													// ﾒﾓﾘDC削除
	m_bitmap.DeleteObject();											// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ削除
	m_pwnd->ReleaseDC(m_pcdc);											// ｸﾗｲｱﾝﾄDCの解放
	if(m_hDIB!=NULL) {
		if(m_CapKind==0) {												// <JPEG形式>
			ret = IKJpegFileSave(path+".jpg", m_hDIB, 0, m_CapJComp, 2,
                                 "JCAD3 GLB Output Picture",
                                 0, NULL, NULL, NULL);					// JPEGﾌｧｲﾙ出力
		} else if(m_CapKind==1) {										// <ﾋﾞｯﾄﾏｯﾌﾟ形式>
			ret = IKBmpFileSave(path+".bmp", m_hDIB, FALSE,
                                0, NULL, NULL, NULL);					// ﾋﾞｯﾄﾏｯﾌﾟﾌｧｲﾙ出力
		}
		::GlobalFree((HGLOBAL)m_hDIB);
		if(!ret) {
			AfxMessageBox(IDS_ERR_FILEWRITE);							// ﾌｧｲﾙｴﾗｰ
		}
	}
}

/**********************************************/
void CJcad3GlbView::InitScreen(int cx, int cy)							//<<< ｽｸﾘｰﾝ初期処理
/**********************************************/
{
	int    xs, ys, xi, yi;
	double xd, yd, rc, gc, bc;

	xs = (int)(ScrDiv1*cx);
	ys = (int)(ScrDiv2*cy);
	xi = (xs==0) ? 0 : xs - 1;											// XY平面ｻｲｽﾞ設定
	yi = (ys==0) ? 0 : ys - 1;
	PlRect[XY].SetRect(0, 0, xi, yi);
	PlSizeH[XY] = PlRect[XY].Height();
	PlSizeW[XY] = PlRect[XY].Width();
	xi = (xs==0) ? 0 : xs - 1;											// YZ平面ｻｲｽﾞ設定
	yi = (ys==0) ? 0 : ((ys==cy) ? ys-1 : ys+1);
	PlRect[YZ].SetRect(0, yi, xi, cy-1);
	PlSizeH[YZ] = PlRect[YZ].Height();
	PlSizeW[YZ] = PlRect[YZ].Width();
	xi = (xs==0) ? 0 : ((xs==cx) ? xs-1 : xs+1);						// ZX平面ｻｲｽﾞ設定
	yi = (ys==0) ? 0 : ((ys==cy) ? ys-1 : ys+1);
	PlRect[ZX].SetRect(xi, yi, cx-1, cy-1);
	PlSizeH[ZX] = PlRect[ZX].Height();
	PlSizeW[ZX] = PlRect[ZX].Width();
	xi = (xs==0) ? 0 : ((xs==cx) ? xs-1 : xs+1);						// 透視図ｻｲｽﾞ設定
	yi = (ys==0) ? 0 : ys-1;
	PlRect[PRS].SetRect(xi, 0, cx-1, yi);
	PlSizeH[PRS] = PlRect[PRS].Height();
	PlSizeW[PRS] = PlRect[PRS].Width();
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列の再構築
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	glNewList(DL_BORDER, GL_COMPILE);									// 4面図境界ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);							// 正射影行列作成
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glLineWidth(1.0f);											// 線幅設定
			glBegin(GL_LINE_STRIP);
				MainWnd->GetColor(MainWnd->BorderSColor, &rc, &gc, &bc);
				glColor3d(rc, gc, bc);
				glVertex2d(0.0, 0.0);									// 4面図外枠垂直ﾗｲﾝ(黒影部)
				glVertex2d(0.0, 1.0);									// 4面図外枠水平ﾗｲﾝ(黒影部)
				glVertex2d(1.0, 1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
				MainWnd->GetColor(MainWnd->BorderHColor, &rc, &gc, &bc);
				glColor3d(rc, gc, bc);
				glVertex2d(1.0, 1.0);									// 4面図外枠垂直ﾗｲﾝ(日向部)
				glVertex2d(1.0, 0.0);									// 4面図外枠水平ﾗｲﾝ(日向部)
				glVertex2d(0.0, 0.0);
			glEnd();
			if((PlSizeH[XY]>0&&PlSizeH[YZ]>0)||
               (PlSizeW[YZ]>0&&PlSizeW[ZX]>0)) {
				glLineWidth(2.0f);										// 線幅設定
				glBegin(GL_LINES);
					MainWnd->GetColor(MainWnd->BorderColor, &rc, &gc, &bc);
					glColor3d(rc, gc, bc);								// 表示色設定(中心部)
					if(PlSizeW[YZ]>0&&PlSizeW[ZX]>0) {
						glVertex2d((double)ScrDiv1*1.0, 0.0);			// 4面図境界垂直ﾗｲﾝ(中心部)
						glVertex2d((double)ScrDiv1*1.0, 1.0);
					}
					if(PlSizeH[XY]>0&&PlSizeH[YZ]>0) {
						glVertex2d(0.0, 1.0-(double)ScrDiv2);			// 4面図境界水平ﾗｲﾝ(中心部)
						glVertex2d(1.0, 1.0-(double)ScrDiv2);
					}
				glEnd();
			}
			glLineWidth(1.0f);											// 線幅設定
			xd = (double)ViewSize.cy/ViewSize.cx, yd = 1.0;				// 縦横比設定
			glBegin(GL_LINES);
				if((PlSizeH[XY]>0&&PlSizeW[XY]>0)||
                   (PlSizeH[ZX]>0&&PlSizeW[ZX]>0)) {
					MainWnd->GetColor(MainWnd->XGuideColor, &rc, &gc, &bc);
					glColor3d(rc, gc, bc);								// 表示色設定
					if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {
						glVertex2d(0.0+(0.025*xd), 1.0-(0.025*yd));		// XY平面X座標軸
						glVertex2d(0.0+(0.025*xd), 1.0-(0.075*yd));
						glVertex2d(0.0+(0.0175*xd), 1.0-(0.08*yd));		// XY平面X文字
						glVertex2d(0.0+(0.0325*xd), 1.0-(0.105*yd));
						glVertex2d(0.0+(0.0325*xd), 1.0-(0.08*yd));
						glVertex2d(0.0+(0.0175*xd), 1.0-(0.105*yd));
					}
					if(PlSizeH[ZX]>0&&PlSizeW[ZX]>0) {
						glVertex2d(1.0-(0.025*xd), 0.0+(0.025*yd));		// ZX平面X座標軸
						glVertex2d(1.0-(0.075*xd), 0.0+(0.025*yd));
						glVertex2d(1.0-(0.095*xd), 0.0+(0.0375*yd));	// ZX平面X文字
						glVertex2d(1.0-(0.08*xd), 0.0+(0.0125*yd));
						glVertex2d(1.0-(0.08*xd), 0.0+(0.0375*yd));
						glVertex2d(1.0-(0.095*xd), 0.0+(0.0125*yd));
					}
				}
				if((PlSizeH[XY]>0&&PlSizeW[XY]>0)||
                   (PlSizeH[YZ]>0&&PlSizeW[YZ]>0)) {
					MainWnd->GetColor(MainWnd->YGuideColor, &rc, &gc, &bc);
					glColor3d(rc, gc, bc);								// 表示色設定
					if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {
						glVertex2d(0.0+(0.025*xd), 1.0-(0.025*yd));		// XY平面Y座標軸
						glVertex2d(0.0+(0.075*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.08*xd), 1.0-(0.0125*yd));		// XY平面Y文字
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.095*xd), 1.0-(0.0125*yd));
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.0375*yd));
					}
					if(PlSizeH[YZ]>0&&PlSizeW[YZ]>0) {
						glVertex2d(0.0+(0.025*xd), 0.0+(0.025*yd));		// YZ平面Y座標軸
						glVertex2d(0.0+(0.075*xd), 0.0+(0.025*yd));
						glVertex2d(0.0+(0.08*xd), 0.0+(0.0375*yd));		// YZ平面Y文字
						glVertex2d(0.0+(0.0875*xd), 0.0+(0.025*yd));
						glVertex2d(0.0+(0.095*xd), 0.0+(0.0375*yd));
						glVertex2d(0.0+(0.0875*xd), 0.0+(0.025*yd));
						glVertex2d(0.0+(0.0875*xd), 0.0+(0.025*yd));
						glVertex2d(0.0+(0.0875*xd), 0.0+(0.0125*yd));
					}
				}
				if((PlSizeH[YZ]>0&&PlSizeW[YZ]>0)||
                   (PlSizeH[ZX]>0&&PlSizeW[ZX]>0)) {
					MainWnd->GetColor(MainWnd->ZGuideColor, &rc, &gc, &bc);
					glColor3d(rc, gc, bc);								// 表示色設定
					if(PlSizeH[YZ]>0&&PlSizeW[YZ]>0) {
						glVertex2d(0.0+(0.025*xd), 0.0+(0.025*yd));		// YZ平面Z座標軸
						glVertex2d(0.0+(0.025*xd), 0.0+(0.075*yd));
						glVertex2d(0.0+(0.0175*xd), 0.0+(0.105*yd));	// YZ平面Z文字
						glVertex2d(0.0+(0.0325*xd), 0.0+(0.105*yd));
						glVertex2d(0.0+(0.0325*xd), 0.0+(0.105*yd));
						glVertex2d(0.0+(0.0175*xd), 0.0+(0.08*yd));
						glVertex2d(0.0+(0.0175*xd), 0.0+(0.08*yd));
						glVertex2d(0.0+(0.0325*xd), 0.0+(0.08*yd));
					}
					if(PlSizeH[ZX]>0&&PlSizeW[ZX]>0) {
						glVertex2d(1.0-(0.025*xd), 0.0+(0.025*yd));		// ZX平面Z座標軸
						glVertex2d(1.0-(0.025*xd), 0.0+(0.075*yd));
						glVertex2d(1.0-(0.0325*xd), 0.0+(0.105*yd));	// ZX平面Z文字
						glVertex2d(1.0-(0.0175*xd), 0.0+(0.105*yd));
						glVertex2d(1.0-(0.0175*xd), 0.0+(0.105*yd));
						glVertex2d(1.0-(0.0325*xd), 0.0+(0.08*yd));
						glVertex2d(1.0-(0.0325*xd), 0.0+(0.08*yd));
						glVertex2d(1.0-(0.0175*xd), 0.0+(0.08*yd));
					}
				}
			glEnd();
		glPopMatrix();
	glEndList();
	SetGrid();
	if(InitDLSetup) {													// <OpenGL DL 初期化:済>
		ChangeTexPixels();												// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	}
}

/************************************/
void CJcad3GlbView::ResetColor(void)									//<<< ｽｸﾘｰﾝｶﾗｰ再設定
/************************************/
{
	double r, g, b;

	MainWnd->GetColor(MainWnd->BackColor, &r, &g, &b);
	glClearColor((float)r, (float)g, (float)b, 0.0f); // 1.0f);			// ｶﾗｰﾊﾞｯﾌｧ初期化値設定
	InitScreen(ViewSize.cx, ViewSize.cy);								// ｽｸﾘｰﾝ初期処理
	Display(0);															// OpenGL再表示
}

/**************************************************************/
void CJcad3GlbView::SetSeishaei(const SCRTYPE& cent, int fovy)			//<<< 正射影行列構築
/**************************************************************/
{
	BOXTYPE box;
	int     i;
	double  cpt[3], mia[3], maa[3], fvy, len, add=0;

	if(PlSizeH[XY]<1&&PlSizeH[YZ]<1&&PlSizeH[ZX]<1) {
		return;
	}
	GetScrBox(&box);													// ｽｸﾘｰﾝﾎﾞｯｸｽ取得
	for(i=0; i<DIM; i++) {
		cpt[i] = cent.p[i]/1000.0;										// ﾊﾟﾗﾒｰﾀ実数化
		mia[i] = (box.bp[0].p[i]>0.0) ? 0.0 : box.bp[0].p[i];			// 表示立体のｻｲｽﾞ
		maa[i] = (box.bp[1].p[i]<0.0) ? 0.0 : box.bp[1].p[i];
		PlDstH[i] = 10.0, PlDstW[i] = 10.0;
	}
	fvy = fovy/1000.0;
	if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {									// ｽｸﾘｰﾝ表示ｻｲｽﾞ
		PlDstH[0] = fvy*ScrDiv2/0.5;
		PlDstW[0] = PlDstH[0]*PlSizeW[XY]/PlSizeH[XY];
	}
	if(PlSizeH[YZ]>0&&PlSizeW[YZ]>0) {
		PlDstH[1] = fvy*(1.0-ScrDiv2)/0.5;
		PlDstW[1] = PlDstH[1]*PlSizeW[YZ]/PlSizeH[YZ];
	}
	if(PlSizeH[ZX]>0&&PlSizeW[ZX]>0) {
		PlDstH[2] = fvy*(1.0-ScrDiv2)/0.5;
		PlDstW[2] = PlDstH[2]*PlSizeW[ZX]/PlSizeH[ZX];
	}
	mia[0] = (mia[0]>cpt[0]-PlDstH[0]/2.0)?cpt[0]-PlDstH[0]/2.0:mia[0];	// ｽｸﾘｰﾝ&立体範囲比較
	mia[0] = (mia[0]>cpt[0]-PlDstW[2]/2.0)?cpt[0]-PlDstW[2]/2.0:mia[0];
	maa[0] = (maa[0]<cpt[0]+PlDstH[0]/2.0)?cpt[0]+PlDstH[0]/2.0:maa[0];
	maa[0] = (maa[0]<cpt[0]+PlDstW[2]/2.0)?cpt[0]+PlDstW[2]/2.0:maa[0];
	mia[1] = (mia[1]>cpt[1]-PlDstW[1]/2.0)?cpt[1]-PlDstW[1]/2.0:mia[1];
	maa[1] = (maa[1]<cpt[1]+PlDstW[1]/2.0)?cpt[1]+PlDstW[1]/2.0:maa[1];
	mia[2] = (mia[2]>cpt[2]-PlDstH[2]/2.0)?cpt[2]-PlDstH[2]/2.0:mia[2];
	maa[2] = (maa[2]<cpt[2]+PlDstH[2]/2.0)?cpt[2]+PlDstH[2]/2.0:maa[2];
	for(i=0; i<3; i++) {
		if((len=maa[i]-mia[i])>add) {									// 最大幅
			add = len;
		}
	}
	add = (add>100.0) ? add*1.5 : 100.0;
	glNewList(DL_ORTHOXY, GL_COMPILE);									// XY正射影ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(cpt[1]-PlDstW[0]/2.0, cpt[1]+PlDstW[0]/2.0, -cpt[0]-PlDstH[0]/2.0,
				-cpt[0]+PlDstH[0]/2.0, mia[2]-add, maa[2]+add);
	glEndList();
	glNewList(DL_ORTHOYZ, GL_COMPILE);									// YZ正射影ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(cpt[1]-PlDstW[1]/2.0, cpt[1]+PlDstW[1]/2.0, cpt[2]-PlDstH[1]/2.0,
				cpt[2]+PlDstH[1]/2.0, mia[0]-add, maa[0]+add);
	glEndList();
	glNewList(DL_ORTHOZX, GL_COMPILE);									// ZX正射影ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-cpt[0]-PlDstW[2]/2.0, -cpt[0]+PlDstW[2]/2.0, cpt[2]-PlDstH[2]/2.0,
				cpt[2]+PlDstH[2]/2.0, mia[1]-add, maa[1]+add);
	glEndList();
}

/****************************************/
void CJcad3GlbView::SetPerspective(void)								//<<< 透視法射影行列の構築
/****************************************/
{
	PNTTYPE pt1, pt2;
	VECTYPE vec;
	int     i;
	double  weye[3], wprs[3], aspect;

	if(PlSizeH[PRS]<1&&PlSizeW[PRS]<1) {
		return;
	}
	ScrToPnt(0, &m_EyeSv, &pt1);
	ScrToPnt(0, &m_CentPs, &pt2);
	VecV(&pt1, &pt2, &vec);												// 開始視線ﾍﾞｸﾄﾙ取得
	for(i=0; i<DIM; i++) {
		weye[i] = m_Eye.p[i]/1000.0;									// ﾊﾟﾗﾒｰﾀ実数化
		wprs[i] = m_Eye.p[i]/1000.0+vec.p[i];
	}
	aspect = (double)PlSizeW[PRS]/PlSizeH[PRS];
	glNewList(DL_PARS, GL_COMPILE);										// 透視法射影ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, aspect, 1.0, 3000.0);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(weye[0], weye[1], weye[2], wprs[0], wprs[1], wprs[2],
                  m_VtVec.p[0], m_VtVec.p[1], m_VtVec.p[2]);
	glEndList();
}

/************************************/
void CJcad3GlbView::Display(int flg)									//<<< 再描画
/************************************/									// flg 0:全面,1:3面図,2:透視図
{
	int i, wflg;

	if(m_Ncdflg||ViewSize.cy<1) {										// NC表示中や領域無は中止
		return;
	}
	wflg = flg;
	if(flg!=m_DisplayFlg) {												// <表示ｴﾘｱが異なる場合>
		wflg = 0;														// 最初は必ず全面表示(画面ちらつき防止)
	}
	m_DisplayFlg = flg;													// 表示ﾌﾗｸﾞ保存
	if(wflg==0) {														// <<全面描画>>
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		if(!m_OpenGLTest) {												// <OpenGLﾃｽﾄ中以外>
			for(i=0; i<3; i++) {										// 3面分(XY:0,YZ:1,ZX:2)
				if(PlSizeH[i]>0&&PlSizeW[i]>0) {						// <3面図領域有>
					glEnable(GL_STENCIL_TEST);
					ViewDisplayXYZ(i);									// 3面図描画
					glDisable(GL_STENCIL_TEST);
				}
			}
		}
		if(PlSizeH[PRS]>0&&PlSizeW[PRS]>0) {							// <透視図領域有>
			ViewDisplayPRS();											// 透視図描画
		}
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, ViewSize.cx-1, ViewSize.cy-1);					// 4面図境界&外枠描画
		glCallList(DL_BORDER);
	} else if(wflg==1) {												// <<3面図描画>>
		if(m_OpenGLTest) {												// OpenGLﾃﾞｽﾄ中は中止
			return;
		}
		for(i=0; i<3; i++) {											// 3面分(XY:0,YZ:1,ZX:2)
			if(PlSizeH[i]>0&&PlSizeW[i]>0) {							// <3面図領域有>
				glScissor(PlRect[i].left+1, ViewSize.cy-PlRect[i].bottom,
                          PlRect[i].Width()-1, PlRect[i].Height()-1);	// 描画領域を制限
				glEnable(GL_SCISSOR_TEST);								// ｼｻﾞｰ･ﾃｽﾄを有効化
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_STENCIL_TEST);
				ViewDisplayXYZ(i);										// 3面図描画
				glDisable(GL_STENCIL_TEST);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_SCISSOR_TEST);								// ｼｻﾞｰ･ﾃｽﾄを無効化
			}
		}
		glViewport(0, 0, ViewSize.cx-1, ViewSize.cy-1);					// 4面図境界&外枠描画
		glCallList(DL_BORDER);
	} else {															// <<透視図描画>>
		if(PlSizeH[PRS]>0&&PlSizeW[PRS]>0) {							// <透視図領域有>
			glScissor(PlRect[PRS].left+1, ViewSize.cy-PlRect[PRS].bottom,
                      PlRect[PRS].Width()-1, PlRect[PRS].Height()-1);	// 描画領域を制限
			glEnable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを有効化
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			ViewDisplayPRS();											// 透視図描画
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを無効化
		}
	}
	glFinish();
	::SwapBuffers(::wglGetCurrentDC());
}

/*********************************************/
void CJcad3GlbView::ViewDisplayXYZ(int plane)							//<<< 3面図描画
/*********************************************/
{
	glPushMatrix();
		glLoadIdentity();												// ﾋﾞｭｰﾎﾟｰﾄ設定
		glViewport(PlRect[plane].left, ViewSize.cy-PlRect[plane].bottom-1,
			       PlRect[plane].Width(), PlRect[plane].Height());
		glCallList(DL_ORTHOXY+plane);									// 正射影変換
		glCallList(DL_XY+plane);										// ﾓﾃﾞﾘﾝｸﾞ変換
		glStencilFunc(GL_ALWAYS, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if((m_RendPlane&&m_Rendering>1)||m_Shading==2||m_DispMode==5) {	// <3面図ｼｪｰﾃﾞｨﾝｸﾞ>
			DisplayAllObj3P(0);											// 3面図立体表示
		}
		glDisable(GL_DEPTH_TEST);										// ﾃﾞﾌﾟｽ･ﾃｽﾄを無効化
		if(Tracker1.EnableTrack()) {
			Tracker1.Draw(plane);										// ﾄﾗｯｶｰ1描画
		}
		if(Tracker2.EnableTrack()) {
			Tracker2.Draw(plane);										// ﾄﾗｯｶｰ2描画
		}
		if(Tracker3.EnableTrack()) {
			Tracker3.Draw(plane);										// ﾄﾗｯｶｰ3描画
		}
		if(Tracker4.EnableTrack()) {
			Tracker4.Draw(plane);										// ﾄﾗｯｶｰ4描画
		}
		if(Line.EnableDraw()) {
			Line.Draw(plane);											// ﾗｲﾝ描画
		}
		if(m_BOX&&m_BoxPlane==plane) {
			glCallList(m_BOX);											// 矩形描画
		}
		if(m_CIRCLE&&m_CirclePlane==plane) {
			glCallList(m_CIRCLE);										// 円描画
		}
		glEnable(GL_DEPTH_TEST);										// ﾃﾞﾌﾟｽ･ﾃｽﾄを有効化
		glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if((m_RendPlane==0||m_Rendering<2)&&
           m_Shading!=2&&m_DispMode!=5) {								// <3面図ｼｪｰﾃﾞｨﾝｸﾞ以外>
			DisplayAllObj3P(2);											// 3面図立体表示(編集対象)
			DisplayAllObj3P(1);											// 3面図立体表示(編集対象外)
		}
		if(m_BASE&&(m_BasePlane==plane||m_BasePlane==3)) {
			glCallList(m_BASE);											// 基準線描画
		}
		if(m_BpDisp) {
			glCallList(DL_BASEPOINT);									// 基準点描画
		}
		if(m_BlDisp) {													// 基準線描画
			glCallList(DL_BASELINE);
		}
		if(m_GridDisp) {
			DispGrid(plane, 0, 1);										// ｸﾞﾘｯﾄﾞ描画
		}
		if(m_SCALE&&m_ScalePlane==plane) {
			glCallList(m_SCALE);										// 定規描画
		}
		if(m_PROT&&m_ProtPlane==plane) {
			glCallList(m_PROT);											// 分度器描画
		}
		if(m_GLB[plane]) {
			glCallList(m_GLB[plane]);		    						// GLBﾃﾝﾌﾟﾚｰﾄ描画
		}
		if(m_TEMP[plane]) {
			glCallList(m_TEMP[plane]);									// ﾃﾝﾌﾟﾚｰﾄ描画
		}
		if(m_STAGE) {
			glCallList(m_STAGE);										// ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ描画
		}
		if(m_TexDisp[plane]) {											// <画像表示中>
			glCallList(m_TEXTURE[plane]);								// ﾃｸｽﾁｬﾋﾟｸｾﾙ描画
		}
		glPopMatrix();
}

/****************************************/
void CJcad3GlbView::ViewDisplayPRS(void)								//<<< 透視図描画
/****************************************/
{
	glPushMatrix();
		glLoadIdentity();												// ﾋﾞｭｰﾎﾟｰﾄ設定
		glViewport(PlRect[PRS].left, ViewSize.cy-PlRect[PRS].bottom-1,
			       PlRect[PRS].Width(), PlRect[PRS].Height());
		glCallList(DL_PARS);											// 透視法射影&ﾓﾃﾞﾘﾝｸﾞ変換
		if(Tracker1.EnableTrack()) {
			Tracker1.Draw(PRS);											// ﾄﾗｯｶｰ1描画
		}
		if(Tracker2.EnableTrack()) {
			Tracker2.Draw(PRS);											// ﾄﾗｯｶｰ2描画
		}
		if(Tracker3.EnableTrack()) {
			Tracker3.Draw(PRS);											// ﾄﾗｯｶｰ3描画
		}
		if(Tracker4.EnableTrack()) {
			Tracker4.Draw(PRS);											// ﾄﾗｯｶｰ4描画
		}
		DisplayAllObjPars();											// 透視図全立体表示
		glCallList(DL_XYZGUIDE);										// XYZｶﾞｲﾄﾞ表示
		if(Line.EnableDraw()) {
			Line.Draw(PRS);												// ﾗｲﾝ描画
		}
		if(m_STAGE) {
			glCallList(m_STAGE);										// ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ描画
		}
	glPopMatrix();
}

/********************************************************************/
void CJcad3GlbView::GetDisplayColor(OBJTYPE* op, int edt,
                                    double* r, double* g, double* b)	//<<< 立体描画色取得
/********************************************************************/
{
	int sel, gno[32];

	sel = GetSelNo(op);													// 選択No取得
	if(sel==1) {														// 選択立体1(BoolColor)
		MainWnd->GetColor(MainWnd->BoolColor, r, g, b);
	} else if(sel==2) {													// 選択立体2(ChooseColor)
		MainWnd->GetColor(MainWnd->ChooseColor, r, g, b);
	} else if(edt==0) {													// 編集対象外立体
		GetGroup(op, gno);												// ｸﾞﾙｰﾌﾟ情報取得
		if(gno[0]==1) {
			MainWnd->GetColor(MainWnd->GroupColor1, r, g, b);
		} else if(gno[1]==1) {
			MainWnd->GetColor(MainWnd->GroupColor2, r, g, b);
		} else if(gno[2]==1) {
			MainWnd->GetColor(MainWnd->GroupColor3, r, g, b);
		} else if(gno[3]==1) {
			MainWnd->GetColor(MainWnd->GroupColor4, r, g, b);
		} else if(gno[4]==1) {
			MainWnd->GetColor(MainWnd->GroupColor5, r, g, b);
		} else if(gno[5]==1) {
			MainWnd->GetColor(MainWnd->GroupColor6, r, g, b);
		} else if(gno[6]==1) {
			MainWnd->GetColor(MainWnd->GroupColor7, r, g, b);
		} else if(gno[7]==1) {
			MainWnd->GetColor(MainWnd->GroupColor8, r, g, b);
		} else if(gno[8]==1) {
			MainWnd->GetColor(MainWnd->GroupColor9, r, g, b);
		} else if(gno[9]==1) {
			MainWnd->GetColor(MainWnd->GroupColor10, r, g, b);
		} else if(gno[10]==1) {
			MainWnd->GetColor(MainWnd->GroupColor11, r, g, b);
		} else if(gno[11]==1) {
			MainWnd->GetColor(MainWnd->GroupColor12, r, g, b);
		} else if(gno[12]==1) {
			MainWnd->GetColor(MainWnd->GroupColor13, r, g, b);
		} else if(gno[13]==1) {
			MainWnd->GetColor(MainWnd->GroupColor14, r, g, b);
		} else if(gno[14]==1) {
			MainWnd->GetColor(MainWnd->GroupColor15, r, g, b);
		} else if(gno[15]==1) {
			MainWnd->GetColor(MainWnd->GroupColor16, r, g, b);
		} else {
			MainWnd->GetColor(MainWnd->ObjColor, r, g, b);
		}
	} else {															// 編集対象立体
		MainWnd->GetColor(MainWnd->SelectColor, r, g, b);
	}
}

/********************************************/
void CJcad3GlbView::DisplayAllObj3P(int flg)							//<<< 3面図立体描画
/********************************************/
{
	OBJTYPE* op;
	int      wfg, dsp, edt, cno, dmode, lmode, uflg, sel, col;
	double   r, g, b;

	if((m_RendPlane&&m_Rendering>1)||m_DispMode==5) {					// <3面図ｼｪｰﾃﾞｨﾝｸﾞ>
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_LIGHTING);
	}
	wfg = (flg!=0) ? flg : 1;
	BaseObjPtr(wfg, &op, &dsp, &edt);									// 先頭立体ﾎﾟｲﾝﾀ取得
	while(op) {
		if(flg==1&&edt==1) {											// 非対象立体指示の場合
			NextObjPtr(wfg, &op, &dsp, &edt);							// 次立体ﾎﾟｲﾝﾀ取得
			continue;													// 次立体へ
		}
		cno = GetObjCmdNo(1, op);										// 立体ｺﾏﾝﾄﾞNo取得
		if(m_DispMode==5&&(cno==SETSURF||cno==SETCURVE)) {				// <透視図ｼｪｰﾃﾞｨﾝｸﾞで対象外>
			glShadeModel(GL_FLAT);										// 設定をﾗｲﾝに変更
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
			GetDisplayColor(op, edt, &r, &g, &b);						// 立体描画色取得
			glColor3d(r, g, b);											// 表示色設定
		} else if((m_RendPlane&&m_Rendering>1)||m_DispMode==5) {		// <3面図ｼｪｰﾃﾞｨﾝｸﾞ>
			if(m_RendPlane&&m_Rendering>1) {
				col = GetObjCol(op);									// 材質取得
			} else {
				sel = GetSelNo(op);										// 選択No取得
				if(sel==1) {											// 選択立体1(BoolColor)
					col = 20;											// DL_BOOL
				} else if(sel==2) {										// 選択立体2(ChooseColor)
					col = 19;											// DL_CHOOSE
				} else if(edt==1) {										// 編集対象立体
					col = 18;											// DL_SELECT
				} else {												// 編集対象外立体
					col = GetObjCol(op);								// 材質取得
				}
			}
			glCallList(DL_GOLD+col);									// 材質設定
		} else if(m_Rendering==1&&m_Shading==2&&GetSelNo(op)==3) {		// <ｼｪｰﾃﾞｨﾝｸﾞ･ﾓｰﾄﾞ>
			glShadeModel(GL_SMOOTH);
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
			glCallList(DL_GOLD);										// 材質設定
		} else {														// <通常表示>
			GetDisplayColor(op, edt, &r, &g, &b);						// 立体描画色取得
			glColor3d(r, g, b);											// 表示色設定
		}
		lmode = GetListMode(op);										// ﾘｽﾄﾓｰﾄﾞ取得(0/1:重い,2:軽い,3:ｼｪｰﾃﾞｨﾝｸﾞ)
		if(m_DispMode<4) {												// <ｼｪｰﾃﾞｨﾝｸﾞ以外>
			dmode = (m_DispMode==0||(m_DispMode==2&&edt==1)||
                    (m_DispMode==3&&edt==0)) ? 1 : 2;					// 表示ﾓｰﾄﾞ取得(1:重い,2:軽い)
			uflg = (lmode==1&&dmode==2) ? 1 : 0;						// 重い表示中に軽いﾓｰﾄﾞは更新ON
			uflg = (lmode==2&&dmode==1) ? 1 : uflg;						// 軽い表示中に重いﾓｰﾄﾞは更新ON
			uflg = (lmode==3&&m_Rendering==0) ? 1 : uflg;				// ｼｪｰﾃﾞｨﾝｸﾞ直後は更新ON
			uflg = (lmode==3&&m_Rendering==1&&
                    m_Shading>0&&GetSelNo(op)!=3) ? 1 : uflg;
		} else {														// <ｼｪｰﾃﾞｨﾝｸﾞ>
			uflg = (lmode!=3&&cno!=SETSURF&&cno!=SETCURVE) ? 1 : 0;		// ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ未設定は更新ON
		}
		if(uflg) {
			SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ設定
		}
		glCallList(GetListNo(op));										// 1立体描画
		if(m_DispMode==5&&(cno==SETSURF||cno==SETCURVE)) {				// <透視図ｼｪｰﾃﾞｨﾝｸﾞで対象外>
			glShadeModel(GL_SMOOTH);									// 設定をｼｪｰﾃﾞｨﾝｸﾞに変更
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
		} else if(m_DispMode!=5&&
                  m_Rendering==1&&m_Shading==2&&GetSelNo(op)==3) {		// <ｼｪｰﾃﾞｨﾝｸﾞ･ﾓｰﾄﾞ>
			glShadeModel(GL_FLAT);										// 設定を元に戻す
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
		}
		NextObjPtr(wfg, &op, &dsp, &edt);								// 次立体ﾎﾟｲﾝﾀ取得
	}
	if((m_RendPlane&&m_Rendering>1)||m_DispMode==5) {					// <3面図ｼｪｰﾃﾞｨﾝｸﾞ>
		glShadeModel(GL_FLAT);											// 設定を元に戻す
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
	}
}

/*******************************************/
void CJcad3GlbView::DisplayAllObjPars(void)								//<<< 透視図立体描画
/*******************************************/
{
	OBJTYPE* op;
	int      dsp, edt, cno, dmode, lmode, uflg, sel, col;
	double   r, g, b;

	if(m_Rendering==2) {												// <ﾃｸｽﾁｬ･ﾚﾝﾀﾞﾘﾝｸﾞ>
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	} else if(m_Rendering==3||m_DispMode>=4) {							// <ｼｪｰﾃﾞｨﾝｸﾞ>
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_LIGHTING);
	}
	BaseObjPtr(1, &op, &dsp, &edt);										// 先頭立体ﾎﾟｲﾝﾀ取得
	while(op) {
		cno = GetObjCmdNo(1, op);										// 立体ｺﾏﾝﾄﾞNo取得
		if(m_DispMode>=4&&(cno==SETSURF||cno==SETCURVE)) {				// <透視図ｼｪｰﾃﾞｨﾝｸﾞで対象外>
			glShadeModel(GL_FLAT);										// 設定をﾗｲﾝに変更
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
			GetDisplayColor(op, edt, &r, &g, &b);						// 立体描画色取得
			glColor3d(r, g, b);											// 表示色設定
		} else if(m_Rendering==2||m_Rendering==3||m_DispMode>=4) {		// <ﾚﾝﾀﾞﾘﾝｸﾞ/ｼｪｰﾃﾞｨﾝｸﾞ>
			if(m_Rendering==2||m_Rendering==3) {
				col = GetObjCol(op);									// 材質取得
			} else {
				sel = GetSelNo(op);										// 選択No取得
				if(sel==1) {											// 選択立体1(BoolColor)
					col = 20;											// DL_BOOL
				} else if(sel==2) {										// 選択立体2(ChooseColor)
					col = 19;											// DL_CHOOSE
				} else if(edt==1) {										// 編集対象立体
					col = 18;											// DL_SELECT
				} else {												// 編集対象外立体
					col = GetObjCol(op);								// 材質取得
				}
			}
			glCallList(DL_GOLD+col);									// 材質設定
		} else if(m_Rendering==1&&m_Shading>0&&GetSelNo(op)==3) {		// <ｼｪｰﾃﾞｨﾝｸﾞ･ﾓｰﾄﾞ>
			glShadeModel(GL_SMOOTH);
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
			glCallList(DL_GOLD);										// 材質設定
		} else {														// <通常表示>
			GetDisplayColor(op, edt, &r, &g, &b);						// 立体描画色取得
			glColor3d(r, g, b);											// 表示色設定
		}
		lmode = GetListMode(op);										// ﾘｽﾄﾓｰﾄﾞ取得(0/1:重い,2:軽い,3:ｼｪｰﾃﾞｨﾝｸﾞ)
		if(m_DispMode<4) {												// <ｼｪｰﾃﾞｨﾝｸﾞ以外>
			dmode = (m_DispMode==0||(m_DispMode==2&&edt==1)||
                    (m_DispMode==3&&edt==0)) ? 1 : 2;					// 表示ﾓｰﾄﾞ取得(1:重い,2:軽い)
			uflg = (lmode==1&&dmode==2) ? 1 : 0;						// 重い表示中に軽いﾓｰﾄﾞは更新ON
			uflg = (lmode==2&&dmode==1) ? 1 : uflg;						// 軽い表示中に重いﾓｰﾄﾞは更新ON
			uflg = (lmode==3&&m_Rendering==0) ? 1 : uflg;				// ｼｪｰﾃﾞｨﾝｸﾞ直後は更新ON
			uflg = (lmode==3&&m_Rendering==1&&
                    m_Shading>0&&GetSelNo(op)!=3) ? 1 : uflg;
		} else {														// <ｼｪｰﾃﾞｨﾝｸﾞ>
			uflg = (lmode!=3&&cno!=SETSURF&&cno!=SETCURVE) ? 1 : 0;		// ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ未設定は更新ON
		}
		if(uflg) {
			SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ設定
		}
		glCallList(GetListNo(op));										// 1立体描画
		if(m_DispMode>=4&&(cno==SETSURF||cno==SETCURVE)) {				// <透視図ｼｪｰﾃﾞｨﾝｸﾞで対象外>
			glShadeModel(GL_SMOOTH);									// 設定をｼｪｰﾃﾞｨﾝｸﾞに変更
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
		} else if(m_DispMode<4&&
                  m_Rendering==1&&m_Shading>0&&GetSelNo(op)==3) {		// <ｼｪｰﾃﾞｨﾝｸﾞ･ﾓｰﾄﾞ>
			glShadeModel(GL_FLAT);										// 設定を元に戻す
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
		}
		NextObjPtr(1, &op, &dsp, &edt);									// 次立体ﾎﾟｲﾝﾀ取得
	}
	if(m_Rendering==2) {												// <ﾃｸｽﾁｬ･ﾚﾝﾀﾞﾘﾝｸﾞ>
		glShadeModel(GL_FLAT);											// 設定を元に戻す
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	} else if(m_Rendering==3||m_DispMode>=4) {							// <全ｼｪｰﾃﾞｨﾝｸﾞ>
		glShadeModel(GL_FLAT);											// 設定を元に戻す
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
	}
}

/*********************************************************/
void CJcad3GlbView::DispGrid(int plane, int ers, int bak)				//<<< ｸﾞﾘｯﾄﾞ表示/消去
/*********************************************************/
{
	int    men=1;
	double rc, gc, bc;

	if(m_Fovy/m_GridSpace>ViewSize.cy/10) {
		return;
	}
	if(ers) {
		MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);			// ｸﾞﾘｯﾄﾞ消去色
	} else {
		MainWnd->GetColor(MainWnd->GridColor, &rc, &gc, &bc);			// ｸﾞﾘｯﾄﾞ表示色
	}
	if(!bak) {
		glDrawBuffer(GL_FRONT_AND_BACK);								// 前ﾊﾞｯﾌｧにも描画指定
	}
	glPushMatrix();
		glLoadIdentity();
		glViewport(PlRect[plane].left, ViewSize.cy-PlRect[plane].bottom-1,
	               PlRect[plane].Width(), PlRect[plane].Height());
		glCallList(DL_ORTHOXY+plane);
		glCallList(DL_XY+plane);
		glColor3d(rc, gc, bc);											// 表示色設定
		glCallList(DL_GRIDXY+plane);									// ｸﾞﾘｯﾄﾞ表示
		if(ers) {
			MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);		// ｸﾞﾘｯﾄﾞ消去色
		} else {
			MainWnd->GetColor(MainWnd->GridBPColor, &rc,&gc,&bc);		// ｸﾞﾘｯﾄﾞ目盛色
		}
		glColor3d(rc, gc, bc);											// 表示色設定
		glCallList(DL_GRIDXY2+plane);									// ｸﾞﾘｯﾄﾞ表示
	glPopMatrix();
	glDrawBuffer(GL_BACK);
}

/********************************************/
void CJcad3GlbView::SetDispList(OBJTYPE* op)							//<<< ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
/********************************************/
{
	PNTTYPE pnt;
	UINT    lno, dmd;
	int     i, cno, num1, num2, flg, ed;

	cno = GetObjCmdNo(1, op);											// 立体ｺﾏﾝﾄﾞNo取得
	if(m_DispMode>=4&&cno!=SETSURF&&cno!=SETCURVE) {					// <<ｼｪｰﾃﾞｨﾝｸﾞﾓｰﾄﾞ>>
		if(GetObjAtr(op)!=1) {											// <宝石ｶｯﾄ以外>
			SetShadingData(op);											// ｼｪｰﾃﾞｨﾝｸﾞﾃﾞｰﾀ設定
		} else {														// <宝石ｶｯﾄ>
			GetJewelFVec(op);											// 面法線ﾍﾞｸﾄﾙ取得
		}
	} else {															// <<ｼｪｰﾃﾞｨﾝｸﾞﾓｰﾄﾞ以外>>
		lno = GetListNo(op);											// ﾘｽﾄNo取得
		num1 = GetFlpNum1(op);											// 面ﾙｰﾌﾟ1数取得
		num2 = GetFlpNum2(op);											// 面ﾙｰﾌﾟ2数取得
		if(num1<2) {
			if(lno!=0) {
				glDeleteLists(lno, 1);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
				PutListNo(op, 0);
			}
			return;
		}
		ed = CheckObjPtr(2, op);										// 編集立体ﾁｪｯｸ
		if(lno==0) {
			lno = glGenLists(1);										// 新規取得
			PutListNo(op, lno);
		}
		if(m_DispMode==0||(m_DispMode==2&&ed==1)||
           (m_DispMode==3&&ed==0)||num2==0) {							// <面ﾙｰﾌﾟ1ﾃﾞｰﾀ>
			dmd = (GetObjAtr(op)/10==1) ? GL_LINE_STRIP : GL_POLYGON;	// 線/面ﾓｰﾄﾞ
			glNewList(lno, GL_COMPILE);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ開始
				if(GetSelNo(op)==2&&(m_ULineCnt>0||m_VLineCnt>0)) {		// <制御点立体で指定有>
					AddLineColorList(op);								// U/Vﾗｲﾝ色変更
				}
				glBegin(dmd);											// ﾙｰﾌﾟ開始
					for(i=0; i<num1-1; i++) {
						GetFlpVtx1(op, i, &pnt, &flg);					// 頂点座標取得
						if(flg==1) {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);	// 頂点登録
							glEnd();									// ﾙｰﾌﾟ終了
							glBegin(dmd);
						} else {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
						}
					}
					GetFlpVtx1(op, i, &pnt, &flg);
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
				glEnd();
			glEndList();												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ終了
			if(num2!=0) {
				PutListMode(op, 1);										// ﾘｽﾄﾓｰﾄﾞ設定
			} else {
				PutListMode(op, 0);
			}
		} else {														// <面ﾙｰﾌﾟ2ﾃﾞｰﾀ>
			glNewList(lno, GL_COMPILE);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ開始
				glBegin(GL_LINE_STRIP);									// ﾙｰﾌﾟ開始
					for(i=0; i<num2-1; i++) {
						GetFlpVtx2(op, i, &pnt, &flg);					// 頂点座標の取得
						if(flg==1) {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);	// 頂点登録
							glEnd();									// ﾙｰﾌﾟ終了
							glBegin(GL_LINE_STRIP);
						} else {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
						}
					}
					GetFlpVtx2(op, i, &pnt, &flg);
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
				glEnd();
			glEndList();
			PutListMode(op, 2);											// ﾘｽﾄﾓｰﾄﾞ設定
		}
	}
}

/***********************************************************/
void CJcad3GlbView::SetRenderList(OBJTYPE* op, VECTYPE* vc)				//<<< ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
/***********************************************************/
{
	PNTTYPE pt;
	UINT    lno, dmd;
	int     i, ln, fg;

	lno = GetListNo(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	ln = GetFlpNum1(op);												// 立体面ﾙｰﾌﾟ数取得1
	if(ln<2) {															// 面ﾙｰﾌﾟ数が2未満
		if(lno!=0) {													// ﾘｽﾄ番号が定義済み
			glDeleteLists(lno, 1);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
			PutListNo(op, 0);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号初期化
		}
		return;
	}
	if(lno==0) {
		lno = glGenLists(1);											// 有効ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
		PutListNo(op, lno);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号指定
	}
	dmd = (GetObjAtr(op)/10==1) ? GL_LINE_STRIP : GL_POLYGON;			// ﾗｲﾝ/ﾎﾟﾘｺﾞﾝ描画指定
	glNewList(lno, GL_COMPILE);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		glBegin(dmd);
			for(i=0; i<ln-1; i++) {
				GetFlpVtx1(op, i, &pt, &fg);							// 指定面ﾙｰﾌﾟ座標取得
				if(fg==1) {
					glNormal3d(vc[i].p[0], vc[i].p[1], vc[i].p[2]);
					glVertex3d(pt.p[0], pt.p[1], pt.p[2]);				// 頂点座標登録
					glEnd();											// 1面終了
					glBegin(dmd);										// 次の面へ
				} else {
					glNormal3d(vc[i].p[0], vc[i].p[1], vc[i].p[2]);
					glVertex3d(pt.p[0], pt.p[1], pt.p[2]);				// 頂点座標登録
				}
			}
			GetFlpVtx1(op, i, &pt, &fg);								// 指定面ﾙｰﾌﾟ座標取得
			glNormal3d(vc[i].p[0], vc[i].p[1], vc[i].p[2]);
			glVertex3d(pt.p[0], pt.p[1], pt.p[2]);						// 最後頂点座標登録
		glEnd();
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	PutListMode(op, 3);													// ﾘｽﾄﾓｰﾄﾞ設定
}

/********************************************/
void CJcad3GlbView::DelDispList(OBJTYPE* op)							//<<< ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
/********************************************/
{
	UINT lno;

	if((lno=GetListNo(op))!=0) {										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
		glDeleteLists(lno, 1);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
		PutListNo(op, 0);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号設定
	}
}

/*******************************************/
void CJcad3GlbView::InitLineNo(OBJTYPE* op)								//<<< ﾗｲﾝ番号初期化
/*******************************************/
{
	m_ULineCnt = 0, m_VLineCnt = 0;
	if(op!=NULL) {
		SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ登録
	}
}

/********************************************************/
void CJcad3GlbView::SetLineNo(int flg, int cnt, int *no)				//<<< ﾗｲﾝ番号設定
/********************************************************/
{
	int i;

	if(!flg) {															// <Uﾗｲﾝの場合>
		m_ULineCnt = cnt;												// Uﾗｲﾝ数設定
		for(i=0; i<cnt; i++) {
			m_ULineNo[i] = no[i];										// Uﾗｲﾝ番号設定
		}
	} else {															// <Vﾗｲﾝの場合>
		m_VLineCnt = cnt;												// Vﾗｲﾝ数設定
		for(i=0; i<cnt; i++) {
			m_VLineNo[i] = no[i];										// Vﾗｲﾝ番号設定
		}
	}
}

/*************************************************/
void CJcad3GlbView::AddLineColorList(OBJTYPE* op)						//<<< ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
/*************************************************/						// <U/Vﾗｲﾝ色変更>
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	UINT     dmode;
	int      un, vn, ub, vb, i, j, ln, st=0;
	double   rc, gc, bc;

	BaseCmdPtr(1, op, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	un = GetParaInt(cmp, 0);											// U制御点数取得
	vn = GetParaInt(cmp, 1);											// V制御点数取得
	ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	MainWnd->GetColor(MainWnd->SelLineColor, &rc, &gc, &bc);			// 選択稜線ｶﾗｰ取得
	dmode = GL_LINE_STRIP;												// 線ﾓｰﾄﾞ
	if(m_ULineCnt>0) {													// <<Uﾗｲﾝ指定時>>
		for(i=0; i<m_ULineCnt; i++) {
			ln = m_ULineNo[i];											// Uﾗｲﾝ番号
			if(ln>=un) {												// 無効番号中止
				break;
			}
			if(!st) {													// <初期処理>
				glLineWidth(1.05f);										// 線幅設定(太め)
				glColor3d(rc, gc, bc);									// 色設定
				st = 1;
			}
			glBegin(dmode);												// ﾙｰﾌﾟ開始
				for(j=0; j<vn; j++) {
					GetVtx(op, (ln+(j*un)), &pnt);						// 頂点座標取得
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// 頂点登録
				}
				if(vb) {												// <V:閉の場合>
					GetVtx(op, ln, &pnt);								// 開始点座標取得
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// 頂点登録
				}
			glEnd();													// ﾙｰﾌﾟ終了
		}
	}
	if(m_VLineCnt>0) {													// <<Vﾗｲﾝ指定時>>
		for(i=0; i<m_VLineCnt; i++) {
			ln = m_VLineNo[i];											// Vﾗｲﾝ番号
			if(ln>=vn) {												// 無効番号中止
				break;
			}
			if(!st) {													// <初期処理>
				glLineWidth(1.05f);										// 線幅設定(太め)
				glColor3d(rc, gc, bc);									// 色設定
				st = 1;
			}
			glBegin(dmode);												// ﾙｰﾌﾟ開始
				for(j=0; j<un; j++) {
					GetVtx(op, ((ln*un)+j), &pnt);						// 頂点座標取得
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// 頂点登録
				}
				if(ub) {												// <U:閉の場合>
					GetVtx(op, (ln*un), &pnt);							// 開始点座標取得
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// 頂点登録
				}
			glEnd();													// ﾙｰﾌﾟ終了
		}
	}
	if(st) {															// <終了処理>
		MainWnd->GetColor(MainWnd->ChooseColor, &rc, &gc, &bc);
		glLineWidth(1.0f);												// 線幅設定(標準に戻す)
		glColor3d(rc, gc, bc);											// 色設定
	}
}

/*******************************************/
void CJcad3GlbView::SetRenderMode(int mode)								//<<< ﾚﾝﾀﾞﾘﾝｸﾞ設定
/*******************************************/
{
	m_Rendering = mode;
	if(mode==2) {														// <ﾃｸｽﾁｬ･ﾚﾝﾀﾞﾘﾝｸﾞ>
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, HBMP, VBMP, 0, GL_RGB, GL_UNSIGNED_BYTE, TexImage);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glDisable(GL_COLOR_MATERIAL);
	} else {															// <ｼｪｰﾃﾞｨﾝｸﾞ/ﾜｲﾔｰﾌﾚｰﾑ>
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

/************************************/
void CJcad3GlbView::InitNCDisp(void)									//<<< NCﾃﾞｰﾀ表示初期処理
/************************************/
{
	if(m_NCLIST==0) {
		m_NCLIST = glGenLists(1);										// 新規取得
	}
	m_Ncdflg = 1;														// NC表示ﾌﾗｸﾞON
	m_Ncpflg = 0;														// NC登録ﾌﾗｸﾞ初期化
	glDrawBuffer(GL_FRONT);												// 表示ﾊﾞｯﾌｧﾌﾛﾝﾄ
}

/*******************************************************/
void CJcad3GlbView::SetNCDisp(PNTTYPE spt, PNTTYPE ept)					//<<< NCﾃﾞｰﾀ登録
/*******************************************************/
{
	if(m_Ncpflg==0) {													// <<<登録開始>>>
		glNewList(m_NCLIST, GL_COMPILE);								// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ開始
			glBegin(GL_LINE_STRIP);										// ﾙｰﾌﾟ開始
				glVertex3d(spt.p[0], spt.p[1], spt.p[2]);				// 座標登録
				glVertex3d(ept.p[0], ept.p[1], ept.p[2]);
		m_Ncpflg = 1;													// 登録開始済
		m_Ncpnt = ept;													// 終了点保存
	} else {															// <<<登録中>>>
		if(fabs(m_Ncpnt.p[0]-spt.p[0])>EPSILON||
           fabs(m_Ncpnt.p[1]-spt.p[1])>EPSILON||
           fabs(m_Ncpnt.p[2]-spt.p[2])>EPSILON) {
			glEnd();													// ﾙｰﾌﾟ終了
			glBegin(GL_LINE_STRIP);										// ﾙｰﾌﾟ開始
				glVertex3d(spt.p[0], spt.p[1], spt.p[2]);				// 座標登録
		}
		glVertex3d(ept.p[0], ept.p[1], ept.p[2]);
		m_Ncpnt = ept;													// 終了点保存
	}
}

/***************************************/
void CJcad3GlbView::DispNCDisp(int col)									//<<< NCﾃﾞｰﾀ表示
/***************************************/
{
	int    i;
	UINT   cno;
	double rc, gc, bc;

	if(m_Ncpflg==1) {													// <<<ﾃﾞｰﾀ登録済>>>
			glEnd();													// ﾙｰﾌﾟ終了
		glEndList();													// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ終了
		cno = (col==0) ? MainWnd->NCDispColor1 : MainWnd->NCDispColor2;
		MainWnd->GetColor(cno, &rc, &gc, &bc);							// 表示色取得
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		for(i=0; i<3; i++) {											// 3面分
			glPushMatrix();
				glLoadIdentity();										// ﾋﾞｭｰﾎﾟｰﾄ設定
				glViewport(PlRect[i].left, ViewSize.cy-PlRect[i].bottom-1,
			               PlRect[i].Width(), PlRect[i].Height());
				glCallList(DL_ORTHOXY+i);								// 正射影変換
				glCallList(DL_XY+i);									// ﾓﾃﾞﾘﾝｸﾞ変換
				glPushMatrix();
					glColor3d(rc, gc, bc);								// 表示色設定
					glCallList(m_NCLIST);								// 立体描画
				glPopMatrix();
			glPopMatrix();
		}
		glPushMatrix();
			glLoadIdentity();											// ﾋﾞｭｰﾎﾟｰﾄ設定
			glViewport(PlRect[PRS].left, ViewSize.cy-PlRect[PRS].bottom-1,
			           PlRect[PRS].Width(), PlRect[PRS].Height());
			glCallList(DL_PARS);										// 透視法射影&ﾓﾃﾞﾘﾝｸﾞ変換
			glPushMatrix();
				glColor3d(rc, gc, bc);									// 表示色設定
				glCallList(m_NCLIST);									// 立体描画
			glPopMatrix();
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
		glFinish();
	}
	m_Ncpflg = 0;														// NC登録ﾌﾗｸﾞ初期化
}

/***********************************/
void CJcad3GlbView::EndNCDisp(void)										//<<< NCﾃﾞｰﾀ表示終了処理
/***********************************/
{
	if(m_NCLIST!=0) {													// <<ﾘｽﾄ登録済み>>
		glDeleteLists(m_NCLIST, 1);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
		m_NCLIST = 0;													// ﾘｽﾄ番号ｸﾘｱ
	}
	if(m_Ncdflg==1) {													// <<ﾃﾞｰﾀ表示中>>
		glDrawBuffer(GL_BACK);											// 表示ﾊﾞｯﾌｧﾊﾞｯｸ
		m_Ncdflg = 0;													// NC表示ﾌﾗｸﾞOFF
	}
}

/***************************************/
CPoint CJcad3GlbView::GetPlCenter(void)									//<<< 画面ｾﾝﾀｰ取得
/***************************************/								//    (ｽｸﾘｰﾝ座標)
{
	CPoint point = PlRect[XY].BottomRight();
	ClientToScreen(&point);
	return point;
}

/***************************************/
void CJcad3GlbView::CheckCtrlKey1(void)									//<<< 制御ｷｰ確認(ﾒﾆｭｰ起動時)
/***************************************/
{
	m_CopyInp = m_CopyBtn;
	m_NumInp = (m_CChar&m_NumKey) ? 1 : m_NumBtn;						// 数値ｼｮｰﾄｶｯﾄｷｰ押下
	if((m_CChar&m_NNuKey)&&m_CopyInp==0) {								// 複写ｼｮｰﾄｶｯﾄｷｰ押下
		m_CopyInp = (m_BtnEnv&0x4) ? 1 : 2;								// ﾀﾞｲｱﾛｸﾞ表示なし/あり
	}
	m_NNuInp = (m_CChar&m_NNuKey) ? 1 : 0;								// 数値ｷｰ以外ｷｰ押下
}

/*******************************************/
void CJcad3GlbView::CheckCtrlKey2(UINT flg)								//<<< 制御ｷｰ確認(制御点選択時)
/*******************************************/
{
	m_CtlInp = (flg&m_CtlKey) ? 1 : 0;									// 制御点追加/解除ｷｰ押下
	m_CurInp = (flg&m_CurKey) ? 1 : 0;									// 折線点/ﾓｰﾄﾞ変更ｷｰ押下
}

/***************************************/
void CJcad3GlbView::SetCopyBtn(int cop)									//<<< 複写モード設定
/***************************************/
{
	if(cop==1) {
		m_CopyBtn = (m_BtnEnv&0x4) ? ((m_FChar&0x1) ? 2 : 1)			// ﾀﾞｲｱﾛｸﾞ表示なし([F5]:反転)
                                   : ((m_FChar&0x1) ? 1 : 2);			// ﾀﾞｲｱﾛｸﾞ表示あり([F5]:反転)
	} else {
		m_CopyBtn = 0;
	}
	if(m_SFuncMode<3) {													// <詳細>
		m_pFunction->OnOffButton(8, cop);								// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
	} else if(m_SFuncMode<6) {											// <簡易>
		m_pFunction2->OnOffButton(8, cop);								// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
	}
}

/**************************************/
void CJcad3GlbView::SetNumBtn(int num)									//<<< 数値モード設定
/**************************************/
{
	m_NumBtn = num;
	if(m_SFuncMode<3) {													// <詳細>
		m_pFunction->OnOffButton(9, num);								// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ更新
	} else if(m_SFuncMode<6) {											// <簡易>
		m_pFunction2->OnOffButton(9, num);								// ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ2更新
	}
}

/********************************************/
int CJcad3GlbView::CheckScrChg(CPoint point)							//<<< ｽｸﾘｰﾝ変更確認
/********************************************/
{
	int ret=0;

	if(PlRect[XY].right<=point.x&&point.x<=PlRect[PRS].left) {
		if((PlRect[XY].top<=point.y&&point.y<=PlRect[XY].bottom)||
           (PlRect[YZ].top<=point.y&&point.y<=PlRect[YZ].bottom)) {		// <XY-PRS,YZ-ZXの境界>
			ret = 1;
		}
	} else if(PlRect[XY].bottom<=point.y&&point.y<=PlRect[YZ].top) {
		if((PlRect[XY].left<=point.x&&point.x<=PlRect[XY].right)||
           (PlRect[PRS].left<=point.x&&point.x<=PlRect[PRS].right)) {	// <XY-YZ, PRS-ZXの境界>
			ret = 2;
		}
	}
	return ret;
}

/*******************************************************/
void CJcad3GlbView::ChangeScreen(int flg, CPoint point)					//<<< ｽｸﾘｰﾝ変更処理
/*******************************************************/
{
	if(flg==1) {														// <横方向移動>
		ScrDiv1 = (double)point.x/ViewSize.cx;
		ScrDiv1 = (ScrDiv1<0.01) ? 0.0 : ScrDiv1;
		ScrDiv1 = (ScrDiv1>0.99) ? 1.0 : ScrDiv1;
	} else {															// <縦方向移動>
		ScrDiv2 = (double)point.y/ViewSize.cy;
		ScrDiv2 = (ScrDiv2<0.01) ? 0.0 : ScrDiv2;
		ScrDiv2 = (ScrDiv2>0.99) ? 1.0 : ScrDiv2;
	}
	InitScreen(ViewSize.cx, ViewSize.cy);								// ｽｸﾘｰﾝ初期処理
	Display(0);															// 再描画
}
