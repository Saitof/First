/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
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
	// �W������R�}���h
//	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView �R���X�g���N�V����/�f�X�g���N�V����

/**********************************/
CJcad3GlbView::CJcad3GlbView(void)										//<<< �׽�\�z
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
	m_CColor[18][0] = RGB(63, 47, 47);									// �I�𗧑̼���ިݸސF
	m_CColor[18][1] = RGB(63, 63, 63);
	m_CColor[18][2] = RGB(31, 31, 31);
	m_Shin[18] = 8;
	m_CColor[19][0] = RGB(47, 63, 47);									// �I����������ިݸސF
	m_CColor[19][1] = RGB(63, 63, 63);
	m_CColor[19][2] = RGB(31, 31, 31);
	m_Shin[19] = 8;
	m_CColor[20][0] = RGB(47, 47, 63);									// �}�`���Z����ިݸސF
	m_CColor[20][1] = RGB(63, 63, 63);
	m_CColor[20][2] = RGB(31, 31, 31);
	m_Shin[20] = 8;
	for(i=0; i<3; i++) {
		m_TexImage[i] = NULL, m_TexWidth[i] = 0, m_TexHeight[i] = 0;
	}
	InitRenderFile(0, FALSE);											// ���������ݸ�̧�ٓǍ�
	InitRColorFile(FALSE);												// ���������ݸ޶װ̧�ٓǍ�
//202006	InitRayTraceEnvFile();												// ����ڲ��ڰ���̧�ٓǍ�
//202006	InitRayTraceLightFile();											// ����ڲ��ڰ�����̧�ٓǍ�
//202006	InitRayTraceColorFile();											// ����ڲ��ڰ��װ̧�ٓǍ�
	InitWorkTable();													// ���ð��ُ�������
	InitData();															// �ް���������
	InitDialog();														// �풓�޲�۸ޏ�����
	SetCtrlKey();														// ����L�[�ݒ�
}

/***********************************/
CJcad3GlbView::~CJcad3GlbView(void)										//<<< �׽����
/***********************************/
{
	int i;

	WriteRenderFile();													// �����ݸޕۑ�̧�ُ���
	WriteSysDataFile();													// �����ް��ۑ�̧�ُ���
	WriteScrInfoFile();													// ��ʐ�����ۑ�̧�ُ���
	for(i=0; i<3; i++) {
		if(m_TexDisp[i]) {
			ClearTexImage(i);											// ø����p�摜�ް�����
		}
	}
	WriteRayTraceFile();												// ڲ��ڰ��ۑ�̧�ُ���
	WritePovRayFile();													// POV-Ray�ۑ�̧�ُ���
	FreeWorkTable();													// ���ð��ى������
	if(m_pMainDC) {
		delete m_pMainDC;
	}
	m_pMainDC = NULL;
}

/*****************************************************/
BOOL CJcad3GlbView::PreCreateWindow(CREATESTRUCT& cs)					//<<< ����޳�쐬�O����
/*****************************************************/
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C�����C�����Ă��������B
	cs.lpszClass = AfxRegisterWndClass(0,								// ����ٸد��͎󂯎��Ȃ�
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_APPWORKSPACE),
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

/**************************************************/
BOOL CJcad3GlbView::PreTranslateMessage(MSG* pMsg)						//<<< ү���ޕϊ�
/**************************************************/
{
	int flg;

	if(pMsg->wParam>=VK_F5&&pMsg->wParam<=VK_F12) {
		if(pMsg->message==WM_KEYDOWN||pMsg->message==WM_SYSKEYDOWN) {	// ���޳݂̏ꍇ
			if(pMsg->wParam==VK_F5) {									// [F5]��
				if(!(m_FPres&0x1)) {									// ���񉟉���
					m_FChar = (m_FChar&0x1) ? m_FChar&~(0x1) : m_FChar|0x1;
					m_FPres = m_FPres|0x1;
					flg = (m_FChar&0x1) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(0, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(0, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			} else if(pMsg->wParam==VK_F6) {							// [F6]��
				if(!(m_FPres&0x2)) {									// ���񉟉���
					m_FChar = (m_FChar&0x2) ? m_FChar&~(0x2) : m_FChar|0x2;
					m_FPres = m_FPres|0x2;
					flg = (m_FChar&0x2) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(1, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(1, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			} else if(pMsg->wParam==VK_F7) {							// [F7]��
				if(!(m_FPres&0x4)) {									// ���񉟉���
					m_FChar = (m_FChar&0x4) ? m_FChar&~(0x4) : m_FChar|0x4;
					m_FPres = m_FPres|0x4;
					flg = (m_FChar&0x4) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(2, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(2, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			} else if(pMsg->wParam==VK_F8) {							// [F8]��
				if(!(m_FPres&0x8)) {									// ���񉟉���
					m_FChar = (m_FChar&0x8) ? m_FChar&~(0x8) : m_FChar|0x8;
					m_FPres = m_FPres|0x8;
					flg = (m_FChar&0x8) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(3, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(3, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			} else if(pMsg->wParam==VK_F9) {							// [F9]��
				if(!(m_FPres&0x10)) {									// ���񉟉���
					m_FChar = (m_FChar&0x10) ? m_FChar&~(0x10) : m_FChar|0x10;
					m_FPres = m_FPres|0x10;
					flg = (m_FChar&0x10) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(4, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(4, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			} else if(pMsg->wParam==VK_F10) {							// [F10]��
				if(!(m_FPres&0x20)) {									// ���񉟉���
					m_FChar = (m_FChar&0x20) ? m_FChar&~(0x20) : m_FChar|0x20;
					m_FPres = m_FPres|0x20;
					flg = (m_FChar&0x20) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(5, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(5, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			} else if(pMsg->wParam==VK_F11) {							// [F11]��
				if(!(m_FPres&0x40)) {									// ���񉟉���
					m_FChar = (m_FChar&0x40) ? m_FChar&~(0x40) : m_FChar|0x40;
					m_FPres = m_FPres|0x40;
					flg = (m_FChar&0x40) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(6, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(6, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			} else if(pMsg->wParam==VK_F12) {							// [F12]��
				if(!(m_FPres&0x80)) {									// ���񉟉���
					m_FChar = (m_FChar&0x80) ? m_FChar&~(0x80) : m_FChar|0x80;
					m_FPres = m_FPres|0x80;
					flg = (m_FChar&0x80) ? 1 : 0;
					if(m_SFuncMode<3) {									// <�ڍ�>
						m_pFunction->OnOffButton(7, flg);				// ̧ݸ��ݐݒ�°ٍX�V
					} else if(m_SFuncMode<6) {							// <�Ȉ�>
						m_pFunction2->OnOffButton(7, flg);				// ̧ݸ��ݐݒ�°�2�X�V
					}
				}
			}
		} else if(pMsg->message==WM_KEYUP||pMsg->message==WM_SYSKEYUP) {// �����߂̏ꍇ
			if(pMsg->wParam==VK_F5) {									// [F5]��
				m_FPres=m_FPres&~(0x1);
			} else if(pMsg->wParam==VK_F6) {							// [F6]��
				m_FPres=m_FPres&~(0x2);
			} else if(pMsg->wParam==VK_F7) {							// [F7]��
				m_FPres=m_FPres&~(0x4);
			} else if(pMsg->wParam==VK_F8) {							// [F8]��
				m_FPres=m_FPres&~(0x8);
			} else if(pMsg->wParam==VK_F9) {							// [F9]��
				m_FPres=m_FPres&~(0x10);
			} else if(pMsg->wParam==VK_F10) {							// [F10]��
				m_FPres=m_FPres&~(0x20);
			} else if(pMsg->wParam==VK_F11) {							// [F11]��
				m_FPres=m_FPres&~(0x40);
			} else if(pMsg->wParam==VK_F12) {							// [F12]��
				m_FPres=m_FPres&~(0x80);
			}
		}
		return TRUE;
	}
	return CView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView �`��

/****************************************/
void CJcad3GlbView::OnDraw(CDC* /*pDC*/)								//<<< View����޳�`��
/****************************************/
{
    Display(0);															// ��ʕ`��
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView ���

/********************************************************/
BOOL CJcad3GlbView::OnPreparePrinting(CPrintInfo* pInfo)				//<<< ����O����
/********************************************************/
{
	if(GetDemoFlg()!=0) {												// ��̌���,�N��NG�
		AfxMessageBox(IDS_TRIAL);
		return 0;
	}
	pInfo->m_bDirect = TRUE;											// �޲�۸ނȂ�
	pInfo->SetMaxPage(1);												// �ŏI�߰�ސݒ�
	DoPreparePrinting(pInfo);											// ��̫�Ĉ������
	LPDEVMODE prtmode = pInfo->m_pPD->GetDevMode();						// ��������擾
	int prtori = (int)prtmode->dmOrientation-1;							// ���݂̈���̌���
	if(!m_PrtKind) {													// <<4�ʐ}���>>
		CPrtParamDlg dlg;
		dlg.SetValue(m_PrtMode, m_PrtColor, m_PrtOut, prtori,
                     m_PrtPos, m_PrtSize, m_PrtPath);					// �����l�\��
		if(dlg.DoModal()==IDOK) {										// �����޲�۸ޕ\��
			dlg.GetValue(&m_PrtMode, &m_PrtColor, &m_PrtOut, &prtori,
                         &m_PrtPos, &m_PrtSize, &m_PrtPath,
                         &m_PrtTCheck, &m_PrtSCheck, &m_PrtFile);		// ����͈͓���
			Display(0);													// ��ʕ`��
			Sleep(100);
			Display(0);													// ��ʕ`��
			if(!m_PrtOut) {												// <������o�͂̏ꍇ>
				m_PrtSCheck = (m_PrtTCheck) ? 0 : m_PrtSCheck;			// ���ޕύX
				prtmode->dmOrientation = (short)(prtori+1);				// ����̌���
				return 1;
			} else {													// <̧�ُo�͂̏ꍇ>
				m_PrtTCheck = 0;										// ��������Ȃ�
				m_PrtSCheck = 0;										// ���ޕύX�Ȃ�
				DDBtoDIB(NULL);											// DDB=>DIB�ϊ�
				OutputFile();											// ̧�ُo��
				return 0;												// ����������~
			}
		} else {
			return 0;													// ���~=�Ԃ�l:0
		}
	} else {															// <<�����ݸވ��>>
		CPrtParam2Dlg dlg;
		dlg.SetValue(m_PrtOut, prtori, m_PrtPos, m_PrtSize, m_PrtPath);	// �����l�\��
		if(dlg.DoModal()==IDOK) {										// �����޲�۸ޕ\��
			dlg.GetValue(&m_PrtOut, &prtori, &m_PrtPos, &m_PrtSize,
                         &m_PrtPath, &m_PrtSCheck, &m_PrtFile);			// ����ݒ����
			if(!m_PrtOut) {												// <������o�͂̏ꍇ>
				m_PrtTCheck = 0;										// ��������Ȃ�
				prtmode->dmOrientation = (short)(prtori+1);				// ����̌���
				return 1;
			} else {													// <̧�ُo�͂̏ꍇ>
				m_PrtTCheck = 0;										// ��������Ȃ�
				m_PrtSCheck = 0;										// ���ޕύX�Ȃ�
				m_pRendDlg->DDBtoDIB(NULL, m_PrtOut,
                                     m_PrtSCheck, m_PrtSize);			// DDB=>DIB�ϊ�
				m_pRendDlg->OutputFile(m_PrtFile);						// ̧�ُo��
				m_PrtKind = 0;
				return 0;												// ����������~
			}
		} else {
			m_PrtKind = 0;
			return 0;													// ���~=�Ԃ�l:0
		}
	}    
}

/****************************************************************/
void CJcad3GlbView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)		//<<< ����J�n����
/****************************************************************/
{
	pDC->ResetDC(pInfo->m_pPD->GetDevMode());							// ��������̍X�V
	if(!m_PrtKind) {													// <<4�ʐ}���>>
		DDBtoDIB(pDC);													// DDB=>DIB�ϊ�
	} else {															// <<�����ݸވ��>>
		m_pRendDlg->DDBtoDIB(pDC, m_PrtOut, m_PrtSCheck, m_PrtSize);	// DDB=>DIB�ϊ�
	}
}

/************************************************************/
void CJcad3GlbView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)			//<<< View����޳���
/************************************************************/
{
	if(!m_PrtKind) {													// <<4�ʐ}���>>
		OutputPrinter(pDC);												// ������o��
	} else {															// <<�����ݸވ��>>
		m_pRendDlg->OutputPrinter(pDC, m_PrtPos);						// ������o��
	}
}

/**********************************************************************/
void CJcad3GlbView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)	//<<< ����I������
/**********************************************************************/
{
	if(!m_PrtKind) {													// <<4�ʐ}���>>
		::GlobalFree((HGLOBAL)m_hDIB);
	} else {															// <<�����ݸވ��>>
		m_pRendDlg->FreeDIB();
	}
	m_PrtKind = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView �f�f

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
// CJcad3GlbView ���b�Z�[�W �n���h��

/**********************************************************/
int CJcad3GlbView::OnCreate(LPCREATESTRUCT lpCreateStruct)				//<<< WM_CREATEү���މ�������
/**********************************************************/
{
	double div1, div2;

	if(CView::OnCreate(lpCreateStruct)==-1) {
		return -1;
	}
	if(!InitGL()) {														// ������
		return -1;
	}
	CreateDialogs();													// �풓�޲�۸ސ���
	div1 = ScrDiv1;														// ��ʔ䗦�ۑ�
	div2 = ScrDiv2;
	ScrDiv1 = 0.5;														// 4�ʐ}�p��ʔ䗦
	ScrDiv2 = 0.5;
	InitDLSetup = 0;													// OpenGL DL ������:��
	ViewSize = CSize(1024, 768);										// ��View���ސݒ�
	PlSize = (int)(768/2.0-1);											// ��XY���ʻ��ސݒ�
	InitScreen(1024, 768);												// ����ذݏ�������
	ScrDiv1 = div1;														// ��ʔ䗦����
	ScrDiv2 = div2;
	Init();																// ��������
	return 0;
}

/***********************************/
void CJcad3GlbView::OnDestroy(void)										//<<< WM_DESTROYү���މ�������
/***********************************/
{
	EndDialogs();														// �풓�޲�۸ޏI��
	::wglMakeCurrent(NULL, NULL);
	::wglDeleteContext(m_hRC);
	CView::OnDestroy();
}

/******************************************************/
void CJcad3GlbView::OnSize(UINT nType, int cx, int cy)					//<<< WM_SIZEү���މ�������
/******************************************************/
{
	CView::OnSize(nType, cx, cy);
	ViewSize = CSize(cx, cy);											// View���ސݒ�
	PlSize = (int)(cy/2.0-1);											// XY���ʻ��ސݒ�
	InitScreen(cx, cy);													// ��ذݏ�������
}

/************************************************************************/
BOOL CJcad3GlbView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)//<<< WM_SETCURSORү���މ�������
/************************************************************************/
{
	POINT point;
	int   ret;

	GetCursorPos(&point);												// ϳ���ذݍ��W�擾
	ScreenToClient(&point);												// �ײ��č��W�ϊ�
	if(m_CursorNo!=0) {													// <�W�����وȊO>
		if(PlRect[XY].PtInRect(point)||
		   PlRect[YZ].PtInRect(point)||
		   PlRect[ZX].PtInRect(point)) {								// <3�ʐ}��>
			if(m_CursorNo==1) {											// [�\������]
				SetCursor(m_hcursor1);
			} else if(m_CursorNo==2) {									// [�趰��]
				SetCursor(m_hcursor2);
			} else {													// [�ްѶ���]
				SetCursor(m_hcursor3);
			}
			return TRUE;
		}
	}
	ret = CheckScrChg(point);											// ��ذݕύX�m�F
	if(ret==1) {														// <XY-PRS,YZ-ZX�̋��E>
		SetCursor(m_hcursor4);											// [���E���ړ�����]
		return TRUE;
	} else if(ret==2) {													// <XY-YZ, PRS-ZX�̋��E>
		SetCursor(m_hcursor5);											// [���E�c�ړ�����]
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

/**********************************************************/
void CJcad3GlbView::OnMouseMove(UINT nFlags, CPoint point)				//<<< WM_MOUSEMOVEү���މ�������
/**********************************************************/
{
	if(m_ScrChg!=0) {													// <��ذݕύX��>
		ChangeScreen(m_ScrChg, point);									// ��ذݕύX����
	} else {															// <��ذݕύX���ȊO>
		CurPos = point;
		if(!m_MoveInit) {
			MenuCall(m_ActMenu, nFlags, point, Plane, 2);				// �ƭ��Ăяo��
			CalcScreenWorld(0, &Cursor, point, HitTestPlane(point));	// ���W�_�擾
			MainWnd->SetCurPos(Cursor.p[0], Cursor.p[1], Cursor.p[2]);
		}
		m_MoveInit = 0;
	}
	CView::OnMouseMove(nFlags, point);
}

/***********************************************************/
BOOL CJcad3GlbView::OnMouseWheel(UINT nFlags, short zDelta,
                                 CPoint point)							//<<< WM_MOUSEHWHEELү���މ�������
/***********************************************************/
{
	OBJTYPE* op;
	PNTTYPE  ept, cpt;
	BOXTYPE  box;
	VECTYPE  vec;
	SCRTYPE  len, pos;
	int      i, ln, sa, plane, dir, dsp, edt;
	double   th;

	if(m_ScrChg==0) {													// <<<��ذݕύX���ȊO>>>
		if(zDelta!=0) {
			plane = HitTestPlane(CurPos);								// ���وʒu�̕���
			if(plane==PRS) {											// <<�����}�̏ꍇ>>
				if(nFlags&MK_CONTROL||nFlags&MK_SHIFT) {				// <Ctrl/Shift��������>
					dir = (nFlags&MK_CONTROL) ? 0 : 1;					// ��]����
					th = ((zDelta>0)?1:-1)*5.0*PI/180.0;				// �p�x���Z�o
					EyeChange(dir, th);									// ���_�ύX
				} else {												// <�������Ȃ�>
					ScrToPnt(0, &m_EyeSv, &ept);						// SCRTYPE => PNTTYPE
					ScrToPnt(0, &m_CentPs, &cpt);						// SCRTYPE => PNTTYPE
					VecV(&ept, &cpt, &vec);								// �J�n�����޸�َ擾
					VecN(&vec);
					for(i=0; i<DIM; i++) {
						ln = (int)(vec.p[i]*m_TruckLen*500.0);			// �덷�̕␳
						sa = ((vec.p[i]*m_TruckLen*500.0)-ln>=0.5) ? 1 : 0;
						sa = ((vec.p[i]*m_TruckLen*500.0)-ln<=-0.5) ? -1 : sa;
						len.p[i] = ln+sa;								// ���_�̈ړ������Z�o
					}
					if(zDelta>0) {										// <�g��\��>
						for(i=0; i<DIM; i++) {
							m_Eye.p[i] = m_Eye.p[i]+len.p[i];			// ���_���ړ�
						}
						if(m_Eye.p[0]==m_Cent3D.p[0]&&
                           m_Eye.p[1]==m_Cent3D.p[1]&&
                           m_Eye.p[2]==m_Cent3D.p[2]) {
							for(i=0; i<DIM; i++) {
								m_Eye.p[i] = m_Eye.p[i]+len.p[i];
							}
						}
					} else {											// <�k���\��>
						for(i=0; i<DIM; i++) {
							m_Eye.p[i] = m_Eye.p[i]-len.p[i];			// ���_���ړ�
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
					SetPerspective();									// �����@�ˉe�s��̍č\�z
					SetXYZGuide();										// XYZ�޲�ސݒ�
					Display(2);											// �ĕ`��
				}
			} else {													// <<3�ʐ}�̏ꍇ>>
				if(nFlags&MK_CONTROL||nFlags&MK_SHIFT) {				// <Ctrl/Shift��������>
					plane = (nFlags&MK_SHIFT) ? plane+1 : plane;		// Shift�������͌���������
					plane = (plane>2) ? 0 : plane;
					dir = (zDelta>0) ? 1 : -1;							// �����̕������Z�o
					Scroll(plane, dir);									// ��ʽ�۰�
				} else {												// <�������Ȃ�>
					m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;			// 3�ʐ}�\����ԕۑ�
					if(m_Char=='Z') {									// <[Z]������>
						BaseObjPtr(1, &op, &dsp, &edt);
						if(op) {										// <�\�����̂���>
							GetAreaBox(1, &box);						// �S���̂�BOX�̍ő�l�擾
							if(m_DAraFlg) {								// <�\���͈͎w��̏ꍇ>
								for(i=0; i<DIM; i++) {
									if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
										box.bp[0].p[i] = m_DAra.bp[0].p[i];
									}
									if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
										box.bp[1].p[i] = m_DAra.bp[1].p[i];
									}
								}
							}
							BoxCenter(box, &cpt);						// �ޯ��������3�ʒ��S
							ScrToPnt(1, &m_Cent3D, &cpt);				// PNTTYPE => SCRTYPE
							ScrToPnt(0, &m_Cent3D, &cpt);				// SCRTYPE => PNTTYPE
							m_pState->SetCent3D(cpt);
						}
					} else if(m_Char=='C') {							// <[C]������>
						ScrToPnt(1, &pos, &Cursor);						// PNTTYPE => SCRTYPE
						for(i=0; i<DIM; i++) {
							len.p[i] = pos.p[i]-m_Cent3D.p[i];			// ���وʒu�ƒ��S�_�̍��Z�o
						}
						if(zDelta>0) {									// <�g��\��>
							if(m_Fovy>200) {							// �\�������K��l(��m)�ȏ�
								for(i=0; i<DIM; i++) {					// �ްь�𐳎��ɎZ�o
									ln = (len.p[i]>0) ? (int)(len.p[i]*0.95+0.5)
                                                      : (int)(len.p[i]*0.95-0.5);
									m_Cent3D.p[i] += (len.p[i]-ln);		// �ްь�̍�����ʒ��S�_�ɉ��Z
								}
							}
						} else {										// <�k���\��>
							if(m_Fovy<1000000) {						// �\�������K��l(��m)�ȉ�
								for(i=0; i<DIM; i++) {					// �ްь�𐳎��ɎZ�o
									ln = (len.p[i]>0) ? (int)(len.p[i]/0.95+0.5)
                                                      : (int)(len.p[i]/0.95-0.5);
									m_Cent3D.p[i] += (len.p[i]-ln);		// �ްь�̍�����ʒ��S�_�ɉ��Z
								}
							}
						}
						ScrToPnt(0, &m_Cent3D, &cpt);					// SCRTYPE => PNTTYPE
						m_pState->SetCent3D(cpt);
					}
					if(zDelta>0) {										// <�g��\��>
						m_Fovy = (m_Fovy>200) ? (int)(m_Fovy*0.95+0.5)	// �\�������K��l(��m)�ȏ�
                                              : m_Fovy;
					} else {											// <�k���\��>
						m_Fovy = (m_Fovy<1000000) ? (int)(m_Fovy/0.95+0.5)
                                                  : m_Fovy;
					}
					SetSeishaei(m_Cent3D, m_Fovy);						// ���ˉe�s��̍č\�z
					ChangeTexPixels();									// ø����߸�ٍX�V
					if(m_GridDisp) {
						SetGrid();										// ��د�ސݒ�
					}
					Display(1);											// �ĕ`��
				}
			}
		}
	}
	return CView::OnMouseWheel(nFlags, zDelta, point);
}

/************************************************************/
void CJcad3GlbView::OnLButtonDown(UINT nFlags, CPoint point)			//<<< WM_LBUTTONDOWNү���މ�������
/************************************************************/
{
	SetCapture();														// ϳ�������
	BasePoint = point;
	CurPos = point;
	Plane = HitTestPlane(point);
	CalcScreenWorld(0, &BaseCursor, point, Plane);						// ���W�_�擾
	if((m_ScrChg=CheckScrChg(point))==0) {								// ��ذݕύX�m�F
		if(m_Char==0||m_Char=='M') {
			m_MoveInit = 1;
			MenuCall(m_ActMenu, nFlags, point, Plane, 1);				// �ƭ��Ăяo��
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

/**********************************************************/
void CJcad3GlbView::OnLButtonUp(UINT nFlags, CPoint point)				//<<< WM_LBUTTONUPү���މ�������
/**********************************************************/
{
	OBJTYPE* op;
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, dsp, edt, pflg;

	ReleaseCapture();
	if(m_ScrChg!=0) {													// <��ذݕύX��>
		ChangeScreen(m_ScrChg, point);									// ��ذݕύX����
		m_ScrChg = 0;
	} else {															// <��ذݕύX���ȊO>
		if(Plane==PRS) {												// <<�����}�̏ꍇ>>
			if(m_Char=='C') {											// <[C]������>
				CentPars();												// �Q�Ɠ_�w��(�����})
			} else if(m_Char=='S') {									// <[S]������>
				SetScreenNo(1);											// �\���͈͐ݒ�(�����})
				Display(0);												// �ĕ\��
			} else if(m_Char=='Q') {									// <[Q]������>
				SetScreenNo(0);											// �\���͈͐ݒ�(�S�ʐ})
				Display(0);												// �ĕ\��
			} else if(m_Char=='X'||m_Char=='Y'||m_Char=='Z'||
                      m_Char=='U'||m_Char=='V'||m_Char=='W') {			// <[X,Y,Z,U,V,W]������>
				if(m_Char=='X') {										// [X]��������XY����(�\)
					pflg = 0;
				} else if(m_Char=='Y') {								// [Y]��������YZ����(�\)
					pflg = 1;
				} else if(m_Char=='Z') {								// [Z]��������ZX����(�\)
					pflg = 2;
				} else if(m_Char=='U') {								// [U]��������XY����(��)
					pflg = 3;
				} else if(m_Char=='V') {								// [V]��������YZ����(��)
					pflg = 4;
				} else if(m_Char=='W') {								// [W]��������ZX����(��)
					pflg = 5;
				}
				EyeSet(pflg);											// �����}�̕ύX
			}
		} else {														// <<3�ʐ}�̏ꍇ>>
			if(m_Char=='Z') {											// <[Z]������>
				BaseObjPtr(1, &op, &dsp, &edt);
				if(op) {												// <�\�����̂���>
					m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;			// 3�ʐ}�\����ԕۑ�
					GetAreaBox(1, &box);								// �S���̂�BOX�̍ő�l�擾
					if(m_DAraFlg) {										// <�\���͈͎w��̏ꍇ>
						for(i=0; i<DIM; i++) {
							if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
								box.bp[0].p[i] = m_DAra.bp[0].p[i];
							}
							if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
								box.bp[1].p[i] = m_DAra.bp[1].p[i];
							}
						}
					}
					BoxCenter(box, &cpt);								// �ޯ��������3�ʒ��S
					ScrToPnt(1, &m_Cent3D, &cpt);						// PNTTYPE => SCRTYPE
					ScrToPnt(0, &m_Cent3D, &cpt);						// SCRTYPE => PNTTYPE
					m_pState->SetCent3D(cpt);
					SetSeishaei(m_Cent3D, m_Fovy);						// ���ˉe�s��̍č\�z
					ChangeTexPixels();									// ø����߸�ٍX�V
					if(m_GridDisp) {
						SetGrid();										// ��د�ސݒ�
					}
					Display(1);											// �ĕ`��
				}
			} else if(m_Char=='C') {									// <[C]������>
				CalcScreenWorld(0, &Cursor, point, HitTestPlane(point));// ���W�_�擾
				MainWnd->SetCurPos(Cursor.p[0], Cursor.p[1], Cursor.p[2]);
				m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;				// 3�ʐ}�\����ԕۑ�
				ScrToPnt(1, &m_Cent3D, &Cursor);						// PNTTYPE => SCRTYPE
				ScrToPnt(0, &m_Cent3D, &Cursor);						// SCRTYPE => PNTTYPE
				m_pState->SetCent3D(Cursor);
				SetSeishaei(m_Cent3D, m_Fovy);							// ���ˉe�s��̍č\�z
				ChangeTexPixels();										// ø����߸�ٍX�V
				if(m_GridDisp) {
					SetGrid();											// ��د�ސݒ�
				}
				Display(1);												// �ĕ`��
			} else if(m_Char=='A') {									// <[A]������>
				AutoZoom();												// �����{��
			} else if(m_Char=='X') {									// <[X]������>
				ExactZoom();											// �����\��
			} else if(m_Char=='S') {									// <[S]������>
				pflg = Plane+2;
				SetScreenNo(pflg);										// �\���͈͐ݒ�(3�ʐ})
				Display(0);												// �ĕ\��
			} else if(m_Char=='Q') {									// <[Q]������>
				SetScreenNo(0);											// �\���͈͐ݒ�(�S�ʐ})
				Display(0);												// �ĕ\��
			} else if(m_Char=='0') {									// <[0]������>
				SetDispMode(0);											// �\��Ӱ�ސݒ�(�ݒ�Ȃ�)
				Display(0);												// �ĕ\��
			} else if(m_Char=='1') {									// <[1]������>
				SetDispMode(1);											// �\��Ӱ�ސݒ�(Ӱ��1)
				Display(0);												// �ĕ\��
			} else if(m_Char=='2') {									// <[2]������>
				SetDispMode(2);											// �\��Ӱ�ސݒ�(Ӱ��2)
				Display(0);												// �ĕ\��
			} else if(m_Char=='3') {									// <[3]������>
				SetDispMode(3);											// �\��Ӱ�ސݒ�(Ӱ��3)
				Display(0);												// �ĕ\��
			} else if(m_Char=='4') {									// <[4]������>
				SetDispMode(4);											// �\��Ӱ�ސݒ�(�����}����ިݸ�)
				Display(0);												// �ĕ\��
			} else if(m_Char=='5') {									// <[5]������>
				SetDispMode(5);											// �\��Ӱ�ސݒ�(�S�ʼ���ިݸ�)
				Display(0);												// �ĕ\��
			}
		}
		if(m_Char==0||m_Char=='M') {
			MenuCall(m_ActMenu, nFlags, point, Plane, 3);				// �ƭ��Ăяo��
		}
	}
	CView::OnLButtonUp(nFlags, point);
}

/************************************************************/
void CJcad3GlbView::OnRButtonDown(UINT nFlags, CPoint point)			//<<< WM_RBUTTONDOWNү���މ�������
/************************************************************/
{
	MenuCall(m_ActMenu, nFlags, point, HitTestPlane(point), 4);			// �ƭ��Ăяo��
	CView::OnRButtonDown(nFlags, point);
}

/*****************************************************/
void CJcad3GlbView::OnKeyDown(UINT nChar, UINT, UINT)					//<<< WM_KEYDOWNү���މ�������
/*****************************************************/
{
	int range, mode;

	if(nChar==VK_DELETE) {												// [Delete]������
		MenuCall(m_ActMenu, 0, 0, 0, 11);
	} else if(nChar==VK_SHIFT) {										// [Shift]������
		m_CChar = m_CChar|0x1;
		if(m_SFuncMode<3) {												// <�ڍ�>
			m_pFunction->OnOffButton(10, 1);							// ̧ݸ��ݐݒ�°ٍX�V
		} else if(m_SFuncMode<6) {										// <�Ȉ�>
			m_pFunction2->OnOffButton(10, 1);							// ̧ݸ��ݐݒ�°�2�X�V
		}
	} else if(nChar==VK_CONTROL) {										// [Ctrl]������
		m_CChar = m_CChar|0x2;
		if(m_SFuncMode<3) {												// <�ڍ�>
			m_pFunction->OnOffButton(11, 1);							// ̧ݸ��ݐݒ�°ٍX�V
		} else if(m_SFuncMode<6) {										// <�Ȉ�>
			m_pFunction2->OnOffButton(11, 1);							// ̧ݸ��ݐݒ�°�2�X�V
		}
	} else if(nChar==VK_SPACE) {										// [SPACE]������
		m_CChar = m_CChar|0x4;
	} else if(nChar==VK_LEFT) {											// [��]������
		m_DChar = m_DChar|0x1;
	} else if(nChar==VK_UP) {											// [��]������
		m_DChar = m_DChar|0x2;
	} else if(nChar==VK_RIGHT) {										// [��]������
		m_DChar = m_DChar|0x4;
	} else if(nChar==VK_DOWN) {											// [��]������
		m_DChar = m_DChar|0x8;
	} else if(nChar>='A'&&nChar<='Z') {									// [��̧�ޯ�]������
		m_Char = nChar;
		MenuCall(m_ActMenu, nChar, 0, 0, 5);							// �ƭ��Ăяo��
	} else if(nChar>='0'&&nChar<='9') {									// [���l]������
		m_Char = nChar;
	} else if(nChar>=VK_NUMPAD0&&nChar<=VK_NUMPAD9) {					// [��]������
		m_Char = nChar - (VK_NUMPAD0-'0');
    }
	if((m_CChar&0x1)&&(m_CChar&0x2)) {									// [Shift]+[Ctrl]��������
		if(m_Char=='A'||m_Char=='S'||m_Char=='Z'||						// [A,S,Z,X,C]��������
           m_Char=='X'||m_Char=='C') {
			if(m_Char=='A') {											// [A]�������͑S����
				range = 0;
			} else if(m_Char=='S') {									// [S]�������͓����}
				range = 1;
			} else if(m_Char=='Z') {									// [Z]��������XY����
				range = 2;
			} else if(m_Char=='X') {									// [X]��������YZ����
				range = 3;
			} else if(m_Char=='C') {									// [C]��������ZX����
				range = 4;
			}
			SetScreenNo(range);											// �\���͈͐ݒ�
			Display(0);													// �ĕ\��
		} else if(m_Char=='0'||m_Char=='1'||m_Char=='2'||				// [Shift]+[Ctrl]��������
                  m_Char=='3'||m_Char=='4'||m_Char=='5') {				// [0,1,2,3,4,5]��������
			if(m_Char=='0') {											// [0]�������͐ݒ�Ȃ�
				mode = 0;
			} else if(m_Char=='1') {									// [1]��������Ӱ��1
				mode = 1;
			} else if(m_Char=='2') {									// [2]��������Ӱ��2
				mode = 2;
			} else if(m_Char=='3') {									// [3]��������Ӱ��3
				mode = 3;
			} else if(m_Char=='4') {									// [4]�������͓����}����ިݸ�
				mode = 4;
			} else if(m_Char=='5') {									// [5]�������͑S�ʼ���ިݸ�
				mode = 5;
			}
			SetDispMode(mode);											// �\��Ӱ�ސݒ�
			Display(0);													// �ĕ\��
		}
	}
}

/**************************************************/
void CJcad3GlbView::OnChar(UINT nChar, UINT, UINT)						//<<< WM_CHARү���މ�������
/**************************************************/
{
	m_Char = nChar;
	if(m_Char>='a'&&m_Char<='z') {										// �p������=>�p�啶��
		m_Char += ('A'-'a');
	}
}

/***************************************************/
void CJcad3GlbView::OnKeyUp(UINT nChar, UINT, UINT)						//<<< WM_KEYUPү���މ�������
/***************************************************/
{
	if(nChar==VK_SHIFT) {												// [Shift]��
		m_CChar = m_CChar&~(0x1);
		if(m_SFuncMode<3) {												// <�ڍ�>
			m_pFunction->OnOffButton(10, 0);							// ̧ݸ��ݐݒ�°ٍX�V
		} else if(m_SFuncMode<6) {										// <�Ȉ�>
			m_pFunction2->OnOffButton(10, 0);							// ̧ݸ��ݐݒ�°�2�X�V
		}
	} else if(nChar==VK_CONTROL) {										// [Ctrl]��
		m_CChar = m_CChar&~(0x2);
		if(m_SFuncMode<3) {												// <�ڍ�>
			m_pFunction->OnOffButton(11, 0);							// ̧ݸ��ݐݒ�°ٍX�V
		} else if(m_SFuncMode<6) {										// <�Ȉ�>
			m_pFunction2->OnOffButton(11, 0);							// ̧ݸ��ݐݒ�°�2�X�V
		}
	} else if(nChar==VK_SPACE) {										// [SPACE]��
		m_CChar = m_CChar&~(0x4);
	} else if(nChar==VK_LEFT) {											// [��]������
		m_DChar = m_DChar&~(0x1);
	} else if(nChar==VK_UP) {											// [��]������
		m_DChar = m_DChar&~(0x2);
	} else if(nChar==VK_RIGHT) {										// [��]������
		m_DChar = m_DChar&~(0x4);
	} else if(nChar==VK_DOWN) {											// [��]������
		m_DChar = m_DChar&~(0x8);
	} else {															// �ȊO�̷�
		m_Char = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// �C���v�������e�[�V����

/********************************/
BOOL CJcad3GlbView::InitGL(void)										//<<< ������(OpenGL)
/********************************/
{
	HDC     hdc;
	CString ext;
	int     i, index, form;
	double  rc, gc, bc;
	BOOL    ret;

	m_pMainDC = new CClientDC(this);									// ���޲����÷�Ď擾
	hdc = m_pMainDC->GetSafeHdc();
	if(!SetupPixelFormat(hdc, &m_PFD)) {
		return FALSE;
	}
	index = ::GetPixelFormat(hdc);
	form = ::DescribePixelFormat(hdc, index, sizeof(PIXELFORMATDESCRIPTOR), &m_PFD);
	if(form<1) {
		return FALSE;
	}
	m_hRC = ::wglCreateContext(hdc);									// �����ݸޥ��÷�Ď擾
	::wglMakeCurrent(hdc, m_hRC);
	MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);				// �F�擾
	glClearColor((float)rc, (float)gc, (float)bc, 0.0f); // 1.0f);		// �װ�ޯ̧�������l�ݒ�
	glClearDepth(1.0);													// ���߽�ޯ̧�������l�ݒ�
	glClearStencil(0x0);												// ��ݼ��ޯ̧�����l�ݒ�
	glDepthFunc(GL_LESS);
	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	for(i=0; i<21; i++) {												// OpenGL�װ�ݒ�
		SetGLColor(i);
	}
	SetGLRender();														// OpenGL�װ�ݒ�
	if(!SetGLRenderMap()) {												// OpenGLϯ�ߐݒ�
		return FALSE;
	}
	SetRenderMode(0);
	for(i=0; i<3; i++) {
		if(m_TexDisp[i]) {
			ext = m_TexFile[i].Right(3);
			if(!ext.CompareNoCase("bmp")) {
				ret = SetTexImageBMP(i, 0);								// ø����pBMP�摜�ް��ݒ�
			} else {
				ret = SetTexImageJPG(i);								// ø����pJPG�摜�ް��ݒ�
			}
			if(ret) {													// <<ø����ް��ݒ�OK>>
				if(m_TexRotate[i]==1) {									// <�E90�x��]>
					RotateTexImage(1, i);								// ø����p�摜�ް���]
				} else if(m_TexRotate[i]==2) {							// <180�x��]>
					RotateTexImage(1, i);								// ø����p�摜�ް���]
					RotateTexImage(1, i);								// ø����p�摜�ް���]
				} else if(m_TexRotate[i]==3) {							// <��90�x��]>
					RotateTexImage(0, i);								// ø����p�摜�ް���]
				}
			}
		}
	}
	glNewList(DL_XY, GL_COMPILE);										// <XY�����ި���ڲ�ؽ�>
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0);
	glEndList();
	glNewList(DL_YZ, GL_COMPILE);										// <YZ�����ި���ڲ�ؽ�>
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glEndList();
	glNewList(DL_ZX, GL_COMPILE);										// <ZX�����ި���ڲ�ؽ�>
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
void CJcad3GlbView::SetGLColor(int ix)									//<<< OpenGL�װ�ݒ�
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
	id = DL_GOLD + ix;													// �ި���ڲ�ؽ�No
	glNewList(id, GL_COMPILE);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambi);			// ����&�g�U��
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);						// ���ʌ�
		glMaterialf(GL_FRONT, GL_SHININESS, (float)m_Shin[ix]);			// ���ʎw��
		glMaterialfv(GL_FRONT, GL_EMISSION, emis);						// ���ˌ�
	glEndList();
}

/*************************************/
void CJcad3GlbView::SetGLRender(void)									//<<< OpenGL�װ�ݒ�
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
	glNewList(DL_LIGHT, GL_COMPILE_AND_EXECUTE);						// �����ި���ڲ�ؽ�
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);							// ����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);							// �g�U��
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);						// ���ʌ�
		glLightfv(GL_LIGHT0, GL_POSITION, ligh);						// �����ʒu
		glEnable(GL_LIGHT0);											// ����0�g�p��
	glEndList();
}

/****************************************/
BOOL CJcad3GlbView::SetGLRenderMap(void)								//<<< OpenGLϯ�ߐݒ�
/****************************************/
{
	WIN32_FIND_DATA wfd;
	FILE* fp;
	int   i, j, w, h;
	BOOL  ret;

	if(FindFirstFile(m_MapFile, &wfd)==INVALID_HANDLE_VALUE) {			// ̧�ٖ��̏ꍇ
		m_MapFile = GetAppPath(0)+MAP_NAME;								// ����̧�ٖ��g�p
	}
	if(fopen_s(&fp, m_MapFile, "rb")!=0) {
		SetSplashMsg(IDS_NO_MAPPING);									// ��ϯ��ݸޖ�:���~
		return FALSE;
	}
	int* hd = new int[54];
	for(i=0; i<54; i++) {
		hd[i] = fgetc(fp);
	}
	w = hd[18]+hd[19]*256+hd[20]*256*256+hd[21]*256*256*256;			// �摜�̉��߸�ِ�
	h = hd[22]+hd[23]*256+hd[24]*256*256+hd[25]*256*256*256;			// �摜�̏c�߸�ِ�
	if(w!=HBMP||h!=VBMP) {												// <��:512,��:256�ȊO>
		AfxMessageBox(IDS_ERR_MAPFILE);									// �摜̧�ٴװ
		for(i=0; i<VBMP; i++) {
			for(j=0; j<HBMP; j++) {
				TexImage[i][j][2] = 0xff;
				TexImage[i][j][1] = 0xff;
				TexImage[i][j][0] = 0xff;
			}
		}
		ret = FALSE;
	} else {															// <�����ް�>
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
void CJcad3GlbView::DDBtoDIB(CDC* pDC)									//<<< DDB=>DIB�ϊ�
/**************************************/
{
	UINT*   col = new UINT[20];
	CWaitCursor wait;													// ���ĥ���ٕ\��
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

	m_hDIB = NULL;														// DIB����ُ�����
	Display(0);															// ��ʕ`��
	if(m_PrtColor!=0) {													// <�w�i�F�ύX�L>
		MainWnd->SaveObjColor(col);										// ���̕\���F�ۑ�
		if(m_PrtColor==1) {												// �w�i�F:���̏ꍇ
			MainWnd->ChangeObjColor(MainWnd->WhiteColor);				// ���̕\���F�ύX
			MainWnd->GetColor(MainWnd->BlackColor, &rc, &gc, &bc);
		} else {														// �w�i�F:���̏ꍇ
			MainWnd->ChangeObjColor(MainWnd->BlackColor);				// ���̕\���F�ύX
			MainWnd->GetColor(MainWnd->WhiteColor, &rc, &gc, &bc);
		}
		glClearColor((float)rc, (float)gc, (float)bc, 0.0f); // 1.0f);	// �װ�ޯ̧�������l�ݒ�
	}
	if(m_PrtTCheck) {													// <�������>
		SetScreenNo(0);													// �\���͈͐ݒ�
	}
	Display(0);															// OpenGL�ĕ\��
	Display(0);															// OpenGL�ĕ\��
	Display(0);															// OpenGL�ĕ\��
	Display(0);															// OpenGL�ĕ\��
	if(!m_PrtMode) {													// <<4�ʐ}�w��>>
		fx = 0, fy = 0;													// 4�ʐ}�]�����ʒu
		fw = ViewSize.cx, fh = ViewSize.cy;								// 4�ʐ}�]��������
	} else {															// <<�����}�w��>>
		fx = PlRect[PRS].left, fy = PlRect[PRS].top;					// �����}�]�����ʒu
		fw = PlRect[PRS].Width(), fh = PlRect[PRS].Height();			// �����}�]��������
		if(fw<10||fh<10) {
			fx = 0, fy = 0;												// �����}���L���͈͕��Ȃ�
			fw = ViewSize.cx, fh = ViewSize.cy;							// �ꍇ��4�ʐ}�]��
		}
	}
	GetActiveWindow()->GetClientRect(&crct);							// �ײ��ė̈�̈ʒu
	ClientToScreen(&crct);												// ��ʍ��W�֕ϊ�
	m_pwnd = GetDesktopWindow();										// �޽�į�߳���޳
	m_pcdc = m_pwnd->GetDC();											// ��è�޳���޳�̸ײ���DC
	m_pwnd->GetWindowRect(&drct);										// �޽�į�߳���޳�̈ʒu
	fx += crct.left, fy += crct.top;									// �޽�į�ߏ�̸ײ��ė̈�̈ʒu
	if(fx>drct.right||fy>drct.bottom||
       fx+fw>drct.right||fy+fh>drct.bottom) {							// �̈悪�޽�į�߻��ނ𒴂���ꍇ
		fx = 0, fy = 0, fw = drct.Width(), fh = drct.Height();			// �޽�į�ߗ̈�̺�߰
	}
	if(!m_PrtOut) {														// <<<������o��>>>
		px = pDC->GetDeviceCaps(LOGPIXELSX);							// X���������dpi
		py = pDC->GetDeviceCaps(LOGPIXELSY);							// Y���������dpi
		if(m_PrtTCheck) {												// <<�������>>
			if(fw>fh) {													// <������>
				tw = (int)((px*PlDstW[0]*2.0)/25.4);					// ����������(dpi)
				th = (int)(((double)fh*py*tw)/(
					fw*px));					// ����������(dpi)
			} else {													// <������>
				th = (int)((py*PlDstH[0]*2.0)/25.4);					// ����������(dpi)
				tw = (int)(((double)fw*px*th)/(fh*py));					// ����������(dpi)
			}
		} else if(m_PrtSCheck&&m_PrtSize>0) {							// <<�o�ͻ��ގw��>>
			if(fw>fh) {													// <������>
				tw = (int)(px*m_PrtSize/25.4);							// ����������(dpi)
				th = (int)(((double)fh*py*tw)/(fw*px));					// ����������(dpi)
			} else {													// <������>
				th = (int)(py*m_PrtSize/25.4);							// ����������(dpi)
				tw = (int)(((double)fw*px*th)/(fh*py));					// ����������(dpi)
			}
		} else {														// <<���ގw�薳>>
			tw = fw, th = fh;											// ���ʌ��Ɠ��껲��
		}
	} else {															// <<<̧�ُo��>>>
		tw = fw, th = fh;												// ���ʌ��Ɠ��껲��
	}
	PrRect.SetRect(0, 0, tw, th);										// ���(�]����)���ޕۑ�
	if(!m_scdc.CreateCompatibleDC(m_pcdc)) {							// ��ذ����DC����
		esw = 1;
	}
	if(!m_bitmap.CreateCompatibleBitmap(m_pcdc, tw, th)) {				// ��ذ݌݊��ޯ�ϯ�ߐ���
		esw = 1;
	}
	CBitmap*  pOldBitmap = m_scdc.SelectObject(&m_bitmap);				// ���DC���ޯ�ϯ�ߐݒ�
	int  pOldSMode = m_scdc.SetStretchBltMode(COLORONCOLOR);			// �L�k���[�h�ݒ�
	if(!m_scdc.StretchBlt(0, 0, tw, th, m_pcdc,
                          fx, fy, fw, fh, SRCCOPY)) {					// ��ذ�->���DC �L�k��߰
		esw = 1;
	}
	m_scdc.SetStretchBltMode(pOldSMode);								// �L�kӰ�ޖ߂�
	m_scdc.SelectObject(pOldBitmap);									// ���DC->�ޯ�ϯ�� �ؗ�
	if(esw) {
		m_scdc.DeleteDC();												// ���DC�폜
		m_bitmap.DeleteObject();										// ��ذ݌݊��ޯ�ϯ�ߍ폜
		if(m_PrtColor!=0) {												// <�w�i�F�ύX�L>
			MainWnd->RestoreObjColor(col);								// ���̕\���F����
			MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);
			glClearColor((float)rc, (float)gc, (float)bc, 0.0f);//1.0f);// �װ�ޯ̧�������l�ݒ�
			Display(0);													// OpenGL�ĕ\��
			Display(0);													// OpenGL�ĕ\��
		}
		AfxMessageBox(IDS_PRTERR);										// �װү���ތ㒆�~
		delete[] col;
		return;
	}
	HBITMAP hbitmap = (HBITMAP)m_bitmap.GetSafeHandle();				// �ޯ�ϯ������َ擾
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	::GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);
	biBits = 24;			// (WORD)(bm.bmPlanes*bm.bmBitsPixel);
	bi.biSize = sizeof(BITMAPINFOHEADER);								// DIBͯ�ް�쐬
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
	if(biBits==1) {														// ��گĻ��ތv�Z
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
	m_scdc.DeleteDC();													// ���DC�폜
	m_bitmap.DeleteObject();											// ��ذ݌݊��ޯ�ϯ�ߍ폜
	if(m_PrtColor!=0) {													// <�w�i�F�ύX�L>
		MainWnd->RestoreObjColor(col);									// ���̕\���F����
		MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);
		glClearColor((float)rc, (float)gc, (float)bc, 0.0f); // 1.0f);	// �װ�ޯ̧�������l�ݒ�
		Display(0);														// OpenGL�ĕ\��
		Display(0);														// OpenGL�ĕ\��
	}
	delete[] col;
}

/*******************************************/
void CJcad3GlbView::OutputPrinter(CDC* pDC)								//<<< ������ւ̈��
/*******************************************/
{
	HDC   hdc;
	LPSTR lpDIBHdr;														// Pointer to BITMAPINFOHEADER
	LPSTR lpDIBBits;													// Pointer to DIB bits
	int   sx, sy, px=0, py=0, bSuccess=0;

	if(m_hDIB==NULL) {
		return;
	}
	if(!m_PrtPos) {														// <�p����������̏ꍇ>
		sx = pDC->GetDeviceCaps(HORZRES);								// X�����������
		sy = pDC->GetDeviceCaps(VERTRES);								// Y�����������
		px = (sx>PrRect.Width()) ? (sx-PrRect.Width())/2 : 0;			// X��������ʒu
		py = (sy>PrRect.Height()) ? (sy-PrRect.Height())/2 : 0;			// Y��������ʒu
	}
	lpDIBHdr = (LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
	lpDIBBits = lpDIBHdr + *(LPDWORD)lpDIBHdr + PalSize;
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	hdc = pDC->m_hDC;
	hpal = ::SelectPalette(hdc, hpal, TRUE);							// ������گĕύX
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
		AfxMessageBox(IDS_PRTERR);										// �𑜓x��������
	}
}

/************************************/
void CJcad3GlbView::OutputFile(void)									//<<< ̧�و��
/************************************/
{
	CString ext;
	BOOL    ret;

	if(m_hDIB!=NULL) {
		ext = m_PrtFile.Right(3);										// ̧�ق̊g���q
		if(!ext.CompareNoCase("jpg")) {									// <JPEG�`��>
			ret = IKJpegFileSave(m_PrtFile, m_hDIB, 0, m_CapJComp, 2,
                                 "JCAD3 GLB Output Picture",
                                 0, NULL, NULL, NULL);					// JPEĢ�ُo��
		} else if(!ext.CompareNoCase("bmp")) {							// <�ޯ�ϯ�ߌ`��>
			ret = IKBmpFileSave(m_PrtFile, m_hDIB, FALSE,
                                0, NULL, NULL, NULL);					// �ޯ�ϯ��̧�ُo��
		}
		::GlobalFree((HGLOBAL)m_hDIB);
		if(!ret) {
			AfxMessageBox(IDS_ERR_FILEWRITE);							// ̧�ٴװ
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// �I�y���[�V����

/******************************/
void CJcad3GlbView::Init(void)											//<<< ������
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
	InitChar();															// �����͏�����
	m_XLmtR = 0.0, m_YLmtR = 0.0, m_ZLmtR = 0.0;						// �ׯ�����菉����
	Tracker1.Reset(1);													// �ׯ��1����
	Tracker2.Reset(1);													// �ׯ��2����
	Tracker3.Reset(1);													// �ׯ��3����
	Tracker4.Reset(1);													// �ׯ��4����
	RestoreHomePosition(0);												// �\����ԕ��A
	InitOifItem();														// ���̍\������я�����
	InitDLSetup = 1;													// OpenGL DL ������:��
}

/**********************************/
void CJcad3GlbView::InitChar(void)										//<<< �����͏�����
/**********************************/
{
	m_Char = 0, m_CChar = 0, m_FChar = 0, m_FPres = 0, m_DChar = 0;
	if(m_SFuncMode<3) {													// <�ڍ�>
		m_pFunction->OffAllButton();									// ̧ݸ��ݐݒ�°ّS�tOFF
	} else if(m_SFuncMode<6) {											// <�Ȉ�>
		m_pFunction2->OffAllButton();									// ̧ݸ��ݐݒ�°�2�S�tOFF
	}
}

/***********************************************/
void CJcad3GlbView::SetCChar(int flg, UINT chr)							//<<< ���䷰�ݒ�
/***********************************************/
{
	m_CChar = (flg) ? m_CChar|chr : m_CChar&~(chr);
}

/***********************************************/
void CJcad3GlbView::SetFChar(int flg, UINT chr)							//<<< ̧ݸ��ݷ��ݒ�
/***********************************************/
{
	m_FChar = (flg) ? m_FChar|chr : m_FChar&~(chr);
}

/***************************************/
void CJcad3GlbView::GetFChar(int* func)									//<<< ̧ݸ��ݷ��擾
/***************************************/
{
	func[0] = (m_FChar&0x1) ? 1 : 0;									// [F5]��������
	func[1] = (m_FChar&0x2) ? 1 : 0;									// [F6]��������
	func[2] = (m_FChar&0x4) ? 1 : 0;									// [F7]��������
	func[3] = (m_FChar&0x8) ? 1 : 0;									// [F8]��������
	func[4] = (m_FChar&0x10) ? 1 : 0;									// [F9]��������
	func[5] = (m_FChar&0x20) ? 1 : 0;									// [F10]��������
	func[6] = (m_FChar&0x40) ? 1 : 0;									// [F11]��������
	func[7] = (m_FChar&0x80) ? 1 : 0;									// [F12]��������
}

/***************************************/
void CJcad3GlbView::SetScreenNo(int no)									//<<< ��ذ�No�ݒ�
/***************************************/
{
	if(no==0) {															// �S����
		ScrDiv1 = 0.5, ScrDiv2 = 0.5;
	} else if(no==1) {													// �����}
		ScrDiv1 = 0.0, ScrDiv2 = 1.0;
	} else if(no==2) {													// XY����
		ScrDiv1 = 1.0, ScrDiv2 = 1.0;
	} else if(no==3) {													// YZ����
		ScrDiv1 = 1.0, ScrDiv2 = 0.0;
	} else if(no==4) {													// ZX����
		ScrDiv1 = 0.0, ScrDiv2 = 0.0;
	}
	InitScreen(ViewSize.cx, ViewSize.cy);								// ��ذݏ�������
}

/***********************************************/
void CJcad3GlbView::OutputCapData(CString path)							//<<< �������ް��o��
/***********************************************/
{
	CWaitCursor wait;													// ���ĥ���ٕ\��
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

	Display(0);															// OpenGL�ĕ\��
	Display(0);															// OpenGL�ĕ\��
	Display(0);															// OpenGL�ĕ\��
	Display(0);															// OpenGL�ĕ\��
	m_hDIB = NULL;														// DIB����ُ�����
	Display(0);															// OpenGL�ĕ\��
	if(m_CapPlane==0) {													// <X-Y����>
		fx = PlRect[XY].left, fy = PlRect[XY].top;						// X-Y���ʓ]�����ʒu
		fw = PlRect[XY].Width(), fh = PlRect[XY].Height();				// X-Y���ʓ]��������
	} else if(m_CapPlane==1) {											// <Y-Z����>
		fx = PlRect[YZ].left, fy = PlRect[YZ].top;						// Y-Z���ʓ]�����ʒu
		fw = PlRect[YZ].Width(), fh = PlRect[YZ].Height();				// Y-Z���ʓ]��������
	} else if(m_CapPlane==2) {											// <Z-X����>
		fx = PlRect[ZX].left, fy = PlRect[ZX].top;						// Z-X���ʓ]�����ʒu
		fw = PlRect[ZX].Width(), fh = PlRect[ZX].Height();				// Z-X���ʓ]��������
	} else if(m_CapPlane==3) {											// <��������>
		fx = PlRect[PRS].left, fy = PlRect[PRS].top;					// �����}�]�����ʒu
		fw = PlRect[PRS].Width(), fh = PlRect[PRS].Height();			// �����}�]��������
	} else {															// <�S��(4��)>
		fx = 0, fy = 0;													// 4�ʐ}�]�����ʒu
		fw = ViewSize.cx, fh = ViewSize.cy;								// 4�ʐ}�]��������
	}
	if(fw<10||fh<10) {													// �L���͈͕��Ȃ��ꍇ��
		fx = 0, fy = 0, fw = ViewSize.cx, fh = ViewSize.cy;				// 4�ʐ}�]��
	}
	sw = fw, sh = fh;													// ���ʻ��ނ�ۑ�
	fw = (int)(((double)fw*(double)m_CapWidth/100.0)+0.5);				// �o�͕�
	fw = (fw<10) ? 10 : fw;
	fh = (int)(((double)fh*(double)m_CapHeight/100.0)+0.5);				// �o�͍���
	fh = (fh<10) ? 10 : fh;
	if(m_CapPos==0) {													// ��ʒ��S�̏ꍇ��
		fx += (sw-fw)/2, fy += (sh-fh)/2;								// �k����/2��]�����ʒu�ɉ��Z
	}
	GetActiveWindow()->GetClientRect(&crct);							// �ײ��ė̈�̈ʒu
	ClientToScreen(&crct);												// ��ʍ��W�֕ϊ�
	m_pwnd = GetDesktopWindow();										// �޽�į�߳���޳
	m_pcdc = m_pwnd->GetDC();											// ��è�޳���޳�̸ײ���DC
	m_pwnd->GetWindowRect(&drct);										// �޽�į�߳���޳�̈ʒu
	fx += crct.left, fy += crct.top;									// �޽�į�ߏ�̸ײ��ė̈�̈ʒu
	if(fx>drct.right||fy>drct.bottom||
       fx+fw>drct.right||fy+fh>drct.bottom) {							// �̈悪�޽�į�߻��ނ𒴂���ꍇ
		fx = 0, fy = 0, fw = drct.Width(), fh = drct.Height();			// �޽�į�ߗ̈�̺�߰
	}
	if(!m_scdc.CreateCompatibleDC(m_pcdc)) {							// ��ذ����DC����
		esw = 1;
	}
	if(!m_bitmap.CreateCompatibleBitmap(m_pcdc, fw, fh)) {				// ��ذ݌݊��ޯ�ϯ�ߐ���
		esw = 1;
	}
	CBitmap* pOldBitmap = m_scdc.SelectObject(&m_bitmap);				// ���DC���ޯ�ϯ�ߐݒ�
	if(!m_scdc.BitBlt(0, 0, fw, fh, m_pcdc, fx, fy, SRCCOPY)) {			// ��ذ�->���DC�ֺ�߰
		esw = 1;
	}
	m_scdc.SelectObject(pOldBitmap);									// ���DC->�ޯ�ϯ�� �ؗ�
	if(esw) {
		m_scdc.DeleteDC();												// ���DC�폜
		m_bitmap.DeleteObject();										// ��ذ݌݊��ޯ�ϯ�ߍ폜
		m_pwnd->ReleaseDC(m_pcdc);										// �ײ���DC�̉��
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �װү���ތ㒆�~
		return;
	}
	HBITMAP hbitmap = (HBITMAP)m_bitmap.GetSafeHandle();				// �ޯ�ϯ������َ擾
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	::GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);
	biBits = 24;			// (WORD)(bm.bmPlanes*bm.bmBitsPixel);
	bi.biSize = sizeof(BITMAPINFOHEADER);								// DIBͯ�ް�쐬
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
	if(biBits==1) {														// ��گĻ��ތv�Z
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
	m_scdc.DeleteDC();													// ���DC�폜
	m_bitmap.DeleteObject();											// ��ذ݌݊��ޯ�ϯ�ߍ폜
	m_pwnd->ReleaseDC(m_pcdc);											// �ײ���DC�̉��
	if(m_hDIB!=NULL) {
		if(m_CapKind==0) {												// <JPEG�`��>
			ret = IKJpegFileSave(path+".jpg", m_hDIB, 0, m_CapJComp, 2,
                                 "JCAD3 GLB Output Picture",
                                 0, NULL, NULL, NULL);					// JPEĢ�ُo��
		} else if(m_CapKind==1) {										// <�ޯ�ϯ�ߌ`��>
			ret = IKBmpFileSave(path+".bmp", m_hDIB, FALSE,
                                0, NULL, NULL, NULL);					// �ޯ�ϯ��̧�ُo��
		}
		::GlobalFree((HGLOBAL)m_hDIB);
		if(!ret) {
			AfxMessageBox(IDS_ERR_FILEWRITE);							// ̧�ٴװ
		}
	}
}

/**********************************************/
void CJcad3GlbView::InitScreen(int cx, int cy)							//<<< ��ذݏ�������
/**********************************************/
{
	int    xs, ys, xi, yi;
	double xd, yd, rc, gc, bc;

	xs = (int)(ScrDiv1*cx);
	ys = (int)(ScrDiv2*cy);
	xi = (xs==0) ? 0 : xs - 1;											// XY���ʻ��ސݒ�
	yi = (ys==0) ? 0 : ys - 1;
	PlRect[XY].SetRect(0, 0, xi, yi);
	PlSizeH[XY] = PlRect[XY].Height();
	PlSizeW[XY] = PlRect[XY].Width();
	xi = (xs==0) ? 0 : xs - 1;											// YZ���ʻ��ސݒ�
	yi = (ys==0) ? 0 : ((ys==cy) ? ys-1 : ys+1);
	PlRect[YZ].SetRect(0, yi, xi, cy-1);
	PlSizeH[YZ] = PlRect[YZ].Height();
	PlSizeW[YZ] = PlRect[YZ].Width();
	xi = (xs==0) ? 0 : ((xs==cx) ? xs-1 : xs+1);						// ZX���ʻ��ސݒ�
	yi = (ys==0) ? 0 : ((ys==cy) ? ys-1 : ys+1);
	PlRect[ZX].SetRect(xi, yi, cx-1, cy-1);
	PlSizeH[ZX] = PlRect[ZX].Height();
	PlSizeW[ZX] = PlRect[ZX].Width();
	xi = (xs==0) ? 0 : ((xs==cx) ? xs-1 : xs+1);						// �����}���ސݒ�
	yi = (ys==0) ? 0 : ys-1;
	PlRect[PRS].SetRect(xi, 0, cx-1, yi);
	PlSizeH[PRS] = PlRect[PRS].Height();
	PlSizeW[PRS] = PlRect[PRS].Width();
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��̍č\�z
	SetPerspective();													// �����@�ˉe�s��̍č\�z
	SetXYZGuide();														// XYZ�޲�ސݒ�
	glNewList(DL_BORDER, GL_COMPILE);									// 4�ʐ}���E�ި���ڲ�ؽ�
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);							// ���ˉe�s��쐬
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glLineWidth(1.0f);											// �����ݒ�
			glBegin(GL_LINE_STRIP);
				MainWnd->GetColor(MainWnd->BorderSColor, &rc, &gc, &bc);
				glColor3d(rc, gc, bc);
				glVertex2d(0.0, 0.0);									// 4�ʐ}�O�g����ײ�(���e��)
				glVertex2d(0.0, 1.0);									// 4�ʐ}�O�g����ײ�(���e��)
				glVertex2d(1.0, 1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
				MainWnd->GetColor(MainWnd->BorderHColor, &rc, &gc, &bc);
				glColor3d(rc, gc, bc);
				glVertex2d(1.0, 1.0);									// 4�ʐ}�O�g����ײ�(������)
				glVertex2d(1.0, 0.0);									// 4�ʐ}�O�g����ײ�(������)
				glVertex2d(0.0, 0.0);
			glEnd();
			if((PlSizeH[XY]>0&&PlSizeH[YZ]>0)||
               (PlSizeW[YZ]>0&&PlSizeW[ZX]>0)) {
				glLineWidth(2.0f);										// �����ݒ�
				glBegin(GL_LINES);
					MainWnd->GetColor(MainWnd->BorderColor, &rc, &gc, &bc);
					glColor3d(rc, gc, bc);								// �\���F�ݒ�(���S��)
					if(PlSizeW[YZ]>0&&PlSizeW[ZX]>0) {
						glVertex2d((double)ScrDiv1*1.0, 0.0);			// 4�ʐ}���E����ײ�(���S��)
						glVertex2d((double)ScrDiv1*1.0, 1.0);
					}
					if(PlSizeH[XY]>0&&PlSizeH[YZ]>0) {
						glVertex2d(0.0, 1.0-(double)ScrDiv2);			// 4�ʐ}���E����ײ�(���S��)
						glVertex2d(1.0, 1.0-(double)ScrDiv2);
					}
				glEnd();
			}
			glLineWidth(1.0f);											// �����ݒ�
			xd = (double)ViewSize.cy/ViewSize.cx, yd = 1.0;				// �c����ݒ�
			glBegin(GL_LINES);
				if((PlSizeH[XY]>0&&PlSizeW[XY]>0)||
                   (PlSizeH[ZX]>0&&PlSizeW[ZX]>0)) {
					MainWnd->GetColor(MainWnd->XGuideColor, &rc, &gc, &bc);
					glColor3d(rc, gc, bc);								// �\���F�ݒ�
					if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {
						glVertex2d(0.0+(0.025*xd), 1.0-(0.025*yd));		// XY����X���W��
						glVertex2d(0.0+(0.025*xd), 1.0-(0.075*yd));
						glVertex2d(0.0+(0.0175*xd), 1.0-(0.08*yd));		// XY����X����
						glVertex2d(0.0+(0.0325*xd), 1.0-(0.105*yd));
						glVertex2d(0.0+(0.0325*xd), 1.0-(0.08*yd));
						glVertex2d(0.0+(0.0175*xd), 1.0-(0.105*yd));
					}
					if(PlSizeH[ZX]>0&&PlSizeW[ZX]>0) {
						glVertex2d(1.0-(0.025*xd), 0.0+(0.025*yd));		// ZX����X���W��
						glVertex2d(1.0-(0.075*xd), 0.0+(0.025*yd));
						glVertex2d(1.0-(0.095*xd), 0.0+(0.0375*yd));	// ZX����X����
						glVertex2d(1.0-(0.08*xd), 0.0+(0.0125*yd));
						glVertex2d(1.0-(0.08*xd), 0.0+(0.0375*yd));
						glVertex2d(1.0-(0.095*xd), 0.0+(0.0125*yd));
					}
				}
				if((PlSizeH[XY]>0&&PlSizeW[XY]>0)||
                   (PlSizeH[YZ]>0&&PlSizeW[YZ]>0)) {
					MainWnd->GetColor(MainWnd->YGuideColor, &rc, &gc, &bc);
					glColor3d(rc, gc, bc);								// �\���F�ݒ�
					if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {
						glVertex2d(0.0+(0.025*xd), 1.0-(0.025*yd));		// XY����Y���W��
						glVertex2d(0.0+(0.075*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.08*xd), 1.0-(0.0125*yd));		// XY����Y����
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.095*xd), 1.0-(0.0125*yd));
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.025*yd));
						glVertex2d(0.0+(0.0875*xd), 1.0-(0.0375*yd));
					}
					if(PlSizeH[YZ]>0&&PlSizeW[YZ]>0) {
						glVertex2d(0.0+(0.025*xd), 0.0+(0.025*yd));		// YZ����Y���W��
						glVertex2d(0.0+(0.075*xd), 0.0+(0.025*yd));
						glVertex2d(0.0+(0.08*xd), 0.0+(0.0375*yd));		// YZ����Y����
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
					glColor3d(rc, gc, bc);								// �\���F�ݒ�
					if(PlSizeH[YZ]>0&&PlSizeW[YZ]>0) {
						glVertex2d(0.0+(0.025*xd), 0.0+(0.025*yd));		// YZ����Z���W��
						glVertex2d(0.0+(0.025*xd), 0.0+(0.075*yd));
						glVertex2d(0.0+(0.0175*xd), 0.0+(0.105*yd));	// YZ����Z����
						glVertex2d(0.0+(0.0325*xd), 0.0+(0.105*yd));
						glVertex2d(0.0+(0.0325*xd), 0.0+(0.105*yd));
						glVertex2d(0.0+(0.0175*xd), 0.0+(0.08*yd));
						glVertex2d(0.0+(0.0175*xd), 0.0+(0.08*yd));
						glVertex2d(0.0+(0.0325*xd), 0.0+(0.08*yd));
					}
					if(PlSizeH[ZX]>0&&PlSizeW[ZX]>0) {
						glVertex2d(1.0-(0.025*xd), 0.0+(0.025*yd));		// ZX����Z���W��
						glVertex2d(1.0-(0.025*xd), 0.0+(0.075*yd));
						glVertex2d(1.0-(0.0325*xd), 0.0+(0.105*yd));	// ZX����Z����
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
	if(InitDLSetup) {													// <OpenGL DL ������:��>
		ChangeTexPixels();												// ø����߸�ٍX�V
	}
}

/************************************/
void CJcad3GlbView::ResetColor(void)									//<<< ��ذݶװ�Đݒ�
/************************************/
{
	double r, g, b;

	MainWnd->GetColor(MainWnd->BackColor, &r, &g, &b);
	glClearColor((float)r, (float)g, (float)b, 0.0f); // 1.0f);			// �װ�ޯ̧�������l�ݒ�
	InitScreen(ViewSize.cx, ViewSize.cy);								// ��ذݏ�������
	Display(0);															// OpenGL�ĕ\��
}

/**************************************************************/
void CJcad3GlbView::SetSeishaei(const SCRTYPE& cent, int fovy)			//<<< ���ˉe�s��\�z
/**************************************************************/
{
	BOXTYPE box;
	int     i;
	double  cpt[3], mia[3], maa[3], fvy, len, add=0;

	if(PlSizeH[XY]<1&&PlSizeH[YZ]<1&&PlSizeH[ZX]<1) {
		return;
	}
	GetScrBox(&box);													// ��ذ��ޯ���擾
	for(i=0; i<DIM; i++) {
		cpt[i] = cent.p[i]/1000.0;										// ���Ұ�������
		mia[i] = (box.bp[0].p[i]>0.0) ? 0.0 : box.bp[0].p[i];			// �\�����̻̂���
		maa[i] = (box.bp[1].p[i]<0.0) ? 0.0 : box.bp[1].p[i];
		PlDstH[i] = 10.0, PlDstW[i] = 10.0;
	}
	fvy = fovy/1000.0;
	if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {									// ��ذݕ\������
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
	mia[0] = (mia[0]>cpt[0]-PlDstH[0]/2.0)?cpt[0]-PlDstH[0]/2.0:mia[0];	// ��ذ�&���͈͔̔�r
	mia[0] = (mia[0]>cpt[0]-PlDstW[2]/2.0)?cpt[0]-PlDstW[2]/2.0:mia[0];
	maa[0] = (maa[0]<cpt[0]+PlDstH[0]/2.0)?cpt[0]+PlDstH[0]/2.0:maa[0];
	maa[0] = (maa[0]<cpt[0]+PlDstW[2]/2.0)?cpt[0]+PlDstW[2]/2.0:maa[0];
	mia[1] = (mia[1]>cpt[1]-PlDstW[1]/2.0)?cpt[1]-PlDstW[1]/2.0:mia[1];
	maa[1] = (maa[1]<cpt[1]+PlDstW[1]/2.0)?cpt[1]+PlDstW[1]/2.0:maa[1];
	mia[2] = (mia[2]>cpt[2]-PlDstH[2]/2.0)?cpt[2]-PlDstH[2]/2.0:mia[2];
	maa[2] = (maa[2]<cpt[2]+PlDstH[2]/2.0)?cpt[2]+PlDstH[2]/2.0:maa[2];
	for(i=0; i<3; i++) {
		if((len=maa[i]-mia[i])>add) {									// �ő啝
			add = len;
		}
	}
	add = (add>100.0) ? add*1.5 : 100.0;
	glNewList(DL_ORTHOXY, GL_COMPILE);									// XY���ˉe�ި���ڲ�ؽ�
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(cpt[1]-PlDstW[0]/2.0, cpt[1]+PlDstW[0]/2.0, -cpt[0]-PlDstH[0]/2.0,
				-cpt[0]+PlDstH[0]/2.0, mia[2]-add, maa[2]+add);
	glEndList();
	glNewList(DL_ORTHOYZ, GL_COMPILE);									// YZ���ˉe�ި���ڲ�ؽ�
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(cpt[1]-PlDstW[1]/2.0, cpt[1]+PlDstW[1]/2.0, cpt[2]-PlDstH[1]/2.0,
				cpt[2]+PlDstH[1]/2.0, mia[0]-add, maa[0]+add);
	glEndList();
	glNewList(DL_ORTHOZX, GL_COMPILE);									// ZX���ˉe�ި���ڲ�ؽ�
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-cpt[0]-PlDstW[2]/2.0, -cpt[0]+PlDstW[2]/2.0, cpt[2]-PlDstH[2]/2.0,
				cpt[2]+PlDstH[2]/2.0, mia[1]-add, maa[1]+add);
	glEndList();
}

/****************************************/
void CJcad3GlbView::SetPerspective(void)								//<<< �����@�ˉe�s��̍\�z
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
	VecV(&pt1, &pt2, &vec);												// �J�n�����޸�َ擾
	for(i=0; i<DIM; i++) {
		weye[i] = m_Eye.p[i]/1000.0;									// ���Ұ�������
		wprs[i] = m_Eye.p[i]/1000.0+vec.p[i];
	}
	aspect = (double)PlSizeW[PRS]/PlSizeH[PRS];
	glNewList(DL_PARS, GL_COMPILE);										// �����@�ˉe�ި���ڲ�ؽ�
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, aspect, 1.0, 3000.0);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(weye[0], weye[1], weye[2], wprs[0], wprs[1], wprs[2],
                  m_VtVec.p[0], m_VtVec.p[1], m_VtVec.p[2]);
	glEndList();
}

/************************************/
void CJcad3GlbView::Display(int flg)									//<<< �ĕ`��
/************************************/									// flg 0:�S��,1:3�ʐ},2:�����}
{
	int i, wflg;

	if(m_Ncdflg||ViewSize.cy<1) {										// NC�\������̈斳�͒��~
		return;
	}
	wflg = flg;
	if(flg!=m_DisplayFlg) {												// <�\���ر���قȂ�ꍇ>
		wflg = 0;														// �ŏ��͕K���S�ʕ\��(��ʂ�����h�~)
	}
	m_DisplayFlg = flg;													// �\���׸ޕۑ�
	if(wflg==0) {														// <<�S�ʕ`��>>
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		if(!m_OpenGLTest) {												// <OpenGLýĒ��ȊO>
			for(i=0; i<3; i++) {										// 3�ʕ�(XY:0,YZ:1,ZX:2)
				if(PlSizeH[i]>0&&PlSizeW[i]>0) {						// <3�ʐ}�̈�L>
					glEnable(GL_STENCIL_TEST);
					ViewDisplayXYZ(i);									// 3�ʐ}�`��
					glDisable(GL_STENCIL_TEST);
				}
			}
		}
		if(PlSizeH[PRS]>0&&PlSizeW[PRS]>0) {							// <�����}�̈�L>
			ViewDisplayPRS();											// �����}�`��
		}
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, ViewSize.cx-1, ViewSize.cy-1);					// 4�ʐ}���E&�O�g�`��
		glCallList(DL_BORDER);
	} else if(wflg==1) {												// <<3�ʐ}�`��>>
		if(m_OpenGLTest) {												// OpenGL�޽Ē��͒��~
			return;
		}
		for(i=0; i<3; i++) {											// 3�ʕ�(XY:0,YZ:1,ZX:2)
			if(PlSizeH[i]>0&&PlSizeW[i]>0) {							// <3�ʐ}�̈�L>
				glScissor(PlRect[i].left+1, ViewSize.cy-PlRect[i].bottom,
                          PlRect[i].Width()-1, PlRect[i].Height()-1);	// �`��̈�𐧌�
				glEnable(GL_SCISSOR_TEST);								// ��ް�ýĂ�L����
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_STENCIL_TEST);
				ViewDisplayXYZ(i);										// 3�ʐ}�`��
				glDisable(GL_STENCIL_TEST);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_SCISSOR_TEST);								// ��ް�ýĂ𖳌���
			}
		}
		glViewport(0, 0, ViewSize.cx-1, ViewSize.cy-1);					// 4�ʐ}���E&�O�g�`��
		glCallList(DL_BORDER);
	} else {															// <<�����}�`��>>
		if(PlSizeH[PRS]>0&&PlSizeW[PRS]>0) {							// <�����}�̈�L>
			glScissor(PlRect[PRS].left+1, ViewSize.cy-PlRect[PRS].bottom,
                      PlRect[PRS].Width()-1, PlRect[PRS].Height()-1);	// �`��̈�𐧌�
			glEnable(GL_SCISSOR_TEST);									// ��ް�ýĂ�L����
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			ViewDisplayPRS();											// �����}�`��
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_SCISSOR_TEST);									// ��ް�ýĂ𖳌���
		}
	}
	glFinish();
	::SwapBuffers(::wglGetCurrentDC());
}

/*********************************************/
void CJcad3GlbView::ViewDisplayXYZ(int plane)							//<<< 3�ʐ}�`��
/*********************************************/
{
	glPushMatrix();
		glLoadIdentity();												// �ޭ��߰Đݒ�
		glViewport(PlRect[plane].left, ViewSize.cy-PlRect[plane].bottom-1,
			       PlRect[plane].Width(), PlRect[plane].Height());
		glCallList(DL_ORTHOXY+plane);									// ���ˉe�ϊ�
		glCallList(DL_XY+plane);										// ����ݸޕϊ�
		glStencilFunc(GL_ALWAYS, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if((m_RendPlane&&m_Rendering>1)||m_Shading==2||m_DispMode==5) {	// <3�ʐ}����ިݸ�>
			DisplayAllObj3P(0);											// 3�ʐ}���̕\��
		}
		glDisable(GL_DEPTH_TEST);										// ���߽�ýĂ𖳌���
		if(Tracker1.EnableTrack()) {
			Tracker1.Draw(plane);										// �ׯ��1�`��
		}
		if(Tracker2.EnableTrack()) {
			Tracker2.Draw(plane);										// �ׯ��2�`��
		}
		if(Tracker3.EnableTrack()) {
			Tracker3.Draw(plane);										// �ׯ��3�`��
		}
		if(Tracker4.EnableTrack()) {
			Tracker4.Draw(plane);										// �ׯ��4�`��
		}
		if(Line.EnableDraw()) {
			Line.Draw(plane);											// ײݕ`��
		}
		if(m_BOX&&m_BoxPlane==plane) {
			glCallList(m_BOX);											// ��`�`��
		}
		if(m_CIRCLE&&m_CirclePlane==plane) {
			glCallList(m_CIRCLE);										// �~�`��
		}
		glEnable(GL_DEPTH_TEST);										// ���߽�ýĂ�L����
		glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if((m_RendPlane==0||m_Rendering<2)&&
           m_Shading!=2&&m_DispMode!=5) {								// <3�ʐ}����ިݸވȊO>
			DisplayAllObj3P(2);											// 3�ʐ}���̕\��(�ҏW�Ώ�)
			DisplayAllObj3P(1);											// 3�ʐ}���̕\��(�ҏW�ΏۊO)
		}
		if(m_BASE&&(m_BasePlane==plane||m_BasePlane==3)) {
			glCallList(m_BASE);											// ����`��
		}
		if(m_BpDisp) {
			glCallList(DL_BASEPOINT);									// ��_�`��
		}
		if(m_BlDisp) {													// ����`��
			glCallList(DL_BASELINE);
		}
		if(m_GridDisp) {
			DispGrid(plane, 0, 1);										// ��د�ޕ`��
		}
		if(m_SCALE&&m_ScalePlane==plane) {
			glCallList(m_SCALE);										// ��K�`��
		}
		if(m_PROT&&m_ProtPlane==plane) {
			glCallList(m_PROT);											// ���x��`��
		}
		if(m_GLB[plane]) {
			glCallList(m_GLB[plane]);		    						// GLB����ڰĕ`��
		}
		if(m_TEMP[plane]) {
			glCallList(m_TEMP[plane]);									// ����ڰĕ`��
		}
		if(m_STAGE) {
			glCallList(m_STAGE);										// ڲ��ڰ�ݸ޽ð�ޕ`��
		}
		if(m_TexDisp[plane]) {											// <�摜�\����>
			glCallList(m_TEXTURE[plane]);								// ø����߸�ٕ`��
		}
		glPopMatrix();
}

/****************************************/
void CJcad3GlbView::ViewDisplayPRS(void)								//<<< �����}�`��
/****************************************/
{
	glPushMatrix();
		glLoadIdentity();												// �ޭ��߰Đݒ�
		glViewport(PlRect[PRS].left, ViewSize.cy-PlRect[PRS].bottom-1,
			       PlRect[PRS].Width(), PlRect[PRS].Height());
		glCallList(DL_PARS);											// �����@�ˉe&����ݸޕϊ�
		if(Tracker1.EnableTrack()) {
			Tracker1.Draw(PRS);											// �ׯ��1�`��
		}
		if(Tracker2.EnableTrack()) {
			Tracker2.Draw(PRS);											// �ׯ��2�`��
		}
		if(Tracker3.EnableTrack()) {
			Tracker3.Draw(PRS);											// �ׯ��3�`��
		}
		if(Tracker4.EnableTrack()) {
			Tracker4.Draw(PRS);											// �ׯ��4�`��
		}
		DisplayAllObjPars();											// �����}�S���̕\��
		glCallList(DL_XYZGUIDE);										// XYZ�޲�ޕ\��
		if(Line.EnableDraw()) {
			Line.Draw(PRS);												// ײݕ`��
		}
		if(m_STAGE) {
			glCallList(m_STAGE);										// ڲ��ڰ�ݸ޽ð�ޕ`��
		}
	glPopMatrix();
}

/********************************************************************/
void CJcad3GlbView::GetDisplayColor(OBJTYPE* op, int edt,
                                    double* r, double* g, double* b)	//<<< ���̕`��F�擾
/********************************************************************/
{
	int sel, gno[32];

	sel = GetSelNo(op);													// �I��No�擾
	if(sel==1) {														// �I�𗧑�1(BoolColor)
		MainWnd->GetColor(MainWnd->BoolColor, r, g, b);
	} else if(sel==2) {													// �I�𗧑�2(ChooseColor)
		MainWnd->GetColor(MainWnd->ChooseColor, r, g, b);
	} else if(edt==0) {													// �ҏW�ΏۊO����
		GetGroup(op, gno);												// ��ٰ�ߏ��擾
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
	} else {															// �ҏW�Ώۗ���
		MainWnd->GetColor(MainWnd->SelectColor, r, g, b);
	}
}

/********************************************/
void CJcad3GlbView::DisplayAllObj3P(int flg)							//<<< 3�ʐ}���̕`��
/********************************************/
{
	OBJTYPE* op;
	int      wfg, dsp, edt, cno, dmode, lmode, uflg, sel, col;
	double   r, g, b;

	if((m_RendPlane&&m_Rendering>1)||m_DispMode==5) {					// <3�ʐ}����ިݸ�>
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_LIGHTING);
	}
	wfg = (flg!=0) ? flg : 1;
	BaseObjPtr(wfg, &op, &dsp, &edt);									// �擪�����߲���擾
	while(op) {
		if(flg==1&&edt==1) {											// ��Ώۗ��̎w���̏ꍇ
			NextObjPtr(wfg, &op, &dsp, &edt);							// �������߲���擾
			continue;													// �����̂�
		}
		cno = GetObjCmdNo(1, op);										// ���̺����No�擾
		if(m_DispMode==5&&(cno==SETSURF||cno==SETCURVE)) {				// <�����}����ިݸނőΏۊO>
			glShadeModel(GL_FLAT);										// �ݒ��ײ݂ɕύX
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
			GetDisplayColor(op, edt, &r, &g, &b);						// ���̕`��F�擾
			glColor3d(r, g, b);											// �\���F�ݒ�
		} else if((m_RendPlane&&m_Rendering>1)||m_DispMode==5) {		// <3�ʐ}����ިݸ�>
			if(m_RendPlane&&m_Rendering>1) {
				col = GetObjCol(op);									// �ގ��擾
			} else {
				sel = GetSelNo(op);										// �I��No�擾
				if(sel==1) {											// �I�𗧑�1(BoolColor)
					col = 20;											// DL_BOOL
				} else if(sel==2) {										// �I�𗧑�2(ChooseColor)
					col = 19;											// DL_CHOOSE
				} else if(edt==1) {										// �ҏW�Ώۗ���
					col = 18;											// DL_SELECT
				} else {												// �ҏW�ΏۊO����
					col = GetObjCol(op);								// �ގ��擾
				}
			}
			glCallList(DL_GOLD+col);									// �ގ��ݒ�
		} else if(m_Rendering==1&&m_Shading==2&&GetSelNo(op)==3) {		// <����ިݸޥӰ��>
			glShadeModel(GL_SMOOTH);
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
			glCallList(DL_GOLD);										// �ގ��ݒ�
		} else {														// <�ʏ�\��>
			GetDisplayColor(op, edt, &r, &g, &b);						// ���̕`��F�擾
			glColor3d(r, g, b);											// �\���F�ݒ�
		}
		lmode = GetListMode(op);										// ؽ�Ӱ�ގ擾(0/1:�d��,2:�y��,3:����ިݸ�)
		if(m_DispMode<4) {												// <����ިݸވȊO>
			dmode = (m_DispMode==0||(m_DispMode==2&&edt==1)||
                    (m_DispMode==3&&edt==0)) ? 1 : 2;					// �\��Ӱ�ގ擾(1:�d��,2:�y��)
			uflg = (lmode==1&&dmode==2) ? 1 : 0;						// �d���\�����Ɍy��Ӱ�ނ͍X�VON
			uflg = (lmode==2&&dmode==1) ? 1 : uflg;						// �y���\�����ɏd��Ӱ�ނ͍X�VON
			uflg = (lmode==3&&m_Rendering==0) ? 1 : uflg;				// ����ިݸޒ���͍X�VON
			uflg = (lmode==3&&m_Rendering==1&&
                    m_Shading>0&&GetSelNo(op)!=3) ? 1 : uflg;
		} else {														// <����ިݸ�>
			uflg = (lmode!=3&&cno!=SETSURF&&cno!=SETCURVE) ? 1 : 0;		// ����ިݸ��ް����ݒ�͍X�VON
		}
		if(uflg) {
			SetDispList(op);											// �ި���ڲؽĐݒ�
		}
		glCallList(GetListNo(op));										// 1���̕`��
		if(m_DispMode==5&&(cno==SETSURF||cno==SETCURVE)) {				// <�����}����ިݸނőΏۊO>
			glShadeModel(GL_SMOOTH);									// �ݒ�𼪰�ިݸނɕύX
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
		} else if(m_DispMode!=5&&
                  m_Rendering==1&&m_Shading==2&&GetSelNo(op)==3) {		// <����ިݸޥӰ��>
			glShadeModel(GL_FLAT);										// �ݒ�����ɖ߂�
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
		}
		NextObjPtr(wfg, &op, &dsp, &edt);								// �������߲���擾
	}
	if((m_RendPlane&&m_Rendering>1)||m_DispMode==5) {					// <3�ʐ}����ިݸ�>
		glShadeModel(GL_FLAT);											// �ݒ�����ɖ߂�
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
	}
}

/*******************************************/
void CJcad3GlbView::DisplayAllObjPars(void)								//<<< �����}���̕`��
/*******************************************/
{
	OBJTYPE* op;
	int      dsp, edt, cno, dmode, lmode, uflg, sel, col;
	double   r, g, b;

	if(m_Rendering==2) {												// <ø���������ݸ�>
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	} else if(m_Rendering==3||m_DispMode>=4) {							// <����ިݸ�>
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_LIGHTING);
	}
	BaseObjPtr(1, &op, &dsp, &edt);										// �擪�����߲���擾
	while(op) {
		cno = GetObjCmdNo(1, op);										// ���̺����No�擾
		if(m_DispMode>=4&&(cno==SETSURF||cno==SETCURVE)) {				// <�����}����ިݸނőΏۊO>
			glShadeModel(GL_FLAT);										// �ݒ��ײ݂ɕύX
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
			GetDisplayColor(op, edt, &r, &g, &b);						// ���̕`��F�擾
			glColor3d(r, g, b);											// �\���F�ݒ�
		} else if(m_Rendering==2||m_Rendering==3||m_DispMode>=4) {		// <�����ݸ�/����ިݸ�>
			if(m_Rendering==2||m_Rendering==3) {
				col = GetObjCol(op);									// �ގ��擾
			} else {
				sel = GetSelNo(op);										// �I��No�擾
				if(sel==1) {											// �I�𗧑�1(BoolColor)
					col = 20;											// DL_BOOL
				} else if(sel==2) {										// �I�𗧑�2(ChooseColor)
					col = 19;											// DL_CHOOSE
				} else if(edt==1) {										// �ҏW�Ώۗ���
					col = 18;											// DL_SELECT
				} else {												// �ҏW�ΏۊO����
					col = GetObjCol(op);								// �ގ��擾
				}
			}
			glCallList(DL_GOLD+col);									// �ގ��ݒ�
		} else if(m_Rendering==1&&m_Shading>0&&GetSelNo(op)==3) {		// <����ިݸޥӰ��>
			glShadeModel(GL_SMOOTH);
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
			glCallList(DL_GOLD);										// �ގ��ݒ�
		} else {														// <�ʏ�\��>
			GetDisplayColor(op, edt, &r, &g, &b);						// ���̕`��F�擾
			glColor3d(r, g, b);											// �\���F�ݒ�
		}
		lmode = GetListMode(op);										// ؽ�Ӱ�ގ擾(0/1:�d��,2:�y��,3:����ިݸ�)
		if(m_DispMode<4) {												// <����ިݸވȊO>
			dmode = (m_DispMode==0||(m_DispMode==2&&edt==1)||
                    (m_DispMode==3&&edt==0)) ? 1 : 2;					// �\��Ӱ�ގ擾(1:�d��,2:�y��)
			uflg = (lmode==1&&dmode==2) ? 1 : 0;						// �d���\�����Ɍy��Ӱ�ނ͍X�VON
			uflg = (lmode==2&&dmode==1) ? 1 : uflg;						// �y���\�����ɏd��Ӱ�ނ͍X�VON
			uflg = (lmode==3&&m_Rendering==0) ? 1 : uflg;				// ����ިݸޒ���͍X�VON
			uflg = (lmode==3&&m_Rendering==1&&
                    m_Shading>0&&GetSelNo(op)!=3) ? 1 : uflg;
		} else {														// <����ިݸ�>
			uflg = (lmode!=3&&cno!=SETSURF&&cno!=SETCURVE) ? 1 : 0;		// ����ިݸ��ް����ݒ�͍X�VON
		}
		if(uflg) {
			SetDispList(op);											// �ި���ڲؽĐݒ�
		}
		glCallList(GetListNo(op));										// 1���̕`��
		if(m_DispMode>=4&&(cno==SETSURF||cno==SETCURVE)) {				// <�����}����ިݸނőΏۊO>
			glShadeModel(GL_SMOOTH);									// �ݒ�𼪰�ިݸނɕύX
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_LIGHTING);
		} else if(m_DispMode<4&&
                  m_Rendering==1&&m_Shading>0&&GetSelNo(op)==3) {		// <����ިݸޥӰ��>
			glShadeModel(GL_FLAT);										// �ݒ�����ɖ߂�
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
		}
		NextObjPtr(1, &op, &dsp, &edt);									// �������߲���擾
	}
	if(m_Rendering==2) {												// <ø���������ݸ�>
		glShadeModel(GL_FLAT);											// �ݒ�����ɖ߂�
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	} else if(m_Rendering==3||m_DispMode>=4) {							// <�S����ިݸ�>
		glShadeModel(GL_FLAT);											// �ݒ�����ɖ߂�
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
	}
}

/*********************************************************/
void CJcad3GlbView::DispGrid(int plane, int ers, int bak)				//<<< ��د�ޕ\��/����
/*********************************************************/
{
	int    men=1;
	double rc, gc, bc;

	if(m_Fovy/m_GridSpace>ViewSize.cy/10) {
		return;
	}
	if(ers) {
		MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);			// ��د�ޏ����F
	} else {
		MainWnd->GetColor(MainWnd->GridColor, &rc, &gc, &bc);			// ��د�ޕ\���F
	}
	if(!bak) {
		glDrawBuffer(GL_FRONT_AND_BACK);								// �O�ޯ̧�ɂ��`��w��
	}
	glPushMatrix();
		glLoadIdentity();
		glViewport(PlRect[plane].left, ViewSize.cy-PlRect[plane].bottom-1,
	               PlRect[plane].Width(), PlRect[plane].Height());
		glCallList(DL_ORTHOXY+plane);
		glCallList(DL_XY+plane);
		glColor3d(rc, gc, bc);											// �\���F�ݒ�
		glCallList(DL_GRIDXY+plane);									// ��د�ޕ\��
		if(ers) {
			MainWnd->GetColor(MainWnd->BackColor, &rc, &gc, &bc);		// ��د�ޏ����F
		} else {
			MainWnd->GetColor(MainWnd->GridBPColor, &rc,&gc,&bc);		// ��د�ޖڐ��F
		}
		glColor3d(rc, gc, bc);											// �\���F�ݒ�
		glCallList(DL_GRIDXY2+plane);									// ��د�ޕ\��
	glPopMatrix();
	glDrawBuffer(GL_BACK);
}

/********************************************/
void CJcad3GlbView::SetDispList(OBJTYPE* op)							//<<< �ި���ڲ�ؽēo�^
/********************************************/
{
	PNTTYPE pnt;
	UINT    lno, dmd;
	int     i, cno, num1, num2, flg, ed;

	cno = GetObjCmdNo(1, op);											// ���̺����No�擾
	if(m_DispMode>=4&&cno!=SETSURF&&cno!=SETCURVE) {					// <<����ިݸ�Ӱ��>>
		if(GetObjAtr(op)!=1) {											// <��ζ�ĈȊO>
			SetShadingData(op);											// ����ިݸ��ް��ݒ�
		} else {														// <��ζ��>
			GetJewelFVec(op);											// �ʖ@���޸�َ擾
		}
	} else {															// <<����ިݸ�Ӱ�ވȊO>>
		lno = GetListNo(op);											// ؽ�No�擾
		num1 = GetFlpNum1(op);											// ��ٰ��1���擾
		num2 = GetFlpNum2(op);											// ��ٰ��2���擾
		if(num1<2) {
			if(lno!=0) {
				glDeleteLists(lno, 1);									// �ި���ڲ�ؽč폜
				PutListNo(op, 0);
			}
			return;
		}
		ed = CheckObjPtr(2, op);										// �ҏW��������
		if(lno==0) {
			lno = glGenLists(1);										// �V�K�擾
			PutListNo(op, lno);
		}
		if(m_DispMode==0||(m_DispMode==2&&ed==1)||
           (m_DispMode==3&&ed==0)||num2==0) {							// <��ٰ��1�ް�>
			dmd = (GetObjAtr(op)/10==1) ? GL_LINE_STRIP : GL_POLYGON;	// ��/��Ӱ��
			glNewList(lno, GL_COMPILE);									// �ި���ڲ�ؽĊJ�n
				if(GetSelNo(op)==2&&(m_ULineCnt>0||m_VLineCnt>0)) {		// <����_���̂Ŏw��L>
					AddLineColorList(op);								// U/VײݐF�ύX
				}
				glBegin(dmd);											// ٰ�ߊJ�n
					for(i=0; i<num1-1; i++) {
						GetFlpVtx1(op, i, &pnt, &flg);					// ���_���W�擾
						if(flg==1) {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);	// ���_�o�^
							glEnd();									// ٰ�ߏI��
							glBegin(dmd);
						} else {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
						}
					}
					GetFlpVtx1(op, i, &pnt, &flg);
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
				glEnd();
			glEndList();												// �ި���ڲ�ؽďI��
			if(num2!=0) {
				PutListMode(op, 1);										// ؽ�Ӱ�ސݒ�
			} else {
				PutListMode(op, 0);
			}
		} else {														// <��ٰ��2�ް�>
			glNewList(lno, GL_COMPILE);									// �ި���ڲ�ؽĊJ�n
				glBegin(GL_LINE_STRIP);									// ٰ�ߊJ�n
					for(i=0; i<num2-1; i++) {
						GetFlpVtx2(op, i, &pnt, &flg);					// ���_���W�̎擾
						if(flg==1) {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);	// ���_�o�^
							glEnd();									// ٰ�ߏI��
							glBegin(GL_LINE_STRIP);
						} else {
							glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
						}
					}
					GetFlpVtx2(op, i, &pnt, &flg);
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);
				glEnd();
			glEndList();
			PutListMode(op, 2);											// ؽ�Ӱ�ސݒ�
		}
	}
}

/***********************************************************/
void CJcad3GlbView::SetRenderList(OBJTYPE* op, VECTYPE* vc)				//<<< �ި���ڲ�ؽēo�^
/***********************************************************/
{
	PNTTYPE pt;
	UINT    lno, dmd;
	int     i, ln, fg;

	lno = GetListNo(op);												// �ި���ڲ�ؽĔԍ��擾
	ln = GetFlpNum1(op);												// ���̖�ٰ�ߐ��擾1
	if(ln<2) {															// ��ٰ�ߐ���2����
		if(lno!=0) {													// ؽĔԍ�����`�ς�
			glDeleteLists(lno, 1);										// �ި���ڲ�ؽč폜
			PutListNo(op, 0);											// �ި���ڲ�ؽĔԍ�������
		}
		return;
	}
	if(lno==0) {
		lno = glGenLists(1);											// �L���ި���ڲ�ؽĔԍ��擾
		PutListNo(op, lno);												// �ި���ڲ�ؽĔԍ��w��
	}
	dmd = (GetObjAtr(op)/10==1) ? GL_LINE_STRIP : GL_POLYGON;			// ײ�/��غ�ݕ`��w��
	glNewList(lno, GL_COMPILE);											// �ި���ڲ�ؽč쐬�J�n
		glBegin(dmd);
			for(i=0; i<ln-1; i++) {
				GetFlpVtx1(op, i, &pt, &fg);							// �w���ٰ�ߍ��W�擾
				if(fg==1) {
					glNormal3d(vc[i].p[0], vc[i].p[1], vc[i].p[2]);
					glVertex3d(pt.p[0], pt.p[1], pt.p[2]);				// ���_���W�o�^
					glEnd();											// 1�ʏI��
					glBegin(dmd);										// ���̖ʂ�
				} else {
					glNormal3d(vc[i].p[0], vc[i].p[1], vc[i].p[2]);
					glVertex3d(pt.p[0], pt.p[1], pt.p[2]);				// ���_���W�o�^
				}
			}
			GetFlpVtx1(op, i, &pt, &fg);								// �w���ٰ�ߍ��W�擾
			glNormal3d(vc[i].p[0], vc[i].p[1], vc[i].p[2]);
			glVertex3d(pt.p[0], pt.p[1], pt.p[2]);						// �Ō㒸�_���W�o�^
		glEnd();
	glEndList();														// �ި���ڲ�ؽč쐬�I��
	PutListMode(op, 3);													// ؽ�Ӱ�ސݒ�
}

/********************************************/
void CJcad3GlbView::DelDispList(OBJTYPE* op)							//<<< �ި���ڲ�ؽč폜
/********************************************/
{
	UINT lno;

	if((lno=GetListNo(op))!=0) {										// �ި���ڲ�ؽĔԍ��擾
		glDeleteLists(lno, 1);											// �ި���ڲ�ؽč폜
		PutListNo(op, 0);												// �ި���ڲ�ؽĔԍ��ݒ�
	}
}

/*******************************************/
void CJcad3GlbView::InitLineNo(OBJTYPE* op)								//<<< ײݔԍ�������
/*******************************************/
{
	m_ULineCnt = 0, m_VLineCnt = 0;
	if(op!=NULL) {
		SetDispList(op);												// �ި���ڲؽēo�^
	}
}

/********************************************************/
void CJcad3GlbView::SetLineNo(int flg, int cnt, int *no)				//<<< ײݔԍ��ݒ�
/********************************************************/
{
	int i;

	if(!flg) {															// <Uײ݂̏ꍇ>
		m_ULineCnt = cnt;												// Uײݐ��ݒ�
		for(i=0; i<cnt; i++) {
			m_ULineNo[i] = no[i];										// Uײݔԍ��ݒ�
		}
	} else {															// <Vײ݂̏ꍇ>
		m_VLineCnt = cnt;												// Vײݐ��ݒ�
		for(i=0; i<cnt; i++) {
			m_VLineNo[i] = no[i];										// Vײݔԍ��ݒ�
		}
	}
}

/*************************************************/
void CJcad3GlbView::AddLineColorList(OBJTYPE* op)						//<<< �ި���ڲ�ؽēo�^
/*************************************************/						// <U/VײݐF�ύX>
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	UINT     dmode;
	int      un, vn, ub, vb, i, j, ln, st=0;
	double   rc, gc, bc;

	BaseCmdPtr(1, op, &cmp);											// �L���擪����ގ擾
	un = GetParaInt(cmp, 0);											// U����_���擾
	vn = GetParaInt(cmp, 1);											// V����_���擾
	ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	MainWnd->GetColor(MainWnd->SelLineColor, &rc, &gc, &bc);			// �I��Ő��װ�擾
	dmode = GL_LINE_STRIP;												// ��Ӱ��
	if(m_ULineCnt>0) {													// <<Uײݎw�莞>>
		for(i=0; i<m_ULineCnt; i++) {
			ln = m_ULineNo[i];											// Uײݔԍ�
			if(ln>=un) {												// �����ԍ����~
				break;
			}
			if(!st) {													// <��������>
				glLineWidth(1.05f);										// �����ݒ�(����)
				glColor3d(rc, gc, bc);									// �F�ݒ�
				st = 1;
			}
			glBegin(dmode);												// ٰ�ߊJ�n
				for(j=0; j<vn; j++) {
					GetVtx(op, (ln+(j*un)), &pnt);						// ���_���W�擾
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// ���_�o�^
				}
				if(vb) {												// <V:�̏ꍇ>
					GetVtx(op, ln, &pnt);								// �J�n�_���W�擾
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// ���_�o�^
				}
			glEnd();													// ٰ�ߏI��
		}
	}
	if(m_VLineCnt>0) {													// <<Vײݎw�莞>>
		for(i=0; i<m_VLineCnt; i++) {
			ln = m_VLineNo[i];											// Vײݔԍ�
			if(ln>=vn) {												// �����ԍ����~
				break;
			}
			if(!st) {													// <��������>
				glLineWidth(1.05f);										// �����ݒ�(����)
				glColor3d(rc, gc, bc);									// �F�ݒ�
				st = 1;
			}
			glBegin(dmode);												// ٰ�ߊJ�n
				for(j=0; j<un; j++) {
					GetVtx(op, ((ln*un)+j), &pnt);						// ���_���W�擾
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// ���_�o�^
				}
				if(ub) {												// <U:�̏ꍇ>
					GetVtx(op, (ln*un), &pnt);							// �J�n�_���W�擾
					glVertex3d(pnt.p[0], pnt.p[1], pnt.p[2]);			// ���_�o�^
				}
			glEnd();													// ٰ�ߏI��
		}
	}
	if(st) {															// <�I������>
		MainWnd->GetColor(MainWnd->ChooseColor, &rc, &gc, &bc);
		glLineWidth(1.0f);												// �����ݒ�(�W���ɖ߂�)
		glColor3d(rc, gc, bc);											// �F�ݒ�
	}
}

/*******************************************/
void CJcad3GlbView::SetRenderMode(int mode)								//<<< �����ݸސݒ�
/*******************************************/
{
	m_Rendering = mode;
	if(mode==2) {														// <ø���������ݸ�>
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
	} else {															// <����ިݸ�/ܲ԰�ڰ�>
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

/************************************/
void CJcad3GlbView::InitNCDisp(void)									//<<< NC�ް��\����������
/************************************/
{
	if(m_NCLIST==0) {
		m_NCLIST = glGenLists(1);										// �V�K�擾
	}
	m_Ncdflg = 1;														// NC�\���׸�ON
	m_Ncpflg = 0;														// NC�o�^�׸ޏ�����
	glDrawBuffer(GL_FRONT);												// �\���ޯ̧����
}

/*******************************************************/
void CJcad3GlbView::SetNCDisp(PNTTYPE spt, PNTTYPE ept)					//<<< NC�ް��o�^
/*******************************************************/
{
	if(m_Ncpflg==0) {													// <<<�o�^�J�n>>>
		glNewList(m_NCLIST, GL_COMPILE);								// �ި���ڲ�ؽĊJ�n
			glBegin(GL_LINE_STRIP);										// ٰ�ߊJ�n
				glVertex3d(spt.p[0], spt.p[1], spt.p[2]);				// ���W�o�^
				glVertex3d(ept.p[0], ept.p[1], ept.p[2]);
		m_Ncpflg = 1;													// �o�^�J�n��
		m_Ncpnt = ept;													// �I���_�ۑ�
	} else {															// <<<�o�^��>>>
		if(fabs(m_Ncpnt.p[0]-spt.p[0])>EPSILON||
           fabs(m_Ncpnt.p[1]-spt.p[1])>EPSILON||
           fabs(m_Ncpnt.p[2]-spt.p[2])>EPSILON) {
			glEnd();													// ٰ�ߏI��
			glBegin(GL_LINE_STRIP);										// ٰ�ߊJ�n
				glVertex3d(spt.p[0], spt.p[1], spt.p[2]);				// ���W�o�^
		}
		glVertex3d(ept.p[0], ept.p[1], ept.p[2]);
		m_Ncpnt = ept;													// �I���_�ۑ�
	}
}

/***************************************/
void CJcad3GlbView::DispNCDisp(int col)									//<<< NC�ް��\��
/***************************************/
{
	int    i;
	UINT   cno;
	double rc, gc, bc;

	if(m_Ncpflg==1) {													// <<<�ް��o�^��>>>
			glEnd();													// ٰ�ߏI��
		glEndList();													// �ި���ڲ�ؽďI��
		cno = (col==0) ? MainWnd->NCDispColor1 : MainWnd->NCDispColor2;
		MainWnd->GetColor(cno, &rc, &gc, &bc);							// �\���F�擾
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		for(i=0; i<3; i++) {											// 3�ʕ�
			glPushMatrix();
				glLoadIdentity();										// �ޭ��߰Đݒ�
				glViewport(PlRect[i].left, ViewSize.cy-PlRect[i].bottom-1,
			               PlRect[i].Width(), PlRect[i].Height());
				glCallList(DL_ORTHOXY+i);								// ���ˉe�ϊ�
				glCallList(DL_XY+i);									// ����ݸޕϊ�
				glPushMatrix();
					glColor3d(rc, gc, bc);								// �\���F�ݒ�
					glCallList(m_NCLIST);								// ���̕`��
				glPopMatrix();
			glPopMatrix();
		}
		glPushMatrix();
			glLoadIdentity();											// �ޭ��߰Đݒ�
			glViewport(PlRect[PRS].left, ViewSize.cy-PlRect[PRS].bottom-1,
			           PlRect[PRS].Width(), PlRect[PRS].Height());
			glCallList(DL_PARS);										// �����@�ˉe&����ݸޕϊ�
			glPushMatrix();
				glColor3d(rc, gc, bc);									// �\���F�ݒ�
				glCallList(m_NCLIST);									// ���̕`��
			glPopMatrix();
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
		glFinish();
	}
	m_Ncpflg = 0;														// NC�o�^�׸ޏ�����
}

/***********************************/
void CJcad3GlbView::EndNCDisp(void)										//<<< NC�ް��\���I������
/***********************************/
{
	if(m_NCLIST!=0) {													// <<ؽēo�^�ς�>>
		glDeleteLists(m_NCLIST, 1);										// �ި���ڲ�ؽč폜
		m_NCLIST = 0;													// ؽĔԍ��ر
	}
	if(m_Ncdflg==1) {													// <<�ް��\����>>
		glDrawBuffer(GL_BACK);											// �\���ޯ̧�ޯ�
		m_Ncdflg = 0;													// NC�\���׸�OFF
	}
}

/***************************************/
CPoint CJcad3GlbView::GetPlCenter(void)									//<<< ��ʾ����擾
/***************************************/								//    (��ذݍ��W)
{
	CPoint point = PlRect[XY].BottomRight();
	ClientToScreen(&point);
	return point;
}

/***************************************/
void CJcad3GlbView::CheckCtrlKey1(void)									//<<< ���䷰�m�F(�ƭ��N����)
/***************************************/
{
	m_CopyInp = m_CopyBtn;
	m_NumInp = (m_CChar&m_NumKey) ? 1 : m_NumBtn;						// ���l���Ķ�ķ�����
	if((m_CChar&m_NNuKey)&&m_CopyInp==0) {								// ���ʼ��Ķ�ķ�����
		m_CopyInp = (m_BtnEnv&0x4) ? 1 : 2;								// �޲�۸ޕ\���Ȃ�/����
	}
	m_NNuInp = (m_CChar&m_NNuKey) ? 1 : 0;								// ���l���ȊO������
}

/*******************************************/
void CJcad3GlbView::CheckCtrlKey2(UINT flg)								//<<< ���䷰�m�F(����_�I����)
/*******************************************/
{
	m_CtlInp = (flg&m_CtlKey) ? 1 : 0;									// ����_�ǉ�/����������
	m_CurInp = (flg&m_CurKey) ? 1 : 0;									// �ܐ��_/Ӱ�ޕύX������
}

/***************************************/
void CJcad3GlbView::SetCopyBtn(int cop)									//<<< ���ʃ��[�h�ݒ�
/***************************************/
{
	if(cop==1) {
		m_CopyBtn = (m_BtnEnv&0x4) ? ((m_FChar&0x1) ? 2 : 1)			// �޲�۸ޕ\���Ȃ�([F5]:���])
                                   : ((m_FChar&0x1) ? 1 : 2);			// �޲�۸ޕ\������([F5]:���])
	} else {
		m_CopyBtn = 0;
	}
	if(m_SFuncMode<3) {													// <�ڍ�>
		m_pFunction->OnOffButton(8, cop);								// ̧ݸ��ݐݒ�°ٍX�V
	} else if(m_SFuncMode<6) {											// <�Ȉ�>
		m_pFunction2->OnOffButton(8, cop);								// ̧ݸ��ݐݒ�°�2�X�V
	}
}

/**************************************/
void CJcad3GlbView::SetNumBtn(int num)									//<<< ���l���[�h�ݒ�
/**************************************/
{
	m_NumBtn = num;
	if(m_SFuncMode<3) {													// <�ڍ�>
		m_pFunction->OnOffButton(9, num);								// ̧ݸ��ݐݒ�°ٍX�V
	} else if(m_SFuncMode<6) {											// <�Ȉ�>
		m_pFunction2->OnOffButton(9, num);								// ̧ݸ��ݐݒ�°�2�X�V
	}
}

/********************************************/
int CJcad3GlbView::CheckScrChg(CPoint point)							//<<< ��ذݕύX�m�F
/********************************************/
{
	int ret=0;

	if(PlRect[XY].right<=point.x&&point.x<=PlRect[PRS].left) {
		if((PlRect[XY].top<=point.y&&point.y<=PlRect[XY].bottom)||
           (PlRect[YZ].top<=point.y&&point.y<=PlRect[YZ].bottom)) {		// <XY-PRS,YZ-ZX�̋��E>
			ret = 1;
		}
	} else if(PlRect[XY].bottom<=point.y&&point.y<=PlRect[YZ].top) {
		if((PlRect[XY].left<=point.x&&point.x<=PlRect[XY].right)||
           (PlRect[PRS].left<=point.x&&point.x<=PlRect[PRS].right)) {	// <XY-YZ, PRS-ZX�̋��E>
			ret = 2;
		}
	}
	return ret;
}

/*******************************************************/
void CJcad3GlbView::ChangeScreen(int flg, CPoint point)					//<<< ��ذݕύX����
/*******************************************************/
{
	if(flg==1) {														// <�������ړ�>
		ScrDiv1 = (double)point.x/ViewSize.cx;
		ScrDiv1 = (ScrDiv1<0.01) ? 0.0 : ScrDiv1;
		ScrDiv1 = (ScrDiv1>0.99) ? 1.0 : ScrDiv1;
	} else {															// <�c�����ړ�>
		ScrDiv2 = (double)point.y/ViewSize.cy;
		ScrDiv2 = (ScrDiv2<0.01) ? 0.0 : ScrDiv2;
		ScrDiv2 = (ScrDiv2>0.99) ? 1.0 : ScrDiv2;
	}
	InitScreen(ViewSize.cx, ViewSize.cy);								// ��ذݏ�������
	Display(0);															// �ĕ`��
}
