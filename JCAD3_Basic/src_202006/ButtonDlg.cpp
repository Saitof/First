/////////////////////////////////////////////////////////////////////////////
// ���W�I�{�^���_�C�A���O�N���X��`
// BottunDlg.cpp
//---------------------------------------------------------------------------
// LastEdit: 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "ButtonDlg.h"

///////////////////////////////////////////////////////////////////////////////
// CDispmodeDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CDispmodeDlg, CDialog)								// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO6, OnCheckBtn)
END_MESSAGE_MAP()

/*******************************************************************************/
CDispmodeDlg::CDispmodeDlg(CWnd* pParent) : CDialog(CDispmodeDlg::IDD, pParent)
/*******************************************************************************/
{
	m_DspMode = 0;
}

/*****************************************************/
void CDispmodeDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_DspMode);
}

/***************************************/
BOOL CDispmodeDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/***************************************/
{
	m_DspMode = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*************************************/
void CDispmodeDlg::SetValue(int prm1)									//<<< ���Ұ��ݒ�
/*************************************/
{
	m_DspMode = prm1;
}

/**************************************/
void CDispmodeDlg::GetValue(int* prm1)									//<<< ���Ұ��擾
/**************************************/
{
	*prm1 = m_DspMode;
}

///////////////////////////////////////////////////////////////////////////////
// CScreenDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO5, OnCheckBtn)
END_MESSAGE_MAP()

/*************************************************************************/
CScreenDlg::CScreenDlg(CWnd* pParent) : CDialog(CScreenDlg::IDD, pParent)
/*************************************************************************/
{
	m_ScreenNo = 0;
}

/***************************************************/
void CScreenDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_ScreenNo);
}

/*************************************/
BOOL CScreenDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/*************************************/
{
	m_ScreenNo = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************/
void CScreenDlg::GetValue(int* prm1)									//<<< ���Ұ��擾
/************************************/
{
	*prm1 = m_ScreenNo;
}

///////////////////////////////////////////////////////////////////////////////
// CEyesetDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CEyesetDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO6, OnCheckBtn)
END_MESSAGE_MAP()

/*************************************************************************/
CEyesetDlg::CEyesetDlg(CWnd* pParent) : CDialog(CEyesetDlg::IDD, pParent)
/*************************************************************************/
{
	m_Eyeset = 0;
}

/***************************************************/
void CEyesetDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Eyeset);
}

/*************************************/
BOOL CEyesetDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/*************************************/
{
	m_Eyeset = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************/
void CEyesetDlg::GetValue(int* prm1)									//<<< ���Ұ��擾
/************************************/
{
	*prm1 = m_Eyeset;
}

///////////////////////////////////////////////////////////////////////////////
// CBasesetDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CBasesetDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************/
CBasesetDlg::CBasesetDlg(CWnd* pParent) : CDialog(CBasesetDlg::IDD, pParent)
/****************************************************************************/
{
	m_Baseset = 0;
}

/****************************************************/
void CBasesetDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Baseset);
}

/**************************************/
BOOL CBasesetDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/**************************************/
{
	m_Baseset = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*************************************/
void CBasesetDlg::GetValue(int* prm1)									//<<< ���Ұ��擾
/*************************************/
{
	*prm1 = m_Baseset;
}

///////////////////////////////////////////////////////////////////////////////
// CGlbDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CGlbDlg, CDialog)										// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO5, OnCheckBtn)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
END_MESSAGE_MAP()

/**************************************************************************/
CGlbDlg::CGlbDlg(int kind, CWnd* pParent) : CDialog(CGlbDlg::IDD, pParent)
/**************************************************************************/
{
	m_Kind = kind;
	m_GlbNo = 0;
	m_ChangeSize = 0;
	m_RingSize = 12.0;
}

/************************************************/
void CGlbDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_GlbNo);
	DDX_Check(pDX, IDC_CHECK1, m_ChangeSize);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Text(pDX, IDC_EDIT1, m_RingSize);
	DDV_MinMaxDouble(pDX, m_RingSize, 0.1, 1000.0);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
}

/********************************/
BOOL CGlbDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ү���މ�������
/********************************/										//    �޲�۸ނ̏�����
{
	CString str;

	CDialog::OnInitDialog();
	if(!m_Kind) {														// <�����O�����w��>
		str.LoadString(IDS_STR26);										// ���ږ��擾(�ݸލ���)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR28);										// �P�ʎ擾(��)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// �P�ʕ\��
	} else {															// <�����O���a�w��>
		str.LoadString(IDS_STR27);										// ���ږ��擾(�ݸޒ��a)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR5);										// �P�ʎ擾(mm)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// �P�ʕ\��
	}
	PermissionCheck();													// ���͋�������
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
BOOL CGlbDlg::OnCheckBtn(UINT nID)										//<<< ���ݓ��͊m�F����
/**********************************/
{
	m_GlbNo = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/***************************/
void CGlbDlg::OnCheck(void)												//<<< ��������
/***************************/
{
	m_ChangeSize = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;	// �t��ON/OFF��Ԃ��擾
	PermissionCheck();													// ���͋�������
}

/***********************************/
void CGlbDlg::PermissionCheck(void)										//<<< ���͋�������
/***********************************/
{
	m_Static1.EnableWindow(m_ChangeSize);
	m_Edit1.EnableWindow(m_ChangeSize);
	m_Static2.EnableWindow(m_ChangeSize);
}

/*************************************************/
void CGlbDlg::SetValue(int p1, int p2, double p3)						//<<< ���Ұ��ݒ�
/*************************************************/
{
	m_GlbNo = p1;
	m_ChangeSize = p2;
	m_RingSize = p3;
}

/****************************************************/
void CGlbDlg::GetValue(int* p1, int* p2, double* p3)					//<<< ���Ұ��擾
/****************************************************/
{
	*p1 = m_GlbNo;
	*p2 = m_ChangeSize;
	*p3 = m_RingSize;
}

///////////////////////////////////////////////////////////////////////////////
// CDispTmpDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CDispTmpDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO9, OnCheckBtn)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
END_MESSAGE_MAP()

/**************************************************************************************/
CDispTmpDlg::CDispTmpDlg(int kind, CWnd* pParent) : CDialog(CDispTmpDlg::IDD, pParent)
/**************************************************************************************/
{
	m_Kind = kind;
	m_DspTmp = 0;
	m_ChangeSize = 0;
	m_RingSize = 12.0;
}

/****************************************************/
void CDispTmpDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_DspTmp);
	DDX_Check(pDX, IDC_CHECK1, m_ChangeSize);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Text(pDX, IDC_EDIT1, m_RingSize);
	DDV_MinMaxDouble(pDX, m_RingSize, 0.1, 1000.0);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
}

/************************************/
BOOL CDispTmpDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/									//    �޲�۸ނ̏�����
{
	CString str;

	CDialog::OnInitDialog();
	if(!m_Kind) {														// <�����O�����w��>
		str.LoadString(IDS_STR26);										// ���ږ��擾(�ݸލ���)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR28);										// �P�ʎ擾(��)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// �P�ʕ\��
	} else {															// <�����O���a�w��>
		str.LoadString(IDS_STR27);										// ���ږ��擾(�ݸޒ��a)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR5);										// �P�ʎ擾(mm)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// �P�ʕ\��
	}
	PermissionCheck();													// ���͋�������
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**************************************/
BOOL CDispTmpDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/**************************************/
{
	m_DspTmp = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*******************************/
void CDispTmpDlg::OnCheck(void)											//<<< ��������
/*******************************/
{
	m_ChangeSize = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;	// �t��ON/OFF��Ԃ��擾
	PermissionCheck();													// ���͋�������
}

/***************************************/
void CDispTmpDlg::PermissionCheck(void)									//<<< ���͋�������
/***************************************/
{
	m_Static1.EnableWindow(m_ChangeSize);
	m_Edit1.EnableWindow(m_ChangeSize);
	m_Static2.EnableWindow(m_ChangeSize);
}

/*****************************************************/
void CDispTmpDlg::SetValue(int p1, int p2, double p3)					//<<< ���Ұ��ݒ�
/*****************************************************/
{
	m_DspTmp = p1;
	m_ChangeSize = p2;
	m_RingSize = p3;
}

/********************************************************/
void CDispTmpDlg::GetValue(int* p1, int* p2, double* p3)				//<<< ���Ұ��擾
/********************************************************/
{
	*p1 = m_DspTmp;
	*p2 = m_ChangeSize;
	*p3 = m_RingSize;
}

///////////////////////////////////////////////////////////////////////////////
// CColorDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CColorDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO18, OnCheckBtn)
END_MESSAGE_MAP()

/**********************************************************************/
CColorDlg::CColorDlg(CWnd* pParent) : CDialog(CColorDlg::IDD, pParent)
/**********************************************************************/
{
	m_Color = 0;
}

/**************************************************/
void CColorDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Color);
}

/************************************/
BOOL CColorDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/************************************/
{
	m_Color = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/**********************************/
void CColorDlg::SetValue(int prm1)										//<<< ���Ұ��ݒ�
/**********************************/
{
	m_Color = prm1;
}

/***********************************/
void CColorDlg::GetValue(int* prm1)										//<<< ���Ұ��擾
/***********************************/
{
	*prm1 = m_Color;
}

///////////////////////////////////////////////////////////////////////////////
// CCurveDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CCurveDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnCheckBtn)
END_MESSAGE_MAP()

/**********************************************************************/
CCurveDlg::CCurveDlg(CWnd* pParent) : CDialog(CCurveDlg::IDD, pParent)
/**********************************************************************/
{
	m_Curve = 0;
}

/**************************************************/
void CCurveDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Curve);
}

/************************************/
BOOL CCurveDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/************************************/
{
	m_Curve = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/***********************************/
void CCurveDlg::GetValue(int* prm1)										//<<< ���Ұ��擾
/***********************************/
{
	*prm1 = m_Curve;
}

///////////////////////////////////////////////////////////////////////////////
// CPrimitiveDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CPrimitiveDlg, CDialog)								// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO6, OnCheckBtn)
END_MESSAGE_MAP()

/**********************************************************************************/
CPrimitiveDlg::CPrimitiveDlg(CWnd* pParent) : CDialog(CPrimitiveDlg::IDD, pParent)
/**********************************************************************************/
{
	m_Primitive = 0;
}

/******************************************************/
void CPrimitiveDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Primitive);
}

/****************************************/
BOOL CPrimitiveDlg::OnCheckBtn(UINT nID)								//<<< ���ݓ��͊m�F����
/****************************************/
{
	m_Primitive = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/***************************************/
void CPrimitiveDlg::GetValue(int* prm1)									//<<< ���Ұ��擾
/***************************************/
{
	*prm1 = m_Primitive;
}

///////////////////////////////////////////////////////////////////////////////
// CBooleanDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CBooleanDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO3, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************/
CBooleanDlg::CBooleanDlg(CWnd* pParent) : CDialog(CBooleanDlg::IDD, pParent)
/****************************************************************************/
{
	m_Boolean = 0;
	m_DeleteB = 0;
}

/****************************************************/
void CBooleanDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Boolean);
	DDX_Check(pDX, IDC_CHECK1, m_DeleteB);
}

/**************************************/
BOOL CBooleanDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/**************************************/
{
	m_Boolean = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************************/
void CBooleanDlg::GetValue(int* prm1, int* prm2)						//<<< ���Ұ��擾
/************************************************/
{
	*prm1 = m_Boolean;
	*prm2 = (!m_Boolean) ? m_DeleteB : 0;
}

///////////////////////////////////////////////////////////////////////////////
// CGemsetDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CGemsetDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO3, OnCheckBtn)
END_MESSAGE_MAP()

/*************************************************************************/
CGemsetDlg::CGemsetDlg(CWnd* pParent) : CDialog(CGemsetDlg::IDD, pParent)
/*************************************************************************/
{
	m_GemSet = 0;
}

/***************************************************/
void CGemsetDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_GemSet);
}

/*************************************/
BOOL CGemsetDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/*************************************/
{
	m_GemSet = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************/
void CGemsetDlg::GetValue(int* prm1)									//<<< ���Ұ��擾
/************************************/
{
	*prm1 = m_GemSet;
}

///////////////////////////////////////////////////////////////////////////////
// CGemDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CGemDlg, CDialog)										// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO11, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************/
CGemDlg::CGemDlg(CWnd* pParent) : CDialog(CGemDlg::IDD, pParent)
/****************************************************************/
{
	m_Gem = 0;
}

/************************************************/
void CGemDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Gem);
}

/**********************************/
BOOL CGemDlg::OnCheckBtn(UINT nID)										//<<< ���ݓ��͊m�F����
/**********************************/
{
	m_Gem = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*********************************/
void CGemDlg::GetValue(int* prm1)										//<<< ���Ұ��擾
/*********************************/
{
	*prm1 = m_Gem;
}

///////////////////////////////////////////////////////////////////////////////
// CRevolveDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CRevolveDlg, CDialog)									// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************/
CRevolveDlg::CRevolveDlg(CWnd* pParent) : CDialog(CRevolveDlg::IDD, pParent)
/****************************************************************************/
{
	m_Revolve = 0;
}

/****************************************************/
void CRevolveDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Revolve);
}

/**************************************/
BOOL CRevolveDlg::OnCheckBtn(UINT nID)									//<<< ���ݓ��͊m�F����
/**************************************/
{
	m_Revolve = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*************************************/
void CRevolveDlg::GetValue(int* prm1)									//<<< ���Ұ��擾
/*************************************/
{
	*prm1 = m_Revolve;
}

///////////////////////////////////////////////////////////////////////////////
// CSetFuncModeDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CSetFuncModeDlg, CDialog)								// ү���ޥϯ��
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO7, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************************/
CSetFuncModeDlg::CSetFuncModeDlg(CWnd* pParent) : CDialog(CSetFuncModeDlg::IDD, pParent)
/****************************************************************************************/
{
	m_FuncMode = 0;
}

/********************************************************/
void CSetFuncModeDlg::DoDataExchange(CDataExchange* pDX)				//<<< �޲�۸ޥ�ް��̌����ƗL��������
/********************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_FuncMode);
}

/******************************************/
BOOL CSetFuncModeDlg::OnCheckBtn(UINT nID)								//<<< ���ݓ��͊m�F����
/******************************************/
{
	m_FuncMode = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/****************************************/
void CSetFuncModeDlg::SetValue(int prm1)								//<<< ���Ұ��ݒ�
/****************************************/
{
	m_FuncMode = prm1;
}

/*****************************************/
void CSetFuncModeDlg::GetValue(int* prm1)								//<<< ���Ұ��擾
/*****************************************/
{
	*prm1 = m_FuncMode;
}
