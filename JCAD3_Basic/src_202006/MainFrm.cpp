/////////////////////////////////////////////////////////////////////////////
// メインフレームウィンドウクラス(CMainFrame)定義
// MainFrm.cpp
//---------------------------------------------------------------------------
// LastEdit: 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <process.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "Splash.h"
#include "FileDlg.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "ButtonDlg.h"
#include "SetSysEnv.h"
#include "SetRndEnv.h"
#include "SetRayEnv.h"
#include "SetPovEnv.h"
#include "CalcDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataFile1.h"
#include "DataFile2.h"
#include "DataFile3.h"
#include "Select.h"
#include "Primitive.h"
#include "CreatPovData.h"
#include "ExportStl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMainFrame* MainWnd;

/*********************************************/
void ReadWindowPlacement(WINDOWPLACEMENT* wp)							//<<< ｳｨﾝﾄﾞｳ表示状態読込
/*********************************************/
{
	UINT length = sizeof(UINT)*3 + sizeof(POINT)*2 + sizeof(RECT);
	wp->length = theApp.GetProfileInt("LAYOUT", "Length", length);
	wp->flags = theApp.GetProfileInt("LAYOUT", "Flags", WPF_SETMINPOSITION|WPF_RESTORETOMAXIMIZED);
	wp->showCmd = theApp.GetProfileInt("LAYOUT", "Show", SW_SHOWNORMAL);
	wp->ptMinPosition.x = theApp.GetProfileInt("LAYOUT", "MinPosLeft", -1);
	wp->ptMinPosition.y = theApp.GetProfileInt("LAYOUT", "MinPosTop", -1);
	wp->ptMaxPosition.x = theApp.GetProfileInt("LAYOUT", "MaxPosLeft", -1);
	wp->ptMaxPosition.y = theApp.GetProfileInt("LAYOUT", "MaxPosTop", -1);
	wp->rcNormalPosition.left = theApp.GetProfileInt("LAYOUT", "MainWinLeft", 0);
	wp->rcNormalPosition.top = theApp.GetProfileInt("LAYOUT", "MainWinTop", 0);
	wp->rcNormalPosition.right = theApp.GetProfileInt("LAYOUT", "MainWinRight", 1024); 
	wp->rcNormalPosition.bottom = theApp.GetProfileInt("LAYOUT", "MainWinBottom", 768); 
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)								// メッセージ・マップ
	ON_UPDATE_COMMAND_UI_RANGE(IDW_JCAD_EDIT, IDW_MEASURE, OnUpdateMenuCtrlBar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_GROUP_SET, ID_GROUP_RESET, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ZOOM_BOX, ID_SCROLL, OnUpdateMenuCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ALLSHADING, ID_RENDERING, OnUpdateMenuEnableOrCheck2)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_CURVE, ID_CRT_EXTRCURVE, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CRT_SWEEP, ID_CRT_SETSECTION, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_SPACE, ID_EDIT_LINES, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_LINE, ID_EDIT_PCUT, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SURF_CPS, ID_SURF_S_ADJUST, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_JWL_STONE, ID_JWL_ARRANGE, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MEASURE_DIST, ID_MEASURE_ANGLE, OnUpdateMeasureCheck)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MEASURE_SCALE, ID_MEASURE_GRAD, OnUpdateMeasurePlaneCheck)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURPOS, OnUpdateCurPos)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_GROUP, OnUpdateGroup)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CRTPRM, OnUpdateCrtPrm)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_GRID, OnUpdateGrid)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FUNC, OnUpdateFunc)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYOBJ, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUTOBJ, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTEOBJ, OnUpdatePasteObject)
	ON_UPDATE_COMMAND_UI(ID_SELECT_OBJ, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI(ID_OBJ_INFO, OnUpdateObjInfo)
	ON_UPDATE_COMMAND_UI(ID_FUNCTION_BTN, OnUpdateFunctionBtn)
	ON_UPDATE_COMMAND_UI(ID_COPY_INPUT, OnUpdateCopyInput)
	ON_UPDATE_COMMAND_UI(ID_NUM_INPUT, OnUpdateNumInput)
	ON_UPDATE_COMMAND_UI(ID_SCR_STATE, OnUpdateScrState)
	ON_UPDATE_COMMAND_UI(ID_DATA_USE, OnUpdateDataUse)
	ON_UPDATE_COMMAND_UI(ID_PARAM_DISP, OnUpdateParamDisp)
	ON_UPDATE_COMMAND_UI(ID_SCROLL_TOOL, OnUpdateScrollTool)
	ON_UPDATE_COMMAND_UI(ID_EYE_CHANGE, OnUpdateEyeChange)
	ON_UPDATE_COMMAND_UI(ID_GRID_DISP, OnUpdateGridDisp)
	ON_UPDATE_COMMAND_UI(ID_GRID_BIND, OnUpdateGridBind)
	ON_UPDATE_COMMAND_UI(ID_BASE_SET, OnUpdateMenuCheck)
	ON_UPDATE_COMMAND_UI(ID_RAYTRACE, OnUpdateRayTrace)
	ON_UPDATE_COMMAND_UI(ID_POVDATA, OnUpdateMenuEnableOrCheck2)
	ON_UPDATE_COMMAND_UI(ID_COLOR, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI(ID_CRT_CURVE, OnUpdateMenuCheck)
	ON_UPDATE_COMMAND_UI(ID_CRT_CENTCURVE, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_CRT_SETCURVE, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_CRT_SECTION, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_CRT_SPIRAL, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_RESET_SURFACE, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_CRT_CONNECT, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_CRT_THICK, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SURFACE, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BOOL, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_SURF_O_ADJUST, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_SURF_PARAM, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_SURF_TENSION, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI(ID_JWL_RING, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_JWL_GEM, OnUpdateMenuCheck)
	ON_UPDATE_COMMAND_UI(ID_JWL_UNIT, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_JWL_EARTH, OnUpdateMenuEnableOrCheck)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_CALC, OnUpdateMeasureCalc)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_VOLUME, OnUpdateMenuEnable)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_CURVE, OnUpdateMeasureEnableOrCheck)
	ON_UPDATE_COMMAND_UI(ID_NC_SIMULATE, OnUpdateMenuEnable)
	ON_COMMAND_EX_RANGE(IDW_JCAD_EDIT, IDW_MEASURE, OnViewMenuCtrlBar)
	ON_COMMAND_EX_RANGE(ID_EDIT_PASTEOBJ, ID_EDIT_PASTEOBJ, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_SELECT_OBJ, ID_SELECT_OBJ, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_GROUP_SET, ID_GROUP_RESET, OnGroupMenu)
	ON_COMMAND_EX_RANGE(ID_ZOOM_BOX, ID_SCROLL, OnScrCtrlMenu)
	ON_COMMAND_EX_RANGE(ID_BASE_SET, ID_BASE_SET, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_ALLSHADING, ID_RENDERING, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_CRT_CURVE, ID_CRT_EXTRCURVE, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_CRT_SWEEP, ID_CRT_SETSECTION, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_EDIT_SPACE, ID_EDIT_LINES, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_EDIT_LINE, ID_EDIT_PCUT, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_SURF_CPS, ID_SURF_S_ADJUST, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_SURF_TENSION, ID_SURF_TENSION, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_JWL_STONE, ID_JWL_GEM, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_JWL_EARTH, ID_JWL_EARTH, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_MEASURE_DIST, ID_MEASURE_CURVE, OnCommonMenu)
	ON_COMMAND_EX_RANGE(ID_MEASURE_SCALE, ID_MEASURE_GRAD, OnMeasureMenu)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_VIEWER_SAVE, OnViewerSave)
	ON_COMMAND(ID_EXPORT_SLC, OnExportSLC)
	ON_COMMAND(ID_CAPTURE_SET, OnCaptureSet)
	ON_COMMAND(ID_PASSWORD_SET, OnPasswordSet)
	ON_COMMAND(ID_REST_BACKUP, OnRestBackUp)
	ON_COMMAND(ID_EDIT_UNDO, OnUndo)
	ON_COMMAND(ID_EDIT_REDO, OnRedo)
	ON_COMMAND(ID_EDIT_COPYOBJ, OnCopyObject)
	ON_COMMAND(ID_EDIT_CUTOBJ, OnCutObject)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_ALL_PIC, OnAllPic)
	ON_COMMAND(ID_OBJ_INFO, OnObjInfo)
	ON_COMMAND(ID_FUNCTION_BTN, OnFunctionBtn)
	ON_COMMAND(ID_COPY_INPUT, OnCopyInput)
	ON_COMMAND(ID_NUM_INPUT, OnNumInput)
	ON_COMMAND(ID_DISP_MODE, OnDispMode)
	ON_COMMAND(ID_DISP_RANGE, OnDispRange)
	ON_COMMAND(ID_ALL_DISP, OnAllDisp)
	ON_COMMAND(ID_ALL_ERASE, OnAllErase)
	ON_COMMAND(ID_SCR_STATE, OnScrState)
	ON_COMMAND(ID_DATA_USE, OnDataUse)
	ON_COMMAND(ID_PARAM_DISP, OnParamDisp)
	ON_COMMAND(ID_GROUP_DSP, OnGroupDsp)
	ON_COMMAND(ID_GROUP_SEL, OnGroupSel)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_AUTO_ZOOM, OnAutoZoom)
	ON_COMMAND(ID_EXACT_ZOOM, OnExactZoom)
	ON_COMMAND(ID_SCROLL_TOOL, OnScrollTool)
	ON_COMMAND(ID_HOME_POS, OnHomePos)
	ON_COMMAND(ID_BACKSCREEN, OnBackScreen)
	ON_COMMAND(ID_TRUCK_UP, OnTruckUp)
	ON_COMMAND(ID_TRUCK_DOWN, OnTruckDown)
	ON_COMMAND(ID_CENT_PARS, OnCentPars)
	ON_COMMAND(ID_EYE_SET, OnEyeSet)
	ON_COMMAND(ID_EYE_CHANGE, OnEyeChange)
	ON_COMMAND(ID_GRID_DISP, OnGridDisp)
	ON_COMMAND(ID_GRID_BIND, OnGridBind)
	ON_COMMAND(ID_GRID_SPACE, OnGridSpace)
	ON_COMMAND(ID_DSP_GLB, OnGuideLineBox)
	ON_COMMAND(ID_DSP_TEMP, OnDspTemplate)
	ON_COMMAND(ID_SET_TEMP, OnSetTemplate)
	ON_COMMAND(ID_TEXTURE, OnTexture)
	ON_COMMAND(ID_SET_HOME, OnSetHome)
	ON_COMMAND(ID_RAYTRACE, OnRayTrace)
	ON_COMMAND(ID_POVDATA, OnPovData)
	ON_COMMAND(ID_COLOR, OnMaterial)
	ON_COMMAND(ID_CRT_PRIMITIVE, OnCrtPrimitive)
	ON_COMMAND(ID_CRT_CENTCURVE, OnCrtCentCurve)
	ON_COMMAND(ID_CRT_SETCURVE, OnCrtSetCurve)
	ON_COMMAND(ID_CRT_SECTION, OnCrtSection)
	ON_COMMAND(ID_CRT_SPIRAL, OnCrtSpiral)
	ON_COMMAND(ID_RESET_SURFACE, OnResetSurface)
	ON_COMMAND(ID_CRT_CONNECT, OnCrtConnect)
	ON_COMMAND(ID_CRT_THICK, OnCrtThick)
	ON_COMMAND(ID_CRT_TEXT, OnCrtText)
	ON_COMMAND(ID_EDIT_SURFACE, OnEditSurface)
	ON_COMMAND(ID_EDIT_BOOL, OnEditBool)
	ON_COMMAND(ID_SURF_O_ADJUST, OnSurfOAdjust)
	ON_COMMAND(ID_SURF_PARAM, OnSurfParam)
	ON_COMMAND(ID_JWL_RING, OnJewelryRing)
	ON_COMMAND(ID_JWL_UNIT, OnSetUnit)
	ON_COMMAND(ID_MEASURE_CALC, OnMeasureCalc)
	ON_COMMAND(ID_MEASURE_VOLUME, OnMeasureVolume)
	ON_COMMAND(ID_NC_SIMULATE, OnSimulation)
	ON_COMMAND(ID_NC_DISP, OnNcDisp)
	ON_COMMAND(ID_SET_ENV, OnSetEnvironment)
	ON_COMMAND(ID_SET_RENDER, OnSetRender)
	ON_COMMAND(ID_SET_RAYTRACE, OnSetRayTrace)
	ON_COMMAND(ID_REF_MANUAL, OnRefManual)
	ON_COMMAND(ID_REGIST_USER, OnRegistUser)
	//
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_RBUTTONUP()
	// グローバル ヘルプ コマンド
//	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,														// ｽﾃｰﾀｽﾗｲﾝｲﾝｼﾞｹｰﾀ
	ID_INDICATOR_CURPOS,												// ｶｰｿﾙ位置ｲﾝｼﾞｹｰﾀ
	ID_INDICATOR_GROUP,													// ｸﾞﾙｰﾌﾟｲﾝｼﾞｹｰﾀ
	ID_INDICATOR_CRTPRM,												// 生成ﾊﾟﾗﾒｰﾀｲﾝｼﾞｹｰﾀ
	ID_INDICATOR_GRID,													// ｸﾞﾘｯﾄﾞｲﾝｼﾞｹｰﾀ
	ID_INDICATOR_FUNC,													// ﾌｧﾝｸｼｮﾝｲﾝｼﾞｹｰﾀ
//	ID_INDICATOR_CAPS,													// Caps Lockｲﾝｼﾞｹｰﾀ
//	ID_INDICATOR_NUM,													// Num Lockｲﾝｼﾞｹｰﾀ
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame コンストラクション/デストラクション

/****************************/
CMainFrame::CMainFrame(void)											//<<< ｸﾗｽの構築
/****************************/
{
	int i;

	MainWnd = this;
	m_nStatusPane1Width = -1;
	m_sFilePath = GetAppPath(0);
	memset(m_cWPassword, NULL, sizeof(m_cWPassword));
	memset(m_cRPassword, NULL, sizeof(m_cRPassword));
	m_PasswordKind = 0, m_PasswordSave = 0;
	m_OpenKind = 0, m_SaveKind = 0, m_ViewerKind = 0;
	m_XPos = 0.0, m_YPos = 0.0, m_ZPos = 0.0;							// ｶｰｿﾙ位置原点
	m_CrtD = 0, m_CrtW = 0.0f;											// 生成ﾊﾟﾗﾒｰﾀ
	for(i=0; i<64; i++) {
		m_Color[i] = RGB(255, 255, 255);								// ｶﾗｰ初期化(全て白)
	}
	ReadFilPathFile();													// ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ読込
	InitSColorFile();													// 初期ｼｽﾃﾑｶﾗｰﾌｧｲﾙ読込
	InitColor();														// 表示色設定
	SetInitFlg(1);														// 初期起動状態ﾌﾗｸﾞON
}

/*****************************/
CMainFrame::~CMainFrame(void)											//<<< ｸﾗｽの消滅
/*****************************/
{
	MainWnd = NULL;
	FreeMem();															// ﾒﾓﾘ解放(ｼｽﾃﾑ終了時)
}

/*******************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)					//<<< WM_CREATE ﾒｯｾｰｼﾞ応答処理
/*******************************************************/
{
	WINDOWPLACEMENT wp;
	CRect   rect;
	CString str;

	CSplashWnd::ShowSplashScreen(this);									// 起動画面表示
	if(CFrameWnd::OnCreate(lpCreateStruct)==-1) {
		return -1;														// ﾒｲﾝ･ｳｨﾝﾄﾞｳ作成
	}
	if(!InitStatusBar(1)) {
		return -1;														// ｽﾃｰﾀｽﾊﾞｰ作成&初期化
	}
	if(!InitToolBar(&m_wndToolBar, IDR_MAINFRAME, IDS_MAINFRAME_T)) {
		return -1;
	}
	if(!InitToolBar(&m_wndJcadEdit, IDR_JCAD_EDIT, IDS_JCAD_EDIT_T, CBRS_BOTTOM, IDW_JCAD_EDIT)) {
		return -1;
	}

	if(!InitToolBar(&m_wndView, IDR_VIEW, IDS_VIEW_T, CBRS_TOP, IDW_VIEW)) {
		return -1;
	}

	if(!InitToolBar(&m_wndGroup, IDR_GROUP, IDS_GROUP_T, CBRS_TOP, IDW_GROUP)) {
		return -1;
	}

	if(!InitToolBar(&m_wndScreenCtl, IDR_SCREEN_CTRL, IDS_SCREEN_CTRL_T, CBRS_TOP, IDW_SCREEN_CTRL)) {
		return -1;
	}
	if(!InitToolBar(&m_wndRendering, IDR_RENDERING, IDS_RENDERING_T, CBRS_TOP, IDW_RENDERING)) {
		return -1;
	}
	if(!InitToolBar(&m_wndCreate, IDR_MENU_CRT, IDS_MENU_CRT_T, CBRS_BOTTOM, IDW_MENU_CRT)) {
		return -1;
	}
	if(!InitToolBar(&m_wndEdit, IDR_MENU_EDIT, IDS_MENU_EDIT_T, CBRS_BOTTOM, IDW_MENU_EDIT)) {
		return -1;
	}
	if(!InitToolBar(&m_wndSurf, IDR_MENU_SURF, IDS_MENU_SURF_T, CBRS_RIGHT, IDW_MENU_SURF)) {
		return -1;
	}
	if(!InitToolBar(&m_wndJewelry, IDR_JEWELRY, IDS_JEWELRY_T, CBRS_RIGHT, IDW_JEWELRY)) {
		return -1;
	}
	if(!InitToolBar(&m_wndMeasure, IDR_MEASURE, IDS_MEASURE_T, CBRS_RIGHT, IDW_MEASURE)) {
		return -1;
	}
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP);					// ﾂｰﾙﾊﾞｰ ﾄﾞｯｷﾝｸﾞ
	DockControlBar(&m_wndSurf, AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_wndCreate, AFX_IDW_DOCKBAR_BOTTOM);
	RecalcLayout();
	DockControlBarLeftOf(&m_wndView, &m_wndToolBar);
//	DockControlBarLeftOf(&m_wndGroup, &m_wndView);
//	DockControlBarLeftOf(&m_wndRendering, &m_wndGroup);
	DockControlBarLeftOf(&m_wndRendering, &m_wndView);
	DockControlBarLeftOf(&m_wndScreenCtl, &m_wndRendering);
	DockControlBarLeftOf(&m_wndJewelry, &m_wndSurf);
	DockControlBarLeftOf(&m_wndMeasure, &m_wndJewelry);
	DockControlBarLeftOf(&m_wndEdit, &m_wndCreate);
	DockControlBarLeftOf(&m_wndJcadEdit, &m_wndEdit);
	ReadWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態読込
	if(wp.showCmd==SW_SHOWMAXIMIZED) {									// 起動時にｳｨﾝﾄﾞｳの
		wp.showCmd = SW_SHOWNORMAL;										// 最大表示は出来ないため
		GetDesktopWindow()->GetWindowRect(&rect);						// ﾃﾞｽｸﾄｯﾌﾟｳｨﾝﾄﾞｳ ｻｲｽﾞ
		wp.rcNormalPosition.left = rect.left;
		wp.rcNormalPosition.top = rect.top;
		wp.rcNormalPosition.right = rect.right;
		wp.rcNormalPosition.bottom = rect.bottom;
	} else if(wp.showCmd==SW_SHOWMINIMIZED) {							// 起動時にｳｨﾝﾄﾞｳの
		wp.showCmd = SW_SHOWNORMAL;										// ｱｲｺﾝ化は出来ないため元のｻｲｽﾞと位置に戻す
	}
	SetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態設定
//202006 リソース等変更直後でLoadBasState エラーになる。 Save後正常
//regedit HKEY_USERS\S-xxx\Software\GLB\JCAD3 Basic for Windows 以下を一旦消去
	LoadBarState("Control");											// ｺﾝﾄﾛｰﾙﾊﾞｰ表示状態設定
	Initialize();														// 初期化
	if(GetFileFlg()) {													// <ﾌｧｲﾙ指定起動>
		CString fname=GetFileName();									// 保存ﾌｧｲﾙ名取得
		DoOpenFile(fname);												// 指定ﾌｧｲﾙ読込み
	}
	return 0;
}

/**************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)						//<<< ｳｨﾝﾄﾞｳ作成前処理
/**************************************************/
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して、Window ｸﾗｽやｽﾀｲﾙを修正してください。
	cs.lpszClass = AfxRegisterWndClass(0,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

/********************************************************************/
LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)	//<<< WM_SETMESSAGESTRING ﾒｯｾｰｼﾞ応答処理
/********************************************************************/
{
	if(wParam>0&&wParam!=AFX_IDS_IDLEMESSAGE) {							// <有効なﾒﾆｭｰIDの場合>
		View->MessageOut((int)wParam);									// ﾌｧﾝｸｼｮﾝﾒｯｾｰｼﾞ出力
	}
	return CFrameWnd::OnSetMessageString(wParam, lParam);
}

/******************************/
void CMainFrame::OnClose(void)											//<<< WM_CLOSE ﾒｯｾｰｼﾞ応答処理
/******************************/
{
	WINDOWPLACEMENT wp;
	OBJTYPE* op;
	int      dsp, edt, chk=0;

	BaseObjPtr(0, &op, &dsp, &edt);										// 先頭立体のﾎﾟｲﾝﾀ取得
	while(op) {
		if(dsp==0) {													// 非表示有の場合
			chk = 1; break;
		}
		NextObjPtr(0, &op, &dsp, &edt);									// 次立体のﾎﾟｲﾝﾀ取得
	}
	if(chk) {															// <非表示立体有>
		if(AfxMessageBox(IDS_NODISP_OBJ, MB_YESNO)==IDNO) {				// ｢終了｣を中止
			return;
		}
	}
	GetActiveDocument()->SetModifiedFlag(CheckCmdFlg());
	SaveBarState("Control");											// ｺﾝﾄﾛｰﾙﾊﾞｰ表示状態保存
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "Length", wp.length);				// ｳｨﾝﾄﾞｳ表示状態保存
	theApp.WriteProfileInt("LAYOUT", "Flags", wp.flags);
	theApp.WriteProfileInt("LAYOUT", "Show", wp.showCmd);
	theApp.WriteProfileInt("LAYOUT", "MinPosLeft", wp.ptMinPosition.x);
	theApp.WriteProfileInt("LAYOUT", "MinPosTop", wp.ptMinPosition.y);
	theApp.WriteProfileInt("LAYOUT", "MaxPosLeft", wp.ptMaxPosition.x);
	theApp.WriteProfileInt("LAYOUT", "MaxPosTop", wp.ptMaxPosition.y);
	theApp.WriteProfileInt("LAYOUT", "MainWinLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "MainWinTop", wp.rcNormalPosition.top);
	theApp.WriteProfileInt("LAYOUT", "MainWinRight", wp.rcNormalPosition.right);
	theApp.WriteProfileInt("LAYOUT", "MainWinBottom", wp.rcNormalPosition.bottom);
	WriteFilPathFile();													// ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ書込
	WriteSColorFile();													// ｼｽﾃﾑｶﾗｰ保存ﾌｧｲﾙ書込
	CFrameWnd::OnClose();												// ﾒｲﾝ･ｳｨﾝﾄﾞｳの終了
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame コマンド ハンドラ

/********************************************/
BOOL CMainFrame::OnViewMenuCtrlBar(UINT nID)							//<<< ｺﾝﾄﾛｰﾙﾊﾞｰ表示/非表示
/********************************************/
{
	CControlBar* pBar = GetControlBar(nID);								// ｺﾏﾝﾄﾞIDからｺﾝﾄﾛｰﾙﾊﾞｰ取得
	if(pBar!=NULL) {													// ｺﾝﾄﾛｰﾙﾊﾞｰ表示/非表示切替
		ShowControlBar(pBar, (pBar->GetStyle()&WS_VISIBLE)==0, FALSE);
		return TRUE;
	}
	return FALSE;
}

/****************************************************/
void CMainFrame::OnUpdateMenuCtrlBar(CCmdUI* pCmdUI)					//<<< ｺﾝﾄﾛｰﾙﾊﾞｰ更新
/****************************************************/
{
	CControlBar* pBar = GetControlBar(pCmdUI->m_nID);					// ｺﾏﾝﾄﾞIDからｺﾝﾄﾛｰﾙﾊﾞｰ取得
	if(pBar!=NULL) {
		pCmdUI->SetCheck((pBar->GetStyle()&WS_VISIBLE)!=0);				// ﾒﾆｭｰﾊﾞｰのﾁｪｯｸ状態切替
		return;
	}
	pCmdUI->ContinueRouting();											// ｺﾏﾝﾄﾞのﾙｰﾃｨﾝｸﾞ
}

/***********************************************/
void CMainFrame::OnUpdateCurPos(CCmdUI* pCmdUI)							//<<< ｽﾃｰﾀｽﾊﾞｰｶｰｿﾙ位置ﾍﾟｲﾝ更新
/***********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strCurP;
	CSize   size;
	UINT    nID, nStyle;
	int     idx, nWidth;

	strCurP.Format(ID_INDICATOR_CURPOS, m_XPos, m_YPos, m_ZPos);		// ｶｰｿﾙ位置
	HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
	CClientDC dc(NULL);
	if(hFont!=NULL) {
		hOldFont = dc.SelectObject(hFont);
	}
	size = dc.GetTextExtent(strCurP);									// 現在文字列幅計算
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURPOS);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strCurP);											// 現在ｶｰｿﾙ位置ﾍﾟｲﾝ更新
	pCmdUI->Enable(TRUE);
}

/**********************************************/
void CMainFrame::OnUpdateGroup(CCmdUI* pCmdUI)							//<<< ｽﾃｰﾀｽﾊﾞｰｸﾞﾙｰﾌﾟﾍﾟｲﾝ更新
/**********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strGroup, strMax, str;
	CSize   size;
	UINT    nID, nStyle;
	int     i, idx, nWidth;

	strGroup = (m_GroupObj[0]) ? ((m_GroupDisp[0]) ? "1*" : "1 ")		// ｸﾞﾙｰﾌﾟ登録/未登録と
                               : "  ";									// 表示/非表示設定
	for(i=1; i<9; i++) {
		if(m_GroupObj[i]) {
			str.Format("%d", i+1);
			strGroup += str;
			strGroup += (m_GroupDisp[i]) ? "*" : " ";
		} else {
			strGroup += "  ";
		}
	}
	for(i=9; i<16; i++) {
		if(m_GroupObj[i]) {
			str.Format("%c", 0x58+i);
			strGroup += str;
			strGroup += (m_GroupDisp[i]) ? "*" : " ";
		} else {
			strGroup += "  ";
		}
	}
	HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
	CClientDC dc(NULL);
	if(hFont!=NULL) {
		hOldFont = dc.SelectObject(hFont);
	}
	strMax.LoadString(ID_INDICATOR_GROUP);
	size = dc.GetTextExtent(strMax);									// 最大文字列幅計算
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_GROUP);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strGroup);											// 現在ｸﾞﾙｰﾌﾟﾍﾟｲﾝ更新
	pCmdUI->Enable(TRUE);
}

/***********************************************/
void CMainFrame::OnUpdateCrtPrm(CCmdUI* pCmdUI)							//<<< ｽﾃｰﾀｽﾊﾞｰ生成ﾊﾟﾗﾒｰﾀﾍﾟｲﾝ更新
/***********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strCrtPrm;
	CSize   size;
	UINT    nID, nStyle;
	int     idx, nWidth;

	strCrtPrm.Format(ID_INDICATOR_CRTPRM, m_CrtD, m_CrtW);				// 生成ﾊﾟﾗﾒｰﾀ
	HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
	CClientDC dc(NULL);
	if(hFont!=NULL) {
		hOldFont = dc.SelectObject(hFont);
	}
	size = dc.GetTextExtent(strCrtPrm);									// 現在文字列幅計算
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CRTPRM);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strCrtPrm);											// 現在生成ﾊﾟﾗﾒｰﾀﾍﾟｲﾝ更新
	pCmdUI->Enable(TRUE);
}

/*********************************************/
void CMainFrame::OnUpdateGrid(CCmdUI* pCmdUI)							//<<< ｽﾃｰﾀｽﾊﾞｰｸﾞﾘｯﾄﾞﾍﾟｲﾝ更新
/*********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strGrid, strMax;
	CSize   size;
	UINT    nID, nStyle;
	int     idx, nWidth;

	strGrid = (View->GetGridDisp()) ? "G*" : "G ";						// ｸﾞﾘｯﾄﾞ/基準点/基準線の
	strGrid += (View->GetGridBind()) ? "*" : " ";						// 表示&拘束設定
	strMax.Format("%.2f ", View->GetGridSpace()/1000.0);				// ｸﾞﾘｯﾄﾞ間隔
	strGrid += strMax;
	strGrid += (View->GetBasePointDisp()) ? "P*" : "P ";
	strGrid += (View->GetBasePointBind()) ? "* " : "  ";
	strGrid += (View->GetBaseLineDisp()) ? "L*" : "L ";
	strGrid += (View->GetBaseLineBind()) ? "*" : " ";
	HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
	CClientDC dc(NULL);
	if(hFont!=NULL) {
		hOldFont = dc.SelectObject(hFont);
	}
	strMax.LoadString(ID_INDICATOR_GRID);
	size = dc.GetTextExtent(strMax);									// 最大文字列幅計算
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_GRID);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strGrid);											// 現在ｸﾞﾘｯﾄﾞﾍﾟｲﾝ更新
	pCmdUI->Enable(TRUE);
}

/*********************************************/
void CMainFrame::OnUpdateFunc(CCmdUI* pCmdUI)							//<<< ｽﾃｰﾀｽﾊﾞｰﾌｧﾝｸｼｮﾝﾍﾟｲﾝ更新
/*********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strFunc, strMax, str;
	CSize   size;
	UINT    nID, nStyle;
	int     i, func[8], idx, nWidth;

	View->GetFChar(func);												// ﾌｧﾝｸｼｮﾝｷｰ設定
	strFunc = (func[0]) ? "5 " : "  ";
	for(i=1; i<8; i++) {
		str.Format("%d", i+5);
		strFunc += (func[i]) ? str : " ";
		strFunc = (i<7) ? strFunc + " " : strFunc;
	}
	HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
	CClientDC dc(NULL);
	if(hFont!=NULL) {
		hOldFont = dc.SelectObject(hFont);
	}
	strMax.LoadString(ID_INDICATOR_FUNC);
	size = dc.GetTextExtent(strMax);									// 最大文字列幅計算
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_FUNC);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strFunc);											// 現在ﾌｧﾝｸｼｮﾝｷｰﾍﾟｲﾝ更新
	pCmdUI->Enable(TRUE);
}

/***************************************/
BOOL CMainFrame::OnCommonMenu(UINT nID)									//<<< 共通標準ﾒﾆｭｰ起動
/***************************************/
{
	int menu = View->GetActMenu();										// 起動ﾒﾆｭｰ取得
	int smnu = View->GetSaveMenu();										// 保存ﾒﾆｭｰ取得

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(nID);												// 前ﾒﾆｭｰ終了
	if(menu!=(int)nID&&smnu!=(int)nID) {
		View->SetActMenu(nID);											// ﾒﾆｭｰ設定
		View->InitActMenu(0);											// ﾒﾆｭｰ初期処理
		View->CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
	}
	return TRUE;
}

/**************************************/
BOOL CMainFrame::OnGroupMenu(UINT nID)									//<<< ｢ｸﾞﾙｰﾌﾟ設定/解除｣釦押下
/**************************************/
{
	OBJTYPE* op;
	int      i, kno, mode, dsp, edt, ano, func[8];

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(View->GetActMenu());								// 前ﾒﾆｭｰ終了
	mode = (nID==ID_GROUP_SET) ? 1 : 0;									// ﾓｰﾄﾞ(1:設定,0:解除)
	kno = GetGroupKey();												// ｸﾞﾙｰﾌﾟ対応ｷｰ取得
	if(kno>0) {															// <英数字ｷｰ押下>
		BaseObjPtr(2, &op, &dsp, &edt);									// 選択立体取得
		while(op) {
			if(dsp==1) {
				SetGroupNo(op, kno-1, mode);							// ｸﾞﾙｰﾌﾟ設定/解除
			}
			NextObjPtr(2, &op, &dsp, &edt);
		}
	} else {															// <ﾌｧﾝｸｼｮﾝｷｰ押下>
		View->GetFChar(func);											// ﾌｧﾝｸｼｮﾝｷｰ取得
		ano = (View->GetCChar()&0x2) ? 8 : 0;							// Ctrlｷｰ押下は+8
		for(i=0; i<8; i++) {											// ｸﾞﾙｰﾌﾟ1～8/9～16
			if(func[i]) {												// 該当ｷｰ押下
				BaseObjPtr(2, &op, &dsp, &edt);							// 選択立体取得
				while(op) {
					if(dsp==1) {
						SetGroupNo(op, i+ano, mode);					// ｸﾞﾙｰﾌﾟ設定/解除
					}
					NextObjPtr(2, &op, &dsp, &edt);
				}
			}
		}
	}
	SetGroupObj();														// ｸﾞﾙｰﾌﾟ立体設定
	View->InitChar();													// ｷｰ入力初期化
	return TRUE;
}

/****************************************/
BOOL CMainFrame::OnScrCtrlMenu(UINT nID)								//<<< 共通画面制御ﾒﾆｭｰ起動
/****************************************/
{
	int menu = View->GetActMenu();										// 起動ﾒﾆｭｰ取得
	int smnu = View->GetSaveMenu();										// 保存ﾒﾆｭｰ取得

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(menu!=(int)nID&&smnu!=(int)nID) {
		if(smnu==NONE) {
			View->SetSaveMenu(menu);									// 保存無は起動中ﾒﾆｭｰ保存
		}
		View->SetActMenu(nID);											// ﾒﾆｭｰ設定
		View->InitActMenu(0);                                           // ﾒﾆｭｰ初期処理
	} else {
		View->ExitActMenu(nID);											// 自ﾒﾆｭｰ終了
	}
	return TRUE;
}

/****************************************/
BOOL CMainFrame::OnMeasureMenu(UINT nID)								//<<< ｢定規/分度器｣釦押下
/****************************************/
{
	BOOL check = View->CheckMeasure(nID);								// 表示確認
	int  menu = View->GetActMenu();										// 起動ﾒﾆｭｰ取得
	int  smnu = View->GetSaveMenu();									// 保存ﾒﾆｭｰ取得

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(check&&menu!=(int)nID&&smnu!=(int)nID) {							// 釦押下中で異ﾒﾆｭｰ起動
		if(smnu==NONE) {
			View->SetSaveMenu(menu);									// 保存無は起動中ﾒﾆｭｰ保存
		}
		View->SetActMenu(nID);
		View->ExitActMenu(nID);											// 定規ﾒﾆｭｰ終了
	} else {															// 上記以外
		View->ExitActMenu(nID);											// 前ﾒﾆｭｰ終了
	}
	if(!check) {
		View->SetActMenu(nID);											// ﾒﾆｭｰ設定
		View->InitActMenu(0);											// ﾒﾆｭｰ初期処理
	}
	return TRUE;
}

/**********************************************************/
void CMainFrame::OnUpdateMenuEnableOrCheck(CCmdUI* pCmdUI)				//<<< ﾒﾆｭｰ釦有効化/無効化&
/**********************************************************/			//    起動ﾒﾆｭｰ釦更新
{
	UINT id=pCmdUI->m_nID;												// ｺﾏﾝﾄﾞID取得

	pCmdUI->Enable(View->GetEdtObjNum()>0?TRUE:FALSE);					// 編集対象の有無にて更新
	BOOL chk = View->GetActMenu()==(int)id||
               View->GetSaveMenu()==(int)id;
	pCmdUI->SetCheck(chk);												// 起動ﾒﾆｭｰのみﾁｪｯｸ
}

/***********************************************************/
void CMainFrame::OnUpdateMenuEnableOrCheck2(CCmdUI* pCmdUI)				//<<< ﾒﾆｭｰ釦有効化/無効化&
/***********************************************************/			//    起動ﾒﾆｭｰ釦更新2
{
	UINT id=pCmdUI->m_nID;												// ｺﾏﾝﾄﾞID取得

	pCmdUI->Enable(View->GetDspObjNum()>0?TRUE:FALSE);					// 表示対象の有無にて更新
	BOOL chk = View->GetActMenu()==(int)id||
               View->GetSaveMenu()==(int)id;
	pCmdUI->SetCheck(chk);												// 起動ﾒﾆｭｰのみﾁｪｯｸ
}

/***************************************************/
void CMainFrame::OnUpdateMenuEnable(CCmdUI* pCmdUI)						//<<< ﾒﾆｭｰ釦有効化/無効化
/***************************************************/
{
	pCmdUI->Enable(View->GetEdtObjNum()>0?TRUE:FALSE);					// 編集対象の有無にて更新
}

/**************************************************/
void CMainFrame::OnUpdateMenuCheck(CCmdUI* pCmdUI)						//<<< 起動ﾒﾆｭｰ釦更新
/**************************************************/
{
	UINT id=pCmdUI->m_nID;												// ｺﾏﾝﾄﾞID取得

	BOOL chk = View->GetActMenu()==(int)id||
               View->GetSaveMenu()==(int)id;
	pCmdUI->SetCheck(chk);												// 起動ﾒﾆｭｰのみﾁｪｯｸ
}

/*************************************************************/
void CMainFrame::OnUpdateMeasureEnableOrCheck(CCmdUI* pCmdUI)			//<<< 計測ﾒﾆｭｰ釦有効化/無効化&
/*************************************************************/			//    起動計測ﾒﾆｭｰ釦更新
{
	UINT id = pCmdUI->m_nID;											// ｺﾏﾝﾄﾞID取得
	int  am, sm, no;

	pCmdUI->Enable(View->GetEdtObjNum()>0?TRUE:FALSE);					// 編集対象の有無にて更新
	am = View->GetActMenu();											// 起動ﾒﾆｭｰ取得
	sm = View->GetSaveMenu();											// 保存ﾒﾆｭｰ取得
	if((am==(int)id||sm==(int)id)&&(!View->IsNumDispShow())) {			// 起動中でﾀﾞｲｱﾛｸﾞ無の場合
		if(sm==(int)id) {
			no = View->GetCursorNo();
			View->SetActMenu(id);
		}
		View->ExitActMenu(am);											// 該当ﾒﾆｭｰ終了
		if(sm==(int)id) {
			View->SetActMenu(am);
			View->SetCursorNo(no);
		}
		am = 0, sm = 0;
	}
	pCmdUI->SetCheck(am==(int)id||sm==(int)id);							// 起動ﾒﾆｭｰのみﾁｪｯｸ
}

/*****************************************************/
void CMainFrame::OnUpdateMeasureCheck(CCmdUI* pCmdUI)					//<<< 起動計測ﾒﾆｭｰ釦更新
/*****************************************************/
{
	UINT id = pCmdUI->m_nID;											// ｺﾏﾝﾄﾞID取得
	int  am, sm, no;

	am = View->GetActMenu();											// 起動ﾒﾆｭｰ取得
	sm = View->GetSaveMenu();											// 保存ﾒﾆｭｰ取得
	if((am==(int)id||sm==(int)id)&&(!View->IsNumDispShow())) {			// 起動中でﾀﾞｲｱﾛｸﾞ無の場合
		if(sm==(int)id) {
			no = View->GetCursorNo();
			View->SetActMenu(id);
		}
		View->ExitActMenu(am);											// 該当ﾒﾆｭｰ終了
		if(sm==(int)id) {
			View->SetActMenu(am);
			View->SetCursorNo(no);
		}
		am = 0, sm = 0;
	}
	pCmdUI->SetCheck(am==(int)id||sm==(int)id);							// 起動ﾒﾆｭｰのみﾁｪｯｸ
}

/**********************************************************/
void CMainFrame::OnUpdateMeasurePlaneCheck(CCmdUI* pCmdUI)				//<<< ｢定規/分度器｣釦更新
/**********************************************************/
{
	UINT id=pCmdUI->m_nID;												// ｺﾏﾝﾄﾞID取得
	int  mn=View->GetActMenu();											// 起動ﾒﾆｭｰ取得
	pCmdUI->SetCheck(View->CheckMeasure(id)||mn==(int)id);				// 起動ﾒﾆｭｰのみﾁｪｯｸ
}

/********************************/
void CMainFrame::OnFileNew(void)										//<<< ｢新規作成｣釦押下
/********************************/
{
	CDocument* pDoc;
	CString    strDoc, title, str;
	OBJTYPE*   op;
	int        dsp, edt, chk=0;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	BaseObjPtr(0, &op, &dsp, &edt);										// 先頭立体のﾎﾟｲﾝﾀ取得
	while(op) {
		if(dsp==0) {													// 非表示有の場合
			chk = 1; break;
		}
		NextObjPtr(0, &op, &dsp, &edt);									// 次立体のﾎﾟｲﾝﾀ取得
	}
	if(chk) {															// <非表示立体有>
		if(AfxMessageBox(IDS_NODISP_OBJ, MB_YESNO)==IDNO) {				// ｢新規作成｣を中止
			return;
		}
	}
	pDoc = GetActiveDocument();											// ﾄﾞｷｭﾒﾝﾄ取得
	pDoc->SetModifiedFlag(CheckCmdFlg());								// ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞｾｯﾄ
	if(!pDoc->SaveModified()) {											// 変更を保存するか問合
		return;
	}
	strDoc.LoadString(AFX_IDS_UNTITLED);								// ｢Untitled｣
	pDoc->SetTitle(strDoc);												// ﾄﾞｷｭﾒﾝﾄﾀｲﾄﾙ設定
	pDoc->OnNewDocument();												// 新規作成
	if(GetDemoFlg()!=0) {												// <体験版の場合>
		GetWindowText(title);											// ｳｨﾝﾄﾞｳﾀｲﾄﾙ取得
		str.LoadString(IDS_STR45);										// ｢体験版｣
		title += str;
		SetWindowText(title);											// ｳｨﾝﾄﾞｳﾀｲﾄﾙ変更
	}
	ResetVar();															// 変数初期化(ｼｽﾃﾑ稼働中)
	StartBackUp(0);														// ﾊﾞｯｸｱｯﾌﾟ開始処理
	Initialize();														// 初期化
	View->Init();														// View初期化
	View->Display(0);													// 再描画
}

/*********************************/
void CMainFrame::OnFileOpen(void)										//<<< ｢開く｣釦押下
/*********************************/
{
	CString fname, ext;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_FILE_OPEN);									// 前ﾒﾆｭｰ終了
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_FILE_OPEN);			// 釦押下状態
	if(DoPromptFName(fname, ext, TRUE)!=FALSE) {
		View->Display(0);												// 再描画
		DoOpenFile(fname);												// ﾌｧｲﾙを開く
	}
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_FILE_OPEN, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/***********************************/
void CMainFrame::OnViewerSave(void)										//<<< ｢ﾋﾞｭｰﾜ･ﾃﾞｰﾀ保存｣釦押下
/***********************************/
{
	CDocument* pDoc;
	CString    file;
	int        i;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(GetDemoFlg()!=0) {												// ｢体験版,起動NG｣
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	pDoc = GetActiveDocument();											// ﾄﾞｷｭﾒﾝﾄを取得
	m_bViewSave = TRUE;													// ﾋﾞｭｰﾜ保存ﾌﾗｸﾞON
	if(pDoc->DoSave(NULL, TRUE)) {										// 名前を付けて保存実行
		View->SetFChar(1, 0x1);											// ﾌｧﾝｸｼｮﾝｷｰ[F5]設定
		View->ExitActMenu(ID_RENDERING);								// 前ﾒﾆｭｰ終了
		View->SetActMenu(ID_RENDERING);									// ﾒﾆｭｰ設定
		View->InitActMenu(0);											// ﾒﾆｭｰ初期処理
		file = pDoc->GetTitle();										// ﾌｧｲﾙ名
		i = file.ReverseFind('.');
		file = (i!=-1) ? file.Left(i) : file;
		file = m_sFilePath + file;										// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)
		View->OutputCapData(file);										// ｷｬﾌﾟﾁｬﾃﾞｰﾀ出力
		View->ExitActMenu(ID_RENDERING);								// 前ﾒﾆｭｰ終了
	}
	m_bViewSave = FALSE;												// ﾋﾞｭｰﾜ保存ﾌﾗｸﾞOFF
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnExportSLC(void)										//<<< ｢ｴｸｽﾎﾟｰﾄSLC｣釦押下
/**********************************/
{
	OBJTYPE* op;
	CString  ext="slc", file=_T(""), filter;
	int      i, i1, i2;
	DWORD    flg;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(GetDemoFlg()!=0) {												// ｢体験版,起動NG｣
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	BaseObjPtr(1, &op, &i1, &i2);										// 表示立体ﾎﾟｲﾝﾀ取得
	if(op==NULL) {														// <立体無は中止>
		return;
	}
	View->ExitActMenu(ID_EXPORT_SLC);									// 前ﾒﾆｭｰ終了
	View->InitChar();													// ｷｰ入力初期化
	if(!CheckFilePath(0, m_sFilePath)) {								// ﾊﾟｽ確認
		m_sFilePath = GetAppPath(0);
	}
	flg = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;							// ｢SLCﾌｧｲﾙを保存｣
	filter.LoadString(IDS_FILE_FILTER104);
	CFileDlg dlg(FALSE, ext, file, flg, filter);						// ﾀﾞｲｱﾛｸﾞ表示
	dlg.m_ofn.lpstrInitialDir = m_sFilePath;
	if(dlg.DoModal()==IDOK) {
		file = dlg.GetPathName();										// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		i = file.ReverseFind('\\');
		m_sFilePath = file.Left(i+1);									// ﾊﾟｽ保存
	} else {
		return;
	}
	ExportSLCFile(file);												// ｴｸｽﾎﾟｰﾄSLC
}

/***********************************/
void CMainFrame::OnCaptureSet(void)										//<<< ｢ｷｬﾌﾟﾁｬｰの設定｣釦押下
/***********************************/
{
	int p1, p2, p3, p4, p5, p6;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	CCaptureDlg dlg;
	p1 = View->GetCapPlane();											// ｷｬﾌﾟﾁｬ取込画面取得
	p2 = View->GetCapPos();												// ｷｬﾌﾟﾁｬ取込位置取得
	p3 = View->GetCapKind();											// ｷｬﾌﾟﾁｬ画像種類取得
	p4 = View->GetCapWidth();											// ｷｬﾌﾟﾁｬ画面幅取得
	p5 = View->GetCapHeight();											// ｷｬﾌﾟﾁｬ画面高さ取得
	p6 = View->GetCapJComp();											// ｷｬﾌﾟﾁｬJPEG圧縮率取得
	dlg.SetValue(p1, p2, p3, p4, p5, p6);								// 現在の設定値を表示
	if(dlg.DoModal()==IDOK) {											// ｷｬﾌﾟﾁｬｰ設定ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(&p1, &p2, &p3, &p4, &p5, &p6);
		View->SetCapPlane(p1);											// ｷｬﾌﾟﾁｬ取込画面設定
		View->SetCapPos(p2);											// ｷｬﾌﾟﾁｬ取込位置設定
		View->SetCapKind(p3);											// ｷｬﾌﾟﾁｬ画像種類設定
		View->SetCapWidth(p4);											// ｷｬﾌﾟﾁｬ画面幅設定
		View->SetCapHeight(p5);											// ｷｬﾌﾟﾁｬ画面高さ設定
		View->SetCapJComp(p6);											// ｷｬﾌﾟﾁｬJPEG圧縮率設定
	}
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnPasswordSet(void)									//<<< ｢ﾊﾟｽﾜｰﾄﾞの設定｣釦押下
/************************************/
{
	CString str1, str2;
	int     len;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	str1.Format("%s", m_cWPassword);
	str2.Format("%s", m_cRPassword);
	CPasswdDlg dlg;
	dlg.SetValue(str1, str2, m_PasswordKind);							// 既存値表示
	if(dlg.DoModal()==IDOK) {											// ﾊﾟｽﾜｰﾄﾞ設定ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(&str1, &str2, &m_PasswordKind);
		len = str1.GetLength();
		memset(m_cWPassword, NULL, sizeof(m_cWPassword));
		memcpy(m_cWPassword, (const char*)str1, len);
		len = str2.GetLength();
		memset(m_cRPassword, NULL, sizeof(m_cRPassword));
		memcpy(m_cRPassword, (const char*)str2, len);
	}
	View->InitChar();													// ｷｰ入力初期化
}

/***********************************/
void CMainFrame::OnRestBackUp(void)										//<<< ｢ﾊﾞｯｸｱｯﾌﾟ復元｣釦押下
/***********************************/
{
	CDocument* pDoc;
	CString    strDoc;
	OBJTYPE*   op;
	int        dsp, edt, chk=0;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(GetDemoFlg()!=0) {												// ｢体験版,起動NG｣
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	BaseObjPtr(0, &op, &dsp, &edt);										// 先頭立体のﾎﾟｲﾝﾀ取得
	while(op) {
		if(dsp==0) {													// 非表示有の場合
			chk = 1; break;
		}
		NextObjPtr(0, &op, &dsp, &edt);									// 次立体のﾎﾟｲﾝﾀ取得
	}
	if(chk) {															// <非表示立体有>
		if(AfxMessageBox(IDS_NODISP_OBJ, MB_YESNO)==IDNO) {				// ｢新規作成｣を中止
			return;
		}
	}
	pDoc = GetActiveDocument();											// ﾄﾞｷｭﾒﾝﾄを取得
	pDoc->SetModifiedFlag(CheckCmdFlg());								// ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞｾｯﾄ
	if(!pDoc->SaveModified()) {											// 変更を保存するか問合
		return;
	}
	strDoc.LoadString(AFX_IDS_UNTITLED);								// ｢Untitled｣
	pDoc->SetTitle(strDoc);												// ﾄﾞｷｭﾒﾝﾄのﾀｲﾄﾙ設定
	pDoc->OnNewDocument();												// 新規作成
	ResetVar();															// 変数初期化(ｼｽﾃﾑ稼働中)
	StartBackUp(0);														// ﾊﾞｯｸｱｯﾌﾟ開始処理
	Initialize();														// 初期化
	View->Init();														// View初期化
	View->Display(0);													// 再描画
	StartMenu();														// ﾒﾆｭｰ開始処理
	InitLevel();														// Undo/Redo初期化
	if(!RestoreBackUp()) {												// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ復元
		BackLevel();													// Undo/Redo戻り
		AfxMessageBox(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ
	} else {
		FileBackUp1();													// ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ処理1
		View->Display(0);
	}
	SetGroupObj();														// ｸﾞﾙｰﾌﾟ立体設定
}

/*****************************/
void CMainFrame::OnUndo(void)											//<<< ｢Undo｣釦押下
/*****************************/
{
	BOOL chk = TRUE;
	int  i, gno[32], menu;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	if(CheckUndo()&&chk) {
		Undo(gno);														// Undo
		for(i=0; i<32; i++) {
			if(gno[i]) {												// Undo対象ｸﾞﾙｰﾌﾟは
				OnDspGroupObject(i);									// ｸﾞﾙｰﾌﾟ立体表示
				m_GroupDisp[i] = 1;										// ｸﾞﾙｰﾌﾟ表示ﾌﾗｸﾞON
			}
		}
		if(View->CountEdtObj()<1) {
			menu = View->GetActMenu();									// 起動ﾒﾆｭｰ取得
			if(menu>=M_ALLSHADING&&menu!=M_EDIT_PASTEOBJ) {
				View->ExitActMenu(menu);								// 前ﾒﾆｭｰ終了
			}
		} else { 
			View->InitActMenu(1);										// ﾒﾆｭｰ初期処理
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
	}
	SetGroupObj();														// ｸﾞﾙｰﾌﾟ立体設定
	View->InitChar();													// ｷｰ入力初期化
	View->Display(0);													// 全立体再表示
}

/*********************************************/
void CMainFrame::OnUpdateUndo(CCmdUI* pCmdUI)							//<<< ｢Undo｣釦更新
/*********************************************/
{
	BOOL chk = TRUE;

	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	pCmdUI->Enable(CheckUndo()&&chk ? TRUE : FALSE);					// Undo条件にて更新
}

/*****************************/
void CMainFrame::OnRedo(void)											//<<< ｢Redo｣釦押下
/*****************************/
{
	BOOL chk = TRUE;
	int  i, gno[32], menu;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	if(CheckRedo()&&chk) {
		Redo(gno);														// Redo
		for(i=0; i<32; i++) {
			if(gno[i]) {												// Redo対象ｸﾞﾙｰﾌﾟは
				OnDspGroupObject(i);									// ｸﾞﾙｰﾌﾟ立体表示
				m_GroupDisp[i] = 1;										// ｸﾞﾙｰﾌﾟ表示ﾌﾗｸﾞON
			}
		}
		if(View->CountEdtObj()<1) {
			menu = View->GetActMenu();									// 起動ﾒﾆｭｰ取得
			if(menu>=M_ALLSHADING&&menu!=M_EDIT_PASTEOBJ) {
				View->ExitActMenu(menu);								// 前ﾒﾆｭｰ終了
			}
		} else {
			View->InitActMenu(1);										// ﾒﾆｭｰ初期処理
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
	}
	SetGroupObj();														// ｸﾞﾙｰﾌﾟ立体設定
	View->InitChar();													// ｷｰ入力初期化
	View->Display(0);													// 全立体再表示
}

/*********************************************/
void CMainFrame::OnUpdateRedo(CCmdUI* pCmdUI)							//<<< ｢Redo｣釦更新
/*********************************************/
{
	BOOL chk = TRUE;

	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	pCmdUI->Enable(CheckRedo()&&chk ? TRUE : FALSE);					// Redo条件にて更新
}

/***********************************/
void CMainFrame::OnCopyObject(void)										//<<< ｢ｺﾋﾟｰ｣釦押下
/***********************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_EDIT_COPYOBJ);									// 前ﾒﾆｭｰ終了
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_COPYOBJ);		// 釦押下状態
	WriteCopyFile(View->GetChar());										// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ出力
	BaseObjPtr(2, &op, &dsp, &edt);										// 有効立体先頭取得
	if(op!=NULL) {
		m_CopyCmdFlg = 1;												// 編集対象立体あり
	}
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_COPYOBJ, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnCutObject(void)										//<<< ｢ｶｯﾄ｣釦押下
/**********************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_EDIT_CUTOBJ);									// 前ﾒﾆｭｰ終了
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_CUTOBJ);			// 釦押下状態
	WriteCopyFile(View->GetChar());										// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ出力
	BaseObjPtr(2, &op, &dsp, &edt);										// 有効立体先頭取得
	if(op!=NULL) {
		m_CopyCmdFlg = 1;												// 編集対象立体あり
	}
	View->MenuCall(View->GetActMenu(), 0, 0, 0, 11);					// 削除
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_CUTOBJ, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/****************************************************/
void CMainFrame::OnUpdatePasteObject(CCmdUI* pCmdUI)					//<<< ﾍﾟｰｽﾄ釦有効化/無効化&
/****************************************************/					//    ﾒﾆｭｰ釦更新
{
	pCmdUI->Enable(m_CopyCmdFlg);										// ｺﾋﾟｰｺﾏﾝﾄﾞ有無にて更新
	BOOL chk = View->GetActMenu()==(int)pCmdUI->m_nID||
               View->GetSaveMenu()==(int)pCmdUI->m_nID;
	pCmdUI->SetCheck(chk);												// 起動ﾒﾆｭｰのみﾁｪｯｸ
}

/*******************************/
void CMainFrame::OnDelete(void)											//<<< ｢削除｣釦押下
/*******************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->MenuCall(View->GetActMenu(), 0, 0, 0, 11);					// ﾒﾆｭｰ呼び出し
}

/*******************************/
void CMainFrame::OnAllPic(void)											//<<< ｢一括選択｣釦押下
/*******************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(View->GetActMenu());								// 前ﾒﾆｭｰ終了
	OnEdtAllObject();													// 編集立体全選択(表示立体)
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->Display(0);
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnObjInfo(void)										//<<< ｢立体情報ﾂｰﾙ｣釦押下
/********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ShowOifTree();												// 情報ﾂｰﾙﾀﾞｲｱﾛｸﾞ表示切替
	View->InitChar();													// ｷｰ入力初期化
}

/************************************************/
void CMainFrame::OnUpdateObjInfo(CCmdUI* pCmdUI)						//<<< ｢立体情報ﾂｰﾙ｣釦更新
/************************************************/
{
	pCmdUI->SetCheck(View->IsOifTreeShow() ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/************************************/
void CMainFrame::OnFunctionBtn(void)									//<<< ｢ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ｣釦押下
/************************************/
{
	int mode;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->InitChar();													// ｷｰ入力初期化
	mode = View->IsFuncMode();											// ﾌｧﾝｸｼｮﾝ表示ﾓｰﾄﾞ取得
	mode = (mode>5) ? 0 : mode+1;										// ﾓｰﾄﾞ変換
	CSetFuncModeDlg dlg;
	dlg.SetValue(mode);
	if(dlg.DoModal()==IDOK) {											// ﾌｧﾝｸｼｮﾝ表示ﾓｰﾄﾞﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(&mode);
		if(mode==0) {													// <ﾀﾞｲｱﾛｸﾞ消去>
			View->ChangeFuncMode(6);									// ﾌｧﾝｸｼｮﾝ表示ﾓｰﾄﾞ設定
			View->ChangeFuncDsp(0);										// 設定ﾂｰﾙﾀﾞｲｱﾛｸﾞ表示切替(消去)
			View->ChangeFuncDsp2(0);									// 設定ﾂｰﾙﾀﾞｲｱﾛｸﾞ2表示切替(消去)
			View->HideFunction();										// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ消去
			View->HideFunction2();										// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ2消去
		} else {														// <ﾀﾞｲｱﾛｸﾞ表示>
			View->ChangeFuncMode(mode-1);								// ﾌｧﾝｸｼｮﾝ表示ﾓｰﾄﾞ設定
			if(mode-1<3) {
				View->ChangeFuncDsp(1);									// 設定ﾂｰﾙﾀﾞｲｱﾛｸﾞ表示切替(表示)
				View->ChangeFuncDsp2(0);								// 設定ﾂｰﾙﾀﾞｲｱﾛｸﾞ2表示切替(消去)
				View->HideFunction2();									// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ2消去
				if(mode-1==0) {											// <常駐>
					View->ShowFunction();								// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ表示
				}
			} else {
				View->ChangeFuncDsp(0);									// 設定ﾂｰﾙﾀﾞｲｱﾛｸﾞ表示切替(消去)
				View->ChangeFuncDsp2(1);								// 設定ﾂｰﾙﾀﾞｲｱﾛｸﾞ2表示切替(表示)
				View->HideFunction();									// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ消去
				if(mode-1==3) {											// <常駐>
					View->ShowFunction2();								// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ2表示
				}
			}
		}
	}
}

/****************************************************/
void CMainFrame::OnUpdateFunctionBtn(CCmdUI* pCmdUI)					//<<< ｢ﾌｧﾝｸｼｮﾝ設定ﾂｰﾙ｣釦更新
/****************************************************/
{
	pCmdUI->SetCheck(View->IsFuncDsp()||View->IsFuncDsp2() ? 1 : 0);	// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/**********************************/
void CMainFrame::OnCopyInput(void)										//<<< ｢複写ﾓｰﾄﾞ｣釦押下
/**********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_COPY_INPUT);									// 前ﾒﾆｭｰ終了
	View->SetCopyBtn(View->GetCopyBtn() ? 0 : 1);						// 複写ﾓｰﾄﾞの切替
	View->InitChar();													// ｷｰ入力初期化
}

/**************************************************/
void CMainFrame::OnUpdateCopyInput(CCmdUI* pCmdUI)						//<<< ｢複写ﾓｰﾄﾞ｣釦更新
/**************************************************/
{
	pCmdUI->SetCheck(View->GetCopyBtn() ? 1 : 0);						// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/*********************************/
void CMainFrame::OnNumInput(void)										//<<< ｢数値ﾓｰﾄﾞ｣釦押下
/*********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_NUM_INPUT);									// 前ﾒﾆｭｰ終了
	View->SetNumBtn(View->GetNumBtn() ? 0 : 1);							// 数値入力の切替
}

/*************************************************/
void CMainFrame::OnUpdateNumInput(CCmdUI* pCmdUI)						//<<< ｢数値ﾓｰﾄﾞ｣釦更新
/*************************************************/
{
	pCmdUI->SetCheck(View->GetNumBtn() ? 1 : 0);						// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/*********************************/
void CMainFrame::OnDispMode(void)										//<<< ｢表示ﾓｰﾄﾞ｣釦押下
/*********************************/
{
	int mode=-1;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_MODE);				// 釦押下状態
	if(View->GetChar()=='0') {											// 0ｷｰ押下はﾓｰﾄﾞなし
		mode = 0;
	} else if(View->GetChar()=='1') {									// 1ｷｰ押下はﾓｰﾄﾞ1
		mode = 1;
	} else if(View->GetChar()=='2') {									// 2ｷｰ押下はﾓｰﾄﾞ2
		mode = 2;
	} else if(View->GetChar()=='3') {									// 3ｷｰ押下はﾓｰﾄﾞ3
		mode = 3;
	} else if(View->GetChar()=='4') {									// 4ｷｰ押下は透視図ｼｪｰﾃﾞｨﾝｸﾞ
		mode = 4;
	} else if(View->GetChar()=='5') {									// 5ｷｰ押下は全面ｼｪｰﾃﾞｨﾝｸﾞ
		mode = 5;
	} else {
		CDispmodeDlg dlg;
		dlg.SetValue(View->GetDispMode());								// 現在の設定値を表示
		if(dlg.DoModal()==IDOK) {										// 表示ﾓｰﾄﾞﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&mode);
		}
	}
	if(mode>=0) {														// <ﾓｰﾄﾞ指定OK>
		View->SetDispMode(mode);										// 表示ﾓｰﾄﾞ設定
		View->Display(0);												// 再表示
	}
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_MODE, FALSE);		// 釦開放状態
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnDispRange(void)										//<<< ｢表示範囲｣釦押下
/**********************************/
{
	int range=-1;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_RANGE);				// 釦押下状態
	if(View->GetChar()=='A') {											// Aｷｰ押下は全平面
		range = 0;
	} else if(View->GetChar()=='S') {									// Sｷｰ押下は透視図
		range = 1;
	} else if(View->GetChar()=='Z') {									// Zｷｰ押下はXY平面
		range = 2;
	} else if(View->GetChar()=='X') {									// Xｷｰ押下はYZ平面
		range = 3;
	} else if(View->GetChar()=='C') {									// Cｷｰ押下はZX平面
		range = 4;
	} else {
		CScreenDlg dlg;
		if(dlg.DoModal()==IDOK) {										// 表示ﾓｰﾄﾞﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&range);
		}
	}
	if(range>=0) {														// <範囲指定OK>
		View->SetScreenNo(range);										// 表示範囲設定
		View->Display(0);												// 再表示
	}
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_RANGE, FALSE);		// 釦開放状態
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnAllDisp(void)										//<<< ｢全表示｣釦押下
/********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(View->GetActMenu());								// 前ﾒﾆｭｰ終了
	OnDspAllObject();													// 全立体の表示
	for(int i=0; i<32; i++) {
		m_GroupDisp[i] = 1;												// ｸﾞﾙｰﾌﾟ表示全表示
	}
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->Display(0);
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnAllErase(void)										//<<< ｢全非表示｣釦押下
/*********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(View->GetActMenu());								// 前ﾒﾆｭｰ終了
	OffDspAllObject();													// 全立体の非表示
	for(int i=0; i<32; i++) {
		m_GroupDisp[i] = 0;												// ｸﾞﾙｰﾌﾟ表示全非表示
	}
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->Display(0);
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnScrState(void)										//<<< ｢画面状態表示｣釦押下
/*********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ShowStateDlg();												// 画面状態ﾀﾞｲｱﾛｸﾞ表示切替
	View->InitChar();													// ｷｰ入力初期化
}

/*************************************************/
void CMainFrame::OnUpdateScrState(CCmdUI* pCmdUI)						//<<< ｢画面状態表示｣釦更新
/*************************************************/
{
	pCmdUI->SetCheck(View->IsStateDlgShow() ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/********************************/
void CMainFrame::OnDataUse(void)										//<<< ｢ﾃﾞｰﾀ使用率表示｣釦押下
/********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ShowDataUse();												// ﾃﾞｰﾀ使用率ﾀﾞｲｱﾛｸﾞ表示切替
	View->InitChar();													// ｷｰ入力初期化
}

/************************************************/
void CMainFrame::OnUpdateDataUse(CCmdUI* pCmdUI)						//<<< ｢ﾃﾞｰﾀ使用率表示｣釦更新
/************************************************/
{
	pCmdUI->SetCheck(View->IsDataUseShow() ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/**********************************/
void CMainFrame::OnParamDisp(void)										//<<< ｢数値表示｣釦押下
/**********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->SetNumDisp(View->IsNumDispShow() ? 0 : 1);					// 表示ﾀﾞｲｱﾛｸﾞ表示切替
	View->InitChar();													// ｷｰ入力初期化
}

/**************************************************/
void CMainFrame::OnUpdateParamDisp(CCmdUI* pCmdUI)						//<<< ｢数値表示｣釦更新
/**************************************************/
{
	pCmdUI->SetCheck(View->IsNumDispShow() ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/*********************************/
void CMainFrame::OnGroupDsp(void)										//<<< ｢ｸﾞﾙｰﾌﾟ表示｣釦押下
/*********************************/
{
	int i, kno, ano, func[8];

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(View->GetActMenu());								// 前ﾒﾆｭｰ終了
	kno = GetGroupKey();												// ｸﾞﾙｰﾌﾟ対応ｷｰ取得
	if(kno>0) {															// <英数字ｷｰ押下>
		if(m_GroupObj[kno-1]) {											// ｷｰ押下で立体あり
			if(!m_GroupDisp[kno-1]) {									// <非表示中の場合>
				OnDspGroupObject(kno-1);								// ｸﾞﾙｰﾌﾟ立体表示
				m_GroupDisp[kno-1] = 1;									// 表示中へ
			} else {													// <表示中の場合>
				OffDspGroupObject(kno-1);								// ｸﾞﾙｰﾌﾟ立体非表示
				m_GroupDisp[kno-1] = 0;									// 非表示中へ
			}
		}
	} else {															// <ﾌｧﾝｸｼｮﾝｷｰ押下>
		View->GetFChar(func);											// ﾌｧﾝｸｼｮﾝｷｰ取得
		ano = (View->GetCChar()&0x2) ? 8 : 0;							// Ctrlｷｰ押下は+8
		for(i=0; i<8; i++) {											// ｸﾞﾙｰﾌﾟ1～8
			if(func[i]&&m_GroupObj[i+ano]) {							// 該当ｷｰ押下で立体あり
				if(!m_GroupDisp[i+ano]) {								// <非表示中の場合>
					OnDspGroupObject(i+ano);							// ｸﾞﾙｰﾌﾟ立体表示
					m_GroupDisp[i+ano] = 1;								// 表示中へ
				} else {												// <表示中の場合>
					OffDspGroupObject(i+ano);							// ｸﾞﾙｰﾌﾟ立体非表示
					m_GroupDisp[i+ano] = 0;								// 非表示中へ
				}
			}
		}
	}
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->Display(0);
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnGroupSel(void)										//<<< ｢ｸﾞﾙｰﾌﾟ選択｣釦押下
/*********************************/
{
	int i, kno, ano, func[8];

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(View->GetActMenu());								// 前ﾒﾆｭｰ終了
	kno = GetGroupKey();												// ｸﾞﾙｰﾌﾟ対応ｷｰ取得
	if(kno>0) {															// <英数字ｷｰ押下>
		OnEdtGroupObject(kno-1);										// 編集立体(ｸﾞﾙｰﾌﾟ)選択
	} else {															// <ﾌｧﾝｸｼｮﾝｷｰ押下>
		View->GetFChar(func);											// ﾌｧﾝｸｼｮﾝｷｰ取得
		ano = (View->GetCChar()&0x2) ? 8 : 0;							// Ctrlｷｰ押下は+8
		for(i=0; i<8; i++) {											// ｸﾞﾙｰﾌﾟ1～8
			if(func[i]) {
				OnEdtGroupObject(i+ano);								// 編集立体(ｸﾞﾙｰﾌﾟ)選択
			}
		}
	}
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->Display(0);
	View->InitChar();													// ｷｰ入力初期化
}

/*******************************/
void CMainFrame::OnZoomIn(void)											//<<< ｢ｽﾞｰﾑ･ｲﾝ｣釦押下
/*******************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ZoomIn();														// 3面図のｽﾞｰﾑ･ｲﾝ
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnZoomOut(void)										//<<< ｢ｽﾞｰﾑ･ｱｳﾄ｣釦押下
/********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ZoomOut();													// 3面図のｽﾞｰﾑ･ｱｳﾄ
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnAutoZoom(void)										//<<< ｢自動倍率｣釦押下
/*********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->AutoZoom();													// 3面図の自動倍率
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnExactZoom(void)										//<<< ｢実寸表示｣釦押下
/**********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExactZoom();													// 3面図の実寸表示
	View->InitChar();													// ｷｰ入力初期化
}

/***********************************/
void CMainFrame::OnScrollTool(void)										//<<< ｢画面ｽｸﾛｰﾙ･ﾂｰﾙ｣釦押下
/***********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ShowScrollTool();												// ｽｸﾛｰﾙﾂｰﾙﾀﾞｲｱﾛｸﾞ表示切替
	View->InitChar();													// ｷｰ入力初期化
}

/***************************************************/
void CMainFrame::OnUpdateScrollTool(CCmdUI* pCmdUI)						//<<< ｢画面ｽｸﾛｰﾙ･ﾂｰﾙ｣釦更新
/***************************************************/
{
	pCmdUI->SetCheck(View->IsScrollToolShow() ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/********************************/
void CMainFrame::OnHomePos(void)										//<<< ｢HOME｣釦押下
/********************************/
{
	int func[8], no=1;

	if(View->GetChar()=='1') {											// [1]ｷｰ押下はHome1
		no = 1;
	} else if(View->GetChar()=='2') {									// [2]ｷｰ押下はHome2
		no = 2;
	} else if(View->GetChar()=='3') {									// [3]ｷｰ押下はHome3
		no = 3;
	} else if(View->GetChar()=='4') {									// [4]ｷｰ押下はHome4
		no = 4;
	} else if(View->GetChar()=='5') {									// [5]ｷｰ押下はHome5
		no = 5;
	} else {															// <[1]～[5]ｷｰ押下なし>
		View->GetFChar(func);											// ﾌｧﾝｸｼｮﾝｷｰ取得
		if(func[0]) {													// [F5]ｷｰ押下はHome2
			no = 2;
		} else if(func[1]) {											// [F6]ｷｰ押下はHome3
			no = 3;
		} else if(func[2]) {											// [F7]ｷｰ押下はHome4
			no = 4;
		} else if(func[3]) {											// [F8]ｷｰ押下はHome5
			no = 5;
		}
	}
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->RestoreHomePosition(no);										// 表示状態復帰
	View->InitChar();													// ｷｰ入力初期化
}

/***********************************/
void CMainFrame::OnBackScreen(void)										//<<< ｢前表示状態｣釦押下
/***********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->BackScreenState();											// 前表示状態戻し
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnTruckUp(void)										//<<< ｢ﾄﾗｯｸ･ｱｯﾌﾟ｣釦押下
/********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->TruckUp();													// 透視図のﾄﾗｯｸ･ｱｯﾌﾟ
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnTruckDown(void)										//<<< ｢ﾄﾗｯｸ･ﾀﾞｳﾝ｣釦押下
/**********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->TruckDown();													// 透視図のﾄﾗｯｸ･ﾀﾞｳﾝ
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnCentPars(void)										//<<< ｢参照点指定｣釦押下
/*********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->CentPars();													// 透視図の参照点指定
	View->InitChar();													// ｷｰ入力初期化
}

/*******************************/
void CMainFrame::OnEyeSet(void)											//<<< ｢透視図変更｣釦押下
/*******************************/
{
	int flg=-1;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_EYE_SET);			// 釦押下状態
	if(View->GetChar()=='X') {											// Xｷｰ押下はXY(表)
		flg = 0;
	} else if(View->GetChar()=='Y') {									// Yｷｰ押下はYZ(表)
		flg = 1;
	} else if(View->GetChar()=='Z') {									// Zｷｰ押下はZX(表)
		flg = 2;
	} else if(View->GetChar()=='U') {									// Uｷｰ押下はXY(裏)
		flg = 3;
	} else if(View->GetChar()=='V') {									// Vｷｰ押下はYZ(裏)
		flg = 4;
	} else if(View->GetChar()=='W') {									// Wｷｰ押下はZX(裏)
		flg = 5;
	} else {
		CEyesetDlg dlg;
		if(dlg.DoModal()==IDOK) {										// 透視図変更ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&flg);
		}
	}
	if(flg>=0) {
		View->EyeSet(flg);												// 透視図の変更
	}
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_EYE_SET, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnEyeChange(void)										//<<< ｢視点変更･ﾂｰﾙ｣釦押下
/**********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ShowEyeChange();												// 変更ﾂｰﾙﾀﾞｲｱﾛｸﾞ表示切替
	View->InitChar();													// ｷｰ入力初期化
}

/**************************************************/
void CMainFrame::OnUpdateEyeChange(CCmdUI* pCmdUI)						//<<< ｢視点変更･ﾂｰﾙ｣釦更新
/**************************************************/
{
	pCmdUI->SetCheck(View->IsEyeChangeShow() ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/*********************************/
void CMainFrame::OnGridDisp(void)										//<<< ｢ｸﾞﾘｯﾄﾞ表示｣釦押下
/*********************************/
{
	int func[8], check1, check2, check3;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	check1 = View->GetGridDisp();										// ｸﾞﾘｯﾄﾞ表示状態取得
	check2 = View->GetBasePointDisp();									// 基準点表示状態取得
	check3 = View->GetBaseLineDisp();									// 基準点表示状態取得
	if(check1||check2||check3) {										// <<何れかが表示中>>
		if(check1) {													// <ｸﾞﾘｯﾄﾞ>
			View->SetGridDisp(0);										// ｸﾞﾘｯﾄﾞ表示状態設定(非表示)
			View->DispGrid(XY, 1);										// ｸﾞﾘｯﾄﾞ消去
			View->DispGrid(YZ, 1);
			View->DispGrid(ZX, 1);
		}
		if(check2) {													// <基準点>
			View->SetBasePointDisp(0);									// 基準点表示状態設定(非表示)
			View->Display(0);											// 再描画
		}
		if(check3) {													// <基準線>
			View->SetBaseLineDisp(0);									// 基準点表示状態設定(非表示)
			View->Display(0);											// 再描画
		}
	} else {															// <<全てが非表示>>
		View->GetFChar(func);											// ﾌｧﾝｸｼｮﾝｷｰ取得
		if(func[0]||(!func[1]&&!func[2])) {								// <ｸﾞﾘｯﾄﾞ>
			View->SetGridDisp(1);										// ｸﾞﾘｯﾄﾞ表示状態設定(表示)
			View->SetGrid();											// ｸﾞﾘｯﾄﾞ設定
			View->DispGrid(XY, 0);										// ｸﾞﾘｯﾄﾞ表示
			View->DispGrid(YZ, 0);
			View->DispGrid(ZX, 0);
		}
		if(func[1]) {													// <基準点>
			View->SetBasePointDisp(1);									// 基準点表示状態設定(表示)
			View->Display(0);											// 再描画
		}
		if(func[2]) {													// <基準線>
			View->SetBaseLineDisp(1);									// 基準点表示状態設定(表示)
			View->Display(0);											// 再描画
		}
	}
	View->InitChar();													// ｷｰ入力初期化
}

/*************************************************/
void CMainFrame::OnUpdateGridDisp(CCmdUI* pCmdUI)						//<<< ｢ｸﾞﾘｯﾄﾞ表示｣釦更新
/*************************************************/
{
	int check1, check2, check3;

	check1 = View->GetGridDisp();										// ｸﾞﾘｯﾄﾞ表示状態取得
	check2 = View->GetBasePointDisp();									// 基準点表示状態取得
	check3 = View->GetBaseLineDisp();									// 基準点表示状態取得
	pCmdUI->SetCheck(check1||check2||check3 ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/*********************************/
void CMainFrame::OnGridBind(void)										//<<< ｢ｸﾞﾘｯﾄﾞ拘束｣釦押下
/*********************************/
{
	int func[8], check1, check2, check3;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	check1 = View->GetGridBind();										// ｸﾞﾘｯﾄﾞ拘束状態取得
	check2 = View->GetBasePointBind();									// 基準点拘束状態取得
	check3 = View->GetBaseLineBind();									// 基準点拘束状態取得
	if(check1||check2||check3) {										// <何れかが拘束中>
		View->SetGridBind(0);											// ｸﾞﾘｯﾄﾞ拘束状態設定(非拘束)
		View->SetBasePointBind(0);										// 基準点拘束状態設定(非拘束)
		View->SetBaseLineBind(0);										// 基準点拘束状態設定(非拘束)
	} else {															// <全てが非拘束>
		View->GetFChar(func);											// ﾌｧﾝｸｼｮﾝｷｰ取得
		if(func[0]||(!func[1]&&!func[2])) {								// <ｸﾞﾘｯﾄﾞ[F5]指定/[F6],[F7]以外>
			View->SetGridBind(1);										// ｸﾞﾘｯﾄﾞ拘束状態設定(拘束)
		}
		if(func[1]) {													// <基準点[F6]指定>
			View->SetBasePointBind(1);									// 基準点拘束状態設定(拘束)
		}
		if(func[2]) {													// <基準線[F7]指定>
			View->SetBaseLineBind(1);									// 基準点拘束状態設定(拘束)
		}
	}
	View->InitChar();													// ｷｰ入力初期化
}

/*************************************************/
void CMainFrame::OnUpdateGridBind(CCmdUI* pCmdUI)						//<<< ｢ｸﾞﾘｯﾄﾞ拘束｣釦更新
/*************************************************/
{
	int check1, check2, check3;

	check1 = View->GetGridBind();										// ｸﾞﾘｯﾄﾞ拘束状態取得
	check2 = View->GetBasePointBind();									// 基準点拘束状態取得
	check3 = View->GetBaseLineBind();									// 基準点拘束状態取得
	pCmdUI->SetCheck(check1||check2||check3 ? 1 : 0);					// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/**********************************/
void CMainFrame::OnGridSpace(void)										//<<< ｢ｸﾞﾘｯﾄﾞ間隔｣釦押下
/**********************************/
{
	int    gitv;
	double gspc;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_GRID_SPACE);			// 釦押下状態
	CInputDlg dlg(1);
	dlg.SetValue(View->GetGridSpace()/1000.0, View->GetGridIntvl());	// ｸﾞﾘｯﾄﾞ間隔取得,目盛間隔取得
	if(dlg.DoModal()==IDOK) {											// ｸﾞﾘｯﾄﾞ間隔設定ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(&gspc, &gitv);
		View->SetGridSpace((int)(gspc*1000.0));							// ｸﾞﾘｯﾄﾞ間隔設定(μm単位)
		View->SetGridIntvl(gitv);										// 目盛間隔設定
		View->SetGrid();												// ｸﾞﾘｯﾄﾞ設定
		View->Display(0);												// 再描画
	}
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_GRID_SPACE, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/*************************************/
void CMainFrame::OnGuideLineBox(void)									//<<< ｢GLB｣釦押下
/*************************************/
{
	int    dsp=-1, chg, kind;
	double size;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_GLB);				// 釦押下状態
	chg = View->GetGlbChange();											// ﾘﾝｸﾞｻｲｽﾞ変更指示取得
	if(View->GetChar()=='0') {											// 0ｷｰ押下はなし
		dsp = 0;
	} else if(View->GetChar()=='1') {									// 1ｷｰ押下はGLB1
		dsp = 1;
	} else if(View->GetChar()=='2') {									// 2ｷｰ押下はGLB2
		dsp = 2;
	} else if(View->GetChar()=='3') {									// 3ｷｰ押下はGLB3
		dsp = 3;
	} else if(View->GetChar()=='4') {									// 4ｷｰ押下はGLB4
		dsp = 4;
	} else {
		kind = View->GetRingKind();										// ﾘﾝｸﾞｻｲｽﾞ種類取得
		size = (!kind) ? View->GetRingSize1() : View->GetRingSize2();	// ﾘﾝｸﾞｻｲｽﾞ取得
		CGlbDlg dlg(kind);
		dlg.SetValue(View->GetDspGlb(), chg, size);						// 現在の設定値を表示
		if(dlg.DoModal()==IDOK) {										// GLBﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&dsp, &chg, &size);
			if(chg) {													// <ﾘﾝｸﾞｻｲｽﾞ変更OK>
				if(!kind) {
					View->SetRingSize1(size);							// 号数ﾘﾝｸﾞｻｲｽﾞ設定
				} else {
					View->SetRingSize2(size);							// 直径ﾘﾝｸﾞｻｲｽﾞ設定
				}
			}
		}
	}
	if(dsp>=0) {														// <GLB指定OK>
		View->SetDspGlb(dsp);											// GLB表示No設定
		View->SetGlbChange(chg);										// ﾘﾝｸﾞｻｲｽﾞ変更指示設定
		if(View->ReadGLB()!=TRUE) {										// GLB入力
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
			View->DeleteGLB();											// GLB破棄
			View->SetDspGlb(0);
		}
		View->Display(0);												// 再表示
	}
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_GLB, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnDspTemplate(void)									//<<< ｢ﾃﾝﾌﾟﾚｰﾄ表示｣釦押下
/************************************/
{
	int    no=-1, chg, kind;
	double size;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_TEMP);				// 釦押下状態
	chg = View->GetGlbChange();											// ﾘﾝｸﾞｻｲｽﾞ変更指示取得
	if(View->GetChar()=='0') {											// 0ｷｰ押下はなし
		no = 0;
	} else if(View->GetChar()=='1') {									// 1ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ1
		no = 1;
	} else if(View->GetChar()=='2') {									// 2ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ2
		no = 2;
	} else if(View->GetChar()=='3') {									// 3ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ3
		no = 3;
	} else if(View->GetChar()=='4') {									// 4ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ4
		no = 4;
	} else if(View->GetChar()=='5') {									// 5ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ5
		no = 5;
	} else if(View->GetChar()=='6') {									// 6ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ6
		no = 6;
	} else if(View->GetChar()=='7') {									// 7ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ7
		no = 7;
	} else if(View->GetChar()=='8') {									// 8ｷｰ押下はﾃﾝﾌﾟﾚｰﾄ8
		no = 8;
	} else {
		kind = View->GetRingKind();										// ﾘﾝｸﾞｻｲｽﾞ種類取得
		size = (!kind) ? View->GetRingSize1() : View->GetRingSize2();	// ﾘﾝｸﾞｻｲｽﾞ取得
		CDispTmpDlg dlg(kind);
		dlg.SetValue(View->GetDspTmpNo(), chg, size);					// 現在の設定値を表示
		if(dlg.DoModal()==IDOK) {										// GLBﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&no, &chg, &size);
			if(chg) {													// <ﾘﾝｸﾞｻｲｽﾞ変更OK>
				if(!kind) {
					View->SetRingSize1(size);							// 号数ﾘﾝｸﾞｻｲｽﾞ設定
				} else {
					View->SetRingSize2(size);							// 直径ﾘﾝｸﾞｻｲｽﾞ設定
				}
			}
		}
	}
	if(no>=0) {															// <ﾃﾝﾌﾟﾚｰﾄ指定OK>
		View->SetDspTmpNo(no);											// ﾃﾝﾌﾟﾚｰﾄ表示No設定
		View->SetGlbChange(chg);										// ﾘﾝｸﾞｻｲｽﾞ変更指示設定
		if(View->ReadTEMP()!=TRUE) {									// ﾃﾝﾌﾟﾚｰﾄ入力
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
			View->DeleteTEMP();											// ﾃﾝﾌﾟﾚｰﾄ破棄
			View->SetDspTmpNo(0);
		}
		View->Display(0);												// 再表示
	}
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_TEMP, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnSetTemplate(void)									//<<< ｢ﾃﾝﾌﾟﾚｰﾄ設定｣釦押下
/************************************/
{
	CString b1, b2, b3;
	int     i;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	CSetTmpDlg dlg;
	b1 = View->GetTmpltPath();											// ﾃﾝﾌﾟﾚｰﾄﾊﾟｽ取得
	dlg.SetValue1(b1);													// 既存値表示(ﾊﾟｽ)
	for(i=0; i<8; i++) {
		View->GetTmpFile(i, &b1, &b2, &b3);								// ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ名取得
		dlg.SetValue2(i, b1, b2, b3);									// 既存値表示(ﾌｧｲﾙ名)
	}
	if(dlg.DoModal()==IDOK) {											// ﾃﾝﾌﾟﾚｰﾄ設定ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue1(&b1);
		View->SetTmpltPath(b1);											// ﾃﾝﾌﾟﾚｰﾄﾊﾟｽ設定
		for(i=0; i<8; i++) {
			dlg.GetValue2(i, &b1, &b2, &b3);
			View->SetTmpFile(i, b1, b2, b3);							// ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ名設定
		}
	}
	if(View->GetDspTmpNo()) {											// ﾃﾝﾌﾟﾚｰﾄ表示No有効な場合
		if(View->ReadTEMP()!=TRUE) {									// ﾃﾝﾌﾟﾚｰﾄ入力
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
			View->DeleteTEMP();											// ﾃﾝﾌﾟﾚｰﾄ破棄
			View->SetDspTmpNo(0);
		}
		View->Display(0);												// 再表示
	}
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnTexture(void)										//<<< ｢ﾃｸｽﾁｬ設定｣釦押下
/********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	CSetTexDlg dlg;
	dlg.DoModal();														// ﾃｸｽﾁｬ設定ﾀﾞｲｱﾛｸﾞ表示
	View->Display(0);													// 再表示
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnSetHome(void)										//<<< ｢HOME登録｣釦押下
/********************************/
{
	int func[8], no=1;

	if(View->GetChar()=='1') {											// [1]ｷｰ押下はHome1
		no = 1;
	} else if(View->GetChar()=='2') {									// [2]ｷｰ押下はHome2
		no = 2;
	} else if(View->GetChar()=='3') {									// [3]ｷｰ押下はHome3
		no = 3;
	} else if(View->GetChar()=='4') {									// [4]ｷｰ押下はHome4
		no = 4;
	} else if(View->GetChar()=='5') {									// [5]ｷｰ押下はHome5
		no = 5;
	} else {															// <[1]～[5]ｷｰ押下なし>
		View->GetFChar(func);											// ﾌｧﾝｸｼｮﾝｷｰ取得
		if(func[0]) {													// [F5]ｷｰ押下はHome2
			no = 2;
		} else if(func[1]) {											// [F6]ｷｰ押下はHome3
			no = 3;
		} else if(func[2]) {											// [F7]ｷｰ押下はHome4
			no = 4;
		} else if(func[3]) {											// [F8]ｷｰ押下はHome5
			no = 5;
		}
	}
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->SetHomePosition(no);											// 表示状態保存
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnRayTrace(void)										// ｢ﾚﾝﾀﾞﾘﾝｸﾞ(ﾚｲ･ﾄﾚ)｣釦押下
/*********************************/
{
	if(!View->IsRendDlgShow()) {
		View->CreateRendDlg();											// ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞ生成
	}
}

/*************************************************/
void CMainFrame::OnUpdateRayTrace(CCmdUI* pCmdUI)						//<<< ｢ﾚﾝﾀﾞﾘﾝｸﾞ(ﾚｲ･ﾄﾚ)｣釦更新
/*************************************************/
{
	UINT id=pCmdUI->m_nID;												// ｺﾏﾝﾄﾞID取得
	pCmdUI->Enable(View->GetDspObjNum()>0?TRUE:FALSE);					// 表示対象の有無にて更新
	pCmdUI->SetCheck(View->IsRendDlgShow());							// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/********************************/
void CMainFrame::OnPovData(void)										// ｢POV-Rayﾃﾞｰﾀ出力｣釦押下
/********************************/
{
	CString str, file;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	if(GetDemoFlg()!=0) {												// ｢体験版,起動NG｣
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	View->ExitActMenu(ID_POVDATA);										// 前ﾒﾆｭｰ終了
	str.LoadString(IDS_STR19);
	CPovEnvSheet pvsht(str);
	if(pvsht.DoModal()==IDOK) {											// POV-Rayﾃﾞｰﾀ出力環境設定ﾀﾞｲｱﾛｸﾞ
		pvsht.SetPovRayData();											// POV-Rayﾃﾞｰﾀ設定
		pvsht.GetValue(&file);
		if(file!="") {
			CreatePovData(file);										// POV-Rayﾃﾞｰﾀ作成
		}
	}
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnMaterial(void)										//<<< ｢ｶﾗｰ設定｣釦押下
/*********************************/
{
	OBJTYPE* op;
	int      dsp, edt, ocl, col=-1;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndRendering.GetToolBarCtrl().CheckButton(ID_COLOR);				// 釦押下状態
	if(View->GetChar()=='G') {											// Gｷｰ押下はｺﾞｰﾙﾄﾞ
		col = 0;
	} else if(View->GetChar()=='L') {									// Lｷｰ押下はｼﾙﾊﾞｰ
		col = 1;
	} else if(View->GetChar()=='Y') {									// Yｷｰ押下はｸﾘｽﾀﾙ
		col = 2;
	} else if(View->GetChar()=='R') {									// Rｷｰ押下はﾙﾋﾞｰ
		col = 3;
	} else if(View->GetChar()=='E') {									// Eｷｰ押下はｴﾒﾗﾙﾄﾞ
		col = 4;
	} else if(View->GetChar()=='S') {									// Lｷｰ押下はｻﾌｧｲｱ
		col = 5;
	} else if(View->GetChar()=='1') {									// 1ｷｰ押下はﾕｰｻﾞｰ1
		col = 6;
	} else if(View->GetChar()=='2') {									// 2ｷｰ押下はﾕｰｻﾞｰ2
		col = 7;
	} else if(View->GetChar()=='3') {									// 3ｷｰ押下はﾕｰｻﾞｰ3
		col = 8;
	} else if(View->GetChar()=='4') {									// 4ｷｰ押下はﾕｰｻﾞｰ4
		col = 9;
	} else if(View->GetChar()=='5') {									// 5ｷｰ押下はﾕｰｻﾞｰ5
		col = 10;
	} else if(View->GetChar()=='6') {									// 6ｷｰ押下はﾕｰｻﾞｰ6
		col = 11;
	} else if(View->GetChar()=='7') {									// 7ｷｰ押下はﾕｰｻﾞｰ7
		col = 12;
	} else if(View->GetChar()=='8') {									// 8ｷｰ押下はﾕｰｻﾞｰ8
		col = 13;
	} else if(View->GetChar()=='9') {									// 9ｷｰ押下はﾕｰｻﾞｰ9
		col = 14;
	} else if(View->GetChar()=='A') {									// Aｷｰ押下はﾕｰｻﾞｰ10
		col = 15;
	} else if(View->GetChar()=='B') {									// Bｷｰ押下はﾕｰｻﾞｰ11
		col = 16;
	} else if(View->GetChar()=='C') {									// Cｷｰ押下はﾕｰｻﾞｰ12
		col = 17;
	} else {
		BaseObjPtr(2, &op, &dsp, &edt);									// 先頭立体のﾎﾟｲﾝﾀ取得
		ocl = (op) ? GetObjCol(op) : 0;									// 立体のｶﾗｰ取得
		CColorDlg dlg;
		dlg.SetValue(ocl);
		if(dlg.DoModal()==IDOK) {										// 材質設定ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&col);
		}
	}
	if(col>=0) {														// <材質設定有効>
		BaseObjPtr(2, &op, &dsp, &edt);									// 先頭立体のﾎﾟｲﾝﾀ取得
		while(op) {
			PutObjCol(op, col);											// 立体のｶﾗｰ設定
			NextObjPtr(2, &op, &dsp, &edt);								// 次立体のﾎﾟｲﾝﾀ取得
		}
		if(View->GetRenderMode()==2||View->GetRenderMode()==3||
           View->GetDispMode()==4) {
			View->Display(0);											// ﾚﾝﾀﾞﾘﾝｸﾞ/ｼｪｰﾃﾞｨﾝｸﾞは再表示
		}
	}
	m_wndRendering.GetToolBarCtrl().CheckButton(ID_COLOR, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/*************************************/
void CMainFrame::OnCrtPrimitive(void)									//<<< ｢ﾌﾟﾘﾐﾃｨﾌﾞ｣釦押下
/*************************************/
{
	int pno=-1;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_PRIMITIVE);			// 釦押下状態
	if(View->GetChar()=='C') {											// Cｷｰ押下は円
		pno = 0;
	} else if(View->GetChar()=='Q') {									// Qｷｰ押下は長方形
		pno = 1;
	} else if(View->GetChar()=='S') {									// Sｷｰ押下は球体
		pno = 2;
	} else if(View->GetChar()=='B') {									// Bｷｰ押下は直方体
		pno = 3;
	} else if(View->GetChar()=='Y') {									// Yｷｰ押下は円柱
		pno = 4;
	} else if(View->GetChar()=='N') {									// Nｷｰ押下は円錐
		pno = 5;
	} else {
		CPrimitiveDlg dlg;
		if(dlg.DoModal()==IDOK) {										// ﾌﾟﾘﾐﾃｨﾌﾞﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&pno);
		}
	}
	if(pno>=0) {														// <ﾌﾟﾘﾐﾃｨﾌﾞ指定OK>
		View->ExitActMenu(ID_CRT_PRIMITIVE);							// 前ﾒﾆｭｰ終了
		if(!ReadPrimitive(pno)) {										// ﾌﾟﾘﾐﾃｨﾌﾞ入力
			View->CountEdtObj();										// 編集対象立体ｶｳﾝﾄ
			View->Display(0);											// 再描画
		} else {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
		View->ExitMenu(0, 1);											// ﾒﾆｭｰ終了
	}
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_PRIMITIVE,FALSE);	// 釦解放状態
}

/*************************************/
void CMainFrame::OnCrtCentCurve(void)									//<<< ｢立体中心点曲線生成｣釦押下
/*************************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_CRT_CENTCURVE);								// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CENTCURVE);			// 釦押下状態
	View->MenuCentCurve();												// 立体中心点曲線生成
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CENTCURVE, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnCrtSetCurve(void)									//<<< ｢曲線組合せ｣釦押下
/************************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_CRT_SETCURVE);									// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SETCURVE);			// 釦押下状態
	View->MenuSetCurve();												// 曲線組合せ
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SETCURVE, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/***********************************/
void CMainFrame::OnCrtSection(void)										//<<< ｢断面指定｣釦押下
/***********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_CRT_SECTION);									// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SECTION);			// 釦押下状態
	View->MenuSection();												// 断面指定
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SECTION, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnCrtSpiral(void)										//<<< ｢螺旋｣釦押下
/**********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_CRT_SPIRAL);									// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SPIRAL);			// 釦押下状態
	View->MenuSpiral();													// 螺旋生成
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SPIRAL, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/*************************************/
void CMainFrame::OnResetSurface(void)									//<<< ｢曲線/曲面再作成｣釦押下
/*************************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_RESET_SURFACE);								// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_RESET_SURFACE);			// 釦押下状態
	View->MenuResetSurface();											// 曲線/曲面再作成
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_RESET_SURFACE, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/***********************************/
void CMainFrame::OnCrtConnect(void)										//<<< ｢連結｣釦押下
/***********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_CRT_CONNECT);									// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CONNECT);			// 釦押下状態
	View->MenuConnect();												// 連結
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CONNECT, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnCrtThick(void)										//<<< ｢厚み付け｣釦押下
/*********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_CRT_THICK);									// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_THICK);				// 釦押下状態
	View->MenuThickness();												// 厚み付け
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_THICK, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnCrtText(void)										//<<< ｢ﾃｷｽﾄ｣釦押下
/********************************/
{
	int    fno, chk;
	double hei, dpt, itv;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_CRT_TEXT);										// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_TEXT);				// 釦押下状態
	fno = View->GetFontNo();											// 既存値取得
	chk = View->GetFontCheck();
	hei = View->GetTextHeight();
	dpt = View->GetTextDepth();
	itv = View->GetTextIntval();
	CSetTextDlg dlg;
	dlg.SetValue(fno, chk, hei, dpt, itv);								// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 立体文字生成
		dlg.GetValue(&fno, &chk, &hei, &dpt, &itv);						// 立体文字生成情報取得
		View->SetFontNo(fno);											// 既存値保存
		View->SetFontCheck(chk);
		View->SetTextHeight(hei);
		View->SetTextDepth(dpt);
		View->SetTextIntval(itv);
	}
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_TEXT, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnEditSurface(void)									//<<< ｢曲面指定移動｣釦押下
/************************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_EDIT_SURFACE);									// 前ﾒﾆｭｰ終了
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_SURFACE);			// 釦押下状態
	View->MenuSurfaceEdit();											// 曲面指定移動
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_SURFACE, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/*********************************/
void CMainFrame::OnEditBool(void)										//<<< ｢図形演算｣釦押下
/*********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_EDIT_BOOL);									// 前ﾒﾆｭｰ終了
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_BOOL);				// 釦押下状態
	View->MenuBoolean();												// 図形演算
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_BOOL, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnSurfOAdjust(void)									//<<< ｢曲面立体合わせ｣釦押下
/************************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_SURF_O_ADJUST);								// 前ﾒﾆｭｰ終了
	m_wndSurf.GetToolBarCtrl().CheckButton(ID_SURF_O_ADJUST);			// 釦押下状態
	View->MenuObjectAdjust();											// 曲面立体合わせ
	m_wndSurf.GetToolBarCtrl().CheckButton(ID_SURF_O_ADJUST, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnSurfParam(void)										//<<< ｢曲線ﾊﾟﾗﾒｰﾀ｣釦押下
/**********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_SURF_PARAM);									// 前ﾒﾆｭｰ終了
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_SURF_PARAM);			// 釦押下状態
	View->MenuChgSurfParam(0);											// 曲線ﾊﾟﾗﾒｰﾀ変更
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_SURF_PARAM, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnJewelryRing(void)									//<<< ｢ﾘﾝｸﾞｻｲｽﾞ合わせ｣釦押下
/************************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_JWL_RING);										// 前ﾒﾆｭｰ終了
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_RING);				// 釦押下状態
	View->MenuJwlRing();												// ﾘﾝｸﾞｻｲｽﾞ合わせ
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_RING, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/********************************/
void CMainFrame::OnSetUnit(void)										//<<< ｢ﾕﾆｯﾄ･ｵﾌﾞｼﾞｪｸﾄ｣釦押下
/********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_JWL_UNIT);										// 前ﾒﾆｭｰ終了
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_UNIT);				// 釦押下状態
	View->MenuSetUnit();												// ﾕﾆｯﾄ･ｵﾌﾞｼﾞｪｸﾄ
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_UNIT, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnMeasureCalc(void)									//<<< ｢計算機｣釦押下
/************************************/
{
	if(!View->IsCalcDlg1Show()&&!View->IsCalcDlg2Show()) {
		View->CreateCalcDlg1();											// 計算機ﾀﾞｲｱﾛｸﾞ1生成
	}
}

/****************************************************/
void CMainFrame::OnUpdateMeasureCalc(CCmdUI* pCmdUI)					//<<< ｢計算機｣釦更新
/****************************************************/
{
	pCmdUI->SetCheck(View->IsCalcDlg1Show()||View->IsCalcDlg2Show());	// ﾂｰﾙﾊﾞｰ釦&ﾒﾆｭｰﾁｪｯｸ
}

/**************************************/
void CMainFrame::OnMeasureVolume(void)									//<<< ｢体積測定｣釦押下
/**************************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_MEASURE_VOLUME);								// 前ﾒﾆｭｰ終了
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_MEASURE_VOLUME);		// 釦押下状態
	View->MenuMeasureVolume();											// 体積測定
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_MEASURE_VOLUME, FALSE);// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/***********************************/
void CMainFrame::OnSimulation(void)										//<<< ｢ｼﾐｭﾚｰｼｮﾝ｣釦押下
/***********************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_NC_SIMULATE);									// 前ﾒﾆｭｰ終了
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_SIMULATE);			// 釦押下状態
	View->MenuSimulation();												// ｼﾐｭﾚｰｼｮﾝ
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_SIMULATE, FALSE);	// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/*******************************/
void CMainFrame::OnNcDisp(void)											//<<< ｢NCﾃﾞｰﾀ表示｣釦押下
/*******************************/
{
	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_NC_DISP);										// 前ﾒﾆｭｰ終了
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_DISP);				// 釦押下状態
	View->MenuNCDisplay();												// NCﾃﾞｰﾀ表示
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_DISP, FALSE);		// 釦解放状態
	View->InitChar();													// ｷｰ入力初期化
}

/***************************************/
void CMainFrame::OnSetEnvironment(void)									//<<< ｢環境設定｣釦押下
/***************************************/
{
	CString str, pth;
	int     i, key, rep, atr, sec, shp, shd, max, sc1, sc2, btn;
	int     n10, n11, n12, n13, n14, n15, n16, n17, n18, n19;
	int     n20, n21, n22, n23, n24, n25, n26, n27, n28, n29;
	double  wgt;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_SET_ENV);										// 前ﾒﾆｭｰ終了
	key = View->GetKeyEnv();											// 制御ｷｰ情報取得
	rep = View->GetCurveRepeat();										// 曲線分割数取得
	wgt = View->GetCurveWeight();										// 曲線重み係数取得
	atr = View->GetAtrEnv();											// 曲面生成立体属性情報取得
	sec = View->GetSecEnv();											// 曲面生成端面形状情報取得
	shp = View->GetShpEnv();											// 曲面編集形状情報取得
	shd = View->GetShdEnv();											// 曲面編集ｼｪｰﾃﾞｨﾝｸﾞ情報取得
	max = View->GetSaveFileMax();										// 最大ﾊﾞｯｸｱｯﾌﾟ数取得
	pth = View->GetSaveFilePath();										// ﾊﾞｯｸｱｯﾌﾟ先ﾌｫﾙﾀﾞ取得	
	sc1 = View->GetScrEnv1();											// 画面初期化情報取得
	sc2 = View->GetScrEnv2();
	btn = View->GetBtnEnv();											// ﾓｰﾄﾞﾎﾞﾀﾝ情報取得
	n10 = GetIntMax();													// 整数ﾊﾟﾗﾒｰﾀ配列数取得
	n11 = GetPntMax();													// 座標ﾊﾟﾗﾒｰﾀ配列数取得
	n12 = GetUndoMax();													// Undo/Redo配列数取得
	n13 = GetCCtlMax();													// 最大曲線制御点数取得
	n14 = GetSCtlMax();													// 最大曲面制御点数取得
	n15 = GetWCtlMax();													// 作業用制御点配列数取得
	n16 = GetCPntMax();													// 最大曲線頂点数取得
	n17 = GetSPntMax();													// 最大曲面頂点数取得
	n18 = GetOPntMax();													// 最大立体頂点数取得
	n19 = GetBPntMax();													// 最大断面頂点数取得
	str.LoadString(IDS_STR17);
	CSysEnvSheet sesht(str);
	sesht.SetValue(key, rep, wgt, atr, sec, shp, shd,
                   max, pth, sc1, sc2, btn, n10, n11,
                   n12, n13, n14, n15, n16, n17, n18, n19);
	if(sesht.DoModal()==IDOK) {											// 環境設定ﾀﾞｲｱﾛｸﾞ
		sesht.GetValue(&key, &rep, &wgt, &atr, &sec, &shp, &shd,
                       &max, &pth, &sc1, &sc2, &btn, &n20, &n21,
                       &n22, &n23, &n24, &n25, &n26, &n27, &n28, &n29);
		View->SetKeyEnv(key);											// 制御ｷｰ情報設定
		View->SetCurveRepeat(rep);										// 曲線分割数設定
		View->SetCurveWeight(wgt);										// 曲線重み係数設定
		View->SetAtrEnv(atr);											// 曲面生成立体属性情報設定
		View->SetSecEnv(sec);											// 曲面生成端面形状情報設定
		View->SetShpEnv(shp);											// 曲面編集形状情報設定
		View->SetShdEnv(shd);											// 曲面編集ｼｪｰﾃﾞｨﾝｸﾞ情報設定
		View->SetSaveFileMax(max);										// 最大ﾊﾞｯｸｱｯﾌﾟ数設定
		View->SetSaveFilePath(pth);										// ﾊﾞｯｸｱｯﾌﾟ先ﾌｫﾙﾀﾞ設定	
		View->SetScrEnv1(sc1);											// 画面初期化情報設定
		View->SetScrEnv2(sc2);
		View->SetCtrlKey();												// 制御ｷｰ設定
		View->SetBtnEnv(btn);											// ﾓｰﾄﾞﾎﾞﾀﾝ情報設定
		if(n10!=n20||n11!=n21||n12!=n22||n13!=n23||n14!=n24||
           n15!=n25||n16!=n26||n17!=n27||n18!=n28||n19!=n29) {			// <領域数変更あり>
			SetIntMax(n20);												// 整数ﾊﾟﾗﾒｰﾀ配列数設定
			SetPntMax(n21);												// 座標ﾊﾟﾗﾒｰﾀ配列数設定
			SetUndoMax(n22);											// Undo/Redoﾊﾟﾗﾒｰﾀ配列数設定
			SetCCtlMax(n23);											// 最大曲線制御点数設定
			SetSCtlMax(n24);											// 最大曲面制御点数設定
			SetWCtlMax(n25);											// 作業用制御点配列数設定
			SetCPntMax(n26);											// 最大曲線頂点数設定
			SetSPntMax(n27);											// 最大曲面頂点数設定
			SetOPntMax(n28);											// 最大立体頂点数設定
			SetBPntMax(n29);											// 最大断面頂点数設定
			AfxMessageBox(IDS_CHANGE_MAX);								// ﾒｯｾｰｼﾞ
			WriteMemDataFile();											// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ書込
		}
		if(sesht.GetCUpdate()) {										// <表示色変更あり>
			for(i=0; i<44; i++) {
				SetColor(i, sesht.GetColor(i));							// 表示色変更
			}
			View->ResetColor();											// ｽｸﾘｰﾝｶﾗｰ再設定
		}
	}
	View->InitChar();													// ｷｰ入力初期化
	SetCrtPrm(rep, wgt);												// 生成ﾊﾟﾗﾒｰﾀ設定
}

/**********************************/
void CMainFrame::OnSetRender(void)										//<<< ｢ﾚﾝﾀﾞﾘﾝｸﾞ環境設定｣釦押下
/**********************************/
{
	CString str;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_SET_RENDER);									// 前ﾒﾆｭｰ終了
	View->SaveRender();													// ﾚﾝﾀﾞﾘﾝｸﾞ情報保存
	View->StartRenderTest();											// ﾚﾝﾀﾞﾘﾝｸﾞﾃｽﾄ開始処理
	str.LoadString(IDS_STR18);
	CRndEnvSheet  resht(str);
	if(resht.DoModal()!=IDOK) {											// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定ﾀﾞｲｱﾛｸﾞ
		View->RestoreRender();											// ﾚﾝﾀﾞﾘﾝｸﾞ情報復元
	}
	View->EndRenderTest();												// ﾚﾝﾀﾞﾘﾝｸﾞﾃｽﾄ終了処理
	View->InitChar();													// ｷｰ入力初期化
}

/************************************/
void CMainFrame::OnSetRayTrace(void)									//<<< ｢ﾚｲﾄﾚｰｼﾝｸﾞ環境設定｣釦押下
/************************************/
{
	CString str;

	View->InitializeMenu();												// ﾒﾆｭｰ初期処理
	View->ExitActMenu(ID_SET_RAYTRACE);									// 前ﾒﾆｭｰ終了
	str.LoadString(IDS_STR20);
	CRayEnvSheet  resht(str);
	if(resht.DoModal()==IDOK) {											// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定ﾀﾞｲｱﾛｸﾞ
		resht.SetRayTraceData();										// ﾚｲ･ﾄﾚｰｽﾃﾞｰﾀ設定
	}
	View->InitChar();													// ｷｰ入力初期化
}

/**********************************/
void CMainFrame::OnRefManual(void)										//<<< ｢ﾏﾆｭｱﾙ参照｣釦押下
/**********************************/
{
	ShellExecute(NULL, "open", "http://jcad3.net/jcadmanual.html", NULL, NULL, SW_SHOWNORMAL);
}

/***********************************/
void CMainFrame::OnRegistUser(void)										//<<< ｢ﾕｰｻﾞｰ登録｣釦押下
/***********************************/
{
	ShellExecute(NULL, "open", "http://www.jcad3.net/USERREGI/userregi.html", NULL, NULL, SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame オペレーション

/*********************************/
static BOOL FindCopyFile(int chr)										//<<< ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ検索
/*********************************/
{
	WIN32_FIND_DATA wfd;
	CString path;

	if(chr==0) {														// <Key押下なし>
		path = GetAppPath(0)+COP_NAME1;									// ｺﾋﾟｰﾌｧｲﾙPATH取得
	} else {															// <Key押下あり>
		path.Format(COP_NAME2, chr);									// Key付ｺﾋﾟｰﾌｧｲﾙ
		path = GetAppPath(0)+path;										// ｺﾋﾟｰﾌｧｲﾙPATH取得
	}
	if(FindFirstFile(path, &wfd)==INVALID_HANDLE_VALUE) {				// ﾌｧｲﾙ無の場合
		return FALSE;
	} else {
		return TRUE;
	}
}

/*********************************/
void CMainFrame::Initialize(void)										//<<< 初期化
/*********************************/
{
	int i;

	View->SetSaveMenu(NONE);
	View->ExitActMenu(View->GetActMenu());
	for(i=0; i<32; i++) {
		m_GroupObj[i] = 0;												// ｸﾞﾙｰﾌﾟ表示全未登録
		m_GroupDisp[i] = 1;												// ｸﾞﾙｰﾌﾟ表示全表示
	}
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_NUM_INPUT, FALSE);		// 数値入力釦OFF
	m_bViewSave = FALSE;												// ﾋﾞｭｰﾜ保存ﾌﾗｸﾞ初期化
	m_CopyCmdFlg = 0;
	if(FindCopyFile(0)) {												// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ検索
		m_CopyCmdFlg = 1;												// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙあり
		return;
	}
	for(i=(int)'0'; i<=(int)'9'; i++) {									// '0'～'9'を検索
		if(FindCopyFile(i)) {											// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ検索
			m_CopyCmdFlg = 1;											// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙあり
			return;
		}
	}
	for(i=(int)'A'; i<=(int)'Z'; i++) {									// 'A'～'Z'を検索
		if(FindCopyFile(i)) {											// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ検索
			m_CopyCmdFlg = 1;											// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙあり
			return;
		}
	}

//202006 del GroupBar
	ShowControlBar(&m_wndGroup, FALSE, TRUE);// ｺﾝﾄﾛｰﾙﾊﾞｰ非表示 //202006 Add
//	ShowControlBar(&m_wndView, FALSE, TRUE);// ｺﾝﾄﾛｰﾙﾊﾞｰ非表示 //202006 Add


}

/*************************************/
void CMainFrame::InitSColorFile(void)									//<<< 初期ｼｽﾃﾑｶﾗｰﾌｧｲﾙ読込
/*************************************/
{
	char*   buf = new char[100];
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCN_NAME;										// JCAD3ｶﾗｰ設定ﾌｧｲﾙ名(新版)
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<44; i++) {
			if(fscanf_s(fp, "%s%d%d%d", buf, 100, &r, &g, &b)!=4) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// 異常ﾃﾞｰﾀは中止
			}
			m_Color[i] = RGB(r, g, b);                                  // ｶﾗｰの書き換え
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
	}
	delete[] buf;
}

/*************************************/
void CMainFrame::ReadSColorFile(void)									//<<< ｼｽﾃﾑｶﾗｰ保存ﾌｧｲﾙ読込
/*************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCS_NAME;										// JCAD3ｶﾗｰ設定保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<44; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// 異常ﾃﾞｰﾀは中止
			}
			m_Color[i] = RGB(r, g, b);									// ｶﾗｰの書き換え
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	}
}

/**************************************/
void CMainFrame::WriteSColorFile(void)									//<<< ｼｽﾃﾑｶﾗｰ保存ﾌｧｲﾙ書込
/**************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCS_NAME;										// JCAD3ｶﾗｰ設定保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<44; i++) {
			r = GetRValue(m_Color[i]);
			g = GetGValue(m_Color[i]);
			b = GetBValue(m_Color[i]);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
}

/**************************************/
void CMainFrame::ReadFilPathFile(void)									//<<< ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ読込
/**************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;

	path = GetAppPath(0)+FPS_NAME;										// ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while(TRUE) {
			memset(spath, NULL, _MAX_PATH);

			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if(strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_sFilePath.Format("%s", lpath);
				if(m_sFilePath=="") {
					m_sFilePath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_OpenKind, &m_SaveKind,
                                      &m_ViewerKind)!=3) {				// ﾌｧｲﾙ入出力種類取得
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d%d%d", &i0, &i1, &i2, &i3, &i4)!=5) {// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ取得
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d%d%d", &i5, &i6, &i7, &i8, &i9)!=5) {
				er = 1; break;
			}
			memset(m_cWPassword, NULL, sizeof(m_cWPassword));
			m_cWPassword[0] = (char)i4;									// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ変換
			m_cWPassword[1] = (char)i7;
			m_cWPassword[2] = (char)i9;
			m_cWPassword[3] = (char)i1;
			m_cWPassword[4] = (char)i8;
			m_cWPassword[5] = (char)i2;
			m_cWPassword[6] = (char)i5;
			m_cWPassword[7] = (char)i0;
			m_cWPassword[8] = (char)i3;
			m_cWPassword[9] = (char)i6;
			if(fscanf_s(fp, "%d%d%d%d%d", &i0, &i1, &i2, &i3, &i4)!=5) {// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ取得
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d%d%d", &i5, &i6, &i7, &i8, &i9)!=5) {
				er = 1; break;
			}
			memset(m_cRPassword, NULL, sizeof(m_cRPassword));
			m_cRPassword[0] = (char)i9;									// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ変換
			m_cRPassword[1] = (char)i3;
			m_cRPassword[2] = (char)i1;
			m_cRPassword[3] = (char)i6;
			m_cRPassword[4] = (char)i2;
			m_cRPassword[5] = (char)i8;
			m_cRPassword[6] = (char)i0;
			m_cRPassword[7] = (char)i7;
			m_cRPassword[8] = (char)i4;
			m_cRPassword[9] = (char)i5;
			if(fscanf_s(fp, "%d", &m_PasswordKind)!=1) {				// ﾊﾟｽﾜｰﾄﾞ種類取得
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	}
	delete[] spath;
	delete[] lpath;
}

/***************************************/
void CMainFrame::WriteFilPathFile(void)									//<<< ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ書込
/***************************************/
{
	char*   spath= new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;

	path = GetAppPath(0)+FPS_NAME;										// ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while(TRUE) {
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sFilePath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_OpenKind, m_SaveKind,
                                         m_ViewerKind)<0) {				// ﾌｧｲﾙ入出力種類設定
				er = 1; break;
			}
			i4 = (int)m_cWPassword[0];									// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ変換
			i7 = (int)m_cWPassword[1];
			i9 = (int)m_cWPassword[2];
			i1 = (int)m_cWPassword[3];
			i8 = (int)m_cWPassword[4];
			i2 = (int)m_cWPassword[5];
			i5 = (int)m_cWPassword[6];
			i0 = (int)m_cWPassword[7];
			i3 = (int)m_cWPassword[8];
			i6 = (int)m_cWPassword[9];
			if(fprintf(fp, "%d %d %d %d %d\n", i0, i1, i2, i3, i4)<0) {	// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ設定
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d %d %d\n", i5, i6, i7, i8, i9)<0) {
				er = 1; break;
			}
			i9 = (int)m_cRPassword[0];									// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ変換
			i3 = (int)m_cRPassword[1];
			i1 = (int)m_cRPassword[2];
			i6 = (int)m_cRPassword[3];
			i2 = (int)m_cRPassword[4];
			i8 = (int)m_cRPassword[5];
			i0 = (int)m_cRPassword[6];
			i7 = (int)m_cRPassword[7];
			i4 = (int)m_cRPassword[8];
			i5 = (int)m_cRPassword[9];
			if(fprintf(fp, "%d %d %d %d %d\n", i0, i1, i2, i3, i4)<0) {	// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ設定
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d %d %d\n", i5, i6, i7, i8, i9)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d\n", m_PasswordKind)<0) {					// ﾊﾟｽﾜｰﾄﾞ種類設定
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	delete[] spath;
}

/*******************************************************************/
void CMainFrame::GetPasswordData(char* wpwd, char* rpwd, int* save)		//<<< ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ取得
/*******************************************************************/
{
	memcpy(wpwd, m_cWPassword, sizeof(m_cWPassword));					// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ複写
	memcpy(rpwd, m_cRPassword, sizeof(m_cRPassword));					// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ複写
	*save = m_PasswordSave;												// ﾊﾟｽﾜｰﾄﾞ保存ﾌﾗｸﾞ
}

/********************************/
void CMainFrame::InitColor(void)										//<<< 表示色初期化
/********************************/
{
	ReadSColorFile();													// ｼｽﾃﾑｶﾗｰ保存ﾌｧｲﾙ読込
	m_Color[44] = RGB(0, 0, 0);											// 黒色(固定)
	m_Color[45] = RGB(255, 255, 255);									// 白色(固定)
	BackColor     = 0;													// 背景色No
	ObjColor      = 1;													// 立体表示色(ｸﾞﾙｰﾌﾟ無)No
	GroupColor1   = 2;													// ｸﾞﾙｰﾌﾟ1表示色No
	GroupColor2   = 3;													// ｸﾞﾙｰﾌﾟ2表示色No
	GroupColor3   = 4;													// ｸﾞﾙｰﾌﾟ3表示色No
	GroupColor4   = 5;													// ｸﾞﾙｰﾌﾟ4表示色No
	GroupColor5   = 6;													// ｸﾞﾙｰﾌﾟ5表示色No
	GroupColor6   = 7;													// ｸﾞﾙｰﾌﾟ6表示色No
	GroupColor7   = 8;													// ｸﾞﾙｰﾌﾟ7表示色No
	GroupColor8   = 9;													// ｸﾞﾙｰﾌﾟ8表示色No
	GroupColor9   = 10;													// ｸﾞﾙｰﾌﾟ9表示色No
	GroupColor10  = 11;													// ｸﾞﾙｰﾌﾟ10表示色No
	GroupColor11  = 12;													// ｸﾞﾙｰﾌﾟ11表示色No
	GroupColor12  = 13;													// ｸﾞﾙｰﾌﾟ12表示色No
	GroupColor13  = 14;													// ｸﾞﾙｰﾌﾟ13表示色No
	GroupColor14  = 15;													// ｸﾞﾙｰﾌﾟ14表示色No
	GroupColor15  = 16;													// ｸﾞﾙｰﾌﾟ15表示色No
	GroupColor16  = 17;													// ｸﾞﾙｰﾌﾟ16表示色No
	SelectColor   = 18;													// 選択立体表示色No
	ChooseColor   = 19;													// 選択解除立体表示色No
	BoolColor     = 20;													// 図形演算表示色No
	GridColor     = 21;													// ｸﾞﾘｯﾄﾞ表示色No
	GridBPColor   = 22;													// ｸﾞﾘｯﾄﾞ基準点表示色No
	TemplateColor = 23;													// ﾃﾝﾌﾟﾚｰﾄ表示色No
	BaseColor     = 24;													// 基準点表示色No
	BoxColor      = 25;													// ﾎﾞｯｸｽ表示色No
	CircleColor   = 26;													// ｻｰｸﾙ表示色No
	LineColor     = 27;													// ﾗｲﾝ表示色No
	MeasureColor  = 28;													// 定規表示色No
	SelLineColor  = 29;													// 選択制御線表示色No
	XGuideColor   = 30;													// Xｶﾞｲﾄﾞ表示色No
	YGuideColor   = 31;													// Yｶﾞｲﾄﾞ表示色No
	ZGuideColor   = 32;													// Zｶﾞｲﾄﾞ表示色No
	NCDispColor1  = 33;													// NC1表示色No
	NCDispColor2  = 34;													// NC2表示色No
	TrackerColor0 = 35;													// ﾄﾗｯｶｰ0表示色No
	TrackerColor1 = 36;													// ﾄﾗｯｶｰ1表示色No
	TrackerColor2 = 37;													// ﾄﾗｯｶｰ2表示色No
	TrackerColor3 = 38;													// ﾄﾗｯｶｰ3表示色No
	TrackerColor4 = 39;													// ﾄﾗｯｶｰ4表示色No
	TrackerColor5 = 40;													// ﾄﾗｯｶｰ5表示色No
	BorderColor   = 41;													// ﾌﾚｰﾑ色(中心境界線)
	BorderHColor  = 42;													// ﾌﾚｰﾑ色(外枠境界線[日向部])
	BorderSColor  = 43;													// ﾌﾚｰﾑ色(外枠境界線[黒影部])
	BlackColor    = 44;													// 黒色No
	WhiteColor    = 45;													// 白色No
}

/****************************************/
void CMainFrame::SaveObjColor(UINT *col)								//<<< 立体表示色保存
/****************************************/
{
	col[0]  = ObjColor;													// 立体表示色(ｸﾞﾙｰﾌﾟ無)No
	col[1]  = GroupColor1;												// ｸﾞﾙｰﾌﾟ1表示色No
	col[2]  = GroupColor2;												// ｸﾞﾙｰﾌﾟ2表示色No
	col[3]  = GroupColor3;												// ｸﾞﾙｰﾌﾟ3表示色No
	col[4]  = GroupColor4;												// ｸﾞﾙｰﾌﾟ4表示色No
	col[5]  = GroupColor5;												// ｸﾞﾙｰﾌﾟ5表示色No
	col[6]  = GroupColor6;												// ｸﾞﾙｰﾌﾟ6表示色No
	col[7]  = GroupColor7;												// ｸﾞﾙｰﾌﾟ7表示色No
	col[8]  = GroupColor8;												// ｸﾞﾙｰﾌﾟ8表示色No
	col[9]  = GroupColor9;												// ｸﾞﾙｰﾌﾟ9表示色No
	col[10] = GroupColor10;												// ｸﾞﾙｰﾌﾟ10表示色No
	col[11] = GroupColor11;												// ｸﾞﾙｰﾌﾟ11表示色No
	col[12] = GroupColor12;												// ｸﾞﾙｰﾌﾟ12表示色No
	col[13] = GroupColor13;												// ｸﾞﾙｰﾌﾟ13表示色No
	col[14] = GroupColor14;												// ｸﾞﾙｰﾌﾟ14表示色No
	col[15] = GroupColor15;												// ｸﾞﾙｰﾌﾟ15表示色No
	col[16] = GroupColor16;												// ｸﾞﾙｰﾌﾟ16表示色No
	col[17] = SelectColor;												// 選択立体表示色No
	col[18] = ChooseColor;												// 選択解除立体表示色No
	col[19] = BoolColor;												// 図形演算表示色No
}

/*******************************************/
void CMainFrame::RestoreObjColor(UINT *col)								//<<< 立体表示色復元
/*******************************************/
{
	ObjColor     = col[0];												// 立体表示色(ｸﾞﾙｰﾌﾟ無)No
	GroupColor1  = col[1];												// ｸﾞﾙｰﾌﾟ1表示色No
	GroupColor2  = col[2];												// ｸﾞﾙｰﾌﾟ2表示色No
	GroupColor3  = col[3];												// ｸﾞﾙｰﾌﾟ3表示色No
	GroupColor4  = col[4];												// ｸﾞﾙｰﾌﾟ4表示色No
	GroupColor5  = col[5];												// ｸﾞﾙｰﾌﾟ5表示色No
	GroupColor6  = col[6];												// ｸﾞﾙｰﾌﾟ6表示色No
	GroupColor7  = col[7];												// ｸﾞﾙｰﾌﾟ7表示色No
	GroupColor8  = col[8];												// ｸﾞﾙｰﾌﾟ8表示色No
	GroupColor9  = col[9];												// ｸﾞﾙｰﾌﾟ9表示色No
	GroupColor10 = col[10];												// ｸﾞﾙｰﾌﾟ10表示色No
	GroupColor11 = col[11];												// ｸﾞﾙｰﾌﾟ11表示色No
	GroupColor12 = col[12];												// ｸﾞﾙｰﾌﾟ12表示色No
	GroupColor13 = col[13];												// ｸﾞﾙｰﾌﾟ13表示色No
	GroupColor14 = col[14];												// ｸﾞﾙｰﾌﾟ14表示色No
	GroupColor15 = col[15];												// ｸﾞﾙｰﾌﾟ15表示色No
	GroupColor16 = col[16];												// ｸﾞﾙｰﾌﾟ16表示色No
	SelectColor  = col[17];												// 選択立体表示色No
	ChooseColor  = col[18];												// 選択解除立体表示色No
	BoolColor    = col[19];												// 図形演算表示色No
}

/*****************************************/
void CMainFrame::ChangeObjColor(UINT col)								//<<< 立体表示色変更
/*****************************************/
{
	ObjColor     = col;													// 立体表示色(ｸﾞﾙｰﾌﾟ無)No
	GroupColor1  = col;													// ｸﾞﾙｰﾌﾟ1表示色No
	GroupColor2  = col;													// ｸﾞﾙｰﾌﾟ2表示色No
	GroupColor3  = col;													// ｸﾞﾙｰﾌﾟ3表示色No
	GroupColor4  = col;													// ｸﾞﾙｰﾌﾟ4表示色No
	GroupColor5  = col;													// ｸﾞﾙｰﾌﾟ5表示色No
	GroupColor6  = col;													// ｸﾞﾙｰﾌﾟ6表示色No
	GroupColor7  = col;													// ｸﾞﾙｰﾌﾟ7表示色No
	GroupColor8  = col;													// ｸﾞﾙｰﾌﾟ8表示色No
	GroupColor9  = col;													// ｸﾞﾙｰﾌﾟ9表示色No
	GroupColor10 = col;													// ｸﾞﾙｰﾌﾟ10表示色No
	GroupColor11 = col;													// ｸﾞﾙｰﾌﾟ11表示色No
	GroupColor12 = col;													// ｸﾞﾙｰﾌﾟ12表示色No
	GroupColor13 = col;													// ｸﾞﾙｰﾌﾟ13表示色No
	GroupColor14 = col;													// ｸﾞﾙｰﾌﾟ14表示色No
	GroupColor15 = col;													// ｸﾞﾙｰﾌﾟ15表示色No
	GroupColor16 = col;													// ｸﾞﾙｰﾌﾟ16表示色No
	SelectColor  = col;													// 選択立体表示色No
	ChooseColor  = col;													// 選択解除立体表示色No
	BoolColor    = col;													// 図形演算表示色No
}

/******************************************************************/
void CMainFrame::GetColor(int no, double* r, double* g, double* b)		//<<< 表示色取得
/******************************************************************/
{
	*r = GetRValue(m_Color[no])/255.0;
	*g = GetGValue(m_Color[no])/255.0;
	*b = GetBValue(m_Color[no])/255.0;
}

/**************************************/
void CMainFrame::ExecuteMenu(UINT nID)									//<<< ﾌｧﾝｸｼｮﾝ選択ﾒﾆｭｰ起動
/**************************************/
{
	if(nID==ID_COPY_INPUT) {
		OnCopyInput();
	} else if(nID==ID_GROUP_SET||nID==ID_GROUP_RESET) {
		OnGroupMenu(nID);
	} else if(nID==ID_GROUP_DSP) {
		OnGroupDsp();
	} else if(nID==ID_GROUP_SEL) {
		OnGroupSel();
	} else if(nID==ID_EXACT_ZOOM) {
		OnExactZoom();
	} else if(nID==ID_HOME_POS) {
		OnHomePos();
	} else if(nID==ID_GRID_DISP) {
		OnGridDisp();
	} else if(nID==ID_GRID_BIND) {
		OnGridBind();
	} else if(nID==ID_SET_HOME) {
		OnSetHome();
	} else if(nID==ID_CRT_PRIMITIVE) {
		OnCrtPrimitive();
	} else if(nID==ID_CRT_CENTCURVE) {
		OnCrtCentCurve();
	} else if(nID==ID_CRT_SETCURVE) {
		OnCrtSetCurve();
	} else if(nID==ID_CRT_SECTION) {
		OnCrtSection();
	} else if(nID==ID_CRT_SPIRAL) {
		OnCrtSpiral();
	} else if(nID==ID_CRT_CONNECT) {
		OnCrtConnect();
	} else if(nID==ID_EDIT_SURFACE) {
		OnEditSurface();
	} else if(nID==ID_SURF_O_ADJUST) {
		OnSurfOAdjust();
	} else if(nID==ID_JWL_RING) {
		OnJewelryRing();
	} else if(nID==ID_JWL_UNIT) {
		OnSetUnit();
	} else {
		OnCommonMenu(nID);
	}
}

/**********************************/
void CMainFrame::SetGroupObj(void)										//<<< ｸﾞﾙｰﾌﾟ立体設定
/**********************************/
{
	OBJTYPE* op;
	int      i;

	for(i=0; i<32; i++) {
		GroupBaseObjPtr(i, &op);										// ｸﾞﾙｰﾌﾟ立体ﾎﾟｲﾝﾀ取得
		m_GroupObj[i] = (op!=NULL) ? 1 : 0;								// ｸﾞﾙｰﾌﾟ立体有無
	}
}

/********************************************************/
void CMainFrame::SetCurPos(double x, double y, double z)				//<<< ｶｰｿﾙ位置設定
/********************************************************/
{
	m_XPos = x, m_YPos = y, m_ZPos = z;
}

/*******************************************/
void CMainFrame::SetCrtPrm(int d, double w)								//<<< 生成ﾊﾟﾗﾒｰﾀ設定
/*******************************************/
{
	m_CrtD = d, m_CrtW = w;
}

/***********************************************************/
static CString GetsFileFilter(int mode, int kind, int* flg)				//<<< ﾌｧｲﾙﾌｨﾙﾀｰ取得
/***********************************************************/
{
	CString str;

	*flg = 0;
	if(mode==0) {														// <Open>
		str.LoadString(IDS_FILE_FILTER13);								// DJF //202006 追加
		//if (kind == 0) {
		//	str.LoadString(IDS_FILE_FILTER13);							// DJF //202006 追加
		//} else if (kind == 1) {
		//	str.LoadString(IDS_FILE_FILTER3);							// JGD
		//} else if(kind==2) {
		//	str.LoadString(IDS_FILE_FILTER2);							// JSF
		//} else if(kind==3) {
		//	str.LoadString(IDS_FILE_FILTER1);							// JSD
		//} else if(kind==4) {
		//	str.LoadString(IDS_FILE_FILTER10);							// GGD
		//} else if(kind==5) {
		//	str.LoadString(IDS_FILE_FILTER9);							// GLF
		//} else if(kind==6) {
		//	str.LoadString(IDS_FILE_FILTER8);							// GLD
		//} else if(kind==7) {
		//	str.LoadString(IDS_FILE_FILTER6);							// DXF
		//} else {
		//	str.LoadString(IDS_FILE_FILTER7);							// STL
		//}
	} else if(mode==1) {												// <Save>
		if(kind==0) {
			str.LoadString(IDS_FILE_FILTER13);							// DJF //202006 追加変更
		}
#ifdef STL_EXPORT	//202006 STL export 
		else {
			str.LoadString(IDS_FILE_FILTER7);							// STL
		}
#endif
		//if(kind==0) {
		//	str.LoadString(IDS_FILE_FILTER4);							// JGD(ﾊﾟｽﾜｰﾄﾞ無)
		//} else if(kind==1) {
		//	str.LoadString(IDS_FILE_FILTER5);							// JGD(ﾊﾟｽﾜｰﾄﾞ有)
		//	*flg = 1;
		//} else if(kind==2) {
		//	str.LoadString(IDS_FILE_FILTER2);							// JSF
		//} else if(kind==3) {
		//	str.LoadString(IDS_FILE_FILTER1);							// JSD
		//} else if(kind==4) {
		//	str.LoadString(IDS_FILE_FILTER6);							// DXF
		//} else {
		//	str.LoadString(IDS_FILE_FILTER7);							// STL
		//}

	} else {															// <Viewer>
		if(kind==0) {
			str.LoadString(IDS_FILE_FILTER11);							// GGD(ﾊﾟｽﾜｰﾄﾞ無)
		} else if(kind==1) {
			str.LoadString(IDS_FILE_FILTER12);							// GGD(ﾊﾟｽﾜｰﾄﾞ有)
			*flg = 1;
		} else if(kind==2) {
			str.LoadString(IDS_FILE_FILTER9);							// GLF
		} else if(kind==3) {
			str.LoadString(IDS_FILE_FILTER8);							// GLD
		} else if(kind==4) {
			str.LoadString(IDS_FILE_FILTER4);							// JGD(ﾊﾟｽﾜｰﾄﾞ無)
		} else if(kind==5) {
			str.LoadString(IDS_FILE_FILTER5);							// JGD(ﾊﾟｽﾜｰﾄﾞ有)
			*flg = 2;
		} else if(kind==6) {
			str.LoadString(IDS_FILE_FILTER2);							// JSF
		} else if(kind==7) {
			str.LoadString(IDS_FILE_FILTER1);							// JSD
		} else if(kind==8) {
			str.LoadString(IDS_FILE_FILTER6);							// DXF
		} else {
			str.LoadString(IDS_FILE_FILTER7);							// STL
		}
	}
	return str;
}

/**********************************************/
static int GetsFileKind(int mode, CString ext)							//<<< ﾌｧｲﾙ種類取得
/**********************************************/
{
	int kind;

	if(mode==0) {														// <Open>
		if (!ext.CompareNoCase("djf")) {								// DJF  //202006
			kind = 0;
		} 
#if 0
		else if (!ext.CompareNoCase("jgd")) {							// JGD
			kind = 1;
		} else if(!ext.CompareNoCase("jsf")) {							// JSF
			kind = 2;
		} else if(!ext.CompareNoCase("jsd")) {							// JSD
			kind = 3;
		} else if(!ext.CompareNoCase("ggd")) {							// GGD
			kind = 4;
		} else if(!ext.CompareNoCase("glf")) {							// GLF
			kind = 5;
		} else if(!ext.CompareNoCase("gld")) {							// GLD
			kind = 6;
		} else if(!ext.CompareNoCase("dxf")) {							// DXF
			kind = 7;
		} else {														// STL
			kind = 8;
		}
#endif
	} else if(mode==1) {												// <Save>
		kind = -1;
		if (!ext.CompareNoCase("djf")) {								// DJF  //202006
			kind = 0;
		}
#ifdef STL_EXPORT	//202006 STL export 
		else {														// STL
			kind = 1;
		}
#endif	
		//if (!ext.CompareNoCase("jgd")) {									// JGD
		//	kind = 01;
		//} else if(!ext.CompareNoCase("jsf")) {							// JSF
		//	kind = 2;
		//} else if(!ext.CompareNoCase("jsd")) {							// JSD
		//	kind = 3;
		//} else if(!ext.CompareNoCase("dxf")) {							// DXF
		//	kind = 4;
		//} else {															// STL
		//	kind = 5;
		//}
	} else {															// <Viewer>
		if(!ext.CompareNoCase("ggd")) {									// GGD
			kind = 0;
		} else if(!ext.CompareNoCase("glf")) {							// GLF
			kind = 2;
		} else if(!ext.CompareNoCase("gld")) {							// GLD
			kind = 3;
		} else if(!ext.CompareNoCase("jgd")) {							// JGD
			kind = 4;
		} else if(!ext.CompareNoCase("jsf")) {							// JSF
			kind = 6;
		} else if(!ext.CompareNoCase("jsd")) {							// JSD
			kind = 7;
		} else if(!ext.CompareNoCase("dxf")) {							// DXF
			kind = 8;
		} else {														// STL
			kind = 9;
		}
	}
	return kind;
}

/************************************************************************/
BOOL CMainFrame::DoPromptFName(CString& fname, CString& ext, BOOL bopen)//<<< ﾌｧｲﾙ名指定ﾀﾞｲｱﾛｸﾞ設定
/************************************************************************/
{
	CString filter, wex;
	DWORD   flg;
	BOOL    ret=FALSE;
	int     i, chk=0, cnt=1, pf, pwd[2];

	if(!CheckFilePath(0, m_sFilePath)) {								// ﾊﾟｽ確認
		m_sFilePath = GetAppPath(0);
	}
	if(bopen) {															// <<開く>>
		flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		m_OpenKind = 0;
		filter = GetsFileFilter(0, m_OpenKind, &pf);					// ﾌｧｲﾙﾌｨﾙﾀｰ取得
		//for(i=0; i<9; i++) {							//202006 8->9 -> 0　djfのみ
		//	if(m_OpenKind!=i) {
		//		filter += GetsFileFilter(0, i, &pf);					// ﾌｧｲﾙﾌｨﾙﾀｰ取得
		//	}
		//}
		filter += "|";
		chk = 1;
	} else {															// <<保存>>
		pwd[0] = -1, pwd[1] = -1;										// ﾊﾟｽﾜｰﾄﾞ有No初期化
		flg = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;
		if(m_bViewSave) {						                		// <ﾋﾞｭｰﾜ･ﾃﾞｰﾀ保存あり>
			filter = GetsFileFilter(2, m_ViewerKind, &pf);				// ﾌｧｲﾙﾌｨﾙﾀｰ取得
			pwd[0] = (pf==1) ? cnt : pwd[0];							// ﾊﾟｽﾜｰﾄﾞ有は位置保存
			pwd[1] = (pf==2) ? cnt : pwd[1];
			for(i=0; i<10; i++) {
				if(m_ViewerKind!=i) {
					filter += GetsFileFilter(2, i, &pf);				// ﾌｧｲﾙﾌｨﾙﾀｰ取得
					cnt++;
					pwd[0] = (pf==1) ? cnt : pwd[0];					// ﾊﾟｽﾜｰﾄﾞ有は位置保存
					pwd[1] = (pf==2) ? cnt : pwd[1];
				}
			}
			filter += "|";
			chk = 1;
		} else {														// <ﾋﾞｭｰﾜ･ﾃﾞｰﾀ保存なし>
			m_OpenKind = 0;
			filter = GetsFileFilter(1, m_SaveKind, &pf);				// ﾌｧｲﾙﾌｨﾙﾀｰ取得
			pwd[0] = (pf==1) ? cnt : pwd[0];							// ﾊﾟｽﾜｰﾄﾞ有は位置保存
			
#ifdef STL_EXPORT	//202006 STL export版のみ有効 FileFilter(1)
			for (i = 0; i < 2; i++) {							//202006 6->2 or 0(djfのみ）
				if (m_SaveKind != i) {
					filter += GetsFileFilter(1, i, &pf);				// ﾌｧｲﾙﾌｨﾙﾀｰ取得
					cnt++;
					pwd[0] = (pf == 1) ? cnt : pwd[0];					// ﾊﾟｽﾜｰﾄﾞ有は位置保存
				}
			}
#endif
			filter += "|";
		}
	}
	wex = fname.Right(4);
	if(chk) {
		if(!wex.CompareNoCase(".djf")||!wex.CompareNoCase(".stl"))		//202006 djf 追加
		//   !wex.CompareNoCase(".jsd")||!wex.CompareNoCase(".gld")||
        //   !wex.CompareNoCase(".jsf")||!wex.CompareNoCase(".glf")||
        //   !wex.CompareNoCase(".jgd")||!wex.CompareNoCase(".ggd")||
        //   !wex.CompareNoCase(".dxf")||
		//   !wex.CompareNoCase(".stl")) 
		{																// <有効な拡張子> 
			i = fname.ReverseFind('.');
			fname = fname.Left(i);										// 拡張子を削除
		}
	} else {
		if (!wex.CompareNoCase(".djf")||!wex.CompareNoCase(".stl"))    //202006 djf 追加
		//if(!wex.CompareNoCase(".jsd")||
        //   !wex.CompareNoCase(".jsf")||
        //   !wex.CompareNoCase(".jgd")||
        //   !wex.CompareNoCase(".dxf")||
		//   !wex.CompareNoCase(".stl")) 
		{	// <有効な拡張子>    //202006 djf 変更追加
			i = fname.ReverseFind('.');
			fname = fname.Left(i);										// 拡張子を削除
		}
	}
	CFileDlg dlg(bopen, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sFilePath;
	if(dlg.DoModal()==IDOK) {
		ext = dlg.GetFileExt();											// ﾌｧｲﾙ拡張子取得
		if(bopen) {														// <<開く>>
			m_OpenKind = GetsFileKind(0, ext);							// ﾌｧｲﾙ種類取得
		} else {														// <<保存>>
			i = (int)dlg.m_ofn.nFilterIndex;							// ﾌｧｲﾙ種類の選択位置(1～)
			if(pwd[0]==i||pwd[1]==i) {									// <JGD/GGDのﾊﾟｽﾜｰﾄﾞ有選択>
				m_PasswordSave = 1;										// ﾊﾟｽﾜｰﾄﾞ保存:有
			} else {													// <上記以外を選択>
				m_PasswordSave = 0;										// ﾊﾟｽﾜｰﾄﾞ保存:無
			}
			if(m_bViewSave) {					                		// <ﾋﾞｭｰﾜ･ﾃﾞｰﾀ保存あり>
				m_ViewerKind = GetsFileKind(2, ext) + m_PasswordSave;	// ﾌｧｲﾙ種類取得
			} else {													// <ﾋﾞｭｰﾜ･ﾃﾞｰﾀ保存なし>
				m_SaveKind = GetsFileKind(1, ext) + m_PasswordSave;		// ﾌｧｲﾙ種類取得
			}
			if (m_SaveKind < 0) {										//202006 file kind check
				m_SaveKind = 0;

			}
		}
		fname = dlg.GetPathName();										// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		i = fname.ReverseFind('\\');
		m_sFilePath = fname.Left(i+1);									// ﾊﾟｽ保存
		ret = TRUE;
	}
	return ret;
}

/*************************************************/
BOOL CMainFrame::DoOpenFile(const CString& fname)						//<<< ﾌｧｲﾙOpen
/*************************************************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙ表示
	CString  ext, str;
	OBJTYPE* op;
	int      dsp, edt, ret=1, uflg=1;

	if(GetInitFlg()) {													// <通常起動状態>
		BaseObjPtr(0, &op, &dsp, &edt);									// 先頭立体ﾎﾟｲﾝﾀ取得
		if(op!=NULL) {
			if(AfxMessageBox(IDS_ADD_OBJECT, MB_YESNO)==IDNO) {			// 追加確認ﾒｯｾｰｼﾞ
				OnFileNew();											// ｢新規作成｣
			} else {													// <追加の場合>
				if(AfxMessageBox(IDS_KEEP_UNDOREDO, MB_YESNO)==IDNO) {	// 追加立体の履歴保持確認ﾒｯｾｰｼﾞ
					uflg = 0;											// 個別Undo/Redoなし
				}
			}
		}
		StartMenu();													// ﾒﾆｭｰ開始処理
		ext = fname.Right(3);
		if((ext.CompareNoCase("jgd")&&ext.CompareNoCase("ggd")
			&& ext.CompareNoCase("djf")) ||		//202006 djf 追加		// <JGD/GGD形式以外,
           !uflg) {														// 又は個別Undo/Redoなし>
			InitLevel();												// Undo/Redo初期化
		}
		if (!ext.CompareNoCase("djf")) {
			ret = ReadJgdFile(fname, 0, uflg);		//202006 DJF 追加 		// DJF形式ﾌｧｲﾙ入力
		} 
		//if (!ext.CompareNoCase("jsd")) {
		//	ret = ReadJsdFile(fname, 0, 1);								// JSD形式ﾌｧｲﾙ入力
		//} else if(!ext.CompareNoCase("jsf")) {
		//	ret = ReadJsdFile(fname, 0, 1);								// JSF形式ﾌｧｲﾙ入力
		//} else if(!ext.CompareNoCase("gld")) {
		//	ret = ReadJsdFile(fname, 1, 1);						    	// GLD形式ﾌｧｲﾙ入力
		//} else if(!ext.CompareNoCase("glf")) {
		//	ret = ReadJsdFile(fname, 1, 1);					    		// GLF形式ﾌｧｲﾙ入力
		//} else if(!ext.CompareNoCase("jgd")) {
		//	ret = ReadJgdFile(fname, 0, uflg);							// JGD形式ﾌｧｲﾙ入力
		//} else if(!ext.CompareNoCase("ggd")) {
		//	ret = ReadJgdFile(fname, 1, uflg);					    	// GGD形式ﾌｧｲﾙ入力
		//} else if(!ext.CompareNoCase("dxf")) {
		//	ret = ReadCommonFile(0, fname);								// DXF形式ﾌｧｲﾙ入力
		//} else if(!ext.CompareNoCase("stl")) {
		//	ret = ReadCommonFile(1, fname);								// STL形式ﾌｧｲﾙ入力
		//}
		if(!ret) {														// <正常>
			if((ext.CompareNoCase("jgd")&&ext.CompareNoCase("ggd")
			   && ext.CompareNoCase("djf"))||	//202006 DJF 追加		// <JGD/GGD形式以外,
               !uflg) {													// 又は個別Undo/Redoなし>
				FileBackUp1();											// ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ処理1
			}
			if(!GetFileFlg()) {											// <ﾌｧｲﾙ指定起動以外>
				GetActiveDocument()->SetPathName(fname, TRUE);			// ﾄﾞｷｭﾒﾝﾄﾌｧｲﾙ名設定&
			}															// MRUﾌｧｲﾙﾘｽﾄ追加
			if(GetDemoFlg()!=0) {										// <体験版の場合>
				GetWindowText(ext);										// ｳｨﾝﾄﾞｳﾀｲﾄﾙ取得
				str.LoadString(IDS_STR45);								// ｢体験版｣
				ext += str;
				SetWindowText(ext);										// ｳｨﾝﾄﾞｳﾀｲﾄﾙ変更
			}
		} else {														// <異常/中止>
			if ((ext.CompareNoCase("jgd") && ext.CompareNoCase("ggd")
			   && ext.CompareNoCase("djf")) ||	//202006 DJF 追加		// <JGD/GGD形式以外,
			   !uflg) {													// 又は個別Undo/Redoなし>
				BackLevel();											// Undo/Redo戻り
			}
			if(ret==1) {
				SetSplashMsg(IDS_ERR_FILEREAD);							// ｢読込NG｣
			} else if(ret==2) {
				SetSplashMsg(IDS_ERR_PASSWORD);							// ｢ﾊﾟｽﾜｰﾄﾞ不一致｣
			} else {
				SetSplashMsg(IDS_STOP_PROCESS);							// ｢処理中止｣
			}
			return FALSE;
		}
		SetFileFlg(0);													// ﾌｧｲﾙ保存ﾌﾗｸﾞ設定
		SetGroupObj();													// ｸﾞﾙｰﾌﾟ立体設定
		View->Display(0);												// 再描画
	} else {															// <ﾌｧｲﾙ起動状態>
		SetFileFlg(1);													// ﾌｧｲﾙ保存ﾌﾗｸﾞ設定
		SetFileName(fname);												// 保存ﾌｧｲﾙ設定
	}
	return TRUE;
}

/***************************************/
BOOL CMainFrame::InitStatusBar(int sec)									//<<< ｽﾃｰﾀｽﾊﾞｰ作成&初期化
/***************************************/
{
	UINT style;

	if(!m_wndStatusBar.Create(this)||									// ｽﾃｰﾀｽﾊﾞｰ作成
       !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))) {
		TRACE0("Failed to create status bar\n");
		return FALSE;													// 作成失敗
	}
	style = m_wndStatusBar.GetPaneStyle(0)&~SBPS_NOBORDERS;
	m_wndStatusBar.SetPaneStyle(0, style);
	setlocale(LC_ALL, "");
	m_wndStatusBar.SetTimer(0x1000, sec*1000, NULL);					// ｱｲﾄﾞﾙ中時刻ﾍﾟｲﾝ更新時間設定
	return TRUE;
}

/******************************************************************/
BOOL CMainFrame::InitToolBar(CToolBar* pBar, UINT nIDR,					//<<< ﾂｰﾙﾊﾞｰ作成&初期化
                             UINT titleID, DWORD style, UINT nIDW)
/******************************************************************/
{
	CString str("");

	str.LoadString(titleID);
	style |= WS_CHILD|WS_VISIBLE|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC|TBSTYLE_WRAPABLE;
	if(!pBar->Create(this, style, nIDW)||!pBar->LoadToolBar(nIDR)) {	// ﾂｰﾙﾊﾞｰ作成
		TRACE1("Failed to create %s toolbar\n", str);
		return FALSE;													// 作成失敗
	}
	if(titleID!=0) {
		pBar->SetWindowText(str);										// ｳｨﾝﾄﾞｳﾀｲﾄﾙ設定
	}
	pBar->EnableDocking(CBRS_ALIGN_ANY);								// ﾂｰﾙﾊﾞｰﾄﾞｯｷﾝｸﾞ可
	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}

/*********************************************************************/
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)	//<<< ﾂｰﾙﾊﾞｰ配置
/*********************************************************************/
{
	CRect rect;
	DWORD dw;
	UINT  n;

	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,2);
	dw = LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (n==0&&dw&CBRS_ALIGN_BOTTOM) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (n==0&&dw&CBRS_ALIGN_LEFT) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (n==0&&dw&CBRS_ALIGN_RIGHT) ? AFX_IDW_DOCKBAR_RIGHT : n;
	DockControlBar(Bar, n, &rect);
}

/*********************************/
int CMainFrame::GetGroupKey(void)										//<<< ｸﾞﾙｰﾌﾟ対応ｷｰ取得
/*********************************/
{
	int kno=0;

	if(View->GetChar()=='1') {											// [1]ｷｰ押下はｸﾞﾙｰﾌﾟ1
		kno = 1;
	} else if(View->GetChar()=='2') {									// [2]ｷｰ押下はｸﾞﾙｰﾌﾟ2
		kno = 2;
	} else if(View->GetChar()=='3') {									// [3]ｷｰ押下はｸﾞﾙｰﾌﾟ3
		kno = 3;
	} else if(View->GetChar()=='4') {									// [4]ｷｰ押下はｸﾞﾙｰﾌﾟ4
		kno = 4;
	} else if(View->GetChar()=='5') {									// [5]ｷｰ押下はｸﾞﾙｰﾌﾟ5
		kno = 5;
	} else if(View->GetChar()=='6') {									// [6]ｷｰ押下はｸﾞﾙｰﾌﾟ6
		kno = 6;
	} else if(View->GetChar()=='7') {									// [7]ｷｰ押下はｸﾞﾙｰﾌﾟ7
		kno = 7;
	} else if(View->GetChar()=='8') {									// [8]ｷｰ押下はｸﾞﾙｰﾌﾟ8
		kno = 8;
	} else if(View->GetChar()=='9') {									// [9]ｷｰ押下はｸﾞﾙｰﾌﾟ9
		kno = 9;
	} else if(View->GetChar()=='0') {									// [0]ｷｰ押下はｸﾞﾙｰﾌﾟ10
		kno = 10;
	} else if(View->GetChar()=='Q') {									// [Q]ｷｰ押下はｸﾞﾙｰﾌﾟ11
		kno = 11;
	} else if(View->GetChar()=='W') {									// [W]ｷｰ押下はｸﾞﾙｰﾌﾟ12
		kno = 12;
	} else if(View->GetChar()=='E') {									// [E]ｷｰ押下はｸﾞﾙｰﾌﾟ13
		kno = 13;
	} else if(View->GetChar()=='R') {									// [R]ｷｰ押下はｸﾞﾙｰﾌﾟ14
		kno = 14;
	} else if(View->GetChar()=='T') {									// [T]ｷｰ押下はｸﾞﾙｰﾌﾟ15
		kno = 15;
	} else if(View->GetChar()=='Y') {									// [Y]ｷｰ押下はｸﾞﾙｰﾌﾟ16
		kno = 16;
	} else if(View->GetChar()=='U') {									// [U]ｷｰ押下はｸﾞﾙｰﾌﾟ17
		kno = 17;
	} else if(View->GetChar()=='I') {									// [I]ｷｰ押下はｸﾞﾙｰﾌﾟ18
		kno = 18;
	} else if(View->GetChar()=='O') {									// [O]ｷｰ押下はｸﾞﾙｰﾌﾟ19
		kno = 19;
	} else if(View->GetChar()=='P') {									// [P]ｷｰ押下はｸﾞﾙｰﾌﾟ20
		kno = 20;
	} else if(View->GetChar()=='A') {									// [A]ｷｰ押下はｸﾞﾙｰﾌﾟ21
		kno = 21;
	} else if(View->GetChar()=='S') {									// [S]ｷｰ押下はｸﾞﾙｰﾌﾟ22
		kno = 22;
	} else if(View->GetChar()=='D') {									// [D]ｷｰ押下はｸﾞﾙｰﾌﾟ23
		kno = 23;
	} else if(View->GetChar()=='F') {									// [F]ｷｰ押下はｸﾞﾙｰﾌﾟ24
		kno = 24;
	} else if(View->GetChar()=='G') {									// [G]ｷｰ押下はｸﾞﾙｰﾌﾟ25
		kno = 25;
	} else if(View->GetChar()=='H') {									// [H]ｷｰ押下はｸﾞﾙｰﾌﾟ26
		kno = 26;
	} else if(View->GetChar()=='J') {									// [J]ｷｰ押下はｸﾞﾙｰﾌﾟ27
		kno = 27;
	} else if(View->GetChar()=='K') {									// [K]ｷｰ押下はｸﾞﾙｰﾌﾟ28
		kno = 28;
	} else if(View->GetChar()=='L') {									// [L]ｷｰ押下はｸﾞﾙｰﾌﾟ29
		kno = 29;
	} else if(View->GetChar()=='Z') {									// [Z]ｷｰ押下はｸﾞﾙｰﾌﾟ30
		kno = 30;
	} else if(View->GetChar()=='X') {									// [X]ｷｰ押下はｸﾞﾙｰﾌﾟ31
		kno = 31;
	} else if(View->GetChar()=='C') {									// [C]ｷｰ押下はｸﾞﾙｰﾌﾟ32
		kno = 32;
	}
	return kno;
}
