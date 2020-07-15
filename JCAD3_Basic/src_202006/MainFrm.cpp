/////////////////////////////////////////////////////////////////////////////
// ���C���t���[���E�B���h�E�N���X(CMainFrame)��`
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
void ReadWindowPlacement(WINDOWPLACEMENT* wp)							//<<< ����޳�\����ԓǍ�
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

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)								// ���b�Z�[�W�E�}�b�v
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
	// �O���[�o�� �w���v �R�}���h
//	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,														// �ð��ײݲݼ޹��
	ID_INDICATOR_CURPOS,												// ���وʒu�ݼ޹��
	ID_INDICATOR_GROUP,													// ��ٰ�߲ݼ޹��
	ID_INDICATOR_CRTPRM,												// �������Ұ��ݼ޹��
	ID_INDICATOR_GRID,													// ��د�޲ݼ޹��
	ID_INDICATOR_FUNC,													// ̧ݸ��ݲݼ޹��
//	ID_INDICATOR_CAPS,													// Caps Lock�ݼ޹��
//	ID_INDICATOR_NUM,													// Num Lock�ݼ޹��
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

/****************************/
CMainFrame::CMainFrame(void)											//<<< �׽�̍\�z
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
	m_XPos = 0.0, m_YPos = 0.0, m_ZPos = 0.0;							// ���وʒu���_
	m_CrtD = 0, m_CrtW = 0.0f;											// �������Ұ�
	for(i=0; i<64; i++) {
		m_Color[i] = RGB(255, 255, 255);								// �װ������(�S�Ĕ�)
	}
	ReadFilPathFile();													// ̧���߽�ۑ�̧�ٓǍ�
	InitSColorFile();													// �������Ѷװ̧�ٓǍ�
	InitColor();														// �\���F�ݒ�
	SetInitFlg(1);														// �����N������׸�ON
}

/*****************************/
CMainFrame::~CMainFrame(void)											//<<< �׽�̏���
/*****************************/
{
	MainWnd = NULL;
	FreeMem();															// ��؉��(���яI����)
}

/*******************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)					//<<< WM_CREATE ү���މ�������
/*******************************************************/
{
	WINDOWPLACEMENT wp;
	CRect   rect;
	CString str;

	CSplashWnd::ShowSplashScreen(this);									// �N����ʕ\��
	if(CFrameWnd::OnCreate(lpCreateStruct)==-1) {
		return -1;														// Ҳݥ����޳�쐬
	}
	if(!InitStatusBar(1)) {
		return -1;														// �ð���ް�쐬&������
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
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP);					// °��ް �ޯ�ݸ�
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
	ReadWindowPlacement(&wp);											// ����޳�\����ԓǍ�
	if(wp.showCmd==SW_SHOWMAXIMIZED) {									// �N�����ɳ���޳��
		wp.showCmd = SW_SHOWNORMAL;										// �ő�\���͏o���Ȃ�����
		GetDesktopWindow()->GetWindowRect(&rect);						// �޽�į�߳���޳ ����
		wp.rcNormalPosition.left = rect.left;
		wp.rcNormalPosition.top = rect.top;
		wp.rcNormalPosition.right = rect.right;
		wp.rcNormalPosition.bottom = rect.bottom;
	} else if(wp.showCmd==SW_SHOWMINIMIZED) {							// �N�����ɳ���޳��
		wp.showCmd = SW_SHOWNORMAL;										// ���݉��͏o���Ȃ����ߌ��̻��ނƈʒu�ɖ߂�
	}
	SetWindowPlacement(&wp);											// ����޳�\����Ԑݒ�
//202006 ���\�[�X���ύX�����LoadBasState �G���[�ɂȂ�B Save�㐳��
//regedit HKEY_USERS\S-xxx\Software\GLB\JCAD3 Basic for Windows �ȉ�����U����
	LoadBarState("Control");											// ���۰��ް�\����Ԑݒ�
	Initialize();														// ������
	if(GetFileFlg()) {													// <̧�َw��N��>
		CString fname=GetFileName();									// �ۑ�̧�ٖ��擾
		DoOpenFile(fname);												// �w��̧�ٓǍ���
	}
	return 0;
}

/**************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)						//<<< ����޳�쐬�O����
/**************************************************/
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C�����āAWindow �׽����ق��C�����Ă��������B
	cs.lpszClass = AfxRegisterWndClass(0,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �f�f

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
// CMainFrame ���b�Z�[�W �n���h��

/********************************************************************/
LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)	//<<< WM_SETMESSAGESTRING ү���މ�������
/********************************************************************/
{
	if(wParam>0&&wParam!=AFX_IDS_IDLEMESSAGE) {							// <�L�����ƭ�ID�̏ꍇ>
		View->MessageOut((int)wParam);									// ̧ݸ���ү���ޏo��
	}
	return CFrameWnd::OnSetMessageString(wParam, lParam);
}

/******************************/
void CMainFrame::OnClose(void)											//<<< WM_CLOSE ү���މ�������
/******************************/
{
	WINDOWPLACEMENT wp;
	OBJTYPE* op;
	int      dsp, edt, chk=0;

	BaseObjPtr(0, &op, &dsp, &edt);										// �擪���̂��߲���擾
	while(op) {
		if(dsp==0) {													// ��\���L�̏ꍇ
			chk = 1; break;
		}
		NextObjPtr(0, &op, &dsp, &edt);									// �����̂��߲���擾
	}
	if(chk) {															// <��\�����̗L>
		if(AfxMessageBox(IDS_NODISP_OBJ, MB_YESNO)==IDNO) {				// ��I����𒆎~
			return;
		}
	}
	GetActiveDocument()->SetModifiedFlag(CheckCmdFlg());
	SaveBarState("Control");											// ���۰��ް�\����ԕۑ�
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Length", wp.length);				// ����޳�\����ԕۑ�
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
	WriteFilPathFile();													// ̧���߽�ۑ�̧�ُ���
	WriteSColorFile();													// ���Ѷװ�ۑ�̧�ُ���
	CFrameWnd::OnClose();												// Ҳݥ����޳�̏I��
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �R�}���h �n���h��

/********************************************/
BOOL CMainFrame::OnViewMenuCtrlBar(UINT nID)							//<<< ���۰��ް�\��/��\��
/********************************************/
{
	CControlBar* pBar = GetControlBar(nID);								// �����ID������۰��ް�擾
	if(pBar!=NULL) {													// ���۰��ް�\��/��\���ؑ�
		ShowControlBar(pBar, (pBar->GetStyle()&WS_VISIBLE)==0, FALSE);
		return TRUE;
	}
	return FALSE;
}

/****************************************************/
void CMainFrame::OnUpdateMenuCtrlBar(CCmdUI* pCmdUI)					//<<< ���۰��ް�X�V
/****************************************************/
{
	CControlBar* pBar = GetControlBar(pCmdUI->m_nID);					// �����ID������۰��ް�擾
	if(pBar!=NULL) {
		pCmdUI->SetCheck((pBar->GetStyle()&WS_VISIBLE)!=0);				// �ƭ��ް��������Ԑؑ�
		return;
	}
	pCmdUI->ContinueRouting();											// ����ނ�ٰèݸ�
}

/***********************************************/
void CMainFrame::OnUpdateCurPos(CCmdUI* pCmdUI)							//<<< �ð���ް���وʒu�߲ݍX�V
/***********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strCurP;
	CSize   size;
	UINT    nID, nStyle;
	int     idx, nWidth;

	strCurP.Format(ID_INDICATOR_CURPOS, m_XPos, m_YPos, m_ZPos);		// ���وʒu
	HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
	CClientDC dc(NULL);
	if(hFont!=NULL) {
		hOldFont = dc.SelectObject(hFont);
	}
	size = dc.GetTextExtent(strCurP);									// ���ݕ����񕝌v�Z
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURPOS);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strCurP);											// ���ݶ��وʒu�߲ݍX�V
	pCmdUI->Enable(TRUE);
}

/**********************************************/
void CMainFrame::OnUpdateGroup(CCmdUI* pCmdUI)							//<<< �ð���ް��ٰ���߲ݍX�V
/**********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strGroup, strMax, str;
	CSize   size;
	UINT    nID, nStyle;
	int     i, idx, nWidth;

	strGroup = (m_GroupObj[0]) ? ((m_GroupDisp[0]) ? "1*" : "1 ")		// ��ٰ�ߓo�^/���o�^��
                               : "  ";									// �\��/��\���ݒ�
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
	size = dc.GetTextExtent(strMax);									// �ő啶���񕝌v�Z
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_GROUP);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strGroup);											// ���ݸ�ٰ���߲ݍX�V
	pCmdUI->Enable(TRUE);
}

/***********************************************/
void CMainFrame::OnUpdateCrtPrm(CCmdUI* pCmdUI)							//<<< �ð���ް�������Ұ��߲ݍX�V
/***********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strCrtPrm;
	CSize   size;
	UINT    nID, nStyle;
	int     idx, nWidth;

	strCrtPrm.Format(ID_INDICATOR_CRTPRM, m_CrtD, m_CrtW);				// �������Ұ�
	HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
	CClientDC dc(NULL);
	if(hFont!=NULL) {
		hOldFont = dc.SelectObject(hFont);
	}
	size = dc.GetTextExtent(strCrtPrm);									// ���ݕ����񕝌v�Z
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CRTPRM);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strCrtPrm);											// ���ݐ������Ұ��߲ݍX�V
	pCmdUI->Enable(TRUE);
}

/*********************************************/
void CMainFrame::OnUpdateGrid(CCmdUI* pCmdUI)							//<<< �ð���ް��د���߲ݍX�V
/*********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strGrid, strMax;
	CSize   size;
	UINT    nID, nStyle;
	int     idx, nWidth;

	strGrid = (View->GetGridDisp()) ? "G*" : "G ";						// ��د��/��_/�����
	strGrid += (View->GetGridBind()) ? "*" : " ";						// �\��&�S���ݒ�
	strMax.Format("%.2f ", View->GetGridSpace()/1000.0);				// ��د�ފԊu
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
	size = dc.GetTextExtent(strMax);									// �ő啶���񕝌v�Z
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_GRID);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strGrid);											// ���ݸ�د���߲ݍX�V
	pCmdUI->Enable(TRUE);
}

/*********************************************/
void CMainFrame::OnUpdateFunc(CCmdUI* pCmdUI)							//<<< �ð���ް̧ݸ����߲ݍX�V
/*********************************************/
{
	HGDIOBJ hOldFont = NULL;
	CString strFunc, strMax, str;
	CSize   size;
	UINT    nID, nStyle;
	int     i, func[8], idx, nWidth;

	View->GetFChar(func);												// ̧ݸ��ݷ��ݒ�
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
	size = dc.GetTextExtent(strMax);									// �ő啶���񕝌v�Z
	if(hOldFont!=NULL) {
		dc.SelectObject(hOldFont);
	}
	idx = m_wndStatusBar.CommandToIndex(ID_INDICATOR_FUNC);
	m_wndStatusBar.GetPaneInfo(idx, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(idx, nID, nStyle, size.cx);
	pCmdUI->SetText(strFunc);											// ����̧ݸ��ݷ��߲ݍX�V
	pCmdUI->Enable(TRUE);
}

/***************************************/
BOOL CMainFrame::OnCommonMenu(UINT nID)									//<<< ���ʕW���ƭ��N��
/***************************************/
{
	int menu = View->GetActMenu();										// �N���ƭ��擾
	int smnu = View->GetSaveMenu();										// �ۑ��ƭ��擾

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(nID);												// �O�ƭ��I��
	if(menu!=(int)nID&&smnu!=(int)nID) {
		View->SetActMenu(nID);											// �ƭ��ݒ�
		View->InitActMenu(0);											// �ƭ���������
		View->CountEdtObj();											// �ҏW�Ώۗ��̶���
	}
	return TRUE;
}

/**************************************/
BOOL CMainFrame::OnGroupMenu(UINT nID)									//<<< ���ٰ�ߐݒ�/������t����
/**************************************/
{
	OBJTYPE* op;
	int      i, kno, mode, dsp, edt, ano, func[8];

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(View->GetActMenu());								// �O�ƭ��I��
	mode = (nID==ID_GROUP_SET) ? 1 : 0;									// Ӱ��(1:�ݒ�,0:����)
	kno = GetGroupKey();												// ��ٰ�ߑΉ����擾
	if(kno>0) {															// <�p����������>
		BaseObjPtr(2, &op, &dsp, &edt);									// �I�𗧑̎擾
		while(op) {
			if(dsp==1) {
				SetGroupNo(op, kno-1, mode);							// ��ٰ�ߐݒ�/����
			}
			NextObjPtr(2, &op, &dsp, &edt);
		}
	} else {															// <̧ݸ��ݷ�����>
		View->GetFChar(func);											// ̧ݸ��ݷ��擾
		ano = (View->GetCChar()&0x2) ? 8 : 0;							// Ctrl��������+8
		for(i=0; i<8; i++) {											// ��ٰ��1�`8/9�`16
			if(func[i]) {												// �Y��������
				BaseObjPtr(2, &op, &dsp, &edt);							// �I�𗧑̎擾
				while(op) {
					if(dsp==1) {
						SetGroupNo(op, i+ano, mode);					// ��ٰ�ߐݒ�/����
					}
					NextObjPtr(2, &op, &dsp, &edt);
				}
			}
		}
	}
	SetGroupObj();														// ��ٰ�ߗ��̐ݒ�
	View->InitChar();													// �����͏�����
	return TRUE;
}

/****************************************/
BOOL CMainFrame::OnScrCtrlMenu(UINT nID)								//<<< ���ʉ�ʐ����ƭ��N��
/****************************************/
{
	int menu = View->GetActMenu();										// �N���ƭ��擾
	int smnu = View->GetSaveMenu();										// �ۑ��ƭ��擾

	View->InitializeMenu();												// �ƭ���������
	if(menu!=(int)nID&&smnu!=(int)nID) {
		if(smnu==NONE) {
			View->SetSaveMenu(menu);									// �ۑ����͋N�����ƭ��ۑ�
		}
		View->SetActMenu(nID);											// �ƭ��ݒ�
		View->InitActMenu(0);                                           // �ƭ���������
	} else {
		View->ExitActMenu(nID);											// ���ƭ��I��
	}
	return TRUE;
}

/****************************************/
BOOL CMainFrame::OnMeasureMenu(UINT nID)								//<<< ���K/���x��t����
/****************************************/
{
	BOOL check = View->CheckMeasure(nID);								// �\���m�F
	int  menu = View->GetActMenu();										// �N���ƭ��擾
	int  smnu = View->GetSaveMenu();									// �ۑ��ƭ��擾

	View->InitializeMenu();												// �ƭ���������
	if(check&&menu!=(int)nID&&smnu!=(int)nID) {							// �t�������ň��ƭ��N��
		if(smnu==NONE) {
			View->SetSaveMenu(menu);									// �ۑ����͋N�����ƭ��ۑ�
		}
		View->SetActMenu(nID);
		View->ExitActMenu(nID);											// ��K�ƭ��I��
	} else {															// ��L�ȊO
		View->ExitActMenu(nID);											// �O�ƭ��I��
	}
	if(!check) {
		View->SetActMenu(nID);											// �ƭ��ݒ�
		View->InitActMenu(0);											// �ƭ���������
	}
	return TRUE;
}

/**********************************************************/
void CMainFrame::OnUpdateMenuEnableOrCheck(CCmdUI* pCmdUI)				//<<< �ƭ��t�L����/������&
/**********************************************************/			//    �N���ƭ��t�X�V
{
	UINT id=pCmdUI->m_nID;												// �����ID�擾

	pCmdUI->Enable(View->GetEdtObjNum()>0?TRUE:FALSE);					// �ҏW�Ώۂ̗L���ɂčX�V
	BOOL chk = View->GetActMenu()==(int)id||
               View->GetSaveMenu()==(int)id;
	pCmdUI->SetCheck(chk);												// �N���ƭ��̂�����
}

/***********************************************************/
void CMainFrame::OnUpdateMenuEnableOrCheck2(CCmdUI* pCmdUI)				//<<< �ƭ��t�L����/������&
/***********************************************************/			//    �N���ƭ��t�X�V2
{
	UINT id=pCmdUI->m_nID;												// �����ID�擾

	pCmdUI->Enable(View->GetDspObjNum()>0?TRUE:FALSE);					// �\���Ώۂ̗L���ɂčX�V
	BOOL chk = View->GetActMenu()==(int)id||
               View->GetSaveMenu()==(int)id;
	pCmdUI->SetCheck(chk);												// �N���ƭ��̂�����
}

/***************************************************/
void CMainFrame::OnUpdateMenuEnable(CCmdUI* pCmdUI)						//<<< �ƭ��t�L����/������
/***************************************************/
{
	pCmdUI->Enable(View->GetEdtObjNum()>0?TRUE:FALSE);					// �ҏW�Ώۂ̗L���ɂčX�V
}

/**************************************************/
void CMainFrame::OnUpdateMenuCheck(CCmdUI* pCmdUI)						//<<< �N���ƭ��t�X�V
/**************************************************/
{
	UINT id=pCmdUI->m_nID;												// �����ID�擾

	BOOL chk = View->GetActMenu()==(int)id||
               View->GetSaveMenu()==(int)id;
	pCmdUI->SetCheck(chk);												// �N���ƭ��̂�����
}

/*************************************************************/
void CMainFrame::OnUpdateMeasureEnableOrCheck(CCmdUI* pCmdUI)			//<<< �v���ƭ��t�L����/������&
/*************************************************************/			//    �N���v���ƭ��t�X�V
{
	UINT id = pCmdUI->m_nID;											// �����ID�擾
	int  am, sm, no;

	pCmdUI->Enable(View->GetEdtObjNum()>0?TRUE:FALSE);					// �ҏW�Ώۂ̗L���ɂčX�V
	am = View->GetActMenu();											// �N���ƭ��擾
	sm = View->GetSaveMenu();											// �ۑ��ƭ��擾
	if((am==(int)id||sm==(int)id)&&(!View->IsNumDispShow())) {			// �N�������޲�۸ޖ��̏ꍇ
		if(sm==(int)id) {
			no = View->GetCursorNo();
			View->SetActMenu(id);
		}
		View->ExitActMenu(am);											// �Y���ƭ��I��
		if(sm==(int)id) {
			View->SetActMenu(am);
			View->SetCursorNo(no);
		}
		am = 0, sm = 0;
	}
	pCmdUI->SetCheck(am==(int)id||sm==(int)id);							// �N���ƭ��̂�����
}

/*****************************************************/
void CMainFrame::OnUpdateMeasureCheck(CCmdUI* pCmdUI)					//<<< �N���v���ƭ��t�X�V
/*****************************************************/
{
	UINT id = pCmdUI->m_nID;											// �����ID�擾
	int  am, sm, no;

	am = View->GetActMenu();											// �N���ƭ��擾
	sm = View->GetSaveMenu();											// �ۑ��ƭ��擾
	if((am==(int)id||sm==(int)id)&&(!View->IsNumDispShow())) {			// �N�������޲�۸ޖ��̏ꍇ
		if(sm==(int)id) {
			no = View->GetCursorNo();
			View->SetActMenu(id);
		}
		View->ExitActMenu(am);											// �Y���ƭ��I��
		if(sm==(int)id) {
			View->SetActMenu(am);
			View->SetCursorNo(no);
		}
		am = 0, sm = 0;
	}
	pCmdUI->SetCheck(am==(int)id||sm==(int)id);							// �N���ƭ��̂�����
}

/**********************************************************/
void CMainFrame::OnUpdateMeasurePlaneCheck(CCmdUI* pCmdUI)				//<<< ���K/���x��t�X�V
/**********************************************************/
{
	UINT id=pCmdUI->m_nID;												// �����ID�擾
	int  mn=View->GetActMenu();											// �N���ƭ��擾
	pCmdUI->SetCheck(View->CheckMeasure(id)||mn==(int)id);				// �N���ƭ��̂�����
}

/********************************/
void CMainFrame::OnFileNew(void)										//<<< ��V�K�쐬��t����
/********************************/
{
	CDocument* pDoc;
	CString    strDoc, title, str;
	OBJTYPE*   op;
	int        dsp, edt, chk=0;

	View->InitializeMenu();												// �ƭ���������
	BaseObjPtr(0, &op, &dsp, &edt);										// �擪���̂��߲���擾
	while(op) {
		if(dsp==0) {													// ��\���L�̏ꍇ
			chk = 1; break;
		}
		NextObjPtr(0, &op, &dsp, &edt);									// �����̂��߲���擾
	}
	if(chk) {															// <��\�����̗L>
		if(AfxMessageBox(IDS_NODISP_OBJ, MB_YESNO)==IDNO) {				// ��V�K�쐬��𒆎~
			return;
		}
	}
	pDoc = GetActiveDocument();											// �޷���Ď擾
	pDoc->SetModifiedFlag(CheckCmdFlg());								// ����ޖ��ۑ��׸޾��
	if(!pDoc->SaveModified()) {											// �ύX��ۑ����邩�⍇
		return;
	}
	strDoc.LoadString(AFX_IDS_UNTITLED);								// �Untitled�
	pDoc->SetTitle(strDoc);												// �޷�������ِݒ�
	pDoc->OnNewDocument();												// �V�K�쐬
	if(GetDemoFlg()!=0) {												// <�̌��ł̏ꍇ>
		GetWindowText(title);											// ����޳���َ擾
		str.LoadString(IDS_STR45);										// ��̌��ţ
		title += str;
		SetWindowText(title);											// ����޳���ٕύX
	}
	ResetVar();															// �ϐ�������(���щғ���)
	StartBackUp(0);														// �ޯ����ߊJ�n����
	Initialize();														// ������
	View->Init();														// View������
	View->Display(0);													// �ĕ`��
}

/*********************************/
void CMainFrame::OnFileOpen(void)										//<<< ��J����t����
/*********************************/
{
	CString fname, ext;

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_FILE_OPEN);									// �O�ƭ��I��
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_FILE_OPEN);			// �t�������
	if(DoPromptFName(fname, ext, TRUE)!=FALSE) {
		View->Display(0);												// �ĕ`��
		DoOpenFile(fname);												// ̧�ق��J��
	}
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_FILE_OPEN, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/***********************************/
void CMainFrame::OnViewerSave(void)										//<<< ��ޭ�ܥ�ް��ۑ���t����
/***********************************/
{
	CDocument* pDoc;
	CString    file;
	int        i;

	View->InitializeMenu();												// �ƭ���������
	if(GetDemoFlg()!=0) {												// ��̌���,�N��NG�
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	pDoc = GetActiveDocument();											// �޷���Ă��擾
	m_bViewSave = TRUE;													// �ޭ�ܕۑ��׸�ON
	if(pDoc->DoSave(NULL, TRUE)) {										// ���O��t���ĕۑ����s
		View->SetFChar(1, 0x1);											// ̧ݸ��ݷ�[F5]�ݒ�
		View->ExitActMenu(ID_RENDERING);								// �O�ƭ��I��
		View->SetActMenu(ID_RENDERING);									// �ƭ��ݒ�
		View->InitActMenu(0);											// �ƭ���������
		file = pDoc->GetTitle();										// ̧�ٖ�
		i = file.ReverseFind('.');
		file = (i!=-1) ? file.Left(i) : file;
		file = m_sFilePath + file;										// ̧�ٖ�(���߽)
		View->OutputCapData(file);										// �������ް��o��
		View->ExitActMenu(ID_RENDERING);								// �O�ƭ��I��
	}
	m_bViewSave = FALSE;												// �ޭ�ܕۑ��׸�OFF
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnExportSLC(void)										//<<< �����߰�SLC��t����
/**********************************/
{
	OBJTYPE* op;
	CString  ext="slc", file=_T(""), filter;
	int      i, i1, i2;
	DWORD    flg;

	View->InitializeMenu();												// �ƭ���������
	if(GetDemoFlg()!=0) {												// ��̌���,�N��NG�
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	BaseObjPtr(1, &op, &i1, &i2);										// �\�������߲���擾
	if(op==NULL) {														// <���̖��͒��~>
		return;
	}
	View->ExitActMenu(ID_EXPORT_SLC);									// �O�ƭ��I��
	View->InitChar();													// �����͏�����
	if(!CheckFilePath(0, m_sFilePath)) {								// �߽�m�F
		m_sFilePath = GetAppPath(0);
	}
	flg = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;							// �SLÇ�ق�ۑ��
	filter.LoadString(IDS_FILE_FILTER104);
	CFileDlg dlg(FALSE, ext, file, flg, filter);						// �޲�۸ޕ\��
	dlg.m_ofn.lpstrInitialDir = m_sFilePath;
	if(dlg.DoModal()==IDOK) {
		file = dlg.GetPathName();										// ̧�ٖ�(���߽)�擾
		i = file.ReverseFind('\\');
		m_sFilePath = file.Left(i+1);									// �߽�ۑ�
	} else {
		return;
	}
	ExportSLCFile(file);												// ����߰�SLC
}

/***********************************/
void CMainFrame::OnCaptureSet(void)										//<<< ���������̐ݒ裖t����
/***********************************/
{
	int p1, p2, p3, p4, p5, p6;

	View->InitializeMenu();												// �ƭ���������
	CCaptureDlg dlg;
	p1 = View->GetCapPlane();											// �������捞��ʎ擾
	p2 = View->GetCapPos();												// �������捞�ʒu�擾
	p3 = View->GetCapKind();											// �������摜��ގ擾
	p4 = View->GetCapWidth();											// ��������ʕ��擾
	p5 = View->GetCapHeight();											// ��������ʍ����擾
	p6 = View->GetCapJComp();											// ������JPEG���k���擾
	dlg.SetValue(p1, p2, p3, p4, p5, p6);								// ���݂̐ݒ�l��\��
	if(dlg.DoModal()==IDOK) {											// ��������ݒ��޲�۸ޕ\��
		dlg.GetValue(&p1, &p2, &p3, &p4, &p5, &p6);
		View->SetCapPlane(p1);											// �������捞��ʐݒ�
		View->SetCapPos(p2);											// �������捞�ʒu�ݒ�
		View->SetCapKind(p3);											// �������摜��ސݒ�
		View->SetCapWidth(p4);											// ��������ʕ��ݒ�
		View->SetCapHeight(p5);											// ��������ʍ����ݒ�
		View->SetCapJComp(p6);											// ������JPEG���k���ݒ�
	}
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnPasswordSet(void)									//<<< ��߽ܰ�ނ̐ݒ裖t����
/************************************/
{
	CString str1, str2;
	int     len;

	View->InitializeMenu();												// �ƭ���������
	str1.Format("%s", m_cWPassword);
	str2.Format("%s", m_cRPassword);
	CPasswdDlg dlg;
	dlg.SetValue(str1, str2, m_PasswordKind);							// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �߽ܰ�ސݒ��޲�۸ޕ\��
		dlg.GetValue(&str1, &str2, &m_PasswordKind);
		len = str1.GetLength();
		memset(m_cWPassword, NULL, sizeof(m_cWPassword));
		memcpy(m_cWPassword, (const char*)str1, len);
		len = str2.GetLength();
		memset(m_cRPassword, NULL, sizeof(m_cRPassword));
		memcpy(m_cRPassword, (const char*)str2, len);
	}
	View->InitChar();													// �����͏�����
}

/***********************************/
void CMainFrame::OnRestBackUp(void)										//<<< ��ޯ����ߕ�����t����
/***********************************/
{
	CDocument* pDoc;
	CString    strDoc;
	OBJTYPE*   op;
	int        dsp, edt, chk=0;

	View->InitializeMenu();												// �ƭ���������
	if(GetDemoFlg()!=0) {												// ��̌���,�N��NG�
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	BaseObjPtr(0, &op, &dsp, &edt);										// �擪���̂��߲���擾
	while(op) {
		if(dsp==0) {													// ��\���L�̏ꍇ
			chk = 1; break;
		}
		NextObjPtr(0, &op, &dsp, &edt);									// �����̂��߲���擾
	}
	if(chk) {															// <��\�����̗L>
		if(AfxMessageBox(IDS_NODISP_OBJ, MB_YESNO)==IDNO) {				// ��V�K�쐬��𒆎~
			return;
		}
	}
	pDoc = GetActiveDocument();											// �޷���Ă��擾
	pDoc->SetModifiedFlag(CheckCmdFlg());								// ����ޖ��ۑ��׸޾��
	if(!pDoc->SaveModified()) {											// �ύX��ۑ����邩�⍇
		return;
	}
	strDoc.LoadString(AFX_IDS_UNTITLED);								// �Untitled�
	pDoc->SetTitle(strDoc);												// �޷���Ă����ِݒ�
	pDoc->OnNewDocument();												// �V�K�쐬
	ResetVar();															// �ϐ�������(���щғ���)
	StartBackUp(0);														// �ޯ����ߊJ�n����
	Initialize();														// ������
	View->Init();														// View������
	View->Display(0);													// �ĕ`��
	StartMenu();														// �ƭ��J�n����
	InitLevel();														// Undo/Redo������
	if(!RestoreBackUp()) {												// �ޯ�����̧�ٕ���
		BackLevel();													// Undo/Redo�߂�
		AfxMessageBox(IDS_BACKUP_ERR);									// �װү����
	} else {
		FileBackUp1();													// ̧���ޯ����ߏ���1
		View->Display(0);
	}
	SetGroupObj();														// ��ٰ�ߗ��̐ݒ�
}

/*****************************/
void CMainFrame::OnUndo(void)											//<<< �Undo��t����
/*****************************/
{
	BOOL chk = TRUE;
	int  i, gno[32], menu;

	View->InitializeMenu();												// �ƭ���������
	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	if(CheckUndo()&&chk) {
		Undo(gno);														// Undo
		for(i=0; i<32; i++) {
			if(gno[i]) {												// Undo�Ώ۸�ٰ�߂�
				OnDspGroupObject(i);									// ��ٰ�ߗ��̕\��
				m_GroupDisp[i] = 1;										// ��ٰ�ߕ\���׸�ON
			}
		}
		if(View->CountEdtObj()<1) {
			menu = View->GetActMenu();									// �N���ƭ��擾
			if(menu>=M_ALLSHADING&&menu!=M_EDIT_PASTEOBJ) {
				View->ExitActMenu(menu);								// �O�ƭ��I��
			}
		} else { 
			View->InitActMenu(1);										// �ƭ���������
		}
		ResetSelNo();													// ���̑I��ؾ��
	}
	SetGroupObj();														// ��ٰ�ߗ��̐ݒ�
	View->InitChar();													// �����͏�����
	View->Display(0);													// �S���̍ĕ\��
}

/*********************************************/
void CMainFrame::OnUpdateUndo(CCmdUI* pCmdUI)							//<<< �Undo��t�X�V
/*********************************************/
{
	BOOL chk = TRUE;

	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	pCmdUI->Enable(CheckUndo()&&chk ? TRUE : FALSE);					// Undo�����ɂčX�V
}

/*****************************/
void CMainFrame::OnRedo(void)											//<<< �Redo��t����
/*****************************/
{
	BOOL chk = TRUE;
	int  i, gno[32], menu;

	View->InitializeMenu();												// �ƭ���������
	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	if(CheckRedo()&&chk) {
		Redo(gno);														// Redo
		for(i=0; i<32; i++) {
			if(gno[i]) {												// Redo�Ώ۸�ٰ�߂�
				OnDspGroupObject(i);									// ��ٰ�ߗ��̕\��
				m_GroupDisp[i] = 1;										// ��ٰ�ߕ\���׸�ON
			}
		}
		if(View->CountEdtObj()<1) {
			menu = View->GetActMenu();									// �N���ƭ��擾
			if(menu>=M_ALLSHADING&&menu!=M_EDIT_PASTEOBJ) {
				View->ExitActMenu(menu);								// �O�ƭ��I��
			}
		} else {
			View->InitActMenu(1);										// �ƭ���������
		}
		ResetSelNo();													// ���̑I��ؾ��
	}
	SetGroupObj();														// ��ٰ�ߗ��̐ݒ�
	View->InitChar();													// �����͏�����
	View->Display(0);													// �S���̍ĕ\��
}

/*********************************************/
void CMainFrame::OnUpdateRedo(CCmdUI* pCmdUI)							//<<< �Redo��t�X�V
/*********************************************/
{
	BOOL chk = TRUE;

	if(View->GetActMenu()==ID_MEASURE_SCALE||
       View->GetActMenu()==ID_MEASURE_GRAD) {
		chk = FALSE;
	}
	pCmdUI->Enable(CheckRedo()&&chk ? TRUE : FALSE);					// Redo�����ɂčX�V
}

/***********************************/
void CMainFrame::OnCopyObject(void)										//<<< ���߰��t����
/***********************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_EDIT_COPYOBJ);									// �O�ƭ��I��
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_COPYOBJ);		// �t�������
	WriteCopyFile(View->GetChar());										// ��߰�ް�̧�ُo��
	BaseObjPtr(2, &op, &dsp, &edt);										// �L�����̐擪�擾
	if(op!=NULL) {
		m_CopyCmdFlg = 1;												// �ҏW�Ώۗ��̂���
	}
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_COPYOBJ, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnCutObject(void)										//<<< ���ģ�t����
/**********************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_EDIT_CUTOBJ);									// �O�ƭ��I��
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_CUTOBJ);			// �t�������
	WriteCopyFile(View->GetChar());										// ��߰�ް�̧�ُo��
	BaseObjPtr(2, &op, &dsp, &edt);										// �L�����̐擪�擾
	if(op!=NULL) {
		m_CopyCmdFlg = 1;												// �ҏW�Ώۗ��̂���
	}
	View->MenuCall(View->GetActMenu(), 0, 0, 0, 11);					// �폜
	m_wndJcadEdit.GetToolBarCtrl().CheckButton(ID_EDIT_CUTOBJ, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/****************************************************/
void CMainFrame::OnUpdatePasteObject(CCmdUI* pCmdUI)					//<<< �߰�Ėt�L����/������&
/****************************************************/					//    �ƭ��t�X�V
{
	pCmdUI->Enable(m_CopyCmdFlg);										// ��߰����ޗL���ɂčX�V
	BOOL chk = View->GetActMenu()==(int)pCmdUI->m_nID||
               View->GetSaveMenu()==(int)pCmdUI->m_nID;
	pCmdUI->SetCheck(chk);												// �N���ƭ��̂�����
}

/*******************************/
void CMainFrame::OnDelete(void)											//<<< ��폜��t����
/*******************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->MenuCall(View->GetActMenu(), 0, 0, 0, 11);					// �ƭ��Ăяo��
}

/*******************************/
void CMainFrame::OnAllPic(void)											//<<< ��ꊇ�I��t����
/*******************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(View->GetActMenu());								// �O�ƭ��I��
	OnEdtAllObject();													// �ҏW���̑S�I��(�\������)
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->Display(0);
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnObjInfo(void)										//<<< ����̏��°٣�t����
/********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ShowOifTree();												// ���°��޲�۸ޕ\���ؑ�
	View->InitChar();													// �����͏�����
}

/************************************************/
void CMainFrame::OnUpdateObjInfo(CCmdUI* pCmdUI)						//<<< ����̏��°٣�t�X�V
/************************************************/
{
	pCmdUI->SetCheck(View->IsOifTreeShow() ? 1 : 0);					// °��ް�t&�ƭ�����
}

/************************************/
void CMainFrame::OnFunctionBtn(void)									//<<< �̧ݸ��ݐݒ�°٣�t����
/************************************/
{
	int mode;

	View->InitializeMenu();												// �ƭ���������
	View->InitChar();													// �����͏�����
	mode = View->IsFuncMode();											// ̧ݸ��ݕ\��Ӱ�ގ擾
	mode = (mode>5) ? 0 : mode+1;										// Ӱ�ޕϊ�
	CSetFuncModeDlg dlg;
	dlg.SetValue(mode);
	if(dlg.DoModal()==IDOK) {											// ̧ݸ��ݕ\��Ӱ���޲�۸ޕ\��
		dlg.GetValue(&mode);
		if(mode==0) {													// <�޲�۸ޏ���>
			View->ChangeFuncMode(6);									// ̧ݸ��ݕ\��Ӱ�ސݒ�
			View->ChangeFuncDsp(0);										// �ݒ�°��޲�۸ޕ\���ؑ�(����)
			View->ChangeFuncDsp2(0);									// �ݒ�°��޲�۸�2�\���ؑ�(����)
			View->HideFunction();										// ̧ݸ����޲�۸ޏ���
			View->HideFunction2();										// ̧ݸ����޲�۸�2����
		} else {														// <�޲�۸ޕ\��>
			View->ChangeFuncMode(mode-1);								// ̧ݸ��ݕ\��Ӱ�ސݒ�
			if(mode-1<3) {
				View->ChangeFuncDsp(1);									// �ݒ�°��޲�۸ޕ\���ؑ�(�\��)
				View->ChangeFuncDsp2(0);								// �ݒ�°��޲�۸�2�\���ؑ�(����)
				View->HideFunction2();									// ̧ݸ����޲�۸�2����
				if(mode-1==0) {											// <�풓>
					View->ShowFunction();								// ̧ݸ����޲�۸ޕ\��
				}
			} else {
				View->ChangeFuncDsp(0);									// �ݒ�°��޲�۸ޕ\���ؑ�(����)
				View->ChangeFuncDsp2(1);								// �ݒ�°��޲�۸�2�\���ؑ�(�\��)
				View->HideFunction();									// ̧ݸ����޲�۸ޏ���
				if(mode-1==3) {											// <�풓>
					View->ShowFunction2();								// ̧ݸ����޲�۸�2�\��
				}
			}
		}
	}
}

/****************************************************/
void CMainFrame::OnUpdateFunctionBtn(CCmdUI* pCmdUI)					//<<< �̧ݸ��ݐݒ�°٣�t�X�V
/****************************************************/
{
	pCmdUI->SetCheck(View->IsFuncDsp()||View->IsFuncDsp2() ? 1 : 0);	// °��ް�t&�ƭ�����
}

/**********************************/
void CMainFrame::OnCopyInput(void)										//<<< �����Ӱ�ޣ�t����
/**********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_COPY_INPUT);									// �O�ƭ��I��
	View->SetCopyBtn(View->GetCopyBtn() ? 0 : 1);						// ����Ӱ�ނ̐ؑ�
	View->InitChar();													// �����͏�����
}

/**************************************************/
void CMainFrame::OnUpdateCopyInput(CCmdUI* pCmdUI)						//<<< �����Ӱ�ޣ�t�X�V
/**************************************************/
{
	pCmdUI->SetCheck(View->GetCopyBtn() ? 1 : 0);						// °��ް�t&�ƭ�����
}

/*********************************/
void CMainFrame::OnNumInput(void)										//<<< ����lӰ�ޣ�t����
/*********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_NUM_INPUT);									// �O�ƭ��I��
	View->SetNumBtn(View->GetNumBtn() ? 0 : 1);							// ���l���͂̐ؑ�
}

/*************************************************/
void CMainFrame::OnUpdateNumInput(CCmdUI* pCmdUI)						//<<< ����lӰ�ޣ�t�X�V
/*************************************************/
{
	pCmdUI->SetCheck(View->GetNumBtn() ? 1 : 0);						// °��ް�t&�ƭ�����
}

/*********************************/
void CMainFrame::OnDispMode(void)										//<<< ��\��Ӱ�ޣ�t����
/*********************************/
{
	int mode=-1;

	View->InitializeMenu();												// �ƭ���������
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_MODE);				// �t�������
	if(View->GetChar()=='0') {											// 0��������Ӱ�ނȂ�
		mode = 0;
	} else if(View->GetChar()=='1') {									// 1��������Ӱ��1
		mode = 1;
	} else if(View->GetChar()=='2') {									// 2��������Ӱ��2
		mode = 2;
	} else if(View->GetChar()=='3') {									// 3��������Ӱ��3
		mode = 3;
	} else if(View->GetChar()=='4') {									// 4�������͓����}����ިݸ�
		mode = 4;
	} else if(View->GetChar()=='5') {									// 5�������͑S�ʼ���ިݸ�
		mode = 5;
	} else {
		CDispmodeDlg dlg;
		dlg.SetValue(View->GetDispMode());								// ���݂̐ݒ�l��\��
		if(dlg.DoModal()==IDOK) {										// �\��Ӱ���޲�۸ޕ\��
			dlg.GetValue(&mode);
		}
	}
	if(mode>=0) {														// <Ӱ�ގw��OK>
		View->SetDispMode(mode);										// �\��Ӱ�ސݒ�
		View->Display(0);												// �ĕ\��
	}
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_MODE, FALSE);		// �t�J�����
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnDispRange(void)										//<<< ��\���͈ͣ�t����
/**********************************/
{
	int range=-1;

	View->InitializeMenu();												// �ƭ���������
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_RANGE);				// �t�������
	if(View->GetChar()=='A') {											// A�������͑S����
		range = 0;
	} else if(View->GetChar()=='S') {									// S�������͓����}
		range = 1;
	} else if(View->GetChar()=='Z') {									// Z��������XY����
		range = 2;
	} else if(View->GetChar()=='X') {									// X��������YZ����
		range = 3;
	} else if(View->GetChar()=='C') {									// C��������ZX����
		range = 4;
	} else {
		CScreenDlg dlg;
		if(dlg.DoModal()==IDOK) {										// �\��Ӱ���޲�۸ޕ\��
			dlg.GetValue(&range);
		}
	}
	if(range>=0) {														// <�͈͎w��OK>
		View->SetScreenNo(range);										// �\���͈͐ݒ�
		View->Display(0);												// �ĕ\��
	}
	m_wndView.GetToolBarCtrl().CheckButton(ID_DISP_RANGE, FALSE);		// �t�J�����
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnAllDisp(void)										//<<< ��S�\����t����
/********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(View->GetActMenu());								// �O�ƭ��I��
	OnDspAllObject();													// �S���̂̕\��
	for(int i=0; i<32; i++) {
		m_GroupDisp[i] = 1;												// ��ٰ�ߕ\���S�\��
	}
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->Display(0);
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnAllErase(void)										//<<< ��S��\����t����
/*********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(View->GetActMenu());								// �O�ƭ��I��
	OffDspAllObject();													// �S���̂̔�\��
	for(int i=0; i<32; i++) {
		m_GroupDisp[i] = 0;												// ��ٰ�ߕ\���S��\��
	}
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->Display(0);
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnScrState(void)										//<<< ���ʏ�ԕ\����t����
/*********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ShowStateDlg();												// ��ʏ���޲�۸ޕ\���ؑ�
	View->InitChar();													// �����͏�����
}

/*************************************************/
void CMainFrame::OnUpdateScrState(CCmdUI* pCmdUI)						//<<< ���ʏ�ԕ\����t�X�V
/*************************************************/
{
	pCmdUI->SetCheck(View->IsStateDlgShow() ? 1 : 0);					// °��ް�t&�ƭ�����
}

/********************************/
void CMainFrame::OnDataUse(void)										//<<< ��ް��g�p���\����t����
/********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ShowDataUse();												// �ް��g�p���޲�۸ޕ\���ؑ�
	View->InitChar();													// �����͏�����
}

/************************************************/
void CMainFrame::OnUpdateDataUse(CCmdUI* pCmdUI)						//<<< ��ް��g�p���\����t�X�V
/************************************************/
{
	pCmdUI->SetCheck(View->IsDataUseShow() ? 1 : 0);					// °��ް�t&�ƭ�����
}

/**********************************/
void CMainFrame::OnParamDisp(void)										//<<< ����l�\����t����
/**********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->SetNumDisp(View->IsNumDispShow() ? 0 : 1);					// �\���޲�۸ޕ\���ؑ�
	View->InitChar();													// �����͏�����
}

/**************************************************/
void CMainFrame::OnUpdateParamDisp(CCmdUI* pCmdUI)						//<<< ����l�\����t�X�V
/**************************************************/
{
	pCmdUI->SetCheck(View->IsNumDispShow() ? 1 : 0);					// °��ް�t&�ƭ�����
}

/*********************************/
void CMainFrame::OnGroupDsp(void)										//<<< ���ٰ�ߕ\����t����
/*********************************/
{
	int i, kno, ano, func[8];

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(View->GetActMenu());								// �O�ƭ��I��
	kno = GetGroupKey();												// ��ٰ�ߑΉ����擾
	if(kno>0) {															// <�p����������>
		if(m_GroupObj[kno-1]) {											// �������ŗ��̂���
			if(!m_GroupDisp[kno-1]) {									// <��\�����̏ꍇ>
				OnDspGroupObject(kno-1);								// ��ٰ�ߗ��̕\��
				m_GroupDisp[kno-1] = 1;									// �\������
			} else {													// <�\�����̏ꍇ>
				OffDspGroupObject(kno-1);								// ��ٰ�ߗ��̔�\��
				m_GroupDisp[kno-1] = 0;									// ��\������
			}
		}
	} else {															// <̧ݸ��ݷ�����>
		View->GetFChar(func);											// ̧ݸ��ݷ��擾
		ano = (View->GetCChar()&0x2) ? 8 : 0;							// Ctrl��������+8
		for(i=0; i<8; i++) {											// ��ٰ��1�`8
			if(func[i]&&m_GroupObj[i+ano]) {							// �Y���������ŗ��̂���
				if(!m_GroupDisp[i+ano]) {								// <��\�����̏ꍇ>
					OnDspGroupObject(i+ano);							// ��ٰ�ߗ��̕\��
					m_GroupDisp[i+ano] = 1;								// �\������
				} else {												// <�\�����̏ꍇ>
					OffDspGroupObject(i+ano);							// ��ٰ�ߗ��̔�\��
					m_GroupDisp[i+ano] = 0;								// ��\������
				}
			}
		}
	}
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->Display(0);
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnGroupSel(void)										//<<< ���ٰ�ߑI��t����
/*********************************/
{
	int i, kno, ano, func[8];

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(View->GetActMenu());								// �O�ƭ��I��
	kno = GetGroupKey();												// ��ٰ�ߑΉ����擾
	if(kno>0) {															// <�p����������>
		OnEdtGroupObject(kno-1);										// �ҏW����(��ٰ��)�I��
	} else {															// <̧ݸ��ݷ�����>
		View->GetFChar(func);											// ̧ݸ��ݷ��擾
		ano = (View->GetCChar()&0x2) ? 8 : 0;							// Ctrl��������+8
		for(i=0; i<8; i++) {											// ��ٰ��1�`8
			if(func[i]) {
				OnEdtGroupObject(i+ano);								// �ҏW����(��ٰ��)�I��
			}
		}
	}
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->Display(0);
	View->InitChar();													// �����͏�����
}

/*******************************/
void CMainFrame::OnZoomIn(void)											//<<< ��ްѥ�ݣ�t����
/*******************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ZoomIn();														// 3�ʐ}�̽ްѥ��
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnZoomOut(void)										//<<< ��ްѥ��ģ�t����
/********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ZoomOut();													// 3�ʐ}�̽ްѥ���
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnAutoZoom(void)										//<<< ������{����t����
/*********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->AutoZoom();													// 3�ʐ}�̎����{��
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnExactZoom(void)										//<<< ������\����t����
/**********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExactZoom();													// 3�ʐ}�̎����\��
	View->InitChar();													// �����͏�����
}

/***********************************/
void CMainFrame::OnScrollTool(void)										//<<< ���ʽ�۰٥°٣�t����
/***********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ShowScrollTool();												// ��۰�°��޲�۸ޕ\���ؑ�
	View->InitChar();													// �����͏�����
}

/***************************************************/
void CMainFrame::OnUpdateScrollTool(CCmdUI* pCmdUI)						//<<< ���ʽ�۰٥°٣�t�X�V
/***************************************************/
{
	pCmdUI->SetCheck(View->IsScrollToolShow() ? 1 : 0);					// °��ް�t&�ƭ�����
}

/********************************/
void CMainFrame::OnHomePos(void)										//<<< �HOME��t����
/********************************/
{
	int func[8], no=1;

	if(View->GetChar()=='1') {											// [1]��������Home1
		no = 1;
	} else if(View->GetChar()=='2') {									// [2]��������Home2
		no = 2;
	} else if(View->GetChar()=='3') {									// [3]��������Home3
		no = 3;
	} else if(View->GetChar()=='4') {									// [4]��������Home4
		no = 4;
	} else if(View->GetChar()=='5') {									// [5]��������Home5
		no = 5;
	} else {															// <[1]�`[5]�������Ȃ�>
		View->GetFChar(func);											// ̧ݸ��ݷ��擾
		if(func[0]) {													// [F5]��������Home2
			no = 2;
		} else if(func[1]) {											// [F6]��������Home3
			no = 3;
		} else if(func[2]) {											// [F7]��������Home4
			no = 4;
		} else if(func[3]) {											// [F8]��������Home5
			no = 5;
		}
	}
	View->InitializeMenu();												// �ƭ���������
	View->RestoreHomePosition(no);										// �\����ԕ��A
	View->InitChar();													// �����͏�����
}

/***********************************/
void CMainFrame::OnBackScreen(void)										//<<< ��O�\����ԣ�t����
/***********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->BackScreenState();											// �O�\����Ԗ߂�
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnTruckUp(void)										//<<< ��ׯ�����ߣ�t����
/********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->TruckUp();													// �����}���ׯ������
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnTruckDown(void)										//<<< ��ׯ���޳ݣ�t����
/**********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->TruckDown();													// �����}���ׯ���޳�
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnCentPars(void)										//<<< ��Q�Ɠ_�w�裖t����
/*********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->CentPars();													// �����}�̎Q�Ɠ_�w��
	View->InitChar();													// �����͏�����
}

/*******************************/
void CMainFrame::OnEyeSet(void)											//<<< ������}�ύX��t����
/*******************************/
{
	int flg=-1;

	View->InitializeMenu();												// �ƭ���������
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_EYE_SET);			// �t�������
	if(View->GetChar()=='X') {											// X��������XY(�\)
		flg = 0;
	} else if(View->GetChar()=='Y') {									// Y��������YZ(�\)
		flg = 1;
	} else if(View->GetChar()=='Z') {									// Z��������ZX(�\)
		flg = 2;
	} else if(View->GetChar()=='U') {									// U��������XY(��)
		flg = 3;
	} else if(View->GetChar()=='V') {									// V��������YZ(��)
		flg = 4;
	} else if(View->GetChar()=='W') {									// W��������ZX(��)
		flg = 5;
	} else {
		CEyesetDlg dlg;
		if(dlg.DoModal()==IDOK) {										// �����}�ύX�޲�۸ޕ\��
			dlg.GetValue(&flg);
		}
	}
	if(flg>=0) {
		View->EyeSet(flg);												// �����}�̕ύX
	}
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_EYE_SET, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnEyeChange(void)										//<<< ����_�ύX�°٣�t����
/**********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ShowEyeChange();												// �ύX°��޲�۸ޕ\���ؑ�
	View->InitChar();													// �����͏�����
}

/**************************************************/
void CMainFrame::OnUpdateEyeChange(CCmdUI* pCmdUI)						//<<< ����_�ύX�°٣�t�X�V
/**************************************************/
{
	pCmdUI->SetCheck(View->IsEyeChangeShow() ? 1 : 0);					// °��ް�t&�ƭ�����
}

/*********************************/
void CMainFrame::OnGridDisp(void)										//<<< ���د�ޕ\����t����
/*********************************/
{
	int func[8], check1, check2, check3;

	View->InitializeMenu();												// �ƭ���������
	check1 = View->GetGridDisp();										// ��د�ޕ\����Ԏ擾
	check2 = View->GetBasePointDisp();									// ��_�\����Ԏ擾
	check3 = View->GetBaseLineDisp();									// ��_�\����Ԏ擾
	if(check1||check2||check3) {										// <<���ꂩ���\����>>
		if(check1) {													// <��د��>
			View->SetGridDisp(0);										// ��د�ޕ\����Ԑݒ�(��\��)
			View->DispGrid(XY, 1);										// ��د�ޏ���
			View->DispGrid(YZ, 1);
			View->DispGrid(ZX, 1);
		}
		if(check2) {													// <��_>
			View->SetBasePointDisp(0);									// ��_�\����Ԑݒ�(��\��)
			View->Display(0);											// �ĕ`��
		}
		if(check3) {													// <���>
			View->SetBaseLineDisp(0);									// ��_�\����Ԑݒ�(��\��)
			View->Display(0);											// �ĕ`��
		}
	} else {															// <<�S�Ă���\��>>
		View->GetFChar(func);											// ̧ݸ��ݷ��擾
		if(func[0]||(!func[1]&&!func[2])) {								// <��د��>
			View->SetGridDisp(1);										// ��د�ޕ\����Ԑݒ�(�\��)
			View->SetGrid();											// ��د�ސݒ�
			View->DispGrid(XY, 0);										// ��د�ޕ\��
			View->DispGrid(YZ, 0);
			View->DispGrid(ZX, 0);
		}
		if(func[1]) {													// <��_>
			View->SetBasePointDisp(1);									// ��_�\����Ԑݒ�(�\��)
			View->Display(0);											// �ĕ`��
		}
		if(func[2]) {													// <���>
			View->SetBaseLineDisp(1);									// ��_�\����Ԑݒ�(�\��)
			View->Display(0);											// �ĕ`��
		}
	}
	View->InitChar();													// �����͏�����
}

/*************************************************/
void CMainFrame::OnUpdateGridDisp(CCmdUI* pCmdUI)						//<<< ���د�ޕ\����t�X�V
/*************************************************/
{
	int check1, check2, check3;

	check1 = View->GetGridDisp();										// ��د�ޕ\����Ԏ擾
	check2 = View->GetBasePointDisp();									// ��_�\����Ԏ擾
	check3 = View->GetBaseLineDisp();									// ��_�\����Ԏ擾
	pCmdUI->SetCheck(check1||check2||check3 ? 1 : 0);					// °��ް�t&�ƭ�����
}

/*********************************/
void CMainFrame::OnGridBind(void)										//<<< ���د�ލS����t����
/*********************************/
{
	int func[8], check1, check2, check3;

	View->InitializeMenu();												// �ƭ���������
	check1 = View->GetGridBind();										// ��د�ލS����Ԏ擾
	check2 = View->GetBasePointBind();									// ��_�S����Ԏ擾
	check3 = View->GetBaseLineBind();									// ��_�S����Ԏ擾
	if(check1||check2||check3) {										// <���ꂩ���S����>
		View->SetGridBind(0);											// ��د�ލS����Ԑݒ�(��S��)
		View->SetBasePointBind(0);										// ��_�S����Ԑݒ�(��S��)
		View->SetBaseLineBind(0);										// ��_�S����Ԑݒ�(��S��)
	} else {															// <�S�Ă���S��>
		View->GetFChar(func);											// ̧ݸ��ݷ��擾
		if(func[0]||(!func[1]&&!func[2])) {								// <��د��[F5]�w��/[F6],[F7]�ȊO>
			View->SetGridBind(1);										// ��د�ލS����Ԑݒ�(�S��)
		}
		if(func[1]) {													// <��_[F6]�w��>
			View->SetBasePointBind(1);									// ��_�S����Ԑݒ�(�S��)
		}
		if(func[2]) {													// <���[F7]�w��>
			View->SetBaseLineBind(1);									// ��_�S����Ԑݒ�(�S��)
		}
	}
	View->InitChar();													// �����͏�����
}

/*************************************************/
void CMainFrame::OnUpdateGridBind(CCmdUI* pCmdUI)						//<<< ���د�ލS����t�X�V
/*************************************************/
{
	int check1, check2, check3;

	check1 = View->GetGridBind();										// ��د�ލS����Ԏ擾
	check2 = View->GetBasePointBind();									// ��_�S����Ԏ擾
	check3 = View->GetBaseLineBind();									// ��_�S����Ԏ擾
	pCmdUI->SetCheck(check1||check2||check3 ? 1 : 0);					// °��ް�t&�ƭ�����
}

/**********************************/
void CMainFrame::OnGridSpace(void)										//<<< ���د�ފԊu��t����
/**********************************/
{
	int    gitv;
	double gspc;

	View->InitializeMenu();												// �ƭ���������
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_GRID_SPACE);			// �t�������
	CInputDlg dlg(1);
	dlg.SetValue(View->GetGridSpace()/1000.0, View->GetGridIntvl());	// ��د�ފԊu�擾,�ڐ��Ԋu�擾
	if(dlg.DoModal()==IDOK) {											// ��د�ފԊu�ݒ��޲�۸ޕ\��
		dlg.GetValue(&gspc, &gitv);
		View->SetGridSpace((int)(gspc*1000.0));							// ��د�ފԊu�ݒ�(��m�P��)
		View->SetGridIntvl(gitv);										// �ڐ��Ԋu�ݒ�
		View->SetGrid();												// ��د�ސݒ�
		View->Display(0);												// �ĕ`��
	}
	m_wndScreenCtl.GetToolBarCtrl().CheckButton(ID_GRID_SPACE, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/*************************************/
void CMainFrame::OnGuideLineBox(void)									//<<< �GLB��t����
/*************************************/
{
	int    dsp=-1, chg, kind;
	double size;

	View->InitializeMenu();												// �ƭ���������
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_GLB);				// �t�������
	chg = View->GetGlbChange();											// �ݸ޻��ޕύX�w���擾
	if(View->GetChar()=='0') {											// 0�������͂Ȃ�
		dsp = 0;
	} else if(View->GetChar()=='1') {									// 1��������GLB1
		dsp = 1;
	} else if(View->GetChar()=='2') {									// 2��������GLB2
		dsp = 2;
	} else if(View->GetChar()=='3') {									// 3��������GLB3
		dsp = 3;
	} else if(View->GetChar()=='4') {									// 4��������GLB4
		dsp = 4;
	} else {
		kind = View->GetRingKind();										// �ݸ޻��ގ�ގ擾
		size = (!kind) ? View->GetRingSize1() : View->GetRingSize2();	// �ݸ޻��ގ擾
		CGlbDlg dlg(kind);
		dlg.SetValue(View->GetDspGlb(), chg, size);						// ���݂̐ݒ�l��\��
		if(dlg.DoModal()==IDOK) {										// GLB�޲�۸ޕ\��
			dlg.GetValue(&dsp, &chg, &size);
			if(chg) {													// <�ݸ޻��ޕύXOK>
				if(!kind) {
					View->SetRingSize1(size);							// �����ݸ޻��ސݒ�
				} else {
					View->SetRingSize2(size);							// ���a�ݸ޻��ސݒ�
				}
			}
		}
	}
	if(dsp>=0) {														// <GLB�w��OK>
		View->SetDspGlb(dsp);											// GLB�\��No�ݒ�
		View->SetGlbChange(chg);										// �ݸ޻��ޕύX�w���ݒ�
		if(View->ReadGLB()!=TRUE) {										// GLB����
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
			View->DeleteGLB();											// GLB�j��
			View->SetDspGlb(0);
		}
		View->Display(0);												// �ĕ\��
	}
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_GLB, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnDspTemplate(void)									//<<< �����ڰĕ\����t����
/************************************/
{
	int    no=-1, chg, kind;
	double size;

	View->InitializeMenu();												// �ƭ���������
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_TEMP);				// �t�������
	chg = View->GetGlbChange();											// �ݸ޻��ޕύX�w���擾
	if(View->GetChar()=='0') {											// 0�������͂Ȃ�
		no = 0;
	} else if(View->GetChar()=='1') {									// 1������������ڰ�1
		no = 1;
	} else if(View->GetChar()=='2') {									// 2������������ڰ�2
		no = 2;
	} else if(View->GetChar()=='3') {									// 3������������ڰ�3
		no = 3;
	} else if(View->GetChar()=='4') {									// 4������������ڰ�4
		no = 4;
	} else if(View->GetChar()=='5') {									// 5������������ڰ�5
		no = 5;
	} else if(View->GetChar()=='6') {									// 6������������ڰ�6
		no = 6;
	} else if(View->GetChar()=='7') {									// 7������������ڰ�7
		no = 7;
	} else if(View->GetChar()=='8') {									// 8������������ڰ�8
		no = 8;
	} else {
		kind = View->GetRingKind();										// �ݸ޻��ގ�ގ擾
		size = (!kind) ? View->GetRingSize1() : View->GetRingSize2();	// �ݸ޻��ގ擾
		CDispTmpDlg dlg(kind);
		dlg.SetValue(View->GetDspTmpNo(), chg, size);					// ���݂̐ݒ�l��\��
		if(dlg.DoModal()==IDOK) {										// GLB�޲�۸ޕ\��
			dlg.GetValue(&no, &chg, &size);
			if(chg) {													// <�ݸ޻��ޕύXOK>
				if(!kind) {
					View->SetRingSize1(size);							// �����ݸ޻��ސݒ�
				} else {
					View->SetRingSize2(size);							// ���a�ݸ޻��ސݒ�
				}
			}
		}
	}
	if(no>=0) {															// <����ڰĎw��OK>
		View->SetDspTmpNo(no);											// ����ڰĕ\��No�ݒ�
		View->SetGlbChange(chg);										// �ݸ޻��ޕύX�w���ݒ�
		if(View->ReadTEMP()!=TRUE) {									// ����ڰē���
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
			View->DeleteTEMP();											// ����ڰĔj��
			View->SetDspTmpNo(0);
		}
		View->Display(0);												// �ĕ\��
	}
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_DSP_TEMP, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnSetTemplate(void)									//<<< �����ڰĐݒ裖t����
/************************************/
{
	CString b1, b2, b3;
	int     i;

	View->InitializeMenu();												// �ƭ���������
	CSetTmpDlg dlg;
	b1 = View->GetTmpltPath();											// ����ڰ��߽�擾
	dlg.SetValue1(b1);													// �����l�\��(�߽)
	for(i=0; i<8; i++) {
		View->GetTmpFile(i, &b1, &b2, &b3);								// ����ڰ�̧�ٖ��擾
		dlg.SetValue2(i, b1, b2, b3);									// �����l�\��(̧�ٖ�)
	}
	if(dlg.DoModal()==IDOK) {											// ����ڰĐݒ��޲�۸ޕ\��
		dlg.GetValue1(&b1);
		View->SetTmpltPath(b1);											// ����ڰ��߽�ݒ�
		for(i=0; i<8; i++) {
			dlg.GetValue2(i, &b1, &b2, &b3);
			View->SetTmpFile(i, b1, b2, b3);							// ����ڰ�̧�ٖ��ݒ�
		}
	}
	if(View->GetDspTmpNo()) {											// ����ڰĕ\��No�L���ȏꍇ
		if(View->ReadTEMP()!=TRUE) {									// ����ڰē���
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
			View->DeleteTEMP();											// ����ڰĔj��
			View->SetDspTmpNo(0);
		}
		View->Display(0);												// �ĕ\��
	}
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnTexture(void)										//<<< �ø����ݒ裖t����
/********************************/
{
	View->InitializeMenu();												// �ƭ���������
	CSetTexDlg dlg;
	dlg.DoModal();														// ø����ݒ��޲�۸ޕ\��
	View->Display(0);													// �ĕ\��
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnSetHome(void)										//<<< �HOME�o�^��t����
/********************************/
{
	int func[8], no=1;

	if(View->GetChar()=='1') {											// [1]��������Home1
		no = 1;
	} else if(View->GetChar()=='2') {									// [2]��������Home2
		no = 2;
	} else if(View->GetChar()=='3') {									// [3]��������Home3
		no = 3;
	} else if(View->GetChar()=='4') {									// [4]��������Home4
		no = 4;
	} else if(View->GetChar()=='5') {									// [5]��������Home5
		no = 5;
	} else {															// <[1]�`[5]�������Ȃ�>
		View->GetFChar(func);											// ̧ݸ��ݷ��擾
		if(func[0]) {													// [F5]��������Home2
			no = 2;
		} else if(func[1]) {											// [F6]��������Home3
			no = 3;
		} else if(func[2]) {											// [F7]��������Home4
			no = 4;
		} else if(func[3]) {											// [F8]��������Home5
			no = 5;
		}
	}
	View->InitializeMenu();												// �ƭ���������
	View->SetHomePosition(no);											// �\����ԕۑ�
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnRayTrace(void)										// ������ݸ�(ڲ���)��t����
/*********************************/
{
	if(!View->IsRendDlgShow()) {
		View->CreateRendDlg();											// �����ݸ��޲�۸ސ���
	}
}

/*************************************************/
void CMainFrame::OnUpdateRayTrace(CCmdUI* pCmdUI)						//<<< ������ݸ�(ڲ���)��t�X�V
/*************************************************/
{
	UINT id=pCmdUI->m_nID;												// �����ID�擾
	pCmdUI->Enable(View->GetDspObjNum()>0?TRUE:FALSE);					// �\���Ώۂ̗L���ɂčX�V
	pCmdUI->SetCheck(View->IsRendDlgShow());							// °��ް�t&�ƭ�����
}

/********************************/
void CMainFrame::OnPovData(void)										// �POV-Ray�ް��o�ͣ�t����
/********************************/
{
	CString str, file;

	View->InitializeMenu();												// �ƭ���������
	if(GetDemoFlg()!=0) {												// ��̌���,�N��NG�
		AfxMessageBox(IDS_TRIAL);
		return;
	}
	View->ExitActMenu(ID_POVDATA);										// �O�ƭ��I��
	str.LoadString(IDS_STR19);
	CPovEnvSheet pvsht(str);
	if(pvsht.DoModal()==IDOK) {											// POV-Ray�ް��o�͊��ݒ��޲�۸�
		pvsht.SetPovRayData();											// POV-Ray�ް��ݒ�
		pvsht.GetValue(&file);
		if(file!="") {
			CreatePovData(file);										// POV-Ray�ް��쐬
		}
	}
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnMaterial(void)										//<<< ��װ�ݒ裖t����
/*********************************/
{
	OBJTYPE* op;
	int      dsp, edt, ocl, col=-1;

	View->InitializeMenu();												// �ƭ���������
	m_wndRendering.GetToolBarCtrl().CheckButton(ID_COLOR);				// �t�������
	if(View->GetChar()=='G') {											// G�������ͺް���
		col = 0;
	} else if(View->GetChar()=='L') {									// L�������ͼ��ް
		col = 1;
	} else if(View->GetChar()=='Y') {									// Y�������͸ؽ��
		col = 2;
	} else if(View->GetChar()=='R') {									// R����������ް
		col = 3;
	} else if(View->GetChar()=='E') {									// E�������ʹ�����
		col = 4;
	} else if(View->GetChar()=='S') {									// L�������ͻ̧��
		col = 5;
	} else if(View->GetChar()=='1') {									// 1��������հ�ް1
		col = 6;
	} else if(View->GetChar()=='2') {									// 2��������հ�ް2
		col = 7;
	} else if(View->GetChar()=='3') {									// 3��������հ�ް3
		col = 8;
	} else if(View->GetChar()=='4') {									// 4��������հ�ް4
		col = 9;
	} else if(View->GetChar()=='5') {									// 5��������հ�ް5
		col = 10;
	} else if(View->GetChar()=='6') {									// 6��������հ�ް6
		col = 11;
	} else if(View->GetChar()=='7') {									// 7��������հ�ް7
		col = 12;
	} else if(View->GetChar()=='8') {									// 8��������հ�ް8
		col = 13;
	} else if(View->GetChar()=='9') {									// 9��������հ�ް9
		col = 14;
	} else if(View->GetChar()=='A') {									// A��������հ�ް10
		col = 15;
	} else if(View->GetChar()=='B') {									// B��������հ�ް11
		col = 16;
	} else if(View->GetChar()=='C') {									// C��������հ�ް12
		col = 17;
	} else {
		BaseObjPtr(2, &op, &dsp, &edt);									// �擪���̂��߲���擾
		ocl = (op) ? GetObjCol(op) : 0;									// ���̶̂װ�擾
		CColorDlg dlg;
		dlg.SetValue(ocl);
		if(dlg.DoModal()==IDOK) {										// �ގ��ݒ��޲�۸ޕ\��
			dlg.GetValue(&col);
		}
	}
	if(col>=0) {														// <�ގ��ݒ�L��>
		BaseObjPtr(2, &op, &dsp, &edt);									// �擪���̂��߲���擾
		while(op) {
			PutObjCol(op, col);											// ���̶̂װ�ݒ�
			NextObjPtr(2, &op, &dsp, &edt);								// �����̂��߲���擾
		}
		if(View->GetRenderMode()==2||View->GetRenderMode()==3||
           View->GetDispMode()==4) {
			View->Display(0);											// �����ݸ�/����ިݸނ͍ĕ\��
		}
	}
	m_wndRendering.GetToolBarCtrl().CheckButton(ID_COLOR, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/*************************************/
void CMainFrame::OnCrtPrimitive(void)									//<<< �����è�ޣ�t����
/*************************************/
{
	int pno=-1;

	View->InitializeMenu();												// �ƭ���������
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_PRIMITIVE);			// �t�������
	if(View->GetChar()=='C') {											// C�������͉~
		pno = 0;
	} else if(View->GetChar()=='Q') {									// Q�������͒����`
		pno = 1;
	} else if(View->GetChar()=='S') {									// S�������͋���
		pno = 2;
	} else if(View->GetChar()=='B') {									// B�������͒�����
		pno = 3;
	} else if(View->GetChar()=='Y') {									// Y�������͉~��
		pno = 4;
	} else if(View->GetChar()=='N') {									// N�������͉~��
		pno = 5;
	} else {
		CPrimitiveDlg dlg;
		if(dlg.DoModal()==IDOK) {										// ����è���޲�۸ޕ\��
			dlg.GetValue(&pno);
		}
	}
	if(pno>=0) {														// <����è�ގw��OK>
		View->ExitActMenu(ID_CRT_PRIMITIVE);							// �O�ƭ��I��
		if(!ReadPrimitive(pno)) {										// ����è�ޓ���
			View->CountEdtObj();										// �ҏW�Ώۗ��̶���
			View->Display(0);											// �ĕ`��
		} else {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
		View->ExitMenu(0, 1);											// �ƭ��I��
	}
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_PRIMITIVE,FALSE);	// �t������
}

/*************************************/
void CMainFrame::OnCrtCentCurve(void)									//<<< ����̒��S�_�Ȑ�������t����
/*************************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_CRT_CENTCURVE);								// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CENTCURVE);			// �t�������
	View->MenuCentCurve();												// ���̒��S�_�Ȑ�����
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CENTCURVE, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnCrtSetCurve(void)									//<<< ��Ȑ��g������t����
/************************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_CRT_SETCURVE);									// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SETCURVE);			// �t�������
	View->MenuSetCurve();												// �Ȑ��g����
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SETCURVE, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/***********************************/
void CMainFrame::OnCrtSection(void)										//<<< ��f�ʎw�裖t����
/***********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_CRT_SECTION);									// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SECTION);			// �t�������
	View->MenuSection();												// �f�ʎw��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SECTION, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnCrtSpiral(void)										//<<< �������t����
/**********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_CRT_SPIRAL);									// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SPIRAL);			// �t�������
	View->MenuSpiral();													// ��������
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_SPIRAL, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/*************************************/
void CMainFrame::OnResetSurface(void)									//<<< ��Ȑ�/�Ȗʍč쐬��t����
/*************************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_RESET_SURFACE);								// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_RESET_SURFACE);			// �t�������
	View->MenuResetSurface();											// �Ȑ�/�Ȗʍč쐬
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_RESET_SURFACE, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/***********************************/
void CMainFrame::OnCrtConnect(void)										//<<< ��A����t����
/***********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_CRT_CONNECT);									// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CONNECT);			// �t�������
	View->MenuConnect();												// �A��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_CONNECT, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnCrtThick(void)										//<<< ����ݕt����t����
/*********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_CRT_THICK);									// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_THICK);				// �t�������
	View->MenuThickness();												// ���ݕt��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_THICK, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnCrtText(void)										//<<< �÷�ģ�t����
/********************************/
{
	int    fno, chk;
	double hei, dpt, itv;

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_CRT_TEXT);										// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_TEXT);				// �t�������
	fno = View->GetFontNo();											// �����l�擾
	chk = View->GetFontCheck();
	hei = View->GetTextHeight();
	dpt = View->GetTextDepth();
	itv = View->GetTextIntval();
	CSetTextDlg dlg;
	dlg.SetValue(fno, chk, hei, dpt, itv);								// �����l�\��
	if(dlg.DoModal()==IDOK) {											// ���̕�������
		dlg.GetValue(&fno, &chk, &hei, &dpt, &itv);						// ���̕����������擾
		View->SetFontNo(fno);											// �����l�ۑ�
		View->SetFontCheck(chk);
		View->SetTextHeight(hei);
		View->SetTextDepth(dpt);
		View->SetTextIntval(itv);
	}
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_CRT_TEXT, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnEditSurface(void)									//<<< ��Ȗʎw��ړ���t����
/************************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_EDIT_SURFACE);									// �O�ƭ��I��
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_SURFACE);			// �t�������
	View->MenuSurfaceEdit();											// �Ȗʎw��ړ�
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_SURFACE, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/*********************************/
void CMainFrame::OnEditBool(void)										//<<< ��}�`���Z��t����
/*********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_EDIT_BOOL);									// �O�ƭ��I��
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_BOOL);				// �t�������
	View->MenuBoolean();												// �}�`���Z
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_EDIT_BOOL, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnSurfOAdjust(void)									//<<< ��Ȗʗ��̍��킹��t����
/************************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_SURF_O_ADJUST);								// �O�ƭ��I��
	m_wndSurf.GetToolBarCtrl().CheckButton(ID_SURF_O_ADJUST);			// �t�������
	View->MenuObjectAdjust();											// �Ȗʗ��̍��킹
	m_wndSurf.GetToolBarCtrl().CheckButton(ID_SURF_O_ADJUST, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnSurfParam(void)										//<<< ��Ȑ����Ұ���t����
/**********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_SURF_PARAM);									// �O�ƭ��I��
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_SURF_PARAM);			// �t�������
	View->MenuChgSurfParam(0);											// �Ȑ����Ұ��ύX
	m_wndCreate.GetToolBarCtrl().CheckButton(ID_SURF_PARAM, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnJewelryRing(void)									//<<< ��ݸ޻��ލ��킹��t����
/************************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_JWL_RING);										// �O�ƭ��I��
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_RING);				// �t�������
	View->MenuJwlRing();												// �ݸ޻��ލ��킹
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_RING, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/********************************/
void CMainFrame::OnSetUnit(void)										//<<< ��Ưĥ��޼ު�ģ�t����
/********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_JWL_UNIT);										// �O�ƭ��I��
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_UNIT);				// �t�������
	View->MenuSetUnit();												// �Ưĥ��޼ު��
	m_wndJewelry.GetToolBarCtrl().CheckButton(ID_JWL_UNIT, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnMeasureCalc(void)									//<<< ��v�Z�@��t����
/************************************/
{
	if(!View->IsCalcDlg1Show()&&!View->IsCalcDlg2Show()) {
		View->CreateCalcDlg1();											// �v�Z�@�޲�۸�1����
	}
}

/****************************************************/
void CMainFrame::OnUpdateMeasureCalc(CCmdUI* pCmdUI)					//<<< ��v�Z�@��t�X�V
/****************************************************/
{
	pCmdUI->SetCheck(View->IsCalcDlg1Show()||View->IsCalcDlg2Show());	// °��ް�t&�ƭ�����
}

/**************************************/
void CMainFrame::OnMeasureVolume(void)									//<<< ��̐ϑ��裖t����
/**************************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_MEASURE_VOLUME);								// �O�ƭ��I��
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_MEASURE_VOLUME);		// �t�������
	View->MenuMeasureVolume();											// �̐ϑ���
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_MEASURE_VOLUME, FALSE);// �t������
	View->InitChar();													// �����͏�����
}

/***********************************/
void CMainFrame::OnSimulation(void)										//<<< ��Эڰ��ݣ�t����
/***********************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_NC_SIMULATE);									// �O�ƭ��I��
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_SIMULATE);			// �t�������
	View->MenuSimulation();												// �Эڰ���
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_SIMULATE, FALSE);	// �t������
	View->InitChar();													// �����͏�����
}

/*******************************/
void CMainFrame::OnNcDisp(void)											//<<< �NC�ް��\����t����
/*******************************/
{
	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_NC_DISP);										// �O�ƭ��I��
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_DISP);				// �t�������
	View->MenuNCDisplay();												// NC�ް��\��
	m_wndMeasure.GetToolBarCtrl().CheckButton(ID_NC_DISP, FALSE);		// �t������
	View->InitChar();													// �����͏�����
}

/***************************************/
void CMainFrame::OnSetEnvironment(void)									//<<< ����ݒ裖t����
/***************************************/
{
	CString str, pth;
	int     i, key, rep, atr, sec, shp, shd, max, sc1, sc2, btn;
	int     n10, n11, n12, n13, n14, n15, n16, n17, n18, n19;
	int     n20, n21, n22, n23, n24, n25, n26, n27, n28, n29;
	double  wgt;

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_SET_ENV);										// �O�ƭ��I��
	key = View->GetKeyEnv();											// ���䷰���擾
	rep = View->GetCurveRepeat();										// �Ȑ��������擾
	wgt = View->GetCurveWeight();										// �Ȑ��d�݌W���擾
	atr = View->GetAtrEnv();											// �Ȗʐ������̑������擾
	sec = View->GetSecEnv();											// �Ȗʐ����[�ʌ`����擾
	shp = View->GetShpEnv();											// �ȖʕҏW�`����擾
	shd = View->GetShdEnv();											// �ȖʕҏW����ިݸޏ��擾
	max = View->GetSaveFileMax();										// �ő��ޯ����ߐ��擾
	pth = View->GetSaveFilePath();										// �ޯ����ߐ�̫��ގ擾	
	sc1 = View->GetScrEnv1();											// ��ʏ��������擾
	sc2 = View->GetScrEnv2();
	btn = View->GetBtnEnv();											// Ӱ�����ݏ��擾
	n10 = GetIntMax();													// �������Ұ��z�񐔎擾
	n11 = GetPntMax();													// ���W���Ұ��z�񐔎擾
	n12 = GetUndoMax();													// Undo/Redo�z�񐔎擾
	n13 = GetCCtlMax();													// �ő�Ȑ�����_���擾
	n14 = GetSCtlMax();													// �ő�Ȗʐ���_���擾
	n15 = GetWCtlMax();													// ��Ɨp����_�z�񐔎擾
	n16 = GetCPntMax();													// �ő�Ȑ����_���擾
	n17 = GetSPntMax();													// �ő�Ȗʒ��_���擾
	n18 = GetOPntMax();													// �ő嗧�̒��_���擾
	n19 = GetBPntMax();													// �ő�f�ʒ��_���擾
	str.LoadString(IDS_STR17);
	CSysEnvSheet sesht(str);
	sesht.SetValue(key, rep, wgt, atr, sec, shp, shd,
                   max, pth, sc1, sc2, btn, n10, n11,
                   n12, n13, n14, n15, n16, n17, n18, n19);
	if(sesht.DoModal()==IDOK) {											// ���ݒ��޲�۸�
		sesht.GetValue(&key, &rep, &wgt, &atr, &sec, &shp, &shd,
                       &max, &pth, &sc1, &sc2, &btn, &n20, &n21,
                       &n22, &n23, &n24, &n25, &n26, &n27, &n28, &n29);
		View->SetKeyEnv(key);											// ���䷰���ݒ�
		View->SetCurveRepeat(rep);										// �Ȑ��������ݒ�
		View->SetCurveWeight(wgt);										// �Ȑ��d�݌W���ݒ�
		View->SetAtrEnv(atr);											// �Ȗʐ������̑������ݒ�
		View->SetSecEnv(sec);											// �Ȗʐ����[�ʌ`����ݒ�
		View->SetShpEnv(shp);											// �ȖʕҏW�`����ݒ�
		View->SetShdEnv(shd);											// �ȖʕҏW����ިݸޏ��ݒ�
		View->SetSaveFileMax(max);										// �ő��ޯ����ߐ��ݒ�
		View->SetSaveFilePath(pth);										// �ޯ����ߐ�̫��ސݒ�	
		View->SetScrEnv1(sc1);											// ��ʏ��������ݒ�
		View->SetScrEnv2(sc2);
		View->SetCtrlKey();												// ���䷰�ݒ�
		View->SetBtnEnv(btn);											// Ӱ�����ݏ��ݒ�
		if(n10!=n20||n11!=n21||n12!=n22||n13!=n23||n14!=n24||
           n15!=n25||n16!=n26||n17!=n27||n18!=n28||n19!=n29) {			// <�̈搔�ύX����>
			SetIntMax(n20);												// �������Ұ��z�񐔐ݒ�
			SetPntMax(n21);												// ���W���Ұ��z�񐔐ݒ�
			SetUndoMax(n22);											// Undo/Redo���Ұ��z�񐔐ݒ�
			SetCCtlMax(n23);											// �ő�Ȑ�����_���ݒ�
			SetSCtlMax(n24);											// �ő�Ȗʐ���_���ݒ�
			SetWCtlMax(n25);											// ��Ɨp����_�z�񐔐ݒ�
			SetCPntMax(n26);											// �ő�Ȑ����_���ݒ�
			SetSPntMax(n27);											// �ő�Ȗʒ��_���ݒ�
			SetOPntMax(n28);											// �ő嗧�̒��_���ݒ�
			SetBPntMax(n29);											// �ő�f�ʒ��_���ݒ�
			AfxMessageBox(IDS_CHANGE_MAX);								// ү����
			WriteMemDataFile();											// ��ؗ̈��ް��ۑ�̧�ُ���
		}
		if(sesht.GetCUpdate()) {										// <�\���F�ύX����>
			for(i=0; i<44; i++) {
				SetColor(i, sesht.GetColor(i));							// �\���F�ύX
			}
			View->ResetColor();											// ��ذݶװ�Đݒ�
		}
	}
	View->InitChar();													// �����͏�����
	SetCrtPrm(rep, wgt);												// �������Ұ��ݒ�
}

/**********************************/
void CMainFrame::OnSetRender(void)										//<<< ������ݸފ��ݒ裖t����
/**********************************/
{
	CString str;

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_SET_RENDER);									// �O�ƭ��I��
	View->SaveRender();													// �����ݸޏ��ۑ�
	View->StartRenderTest();											// �����ݸ�ýĊJ�n����
	str.LoadString(IDS_STR18);
	CRndEnvSheet  resht(str);
	if(resht.DoModal()!=IDOK) {											// �����ݸފ��ݒ��޲�۸�
		View->RestoreRender();											// �����ݸޏ�񕜌�
	}
	View->EndRenderTest();												// �����ݸ�ýďI������
	View->InitChar();													// �����͏�����
}

/************************************/
void CMainFrame::OnSetRayTrace(void)									//<<< �ڲ�ڰ�ݸފ��ݒ裖t����
/************************************/
{
	CString str;

	View->InitializeMenu();												// �ƭ���������
	View->ExitActMenu(ID_SET_RAYTRACE);									// �O�ƭ��I��
	str.LoadString(IDS_STR20);
	CRayEnvSheet  resht(str);
	if(resht.DoModal()==IDOK) {											// �����ݸފ��ݒ��޲�۸�
		resht.SetRayTraceData();										// ڲ��ڰ��ް��ݒ�
	}
	View->InitChar();													// �����͏�����
}

/**********************************/
void CMainFrame::OnRefManual(void)										//<<< ��ƭ�َQ�ƣ�t����
/**********************************/
{
	ShellExecute(NULL, "open", "http://jcad3.net/jcadmanual.html", NULL, NULL, SW_SHOWNORMAL);
}

/***********************************/
void CMainFrame::OnRegistUser(void)										//<<< �հ�ް�o�^��t����
/***********************************/
{
	ShellExecute(NULL, "open", "http://www.jcad3.net/USERREGI/userregi.html", NULL, NULL, SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �I�y���[�V����

/*********************************/
static BOOL FindCopyFile(int chr)										//<<< ��߰�ް�̧�ٌ���
/*********************************/
{
	WIN32_FIND_DATA wfd;
	CString path;

	if(chr==0) {														// <Key�����Ȃ�>
		path = GetAppPath(0)+COP_NAME1;									// ��̧߰��PATH�擾
	} else {															// <Key��������>
		path.Format(COP_NAME2, chr);									// Key�t��̧߰��
		path = GetAppPath(0)+path;										// ��̧߰��PATH�擾
	}
	if(FindFirstFile(path, &wfd)==INVALID_HANDLE_VALUE) {				// ̧�ٖ��̏ꍇ
		return FALSE;
	} else {
		return TRUE;
	}
}

/*********************************/
void CMainFrame::Initialize(void)										//<<< ������
/*********************************/
{
	int i;

	View->SetSaveMenu(NONE);
	View->ExitActMenu(View->GetActMenu());
	for(i=0; i<32; i++) {
		m_GroupObj[i] = 0;												// ��ٰ�ߕ\���S���o�^
		m_GroupDisp[i] = 1;												// ��ٰ�ߕ\���S�\��
	}
	m_wndEdit.GetToolBarCtrl().CheckButton(ID_NUM_INPUT, FALSE);		// ���l���͖tOFF
	m_bViewSave = FALSE;												// �ޭ�ܕۑ��׸ޏ�����
	m_CopyCmdFlg = 0;
	if(FindCopyFile(0)) {												// ��߰�ް�̧�ٌ���
		m_CopyCmdFlg = 1;												// ��߰�ް�̧�ق���
		return;
	}
	for(i=(int)'0'; i<=(int)'9'; i++) {									// '0'�`'9'������
		if(FindCopyFile(i)) {											// ��߰�ް�̧�ٌ���
			m_CopyCmdFlg = 1;											// ��߰�ް�̧�ق���
			return;
		}
	}
	for(i=(int)'A'; i<=(int)'Z'; i++) {									// 'A'�`'Z'������
		if(FindCopyFile(i)) {											// ��߰�ް�̧�ٌ���
			m_CopyCmdFlg = 1;											// ��߰�ް�̧�ق���
			return;
		}
	}

//202006 del GroupBar
	ShowControlBar(&m_wndGroup, FALSE, TRUE);// ���۰��ް��\�� //202006 Add
//	ShowControlBar(&m_wndView, FALSE, TRUE);// ���۰��ް��\�� //202006 Add


}

/*************************************/
void CMainFrame::InitSColorFile(void)									//<<< �������Ѷװ̧�ٓǍ�
/*************************************/
{
	char*   buf = new char[100];
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCN_NAME;										// JCAD3�װ�ݒ�̧�ٖ�(�V��)
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		for(i=0; i<44; i++) {
			if(fscanf_s(fp, "%s%d%d%d", buf, 100, &r, &g, &b)!=4) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// �ُ��ް��͒��~
			}
			m_Color[i] = RGB(r, g, b);                                  // �װ�̏�������
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ��ް�̧�ق�Ǎ�NG�
	}
	delete[] buf;
}

/*************************************/
void CMainFrame::ReadSColorFile(void)									//<<< ���Ѷװ�ۑ�̧�ٓǍ�
/*************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCS_NAME;										// JCAD3�װ�ݒ�ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		for(i=0; i<44; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// �ُ��ް��͒��~
			}
			m_Color[i] = RGB(r, g, b);									// �װ�̏�������
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	}
}

/**************************************/
void CMainFrame::WriteSColorFile(void)									//<<< ���Ѷװ�ۑ�̧�ُ���
/**************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCS_NAME;										// JCAD3�װ�ݒ�ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		for(i=0; i<44; i++) {
			r = GetRValue(m_Color[i]);
			g = GetGValue(m_Color[i]);
			b = GetBValue(m_Color[i]);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
}

/**************************************/
void CMainFrame::ReadFilPathFile(void)									//<<< ̧���߽�ۑ�̧�ٓǍ�
/**************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;

	path = GetAppPath(0)+FPS_NAME;										// ̧���߽�ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		while(TRUE) {
			memset(spath, NULL, _MAX_PATH);

			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if(strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				m_sFilePath.Format("%s", lpath);
				if(m_sFilePath=="") {
					m_sFilePath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_OpenKind, &m_SaveKind,
                                      &m_ViewerKind)!=3) {				// ̧�ٓ��o�͎�ގ擾
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d%d%d", &i0, &i1, &i2, &i3, &i4)!=5) {// �����߽ܰ���ް��擾
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d%d%d", &i5, &i6, &i7, &i8, &i9)!=5) {
				er = 1; break;
			}
			memset(m_cWPassword, NULL, sizeof(m_cWPassword));
			m_cWPassword[0] = (char)i4;									// �����߽ܰ���ް��ϊ�
			m_cWPassword[1] = (char)i7;
			m_cWPassword[2] = (char)i9;
			m_cWPassword[3] = (char)i1;
			m_cWPassword[4] = (char)i8;
			m_cWPassword[5] = (char)i2;
			m_cWPassword[6] = (char)i5;
			m_cWPassword[7] = (char)i0;
			m_cWPassword[8] = (char)i3;
			m_cWPassword[9] = (char)i6;
			if(fscanf_s(fp, "%d%d%d%d%d", &i0, &i1, &i2, &i3, &i4)!=5) {// �Ǎ��߽ܰ���ް��擾
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d%d%d", &i5, &i6, &i7, &i8, &i9)!=5) {
				er = 1; break;
			}
			memset(m_cRPassword, NULL, sizeof(m_cRPassword));
			m_cRPassword[0] = (char)i9;									// �Ǎ��߽ܰ���ް��ϊ�
			m_cRPassword[1] = (char)i3;
			m_cRPassword[2] = (char)i1;
			m_cRPassword[3] = (char)i6;
			m_cRPassword[4] = (char)i2;
			m_cRPassword[5] = (char)i8;
			m_cRPassword[6] = (char)i0;
			m_cRPassword[7] = (char)i7;
			m_cRPassword[8] = (char)i4;
			m_cRPassword[9] = (char)i5;
			if(fscanf_s(fp, "%d", &m_PasswordKind)!=1) {				// �߽ܰ�ގ�ގ擾
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	}
	delete[] spath;
	delete[] lpath;
}

/***************************************/
void CMainFrame::WriteFilPathFile(void)									//<<< ̧���߽�ۑ�̧�ُ���
/***************************************/
{
	char*   spath= new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;

	path = GetAppPath(0)+FPS_NAME;										// ̧���߽�ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		while(TRUE) {
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sFilePath, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_OpenKind, m_SaveKind,
                                         m_ViewerKind)<0) {				// ̧�ٓ��o�͎�ސݒ�
				er = 1; break;
			}
			i4 = (int)m_cWPassword[0];									// �����߽ܰ���ް��ϊ�
			i7 = (int)m_cWPassword[1];
			i9 = (int)m_cWPassword[2];
			i1 = (int)m_cWPassword[3];
			i8 = (int)m_cWPassword[4];
			i2 = (int)m_cWPassword[5];
			i5 = (int)m_cWPassword[6];
			i0 = (int)m_cWPassword[7];
			i3 = (int)m_cWPassword[8];
			i6 = (int)m_cWPassword[9];
			if(fprintf(fp, "%d %d %d %d %d\n", i0, i1, i2, i3, i4)<0) {	// �����߽ܰ���ް��ݒ�
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d %d %d\n", i5, i6, i7, i8, i9)<0) {
				er = 1; break;
			}
			i9 = (int)m_cRPassword[0];									// �Ǎ��߽ܰ���ް��ϊ�
			i3 = (int)m_cRPassword[1];
			i1 = (int)m_cRPassword[2];
			i6 = (int)m_cRPassword[3];
			i2 = (int)m_cRPassword[4];
			i8 = (int)m_cRPassword[5];
			i0 = (int)m_cRPassword[6];
			i7 = (int)m_cRPassword[7];
			i4 = (int)m_cRPassword[8];
			i5 = (int)m_cRPassword[9];
			if(fprintf(fp, "%d %d %d %d %d\n", i0, i1, i2, i3, i4)<0) {	// �Ǎ��߽ܰ���ް��ݒ�
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d %d %d\n", i5, i6, i7, i8, i9)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d\n", m_PasswordKind)<0) {					// �߽ܰ�ގ�ސݒ�
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
	delete[] spath;
}

/*******************************************************************/
void CMainFrame::GetPasswordData(char* wpwd, char* rpwd, int* save)		//<<< �߽ܰ���ް��擾
/*******************************************************************/
{
	memcpy(wpwd, m_cWPassword, sizeof(m_cWPassword));					// �����߽ܰ���ް�����
	memcpy(rpwd, m_cRPassword, sizeof(m_cRPassword));					// �Ǎ��߽ܰ���ް�����
	*save = m_PasswordSave;												// �߽ܰ�ޕۑ��׸�
}

/********************************/
void CMainFrame::InitColor(void)										//<<< �\���F������
/********************************/
{
	ReadSColorFile();													// ���Ѷװ�ۑ�̧�ٓǍ�
	m_Color[44] = RGB(0, 0, 0);											// ���F(�Œ�)
	m_Color[45] = RGB(255, 255, 255);									// ���F(�Œ�)
	BackColor     = 0;													// �w�i�FNo
	ObjColor      = 1;													// ���̕\���F(��ٰ�ߖ�)No
	GroupColor1   = 2;													// ��ٰ��1�\���FNo
	GroupColor2   = 3;													// ��ٰ��2�\���FNo
	GroupColor3   = 4;													// ��ٰ��3�\���FNo
	GroupColor4   = 5;													// ��ٰ��4�\���FNo
	GroupColor5   = 6;													// ��ٰ��5�\���FNo
	GroupColor6   = 7;													// ��ٰ��6�\���FNo
	GroupColor7   = 8;													// ��ٰ��7�\���FNo
	GroupColor8   = 9;													// ��ٰ��8�\���FNo
	GroupColor9   = 10;													// ��ٰ��9�\���FNo
	GroupColor10  = 11;													// ��ٰ��10�\���FNo
	GroupColor11  = 12;													// ��ٰ��11�\���FNo
	GroupColor12  = 13;													// ��ٰ��12�\���FNo
	GroupColor13  = 14;													// ��ٰ��13�\���FNo
	GroupColor14  = 15;													// ��ٰ��14�\���FNo
	GroupColor15  = 16;													// ��ٰ��15�\���FNo
	GroupColor16  = 17;													// ��ٰ��16�\���FNo
	SelectColor   = 18;													// �I�𗧑̕\���FNo
	ChooseColor   = 19;													// �I���������̕\���FNo
	BoolColor     = 20;													// �}�`���Z�\���FNo
	GridColor     = 21;													// ��د�ޕ\���FNo
	GridBPColor   = 22;													// ��د�ފ�_�\���FNo
	TemplateColor = 23;													// ����ڰĕ\���FNo
	BaseColor     = 24;													// ��_�\���FNo
	BoxColor      = 25;													// �ޯ���\���FNo
	CircleColor   = 26;													// ���ٕ\���FNo
	LineColor     = 27;													// ײݕ\���FNo
	MeasureColor  = 28;													// ��K�\���FNo
	SelLineColor  = 29;													// �I�𐧌���\���FNo
	XGuideColor   = 30;													// X�޲�ޕ\���FNo
	YGuideColor   = 31;													// Y�޲�ޕ\���FNo
	ZGuideColor   = 32;													// Z�޲�ޕ\���FNo
	NCDispColor1  = 33;													// NC1�\���FNo
	NCDispColor2  = 34;													// NC2�\���FNo
	TrackerColor0 = 35;													// �ׯ��0�\���FNo
	TrackerColor1 = 36;													// �ׯ��1�\���FNo
	TrackerColor2 = 37;													// �ׯ��2�\���FNo
	TrackerColor3 = 38;													// �ׯ��3�\���FNo
	TrackerColor4 = 39;													// �ׯ��4�\���FNo
	TrackerColor5 = 40;													// �ׯ��5�\���FNo
	BorderColor   = 41;													// �ڰѐF(���S���E��)
	BorderHColor  = 42;													// �ڰѐF(�O�g���E��[������])
	BorderSColor  = 43;													// �ڰѐF(�O�g���E��[���e��])
	BlackColor    = 44;													// ���FNo
	WhiteColor    = 45;													// ���FNo
}

/****************************************/
void CMainFrame::SaveObjColor(UINT *col)								//<<< ���̕\���F�ۑ�
/****************************************/
{
	col[0]  = ObjColor;													// ���̕\���F(��ٰ�ߖ�)No
	col[1]  = GroupColor1;												// ��ٰ��1�\���FNo
	col[2]  = GroupColor2;												// ��ٰ��2�\���FNo
	col[3]  = GroupColor3;												// ��ٰ��3�\���FNo
	col[4]  = GroupColor4;												// ��ٰ��4�\���FNo
	col[5]  = GroupColor5;												// ��ٰ��5�\���FNo
	col[6]  = GroupColor6;												// ��ٰ��6�\���FNo
	col[7]  = GroupColor7;												// ��ٰ��7�\���FNo
	col[8]  = GroupColor8;												// ��ٰ��8�\���FNo
	col[9]  = GroupColor9;												// ��ٰ��9�\���FNo
	col[10] = GroupColor10;												// ��ٰ��10�\���FNo
	col[11] = GroupColor11;												// ��ٰ��11�\���FNo
	col[12] = GroupColor12;												// ��ٰ��12�\���FNo
	col[13] = GroupColor13;												// ��ٰ��13�\���FNo
	col[14] = GroupColor14;												// ��ٰ��14�\���FNo
	col[15] = GroupColor15;												// ��ٰ��15�\���FNo
	col[16] = GroupColor16;												// ��ٰ��16�\���FNo
	col[17] = SelectColor;												// �I�𗧑̕\���FNo
	col[18] = ChooseColor;												// �I���������̕\���FNo
	col[19] = BoolColor;												// �}�`���Z�\���FNo
}

/*******************************************/
void CMainFrame::RestoreObjColor(UINT *col)								//<<< ���̕\���F����
/*******************************************/
{
	ObjColor     = col[0];												// ���̕\���F(��ٰ�ߖ�)No
	GroupColor1  = col[1];												// ��ٰ��1�\���FNo
	GroupColor2  = col[2];												// ��ٰ��2�\���FNo
	GroupColor3  = col[3];												// ��ٰ��3�\���FNo
	GroupColor4  = col[4];												// ��ٰ��4�\���FNo
	GroupColor5  = col[5];												// ��ٰ��5�\���FNo
	GroupColor6  = col[6];												// ��ٰ��6�\���FNo
	GroupColor7  = col[7];												// ��ٰ��7�\���FNo
	GroupColor8  = col[8];												// ��ٰ��8�\���FNo
	GroupColor9  = col[9];												// ��ٰ��9�\���FNo
	GroupColor10 = col[10];												// ��ٰ��10�\���FNo
	GroupColor11 = col[11];												// ��ٰ��11�\���FNo
	GroupColor12 = col[12];												// ��ٰ��12�\���FNo
	GroupColor13 = col[13];												// ��ٰ��13�\���FNo
	GroupColor14 = col[14];												// ��ٰ��14�\���FNo
	GroupColor15 = col[15];												// ��ٰ��15�\���FNo
	GroupColor16 = col[16];												// ��ٰ��16�\���FNo
	SelectColor  = col[17];												// �I�𗧑̕\���FNo
	ChooseColor  = col[18];												// �I���������̕\���FNo
	BoolColor    = col[19];												// �}�`���Z�\���FNo
}

/*****************************************/
void CMainFrame::ChangeObjColor(UINT col)								//<<< ���̕\���F�ύX
/*****************************************/
{
	ObjColor     = col;													// ���̕\���F(��ٰ�ߖ�)No
	GroupColor1  = col;													// ��ٰ��1�\���FNo
	GroupColor2  = col;													// ��ٰ��2�\���FNo
	GroupColor3  = col;													// ��ٰ��3�\���FNo
	GroupColor4  = col;													// ��ٰ��4�\���FNo
	GroupColor5  = col;													// ��ٰ��5�\���FNo
	GroupColor6  = col;													// ��ٰ��6�\���FNo
	GroupColor7  = col;													// ��ٰ��7�\���FNo
	GroupColor8  = col;													// ��ٰ��8�\���FNo
	GroupColor9  = col;													// ��ٰ��9�\���FNo
	GroupColor10 = col;													// ��ٰ��10�\���FNo
	GroupColor11 = col;													// ��ٰ��11�\���FNo
	GroupColor12 = col;													// ��ٰ��12�\���FNo
	GroupColor13 = col;													// ��ٰ��13�\���FNo
	GroupColor14 = col;													// ��ٰ��14�\���FNo
	GroupColor15 = col;													// ��ٰ��15�\���FNo
	GroupColor16 = col;													// ��ٰ��16�\���FNo
	SelectColor  = col;													// �I�𗧑̕\���FNo
	ChooseColor  = col;													// �I���������̕\���FNo
	BoolColor    = col;													// �}�`���Z�\���FNo
}

/******************************************************************/
void CMainFrame::GetColor(int no, double* r, double* g, double* b)		//<<< �\���F�擾
/******************************************************************/
{
	*r = GetRValue(m_Color[no])/255.0;
	*g = GetGValue(m_Color[no])/255.0;
	*b = GetBValue(m_Color[no])/255.0;
}

/**************************************/
void CMainFrame::ExecuteMenu(UINT nID)									//<<< ̧ݸ��ݑI���ƭ��N��
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
void CMainFrame::SetGroupObj(void)										//<<< ��ٰ�ߗ��̐ݒ�
/**********************************/
{
	OBJTYPE* op;
	int      i;

	for(i=0; i<32; i++) {
		GroupBaseObjPtr(i, &op);										// ��ٰ�ߗ����߲���擾
		m_GroupObj[i] = (op!=NULL) ? 1 : 0;								// ��ٰ�ߗ��̗L��
	}
}

/********************************************************/
void CMainFrame::SetCurPos(double x, double y, double z)				//<<< ���وʒu�ݒ�
/********************************************************/
{
	m_XPos = x, m_YPos = y, m_ZPos = z;
}

/*******************************************/
void CMainFrame::SetCrtPrm(int d, double w)								//<<< �������Ұ��ݒ�
/*******************************************/
{
	m_CrtD = d, m_CrtW = w;
}

/***********************************************************/
static CString GetsFileFilter(int mode, int kind, int* flg)				//<<< ̧��̨����擾
/***********************************************************/
{
	CString str;

	*flg = 0;
	if(mode==0) {														// <Open>
		str.LoadString(IDS_FILE_FILTER13);								// DJF //202006 �ǉ�
		//if (kind == 0) {
		//	str.LoadString(IDS_FILE_FILTER13);							// DJF //202006 �ǉ�
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
			str.LoadString(IDS_FILE_FILTER13);							// DJF //202006 �ǉ��ύX
		}
#ifdef STL_EXPORT	//202006 STL export 
		else {
			str.LoadString(IDS_FILE_FILTER7);							// STL
		}
#endif
		//if(kind==0) {
		//	str.LoadString(IDS_FILE_FILTER4);							// JGD(�߽ܰ�ޖ�)
		//} else if(kind==1) {
		//	str.LoadString(IDS_FILE_FILTER5);							// JGD(�߽ܰ�ޗL)
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
			str.LoadString(IDS_FILE_FILTER11);							// GGD(�߽ܰ�ޖ�)
		} else if(kind==1) {
			str.LoadString(IDS_FILE_FILTER12);							// GGD(�߽ܰ�ޗL)
			*flg = 1;
		} else if(kind==2) {
			str.LoadString(IDS_FILE_FILTER9);							// GLF
		} else if(kind==3) {
			str.LoadString(IDS_FILE_FILTER8);							// GLD
		} else if(kind==4) {
			str.LoadString(IDS_FILE_FILTER4);							// JGD(�߽ܰ�ޖ�)
		} else if(kind==5) {
			str.LoadString(IDS_FILE_FILTER5);							// JGD(�߽ܰ�ޗL)
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
static int GetsFileKind(int mode, CString ext)							//<<< ̧�َ�ގ擾
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
BOOL CMainFrame::DoPromptFName(CString& fname, CString& ext, BOOL bopen)//<<< ̧�ٖ��w���޲�۸ސݒ�
/************************************************************************/
{
	CString filter, wex;
	DWORD   flg;
	BOOL    ret=FALSE;
	int     i, chk=0, cnt=1, pf, pwd[2];

	if(!CheckFilePath(0, m_sFilePath)) {								// �߽�m�F
		m_sFilePath = GetAppPath(0);
	}
	if(bopen) {															// <<�J��>>
		flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		m_OpenKind = 0;
		filter = GetsFileFilter(0, m_OpenKind, &pf);					// ̧��̨����擾
		//for(i=0; i<9; i++) {							//202006 8->9 -> 0�@djf�̂�
		//	if(m_OpenKind!=i) {
		//		filter += GetsFileFilter(0, i, &pf);					// ̧��̨����擾
		//	}
		//}
		filter += "|";
		chk = 1;
	} else {															// <<�ۑ�>>
		pwd[0] = -1, pwd[1] = -1;										// �߽ܰ�ޗLNo������
		flg = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;
		if(m_bViewSave) {						                		// <�ޭ�ܥ�ް��ۑ�����>
			filter = GetsFileFilter(2, m_ViewerKind, &pf);				// ̧��̨����擾
			pwd[0] = (pf==1) ? cnt : pwd[0];							// �߽ܰ�ޗL�͈ʒu�ۑ�
			pwd[1] = (pf==2) ? cnt : pwd[1];
			for(i=0; i<10; i++) {
				if(m_ViewerKind!=i) {
					filter += GetsFileFilter(2, i, &pf);				// ̧��̨����擾
					cnt++;
					pwd[0] = (pf==1) ? cnt : pwd[0];					// �߽ܰ�ޗL�͈ʒu�ۑ�
					pwd[1] = (pf==2) ? cnt : pwd[1];
				}
			}
			filter += "|";
			chk = 1;
		} else {														// <�ޭ�ܥ�ް��ۑ��Ȃ�>
			m_OpenKind = 0;
			filter = GetsFileFilter(1, m_SaveKind, &pf);				// ̧��̨����擾
			pwd[0] = (pf==1) ? cnt : pwd[0];							// �߽ܰ�ޗL�͈ʒu�ۑ�
			
#ifdef STL_EXPORT	//202006 STL export�ł̂ݗL�� FileFilter(1)
			for (i = 0; i < 2; i++) {							//202006 6->2 or 0(djf�̂݁j
				if (m_SaveKind != i) {
					filter += GetsFileFilter(1, i, &pf);				// ̧��̨����擾
					cnt++;
					pwd[0] = (pf == 1) ? cnt : pwd[0];					// �߽ܰ�ޗL�͈ʒu�ۑ�
				}
			}
#endif
			filter += "|";
		}
	}
	wex = fname.Right(4);
	if(chk) {
		if(!wex.CompareNoCase(".djf")||!wex.CompareNoCase(".stl"))		//202006 djf �ǉ�
		//   !wex.CompareNoCase(".jsd")||!wex.CompareNoCase(".gld")||
        //   !wex.CompareNoCase(".jsf")||!wex.CompareNoCase(".glf")||
        //   !wex.CompareNoCase(".jgd")||!wex.CompareNoCase(".ggd")||
        //   !wex.CompareNoCase(".dxf")||
		//   !wex.CompareNoCase(".stl")) 
		{																// <�L���Ȋg���q> 
			i = fname.ReverseFind('.');
			fname = fname.Left(i);										// �g���q���폜
		}
	} else {
		if (!wex.CompareNoCase(".djf")||!wex.CompareNoCase(".stl"))    //202006 djf �ǉ�
		//if(!wex.CompareNoCase(".jsd")||
        //   !wex.CompareNoCase(".jsf")||
        //   !wex.CompareNoCase(".jgd")||
        //   !wex.CompareNoCase(".dxf")||
		//   !wex.CompareNoCase(".stl")) 
		{	// <�L���Ȋg���q>    //202006 djf �ύX�ǉ�
			i = fname.ReverseFind('.');
			fname = fname.Left(i);										// �g���q���폜
		}
	}
	CFileDlg dlg(bopen, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sFilePath;
	if(dlg.DoModal()==IDOK) {
		ext = dlg.GetFileExt();											// ̧�يg���q�擾
		if(bopen) {														// <<�J��>>
			m_OpenKind = GetsFileKind(0, ext);							// ̧�َ�ގ擾
		} else {														// <<�ۑ�>>
			i = (int)dlg.m_ofn.nFilterIndex;							// ̧�َ�ނ̑I���ʒu(1�`)
			if(pwd[0]==i||pwd[1]==i) {									// <JGD/GGD���߽ܰ�ޗL�I��>
				m_PasswordSave = 1;										// �߽ܰ�ޕۑ�:�L
			} else {													// <��L�ȊO��I��>
				m_PasswordSave = 0;										// �߽ܰ�ޕۑ�:��
			}
			if(m_bViewSave) {					                		// <�ޭ�ܥ�ް��ۑ�����>
				m_ViewerKind = GetsFileKind(2, ext) + m_PasswordSave;	// ̧�َ�ގ擾
			} else {													// <�ޭ�ܥ�ް��ۑ��Ȃ�>
				m_SaveKind = GetsFileKind(1, ext) + m_PasswordSave;		// ̧�َ�ގ擾
			}
			if (m_SaveKind < 0) {										//202006 file kind check
				m_SaveKind = 0;

			}
		}
		fname = dlg.GetPathName();										// ̧�ٖ�(���߽)�擾
		i = fname.ReverseFind('\\');
		m_sFilePath = fname.Left(i+1);									// �߽�ۑ�
		ret = TRUE;
	}
	return ret;
}

/*************************************************/
BOOL CMainFrame::DoOpenFile(const CString& fname)						//<<< ̧��Open
/*************************************************/
{
	CWaitCursor wait;													// ���ĥ���ٕ\��
	CString  ext, str;
	OBJTYPE* op;
	int      dsp, edt, ret=1, uflg=1;

	if(GetInitFlg()) {													// <�ʏ�N�����>
		BaseObjPtr(0, &op, &dsp, &edt);									// �擪�����߲���擾
		if(op!=NULL) {
			if(AfxMessageBox(IDS_ADD_OBJECT, MB_YESNO)==IDNO) {			// �ǉ��m�Fү����
				OnFileNew();											// ��V�K�쐬�
			} else {													// <�ǉ��̏ꍇ>
				if(AfxMessageBox(IDS_KEEP_UNDOREDO, MB_YESNO)==IDNO) {	// �ǉ����̗̂���ێ��m�Fү����
					uflg = 0;											// ��Undo/Redo�Ȃ�
				}
			}
		}
		StartMenu();													// �ƭ��J�n����
		ext = fname.Right(3);
		if((ext.CompareNoCase("jgd")&&ext.CompareNoCase("ggd")
			&& ext.CompareNoCase("djf")) ||		//202006 djf �ǉ�		// <JGD/GGD�`���ȊO,
           !uflg) {														// ���͌�Undo/Redo�Ȃ�>
			InitLevel();												// Undo/Redo������
		}
		if (!ext.CompareNoCase("djf")) {
			ret = ReadJgdFile(fname, 0, uflg);		//202006 DJF �ǉ� 		// DJF�`��̧�ٓ���
		} 
		//if (!ext.CompareNoCase("jsd")) {
		//	ret = ReadJsdFile(fname, 0, 1);								// JSD�`��̧�ٓ���
		//} else if(!ext.CompareNoCase("jsf")) {
		//	ret = ReadJsdFile(fname, 0, 1);								// JSF�`��̧�ٓ���
		//} else if(!ext.CompareNoCase("gld")) {
		//	ret = ReadJsdFile(fname, 1, 1);						    	// GLD�`��̧�ٓ���
		//} else if(!ext.CompareNoCase("glf")) {
		//	ret = ReadJsdFile(fname, 1, 1);					    		// GLF�`��̧�ٓ���
		//} else if(!ext.CompareNoCase("jgd")) {
		//	ret = ReadJgdFile(fname, 0, uflg);							// JGD�`��̧�ٓ���
		//} else if(!ext.CompareNoCase("ggd")) {
		//	ret = ReadJgdFile(fname, 1, uflg);					    	// GGD�`��̧�ٓ���
		//} else if(!ext.CompareNoCase("dxf")) {
		//	ret = ReadCommonFile(0, fname);								// DXF�`��̧�ٓ���
		//} else if(!ext.CompareNoCase("stl")) {
		//	ret = ReadCommonFile(1, fname);								// STL�`��̧�ٓ���
		//}
		if(!ret) {														// <����>
			if((ext.CompareNoCase("jgd")&&ext.CompareNoCase("ggd")
			   && ext.CompareNoCase("djf"))||	//202006 DJF �ǉ�		// <JGD/GGD�`���ȊO,
               !uflg) {													// ���͌�Undo/Redo�Ȃ�>
				FileBackUp1();											// ̧���ޯ����ߏ���1
			}
			if(!GetFileFlg()) {											// <̧�َw��N���ȊO>
				GetActiveDocument()->SetPathName(fname, TRUE);			// �޷����̧�ٖ��ݒ�&
			}															// MRU̧��ؽĒǉ�
			if(GetDemoFlg()!=0) {										// <�̌��ł̏ꍇ>
				GetWindowText(ext);										// ����޳���َ擾
				str.LoadString(IDS_STR45);								// ��̌��ţ
				ext += str;
				SetWindowText(ext);										// ����޳���ٕύX
			}
		} else {														// <�ُ�/���~>
			if ((ext.CompareNoCase("jgd") && ext.CompareNoCase("ggd")
			   && ext.CompareNoCase("djf")) ||	//202006 DJF �ǉ�		// <JGD/GGD�`���ȊO,
			   !uflg) {													// ���͌�Undo/Redo�Ȃ�>
				BackLevel();											// Undo/Redo�߂�
			}
			if(ret==1) {
				SetSplashMsg(IDS_ERR_FILEREAD);							// ��Ǎ�NG�
			} else if(ret==2) {
				SetSplashMsg(IDS_ERR_PASSWORD);							// ��߽ܰ�ޕs��v�
			} else {
				SetSplashMsg(IDS_STOP_PROCESS);							// ��������~�
			}
			return FALSE;
		}
		SetFileFlg(0);													// ̧�ٕۑ��׸ސݒ�
		SetGroupObj();													// ��ٰ�ߗ��̐ݒ�
		View->Display(0);												// �ĕ`��
	} else {															// <̧�ًN�����>
		SetFileFlg(1);													// ̧�ٕۑ��׸ސݒ�
		SetFileName(fname);												// �ۑ�̧�ِݒ�
	}
	return TRUE;
}

/***************************************/
BOOL CMainFrame::InitStatusBar(int sec)									//<<< �ð���ް�쐬&������
/***************************************/
{
	UINT style;

	if(!m_wndStatusBar.Create(this)||									// �ð���ް�쐬
       !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))) {
		TRACE0("Failed to create status bar\n");
		return FALSE;													// �쐬���s
	}
	style = m_wndStatusBar.GetPaneStyle(0)&~SBPS_NOBORDERS;
	m_wndStatusBar.SetPaneStyle(0, style);
	setlocale(LC_ALL, "");
	m_wndStatusBar.SetTimer(0x1000, sec*1000, NULL);					// ����ْ������߲ݍX�V���Ԑݒ�
	return TRUE;
}

/******************************************************************/
BOOL CMainFrame::InitToolBar(CToolBar* pBar, UINT nIDR,					//<<< °��ް�쐬&������
                             UINT titleID, DWORD style, UINT nIDW)
/******************************************************************/
{
	CString str("");

	str.LoadString(titleID);
	style |= WS_CHILD|WS_VISIBLE|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC|TBSTYLE_WRAPABLE;
	if(!pBar->Create(this, style, nIDW)||!pBar->LoadToolBar(nIDR)) {	// °��ް�쐬
		TRACE1("Failed to create %s toolbar\n", str);
		return FALSE;													// �쐬���s
	}
	if(titleID!=0) {
		pBar->SetWindowText(str);										// ����޳���ِݒ�
	}
	pBar->EnableDocking(CBRS_ALIGN_ANY);								// °��ް�ޯ�ݸމ�
	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}

/*********************************************************************/
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)	//<<< °��ް�z�u
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
int CMainFrame::GetGroupKey(void)										//<<< ��ٰ�ߑΉ����擾
/*********************************/
{
	int kno=0;

	if(View->GetChar()=='1') {											// [1]�������͸�ٰ��1
		kno = 1;
	} else if(View->GetChar()=='2') {									// [2]�������͸�ٰ��2
		kno = 2;
	} else if(View->GetChar()=='3') {									// [3]�������͸�ٰ��3
		kno = 3;
	} else if(View->GetChar()=='4') {									// [4]�������͸�ٰ��4
		kno = 4;
	} else if(View->GetChar()=='5') {									// [5]�������͸�ٰ��5
		kno = 5;
	} else if(View->GetChar()=='6') {									// [6]�������͸�ٰ��6
		kno = 6;
	} else if(View->GetChar()=='7') {									// [7]�������͸�ٰ��7
		kno = 7;
	} else if(View->GetChar()=='8') {									// [8]�������͸�ٰ��8
		kno = 8;
	} else if(View->GetChar()=='9') {									// [9]�������͸�ٰ��9
		kno = 9;
	} else if(View->GetChar()=='0') {									// [0]�������͸�ٰ��10
		kno = 10;
	} else if(View->GetChar()=='Q') {									// [Q]�������͸�ٰ��11
		kno = 11;
	} else if(View->GetChar()=='W') {									// [W]�������͸�ٰ��12
		kno = 12;
	} else if(View->GetChar()=='E') {									// [E]�������͸�ٰ��13
		kno = 13;
	} else if(View->GetChar()=='R') {									// [R]�������͸�ٰ��14
		kno = 14;
	} else if(View->GetChar()=='T') {									// [T]�������͸�ٰ��15
		kno = 15;
	} else if(View->GetChar()=='Y') {									// [Y]�������͸�ٰ��16
		kno = 16;
	} else if(View->GetChar()=='U') {									// [U]�������͸�ٰ��17
		kno = 17;
	} else if(View->GetChar()=='I') {									// [I]�������͸�ٰ��18
		kno = 18;
	} else if(View->GetChar()=='O') {									// [O]�������͸�ٰ��19
		kno = 19;
	} else if(View->GetChar()=='P') {									// [P]�������͸�ٰ��20
		kno = 20;
	} else if(View->GetChar()=='A') {									// [A]�������͸�ٰ��21
		kno = 21;
	} else if(View->GetChar()=='S') {									// [S]�������͸�ٰ��22
		kno = 22;
	} else if(View->GetChar()=='D') {									// [D]�������͸�ٰ��23
		kno = 23;
	} else if(View->GetChar()=='F') {									// [F]�������͸�ٰ��24
		kno = 24;
	} else if(View->GetChar()=='G') {									// [G]�������͸�ٰ��25
		kno = 25;
	} else if(View->GetChar()=='H') {									// [H]�������͸�ٰ��26
		kno = 26;
	} else if(View->GetChar()=='J') {									// [J]�������͸�ٰ��27
		kno = 27;
	} else if(View->GetChar()=='K') {									// [K]�������͸�ٰ��28
		kno = 28;
	} else if(View->GetChar()=='L') {									// [L]�������͸�ٰ��29
		kno = 29;
	} else if(View->GetChar()=='Z') {									// [Z]�������͸�ٰ��30
		kno = 30;
	} else if(View->GetChar()=='X') {									// [X]�������͸�ٰ��31
		kno = 31;
	} else if(View->GetChar()=='C') {									// [C]�������͸�ٰ��32
		kno = 32;
	}
	return kno;
}
