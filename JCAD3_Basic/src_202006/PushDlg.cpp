/////////////////////////////////////////////////////////////////////////////
// �v�b�V���{�^���_�C�A���O�N���X��`
// PushDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#define   TIMERID    1

/////////////////////////////////////////////////////////////////////////////
// CScrollDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CScrollDlg, CDialog)									// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/*************************************************************************/
CScrollDlg::CScrollDlg(CWnd* pParent) : CDialog(CScrollDlg::IDD, pParent)
/*************************************************************************/
{
	m_pParent = pParent;
	Axis      = 0;														// ���ޕϐ��̏�����
	Dir       = 0;
	m_nID     = 0;
}

/***************************************************/
void CScrollDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
}

/***********************************/
BOOL CScrollDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ү���މ�������
/***********************************/
{
	CDialog::OnInitDialog();
	VERIFY(LeftXY.AutoLoad(IDC_LEFTXY, this));							// �t���ޯ�ϯ�߂����ѕt����
	VERIFY(RightXY.AutoLoad(IDC_RIGHTXY, this));
	VERIFY(UpXY.AutoLoad(IDC_UPXY, this));
	VERIFY(DownXY.AutoLoad(IDC_DOWNXY, this));
	VERIFY(LeftYZ.AutoLoad(IDC_LEFTYZ, this));
	VERIFY(RightYZ.AutoLoad(IDC_RIGHTYZ, this));
	VERIFY(UpYZ.AutoLoad(IDC_UPYZ, this));
	VERIFY(DownYZ.AutoLoad(IDC_DOWNYZ, this));
	VERIFY(LeftZX.AutoLoad(IDC_LEFTZX, this));
	VERIFY(RightZX.AutoLoad(IDC_RIGHTZX, this));
	VERIFY(UpZX.AutoLoad(IDC_UPZX, this));
	VERIFY(DownZX.AutoLoad(IDC_DOWNZX, this));
	CenterWindow(m_pParent);											// �޲�۸ނ�e����޳�̒��S�ɕ\��
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*******************************/
void CScrollDlg::OnCancel(void)											//<<< WM_CANCEL ү���މ�������
/*******************************/
{
	View->HideScrollTool();
	ShowWindow(SW_HIDE);
}

/***************************************/
LRESULT CScrollDlg::OnNcHitTest(CPoint)									//<<< WM_NCHITTEST ү���މ�������
/***************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/***************************************/
void CScrollDlg::OnTimer(UINT_PTR nIDTimer)									//<<< WM_TIMER ү���މ�������
/***************************************/
{
	KillTimer(TIMERID);													// ��ϰ����
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// �����݉�����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ϳ����ޯ�ϯ�ߖt��
				View->Scroll(Axis, Dir);								// ��ʽ�۰�
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/*****************************************************************/
BOOL CScrollDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)		//<<< WM_SETCURSOR ү���މ�������
/*****************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖtID�擾
			if(id>=IDC_UPXY&&id<=IDC_UPZX) {							// ϳ����ޯ�ϯ�ߖt��
				m_nID = id;
				id -= IDC_UPXY;
				Dir = (id % 2) ? 1 : -1;								// �����̕������Z�o
				Axis = id / 4;											// X,Y,Z���̓�,���ꂩ���Z�o
				View->Scroll(Axis, Dir);								// ��ʽ�۰�
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/********************************/
void CScrollDlg::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "ScrLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "ScrTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*****************************/
BOOL CScrollDlg::Create(void)											//<<< �޲�۸ސ���
/*****************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "ScrLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "ScrTop", -1);
	if(left>0) {
		width = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		height = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		wp.showCmd = SW_HIDE;
		wp.rcNormalPosition.left = left;
		wp.rcNormalPosition.top = top;
		wp.rcNormalPosition.right = left + width;
		wp.rcNormalPosition.bottom = top + height;
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CEyeDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CEyeDlg, CDialog)										// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/****************************************************************/
CEyeDlg::CEyeDlg(CWnd* pParent) : CDialog(CEyeDlg::IDD, pParent)
/****************************************************************/
{
	m_pParent = pParent;
	Theta     = 0.0;													// ���ޕϐ��̏�����
	Dir       = 0;
	m_nID     = 0;
}

/************************************************/
void CEyeDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/************************************************/
{
	CDialog::DoDataExchange(pDX);
}

/********************************/
BOOL CEyeDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ү���މ�������
/********************************/
{
	CDialog::OnInitDialog();
	VERIFY(Left.AutoLoad(IDC_LEFT, this));								// �t���ޯ�ϯ�߂����ѕt����
	VERIFY(Right.AutoLoad(IDC_RIGHT, this));
	VERIFY(Down.AutoLoad(IDC_DOWN, this));
	VERIFY(Up.AutoLoad(IDC_UP, this));
	CenterWindow(m_pParent);											// �޲�۸ނ�e����޳�̒��S�ɕ\��
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/****************************/
void CEyeDlg::OnCancel(void)											//<<< WM_CANCEL ү���މ�������
/****************************/
{
	View->HideEyeChange();
	ShowWindow(SW_HIDE);
}

/************************************/
LRESULT CEyeDlg::OnNcHitTest(CPoint)									//<<< WM_NCHITTEST ү���މ�������
/************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/************************************/
void CEyeDlg::OnTimer(UINT_PTR nIDTimer)									//<<< WM_TIMER ү���މ�������
/************************************/
{
	KillTimer(TIMERID);													// ��ϰ����
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// �����݉�����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ϳ����ޯ�ϯ�ߖt��
				View->EyeChange(Dir, Theta);							// ���_�ύX
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/**************************************************************/
BOOL CEyeDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)			//<<< WM_SETCURSOR ү���މ�������
/**************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖtID�擾
			if(id>=IDC_LEFT&&id<=IDC_UP) {								// ϳ����ޯ�ϯ�ߖt��
				m_nID = id;
				Dir = (id - IDC_LEFT) / 2;								// �������Z�o
				Theta = ((id-IDC_LEFT)%2?1:-1)*10.0*PI/180.0;			// �p�x���Z�o
				View->EyeChange(Dir, Theta);							// ���_�ύX
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/*****************************/
void CEyeDlg::OnDestroy(void)											//<<< WM_DESTROY ү���މ�������
/*****************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "EyeLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "EyeTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**************************/
BOOL CEyeDlg::Create(void)												//<<< �޲�۸ސ���
/**************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "EyeLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "EyeTop", -1);
	if(left>0) {
		width = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		height = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		wp.showCmd = SW_HIDE;
		wp.rcNormalPosition.left = left;
		wp.rcNormalPosition.top = top;
		wp.rcNormalPosition.right = left + width;
		wp.rcNormalPosition.bottom = top + height;
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg1 �_�C�A���O

BEGIN_MESSAGE_MAP(CSetBaseDlg1, CDialog)								// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO9, OnBaseButton)
	ON_COMMAND_EX_RANGE(IDC_BUTTON1, IDC_BUTTON3, OnDirButton)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg1::CSetBaseDlg1(CWnd* pParent) : CDialog(CSetBaseDlg1::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint = 0;
	m_Direction = 0;
}

/*****************************************************/
void CSetBaseDlg1::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
	DDX_Control(pDX, IDC_RADIO4, m_Base4);
	DDX_Control(pDX, IDC_RADIO5, m_Base5);
	DDX_Control(pDX, IDC_RADIO6, m_Base6);
	DDX_Control(pDX, IDC_RADIO7, m_Base7);
	DDX_Control(pDX, IDC_RADIO8, m_Base8);
	DDX_Control(pDX, IDC_RADIO9, m_Base9);
	DDX_Control(pDX, IDC_BUTTON1, m_DirSN);
	DDX_Control(pDX, IDC_BUTTON2, m_DirEWSN);
	DDX_Control(pDX, IDC_BUTTON3, m_DirEW);
}

/*************************************/
BOOL CSetBaseDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/*************************************/
{
	CDialog::OnInitDialog();
	m_DirSN.LoadBitmap("SN");
	m_DirEWSN.LoadBitmap("EWSN");
	m_DirEW.LoadBitmap("EW");
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	m_Base4.LoadBitmap();
	m_Base5.LoadBitmap();
	m_Base6.LoadBitmap();
	m_Base7.LoadBitmap();
	m_Base8.LoadBitmap();
	m_Base9.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint))->SetCheckBtn(1);
	((CCheckButton*)GetDlgItem(IDC_BUTTON1+m_Direction))->SetCheckBtn(1);
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*****************************************/
BOOL CSetBaseDlg1::OnBaseButton(UINT nID)								//<<< ��_�t����
/*****************************************/
{
	m_BasePoint = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO9; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/****************************************/
BOOL CSetBaseDlg1::OnDirButton(UINT nID)								//<<< �����t����
/****************************************/
{
	m_Direction = nID-IDC_BUTTON1;
	for(UINT id=IDC_BUTTON1; id<=IDC_BUTTON3; id++) {
		((CCheckButton*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/*******************************************/
void CSetBaseDlg1::SetValue(int p1, int p2)								//<<< ���Ұ��ݒ�
/*******************************************/
{
	m_BasePoint = p1;
	m_Direction = p2;
}

/*********************************************/
void CSetBaseDlg1::GetValue(int* p1, int* p2)							//<<< ���Ұ��擾
/*********************************************/
{
	*p1 = m_BasePoint;
	*p2 = m_Direction;
}

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg2 �_�C�A���O

BEGIN_MESSAGE_MAP(CSetBaseDlg2, CDialog)								// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO9, OnBaseButton)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg2::CSetBaseDlg2(CWnd* pParent) : CDialog(CSetBaseDlg2::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint = 0;
}

/*****************************************************/
void CSetBaseDlg2::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
	DDX_Control(pDX, IDC_RADIO4, m_Base4);
	DDX_Control(pDX, IDC_RADIO5, m_Base5);
	DDX_Control(pDX, IDC_RADIO6, m_Base6);
	DDX_Control(pDX, IDC_RADIO7, m_Base7);
	DDX_Control(pDX, IDC_RADIO8, m_Base8);
	DDX_Control(pDX, IDC_RADIO9, m_Base9);
}

/*************************************/
BOOL CSetBaseDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/*************************************/
{
	CDialog::OnInitDialog();
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	m_Base4.LoadBitmap();
	m_Base5.LoadBitmap();
	m_Base6.LoadBitmap();
	m_Base7.LoadBitmap();
	m_Base8.LoadBitmap();
	m_Base9.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint))->SetCheckBtn(1);
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*****************************************/
BOOL CSetBaseDlg2::OnBaseButton(UINT nID)								//<<< ��_�t����
/*****************************************/
{
	m_BasePoint = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO9; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/***********************************/
void CSetBaseDlg2::SetValue(int p1)										//<<< ���Ұ��ݒ�
/***********************************/
{
	m_BasePoint = p1;
}

/************************************/
void CSetBaseDlg2::GetValue(int* p1)									//<<< ���Ұ��擾
/************************************/
{
	*p1 = m_BasePoint;
}

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg3 �_�C�A���O

BEGIN_MESSAGE_MAP(CSetBaseDlg3, CDialog)								// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO4, OnBaseButton1)
	ON_COMMAND_EX_RANGE(IDC_RADIO5, IDC_RADIO7, OnBaseButton2)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg3::CSetBaseDlg3(CWnd* pParent) : CDialog(CSetBaseDlg3::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint1 = 0;
	m_BasePoint2 = 0;
	m_Size       = 0;
}

/*****************************************************/
void CSetBaseDlg3::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
	DDX_Control(pDX, IDC_RADIO4, m_Base4);
	DDX_Control(pDX, IDC_RADIO5, m_Base5);
	DDX_Control(pDX, IDC_RADIO6, m_Base6);
	DDX_Control(pDX, IDC_RADIO7, m_Base7);
	DDX_Radio(pDX, IDC_RADIO8, m_Size);
}

/*************************************/
BOOL CSetBaseDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/*************************************/
{
	CDialog::OnInitDialog();
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	m_Base4.LoadBitmap();
	m_Base5.LoadBitmap();
	m_Base6.LoadBitmap();
	m_Base7.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint1))->SetCheckBtn(1);
	((CCheckBox*)GetDlgItem(IDC_RADIO5+m_BasePoint2))->SetCheckBtn(1);
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************************/
BOOL CSetBaseDlg3::OnBaseButton1(UINT nID)								//<<< ��_�t1����
/******************************************/
{
	m_BasePoint1 = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO4; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/******************************************/
BOOL CSetBaseDlg3::OnBaseButton2(UINT nID)								//<<< ��_�t2����
/******************************************/
{
	m_BasePoint2 = nID-IDC_RADIO5;
	for(UINT id=IDC_RADIO5; id<=IDC_RADIO7; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/***************************************************/
void CSetBaseDlg3::SetValue(int p1, int p2, int p3)						//<<< ���Ұ��ݒ�
/***************************************************/
{
	m_BasePoint1 = p1;
	m_BasePoint2 = p2;
	m_Size       = p3;
}

/******************************************************/
void CSetBaseDlg3::GetValue(int* p1, int* p2, int* p3)					//<<< ���Ұ��擾
/******************************************************/
{
	*p1 = m_BasePoint1;
	*p2 = m_BasePoint2;
	*p3 = m_Size;
}

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg4 �_�C�A���O

BEGIN_MESSAGE_MAP(CSetBaseDlg4, CDialog)								// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO3, OnBaseButton)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg4::CSetBaseDlg4(CWnd* pParent) : CDialog(CSetBaseDlg4::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint = 0;
}

/*****************************************************/
void CSetBaseDlg4::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
}

/*************************************/
BOOL CSetBaseDlg4::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/*************************************/
{
	CDialog::OnInitDialog();
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint))->SetCheckBtn(1);
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*****************************************/
BOOL CSetBaseDlg4::OnBaseButton(UINT nID)								//<<< ��_�t����
/*****************************************/
{
	m_BasePoint = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO3; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/***********************************/
void CSetBaseDlg4::SetValue(int p1)										//<<< ���Ұ��ݒ�
/***********************************/
{
	m_BasePoint = p1;
}

/************************************/
void CSetBaseDlg4::GetValue(int* p1)									//<<< ���Ұ��擾
/************************************/
{
	*p1 = m_BasePoint;
}
